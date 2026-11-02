using System;
using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
internal struct Small1
{
    public byte Value;
}

[StructLayout(LayoutKind.Sequential)]
internal struct Small8
{
    public int First;
    public int Second;
}

[StructLayout(LayoutKind.Sequential)]
internal struct Large24
{
    public int A;
    public int B;
    public int C;
    public int D;
    public int E;
    public int F;
}

internal static class Abi
{
    private const long I64Left = 0x0102030405060708;
    private const long I64Right = -0x102030405;

    [DllImport("__Internal")] private static extern int mono_abi_i32(int left, int right);
    [DllImport("__Internal")] private static extern long mono_abi_i64(long left, long right);
    [DllImport("__Internal")] private static extern float mono_abi_f32(float left, float right);
    [DllImport("__Internal")] private static extern double mono_abi_f64(double left, double right);
    [DllImport("__Internal")] private static extern Small1 mono_abi_small1(Small1 input);
    [DllImport("__Internal")] private static extern Small8 mono_abi_small8(Small8 input);
    [DllImport("__Internal")] private static extern Large24 mono_abi_large24(Large24 input);
    [DllImport("__Internal")] private static extern long mono_abi_mixed(
        int a, int b, int c, int d, int e, int f, int g, Large24 input, int after, double scale);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    private delegate long TestCallback(Small8 small, Large24 large, double value);

    [DllImport("__Internal")] private static extern long mono_abi_callback(TestCallback callback);

    private static Large24 MakeLarge()
    {
        return new Large24 { A = 2, B = 3, C = 5, D = 7, E = 11, F = 13 };
    }

    private static long Callback(Small8 small, Large24 large, double value)
    {
        bool valid = small.First == -123456 && small.Second == 0x01020304 && value == 2.5;
        valid &= large.A == 2 && large.B == 3 && large.C == 5;
        valid &= large.D == 7 && large.E == 11 && large.F == 13;
        if (!valid)
        {
            AbiCheck.LogFields("abi: callback small input", new uint[] { (uint)small.First, (uint)small.Second }, new uint[] { unchecked((uint)-123456), 0x01020304 });
            AbiCheck.LogFields("abi: callback large input", new uint[] { (uint)large.A, (uint)large.B, (uint)large.C, (uint)large.D, (uint)large.E, (uint)large.F }, new uint[] { 2, 3, 5, 7, 11, 13 });
            AbiCheck.F64("abi: callback double input", value, 2.5);
            return -777;
        }
        return small.First + small.Second + large.A * 1 + large.B * 2 + large.C * 3 +
            large.D * 4 + large.E * 5 + large.F * 6 + (long)value;
    }

    private static int Main()
    {
        AbiCheck.I32("abi: int32 both directions", mono_abi_i32(-17, 41), -10);
        AbiCheck.I64("abi: int64 both directions", mono_abi_i64(I64Left, I64Right), I64Left - I64Right);
        long multiplyLeft = mono_abi_i64(I64Left, 0);
        long multiplyRight = mono_abi_i64(I64Right, 0);
        const long multiplyExpected = unchecked((long)0xdccdbeafa9adbcd8UL);
        AbiCheck.I64("abi: non-constant int64 multiplication", multiplyLeft * multiplyRight, multiplyExpected);
        AbiCheck.F32("abi: float both directions", mono_abi_f32(1.5f, -0.25f), 2.75f);
        AbiCheck.F64("abi: double both directions", mono_abi_f64(-3.5, 0.125), -1.625);

        Small1 small1 = mono_abi_small1(new Small1 { Value = 0xA7 });
        AbiCheck.Fields("abi: one-byte struct argument and return", new uint[] { small1.Value }, new uint[] { 0x58 });

        Small8 small8 = mono_abi_small8(new Small8 { First = -123456, Second = 0x01020304 });
        AbiCheck.Fields("abi: eight-byte struct argument and return",
            new uint[] { (uint)small8.First, (uint)small8.Second }, new uint[] { unchecked((uint)-123449), 0x01020304 ^ 0x05555555 });

        Large24 large = mono_abi_large24(MakeLarge());
        AbiCheck.Fields("abi: 24-byte struct argument and return",
            new uint[] { (uint)large.A, (uint)large.B, (uint)large.C, (uint)large.D, (uint)large.E, (uint)large.F }, new uint[] { 13, 25, 38, 51, 66, 79 });

        long mixed = mono_abi_mixed(1, 2, 3, 4, 5, 6, 7, MakeLarge(), 99, 2.0);
        const long mixedExpected = 1 + 2 + 3 + 4 + 5 + 6 + 7 + 99 + 2 +
            2 * 1 + 3 * 2 + 5 * 3 + 7 * 4 + 11 * 5 + 13 * 6;
        AbiCheck.I64("abi: split struct and independent stack count", mixed, mixedExpected);

        TestCallback callback = Callback;
        long callbackResult = mono_abi_callback(callback);
        const long callbackExpected = -123456 + 0x01020304 + 2 * 1 + 3 * 2 + 5 * 3 +
            7 * 4 + 11 * 5 + 13 * 6 + 2;
        AbiCheck.I64("abi: native to managed callback", callbackResult, callbackExpected);
        GC.KeepAlive(callback);
        AbiMatrix.Run();

#if BROKEN
        AbiCheck.I32("abi: intentional broken expectation", mono_abi_i32(-17, 41), 10);
#endif
        return 0;
    }
}
