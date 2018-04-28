//! @file file.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the file access interface.
//!
//!
//! @version 1.33 snd3-dvk-1_9_5 $Id: file.c,v 1.33 2007/10/11 13:36:15 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _file_c_
#define _file_c_

//_____  I N C L U D E S ___________________________________________________
#include "lib_mcu\compiler.h"
#include "conf\conf_explorer.h"
#include "file.h"
#include "navigation.h"
//#include "lib_system\memory\memory.h"
//#include "modules\control_access\ctrl_access.h"

#include "conf\conf_sio.h"

#include "lib_mcu\timer\timer_drv.h"
#include "fs.h"
#include "fs_patch.h"
#include "fc_internal.h"
#include "sio\com.h"

#pragma DATA = FILE_DATA
#pragma CODE = FILE_CODE

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________

//! Use the "FAT sector cache" to store a file sector during a file_putc() or file_getc() routines.
//extern   _MEM_TYPE_SLOW_   U8    fs_g_sector[ FS_CACHE_SIZE ];

//! Internal flag to signal the string format (ASCII, UNICODE) to use for getc() and putc() routines.
//Bool     g_b_file_unicode  =  FALSE;

//_____ D E C L A R A T I O N S ____________________________________________

// Sub routine to store the internal segment structure to other structure
//static   void  file_load_segment_value( Fs_file_segment _MEM_TYPE_SLOW_ *segment );

FCFILE         __X   *fat_g_files[ FS_NB_NAVIGATOR ];
FILE_INFO      __X   fat_g_navs[ 1 ];
FS_ENTRY_INFO  __X   fat_g_entry[ 1 ];
FCFILE         __X   **fat_g_cur_file;
FILE_INFO      __X   *fat_g_cur_nav;
FS_ENTRY_INFO  __X   *fat_g_cur_entry;

FCFILE         __X   *adb_g_file;
#if( FAT_CLUSTER_BUF == ENABLE )
__X   ClusterListItem   cluster_buf[ FS_NB_CACHE_CLUSLIST ];

FatCluster  kb_fat_check_cluster ( FSINFO *fs_info, U32 cluster );
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

//**********************************************************************
//********** String format select for gets, getc, puts, putc ***********
#if 0
//! This fonction select the UNICODE mode for the routines getc() and putc()
//!
void  file_string_unicode( void )
{
   g_b_file_unicode = TRUE;
}

//! This fonction select the ASCII mode for the routines getc() and putc()
//!
void  file_string_ascii( void )
{
   g_b_file_unicode = FALSE;
}


//! This fonction check if a file is selected and it is not a directory
//!
//! @return    TRUE, a file is selected
//! @return    FALSE, otherwise
//!
Bool  file_ispresent( void )
{
   if( !fat_check_mount_select() )
      return FALSE;
   return fat_check_is_file();
}


#define  LARGE_FILE_SIZE      (( U32 )100 * 1024 * 1024 )
void prepare_for_large_file( void )
{
   U8 iter;
   extern _MEM_TYPE_SLOW_ Fs_clusterlist_cache fs_g_cache_clusterlist[ FS_NB_CACHE_CLUSLIST ];
   if( fs_g_nav_entry.u32_size >= LARGE_FILE_SIZE )
   {
      for( iter = 0; iter < 3; ++iter )
      {
         fs_g_cluster.u32_pos = fs_g_nav_entry.u32_cluster + ( fs_g_nav_entry.u32_size *( iter + 1 )/ 4 + fs_g_nav.u8_BPB_SecPerClus * FS_512B - 1 )/( fs_g_nav.u8_BPB_SecPerClus * FS_512B )- 1;
         if( fat_cluster_val( 0 ))
         {
            if( fs_g_cluster.u32_val != fs_g_cluster.u32_pos + 1 )
               return;
         }
      }
      fs_g_cluster.u32_pos = fs_g_nav_entry.u32_cluster + ( fs_g_nav_entry.u32_size + fs_g_nav.u8_BPB_SecPerClus * FS_512B - 1 )/( fs_g_nav.u8_BPB_SecPerClus * FS_512B )- 1;
      if( !fat_cluster_val( 0 ))
         return;
      if( fat_checkcluster() == FS_CLUS_END )
      {
         //tx_send_string("get cluster list\r\n");
         fs_g_cache_clusterlist[ 0 ].u8_lun      = fs_g_nav.u8_lun;
         fs_g_cache_clusterlist[ 0 ].u32_cluster = fs_g_nav_entry.u32_cluster;
         fs_g_cache_clusterlist[ 0 ].u32_size    = ( fs_g_nav_entry.u32_size + FS_512B - 1 )/ FS_512B;
         fs_g_cache_clusterlist[ 0 ].u32_start   = 0;
         fs_g_cache_clusterlist[ 0 ].u32_addr    = (( fs_g_nav_entry.u32_cluster - 2 )* fs_g_nav.u8_BPB_SecPerClus )+ fs_g_nav.u32_ptr_fat + fs_g_nav.u32_offset_data;
      }
      /*else
         tx_send_string("get cluster error\r\n");*/
   }
}
#endif

