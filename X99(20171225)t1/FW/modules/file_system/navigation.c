//! @file navigation.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the file system interface.
//!
//!
//! @version 1.62 snd3-dvk-1_9_5 $Id: navigation.c,v 1.62 2007/09/26 09:41:14 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _navigation_c_
#define _navigation_c_

//_____  I N C L U D E S ___________________________________________________
//#include "dp8051.h"
#include "lib_mcu\compiler.h"
#include "conf\conf_explorer.h"
//#include "modules/control_access/ctrl_access.h"
//#include "lib_system/memory/memory.h"
#include "navigation.h"
#include "file.h"
#include "stdio.h"
#include "sio\com.h"
#include "fs_patch.h"
#include "string.h"
#include "fs.h"
#include <ctype.h>
#include "sio\com.h"
#include "mmi\status\mmi_status.h"

#pragma DATA = NAVIGATION_DATA
#pragma CODE = NAVIGATION_CODE

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________

#if (FSFEATURE_WRITE == (FS_LEVEL_FEATURES & FSFEATURE_WRITE))
//! This variable store the current ID transfer used by the internal copy-paste routines
   //_MEM_TYPE_SLOW_ U8 g_id_trans_memtomem = ID_STREAM_ERR;
#endif

#if 0//(FS_NB_NAVIGATOR > 1)
//! This one containt the current navigator selected
   _MEM_TYPE_SLOW_ U8 fs_g_u8_nav_selected;
#endif

//! Variables to select string format (initialised in nav_reset())
                     Bool        g_b_unicode;

//! Global file system status used to return error
_MEM_TYPE_MEDSLOW_   Fs_status   fs_g_status;

extern   char  __X   CurrentShortFilePath[ 256 ];

//_____ D E C L A R A T I O N S ____________________________________________

//**********************************************************************
//************************ String format select ************************
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED))
#pragma CODE = FILE_TEMP_CODE02

//! This fonction select the UNICODE mode for all routines with FS_STRING parameter 
//!
//! @verbatim
//! If you have enable the FS_ASCII AND FS_UNICODE define
//! then FS_STRING parameter can be a ASCII or UNICODE table.
//! Use this routines to select the type UNICODE of string table.
//! @endverbatim
//!
void  nav_string_unicode( void )
{
   g_b_unicode = TRUE;
}
//! This fonction select the ASCII mode for all routines with FS_STRING parameter 
//!
//! @verbatim
//! If you have enable the FS_ASCII AND FS_UNICODE define
//! then FS_STRING parameter can be a ASCII or UNICODE table.
//! Use this routines to select the type ASCII for string table.
//! @endverbatim
//!
void  nav_string_ascii( void )
{
   g_b_unicode = FALSE;
}
#endif


//**********************************************************************
//********************** To optimize speed access **********************
#if 0
//! This fonction disable the disk check before each action on disk
//!
//! @verbatim
//! By default, between each read/write access a check disk (test unit ready) is send at device.
//! This check can reduce the speed access on specific disk.
//! @endverbatim
//!
void  nav_checkdisk_disable( void )
{
   g_b_no_check_disk = TRUE;
}

//! This fonction enable the disk check before each action on disk
//!
//! @verbatim
//! By default, between each read/write access a check disk (test unit ready) is send at device.
//! This check can reduce the speed access on specific disk.
//! @endverbatim
//!
void  nav_checkdisk_enable( void )
{
   g_b_no_check_disk = FALSE;
}
#endif

//**********************************************************************
//************** Initialise or Stop navigation module ****************** 

#if 0
//! This fonction reset ALL navigations to init file sytem module
//!
//! @verbatim
//! Call this one at the startup of program
//! or after a other program have change the disk (write disk access)
//! @endverbatim
//!
void  nav_reset( void )
{
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == ENABLED))
   g_b_unicode = TRUE;
#endif
   //g_b_no_check_disk = FALSE;

   fat_cache_reset();
   fat_cache_clusterlist_reset();

#if (FS_NB_NAVIGATOR > 1)
   {
   U8 i;
   // Reset variable of all navigators
   for( i=0 ; i!=FS_NB_NAVIGATOR ; i++ )
   {
      nav_select(i);
      fs_g_nav_fast.u8_type_fat = FS_TYPE_FAT_UNM; // By default the fat isn't mounted
      fs_g_nav.u8_lun = 0xFF;                      // By default select drive 0
#if (FS_MULTI_PARTITION  ==  ENABLED)
      fs_g_nav.u8_partition=0;                     // By default select the first partition
#endif
      Fat_file_close;                              // By default the file is not open
   }
   // Reset variable of default navigator
   fs_g_u8_nav_selected = 0;
   }
#else
   fs_g_nav_fast.u8_type_fat = FS_TYPE_FAT_UNM; // By default the fat isn't mounted
   fs_g_nav.u8_lun = 0xFF;                      // By default select drive 0
#  if (FS_MULTI_PARTITION  ==  ENABLED)
   fs_g_nav.u8_partition=0;                     // By default select the first partition
#  endif
   Fat_file_close;                              // By default the file is not open
#endif // (FS_NB_NAVIGATOR > 1)
}


//! This fonction select the current navigation to use
//!
//! @param     u8_idnav    Id navigator to select (0 to FS_NB_NAVIGATOR-1)
//!
//! @return    FALSE if ID navigator don't exist
//! @return    TRUE otherwise
//!
Bool  nav_select( U8 u8_idnav )
{
   if( FS_NB_NAVIGATOR <= u8_idnav )
   {
      fs_g_status = FS_ERR_BAD_NAV; // The navigator doesn't exist
      return FALSE;
   }
#if (FS_NB_NAVIGATOR > 1)
   if( fs_g_u8_nav_selected != u8_idnav )
   {
      fat_invert_nav( fs_g_u8_nav_selected );   // Deselect previous navigator = Select default navigator
      fat_invert_nav( u8_idnav );               // Select new navigator
      fs_g_u8_nav_selected = u8_idnav;
   }
#endif
   return TRUE;
}


//! This fonction return the current navigation used
//!
//! @return    u8_idnav    Id navigator selected
//!
U8    nav_get( void )
{
#if (FS_NB_NAVIGATOR > 1)
   return fs_g_u8_nav_selected;
#else
   return 0;
#endif
}
#endif

#if 0
//! This fonction copy all current navigator informations to a other navigator
//!
//! @param     u8_idnav    Id navigator where the main navigator will be to copy
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! Use this routine to open and select quickly the same file of a other navigator
//! @endverbatim
//!
Bool  nav_copy_file_current( U8 u8_idnav )
{
#if (FS_NB_NAVIGATOR > 1)
   if( fs_g_u8_nav_selected == u8_idnav )
      return TRUE;  // It is the source and destination is the same navigator
   if( 0 == u8_idnav )
      u8_idnav = fs_g_u8_nav_selected; // the default navigator is invert with the current navigator
   return fat_copy_file_current( u8_idnav );
#else
   u8_idnav++;
   return FALSE;
#endif
}

//**********************************************************************
//********************* Drive navigation fonctions *********************


//! This fonction return the number of device possible
//!
//! @return    number of device, 0 = NO DEVICE AVIABLE
//!
//! @verbatim
//! This value may be dynamic because it depends of memory drivers (e.g. in case of USB HOST)
//! @endverbatim
//!
U8    nav_drive_nb( void )
{
   return get_nb_lun(); // Number of device = Number of lun
}
//#endif


//! This fonction selects the drive in current navigator but don't mount the disk
//!
//! @param     u8_number    device number (0 to nav_drive_nb()-1 )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_drive_set( U8 u8_number )
{
   if ( !fat_check_noopen() )
      return FALSE;

   if (u8_number >= get_nb_lun() )
   {
      fs_g_status = FS_ERR_END_OF_DRIVE;   // There are not other drivers
      return FALSE;
   }

   if ( fs_g_nav.u8_lun == u8_number)
      return TRUE;   // It is the same
      
   // Go to device
   fs_g_nav.u8_lun = u8_number;
   fs_g_nav_fast.u8_type_fat = FS_TYPE_FAT_UNM;
#if (FS_MULTI_PARTITION  ==  ENABLED)
   fs_g_nav.u8_partition=0;   // by default select the first partition
#endif
   return TRUE;
}

//#if 0
//! This fonction return the drive number selected in current navigator
//!
//! @return    0 to nav_drive_nb()-1
//! @return    0xFF in case of no drive selected
//!
U8    nav_drive_get( void )
{
#if (FS_MULTI_PARTITION  ==  ENABLED)
   if(0xFF == fs_g_nav.u8_lun)
      return 0xFF;
   return ((fs_g_nav.u8_lun*4) + fs_g_nav.u8_partition); // Maximum 4 partitions by device
#else
   return (fs_g_nav.u8_lun);
#endif
}


