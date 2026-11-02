/* Written by xsdk/native_table.py from pal_errno.c, pal_io.c, pal_memory.c, pal_random.c, pal_time.c, pal-xsdk.c: System.Native by name, for P/Invoke on xsdk. */

#include <stddef.h>
#include <string.h>

#include "mono/utils/mono-xsdk.h"

/* Declared alike, as only their addresses are taken here. */
extern void SystemNative_Access (void);
extern void SystemNative_ChDir (void);
extern void SystemNative_ChMod (void);
extern void SystemNative_Close (void);
extern void SystemNative_CloseDir (void);
extern void SystemNative_ConvertErrorPalToPlatform (void);
extern void SystemNative_ConvertErrorPlatformToPal (void);
extern void SystemNative_CopyFile (void);
extern void SystemNative_Dup (void);
extern void SystemNative_FChMod (void);
extern void SystemNative_FLock (void);
extern void SystemNative_FStat2 (void);
extern void SystemNative_FSync (void);
extern void SystemNative_FTruncate (void);
extern void SystemNative_FcntlCanGetSetPipeSz (void);
extern void SystemNative_FcntlGetPipeSz (void);
extern void SystemNative_FcntlSetCloseOnExec (void);
extern void SystemNative_FcntlSetIsNonBlocking (void);
extern void SystemNative_FcntlSetPipeSz (void);
extern void SystemNative_FnMatch (void);
extern void SystemNative_GetAbsoluteTime (void);
extern void SystemNative_GetEGid (void);
extern void SystemNative_GetEUid (void);
extern void SystemNative_GetLine (void);
extern void SystemNative_GetNonCryptographicallySecureRandomBytes (void);
extern void SystemNative_GetPeerID (void);
extern void SystemNative_GetReadDirRBufferSize (void);
extern void SystemNative_GetTimebaseInfo (void);
extern void SystemNative_GetTimestamp (void);
extern void SystemNative_GetTimestampResolution (void);
extern void SystemNative_INotifyAddWatch (void);
extern void SystemNative_INotifyInit (void);
extern void SystemNative_INotifyRemoveWatch (void);
extern void SystemNative_LChflags (void);
extern void SystemNative_LChflagsCanSetHiddenFlag (void);
extern void SystemNative_LSeek (void);
extern void SystemNative_LStat2 (void);
extern void SystemNative_Link (void);
extern void SystemNative_LockFileRegion (void);
extern void SystemNative_MAdvise (void);
extern void SystemNative_MLock (void);
extern void SystemNative_MMap (void);
extern void SystemNative_MProtect (void);
extern void SystemNative_MSync (void);
extern void SystemNative_MUnlock (void);
extern void SystemNative_MUnmap (void);
extern void SystemNative_MemSet (void);
extern void SystemNative_MkDir (void);
extern void SystemNative_MksTemps (void);
extern void SystemNative_Open (void);
extern void SystemNative_OpenDir (void);
extern void SystemNative_Pipe (void);
extern void SystemNative_Poll (void);
extern void SystemNative_PosixFAdvise (void);
extern void SystemNative_Read (void);
extern void SystemNative_ReadDirR (void);
extern void SystemNative_ReadLink (void);
extern void SystemNative_RealPath (void);
extern void SystemNative_Rename (void);
extern void SystemNative_RmDir (void);
extern void SystemNative_SetEUid (void);
extern void SystemNative_ShmOpen (void);
extern void SystemNative_ShmUnlink (void);
extern void SystemNative_Stat2 (void);
extern void SystemNative_StrErrorR (void);
extern void SystemNative_Symlink (void);
extern void SystemNative_Sync (void);
extern void SystemNative_SysConf (void);
extern void SystemNative_UTime (void);
extern void SystemNative_UTimes (void);
extern void SystemNative_Unlink (void);
extern void SystemNative_Write (void);

