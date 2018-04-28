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

/*!@{
  \defgroup FATGroup FAT Compatible File System
   KBDM9 provides FAT16/FAT32 compatible fise system functions. \n
   - version 1.0\n\n
@}*/

//! @{	\ingroup FATGroup

/*!
- 1.	\b Overview \n
The FAT (File Allocation Table) file system has its origins in the late 1970s and early 1980s and was the file system supported by the MicrosoftR MS-DOSR operating system. It was originally developed as a simple file system suitable for floppy disk drives less than 500K in size. Over time it has been enhanced to support larger and larger media. Currently there are three FAT file system types: FAT12, FAT16 and FAT32. The basic difference in these FAT subtypes, and the reason for the names, is the size, in bits, of the entries in the actual FAT structure on the disk. There are 12 bits in a FAT12 FAT entry, 16 bits in a FAT16 FAT entry and 32 bites in a FAT32 FAT entry.
A FAT16 file system volume is composed of five basic regions: MBR, PBR, FAT, Root Entry Region, and Data Region. MBR (Master Boot Record) records the information of Cylinder, Head, and Sector of a hard-disk. The contents of MBR show the location of PBR sector and number of Last Sector. PBR (Partition Boot Record) means a starting sector of available sectors, which represents the format of file system. FAT (File Allocation Table) defines a single linked-list of the extents (clusters) of a file. The cluster is an elementary unit of allocation of a file, which is made up of one or more sectors. FAT is used to keep track of the physical position of data and locate free space for storing new files. Root Entry Region contains the directory structures in the root of a hierarchy (A file system is usually organized as a hierarchy or tree). Data Region contains the contents of sub-directories and files.
The file system organizes directories and files. Use of the file system in combination with driver software allows files to be stored in various types of storage media. Note that there is no built-in security or compression with the FAT file system.
This version of FAT library support reading the long file name attribute of a file. But no create or open file by long file name ability.

- 1.1.	\b File \n
A file is an element of data storage in a file system. In the FAT file system, it is composed of filename, file size, attribute, date & time stamps, and contents of data.


- 1.1.1.	\b File \b Specification  \n
In the FAT file system, the file specification is composed of a name and an extension that are separated by a period ("."). Every name is truncated to eight characters, and every extension is truncated to three characters. Moreover, case is not significant in file specifications. Hence "MYFILENAME.EXTENSION", "MYFILENA.EXT", and "myfilena.ext" all represent the same file. The file specification used to open a file should be use short file name. The lfn name in file information is only used for display the long file name only.

- 1.1.2.	\b Open  \b File \n
You MUST open a file before accessing it. You can open a file as three kinds of mode: CREATE, READ, and READ/WRITE. CREATE mode is that delete the file if it already exists and create it for writing. READ mode is that open the file for reading. READ/WRITE mode is that open the file for reading and writing.

- 1.1.3.	\b Close \b File \n
If a file has been written or otherwise modified, you MUST close it in order to flush buffer to the file and update its directory entry.

- 1.1.4.	\b Read \b Data \n
Reading from a file starts at the current file pointer position. If the operation is successful, the file pointer is advanced to the byte immediately following the last bytes read.

- 1.1.5.	\b Write \b Data \n
Writing to a file starts at the current file pointer position. If the operation is successful, the file pointer is advanced to the byte immediately following the last byte written. If necessary, the file is lengthened to accommodate the bytes being written. Writing zero length of data to a file also causes the FAT file system to update the directory when the file is later closed.

- 1.1.6.	\b Delete \b File \n
Deleting a file is to release clusters of the FAT file system as free space that is for future writing. Actually, deleting a file in the FAT file system is just to mark the first byte of the filename. So you SHOULD recover the contents of a file immediately when you made a mistake to delete it.

- 1.1.7.	\b File \b Information \n
The file information includes the filename, file size, attribute, and date-time stamps. The filename is described in "file specification" section, and the others are described as follows: \n
The file size is the length of a file in bytes. A file with zero size is permitted. In the FAT file system, a file is made up of a whole number of possibly non-contiguous clusters, so the occupied size of a file MAY larger than its actual size.\n
The file attribute indicates the properties of a file or directory. There are six attributes of a file: ARCHIVE, READONLY, HIDDEN, SYSTEM, DIRECTORY, and VOLUME. ARCHIVE describes the file is an archive file that is used to be marked for backup or removal by applications. READONLY describes the file or directory is read-only that cannot be modified or deleted. HIDDEN describes the file is hidden treated by operation system and various applications. SYSTEM describes the file is essentially important for proper functioning of system. It is usually set by operation system or some other programs. DIRECTORY indicates the entry is a directory. VOLUME indicates the entry is a drive volume. You are just permitted to modify the anterior four attributes.\n
The date-time stamps record the created or last modified date and time of files and directories. It is usually set by the file system.\n
The long file name can be accessed from the file information that get from FSGetEntry or FSGetNextEntry.\n
The lfn entry in the struct FILE_INFO is the Unicode file name of the found file.

- 1.2.	\b Directory \n
A directory is a container that contains zero or more files or other directories in hierarchical file system. Many graphical user interfaces use the term folder instead of directory. The topmost directory in any file is called the root directory. A directory that is below another directory is called a subdirectory. A directory that is above a subdirectory is called the parent directory.

- 1.2.1.	\b Directory \b Specification \n
In the FAT file system, a directory specification is the same as a file specification, except that it refers to a subdirectory. Since there is no "current directory", the file specification must begin at the root and contain the names of all intermediate subdirectories that are separated by backslashes ("\"). You do this by specifying a path. KB's FAT Compatible File System doesn't permit the special subdirectories "." and ".." in a directory path, but to maintain disk-level compatibility with WindowsR, it puts them into subdirectories that it creates.\n
The root directory is the directory that includes all other directories. The FAT file systems have only one root directory that has no name, and it is represented as a backslash ("\"). In KB's FAT Compatible File System, you can specify a file at the root directory with simple filename rather than prefix a root character.

- 1.2.2.	\b Create \b Directory \n
Due to there is a limitation that the root is able to contain at most 512 entries including files and directories, you need to create new subdirectories to store more files. You cannot specify a directory which name is the same as that of any certain files or directories.

- 1.2.3.	\b Delete \b Directory \n
In the FAT file system, the directory must be empty before it can be deleted. That means there cannot be any files or subdirectories below the parent directory to be deleted.


- 1.3.	\b Drive \n
A drive is a logical partition or disk that is managed by a file system. In the FAT file system, it usually uses a letter to represent a drive. To lay the FAT file system on a flash, the flash access routines must simulate the flash as a hard-disk.


- 1.3.1.	\b Drive \b Specification \n
Use a digital character ("0"~"9") which is called drive letter and a colon sign (":") followed to represent a drive. You must add a drive specification as the prefix to the file or directory path if multiple drives were supported. You can omit the drive specification in a single drive system. In other cases, the FAT file system used by WindowsR uses alphabetic letters to represent drives.

- 1.3.2.	\b Volume \b Label \n
The volume label is an identification of drive for users. The FAT file system identifies a drive according to its drive letter rather than volume label. It may consist of up to eleven characters (the same characters that are legal for a filename).

- 1.4.	\b Limitations \n
KB's FAT Compatible File System is a part of the FAT16 file system. There are some features not ported in the current version.

- Limitations 1 ：	It doesn't support partition \n
KB's FAT Compatible File System is designed mostly for laying on NAND flashes. It doesn't make sense to support partition for NAND flashes came out presently in the world.

- Limitations 2 ： 	Multi-drive is dependent on storage drivers \n
The multi-drive feature is supported by calling the function that changes drives implemented by storage drivers.

- Limitations 3 ： 	File can't be created or opened by long file name \n
The long file name attribute can be extract from file information that got from KB's FAT library.  But this version of FAT library can only read the long file name that created from windows mass storage driver. Application can't create file by long file name. FSRename also can't rename a file that with long file name.


*/

