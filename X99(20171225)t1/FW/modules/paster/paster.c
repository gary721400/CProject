//!
//! @file paster.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster event.
//!
//! @version 1.60 snd3b-dvk-1_0_33 $Id: paster.c,v 1.60 2006/12/19 12:59:28 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _paster_c_
#define _paster_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "paster.h"
#include "paster_task.h"

#include <stdio.h>

#include "paster_base.h"
#include "modules\cue\cue.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "drivers\mmi\img_handler.h"
#include "mmi\kbd\keypad.h"

#include "modules\file_aux\file_aux.h"
//#include "lib_system\utilities\string.h"

#include "modules\player\player.h"
#include "modules\player_ap3\player_ap3.h"
#include "mmi\player\srv_player.h"

#include "modules\search\search_remote.h"

#include "conf\conf_sio.h"
#include "modules\power\power.h"             // Power module definition
#include "lib_mcu\timer\timer_drv.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "mmi\paster\mmi_paster.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\search\search.h"
#include "mmi\recorder\srv_recorder.h"
#include "drivers\oid\oid.h"
#include "sio\com.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "modules\recorder\recorder_task.h"
#include "mmi\startup\mmi_startup.h"

#include "conf\conf_lcd.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "modules\oid_cmd\oid_cmd.h"
#include "mmi\shared\com_appli.h"
#include "mmi\ap3\mmi_ap3.h"
#include "modules\search\search_remote.h"
#include "string.h"
#include "modules\recorder\recorder.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "mmi\status\mmi_status.h"

#pragma DATA = PASTER_DATA
#pragma CODE = PASTER_CODE

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( PASTER_TEMP_CODE00 );
ImageDeclareType( PASTER_TEMP_CODE01 );
ImageDeclareType( PASTER_TEMP_CODE02 );
ImageDeclareType( PASTER_TEMP_CODE03 );
ImageDeclareType( PASTER_TEMP_CODE04 );
ImageDeclareType( PASTER_TEMP_CODE05 );
ImageDeclareType( PASTER_TEMP_CODE06 );
ImageDeclareType( PASTER_TEMP_CODE07 );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( PASTER_BASE_TEMP_CODE00 );
ImageDeclareType( PASTER_BASE_TEMP_CODE01 );
ImageDeclareType( PASTER_BASE_TEMP_CODE02 );
ImageDeclareType( PASTER_BASE_TEMP_CODE03 );
ImageDeclareType( PASTER_BASE_TEMP_CODE04 );

ImageDeclareType( PLAYER_AP3_TEMP_CODE00 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( PASTER_STATE_CHECK_CODE );
ImageDeclareType( PASTER_SOUND_START_CODE );
ImageDeclareType( PASTER_SOUND_WAIT_CODE );
ImageDeclareType( PASTER_RECORD_CODE );
ImageDeclareType( PASTER_PLAY_CODE );
ImageDeclareType( PASTER_RECORD_READY_CODE );
ImageDeclareType( PASTER_IDLE_CODE );
ImageDeclareType( PASTER_CUE_PLAY_CODE );
ImageDeclareType( PASTER_CMD_CODE );

ImageDeclareType( RECORDER_TEMP_CODE06 );
ImageDeclareType( RECORDER_TEMP_CODE07 );

ImageDeclareType( PASTER_RECORD_STOP_CODE );
ImageDeclareType( PASTER_PLAY_STOP_CODE );
ImageDeclareType( PASTER_CMD_OID_TEMP_CODE00 );
ImageDeclareType( PASTER_CMD_OID_TEMP_CODE01 );

ImageDeclareType( CHECK_STATE_CMD_CODE );
ImageDeclareType( RECORD_STATE_CMD_CODE );
ImageDeclareType( PLAY_STATE_CMD_CODE );

ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );

ImageDeclareType( PASTER_RECORD_PREPARE_CODE );

#if( PASTER == ENABLE )

//_____ M A C R O S ________________________________________________________

void  paster_disk_full_display   ( void );
void  paster_error_display       ( void );
void  paster_record_fail_display ( void );
void  paster_nofile_display      ( void );
void  paster_state_check         ( void );
void  paster_state_sound_start   ( void );
void  paster_state_sound_wait    ( void );
void  paster_state_record	      ( void );
void  paster_state_play          ( void );
void  paster_record_ready        ( void );
void  paster_idle_deal           ( void );
void  cue_play_apf_deal          ( void );
void  paster_state_record_stop   ( void );
void  paster_state_play_stop     ( void );
U8    paster_oid_cmd_funtion     ( void );
U8    paster_oid_cmd_normal      ( void );
void  check_state_oid_cmd_deal   ( void );
void  record_state_oid_cmd_deal  ( void );
void  play_state_oid_cmd_deal    ( void );

void  paster_record_prepare_display ( void );


//_____ D E F I N I T I O N ________________________________________________
extern __X   FCFILE   *ADPCM_File;

/*static*/ U8 _MEM_TYPE_SLOW_  s_paster_cue /*= 0*/;

U8    _MEM_TYPE_SLOW_   g_paster_state;
U8    _MEM_TYPE_SLOW_   g_paster_last_state;
U8    _MEM_TYPE_SLOW_   g_paster_cue_next_state;
U8    _MEM_TYPE_SLOW_   g_paster_error_next_state;
U8    _MEM_TYPE_SLOW_   g_paster_error;
U8    _MEM_TYPE_SLOW_   g_paster_cue_index;

U8    _MEM_TYPE_SLOW_   g_paster_cmd_id;
U16   _MEM_TYPE_SLOW_   g_paster_cmd_param;

U16   _MEM_TYPE_SLOW_   g_paster_oid_index;
//U16   _MEM_TYPE_SLOW_   g_paster_oid_index_copy;

U8    _MEM_TYPE_SLOW_   g_paster_stop_next_state;

//U16   _MEM_TYPE_SLOW_   g_paster_last_time;
//U32   _MEM_TYPE_SLOW_	  g_paster_last_pos;

// ID of the timer soft used by this MMI appli
_MEM_TYPE_SLOW_  U8    s_paster_display_id;

#define  SOUND_FILENAME_LENGTH         18
typedef  struct   _PasterSoundFile
{
   U8    cue_index;
   U8    file_name[ SOUND_FILENAME_LENGTH ];
}PasterSoundFile;


#pragma CODE = PASTER_TEMP_CODE03

PasterSoundFile   const   g_paster_sound_files[] = 
{
   { CUE_INDEX_CHECK,                     "pastermenu.mp3"  }
	
,  { CUE_INDEX_RECORD,                    "recordstart.mp3" }
,  { CUE_INDEX_RECORD_PAUSE,              "recordpause.mp3" }
,  { CUE_INDEX_RECORD_RESUME,             "recordresume.mp3"}
,  { CUE_INDEX_RECORD_END,                "recordend.mp3"   }
	
,  { CUE_INDEX_PLAY,                      "playstart.mp3"   }
,  { CUE_INDEX_PLAY_PAUSE,                "playpause.mp3"   }
,  { CUE_INDEX_PLAY_RESUME,               "playresume.mp3"  }
,  { CUE_INDEX_PLAY_END,                  "playend.mp3"     }

,  { CUE_INDEX_ERROR_DISK_IS_FULL,        "diskfull.mp3"    }
,  { CUE_INDEX_ERROR_FIND_FILE_FAILED,    "filenotfound.mp3"}
//,  { CUE_INDEX_POWER_OFF,                 "poweroff.mp3"}
,  { CUE_INDEX_PASTER_START,              "pasterstart.mp3" }
,  { CUE_INDEX_PASTER_POWER_ON,           "poweron.mp3"     }

,  { CUE_INDEX_VOLUME,                    "volume.mp3"      }
};

_MEM_TYPE_SLOW_   U8    paster_sound_file_name[ SOUND_FILENAME_LENGTH ];


Bool paster_cue_to_sound_file( U8 cue_index )
{
   U8 _MEM_TYPE_SLOW_ iter;
   //memset( paster_sound_file_name, 0, SOUND_FILENAME_LENGTH );
   for( iter = 0; iter < sizeof( g_paster_sound_files ) / sizeof( g_paster_sound_files[ 0 ]); ++iter )
   {
      if( cue_index == g_paster_sound_files[ iter ].cue_index )
      {
         //return g_paster_sound_files[ iter ].file_name;
         //memcpy( paster_sound_file_name, g_paster_sound_files[ iter ].file_name, SOUND_FILENAME_LENGTH );
         strcpy(( char *)paster_sound_file_name, ( char *)g_paster_sound_files[ iter ].file_name );
         return TRUE;
      }
   }
   return FALSE;
}


#pragma CODE = PASTER_CODE