#pragma CODE = FILE_TEMP_CODE00

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
Bool  file_open( U8 fopen_mode )
{
   unsigned int	_MEM_TYPE_SLOW_	mode;
   FS_ERR	_MEM_TYPE_SLOW_	error;
   
   if( fopen_mode == FOPEN_MODE_R )
      mode = FS_MODE_READ;
   else if( fopen_mode == FOPEN_MODE_R_PLUS )
      mode = FS_MODE_READ_WRITE;
   else if( fopen_mode == FOPEN_MODE_W_PLUS )
      mode = FS_MODE_CREATE;
   else
      mode = FS_MODE_READ;
   *fat_g_cur_file = ( FCFILE __X *)FSOpenEntry(( __X FILE_INFO *)fat_g_cur_nav, ( __X FS_ENTRY_INFO *)fat_g_cur_entry, mode, &error );
	if( error )
      return	FALSE;
   return TRUE;
}


#if 0//(( AP3 == ENABLE )||( PASTER == ENABLE ))
//! This fonction load the global segment in param segment
//!
//! @param     segment  Pointer on the segment structure to load
//!
static void file_load_segment_value( Fs_file_segment _MEM_TYPE_SLOW_ *segment )
{
   segment->u8_lun = fs_g_nav.u8_lun;
   segment->u32_addr = fs_g_seg.u32_addr;
   segment->u16_size = fs_g_seg.u32_size_or_pos;
}
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))


#if 0
//! This fonction return a continue physical memory segment corresponding at a file to read
//!
//! @param     segment  Pointer on the segment structure: <br>
//!                     ->u32_size_or_pos    IN,   shall containt the maximum number of sector to read in file (0 = unlimited) <br>
//!                     ->u32_size_or_pos    OUT,  containt the segment size (unit sector) <br>
//!                     ->other              IN,   ignored <br>
//!                     ->other              OUT,  containt the segment position <br>
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
Bool  file_read( Fs_file_segment _MEM_TYPE_SLOW_ *segment )
{
   U8 u8_nb_sector_truncated;

   if( !fat_check_mount_select_open())
      return FALSE;

   if(!(FOPEN_READ_ACCESS & fs_g_nav_entry.u8_open_mode))
   {
      fs_g_status = FS_ERR_WRITE_ONLY;
      return FALSE;
   }
   
   if ( file_eof() )
   {
      // End of the file
      fs_g_status = FS_ERR_EOF;
      return FALSE;
   }

   if( !fat_read_file(FS_CLUST_ACT_SEG))
   {
      if( FS_ERR_OUT_LIST == fs_g_status )
         fs_g_status = FS_ERR_EOF;  // translate the error
      return FALSE;
   }
   // Truncate the segment found if more larger than size asked
   if( (segment->u16_size != 0)  // if not undefine limit
   &&  (segment->u16_size < fs_g_seg.u32_size_or_pos) )
   {
      u8_nb_sector_truncated   = fs_g_seg.u32_size_or_pos - segment->u16_size;
      fs_g_seg.u32_size_or_pos = segment->u16_size ;
   }else{
      u8_nb_sector_truncated = 0;
   }

   // Update position file
   fs_g_nav_entry.u32_pos_in_file += (U32)fs_g_seg.u32_size_or_pos * FS_512B;
   if( fs_g_nav_entry.u32_size < fs_g_nav_entry.u32_pos_in_file )
   {  
      // Limit the read
      // FYC: this case is posible, if the end of file isn't at the end of cluster
      // compute the sector not used by file      
      U8 u8_nb_sector_not_used;
      
      // Compute the number of sector used in last cluster
      // remark: also the two first byte of size is used, because the cluster size can't be more larger
      u8_nb_sector_not_used = LSB1( fs_g_nav_entry.u32_size ) >> (FS_512B_SHIFT_BIT-8);
      if( 0 != (fs_g_nav_entry.u32_size & FS_512B_MASK) )
      {  // last sector of file isn't full, but it must be read
         u8_nb_sector_not_used++;
      }

      // Compute the number of sector not used in last cluster
      u8_nb_sector_not_used = fs_g_nav.u8_BPB_SecPerClus - (u8_nb_sector_not_used % fs_g_nav.u8_BPB_SecPerClus);
      // if all space of cluster isn't used, then it is wrong
      if( u8_nb_sector_not_used == fs_g_nav.u8_BPB_SecPerClus )
         u8_nb_sector_not_used = 0; // The file use all last cluster

      // subtract this value a the position and segment
      u8_nb_sector_not_used -= u8_nb_sector_truncated;
      fs_g_seg.u32_size_or_pos -= u8_nb_sector_not_used; // Unit sector
      fs_g_nav_entry.u32_pos_in_file -= ((U16)u8_nb_sector_not_used) << FS_512B_SHIFT_BIT;   // unit byte
   }
   file_load_segment_value( segment );
   return TRUE;
}
#endif

#pragma CODE = FILE_CODE

//! This fonction copy in data buffer the data file corresponding at the current position
//!
//! @param     buffer         buffer to fill
//! @param     u16_buf_size   buffer size
//!
//! @return    number of byte read
//! @return    0, in case of error
//!
U16   file_read_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size )
{
   FS_ERR	_MEM_TYPE_SLOW_	error;
   return	( U16 )FSRead(( __X FCFILE *)*fat_g_cur_file, (void __X *)buffer, u16_buf_size, &error );
}

