/*
 * Services Mono needs on xsdk that the console's libraries don't give (mono-xsdk.c).
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#ifndef __MONO_UTILS_XSDK_H__
#define __MONO_UTILS_XSDK_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void *mono_xsdk_file_map (size_t length, int flags, int fd, uint64_t offset, void **ret_handle);
int mono_xsdk_file_unmap (void *addr, void *handle);

int mono_xsdk_pinvoke_register (const char *module, const char *name, void *address);
int mono_xsdk_pinvoke_has_module (const char *module);
void *mono_xsdk_pinvoke_lookup (const char *module, const char *name);
/* System.Native's function NAME (mono/native/pal-xsdk-table.c), or NULL. */
void *mono_xsdk_native_lookup (const char *name);

/* FATX's longest file name: 44 characters failed with STATUS_OBJECT_NAME_INVALID (xsdk:docs/HARDWARE.md,
 * "Shaders compiled as the game asks"). */
#define MONO_XSDK_FILE_NAME_MAX 42
/*
 * NAME, longer than FATX allows, as the file it's kept in: its start, '~' and 8 hex digits of a hash of NAME in lower
 * case, then its extension, 42 characters in all. g_free the result. xsdk/short_names.py names files the same way.
 */
char *mono_xsdk_short_file_name (const char *name);

void *mono_xsdk_jit_alloc (size_t size);
/* Bytes of the JIT's pool handed out so far. */
size_t mono_xsdk_jit_used (void);
void mono_xsdk_instruction_sync (void);

#ifdef __cplusplus
}
#endif

#endif
