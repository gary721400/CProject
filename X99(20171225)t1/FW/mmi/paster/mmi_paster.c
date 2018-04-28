//!
//! @file mmi_paster.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI paster
//!
//! @version 1.26 snd3b-dvk-1_0_33 $Id: mmi_paster.c,v 1.26 2006/12/08 16:22:06 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_paster_c_
#define _mmi_paster_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include <stdio.h>
#include <string.h>

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "modules\player\player.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "modules\control_access\ctrl_access.h"
#include "conf\conf_mmi.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "mmi\kbd\keypad.h"

#include "lib_system\unicode\unicode.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "modules\player_ap3\player_ap3.h"
#include "modules\audio\audio.h"
#include "modules\recorder\recorder.h"
#include "mmi\recorder\srv_recorder.h"
#include "modules\cue\cue.h"

#include "drivers\oid\oid.h"

#include "mmi_paster.h"
#include "modules\paster\paster.h"
#include "modules\paster\paster_task.h"
#include "modules\paster\paster_base.h"
#include "mmi\status\mmi_status.h"
#include "conf\conf_sio.h"
#include "mmi\player\srv_player.h"
#include "modules\search\search_remote.h"
#include "modules\search\search.h"
#include "modules\power\power.h"             // Power module definition
#include "sio\com.h"
#include "mmi\startup\mmi_startup.h"

#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "mmi\status\mmi_status.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\file_aux\file_aux.h"


#pragma DATA = MMI_PASTER_DATA
#pragma CODE = MMI_PASTER_CODE

ImageDeclareType( MMI_PASTER_START_CODE );
ImageDeclareType( MMI_PASTER_BATLOW_CODE );
ImageDeclareType( MMI_PASTER_USB_CODE );

ImageDeclareType( MMI_PASTER_TEMP_CODE00 );
ImageDeclareType( MMI_PASTER_TEMP_CODE01 );
ImageDeclareType( MMI_PASTER_TEMP_CODE02 );
ImageDeclareType( MMI_PASTER_TEMP_CODE03 );
ImageDeclareType( MMI_PASTER_TEMP_CODE04 );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( PASTER_TEMP_CODE00 );
ImageDeclareType( PASTER_TEMP_CODE01 );
ImageDeclareType( PASTER_TEMP_CODE02 );
ImageDeclareType( PASTER_TEMP_CODE03 );
ImageDeclareType( PASTER_TEMP_CODE04 );
ImageDeclareType( PASTER_TEMP_CODE05 );
ImageDeclareType( PASTER_TEMP_CODE06 );

ImageDeclareType( RECORDER_TEMP_CODE00 );
ImageDeclareType( RECORDER_TEMP_CODE01 );
ImageDeclareType( RECORDER_TEMP_CODE02 );
ImageDeclareType( RECORDER_TEMP_CODE03 );
ImageDeclareType( RECORDER_TEMP_CODE04 );
ImageDeclareType( RECORDER_TEMP_CODE06 );
ImageDeclareType( RECORDER_TEMP_CODE07 );
ImageDeclareType( AUADC_COMMON_CODE );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE07 );
ImageDeclareType( KEY_LONG_PRESS_CODE );
ImageDeclareType( KEY_PRESS_CODE );
ImageDeclareType( POWER_KEY_PRESS_CODE );
ImageDeclareType( MENU_KEY_PRESS_CODE );
ImageDeclareType( VOL_UP_KEY_PRESS_CODE );
ImageDeclareType( VOL_DOWN_KEY_PRESS_CODE );

ImageDeclareType( PASTER_EVT_REMOTE_CODE );
ImageDeclareType( MMI_PASTER_INIT_CODE );
ImageDeclareType( MMI_PASTER_KEY_PRESS_CODE );
ImageDeclareType( MMI_PASTER_EXIT_CODE );
ImageDeclareType( MMI_PASTER_POWEROFF_CODE );

ImageDeclareType( OID_TEMP_CODE00 );



#if( PASTER == ENABLE )

//_____ P R I V A T E   D E F I N I T I O N S _____________________________
// Icon for each player state
_CONST_TYPE_ U32 mmipaster_state_icon[] =
{
   IDP_PLAYER_STOP
,  IDP_PLAYER_PLAY
,  IDP_PLAYER_PAUSE
,  IDP_PLAYER_FFW
,  IDP_PLAYER_FRW
,  IDP_PLAYER_MARK_A
,  IDP_PLAYER_MARK_AB
,  IDP_PLAYER_MARK_AB
};

//_____ D E F I N I T I O N S ______________________________________________
// ID of the timer soft used by this MMI appli 
static _MEM_TYPE_SLOW_ U8 s_ts_id;

extern   Bool                    g_b_recorder_on;
extern   Bool                    g_b_player_on;

enum
{
   PASTER_EXIT,
   PASTER_POWER_OFF,
   PASTER_POWER_LOW
};

         _MEM_TYPE_SLOW_   U8    paster_exit_status;
//extern                     Bool  g_b_search_record;


typedef enum
{
   APF_PLAY_MODE
,  APF_PLAY_NO_FILE
,  APF_PLAY_STARTUP
,  APF_PLAY_ERROR
}PASTER_STATE;
_MEM_TYPE_MEDSLOW_   U8    apf_play_state /*= APF_PLAY_ERROR*/;

                     Bool  b_kbd_process_disable;

Bool  mmi_paster_playintro          ( void );
Bool  mmi_pasterno_file_playintro   ( void );
void  paster_no_apf_file_deal       ( void );
void  mmi_paster_player_eof_deal    ( void );
void  paster_start_appli_deal       ( void );
void  paster_bat_low_deal           ( void );
void  paster_usb_deal               ( void );
void  key_long_press_deal           ( U8 keyID );
void  key_press_deal                ( U8 keyID );
void  paster_evt_remote             ( U8 param );
void  paster_appli_init_deal        ( void );
Bool  mmi_paster_key_press_deal     ( U16 param );
void  mmi_paster_exit_deal          ( void );
void  paster_poweroff_deal          ( void );

