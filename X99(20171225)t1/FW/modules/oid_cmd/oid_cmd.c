//! @file oid_cmd.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the oid cmd management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: oid_cmd.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _oid_cmd_c_
#define _oid_cmd_c_


#include "config.h"
#include "oid_cmd.h"

#include "modules/file_aux/file_aux.h"
#include "modules/file_system/navigation.h"
#include "modules\cue\cue.h"

//#include "modules/file_system/fat.h"

//#include "lib_system/utilities/string.h"

#include "modules/apf/apf.h"
#include "conf\conf_sio.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "modules\paster\paster_base.h"
#include "sio\com.h"
#include "mmi\ap3\mmi_ap3.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"


#pragma DATA = OID_CMD_DATA
#pragma CODE = OID_CMD_CODE

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( FS_CREATE_BUF_CODE );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( OID_CMD_TEMP_CODE00 );
ImageDeclareType( OID_CMD_TEMP_CODE01 );
ImageDeclareType( OID_CMD_TEMP_CODE02 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );


#if( PASTER == ENABLE )

/*
	PASTER_OID_CMD_RECORD,	//3994
	PASTER_OID_CMD_PLAY,		//3995
	PASTER_OID_CMD_PAUSE,		//3996
	PASTER_OID_CMD_STOP,		//3997
	PASTER_OID_CMD_COPY,		//3998
	PASTER_OID_CMD_BACK,		//3999
*/
U16   _MEM_TYPE_SLOW_   g_oid_cmd[ OID_CMD_NUM ];

struct   OID_CMD_ITEM   _MEM_TYPE_SLOW_   g_oid_cmd_item;

_MEM_TYPE_SLOW_   U16   Current_apf_num /*= 0*/;
_MEM_TYPE_SLOW_   U16   Total_apf_num /*= 0*/;
//                  Bool  b_apf_file_change = TRUE;

_MEM_TYPE_SLOW_   APF_CHANGE_FILE   apf_change_file /*= MMI_PASTER_NO_CHANGE_FILE*/;


U16   Get_Apf_File_Number     ( U8 *sz_filterext );
Bool  open_apf_file           ( void );
Bool  read_apf_info           ( void );

#pragma CODE = PASTER_TEMP_CODE00

Bool oid_cmd_init( void )
{
   SDKLoadCode( NAVIGATION_TEMP_CODE00 );
   if( !nav_dir_root())
      return FALSE;
   //paster_back_to_top_dir();
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   nav_string_ascii();
#endif
   SDKLoadCode( NAVIGATION_TEMP_CODE03 );
   if( nav_filelist_findname(( U8 *)PASTER_DIR, FALSE ))
   {
      //SendString((U8 *)"find\r\n");
      goto oid_cmd_init_enter_dir;
   }
   SDKLoadCode( NAVIGATION_TEMP_CODE02 );
   if( !nav_dir_make(( U8 *)PASTER_DIR ))
   {
      if( FS_ERR_FILE_EXIST != fs_g_status )
      {
         SDKLoadCode( NAVIGATION_TEMP_CODE04 );
         ( void )nav_file_del( TRUE );
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
         nav_string_unicode();
#endif
         return FALSE;
      }
   }
oid_cmd_init_enter_dir:
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
   nav_string_unicode();
#endif
   //SendString((U8 *)"cd\r\n");
   SDKLoadCode( NAVIGATION_TEMP_CODE01 );
   if( !nav_dir_cd())
   {
      return FALSE;
   }

   SDKLoadCode( NAVIGATION_CODE );
   SDKLoadCode( OID_CMD_TEMP_CODE00 );
   Total_apf_num = Get_Apf_File_Number(( U8 *)APF_DEFAULT_FILE_NAME );
   if( !Total_apf_num )
   {
      return FALSE;
   }
   /*if( b_apf_file_change )
   {
      b_apf_file_change = FALSE;
      Current_apf_num = 0;
   }*/
find_next_apf:
   SDKLoadCode( NAVIGATION_CODE );
   SDKLoadCode( OID_CMD_TEMP_CODE01 );
   if( !open_apf_file())
   {
      if( apf_change_file == MMI_PASTER_NO_CHANGE_FILE )
      {
         Current_apf_num++;
         if( Current_apf_num < Total_apf_num )
            goto find_next_apf;
      }
      else if( apf_change_file == MMI_PASTER_KEY_NEXT_CHANGE_FILE )
      {
         Current_apf_num++;
         if( Current_apf_num >= Total_apf_num )
         {
            apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
            Current_apf_num = 0;
         }
         goto find_next_apf;
      }
      else if( apf_change_file == MMI_PASTER_KEY_PREV_CHANGE_FILE )
      {
         if( !Current_apf_num )
         {
            Current_apf_num = Total_apf_num - 1;
         }
         else
            Current_apf_num--;
         goto find_next_apf;
      }
      return FALSE;
   }

   SDKLoadCode( FS_CREATE_BUF_CODE );
   ( void )FSCreateBuf();

   SDKLoadCode( OID_CMD_TEMP_CODE02 );
   if( !read_apf_info())
      goto find_next_apf;

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 0, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );

   return TRUE;

#if 0
   //nav_filelist_reset();
