# Xbox 360 (xsdk)

Mono for the Xbox 360 through [xsdk](https://github.com/xenosworks/xsdk), a homebrew SDK that builds titles for the
console's retail kernel. Mono's JIT runs C# on the console: the runtime is a static library a title links, and the
class libraries go beside its `.xex`. This is an altered version of Mono 6.13.0; the changes are on the `xenon` branch.

## Building

Build xsdk first (`make` in its checkout builds `libxsdk.a`) with its GCC (`tools/toolchain/build.sh` there), then, on
Linux or in WSL:

```bash
XSDK=<xsdk> xsdk/build.sh
```

It builds a Mono for the PC, whose C# compiler builds the class libraries, then the runtime for the console, into
`build-xsdk/`:

-   `lib/libmonosgen-2.0.a` and `include/mono-2.0/`, which a title links and includes;
-   `managed/`, the class libraries (.NET 4.5's profile) with their `Facades/`, which go beside the title's `.xex`;
-   `host/`, the PC's Mono, whose `csc` compiles a title's C#.

`OUT` names another folder and `JOBS` make's parallelism. The checkout is left configured for the console, so `make`
there rebuilds the runtime after a change.

## A title

```c
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

int main(void)
{
	char *argv[] = { "Game" };
	mono_set_dirs("game:\\", "game:\\");
	mono_set_assemblies_path("game:\\");
	MonoDomain *domain = mono_jit_init_version("Game", "v4.0.30319");
	MonoAssembly *assembly = mono_assembly_open("game:\\Game.exe", NULL);
	const int status = assembly ? mono_jit_exec(domain, assembly, 1, argv) : 1;
	mono_jit_cleanup(domain);
	return status;
}
```

It links `libmonosgen-2.0.a` and turns its ELF into a `.xex` with xsdk's `xsdk_add_xex()`, as `xsdk/tests` does. Beside
the `.xex` go `build-xsdk/managed`'s files and the title's own assemblies. Twenty of the class libraries' names are
past the 42 characters FATX holds, so `xsdk/short_names.py build-xsdk/managed` and
`xsdk/short_names.py build-xsdk/managed/Facades` rename them first (below).

## P/Invoke

There's no `dlopen`, so `[DllImport]` finds a function in one of three places:

-   `__Internal`, or any module name a title gives one: `mono_xsdk_pinvoke_register(module, name, address)` from
    `<mono/utils/mono-xsdk.h>` registers it before managed code calls it.
-   System.Native, which the class libraries' corefx code calls, under any name Mono's dllmap gives it: it's built into
    the runtime, on xsdk's POSIX calls (`mono/native`, `xsdk/native_table.py`).
-   `libc`: `uname`, `getifaddrs` and `freeifaddrs`, which System asks before it makes a socket.

Any other module is a `DllNotFoundException`, and a function a module doesn't have an `EntryPointNotFoundException`.
Calls take xsdk's native ABI (`-mcall-xsdk`). A C function with variable arguments can't be called; managed
`__arglist` works.

## The JIT's pool

The JIT writes its code into the title's `.jit` pool (xsdk's `docs/GUIDE.md`). Mono brings 4 MB of it; a title that runs
more code adds a section of its own, which the linker lays beside Mono's:

```asm
.section .jit,"awx",@nobits
.balign 0x10000
.skip 0x1000000
```

Bytes taken from the pool stay Mono's, which reuses the code chunks it frees. `mono_xsdk_jit_used()` gives the bytes
taken; when the pool is full, Mono logs `xsdk: the title's .jit pool of ... bytes has no room for ... more` and can't
go on, so a title sizes the pool for the code it runs.

## Notes

-   The current directory is `game:\`, which managed code sees as `/game:/`: the class libraries' Unix path rules take
    only a leading `/` as rooted, and xsdk reads `/game:/` as `game:\`. Paths in either form work.
-   FATX keeps names of up to 42 characters. An assembly loaded by a longer name is looked up under a short one: its
    start, `~`, 8 hex digits of a hash, then the extension. `xsdk/short_names.py <folder>` renames a folder's files to
    those names before they go to the console.
-   `TimeZoneInfo.Local` is UTC: Mono looks for a zone file named by `TZ`, which xsdk sets to the console's zone as a
    POSIX rule, and the console has no zone files. C's `localtime` keeps the console's zone.
-   Threads stop for the collector cooperatively (`--enable-cooperative-suspend`); a thread in native code counts as
    stopped. The console has no signals, so aborting or interrupting a thread that is inside a native call takes
    effect when the call returns.
-   Asynchronous socket calls go through the I/O thread pool, whose `poll` returns every 10 ms to take sockets added
    and removed, as there's no pipe to wake it; one added while it waits can wait that long to start.
-   No processes, pipes, other users or host name: `Process.Start` and the like fail as POSIX calls do without them.
    `flock` fails, so `FileStream` takes no lock; Mono keeps the sharing modes between a title's own streams.
-   Not built: AOT, LLVM, the Boehm collector, BTLS (so no TLS in the class libraries), crash reporting and the
    profilers.

## The checks

`xsdk/tests` builds titles against `build-xsdk/`:

```bash
cmake -S xsdk/tests -B build-tests -DCMAKE_TOOLCHAIN_FILE=<xsdk>/cmake/xsdk.cmake
cmake --build build-tests
```

-   `mono_hello`, `mono_abi` (native calls and callbacks across the ABI's argument and return types), `mono_gc` and
    `mono_platform` (threads, the thread pool, finalizers, files and their times through `game:\`, managed varargs, an
    assembly named past 42 characters, and sockets through the I/O thread pool). Each runs the assembly of its own
    name, built beside it.
-   Each has a `_broken` variant that fails one check on purpose: `hello: Console.WriteLine ran on the JIT`, `abi:
    intentional broken expectation`, `gc: all managed allocations completed` and `platform: managed varargs`.
-   `mono_regress` runs `mono/mini`'s regression tests (its `Makefile.am`'s `regtests`) as `mono --regression` does,
    every optimisation set; a `regress_args.txt` beside it, one argument a line, replaces the list.

Beside them go `build-xsdk/managed`'s files and what the build puts beside each title; `mono_platform` copies
`long_named.dll` to the name in `long_name.txt` itself. It also needs `net_host.txt`, holding the PC's address and the
port of xsdk's `tools/net_echo.py`, running there, as `192.168.0.125 7370`. Each logs `PASS` or `FAIL` for each check, then
`<name>: <passed> passed, <failed> failed`.

## Measured on the console

From the checks a fresh clone builds, on 2026-11-02:

-   **Memory:** Mono adds 4.7 MB of code and read-only data to a title, which loads 5.1 MB of them against 0.4 MB for
    an xsdk title without it, and 4 MB of `.jit` pool: an image of 9.3 MB. From the runtime's start to the end of
    `Main`, `mono_hello` took 20.0 MB of free memory, `mono_gc` 22.2 MB and `mono_platform` 29.5 MB: the collector's
    heap, and the class libraries opened, read into memory as there's no `mmap` of files.
-   **Start-up:** `mono_jit_init_version` takes 264 to 296 ms, opening a title's assembly 2 to 8 ms, and `mono_hello`'s
    `Main`, compiled and run, 156 to 159 ms: about 0.45 s from the runtime's start to the end of a `Main` that prints a
    line.
-   **The JIT:** `mono_regress` runs the 28,686 regression tests in all 23 optimisation sets in 43 s, taking 35 MB of
    the pool. Over the 380 sets it and the PC's Mono both run (a Ryzen 9 9950X3D; `builtin-types` is 32-bit only),
    compiling the test methods takes 9.3 s against 0.57 s, 16 times as long, about 1.2 MB of code a second, and running
    them, compiling what they call as they go, 30.4 s against 1.44 s, 21 times.
