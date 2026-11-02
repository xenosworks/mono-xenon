#include <stdint.h>

#include <mono/utils/mono-xsdk.h>
#include <xsdk/log.h>

int64_t mono_abi_unwind64(void *callback);

#pragma pack(push, 1)
typedef struct {
	uint8_t a;
} MatrixS1;
typedef struct {
	uint16_t a;
} MatrixS2;
typedef struct {
	uint8_t a, b, c;
} MatrixS3;
typedef struct {
	uint32_t a;
} MatrixS4;
typedef struct {
	uint32_t a;
	uint8_t b;
} MatrixS5;
typedef struct {
	uint16_t a;
	uint32_t b;
} MatrixS6;
typedef struct {
	uint32_t a;
	uint16_t b;
	uint8_t c;
} MatrixS7;
typedef struct {
	uint32_t a, b;
} MatrixS8;
typedef struct {
	uint32_t a, b;
	uint8_t c;
} MatrixS9;
typedef struct {
	uint32_t a, b, c;
} MatrixS12;
typedef struct {
	uint32_t a, b, c, d;
} MatrixS16;
typedef struct {
	uint32_t a, b, c, d, e, f;
} MatrixS24;
typedef struct {
	uint32_t a, b, c, d, e, f, g, h, i, j, k, l;
} MatrixS48;
#pragma pack(pop)

#define XOR8(v)  ((v) ^ 0x5A)
#define XOR16(v) ((v) ^ 0x5A5A)
#define XOR32(v) ((v) ^ 0x5A5A5A5A)

#define MATRIX_FUNCTION(type, name, body) \
	type name(type value)                 \
	{                                     \
		body;                             \
		return value;                     \
	}

MATRIX_FUNCTION(MatrixS1, mono_abi_matrix_s1, value.a = XOR8(value.a))
MATRIX_FUNCTION(MatrixS2, mono_abi_matrix_s2, value.a = XOR16(value.a))
MATRIX_FUNCTION(MatrixS3, mono_abi_matrix_s3, value.a = XOR8(value.a); value.b = XOR8(value.b); value.c = XOR8(value.c))
MATRIX_FUNCTION(MatrixS4, mono_abi_matrix_s4, value.a = XOR32(value.a))
MATRIX_FUNCTION(MatrixS5, mono_abi_matrix_s5, value.a = XOR32(value.a); value.b = XOR8(value.b))
MATRIX_FUNCTION(MatrixS6, mono_abi_matrix_s6, value.a = XOR16(value.a); value.b = XOR32(value.b))
MATRIX_FUNCTION(MatrixS7, mono_abi_matrix_s7, value.a = XOR32(value.a); value.b = XOR16(value.b);
	value.c = XOR8(value.c))
MATRIX_FUNCTION(MatrixS8, mono_abi_matrix_s8, value.a = XOR32(value.a); value.b = XOR32(value.b))
MATRIX_FUNCTION(MatrixS9, mono_abi_matrix_s9, value.a = XOR32(value.a); value.b = XOR32(value.b);
	value.c = XOR8(value.c))
MATRIX_FUNCTION(MatrixS12, mono_abi_matrix_s12, value.a = XOR32(value.a); value.b = XOR32(value.b);
	value.c = XOR32(value.c))
MATRIX_FUNCTION(MatrixS16, mono_abi_matrix_s16, value.a = XOR32(value.a); value.b = XOR32(value.b);
	value.c = XOR32(value.c); value.d = XOR32(value.d))
MATRIX_FUNCTION(MatrixS24, mono_abi_matrix_s24, value.a = XOR32(value.a); value.b = XOR32(value.b);
	value.c = XOR32(value.c); value.d = XOR32(value.d); value.e = XOR32(value.e); value.f = XOR32(value.f))
MATRIX_FUNCTION(MatrixS48, mono_abi_matrix_s48, value.a = XOR32(value.a); value.b = XOR32(value.b);
	value.c = XOR32(value.c); value.d = XOR32(value.d); value.e = XOR32(value.e); value.f = XOR32(value.f);
	value.g = XOR32(value.g); value.h = XOR32(value.h); value.i = XOR32(value.i); value.j = XOR32(value.j);
	value.k = XOR32(value.k); value.l = XOR32(value.l))

