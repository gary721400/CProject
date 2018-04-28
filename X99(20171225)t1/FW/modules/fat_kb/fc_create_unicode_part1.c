

#pragma CODE = FAT_TEMP_CODE15
#define FAT_OPEN FAT_TEMP_CODE15

#define FILE_CREATE_USING_UNICODE

#define  UpdateDirectoryEntry  unicode_UpdateDirectoryEntry
#define _UpdateDirectoryEntry _unicode_UpdateDirectoryEntry
#define  ClearCluster   unicode_ClearCluster
#define _ClearCluster  _unicode_ClearCluster
#define  CheckWriteAccess  unicode_CheckWriteAccess
#define _CheckWriteAccess _unicode_CheckWriteAccess
#define _FindDirectory   unicode_FindDirectory
#define __FindDirectory _unicode_FindDirectory
#define  ReleaseFATEntries  unicode_ReleaseFATEntries
#define _ReleaseFATEntries _unicode_ReleaseFATEntries
#define  SpecToNameExt  unicode_SpecToNameExt
#define _SpecToNameExt _unicode_SpecToNameExt
#define  CheckFileCharacter unicode_CheckFileCharacter
#define _CheckFileCharacter _unicode_CheckFileCharacter
#define  ReadDirectoryEntry unicode_ReadDirectoryEntry
#define _ReadDirectoryEntry _unicode_ReadDirectoryEntry
#define  FindFile  unicode_FindFile
#define _FindFile _unicode_FindFile
#define  FindFileInDirectory   unicode_FindFileInDirectory
#define _FindFileInDirectory  _unicode_FindFileInDirectory

#include "fc_internal.h"
#include "fc_updatedirectoryentry.c"
#include "fc_access_right.c"
//#include "fc_cluster_clear.c"
#include "fc_finddiretcory.c"
#include "fc_release_table.c"
#include "fc_check_character.c"
#include "fc_spec2name.c"
#include "fc_readdiretcory.c"
#include "fc_findfile.c"
#include "fc_findfile_directory.c"

#include "sio\com.h"