//! This fonction return the drive letter selected in current navigator
//!
//! @return    'A','B',...
//! @return    'X', in case of no drive selected
//!
U8    nav_drive_getname( void )
{
   if(0xFF == fs_g_nav.u8_lun)
      return 'X';
#if (FS_MULTI_PARTITION  ==  ENABLED)
   return ('A' + (fs_g_nav.u8_lun*4) + fs_g_nav.u8_partition); // Maximum 4 partitions by device
#else
   return ('A' + fs_g_nav.u8_lun);
#endif
}
#endif

#if (FSFEATURE_WRITE_COMPLET == (FS_LEVEL_FEATURES & FSFEATURE_WRITE_COMPLET))
//! This fonction format the current drive (disk)
//!
//! @param     u8_fat_type    Select the type of format <br>
//!            FS_FORMAT_DEFAULT, The file system module choose the better FAT format for the drive space <br>
//!            FS_FORMAT_FAT, The FAT12 or FAT16 is used to format the drive, if possible (disk space <2GB) <br>
//!            FS_FORMAT_FAT32, The FAT32 is used to format the drive, if possible (disk space >32MB) <br>
//!            FS_FORMAT_NOMBR_FLAG, if you don't want a MRB in disk then add this flag (e.g. FAT format on a CD support)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine can't format a multi-partiton, if the disk containt as a multi-partition
//! then the multi-partition is erase and replace by a single partition on ALL disk space.
//! @endverbatim
//!
#if 0
Bool  nav_drive_format( U8 u8_fat_type )
{
   if ( !fat_check_noopen() )
      return FALSE;
   if ( !fat_check_nav_access_disk() )
      return FALSE;
   if ( !fat_format( u8_fat_type ) )
      return FALSE;
   return fat_mount(); // mount the new FS
}
#endif
#endif  // FS_LEVEL_FEATURES


//**********************************************************************
//******************* Partition navigation fonctions ******************* 


#if 0//(FS_MULTI_PARTITION  ==  ENABLED)
//! This fonction return the number of partition present on current drive selected
//!
//! @return    u8_number   number of partition
//!
U8    nav_partition_nb( void )
{
   return fat_get_nbpartition();
}


//! This fonction select a partition available on current drive
//!
//! @param     partition_number     partition number (0 to 3)
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_partition_set( U8 partition_number )
{
   if ( fs_g_nav.u8_partition == partition_number)
      return TRUE;   // It is the same
      
   if ( !fat_check_noopen() )
      return FALSE;

   // Go to partition
   fs_g_nav.u8_partition = partition_number;
   fs_g_nav_fast.u8_type_fat = FS_TYPE_FAT_UNM;
   return TRUE;
}
#endif

#if 0
//! This fonction mount the current partition selected 
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! If the option FS_MULTI_PARTITION is disable
//! then the mount routine select the first partition supported by file system. <br>
//! After mount, the file list corresponding at the files and directorys present in ROOT directory 
//! @endverbatim
//!
Bool  nav_partition_mount( void )
{
   if ( !fat_check_noopen() )
      return FALSE;

   if( FS_TYPE_FAT_UNM != fs_g_nav_fast.u8_type_fat)
   {
      // Already mount
      // Go to root directory
      fs_g_nav.u32_cluster_sel_dir   = 0;
      // No file is selected
      fat_clear_entry_info_and_ptr();
      return TRUE;
   }

   return fat_mount(); // mount the current partition and device
}
#endif

//! This fonction give the partition type
//!
//! @return partition type : FS_TYPE_FAT_12, FS_TYPE_FAT_16, FS_TYPE_FAT_32
//! @return FS_TYPE_FAT_UNM, in case error or unknow format
//!
#if 0
U8    nav_partition_type( void )
{
   fat_check_device();
   if( FS_TYPE_FAT_UNM == fs_g_nav_fast.u8_type_fat)
   {
      nav_partition_mount();
   }
   return fs_g_nav_fast.u8_type_fat;
}
#endif
#if 0//( LCD_PRESENT == ENABLE )
//! This fonction read or write the serial number of current partition selected
//!
//! @param     b_action    to select the action <br>
//!                        FS_SN_READ to read serial number <br>
//!                        FS_SN_WRITE to write serial number <br>
//! @param     a_u8_sn     pointer on a array of 4 bytes <br>
//!                        if FS_SN_READ, then use to store serial number <br>
//!                        if FS_SN_WRITE, then use to pass the new serial number <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_partition_serialnumber( Bool b_action , U8 _MEM_TYPE_SLOW_ *a_u8_sn )
{
   if( !fat_check_mount())
      return FALSE;
      
   return fat_serialnumber( b_action , a_u8_sn );
}
#endif
#if 0
//! This fonction read or write the label of current partition selected
//!
//! @param     b_action    to select the action <br>
//!                        FS_LABEL_READ to read label <br>
//!                        FS_LABEL_WRITE to write label <br>
//! @param     sz_label    pointer on a string ASCII of 11 characters + NULL terminator (=12 bytes) <br>
//!                        if FS_LABEL_READ, then use to store label <br>
//!                        if FS_LABEL_WRITE, then use to pass the new label <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_partition_label( Bool b_action , FS_STRING sz_label )
{
   _MEM_TYPE_SLOW_   Fs_index index;
   Bool status = TRUE;
         
   // Save index
   index = nav_getindex();       // Save current position

   // Go to root dir
   if( nav_dir_root())
   {
      // find field label
      fs_g_nav_fast.u16_entry_pos_sel_file = 0; // Go to first entry
      while( 1 )
      {
         if ( !fat_read_dir())
         {
            status = FALSE;
            break; // error
         } 
         
         if( fat_entry_label( FS_LABEL_READ , NULL ) )
            break;
         if( FS_ERR_ENTRY_EMPTY == fs_g_status )
            break;
         fs_g_nav_fast.u16_entry_pos_sel_file++;
      }

      if( TRUE == status )
      {
         // Find OK
         if( FS_LABEL_READ == b_action )
         {
            // change field label
            if( !fat_entry_label( FS_LABEL_READ , sz_label ) )
               sz_label[0]=0; // empty name
         }else{
            // change field label
            status = fat_entry_label( FS_LABEL_WRITE , sz_label );
         }
      }  
   }
   // Restore index
   nav_gotoindex( &index );
   return status;
}
#endif
#if 0
//! This fonction return the total space of the current partition
//!
//! @return    number of sector
//! @return    0, in case of error
//!
//! @verbatim
//! You shall mount the partition before call this routine
//! @endverbatim
//!
U32   nav_partition_space( void )
{
   fat_check_device();

   //! Check list before navigation in file
   if (FS_TYPE_FAT_UNM == fs_g_nav_fast.u8_type_fat)
      return 0;
      
   return (fs_g_nav.u32_CountofCluster * fs_g_nav.u8_BPB_SecPerClus);
}


//! This fonction return the cluster size of the current partition
//!
//! @return    cluster size (unit sector)
//!
U8    nav_partition_cluster_size( void )
{
   return fs_g_nav.u8_BPB_SecPerClus;
}

//! This fonction return the free space of the current partition
//!
//! @return    number of free sector
//! @return    0 in case of error or full partition
//!
//! @verbatim
//! You shall mount the partition before call this routine
//! @endverbatim
//!
U32   nav_partition_freespace( void )
{
   fat_check_device();

   //! Check list before navigation in file
   if (FS_TYPE_FAT_UNM == fs_g_nav_fast.u8_type_fat)
      return 0;
   return fat_getfreespace();
}


//! This fonction return the space free in percent of the current partition
//!
//! @return    percent of free space (0% to 100%)
//! @return    0% in case of error or full partition
//!
//! @verbatim
//! To speed up the compute, the resultat have a error delta of 1%
//! @endverbatim
//!
U8    nav_partition_freespace_percent( void )
{
   fat_check_device();

   //! Check list before navigation in file
   if (FS_TYPE_FAT_UNM == fs_g_nav_fast.u8_type_fat)
      return 0;
   return fat_getfreespace_percent();
}
#endif

//**********************************************************************
//****************** File list navigation fonctions ******************** 

#pragma CODE = NAVIGATION_CODE

//! This fonction reset the pointer of selection, so "no file selected" in file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_reset( void )
{
   return	FSSeekEntry(( __X FILE_INFO * )fat_g_cur_nav, ( __X FS_ENTRY_INFO *)fat_g_cur_entry, 0 ) != FS_NO_ERROR;
}