U8	_MEM_TYPE_SLOW_	g_paster_sound_next_state;
U8	_MEM_TYPE_SLOW_	g_paster_sound_index;
//Fs_index _MEM_TYPE_SLOW_   g_paster_sound_old_pos;


//Bool                    g_oid = FALSE;

extern            Bool              g_b_player_on;
//extern            Bool              g_b_search_record;
extern 	Bool 	g_b_recorder_on;

U32      _MEM_TYPE_SLOW_   pause_time;

#define	paster_error( code )      paster_goto_error( code, PASTER_STATE_CHECK )
U8    paster_translate_cmd ( void );


void paster_send_cmd( U8 id, U16 param )
{
   mail_send_command( JOIN16( PASTER_TASK_ID, id ), param );
}


void paster_reset_cmd( void )
{
   g_paster_cmd_id    = CMD_PASTER_NONE;
   g_paster_cmd_param = 0;
}


void paster_goto_error( U8 error_code, U8 next_state )	
{
   g_paster_error            = error_code;
   g_paster_error_next_state = next_state;
   g_paster_state            = PASTER_STATE_ERROR;
}


void paster_goto_cue( U8 cue_index, U8 next_state )
{
   g_paster_state          = PASTER_STATE_CUE_START;
   g_paster_cue_next_state = next_state;
   g_paster_cue_index      = cue_index;
}


#pragma CODE = PASTER_TEMP_CODE01

void paster_evt_init( void )
{
   g_paster_last_state = PASTER_STATE_IDLE;
   g_paster_state      = PASTER_STATE_IDLE;

   g_paster_cmd_id     = CMD_PASTER_NONE;
   g_paster_cmd_param  = 0;
}


#pragma CODE = PASTER_CODE

void paster_evt( void )
{
   //U8 _MEM_TYPE_SLOW_ g_paster_sound_vol[] = "volume.mp3";
   //U8 _MEM_TYPE_SLOW_ g_paster_sound_dir[] = "subdir";
   //U8 _MEM_TYPE_SLOW_ g_u8_disk_full[] = "diskfull.mp3";
   //U8 _MEM_TYPE_SLOW_ *sound_file_name;
   U16 _MEM_TYPE_SLOW_ param;
   
   if( g_paster_cmd_id == CMD_PASTER_NONE )
   {
      if( g_oid )
      {
         g_paster_cmd_id = CMD_PASTER_OID;
         g_paster_cmd_param = Current_Index;
         g_oid = FALSE;
      }
   }
   switch( g_paster_state )
   {
      case PASTER_STATE_IDLE:
      SDKLoadCode( PASTER_IDLE_CODE );
      paster_idle_deal();
#if 0
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_START:
         SDKLoadCode( PASTER_TEMP_CODE00 );
         if( !paster_init_disk())
         {
            mail_send_event( EVT_PASTER_NO_APF_FILE, 0 );
            break;
         }

         /*apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
         scan_oid_enable();
         mmi_paster_kbd_enable();
         //g_paster_state = PASTER_STATE_START;
         if( !s_paster_cue )
         {
            s_paster_cue = 1;
            paster_goto_cue( CUE_INDEX_PASTER_POWER_ON, PASTER_STATE_START );
         }
         else
            paster_goto_cue( CUE_INDEX_PASTER_START, PASTER_STATE_START );*/
         break;
      }
      paster_reset_cmd();
#endif
      break;

      case PASTER_STATE_START:
      //file_aux_free_cluster_init();
      paster_goto_cue( CUE_INDEX_CHECK, PASTER_STATE_CHECK );
      break;
    
      case PASTER_STATE_CHECK:
      SDKLoadCode( PASTER_STATE_CHECK_CODE );
      paster_state_check();
#if 0
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_RECORD:
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
         g_paster_state = PASTER_STATE_RECORD_START;
         break;
      
         case CMD_PASTER_PLAY:
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
         g_paster_state = PASTER_STATE_PLAY_START;
         break;
      
         /*case CMD_PASTER_COPY:
         paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_SRC_OID );
         break;*/
      
         //case CMD_PASTER_STOP:
         case CMD_PASTER_END:
         //case CMD_PASTER_BACK:
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         //g_paster_state = PASTER_STATE_END;
         break;
      
         /*case CMD_PASTER_POWER_LOW:
         paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
         break;*/

         case CMD_PASTER_VOL_UP:
         case CMD_PASTER_VOL_DOWN:
         //g_paster_sound_old_pos = nav_getindex();
         //g_paster_sound_name = g_paster_sound_vol;
         g_paster_sound_next_state = PASTER_STATE_CHECK;
         g_paster_sound_index = CUE_INDEX_VOLUME;
         g_paster_state = PASTER_STATE_SOUND_START;
         break;
      
         case CMD_PASTER_POWER_OFF:
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         break;
      }
      paster_reset_cmd();
#endif
      break;

      case PASTER_STATE_CUE_START:
      SDKLoadCode( PASTER_CUE_PLAY_CODE );
      cue_play_apf_deal();
#if 0
      SDKLoadCode( PLAYER_AP3_TEMP_CODE00 );
      if( !cue_play( g_paster_cue_index ))
      {
         //paster_error( PASTER_ERROR_CUE_ERROR );
         //g_paster_state = g_paster_cue_next_state;
         //g_paster_sound_old_pos = nav_getindex();
         g_paster_sound_next_state = g_paster_cue_next_state;
         g_paster_sound_index = g_paster_cue_index;
         g_paster_state = PASTER_STATE_SOUND_START;
         break;
      }
      g_paster_state = PASTER_STATE_CUE_PLAY;
#endif
      break;

      case PASTER_STATE_CUE_PLAY:
      if( g_paster_cmd_id == CMD_PASTER_PLAY_EOF )	
      {
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //cue_off();
         g_paster_state = g_paster_cue_next_state;
      }
      goto cue_eof_next;
#if 0
      if( g_paster_cmd_id == CMD_PASTER_NONE )
         break;
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_RECORD:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
         }
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
         g_paster_state = PASTER_STATE_RECORD_START;
         break;

         case CMD_PASTER_PLAY:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
         }
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
         g_paster_state = PASTER_STATE_PLAY_START;
         break;

         case CMD_PASTER_END:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
         }
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;

         case CMD_PASTER_VOL_UP:
         case CMD_PASTER_VOL_DOWN:
         //g_paster_sound_name = g_paster_sound_vol;
         //g_paster_state = PASTER_STATE_SOUND_START;
         break;

         case CMD_PASTER_POWER_OFF:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
         }
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         break;
      }
      paster_reset_cmd();
      break;
#endif

      case PASTER_STATE_SOUND_START:
      SDKLoadCode( PASTER_SOUND_START_CODE );
      paster_state_sound_start();
#if 0
      SDKLoadCode( PASTER_TEMP_CODE03 );
      if( !paster_cue_to_sound_file( g_paster_sound_index ))
      {
         g_paster_state = g_paster_sound_next_state;
         break;
      }
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
         //file_close();
      }

      SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
      // Search file to play
      if( !search_subdir_mp3_file(( char *)paster_sound_file_name ))
      {
         goto __sound_failed;
      }
      SDKLoadCode( MMI_PLAYINTRO_CODE01 );
      if( !play_sbdir_mp3_file())
      {
         goto __sound_failed;
      }
      g_paster_state = PASTER_STATE_SOUND_WAIT;
      break;
      
__sound_failed:
      g_paster_state = g_paster_sound_next_state;
#endif
      break;

      case PASTER_STATE_SOUND_WAIT:
      if( g_paster_cmd_id == CMD_PASTER_PLAY_EOF )	
      {
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //cue_off();
         //nav_gotoindex( &g_paster_sound_old_pos );
         //g_paster_state = PASTER_STATE_CHECK;
         g_paster_state = g_paster_sound_next_state;
      }
cue_eof_next:
      SDKLoadCode( PASTER_SOUND_WAIT_CODE );
      paster_state_sound_wait();
#if 0
      if( g_paster_cmd_id == CMD_PASTER_NONE )
         break;
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_RECORD:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
            //nav_gotoindex( &g_paster_sound_old_pos );
         }
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
         g_paster_state = PASTER_STATE_RECORD_START;
         break;

         case CMD_PASTER_PLAY:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
            //nav_gotoindex( &g_paster_sound_old_pos );
         }
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
         g_paster_state = PASTER_STATE_PLAY_START;
         break;

         case CMD_PASTER_END:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
            //nav_gotoindex( &g_paster_sound_old_pos );
         }
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;

         case CMD_PASTER_VOL_UP:
         case CMD_PASTER_VOL_DOWN:
         //g_paster_sound_name = g_paster_sound_vol;
         //g_paster_state = PASTER_STATE_SOUND_START;
         break;

         case CMD_PASTER_POWER_OFF:
         if( player_ap3_is_busy())
         {
            player_ap3_stop();
            //cue_off();
            //nav_gotoindex( &g_paster_sound_old_pos );
         }
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         break;
      }
      paster_reset_cmd();