//! \defgroup OverviewGroup FAT Overview


//! \ingroup FATGroup
/*!
The FAT file system is independent of the underlying storage media. However, it was originally developed for floppy and hard disk drives, the flash drivers are necessary to simulate the operations of a hard disk.\n
Put fsconfig_kbmp3.c with your project files properly. Besides, you SHALL prepare the program of preferred storage media (e.g., NAND flashes), and copy them all into where you can refer to with your project. \n
You may need to modify fsconfig_kbmp3.c to set the file system up and configure the interface between the file system and the underlying storage driver. It is NOT recommended to modify what are not listed below.\n
-	File System Setup \n
1.	FOPEN_MAX is the maximum opened files in the same time. You may customize its value for your need.\n
2.	DRV_COUNT is the number of drives (storage media) in fact. You shall specify its value depended on how many drives attached.\n
3.	You MAY implement the ChangeDrive() function for multiple drives support.\n
4.	You MUST implement the MountDrive() function to initialize the specified drive and invoke ReadFileSystemInfo() to attempt to load information of its file system.\n
5.	You MUST implement the GetDateNTime() function to provide the date and time. In a non-OS environment (that is, no timer ticking), you shall make pseudo one.\n\n

-	Storage Software Interface\n
The interface is defined in fs.h. You must provide function pointers and a buffer for buffering use as a structure array declared in fsconfig.c. The unit of accessing the storage media is in 'sector' and its size is defined in fs.h. It is normally 512 bytes. \n

 1.	int (*drv_clear)(void)\n
This function is responsible for making the media ready to access and erasing all data of the drive physically if necessary. It is invoked by FSFormat() which formats a drive as the FAT file system. It returns 0 if successful, and otherwise if failed.\n

 2.	int (*drv_prepare)(unsigned long sector, unsigned int num) \n
This function is responsible for storing the 'sector' number and reserving an amount of sectors specified by 'num' for later reading or writing. It returns 0 if successful, and otherwise if failed. \n

 3.	int (*drv_read)(unsigned char *buf)\n
This function is responsible for reading data in the size of a sector. When the read operation is completed, it must add 1 to the sector number for the next operation referring. It returns 0 if successful, and otherwise if failed.\n

 4.	int (*drv_write)(unsigned char *buf)\n
This function is responsible for writing data in the size of a sector. When the write operation is completed, it must add 1 to the sector number for the next operation referring. It returns 0 if successful, and otherwise if failed.\n

 5.	unsigned long (*drv_size)(void)\n
This function is responsible for providing the drive volume in sectors.\n

 6.	unsigned char *buffer\n
It points the address of a buffer which size is the size of a sector. It cannot be NULL.\n

*/

