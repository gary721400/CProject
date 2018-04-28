//!
//! @file srv_player.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the services for the application PLAYER
//!
//! @version 2.6 snd3-dvk-1_9_5 $Id: srv_player.c,v 2.6 2007/07/26 15:51:33 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _srv_player_c_
#define _srv_player_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_audio.h"

#include <stdio.h>

#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "srv_player.h"

#include "modules\mmi_manager\mmgr_kernel.h"
//#include "modules\control_access\ctrl_access.h"
#include "modules\player\player_task.h"
#include "modules\player\player.h"
#include "modules\file_system\file.h"           // for file management 
#include "modules\power\power.h"             // Power module definition
//#include "mmi\player\mmi_player.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "mmi\status\mmi_status.h"
#include "modules\file_system\navigation.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "conf\conf_mmi.h"

#pragma DATA = SRV_PLAYER_DATA
#pragma CODE = SRV_PLAYER_CODE

ImageDeclareType( NAVIGATION_CODE );

//________________ D E C L A R A T I O N S ___________________



//_____ D E F I N I T I O N S ______________________________________________
_MEM_TYPE_MEDSLOW_   Srv_player_states    srvplayer_g_player_state /*= MMI_PLAYER_STATE_STOP*/;
_MEM_TYPE_MEDSLOW_   St_player_arg        srvplayer_g_arg;
_MEM_TYPE_MEDSLOW_   Elapsed_time         srvplayer_all_time;




//_____ D E F I N I T I O N S ______________________________________________

#pragma CODE = SRV_PLAYER_TEMP_CODE00

//! Switch the player on
//!
Bool srvplayer_switch_on( void )
{
   srvplayer_g_player_state      = MMI_PLAYER_STATE_STOP;
   srvplayer_g_arg.start_time.hours   = 0;
   srvplayer_g_arg.start_time.minutes = 0;
   srvplayer_g_arg.start_time.seconds = 0;
   return player_on();
}


#pragma CODE = SRV_PLAYER_TEMP_CODE01

//! Switch the player off
//!
void srvplayer_switch_off( void )
{
   player_off();
   srvplayer_g_player_state      = MMI_PLAYER_STATE_STOP;
} 


#pragma CODE = SRV_PLAYER_TEMP_CODE02

//! Get file informations
//!
void srvplayer_file_getinfos( void )
{
#if 0
   // Get folder name
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_dir_name( g_string2, G_STRING_SIZE2 );
#else
   unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
   nav_dir_name( Unicode_get_addr_string( g_string2 ), Unicode_get_char_space( g_string2 ) );
#endif
#endif

   // Get name of file
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_file_name( g_string, G_STRING_SIZE, FS_NAME_GET, FALSE );
#else
   unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   SDKLoadCode( NAVIGATION_CODE );
   ( void )nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
   //nav_file_name( g_string, G_STRING_SIZE / 2, FS_NAME_GET, FALSE );
#endif
/*
   // Get ID3 informations
   player_id3_read( &srvplayer_id3 );
   // Reset others information avialable only in play
   srvplayer_g_codec_type  = CODEC_PLAY_MAX;
   srvplayer_stream_channel_type    = 0;
   srvplayer_stream_sampling_freq   = 0;
   srvplayer_stream_bitrate         = 0;
   */
}


#pragma CODE = SRV_PLAYER_TEMP_CODE03

//! Play the current selected file
//!
//! @return SRVPLAYER_PLAY_OK , SRVPLAYER_PLAY_FAIL, SRVPLAYER_PLAY_NOCODEC, SRVPLAYER_PLAY_NOSUPPORTED
//!
U8 srvplayer_play( Bool b_resume_time )
{
   extern Bool g_b_play_ap3;
   if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
      return SRVPLAYER_PLAY_OK;   // Already in play mode

   // Init player sound and speed
   //srvplayer_volume_send();

   if( !b_resume_time )
   {
   }
   g_b_play_ap3 = FALSE;
   if( !player_play_file(( char *)tisiFileShortpath ))
   {
      //if( PLAYER_ERR_NO_CODEC_FOUND == player_g_status )
      //   return SRVPLAYER_PLAY_NOCODEC;
      return SRVPLAYER_PLAY_FAIL;
   }
   /*power_forbid_autopoweroff();

   srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
   player_task_start();       // Enable event of player task*/
   srvplayer_g_arg.start_time.hours   = 0;
   srvplayer_g_arg.start_time.minutes = 0;
   srvplayer_g_arg.start_time.seconds = 0;
   return SRVPLAYER_PLAY_OK;
}


