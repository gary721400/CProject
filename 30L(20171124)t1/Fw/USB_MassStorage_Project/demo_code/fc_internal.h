#ifndef _FC_INTERNAL_H_
#define _FC_INTERNAL_H_

#pragma DATA = FAT_DATA

#include "fs.h"
#include <string.h>

#ifdef  FAT_VARIABLE
#define _RAM_AREA
#else
#define _RAM_AREA   __X
#endif

void* nearcpy(__X void* d, __X void* s, unsigned int l);
void* nearset(__X void* p, unsigned int d,  unsigned int l);

//#define FAT_VERSION         101
//#define FAT_WRITE_OPTM      0

#define RSD_SECTORS         (1)
#define RSD_SECTORS32       (32)
#define NUM_FATS            (2)
#define DIR_ENTRIES         (512)
#define DIR_ENTRIES32       (0)
#define MEDIA_DESCRIPTOR    (0xF8)    // 0xF8: fixed media, 0xF0: removable media

#define DFT_PBR_SECTOR      (0x0000)
#define MBR_SECTOR          (0x0000)

#define INVALID_ADDRESS     (__X void*)1

#define ENTRIES_PER_SECTOR  (SECTOR_SIZE/(unsigned int)sizeof(struct _directory))

#define NO_DIRECTORY_INDEX  (0xFFFF)

/* Special values for first byte of name. */
#define END_DIRECTORY	(0x00)
#define DELETED_FILE	(0xE5)

#define EMPTY_CLUSTER	(0x0000)
#define INVALID_CLUSTER (unsigned long)(0xFFFFFFFF)

//#define LAST_CLUSTER_MARK 0xFFF8
#define RESERVED_CLUSTER_16_BIT (unsigned long)(0xFFF5)
#define RESERVED_CLUSTER_12_BIT (0xFF5)
#define LAST_CLUSTER_12_BIT	(0xFF8)
#define LAST_CLUSTER_16_BIT	(0xFFF8)
#define LAST_CLUSTER_32_BIT (unsigned long)(0x0ffffff8)
#define MASK_CLUSTER_32_BIT (unsigned long)(0x0fffffff)

#ifdef DMCU_ED1
#define RESET_WATCH_DOG()   __asm("BCLR WDT.0\n")
#else
#define RESET_WATCH_DOG()   __asm("BCLR RTCWDT.14\n")
#endif

/* additional mode bit */
#define WRITTEN    (1<<7)

#define LFN_ATTR    \
    (FS_ATTR_READONLY|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM|FS_ATTR_VOLUME)

#define LFN_ATTR_MASK  \
    (FS_ATTR_READONLY|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM|FS_ATTR_VOLUME|FS_ATTR_DIRECTORY|FS_ATTR_ARCHIVE)

#define LAST_LFN_ENTRY     0x40

struct _directory {
    unsigned char   name_ext[NAME_SIZE+EXT_SIZE];   /* filename + extension */
    unsigned char   attribute;                      /* file attributes */
    unsigned int    reserved;                   /* reserved */
    struct _date_n_time    date_n_time1;                             /* time created */
	unsigned int    reserved1;                   /* reserved */
    unsigned int    first_cluster_high;                    /* first cluster number */
    struct _date_n_time    date_n_time;                             /* time last modified */
    unsigned int    first_cluster;                    /* first cluster number */
    unsigned long   size;                           /* file size */
};
typedef __X struct _directory*  PxDIRECTORY;

/* Global variables */
extern const unsigned char fat_ch_table[];
extern _RAM_AREA FCFILE*                  _RAM_AREA gfslot;
extern _RAM_AREA struct _drive*           _RAM_AREA gdrv;
//extern struct _fc_trash_info*   _RAM_AREA gtrash;
//#define GET_TRASH_ENTRY(idx, x)  (((struct _fc_trash_info*)(((char*)gtrash)+((unsigned int)(idx)*(unsigned int)sizeof(struct _fc_trash_info))))->##x)
#define GET_DRIVE_ENTRY(idx, x)  (((struct _drive*)(((char*)gdrv)+((unsigned int)(idx)*(unsigned int)sizeof(struct _drive))))->##x)

typedef   __X FCFILE*  PxFCFILE;
typedef   __X FSINFO*  PxFSINFO;

FCFILE*     GetFileSlot(void);

#include <ctype.h>
#define to_upper_case(__c)  ((char)(toupper((int)(__c))))