//! \defgroup ConfigurationGroup FAT Configuration


//! \ingroup FATGroup

//! @}

//! \defgroup FATErrGroup FAT Error Code Lists
//!@{  \ingroup FATGroup
/*!
Error message for FAT functions.
*/

typedef enum{
    FS_NO_ERROR              = 0  ,  /**< No error.     */
    FS_DRIVE_ERROR           = -1 ,  /**< An error occurres when accessing the drive. */
    FS_FORMAT_ERROR          = -2 ,  /**< The file system doesn't exist or the essential information is damaged. */
    FS_SYSTEM_ERROR          = -3 ,  /**< There is error in file system.*/
    FS_INVALID_DIRECTORY     = -4 ,  /**< There is a directory among the specification doesn't exist. */
    FS_INVALID_SPEC          = -5 ,  /**< The drive, directory, or file specification is invalid. */
    FS_INVALID_MODE          = -6 ,  /**< An invalid mode is specified. */
    FS_FILE_NOT_FOUND        = -7 ,  /**< 1. The file doesn't exist.  \n 2. The entry in directory is invalid. */
    FS_DIRECTORY_CONFLICT    = -8 ,  /**< The specification is a directory in fact. */
    FS_DIRECTORY_FULL        = -9 ,  /**< The capacity of specified directory is full. */
    FS_OPENFILE_FULL         = -10,  /**< The customized file descriptors are full. */
    FS_VOLUME_FULL           = -11,  /**< The capacity of system is full. */
    FS_ACCESS_DENIED         = -12,  /**< 1. The file is a read-only, hidden, or system file. \n 2. The file system is in write operation. */
    FS_SEEK_OUT_OF_RANGE     = -13,  /**< The seek poisition is out of file size. */
    FS_RENAMING_ERROR        = -14,  /**< The same name file already exists. */
    FS_UNRECOVERABLE_FILE    = -15,  /**< The file is not recoverable. */
    FS_END_DIRECTORY         = -16,  /**< The index is out of the directory. */
    FS_TRASH_ERROR           = -17,  /**< No trash directory  exists. */
    FS_PARAMETER_ERROR       = -18,  /**< Invalid parameter. */
    FS_FILE_INCONSISTENT     = -19,  /**< File size is not match between entry and links. */
    FS_WRITE_OUT_RANGE       = -20,  /**< Will write data to invalid sectors. */
    FS_TABLE_INCONSISTENT    = -21,
    FS_DRIVE_NOT_READY       = -22,
    FS_INVALID_CLUSTER       = -23
} FS_ERR;
//! @}
//typedef int FS_ERR ;

//! \defgroup FATFunGroup FAT Functions
//! @{\ingroup FATGroup
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

