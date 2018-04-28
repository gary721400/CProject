//!
//! @file cue.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the cue.
//!
//! @version 1.20 snd3b-dvk-1_0_33 $Id: cue.c,v 1.20 2006/12/21 16:02:30 coger Exp $
//!
//! @todo
//! @bug

#ifndef _cue_c_
#define	_cue_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "cue.h"
#include "modules\file_aux\file_aux.h"
#include "modules\file_system\navigation.h"
//#include "modules\file_system\fat.h"
#include "modules\player_ap3\player_ap3.h"
//#include "lib_system\utilities\string.h"

#include "conf\conf_sio.h"
#include "mmi\shared\com_var.h"
#include "modules/apf/apf.h"


//_____ M A C R O S ________________________________________________________
#if( PASTER == ENABLE )

#if 0
U8    _MEM_TYPE_SLOW_   g_cue_dir[] = "CUE";

#define  CUE_FILE_NAME_SIZE_ASCII        STRING_FILE_NAME_SIZE_ASCII
#define  CUE_FILE_NAME_SIZE_UNICODE      STRING_FILE_NAME_SIZE_UNICODE

#define  cue_buffer_unicode()            g_buffer_unicode
#define  cue_buffer_ascii()              g_buffer_ascii
#endif

U16   _MEM_TYPE_SLOW_   g_u16_oid_start;
U16   _MEM_TYPE_SLOW_   g_u16_oid_division;

struct APF_CUE_INFO   _MEM_TYPE_SLOW_   g_cue_info;

#if 0
Bool g_b_cue_file_is_load = FALSE;
Bool g_b_cue_file_is_open = FALSE;
U16 _MEM_TYPE_SLOW_ g_u16_cue_division = 0;

enum
{
  CUE_INDEX_RECORD = 0,
  CUE_INDEX_PLAY,
  CUE_INDEX_PAUSE,
  CUE_INDEX_STOP,
  CUE_INDEX_COPY,
  CUE_INDEX_INVALID,
	
  CUE_INDEX_NUM
};


U8 *cue_index_to_file_name( U16 index, U8 *str )
{
  U16 num = CUE_MAX_NUM;
  while(( num > index )&&( num > 1 ))
  {
    num /= 10;
  }
  while( num )
  {
    *str++ = index / num + '0';
    index %= num;
    num /= 10;
  }
  *str++ = '.';
  *str++ = 'M';
  *str++ = 'P';
  *str++ = '3';
  *str = '\0';
  return str;
}


//U16 *cue_index_to_unicode( U16 index )
U8 *cue_index_to_unicode( U16 index )
{
  cue_index_to_file_name( index, cue_buffer_ascii());
  return string_unicode_ptr( cue_buffer_ascii());
}


Bool cue_file_open()
{
  U8 nav_id_saved;
  if( g_b_cue_file_is_open )
  {
    return TRUE;
  }
  nav_id_saved = nav_get();
  nav_select( FS_NAV_ID_CUE );
  {
    if( !nav_dir_root())
    {
      return FALSE;
    }
    if( !nav_filelist_reset())
    {
      return FALSE;
    }
    string_ascii_to_unicode( g_file_name_buffer, g_cue_file_name );
    if( !nav_filelist_findname( string_unicode( g_file_name_buffer )))
    {
      return FALSE;
    }
    if( !file_open( FOPEN_MODE_R ))
    {
      return FALSE;
    }
  }	
  nav_select( nav_id_saved );
  g_b_cue_file_is_open = TRUE;
  return TRUE;
}

Bool cue_file_load()
{
  U8 iter;
  U8 nav_id_saved;
  U32 pos;
  if( !g_b_cue_file_is_open )
  {
    return g_b_cue_file_is_load = FALSE;
  }
  nav_id_saved = nav_get();
  nav_select( FS_NAV_ID_CUE );
  {
    pos = fs_g_nav_entry.u32_pos_in_file;
    file_seek( 0, FS_SEEK_SET );
		
    file_read_u16( &g_u16_cue_division );
    for( iter = 0; iter < 6; ++iter )
    {
      file_read_u16( &g_cue_item[ iter ].cue_code );
      file_read_u32( &g_cue_item[ iter ].offset );
    }
    g_b_cue_file_is_load = TRUE;

    file_seek( pos, FS_SEEK_SET );
  }
  nav_select( nav_id_saved );
  return TRUE;
}