#if 0
//! This fonction check if a file is selected
//!
//! @return    TRUE  if a file is selected
//! @return    FALSE if no file is selected
//!
Bool  nav_filelist_validpos( void )
{
   return fat_check_mount_select_noopen();
}


//! This fonction check if no file is open
//!
//! @return    TRUE  if no file is open
//! @return    FALSE if a file is open
//!
Bool  nav_filelist_fileisnotopen( void )
{
   return fat_check_noopen();
}
#endif

//! This fonction move the pointer of selection in file list
//!
//! @param     u16_nb      numbers file to jump before stopping action <br>
//!                        0, stop at the first file found <br>
//!                        1, stop at the second file found <br>
//!
//! @param     b_direction search direction <br>
//!                        FS_FIND_NEXT, move to next file or directory
//!                        FS_FIND_PREV, move to previous file or directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! Note: if no file is selected then nav_filelist_set( 0 , FS_NEXT ) go to first file of the File list.
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_set( U16 u16_nb , Bool b_direction )
{
   ++u16_nb;
   if( b_direction == FS_FIND_NEXT )
   {
      while( u16_nb )
      {
         if( FSNextValidEntry(( FILE_INFO __X *)fat_g_cur_nav ) != FS_NO_ERROR )
            return FALSE;
         --u16_nb;
      }
   }
   else
   {
      while( u16_nb )
      {
         if( FSPrevValidEntry(( FILE_INFO __X *)fat_g_cur_nav ) != FS_NO_ERROR )
            return FALSE;
         --u16_nb;
      }
   }
   return TRUE;
}

#if 0
//! This fonction return the position of selected file in file list
//!
//! @return    position of selected file in current directory, 0 is the first position
//! @return    FS_NO_SEL, in case of no file selected
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
U16   nav_filelist_get( void )
{
   return fat_g_cur_nav->index;
}


//! This fonction go to a position in file list
//!
//! @param     u16_newpos     new position to select in file list, 0 is the first position
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_goto( U16 u16_newpos )
{
   FS_ENTRY_INFO _MEM_TYPE_SLOW_ entry;
   return FSSeekEntry( fat_g_cur_nav, &entry, u16_newpos ) == FS_NO_ERROR;
}
#endif


#pragma CODE = NAVIGATION_TEMP_CODE03

//! This fonction search a file name in file list 
//!
//! @param     sz_name        name to search (UNICODE or ASCII) <br>
//!                           It must be terminate by NULL or '*' value
//! @param     b_match_case   FALSE to ignore the case
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This fonction start a search at the current position the file name in file list 
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_findname( FS_STRING sz_name , Bool b_match_case )
{
   unsigned char _MEM_TYPE_SLOW_ buffer[ FS_MAX_NAME_BUFFER ];
//   if( FSSeekEntry( fat_g_cur_nav, fat_g_cur_entry, 0 ) == FS_END_DIRECTORY )
//      return FALSE;
   if( FSNearValidEntry(( FILE_INFO __X *)fat_g_cur_nav, 0 ) == FS_END_DIRECTORY )
      return FALSE;	

   if( g_b_unicode )
   {
#if	1
      do
      {
         //	local_to_unicode((U8 *)fat_g_cur_nav->lfn,(U16 *)buffer,sizeof(buffer));
         if( str_comp_u(( U16 *)sz_name, ( U16 *)fat_g_cur_nav->lfn, b_match_case ) == 0 )
            return TRUE;
         unicode_to_local(( U16 *)sz_name, buffer, sizeof( buffer ));
         if( str_comp_a(( U8 *)buffer, ( U8 *)fat_g_cur_nav->name, b_match_case ) == 0 )
            return TRUE;
      }
      while( FSNextValidEntry(( FILE_INFO __X *)fat_g_cur_nav ) == FS_NO_ERROR );
#endif		
   }
   else
   {
      do
      {
         if( str_comp_u( atou( sz_name ), ( U16 *)fat_g_cur_nav->lfn, b_match_case ) == 0 )
            return TRUE;
         if( str_comp_a(( U8 *)sz_name, ( U8 *)fat_g_cur_nav->name, b_match_case ) == 0 )
            return TRUE;
      }
      while( FSNextValidEntry(( FILE_INFO __X *)fat_g_cur_nav ) == FS_NO_ERROR );
   }
   return FALSE;
}

#if 0
//! This fonction check the end of file list
//!
//! @return    FALSE, NO end of file list
//! @return    TRUE, in case of end of list or error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_eol( void )
{
   if( FALSE == nav_filelist_set( 0 , FS_FIND_NEXT ) )
      return TRUE;   // end of list or error (remark: the position haven't changed)
   
   // the next file is the first of the list ?
   if( 0 != fs_g_nav.u16_pos_sel_file )
   {
      // Go to previous position
      nav_filelist_set( 0 , FS_FIND_PREV );
   }else{
      // No select file before eol fnct
      fs_g_nav_fast.u16_entry_pos_sel_file= FS_NO_SEL;
      fs_g_nav.u16_pos_sel_file           = FS_NO_SEL;
      fs_g_nav.b_mode_nav                 = FS_DIR;
   }
   return FALSE;
}


//! This fonction check the beginning of file list
//!
//! @return    FALSE, NO begining of file list
//! @return    TRUE, in case of the file selected is the first file, or in case of error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_bol( void )
{
   if( FALSE == nav_filelist_set( 0 , FS_FIND_PREV ) )
      return TRUE;   // end of list or error (remark: the position haven't changed)
   // Go to previous position
   nav_filelist_set( 0 , FS_FIND_NEXT );
   return FALSE;
}


//! This fonction check the presence of files or directorys in file list
//!
//! @param     b_type   FS_DIR  to check the directory presence <br>
//!                     FS_FILE to check the file presence <br>
//!
//! @return    TRUE, in case of a file or a directory exist
//! @return    FALSE, in case of no file or no directory exist, or error
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_exist( Bool b_type )
{
   Bool status;
   U16   u16_save_position;
   
   // save current position
   u16_save_position = fs_g_nav.u16_pos_sel_file;
   status = nav_filelist_first( b_type );

   // reselect previous position
   nav_filelist_reset();
   if ( u16_save_position != FS_NO_SEL )
   {  // After operation, there are a file selected
      nav_filelist_set( u16_save_position , FS_FIND_NEXT );
   }
   return status;   
}


//! This fonction compute the number of files or directorys in file list
//!
//! @param     b_type   FS_DIR  to compute the directory presence <br>
//!                     FS_FILE to compute the file presence <br>
//!
//! @return    number of file or directory present in file list
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
U16   nav_filelist_nb( Bool b_type )
{
   U16   u16_save_position;
   U16   u16_save_number_dir;
   U16   u16_save_number_file;
   
   // save current position
   u16_save_position = fs_g_nav.u16_pos_sel_file;

   // Reset position
   if ( !nav_filelist_reset())
      return 0;

   // Scan all directory
   u16_save_number_dir  = 0;
   u16_save_number_file = 0;
   while( nav_filelist_set( 0 , FS_FIND_NEXT ) )
   {
      // Check if file or dir
      if( FS_FILE == fs_g_nav.b_mode_nav )
         u16_save_number_file++;    // It is a file
      else
         u16_save_number_dir++;     // It is a directory
   }
   
   // reselect previous position
   nav_filelist_reset();
   if ( u16_save_position != FS_NO_SEL )
   {  // After operation, there are a file selected
      nav_filelist_set( u16_save_position , FS_FIND_NEXT );
   }
   
   // Return the value asked
   if( FS_FILE == b_type )
      return u16_save_number_file;
   else
      return u16_save_number_dir;
}


//! This fonction go to at the first file or directory in file list
//!
//! @param     b_type   FS_DIR  to go at the first directory <br>
//!                     FS_FILE to go at the first file <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_first( Bool b_type )
{
   // Reset position
   if ( !nav_filelist_reset())
      return FALSE;
   
   // Find the first file corresponding of type
   while( nav_filelist_set( 0 , FS_FIND_NEXT ) )
   {
      // Check if file
      if( b_type == fs_g_nav.b_mode_nav )
         return TRUE;
   }
   
   fs_g_status = FS_ERR_NO_FIND;
   return FALSE;  // NO FILE FOUND
}


//! This fonction go to at the last file or directory in file list
//!
//! @param     b_type   FS_DIR  to go at the last directory <br>
//!                     FS_FILE to go at the last file <br>
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_filelist_last( Bool b_type )
{
   U16 u16_nb;

   // Get nb file of good type
   u16_nb = nav_filelist_nb( b_type  );
   if( 0 == u16_nb )
   {
      // No file with a good type
      fs_g_status = FS_ERR_NO_FIND;
      return FALSE;  // NO FILE FOUND
   }

   // Go to the first file
   if ( !nav_filelist_first( b_type ))
      return FALSE;
      
   // If there are more one file, then go to last of list
   if( 1 == u16_nb )
      return TRUE;
   u16_nb -= 2;
   return nav_filelist_set( u16_nb , FS_FIND_NEXT );
}


