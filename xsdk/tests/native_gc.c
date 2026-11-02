#include <stdint.h>
#include <time.h>

#include <mono/utils/mono-xsdk.h>
#include <xsdk/log.h>

static int native_entered;
static int native_release;

int mono_gc_native_wait(void)
{
	const struct timespec delay = { 0, 1000000 };

	__atomic_store_n(&native_entered, 1, __ATOMIC_RELEASE);
	while (!__atomic_load_n(&native_release, __ATOMIC_ACQUIRE)) {
		if (nanosleep(&delay, NULL) != 0)
			return -1;
	}
	return 0x1234;
}

int mono_gc_native_state(void)
{
	return __atomic_load_n(&native_entered, __ATOMIC_ACQUIRE) | __atomic_load_n(&native_release, __ATOMIC_ACQUIRE) << 1;
}

void mono_gc_native_release(void)
{
	__atomic_store_n(&native_release, 1, __ATOMIC_RELEASE);
}

void mono_gc_pause(int index, int64_t microseconds)
{
	xsdk_log_write("mono_gc: collection %d pause %lld us", index, (long long)microseconds);
}

int64_t mono_gc_now_us(void)
{
	struct timespec now;

	if (clock_gettime(CLOCK_MONOTONIC, &now) != 0)
		return -1;
	return (int64_t)now.tv_sec * 1000000 + now.tv_nsec / 1000;
}

int mono_test_register(void)
{
	struct Entry {
		const char *name;
		void *address;
	};
	static const struct Entry entries[] = {
		{ "mono_gc_native_wait", mono_gc_native_wait },
		{ "mono_gc_native_state", mono_gc_native_state },
		{ "mono_gc_native_release", mono_gc_native_release },
		{ "mono_gc_pause", mono_gc_pause },
		{ "mono_gc_now_us", mono_gc_now_us },
	};

	native_entered = 0;
	native_release = 0;
	for (unsigned i = 0; i < sizeof entries / sizeof entries[0]; ++i) {
		if (mono_xsdk_pinvoke_register("__Internal", entries[i].name, entries[i].address) != 0) {
			xsdk_log_write("mono_gc: registering %s failed", entries[i].name);
			return -1;
		}
	}
	return 0;
}