#if 0
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
U16   file_gets( U8 _MEM_TYPE_SLOW_ *string , U16 u16_str_size )
{
   U16 u16_save_buf_size;
   U16 u16_nb_read;

   if( g_b_file_unicode )
   {
      u16_nb_read = file_read_buf( string , u16_str_size*2 );
      u16_nb_read /= 2;
   }
   else
   {
      u16_nb_read = file_read_buf( string , u16_str_size );
   }
   if( 0 == u16_nb_read )
      return 0;

   u16_save_buf_size = u16_str_size;

   // Search end of line
   while( 1 )
   {                      
      // Increment the number of character read
      u16_nb_read--;
      u16_str_size--;

      if( (( g_b_file_unicode) && ('\0'  == ((FS_STR_UNICODE)string )[0]))
      ||  ((!g_b_file_unicode) && ('\0'  == string [0])) )
      {
         // End of texte file
         break;
      }
      
      if( 0 == u16_str_size )
      {
         // string full (u16_str_size = u16_nb_read = 0)
         u16_nb_read = u16_str_size = 1;              // Decrement the number of character read
         break;
      }

      if( (( g_b_file_unicode) && ('\n'  == ((FS_STR_UNICODE)string )[0]))
      ||  ((!g_b_file_unicode) && ('\n'  == string [0]))
      ||  ( 0 == u16_nb_read ) )
      {  
         // End of line
         string += (g_b_file_unicode? 2 : 1 );  // Go to next character
         break;
      }
      string += (g_b_file_unicode? 2 : 1 );  // Go to next character
   }
   // add a '\0' at the end of string
   if( g_b_file_unicode )
   {
      ((FS_STR_UNICODE)string )[0] = '\0';
      file_seek( u16_nb_read*2 , FS_SEEK_CUR_RE );
   }else{
      string [0] = '\0';
      file_seek( u16_nb_read , FS_SEEK_CUR_RE );
   }
   return (u16_save_buf_size-u16_str_size) ;
}
#endif

#if 0//(( AP3 == ENABLE )||( PASTER == ENABLE ))
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
U16   file_getc( void )
{
   U16   u16_byte;
   U16   u16_buf_pos;

   while(1)
   {
      if( !fat_check_mount_select_open())
         break;
      if(!(FOPEN_READ_ACCESS & fs_g_nav_entry.u8_open_mode))
      {
         fs_g_status = FS_ERR_WRITE_ONLY;
         break;
      }
      if ( file_eof() )
      {
         // End of the file
         fs_g_status = FS_ERR_EOF;
         break;
      }
  
      if( !fat_read_file( FS_CLUST_ACT_ONE ))
      {
         if( FS_ERR_OUT_LIST == fs_g_status )
         {  // Translate the error
            fs_g_status = FS_ERR_EOF;   // End of file
         }
         break;
      }
      
      u16_buf_pos = fs_g_nav_entry.u32_pos_in_file & FS_512B_MASK;

      if( g_b_file_unicode )
      {
         if( 0x01 & u16_buf_pos )
         {
            fs_g_status = FS_ERR_POS_UNICODE_BAD;   // Error in position file
            break;
         }
         MSB(u16_byte) = fs_g_sector[ u16_buf_pos ];
         LSB(u16_byte) = fs_g_sector[ u16_buf_pos +1];
         fs_g_nav_entry.u32_pos_in_file+=2;
      }
      else
      {
         u16_byte = fs_g_sector[ u16_buf_pos ];
      fs_g_nav_entry.u32_pos_in_file++;
      }
      return u16_byte;
   }
   return FS_EOF;   // No read data
}
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))


#if 0//(FSFEATURE_WRITE == (FS_LEVEL_FEATURES & FSFEATURE_WRITE))
#if(( AP3 == ENABLE )||( PASTER == ENABLE ))
//! This fonction return a continue physical memory segment corresponding at a file to write
//!
//! @param     segment  Pointer on the segment structure: <br>
//!                     ->u32_size_or_pos    IN,   shall containt the maximum number of sector to write in file <br>
//!                     ->u32_size_or_pos    OUT,  containt the segment size (unit sector) <br>
//!                     ->other              IN,   ignored <br>
//!                     ->other              OUT,  containt the segment position <br>
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
Bool  file_write( Fs_file_segment _MEM_TYPE_SLOW_ *segment )
{
   if( !fat_check_mount_select_open())
      return FALSE;

   if(!(FOPEN_WRITE_ACCESS & fs_g_nav_entry.u8_open_mode))
   {
      fs_g_status = FS_ERR_READ_ONLY;
      return FALSE;
   }

   if( !fat_write_file( FS_CLUST_ACT_SEG , segment->u16_size ))
      return FALSE;

   // Truncate the segment found if more larger than size asked
   if( (segment->u16_size != 0)  // if not undefine limit
   &&  (segment->u16_size < fs_g_seg.u32_size_or_pos) )
   {
      fs_g_seg.u32_size_or_pos = segment->u16_size ;
   }
   
   // Update position file
   fs_g_nav_entry.u32_pos_in_file += ((U32)fs_g_seg.u32_size_or_pos * FS_512B);
   
   // Update the file size
   if( fs_g_nav_entry.u32_pos_in_file > fs_g_nav_entry.u32_size )
   {
      fs_g_nav_entry.u32_size = fs_g_nav_entry.u32_pos_in_file;
   }
   file_load_segment_value( segment );
   return TRUE;
}


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
Bool  file_set_eof( void )
{
   if( !fat_check_mount_select_open())
      return FALSE;

   if(!(FOPEN_WRITE_ACCESS & fs_g_nav_entry.u8_open_mode))
   {
      fs_g_status = FS_ERR_READ_ONLY;
      return FALSE;
   }

   // Update the file size
   fs_g_nav_entry.u32_size = fs_g_nav_entry.u32_pos_in_file;

   if( !fat_read_file( FS_CLUST_ACT_CLR ))
      return FALSE;

   return fat_cache_flush();
}

