

#pragma CODE = FAT_TEMP_CODE16
#define FAT_OPEN FAT_TEMP_CODE16

#define FILE_CREATE_USING_UNICODE

#define  UpdateDirectoryEntry  unicode_ext_UpdateDirectoryEntry
#define _UpdateDirectoryEntry _unicode_ext_UpdateDirectoryEntry
#define  ClearCluster   unicode_ext_ClearCluster
#define _ClearCluster  _unicode_ext_ClearCluster
#define  CheckWriteAccess  unicode_ext_CheckWriteAccess
#define _CheckWriteAccess _unicode_ext_CheckWriteAccess
#define _FindDirectory   unicode_ext_FindDirectory
#define __FindDirectory _unicode_ext_FindDirectory
#define  ReleaseFATEntries  unicode_ext_ReleaseFATEntries
#define _ReleaseFATEntries _unicode_ext_ReleaseFATEntries
#define  SpecToNameExt  unicode_ext_SpecToNameExt
#define _SpecToNameExt _unicode_ext_SpecToNameExt
#define  CheckFileCharacter unicode_ext_CheckFileCharacter
#define _CheckFileCharacter _unicode_ext_CheckFileCharacter
#define  ReadDirectoryEntry unicode_ext_ReadDirectoryEntry
#define _ReadDirectoryEntry _unicode_ext_ReadDirectoryEntry
#define  FindFile  unicode_ext_FindFile
#define _FindFile _unicode_ext_FindFile
#define  FindFileInDirectory   unicode_ext_FindFileInDirectory
#define _FindFileInDirectory  _unicode_ext_FindFileInDirectory

#include "fc_internal.h"
#include "fc_updatedirectoryentry.c"
//#include "fc_access_right.c"
#include "fc_cluster_clear.c"
#include "fc_finddiretcory.c"
//#include "fc_release_table.c"
#include "fc_check_character.c"
#include "fc_spec2name.c"
//#include "fc_readdiretcory.c"
//#include "fc_findfile.c"
//#include "fc_findfile_directory.c"

#include "sio\com.h"