#if( LCD_PRESENT == ENABLE )
void  mmi_paster_display_volume     ( void );
void  mmi_paster_display_backgroung ( void );
void  mmipaster_display_time        ( void );

#else
#define mmi_paster_display_volume()
#define mmi_paster_display_backgroung()
#define mmipaster_display_time()
#endif   //#if( LCD_PRESENT == ENABLE )

char  *convert_U8                   ( char *p, U8 i );

void  power_key_press               ( void );
void  menu_key_press                ( void );
void  vol_up_key_press              ( void );
void  vol_down_key_press            ( void );

//!
//! template_mmi_appli
//!
//! Manage the application template 
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void paster_mmi_appli( U8 event, U16 param )
{
   //FS_ERR error;
   switch( event )
   {
      case EVT_START_APPLI:
      SDKLoadCode( MMI_PASTER_START_CODE );
      paster_start_appli_deal();
#if 0
      SendString((U8 *)"paster\r\n");
      mmgr_set_appli_full_screen();
      SDKLoadCode( PASTER_TEMP_CODE01 );
      paster_evt_init();
      SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
      unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
      Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));

      s_paster_display_id              = UNDEFINED_TIMER;
      mmipaster_play_id_timerdisplay   = UNDEFINED_TIMER;
      s_ts_id                          = UNDEFINED_TIMER;

      srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
      SDKLoadCode( MMI_PASTER_TEMP_CODE03 );
      mmi_paster_display_backgroung();

#if( OID == ENABLE )
      //b_oid_init_flag = FALSE;
      if( !b_oid_init_flag )
      {
         reinit_oid_pen();
      }
#endif   //#if( OID == ENABLE )

      Kbd_set_evt_enabled();
      if( Kbd_is_event_triggered())
         Kbd_set_evt_disabled();
      
      scan_oid_disable();
      mmi_paster_kbd_disable();
      //g_b_search_record = FALSE;
      //mmi_file_flag &= ~APF_NO_FILE_FLAG;
#if( DIC == DISABLE )
      //mmi_file_flag |= ADB_NO_FILE_FLAG;
#endif   //#if( DIC == DISABLE )
      apf_play_state = APF_PLAY_ERROR;
      SDKLoadCode( MMI_PASTER_TEMP_CODE00 );
      if( !mmi_paster_playintro())
      {
         mail_send_event( EVT_START_PASTER_APPLI, 0 );
      }
      //else
      //   apf_play_state = APF_PLAY_MODE;
#endif
      break;

      case EVT_START_PASTER_APPLI:
      SDKLoadCode( MMI_PASTER_INIT_CODE );
      paster_appli_init_deal();
#if 0
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
      power_authorize_autopoweroff();
      //srvplayer_volume_send();
      
      paster_task_start();
      //apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
      paster_send_cmd( CMD_PASTER_START, 0 );
      //g_b_search_record = FALSE;
#endif
      break;

      /*case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;*/

      case EVT_RECORDER:
      //if( param == RECORD_EOF )
      {
         record_result = RECORD_MEMORY_FULL;
         paster_send_cmd( CMD_PASTER_RECORD_EOF, 0 );
      }
      //power_authorize_autopoweroff();
      break;
      
      case EVT_PLAYER:
      SDKLoadCode( MMI_PASTER_TEMP_CODE02 );
      mmi_paster_player_eof_deal();
#if 0
      //if( param == PLAYER_EOF )
      {
         if( apf_play_state != APF_PLAY_ERROR )
         {
            power_authorize_autopoweroff();
            switch( apf_play_state )
            {
               case APF_PLAY_MODE:
               apf_play_state = APF_PLAY_ERROR;
               //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               //srvplayer_switch_off();  // Power off player
               mail_send_event( EVT_START_PASTER_APPLI, 0 );
               break;

               case APF_PLAY_NO_FILE:
               apf_play_state = APF_PLAY_ERROR;
               //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               //srvplayer_switch_off();  // Power off player
               //if( player_ap3_is_busy())
               //   player_ap3_stop();
               //file_close();
               paster_task_stop();
               scan_oid_disable();
               if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
                  ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
               {
                  if( power_is_vbus_present())
                     g_select_appli = IDLE_APPLI;
                  else
                  {
                     g_power_off_mode = POWER_OFF;
                     g_select_appli = POWER_OFF_APPLI;
                  }
               }
               else
               {
#if( DIC == DISABLE )
                  g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
                  g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
               }
               Mmgr_kill_this_mmi_appli();
               break;
               
               default:
               apf_play_state = APF_PLAY_ERROR;
               break;
            }
         }
         else
            paster_send_cmd( CMD_PASTER_PLAY_EOF, 0 );
      }
#endif
      break;
      
      case EVT_KEY_PRESSED:
      SDKLoadCode( MMI_PASTER_KEY_PRESS_CODE );
      if( mmi_paster_key_press_deal( param ))
         break;
#if 0
#if( LCD_PRESENT == ENABLE )
#  if(BACKLIGHT == ENABLE)
      if( g_mmi_lcd_backlight_off )
      {
         if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
            g_mmi_lcd_backlight_off = FALSE;
         break;
      }
#  endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
      if( apf_play_state != APF_PLAY_ERROR )
      {
         if(( KBD_KEY_PRESSED | KBD_KEY_LONGPRESSED | KBD_KEY_REPEAT | KBD_KEY_REPEAT_RELEASED )
            & KEY_STATUS( param ))
         {
            break;
         }
         if( KBD_KEY_RELEASED & KEY_STATUS( param ))
         {
            if(( KEY_ID( param )!= KEY_VOL_UP )
               &&( KEY_ID( param )!= KEY_VOL_DOWN ))
               break;
         }
      }
      else
      {
         if( mmi_paster_kbd_is_disable())
            break;
      }
