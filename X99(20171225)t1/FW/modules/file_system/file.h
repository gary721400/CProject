//! @file file.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the file access interface.
//!
//! @version 1.18 snd3-dvk-1_9_5 $Id: file.h,v 1.18 2007/08/30 09:24:35 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _FILE_H_
#define _FILE_H_

#include "fs_com.h"
//#include "fat.h"
#include "fs.h"

//_____ D E F I N I T I O N S ______________________________________________

//! \name file_seek() options parameter
//! @{
#define  FS_SEEK_SET       0x00  //!< start at the beginning
#define  FS_SEEK_END       0x01  //!< start at the end of file and rewind
#define  FS_SEEK_CUR_RE    0x02  //!< start at the current position and rewind
#define  FS_SEEK_CUR_FW    0x03  //!< start at the current position and foward
//! @}

//! \name Struture to define a physical memory segment
//! @{
typedef struct {
   U8   u8_lun;            //!< logical unit(drive) number
   U32  u32_addr;          //!< segment address (unit sector)
   U16  u16_size;          //!< segment size (unit sector), or IN parameter for file_write() and file_read() routines
} Fs_file_segment;
//! @}

#define  FS_MAX_NAME_BUFFER   512
//_____ D E C L A R A T I O N S ____________________________________________


//! This fonction select the UNICODE mode for the routines getc() and putc()
//!
void  file_string_unicode( void );

//! This fonction select the ASCII mode for the routines getc() and putc()
//!
void  file_string_ascii( void );

//! This fonction check if a file is selected and it is not a directory
//!
//! @return    TRUE, a file is selected
//! @return    FALSE, otherwise
//!
Bool  file_ispresent( void );

//! This fonction open the file selected
//!
//! @param     fopen_mode  option to open file : <br>
//!                        FOPEN_MODE_R         R   access, flux pointer = 0, size not modify <br>
//!                        FOPEN_MODE_R_PLUS    R/W access, flux pointer = 0, size not modify <br>
//!                        FOPEN_MODE_W         W   access, flux pointer = 0, size = 0 <br>
//!                        FOPEN_MODE_W_PLUS    R/W access, flux pointer = 0, size = 0 <br>
//!                        FOPEN_MODE_APPEND    W   access, flux pointer = at the end, size not modify <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  file_open( U8 fopen_mode );

//! This fonction return a continue physical memory segment corresponding at a file to read
//!
//! @param     segment  Pointer on the segment structure: <br>
//!                     ->u32_size_or_pos    IN,   shall containt the maximum number of sector to read in file (0 = unlimited)
//!                     ->u32_size_or_pos    OUT,  containt the segment size (unit sector)
//!                     ->other              IN,   ignored
//!                     ->other              OUT,  containt the segment position
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine is interesting to read a file via a DMA and avoid the file system decode
//! because this routine return a physical memory segment without File System data.
//! Note: the file can be fragmented and it can be necessary to call several time file_read().
//! @endverbatim
//!
Bool  file_read( Fs_file_segment _MEM_TYPE_SLOW_ *segment );

//! This fonction copy in data buffer the data file corresponding at the current position
//!
//! @param     buffer         buffer to fill
//! @param     u16_buf_size   buffer size
//!
//! @return    number of byte read
//! @return    0, in case of error
//!
U16   file_read_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size );

//! This fonction copy in a data buffer the current line of open file
//!
//! @param     string         string to fill
//! @param     u16_str_size   string size (unit ASCII or UNICODE)
//!
//! @return    number of byte read
//! @return    0, in case of error
//!
//! @verbatim
//! Before, if you want get a UNICODE string then call file_string_unicode().
//! This routine write in string the back line character.
//! @endverbatim
//!
U16   file_gets( U8 _MEM_TYPE_SLOW_ *string , U16 u16_str_size );

//! This fonction return the next character at current file position
//!
//! @return    The character reading (UNICOCE, or ASCII storage in LSB)
//! @return    EOF, in case of error or end of file
//!
//! @verbatim
//! Before, if you want get a UNICODE character then call file_string_unicode().
//! This routine return ALL characters.
//! @endverbatim
//!
U16   file_getc( void );

