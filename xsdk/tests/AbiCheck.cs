using System;
using System.Globalization;

internal static class AbiCheck
{
    private static string Hex32(uint value)
    {
        return "0x" + value.ToString("x8", CultureInfo.InvariantCulture);
    }

    internal static void Bool(string name, bool actual, bool expected)
    {
        if (actual != expected)
            Native.Log(name + ": expected=" + expected + " actual=" + actual);
        Native.Check(name, actual == expected);
    }

    internal static void I32(string name, int actual, int expected)
    {
        if (actual != expected)
            Native.Log(name + ": expected=" + expected + " (" + Hex32(unchecked((uint)expected)) + ") actual=" + actual + " (" + Hex32(unchecked((uint)actual)) + ")");
        Native.Check(name, actual == expected);
    }

    internal static void I64(string name, long actual, long expected)
    {
        if (actual != expected)
        {
            uint actualHigh = (uint)((ulong)actual >> 32);
            uint actualLow = (uint)actual;
            uint expectedHigh = (uint)((ulong)expected >> 32);
            uint expectedLow = (uint)expected;
            Native.Log(name + ": expected.hi=" + Hex32(expectedHigh) + " actual.hi=" + Hex32(actualHigh));
            Native.Log(name + ": expected.lo=" + Hex32(expectedLow) + " actual.lo=" + Hex32(actualLow));
        }
        Native.Check(name, actual == expected);
    }

    internal static void F32(string name, float actual, float expected)
    {
        if (actual != expected)
            Native.Log(name + ": expected=" + expected.ToString("R", CultureInfo.InvariantCulture) + " actual=" + actual.ToString("R", CultureInfo.InvariantCulture));
        Native.Check(name, actual == expected);
    }

    internal static void F64(string name, double actual, double expected)
    {
        if (actual != expected)
            Native.Log(name + ": expected=" + expected.ToString("R", CultureInfo.InvariantCulture) + " actual=" + actual.ToString("R", CultureInfo.InvariantCulture));
        Native.Check(name, actual == expected);
    }

    internal static void LogFields(string name, uint[] actual, uint[] expected)
    {
        Native.Log(name + ": expected fields=" + expected.Length + " actual fields=" + actual.Length);
        int count = Math.Max(actual.Length, expected.Length);
        for (int i = 0; i < count; ++i)
        {
            string expectedText = i < expected.Length ? Hex32(expected[i]) : "<missing>";
            string actualText = i < actual.Length ? Hex32(actual[i]) : "<missing>";
            Native.Log(name + ": field[" + i + "] expected=" + expectedText + " actual=" + actualText);
        }
    }

    internal static void Fields(string name, uint[] actual, uint[] expected)
    {
        bool passed = actual.Length == expected.Length;
        if (passed)
        {
            for (int i = 0; i < actual.Length; ++i)
                passed &= actual[i] == expected[i];
        }
        if (!passed)
            LogFields(name, actual, expected);
        Native.Check(name, passed);
    }
}