#define MATRIX_CALLBACK(type, name, ...) \
	type name(type (*callback)(type))    \
	{                                    \
		type value = __VA_ARGS__;        \
		return callback(value);          \
	}

MATRIX_CALLBACK(MatrixS1, mono_abi_callback_s1, { 0x11 })
MATRIX_CALLBACK(MatrixS2, mono_abi_callback_s2, { 0x1234 })
MATRIX_CALLBACK(MatrixS3, mono_abi_callback_s3, { 0x11, 0x22, 0x33 })
MATRIX_CALLBACK(MatrixS4, mono_abi_callback_s4, { 0x11223344 })
MATRIX_CALLBACK(MatrixS5, mono_abi_callback_s5, { 0x11223344, 0x55 })
MATRIX_CALLBACK(MatrixS6, mono_abi_callback_s6, { 0x1234, 0x55667788 })
MATRIX_CALLBACK(MatrixS7, mono_abi_callback_s7, { 0x11223344, 0x5566, 0x77 })
MATRIX_CALLBACK(MatrixS8, mono_abi_callback_s8, { 0x11223344, 0x55667788 })
MATRIX_CALLBACK(MatrixS9, mono_abi_callback_s9, { 0x11223344, 0x55667788, 0x99 })
MATRIX_CALLBACK(MatrixS12, mono_abi_callback_s12, { 0x11223344, 0x55667788, 0x99AABBCC })
MATRIX_CALLBACK(MatrixS16, mono_abi_callback_s16, { 0x11223344, 0x55667788, 0x99AABBCC, 0xDDEFF001 })
MATRIX_CALLBACK(MatrixS24, mono_abi_callback_s24, { 1, 2, 3, 4, 5, 6 })
MATRIX_CALLBACK(MatrixS48, mono_abi_callback_s48, { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 })

static int64_t sum24(MatrixS24 value)
{
	return value.a + 2 * (int64_t)value.b + 3 * value.c + 4 * value.d + 5 * value.e + 6 * value.f;
}

static int64_t sum48(MatrixS48 value)
{
	return value.a + 2 * (int64_t)value.b + 3 * value.c + 4 * value.d + 5 * value.e + 6 * value.f + 7 * value.g +
		8 * value.h + 9 * value.i + 10 * value.j + 11 * value.k + 12 * value.l;
}

int64_t mono_abi_counts5(int a, int b, int c, int d, int e, MatrixS24 value, int after)
{
	return a + b + c + d + e + sum24(value) + after;
}

int64_t mono_abi_counts6(int a, int b, int c, int d, int e, int f, MatrixS24 value, int after)
{
	return a + b + c + d + e + f + sum24(value) + after;
}

#define COUNTS7(name, type, sum)                                                         \
	int64_t name(int a, int b, int c, int d, int e, int f, int g, type value, int after) \
	{                                                                                    \
		return a + b + c + d + e + f + g + sum(value) + after;                           \
	}

static int64_t sum9(MatrixS9 value)
{
	return value.a + 2 * (int64_t)value.b + 3 * value.c;
}
static int64_t sum12(MatrixS12 value)
{
	return value.a + 2 * (int64_t)value.b + 3 * value.c;
}
static int64_t sum16(MatrixS16 value)
{
	return value.a + 2 * (int64_t)value.b + 3 * value.c + 4 * value.d;
}

COUNTS7(mono_abi_counts7_s9, MatrixS9, sum9)
COUNTS7(mono_abi_counts7_s12, MatrixS12, sum12)
COUNTS7(mono_abi_counts7_s16, MatrixS16, sum16)
COUNTS7(mono_abi_counts7_s24, MatrixS24, sum24)

int64_t mono_abi_counts3_s48(int a, int b, int c, MatrixS48 value, int after)
{
	return a + b + c + sum48(value) + after;
}