//**********************************************************************
//************************ Index fonctions *****************************


//! This fonction return a small pointer on the current file system position (disk, partition, dir, file/dir selected )
//!
//! @return    It is a small index structure with the informations on file system position (disk, partition, dir, file/dir selected )
//!
//! @verbatim
//! This routine is interresting to save a file pointer in small variable.
//! This pointer permit to reinit a navigator quickly with nav_gotoindex() routine.
//! @endverbatim
//!
Fs_index nav_getindex( void )
{
   Fs_index index;

   //!< number of logical driver
   index.u8_lun = fs_g_nav.u8_lun;
#if (FS_MULTI_PARTITION  ==  ENABLED)
   index.u8_partition = fs_g_nav.u8_partition;
#endif
   //!< number of first cluster of directory selected (0 for the root directory)
   index.u32_cluster_sel_dir = fs_g_nav.u32_cluster_sel_dir;
   //!< entry offset of file selected in directory selected (unit = FS_SIZE_FILE_ENTRY)
   index.u16_entry_pos_sel_file = fs_g_nav_fast.u16_entry_pos_sel_file;
   return index;
}


//! This fonction reinit a navigator via a file pointer
//!
//! @param     index    It is a structure with the informations on file system position (disk, partition, dir, file/dir selected )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! This routine permit to reinit a navigator quickly via file pointer (disk, partition, dir, file/dir selected )
//! To get a file pointer use the routine nav_getindex()
//! @endverbatim
//!
Bool  nav_gotoindex( const Fs_index _MEM_TYPE_SLOW_ *index )
{
   if( !nav_drive_set( index->u8_lun ))
      return FALSE;
#if (FS_MULTI_PARTITION  ==  ENABLED)
   if( !nav_partition_set(index->u8_partition))
      return FALSE;
#endif
   if( !nav_partition_mount())
      return FALSE;

   // Initialisation of the current entry file with index informations
   fs_g_nav.u32_cluster_sel_dir   = index->u32_cluster_sel_dir;

   // Reset position
   if ( !nav_filelist_reset())
      return FALSE;

   // Research the index in directory
   while( fs_g_nav_fast.u16_entry_pos_sel_file != index->u16_entry_pos_sel_file )
   {
      if( !nav_filelist_set( 0 , FS_FIND_NEXT ) )
      {
         nav_filelist_reset();
         return FALSE;
      }
   }
   return TRUE;
}
#endif

//**********************************************************************
//************************ Directory fonctions *************************

#pragma CODE = NAVIGATION_TEMP_CODE00

//! This fonction init the file list on the root directory
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_root( void )
{
   return FSPatchInit(( __X FILE_INFO *)fat_g_cur_nav, ( __X FS_ENTRY_INFO *)fat_g_cur_entry );
}


#pragma CODE = NAVIGATION_TEMP_CODE01

//! This fonction enter in the current directory selected in file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After this routine the file list change and containt the files of new current directory,
//! also no file is selected
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool nav_dir_cd( void )
{
   return FSSubdirEntry(( __X FILE_INFO *)fat_g_cur_nav, ( __X FS_ENTRY_INFO *)fat_g_cur_entry ) == FS_NO_ERROR;
}

#if 0
//! This fonction go to at the parent directory of current directory corresponding at file list
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! After, the file list change and containt the files of parent directory,
//! also the file selected is the old directory.
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_gotoparent( void )
{
   U32 u32_cluster_old_dir;

   if (!fat_check_mount_noopen())
      return FALSE;
   
   if (0 == fs_g_nav.u32_cluster_sel_dir)
   {
      fs_g_status = FS_ERR_IS_ROOT;        // There aren't parent
      return FALSE;
   }

   // Select directory ".."
   fs_g_nav_fast.u16_entry_pos_sel_file = 1;
   if ( !fat_read_dir())
      return FALSE;
   fat_get_entry_info();
   
   // Save the sub directory cluster for select the previous sub directory at the end of "cd.."
   u32_cluster_old_dir = fs_g_nav.u32_cluster_sel_dir;
   // Select the directory ".."
   fs_g_nav.u32_cluster_sel_dir = fs_g_nav_entry.u32_cluster;

   // Find the old dir in your parent dir
   if( FALSE == nav_filelist_reset())
      return FALSE;
   // Scan the list
   while( nav_filelist_set( 0 , FS_FIND_NEXT ) )
   {
      if (fs_g_nav_entry.u32_cluster == u32_cluster_old_dir)
         return TRUE;   // It is the old directory
   }

   fs_g_status = FS_ERR_FS;   // File system error
   return FALSE;
}


//! This fonction return the directory name corresponding at the file list
//!
//! @param     sz_path        buffer to store the name file (ASCII or UNICODE )
//! @param     u8_size_max    buffer size (unit ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_name( FS_STRING sz_path  , U8 u8_size_max  )
{
   Bool status;
   _MEM_TYPE_SLOW_   Fs_index index;
   U8 u8_i, u8_character;

   if ( !fat_check_mount_noopen())
      return FALSE;

   index = nav_getindex();       // Save current position
   if ( nav_dir_gotoparent() )   // Go to parent directory and select the current directory
   {
      status = nav_file_name( sz_path  , u8_size_max , FS_NAME_GET , FALSE  );
   }
   else
   {
      if ( FS_ERR_IS_ROOT == fs_g_status )
      {
         // Create a device name
         for( u8_i = 0 ; u8_i<3 ; u8_i++ )
         {
            switch( u8_i )
            {
               case 0:
               u8_character = nav_drive_getname();    // Letter
               break;
               case 1:
               u8_character = ':';            			// ":"
               break;
               case 2:
               u8_character = 0;              			// end of string
               break;
            }
            if( Is_unicode )
            {
               ((FS_STR_UNICODE)sz_path )[0] = u8_character;
            }else{
               sz_path [0] = u8_character;
            }
            sz_path  += (Is_unicode? 2 : 1 );
         }
         status = TRUE;
      }
      else
      {
         status = FALSE;
      }
   }
   // Reinitialise the current position
   nav_gotoindex( &index );
   return status;
}
#endif

#if (FSFEATURE_WRITE_COMPLET == (FS_LEVEL_FEATURES & FSFEATURE_WRITE_COMPLET))
#pragma CODE = NAVIGATION_TEMP_CODE02

//! This fonction create a sub directory in the directory correcponding at file list
//!
//! @param     sz_name     buffer with the directory name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_dir_make( FS_STRING sz_name  )
{
   char __X temp[ 32 ];
   FS_ERR error;

   strcpy(( char *)temp, ( char *)CurrentShortFilePath );
   strcat(( char *)temp, ( char *)"\\" );
   strcat(( char *)temp, ( char *)sz_name );
   error = FSDirectory(( char *)temp );
   //error = FSDirForUnicode( CurrentShortFilePath, ( char *)sz_name, ( U8 *)atou( sz_name ));
   /*SendString((U8 *)"error=");
   send_hex_u8(error);
   SendString((U8 *)"\r\n");*/
   if( error == FS_DIRECTORY_CONFLICT )
      fs_g_status = FS_ERR_FILE_EXIST;
   return	( error == FS_NO_ERROR );
}
#endif  // FS_LEVEL_FEATURES


