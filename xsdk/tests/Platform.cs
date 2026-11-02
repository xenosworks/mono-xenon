using System;
using System.IO;
using System.Net.Sockets;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

// What a title leans on beyond the JIT: threads, finalizers, files through game:\, managed varargs, an assembly whose
// file name FATX can't hold, and sockets through Mono's I/O thread pool.
internal static class Platform
{
    private const int Finalizables = 100;
    private const int WaitMs = 5000;

    private static int finalized;

    private sealed class Finalizable
    {
        ~Finalizable()
        {
            Interlocked.Increment(ref finalized);
        }
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void MakeGarbage()
    {
        for (int i = 0; i < Finalizables; ++i)
            new Finalizable();
    }

    private static int Sum(__arglist)
    {
        ArgIterator arguments = new ArgIterator(__arglist);
        int sum = 0;
        while (arguments.GetRemainingCount() > 0)
            sum += __refvalue(arguments.GetNextArg(), int);
        return sum;
    }

    private static void Threads()
    {
        object gate = new object();
        bool ready = false;
        int handedOver = 0;
        Thread waiter = new Thread(() =>
        {
            lock (gate)
            {
                while (!ready)
                    Monitor.Wait(gate);
                handedOver = 42;
            }
        });
        waiter.Start();
        lock (gate)
        {
            ready = true;
            Monitor.PulseAll(gate);
        }
        bool joined = waiter.Join(WaitMs);

        ManualResetEvent done = new ManualResetEvent(false);
        int pooled = 0;
        ThreadPool.QueueUserWorkItem(state => { pooled = 7; done.Set(); });
        bool poolRan = done.WaitOne(WaitMs) && pooled == 7;
        int tasked = Task.Run(() => 6 * 7).Result;
        Native.Log("platform: thread " + handedOver + ", pool " + pooled + ", task " + tasked);
        Native.Check("platform: a thread, the thread pool and a task", joined && handedOver == 42 && poolRan && tasked == 42);
    }

    private static void Finalizers()
    {
        MakeGarbage();
        GC.Collect();
        GC.WaitForPendingFinalizers();
        Native.Log("platform: " + finalized + " of " + Finalizables + " finalized");
        // SGen scans stacks conservatively, so a stale slot can keep one or two alive past the collection.
        Native.Check("platform: finalizers run", finalized >= Finalizables - 5);
    }

    private static void Files()
    {
        string folder = "game:\\platform_check";
        string path = Path.Combine(folder, "file.txt");
        DateTime written = new DateTime(2004, 6, 8, 12, 30, 0, DateTimeKind.Utc);
        if (Directory.Exists(folder))
            Directory.Delete(folder, true);
        Directory.CreateDirectory(folder);
        File.WriteAllText(path, "xsdk");
        string read = File.ReadAllText(path);
        File.SetLastWriteTimeUtc(path, written);
        DateTime readBack = File.GetLastWriteTimeUtc(path);
        File.Delete(path);
        Directory.Delete(folder);
        Native.Log("platform: read \"" + read + "\", time " + readBack.ToString("o"));
        Native.Check("platform: a file written, read and removed through game:\\", read == "xsdk" && !Directory.Exists(folder));
        Native.Check("platform: a file's time set and read back", readBack == written);
    }

    private static void Varargs()
    {
        int sum = Sum(__arglist(2, 3, 4));
#if BROKEN
        sum += 1;
#endif
        Native.Check("platform: managed varargs", sum == 9);
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static int CallLongNamed()
    {
        return LongNamedLibrary.Value();
    }

    private static void LongName()
    {
        // long_name.txt holds the name xsdk/short_names.py gave the library; copying it there here, before the JIT
        // first resolves LongNamedLibrary, also shows FATX takes a 42-character name.
        string shortName = File.ReadAllText("game:\\long_name.txt").Trim();
        File.Copy("game:\\long_named.dll", "game:\\" + shortName, true);
        int value = CallLongNamed();
        File.Delete("game:\\" + shortName);
        Native.Check("platform: an assembly whose file name is past FATX's 42 characters", value == 0x42);
    }

    private static void Sockets()
    {
        string[] host = File.ReadAllText("game:\\net_host.txt").Trim().Split(' ');
        byte[] sent = Encoding.ASCII.GetBytes("mono async\n");
        byte[] received = new byte[sent.Length];
        int count = 0;
        using (TcpClient client = new TcpClient())
        {
            // ConnectAsync, WriteAsync and ReadAsync complete through Mono's I/O selector thread.
            bool connected = client.ConnectAsync(host[0], int.Parse(host[1])).Wait(WaitMs);
            if (connected)
            {
                NetworkStream stream = client.GetStream();
                stream.WriteAsync(sent, 0, sent.Length).Wait(WaitMs);
                while (count < received.Length)
                {
                    Task<int> read = stream.ReadAsync(received, count, received.Length - count);
                    if (!read.Wait(WaitMs) || read.Result == 0)
                        break;
                    count += read.Result;
                }
            }
            Native.Log("platform: connected " + connected + ", echoed " + count + " bytes");
        }
        Native.Check("platform: async sockets through the I/O thread pool", count == sent.Length &&
            Encoding.ASCII.GetString(received) == Encoding.ASCII.GetString(sent));
    }

    private static int Main()
    {
        Threads();
        Finalizers();
        Files();
        Varargs();
        LongName();
        Sockets();
        return 0;
    }
}
