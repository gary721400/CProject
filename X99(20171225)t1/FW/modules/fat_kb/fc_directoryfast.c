
#if 0

#else

#define DOTDOT

#pragma CODE = FAT_TEMP_CODE06
#define FAT_OPEN FAT_TEMP_CODE06

#define  LengthenDirectory  mkdir_LengthenDirectory
#define _LengthenDirectory _mkdir_LengthenDirectory
#define  ClearCluster   mkdir_ClearCluster
#define _ClearCluster  _mkdir_ClearCluster
#define  UpdateDotNDotDot  mkdir_UpdateDotNDotDot
#define _UpdateDotNDotDot _mkdir_UpdateDotNDotDot
#define  UpdateDirectoryEntry  mkdir_UpdateDirectoryEntry
#define _UpdateDirectoryEntry _mkdir_UpdateDirectoryEntry
#define  SpecToNameExt  mkdir_SpecToNameExt
#define _SpecToNameExt _mkdir_SpecToNameExt
#define  CheckFileCharacter mkdir_CheckFileCharacter
#define _CheckFileCharacter _mkdir_CheckFileCharacter
#define  _FindDirectory  mkdir_FindDirectory
#define  __FindDirectory  _mkdir_FindDirectory
#define  FindFile  mkdir_FindFile
#define _FindFile _mkdir_FindFile
#define  FindFileInDirectory   mkdir_FindFileInDirectory
#define _FindFileInDirectory  _mkdir_FindFileInDirectory
#define  ReadDirectoryEntry mkdir_ReadDirectoryEntry
#define _ReadDirectoryEntry _mkdir_ReadDirectoryEntry

#include "fc_internal.h"
#include "fc_lengthen.c"
#include "fc_cluster_clear.c"
#include "fc_updatedotdot.c"
#include "fc_updatedirectoryentry.c"
#include "fc_spec2name.c"
#include "fc_check_character.c"
#include "fc_finddiretcory.c"
#include "fc_findfile_directory.c"
#include "fc_findfile.c"
#include "fc_readdiretcory.c"

FS_ERR FSDirectory_(char *spec)
{
    PxFCFILE file ;//= &internal_slot;
//    unsigned long cluster;
	int err, drive;// = ParseDrive(&spec);
	
	spec = ParseDrive( spec, &drive );

	if( drive < 0 ) {
        return FS_INVALID_SPEC;
    }

    if( 0 == gdrv[drive].fsinfo->total_sectors ){
        return FS_FORMAT_ERROR;
    }

    if( isCWriteProtect(drive) ){
        return FS_ACCESS_DENIED;
    }

    SetFileSystemBusy(drive);

	file = &(gdrv[drive].fsinfo->internal_slot);
    file->first_cluster = 0;
	file->drive = drive;

    if( FS_FILE_NOT_FOUND != (err=FindFile(file, spec)) ) {
        if( FS_NO_ERROR == err )
            err = FS_DIRECTORY_CONFLICT;
        goto GoodBye_FCDirectory;
    }

    if( FS_NO_ERROR != (err = LengthenDirectory(file)) ){
        goto GoodBye_FCDirectory;
    }

    file->attribute = FS_ATTR_DIRECTORY;
    file->first_cluster = AddNewCluster(drive,EMPTY_CLUSTER, 2);

    if( 0 == file->first_cluster ){
        err = FS_DIRECTORY_FULL;
        goto GoodBye_FCDirectory;
    }

    file->size = 0L;
    GetDateNTime(&file->date_n_time);
    //critical
    /*
    if( 0xffffffff == gtrash[drive].cluster
        &&  -1 == gtrash[drive].index )
    {
        file->attribute |= FS_ATTR_SYSTEM|FS_ATTR_HIDDEN;
        gtrash[drive].cluster = file->first_cluster;
        gtrash[drive].index = 0;
    }
*/
    UpdateDirectoryEntry(file, 0);
    UpdateDotNDotDot(file);

    if( FS_NO_ERROR != (err=FlushBuffer(drive)) ){
        goto GoodBye_FCDirectory;
    }
    if( gdrv[drive].drv_flush() ) {
            err = FS_DRIVE_ERROR;
    }
    //end critical
GoodBye_FCDirectory:
    ClearFileSystemBusy(drive);
    return err;
}
#endif
#include <KBDM9_DDK.H>
#pragma CODE = FAT_COMMON_CODE

ImageDeclareType(FAT_TEMP_CODE06);
FS_ERR FSDirectory(char *spec)
{
    SDKLoadCode(FAT_TEMP_CODE06);
    return FSDirectory_(spec);
}
