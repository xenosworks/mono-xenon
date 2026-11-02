using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS1 { public byte A; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS2 { public ushort A; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS3 { public byte A, B, C; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS4 { public uint A; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS5 { public uint A; public byte B; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS6 { public ushort A; public uint B; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS7 { public uint A; public ushort B; public byte C; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS8 { public uint A, B; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS9 { public uint A, B; public byte C; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS12 { public uint A, B, C; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS16 { public uint A, B, C, D; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS24 { public uint A, B, C, D, E, F; }
[StructLayout(LayoutKind.Sequential, Pack = 1)] internal struct MatrixS48 { public uint A, B, C, D, E, F, G, H, I, J, K, L; }

internal static class AbiMatrix
{
    private static byte X8(byte value) { return (byte)(value ^ 0x5A); }
    private static ushort X16(ushort value) { return (ushort)(value ^ 0x5A5A); }
    private static uint X32(uint value) { return value ^ 0x5A5A5A5A; }

    [DllImport("__Internal")] private static extern MatrixS1 mono_abi_matrix_s1(MatrixS1 value);
    [DllImport("__Internal")] private static extern MatrixS2 mono_abi_matrix_s2(MatrixS2 value);
    [DllImport("__Internal")] private static extern MatrixS3 mono_abi_matrix_s3(MatrixS3 value);
    [DllImport("__Internal")] private static extern MatrixS4 mono_abi_matrix_s4(MatrixS4 value);
    [DllImport("__Internal")] private static extern MatrixS5 mono_abi_matrix_s5(MatrixS5 value);
    [DllImport("__Internal")] private static extern MatrixS6 mono_abi_matrix_s6(MatrixS6 value);
    [DllImport("__Internal")] private static extern MatrixS7 mono_abi_matrix_s7(MatrixS7 value);
    [DllImport("__Internal")] private static extern MatrixS8 mono_abi_matrix_s8(MatrixS8 value);
    [DllImport("__Internal")] private static extern MatrixS9 mono_abi_matrix_s9(MatrixS9 value);
    [DllImport("__Internal")] private static extern MatrixS12 mono_abi_matrix_s12(MatrixS12 value);
    [DllImport("__Internal")] private static extern MatrixS16 mono_abi_matrix_s16(MatrixS16 value);
    [DllImport("__Internal")] private static extern MatrixS24 mono_abi_matrix_s24(MatrixS24 value);
    [DllImport("__Internal")] private static extern MatrixS48 mono_abi_matrix_s48(MatrixS48 value);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS1 CallbackS1(MatrixS1 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS2 CallbackS2(MatrixS2 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS3 CallbackS3(MatrixS3 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS4 CallbackS4(MatrixS4 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS5 CallbackS5(MatrixS5 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS6 CallbackS6(MatrixS6 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS7 CallbackS7(MatrixS7 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS8 CallbackS8(MatrixS8 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS9 CallbackS9(MatrixS9 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS12 CallbackS12(MatrixS12 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS16 CallbackS16(MatrixS16 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS24 CallbackS24(MatrixS24 value);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate MatrixS48 CallbackS48(MatrixS48 value);

    [DllImport("__Internal")] private static extern MatrixS1 mono_abi_callback_s1(CallbackS1 callback);
    [DllImport("__Internal")] private static extern MatrixS2 mono_abi_callback_s2(CallbackS2 callback);
    [DllImport("__Internal")] private static extern MatrixS3 mono_abi_callback_s3(CallbackS3 callback);
    [DllImport("__Internal")] private static extern MatrixS4 mono_abi_callback_s4(CallbackS4 callback);
    [DllImport("__Internal")] private static extern MatrixS5 mono_abi_callback_s5(CallbackS5 callback);
    [DllImport("__Internal")] private static extern MatrixS6 mono_abi_callback_s6(CallbackS6 callback);
    [DllImport("__Internal")] private static extern MatrixS7 mono_abi_callback_s7(CallbackS7 callback);
    [DllImport("__Internal")] private static extern MatrixS8 mono_abi_callback_s8(CallbackS8 callback);
    [DllImport("__Internal")] private static extern MatrixS9 mono_abi_callback_s9(CallbackS9 callback);
    [DllImport("__Internal")] private static extern MatrixS12 mono_abi_callback_s12(CallbackS12 callback);
    [DllImport("__Internal")] private static extern MatrixS16 mono_abi_callback_s16(CallbackS16 callback);
    [DllImport("__Internal")] private static extern MatrixS24 mono_abi_callback_s24(CallbackS24 callback);
    [DllImport("__Internal")] private static extern MatrixS48 mono_abi_callback_s48(CallbackS48 callback);

    private static MatrixS1 Callback1(MatrixS1 value) { if (value.A != 0x11) return default(MatrixS1); value.A = X8(value.A); return value; }
    private static MatrixS2 Callback2(MatrixS2 value) { if (value.A != 0x1234) return default(MatrixS2); value.A = X16(value.A); return value; }
    private static MatrixS3 Callback3(MatrixS3 value) { if (value.A != 0x11 || value.B != 0x22 || value.C != 0x33) return default(MatrixS3); value.A = X8(value.A); value.B = X8(value.B); value.C = X8(value.C); return value; }
    private static MatrixS4 Callback4(MatrixS4 value) { if (value.A != 0x11223344) return default(MatrixS4); value.A = X32(value.A); return value; }
    private static MatrixS5 Callback5(MatrixS5 value) { if (value.A != 0x11223344 || value.B != 0x55) return default(MatrixS5); value.A = X32(value.A); value.B = X8(value.B); return value; }
    private static MatrixS6 Callback6(MatrixS6 value) { if (value.A != 0x1234 || value.B != 0x55667788) return default(MatrixS6); value.A = X16(value.A); value.B = X32(value.B); return value; }
    private static MatrixS7 Callback7(MatrixS7 value) { if (value.A != 0x11223344 || value.B != 0x5566 || value.C != 0x77) return default(MatrixS7); value.A = X32(value.A); value.B = X16(value.B); value.C = X8(value.C); return value; }
    private static MatrixS8 Callback8(MatrixS8 value) { if (value.A != 0x11223344 || value.B != 0x55667788) return default(MatrixS8); value.A = X32(value.A); value.B = X32(value.B); return value; }
    private static MatrixS9 Callback9(MatrixS9 value) { if (value.A != 0x11223344 || value.B != 0x55667788 || value.C != 0x99) return default(MatrixS9); value.A = X32(value.A); value.B = X32(value.B); value.C = X8(value.C); return value; }
    private static MatrixS12 Callback12(MatrixS12 value) { if (value.A != 0x11223344 || value.B != 0x55667788 || value.C != 0x99AABBCC) return default(MatrixS12); value.A = X32(value.A); value.B = X32(value.B); value.C = X32(value.C); return value; }
    private static MatrixS16 Callback16(MatrixS16 value) { if (value.A != 0x11223344 || value.B != 0x55667788 || value.C != 0x99AABBCC || value.D != 0xDDEFF001) return default(MatrixS16); value.A = X32(value.A); value.B = X32(value.B); value.C = X32(value.C); value.D = X32(value.D); return value; }
    private static MatrixS24 Callback24(MatrixS24 value) { if (value.A != 1 || value.B != 2 || value.C != 3 || value.D != 4 || value.E != 5 || value.F != 6) return default(MatrixS24); value.A = X32(value.A); value.B = X32(value.B); value.C = X32(value.C); value.D = X32(value.D); value.E = X32(value.E); value.F = X32(value.F); return value; }
    private static MatrixS48 Callback48(MatrixS48 value) { if (value.A != 1 || value.B != 2 || value.C != 3 || value.D != 4 || value.E != 5 || value.F != 6 || value.G != 7 || value.H != 8 || value.I != 9 || value.J != 10 || value.K != 11 || value.L != 12) return default(MatrixS48); value.A = X32(value.A); value.B = X32(value.B); value.C = X32(value.C); value.D = X32(value.D); value.E = X32(value.E); value.F = X32(value.F); value.G = X32(value.G); value.H = X32(value.H); value.I = X32(value.I); value.J = X32(value.J); value.K = X32(value.K); value.L = X32(value.L); return value; }

    private static long Sum24(MatrixS24 value) { return value.A + 2L * value.B + 3L * value.C + 4L * value.D + 5L * value.E + 6L * value.F; }
    private static long Sum48(MatrixS48 value) { return value.A + 2L * value.B + 3L * value.C + 4L * value.D + 5L * value.E + 6L * value.F + 7L * value.G + 8L * value.H + 9L * value.I + 10L * value.J + 11L * value.K + 12L * value.L; }
    private static long Sum9(MatrixS9 value) { return value.A + 2L * value.B + 3L * value.C; }
    private static long Sum12(MatrixS12 value) { return value.A + 2L * value.B + 3L * value.C; }
    private static long Sum16(MatrixS16 value) { return value.A + 2L * value.B + 3L * value.C + 4L * value.D; }
    private static MatrixS24 Make24() { return new MatrixS24 { A = 1, B = 2, C = 3, D = 4, E = 5, F = 6 }; }
    private static MatrixS48 Make48() { return new MatrixS48 { A = 1, B = 2, C = 3, D = 4, E = 5, F = 6, G = 7, H = 8, I = 9, J = 10, K = 11, L = 12 }; }

    [DllImport("__Internal")] private static extern long mono_abi_counts5(int a, int b, int c, int d, int e, MatrixS24 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts6(int a, int b, int c, int d, int e, int f, MatrixS24 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts7_s9(int a, int b, int c, int d, int e, int f, int g, MatrixS9 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts7_s12(int a, int b, int c, int d, int e, int f, int g, MatrixS12 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts7_s16(int a, int b, int c, int d, int e, int f, int g, MatrixS16 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts7_s24(int a, int b, int c, int d, int e, int f, int g, MatrixS24 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts3_s48(int a, int b, int c, MatrixS48 value, int after);
    [DllImport("__Internal")] private static extern long mono_abi_counts9(int a, int b, int c, int d, int e, int f, int g, int h, int i, MatrixS24 value, int after);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts5Callback(int a, int b, int c, int d, int e, MatrixS24 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts6Callback(int a, int b, int c, int d, int e, int f, MatrixS24 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts7S9Callback(int a, int b, int c, int d, int e, int f, int g, MatrixS9 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts7S12Callback(int a, int b, int c, int d, int e, int f, int g, MatrixS12 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts7S16Callback(int a, int b, int c, int d, int e, int f, int g, MatrixS16 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts7S24Callback(int a, int b, int c, int d, int e, int f, int g, MatrixS24 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts3S48Callback(int a, int b, int c, MatrixS48 value, int after);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate long Counts9Callback(int a, int b, int c, int d, int e, int f, int g, int h, int i, MatrixS24 value, int after);

    [DllImport("__Internal")] private static extern long mono_abi_callback_counts5(Counts5Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts6(Counts6Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts7_s9(Counts7S9Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts7_s12(Counts7S12Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts7_s16(Counts7S16Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts7_s24(Counts7S24Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts3_s48(Counts3S48Callback callback);
    [DllImport("__Internal")] private static extern long mono_abi_callback_counts9(Counts9Callback callback);

    private static long Counts5(int a, int b, int c, int d, int e, MatrixS24 value, int after) { return a + b + c + d + e + Sum24(value) + after; }
    private static long Counts6(int a, int b, int c, int d, int e, int f, MatrixS24 value, int after) { return a + b + c + d + e + f + Sum24(value) + after; }
    private static long Counts7S9(int a, int b, int c, int d, int e, int f, int g, MatrixS9 value, int after) { return a + b + c + d + e + f + g + Sum9(value) + after; }
    private static long Counts7S12(int a, int b, int c, int d, int e, int f, int g, MatrixS12 value, int after) { return a + b + c + d + e + f + g + Sum12(value) + after; }
    private static long Counts7S16(int a, int b, int c, int d, int e, int f, int g, MatrixS16 value, int after) { return a + b + c + d + e + f + g + Sum16(value) + after; }
    private static long Counts7S24(int a, int b, int c, int d, int e, int f, int g, MatrixS24 value, int after) { return a + b + c + d + e + f + g + Sum24(value) + after; }
    private static long Counts3S48(int a, int b, int c, MatrixS48 value, int after) { return a + b + c + Sum48(value) + after; }
    private static long Counts9(int a, int b, int c, int d, int e, int f, int g, int h, int i, MatrixS24 value, int after) { return a + b + c + d + e + f + g + h + i + Sum24(value) + after; }

    [DllImport("__Internal")] private static extern double mono_abi_d15(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o);
    [DllImport("__Internal")] private static extern float mono_abi_f15(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate double D15Callback(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)] private delegate float F15Callback(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o);
    [DllImport("__Internal")] private static extern double mono_abi_callback_d15(D15Callback callback);
    [DllImport("__Internal")] private static extern float mono_abi_callback_f15(F15Callback callback);

    private static double D15(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o) { return a + 2 * b + 3 * c + 4 * d + 5 * e + 6 * f + 7 * g + 8 * h + 9 * i + 10 * j + 11 * k + 12 * l + 13 * m + 14 * n + 15 * o; }
    private static float F15(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o) { return a + 2 * b + 3 * c + 4 * d + 5 * e + 6 * f + 7 * g + 8 * h + 9 * i + 10 * j + 11 * k + 12 * l + 13 * m + 14 * n + 15 * o; }

    private static void CheckStruct(string name, MatrixS1 actual, MatrixS1 expected) { AbiCheck.Fields(name, new uint[] { actual.A }, new uint[] { expected.A }); }
    private static void CheckStruct(string name, MatrixS2 actual, MatrixS2 expected) { AbiCheck.Fields(name, new uint[] { actual.A }, new uint[] { expected.A }); }
    private static void CheckStruct(string name, MatrixS3 actual, MatrixS3 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C }, new uint[] { expected.A, expected.B, expected.C }); }
    private static void CheckStruct(string name, MatrixS4 actual, MatrixS4 expected) { AbiCheck.Fields(name, new uint[] { actual.A }, new uint[] { expected.A }); }
    private static void CheckStruct(string name, MatrixS5 actual, MatrixS5 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B }, new uint[] { expected.A, expected.B }); }
    private static void CheckStruct(string name, MatrixS6 actual, MatrixS6 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B }, new uint[] { expected.A, expected.B }); }
    private static void CheckStruct(string name, MatrixS7 actual, MatrixS7 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C }, new uint[] { expected.A, expected.B, expected.C }); }
    private static void CheckStruct(string name, MatrixS8 actual, MatrixS8 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B }, new uint[] { expected.A, expected.B }); }
    private static void CheckStruct(string name, MatrixS9 actual, MatrixS9 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C }, new uint[] { expected.A, expected.B, expected.C }); }
    private static void CheckStruct(string name, MatrixS12 actual, MatrixS12 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C }, new uint[] { expected.A, expected.B, expected.C }); }
    private static void CheckStruct(string name, MatrixS16 actual, MatrixS16 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C, actual.D }, new uint[] { expected.A, expected.B, expected.C, expected.D }); }
    private static void CheckStruct(string name, MatrixS24 actual, MatrixS24 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C, actual.D, actual.E, actual.F }, new uint[] { expected.A, expected.B, expected.C, expected.D, expected.E, expected.F }); }
    private static void CheckStruct(string name, MatrixS48 actual, MatrixS48 expected) { AbiCheck.Fields(name, new uint[] { actual.A, actual.B, actual.C, actual.D, actual.E, actual.F, actual.G, actual.H, actual.I, actual.J, actual.K, actual.L }, new uint[] { expected.A, expected.B, expected.C, expected.D, expected.E, expected.F, expected.G, expected.H, expected.I, expected.J, expected.K, expected.L }); }

    private static void CheckStructs()
    {
        MatrixS1 s1 = mono_abi_matrix_s1(new MatrixS1 { A = 1 });
        CheckStruct("matrix: 1-byte struct argument and return", s1, new MatrixS1 { A = X8(1) });
        CallbackS1 cb1 = Callback1;
        MatrixS1 cr1 = mono_abi_callback_s1(cb1);
        CheckStruct("matrix: 1-byte callback", cr1, Callback1(new MatrixS1 { A = 0x11 }));
        GC.KeepAlive(cb1);

        MatrixS2 s2 = mono_abi_matrix_s2(new MatrixS2 { A = 2 });
        CheckStruct("matrix: 2-byte struct argument and return", s2, new MatrixS2 { A = X16(2) });
        CallbackS2 cb2 = Callback2;
        MatrixS2 cr2 = mono_abi_callback_s2(cb2);
        CheckStruct("matrix: 2-byte callback", cr2, Callback2(new MatrixS2 { A = 0x1234 }));
        GC.KeepAlive(cb2);

        MatrixS3 s3 = mono_abi_matrix_s3(new MatrixS3 { A = 1, B = 2, C = 3 });
        CheckStruct("matrix: 3-byte struct argument and return", s3, new MatrixS3 { A = X8(1), B = X8(2), C = X8(3) });
        CallbackS3 cb3 = Callback3;
        MatrixS3 cr3 = mono_abi_callback_s3(cb3);
        CheckStruct("matrix: 3-byte callback", cr3, Callback3(new MatrixS3 { A = 0x11, B = 0x22, C = 0x33 }));
        GC.KeepAlive(cb3);

        MatrixS4 s4 = mono_abi_matrix_s4(new MatrixS4 { A = 4 });
        CheckStruct("matrix: 4-byte struct argument and return", s4, new MatrixS4 { A = X32(4) });
        CallbackS4 cb4 = Callback4;
        MatrixS4 cr4 = mono_abi_callback_s4(cb4);
        CheckStruct("matrix: 4-byte callback", cr4, Callback4(new MatrixS4 { A = 0x11223344 }));
        GC.KeepAlive(cb4);

        MatrixS5 s5 = mono_abi_matrix_s5(new MatrixS5 { A = 4, B = 5 });
        CheckStruct("matrix: 5-byte struct argument and return", s5, new MatrixS5 { A = X32(4), B = X8(5) });
        CallbackS5 cb5 = Callback5;
        MatrixS5 cr5 = mono_abi_callback_s5(cb5);
        CheckStruct("matrix: 5-byte callback", cr5, Callback5(new MatrixS5 { A = 0x11223344, B = 0x55 }));
        GC.KeepAlive(cb5);

        MatrixS6 s6 = mono_abi_matrix_s6(new MatrixS6 { A = 5, B = 6 });
        CheckStruct("matrix: 6-byte struct argument and return", s6, new MatrixS6 { A = X16(5), B = X32(6) });
        CallbackS6 cb6 = Callback6;
        MatrixS6 cr6 = mono_abi_callback_s6(cb6);
        CheckStruct("matrix: 6-byte callback", cr6, Callback6(new MatrixS6 { A = 0x1234, B = 0x55667788 }));
        GC.KeepAlive(cb6);

        MatrixS7 s7 = mono_abi_matrix_s7(new MatrixS7 { A = 6, B = 7, C = 8 });
        CheckStruct("matrix: 7-byte struct argument and return", s7, new MatrixS7 { A = X32(6), B = X16(7), C = X8(8) });
        CallbackS7 cb7 = Callback7;
        MatrixS7 cr7 = mono_abi_callback_s7(cb7);
        CheckStruct("matrix: 7-byte callback", cr7, Callback7(new MatrixS7 { A = 0x11223344, B = 0x5566, C = 0x77 }));
        GC.KeepAlive(cb7);

        MatrixS8 s8 = mono_abi_matrix_s8(new MatrixS8 { A = 7, B = 8 });
        CheckStruct("matrix: 8-byte struct argument and return", s8, new MatrixS8 { A = X32(7), B = X32(8) });
        CallbackS8 cb8 = Callback8;
        MatrixS8 cr8 = mono_abi_callback_s8(cb8);
        CheckStruct("matrix: 8-byte callback", cr8, Callback8(new MatrixS8 { A = 0x11223344, B = 0x55667788 }));
        GC.KeepAlive(cb8);

        MatrixS9 s9 = mono_abi_matrix_s9(new MatrixS9 { A = 8, B = 9, C = 10 });
        CheckStruct("matrix: 9-byte struct argument and return", s9, new MatrixS9 { A = X32(8), B = X32(9), C = X8(10) });
        CallbackS9 cb9 = Callback9;
        MatrixS9 cr9 = mono_abi_callback_s9(cb9);
        CheckStruct("matrix: 9-byte callback", cr9, Callback9(new MatrixS9 { A = 0x11223344, B = 0x55667788, C = 0x99 }));
        GC.KeepAlive(cb9);

        MatrixS12 s12 = mono_abi_matrix_s12(new MatrixS12 { A = 9, B = 10, C = 11 });
        CheckStruct("matrix: 12-byte struct argument and return", s12, new MatrixS12 { A = X32(9), B = X32(10), C = X32(11) });
        CallbackS12 cb12 = Callback12;
        MatrixS12 cr12 = mono_abi_callback_s12(cb12);
        CheckStruct("matrix: 12-byte callback", cr12, Callback12(new MatrixS12 { A = 0x11223344, B = 0x55667788, C = 0x99AABBCC }));
        GC.KeepAlive(cb12);

        MatrixS16 s16 = mono_abi_matrix_s16(new MatrixS16 { A = 10, B = 11, C = 12, D = 13 });
        CheckStruct("matrix: 16-byte struct argument and return", s16, new MatrixS16 { A = X32(10), B = X32(11), C = X32(12), D = X32(13) });
        CallbackS16 cb16 = Callback16;
        MatrixS16 cr16 = mono_abi_callback_s16(cb16);
        CheckStruct("matrix: 16-byte callback", cr16, Callback16(new MatrixS16 { A = 0x11223344, B = 0x55667788, C = 0x99AABBCC, D = 0xDDEFF001 }));
        GC.KeepAlive(cb16);

        MatrixS24 s24 = mono_abi_matrix_s24(Make24());
        CheckStruct("matrix: 24-byte struct argument and return", s24, new MatrixS24 { A = X32(1), B = X32(2), C = X32(3), D = X32(4), E = X32(5), F = X32(6) });
        CallbackS24 cb24 = Callback24;
        MatrixS24 cr24 = mono_abi_callback_s24(cb24);
        CheckStruct("matrix: 24-byte callback", cr24, Callback24(Make24()));
        GC.KeepAlive(cb24);

        MatrixS48 s48 = mono_abi_matrix_s48(Make48());
        CheckStruct("matrix: 48-byte struct argument and return", s48, new MatrixS48 { A = X32(1), B = X32(2), C = X32(3), D = X32(4), E = X32(5), F = X32(6), G = X32(7), H = X32(8), I = X32(9), J = X32(10), K = X32(11), L = X32(12) });
        CallbackS48 cb48 = Callback48;
        MatrixS48 cr48 = mono_abi_callback_s48(cb48);
        CheckStruct("matrix: 48-byte callback", cr48, Callback48(Make48()));
        GC.KeepAlive(cb48);
    }

    private static void CheckCountsAndFloats()
    {
        Counts5Callback counts5 = Counts5;
        AbiCheck.I64("matrix: five ints plus 24-byte struct", mono_abi_counts5(1, 2, 3, 4, 5, Make24(), 99), 205);
        AbiCheck.I64("matrix: five-int row callback", mono_abi_callback_counts5(counts5), 205);
        GC.KeepAlive(counts5);

        Counts6Callback counts6 = Counts6;
        AbiCheck.I64("matrix: six ints plus 24-byte struct", mono_abi_counts6(1, 2, 3, 4, 5, 6, Make24(), 99), 211);
        AbiCheck.I64("matrix: six-int row callback", mono_abi_callback_counts6(counts6), 211);
        GC.KeepAlive(counts6);

        Counts7S9Callback counts7S9 = Counts7S9;
        AbiCheck.I64("matrix: seven ints plus 9-byte struct", mono_abi_counts7_s9(1, 2, 3, 4, 5, 6, 7, new MatrixS9 { A = 1, B = 2, C = 3 }, 99), 141);
        AbiCheck.I64("matrix: 9-byte row callback", mono_abi_callback_counts7_s9(counts7S9), 141);
        GC.KeepAlive(counts7S9);

        Counts7S12Callback counts7S12 = Counts7S12;
        AbiCheck.I64("matrix: seven ints plus 12-byte struct", mono_abi_counts7_s12(1, 2, 3, 4, 5, 6, 7, new MatrixS12 { A = 1, B = 2, C = 3 }, 99), 141);
        AbiCheck.I64("matrix: 12-byte row callback", mono_abi_callback_counts7_s12(counts7S12), 141);
        GC.KeepAlive(counts7S12);

        Counts7S16Callback counts7S16 = Counts7S16;
        AbiCheck.I64("matrix: seven ints plus 16-byte struct", mono_abi_counts7_s16(1, 2, 3, 4, 5, 6, 7, new MatrixS16 { A = 1, B = 2, C = 3, D = 4 }, 99), 157);
        AbiCheck.I64("matrix: 16-byte row callback", mono_abi_callback_counts7_s16(counts7S16), 157);
        GC.KeepAlive(counts7S16);

        Counts7S24Callback counts7S24 = Counts7S24;
        AbiCheck.I64("matrix: seven ints plus 24-byte struct", mono_abi_counts7_s24(1, 2, 3, 4, 5, 6, 7, Make24(), 99), 218);
        AbiCheck.I64("matrix: 24-byte row callback", mono_abi_callback_counts7_s24(counts7S24), 218);
        GC.KeepAlive(counts7S24);

        Counts3S48Callback counts3S48 = Counts3S48;
        AbiCheck.I64("matrix: three ints plus 48-byte struct", mono_abi_counts3_s48(1, 2, 3, Make48(), 99), 755);
        AbiCheck.I64("matrix: 48-byte row callback", mono_abi_callback_counts3_s48(counts3S48), 755);
        GC.KeepAlive(counts3S48);

        Counts9Callback counts9 = Counts9;
        AbiCheck.I64("matrix: nine ints plus 24-byte struct", mono_abi_counts9(1, 2, 3, 4, 5, 6, 7, 8, 9, Make24(), 99), 235);
        AbiCheck.I64("matrix: nine-int row callback", mono_abi_callback_counts9(counts9), 235);
        GC.KeepAlive(counts9);

        D15Callback d15 = D15;
        AbiCheck.F64("matrix: fifteen doubles", mono_abi_d15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), D15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        AbiCheck.F64("matrix: fifteen-double callback", mono_abi_callback_d15(d15), D15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        GC.KeepAlive(d15);

        F15Callback f15 = F15;
        AbiCheck.F32("matrix: fifteen floats", mono_abi_f15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15), F15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        AbiCheck.F32("matrix: fifteen-float callback", mono_abi_callback_f15(f15), F15(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15));
        GC.KeepAlive(f15);
    }

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate void UnwindCallback();

    [DllImport("__Internal")]
    private static extern long mono_abi_unwind64(UnwindCallback callback);

    private static void ThrowCatch()
    {
        ThrowLevel1();
    }

    private static void ThrowLevel1()
    {
        ThrowLevel2();
    }

    private static void ThrowLevel2()
    {
        try
        {
            ThrowLevel3();
        }
        catch (Exception)
        {
        }
    }

    private static void ThrowLevel3()
    {
        throw new Exception("mono_abi unwind test");
    }

    private static void CheckUnwind()
    {
        UnwindCallback callback = ThrowCatch;
        AbiCheck.I64("unwind: all 18 nonvolatile 64-bit registers preserved", mono_abi_unwind64(callback), 0);
        GC.KeepAlive(callback);
    }

    internal static void Run()
    {
        CheckStructs();
        CheckCountsAndFloats();
        CheckUnwind();
    }
}
