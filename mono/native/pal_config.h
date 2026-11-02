#include <config.h>

#define MONO 1
/* clock_gettime () is found by configure on Apple builds, but its only present from ios 10, macos 10.12, tvos 10 and watchos 3 */
#if defined (TARGET_WASM) || defined (TARGET_IOS) || defined (TARGET_OSX) || defined (TARGET_WATCHOS) || defined (TARGET_TVOS)
#undef HAVE_CLOCK_MONOTONIC
#undef HAVE_CLOCK_MONOTONIC_COARSE
#endif

#ifdef HOST_XSDK
/* newlib has no ESHUTDOWN, which pal_errno.c maps; nothing on xsdk sets it, so a value of its own will do. */
#define ESHUTDOWN 1000
/* BSD's whiteout entry, which pal_io.c checks its own value against; FATX has none. */
#define DT_WHT 14
/* FATX has no symbolic links, so lstat is stat; newlib declares no lstat for this target. */
#define lstat stat
/* newlib implements getline as __getline and doesn't declare getline itself. */
#define getline __getline
/* The PAL is compiled with _GNU_SOURCE here (configure.ac), which gives newlib's GNU strerror_r. */
#define HAVE_GNU_STRERROR_R 1
/* What the console has no use for, in pal-xsdk.c; renamed so that newlib's own prototypes declare them. */
#define pipe mono_xsdk_pal_pipe
#define flock mono_xsdk_pal_flock
#endif