//! This fonction return a continue physical memory segment corresponding at a file to write
//!
//! @param     segment  Pointer on the segment structure: <br>
//!                     ->u32_size_or_pos    IN,   shall containt the maximum number of sector to write in file (0 = maximum)
//!                     ->u32_size_or_pos    OUT,  containt the segment size (unit sector)
//!                     ->other              IN,   ignored
//!                     ->other              OUT,  containt the segment position
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine is interesting to write a file via a DMA and avoid the file system decode
//! because this routine return a physical memory segment without File System data.
//! Note: the file can be fragmented and it can be necessary to call several time file_write().
//! @endverbatim
//!
Bool  file_write( Fs_file_segment _MEM_TYPE_SLOW_ *segment );

//! This fonction set the end of file at the current position
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine is use after the last file_write() call.
//! The file_write() routine work on sector (512B) unit,
//! and you can set a Byte size with file_seek() and this routine.
//! @endverbatim
//!
Bool  file_set_eof( void );

//! This fonction copy the data buffer in file at the current position
//!
//! @param     buffer         data buffer
//! @param     u16_buf_size   data size
//!
//! @return    number of byte write
//! @return    0, in case of error
//!
U16   file_write_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size );

//! This fonction copy the string in the file at the current position
//!
//! @param     string         string to write (NULL terminate)
//!
//! @return    number of byte write
//! @return    0, in case of error
//!
//! @verbatim
//! Before, if you want write a UNICODE string then call file_string_unicode().
//! @endverbatim
//!
U16   file_puts( U8 _MEM_TYPE_SLOW_ *string );

//! This fonction write a character in the file at current position
//!
//! @param     u16_byte    character to write in the file (ASCII in LSB or UNICODE)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! Before, if you want write a UNICODE character then call file_string_unicode().
//! @endverbatim
//!
Bool  file_putc( U16 u16_byte );

//! This fonction return the position in the file
//!
//! @return    Position in file
//!
U32   file_getpos( void );

//! This fonction change the position in file selected
//!
//! @param     u32_pos     number of byte to seek
//! @param     u8_whence   direction of seek <br>
//!                        FS_SEEK_SET   , start at the beginning and foward <br>
//!                        FS_SEEK_END   , start at the end of file and rewing <br>
//!                        FS_SEEK_CUR_RE, start at the current position and rewing <br>
//!                        FS_SEEK_CUR_FW, start at the current position and foward <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  file_seek( U32 u32_pos , U8 u8_whence );

//! This fonction test if the position is at the beginning of file 
//!
//! @return    1     the position is at the beginning of file
//! @return    0     the position isn't at the beginning of file
//! @return    FFh   error
//!
U8    file_bof( void );

//! This fonction test if the position is at the end of file
//!
//! @return    1     the position is at the end of file
//! @return    0     the position isn't at the end of file
//! @return    FFh   error
//!
U8    file_eof( void );

//! This fonction flush the data include in file and information about file
//!
void  file_flush( void );

//! This fonction close the file
//!
void  file_close( void );

Bool  file_prepare_all_cluster_lists   ( void );

typedef enum
{
   Fat_Empty_Cluster,
   Fat_Valid_Cluster,
   Fat_End_Cluster,
   Fat_Invalid_Cluster
}FatCluster;

/*
typedef struct
{
	U16	disk_id;
	U32	start_cluster;
	U32	offset_by_clus;
	U32	cluster;
	U32	seg_len;
}ClusterListItem;
*/

#if( FAT_CLUSTER_BUF == ENABLE )
extern   __X   ClusterListItem   cluster_buf[ FS_NB_CACHE_CLUSLIST ];
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

#define  file_getsize()	      (( *fat_g_cur_file )->size )

extern   FCFILE         __X   *fat_g_files[ FS_NB_NAVIGATOR ];
extern   FILE_INFO      __X   fat_g_navs[ 1 ];
extern   FS_ENTRY_INFO  __X   fat_g_entry[ 1 ];
extern   FCFILE         __X   **fat_g_cur_file;
extern   FILE_INFO      __X   *fat_g_cur_nav;
extern   FS_ENTRY_INFO  __X   *fat_g_cur_entry;
extern   FCFILE         __X   *adb_g_file;

FS_ERR FSBufSeek  ( U32 new_position );
Bool  fs_seek     ( U32 addr );
Bool  FSCreateBuf ( void );

#endif  // _FILE_H_
