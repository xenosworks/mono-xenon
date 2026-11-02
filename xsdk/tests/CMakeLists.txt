cmake_minimum_required(VERSION 3.25)
project(mono_xenon_checks C ASM)

# xsdk/build.sh's output: the runtime and its headers, the class libraries, and the PC's Mono, whose csc and ilasm
# build the checks' C# and IL.
set(MONO_XENON_BUILD "${CMAKE_CURRENT_LIST_DIR}/../../build-xsdk" CACHE PATH "xsdk/build.sh's output folder")
set(MONO_MINI "${CMAKE_CURRENT_LIST_DIR}/../../mono/mini")
set(MANAGED "${MONO_XENON_BUILD}/managed")
set(CSC "${MONO_XENON_BUILD}/host/bin/csc")
set(ILASM "${MONO_XENON_BUILD}/host/bin/ilasm")
set(CSC_FLAGS /nologo /noconfig /nostdlib+ /optimize+ /debug- /unsafe /langversion:8.0
    /nowarn:0162,0169,0219,0414,0618,0649
    /reference:${MANAGED}/mscorlib.dll /reference:${MANAGED}/System.dll /reference:${MANAGED}/System.Core.dll)

add_library(mono_check_runtime STATIC mono_test.c)
target_include_directories(mono_check_runtime PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${MONO_XENON_BUILD}/include/mono-2.0")
target_compile_options(mono_check_runtime PUBLIC -mstrict-align -Wall -Wextra -Werror)
set_target_properties(mono_check_runtime PROPERTIES C_STANDARD 17 C_EXTENSIONS ON)
target_link_libraries(mono_check_runtime PUBLIC "${MONO_XENON_BUILD}/lib/libmonosgen-2.0.a")

set(OUT "${CMAKE_CURRENT_BINARY_DIR}")

# csharp(<output> <target or library> <source>... [OPTIONS <csc option>...]): C# built beside the titles, its sources
# taken from beside this file and the assemblies it references built first.
function(csharp output kind)
    cmake_parse_arguments(PARSE_ARGV 2 ARG "" "" "OPTIONS")
    set(references ${ARG_OPTIONS})
    list(FILTER references INCLUDE REGEX "^/reference:")
    list(TRANSFORM references REPLACE "^/reference:" "")
    add_custom_command(OUTPUT "${OUT}/${output}"
        COMMAND "${CSC}" ${CSC_FLAGS} /target:${kind} "/out:${OUT}/${output}" ${ARG_OPTIONS} ${ARG_UNPARSED_ARGUMENTS}
        DEPENDS ${ARG_UNPARSED_ARGUMENTS} ${references}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        VERBATIM)
endfunction()

# Platform.cs loads it by assembly name, past the 42 characters FATX keeps; the title copies long_named.dll to the
# name xsdk/short_names.py gives it, which long_name.txt holds.
set(LONG_NAMED Xsdk.Check.AssemblyNamedPastFatxFortyTwoCharacters)
csharp(${LONG_NAMED}.dll library LongNamedLibrary.cs)
add_custom_command(OUTPUT "${OUT}/long_named.dll" "${OUT}/long_name.txt"
    COMMAND "${CMAKE_COMMAND}" -E copy ${LONG_NAMED}.dll long_named.dll
    COMMAND python3 -c "import importlib.util as u; s = u.spec_from_file_location('s', '${CMAKE_CURRENT_LIST_DIR}/../short_names.py'); m = u.module_from_spec(s); s.loader.exec_module(m); open('long_name.txt', 'w').write(m.short_name('${LONG_NAMED}.dll') + '\\n')"
    DEPENDS "${OUT}/${LONG_NAMED}.dll" "${CMAKE_CURRENT_LIST_DIR}/../short_names.py"
    VERBATIM)

# check(<name> <C sources> <C# sources> [BROKEN]): a title and the managed program it runs, named alike.
function(check name native managed)
    set(options)
    if(ARGN STREQUAL "BROKEN")
        set(options /define:BROKEN)
    endif()
    if(name MATCHES "^mono_platform")
        list(APPEND options "/reference:${OUT}/${LONG_NAMED}.dll")
        set(extra "${OUT}/long_named.dll" "${OUT}/long_name.txt")
    endif()
    csharp(${name}.exe exe Native.cs ${managed} OPTIONS ${options})
    add_executable(${name} mono_title.c ${native} "${OUT}/${name}.exe" ${extra})
    set_target_properties(${name} PROPERTIES C_STANDARD 17 C_EXTENSIONS ON)
    target_compile_definitions(${name} PRIVATE MONO_TEST_NAME="${name}")
    target_link_libraries(${name} PRIVATE mono_check_runtime)
    xsdk_add_xex(${name})
endfunction()

check(mono_hello "" Hello.cs)
check(mono_hello_broken "" Hello.cs BROKEN)
check(mono_abi "native_abi.c;native_abi_matrix.c;native_abi_unwind.S" "Abi.cs;AbiCheck.cs;AbiMatrix.cs")
check(mono_abi_broken "native_abi.c;native_abi_matrix.c;native_abi_unwind.S" "Abi.cs;AbiCheck.cs;AbiMatrix.cs" BROKEN)
check(mono_gc native_gc.c Gc.cs)
check(mono_gc_broken native_gc.c Gc.cs BROKEN)
check(mono_platform "" Platform.cs)
check(mono_platform_broken "" Platform.cs BROKEN)

# mono/mini's regression tests (its Makefile.am's regtests), which mono_regress runs as rcheck does.
set(REGRESSION_TESTS aot-tests basic basic-float basic-long basic-calls gshared objects arrays basic-math exceptions
    devirtualization ratests)
set(DRIVER "/reference:${OUT}/TestDriver.dll")
csharp(TestDriver.dll library ${MONO_MINI}/TestDriver.cs ${MONO_MINI}/TestHelpers.cs)
set(regression_outputs "${OUT}/TestDriver.dll")
foreach(test ${REGRESSION_TESTS})
    csharp(${test}.exe exe ${MONO_MINI}/${test}.cs OPTIONS ${DRIVER})
    list(APPEND regression_outputs "${OUT}/${test}.exe")
endforeach()
csharp(builtin-types.exe exe ${MONO_MINI}/builtin-types.cs OPTIONS /define:ARCH_32 ${DRIVER})
foreach(il iltests.exe:exe generics-variant-types.dll:dll MemoryIntrinsics.dll:dll)
    string(REPLACE ":" ";" parts ${il})
    list(GET parts 0 output)
    list(GET parts 1 kind)
    get_filename_component(stem ${output} NAME_WE)
    set(dll_flag)
    if(kind STREQUAL "dll")
        set(dll_flag -dll)
    endif()
    add_custom_command(OUTPUT "${OUT}/${output}"
        COMMAND "${ILASM}" ${dll_flag} "-output=${OUT}/${output}" ${MONO_MINI}/${stem}.il
        DEPENDS ${MONO_MINI}/${stem}.il
        VERBATIM)
    list(APPEND regression_outputs "${OUT}/${output}")
endforeach()
csharp(generics.exe exe ${MONO_MINI}/generics.cs OPTIONS ${DRIVER} "/reference:${OUT}/generics-variant-types.dll")
csharp(basic-simd.exe exe ${MONO_MINI}/basic-simd.cs OPTIONS ${DRIVER} /reference:${MANAGED}/Mono.Simd.dll)
csharp(unaligned.exe exe ${MONO_MINI}/unaligned.cs OPTIONS ${DRIVER} "/reference:${OUT}/MemoryIntrinsics.dll")
csharp(basic-vectors.exe exe ${MONO_MINI}/basic-vectors.cs OPTIONS ${DRIVER} /reference:${MANAGED}/System.Numerics.dll
    /reference:${MANAGED}/System.Numerics.Vectors.dll)
foreach(output builtin-types.exe generics.exe basic-simd.exe unaligned.exe basic-vectors.exe)
    list(APPEND regression_outputs "${OUT}/${output}")
endforeach()

# --regression compiles every method once for each optimisation set and frees none of it, so jit.S adds 64 MB to the
# .jit pool Mono brings.
add_executable(mono_regress mono_regress.c jit.S ${regression_outputs})
target_compile_definitions(mono_regress PRIVATE JIT_POOL_BYTES=0x4000000)
set_target_properties(mono_regress PROPERTIES C_STANDARD 17 C_EXTENSIONS ON)
target_link_libraries(mono_regress PRIVATE mono_check_runtime)
xsdk_add_xex(mono_regress)
