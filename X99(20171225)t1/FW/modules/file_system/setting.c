
//! @file setting.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the file system interface.
//!
//!
//! @version 1.23 snd3-dvk-1_9_5 $Id: setting.c,v 1.23 2007/10/05 13:43:11 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _setting_c_
#define _setting_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_mmi.h"
#include "setting.h"
#include "navigation.h"
#include "file.h"
#include "fat.h"
#include "modules\control_access\ctrl_access.h"
#include LIB_MEM
#include "mmi\player\srv_player.h"
#include <stdio.h>

//_____ M A C R O S ________________________________________________________

// Check configuration
#if (SETTING_STRING_UNICODE == FALSE)
#  if( FS_ASCII == DISABLED )
#     error Setting string format cannot be supported, change option format string in FileSystem or Setting
#  endif
#else
#  if( FS_UNICODE == DISABLED )
#     error Setting string format cannot be supported, change option format string in FileSystem or Setting
#  endif
#endif


//_____ D E F I N I T I O N S ______________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

#if (SETTING_AREA_FAT == DISABLED)

code U8 CO_SETTING=0; // For support the BANK.(?CO?SETTING(0x....)) in projet option

//! This fonction write list of setting (p_struct)
//! in a file named UNICODE_FILE_SETTING
Bool setting_save(U8 *p_struct)
{
   U8 size;
   extern Bool g_b_battery_low;

   //volume_save_flag = FALSE;
   if( g_b_battery_low )
      return FALSE;

	if( !trans_buffer_alloc())
      return FALSE;

   // Fill buffer   
   size = *p_struct;
   memcpy_ram2ram( g_trans_buffer, p_struct, size );

   if (CTRL_GOOD != ram_2_memory( LUN_ID_NF_SETTING, 0, g_trans_buffer ))
   {
      trans_buffer_free();
      return FALSE;
   }

   // Make a read on Nand-flash to force "copy tail". This avoid recovery
   // on power-on
   nf_sync();

   trans_buffer_free();
   return TRUE;
}

//! This fonction loads list of setting in p_struct
//!
Bool setting_load(U8 *p_struct)
{
   U8 size;

   if( !trans_buffer_alloc())
      return FALSE;

   if (CTRL_GOOD != memory_2_ram( LUN_ID_NF_SETTING , 0 , g_trans_buffer ))
   {
      trans_buffer_free();
      return FALSE;
   }
   
   // check the structure size and version
   if( p_struct[0] != g_trans_buffer[0] )
   {
      trans_buffer_free();
      return FALSE;  // Bad value in sector setting
   }

   // Fill structure   
   size = *p_struct;
   memcpy_ram2ram( p_struct, g_trans_buffer, size );

   trans_buffer_free();
   return TRUE;
}

#if 0
//! This fonction update list of setting via a specific file
//!
//!! WARNING: The update routine transfer a blocks of 512B size in setting zone.
//!
Bool setting_update( void )
{
   _MEM_TYPE_SLOW_ Fs_index idx;
#if (SETTING_STRING_UNICODE == FALSE)
   const U8       _MEM_TYPE_SLOW_ path_update[] = UPDATE_SETTING_PATH;
#else
   const UNICODE  _MEM_TYPE_SLOW_ path_update[] = UPDATE_SETTING_PATH;
#endif
   _MEM_TYPE_SLOW_ Fs_file_segment segment_src;
   _MEM_TYPE_SLOW_ Ctrl_status status_stream;
   U8 id_trans;
   Bool b_status = FALSE;

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_ascii();
#  endif
#endif

   //** Select the specific file in current navigator
   // Save index in navigateur (in case of)
   idx = nav_getindex();

   if( nav_setcwd( (FS_STRING) path_update , SETTING_CASE_SENSITIVE , FALSE ) )
   {
      if (file_open(FOPEN_MODE_R))        // Open the update file
      {
         b_status = TRUE;
         while( !file_eof())
         {
            // Get the maximum segment supported by stream routines (U16)
            segment_src.u16_size = 0xFFFF;
            if (!file_read( &segment_src ))
            {
               b_status = FALSE;
               break;
            }
            // Start transfer
            id_trans = stream_mem_to_mem(  segment_src.u8_lun , segment_src.u32_addr , LUN_ID_NF_SETTING , 0 , segment_src.u16_size  );
            if( id_trans == ID_STREAM_ERR)
            {
               b_status = FALSE;
               break;
            }
            // Wait end of transfer
            do{
            status_stream = stream_state( id_trans );
            } while( CTRL_BUSY == status_stream);
            stream_stop( id_trans );
            if( status_stream != CTRL_GOOD)
            {
               b_status = FALSE;
               break;
            }
         }
         file_close();
      }
   }

   // Make a read on Nand-flash to force "copy tail". This avoid recovery
   nf_sync();

   // Return in index where you previously pointed
   nav_gotoindex(&idx);

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif

   return b_status;
}
#endif