int64_t mono_abi_counts9(int a, int b, int c, int d, int e, int f, int g, int h, int i, MatrixS24 value, int after)
{
	return a + b + c + d + e + f + g + h + i + sum24(value) + after;
}

double mono_abi_d15(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j,
	double k, double l, double m, double n, double o)
{
	return a + 2 * b + 3 * c + 4 * d + 5 * e + 6 * f + 7 * g + 8 * h + 9 * i + 10 * j + 11 * k + 12 * l + 13 * m +
		14 * n + 15 * o;
}

float mono_abi_f15(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k,
	float l, float m, float n, float o)
{
	return a + 2 * b + 3 * c + 4 * d + 5 * e + 6 * f + 7 * g + 8 * h + 9 * i + 10 * j + 11 * k + 12 * l + 13 * m +
		14 * n + 15 * o;
}

typedef int64_t (*Counts5Callback)(int, int, int, int, int, MatrixS24, int);
typedef int64_t (*Counts6Callback)(int, int, int, int, int, int, MatrixS24, int);
typedef int64_t (*Counts7S9Callback)(int, int, int, int, int, int, int, MatrixS9, int);
typedef int64_t (*Counts7S12Callback)(int, int, int, int, int, int, int, MatrixS12, int);
typedef int64_t (*Counts7S16Callback)(int, int, int, int, int, int, int, MatrixS16, int);
typedef int64_t (*Counts7S24Callback)(int, int, int, int, int, int, int, MatrixS24, int);
typedef int64_t (*Counts3S48Callback)(int, int, int, MatrixS48, int);
typedef int64_t (*Counts9Callback)(int, int, int, int, int, int, int, int, int, MatrixS24, int);
typedef double (*D15Callback)(double, double, double, double, double, double, double, double, double, double, double,
	double, double, double, double);
typedef float (*F15Callback)(
	float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);

int64_t mono_abi_callback_counts5(Counts5Callback callback)
{
	MatrixS24 value = { 1, 2, 3, 4, 5, 6 };
	return callback(1, 2, 3, 4, 5, value, 99);
}

int64_t mono_abi_callback_counts6(Counts6Callback callback)
{
	MatrixS24 value = { 1, 2, 3, 4, 5, 6 };
	return callback(1, 2, 3, 4, 5, 6, value, 99);
}

int64_t mono_abi_callback_counts7_s9(Counts7S9Callback callback)
{
	MatrixS9 value = { 1, 2, 3 };
	return callback(1, 2, 3, 4, 5, 6, 7, value, 99);
}

int64_t mono_abi_callback_counts7_s12(Counts7S12Callback callback)
{
	MatrixS12 value = { 1, 2, 3 };
	return callback(1, 2, 3, 4, 5, 6, 7, value, 99);
}

int64_t mono_abi_callback_counts7_s16(Counts7S16Callback callback)
{
	MatrixS16 value = { 1, 2, 3, 4 };
	return callback(1, 2, 3, 4, 5, 6, 7, value, 99);
}

int64_t mono_abi_callback_counts7_s24(Counts7S24Callback callback)
{
	MatrixS24 value = { 1, 2, 3, 4, 5, 6 };
	return callback(1, 2, 3, 4, 5, 6, 7, value, 99);
}

int64_t mono_abi_callback_counts3_s48(Counts3S48Callback callback)
{
	MatrixS48 value = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	return callback(1, 2, 3, value, 99);
}

int64_t mono_abi_callback_counts9(Counts9Callback callback)
{
	MatrixS24 value = { 1, 2, 3, 4, 5, 6 };
	return callback(1, 2, 3, 4, 5, 6, 7, 8, 9, value, 99);
}