FCFILE* _FSOpenForUnicode2_(char *dir, unsigned char* unicode_name, unsigned int mode, __X FS_ERR* ret_err)
{
    PxFCFILE     file;
    int          drive;
    FS_ERR       err;
    PxFSINFO     fsinfo;
    unsigned int i1, i2, k, num_of_dir_entries, cc, checked;
    unsigned long       dir_cluster;
    PxDIRECTORY     d;

    dir = ParseDrive( dir, &drive );
    
    if( drive < 0 || 0 == unicode_name ) {
        err = FS_INVALID_SPEC;
        goto GoodBye_FCOpen00;
    }
    fsinfo = GdrvGet_fsinfo(drive);

    if( 0 == fsinfo->total_sectors ){
        err = FS_FORMAT_ERROR;
        goto GoodBye_FCOpen00;
    }

    SetFileSystemBusy(drive);
    
    file = (PxFCFILE)GetFCB(&err, drive);
    
    if( FS_NO_ERROR != err  ) {
        goto GoodBye_FCOpen;
    }

    file->mode = (unsigned char)mode;
    file->node_count = 0;
    file->pcluster_nodes = 0;
    file->skips_clusters = 1;
    file->old_final_cluster = 0;
    file->new_start_cluster = 0;

    num_of_dir_entries = (unsigned int)fsinfo->sectors_per_cluster * ENTRIES_PER_SECTOR ;
    
    if( dir[0] ){
        if( FS_NO_ERROR != (err=FindFile(file, dir)) ) {
            goto GoodBye_FCOpen;
        }
        if( 0 == file->attribute & FS_ATTR_DIRECTORY ) {
            err = FS_DIRECTORY_CONFLICT;
            goto GoodBye_FCOpen;
        }
        dir_cluster = file->first_cluster;
    }else{//root
    	if( fsinfo->max_cluster_num < RESERVED_CLUSTER_16_BIT ){
    		num_of_dir_entries = (unsigned int)(fsinfo->first_dat_sector - fsinfo->first_dir_sector) * ENTRIES_PER_SECTOR;
    		dir_cluster = 0;
    	}
    	else{
    	    dir_cluster = 2;
    	}
    }
	file->dir_cluster       = dir_cluster;
    file->dir_first_cluster = dir_cluster;
    file->lfn_cluster       = INVALID_CLUSTER;
    file->new_start_cluster = num_of_dir_entries;
    
    while(1) {
        k = 0;
        checked = 1;
        for(file->dir_index = 0; file->dir_index < num_of_dir_entries; ++file->dir_index) {
            RESET_WATCH_DOG();  // Clear Watch Dog
            
            if( !(d = FindDirectory(file)) ){
                err = FS_INVALID_CLUSTER;//FS_FILE_ONT_FOUND;
                goto GoodBye_FCOpen;//break;
            }
            if( d == INVALID_ADDRESS ){
                err = FS_DRIVE_ERROR;//FS_FILE_ONT_FOUND;
                goto GoodBye_FCOpen;//break;
            }
            
            cc = d->name_ext[0];
            if( cc == END_DIRECTORY ){
                err = 1;
                goto FileNotFound_FCOpen00;//break;
            }
			//long file name entry
			if( cc != DELETED_FILE && (d->attribute & LFN_ATTR) == LFN_ATTR  ) {
			    if( 0 == checked )
			        goto _Lunmatch;
			    if( (cc & LAST_LFN_ENTRY) == LAST_LFN_ENTRY ){
			        file->lfn_cluster = dir_cluster;
			    }
			    cc = ( cc & 0x1F ) - 1;
			    for( k = cc*26, i1=0; k < MAX_PATH_LEN && i1<26; i1 += 2, k += 2, i2 += 2 ){
			        switch( i1 ){
			            case 0:
			                i2 = 1;
			                break;
			            case 10:
			                i2 = 14;
			                break;
			            case 22:
			                i2 = 28;
			                break;
			        }
			        if( unicode_name[k] == 0 && unicode_name[k+1] == 0 )
			            break;
			        if( unicode_name[k] != d->name_ext[i2+1] || unicode_name[k+1] != d->name_ext[i2] )
		            {//next entry
		                checked = 0;
		                goto _Lunmatch;
		            }
			    }
			    if( cc == 0 ){//last
			        //matched !!
			        if( num_of_dir_entries == (cc = file->dir_index+1) ){
			            file->dir_cluster = GetFATEntry(drive, dir_cluster);
			            cc = 0;
			        }
			        file->dir_index = cc;
			        ReadDirectoryEntry(file);
			        goto End_FCOpen;
			    }
			}else{
			    checked = 1;
			    k = 0;
			    _Lunmatch:
			    file->lfn_cluster = INVALID_CLUSTER;
    		}
        }

        if( ( dir_cluster == 0 && fsinfo->max_cluster_num < RESERVED_CLUSTER_16_BIT ) )
        {
            err = FS_DIRECTORY_FULL;
            goto GoodBye_FCOpen;//break;
        }

        if(INVALID_CLUSTER == (dir_cluster = GetFATEntry(drive, dir_cluster))) {
            err = FS_INVALID_CLUSTER;
            goto GoodBye_FCOpen;//break;

        }

        if( dir_cluster >= fsinfo->last_cluster_mark )
        {
            err = 2;
            goto FileNotFound_FCOpen01;//break;
        }
        file->dir_cluster = dir_cluster;
    }    
    
End_FCOpen:    
    file->possible_cluster = 2;
    file->position = 0;
    file->last_cluster = EMPTY_CLUSTER;
    file->cluster = file->first_cluster;

    switch(mode)
    {
        case FS_MODE_READ_WRITE:
            if( CheckWriteAccess(file) ) {
                err = FS_ACCESS_DENIED;
                goto GoodBye_FCOpen;
            }
        case FS_MODE_READ:
            break;

#ifdef FILE_CREATE_USING_UNICODE
        case FS_MODE_CREATE:
            if( CheckWriteAccess(file) ) {
                err = FS_ACCESS_DENIED;
                goto GoodBye_FCOpen;
            }
            ReleaseFATEntries(file);
_LCreateFile:
            file->attribute     = FS_ATTR_ARCHIVE;
            file->first_cluster = EMPTY_CLUSTER;
            file->size          = 0L;
            GetDateNTime(&file->date_n_time);
            file->mode          |= WRITTEN;
            
            UpdateDirectoryEntry(file);

            if( (FlushBuffer(drive))|| gdrv[drive].drv_flush() ) {
                err = FS_DRIVE_ERROR;
                goto GoodBye_FCOpen;
            }
            break;
#endif            
        default:
            err = FS_INVALID_MODE;
            goto GoodBye_FCOpen;
    }
    *ret_err = FS_NO_ERROR;

GoodBye_FCOpen01:
    ClearFileSystemBusy(drive);
    return file;

FileNotFound_FCOpen00:
FileNotFound_FCOpen01:    
    *ret_err = err;
    goto GoodBye_FCOpen01;
    
GoodBye_FCOpen:
    ReleaseFileSlot(file);
    ClearFileSystemBusy(drive);
    
GoodBye_FCOpen00:    
    *ret_err = err;
    return NULL;     
}


#pragma CODE = FAT_COMMON_CODE

#include <KBDM9_DDK.H>

ImageDeclareType(FAT_TEMP_CODE15);
ImageDeclareType(FAT_TEMP_CODE16);

extern int _FSOpenForunicode_ext_Ext(PxFCFILE file, char* short_name, unsigned char* unicode_ext_name, unsigned int value );

FCFILE* _FSOpenForUnicode2(char *dir, char* short_name, unsigned char* unicode_name, unsigned int mode, __X FS_ERR* ret_err)
{
    PxFCFILE file;
    SDKLoadCode(FAT_TEMP_CODE15);
    file = (PxFCFILE)_FSOpenForUnicode2_( dir, unicode_name, mode, ret_err);
    if( FS_MODE_CREATE == mode )
    {
        SendString((U8 *)"u2 ext\r\n");
        SDKLoadCode(FAT_TEMP_CODE16);
        *ret_err = _FSOpenForunicode_ext_Ext( file, short_name, unicode_name, *ret_err );
    }
    else if( *ret_err > 0 )
    {
        *ret_err = FS_FILE_NOT_FOUND;
    }
    return file;
}
