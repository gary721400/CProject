/*******************************************************************************
*
*   fs.h - FAT Compatible File System
*
*******************************************************************************/

#include "storbase.h"	//including the storage base header files
#include <stddef.h>

#ifdef  FAT_VARIABLE
#define _RAM_AREA
#else
#define _RAM_AREA   __X
#endif




/******************************
* function for mp3 internal use
******************************/
/******************************
* Storage Status Checking Routines
******************************/

#ifndef _FS_H_
#define _FS_H_

//__EXTMEM extern int fserr;
#define NAME_SIZE           (8)
#define DOT_CHAR_SIZE       (1)
#define EXT_SIZE            (3)
#define SHORT_NAME_SIZE     (NAME_SIZE + DOT_CHAR_SIZE + EXT_SIZE)

#define MAX_PATH_LEN        (256)
#define LFN_SIZE            (255)
#define CLUSTER_CACHE_SKIPS (128)

#define _BIG_ENDIAN

#define CACHE_CHUNK_SIZE    ((unsigned int)sizeof(FAT_CACHE) - 2 + SECTOR_SIZE)

/* File Opened Mode */
#define FS_MODE_CREATE      (0)
#define FS_MODE_READ        (1)
#define FS_MODE_READ_WRITE  (2)

/* File Attributes */
#define FS_ATTR_READONLY    (0x01)
#define FS_ATTR_HIDDEN      (0x02)
#define FS_ATTR_SYSTEM      (0x04)
#define FS_ATTR_VOLUME      (0x08)
#define FS_ATTR_DIRECTORY   (0x10)
#define FS_ATTR_ARCHIVE     (0x20)

enum buffer_status { EMPTY, CLEAN, DIRTY };

typedef enum{
    FS_NO_ERROR              = 0  ,
    FS_DRIVE_ERROR           = -1 ,
    FS_FORMAT_ERROR          = -2 ,
    FS_SYSTEM_ERROR          = -3 ,
    FS_INVALID_DIRECTORY     = -4 ,
    FS_INVALID_SPEC          = -5 ,
    FS_INVALID_MODE          = -6 ,
    FS_FILE_NOT_FOUND        = -7 ,
    FS_DIRECTORY_CONFLICT    = -8 ,
    FS_DIRECTORY_FULL        = -9 ,
    FS_OPENFILE_FULL         = -10,
    FS_VOLUME_FULL           = -11,
    FS_ACCESS_DENIED         = -12,
    FS_SEEK_OUT_OF_RANGE     = -13,
    FS_RENAMING_ERROR        = -14,
    FS_UNRECOVERABLE_FILE    = -15,
    FS_END_DIRECTORY         = -16,
    FS_TRASH_ERROR           = -17,
    FS_PARAMETER_ERROR       = -18,
    FS_FILE_INCONSISTENT     = -19,
    FS_WRITE_OUT_RANGE       = -20,
    FS_TABLE_INCONSISTENT    = -21,
    FS_DRIVE_NOT_READY       = -22,
    FS_INVALID_CLUSTER       = -23
} FS_ERR;
//typedef int FS_ERR ;

struct _date_n_time {
    unsigned int year:7;
    unsigned int month:4;
    unsigned int day:5;
    unsigned int hour:5;
    unsigned int minute:6;
    unsigned int second:5;
};

typedef struct _file_info {
    unsigned char       name[SHORT_NAME_SIZE + 1];
    unsigned char       attribute;
    struct _date_n_time date_n_time;
    unsigned long       size;
    unsigned int        index;
    unsigned char       lfn[LFN_SIZE+1];
} FILE_INFO;

#define OFFSET_date_n_time  22
#define OFFSET_spec         6
typedef struct _fcb {
    __X struct _fcb *   next;
    int        			used;               /* in use or not */
    unsigned char       spec[14];
    unsigned char       attribute;          /* open attributes */
    unsigned char       mode;
    struct _date_n_time date_n_time;
    unsigned int        sector_in_cluster;  /* sector within cluster */
    unsigned long       first_cluster;      /* first cluster in file */
    unsigned long       last_cluster;       /* last cluster read/written */
    unsigned long       cluster;            /* next cluster to read/write */
    unsigned long       position;           /* read/write position */
    unsigned long       size;               /* file size */

    unsigned long       dir_cluster;
    unsigned int        dir_index;
    unsigned long       dir_first_cluster;
    unsigned int        fmode;
    unsigned long       possible_cluster;
    unsigned int        entry_index;
    unsigned long       lfn_cluster;
    unsigned long       old_final_cluster;      /* file end when open file */
    unsigned long       new_start_cluster;      /* first when file append */
    unsigned int        *pcluster_nodes;
    unsigned int        node_count;
    unsigned int        skips_clusters;
    int        			drive;              /* drive id */    
} FCFILE;