#if 0
//! This fonction return the path of the current file list
//!
//! @param     sz_path              buffer to store path (ASCII or UNICODE )
//! @param     u8_size_path         buffer size (unit ASCII or UNICODE )
//! @param     b_view_file_select   TRUE, include in path the name of file selected
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_getcwd( FS_STRING sz_path  , U8 u8_size_path , Bool b_view_file_select  )
{
   Bool status;
   _MEM_TYPE_SLOW_   Fs_index index;
   U8 u8_save_size_path, u8_i;


   if ( !fat_check_mount_noopen())
      return FALSE;

   // Save current position
   index = nav_getindex();


   // Set flag end of path
   u8_save_size_path = u8_size_path;
   u8_size_path--;
   if( Is_unicode )
   {
      ((FS_STR_UNICODE)sz_path )[u8_size_path] = 0;
   }else{
      sz_path [u8_size_path]   = 0;
   }

   // Loop for read each dir name of path
   while( 1 )    // Go to parent directory and select the current directory
   {
      if( b_view_file_select )
      {
         b_view_file_select = FALSE;
         // File selected ?
         if( !fat_check_select() )
            continue;   // No file selected, then restart loop to read first directory
      }
      else
      {
         if( !nav_dir_gotoparent() )    // Go to parent directory and select the current directory
            break;
      }
   
      // Read name
      if( !nav_file_name( sz_path  , u8_size_path , FS_NAME_GET, FALSE  ))
         break;

      // Compute the size of name
      u8_i = 0;
      while( 1 )
      {
         if( Is_unicode )
         {
            if( 0 == ((FS_STR_UNICODE)sz_path )[u8_i])
               break;
         }else{
            if( 0 == sz_path [u8_i])
               break;
         }
         u8_i++;
      }
      
      // check size buffer
      if( (u8_i+1) == u8_size_path )
      {
         fs_g_status = FS_ERR_BUFFER_FULL;   // The buffer of path name is full
         break;
      }
      
      // Move the name at the end of path
      while( 0 != u8_i )
      {
         u8_i--;
         u8_size_path--;
         if( Is_unicode )
         {
            ((FS_STR_UNICODE)sz_path )[u8_size_path] = ((FS_STR_UNICODE)sz_path )[u8_i];
         }else{
            sz_path [u8_size_path]   = sz_path [u8_i];
         }
      }
      // Set '\' caracter before name
      u8_size_path--;
      if( Is_unicode )
      {
         ((FS_STR_UNICODE)sz_path )[u8_size_path] = '\\';
      }else{
         sz_path [u8_size_path]   = '\\';
      }
   }

   if ( FS_ERR_IS_ROOT == fs_g_status )
   {
      // End of path -> add device name
      if( 2 > u8_size_path )
      {
         fs_g_status = FS_ERR_BUFFER_FULL;   // The buffer of path name is full
         status = FALSE;
      }
      else
      {
         // Create a device name
         if( Is_unicode )
         {
            ((FS_STR_UNICODE)sz_path )[0] = nav_drive_getname(); 	// Letter
            ((FS_STR_UNICODE)sz_path )[1] = ':';             		// ":"
         }else{
            sz_path [0] = nav_drive_getname(); 	// Letter
            sz_path [1] = ':';             		   // ":"
         }

         // Move all path at the beginning of buffer
         u8_i = 2;
         while( u8_save_size_path != u8_size_path )
         {
            if( Is_unicode )
            {
               ((FS_STR_UNICODE)sz_path )[u8_i] = ((FS_STR_UNICODE)sz_path )[u8_size_path];
            }else{
               sz_path [u8_i] = sz_path [u8_size_path];
            }
            u8_i++;
            u8_size_path++;
         }
         status = TRUE;
      }
   }
   else
   {  // Error system
      status = FALSE;
   }

   // Reinitialise the current position
   nav_gotoindex( &index );
   return status;
}


//! This fonction init navigator position via a path
//!
//! @param     sz_path           buffer with the path (ASCII or UNICODE )
//! @param     b_match_case      FALSE to ignore the case
//! @param     b_create          TRUE, if path no exist then create it <br>
//!                              FALSE, no create path <br>
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! The syntact "./../../file_name" is supported.
//! With syntact "./dir_parent/directory_name"  the file list corresponding at "dir_parent" and "directory_name" is selected.
//! With syntact "./dir_parent/directory_name/" the file list corresponding at "directory_name" and no file is selected.
//! @endverbatim
//!
Bool  nav_setcwd( FS_STRING sz_path , Bool b_match_case , Bool b_create )
{
   _MEM_TYPE_SLOW_   Fs_index index;
   FS_STRING sz_save_path = 0;
   Bool b_create_name = FALSE;

   if ( !fat_check_noopen())
      return FALSE;

   // Save current position
   index = nav_getindex();

   // Check syntact "\path..."
   if( (( Is_unicode) && (('\\'  == ((FS_STR_UNICODE)sz_path )[0]) || ('/'  == ((FS_STR_UNICODE)sz_path )[0])) )
   ||  ((!Is_unicode) && (('\\'  == sz_path [0]) || ('/'  == sz_path [0])) ) )
   {
      // Go to root dir of current drive
      if( !nav_dir_root())
         goto nav_setcwd_fail;
      sz_path  += (Is_unicode? 2 : 1 );
   }else
   
   // Check syntact "x:\path..."
   if( (( Is_unicode) && (( ':'  == ((FS_STR_UNICODE)sz_path )[1] ) && (('\\'  == ((FS_STR_UNICODE)sz_path )[2] ) || ('/'  == ((FS_STR_UNICODE)sz_path )[2]))) )
   ||  ((!Is_unicode) && (( ':'  == sz_path [1] ) && (('\\'  == sz_path [2] ) || ('/'  == sz_path [2]))) ) )
   {
      // Go to drive
      if( Is_unicode )
      {
         if( !nav_drive_set( ((FS_STR_UNICODE)sz_path )[0]-'A' ) )
            goto nav_setcwd_fail;
      }else{
         if( !nav_drive_set( sz_path [0]-'A' ) )
            goto nav_setcwd_fail;
      }
      if( !nav_partition_mount())
         goto nav_setcwd_fail;
      sz_path  += 3*(Is_unicode? 2 : 1 );
   }else

   // Check syntact ".\path..."
   if( (( Is_unicode) && (( '.'  == ((FS_STR_UNICODE)sz_path )[0] ) && (('\\'  == ((FS_STR_UNICODE)sz_path )[1] ) || ('/'  == ((FS_STR_UNICODE)sz_path )[1] ))) )
   ||  ((!Is_unicode) && (( '.'  == sz_path [0] ) && (('\\'  == sz_path [1] ) || ('/'  == sz_path [1] ))) ) )
   {
      // Search in current directory
      sz_path  += 2*(Is_unicode? 2 : 1 );
   }else
   
   {
      // Check syntact "..\..\path..."
      if( Is_unicode )
      {
         while(( '.'  == ((FS_STR_UNICODE)sz_path )[0] )
         &&    ( '.'  == ((FS_STR_UNICODE)sz_path )[1] )
         &&    (('\\'  == ((FS_STR_UNICODE)sz_path )[2]) || ('/'  == ((FS_STR_UNICODE)sz_path )[2]) || (0  == ((FS_STR_UNICODE)sz_path )[2])) )
         {
            // Go to parent directory
            if( !nav_dir_gotoparent() )
               goto nav_setcwd_fail;
            sz_path  += (2*2); // jump ".."
            if( 0 != ((FS_STR_UNICODE)sz_path )[0])
               sz_path  += (2*1); // jump "/"
         }
      }else{
         while(( '.'  == sz_path [0] )
         &&    ( '.'  == sz_path [1] )
         &&    (('\\'  == sz_path [2]) || ('/'  == sz_path [2]) || (0  == sz_path [2])) )
         {
         // Go to parent directory
         if( !nav_dir_gotoparent() )
            goto nav_setcwd_fail;
            sz_path  += 2; // jump ".."
            if( 0 != sz_path [0])
               sz_path  +=1; // jump "/"
         }
      }
   }

   // Reset list to start the search at the beginning
   if( !nav_filelist_reset())
      goto nav_setcwd_fail;

   while( 1 )
   {
      if( (( Is_unicode) && ( 0 == ((FS_STR_UNICODE)sz_path )[0] ) )
      ||  ((!Is_unicode) && ( 0 == sz_path [0] ) ) )
      {
         return TRUE;   // path (without file) is find or create
      }
      if( !nav_filelist_findname( sz_path  , b_match_case  ))
      {
         // Fileor folder not found
         if( !b_create )
            goto nav_setcwd_fail;   // don't creat path then exit
         // Set flag to create path
         b_create_name = TRUE;
         sz_save_path = sz_path;
      }
      
      while( 1 )
      {
         sz_path  += (Is_unicode? 2 : 1 );
         if( (( Is_unicode) && ( 0 == ((FS_STR_UNICODE)sz_path )[0] ) )
         ||  ((!Is_unicode) && ( 0 == sz_path [0] ) ) )
         {
            // Is it the last name of path and it is a file
            if( b_create_name )
            {
               // the file must be create
               nav_file_create( sz_save_path );
            }
            break;   // file of path is found or created, then end of set_cwd
         }

         if( (( Is_unicode) && (('\\' == ((FS_STR_UNICODE)sz_path )[0] ) || ('/' == ((FS_STR_UNICODE)sz_path )[0] )) )
         ||  ((!Is_unicode) && (('\\' == sz_path [0] ) || ('/' == sz_path [0] )) ) )
         {
            // Is it a folder name
            if( b_create_name )
            {
               // The folder don't exist and it must be create
               nav_dir_make( sz_save_path );
            }
            if( !fat_entry_is_dir() )
               goto nav_setcwd_fail;
            // jump '\'
            sz_path  += (Is_unicode? 2 : 1 );
            if( !nav_dir_cd())
               goto nav_setcwd_fail;
            break;
         }
      }
      
   }
      
nav_setcwd_fail:
   // Reinitialise the current position
   nav_gotoindex( &index );
   return FALSE;
}
#endif



