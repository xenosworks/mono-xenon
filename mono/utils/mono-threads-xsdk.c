/**
 * \file
 * Mono's thread platform on xsdk: no signals to suspend threads with, as they stop cooperatively, and each thread's stack
 * read from the kernel's processor block.
 *
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <config.h>

#ifdef TARGET_XSDK

#include <limits.h>
#include <stdint.h>

#include <glib.h>

#include "mono-threads.h"

void
mono_threads_suspend_init_signals (void)
{
}

gint
mono_threads_suspend_get_suspend_signal (void)
{
	return -1;
}

gint
mono_threads_suspend_get_restart_signal (void)
{
	return -1;
}

gint
mono_threads_suspend_get_abort_signal (void)
{
	return -1;
}

void
mono_threads_platform_get_stack_bounds (guint8 **staddr, size_t *stsize)
{
	uint8_t *pcr;
	uintptr_t base;
	uintptr_t limit;

	/* r13 holds the KPCR, whose stack base and end are at 0x70 and 0x74 (xenia:src/xenia/kernel/xthread.h:73-80). */
	__asm__ volatile ("mr %0, 13" : "=r" (pcr));
	base = *(volatile uint32_t *)(pcr + 0x70);
	limit = *(volatile uint32_t *)(pcr + 0x74);
	*staddr = (guint8 *)limit;
	*stsize = base - limit;
}

int
mono_thread_info_get_system_max_stack_size (void)
{
	return INT_MAX;
}

#endif
