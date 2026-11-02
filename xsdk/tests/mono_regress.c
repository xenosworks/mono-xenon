#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mono/metadata/assembly.h>
#include <mono/utils/mono-xsdk.h>
#include <xsdk/log.h>
#include <xsdk/splash.h>

// MONO_API, but declared in mini-runtime.h, which only the runtime itself can include.
int mono_main(int argc, char *argv[]);

enum { SPLASH_MS = 2000, ARGUMENTS_MAX = 32, ARGUMENT_BYTES = 128 };

// mono/mini/Makefile.am's regtests, run as its rcheck target runs them: every test under every optimisation set.
static char *arguments[] = {
	"mono",
	"--regression",
	"game:\\basic.exe",
	"game:\\basic-float.exe",
	"game:\\basic-long.exe",
	"game:\\basic-calls.exe",
	"game:\\basic-math.exe",
	"game:\\objects.exe",
	"game:\\arrays.exe",
	"game:\\exceptions.exe",
	"game:\\iltests.exe",
	"game:\\generics.exe",
	"game:\\gshared.exe",
	"game:\\devirtualization.exe",
	"game:\\builtin-types.exe",
	"game:\\aot-tests.exe",
	"game:\\unaligned.exe",
	"game:\\ratests.exe",
	"game:\\basic-simd.exe",
	"game:\\basic-vectors.exe",
};

// regress_args.txt beside the title, one argument a line, replaces the list above, to narrow a failure down; a line
// "env:NAME=VALUE" sets an environment variable instead, such as MONO_VERBOSE_METHOD.
static int read_arguments(char *list[ARGUMENTS_MAX], char storage[ARGUMENTS_MAX][ARGUMENT_BYTES])
{
	FILE *file = fopen("game:\\regress_args.txt", "r");
	int count = 1;

	if (!file)
		return 0;
	list[0] = "mono";
	while (count < ARGUMENTS_MAX && fgets(storage[count], ARGUMENT_BYTES, file)) {
		storage[count][strcspn(storage[count], "\r\n")] = '\0';
		if (strncmp(storage[count], "env:", 4) == 0)
			(void)putenv(storage[count] + 4);
		else if (storage[count][0])
			list[count] = storage[count], count++;
	}
	(void)fclose(file);
	return count;
}

int main(void)
{
	static char storage[ARGUMENTS_MAX][ARGUMENT_BYTES];
	char *from_file[ARGUMENTS_MAX];
	struct timespec start;
	struct timespec end;
	int count;
	int status;

	xsdk_log_write("mono_regress: Mono's regression tests");
	(void)xsdk_splash_show(SPLASH_MS);
	mono_set_dirs("game:\\", "game:\\");
	mono_set_assemblies_path("game:\\");
	(void)clock_gettime(CLOCK_MONOTONIC, &start);
	count = read_arguments(from_file, storage);
	status = count ? mono_main(count, from_file) : mono_main((int)(sizeof arguments / sizeof arguments[0]), arguments);
	(void)clock_gettime(CLOCK_MONOTONIC, &end);
	xsdk_log_write("mono_regress: mono_main returned %d after %lld ms, JIT pool used %u bytes", status,
		(long long)(end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000,
		(unsigned)mono_xsdk_jit_used());
	return status;
}