//**********************************************************************
//*********************** File control fonctions ***********************

#pragma CODE = NAVIGATION_CODE

//! This fonction return name of file selected or check the name of file selected
//!
//! @param     b_mode         action mode: <br>
//!                           FS_NAME_GET    to get the name of file selected <br>
//!                           FS_NAME_CHECK  to check the name of file selected <br>
//! @param     sz_name        if FS_NAME_GET    then buffer to store the name file (ASCII or UNICODE ) <br>
//!                           if FS_NAME_CHECK  then buffer containt the filter name (ASCII or UNICODE),
//!                                             it must be terminate by NULL or '*' value <br>
//! @param     b_match_case   FALSE, ignore the case (only used in "FS_NAME_CHECK" action mode)
//!
//! @param     u8_size_max    buffer size (unit ASCII or UNICODE ), only used in "FS_NAME_GET" action mode
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! "File list" is a list of files and directorys present in current directory
//! @endverbatim
//!
Bool  nav_file_name( FS_STRING sz_name , U8 u8_size_max , Bool b_mode , Bool b_match_case  )
{
   unsigned char	_MEM_TYPE_SLOW_ buffer[ FS_MAX_NAME_BUFFER ];

   if( g_b_unicode )
   {
#if 1
      if( b_mode == FS_NAME_GET )
      {
         if( *( U16 *)fat_g_cur_nav->lfn != 0 )
         {
            //local_to_unicode(( U8 *)fat_g_cur_nav->lfn, ( U16 *)buffer, sizeof( buffer ));
            str_copy_u_n(( U16 *)sz_name, ( U16 *)fat_g_cur_nav->lfn, u8_size_max - 1 );
         }
         else
         {
            local_to_unicode(( U8 *)fat_g_cur_nav->name, ( U16 *)buffer, sizeof( buffer ));
            str_copy_u_n(( U16 *)sz_name, ( U16 *)buffer, min( 13, sizeof( buffer )/2));
         }
         return TRUE;
      }
      else
      {
         //local_to_unicode(( U8 *)fat_g_cur_nav->lfn, ( U16 *)buffer, sizeof( buffer ));
         if( str_comp_u(( U16 *)sz_name, ( U16 *)fat_g_cur_nav->lfn, b_match_case ))
            return TRUE;
         local_to_unicode(( U8 *)fat_g_cur_nav->name, ( U16 *)buffer, sizeof( buffer ));
         if( str_comp_u(( U16 *)sz_name, ( U16 *)buffer, b_match_case ))
            return TRUE;
      }
#else
      return	FALSE;
#endif
   }
   else
   {
      if( b_mode == FS_NAME_GET )
      {
         if( *( U16 *)fat_g_cur_nav->lfn != 0 )
         {
            str_copy_n(( U8 *)sz_name, fat_g_cur_nav->lfn, u8_size_max );
            //unicode_to_local( fat_g_cur_nav->lfn, ( U8 *)sz_name, u8_size_max );
         }
         else
         {
            str_copy_n(( U8 *)sz_name, fat_g_cur_nav->name, u8_size_max );
         }
         return TRUE;
      }
      else
      {
         if( str_comp_a(( U8 *)sz_name, ( U8 *)fat_g_cur_nav->lfn, b_match_case ))
            return TRUE;
         if( str_comp_a(( U8 *)sz_name, ( U8 *)fat_g_cur_nav->name, b_match_case ))
            return TRUE;
      }
   }
   return FALSE;
}

#if 0
//! This fonction return the size of the file selected
//!
//! @return    Size of file selected (unit byte)
//!
U32   nav_file_lgt( void )
{
   return fs_g_nav_entry.u32_size;
}
#endif
#if 0
//! This fonction return sector size of the file selected
//!
//! @return    Size of file selected (unit 512B)
//!
U16   nav_file_lgtsector( void )
{
   return (fs_g_nav_entry.u32_size >> FS_512B_SHIFT_BIT);
}


//! This fonction check the disk write protection and attribut "read only" of file selected
//!
//! @return    FALSE, it is possible to modify the file selected
//! @return    TRUE, in other case
//!
Bool  nav_file_isreadonly( void )
{
   if( !fat_check_mount_select() )
      return TRUE;   // No file selected
   if( mem_wr_protect( fs_g_nav.u8_lun ) )
      return TRUE;   // Disk protected
   return (0!=(FS_ATTR_READ_ONLY & fs_g_nav_entry.u8_attr));
}
#endif

//! This fonction return the type of file selected
//!
//! @return    TRUE, it is a directory
//! @return    FALSE, in other case
//!
Bool  nav_file_isdir( void )
{
   return (( fat_g_cur_nav->attribute & FS_ATTR_LFN_ENTRY ) != FS_ATTR_LFN_ENTRY )&&( fat_g_cur_nav->attribute & FS_ATTR_DIRECTORY );
}


//! This fonction check the extension of file selected
//!
//! @param     sz_filterext   extension filter in ASCII format (e.g. "txt") (e.g. "txt,d*,wk" )
//!
//! @return    TRUE, the file extension corresponding at extension filter
//! @return    FALSE, in other case
//!
Bool  nav_file_checkext( FS_STRING sz_filterext )
{
   unsigned int _MEM_TYPE_SLOW_ dot_index;

   for( dot_index = 0; dot_index < SHORT_NAME_SIZE; ++dot_index )
   {
      if( fat_g_cur_nav->name[ dot_index ] == '.' )
         break;
   }
   if( dot_index == SHORT_NAME_SIZE )
      return FALSE;
   return str_comp_a(( U8 *)( &fat_g_cur_nav->name[ dot_index + 1 ]), ( U8 *)sz_filterext, FALSE ) == 0;
}

#if 0//( LCD_PRESENT == ENABLE )
//! This fonction return the date of file selected
//!
//! @param     type_date      FS_DATE_LAST_WRITE,  to get the date of last write access <br>
//!                           FS_DATE_CREATION,    to get the date of creation
//! @param     sz_date        ASCCI string buffer (>17B) to store the information about date <br>
//!                           "YYYYMMDDHHMMSSMS" = year, month, day, hour, minute, seconde, miliseconde
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_dateget( FS_STRING sz_date , Bool type_date )
{
   if ( !fat_check_mount_select())
      return FALSE;

   // read directory
   if ( !fat_read_dir())
      return FALSE;
   
   fat_get_date( sz_date , type_date );
   return TRUE;
}


//! This fonction return the attribut of file selected
//!
//! @return    attribut of file selected, see file system masks "FS_ATTR_" in fs_com.h file.
//!
U8    nav_file_attributget( void )
{
   return fs_g_nav_entry.u8_attr;
}
#endif


#if (FSFEATURE_WRITE_COMPLET == (FS_LEVEL_FEATURES & FSFEATURE_WRITE_COMPLET))
#if 0
//! This fonction change the date of file selected
//!
//! @param     type_date      FS_DATE_LAST_WRITE,  to get the date of last write access <br>
//!                           FS_DATE_CREATION,    to get the date of creation
//! @param     sz_date        ASCCI string buffer containt the informations about date to modify <br>
//!                           "YYYYMMDDHHMMSSMS" = year, month, day, hour, minute, seconde, miliseconde
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_dateset( const FS_STRING sz_date , Bool type_date )
{
   if ( !fat_check_mount_select())
      return TRUE;

   // read directory
   if ( !fat_read_dir())
      return FALSE;
      
   fat_set_date( sz_date , type_date );
   return fat_cache_flush();  // In case of error during writing data, flush the data before exit fonction
}
#endif
#endif  // FS_LEVEL_FEATURES


#if (FSFEATURE_WRITE_COMPLET == (FS_LEVEL_FEATURES & FSFEATURE_WRITE_COMPLET))
#if 0
//! This fonction change the attribut of file selected
//!
//! @param   u8_attribut   value to set at file selected, see file system masks "FS_ATTR_" in fs_com.h file.
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
Bool  nav_file_attributset( U8 u8_attribut )
{
   if ( !fat_check_mount_select())
      return FALSE;

   // read directory
   if ( !fat_read_dir())
      return FALSE;

   // save the new attribut with a filter attribut
   fs_g_nav_entry.u8_attr &= (~(FS_ATTR_READ_ONLY|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM|FS_ATTR_ARCHIVE));
   fs_g_nav_entry.u8_attr |= u8_attribut & (FS_ATTR_READ_ONLY|FS_ATTR_HIDDEN|FS_ATTR_SYSTEM|FS_ATTR_ARCHIVE);
   //fs_g_nav_entry.u8_attr = u8_attribut;
   fat_write_entry_file();
   return fat_cache_flush();  // In case of error during writing data, flush the data before exit fonction
}
#endif
#endif  // FS_LEVEL_FEATURES