void cue_file_close()
{
  U8 nav_id_saved;
  if( !g_b_cue_file_is_open )
  {
    return;
  }
  nav_id_saved = nav_get();
  nav_select( FS_NAV_ID_CUE );
  file_close();
  nav_select( nav_id_saved );
  g_b_cue_file_is_open = FALSE;
}


U32 cue_get_offset( U8 index )
{
  if( index >= CUE_INDEX_NUM )
  {
    index = CUE_INDEX_INVALID;
  }
  return g_cue_item[ index ].offset;
}


Bool cue_init( void )
{
   Bool result = FALSE;
   U8 nav_id_saved;
   nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_CUE );
   nav_drive_set( LUN_ID_NF_DISKMASS );
   nav_dir_root();
   paster_back_to_top_dir();
   //nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( nav_file_checkext( APF_DEFAULT_FILE_NAME ))
      {
         if( !( fs_g_nav_entry.u8_attr & FS_ATTR_DIRECTORY ))
         {
            result = TRUE;
            break;
         }
      }
   }
   if( !result )
      goto __error;
   //if( !nav_filelist_findname( string_unicode_ptr( APF_DEFAULT_FILE_NAME ), FALSE ))
      //goto	__error;
   if( !file_open( FOPEN_MODE_R ))
      goto	__error;
   file_seek( APF_OFFSET_INDEX_START, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_start, sizeof( g_u16_oid_start ));
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_division,	sizeof( g_u16_oid_division ));
   file_close();
   nav_select( nav_id_saved );
   return TRUE;
__error:
   //nav_drive_set( LUN_ID_NF_DISKMASS );
   //nav_dir_root();
   nav_select( nav_id_saved );
   return FALSE;
}
#endif

/*
U16 cue_get_oid_index_start( void )
{
  return g_u16_oid_start;
}


U16 cue_get_oid_index_division( void )
{
  return g_u16_oid_division;
}
*/

#if 0
Bool cue_play( U8 index )
{
   U8 nav_id_saved;
   Bool result = FALSE;
   //if( cue_is_busy())
   if( player_ap3_is_busy())
   {
      return FALSE;
   }
   nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_CUE );
   //file_close();
   nav_drive_set( LUN_ID_NF_DISKMASS );
   nav_dir_root();
   paster_back_to_top_dir();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( nav_file_checkext( APF_DEFAULT_FILE_NAME ))
      {
         if( !( fs_g_nav_entry.u8_attr & FS_ATTR_DIRECTORY ))
         {
            result = TRUE;
            break;
         }
      }
   }
   //if( !nav_filelist_findname( string_unicode_ptr( g_cue_dir )))
   //if( !nav_filelist_findname( string_unicode_ptr( APF_DEFAULT_FILE_NAME ), FALSE ))
   //{
         //return FALSE;
   //}
   if( !result || !file_open( FOPEN_MODE_R ))
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   file_seek( APF_OFFSET_CUE_INDEX_TABLE + index * sizeof( g_cue_info ), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_cue_info, sizeof( g_cue_info ));
   if(( g_cue_info.offset == 0 )||( g_cue_info.size == 0 ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !player_apf_start( &g_cue_info, CODEC_MP3 ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   nav_select( nav_id_saved );
   return TRUE;
}


void cue_off( void )
{
   U8 nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_CUE );
   file_close();
   nav_select( nav_id_saved );
}


Bool cue_is_busy()
{
   return player_ap3_is_busy();
}
#endif

#endif  //#if( PASTER == ENABLE )

#endif  //_cue_c_