#pragma CODE = SRV_PLAYER_TEMP_CODE07

//! Get time during play
//!
///*
void srvplayer_gettime( void )
{
   ts_set_time( mmipaster_play_id_timerdisplay, TS_FLASH_TIME );
   if( MMI_PLAYER_STATE_PLAY == srvplayer_g_player_state)
   {
      srvplayer_g_arg.start_time.seconds++;
      if( srvplayer_g_arg.start_time.seconds >= 60 )
      {
         srvplayer_g_arg.start_time.seconds = 0;
         srvplayer_g_arg.start_time.minutes++;
         if( srvplayer_g_arg.start_time.minutes >= 60 )
         {
            srvplayer_g_arg.start_time.minutes = 0;
            srvplayer_g_arg.start_time.hours++;
         }
      }
   }
}
//*/

#pragma CODE = SRV_PLAYER_TEMP_CODE04

//! Stop play of the current selected file
//!
void srvplayer_stop( void )
{
   srvplayer_g_arg.start_time.hours   = 0;
   srvplayer_g_arg.start_time.minutes = 0;
   srvplayer_g_arg.start_time.seconds = 0;
   if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
      return;   // Already in stop mode

   //( void )player_set_volume( 0 );
   if( player_stop())
   {
      player_off();
   }
   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   player_task_stop();     // Disable event of player task
}


#pragma CODE = PLAYER_CODE

//! Modify the sound level
//!
//! @param   U8 : the request in the volume level : SRVPLAYER_VOL_UP or SRVPLAYER_VOL_DOWN
//!
void  srvplayer_volume_change( Bool change )
{
   if( SRVPLAYER_VOL_UP == change )
   {
      if( g_volume < (VOL_LEVEL-1))
      {
         g_volume++;
      }
   }
   else
   {
      if( g_volume )
      {
         g_volume--;
      }
   }
   srvplayer_volume_send();
}


//! Send the volume at the player
//!
void  srvplayer_volume_send( void )
{
   /*U8 value;
   value = (VOLUME_LEVEL_NB_STEP*(U16)srvplayer_g_arg.volume) / SRVPLAYER_VOL_LEVEL;
   player_set_volume( value | (((U16)value)<<8) );*/
   ( void )player_set_volume(( U16 )g_volume );
}


#pragma CODE = SRV_PLAYER_TEMP_CODE05

//! Pause function
//!
Bool srvplayer_pause( void )
{
   if(MMI_PLAYER_STATE_PLAY  != srvplayer_g_player_state)
      return FALSE;
   
   if( !player_pause())
      return FALSE;
   
   srvplayer_g_player_state = MMI_PLAYER_STATE_PAUSE;
   return TRUE;
}


#pragma CODE = SRV_PLAYER_TEMP_CODE06

//!  Stop FFW / FRW / PAUSE and restart play
//!
Bool srvplayer_restart_play( void )
{
   if( (MMI_PLAYER_STATE_PAUSE      != srvplayer_g_player_state)
   &&  (MMI_PLAYER_STATE_PLAY_FFW   != srvplayer_g_player_state)
   &&  (MMI_PLAYER_STATE_PLAY_FRW   != srvplayer_g_player_state) )
      return FALSE;
   
   // Restart play after a pause, FWW, FRW.
   if( !player_restart_play())
      return FALSE;

   srvplayer_g_player_state = MMI_PLAYER_STATE_PLAY;
   return TRUE;
}

#endif // _srv_player_c_