#if 0
//! This fonction copy the data buffer in file at the current position
//!
//! @param     buffer         data buffer
//! @param     u16_buf_size   data size
//!
//! @return    number of byte write
//! @return    0, in case of error
//!
U16   file_write_buf( U8 _MEM_TYPE_SLOW_ *buffer , U16 u16_buf_size )
{
   _MEM_TYPE_FAST_ U16 u16_nb_write_tmp;
   _MEM_TYPE_FAST_ U16 u16_nb_write;
   _MEM_TYPE_FAST_ U16 u16_pos_in_sector;

   if( !fat_check_mount_select_open())
      return FALSE;

   if(!(FOPEN_WRITE_ACCESS & fs_g_nav_entry.u8_open_mode))
   {
      fs_g_status = FS_ERR_READ_ONLY;
      return FALSE;
   }

   u16_nb_write = 0;  // Init to "no data read"

   while( 0 != u16_buf_size )
   {
      u16_pos_in_sector = fs_g_nav_entry.u32_pos_in_file % FS_512B;
      if( (0== u16_pos_in_sector)
      &&  (FS_512B <= u16_buf_size))
      {
         u16_nb_write_tmp = u16_buf_size / FS_512B;  // read a modulo sector size

         // It is possible to read more than sector
         if( !fat_write_file( FS_CLUST_ACT_SEG , u16_nb_write_tmp ))
            return FALSE;
         
         // Truncate the segment found if more larger than size asked
         if( u16_nb_write_tmp < fs_g_seg.u32_size_or_pos)
         {
            fs_g_seg.u32_size_or_pos = u16_nb_write_tmp;
         }else{
            u16_nb_write_tmp = fs_g_seg.u32_size_or_pos;
         }
   
         // Fill buffer directly
         while( 0 != fs_g_seg.u32_size_or_pos )
         {
            if( CTRL_GOOD != ram_2_memory( fs_g_nav.u8_lun  , fs_g_seg.u32_addr, buffer))
            {
               fs_g_status = FS_ERR_HW;
               return u16_nb_write;
            }
            fs_g_seg.u32_size_or_pos--;
            fs_g_seg.u32_addr++;
            buffer += FS_512B;
         }
         // Translate to Byte unit
         u16_nb_write_tmp *= FS_512B;
      }
      else
      {
         if( fs_g_nav_entry.u32_pos_in_file == fs_g_nav_entry.u32_size )
         {
            // New sector for the file then no necessary to read sector
            if( !fat_write_file( FS_CLUST_ACT_SEG  , 1 ))
               return FALSE;
            // Upudate cache
            fs_gu32_addrsector = fs_g_seg.u32_addr;
            //if( !fat_cache_read_sector( FALSE ))
//***********************************************
            //change by xuanyu, because add file to playlist error
            if( !fat_cache_read_sector( TRUE ))
//***********************************************
               return FALSE;
         }else{
            // Sector containt a data used by file then read sector
            if( !fat_write_file( FS_CLUST_ACT_ONE  , 1 ))
               return FALSE;
         }

         // Write the data into the cache
         fat_cache_sector_is_modify();
                
         // Compute the number of data to transfer
         u16_nb_write_tmp = FS_512B - u16_pos_in_sector;  // Compute the maximum to next sector limit
         if( u16_nb_write_tmp > u16_buf_size )
            u16_nb_write_tmp = u16_buf_size;

         // Fill buffer
         memcpy_ram2ram( &fs_g_sector[ u16_pos_in_sector ], buffer , u16_nb_write_tmp );
         buffer += u16_nb_write_tmp;
      }
      // Update positions
      fs_g_nav_entry.u32_pos_in_file+= u16_nb_write_tmp;
      u16_nb_write                  += u16_nb_write_tmp;
      u16_buf_size                  -= u16_nb_write_tmp;

      // Update the file size
      if( fs_g_nav_entry.u32_pos_in_file > fs_g_nav_entry.u32_size )
      {
         fs_g_nav_entry.u32_size = fs_g_nav_entry.u32_pos_in_file;
      }
   }
   return u16_nb_write;  // All buffer is writed
}


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
U16   file_puts( U8 _MEM_TYPE_SLOW_ *string )
{
   U16 u16_size_string=0;
   if( g_b_file_unicode )
   {
      while( 0 != ((FS_STR_UNICODE)string )[u16_size_string] ) u16_size_string++;
      return file_write_buf( string, u16_size_string*2 );
   }else{
      while( 0 != string[u16_size_string] ) u16_size_string++;
      return file_write_buf( string, u16_size_string );
   }
}
#endif

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
Bool  file_putc( U16 u16_byte )
{
   U16 u16_buf_pos;

   if( !fat_check_mount_select_open())
      return FALSE;

   if(!(FOPEN_WRITE_ACCESS & fs_g_nav_entry.u8_open_mode))
   {
      fs_g_status = FS_ERR_READ_ONLY;
      return FALSE;
   }

   if( !fat_write_file( FS_CLUST_ACT_ONE  , 1 ))
      return FALSE;

   // Write the data into the cache
   fat_cache_sector_is_modify();

   u16_buf_pos = fs_g_nav_entry.u32_pos_in_file & FS_512B_MASK;

   if( g_b_file_unicode )
   {
      if( 0x01 & u16_buf_pos )
      {
         fs_g_status = FS_ERR_POS_UNICODE_BAD;   // Error in position file
         return FALSE;
      }
      fs_g_sector[ u16_buf_pos ]    = MSB(u16_byte);
      fs_g_sector[ u16_buf_pos +1]  = LSB(u16_byte);
      fs_g_nav_entry.u32_pos_in_file+=2;
   }
   else
   {
      fs_g_sector[ u16_buf_pos ]    = LSB(u16_byte);
   fs_g_nav_entry.u32_pos_in_file++;
   }
   
   // Update the file size
   if( fs_g_nav_entry.u32_pos_in_file > fs_g_nav_entry.u32_size )
   {
      fs_g_nav_entry.u32_size = fs_g_nav_entry.u32_pos_in_file;
   }
   return TRUE;
}
#endif   //#if(( AP3 == ENABLE )||( PASTER == ENABLE ))