#endif
      break;

      case PASTER_STATE_RECORD_START:
      if( g_b_disk_is_full )
      {
disk_full_next:
         SDKLoadCode( PASTER_TEMP_CODE02 );
         paster_disk_full_display();
#if 0
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
         Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
         lcd_goto_xy( 20, 12 );
         lcd_puts( "Memory full" );
#elif( LANGUAGE == Simplified )
         lcd_goto_xy( 32, 12 );
         lcd_puts( "¿Õ¼ä²»×ã" );
#elif( LANGUAGE == Traditional )
         lcd_goto_xy( 32, 12 );
         lcd_puts( "ªÅ¶¡¤£¨¬" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
         switch( g_setting_list.language_ihm )
         {
            case LANG_ENGLISH:
            lcd_goto_xy( 20, 12 );
            lcd_puts( "Memory full" );
            break;
            
            case LANG_CHINESE_SIMP:
            lcd_goto_xy( 32, 12 );
            lcd_puts( "¿Õ¼ä²»×ã" );
            break;

            case LANG_CHINESE_TRAD:
            lcd_goto_xy( 32, 12 );
            lcd_puts( "ªÅ¶¡¤£¨¬" );
            break;
         }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
         //paster_error( PASTER_ERROR_DISK_IS_FULL );
         //g_paster_state = PASTER_STATE_DISK_FULL_NOTICE;
         //g_paster_sound_old_pos = nav_getindex();
         g_paster_sound_next_state = PASTER_STATE_CHECK;
         g_paster_sound_index = CUE_INDEX_ERROR_DISK_IS_FULL;
         g_paster_state = PASTER_STATE_SOUND_START;
#endif
         break;
      }


      SDKLoadCode( PASTER_RECORD_PREPARE_CODE );
      paster_record_prepare_display();
      
      SDKLoadCode( PASTER_BASE_TEMP_CODE01 );
      if( !paster_record_find( g_paster_oid_index ))
      {
         if( g_b_disk_is_full )
         {
            goto disk_full_next;
#if 0
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 20, 12 );
            lcd_puts( "Memory full" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "¿Õ¼ä²»×ã" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "ªÅ¶¡¤£¨¬" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            switch( g_setting_list.language_ihm )
            {
               case LANG_ENGLISH:
               lcd_goto_xy( 20, 12 );
               lcd_puts( "Memory full" );
               break;
               
               case LANG_CHINESE_SIMP:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "¿Õ¼ä²»×ã" );
               break;

               case LANG_CHINESE_TRAD:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "ªÅ¶¡¤£¨¬" );
               break;
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            //paster_error( PASTER_ERROR_DISK_IS_FULL );
            //g_paster_state = PASTER_STATE_DISK_FULL_NOTICE;
            //g_paster_sound_old_pos = nav_getindex();
            g_paster_sound_next_state = PASTER_STATE_CHECK;
            g_paster_sound_index = CUE_INDEX_ERROR_DISK_IS_FULL;
            g_paster_state = PASTER_STATE_SOUND_START;
#endif
         }
         else
         {
            SDKLoadCode( PASTER_TEMP_CODE04 );
            paster_error_display();
#if 0
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 16, 12 );
            lcd_puts( "Record error" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "Â¼Òô´íÎó" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "¿ý­µ¿ù»~" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            switch( g_setting_list.language_ihm )
            {
               case LANG_ENGLISH:
               lcd_goto_xy( 16, 12 );
               lcd_puts( "Record error" );
               break;
            
               case LANG_CHINESE_SIMP:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "Â¼Òô´íÎó" );
               break;

               case LANG_CHINESE_TRAD:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "¿ý­µ¿ù»~" );
               break;
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            paster_error( PASTER_ERROR_FIND_FILE_FAILED );
#endif
         }
         break;
      }
      SDKLoadCode( PASTER_TEMP_CODE07 );
      //mmipaster_display_name();
      paster_file_getinfos();
      /*srvplayer_g_arg.start_time.hours   = 0;
      srvplayer_g_arg.start_time.minutes = 0;
      srvplayer_g_arg.start_time.seconds = 0;
      mmi_paster_display_icon();
      mmipaster_play_time();*/
      //paster_goto_cue( CUE_INDEX_RECORD_READY, PASTER_STATE_RECORD_START_READY );
      paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START_READY );
      break;

      case PASTER_STATE_RECORD_START_READY:
      SDKLoadCode( PASTER_RECORD_READY_CODE );
      paster_record_ready();
#if 0
      SDKLoadCode( PASTER_BASE_TEMP_CODE02 );
      SendString((U8 *)"record\r\n");
      if( !paster_record_start())
      {
         SDKLoadCode( PASTER_TEMP_CODE05 );
         paster_record_fail_display();
#if 0
         //paster_record_stop();
         //paster_error( PASTER_ERROR_RECORD_START_FAILED );
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
         Ih_set_curr_font( IDC_FONT_16 );
         switch( record_result )
         {
            case RECORD_ERROR:
            paster_error( PASTER_ERROR_RECORD_START_FAILED );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 16, 12 );
            lcd_puts( "Record error" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "Â¼Òô´íÎó" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "¿ý­µ¿ù»~" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            switch( g_setting_list.language_ihm )
            {
               case LANG_ENGLISH:
               lcd_goto_xy( 16, 12 );
               lcd_puts( "Record error" );
               break;
               
               case LANG_CHINESE_SIMP:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "Â¼Òô´íÎó" );
               break;

               case LANG_CHINESE_TRAD:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "¿ý­µ¿ù»~" );
               break;
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            break;

            case RECORD_FILE_FULL:
            case RECORD_FILE_ERROR:
            case RECORD_MEMORY_FULL:
            paster_error( PASTER_ERROR_DISK_IS_FULL );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 20, 12 );
            lcd_puts( "Memory full" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "¿Õ¼ä²»×ã" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts( "ªÅ¶¡¤£¨¬" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            switch( g_setting_list.language_ihm )
            {
               case LANG_ENGLISH:
               lcd_goto_xy( 20, 12 );
               lcd_puts( "Memory full" );
               break;
               
               case LANG_CHINESE_SIMP:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "¿Õ¼ä²»×ã" );
               break;
               
               case LANG_CHINESE_TRAD:
               lcd_goto_xy( 32, 12 );
               lcd_puts( "ªÅ¶¡¤£¨¬" );
               break;
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            break;
         }
#endif
         break;
      }
      record_result = RECORD_OK;
      lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
      s_paster_display_id = ts_alloc();
      ts_set_time( s_paster_display_id, PASTER_RECORD_TIME );
      
      green_led_stop_timer();
      red_led_stop_timer();
      red_led_on();
      green_led_on();
      pause_time = Timer_set_timer( TIMER_S( 1 ));
      g_paster_state = PASTER_STATE_RECORD;
#endif
      break;

      case PASTER_STATE_RECORD:
      SDKLoadCode( PASTER_RECORD_CODE );
      paster_state_record();
#if 0
      if( !Timer_is_timeout( pause_time ))
         break;
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_RECORD:
         g_paster_state = PASTER_STATE_RECORD_START;
         goto paster_record_next;
#if 0
         paster_record_stop();
         //paster_record_file_end();
         
         //Ts_free( s_paster_display_id );
         //power_authorize_autopoweroff();
         mmi_paster_display_icon();
         mmipaster_play_time();
         
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
#endif
         break;

         case CMD_PASTER_PLAY:
         g_paster_state = PASTER_STATE_PLAY_START;
paster_record_next:
         paster_record_stop();
         //paster_record_file_end();
         
         //Ts_free( s_paster_display_id );
         //power_authorize_autopoweroff();
         mmi_paster_display_icon();
         mmipaster_play_time();
         
         g_paster_oid_index = g_paster_cmd_param;
         //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
         break;
         
         /*case CMD_PASTER_PAUSE:
         g_paster_stop_next_state = PASTER_STATE_RECORD_PAUSE;
         paster_record_stop();
         //paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_PAUSE );
         paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_STOP );
         break;*/
      
         case CMD_PASTER_STOP:
         case CMD_PASTER_RECORD_EOF:
         g_paster_stop_next_state = PASTER_STATE_CHECK;
         paster_record_stop();
         /*if( s_b_vbus_present )
            green_led_off();
         else
            red_led_off();*/
         //paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
         paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
         break;
      
         /*case CMD_PASTER_COPY:
         //paster_record_stop();
         //paster_goto_cue(CUE_INDEX_COPY,PASTER_STATE_COPY_DEST_OID);
         break;
      
         case CMD_PASTER_BACK:
         g_paster_stop_next_state = PASTER_STATE_CHECK;
         paster_record_stop();
         //paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_RECORD_STOP );
         break;*/
      
         case CMD_PASTER_END:
         g_paster_stop_next_state = PASTER_STATE_END;
         paster_record_stop();
         //paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_RECORD_STOP );
         break;
      
         /*case CMD_PASTER_POWER_LOW:
         g_paster_stop_next_state = PASTER_STATE_END;
         paster_record_stop();
         paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_RECORD_STOP );
         break;*/
      
         case CMD_PASTER_POWER_OFF:
         g_paster_stop_next_state = PASTER_STATE_END;
         paster_record_stop();
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_RECORD_STOP );
         break;
      }
      paster_reset_cmd();
