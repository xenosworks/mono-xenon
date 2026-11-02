/**
 * \file
 * What mini-posix.c installs for signals and crash reports, on a console without signals: nothing.
 *
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <config.h>

#ifdef TARGET_XSDK

#include <glib.h>

#include "mini.h"
#include "mini-runtime.h"
#include "mono/utils/mono-threads.h"

void
mono_runtime_setup_stat_profiler (void)
{
}

void
mono_runtime_shutdown_stat_profiler (void)
{
}

gboolean
MONO_SIG_HANDLER_SIGNATURE (mono_chain_signal)
{
	return FALSE;
}

void
mono_runtime_install_handlers (void)
{
}

void
mono_runtime_cleanup_handlers (void)
{
}

void
mono_init_native_crash_info (void)
{
}

void
mono_cleanup_native_crash_info (void)
{
}

void
mono_dump_native_crash_info (const char *signal, MonoContext *mctx, MONO_SIG_HANDLER_INFO_TYPE *info)
{
}

void
mono_post_native_crash_handler (const char *signal, MonoContext *mctx, MONO_SIG_HANDLER_INFO_TYPE *info, gboolean crash_chaining)
{
}

void
mini_register_sigterm_handler (void)
{
}

gboolean
mono_thread_state_init_from_handle (MonoThreadUnwindState *tctx, MonoThreadInfo *info, void *sigctx)
{
	return FALSE;
}

#endif
