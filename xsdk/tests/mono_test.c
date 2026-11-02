#include "mono_test.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/utils/mono-xsdk.h>
#include <xsdk/kernel.h>
#include <xsdk/log.h>
#include <xsdk/splash.h>

enum {
	SPLASH_MS = 2000,
	// MmQueryStatistics' pages (xsdk:include/xsdk/kernel.h).
	PAGE_KB = 4,
	MICROSECONDS_PER_MILLISECOND = 1000,
};

static unsigned passed_count;
static unsigned failed_count;

void mono_test_check(const char *name, int passed)
{
	xsdk_log_write("%s: %s", passed ? "PASS" : "FAIL", name);
	passed_count += passed != 0;
	failed_count += passed == 0;
}

static void managed_log(const char *message)
{
	if (!message) {
		xsdk_log_write("managed: (null)");
		return;
	}
	xsdk_log_write("managed: %s", message);
}

static int register_function(const char *name, void *address)
{
	if (mono_xsdk_pinvoke_register("__Internal", name, address) == 0)
		return 0;
	xsdk_log_write("mono: registering %s failed", name);
	return -1;
}

__attribute__((weak)) int mono_test_register(void)
{
	return 0;
}

static MM_QUERY_STATISTICS_SECTION title_memory(void)
{
	MM_QUERY_STATISTICS_RESULT statistics = { .size = sizeof(statistics) };

	(void)MmQueryStatistics(&statistics);
	return statistics.title;
}

static uint64_t now_us(void)
{
	struct timespec now;

	(void)clock_gettime(CLOCK_MONOTONIC, &now);
	return (uint64_t)now.tv_sec * 1000000 + (uint64_t)now.tv_nsec / 1000;
}

static unsigned since_ms(uint64_t start_us, uint64_t end_us)
{
	return (unsigned)((end_us - start_us) / MICROSECONDS_PER_MILLISECOND);
}

static uint64_t read_register(unsigned reg)
{
	uint64_t value;

	if (reg == 2)
		__asm__ volatile("mr %0, 2" : "=r"(value));
	else
		__asm__ volatile("mr %0, 13" : "=r"(value));
	return value;
}

int mono_test_run(const char *name)
{
	char path[128];
	char *argv[] = { (char *)name };
	MonoDomain *domain;
	MonoAssembly *assembly;
	MonoImageOpenStatus image_status;
	const uint64_t r2 = read_register(2);
	const uint64_t r13 = read_register(13);
	int managed_status;

	passed_count = 0;
	failed_count = 0;
	xsdk_log_write("%s: Mono on xsdk", name);
	mono_test_check("splash shown", NT_SUCCESS(xsdk_splash_show(SPLASH_MS)));
	if (register_function("mono_test_check", mono_test_check) != 0 ||
		register_function("mono_test_log", managed_log) != 0 || mono_test_register() != 0)
		return 1;

	const MM_QUERY_STATISTICS_SECTION before = title_memory();
	const uint64_t start_us = now_us();
	mono_set_dirs("game:\\", "game:\\");
	mono_set_assemblies_path("game:\\");
	domain = mono_jit_init_version(name, "v4.0.30319");
	const uint64_t started_us = now_us();
	mono_test_check("Mono runtime initialized", domain != NULL);
	if (!domain)
		return 1;

	(void)snprintf(path, sizeof path, "game:\\%s.exe", name);
	const uint64_t open_us = now_us();
	assembly = mono_assembly_open(path, &image_status);
	const uint64_t opened_us = now_us();
	mono_test_check("managed assembly loaded", assembly != NULL);
	if (!assembly) {
		xsdk_log_write("mono: assembly status %d for %s", image_status, path);
		mono_jit_cleanup(domain);
		return 1;
	}

	const uint64_t exec_us = now_us();
	managed_status = mono_jit_exec(domain, assembly, 1, argv);
	const uint64_t ran_us = now_us();
	const MM_QUERY_STATISTICS_SECTION after = title_memory();
	xsdk_log_write("%s: runtime started in %u ms, assembly opened in %u ms, Main ran in %u ms", name,
		since_ms(start_us, started_us), since_ms(open_us, opened_us), since_ms(exec_us, ran_us));
	xsdk_log_write("%s: image %u KB; %u KB free before the runtime started, %u KB after Main; JIT pool used %u bytes",
		name, before.image_pages * PAGE_KB, before.available_pages * PAGE_KB, after.available_pages * PAGE_KB,
		(unsigned)mono_xsdk_jit_used());
	mono_test_check("managed Main returned zero", managed_status == 0);
	if (strcmp(name, "mono_abi") == 0)
		mono_test_check("r2 and r13 preserved", read_register(2) == r2 && read_register(13) == r13);
	mono_jit_cleanup(domain);
	xsdk_log_write("%s: %u passed, %u failed", name, passed_count, failed_count);
	return failed_count != 0 || managed_status != 0;
}
