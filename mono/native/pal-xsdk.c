/**
 * \file
 * System.Native's calls on xsdk that the console can't make as POSIX does: the user ids, as pal_uid.c's need a user
 * database the console doesn't have, and pipes and file locks, which pal_config.h points pal_io.c here for.
 *
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include "pal_config.h"
#include "pal_uid.h"

#include <errno.h>
#include <sys/file.h>
#include <unistd.h>

/* The console has no users: a title runs as uid and gid 0 (mono-security.c makes the same choice). */
uint32_t
SystemNative_GetEUid (void)
{
	return 0;
}

uint32_t
SystemNative_GetEGid (void)
{
	return 0;
}

int32_t
SystemNative_SetEUid (uid_t euid)
{
	if (euid == 0)
		return 0;
	errno = EPERM;
	return -1;
}

/* No pipes, as there's no other process to talk to (w32file-unix.c's mono_w32file_create_pipe fails the same way). */
int
mono_xsdk_pal_pipe (int descriptors [2])
{
	errno = ENOSYS;
	return -1;
}

/*
 * flock locks between processes, and a title is the only one. FileStream takes any failure but EWOULDBLOCK as no lock;
 * Mono's own file layer keeps the sharing modes between a title's streams (w32file-unix.c).
 */
int
mono_xsdk_pal_flock (int fd, int operation)
{
	errno = ENOTSUP;
	return -1;
}