char*   SpecToNameExt(__X unsigned char *name_ext, unsigned char *spec);
int     isCWriteProtect(int drive);
int     CheckWriteAccess(PxFCFILE file);
int     DeleteLFN(PxFCFILE file);
int     FindEntry(PxFCFILE file, char *spec, unsigned int index);
int     FindFile(PxFCFILE file, char *spec);
int     FindFileInDirectory(PxFCFILE file, __X unsigned char *name_ext);
FCFILE*     GetFCB(__X FS_ERR *err, int drive);
int     GetInternalFCB( int drive );
int     LengthenDirectory(PxFCFILE file);
char*   ParseDrive(char *spec, __X int* drive );
char*   SkipSlash( char* spec );
#ifdef RDDIRECTENT
int     ReadDirectoryEntry(PxFCFILE file, __X unsigned char *lfn);
#else
int     ReadDirectoryEntry(PxFCFILE file);
#endif
int     ReadFileSystemInfo(unsigned int drive);
int     RetrieveLFN(PxFCFILE file, __X FILE_INFO *finfo);
int     BuildMBR(unsigned int drive);
int     BuildPBR(unsigned int drive);
int     CheckCluster(unsigned int drive, unsigned long cluster);
int     CheckFileCharacter(unsigned int c);
FS_ERR     FlushBuffer(unsigned int drive);

int     ReadBuffer(unsigned int drive, unsigned long sector);

struct _directory* _FindDirectory(PxFCFILE file);
#define FindDirectory  (PxDIRECTORY)_FindDirectory

struct _directory* SearchTrash( int drive );

unsigned long   AddNewCluster(unsigned int drive, unsigned long cluster, unsigned long possible_empty);
unsigned int    CheckEmptyClusters(unsigned int drive, unsigned long cluster, unsigned int num);
unsigned long   GetFATEntry(unsigned int drive, unsigned long cluster);
unsigned int    PBRSectorNum(unsigned int drive);


unsigned long   FirstSectorInCluster(unsigned int drive, unsigned long cluster);
unsigned long   BYTES_TO_DWORD(__X unsigned char *p);
void    WORD_TO_BYTES(__X unsigned char *p, unsigned int n);
void    DWORD_TO_BYTES(__X unsigned char *p, unsigned long n);
unsigned int    BYTES_TO_WORD(__X unsigned char *p);

int     ClearCluster(unsigned int drive, unsigned long cluster);
void    ReleaseFATEntries(PxFCFILE file);
void    ReleaseFileSlot(PxFCFILE file);
int     SearchFATTable( PxFCFILE file );
//void    DeleteDirectoryEntry(PxFCFILE file);
//void    RenameDirectoryEntry(PxFCFILE file);
#ifdef DOTDOT
void    UpdateDirectoryEntry(PxFCFILE file, int dir);
#else
void    UpdateDirectoryEntry(PxFCFILE file);
#endif
void    UpdateDotNDotDot(PxFCFILE file);
void    NameExtToSpec(__X unsigned char *spec, __X unsigned char *name_ext);
void    SearchBestCluster(PxFCFILE file);
void    SearchBestCluster(PxFCFILE file);
void    SetFATEntry(unsigned int drive, unsigned long cluster, unsigned long x);

void    GetDateNTime(struct _date_n_time *p);

int     FlushDrive(unsigned int drive);
FSINFO*  _GdrvGet_fsinfo( unsigned int drive );
void SetDirty( unsigned int drive );

#define GdrvGet_fsinfo (PxFSINFO)_GdrvGet_fsinfo

void SetFileSystemBusy(int drive);
void ClearFileSystemBusy(int drive);
int iChangeDrive(unsigned char drive);

void GdrvbufferSave1( unsigned int drive, unsigned int byte_offset, unsigned int value );
void GdrvbufferSave2( unsigned int drive, unsigned int byte_offset, unsigned int value );
void GdrvbufferSave4( unsigned int drive, unsigned int byte_offset, unsigned long value );
void GdrvbufferSets( unsigned int drive, unsigned int byte_offset, unsigned int value, unsigned int size );
unsigned int GdrvbufferLoad1( unsigned int drive, unsigned int byte_offset );

typedef int (*type_drv_write)(void* buffer);
int Gdrv_drv_write( unsigned int drive, void* buffer );
int Gdrv_drv_read( unsigned int drive, void* buffer );
int Gdrv_drv_write_buffer( unsigned int drive );
int Gdrv_drv_read_buffer( unsigned int drive );
typedef int (*type_drv_prepare)(unsigned long sector, unsigned int num);
int Gdrv_drv_prepare( unsigned int drive, unsigned long sector, unsigned int num );
typedef int (*type_drv_clear)(void);


#define RESET_WATCH_DOG_ASM
#undef _FAT32_ASM
#endif
