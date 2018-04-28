//!
//! @file paster_base.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster base.
//!
//! @version 1.20 snd3b-dvk-1_0_33 $Id: paster_base.c,v 1.20 2006/12/21 16:02:30 coger Exp $
//!
//! @todo
//! @bug
#ifndef _paster_base_c_
#define _paster_base_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include <stdio.h>

#include "paster_base.h"
//#include "lib_system\utilities\string.h"
#include "lib_system\unicode\unicode.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "modules\audio\audio.h"

#include "modules\player_ap3\player_ap3.h"
#include "modules\recorder\recorder.h"
#include "mmi\recorder\srv_recorder.h"


#include "conf\conf_sio.h"
#include "mmi\shared\com_var.h"
#include "modules/apf/apf.h"
#include "modules/file_aux/file_aux.h"
#include "mmi\player\srv_player.h"
#include "modules\recorder\recorder_task.h"
#include "mmi\paster\mmi_paster.h"
#include "modules\power\power.h"             // Power module definition
#include "conf\conf_audio.h"                 // Audio user's configuration
#include "modules\player\player_task.h"
#include "sio\com.h"
#include "modules\player\player.h"
#include "mmi\shared\com_appli.h"
#include "modules\search\search.h"
#include <string.h>
#include "mmi\status\mmi_status.h"
#include "paster.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "conf\conf_mmi.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fc_internal.h"

#pragma DATA = PASTER_BASE_DATA
#pragma CODE = PASTER_BASE_CODE

ImageDeclareType( RECORDER_TEMP_CODE00 );
ImageDeclareType( RECORDER_TEMP_CODE01 );
ImageDeclareType( RECORDER_TEMP_CODE02 );
ImageDeclareType( RECORDER_TEMP_CODE03 );
ImageDeclareType( RECORDER_TEMP_CODE04 );

ImageDeclareType( PASTER_BASE_TEMP_CODE00 );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );

ImageDeclareType( KB_RESIZE_CODE );


//_____ M A C R O S ________________________________________________________
#if( PASTER == ENABLE )
#define  PASTERDIR                 "0:\\REC"

#define  PASTER_INDEX_MASK          (( U16 )0xF000 )

//_____ P R I V A T E    D E C L A R A T I O N S ___________________________

U8       _MEM_TYPE_SLOW_ g_paster_play_file_type;

//extern                     Bool  g_b_search_record;
//extern   xdata    AL_Variables     	varAl;
//extern   xdata    SSSRECCODECREG    RecCodecReg;

extern  Bool                  g_b_play_ap3;

extern __X   FCFILE   *ADPCM_File;

//const    U8    g_paster_dir[] = "REC";

#define  PASTER_PATH_LENGTH      64
char  /*_MEM_TYPE_SLOW_*/  paster_path[ PASTER_PATH_LENGTH ];
char  /*_MEM_TYPE_SLOW_*/  paster_file[ 13 ];
U16 *paster_point;

//_____ D E F I N I T I O N S ______________________________________________
char  *convert_U16   ( char *p, U16 i );


#pragma CODE = PASTER_BASE_TEMP_CODE00