#endif
      if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         SDKLoadCode( KEY_PRESS_CODE );
         key_press_deal( KEY_ID( param ));
#if 0
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
            /*if( g_b_recorder_on )
               paster_send_cmd( CMD_PASTER_STOP, 0 );
            break;*/
            
            case KEY_MODE:
            if( g_b_recorder_on )
               paster_send_cmd( CMD_PASTER_STOP, 0 );
            break;

            case KEY_MEN:
            if( g_b_recorder_on )
               paster_send_cmd( CMD_PASTER_STOP, 0 );
            else
            {
               /*if( g_b_recorder_on )
               {
                  srvrec_stop();
                  Ts_free( s_paster_display_id );
                  power_authorize_autopoweroff();
               }*/
               if( player_ap3_is_busy())
                  player_ap3_stop();
               //cue_off();
               SDKLoadCode( FILE_TEMP_CODE01 );
               file_close();
               paster_task_stop();
               scan_oid_disable();
               Ts_free( mmipaster_play_id_timerdisplay );
               //g_select_appli = g_old_select_appli = RECORDER_APPLI;
               Mmgr_kill_this_mmi_appli();
               //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
               g_select_appli = RECORDER_APPLI;
               recorder_mode = 1;
            }
            break;

            case KEY_VOL_UP:
            if( g_b_recorder_on )
               paster_send_cmd( CMD_PASTER_STOP, 0 );
            else
            {
               srvplayer_volume_change( SRVPLAYER_VOL_UP );
               mmi_paster_display_volume();
               if( apf_play_state == APF_PLAY_ERROR )
                  paster_send_cmd( CMD_PASTER_VOL_DOWN, 0 );
            }
            break;

            case KEY_VOL_DOWN:
            if( g_b_recorder_on )
               paster_send_cmd( CMD_PASTER_STOP, 0 );
            else
            {
               srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
               if( apf_play_state == APF_PLAY_ERROR )
                  paster_send_cmd( CMD_PASTER_VOL_UP, 0 );
               mmi_paster_display_volume();
            }
            break;
         }
#endif
      }
      else if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         SDKLoadCode( KEY_LONG_PRESS_CODE );
         key_long_press_deal( KEY_ID( param ));
#if 0
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
            if( power_is_vbus_present())
            {
               //if( Usb_connect )
                  mail_send_event( EVT_USB_POWERED, 0 );
               break;
            }
            /*if( player_ap3_is_busy())
               player_ap3_stop();
            file_close();
            player_off();
            paster_task_stop();
            Mmgr_kill_this_mmi_appli();*/
            goto paster_power_off;
            /*paster_send_cmd( CMD_PASTER_POWER_OFF, 0 );
            g_select_appli = IDLE_APPLI;
            paster_exit_status = PASTER_POWER_OFF;
            g_power_off_mode = POWER_OFF;
            //mail_send_command( CMD_POWER_OFF, 0 );
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            break;*/

            /*case KEY_MEN:
            break;*/

            case KEY_MODE:
            //g_select_appli = AP3_APPLI;
            //g_select_appli = RECORDER_APPLI;
            g_select_appli = SETTING_APPLI;
            //g_select_appli = DIC_APPLI;
paster_exit:
            paster_send_cmd( CMD_PASTER_END, 0 );
            paster_exit_status = PASTER_EXIT;
            break;

            case KEY_VOL_UP:
            if( g_b_recorder_on )
               break;
            apf_change_file = MMI_PASTER_KEY_NEXT_CHANGE_FILE;
            Current_apf_num++;
            if( Current_apf_num >= Total_apf_num )
            {
               Current_apf_num = 0;
            }
            goto key_next_apf_file;
#if 0
            scan_oid_disable();
            mmi_paster_kbd_disable();
            if( player_ap3_is_busy())
               player_ap3_stop();
            //cue_off();
            SDKLoadCode( FILE_TEMP_CODE01 );
            file_close();
            Ts_free( mmipaster_play_id_timerdisplay );
            SDKLoadCode( PASTER_TEMP_CODE01 );
            paster_evt_init();
            paster_send_cmd( CMD_PASTER_START, 0 );
#endif
            break;

            case KEY_VOL_DOWN:
            if( g_b_recorder_on )
               break;
            apf_change_file = MMI_PASTER_KEY_PREV_CHANGE_FILE;
            if( !Current_apf_num )
               Current_apf_num = Total_apf_num - 1;
            else
               Current_apf_num--;
key_next_apf_file:
            scan_oid_disable();
            mmi_paster_kbd_disable();
            if( player_ap3_is_busy())
               player_ap3_stop();
            //cue_off();
            SDKLoadCode( FILE_TEMP_CODE01 );
            file_close();
            Ts_free( mmipaster_play_id_timerdisplay );
            SDKLoadCode( PASTER_TEMP_CODE01 );
            paster_evt_init();
            paster_send_cmd( CMD_PASTER_START, 0 );
            break;
         }
#endif
      }
      break;

      case EVT_PASTER_STATE_CHANGE:
      if( LSB( param ) == PASTER_STATE_END )
      {
         SDKLoadCode( MMI_PASTER_EXIT_CODE );
         mmi_paster_exit_deal();
#if 0
         if( player_ap3_is_busy())
            player_ap3_stop();
         SDKLoadCode( FILE_TEMP_CODE01 );
         file_close();
         paster_task_stop();
         scan_oid_disable();
         Ts_free( mmipaster_play_id_timerdisplay );
         Mmgr_kill_this_mmi_appli();
         if(( paster_exit_status == PASTER_POWER_OFF )
	  	      ||( paster_exit_status == PASTER_POWER_LOW ))
         {
            //mail_send_command( CMD_POWER_OFF, 0 );
            g_select_appli = POWER_OFF_APPLI;
         }
#endif
      }
      break;
    
      /*case EVT_PASTER_COPY_PROC:
      break;
    
      case EVT_PASTER_ERROR:
      break;*/

      case EVT_BAT_WARING:
      if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
      break;
      
      case EVT_ALERT_BATTERY_LOW:
      SDKLoadCode( MMI_PASTER_BATLOW_CODE );
      paster_bat_low_deal();
