using System;
using System.Diagnostics;
using System.Runtime.InteropServices;
using System.Threading;

internal static class GcTest
{
    private const int WorkerCount = 3;
    private const int AllocationsPerWorker = 4000;
    private const int AllocationBytes = 128;

    private static int allocated;
    private static int errors;
    private static int nativeResult;

    [DllImport("__Internal")] private static extern int mono_gc_native_wait();
    [DllImport("__Internal")] private static extern int mono_gc_native_state();
    [DllImport("__Internal")] private static extern void mono_gc_native_release();
    [DllImport("__Internal")] private static extern void mono_gc_pause(int index, long microseconds);
    [DllImport("__Internal")] private static extern long mono_gc_now_us();

    private static long Microseconds(Stopwatch watch)
    {
        return watch.ElapsedTicks * 1000000 / Stopwatch.Frequency;
    }

    private static void Allocate()
    {
        try
        {
            for (int i = 0; i < AllocationsPerWorker; ++i)
            {
                byte[] value = new byte[AllocationBytes];
                value[0] = (byte)i;
                Interlocked.Add(ref allocated, value.Length);
            }
        }
        catch
        {
            Interlocked.Increment(ref errors);
        }
    }

    private static void NativeWait()
    {
        try
        {
            nativeResult = mono_gc_native_wait();
        }
        catch
        {
            Interlocked.Increment(ref errors);
        }
    }

    private static int Main()
    {
        allocated = 0;
        errors = 0;
        nativeResult = 0;
        Thread[] workers = new Thread[WorkerCount];
        for (int i = 0; i < workers.Length; ++i)
        {
            workers[i] = new Thread(Allocate);
            workers[i].Start();
        }
        Thread nativeThread = new Thread(NativeWait);
        nativeThread.Start();

        int initialCollections = GC.CollectionCount(GC.MaxGeneration);
        Native.Log("mono_gc: initial collection count " + initialCollections);
        for (int i = 0; i < 2000 && (mono_gc_native_state() & 1) == 0; ++i)
            Thread.Sleep(1);
        Native.Check("gc: thread entered long native call", (mono_gc_native_state() & 1) != 0);

        int collectionsAfterLoop = initialCollections;
        bool nativeStayedBlocked = true;
        for (int i = 0; i < 12; ++i)
        {
            Stopwatch watch = Stopwatch.StartNew();
            GC.Collect();
            watch.Stop();
            collectionsAfterLoop = GC.CollectionCount(GC.MaxGeneration);
            nativeStayedBlocked &= (mono_gc_native_state() & 2) == 0;
            mono_gc_pause(i, Microseconds(watch));
            Native.Log("mono_gc: collection " + i + " count " + collectionsAfterLoop + " native state " + mono_gc_native_state());
        }
        mono_gc_native_release();

        // The pauses above are only as good as Stopwatch; it has to keep the native monotonic clock's time.
        long nativeStart = mono_gc_now_us();
        Stopwatch clock = Stopwatch.StartNew();
        Thread.Sleep(50);
        clock.Stop();
        long nativeMicroseconds = mono_gc_now_us() - nativeStart;
        long managedMicroseconds = Microseconds(clock);
        Native.Log("gc: 50 ms sleep: Stopwatch " + managedMicroseconds + " us, native clock " + nativeMicroseconds + " us");
        Native.Check("gc: Stopwatch keeps the native monotonic clock's time",
            managedMicroseconds >= 50000 && nativeMicroseconds - managedMicroseconds >= 0 &&
            nativeMicroseconds - managedMicroseconds <= 500);

        for (int i = 0; i < workers.Length; ++i)
            workers[i].Join();
        nativeThread.Join();

        int expected = WorkerCount * AllocationsPerWorker * AllocationBytes;
#if BROKEN
        expected += 1;
#endif
        Native.Check("gc: all managed allocations completed", allocated == expected);
        if (collectionsAfterLoop < initialCollections + 12 || !nativeStayedBlocked)
            Native.Log("gc: collections expected>=" + (initialCollections + 12) + " actual=" + collectionsAfterLoop + " nativeStayedBlocked=" + nativeStayedBlocked);
        Native.Check("gc: collections ran during native call", collectionsAfterLoop >= initialCollections + 12 && nativeStayedBlocked);
        Native.Check("gc: native call returned correctly", nativeResult == 0x1234);
        Native.Check("gc: no worker thread failed", errors == 0);
        return 0;
    }
}