#else
// (SETTING_AREA_FAT == ENABLED)

//! This fonction write list of setting (p_struct)
//! in a file named UNICODE_FILE_SETTING
Bool setting_save(U8 *p_struct)
{
   //U8 _MEM_TYPE_SLOW_ subdir_attr;
   _MEM_TYPE_SLOW_ Fs_index idx;
#if (SETTING_STRING_UNICODE == FALSE)
   const U8       _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#else
   const UNICODE  _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#endif

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_ascii();
#  endif
#endif

   // Save index in navigateur (in case of)
   idx = nav_getindex();
   // Enter in setting disk
   if (!nav_drive_set(LUN_ID_MMC_SD))
      goto setting_save_error;
   if (!nav_partition_mount())
   {
      goto setting_save_error;
   }

   sprintf( g_string, "subdir" );
   /*if( !nav_filelist_findname( g_string, FALSE )
      || !nav_dir_cd())
      goto setting_save_error;*/
   if( !nav_dir_make( g_string ) )
   {
      if( FS_ERR_FILE_EXIST != fs_g_status )
      {
         goto setting_save_error;
      }
   }
   if( !nav_dir_cd() )
      goto setting_save_error;
   
   // Create file
   if( !nav_file_create( name_setting ) )
   {
      if( FS_ERR_FILE_EXIST != fs_g_status )
         goto setting_save_error;
      // here file exist, then open this one
   }
   //subdir_attr = fs_g_nav_entry.u8_attr;
   // Open file in write mode with size 0
   if (!file_open(FOPEN_MODE_W))
      goto setting_save_error;

   // Write structure (size of struct = p_struct[0])
	if( p_struct[0] != file_write_buf( p_struct , p_struct[0] ) )
   {
      file_close();
      goto setting_save_error;
   }
   file_close();

   // Hide the settings file
   nav_file_attributset(FS_ATTR_HIDDEN | FS_ATTR_SYSTEM);
   //subdir_attr |= FS_ATTR_HIDDEN | FS_ATTR_SYSTEM;
   //nav_file_attributset(subdir_attr);

   fat_cache_flush();  // In case of error during writing data, flush the data before exit fonction

   // Return in index where you previously pointed
   nav_gotoindex(&idx);
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif
   return TRUE;

setting_save_error:
   fat_cache_flush();  // In case of error during writing data, flush the data before exit fonction

   // Return in index where you previously pointed
   nav_gotoindex(&idx);
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif
   return FALSE;
}