#endif  // FS_LEVEL_FEATURES


#if 0
//! This fonction return the position in the file
//!
//! @return    Position in file
//!
U32   file_getpos( void )
{
   if( !fat_check_mount_select_open() )
      return 0;

   return fs_g_nav_entry.u32_pos_in_file;
}
#endif

//! This fonction change the position in file selected
//!
//! @param     u32_pos     number of byte to seek
//! @param     u8_whence   direction of seek <br>
//!                        FS_SEEK_SET   , start at the beginning and foward <br>
//!                        FS_SEEK_END   , start at the end of file and rewind <br>
//!                        FS_SEEK_CUR_RE, start at the current position and rewind <br>
//!                        FS_SEEK_CUR_FW, start at the current position and foward <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  file_seek( U32 u32_pos , U8 u8_whence )
{
   switch(u8_whence)
   {
      /*case FS_SEEK_CUR_RE:
      if( u32_pos >( *fat_g_cur_file )->position )
         return FALSE;
      if( FSSeek((__X FCFILE *)*fat_g_cur_file, ( *fat_g_cur_file )->position - u32_pos ) != FS_NO_ERROR ) 
         return FALSE;
      break;*/

      case FS_SEEK_SET:
#if( FAT_CLUSTER_BUF == DISABLE )
      if( FSSeek((__X FCFILE *)*fat_g_cur_file, u32_pos ) != FS_NO_ERROR )
         return FALSE;
      break;
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )
#if( FAT_CLUSTER_BUF == ENABLE )
      return ( fs_seek( u32_pos ));
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

      case FS_SEEK_END:
      if( u32_pos >( *fat_g_cur_file )->size )
         return FALSE;
#if( FAT_CLUSTER_BUF == DISABLE )
      if( FSSeek((__X FCFILE *)*fat_g_cur_file, ( *fat_g_cur_file )->size - u32_pos ) != FS_NO_ERROR )
         return FALSE;
      break;
#endif   //#if( FAT_CLUSTER_BUF == DISABLE )
#if( FAT_CLUSTER_BUF == ENABLE )
      return ( fs_seek(( *fat_g_cur_file )->size - u32_pos ));
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

      /*case FS_SEEK_CUR_FW:
      if( FSSeek((__X FCFILE *)*fat_g_cur_file, ( *fat_g_cur_file )->position + u32_pos ) != FS_NO_ERROR )
         return FALSE;
      break;*/
   }
   return TRUE;
}


#if( FAT_CLUSTER_BUF == ENABLE )
FatCluster kb_fat_check_cluster( FSINFO *fs_info, U32 cluster )
{
   cluster &= MASK_CLUSTER_32_BIT;
   if( !cluster )
      return Fat_Empty_Cluster;
   if( cluster >= fs_info->last_cluster_mark )
      return Fat_End_Cluster;
   if(( cluster < 2 )
      ||( cluster > fs_info->max_cluster_num ))
      return Fat_Invalid_Cluster;
   return Fat_Valid_Cluster;
}