#if (FSFEATURE_WRITE == (FS_LEVEL_FEATURES & FSFEATURE_WRITE))
//! This fonction delete the file or folder tree selected in navigator
//!
//! @param     b_only_empty      TRUE, del directory selected only if empty <br>
//!                              FALSE, del directorys and files include in directory selected <br>
//!                              If the file selected is not a directory, then this param is ignored.
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
#if 1
#pragma CODE = NAVIGATION_TEMP_CODE04

Bool  nav_file_del( Bool b_only_empty )
{
   extern char __X CurrentShortFilePath[ 256 ];
   char __X temp[ 32 ];
   //FS_ERR __X error;
   //FCFILE __X * __X file;

   strcpy(( char *)temp, ( char *)CurrentShortFilePath );
   strcat(( char *)temp, ( char *)"\\" );
   strcat(( char *)temp, ( char *)fat_g_cur_nav->name );
   return FSDelete( temp )== FS_NO_ERROR;
}
#endif
#endif  // FS_LEVEL_FEATURES


#if (FSFEATURE_WRITE_COMPLET == (FS_LEVEL_FEATURES & FSFEATURE_WRITE_COMPLET))

#if 0
//! This fonction rename the directory or file selected
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
Bool  nav_file_rename( const FS_STRING sz_name  )
{
   U16 u16_save_entry_pos;
   Bool b_save_entry_type;
   U8 u8_attr;
   U32 u32_cluster;
   U32 u32_size;

   if ( !fat_check_mount_select_noopen())
      return FALSE;

   if( !fat_check_nav_access_file( TRUE ) )
      return FALSE;

   // save information for create new name before delete the current name in case of error in creation
   u16_save_entry_pos = fs_g_nav_fast.u16_entry_pos_sel_file;
   b_save_entry_type  = fs_g_nav.b_mode_nav;
   // Get file attributs
   u8_attr = fs_g_nav_entry.u8_attr;
   u32_cluster = fs_g_nav_entry.u32_cluster;
   u32_size = fs_g_nav_entry.u32_size;
      
   // Create entry file
   if ( !nav_file_create( sz_name  ))
      return FALSE; // error
   // Modify current file
   if ( !fat_read_dir())
      return FALSE;
   
   // Set file attributs
   fs_g_nav_entry.u8_attr = u8_attr;
   fs_g_nav_entry.u32_cluster = u32_cluster;
   fs_g_nav_entry.u32_size = u32_size;
   fat_write_entry_file();
   
   // delete entry file name
   fs_g_nav_fast.u16_entry_pos_sel_file = u16_save_entry_pos; // go to previous entry name
   if ( !fat_delete_file(FALSE) )
      return FALSE; // error
   
   if ( !fat_cache_flush() )
      return FALSE; // error

   // Go to position of new entry (it is the last file or directory )
   return nav_filelist_last( b_save_entry_type );
}
#endif

#endif  // FS_LEVEL_FEATURES

#if (FSFEATURE_WRITE == (FS_LEVEL_FEATURES & FSFEATURE_WRITE))
#if 0
//! This fonction create a file with size 0 and file attribut 0
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! Use this routine to create a file and after call file_open() to open this new file
//! @endverbatim
//!
Bool  nav_file_create( const FS_STRING sz_name  )
{

   // Check if a setting file already exists
   if (!nav_filelist_reset())          // Go to neginning of file list
      return FALSE;
   if (nav_filelist_findname(sz_name , FALSE)) // Search name in file list (no case sensitive)
   {
      fs_g_status = FS_ERR_FILE_EXIST;
      return FALSE;  // File exist -> it is impossible to create this name
   }
   // FYC: here, the selection is at the end of the list
   // Create entry file
   if ( !fat_create_entry_file_name( sz_name ))
      return FALSE; // error
   // By default the informations about the new file is egal to 0 (a free entry in a cluster is filled of 0) 
   fs_g_nav_entry.u32_cluster = 0;     // No first cluster
   fs_g_nav_entry.u32_size    = 0;     // The size is null
   fs_g_nav_entry.u8_attr     = 0;     // Attribut is a file

   // It is the last FILE of the list
   fs_g_nav.u16_pos_sel_file++;
   fs_g_nav.b_mode_nav = FS_FILE;
   return fat_cache_flush();
}
#endif
#if 0
//! This fonction init the COPY navigator with the position of the current navigator
//!
//! @return  FALSE in case of error, see global value "fs_g_status" for more detail
//! @return  TRUE otherwise
//!
//! @verbatim
//! If you use the COPY navigator (see FS_NAV_ID_COPYFILE in conf_explorer.h) after this routine then the copy information is lost.
//! @endverbatim
//!
Bool  nav_file_copy( void )
{
   _MEM_TYPE_SLOW_ Fs_index index;
   U8 nav_id_save;
   Bool  status;

   if( nav_file_isdir() )
   {
      fs_g_status = FS_ERR_COPY_DIR; // Impossible to copy a directory
      return FALSE;
   }
   // Get reference of copy file 
   index = nav_getindex();

   // In "copy file" navigator select the copy file
   nav_id_save = nav_get();      
   nav_select( FS_NAV_ID_COPYFILE );
   status = nav_gotoindex( &index );
   nav_select( nav_id_save );

   return status;
}


//! This fonction paste the file selected in COPY navigator in the file list of the current navigator
//!
//! @param     sz_name     buffer with the name (ASCII or UNICODE )
//!
//! @return    FALSE in case of error, see global value "fs_g_status" for more detail
//! @return    TRUE otherwise
//!
//! @verbatim
//! After this routine, you shall call nav_file_paste_state() to run and way the copy
//! @endverbatim
//!
Bool  nav_file_paste_start( const FS_STRING sz_name  )
{
   U8 nav_id_save;
   Bool status;

   if( ID_STREAM_ERR != g_id_trans_memtomem )
   {
     
      fs_g_status = FS_ERR_COPY_RUNNING;  // A copy action is always running
      return FALSE;
   }

   // Create file to paste
   if( !nav_file_create( sz_name  ) )
      return FALSE;
   // Open file in write mode with size 0
   if( !file_open( FOPEN_MODE_W_PLUS ))
      return FALSE;

   // Open file to copy
   nav_id_save = nav_get();      
   nav_select( FS_NAV_ID_COPYFILE );
   status = file_open(FOPEN_MODE_R);
   nav_select( nav_id_save );

   // If error then close "paste file"
   if( !status )
   {
      file_close();
      return FALSE;
   }else{
      // Signal start copy
      g_id_trans_memtomem = ID_STREAM_ERR-1;
      return TRUE;
   }
}