//! This fonction loads list of setting in p_struct
//!
Bool setting_load(U8 *p_struct)
{
   _MEM_TYPE_SLOW_ Fs_index idx;
#if (SETTING_STRING_UNICODE == FALSE)
   const U8       _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#else
   const UNICODE  _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#endif

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_ascii();
#  endif
#endif

   // Get index in navigateur
   idx = nav_getindex();
   // Enter in setting disk
   if( !nav_drive_set(LUN_ID_MMC_SD) )
      goto setting_load_error;
   if( !nav_partition_mount() )
      goto setting_load_error;

   sprintf( g_string, "subdir" );
   /*if( !nav_filelist_findname( g_string, FALSE )
      || !nav_dir_cd())
      goto setting_load_error;*/
   if( !nav_dir_make( g_string ) )
   {
      if( FS_ERR_FILE_EXIST != fs_g_status )
      {
         goto setting_load_error;
      }
   }
   if( !nav_dir_cd() )
      goto setting_load_error;

    // Check if a setting file already exists
   if( !nav_filelist_findname(name_setting, FALSE) )
      goto setting_load_error;

   // Open file
   if( !file_open(FOPEN_MODE_R) )
      goto setting_load_error;

   // check the structure size
   if( p_struct[0] != file_getc() )
   {
      file_close();
      goto setting_load_error;
   }

   // Read the structure
	if( (p_struct[0]-1) != file_read_buf( p_struct+1 , p_struct[0]-1 ) )
   {
      file_close();
      goto setting_load_error;
   }
   file_close();

   // Return in index where you previously pointed
   nav_gotoindex(&idx);
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif
   return TRUE;

setting_load_error:
   // Return in index where you previously pointed
   nav_gotoindex(&idx);
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif
   return FALSE;
}

#if 0
//! This fonction update list of setting via a specific file
//!
Bool setting_update( void )
{
   _MEM_TYPE_SLOW_ Fs_index idx;
#if (SETTING_STRING_UNICODE == FALSE)
   const U8       _MEM_TYPE_SLOW_ path_update[] = UPDATE_SETTING_PATH;
   const U8       _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#else
   const UNICODE  _MEM_TYPE_SLOW_ path_update[] = UPDATE_SETTING_PATH;
   const UNICODE  _MEM_TYPE_SLOW_ name_setting[] = FILE_SETTING;
#endif
   Bool b_status = FALSE;

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_ascii();
#  endif
#endif

   // Use default navigator
   nav_select( 0 );
   // Save current index of navigateur
   idx = nav_getindex();
   // Search the update file
   if( !nav_setcwd( (FS_STRING) path_update , SETTING_CASE_SENSITIVE , FALSE ) )
      goto settings_update_end;

   // file found, put this file in "COPY" index (= navigator FS_NAV_ID_COPYFILE)
   if( !nav_file_copy())
      goto settings_update_end;

   // Now, search destination file to delete this one
   if( !nav_drive_set (LUN_ID_NF_SETTING) )
      goto settings_update_end;
   if( !nav_partition_mount() )
      goto settings_update_end;
   if( !nav_filelist_reset() )
      goto settings_update_end;
   if( nav_filelist_findname ( name_setting , FALSE ) )
   {  
      // Found the file so delete this one
      if( !nav_file_del( TRUE ) )
         goto settings_update_end; 
   }
   // HERE, the navigator is in root dir of NF

   // Paste file in root dir of NF
   if( !nav_file_paste_start( name_setting ) )
      goto settings_update_end;
   {
   U8 status;
   do{
      status = nav_file_paste_state(FALSE);
   }while( COPY_BUSY == status );

   if( COPY_FINISH == status )
      b_status = TRUE;
   }

settings_update_end:
   // Make a read on Nand-flash to force "copy tail". This avoid recovery
   nf_sync();
   // Return in index where you previously pointed
   nav_gotoindex(&idx);
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
#  if (SETTING_STRING_UNICODE == FALSE)
   nav_string_unicode();
#  endif
#endif
   return b_status;
}
#endif
#endif // (SETTING_AREA_FAT == DISABLED)


