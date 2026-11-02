using System;

internal static class Hello
{
    private static int Main()
    {
        const string expected = "mono_hello: managed JIT ran";
#if BROKEN
        string message = "mono_hello: intentionally wrong message";
#else
        string message = expected;
#endif
        Console.WriteLine(message);
        Native.Check("hello: Console.WriteLine ran on the JIT", message == expected);
        return 0;
    }
}