#if 0
      //g_b_battery_low = TRUE;
      if( g_b_recorder_on )
      {
         srvrec_stop();
         Ts_free( s_paster_display_id );
         //power_authorize_autopoweroff();
      }
      if( player_ap3_is_busy())
         player_ap3_stop();
      //cue_off();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      paster_task_stop();
      scan_oid_disable();
      Ts_free( mmipaster_play_id_timerdisplay );
      g_select_appli = POWER_OFF_APPLI;//IDLE_APPLI;
      g_power_off_mode = BATTERY_LOW;
      Mmgr_kill_this_mmi_appli();
      /*paster_send_cmd( CMD_PASTER_POWER_LOW, 0 );
      g_select_appli = IDLE_APPLI;
      paster_exit_status = PASTER_POWER_LOW;*/
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif
      break;

      case EVT_ENTER_IN_POWEROFF:
      SDKLoadCode( MMI_PASTER_POWEROFF_CODE );
      paster_poweroff_deal();
#if 0
paster_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      paster_send_cmd( CMD_PASTER_POWER_OFF, 0 );
      g_select_appli = IDLE_APPLI;
      paster_exit_status = PASTER_POWER_OFF;
      g_power_off_mode = POWER_OFF;
#endif
      break;
    
//***********************************************
      case EVT_USB_POWERED:
      SDKLoadCode( MMI_PASTER_USB_CODE );
      paster_usb_deal();
#if 0
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      if( g_b_recorder_on )
      {
         srvrec_stop();
         Ts_free( s_paster_display_id );
         //power_authorize_autopoweroff();
      }
      if( player_ap3_is_busy())
         player_ap3_stop();
      //cue_off();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      paster_task_stop();
      scan_oid_disable();
      Ts_free( mmipaster_play_id_timerdisplay );
      Ts_free( s_ts_id );
      //g_select_appli = MSTORAGE_APPLI;
      //Mmgr_kill_this_mmi_appli();
      //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      SDKLoadCode( AUADC_COMMON_CODE );
      AUADC_Power_Down();
      CommonLoadCode( C_USB );
#endif
      break;
//***********************************************

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      SDKLoadCode( PASTER_EVT_REMOTE_CODE );
      paster_evt_remote( LSB( param ));
#if 0
      if( apf_play_state != APF_PLAY_ERROR )
         break;
      switch( LSB( param ))
      {
         case REMOTE_VOL_UP:
         if( g_b_recorder_on )
            break;
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_paster_display_volume();
         break;
         
         case REMOTE_VOL_DOWN:
         if( g_b_recorder_on )
            break;
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_paster_display_volume();
         break;

         case REMOTE_VOL0:
         case REMOTE_VOL1:
         case REMOTE_VOL2:
         case REMOTE_VOL3:
         case REMOTE_VOL4:
         case REMOTE_VOL5:
         case REMOTE_VOL6:
         case REMOTE_VOL7:
         case REMOTE_VOL8:
         case REMOTE_VOL9:
         case REMOTE_VOL10:
         case REMOTE_VOL11:
         case REMOTE_VOL12:
         case REMOTE_VOL13:
         case REMOTE_VOL14:
         if( g_b_recorder_on )
            break;
         if(( LSB( param )- REMOTE_VOL0 )>( SRVPLAYER_VOL_LEVEL - 1 ))
            break;
         g_volume = LSB( param )- REMOTE_VOL0;
         srvplayer_volume_send();
         mmi_paster_display_volume();
         break;
         
         case REMOTE_POWER_OFF:
         if( power_is_vbus_present())
            break;
         goto paster_power_off;
         /*paster_send_cmd( CMD_PASTER_POWER_OFF, 0 );
         g_select_appli = IDLE_APPLI;
         paster_exit_status = PASTER_POWER_OFF;
         g_power_off_mode = POWER_OFF;
         g_b_remote_on = FALSE;
         break;*/
         
         /*case REMOTE_EXIT_MP3:
         case REMOTE_MP3_PLAY_PAUSE:
         case REMOTE_MP3_PREV:
         case REMOTE_MP3_NEXT:
         case REMOTE_PASTER_MODE:
         case REMOTE_REC_START:
         case REMOTE_REC_PLAY:
         case REMOTE_REC_STOP:
         break;*/

#if( REMOTE_REC == ENABLE )
         case REMOTE_REC_MODE:
         g_select_appli = RECORDER_APPLI;
         recorder_mode = 0;
         goto paster_exit;
         /*paster_send_cmd( CMD_PASTER_END, 0 );
         paster_exit_status = PASTER_EXIT;
         break;*/
#endif   //#if( REMOTE_REC == ENABLE )
         
         case REMOTE_CAL_MODE:
         bRemoteEnterCal = TRUE;
         g_select_appli = AP3_APPLI;
         goto paster_exit;
         /*paster_send_cmd( CMD_PASTER_END, 0 );
         paster_exit_status = PASTER_EXIT;
         break;*/
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         bRemoteEnterCal = FALSE;
         g_select_appli = AP3_APPLI;
         goto paster_exit;
         /*paster_send_cmd( CMD_PASTER_END, 0 );
         paster_exit_status = PASTER_EXIT;
         break;*/

#if( REMOTE_MP3 == ENABLE )
         case REMOTE_MP3_MODE:
         g_select_appli = PLAYER_APPLI;
         goto paster_exit;
         /*paster_send_cmd( CMD_PASTER_END, 0 );
         paster_exit_status = PASTER_EXIT;
         break;*/
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_DIC == ENABLE )
         case REMOTE_DIC_MODE:
         g_select_appli = DIC_APPLI;
         goto paster_exit;
         /*paster_send_cmd( CMD_PASTER_END, 0 );
         paster_exit_status = PASTER_EXIT;
         break;*/