#if 1
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( APF_DEFAULT_FILE_NAME )
            && nav_file_name( Unicode_get_addr_string( g_string2 ), Unicode_get_char_space( g_string2 ), FS_NAME_GET, FALSE ))
         {
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 0, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string2 );
            result = TRUE;
            break;
         }
      }
   }
   //if( !nav_filelist_findname( string_unicode_ptr( OID_CMD_DEFAULT_FILE_NAME ), FALSE ))
      //return FALSE;
   if( !result || !file_open( FOPEN_MODE_R ))
#else
   nav_string_unicode();
   if( !nav_filelist_findname( Unicode_get_addr_string( g_string1 ), FALSE ))
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 0, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string1 );
   if( !file_open( FOPEN_MODE_R ))
#endif
   {
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !file_seek( APF_OFFSET_OID_CMD_NUM, FS_SEEK_SET ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&num, sizeof( num ))!= sizeof( num ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( !file_seek( APF_OFFSET_OID_CMD_TABLE, FS_SEEK_SET ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   for( iter = 0; iter < OID_CMD_NUM; ++iter )
   {
      if( iter >= num )
      {
         g_oid_cmd[ iter ] = 0;
      }
      else
      {
         if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_oid_cmd_item, sizeof( g_oid_cmd_item ))!= sizeof( g_oid_cmd_item ))
         {
            file_close();
            nav_select( nav_id_saved );
            return FALSE;
         }
         g_oid_cmd[ iter ] = g_oid_cmd_item.oid_index;
      }
   }
//***********************************************
   if( !file_seek( APF_OFFSET_INDEX_START, FS_SEEK_SET ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_start, sizeof( g_u16_oid_start ))!= sizeof( g_u16_oid_start ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_division, sizeof( g_u16_oid_division ))!= sizeof( g_u16_oid_division ))
   {
      file_close();
      nav_select( nav_id_saved );
      return FALSE;
   }
//***********************************************
   file_close();
   nav_select( nav_id_saved );
   return TRUE;
#endif
}


#pragma CODE = OID_CMD_CODE

U8 oid_cmd_trans( U16 oid_value )
{
   U8 iter;
   if( !oid_value )
      return OID_CMD_NUM;
   for( iter = 0; iter < OID_CMD_NUM; ++iter )
   {
      if( oid_value == g_oid_cmd[ iter ])
         return iter;
   }
   return OID_CMD_NUM;
}


#pragma CODE = OID_CMD_TEMP_CODE00

//the function is find the number of apf in the root;
U16 Get_Apf_File_Number( U8 *sz_filterext )
{
   U16 _MEM_TYPE_SLOW_ Number = 0;
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext ))
         {
            Number++;
         }
      }
   }
   return Number;
}


#pragma CODE = OID_CMD_TEMP_CODE01

//the funcion is open the first file of changed file;
Bool open_apf_file( void )
{
   U16 _MEM_TYPE_SLOW_ Number = 0;
   //U16 _MEM_TYPE_SLOW_ iter;
   //U16 _MEM_TYPE_SLOW_ num;
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext(( U8 *)APF_DEFAULT_FILE_NAME )
            && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE ))
         {
            if( Number == Current_apf_num )
            {
               SendString((U8 *)"open\r\n");
               SDKLoadCode( FILE_TEMP_CODE00 );
               if( !file_open( FOPEN_MODE_R ))
                  return FALSE;

               return TRUE;
            }
            Number++;
         }
      }
   }
   return FALSE;
}


#pragma CODE = OID_CMD_TEMP_CODE02

Bool read_apf_info( void )
{
   U16 _MEM_TYPE_SLOW_ iter;
   U16 _MEM_TYPE_SLOW_ num;
   
   //SendString((U8 *)"seek\r\n");
   if( !file_seek( APF_OFFSET_OID_CMD_NUM, FS_SEEK_SET ))
   {
open_apf_error:
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&num, sizeof( num ))!= sizeof( num ))
   {
      //file_close();
      //return FALSE;
      goto open_apf_error;
   }
   if( !file_seek( APF_OFFSET_OID_CMD_TABLE, FS_SEEK_SET ))
   {
      //file_close();
      //return FALSE;
      goto open_apf_error;
   }
   for( iter = 0; iter < OID_CMD_NUM; ++iter )
   {
      if( iter >= num )
      {
         g_oid_cmd[ iter ] = 0;
      }
      else
      {
         if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_oid_cmd_item, sizeof( g_oid_cmd_item ))!= sizeof( g_oid_cmd_item ))
         {
            //file_close();
            //return FALSE;
            goto open_apf_error;
         }
         g_oid_cmd[ iter ] = g_oid_cmd_item.oid_index;
      }
   }
   if( !file_seek( APF_OFFSET_INDEX_START, FS_SEEK_SET ))
   {
      //file_close();
      //return FALSE;
      goto open_apf_error;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_start, sizeof( g_u16_oid_start ))!= sizeof( g_u16_oid_start ))
   {
      //file_close();
      //return FALSE;
      goto open_apf_error;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_u16_oid_division, sizeof( g_u16_oid_division ))!= sizeof( g_u16_oid_division ))
   {
      //file_close();
      //return FALSE;
      goto open_apf_error;
   }
   //file_close();
   return TRUE;
}


#endif  //#if( PASTER == ENABLE )

#endif  //_oid_cmd_c_