//! This example copy all "tree folders" from SD to NandFlash Disk.
//!
/*
Bool filesystem_example_copytree( void )
{
   const UNICODE _MEM_TYPE_SLOW_ name[50]={'u','p','d','a','t','e',0};
   U8 u8_folder_level = 0;

//trace("Mount drive\n");
   //** Use three navigators (0 to explore SD, 1 to explore NF disk, 2 used by copy file routine)
   nav_select( 0 );
   if( !nav_drive_set( LUN_ID_MMC_SD ))
      return FALSE;
   if( !nav_partition_mount() )
      return FALSE;
   if( !nav_filelist_findname( (FS_STRING )name , SETTING_CASE_SENSITIVE ) )   // search dir "update"
      return FALSE;
   if( !nav_dir_cd())
      return FALSE;
   nav_select( 1 );
   if( !nav_drive_set( LUN_ID_NF_DISKMASS ))
      return FALSE;
   if( !nav_partition_mount() )
      return FALSE;
  
   // loop to scan and create ALL folders and files
   while(1)
   {
      // No dir in current dir then go to parent dir on SD and NandFlash disk
      while(1)
      {
//trace("Search files or dir\n");
         // Reselect SD
         nav_select( 0 );
         if( nav_filelist_set( 0 , FS_FIND_NEXT ) )
            break;   // a next file and directory is found
   
         // No other dir or file in current dir then go to parent dir on SD and NandFlash disk
         if( 0 == u8_folder_level )
         {
            // end of update folder
//trace("End of copy\n");
            return TRUE;   //********* END OF COPY **************
         }

//trace("Go to parent\n");
         // Remark, nav_dir_gotoparent() routine go to in parent dir and select the children dir in list
         u8_folder_level--;
         if( !nav_dir_gotoparent() )
            return FALSE;
         // Select NandFlash navigator and go to the same dir of SD
         nav_select( 1 );
         if( !nav_dir_gotoparent() )
            return FALSE;
      } // end of while (1)
      
      if( nav_file_isdir())
      {
//trace("Dir found - create dir & CD\n");
         //** here, a new directory is found and is selected
         // Get name of current selection (= dir name on SD)
         if( !nav_file_name( (FS_STRING )name , 50  , FS_NAME_GET, FALSE  ))
            return FALSE;
         // Enter in dir (on SD)
         if( !nav_dir_cd())
            return FALSE;
         u8_folder_level++;
         // Select NandFlash disk
         nav_select( 1 );
         // Create folder in NandFlash disk
         if( !nav_dir_make( (FS_STRING )name ))
         {
            if( FS_ERR_FILE_EXIST != fs_g_status ) // !!!! Check avialable only on last version of FileSystem module package >=1.0.32
               return FALSE;
            // here, error the name exist
         }
         // Here the navigator have selected the folder on NandFlash
         if( !nav_dir_cd())
         {
            if( FS_ERR_NO_DIR == fs_g_status )
            {
               // FYC -> Copy impossible, because a file have the same name of folder
            }
            return FALSE;
         }
         // here, the folder is create and the navigatorS is enter in this dir
      }
      else
      {
//trace("File found - copy file\n");
         //** here, a new file is found and is selected
         // Get name of current selection (= file name on SD)
         if( !nav_file_name( (FS_STRING )name , 50  , FS_NAME_GET , FALSE  ))
            return FALSE;
         if( !nav_file_copy())
            return FALSE;

         // Paste file in current dir of NandFlash disk
         nav_select( 1 );
         while( !nav_file_paste_start( (FS_STRING)name ) )
         {
            // Error
            if( fs_g_status != FS_ERR_FILE_EXIST )
               return FALSE;
//trace("del file\n");
            // File exist then del this one
            if( !nav_file_del( TRUE ) )
               return FALSE;
            // here, retry PASTE                   
         }
         // Copy running
         {
         U8 status;
         do{
            status = nav_file_paste_state(FALSE);
         }while( COPY_BUSY == status );

         if( COPY_FINISH != status )
            return FALSE;
         }
      } // if dir OR file
   } // end of first while(1)
}
*/