double mono_abi_callback_d15(D15Callback callback)
{
	return callback(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}

float mono_abi_callback_f15(F15Callback callback)
{
	return callback(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
}

int mono_abi_matrix_register(void)
{
	struct Entry {
		const char *name;
		void *address;
	};
	static const struct Entry entries[] = {
		{ "mono_abi_matrix_s1", mono_abi_matrix_s1 },
		{ "mono_abi_matrix_s2", mono_abi_matrix_s2 },
		{ "mono_abi_matrix_s3", mono_abi_matrix_s3 },
		{ "mono_abi_matrix_s4", mono_abi_matrix_s4 },
		{ "mono_abi_matrix_s5", mono_abi_matrix_s5 },
		{ "mono_abi_matrix_s6", mono_abi_matrix_s6 },
		{ "mono_abi_matrix_s7", mono_abi_matrix_s7 },
		{ "mono_abi_matrix_s8", mono_abi_matrix_s8 },
		{ "mono_abi_matrix_s9", mono_abi_matrix_s9 },
		{ "mono_abi_matrix_s12", mono_abi_matrix_s12 },
		{ "mono_abi_matrix_s16", mono_abi_matrix_s16 },
		{ "mono_abi_matrix_s24", mono_abi_matrix_s24 },
		{ "mono_abi_matrix_s48", mono_abi_matrix_s48 },
		{ "mono_abi_callback_s1", mono_abi_callback_s1 },
		{ "mono_abi_callback_s2", mono_abi_callback_s2 },
		{ "mono_abi_callback_s3", mono_abi_callback_s3 },
		{ "mono_abi_callback_s4", mono_abi_callback_s4 },
		{ "mono_abi_callback_s5", mono_abi_callback_s5 },
		{ "mono_abi_callback_s6", mono_abi_callback_s6 },
		{ "mono_abi_callback_s7", mono_abi_callback_s7 },
		{ "mono_abi_callback_s8", mono_abi_callback_s8 },
		{ "mono_abi_callback_s9", mono_abi_callback_s9 },
		{ "mono_abi_callback_s12", mono_abi_callback_s12 },
		{ "mono_abi_callback_s16", mono_abi_callback_s16 },
		{ "mono_abi_callback_s24", mono_abi_callback_s24 },
		{ "mono_abi_callback_s48", mono_abi_callback_s48 },
		{ "mono_abi_counts5", mono_abi_counts5 },
		{ "mono_abi_counts6", mono_abi_counts6 },
		{ "mono_abi_counts7_s9", mono_abi_counts7_s9 },
		{ "mono_abi_counts7_s12", mono_abi_counts7_s12 },
		{ "mono_abi_counts7_s16", mono_abi_counts7_s16 },
		{ "mono_abi_counts7_s24", mono_abi_counts7_s24 },
		{ "mono_abi_counts3_s48", mono_abi_counts3_s48 },
		{ "mono_abi_counts9", mono_abi_counts9 },
		{ "mono_abi_d15", mono_abi_d15 },
		{ "mono_abi_f15", mono_abi_f15 },
		{ "mono_abi_callback_counts5", mono_abi_callback_counts5 },
		{ "mono_abi_callback_counts6", mono_abi_callback_counts6 },
		{ "mono_abi_callback_counts7_s9", mono_abi_callback_counts7_s9 },
		{ "mono_abi_callback_counts7_s12", mono_abi_callback_counts7_s12 },
		{ "mono_abi_callback_counts7_s16", mono_abi_callback_counts7_s16 },
		{ "mono_abi_callback_counts7_s24", mono_abi_callback_counts7_s24 },
		{ "mono_abi_callback_counts3_s48", mono_abi_callback_counts3_s48 },
		{ "mono_abi_callback_counts9", mono_abi_callback_counts9 },
		{ "mono_abi_callback_d15", mono_abi_callback_d15 },
		{ "mono_abi_callback_f15", mono_abi_callback_f15 },
		{ "mono_abi_unwind64", mono_abi_unwind64 },
	};

	for (unsigned i = 0; i < sizeof entries / sizeof entries[0]; ++i) {
		if (mono_xsdk_pinvoke_register("__Internal", entries[i].name, entries[i].address) != 0) {
			xsdk_log_write("mono_abi: registering %s failed", entries[i].name);
			return -1;
		}
	}
	return 0;
}