char *convert_U16( char *p, U16 i )
{
   if( i / 10 )
      p = convert_U16( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#if 0
U8 *paster_index_to_string( U16 index, U8 *str )
{
   //sprintf( str, "Rec%05u", index );
   return ( str + 8 );
}

U8 *paster_index_to_filename( U16 index, U8 * filename, U8 codec )
{
   filename = paster_index_to_string( index, filename );
   *filename++ = '.';
   switch( codec )
   {
      case CODEC_WAV:
      *filename++ = 'w';
      *filename++ = 'a';
      *filename++ = 'v';
      break;

      case CODEC_MP3:
      *filename++ = 'm';
      *filename++ = 'p';
      *filename++ = '3';
      break;

      default:
      *filename++ = 'w';
      *filename++ = 'a';
      *filename++ = 'v';
      break;
   }
   *filename = '\0';
   return filename;
}


//U16 *paster_index_to_unicode( U16 index, Bool is_file )
U8 *paster_index_to_unicode( U16 index, Bool is_file, U8 codec )
{
   if( is_file )
   {
      paster_index_to_filename( index, paster_ascii_buffer(), codec );
   }
   else
   {
      paster_index_to_dirname( index, paster_ascii_buffer());
   }
   //string_ascii_to_unicode( paster_unicode_buffer(), paster_ascii_buffer());
   return string_unicode( paster_unicode_buffer());
}

#if 0
Bool paster_back_to_top_dir( void )
{
   if( g_paster_dir_index.u8_lun == 0xFF )
   {
      if( !nav_dir_root())
      {
         return FALSE;
      }
      string_ascii_to_unicode( paster_unicode_buffer(), g_paster_dir );
      //if( !nav_filelist_findname( string_unicode( paster_unicode_buffer())))
      if( !nav_filelist_findname( string_unicode( paster_unicode_buffer()), FALSE ))
      {
         if( !nav_dir_make( string_unicode( paster_unicode_buffer())))
         {
            return FALSE;
         }
      }
      if( !nav_dir_cd())
      {
         return FALSE;
      }
      g_paster_dir_index = nav_getindex();
   }
   else
   {
      if( !nav_gotoindex( &g_paster_dir_index ))
      {
         return  FALSE;
      }
   }
   return TRUE;
}
#endif
#endif


#pragma CODE = PASTER_BASE_TEMP_CODE00

Bool paster_index_file_find( U16 index, Bool create_path )
{
   U16 _MEM_TYPE_SLOW_ mask;
   U16 _MEM_TYPE_SLOW_ current_dir_mask;
   //U16 *_MEM_TYPE_SLOW_ file_name;
   U8 file_name[ 9+6 ], i;
   FS_ERR error;
   U16 *p;

   //nav_filelist_reset();
   //strcpy( paster_path, PASTERDIR );
   strcpy( tisiFileName, "0:\\Rec\\" );
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath ))
      return FALSE;
   strcpy( paster_path, tisiFileShortpath );
   paster_point = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\Rec\\"), 7+1 );
   
   current_dir_mask = PASTER_INDEX_MASK;
   mask = PASTER_INDEX_MASK >> 4;
   while( mask )
   {
      //file_name = paster_index_to_unicode( index & current_dir_mask, PASTER_INDEX_TO_DIR, 0 );
      //strcat( paster_path, "REC");
      memset( file_name, 0, 9+6 );
      strcpy(( char *)file_name, "REC" );
      convert_U16(( char *)&file_name[ 9 ], index & current_dir_mask );
      i = strlen(( char *)&file_name[ 9 ]);
      for( ; i < 5; i++ )
         strcat(( char *)file_name, "0" );
      strcat(( char *)file_name, ( char *)&file_name[ 9 ]);
      //strcat( paster_path, "\\" );

      p = str_copy_u_n( paster_point, atou( file_name ), 8+1 );
      paster_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
      if( !find_path_u(( U16 *)tisiFileName, ( U8 *)tisiFileShortpath ))
      {
         if( create_path )
         {
            //if( !nav_dir_make( file_name ))
            //error = FSDirectory(( char *)paster_path );
            error = FSDirForUnicode( paster_path, ( char *)file_name, ( U8 *)atou( file_name ));
            if( error == FS_DIRECTORY_FULL )
            {
               g_b_disk_is_full = TRUE;
               return FALSE;
            }
            if(( error != FS_DIRECTORY_CONFLICT )
               &&( error != FS_NO_ERROR ))
            {
               return FALSE;
            }
            //paster_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
            ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
         }
         else
            return FALSE;
      }

      strcpy( paster_path, tisiFileShortpath );
      current_dir_mask |= mask;
      mask >>= 4;
   }

   //file_name = paster_index_to_unicode( index, PASTER_INDEX_TO_FILE, CODEC_WAV );
   strcpy( paster_file, "Rec" );
   memset( file_name, 0, 6 );
   convert_U16(( char *)file_name, index & current_dir_mask );
   i = strlen(( char *)file_name );
   for( ; i < 5; i++ )
      strcat(( char *)paster_file, "0" );
   strcat(( char *)paster_file, ( char *)file_name );
   strcat( paster_file, ".wav" );
   
   return TRUE;
}


#pragma CODE = PASTER_BASE_TEMP_CODE01

Bool paster_record_find( U16 index )
{
   __X FS_ERR err;

#if 0
   free_space = FSFreeSpace( 0, &err );
   free_space *= 512ul;
   /*SendString((U8 *)"free_space=");
   send_hex_u32(free_space);
   SendString((U8 *)"\r\n");*/
   if( err )
      return FALSE;
#endif

   SDKLoadCode( PASTER_BASE_TEMP_CODE00 );
   if( !paster_index_file_find( index, TRUE ))
   {
      return FALSE;
   }

   return TRUE;
}