//! This fonction execute the copy file
//!
//! @param     b_stop      set TRUE to stop copy action
//!
//! @return    copy status <br>
//!            COPY_BUSY,     copy running
//!            COPY_FAIL,     copy fail
//!            COPY_FINISH,   copy finish
//!
U8    nav_file_paste_state( Bool b_stop )
{
   _MEM_TYPE_SLOW_ Fs_file_segment segment_src;
   _MEM_TYPE_SLOW_ Fs_file_segment segment_dest;

   Ctrl_status status_stream;
   U8 status_copy;
   U8 nav_id_save;

   nav_id_save = nav_get();   // Get current navigator

   // Check, if copy is running
   if( ID_STREAM_ERR == g_id_trans_memtomem )
      return COPY_FAIL;

   if( b_stop )
   {
      status_copy = COPY_FAIL;
   }
   else
   {
      if( (ID_STREAM_ERR-1) != g_id_trans_memtomem )
      {
         // it isn't the beginning of copy action, then check current stream
         status_stream = stream_state( g_id_trans_memtomem );
         switch( status_stream )  
         {
         case CTRL_BUSY:
            status_copy = COPY_BUSY;
            break;
         case CTRL_GOOD:
            status_copy = COPY_FINISH;
            break;
         case CTRL_FAIL:
         default:
            status_copy = COPY_FAIL;
            break;
         }
      }else{
         status_copy = COPY_FINISH;
      }
   
      // Compute a new stream
      if( COPY_FINISH == status_copy )
      {
         stream_stop( g_id_trans_memtomem );

         // check eof source file
         nav_select( FS_NAV_ID_COPYFILE );
         if( 0 == file_eof() )
         {
            status_copy = COPY_BUSY;
            segment_src.u16_size = 0xFFFF; // Get the maximum segment supported by navigation (U16)
            if( !file_read( &segment_src ))
            {
               status_copy = COPY_FAIL;
            }
         }
         nav_select( nav_id_save );
   
         // check destination file
         if( COPY_BUSY == status_copy )
         {
            segment_dest.u16_size = segment_src.u16_size; // Ask the segment no more larger than source segment
            if( !file_write( &segment_dest ))
            {
               status_copy = COPY_FAIL;
            }
         }

         // Start new stream
         if( COPY_BUSY == status_copy )
         {
            // Compute a minimal segment
            if( segment_src.u16_size > segment_dest.u16_size )
            {
               // reposition source file
               nav_select( FS_NAV_ID_COPYFILE );
               if( !file_seek( (U32)(segment_src.u16_size - segment_dest.u16_size)*FS_512B , FS_SEEK_CUR_RE ))
               {
                  status_copy = COPY_FAIL;
               }
               nav_select( nav_id_save );
               segment_src.u16_size = segment_dest.u16_size; // Update source to start a correct transfer
            }
         }
         if( COPY_BUSY == status_copy )
         {
            g_id_trans_memtomem = stream_mem_to_mem( segment_src.u8_lun , segment_src.u32_addr , segment_dest.u8_lun , segment_dest.u32_addr , segment_src.u16_size );               
            if( ID_STREAM_ERR == g_id_trans_memtomem )
                  status_copy = COPY_FAIL;
         }
      }
   }

   // Check end of copy
   if( COPY_BUSY != status_copy )
   {
      U32 u32_size_exact;

      // Stop copy
      stream_stop( g_id_trans_memtomem );
      g_id_trans_memtomem = ID_STREAM_ERR;

      // Get exact size and close the source file
      nav_select( FS_NAV_ID_COPYFILE );
      u32_size_exact = nav_file_lgt();
      file_close();
      nav_select( nav_id_save );

      // If no error then set the exact size on the destination file
      if( COPY_FINISH == status_copy )
      {
         if( !file_seek( u32_size_exact , FS_SEEK_SET ))
         {
            status_copy = COPY_FAIL;
         }else{
            if( !file_set_eof() )
            {
               status_copy = COPY_FAIL;
            }
         }
      }
      file_close();
      // If error then delete the destination file
      if( COPY_FAIL == status_copy )
      {
         nav_file_del( TRUE );
      }
   }
   return status_copy;
}
#endif
#endif  // FS_LEVEL_FEATURES


#pragma CODE = NAVIGATION_TEMP_CODE05

void file_init (void )
{
   U16 iter;

   for( iter = 0; iter < sizeof( fat_g_files )/ sizeof( fat_g_files[ 0 ]); ++iter )
      fat_g_files[ iter ] = 0;
   fat_g_cur_file = &fat_g_files[ 0 ];
   fat_g_cur_nav = &fat_g_navs[ 0 ];
   fat_g_cur_entry = &fat_g_entry[ 0 ];
   //FSPatchInit(( __X FILE_INFO *)fat_g_cur_nav, ( __X FS_ENTRY_INFO *)fat_g_cur_entry );
}


#pragma CODE = NAVIGATION_TEMP_CODE03

FS_ERR FSNearValidEntry( FILE_INFO __X *finfo, unsigned int index )
{
   FS_ERR _MEM_TYPE_SLOW_ error;

   if( FSSeekEntry( finfo, ( __X FS_ENTRY_INFO *)fat_g_cur_entry, index )== FS_END_DIRECTORY )
      return FS_END_DIRECTORY;
   while( 1 )
   {
      if(( finfo->attribute & FS_ATTR_LFN_ENTRY ) != FS_ATTR_LFN_ENTRY
         &&( finfo->attribute & FS_ATTR_VOLUME ) != FS_ATTR_VOLUME )
         return FS_NO_ERROR;
      error = FSNextEntry( finfo, ( __X FS_ENTRY_INFO *)fat_g_cur_entry );
      if( error == FS_END_DIRECTORY )
         return error;
      if( error == FS_NO_ERROR )
         return error;
   }
   return FS_SYSTEM_ERROR;
}


#pragma CODE = NAVIGATION_CODE

FS_ERR FSNextValidEntry( FILE_INFO __X *finfo )
{
   FS_ERR _MEM_TYPE_SLOW_ error;
   while( 1 )
	{
      error = FSNextEntry( finfo, ( __X FS_ENTRY_INFO *)fat_g_cur_entry );
      /*SendString((U8 *)"error=");
      send_hex_u8(error);
      SendString((U8 *)"\r\n");*/
      if( error == FS_END_DIRECTORY )
         return	error;
      if( error == FS_FORMAT_ERROR )
         return	error;
      if( error == FS_NO_ERROR )
         return	error;
   }
   return FS_SYSTEM_ERROR;
}


FS_ERR FSPrevValidEntry( FILE_INFO __X *finfo )
{
   FS_ERR _MEM_TYPE_SLOW_ error;
   while( 1 )
   {
      error = FSPrevEntry( finfo, ( __X FS_ENTRY_INFO *)fat_g_cur_entry );
      if( error == FS_END_DIRECTORY )
         return error;
      if( error == FS_NO_ERROR )
         return error;
   }
   return FS_SYSTEM_ERROR;
}


#pragma CODE = FILE_CODE

int str_comp_u( U16 *sl, U16 *sr, U8 use_case )
{
   if( use_case )
   {
      while( *sl && *sl == *sr )
      {
         ++sl;
         ++sr;
      }
      return *sl - *sr;
   }
   else
   {
      while( *sl && tolower( *sl )== tolower( *sr ))
      {
         ++sl;
         ++sr;
      }
      return tolower( *sl )- tolower( *sr );
   }
}

#if 0
U8 *str_copy( U8 *dst, U8 *src )
{
   U8 *old_ptr = dst;

   while( *dst++ = *src++ );
   return old_ptr;
}


U16 *str_copy_u( U16 *dst, U16 *src )
{
   U16 *old_ptr = dst;

   while( *dst++ = *src++ );
   return old_ptr;
}
#endif


//#pragma CODE = NAVIGATION_CODE

U16 *str_copy_u_n( U16 *dst, U16 *src, U16 num )
{
   //U16 *old_ptr = dst;

   while( num )
   {
      *dst = *src;
      if( *src == 0 )
         break;
      --num;
      dst++; src++;
   }
   if( !num )
      *--dst = 0;
   //return old_ptr;
   return dst;
}


#pragma CODE = NAVIGATION_TEMP_CODE03

//unicode×Ö·û´®×ª»¯Îª±¾µØ×Ö·û´®(ascii×Ö·û´®)
void unicode_to_local( U16 *unicode_str, U8 *local_str, U16 buffer_size )
{
   while( *unicode_str )
   {
      if( *unicode_str < 256 )
      {
         *local_str++ = *unicode_str++;
         --buffer_size;
         if( buffer_size == 1 )
         {
            *local_str = 0;
            return;
         }
      }
      else
      {
         *local_str++ = *unicode_str;
         --buffer_size;
         if( buffer_size == 1 )
         {
            *local_str = 0;
            return;
         }
         *local_str++ = (( *unicode_str )& 0xFF )>> 8;
         --buffer_size;
         if( buffer_size == 1 )
         {
            *local_str = 0;
            return;
         }
         ++unicode_str;
      }
   }
   *local_str = 0;
}


#pragma CODE = FILE_CODE

//±¾µØ×Ö·û´®(ascii×Ö·û´®)×ª»¯Îªunicode×Ö·û´®
void local_to_unicode( U8 *local_str, U16 *unicode_str, U16 buffer_size )
{
   while( *local_str )
   {
      *unicode_str++ = *local_str++;
      buffer_size -= 2;
      if( buffer_size <= 3 )
      {
         //*unicode_str = 0;
         //return;
         break;
      }
   }
   *unicode_str = 0;
}


#pragma CODE = NAVIGATION_CODE

int str_comp_a( U8 *sl, U8 *sr, U8 use_case )
{
   if( use_case )
   {
      while( *sl && *sl == *sr )
      {
         ++sl;
         ++sr;
      }
      return *sl - *sr;
   }
   else
   {
      while( *sl && tolower( *sl )== tolower( *sr ))
      {
         ++sl;
         ++sr;
      }
      return tolower( *sl )- tolower( *sr );
   }
}


U8 *str_copy_n( U8 *dst, U8 *src, U16 num )
{
   U8 *old_ptr = dst;

   while( num )
   {
      *dst++ = *src++;
      if( *src == 0 )
         break;
      --num;
   }
   if( num )
      *dst = 0;
   return old_ptr;
}

#endif 	// _navigation_c_