#endif   //#if( REMOTE_DIC == ENABLE )
      }
#endif
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/
      
      case EVT_TIMER:
      if( LSB(param) == s_ts_id )
      {
         Ts_free( s_ts_id );
         //SDKLoadCode( FILE_TEMP_CODE01 );
         //file_close();
         paster_task_stop();
         scan_oid_disable();
         //Ts_free( mmipaster_play_id_timerdisplay );
         g_select_appli = IDLE_APPLI;
         Mmgr_kill_this_mmi_appli();
         break;
      }
      else if( LSB(param) == s_paster_display_id )
      {
         ts_set_time( s_paster_display_id, PASTER_RECORD_TIME );
         SDKLoadCode( MMI_PASTER_TEMP_CODE04 );
         mmipaster_display_time();
      }
      else if( LSB(param) == mmipaster_play_id_timerdisplay )
      {
         ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
         SDKLoadCode( SRV_PLAYER_TEMP_CODE07 );
         srvplayer_gettime();
         mmipaster_play_time();
      }
      else
      { 
         Mmgr_krn_forward_current_event();
      }
      break;

      case EVT_PASTER_NO_APF_FILE:
      SDKLoadCode( MMI_PASTER_TEMP_CODE01 );
      paster_no_apf_file_deal();
#if 0
      scan_oid_disable();
      mmi_paster_kbd_disable();
      apf_play_state = APF_PLAY_ERROR;
      mmi_file_flag |= APF_NO_FILE_FLAG;
      if( !mmi_pasterno_file_playintro())
      {
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //file_close();
         paster_task_stop();
         if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
            ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         {
            if( power_is_vbus_present())
               g_select_appli = IDLE_APPLI;
            else
            {
               g_power_off_mode = POWER_OFF;
               g_select_appli = POWER_OFF_APPLI;
            }
         }
         else
#if( DIC == DISABLE )
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
         Mmgr_kill_this_mmi_appli();
      }
      //else
      //   apf_play_state = APF_PLAY_NO_FILE;
#endif
      break;
      
      default:
      Mmgr_krn_forward_current_event();
      break;
   }
}


