/**
 * \file
 * Services Mono needs on xsdk that the console's libraries don't give: P/Invoke into the title's own functions, as
 * there's no dlopen; file "maps" read into memory, as there's no mmap; and executable memory for the JIT from the
 * title's .jit pool (xsdk:docs/GUIDE.md), Mono's 4 MB and whatever the title adds for the code it runs.
 *
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <config.h>

#ifdef HOST_XSDK

#include <errno.h>
#include <ifaddrs.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <glib.h>
#include <xsdk/jit.h>

#include "mono-xsdk.h"

enum {
	/* mono-mmap.h's MONO_MMAP_WRITE, _EXEC, _SHARED and _FIXED, which a copy into memory can't honour. */
	UNMAPPABLE_FLAGS = (1 << 1) | (1 << 2) | (1 << 5) | (1 << 7),
	READ_CHUNK_BYTES = 1024 * 1024,
	JIT_ALIGNMENT = 16,
};

static pthread_mutex_t registry_lock = PTHREAD_MUTEX_INITIALIZER;
/* "module!name" to the function's address, and each module registered. */
static GHashTable *functions;
static GHashTable *modules;

/*
 * Mono's own 4 MB of the title's .jit pool, which comes with the JIT as this file does: the linker lays every .jit
 * section side by side as the one pool (xsdk:src/xsdk.ld), so a title that runs more code adds a section of its own.
 */
__asm__ (".section .jit.mono,\"awx\",@nobits\n"
	"\t.balign 0x10000\n"
	"\t.skip 0x400000\n"
	"\t.previous\n");

static pthread_mutex_t jit_lock = PTHREAD_MUTEX_INITIALIZER;
static unsigned char *jit_pool;
static size_t jit_pool_bytes;
static size_t jit_used;

/* What Mono's class libraries P/Invoke in "libc" that xsdk has: System's Platform.CheckOS, and its network interfaces,
 * which a socket asks for. */
static const struct {
	const char *name;
	void *address;
} libc_functions [] = {
	{ "freeifaddrs", (void *)freeifaddrs },
	{ "getifaddrs", (void *)getifaddrs },
	{ "uname", (void *)uname },
};

static const char *
module_base (const char *module)
{
	const char *base = module;

	for (const char *c = module; *c; c++) {
		if (*c == '/' || *c == '\\')
			base = c + 1;
	}
	return base;
}

/* System.Native, or the library Mono's dllmap names for it (data/config.in), on any path: built into this library. */
static gboolean
is_system_native (const char *module)
{
	const char *base = module_base (module);

	return g_str_has_prefix (base, "System.Native") || g_str_has_prefix (base, "libSystem.Native") ||
		g_str_has_prefix (base, "libmono-native");
}

/* The C library, by its DllImport name or the one Mono's dllmap gives it (data/config.in): the title's own. */
static gboolean
is_libc (const char *module)
{
	const char *base = module_base (module);

	return strcmp (base, "libc") == 0 || g_str_has_prefix (base, "libc.so");
}

static void *
libc_lookup (const char *name)
{
	for (size_t i = 0; i < G_N_ELEMENTS (libc_functions); i++) {
		if (strcmp (libc_functions [i].name, name) == 0)
			return libc_functions [i].address;
	}
	return NULL;
}

static char *
function_key (const char *module, const char *name)
{
	return g_strconcat (module, "!", name, (const char *)NULL);
}

int
mono_xsdk_pinvoke_register (const char *module, const char *name, void *address)
{
	if (!module || !name || !address) {
		errno = EINVAL;
		return -1;
	}
	pthread_mutex_lock (&registry_lock);
	if (!functions) {
		functions = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
		modules = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);
	}
	g_hash_table_insert (functions, function_key (module, name), address);
	g_hash_table_insert (modules, g_strdup (module), GINT_TO_POINTER (1));
	pthread_mutex_unlock (&registry_lock);
	return 0;
}