#endif
      break;
      
#if 0
      case PASTER_STATE_RECORD_PAUSE:
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_PAUSE:
         paster_goto_cue( CUE_INDEX_RECORD_RESUME, PASTER_STATE_RECORD_RESUME );
         break;
      
         case CMD_PASTER_STOP:
         //paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
         paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_CHECK );
         break;
      
         case CMD_PASTER_COPY:
         paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_DEST_OID );
         break;
      
         case CMD_PASTER_BACK:
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         break;
      
         case CMD_PASTER_END:
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;
      
         case CMD_PASTER_POWER_LOW:
         paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
         break;
      
         case CMD_PASTER_POWER_OFF:
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         break;
      }
      paster_reset_cmd();
      break;
    
      case PASTER_STATE_RECORD_RESUME:
      if( !paster_record_start( PASTER_RECORD_APPEND ))
      {
         paster_record_stop();
         //paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
         paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_CHECK );
         break;
      }
      green_led_stop_timer();
      red_led_stop_timer();
      red_led_on();
      green_led_on();
      g_paster_state = PASTER_STATE_RECORD;
      break;
#endif

      case PASTER_STATE_RECORD_STOP:
      SDKLoadCode( PASTER_RECORD_STOP_CODE );
      paster_state_record_stop();
#if 0
      //g_paster_state = PASTER_STATE_CHECK;
      //paster_record_file_end();
      g_paster_state = g_paster_stop_next_state;
      
      //Ts_free( s_paster_display_id );
      //power_authorize_autopoweroff();
      mmi_paster_display_icon();
      mmipaster_play_time();
#endif
      break;

      case PASTER_STATE_PLAY_START:
      SDKLoadCode( PASTER_BASE_TEMP_CODE03 );
      if( !paster_play_find( g_paster_oid_index ))
      {
         SDKLoadCode( PASTER_TEMP_CODE06 );
         paster_nofile_display();
#if 0
         lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
         Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
         lcd_goto_xy( 36, 12 );
         lcd_puts( "No file" );
#elif( LANGUAGE == Simplified )
         lcd_goto_xy( 40, 12 );
         lcd_puts( "ÎÞÎÄ¼þ" );
#elif( LANGUAGE == Traditional )
         lcd_goto_xy( 40, 12 );
         lcd_puts( "µL¤å¥ó" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
         switch( g_setting_list.language_ihm )
         {
            case LANG_ENGLISH:
            lcd_goto_xy( 36, 12 );
            lcd_puts( "No file" );
            break;
            
            case LANG_CHINESE_SIMP:
            lcd_goto_xy( 40, 12 );
            lcd_puts( "ÎÞÎÄ¼þ" );
            break;

            case LANG_CHINESE_TRAD:
            lcd_goto_xy( 40, 12 );
            lcd_puts( "µL¤å¥ó" );
            break;
         }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
         
         paster_goto_cue( CUE_INDEX_ERROR_FIND_FILE_FAILED, PASTER_STATE_CHECK );
#endif
         break;
      }
      SDKLoadCode( PASTER_TEMP_CODE07 );
      paster_file_getinfos();
      /*srvplayer_g_arg.start_time.hours   = 0;
      srvplayer_g_arg.start_time.minutes = 0;
      srvplayer_g_arg.start_time.seconds = 0;
      mmi_paster_display_icon();
      mmipaster_play_time();*/
      
      paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START_READY );
      break;

      case PASTER_STATE_PLAY_START_READY:
      SDKLoadCode( PASTER_BASE_TEMP_CODE04 );
      if( !paster_play_start())
      {
         //g_b_search_record = FALSE;
         
         /*srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         mmi_paster_display_icon();
         mmipaster_play_time();*/
         paster_error( PASTER_ERROR_PLAY_START_FAILED );
         //break;
      }
      /*g_paster_state = PASTER_STATE_PLAY;

      srvplayer_g_arg.start_time.hours   = 0;
      srvplayer_g_arg.start_time.minutes = 0;
      srvplayer_g_arg.start_time.seconds = 0;
      mmipaster_play_id_timerdisplay = ts_alloc();
      ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
      mmi_paster_display_icon();
      mmipaster_play_time();*/
      break;
    
      case PASTER_STATE_PLAY:
      SDKLoadCode( PASTER_PLAY_CODE );
      paster_state_play();
#if 0
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         /*case CMD_PASTER_PAUSE:
         //g_paster_last_time = player_get_time();
         g_paster_last_pos = paster_play_stop();
         paster_goto_cue( CUE_INDEX_PLAY_PAUSE, PASTER_STATE_PLAY_PAUSE );
         break;*/
      
         case CMD_PASTER_STOP:
         paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
paster_play_next:
         paster_play_stop();
paster_play_next1:
         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
         break;

         case CMD_PASTER_PLAY_EOF:
         //paster_play_stop();
         //paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
         g_paster_state = PASTER_STATE_PLAY_STOP;
         goto paster_play_next1;
#if 0
         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
#endif
         break;
      
         /*case CMD_PASTER_COPY:
         paster_play_stop();
         paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_DEST_OID );
         break;
      
         case CMD_PASTER_BACK:
         paster_play_stop();
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         break;*/
      
         case CMD_PASTER_END:
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         goto paster_play_next;
#if 0
         paster_play_stop();
         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
#endif
         break;
      
         /*case CMD_PASTER_POWER_LOW:
         paster_play_stop();
         paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
         break;*/
      
         case CMD_PASTER_POWER_OFF:
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         goto paster_play_next;
#if 0
         paster_play_stop();
         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
#endif
         break;

         case CMD_PASTER_PLAY:
         g_paster_state = PASTER_STATE_PLAY_START;
         g_paster_oid_index = g_paster_cmd_param;
         goto paster_play_next;
#if 0
         paster_play_stop();
         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
#endif
         break;

         case CMD_PASTER_RECORD:
         g_paster_state = PASTER_STATE_RECORD_START;
         g_paster_oid_index = g_paster_cmd_param;
         goto paster_play_next;
#if 0
         paster_play_stop();

         srvplayer_g_arg.start_time.hours   = 0;
         srvplayer_g_arg.start_time.minutes = 0;
         srvplayer_g_arg.start_time.seconds = 0;
         Ts_free( mmipaster_play_id_timerdisplay );
         mmi_paster_display_icon();
         mmipaster_play_time();
         break;
#endif
      }
      paster_reset_cmd();
#endif
      break;
#if 0
      case PASTER_STATE_PLAY_PAUSE:
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_PAUSE:
         paster_goto_cue( CUE_INDEX_PLAY_RESUME, PASTER_STATE_PLAY_RESUME );
         break;
      
         case CMD_PASTER_STOP:
         paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
         break;
      
         case CMD_PASTER_COPY:
         paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_DEST_OID );
         break;
      
         case CMD_PASTER_BACK:
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         break;
      
         case CMD_PASTER_END:
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;
      
         case CMD_PASTER_POWER_LOW:
         paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
         break;
      
         case CMD_PASTER_POWER_OFF:
         paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
         break;
      }
      paster_reset_cmd();
      break;
		
      case PASTER_STATE_PLAY_RESUME:
      if(( g_paster_last_pos >= fs_g_nav_entry.u32_size )
         ||(( fs_g_nav_entry.u32_size - g_paster_last_pos )<=( FS_512B * 2 )))
      {
         paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
         break;
      }
      if( g_paster_last_pos >= FS_512B * 5 )
         g_paster_last_pos -= FS_512B * 5 ;
      if( !paster_play_start( g_paster_last_pos ))
      {
         paster_play_stop();
         paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
         break;
      }
      g_paster_state = PASTER_STATE_PLAY;
      break;
#endif

      case PASTER_STATE_PLAY_STOP:
      SDKLoadCode( PASTER_PLAY_STOP_CODE );
      paster_state_play_stop();
#if 0
      g_paster_state = PASTER_STATE_CHECK;
#endif
      break;

