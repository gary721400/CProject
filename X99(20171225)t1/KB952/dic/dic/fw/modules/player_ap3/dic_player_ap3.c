//! @file player_ap3.c,v
//!
//! Copyright (c) 2005 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player ap3.
//!
//! @version 1.2 snd3b-dvk-1_0_33 $Id: player_ap3.c,v 1.2 2005/10/19 15:09:19 eaudreze Exp $
//!
//! @todo
//! @bug
//!
#ifndef _player_ap3_c_
#define _player_ap3_c_


//_____  I N C L U D E S ___________________________________________________

#include "lib_mcu\compiler.h"
#include "modules\player\player.h"
#include "player_ap3.h"

// Include Audio module definition
#include "modules\audio\audio.h"
// Include the file system access
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"
//#include "modules\file_system\fat.h"
// Include for hardware access
#include "modules\control_access\ctrl_status.h"
#include "modules\control_access\ctrl_access.h"

//#include "modules\file_aux\file_aux.h"

#include "conf\conf_sio.h"
#include "conf\conf_oid.h"
#include "modules/apf/apf.h"

#include "modules\search\search_game.h"
//#include "lib_system\utilities\string.h"
#include "modules\paster\paster_base.h"
#include "mmi\shared\com_var.h"
#include "modules\cue\cue.h"
#include "modules\search\search.h"
//#include <stdlib.h> // Contains Funktion "rand()"
#include "string.h"
#include "lib_system\mailbox\mail_evt.h"
#include "mmi\player\srv_player.h"
#include "lib_system\mailbox\mail.h"
#include "modules\power\power.h"             // Power module definition
#include "mmi\player\mmi_player.h"
#include "lib_mcu\timer\timer_drv.h"
//#include "SSSMP3_Regs.h"
#include "modules\search\search_task.h"
#include "sio\com.h"
#include "modules\search\search_remote.h"
#include "modules\player\player_task.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = PLAYER_AP3_DATA
#pragma CODE = PLAYER_AP3_CODE

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

extern  Bool                  g_b_play_ap3;

extern  Bool                  g_b_player_on;

//extern   xdata    AL_Variables     	varAl;

//_____ D E C L A R A T I O N ______________________________________________
//extern xdata U8  g_b_player_on;
extern  void  player_stop_stream    ( void );
extern  void  player_codec_stop     ( void );
extern  void  player_volume_off     ( void );

#if 1
#if( AP3 == ENABLE )

#endif  //#if( AP3 == ENABLE )


#if 0//( PASTER == ENABLE )

Bool player_apf_start( struct APF_CUE_INFO _MEM_TYPE_SLOW_ *cue_info, U8 codec )
{
   //if( player_ap3_is_busy())
   //   return FALSE;
   if( !file_seek( cue_info->offset, FS_SEEK_SET ))
      return FALSE;
   ap4_audio_var.audio_var.file_size_count = cue_info->offset;
   ap4_audio_var.audio_var.play_file_size = cue_info->size;
   //if( ap4_audio_var.audio_var.play_file_size < FS_512B*4 )
   //   return FALSE;
   switch( codec )
   {
      case 0:
      audio_is_wav = 0;
      break;

      case 2:
      audio_is_wav = 1;
      //break;

      default:
      return FALSE;
   }
   wav_encrypt = FALSE;
   //ap4_audio_var.audio_var.play_file_size += ap4_audio_var.audio_var.file_size_count;// + FS_512B*2;
   return player_music_start();
}

#endif  //#if( PASTER == ENABLE )