int
mono_xsdk_pinvoke_has_module (const char *module)
{
	int found;

	if (!module)
		return 0;
	if (strcmp (module, "__Internal") == 0 || is_system_native (module) || is_libc (module))
		return 1;
	pthread_mutex_lock (&registry_lock);
	found = modules && g_hash_table_lookup (modules, module) != NULL;
	pthread_mutex_unlock (&registry_lock);
	return found;
}

void *
mono_xsdk_pinvoke_lookup (const char *module, const char *name)
{
	char *key;
	void *address = NULL;

	if (!module || !name)
		return NULL;
	if (is_system_native (module))
		return mono_xsdk_native_lookup (name);
	if (is_libc (module))
		return libc_lookup (name);
	key = function_key (module, name);
	pthread_mutex_lock (&registry_lock);
	if (functions)
		address = g_hash_table_lookup (functions, key);
	pthread_mutex_unlock (&registry_lock);
	g_free (key);
	return address;
}

void *
mono_xsdk_file_map (size_t length, int flags, int fd, uint64_t offset, void **ret_handle)
{
	uint8_t *buffer;
	size_t done = 0;

	if (!ret_handle) {
		errno = EINVAL;
		return NULL;
	}
	if ((flags & UNMAPPABLE_FLAGS) || offset > INT32_MAX) {
		errno = ENOTSUP;
		return NULL;
	}
	buffer = (uint8_t *)malloc (length ? length : 1);
	if (!buffer)
		return NULL;
	while (done < length) {
		size_t chunk = MIN (length - done, READ_CHUNK_BYTES);
		ssize_t bytes = pread (fd, buffer + done, chunk, (off_t)(offset + done));

		if (bytes < 0 && errno == EINTR)
			continue;
		if (bytes <= 0) {
			if (bytes == 0)
				errno = EIO;
			free (buffer);
			return NULL;
		}
		done += (size_t)bytes;
	}
	*ret_handle = NULL;
	return buffer;
}

int
mono_xsdk_file_unmap (void *addr, void *handle)
{
	free (addr);
	return 0;
}

char *
mono_xsdk_short_file_name (const char *name)
{
	const char *extension = strrchr (name, '.');
	size_t extension_length = extension ? strlen (extension) : 0;
	size_t kept;
	/* FNV-1a, 32 bits, over the name in lower case, as FATX matches names without case. */
	guint32 hash = 2166136261u;

	for (const char *c = name; *c; c++)
		hash = (hash ^ (guint8)g_ascii_tolower (*c)) * 16777619u;
	/* An extension too long to keep is part of the name. */
	if (extension_length > MONO_XSDK_FILE_NAME_MAX - 10) {
		extension = NULL;
		extension_length = 0;
	}
	kept = MONO_XSDK_FILE_NAME_MAX - extension_length - 9;
	return g_strdup_printf ("%.*s~%08x%s", (int)kept, name, hash, extension ? extension : "");
}

void *
mono_xsdk_jit_alloc (size_t size)
{
	unsigned char *result = NULL;
	size_t aligned;

	if (size > SIZE_MAX - (JIT_ALIGNMENT - 1))
		return NULL;
	aligned = MAX ((size + JIT_ALIGNMENT - 1) & ~(size_t)(JIT_ALIGNMENT - 1), JIT_ALIGNMENT);
	pthread_mutex_lock (&jit_lock);
	if (!jit_pool) {
		void *base;

		xsdk_jit_pool (&base, &jit_pool_bytes);
		jit_pool = (unsigned char *)base;
	}
	if (aligned <= jit_pool_bytes - jit_used) {
		result = jit_pool + jit_used;
		jit_used += aligned;
	}
	pthread_mutex_unlock (&jit_lock);
	if (!result)
		g_warning ("xsdk: the title's .jit pool of %u bytes has no room for %u more; add a .jit section to it (docs/README-xsdk.md)",
			(unsigned)jit_pool_bytes, (unsigned)size);
	return result;
}

size_t
mono_xsdk_jit_used (void)
{
	size_t used;

	pthread_mutex_lock (&jit_lock);
	used = jit_used;
	pthread_mutex_unlock (&jit_lock);
	return used;
}

void
mono_xsdk_instruction_sync (void)
{
	__asm__ volatile ("isync" : : : "memory");
}

#endif
