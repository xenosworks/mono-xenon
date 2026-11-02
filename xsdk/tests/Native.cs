using System.Runtime.InteropServices;

internal static class Native
{
    [DllImport("__Internal", EntryPoint = "mono_test_check")]
    internal static extern void Check(string name, bool passed);

    [DllImport("__Internal", EntryPoint = "mono_test_log")]
    internal static extern void Log(string message);
}
