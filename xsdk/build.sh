#!/bin/bash
# Builds Mono for xsdk from a clone of mono-xenon: first a Mono for the PC, whose C# compiler builds the class
# libraries, then the runtime for the console. Needs xsdk, built (make in its checkout), and its GCC
# (tools/toolchain/build.sh there); runs on Linux or in WSL.
#
#   XSDK=<xsdk checkout> xsdk/build.sh
#
# Writes build-xsdk/ (OUT for another folder): lib/libmonosgen-2.0.a and include/mono-2.0/ for titles; managed/, the
# class libraries a title puts beside its .xex; and host/, the PC's Mono, which xsdk/tests' C# is compiled with.
# JOBS sets make's parallelism. Leaves the checkout configured for the console, so make there rebuilds the runtime.
set -euo pipefail

root=$(cd "$(dirname "$0")/.." && pwd)
xsdk=$(cd "${XSDK:?set XSDK to an xsdk checkout, built}" && pwd)
gcc=${XSDK_GCC:-$HOME/xsdk-gcc}/bin
out=$(mkdir -p "${OUT:-$root/build-xsdk}" && cd "${OUT:-$root/build-xsdk}" && pwd)
jobs=${JOBS:-$(nproc)}

# What the runtime and the class libraries use. autogen.sh would take every submodule, llvm-project's too.
submodules=(
	external/Newtonsoft.Json external/api-doc-tools external/aspnetwebstack external/binary-reference-assemblies
	external/cecil external/cecil-legacy external/corefx external/corert external/ikdasm external/ikvm
	external/linker external/roslyn-binaries external/rx
)
git -C "$root" submodule update --init "${submodules[@]}"

cd "$root"
# autogen.sh's own first step, done here as autogen.sh can't run without every submodule.
cat mono/mini/Makefile.am.in > mono/mini/Makefile.am
NOCONFIGURE=1 autoreconf -i
if [ -f Makefile ]; then
	make distclean > /dev/null
fi

# The PC's Mono and the class libraries, bootstrapped from monolite, a prebuilt compiler and corlib.
./configure --prefix="$out/host" --disable-boehm --disable-btls --with-sigaltstack=no --enable-nls=no
make get-monolite-latest
make -j"$jobs"
make install
make distclean > /dev/null

# The runtime for the console: the static library alone (configure.ac, host_xsdk).
./configure --host=powerpc-xsdk-elf --prefix="$out" \
	--disable-boehm --disable-btls --enable-cooperative-suspend --with-sigaltstack=no --with-tls=pthread \
	--with-sgen-default-concurrent=no --disable-shared --enable-static --disable-crash-reporting \
	--disable-executables --disable-mcs-build \
	CC="$gcc/powerpc-xsdk-elf-gcc" CXX="$gcc/powerpc-xsdk-elf-g++" \
	CFLAGS="-mcall-xsdk -O2 -fno-pic -mstrict-align -I$xsdk/include" \
	CXXFLAGS="-mcall-xsdk -O2 -fno-pic -mstrict-align -I$xsdk/include" \
	LDFLAGS="-static -nostdlib -T$xsdk/src/xsdk.ld" \
	LIBS="-Wl,--start-group -lstdc++ $xsdk/build/libxsdk.a -lc -lm -lgcc -Wl,--end-group"
make -j"$jobs"
make install

rm -rf "$out/managed"
mkdir -p "$out/managed/Facades"
cp "$out/host/lib/mono/4.5/"*.dll "$out/managed/"
cp "$out/host/lib/mono/4.5/Facades/"*.dll "$out/managed/Facades/"
echo "mono-xenon: $out/lib/libmonosgen-2.0.a, $out/include/mono-2.0, $out/managed"