/*!< \struct _fcb
    File descriptor
*/
typedef struct _fcb {
    __X struct _fcb *   next;
    int        			used;               /**< in use or not */
    unsigned char       spec[14];
    unsigned char       attribute;      /**< open attributes */
    unsigned char       mode;
    struct _date_n_time date_n_time;
    unsigned int        sector_in_cluster;  /**< sector within cluster */
    unsigned long       first_cluster;      /**< first cluster in file */
    unsigned long       last_cluster;       /**< last cluster read/written */
    unsigned long       cluster;            /**< next cluster to read/write */
    unsigned long       position;           /**< read/write position */
    unsigned long       size;               /**< file size */

    unsigned long       dir_cluster;
    unsigned int        dir_index;
    unsigned long       dir_first_cluster;
    unsigned int        fmode;
    unsigned long       possible_cluster;
    unsigned int        entry_index;
    unsigned long       lfn_cluster;
    unsigned long       old_final_cluster;      /**< file end when open file */
    unsigned long       new_start_cluster;      /**< first when file append */
    unsigned int        *pcluster_nodes;
    unsigned int        node_count;
    unsigned int        skips_clusters;
    int        			drive;              /**< drive id */
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



/*!
Declare how many drives that will be attached to the FAT system.
@param MaxDrvCount   The number of drives
@note FSInit() must be called prior to calling any other FAT functions.
*/
void FSInit( void );

/*!
Format a drive as FAT16/FAT32 Compatible File System.
@param drive : which drive will be formated..
@return 0 is correct, others value refer to \ref FS_ERR.
@note 1. FSInit() must be called prior to calling FSFormat().
@note 2. You cannot call FSFormat() if FSInit() returns FS_DRIVE_ERROR.
*/
FS_ERR  FSFormat(unsigned int drive);

/*! Format a drive using FAT32 file-system\n
 @param  drive : which drive will be formated.
 @return  0 is correct, others value refer to \ref FS_ERR.
 @note       The total sectors of the disk suould be more than 10000.\n
             This function does not check this restrictions.
*/
FS_ERR  FSFormat32(unsigned int drive);

/*!
Open a specified file for later reading or writing.
@param spec	: the file specification string, with terminating '\ 0'.
@param mode	: the opened mode: FS_MODE_CREATE, FS_MODE_READ, or FS_MODE_READ_WRITE. The details are described in "FAT Overview -> Open Files" section.
@param err  : see \ref FS_ERR
@return NULL if an error occurred
@note You must open a file first before reading or writing operation.
*/
FCFILE* _FSOpen(char *spec, unsigned int mode, __X FS_ERR* err);

/*!
It opens a specified file for later reading or writing. And it will allocate/prepare some LARGE space buffer for speed up when writing file.
@param spec	: the file specification string, with terminating '\ 0'.
@param mode	: the opened mode: FS_MODE_CREATE, FS_MODE_READ, or FS_MODE_READ_WRITE. The details are described in "FAT Overview -> Open Files" section.
@param err  : see \ref FS_ERR
@return NULL if an error occurred
@note You must open a file first before reading from it or writing to it.
*/
FCFILE* _FSOpenForFastWrite(char *spec, unsigned int mode, __X FS_ERR* err);

/*!
It opens a specified file for later reading or writing. And it will read entry-links from FAT table, and put into cache_buffer according size of buffer.\n
It can improve speed of FSSeek because some links already in memory.
@param spec	: the file specification string, with terminating '\ 0'.
@param mode	: the opened mode: FS_MODE_CREATE, FS_MODE_READ, or FS_MODE_READ_WRITE. The details are described in "FAT Overview -> Open Files" section.
@param cache_buf : buffer pointer for cache, must be alignment in even.
@param buf_size : size of buffer, in bytes
@param err : see \ref FS_ERR
@return NULL if an error occurred
@note The cache buffer will be cast to integer pointer, so it must be alignment in even.
*/
FCFILE* _FSOpenForFastSeek (char *spec, unsigned int mode, __X void* cache_buf, unsigned int buf_size, __X FS_ERR* err );

/*!
It opens a specified file for later reading or writing. And it will read entry-links from FAT table, and put into cache_buffer according size of buffer.\n
It can improve speed of FSSeek because some links already in memory.
@param spec	: the file specification string, with terminating '\ 0'.
@param mode	: the opened mode: FS_MODE_CREATE, FS_MODE_READ, or FS_MODE_READ_WRITE. The details are described in "FAT Overview ->Open Files" section.
@param cache_buf : buffer pointer for cache, must be alignment in even.
@param buf_size : size of buffer, in bytes
@param skips_clusters : how many clusters between nodes, the value must be power of 2
@param err : see \ref FS_ERR
@return NULL if an error occurred
@note: The cache buffer will be cast to integer pointer, so it must be alignment in even.
*/
FCFILE* _FSOpenForFastSeekCustom(char *spec, unsigned int mode, __X void* cache_buf, unsigned int buf_size, unsigned int skips_clusters, __X FS_ERR* err );

//FCFILE* _FSOpenForUnicode (char *dir, unsigned char* unicode_name,  unsigned int mode, __X FS_ERR* ret_err);

/*!
using unicode to open/create file.
@param dir	:  - the file's directory specification string, null-terminated
@param short_name : the file's 8.3 format short name string, with null-terminated.
@param unicode_name	: the file's unicode specification string, with '\ 0 \ 0' terminate.,
@param mode	:  - the opened mode: only FS_MODE_READ, FS_MODE_READ_WRITE, FS_MODE_CREATE
@param ret_err	: see \ref FS_ERR
@return NULL if an error occurred \n

\n  使用範例\n
   fserr = FSGetEntry( "0:", 0, &finfo, &fentry );\n
    while(1){\n
        if( fserr == FS_NO_ERROR ){\n
            if( *(unsigned int*)finfo.lfn ){ //有長檔名\n
                fd1   = FSOpenForUnicode2( "0:", finfo.name, finfo.lfn, FS_MODE_CREATE, &fserr );\n
                fserr = FSClose(fd1);\n
            }\n
        }\n
        fserr = FSGetNextEntry( &finfo, &fentry );\n
        if( fserr == FS_END_DIRECTORY )\n
            break;\n
    }\n

*/
FCFILE* _FSOpenForUnicode2(char *dir, char* short_name, unsigned char* unicode_name, unsigned int mode, __X FS_ERR* ret_err);

/*!
Macro definition for calling _FSOpen
*/
#define FSOpen                  (__X FCFILE*)_FSOpen

/*!
Macro definition for calling _FSOpenForFastWrite
*/
#define FSOpenForFastWrite      (__X FCFILE*)_FSOpenForFastWrite

/*!
Macro definition for calling _FSOpenForFastWrite
*/
#define FSOpenForFastSeek       (__X FCFILE*)_FSOpenForFastSeek

/*!
Macro definition for calling _FSOpenForFastSeekCustom
*/
#define FSOpenForFastSeekCustom (__X FCFILE*)_FSOpenForFastSeekCustom
//#define FSOpenForUnicode        (__X FCFILE*)_FSOpenForUnicode

/*!
Macro definition for calling _FSOpenForUnicode2
*/
#define FSOpenForUnicode2       (__X FCFILE*)_FSOpenForUnicode2


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
 -   4.  1 sector always be 512Bytes\n
 -   5.  Put above all tegother. A file with 10MBytes = 10,485,760Bytes\n
        The it need \n
            10,485,760 / (   128 //every 128 clusters keep 1 entry
                          * gdrv[0].fsinfo->sectors_per_cluster
                          * 512 //1 sector = 512Bytes
                        )\n
            -> 20 entries in buffer (if the file is on disk 0,
               and 'gdrv[0].fsinfo->sectors_per_cluster' is '8')\n
            -> 40Bytes (1 entry = 2Bytes)\n

  -  6.  So, in this 10MBytes sample, you need give a "cache_buf" have
        at least 40Bytes and "buf_size" as 40
        Shortly, 512KBytes need 2Bytes (i.e., 1 entry)\n
*/
#define HowMuchBytesSeekBufferNeed(file, drive) ((unsigned int)(((file)->size)/((32768)*(gdrv[(drive)].fsinfo->sectors_per_cluster))))

/*!
It reads a certain amount of data from a specified file.
@param pfile	: the file descriptor that returned by FSOpen().
@param buf	: the buffer to be stored data.
@param len	: the length of data to be read.
@param err_arg : see \ref FS_ERR
@return ">= 0" show the length of data read in bytes.
@return Otherwise, see \ref FS_ERR \n
*/
unsigned int  FSRead (__X FCFILE *pfile, __X void *buf, unsigned int len, __X FS_ERR* err_arg);

/*!
It writes a certain amount of data to a specified file.
@param pfile	: the file descriptor that returned by FSOpen().
@param buf	: the data to be written.
@param len : the length of data to be written.
@param err_arg : see \ref FS_ERR
@return ">= 0" show the length of data written in byte.
@return Otherwise, see \ref FS_ERR \n
*/
unsigned int  FSWrite(__X FCFILE *pfile, __X void *buf, unsigned int len, __X FS_ERR* err_arg);

/*!
It flushes data from the buffer, updates the date-time stamps, and releases the file descriptor.
@param file	: the file descriptor that returned by FSOpen().
@return see \ref FS_ERR
@note You MUST close an opened file after access finished.
@sa FSFlush
*/
FS_ERR FSClose (__X FCFILE *file);

/*!
It flushes data from the buffer, updates the date-time stamps, but not close the file.
@param file	: the file descriptor that returned by FSOpen().
@return see \ref FS_ERR
@sa FSClose
*/
FS_ERR FSFlush (__X FCFILE *file);

/*!
將一個已經開啟的 file, 調整大小,可以利用來加速寫入\n
EX:\n
    開啟一檔案\n
    fd = FSOpen("0:\\mdata.bin", FS_MODE_CREATE, &err);\n
\n
    先調整保留為 5 MB 大小的\n
    FSResize( fd, 5UL*1024*1024 );  \n
\n
    寫入資料約 N bytes\n
    for( i=0; i<n1; ++i ){\n
        FSWrite(fd, (void*)0x10, 512, &err);\n
    }\n
\n
    最後調整為實際寫入的大小\n
    FSResize( fd, fd->position );  \n
    FSClose(fd);\n
@param     file --> file handle pointer.
@param	   new_size -->調整後的大小
@return see \ref FS_ERR
*/
FS_ERR FSResize(__X FCFILE *file, unsigned long new_size);

/*!
Repair file's link and size\n
 @param  file --> file handle pointer.
 @return 0 is correct, others value see \ref FS_ERR.
*/
FS_ERR FSRepair(__X FCFILE *file);

/*!
It moves the internal file pointer to a specified position.
@param file	: the file descriptor that returned by FSOpen().
@param offset : the position relative to the start of the file.
@return 0 is correct, others value see \ref FS_ERR.
@note The offset relative to the start of a file starts from 0.
*/
FS_ERR FSSeek(__X FCFILE *file, unsigned long offset);

/*!
It opens a specified file for later reading or writing. And it will read entry-links from FAT table, and put into cache_buffer according size of buffer.\n
It can improve speed of FSSeek because some links already in memory.\n
@param file	: non-NULL opened file handle.
@param cache_buf : buffer pointer for cache, must be alignment in even.
@param buf_size_in_bytes : size of buffer, in bytes
@param skips_clusters : how many clusters between nodes, the value must be power of 2, 0 will using default value: 128
@return 0 is correct, others value see \ref FS_ERR.
@note The cache buffer will be cast to integer pointer, so it must be alignment in even.
*/
FS_ERR FSSetFileFastSeek(__X FCFILE* file, unsigned int* cache_buf, unsigned int buf_size_in_bytes, unsigned int skips_clusters );

/*!
It deletes a file, subdirectory, or volume name depended on the 'spec'. Specify an empty string ("") to delete the volume name, a directory path to remove the subdirectory, and a file path to delete the file.
@param spec	: the file, directory, or drive specification string, with terminating '\ 0'.
@return 0 is correct, others value see \ref FS_ERR.
@note There cannot be any files or subdirectories below the parent directory to be deleted.
*/
FS_ERR FSDelete(char *spec);

/*!
It returns the size of specified file in byte.
@param spec	: the file specification string, with terminating '\ 0'.
@param err : see \ref FS_ERR.
@return The file size in byte if no error.
*/
unsigned long FSGetFileSize(char *spec, __X FS_ERR* err);

/*!
It returns the attribute of specified file or directory.
@param spec	: the file or directory specification string, with terminating '\ 0'.
@param err : see \ref FS_ERR.
@return Return the attribute value if no error.
*/
unsigned char FSGetAttribute(char *spec, __X FS_ERR* err);

/*!
Change the file attribute of certain file.
@param spec	: the file or directory specification string, with terminating '\ 0'.
@param attr : the new attribute. \n
            FS_ATTR_READONLY	\n
            FS_ATTR_ARCHIVE		\n
            FS_ATTR_HIDDEN

@return Return the attribute value if no error. The error code: \n
FS_NO_ERROR	: the operation is successful. \n
FS_DRIVE_ERROR	: an error occurred when accessing the drive. \n
FS_INVALID_SPEC	: the drive, directory, or file specification is invalid.\n
FS_FILE_NOT_FOUND	: the specified file or directory doesn't exist.\n
Others            : see \ref FS_ERR.
@note  The attribute setting is "binary-and" operation. I.e., given attribute will set to '1', otherwise it will set into '0'

*/
FS_ERR FSSetAttribute(char *spec, unsigned char attr);

/*!
It returns the date and time stamps of specified file or directory.
@param spec	: the file or directory specification string, with terminating '\ 0'.
@param stamp : the pointer to the date-time-stamps structure.

@return The returned value specify the error code: \n
FS_NO_ERROR	: the operation is successful. \n
FS_DRIVE_ERROR	: an error occurred when accessing the drive.\n
FS_INVALID_SPEC	: the drive, directory, or file specification is invalid.\n
FS_FILE_NOT_FOUND : the specified file or directory doesn't exist.\n
Others : see \ref FS_ERR.
*/
FS_ERR FSGetDateTime(char *spec, __X struct _date_n_time *stamp);

/*!
It creates a new subdirectory on the specified path.
@param spec	: the directory specification string, with terminating '\ 0'.
@return The returned value specify the error code:\n
FS_NO_ERROR	: the operation is successful.\n
FS_DRIVE_ERROR	: an error occurred when accessing the drive.\n
FS_INVALID_SPEC	: the drive or directory specification is invalid.\n
FS_DIRECTORY_CONFLICT : the name conflicts with file or subdirectory.\n
FS_DIRECTORY_FULL : insufficient directory or data space.\n
Others : see \ref FS_ERR.
*/
FS_ERR FSDirectory(char *spec);

/*!
using unicode to create a directory.
@param dir	:  - 新目錄所在的目錄.
@param short_name : 新目錄的8.3短檔名, 不能跟該目錄下有相同的 8.3 短檔名.
@param unidir_name	: 新目錄的unicode名稱, 不能有相同的長檔名.

@return see \ref FS_ERR

\n  使用範例\n
   const char test_lfn[] = { 0, 'T', o, 'e', 0, 's', 0, 't', 0, 'D', 0, 'i', 0, 'r', 0, 0 }; \n
   const char test_short[]= "tes~1"; \n

   //create "TestDir" \n
   fserr =  FSDirForUnicode( "0:", test_short, test_lfn ); \n
*/
FS_ERR FSDirForUnicode(char* dir, char* short_name, unsigned char* unidir_name );

/*!
It returns the file information by specifying a directory path and entry index.
@param spec	: the directory specification string, with terminating '\ 0'.
@param index : the entry index at the specified subdirectory.
@param finfo : the pointer to the FILE_INFO structure.
@param fentry : the pointer to the FS_ENTRY_INFO structure.
@return The returned value specify the error code:\n
FS_NO_ERROR	: the operation is successful.\n
FS_DRIVE_ERROR	: an error occurred when accessing the drive.\n
FS_INVALID_SPEC	: the drive or directory specification is invalid.\n
FS_DIRECTORY_FULL	: the specified entry index is invalid.\n
FS_INVALID_DIRECTORY : there is a directory among the specification doesn't exist.\n
FS_FORMAT_ERROR	: there are errors in the file system.\n
FS_FILE_NOT_FOUND : the specified entry is deleted.\n
FS_END_DIRECTORY : the index is out of the directory\n
Others : see \ref FS_ERR.
@note 1. It can be used to list all files and subdirectories below a specified parent directory by increasing the entry index.
@note 2. If err is FS_FILE_NOT_FOUND, maybe the file entry is a deleted file or others, it not means the end of the directory. Please check if err is FS_END_DIRECTORY to ensure the end of the directory.
@note 3. It is recommended that calls FSGetNextFile() repeatedly after FSGetFile() is invoked once.
@note 4. The long file name can be got from finfo->lfn. That is a Unicode string.
*/
FS_ERR FSGetEntry(char *spec, unsigned int index, __X FILE_INFO *finfo, __X FS_ENTRY_INFO* fentry);

/*!
It returns the file information by specifying a directory path, entry index and wanted associate file.
@param spec	: the directory specification string, with terminating '\ 0'.
@param index : the entry index at the specified subdirectory.
@param finfo : the pointer to the FILE_INFO structure.
@param select : wanted associate file name.
@param fentry : the pointer to the FS_ENTRY_INFO structure.
@return The returned value specify the error code: \n
FS_NO_ERROR	- the operation is successful.\n
FS_DRIVE_ERROR	- an error occurred when accessing the drive.\n
FS_INVALID_SPEC	- the drive or directory specification is invalid.\n
FS_DIRECTORY_FULL	- the specified entry index is invalid.\n
FS_INVALID_DIRECTORY	- there is a directory among the specification doesn't exist.\n
FS_FORMAT_ERROR	- there are errors in the file system.\n
FS_FILE_NOT_FOUND	- the specified entry is deleted.\n
FS_END_DIRECTORY	- the index is out of the directory\n
Others : see \ref FS_ERR.
@note 1. It is similar to FSGetEntry, but it can be used to enclosing the associate name of file.
@note 2. If papameter is NULL or empty string, this function is the same as FSGetEntry.
@note 3. After call FSGetSelEntry, and then call FSGetNextEntry will get the file matched.
*/
FS_ERR FSGetSelEntry(char *spec, unsigned int index, __X FILE_INFO *finfo, char* select, __X FS_ENTRY_INFO* fentry );

/*!
It returns the next file information. The long file name can be got from finfo->lfn. That is an Unicode string.\n
The usage of this API is similar to FSGetEntry, see example.
@param finfo : the pointer to the FILE_INFO structure.
@param fentry : the pointer to the FS_ENTRY_INFO structure.
@return see \ref FS_ERR.
*/
FS_ERR FSGetNextEntry(__X FILE_INFO *finfo, __X FS_ENTRY_INFO* fentry);

/*!
It rename file in the same directory. We cannot rename/move file to other directory.
@param old_spec	: the directory specification string, with terminating '\ 0'.
@param new_spec	: new name specification string, with terminating '\ 0'. No directory path included.
@return The returned value specify the error code:\n
FS_NO_ERROR	: the operation is successful.\n
FS_DRIVE_ERROR	: an error occurred when accessing the drive.\n
FS_INVALID_SPEC	: the drive or directory specification is invalid.\n
FS_DIRECTORY_CONFLICT	: the name conflicts with file or subdirectory.\n
FS_DIRECTORY_FULL	: insufficient directory or data space.\n
Others : see \ref FS_ERR.
*/
FS_ERR FSRename(char *old_spec, char *new_spec);

/*!
It returns the free space of specified drive in sectors.
@param drive : the drive number that is an integer from 0 to 9.
@param err : reference error code table
@return The free space in sector if no error. One sector has 512 bytes. The error code:\n
FS_NO_ERROR	: the operation is successful.\n
FS_DRIVE_ERROR	: an error occurred when accessing the drive.\n
Others : see \ref FS_ERR.
*/
unsigned long FSFreeSpace(unsigned int drive, __X FS_ERR *err);



/*!
Set the volume of the drive
@param drive : the drive specification id.
@param name : the charactot length is 12-byte, including the terminating '\ 0'
@param  flag : 0 --> get volume ; 1 --> set volume
*/
FS_ERR FSVolume(unsigned int drive, char *name, int flag);

//! @}
unsigned long FSDriveSize(unsigned int drive, __X FS_ERR *err);

//=============================================================================
// MACRO DEFINE
//=============================================================================

#define FS_BUSY_FLAG    0x0001u     // bit 0: File System busy flag.
#define FLASH_BUSY_FLAG 0x0002u	    // bit 1: Trans-Layer busy flag.
#define BUS_BUSY_FLAG   0x0008u     // bit 3: Bus busy flag.
typedef struct {
    struct _drive Drv;
    FS_ERR (*pMountFunc)(void);
}tFSDrvHandle;

extern _RAM_AREA unsigned int STORAGEFLAGS;
extern _RAM_AREA struct _drive _drives_xram[];
extern const tFSDrvHandle SD1_Drive;
extern const tFSDrvHandle SD2_Drive;
extern const tFSDrvHandle SD1_Drive_2k;
extern const tFSDrvHandle SD2_Drive_2k;
extern const tFSDrvHandle SD1_Drive_4k;
extern const tFSDrvHandle SD2_Drive_4k;
extern const tFSDrvHandle SD1_Drive_8k;
extern const tFSDrvHandle SD2_Drive_8k;
extern const tFSDrvHandle SPIF_Drive;

typedef void (*tStorageBusyCallBackFunc)(void);
extern int FSCloseAll(void);
extern FS_ERR FS_mount( int Drv, const tFSDrvHandle *pdrive );
extern int IsFileSystemBusy(int drive);

extern void SetFileSystemBusy(int drive);
extern void ClearFileSystemBusy(int drive);

extern _RAM_AREA tStorageBusyCallBackFunc pStorageBusyCallBackFunc;


#endif /* _FS_H_ */