#pragma CODE = PASTER_BASE_TEMP_CODE02

Bool paster_record_start( void )
{
   __X FS_ERR err;
   __X U32 file_size;
   U16 mode;
   U8 i;

   record_result = RECORD_ERROR;
   
   mode = FS_MODE_READ_WRITE;
   str_copy_u_n( paster_point, atou(( U8 *)paster_file ), 12+1 );
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
   {
      mode = FS_MODE_CREATE;
      file_size = 0;

      *paster_point = 0;
      ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
      strcpy( paster_path, tisiFileShortpath );
   }
#if 0
   else
      file_size = FSGetFileSize( tisiFileShortpath, &err );

   free_space += file_size;
   if( free_space <= MIN_DISK_SPACE ) 
   {
      record_result = RECORD_MEMORY_FULL;
      return FALSE;
   }
   if( free_space >= MAX_LENGTH )
   {
      file_size = MAX_LENGTH;
   }
   else
   {
      file_size = free_space;
   }
#else
   file_size = MAX_LENGTH;
#endif

   if( mode == FS_MODE_READ_WRITE )
   {
      ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
   }
   else
   {
      ADPCM_File = ( __X FCFILE *)FSOpenForUnicode2(( char *)paster_path, ( char *)paster_file, ( U8 *)atou(( U8 *)paster_file), FS_MODE_CREATE, &err );
      
      SDKLoadCode( KB_RESIZE_CODE );
      kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
      FSClose(( __X FCFILE *)ADPCM_File );

      strcpy( tisiFileShortpath, paster_path );
      strcat( tisiFileShortpath, "\\" );
      strcat( tisiFileShortpath, paster_file );
      ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
   }
   if( err )
   {
      FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
   }

#if 0
   FSResize(( __X FCFILE *)ADPCM_File, file_size );
#else
   SDKLoadCode( KB_RESIZE_CODE );
   err = kb_fat_resize(( __X FCFILE *)ADPCM_File, file_size );
   if( err != FS_NO_ERROR )
   {
      kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
      FSClose(( __X FCFILE *)ADPCM_File );
      if( err == FS_WRITE_OUT_RANGE )
         record_result = RECORD_MEMORY_FULL;
      return FALSE;
   }
#endif
   WavStepSize = file_size;
   //WavWriteSize = 0;

   SDKLoadCode( RECORDER_TEMP_CODE00 );
   SendString((U8 *)"record on\r\n");
   if( !record_on())
   {
      //FSResize(( __X FCFILE *)ADPCM_File, 0 );
      kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
      FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
   }
   SDKLoadCode( RECORDER_TEMP_CODE01 );
   if( !record_start())
   {
      //FSResize(( __X FCFILE *)ADPCM_File, 0 );
      kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
      FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
   }
   //recorder_task_start();  // Enable event of recorder task
   power_forbid_autopoweroff();
   return TRUE;
}


#if 0
//U32	_MEM_TYPE_SLOW_	u32_data_remaining;
void _paster_stream_stop( void )
{
   extern U8 _MEM_TYPE_SLOW_ recorder_id_stream;
   if( ID_STREAM_ERR != recorder_id_stream )
   {
      WAV_RecPrevInit();
      varAl.Run_status = SysStopMusic;
      /*RecCodecReg.Ctl |= 0x02;
      SendString("Pause Recording:");
      send_hex_u8(RecCodecReg.Ctl);
      SendString("\r\n");
      varAl.Run_status = SysPauseRecord;*/
      recorder_id_stream = ID_STREAM_ERR; // reinit value with no stream
   }
}


/*void paster_record_pause( void )
{
  record_pause();
}

void paster_record_goon( void )
{
  record_goon();
}*/
#endif


//#pragma CODE = PASTER_BASE_CODE
#pragma CODE = PASTER_RECORD_CODE

void paster_record_stop( void )
{
   srvrec_stop();
   //_paster_stream_stop();
   //recorder_task_stop();
   //power_authorize_autopoweroff();
   Ts_free( s_paster_display_id );
}