FS_ERR FSBufSeek( U32 new_position )
{
   U32/* i, */cluster, new_cluster;
   FS_ERR err = FS_NO_ERROR;
   __X FSINFO *fsinfo;
	FCFILE file;
	U32 clus_offset;
	U32 cur_offset;
	FCFILE *pfile;
	ClusterListItem *cluster_list;
	//U8 list_num = sizeof( cluster_buf )/ sizeof( ClusterListItem );
	int i, j = -1;

	//test_timer = timer_get_time();
   pfile = *fat_g_cur_file;
   cluster_list = cluster_buf;

   if( 0 == pfile )
      return FS_OPENFILE_FULL;
   file = *pfile;
   fsinfo = gdrv[ file.drive ].fsinfo;  
   if( 0 == fsinfo->total_sectors )
      return FS_FORMAT_ERROR;

   SetFileSystemBusy( file.drive );

   if( 0xFFFFFFFFUL == new_position )
   {
      new_position = file.size;
   }
   else if( new_position > file.size )
   {
      err = FS_SEEK_OUT_OF_RANGE;
      goto _LEnd_FSSeek;
   }
   else if( new_position == file.position )
   {
      goto _LEnd_FSSeek;
   }

   gdrv[ file.drive ].buffer_status = EMPTY;
   clus_offset = ( new_position /( fsinfo->sectors_per_cluster )/ SECTOR_SIZE );
   //last_index = -1;
   //for( i = 0; i < list_num; ++i )
   cur_offset = 0;
   for( i = 0; i < FS_NB_CACHE_CLUSLIST; ++i )
   {
#if 1
      //if( cluster_list[ i ].disk_id == 0xFFFF )
      //   continue;

      if(( pfile->drive == cluster_list[ i ].disk_id )
         &&( pfile->first_cluster == cluster_list[ i ].start_cluster ))
      {
         if( cluster_list[ i ].offset_by_clus >= cur_offset )
         {
            cur_offset = cluster_list[ i ].offset_by_clus;
            j = i;
         }
         
         if(( clus_offset >= cluster_list[ i ].offset_by_clus )
            &&( clus_offset <( cluster_list[ i ].offset_by_clus + cluster_list[ i ].seg_len )))
         {
            cluster = cluster_list[ i ].cluster + clus_offset - cluster_list[ i ].offset_by_clus;
            goto fs_buf_seek_end;
            //break;
         }
      }
#else
      if(( pfile->drive == cluster_list[ i ].disk_id )
         &&( pfile->first_cluster == cluster_list[ i ].start_cluster )
            &&( clus_offset >= cluster_list[ i ].offset_by_clus )
               &&( clus_offset <( cluster_list[ i ].offset_by_clus + cluster_list[ i ].seg_len )))
      {
         //
         cluster = cluster_list[ i ].cluster + clus_offset - cluster_list[ i ].offset_by_clus;
         goto fs_buf_seek_end;
         //break;
      }
      else if( cluster_list[ i ].disk_id == 0xFFFF )
      {
         break;
      }
#endif
   }
   //if( cluster_list[ i ].disk_id == 0xFFFF )
   {
#if 0
      //
      if( i == 0 )
      {
         cur_offset = 0;
         cluster = file.first_cluster;
      }
      else
      {
         ClusterListItem *p = &cluster_list[ i - 1 ];
         cur_offset = p->offset_by_clus + p->seg_len - 1;
         cluster = p->cluster + p->seg_len - 1;
      }
#else
      cur_offset = 0;
      cluster = file.first_cluster;
      if( j != -1 )
      {
         ClusterListItem *p = &cluster_list[ j ];
         cur_offset = p->offset_by_clus + p->seg_len - 1;
         cluster = p->cluster + p->seg_len - 1;
      }
#endif
      
      while( cur_offset != clus_offset )
      {
         new_cluster = GetFATEntry( file.drive, cluster );
         if( INVALID_CLUSTER == new_cluster )
         {
            err = FS_DRIVE_ERROR;
            goto _LEnd_FSSeek;
         }
         else if( kb_fat_check_cluster( fsinfo, new_cluster )!= Fat_Valid_Cluster )
         {
            err = FS_SEEK_OUT_OF_RANGE;
            goto _LEnd_FSSeek;
         }
         ++cur_offset;
         cluster = new_cluster;
      }
   }

fs_buf_seek_end:
   file.cluster = cluster;
   file.position = new_position;
   //test_timer = timer_get_time() - test_timer;
_LEnd_FSSeek:
   *pfile = file;
   ClearFileSystemBusy( file.drive );
   return err;
}


Bool fs_seek( U32 addr )
{
#if 0
   FS_ERR err;
   err = FSBufSeek( addr );
   SendString((U8 *)"\r\ne=");
   send_hex_u8(err);
   if( err == FS_NO_ERROR )
      return TRUE;
   return FALSE;
#else   
   if( FSBufSeek( addr ) == FS_NO_ERROR )
      return TRUE;
   return FALSE;
#endif   
}


#pragma CODE = FS_CREATE_BUF_CODE