#if 0
      case PASTER_STATE_COPY_SRC_OID:
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_BACK:
         case CMD_PASTER_STOP:
         //g_paster_state = PASTER_STATE_CHECK;
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         break;
      
         case CMD_PASTER_END:
         //g_paster_state = PASTER_STATE_END;
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;
      
         //case CMD_PASTER_OID:
         case CMD_PASTER_RECORD:
         g_paster_oid_index = g_paster_cmd_param;
         if( !paster_play_find( g_paster_oid_index ))
         {
            paster_goto_cue( CUE_INDEX_ERROR_COPY_SRC_FILE, PASTER_STATE_CHECK );
            break;
         }
         paster_goto_cue( CUE_INDEX_COPY_DEST, PASTER_STATE_COPY_DEST_OID );
         break;
      }
      paster_reset_cmd();
      break;
    
      case PASTER_STATE_COPY_DEST_OID:
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_BACK:
         case CMD_PASTER_STOP:
         paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
         break;
      
         case CMD_PASTER_END:
         paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
         break;
      
         //case CMD_PASTER_OID:
         case CMD_PASTER_RECORD:
         g_paster_oid_index_copy = g_paster_cmd_param;
         if( g_paster_oid_index == g_paster_oid_index_copy )
         {
            //paster_error( PASTER_ERROR_FIND_COPY_DEST_FILE );
            break;
         }
         g_paster_state = PASTER_STATE_COPY_START;
         break;
      }
      paster_reset_cmd();
      break;
    
      case PASTER_STATE_COPY_START:
      if( !file_aux_paste_copy())
      {
         paster_error( PASTER_ERROR_COPY_START_FAILED );
         break;
      }
      paster_back_to_top_dir();
      if( g_b_disk_is_full )
      {
         paster_error( PASTER_ERROR_DISK_IS_FULL );
         break;
      }
      if( !paster_record_find( g_paster_oid_index_copy ))
      {
         paster_error( PASTER_ERROR_FIND_FILE_FAILED );
         break;
      }
      if( !file_aux_paste_start())
      {
         paster_error( PASTER_ERROR_COPY_START_FAILED );
         break;
      }
      g_paster_state = PASTER_STATE_COPY;
      break;
      
      case PASTER_STATE_COPY:
      g_paster_cmd_id = paster_translate_cmd();
      switch( g_paster_cmd_id )
      {
         case CMD_PASTER_BACK:
         case CMD_PASTER_STOP:
         file_aux_paste( TRUE );
         paster_error( PASTER_ERROR_COPY_ABORT );
         break;
      
         case CMD_PASTER_END:
         file_aux_paste( TRUE );
         paster_goto_error( PASTER_ERROR_COPY_ABORT, PASTER_STATE_END );
         break;
         
         default:
         {
         U8 result;
         result = file_aux_paste( FALSE );
         mail_send_event( EVT_PASTER_COPY_PROC, file_aux_copy_proccess());
         if( result == COPY_FINISH )
         {
            g_paster_state = PASTER_STATE_COPY_END;
         }
         else if( result == COPY_FAIL )
         {
            if( g_b_disk_is_full )
               paster_error( PASTER_ERROR_DISK_IS_FULL );
            else
               paster_error( PASTER_ERROR_COPY_ABORT );
         }
         }
         break;
      }
      paster_reset_cmd();
      break;
      
      case PASTER_STATE_COPY_END:
      file_close();
      paster_goto_cue( CUE_INDEX_COPY_END, PASTER_STATE_CHECK );
      break;
#endif

////////////////////////////////////////////////////////////////////////////////				
      case PASTER_STATE_ERROR:
      //cue_off();
      //mail_send_event( EVT_PASTER_ERROR, g_paster_error );
      paster_goto_cue( g_paster_error, PASTER_STATE_ERROR_WAIT );
      //g_paster_state = PASTER_STATE_ERROR_WAIT;
      break;

      case PASTER_STATE_ERROR_WAIT:
      //if( g_paster_cmd_id == CMD_PASTER_BACK )
      {
         g_paster_state = g_paster_error_next_state;
      }
      //paster_reset_cmd();
      break;

      case PASTER_STATE_END:
      //cue_off();
      player_off();
      //file_aux_free_cluster_end();
      //g_paster_state = PASTER_STATE_IDLE;
      g_paster_state = PASTER_STATE_NULL;
      break;
   }
   if( g_paster_last_state != g_paster_state )
   {
      MSB( param ) = g_paster_last_state;
      LSB( param ) = g_paster_state;
      mail_send_event( EVT_PASTER_STATE_CHANGE, param );
      g_paster_last_state = g_paster_state;
   }
}


#pragma CODE = PASTER_STATE_CHECK_CODE

void paster_state_check( void )
{
   SDKLoadCode( PASTER_CMD_CODE );
   g_paster_cmd_id = paster_translate_cmd();

   SDKLoadCode( CHECK_STATE_CMD_CODE );
   check_state_oid_cmd_deal();
#if 0
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_RECORD:
      g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
      g_paster_state = PASTER_STATE_RECORD_START;
      break;
   
      case CMD_PASTER_PLAY:
      g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
      g_paster_state = PASTER_STATE_PLAY_START;
      break;
   
      /*case CMD_PASTER_COPY:
      paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_SRC_OID );
      break;*/
   
      //case CMD_PASTER_STOP:
      case CMD_PASTER_END:
      //case CMD_PASTER_BACK:
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      //g_paster_state = PASTER_STATE_END;
      break;
   
      /*case CMD_PASTER_POWER_LOW:
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
      break;*/

      case CMD_PASTER_VOL_UP:
      case CMD_PASTER_VOL_DOWN:
      //g_paster_sound_old_pos = nav_getindex();
      //g_paster_sound_name = g_paster_sound_vol;
      g_paster_sound_next_state = PASTER_STATE_CHECK;
      g_paster_sound_index = CUE_INDEX_VOLUME;
      g_paster_state = PASTER_STATE_SOUND_START;
      break;
   
      case CMD_PASTER_POWER_OFF:
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
      break;
   }
   paster_reset_cmd();
#endif
}


#pragma CODE = CHECK_STATE_CMD_CODE

void check_state_oid_cmd_deal( void )
{
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_RECORD:
      //g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
      g_paster_state = PASTER_STATE_RECORD_START;
      break;
   
      case CMD_PASTER_PLAY:
      //g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
      g_paster_state = PASTER_STATE_PLAY_START;
      break;
   
      /*case CMD_PASTER_COPY:
      paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_SRC_OID );
      break;*/
   
      //case CMD_PASTER_STOP:
      case CMD_PASTER_END:
      //case CMD_PASTER_BACK:
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      //g_paster_state = PASTER_STATE_END;
      break;
   
      /*case CMD_PASTER_POWER_LOW:
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
      break;*/

      case CMD_PASTER_VOL_UP:
      case CMD_PASTER_VOL_DOWN:
      //g_paster_sound_old_pos = nav_getindex();
      //g_paster_sound_name = g_paster_sound_vol;
      g_paster_sound_next_state = PASTER_STATE_CHECK;
      g_paster_sound_index = CUE_INDEX_VOLUME;
      g_paster_state = PASTER_STATE_SOUND_START;
      break;
   
      case CMD_PASTER_POWER_OFF:
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
      break;
   }
   paster_reset_cmd();
}


#pragma CODE = PASTER_SOUND_START_CODE

void paster_state_sound_start( void )
{
   if( player_ap3_is_busy())
   {
      player_ap3_stop();
      //file_close();
   }
   
   SDKLoadCode( PASTER_TEMP_CODE03 );
   if( !paster_cue_to_sound_file( g_paster_sound_index ))
   {
      //g_paster_state = g_paster_sound_next_state;
      //return;
      goto __sound_failed;
   }

   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file(( char *)paster_sound_file_name ))
   {
      goto __sound_failed;
   }
   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
   {
      goto __sound_failed;
   }
   g_paster_state = PASTER_STATE_SOUND_WAIT;
   return;
   
__sound_failed:
   g_paster_state = g_paster_sound_next_state;
}


#pragma CODE = PASTER_SOUND_WAIT_CODE

void paster_state_sound_wait( void )
{
   if( g_paster_cmd_id == CMD_PASTER_NONE )
      return;
   SDKLoadCode( PASTER_CMD_CODE );
   g_paster_cmd_id = paster_translate_cmd();
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_RECORD:
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
         //cue_off();
         //nav_gotoindex( &g_paster_sound_old_pos );
      }
      //g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_RECORD, PASTER_STATE_RECORD_START );
      g_paster_state = PASTER_STATE_RECORD_START;
      break;

      case CMD_PASTER_PLAY:
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
         //cue_off();
         //nav_gotoindex( &g_paster_sound_old_pos );
      }
      //g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
      g_paster_state = PASTER_STATE_PLAY_START;
      break;

      case CMD_PASTER_END:
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
         //cue_off();
         //nav_gotoindex( &g_paster_sound_old_pos );
      }
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      break;

      /*case CMD_PASTER_VOL_UP:
      case CMD_PASTER_VOL_DOWN:
      //g_paster_sound_name = g_paster_sound_vol;
      //g_paster_state = PASTER_STATE_SOUND_START;
      break;*/

      case CMD_PASTER_POWER_OFF:
      if( player_ap3_is_busy())
      {
         player_ap3_stop();
         //cue_off();
         //nav_gotoindex( &g_paster_sound_old_pos );
      }
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
      break;
   }
   paster_reset_cmd();
}