char *convert_U8( char *p, U8 i )
{
   if( i / 10 )
      p = convert_U8( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#pragma CODE = POWER_KEY_PRESS_CODE

void power_key_press( void )
{
   if( g_b_recorder_on )
      paster_send_cmd( CMD_PASTER_STOP, 0 );
}


#pragma CODE = MENU_KEY_PRESS_CODE

void menu_key_press( void )
{
   if( g_b_recorder_on )
      paster_send_cmd( CMD_PASTER_STOP, 0 );
   else
   {
      if( player_ap3_is_busy())
         player_ap3_stop();
      //cue_off();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      paster_task_stop();
      scan_oid_disable();
      Ts_free( mmipaster_play_id_timerdisplay );
      //g_select_appli = g_old_select_appli = RECORDER_APPLI;
      Mmgr_kill_this_mmi_appli();
      //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
      g_select_appli = RECORDER_APPLI;
      recorder_mode = 1;
   }
}


#pragma CODE = VOL_UP_KEY_PRESS_CODE

void vol_up_key_press( void )
{
   if( g_b_recorder_on )
      paster_send_cmd( CMD_PASTER_STOP, 0 );
   else
   {
      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      mmi_paster_display_volume();
      if( apf_play_state == APF_PLAY_ERROR )
         paster_send_cmd( CMD_PASTER_VOL_DOWN, 0 );
   }
}


#pragma CODE = VOL_DOWN_KEY_PRESS_CODE

void vol_down_key_press( void )
{
   if( g_b_recorder_on )
      paster_send_cmd( CMD_PASTER_STOP, 0 );
   else
   {
      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      if( apf_play_state == APF_PLAY_ERROR )
         paster_send_cmd( CMD_PASTER_VOL_UP, 0 );
      mmi_paster_display_volume();
   }
}


#pragma CODE = KEY_PRESS_CODE

void key_press_deal( U8 keyID )
{
   switch( keyID )
   {
      case KEY_POWER:
      /*if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
      break;*/
      
      case KEY_MODE:
      SDKLoadCode( POWER_KEY_PRESS_CODE );
      power_key_press();
#if 0
      if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
#endif
      break;

      case KEY_MEN:
      SDKLoadCode( MENU_KEY_PRESS_CODE );
      menu_key_press();
#if 0
      if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
      else
      {
         /*if( g_b_recorder_on )
         {
            srvrec_stop();
            Ts_free( s_paster_display_id );
            power_authorize_autopoweroff();
         }*/
         if( player_ap3_is_busy())
            player_ap3_stop();
         //cue_off();
         SDKLoadCode( FILE_TEMP_CODE01 );
         file_close();
         paster_task_stop();
         scan_oid_disable();
         Ts_free( mmipaster_play_id_timerdisplay );
         //g_select_appli = g_old_select_appli = RECORDER_APPLI;
         Mmgr_kill_this_mmi_appli();
         //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
         g_select_appli = RECORDER_APPLI;
         recorder_mode = 1;
      }
#endif
      break;

      case KEY_VOL_UP:
      SDKLoadCode( VOL_UP_KEY_PRESS_CODE );
      vol_up_key_press();
#if 0
      if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
      else
      {
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_paster_display_volume();
         if( apf_play_state == APF_PLAY_ERROR )
            paster_send_cmd( CMD_PASTER_VOL_DOWN, 0 );
      }
#endif
      break;

      case KEY_VOL_DOWN:
      SDKLoadCode( VOL_DOWN_KEY_PRESS_CODE );
      vol_down_key_press();
#if 0
      if( g_b_recorder_on )
         paster_send_cmd( CMD_PASTER_STOP, 0 );
      else
      {
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         if( apf_play_state == APF_PLAY_ERROR )
            paster_send_cmd( CMD_PASTER_VOL_UP, 0 );
         mmi_paster_display_volume();
      }
#endif
      break;
   }
}


#pragma CODE = KEY_LONG_PRESS_CODE

void key_long_press_deal( U8 keyID )
{
   switch( keyID )
   {
      case KEY_POWER:
      if( power_is_vbus_present())
      {
         //if( Usb_connect )
            mail_send_event( EVT_USB_POWERED, 0 );
         break;
      }
      mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
      break;

      /*case KEY_MEN:
      break;*/

      case KEY_MODE:
      //g_select_appli = AP3_APPLI;
      //g_select_appli = RECORDER_APPLI;
      g_select_appli = SETTING_APPLI;
      //g_select_appli = DIC_APPLI;
      paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;

      case KEY_VOL_UP:
      if( g_b_recorder_on )
         break;
      apf_change_file = MMI_PASTER_KEY_NEXT_CHANGE_FILE;
      Current_apf_num++;
      if( Current_apf_num >= Total_apf_num )
      {
         Current_apf_num = 0;
      }
      goto key_next_apf_file;
      //break;

      case KEY_VOL_DOWN:
      if( g_b_recorder_on )
         break;
      apf_change_file = MMI_PASTER_KEY_PREV_CHANGE_FILE;
      if( !Current_apf_num )
         Current_apf_num = Total_apf_num - 1;
      else
         Current_apf_num--;
key_next_apf_file:
      scan_oid_disable();
      mmi_paster_kbd_disable();
      if( player_ap3_is_busy())
         player_ap3_stop();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      Ts_free( mmipaster_play_id_timerdisplay );
      SDKLoadCode( PASTER_TEMP_CODE01 );
      paster_evt_init();
      paster_send_cmd( CMD_PASTER_START, 0 );
      break;
   }
}


#pragma CODE = PASTER_EVT_REMOTE_CODE

void paster_evt_remote( U8 param )
{
   if( apf_play_state != APF_PLAY_ERROR )
      return;
   switch( param )
   {
      case REMOTE_VOL_UP:
      if( g_b_recorder_on )
         break;
      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      mmi_paster_display_volume();
      break;
      
      case REMOTE_VOL_DOWN:
      if( g_b_recorder_on )
         break;
      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      mmi_paster_display_volume();
      break;

      case REMOTE_VOL0:
      case REMOTE_VOL1:
      case REMOTE_VOL2:
      case REMOTE_VOL3:
      case REMOTE_VOL4:
      case REMOTE_VOL5:
      case REMOTE_VOL6:
      case REMOTE_VOL7:
      case REMOTE_VOL8:
      case REMOTE_VOL9:
      case REMOTE_VOL10:
      case REMOTE_VOL11:
      case REMOTE_VOL12:
      case REMOTE_VOL13:
      case REMOTE_VOL14:
      if( g_b_recorder_on )
         break;
      if(( LSB( param )- REMOTE_VOL0 )>( SRVPLAYER_VOL_LEVEL - 1 ))
         break;
      g_volume = LSB( param )- REMOTE_VOL0;
      srvplayer_volume_send();
      mmi_paster_display_volume();
      break;
      
      case REMOTE_POWER_OFF:
      if( power_is_vbus_present())
         break;
      mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
      break;

      /*case REMOTE_EXIT_MP3:
      case REMOTE_MP3_PLAY_PAUSE:
      case REMOTE_MP3_PREV:
      case REMOTE_MP3_NEXT:
      case REMOTE_PASTER_MODE:
      case REMOTE_REC_START:
      case REMOTE_REC_PLAY:
      case REMOTE_REC_STOP:
      break;*/

#if( REMOTE_REC == ENABLE )
      case REMOTE_REC_MODE:
      g_select_appli = RECORDER_APPLI;
      recorder_mode = 0;
      goto paster_exit;
      /*paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;*/
#endif   //#if( REMOTE_REC == ENABLE )
      
      case REMOTE_CAL_MODE:
      bRemoteEnterCal = TRUE;
      g_select_appli = AP3_APPLI;
      goto paster_exit;
      /*paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;*/
      
      case REMOTE_AP4_MODE:
      //g_b_remote_on = FALSE;
      bRemoteEnterCal = FALSE;
      g_select_appli = AP3_APPLI;
      goto paster_exit;
      /*paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;*/

#if( REMOTE_MP3 == ENABLE )
      case REMOTE_MP3_MODE:
      g_select_appli = PLAYER_APPLI;
      goto paster_exit;
      /*paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;*/
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_DIC == ENABLE )
      case REMOTE_DIC_MODE:
      g_select_appli = DIC_APPLI;
paster_exit:
      paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      /*paster_send_cmd( CMD_PASTER_END, 0 );
      paster_exit_status = PASTER_EXIT;
      break;*/
#endif   //#if( REMOTE_DIC == ENABLE )
   }
}


#pragma CODE = MMI_PASTER_TEMP_CODE00

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_paster_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
   // Search file to play
   if( !search_subdir_mp3_file( "recording.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   apf_play_state = APF_PLAY_MODE;
   return TRUE;
}


#pragma CODE = MMI_PASTER_TEMP_CODE01

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_pasterno_file_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
   // Search file to play
   if( !search_subdir_mp3_file( "noapffile.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   apf_play_state = APF_PLAY_NO_FILE;
   return TRUE;
}


void paster_no_apf_file_deal( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 20, 12 );
   lcd_puts(( U8 *)"No APF file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 28, 12 );
   lcd_puts(( U8 *)"无APF文件" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 28, 12 );
   lcd_puts(( U8 *)"礚APFゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"No APF file" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"无APF文件" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"礚APFゅン" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )

   scan_oid_disable();
   mmi_paster_kbd_disable();
   apf_play_state = APF_PLAY_ERROR;
   mmi_file_flag |= APF_NO_FILE_FLAG;
   if( !mmi_pasterno_file_playintro())
   {
      //if( player_ap3_is_busy())
      //   player_ap3_stop();
#if 0
      //file_close();
      paster_task_stop();
      if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
      {
         if( power_is_vbus_present())
            g_select_appli = IDLE_APPLI;
         else
         {
            g_power_off_mode = POWER_OFF;
            g_select_appli = POWER_OFF_APPLI;
         }
      }
      else
      {
#if( DIC == DISABLE )
         g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
         g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
      }
      Mmgr_kill_this_mmi_appli();
#else
      s_ts_id = ts_alloc();
      ts_set_time( s_ts_id, SPLASH_TIME );
#endif
   }
   //else
   //   apf_play_state = APF_PLAY_NO_FILE;
}


#pragma CODE = MMI_PASTER_TEMP_CODE02

void mmi_paster_player_eof_deal( void )
{
   if( apf_play_state != APF_PLAY_ERROR )
   {
      power_authorize_autopoweroff();
      switch( apf_play_state )
      {
         case APF_PLAY_MODE:
         apf_play_state = APF_PLAY_ERROR;
         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //srvplayer_switch_off();  // Power off player
         mail_send_event( EVT_START_PASTER_APPLI, 0 );
         break;

         case APF_PLAY_NO_FILE:
         apf_play_state = APF_PLAY_ERROR;
         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //srvplayer_switch_off();  // Power off player
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //file_close();
         paster_task_stop();
         scan_oid_disable();
#if 0
         if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
            ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         {
            if( power_is_vbus_present())
               g_select_appli = IDLE_APPLI;
            else
            {
               g_power_off_mode = POWER_OFF;
               g_select_appli = POWER_OFF_APPLI;
            }
         }
         else
         {
#if( DIC == DISABLE )
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
         }
#else
         Ts_free( mmipaster_play_id_timerdisplay );
         Ts_free( s_ts_id );
         g_select_appli = IDLE_APPLI;
#endif
         Mmgr_kill_this_mmi_appli();
         break;

         default:
         apf_play_state = APF_PLAY_ERROR;
         break;
      }
   }
   else
      paster_send_cmd( CMD_PASTER_PLAY_EOF, 0 );
}


#if( LCD_PRESENT == ENABLE )

#pragma CODE = MMI_PASTER_TEMP_CODE03

//! Draw the background
//!
void mmi_paster_display_backgroung( void )
{
   lcd_turn_on();
   Lcd_start_backlight_timer();
   
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_screen();
   lcd_goto_xy( 40, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts(( U8 *)"Paster" );
   lcd_goto_xy( 46, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"PASTER" );
   status_display();
   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
   mmi_paster_display_volume();
}


#pragma CODE = MMI_PASTER_CODE

//! @brief Display volume
//!
void mmi_paster_display_volume( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 10, 0 );
   //sprintf( string , "%02bu", srvplayer_g_arg.volume );
   memset( string, 0, 8 );
   convert_U8(( char *)&string[ 4 ], g_volume );
   i = strlen(( char *)&string[ 4 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 4 ] );
   lcd_puts( string );
}

#if 0
//! Display record name + patch
//!
void mmipaster_display_name( void )
{
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
/*
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_dir_name( g_string2, G_STRING_SIZE2 );
#else
   unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
   nav_dir_name( Unicode_get_addr_string( g_string2 ), Unicode_get_char_space( g_string2 ) );
#endif
   //lcd_goto_xy( 15, 8 );
   //lcd_puts(g_string2);   
*/
#if ( (FS_ASCII   == ENABLED) && (FS_UNICODE == DISABLED))
   nav_file_name( g_string, G_STRING_SIZE, FS_NAME_GET, FALSE );
#else
   unicode_init_buffer_enc( g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
#endif
   lcd_goto_xy( 15, 8 );
   lcd_puts( g_string );
   //lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
}
#endif


#pragma CODE = MMI_PASTER_TEMP_CODE04

//! Display time
//!
void mmipaster_display_time( void )
{  
   _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;
   U32 time;

   SDKLoadCode( RECORDER_TEMP_CODE04 );
   time = record_gettime();
   //sprintf( string , "%02bu:%02bu:%02bu", MSB1( time ), MSB2( time ), MSB3( time ));
   lcd_goto_xy( 32, 24 );
   Ih_set_curr_font( IDC_FONT_08 );
   memset( string, 0, 9+4 );
   convert_U8(( char *)&string[ 9 ], MSB1( time ));
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   strcat(( char *)string, ":" );
   
   memset( &string[ 9 ], 0, 4 );
   convert_U8(( char *)&string[ 9 ], MSB2( time ));
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   strcat(( char *)string, ":" );

   memset( &string[ 9 ], 0, 4 );
   convert_U8(( char *)&string[ 9 ], MSB3( time ));
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   lcd_puts( string );
}


#pragma CODE = MMI_PASTER_CODE

void mmipaster_play_time( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 32, 24 );
   //sprintf( string , "%02bu:%02bu:%02bu", srvplayer_g_arg.start_time.hours, srvplayer_g_arg.start_time.minutes, srvplayer_g_arg.start_time.seconds );
   memset( string, 0, 9+4 );
   convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.hours );
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   strcat(( char *)string, ":" );
   
   memset( &string[ 9 ], 0, 4 );
   convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.minutes );
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   strcat(( char *)string, ":" );

   memset( &string[ 9 ], 0, 4 );
   convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.seconds );
   i = strlen(( char *)&string[ 9 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 9 ] );
   lcd_puts( string );
}


//! @brief Display state icon
//!
void mmi_paster_display_icon( void )
{
   lcd_draw_picture(4,13, mmipaster_state_icon[ srvplayer_g_player_state ] );
}

#endif   //#if( LCD_PRESENT == ENABLE )


#pragma CODE = MMI_PASTER_START_CODE

void paster_start_appli_deal( void )
{
   SendString((U8 *)"paster\r\n");
   mmgr_set_appli_full_screen();
   SDKLoadCode( PASTER_TEMP_CODE01 );
   paster_evt_init();
   SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
   unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));

   s_paster_display_id              = UNDEFINED_TIMER;
   mmipaster_play_id_timerdisplay   = UNDEFINED_TIMER;
   s_ts_id                          = UNDEFINED_TIMER;

   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   SDKLoadCode( MMI_PASTER_TEMP_CODE03 );
   mmi_paster_display_backgroung();