typedef struct _fsinfo {
    unsigned int    bytes_per_sector;       /* bytes per sector */
    unsigned int    sectors_per_cluster;    /* sectors per cluster */
    unsigned int    num_of_fats;            /* number of FAT's */
    unsigned int    dir_entries;            /* number of directory entries */
    unsigned long   total_sectors;          /* sectors on the volume */
    unsigned long   sectors_per_fat;        /* sectors per FAT */
    unsigned char   media_id;               /* mdeia ID */
    unsigned char   dummy;
    unsigned long    first_fat_sector;
    unsigned long    first_dir_sector;
    unsigned long    first_dat_sector;
    unsigned long    max_cluster_num;
    unsigned long    last_cluster_mark;
#ifdef KBOS
	OS_EVENT*    	fs_semaphore;
#else
    unsigned int *  fs_semaphore;
#endif
	FCFILE 			internal_slot;
	char            entry_ext[6];   //ext file name that user wanted
} FSINFO;


struct _fc_trash_info{
    unsigned long first_cluster;
    unsigned long cluster;
    int index;
    int secidx;
};

struct _drive {
    int (*drv_clear)(void);
    int (*drv_read)(unsigned long sector,unsigned char *buf);
    int (*drv_write)(unsigned long sector,unsigned char *buf);

    unsigned long (*drv_size)(void);

    int (*drv_flush)(void);

    __X unsigned char *buffer;
    unsigned long sector_in_buffer;
    unsigned int buffer_status;
    __X FSINFO * fsinfo;
};

typedef struct _fat_cache {
    struct _fat_cache *next;
    int drive;
    int index;
    int state;
    unsigned char data[2];
} FAT_CACHE;


typedef struct _fs_entry_info {
	int drive ;
}FS_ENTRY_INFO;

//typedef void (*_fpGetDateNTime)(struct _date_n_time *p);
//extern _fpGetDateNTime fpGetDateNTime;

/* Function Prototypes */

void FSInit(int MaxDrvCount);
//void FSInit(__X struct _drive *pDrvSpace, int MaxDrvCount);

FS_ERR  FSFormat(unsigned int drive);
FS_ERR  FSFormat32(unsigned int drive);

FCFILE* _FSOpen(char *spec, unsigned int mode, __X FS_ERR* err);
FCFILE* _FSOpenForFastWrite(char *spec, unsigned int mode, __X FS_ERR* err);
FCFILE* _FSOpenForFastSeek (char *spec, unsigned int mode, __X void* cache_buf, unsigned int buf_size, __X FS_ERR* err );
FCFILE* _FSOpenForFastSeekCustom(char *spec, unsigned int mode, __X void* cache_buf, unsigned int buf_size, unsigned int skips_clusters, __X FS_ERR* err );
//FCFILE* _FSOpenForUnicode (char *dir, unsigned char* unicode_name,  unsigned int mode, __X FS_ERR* ret_err);
FCFILE* _FSOpenForUnicode2(char *dir, char* short_name, unsigned char* unicode_name, unsigned int mode, __X FS_ERR* ret_err);
#define FSOpen                  (__X FCFILE*)_FSOpen
#define FSOpenForFastWrite      (__X FCFILE*)_FSOpenForFastWrite
#define FSOpenForFastSeek       (__X FCFILE*)_FSOpenForFastSeek
#define FSOpenForFastSeekCustom (__X FCFILE*)_FSOpenForFastSeekCustom
//#define FSOpenForUnicode        (__X FCFILE*)_FSOpenForUnicode
#define FSOpenForUnicode2       (__X FCFILE*)_FSOpenForUnicode2

#define HowMuchBytesSeekBufferNeed(file, drive) ((unsigned int)(((file)->size)/((32768)*(gdrv[(drive)].fsinfo->sectors_per_cluster))))