#pragma CODE = PASTER_RECORD_CODE

void paster_state_record( void )
{
   if( !Timer_is_timeout( pause_time ))
      return;
   SDKLoadCode( PASTER_CMD_CODE );
   g_paster_cmd_id = paster_translate_cmd();

   SDKLoadCode( RECORD_STATE_CMD_CODE );
   record_state_oid_cmd_deal();
#if 0
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_RECORD:
      g_paster_state = PASTER_STATE_RECORD_START;
      goto paster_record_next;
      //break;

      case CMD_PASTER_PLAY:
      g_paster_state = PASTER_STATE_PLAY_START;
paster_record_next:
      paster_record_stop();
      //paster_record_file_end();
      
      //Ts_free( s_paster_display_id );
      //power_authorize_autopoweroff();
      mmi_paster_display_icon();
      mmipaster_play_time();
      
      g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
      break;
      
      case CMD_PASTER_PAUSE:
      /*g_paster_stop_next_state = PASTER_STATE_RECORD_PAUSE;
      paster_record_stop();
      //paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_PAUSE );
      paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_STOP );*/
      switch( g_recorder_state )
      {
         case MMI_RECORDER_STATE_PAUSE:
         SDKLoadCode( RECORDER_TEMP_CODE06 );
         record_restart();
         lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
         break;

         case MMI_RECORDER_STATE_RECORDING:
         SDKLoadCode( RECORDER_TEMP_CODE07 );
         record_pause();
         lcd_draw_picture( 4, 13, IDP_PLAYER_PAUSE );
         break;
      }
      break;
   
      case CMD_PASTER_STOP:
      case CMD_PASTER_RECORD_EOF:
      g_paster_stop_next_state = PASTER_STATE_CHECK;
      paster_record_stop();
      /*if( s_b_vbus_present )
         green_led_off();
      else
         red_led_off();*/
      //paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
      paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
      break;
   
      /*case CMD_PASTER_COPY:
      //paster_record_stop();
      //paster_goto_cue(CUE_INDEX_COPY,PASTER_STATE_COPY_DEST_OID);
      break;
   
      case CMD_PASTER_BACK:
      g_paster_stop_next_state = PASTER_STATE_CHECK;
      paster_record_stop();
      //paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
      paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_RECORD_STOP );
      break;*/
   
      case CMD_PASTER_END:
      g_paster_stop_next_state = PASTER_STATE_END;
      paster_record_stop();
      //paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_RECORD_STOP );
      break;
   
      /*case CMD_PASTER_POWER_LOW:
      g_paster_stop_next_state = PASTER_STATE_END;
      paster_record_stop();
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_RECORD_STOP );
      break;*/
   
      case CMD_PASTER_POWER_OFF:
      g_paster_stop_next_state = PASTER_STATE_END;
      paster_record_stop();
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_RECORD_STOP );
      break;
   }
   paster_reset_cmd();
#endif
}


#pragma CODE = RECORD_STATE_CMD_CODE

void record_state_oid_cmd_deal( void )
{
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_RECORD:
      g_paster_state = PASTER_STATE_RECORD_START;
      goto paster_record_next;
      //break;

      case CMD_PASTER_PLAY:
      g_paster_state = PASTER_STATE_PLAY_START;
paster_record_next:
      paster_record_stop();
      //paster_record_file_end();
      
      //Ts_free( s_paster_display_id );
      //power_authorize_autopoweroff();
      mmi_paster_display_icon();
      mmipaster_play_time();
      
      //g_paster_oid_index = g_paster_cmd_param;
      //paster_goto_cue( CUE_INDEX_PLAY, PASTER_STATE_PLAY_START );
      break;
      
      case CMD_PASTER_PAUSE:
      /*g_paster_stop_next_state = PASTER_STATE_RECORD_PAUSE;
      paster_record_stop();
      //paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_PAUSE );
      paster_goto_cue( CUE_INDEX_RECORD_PAUSE, PASTER_STATE_RECORD_STOP );*/
      switch( g_recorder_state )
      {
         case MMI_RECORDER_STATE_PAUSE:
         SDKLoadCode( RECORDER_TEMP_CODE06 );
         record_restart();
         lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
         break;

         case MMI_RECORDER_STATE_RECORDING:
         SDKLoadCode( RECORDER_TEMP_CODE07 );
         record_pause();
         lcd_draw_picture( 4, 13, IDP_PLAYER_PAUSE );
         break;
      }
      break;
   
      case CMD_PASTER_STOP:
      case CMD_PASTER_RECORD_EOF:
      g_paster_stop_next_state = PASTER_STATE_CHECK;
      paster_record_stop();
      /*if( s_b_vbus_present )
         green_led_off();
      else
         red_led_off();*/
      //paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
      paster_goto_cue( CUE_INDEX_RECORD_END, PASTER_STATE_RECORD_STOP );
      break;
   
      /*case CMD_PASTER_COPY:
      //paster_record_stop();
      //paster_goto_cue(CUE_INDEX_COPY,PASTER_STATE_COPY_DEST_OID);
      break;
   
      case CMD_PASTER_BACK:
      g_paster_stop_next_state = PASTER_STATE_CHECK;
      paster_record_stop();
      //paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
      paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_RECORD_STOP );
      break;*/
   
      case CMD_PASTER_END:
      //g_paster_stop_next_state = PASTER_STATE_END;
      //paster_record_stop();
      //paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_RECORD_STOP );
      goto paster_record_next1;
      //break;
   
      /*case CMD_PASTER_POWER_LOW:
      g_paster_stop_next_state = PASTER_STATE_END;
      paster_record_stop();
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_RECORD_STOP );
      break;*/
   
      case CMD_PASTER_POWER_OFF:
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_RECORD_STOP );
paster_record_next1:
      g_paster_stop_next_state = PASTER_STATE_END;
      paster_record_stop();
      break;
   }
   paster_reset_cmd();
}


#pragma CODE = PASTER_PLAY_CODE

void paster_state_play( void )
{
   SDKLoadCode( PASTER_CMD_CODE );
   g_paster_cmd_id = paster_translate_cmd();

   SDKLoadCode( PLAY_STATE_CMD_CODE );
   play_state_oid_cmd_deal();
#if 0
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_PAUSE:
      //g_paster_last_time = player_get_time();
      //g_paster_last_pos = paster_play_stop();
      //paster_goto_cue( CUE_INDEX_PLAY_PAUSE, PASTER_STATE_PLAY_PAUSE );
      break;
   
      case CMD_PASTER_STOP:
      paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
paster_play_next:
      if( player_ap3_is_busy())
         paster_play_stop();
paster_play_next1:
      srvplayer_g_arg.start_time.hours   = 0;
      srvplayer_g_arg.start_time.minutes = 0;
      srvplayer_g_arg.start_time.seconds = 0;
      Ts_free( mmipaster_play_id_timerdisplay );
      mmi_paster_display_icon();
      mmipaster_play_time();
      break;

      case CMD_PASTER_PLAY_EOF:
      //paster_play_stop();
      //paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
      g_paster_state = PASTER_STATE_PLAY_STOP;
      goto paster_play_next1;
      break;
   
      /*case CMD_PASTER_COPY:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_DEST_OID );
      break;
   
      case CMD_PASTER_BACK:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
      break;*/
   
      case CMD_PASTER_END:
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      goto paster_play_next;
      break;
   
      /*case CMD_PASTER_POWER_LOW:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
      break;*/
   
      case CMD_PASTER_POWER_OFF:
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
      goto paster_play_next;
      break;

      case CMD_PASTER_PLAY:
      g_paster_state = PASTER_STATE_PLAY_START;
      g_paster_oid_index = g_paster_cmd_param;
      goto paster_play_next;
      break;

      case CMD_PASTER_RECORD:
      g_paster_state = PASTER_STATE_RECORD_START;
      g_paster_oid_index = g_paster_cmd_param;
      goto paster_play_next;
   }
   paster_reset_cmd();
#endif
}


#pragma CODE = PLAY_STATE_CMD_CODE