#if( OID == ENABLE )
   //b_oid_init_flag = FALSE;
   if( !b_oid_init_flag )
   {
      SDKLoadCode( OID_TEMP_CODE00 );
      reinit_oid_pen();
   }
#endif   //#if( OID == ENABLE )

   Kbd_set_evt_enabled();
   if( Kbd_is_event_triggered())
      Kbd_set_evt_disabled();

   scan_oid_disable();
   mmi_paster_kbd_disable();
   //g_b_search_record = FALSE;
   //mmi_file_flag &= ~APF_NO_FILE_FLAG;
#if( DIC == DISABLE )
   //mmi_file_flag |= ADB_NO_FILE_FLAG;
#endif   //#if( DIC == DISABLE )
   apf_play_state = APF_PLAY_ERROR;
   SDKLoadCode( MMI_PASTER_TEMP_CODE00 );
   if( !mmi_paster_playintro())
   {
      mail_send_event( EVT_START_PASTER_APPLI, 0 );
   }
   //else
   //   apf_play_state = APF_PLAY_MODE;
}


#pragma CODE = MMI_PASTER_BATLOW_CODE

void paster_bat_low_deal( void )
{
   //g_b_battery_low = TRUE;
   if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_paster_display_id );
      //power_authorize_autopoweroff();
   }
   if( player_ap3_is_busy())
      player_ap3_stop();
   //cue_off();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   paster_task_stop();
   scan_oid_disable();
   Ts_free( mmipaster_play_id_timerdisplay );
   g_select_appli = POWER_OFF_APPLI;//IDLE_APPLI;
   g_power_off_mode = BATTERY_LOW;
   Mmgr_kill_this_mmi_appli();
   /*paster_send_cmd( CMD_PASTER_POWER_LOW, 0 );
   g_select_appli = IDLE_APPLI;
   paster_exit_status = PASTER_POWER_LOW;*/
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
}