#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
#if 0
Bool paster_back_to_top_dir( void )
{
   //if( g_paster_dir_index.u8_lun == 0xFF )
   {
      if( !nav_drive_set( LUN_ID_MMC_SD ))
      {
         return FALSE;
      }
      if( !nav_dir_root())
      {
         return FALSE;
      }
      unicode_init_buffer_enc( paster_unicode_buffer(), G_STRING_SIZE2, ENCODING_UTF16BE );
      string_ascii_to_unicode( paster_unicode_buffer(), g_paster_dir );
      if( !nav_filelist_findname( string_unicode( paster_unicode_buffer()), FALSE ))
      {
         if( !nav_dir_make( string_unicode( paster_unicode_buffer())))
         {
            if( FS_ERR_FILE_EXIST != fs_g_status )
               return FALSE;
         }
      }
      if( !nav_dir_cd())
      {
         return FALSE;
      }
      //g_paster_dir_index = nav_getindex();
   }
   /*else
   {
      if( !nav_gotoindex( &g_paster_dir_index ))
      {
         return  FALSE;
      }
   }*/
   return TRUE;
}


Bool cue_play( U8 index )
{
   if( player_ap3_is_busy())
   {
      return FALSE;
   }

   //unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
   //Unicode_prepare_buffer_for_mailing( g_string2, Unicode_get_char_space( g_string2 ));
   if( !file_seek( APF_OFFSET_CUE_INDEX_TABLE + ( U32 )index * sizeof( g_cue_info ), FS_SEEK_SET ))
   {
      return FALSE;
   }
   if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&g_cue_info, sizeof( g_cue_info ))!= sizeof( g_cue_info ))
   {
      return FALSE;
   }
   if(( g_cue_info.offset == 0 )||( g_cue_info.size < 512 ))
   {
      return FALSE;
   }
   
   /*if( !player_apf_start( &g_cue_info, CODEC_MP3 ))
   {
      return FALSE;
   }*/
   ap4_audio_var.audio_var.file_size_count = g_cue_info.offset;
   ap4_audio_var.audio_var.play_file_size = g_cue_info.size;
   Key_encrypt_mp3 = 0x00;
   audio_is_wav = 0;
   wav_encrypt = FALSE;
   return player_music_start();
}


void cue_off( void )
{
   U8 nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_CUE );
   file_close();
   nav_select( nav_id_saved );
}


Bool player_file_start( U32 start_addr, U8 codec )
{
   if( player_ap3_is_busy())
   {
      return FALSE;
   }
   if( !file_seek( start_addr, FS_SEEK_SET ))
   {
      return FALSE;
   }

   if( codec == CODEC_MP3 )
   {
      varAl.WAVE_Flg = 0;
   }
   else if( codec == CODEC_WAV )
   {
      varAl.WAVE_Flg = 1;
   }
   else
   {
      return FALSE;
   }
   wav_encrypt = FALSE;
   ap4_audio_var.audio_var.file_size_count = start_addr;
   ap4_audio_var.audio_var.play_file_size = ap4_audio_var.audio_var.file_size_count + fs_g_nav_entry.u32_size;// + FS_512B*2;
   LROM_SCRAMBLE = 0x00;
   //LROM_EXCHANGE = 0x00;
   return player_music_start();
}
#endif


//#pragma CODE = SEARCH_TEMP_CODE05

Bool player_music_start( void )
{
   g_b_play_ap3 = TRUE;
   g_b_player_on = TRUE;

//***********************************************
   //change by xuanyu
   if( player_play_file( 0 ))
   {
      power_forbid_autopoweroff();
      //GameTimeOverTick = 0;
      //GameTipTick = 0;
      srvplayer_g_player_state = MMI_PLAYER_STATE_PLAY;
      return TRUE;
   }
   g_b_play_ap3 = FALSE;
   g_b_player_on = FALSE;
   return FALSE;
//***********************************************
}


#pragma CODE = PLAYER_AP3_CODE

void player_ap3_stop( void )
{
   player_volume_off();
   //MP3_PrevInit();
   player_stop_stream();
   //player_codec_stop();

   //add by xuanyu
   power_authorize_autopoweroff();
   //GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
   //GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
   //bSpeechEnd = TRUE;
   player_task_stop();
   //speech_status = SPEECH_IDLE;
   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
}


Bool player_ap3_is_busy( void ) 
{
   //return g_b_play_ap3 && g_b_player_on;
   return ( g_b_play_ap3 || g_b_player_on );
}
#endif  //#if(( PASTER == ENABLE )||( AP3 == ENABLE ))
#endif

#endif  //_player_ap3_c_