void play_state_oid_cmd_deal( void )
{
   switch( g_paster_cmd_id )
   {

#if 1
      case CMD_PASTER_PAUSE:
      //g_paster_last_time = player_get_time();
      //g_paster_last_pos = paster_play_stop();
      //paster_goto_cue( CUE_INDEX_PLAY_PAUSE, PASTER_STATE_PLAY_PAUSE );
      if( g_b_player_on )
      {
         switch( srvplayer_g_player_state )
         {
            case MMI_PLAYER_STATE_PLAY :
            SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
            ( void )srvplayer_pause();
            mmi_paster_display_icon();
            power_authorize_autopoweroff();

            //if( mmipaster_play_id_timerdisplay != UNDEFINED_TIMER )
            //   ts_stop( mmipaster_play_id_timerdisplay );
            break;

            case MMI_PLAYER_STATE_PAUSE :
            SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
            ( void )srvplayer_restart_play();
            mmi_paster_display_icon();
            power_forbid_autopoweroff();

            //if( mmipaster_play_id_timerdisplay != UNDEFINED_TIMER )
            //   ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
            break;
         }
      }
      break;
#endif
   
      case CMD_PASTER_STOP:
      paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
paster_play_next:
      if( player_ap3_is_busy())
         paster_play_stop();
paster_play_next1:
      srvplayer_g_arg.start_time.hours   = 0;
      srvplayer_g_arg.start_time.minutes = 0;
      srvplayer_g_arg.start_time.seconds = 0;
      Ts_free( mmipaster_play_id_timerdisplay );
      mmi_paster_display_icon();
      mmipaster_play_time();
      break;

      case CMD_PASTER_PLAY_EOF:
      //paster_play_stop();
      //paster_goto_cue( CUE_INDEX_PLAY_END, PASTER_STATE_PLAY_STOP );
      g_paster_state = PASTER_STATE_PLAY_STOP;
      goto paster_play_next1;
      //break;
   
      /*case CMD_PASTER_COPY:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_COPY, PASTER_STATE_COPY_DEST_OID );
      break;
   
      case CMD_PASTER_BACK:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_BACK, PASTER_STATE_CHECK );
      break;*/
   
      case CMD_PASTER_END:
      paster_goto_cue( CUE_INDEX_END, PASTER_STATE_END );
      goto paster_play_next;
      //break;
   
      /*case CMD_PASTER_POWER_LOW:
      paster_play_stop();
      paster_goto_cue( CUE_INDEX_POWER_LOW, PASTER_STATE_END );
      break;*/
   
      case CMD_PASTER_POWER_OFF:
      paster_goto_cue( CUE_INDEX_POWER_OFF, PASTER_STATE_END );
      goto paster_play_next;
      //break;

      case CMD_PASTER_PLAY:
      g_paster_state = PASTER_STATE_PLAY_START;
      //g_paster_oid_index = g_paster_cmd_param;
      goto paster_play_next;
      //break;

      case CMD_PASTER_RECORD:
      g_paster_state = PASTER_STATE_RECORD_START;
      //g_paster_oid_index = g_paster_cmd_param;
      goto paster_play_next;
      //break;
   }
   paster_reset_cmd();
}


#pragma CODE = PASTER_RECORD_READY_CODE

void paster_record_ready( void )
{
   SDKLoadCode( PASTER_BASE_TEMP_CODE02 );
   SendString((U8 *)"record\r\n");
   if( !paster_record_start())
   {
      SDKLoadCode( PASTER_TEMP_CODE05 );
      paster_record_fail_display();
      return;
   }
   record_result = RECORD_OK;
   lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
   s_paster_display_id = ts_alloc();
   ts_set_time( s_paster_display_id, PASTER_RECORD_TIME );
   
   green_led_stop_timer();
   red_led_stop_timer();
   red_led_on();
   green_led_on();
   pause_time = Timer_set_timer( TIMER_S( 1 ));
   g_paster_state = PASTER_STATE_RECORD;
}


#pragma CODE = PASTER_TEMP_CODE00

Bool paster_init_disk( void )
{
   lcd_goto_xy( 46, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)" WAIT " );
   
   SDKLoadCode( NAVIGATION_TEMP_CODE05 );
   SDKLoadCode( NAVIGATION_CODE );
   file_init();
   if( !oid_cmd_init())
      return FALSE;

   apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
   scan_oid_enable();
   mmi_paster_kbd_enable();
   //g_paster_state = PASTER_STATE_START;
   if( !s_paster_cue )
   {
      s_paster_cue = 1;
      paster_goto_cue( CUE_INDEX_PASTER_POWER_ON, PASTER_STATE_START );
   }
   else
      paster_goto_cue( CUE_INDEX_PASTER_START, PASTER_STATE_START );

   lcd_goto_xy( 46, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"PASTER" );
   return TRUE;
}


//#pragma CODE = PASTER_CODE
#pragma CODE = PASTER_CMD_CODE

Bool  oid_cmd_normal_flag;

U8 paster_translate_cmd( void )
{
   U8 new_cmd;

   SDKLoadCode( PASTER_CMD_OID_TEMP_CODE00 );
   new_cmd = paster_oid_cmd_funtion();

   if( oid_cmd_normal_flag )
   {
      SDKLoadCode( PASTER_CMD_OID_TEMP_CODE01 );
      new_cmd = paster_oid_cmd_normal();
   }
#if 0
   U8 oid_cmd;
   U8 new_cmd;
   new_cmd = g_paster_cmd_id;
   //switch( g_paster_cmd_id )
   if( new_cmd == CMD_PASTER_OID )
   {
      //case CMD_PASTER_OID:
      oid_cmd = paster_trans_oid_cmd( g_paster_cmd_param );
      //if( oid_cmd == CUE_INDEX_NUM )
         //break;
      switch( oid_cmd )
      {
         case PASTER_OID_CMD_STOP:
         new_cmd = CMD_PASTER_STOP;
         break;
         
         case PASTER_OID_CMD_RECORD:
         //new_cmd = CMD_PASTER_RECORD;
         //break;
      
         case PASTER_OID_CMD_PLAY:
         //new_cmd = CMD_PASTER_PLAY;
         break;
      
         case PASTER_OID_CMD_PAUSE:
         new_cmd = CMD_PASTER_PAUSE;
         break;
      
         case PASTER_OID_CMD_COPY:
         //new_cmd = CMD_PASTER_COPY;
         //break;
      
         case PASTER_OID_CMD_BACK:
         //new_cmd = CMD_PASTER_BACK;
         break;
      
         default:
         //if(g_paster_cmd_param>0&&g_paster_cmd_param<=OID_INDEX_DIVISION)
         if(( g_paster_cmd_param >= cue_get_oid_index_start())
            &&( g_paster_cmd_param <( cue_get_oid_index_start()+ cue_get_oid_index_division())))
         {
            new_cmd = CMD_PASTER_RECORD;
         }
         //else	if(g_paster_cmd_param>OID_INDEX_DIVISION&&g_paster_cmd_param<=2*OID_INDEX_DIVISION)
         else if(( g_paster_cmd_param >=( cue_get_oid_index_start()+ cue_get_oid_index_division()))
                  &&( g_paster_cmd_param <( cue_get_oid_index_start()+ 2 * cue_get_oid_index_division())))
         {
            new_cmd = CMD_PASTER_PLAY;
            //g_paster_cmd_param -= OID_INDEX_DIVISION;
            g_paster_cmd_param -= cue_get_oid_index_division();
         }
         break;
      }
      //break;
   }
#endif

   g_paster_oid_index = g_paster_cmd_param;
   return new_cmd;
}


#pragma CODE = PASTER_CMD_OID_TEMP_CODE00

U8 paster_oid_cmd_funtion( void )
{
   U8 oid_cmd;
   U8 new_cmd;
   
   oid_cmd_normal_flag = FALSE;
   new_cmd = g_paster_cmd_id;
   if( new_cmd == CMD_PASTER_OID )
   {
      oid_cmd = paster_trans_oid_cmd( g_paster_cmd_param );
      //if( oid_cmd == CUE_INDEX_NUM )
         //break;
      switch( oid_cmd )
      {
         case PASTER_OID_CMD_STOP:
         new_cmd = CMD_PASTER_STOP;
         break;
         
         case PASTER_OID_CMD_RECORD:
         //new_cmd = CMD_PASTER_RECORD;
         //break;
      
         case PASTER_OID_CMD_PLAY:
         //new_cmd = CMD_PASTER_PLAY;
         break;
      
         case PASTER_OID_CMD_PAUSE:
         new_cmd = CMD_PASTER_PAUSE;
         break;
      
         case PASTER_OID_CMD_COPY:
         //new_cmd = CMD_PASTER_COPY;
         //break;
      
         case PASTER_OID_CMD_BACK:
         //new_cmd = CMD_PASTER_BACK;
         break;
      
         default:
         /*
         //if(g_paster_cmd_param>0&&g_paster_cmd_param<=OID_INDEX_DIVISION)
         if(( g_paster_cmd_param >= cue_get_oid_index_start())
            &&( g_paster_cmd_param <( cue_get_oid_index_start()+ cue_get_oid_index_division())))
         {
            new_cmd = CMD_PASTER_RECORD;
         }
         //else	if(g_paster_cmd_param>OID_INDEX_DIVISION&&g_paster_cmd_param<=2*OID_INDEX_DIVISION)
         else if(( g_paster_cmd_param >=( cue_get_oid_index_start()+ cue_get_oid_index_division()))
                  &&( g_paster_cmd_param <( cue_get_oid_index_start()+ 2 * cue_get_oid_index_division())))
         {
            new_cmd = CMD_PASTER_PLAY;
            //g_paster_cmd_param -= OID_INDEX_DIVISION;
            g_paster_cmd_param -= cue_get_oid_index_division();
         }
         */
         new_cmd = CMD_PASTER_NONE;
         oid_cmd_normal_flag = TRUE;
         break;
      }
   }
   return new_cmd;
}