//! This example create a file and change the dates
//!
/*
Bool filesystem_example_changedate( void )
{
   const UNICODE _MEM_TYPE_SLOW_ name[]={'d','a','t','e','.','t','x','t',0};

// Exemple to modify a file date
   const U8 _MEM_TYPE_SLOW_ date_create[]="2005122512301050";
   const U8 _MEM_TYPE_SLOW_ date_write[]="2006072319005130";
   nav_select( 0 );
   if( !nav_drive_set( LUN_ID_NF_DISKMASS ))
      return FALSE;
   if( !nav_partition_mount() )
      return FALSE;
   // Create file
   if( !nav_file_create( name ) )
      return FALSE;

   // Modify the current selected file (here is the new file created by previous command)
   if( !nav_file_dateset( date_create , FS_DATE_CREATION) )
      return FALSE;
   if( !nav_file_dateset( date_write , FS_DATE_LAST_WRITE) )
      return FALSE;

   return TRUE;
}
*/

//! This example save the physical disk from USB data in a file on NF
//!
/*
Bool filesystem_example_backupdisk( void )
{
   _MEM_TYPE_SLOW_ UNICODE name[]={'d','i','s','k','.','b','i','n',0};
   _MEM_TYPE_SLOW_ Fs_file_segment  seg_create;
   U8 id_trans;
   U32 u32_disk_copy_size;
   U32 u32_disk_copy_pos;

   nav_select( 0 );
   if( !nav_drive_set( LUN_ID_NF_DISKMASS ))
      return FALSE;
   if( !nav_partition_mount() )
      return FALSE;
   // Create file
   if( !nav_file_create( (const FS_STRING) name ) )
      return FALSE;
   // open the current file
   if( !file_open(FOPEN_MODE_W))
      return FALSE;

   u32_disk_copy_pos    = 0;
   u32_disk_copy_size   = 0x13000*1024L/FS_512B;
   while( 0 != u32_disk_copy_size )
   {
      // Define size of segment to create
      if( u32_disk_copy_size > 0xFFFF )
      {
         seg_create.u16_size = 0xFFFF;
      }else{
         seg_create.u16_size = u32_disk_copy_size;
      }
      // create segment
      if( !file_write( &seg_create ))
      {
         file_close();
         return FALSE;
      }
      // Fill segment
      id_trans = stream_mem_to_mem( LUN_ID_USB , u32_disk_copy_pos , seg_create.u8_lun , seg_create.u32_addr , seg_create.u16_size );
      if( ID_STREAM_ERR == id_trans )
      {
         file_close();
         return FALSE;
      }
      while( ID_STREAM_ERR != id_trans )
      {
         switch( stream_state( id_trans ) )  
         {
            case CTRL_BUSY:
            break;
            
            case CTRL_GOOD:
            stream_stop( id_trans );
            id_trans = ID_STREAM_ERR;
            break;

            case CTRL_FAIL:
            default:
            stream_stop( id_trans );
            id_trans = ID_STREAM_ERR;
            file_close();
            return FALSE;
            break;
         }
      }

      // Update value
      u32_disk_copy_pos    += seg_create.u16_size;
      u32_disk_copy_size   -= seg_create.u16_size;
   }

   // Copy finish and OK
   file_close();
   return TRUE;
}
*/


/*
extern Bool data_load( U8 _MEM_TYPE_SLOW_ *ram ,U8 num_sector );
#include "lib_system\memory\memory.h"

Bool load_data_via_picture_zone( void )
{
	Bool status;
   
   if( !trans_buffer_alloc())
      return FALSE;

   memset( g_trans_buffer , 0x55 , 512 );
   status = data_load( g_trans_buffer , 0 );
   status = data_load( g_trans_buffer , 1 );
   status = data_load( g_trans_buffer , 2 );
   status = data_load( g_trans_buffer , 3 );

   trans_buffer_free();
   return status;
}
*/


#endif	// _setting_c_