Bool FSCreateBuf( void )
{
   //U32 test_timer;
   U32 cluster, new_cluster;
   int list_index, i;
   FSINFO *fsinfo;
   FCFILE *pfile;
   ClusterListItem *cluster_list;
   //U8 list_num;
   Bool clear_cluster_list = FALSE;

   //test_timer = timer_get_time();
   pfile = *fat_g_cur_file;
   //list_num = sizeof( cluster_buf )/ sizeof( ClusterListItem );
   cluster_list = cluster_buf;
   fsinfo = gdrv[ pfile->drive ].fsinfo;  
   if( fsinfo == NULL )
      return FALSE;
 
   if( !b_clusterlist )
   {
      for( list_index = 0; list_index < FS_NB_CACHE_CLUSLIST; ++list_index )
      {
         cluster_list[ list_index ].disk_id = 0xFFFF;
         cluster_list[ list_index ].start_cluster = fsinfo->last_cluster_mark;
      }
      b_clusterlist = TRUE;
   }
   else
   {
      for( list_index = 0; list_index < FS_NB_CACHE_CLUSLIST; ++list_index )
      {
         if(( pfile->drive == cluster_list[ list_index ].disk_id )
            &&( pfile->first_cluster == cluster_list[ list_index ].start_cluster ))
            return TRUE;
      }
      for( list_index = 0; list_index < FS_NB_CACHE_CLUSLIST; ++list_index )
      {
         if( cluster_list[ list_index ].disk_id == 0xFFFF )
            goto null_cluster_list;
      }
   }
   
   list_index = 0;
null_cluster_list:
   i = 0;
   if( cluster_list[ list_index ].disk_id != 0xFFFF )
      clear_cluster_list = TRUE;
   cluster_list[ list_index ].disk_id = pfile->drive;
   cluster_list[ list_index ].start_cluster = pfile->first_cluster;
   cluster_list[ list_index ].cluster = pfile->first_cluster;
   cluster_list[ list_index ].seg_len = 1;
   cluster_list[ list_index ].offset_by_clus = 0;
   cluster = pfile->first_cluster;
   while( 1 )
   {
      new_cluster = GetFATEntry( pfile->drive, cluster );
      if( new_cluster ==( cluster + 1 ))
      {
         ++cluster_list[ list_index ].seg_len;
      }
      else
      {
         if( kb_fat_check_cluster( fsinfo, new_cluster )!= Fat_Valid_Cluster )
         {
            break;
         }
         if( i >=( FS_NB_CACHE_CLUSLIST - 1 ))
            break;
         if( list_index ==( FS_NB_CACHE_CLUSLIST - 1 ))
         {
            if( cluster_list[ 0 ].disk_id != 0xFFFF )
               clear_cluster_list = TRUE;
            cluster_list[ 0 ].disk_id = pfile->drive;
            cluster_list[ 0 ].start_cluster = pfile->first_cluster;
            cluster_list[ 0 ].cluster = new_cluster;
            cluster_list[ 0 ].seg_len = 1;
            cluster_list[ 0 ].offset_by_clus = cluster_list[ list_index ].offset_by_clus + cluster_list[ list_index ].seg_len;
            
            list_index = -1;
         }
         else
         {
            if( cluster_list[ list_index + 1 ].disk_id != 0xFFFF )
               clear_cluster_list = TRUE;
            cluster_list[ list_index + 1 ].disk_id = pfile->drive;
            cluster_list[ list_index + 1 ].start_cluster = pfile->first_cluster;
            cluster_list[ list_index + 1 ].cluster = new_cluster;
            cluster_list[ list_index + 1 ].seg_len = 1;
            cluster_list[ list_index + 1 ].offset_by_clus = cluster_list[ list_index ].offset_by_clus + cluster_list[ list_index ].seg_len;
         }
         ++list_index;
         ++i;
      }
      cluster = new_cluster;
   }
   //test_timer = timer_get_time() - test_timer;
   //list_index = test_timer;
   //list_index = timer_get_time();
   //list_index = timer_get_time() - list_index;
   if( clear_cluster_list )
   {
      /*++list_index;
      if( list_index >= FS_NB_CACHE_CLUSLIST )
         list_index = 0;*/
      for( i = 0; i < FS_NB_CACHE_CLUSLIST; ++i )
      {
         ++list_index;
         if( list_index >= FS_NB_CACHE_CLUSLIST )
            list_index = 0;
         if( cluster_list[ list_index ].disk_id != 0xFFFF )
         {
            if( cluster_list[ list_index ].offset_by_clus == 0 )
               break;
         }
         cluster_list[ list_index ].disk_id = 0xFFFF;
         /*list_index++;
         if( list_index >= FS_NB_CACHE_CLUSLIST )
            list_index = 0;*/
      }
   }
   return TRUE;
}
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

#if 0
//! This fonction test if the position is at the beginning of file 
//!
//! @return    1     the position is at the beginning of file
//! @return    0     the position isn't at the beginning of file
//! @return    FFh   error
//!
U8    file_bof( void )
{
   if( !fat_check_mount_select_open() )
      return 0xFF;

   return (0 == fs_g_nav_entry.u32_pos_in_file );
}


//! This fonction test if the position is at the end of file
//!
//! @return    1     the position is at the end of file
//! @return    0     the position isn't at the end of file
//! @return    FFh   error
//!
U8    file_eof( void )
{
   if( !fat_check_mount_select_open() )
      return 0xFF;

   return (fs_g_nav_entry.u32_size <= fs_g_nav_entry.u32_pos_in_file );
}


//! This fonction flush the data include in file and information about file
//!
void  file_flush( void )
{
   U8 save_open_mode;
   save_open_mode = fs_g_nav_entry.u8_open_mode;
   file_close();
   fs_g_nav_entry.u8_open_mode = save_open_mode;
}
#endif

#pragma CODE = FILE_TEMP_CODE01

//! This fonction close the file
//!
void  file_close( void )
{
   // If a file is open, then close this one
   if( *fat_g_cur_file == 0 )
      return;
   FSClose((__X FCFILE *)*fat_g_cur_file );
   *fat_g_cur_file = 0;
}