#pragma CODE = PASTER_CMD_OID_TEMP_CODE01

U8 paster_oid_cmd_normal( void )
{
   //U8 new_cmd;
   
   oid_cmd_normal_flag = FALSE;
   //new_cmd = g_paster_cmd_id;

   if(( g_paster_cmd_param >= cue_get_oid_index_start())
      &&( g_paster_cmd_param <( cue_get_oid_index_start()+ cue_get_oid_index_division())))
   {
      return CMD_PASTER_RECORD;
   }
   if(( g_paster_cmd_param >=( cue_get_oid_index_start()+ cue_get_oid_index_division()))
            &&( g_paster_cmd_param <( cue_get_oid_index_start()+ 2 * cue_get_oid_index_division())))
   {
      g_paster_cmd_param -= cue_get_oid_index_division();
      return CMD_PASTER_PLAY;
   }
   return CMD_PASTER_NONE;
}




#pragma CODE = PASTER_TEMP_CODE07

//! Get file informations
//!
void paster_file_getinfos( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );

   lcd_goto_xy( 15, 8 );
   lcd_puts(( U8 *)paster_file );

   srvplayer_g_arg.start_time.hours   = 0;
   srvplayer_g_arg.start_time.minutes = 0;
   srvplayer_g_arg.start_time.seconds = 0;
   mmi_paster_display_icon();
   mmipaster_play_time();
}


#pragma CODE = PASTER_TEMP_CODE02

void paster_disk_full_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 20, 12 );
   lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"¿Õ¼ä²»×ã" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"ªÅ¶¡¤£¨¬" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"Memory full" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"¿Õ¼ä²»×ã" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"ªÅ¶¡¤£¨¬" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
   //paster_error( PASTER_ERROR_DISK_IS_FULL );
   //g_paster_state = PASTER_STATE_DISK_FULL_NOTICE;
   //g_paster_sound_old_pos = nav_getindex();
   g_paster_sound_next_state = PASTER_STATE_CHECK;
   g_paster_sound_index = CUE_INDEX_ERROR_DISK_IS_FULL;
   g_paster_state = PASTER_STATE_SOUND_START;
}


#pragma CODE = PASTER_TEMP_CODE04

void paster_error_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 16, 12 );
   lcd_puts(( U8 *)"Record error" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 12 );
   lcd_puts( "Â¼Òô´íÎó" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"¿ý­µ¿ù»~" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 16, 12 );
      lcd_puts(( U8 *)"Record error" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"Â¼Òô´íÎó" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"¿ý­µ¿ù»~" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
   paster_error( PASTER_ERROR_FIND_FILE_FAILED );
}


#pragma CODE = PASTER_TEMP_CODE05

void paster_record_fail_display( void )
{
   //paster_record_stop();
   //paster_error( PASTER_ERROR_RECORD_START_FAILED );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
   switch( record_result )
   {
      case RECORD_ERROR:
      paster_error( PASTER_ERROR_RECORD_START_FAILED );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy( 16, 12 );
      lcd_puts(( U8 *)"Record error" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"Â¼Òô´íÎó" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"¿ý­µ¿ù»~" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
         lcd_goto_xy( 16, 12 );
         lcd_puts(( U8 *)"Record error" );
         break;

         case LANG_CHINESE_SIMP:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"Â¼Òô´íÎó" );
         break;

         case LANG_CHINESE_TRAD:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"¿ý­µ¿ù»~" );
         break;
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      break;

      case RECORD_FILE_FULL:
      case RECORD_FILE_ERROR:
      case RECORD_MEMORY_FULL:
      paster_error( PASTER_ERROR_DISK_IS_FULL );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"¿Õ¼ä²»×ã" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"ªÅ¶¡¤£¨¬" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
         lcd_goto_xy( 20, 12 );
         lcd_puts(( U8 *)"Memory full" );
         break;

         case LANG_CHINESE_SIMP:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"¿Õ¼ä²»×ã" );
         break;

         case LANG_CHINESE_TRAD:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"ªÅ¶¡¤£¨¬" );
         break;
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      break;
   }
}


#pragma CODE = PASTER_TEMP_CODE06

void paster_nofile_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 36, 12 );
   lcd_puts(( U8 *)"No file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 40, 12 );
   lcd_puts(( U8 *)"ÎÞÎÄ¼þ" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 40, 12 );
   lcd_puts(( U8 *)"µL¤å¥ó" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 36, 12 );
      lcd_puts(( U8 *)"No file" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 40, 12 );
      lcd_puts(( U8 *)"ÎÞÎÄ¼þ" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 40, 12 );
      lcd_puts(( U8 *)"µL¤å¥ó" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )

   paster_goto_cue( CUE_INDEX_ERROR_FIND_FILE_FAILED, PASTER_STATE_CHECK );
}


#pragma CODE = PASTER_IDLE_CODE

void paster_idle_deal( void )
{
   switch( g_paster_cmd_id )
   {
      case CMD_PASTER_START:
      SDKLoadCode( PASTER_TEMP_CODE00 );
      if( !paster_init_disk())
      {
         mail_send_event( EVT_PASTER_NO_APF_FILE, 0 );
      }
      break;
   }
   paster_reset_cmd();
}


#pragma CODE = PASTER_CUE_PLAY_CODE

void cue_play_apf_deal( void )
{
   SDKLoadCode( PLAYER_AP3_TEMP_CODE00 );
   if( !cue_play( g_paster_cue_index ))
   {
      //paster_error( PASTER_ERROR_CUE_ERROR );
      //g_paster_state = g_paster_cue_next_state;
      //g_paster_sound_old_pos = nav_getindex();
      g_paster_sound_next_state = g_paster_cue_next_state;
      g_paster_sound_index = g_paster_cue_index;
      g_paster_state = PASTER_STATE_SOUND_START;
      return;
   }
   g_paster_state = PASTER_STATE_CUE_PLAY;
}


#pragma CODE = PASTER_RECORD_STOP_CODE

void paster_state_record_stop( void )
{
   //g_paster_state = PASTER_STATE_CHECK;
   //paster_record_file_end();
   g_paster_state = g_paster_stop_next_state;
   
   //Ts_free( s_paster_display_id );
   //power_authorize_autopoweroff();
   mmi_paster_display_icon();
   mmipaster_play_time();

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 0, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );
}


#pragma CODE = PASTER_PLAY_STOP_CODE

void paster_state_play_stop( void )
{
   g_paster_state = PASTER_STATE_CHECK;

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   lcd_goto_xy( 0, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );
}


#pragma CODE = PASTER_RECORD_PREPARE_CODE

void paster_record_prepare_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
   if( g_b_battery_low )
   {
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy(( U8 *)20, 12 );
      lcd_puts( "Low voltage" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"µçÁ¿²»×ã!" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"¹q¶q¤£¨¬!" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
         lcd_goto_xy( 20, 12 );
         lcd_puts(( U8 *)"Low voltage" );
         break;

         case LANG_CHINESE_SIMP:
         lcd_goto_xy( 28, 12 );
         lcd_puts(( U8 *)"µçÁ¿²»×ã!" );
         break;

         case LANG_CHINESE_TRAD:
         lcd_goto_xy( 28, 12 );
         lcd_puts(( U8 *)"¹q¶q¤£¨¬!" );
         break;
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      return;
   }
      
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 28, 12 );
   lcd_puts(( U8 *)"Preparing" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"×¼±¸Â¼Òô" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"­ã³Æ¿ý­µ" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"Preparing" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"×¼±¸Â¼Òô" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"­ã³Æ¿ý­µ" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#endif  //#if( PASTER == ENABLE )

#endif  //_paster_c