#pragma CODE = MMI_PASTER_USB_CODE

void paster_usb_deal( void )
{
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_paster_display_id );
      //power_authorize_autopoweroff();
   }
   if( player_ap3_is_busy())
      player_ap3_stop();
   //cue_off();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   paster_task_stop();
   scan_oid_disable();
   Ts_free( mmipaster_play_id_timerdisplay );
   Ts_free( s_ts_id );
   //g_select_appli = MSTORAGE_APPLI;
   //Mmgr_kill_this_mmi_appli();
   //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
   SDKLoadCode( AUADC_COMMON_CODE );
   AUADC_Power_Down();
   CommonLoadCode( C_USB );
}


#pragma CODE = MMI_PASTER_INIT_CODE

void paster_appli_init_deal( void )
{
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
   bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
   power_authorize_autopoweroff();
   //srvplayer_volume_send();
   
   paster_task_start();
   //apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
   paster_send_cmd( CMD_PASTER_START, 0 );
   //g_b_search_record = FALSE;

   Current_apf_num = 0;
   apf_change_file = MMI_PASTER_NO_CHANGE_FILE;
   s_paster_cue = 0;
   g_b_disk_is_full = FALSE;
}


#pragma CODE = MMI_PASTER_KEY_PRESS_CODE

Bool mmi_paster_key_press_deal( U16 param )
{
#if( LCD_PRESENT == ENABLE )
#  if(BACKLIGHT == ENABLE)
   if( g_mmi_lcd_backlight_off )
   {
      if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
         g_mmi_lcd_backlight_off = FALSE;
      return TRUE;
   }
#  endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
   if( apf_play_state != APF_PLAY_ERROR )
   {
      if(( KBD_KEY_PRESSED | KBD_KEY_LONGPRESSED | KBD_KEY_REPEAT | KBD_KEY_REPEAT_RELEASED )
         & KEY_STATUS( param ))
      {
         return TRUE;
      }
      if( KBD_KEY_RELEASED & KEY_STATUS( param ))
      {
         if(( KEY_ID( param )!= KEY_VOL_UP )
            &&( KEY_ID( param )!= KEY_VOL_DOWN ))
            return TRUE;
      }
   }
   else
   {
      if( mmi_paster_kbd_is_disable())
         return TRUE;
   }
   return FALSE;
}


#pragma CODE = MMI_PASTER_EXIT_CODE

void mmi_paster_exit_deal( void )
{
   if( player_ap3_is_busy())
      player_ap3_stop();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   paster_task_stop();
   scan_oid_disable();
   Ts_free( mmipaster_play_id_timerdisplay );
   Mmgr_kill_this_mmi_appli();
   if(( paster_exit_status == PASTER_POWER_OFF )
	      ||( paster_exit_status == PASTER_POWER_LOW ))
   {
      //mail_send_command( CMD_POWER_OFF, 0 );
      g_select_appli = POWER_OFF_APPLI;
   }
}


#pragma CODE = MMI_PASTER_POWEROFF_CODE

void paster_poweroff_deal( void )
{
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   paster_send_cmd( CMD_PASTER_POWER_OFF, 0 );
   g_select_appli = IDLE_APPLI;
   paster_exit_status = PASTER_POWER_OFF;
   g_power_off_mode = POWER_OFF;
}


#endif  //#if( PASTER == ENABLE )

#endif // _mmi_paster_c_
