#include <stdint.h>

#include <mono/utils/mono-xsdk.h>
#include <xsdk/log.h>

typedef struct {
	uint8_t value;
} Small1;

typedef struct {
	int32_t first;
	int32_t second;
} Small8;

typedef struct {
	int32_t values[6];
} Large24;

typedef int64_t (*TestCallback)(Small8 small, Large24 large, double value);

int32_t mono_abi_i32(int32_t left, int32_t right)
{
	return left * 3 + right;
}

int64_t mono_abi_i64(int64_t left, int64_t right)
{
	return left - right;
}

float mono_abi_f32(float left, float right)
{
	return left * 2.0f + right;
}

double mono_abi_f64(double left, double right)
{
	return left * 0.5 + right;
}

Small1 mono_abi_small1(Small1 input)
{
	Small1 result = { (uint8_t)(input.value ^ 0xFF) };
	return result;
}

Small8 mono_abi_small8(Small8 input)
{
	Small8 result = { input.first + 7, input.second ^ 0x05555555 };
	return result;
}

Large24 mono_abi_large24(Large24 input)
{
	Large24 result;
	for (int i = 0; i < 6; ++i)
		result.values[i] = input.values[i] + (i + 1) * 11;
	return result;
}

int64_t mono_abi_mixed(int32_t a, int32_t b, int32_t c, int32_t d, int32_t e, int32_t f, int32_t g, Large24 input,
	int32_t after, double scale)
{
	int64_t result = a + b + c + d + e + f + g + after + (int64_t)scale;
	for (int i = 0; i < 6; ++i)
		result += input.values[i] * (i + 1);
	return result;
}

int64_t mono_abi_callback(TestCallback callback)
{
	const Small8 small = { -123456, 0x01020304 };
	const Large24 large = { { 2, 3, 5, 7, 11, 13 } };
	return callback(small, large, 2.5);
}

int mono_abi_matrix_register(void);

int mono_test_register(void)
{
	struct Entry {
		const char *name;
		void *address;
	};
	static const struct Entry entries[] = {
		{ "mono_abi_i32", mono_abi_i32 },
		{ "mono_abi_i64", mono_abi_i64 },
		{ "mono_abi_f32", mono_abi_f32 },
		{ "mono_abi_f64", mono_abi_f64 },
		{ "mono_abi_small1", mono_abi_small1 },
		{ "mono_abi_small8", mono_abi_small8 },
		{ "mono_abi_large24", mono_abi_large24 },
		{ "mono_abi_mixed", mono_abi_mixed },
		{ "mono_abi_callback", mono_abi_callback },
	};

	for (unsigned i = 0; i < sizeof entries / sizeof entries[0]; ++i) {
		if (mono_xsdk_pinvoke_register("__Internal", entries[i].name, entries[i].address) != 0) {
			xsdk_log_write("mono_abi: registering %s failed", entries[i].name);
			return -1;
		}
	}
	return mono_abi_matrix_register();
}