#if 0
void paster_record_file_end( void )
{
   extern  _MEM_TYPE_SLOW_ U8 recorder_id_nav;
   U32 u32_data_remaining;
   U8 nav_id_save;

   nav_id_save = nav_get();
   nav_select( recorder_id_nav );
   //if( file_seek( u32_data_remaining * FS_512B, FS_SEEK_CUR_RE ))
   if( FileData.TotalWriteDataSz < CUT_WAV_END_LEN )
      FileData.TotalWriteDataSz = CUT_WAV_END_LEN;
   if( file_seek( FileData.TotalWriteDataSz - CUT_WAV_END_LEN, FS_SEEK_SET ))
   {
      file_set_eof();
   }
   if( file_seek( 4, FS_SEEK_SET ))
   {
      u32_data_remaining = nav_file_lgt() - 8;

      file_putc( LSB0( u32_data_remaining ));
      file_putc( LSB1( u32_data_remaining ));
      file_putc( LSB2( u32_data_remaining ));
      file_putc( LSB3( u32_data_remaining ));
      if( FileData.wfx.wFormatTag == WAVE_FORMAT_IMA_ADPCM )
      {
         if( file_seek( 56 , FS_SEEK_SET ))
         {
            u32_data_remaining -= ( 60 - 8 );
            file_putc( LSB0( u32_data_remaining ));
            file_putc( LSB1( u32_data_remaining ));
            file_putc( LSB2( u32_data_remaining ));
            file_putc( LSB3( u32_data_remaining ));
         }
      }
      else
      {
         if( file_seek( 40, FS_SEEK_SET ))
         {
            u32_data_remaining -= ( 44 - 8 );
            file_putc( LSB0( u32_data_remaining ));
            file_putc( LSB1( u32_data_remaining ));
            file_putc( LSB2( u32_data_remaining ));
            file_putc( LSB3( u32_data_remaining ));
         }
      }
   }
   file_close();
   
   fat_cache_flush();
   nav_select( nav_id_save );
}
#endif


#pragma CODE = PASTER_BASE_TEMP_CODE03

Bool paster_play_find( U16 index )
{
   __X FS_ERR err;
   U8 i;
   __X U32 file_size;

   SDKLoadCode( PASTER_BASE_TEMP_CODE00 );
   if( !paster_index_file_find( index, FALSE ))
   {
      return FALSE;
   }

   g_paster_play_file_type = CODEC_WAV;
   str_copy_u_n( paster_point, atou(( U8 *)paster_file ), 12+1 );
   if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
   {
      i = strlen(( char *)paster_file );
      paster_file[ i - 3 ] = 0;
      strcat( paster_file, "MP3" );
      str_copy_u_n( paster_point, atou(( U8 *)paster_file ), 12+1 );
      if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
         return FALSE;
      g_paster_play_file_type = CODEC_MP3;
   }

   file_size = FSGetFileSize( tisiFileShortpath, &err );
   /*SendString((U8 *)"file_size=");
   send_hex_u32(file_size);
   SendString((U8 *)"\r\n");*/
   if( !file_size )
   {
      return FALSE;
   }
   return TRUE;
}


#pragma CODE = PASTER_BASE_TEMP_CODE04

Bool paster_play_start( void  )
{
   //wav_encrypt = FALSE;
   g_b_play_ap3 = FALSE;

   switch( g_paster_play_file_type )
   {
      case CODEC_MP3:
      audio_is_wav = 0;
      break;

      case CODEC_WAV:
      audio_is_wav = 1;
      break;

      default:
      return FALSE;
   }
   if( !player_play_file( tisiFileShortpath ))
   {
      return FALSE;
   }
   power_forbid_autopoweroff();

   srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
   player_task_start();       // Enable event of player task

   g_paster_state = PASTER_STATE_PLAY;

   srvplayer_g_arg.start_time.hours   = 0;
   srvplayer_g_arg.start_time.minutes = 0;
   srvplayer_g_arg.start_time.seconds = 0;
   mmipaster_play_id_timerdisplay = ts_alloc();
   ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
   mmi_paster_display_icon();
   mmipaster_play_time();
   return TRUE;
}


/*void paster_play_stop( void )
{
   player_ap3_stop();
   //file_close();
   //g_b_search_record = FALSE;
}*/


#if 0
Bool paster_play_is_busy( void )
{
   return player_ap3_is_busy();
}

void paster_play_off( void )
{
   file_close();
}
#endif

#endif  //#if( PASTER == ENABLE )

#endif  //_paster_base_c_