/*
Formula to decided how much buffer need for certain file:
    1.  Per entry in buffer is 2Bytes
    2.  This function keep file's FAT position in buffer every 128
        clusters
    3.  In different disk, 1 cluster may be different sectors count
        We can get from 'gdrv[DRIVE].fsinfo->sectors_per_cluster'
        The 'DRIVE' is the index to your disk on DMCU FAT system
        In mostly case, the gdrv[0] (zero) indicats your NAND flash
        In addition, the 'gdrv[0].fsinfo->sectors_per_cluster' should
        be '8' in DMCU FAT system, in mostly case.
    4.  1 sector always be 512Bytes
    5.  Put above all tegother. A file with 10MBytes = 10,485,760Bytes
        The it need
            10,485,760 / (   128 //every 128 clusters keep 1 entry
                          * gdrv[0].fsinfo->sectors_per_cluster
                          * 512 //1 sector = 512Bytes
                        )
            -> 20 entries in buffer (if the file is on disk 0,
               and 'gdrv[0].fsinfo->sectors_per_cluster' is '8')
            -> 40Bytes (1 entry = 2Bytes)
    6.  So, in this 10MBytes sample, you need give a "cache_buf" have
        at least 40Bytes and "buf_size" as 40
        Shortly, 512KBytes need 2Bytes (i.e., 1 entry)
*/

unsigned int  FSRead (__X FCFILE *pfile, __X void *buf, unsigned int len, __X FS_ERR* err_arg);

unsigned int  FSWrite(__X FCFILE *pfile, __X void *buf, unsigned int len, __X FS_ERR* err_arg);

FS_ERR FSClose (__X FCFILE *file);
FS_ERR FSFlush (__X FCFILE *file);
FS_ERR FSResize(__X FCFILE *file, unsigned long new_size);
FS_ERR FSRepair(__X FCFILE *file);

FS_ERR FSSeek(__X FCFILE *file, unsigned long offset);
FS_ERR FSSetFileFastSeek(__X FCFILE* file, unsigned int* cache_buf, unsigned int buf_size_in_bytes, unsigned int skips_clusters );

FS_ERR FSDelete(char *spec);

unsigned long FSGetFileSize(char *spec, __X FS_ERR* err);

unsigned char FSGetAttribute(char *spec, __X FS_ERR* err);

FS_ERR FSSetAttribute(char *spec, unsigned char attr);

FS_ERR FSGetDateTime(char *spec, __X struct _date_n_time *stamp);

FS_ERR FSDirectory(char *spec);

FS_ERR FSGetEntry(char *spec, unsigned int index, __X FILE_INFO *finfo, __X FS_ENTRY_INFO* fentry);

FS_ERR FSGetSelEntry(char *spec, unsigned int index, __X FILE_INFO *finfo, char* select, __X FS_ENTRY_INFO* fentry );

FS_ERR FSGetNextEntry(__X FILE_INFO *finfo, __X FS_ENTRY_INFO* fentry);

FS_ERR FSRename(char *old_spec, char *new_spec);

unsigned long FSFreeSpace(unsigned int drive, __X FS_ERR *err);

unsigned long FSDriveSize(unsigned int drive, __X FS_ERR *err);

FS_ERR FSVolume(unsigned int drive, char *name, int flag);

//=============================================================================
// MACRO DEFINE
//=============================================================================

#define FS_BUSY_FLAG    0x0001u     // bit 0: File System busy flag.
#define FLASH_BUSY_FLAG 0x0002u	    // bit 1: Trans-Layer busy flag.
#define BUS_BUSY_FLAG   0x0008u     // bit 3: Bus busy flag.
typedef struct {
    struct _drive Drv;
    //tKBMP3_IP_JumpTableTrapNo TrapNo;
    //void (*pFunc)(void);
    FS_ERR (*pMountFunc)(void);
}tFSDrvHandle;

extern _RAM_AREA unsigned int STORAGEFLAGS;
extern _RAM_AREA struct _drive _drives_xram[];
extern const tFSDrvHandle NF_Drive;
extern const tFSDrvHandle SD_Drive;

typedef void (*tStorageBusyCallBackFunc)(void);
extern int FSCloseAll(void);
extern FS_ERR FS_mount(const tFSDrvHandle *pdrive);
extern int IsFileSystemBusy(int drive);

extern _RAM_AREA tStorageBusyCallBackFunc pStorageBusyCallBackFunc;

#endif /* _FS_H_ */