#if 0
//!辅助函数，快速读一个簇链
//!
//!(1)参数
//!  IN:
//!    cl->cluster  从cluster开始的簇链
//!    cl->u32_start  簇链相对偏移
//!  OUT:
//!    cl->u8_lun  簇链所在的磁盘分区
//!    cl->u32_addr  簇链段的绝对位置
//!    cl->u32_size  读取的簇链的长度
//!(2)返回值
//!  此簇链段的下一个簇
//!  如果为0表示过程出错
//!  如果为 FS_CLUST_VAL_EOL表示簇链到了文件尾部
//!(3)其它
//!  在fat_cluster_chain_search中使用，绕过fat_cluster_rw函数，直接读缓冲区，加快速度
U32 __fat_read_segment( Fs_clusterlist_cache _MEM_TYPE_SLOW_ *cl, U32 cur_cluster )
{
   U32 _MEM_TYPE_FAST_ next_cluster;
   U32 _MEM_TYPE_FAST_ temp;
   U16 _MEM_TYPE_FAST_ index;
   U32 _MEM_TYPE_FAST_ count;
   U32 _MEM_TYPE_FAST_ cur_sector;
   U32 _MEM_TYPE_SLOW_ end_sector;

   if( Is_fat12 )
      return 0;
   if( Is_fat32 )
      cur_sector = ( cur_cluster >> 7 ) + fs_g_nav.u32_ptr_fat;
   else
      cur_sector = ( cur_cluster >> 8 ) + fs_g_nav.u32_ptr_fat;
   next_cluster = 0;
   count = 0;
   cl->u8_lun = fs_g_nav.u8_lun;
   cl->u32_addr = (( cur_cluster - 2 )* fs_g_nav.u8_BPB_SecPerClus ) + fs_g_nav.u32_ptr_fat + fs_g_nav.u32_offset_data;
   cl->u32_size = 0;
   if( !fat_cache_flush())
      return 0;
   fat_cache_reset();
   end_sector = fs_g_nav.u32_ptr_fat + fs_g_nav.u16_fat_size;
   for( ; cur_sector < end_sector; ++cur_sector )
   {
      if( memory_2_ram( cl->u8_lun, cur_sector, fs_g_sector )!= CTRL_GOOD )
         return 0;
      if( Is_fat32 )
      {
         for( index = cur_cluster & 127; index < 128; ++index )
         {
            temp = (( U32 _MEM_TYPE_SLOW_ *)fs_g_sector)[ index ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 0 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 3 ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 1 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 2 ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 2 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 1 ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 3 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 0 ];
            ++count;
            if( next_cluster != cur_cluster + 1 )
            {
               cl->u32_size = count * fs_g_nav.u8_BPB_SecPerClus;
               return next_cluster;
            }
            else
               cur_cluster = next_cluster;
         }
      }
      else
      {
         for( index = cur_cluster & 255; index < 256; ++index )
         {
            temp = (( U16 _MEM_TYPE_SLOW_ *)fs_g_sector )[ index ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 2 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 1 ];
            (( U8 _MEM_TYPE_FAST_ *)( &next_cluster ))[ 3 ] = (( U8 _MEM_TYPE_FAST_ *)( &temp ))[ 0 ];
            ++count;
            if( next_cluster != cur_cluster + 1 )
            {
               cl->u32_size = count * fs_g_nav.u8_BPB_SecPerClus;

               if( next_cluster == 0xFFFF )
                  next_cluster = FS_CLUST_VAL_EOL;
               return next_cluster;
            }
            else
               cur_cluster = next_cluster;
         }
      }
   }
   return 0;
}


extern   Fs_clusterlist_cache _MEM_TYPE_SLOW_   fs_g_cache_clusterlist[ FS_NB_CACHE_CLUSLIST ];

//extern   void  output_cluster_list  ( Fs_clusterlist_cache _MEM_TYPE_SLOW_ *seg );

//Bool  __have_fragments = FALSE;


Bool file_prepare_all_cluster_lists( void )
{
   //U32 _MEM_TYPE_SLOW_ old_time;

   //old_time = timer_get_time();
   //if( Is_fat12 )
   {
      Fs_segment _MEM_TYPE_SLOW_ old_seg;
      old_seg.u32_addr = fs_g_nav_entry.u32_cluster;
      old_seg.u32_size_or_pos = 0;
      fs_g_seg.u32_addr = fs_g_nav_entry.u32_cluster;
      fs_g_seg.u32_size_or_pos = old_seg.u32_size_or_pos;
      while( fat_cluster_list( FS_CLUST_ACT_SEG ))
      {
         old_seg.u32_size_or_pos += fs_g_seg.u32_size_or_pos;
         fs_g_seg.u32_addr = fs_g_nav_entry.u32_cluster;
         fs_g_seg.u32_size_or_pos = old_seg.u32_size_or_pos;
      }
   }
#if 0
   else
   {
      Fs_clusterlist_cache _MEM_TYPE_SLOW_ cluster_list;
      U32 _MEM_TYPE_SLOW_ next_cluster;
      
      cluster_list.u32_cluster = fs_g_nav_entry.u32_cluster;
      cluster_list.u32_start = 0;
      next_cluster = fs_g_nav_entry.u32_cluster;
      for( ; ; )
      {
         next_cluster = __fat_read_segment( &cluster_list, next_cluster );
         if( next_cluster == 0 )
            return FALSE;
         if( !fat_cache_clusterlist_update_add( &cluster_list ))
            break;
         if( next_cluster == FS_CLUST_VAL_EOL )
            break;
         cluster_list.u32_cluster = fs_g_nav_entry.u32_cluster;
         cluster_list.u32_start += cluster_list.u32_size;
      }
   }
#endif
   return TRUE;
}
#endif
#endif 	// _file_c_