static const struct {
	const char *name;
	void (*address) (void);
} functions [] = {
	{ "SystemNative_Access", SystemNative_Access },
	{ "SystemNative_ChDir", SystemNative_ChDir },
	{ "SystemNative_ChMod", SystemNative_ChMod },
	{ "SystemNative_Close", SystemNative_Close },
	{ "SystemNative_CloseDir", SystemNative_CloseDir },
	{ "SystemNative_ConvertErrorPalToPlatform", SystemNative_ConvertErrorPalToPlatform },
	{ "SystemNative_ConvertErrorPlatformToPal", SystemNative_ConvertErrorPlatformToPal },
	{ "SystemNative_CopyFile", SystemNative_CopyFile },
	{ "SystemNative_Dup", SystemNative_Dup },
	{ "SystemNative_FChMod", SystemNative_FChMod },
	{ "SystemNative_FLock", SystemNative_FLock },
	{ "SystemNative_FStat2", SystemNative_FStat2 },
	{ "SystemNative_FSync", SystemNative_FSync },
	{ "SystemNative_FTruncate", SystemNative_FTruncate },
	{ "SystemNative_FcntlCanGetSetPipeSz", SystemNative_FcntlCanGetSetPipeSz },
	{ "SystemNative_FcntlGetPipeSz", SystemNative_FcntlGetPipeSz },
	{ "SystemNative_FcntlSetCloseOnExec", SystemNative_FcntlSetCloseOnExec },
	{ "SystemNative_FcntlSetIsNonBlocking", SystemNative_FcntlSetIsNonBlocking },
	{ "SystemNative_FcntlSetPipeSz", SystemNative_FcntlSetPipeSz },
	{ "SystemNative_FnMatch", SystemNative_FnMatch },
	{ "SystemNative_GetAbsoluteTime", SystemNative_GetAbsoluteTime },
	{ "SystemNative_GetEGid", SystemNative_GetEGid },
	{ "SystemNative_GetEUid", SystemNative_GetEUid },
	{ "SystemNative_GetLine", SystemNative_GetLine },
	{ "SystemNative_GetNonCryptographicallySecureRandomBytes", SystemNative_GetNonCryptographicallySecureRandomBytes },
	{ "SystemNative_GetPeerID", SystemNative_GetPeerID },
	{ "SystemNative_GetReadDirRBufferSize", SystemNative_GetReadDirRBufferSize },
	{ "SystemNative_GetTimebaseInfo", SystemNative_GetTimebaseInfo },
	{ "SystemNative_GetTimestamp", SystemNative_GetTimestamp },
	{ "SystemNative_GetTimestampResolution", SystemNative_GetTimestampResolution },
	{ "SystemNative_INotifyAddWatch", SystemNative_INotifyAddWatch },
	{ "SystemNative_INotifyInit", SystemNative_INotifyInit },
	{ "SystemNative_INotifyRemoveWatch", SystemNative_INotifyRemoveWatch },
	{ "SystemNative_LChflags", SystemNative_LChflags },
	{ "SystemNative_LChflagsCanSetHiddenFlag", SystemNative_LChflagsCanSetHiddenFlag },
	{ "SystemNative_LSeek", SystemNative_LSeek },
	{ "SystemNative_LStat2", SystemNative_LStat2 },
	{ "SystemNative_Link", SystemNative_Link },
	{ "SystemNative_LockFileRegion", SystemNative_LockFileRegion },
	{ "SystemNative_MAdvise", SystemNative_MAdvise },
	{ "SystemNative_MLock", SystemNative_MLock },
	{ "SystemNative_MMap", SystemNative_MMap },
	{ "SystemNative_MProtect", SystemNative_MProtect },
	{ "SystemNative_MSync", SystemNative_MSync },
	{ "SystemNative_MUnlock", SystemNative_MUnlock },
	{ "SystemNative_MUnmap", SystemNative_MUnmap },
	{ "SystemNative_MemSet", SystemNative_MemSet },
	{ "SystemNative_MkDir", SystemNative_MkDir },
	{ "SystemNative_MksTemps", SystemNative_MksTemps },
	{ "SystemNative_Open", SystemNative_Open },
	{ "SystemNative_OpenDir", SystemNative_OpenDir },
	{ "SystemNative_Pipe", SystemNative_Pipe },
	{ "SystemNative_Poll", SystemNative_Poll },
	{ "SystemNative_PosixFAdvise", SystemNative_PosixFAdvise },
	{ "SystemNative_Read", SystemNative_Read },
	{ "SystemNative_ReadDirR", SystemNative_ReadDirR },
	{ "SystemNative_ReadLink", SystemNative_ReadLink },
	{ "SystemNative_RealPath", SystemNative_RealPath },
	{ "SystemNative_Rename", SystemNative_Rename },
	{ "SystemNative_RmDir", SystemNative_RmDir },
	{ "SystemNative_SetEUid", SystemNative_SetEUid },
	{ "SystemNative_ShmOpen", SystemNative_ShmOpen },
	{ "SystemNative_ShmUnlink", SystemNative_ShmUnlink },
	{ "SystemNative_Stat2", SystemNative_Stat2 },
	{ "SystemNative_StrErrorR", SystemNative_StrErrorR },
	{ "SystemNative_Symlink", SystemNative_Symlink },
	{ "SystemNative_Sync", SystemNative_Sync },
	{ "SystemNative_SysConf", SystemNative_SysConf },
	{ "SystemNative_UTime", SystemNative_UTime },
	{ "SystemNative_UTimes", SystemNative_UTimes },
	{ "SystemNative_Unlink", SystemNative_Unlink },
	{ "SystemNative_Write", SystemNative_Write },
};

void *
mono_xsdk_native_lookup (const char *name)
{
	for (size_t i = 0; i < sizeof (functions) / sizeof (functions [0]); i++) {
		if (strcmp (functions [i].name, name) == 0)
			return (void *)functions [i].address;
	}
	return NULL;
}