void swap_memcpy( void* d, void* s, int k, int ff );
__asm{
.area _$swap_memcpy(FAT_OPEN,REL)
_swap_memcpy::
    link #0
    PUSH    R2
	push	R3
	push	R4
	push	R1
	push	R6
	push    RC
	push    SRC
	push    RS
	push    RE
	PUSH    PSR

	MOV     RC, #0
	MOV     PSR, #0x3800

	MOV     R4, [fp+#6] ;dst
	MOVX	ES1, R4
	MOV		R4, [fp+#8]

    MOV     R3, [fp+#10] ;src
	MOVX	ES0, R3
	MOV		R3, [fp+#12] ;
	MOV		R6, [fp+#14] ;k

	SR      R6, #1
	SUB		R6, #1
	RJMP	IF_N , $2
	REPB    R6, $2
	EX0B	Rb0, [PTR0++]
	EX0B	Rb1, [PTR0++]
	EX0B    [PTR1++],Rb1
	EX0B    [PTR1++],Rb0
    $2:
    MOV		R6, [fp+#16] ;ff
    SUB     R6, #1
    RJMP	IF_N, $3
    ;ADD     R4, #2
    MOV     R0, #0xFFFF
    REPB    R6, $3
    EX0B    [PTR1++],Rb1
    NOP
    $3:
    POP     PSR
	pop     RE
	pop     RS
	pop     SRC
	pop     RC
	pop     R6
	pop     R1
	pop     R4
	pop     R3
	POP     R2
	unlink
	RETS
}

unsigned char ChkSum (__X unsigned char *pFcbName)
{
    unsigned char sum;
    int i;
    for (sum = i = 0; i < 11; i++) {                             
        sum = (((sum & 1) << 7) | ((sum & 0xfe) >> 1)) + pFcbName[i];
    }
    return sum;
}                                                            

typedef struct
{
    unsigned char dir_sig        ;//DB ? ; signature byte
    unsigned char dir_lname1[10] ;//DB 10 DUP (?) ; long name string
    unsigned char dir_attr       ;//DB ? ; file attributes
    unsigned char dir_flags      ;//DB ? ; flags byte (TBD)
    unsigned char dir_chksum     ;//DB ? ; checksum of 8.3 name
    unsigned char dir_lname2[12] ;//DB 12 DUP (?) ; long name string
    unsigned char dir_first[2]   ;//DW ? ; first cluster number, must be 0
    unsigned char dir_lname3[4]  ;//DB 4 DUP (?) ; long name string    
} lfn_entry;


/*int ClearClusterT(unsigned int drive, unsigned long cluster)
{
    __X struct _drive * p = &gdrv[drive];
    unsigned int count = p->fsinfo->sectors_per_cluster;
    unsigned long sector;

    if( 0 == (sector = FirstSectorInCluster(drive, cluster)) ){
        return FS_INVALID_CLUSTER;
    }

    FlushBuffer(drive);
    p->buffer_status = EMPTY;
    memset(p->buffer, 0, SECTOR_SIZE);
    while( count-- > 0 )
    {
        p->drv_write(sector++, p->buffer);
      FlushBuffer(drive);
    }

    return FS_NO_ERROR;
}*/

int AddNewDirCluster(PxFCFILE file)
{
    unsigned long dir_cluster = AddNewCluster(file->drive, file->dir_cluster, 2);

   PxFSINFO fsinfo;
    
    file->dir_cluster = dir_cluster;
    
    if( dir_cluster == 0 ){
        return FS_DIRECTORY_FULL;
    }

    file->dir_index = 0 ;
    ClearCluster(file->drive, dir_cluster);
    //ClearClusterT(file->drive, dir_cluster);

    return FS_NO_ERROR;
}

int _FSOpenForunicode_ext_Ext(PxFCFILE file, char* short_name, unsigned char* unicode_ext_name, unsigned int value )
{
    int err=0;
    unsigned int i1, i2, k, num_of_dir_entries = (unsigned int)file->new_start_cluster, cc;
    PxDIRECTORY     d;
    int drive = file->drive;
    unsigned long       dir_cluster;
    
    SetFileSystemBusy(drive);
    
    file->new_start_cluster = 0ul;
    
    if( value == 2 ){
FileNotFound_FCOpen01: // directory ending in cluster end
        SendString((U8 *)"add new clu2\r\n");
        if( FS_NO_ERROR != (err = AddNewDirCluster(file) ) ){
            goto GoodBye_FCOpen;//break;
        }
    }

FileNotFound_FCOpen00: // directory ending in one cluster

    for( k=0; *(unsigned int*)(unicode_ext_name+k); k+=2);
    
    SpecToNameExt(file->spec, (unsigned char*)short_name);
    
    i2= ChkSum(file->spec);
    i1 = (k+25)/26-1;
    k -= (i1)*26;
    
    k+=2;
    
    cc = LAST_LFN_ENTRY| (i1+1);
    while(1){
        for(; file->dir_index < num_of_dir_entries; ++file->dir_index) {
            if( !(d = FindDirectory(file)) ){
                err = FS_INVALID_CLUSTER;//FS_FILE_ONT_FOUND;
                goto GoodBye_FCOpen;//break;
            }
            SetDirty(drive);//gdrv[drive].buffer_status = DIRTY;
            if( 0 == cc ){
                memcpy( file->spec, short_name, 14 );

                file->attribute     = FS_ATTR_ARCHIVE;
                file->first_cluster = EMPTY_CLUSTER;
                file->size          = 0L;
                GetDateNTime(&file->date_n_time);
                file->mode          |= WRITTEN;
                
                UpdateDirectoryEntry(file);
    
                if( (FlushBuffer(drive))|| gdrv[drive].drv_flush() ) {
                    err = FS_DRIVE_ERROR;
                }
                goto GoodBye_FCOpen01;
            }
            //fill lfn
            ((__X lfn_entry*)d)->dir_sig = cc;
            ((__X lfn_entry*)d)->dir_attr = LFN_ATTR;
            ((__X lfn_entry*)d)->dir_flags = \
            ((__X lfn_entry*)d)->dir_chksum = \
            ((__X lfn_entry*)d)->dir_first[0] =\
            ((__X lfn_entry*)d)->dir_first[1] = 0;
            
            if( k > 22 ){
                swap_memcpy( ((__X lfn_entry*)d)->dir_lname3, unicode_ext_name+(i1)*26+22, k-22, 26-k );
                k = 22;//-=(k-22);
            }else{
                memset( ((__X lfn_entry*)d)->dir_lname3, 0xFF, 4 );
            }
            if( k > 10 ){
                swap_memcpy( ((lfn_entry*)d)->dir_lname2, unicode_ext_name+(i1)*26+10, k-10, 22-k );
                k = 10;//-=(k-10);
            }else{
                memset( ((__X lfn_entry*)d)->dir_lname2, 0xFF, 12 );
            }
            swap_memcpy( ((__X lfn_entry*)d)->dir_lname1, unicode_ext_name+(i1)*26, k, 10-k );
            ((__X lfn_entry*)d)->dir_chksum = i2;//ChkSum( (unsigned char*)d );
            cc = (i1);
            --i1;
            k = 26;
        }    
        if(INVALID_CLUSTER == (dir_cluster = GetFATEntry(drive, dir_cluster))) {
            err = FS_INVALID_CLUSTER;
            goto GoodBye_FCOpen;//break;
        }

        if( dir_cluster >= (GdrvGet_fsinfo(drive))->last_cluster_mark )
        {
            SendString((U8 *)"new clu\r\n");
            if( FS_NO_ERROR != (err = AddNewDirCluster(file) ) ){
                goto GoodBye_FCOpen;//break;
            }
        }
    }
    goto GoodBye_FCOpen01;
    
GoodBye_FCOpen:
    ReleaseFileSlot(file);

GoodBye_FCOpen01:        
    ClearFileSystemBusy(drive);
    
    return err;     
}
