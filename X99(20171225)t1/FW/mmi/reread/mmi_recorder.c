//!
//! @file mmi_recorder.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application RECORDER
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: mmi_recorder.c,v 2.1 2007/04/03 09:52:46 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_recorder_c_
#define _mmi_recorder_c_


//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_lcd.h"

#include <stdio.h>
#include <string.h>

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_mmi.h"
#include "mmi\status\mmi_status.h"
#include "mmi\kbd\keypad.h"
#include "mmi_recorder.h"
#include "srv_recorder.h"

#include "modules\control_access\ctrl_access.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "lib_system\unicode\unicode.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"


//#include "modules\player_ap3\player_ap3.h"
#include "modules\audio\audio.h"
#include "modules\recorder\recorder.h"
//#include "modules\cue\cue.h"

#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

//#include "modules\paster\paster.h"
//#include "modules\paster\paster_task.h"
//#include "modules\paster\paster_base.h"
#include "mmi\status\mmi_status.h"
#include "conf\conf_sio.h"
#include "mmi\player\srv_player.h"
#include "modules\player\player.h"
#include "modules\search\search_remote.h"
#include "drivers\oid\oid.h"
#include "modules\search\search.h"
#include "modules\power\power.h"             // Power module definition
#include "sio\com.h"
#include "mmi\startup\mmi_startup.h"

#include "lib_mcu\lcd\lcd_drv.h"
//#include "mmi\rw_nor\rw_nor.h"
//#include "drivers\lcd\lcd_SSD1303.h"
#include "mmi\images\tab_pict.h"

//#include "mmi\setting\mmi_setting.h"
//#include "mmi\paster\mmi_paster.h"
#include "modules\recorder\recorder_task.h"

#include "mp3_include.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fc_internal.h"

#include "point_g.h"


#pragma DATA = MMI_RECORDER_DATA
#pragma CODE = MMI_RECORDER_CODE


ImageDeclareType( MMI_RECORDER_TEMP_CODE00 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE01 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE02 );
ImageDeclareType( MMI_REREAD_PLAYTIME_CODE );
//ImageDeclareType( MMI_REREAD_DISPTIME_CODE );
ImageDeclareType( MMI_RECORDER_TEMP_CODE05 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE06 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE07 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE08 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE09 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE10 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE11 );
ImageDeclareType( MMI_RECORDER_TEMP_CODE12 );

//ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE00 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE01 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE03 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE04 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE07 );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( RECORDER_TEMP_CODE00 );
ImageDeclareType( RECORDER_TEMP_CODE01 );
ImageDeclareType( RECORDER_TEMP_CODE02 );
ImageDeclareType( RECORDER_TEMP_CODE03 );
ImageDeclareType( RECORDER_TEMP_CODE04 );
ImageDeclareType( AUADC_COMMON_CODE );

//ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( KB_RESIZE_CODE );

ImageDeclareType( SRVREC_TEMP_CODE00 );
ImageDeclareType( SRVREC_TEMP_CODE01 );

ImageDeclareType( REC_DIR_END_TEMP_CODE00 );

ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE00 );
ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE15 );
ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE16 );
ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE21 );
ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE22 );
ImageDeclareType( MMI_RECORDER_KEY_TEMP_CODE23 );

ImageDeclareType( MMI_RECORDER_USB_CODE );

ImageDeclareType( RECORDER_REMOTE_TEMP_CODE00 );
ImageDeclareType( RECORDER_REMOTE_TEMP_CODE06 );
ImageDeclareType( RECORDER_REMOTE_TEMP_CODE09 );

ImageDeclareType( RECORDER_STAR_TEMP_CODE00 );
ImageDeclareType( RECORDER_STAR_TEMP_CODE01 );

ImageDeclareType( MMI_REREAD_KEYGENDU_CODE );
ImageDeclareType( MMI_REREAD_KEYDUIBI_CODE );
ImageDeclareType( REREAD_MENU_INIT_CODE );
ImageDeclareType( PLAYER_WAV_CODE );
ImageDeclareType( SET_OID_COMMAND );

//________________ P R I V A T E   D E C L A R A T I O N S ___________________

// Recorder messages
//#define MMIRECORDER_MSG_PREPARING      "准备录音"
//#define MMIRECORDER_MSG_MEM_FULL       "Memory full"
//#define MMIRECORDER_MSG_ERR_RECORD     "record\nimpossible"
 
// ID of the timer soft used by this MMI appli
_MEM_TYPE_SLOW_  U8     s_ts_display_id;






#define  RECORDER_TIME  TIMER_MS(400) 

// Icon for each player state
_CONST_TYPE_ U32 mmirecorder_state_icon[] =
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

extern                     Bool  g_b_recorder_on;
extern                     Bool  g_b_player_on;

extern                     Bool  sd_format_error;

//extern   _MEM_TYPE_SLOW_   U8    volume;
//extern   _MEM_TYPE_SLOW_   U8    record_result;

//#define  mmi_recorder_display_volume()  mmi_ap3_display_volume()
typedef enum
{
   REC_PLAY_MODE
,  REC_PLAY_ERROR
,  REC_PLAY_BAT_WARN
}REC_STATE;

typedef enum
{
   REREAD_IDLE = 0
,  REREAD_REC
,  REREAD_PLAY
,  REREAD_ERROR

}REREAD_STATE;

_MEM_TYPE_SLOW_   U8          rec_play_state;
//#define  rec_play_state       ap4_play_state

//const    U8    recorder_dir[] = "RECORDER";

_MEM_TYPE_SLOW_   U8          rec_ext[]= "wav";
_MEM_TYPE_SLOW_   U8          recorder_file;


// ID of the timer soft used by this MMI appli
//_MEM_TYPE_SLOW_   U8          mmirecorder_play_id_timerdisplay;
#define  mmirecorder_play_id_timerdisplay    mmipaster_play_id_timerdisplay

_MEM_TYPE_SLOW_   U32         recorder_wait_time;

//#define  REC_PATH       "0:\\Recorder"
char  /*_MEM_TYPE_SLOW_*/  recorder_path[ 24 ];
char  /*_MEM_TYPE_SLOW_*/  recorder_file_name[ 13 ];

char  _MEM_TYPE_SLOW_   recorderpath[ 13 ];
//char  _MEM_TYPE_SLOW_   cur_recorder_path[ 30 ];
_MEM_TYPE_SLOW_   U8    file_mode;

_MEM_TYPE_SLOW_  U16     diy_min;
_MEM_TYPE_SLOW_  U16     diy_max;

_MEM_TYPE_SLOW_  REREAD_STATE     reread_state;
_MEM_TYPE_SLOW_  U16     cur_reread_num;


//** Routine to manage the display
#if( LCD_PRESENT == ENABLE )
void  mmirecorder_display_background( void );
void  mmirecorder_display_preparing ( void );
void  mmirecorder_display_name      ( void );
void  mmirecorder_display_time      ( U8 time_flag );
void  mmirecorder_disrecord_time	( void );
void  mmirecorder_display_freetime  ( void );
//void  mmirecorder_play_time         ( void );
void  mmi_recorder_display_volume   ( void );
void  mmi_recorder_display_icon     ( void );
void  mmi_recorder_no_file          ( void );
#else
#define mmirecorder_display_background()
#define mmirecorder_display_preparing()
#define mmirecorder_display_name()
#define mmirecorder_display_time()
#define mmirecorder_disrecord_time()
#define mmirecorder_display_freetime()
//define mmirecorder_play_time()
#define mmi_recorder_display_volume()
#define mmi_recorder_display_icon()
#endif  //#if( LCD_PRESENT == ENABLE )

Bool  mmi_recorder_playintro        ( void );
Bool  find_rec_file                 ( U8 _MEM_TYPE_SLOW_ * sz_filterext );
Bool  find_next_rec_file            ( U8 _MEM_TYPE_SLOW_ * sz_filterext );
Bool  find_prev_rec_file            ( U8 _MEM_TYPE_SLOW_ * sz_filterext );
Bool  find_index_rec_file			( U8 _MEM_TYPE_SLOW_ * sz_filterext );
void  recorder_play_stop            ( void );
char  *convert_U8                   ( char *p, U8 i );
void  mmi_recorder_player_eof_deal  ( void );
void  record_fail_deal              ( void );
void  record_eof_deal               ( void );

Bool  srv_rec_find_file             ( Bool flag );
Bool  srv_rec_creat_file            ( void );
Bool  reload_rec_file_dir           ( void );

void  power_key_press_deal          ( void );
void  up_key_long_press_deal        ( void );
void  down_key_long_press_deal      ( void );
void  mmi_recorder_usb_deal         ( void );
void  remote_rec_play_deal          ( void );
void  remote_prev_deal              ( void );
void  play_remote_prev              ( void );
void  remote_next_deal              ( void );
void  recorder_star_appli_deal      ( void );
void  recorder_appli_init_deal      ( U8 mode );
void  gendu_key_press_deal            ( void );
void  duibi_key_press_deal			( void );
void  power_key_long_press_deal     ( void );
void  men_key_long_press_deal       ( void );
void  mode_key_long_press_deal      ( U8 appli_id );
void  reread_menu_init(void);
void bat_low_waring_deal( U16 param );
void find_recod_dir(void);
Bool global_key_deal(U16 param);
void judge_current(void);
void stop_rec_play(void);


extern void set_oid_type(Bool set_type);

//_____ D E F I N I T I O N S ______________________________________________


//! Manage the record behavior
//! 
//! @param U8  event
//!        U16 param
//!
void recorder_mmi_appli(U8 event, U16 param)
{
   switch( event )
   {
      case EVT_START_APPLI:
	
      SDKLoadCode( RECORDER_STAR_TEMP_CODE00 );
      recorder_star_appli_deal();
      break;

      case EVT_STOP_RECORDER_APPLI:
//	  SendString((U8 *)"\r\nEVT_STOP_RECORDER_APPLI");
//	  find_recod_dir();
	  SDKLoadCode(REREAD_MENU_INIT_CODE);
      reread_menu_init();
      SDKLoadCode( RECORDER_STAR_TEMP_CODE01 );
      recorder_appli_init_deal( LSB( param ));
      break;

      case EVT_BAT_WARING:
	  	/*
      if( !g_b_recorder_on )
         break;
      srvrec_stop();
      Ts_free( s_ts_display_id );
      //power_authorize_autopoweroff();
      mmi_recorder_display_icon();
	  */
	   SDKLoadCode( MMI_AP4_BAT_LOW_CODE );
      bat_low_waring_deal(param);
//      mmirecorder_play_time();
      break;
      
      case EVT_START_RECORDER_APPLI:
      SDKLoadCode( MMI_RECORDER_TEMP_CODE02 );
      mmirecorder_display_preparing();
      //SDKLoadCode( MMI_RECORDER_TEMP_CODE10 );
      if( !srvrec_start())
      {
         SDKLoadCode( MMI_RECORDER_TEMP_CODE11 );
         record_fail_deal();
#if 0
#endif
         //break;
      }
      break;

      /*case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;*/
   
      // Running only if feature keyboard is activated
      case EVT_KEY_PRESSED:
	  SendString((U8 *)"\r\nrecorder_key=");
	  send_hex_u16(param);
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
      if( rec_play_state != REC_PLAY_ERROR )
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
	  if(global_key_deal(param))break;
      if( KBD_KEY_RELEASED == KEY_STATUS(param) )
      {
         switch( KEY_ID(param) )
         {
            case KEY_POWER:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE00 );
            power_key_press_deal();
            break;
            case KEY_VOL_UP:
            if( !g_b_recorder_on )
            {
               srvplayer_volume_change( SRVPLAYER_VOL_UP );
               mmi_recorder_display_volume();
            }
            break;

            case KEY_VOL_DOWN:
            if( !g_b_recorder_on )
            {
               srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
               mmi_recorder_display_volume();
            }
            break;

			/*
			case KEY_BAK:
            SDKLoadCode( MMI_REREAD_KEYGENDU_CODE );
            gendu_key_press_deal();
            break;
			*/

			
			/*
			case KEY_GENDU:
  			recorder_mode = 1;
  			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(REREAD_APPLI);
            break;
			
            case KEY_DUIBI:
			recorder_mode = 0;
  			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(REREAD_APPLI);
            break;
			*/	
/*			
			case KEY_PREV:
			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE16 );
            down_key_long_press_deal();	
			break;
			
			case KEY_NEXT:
			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE15 );
            up_key_long_press_deal();	
			break;

			case KEY_MODE:
			//case KEY_FUDU:
		    SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(IDLE_APPLI);
			break;
//*/
			case KEY_FUDU:
			//case KEY_MODE:
#if 0// (MP3_AB == ENABLE)			
			if(srvplayer_g_player_state != MMI_PLAYER_STATE_PLAY)break;
			if(count_AB > 0x7F)break;
			if(count_AB == 3){
//				SendString((U8*)"\r\n3");
				 count_AB = 0;
				if( audio_is_wav == 0 ){
					}
				else{
					DAC_Pause();
					SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  				 	lcd_clear_part_of_screen(24, 56, 16, 8);
					FSSeek(ADPCM_File,(((media_paly_all_time - point_A)<<8)+60));
					Frame_Count = point_A;
					SDKLoadCode( PLAYER_WAV_CODE );
					DAC_Continue();
					}
				
				 break;
				}
			if(count_AB == 2){
//				SendString((U8*)"\r\n2");
				 count_AB ++;
				 if( audio_is_wav == 0 ){
				 	}
				 else{
				 	if((point_B - point_A)<(size_psec_wav<<1)){
					 	 count_AB = 0;
						 SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  				 		 lcd_clear_part_of_screen(24, 56, 16, 8);
						 break;
				 	 	}
					 else{
					 	DAC_Pause();
					 	FSSeek(ADPCM_File,(((media_paly_all_time - point_A)<<8)+60));
						Frame_Count = point_A;
						DAC_Continue();
					 	}
				 	}
				 lcd_draw_picture(24, 56, IDP_PLAYER_MARK_AB);
				}
			if(count_AB == 1){
//				SendString((U8*)"\r\n1");
				if( audio_is_wav == 0 ){
					 point_B = current_play_time;
					}
				else{
					 point_B = Frame_Count;
					}
				 count_AB ++;
				 lcd_draw_picture(24, 56, IDP_PLAYER_MARK_B);
				}
			if(count_AB == 0){
//				 SendString((U8*)"\r\n0");
				 if( audio_is_wav == 0 ){
				 	point_A = current_play_time;
				 	}
				 else{
				 	point_A = Frame_Count;
				 	}
				 count_AB ++;
				 lcd_draw_picture(24, 56, IDP_PLAYER_MARK_A);
				}
#endif //#if (MP3_AB == ENABLE)			
			break;
			
         }
      }
      else if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE21 );
            power_key_long_press_deal();
            break;

#if 0//( PASTER_MODE == ENABLE )
            case KEY_MEN:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE22 );
            men_key_long_press_deal();
            break;
#endif   //#if( PASTER_MODE == ENABLE )

			/*
            case KEY_MODE:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(IDLE_APPLI);
            break;
			*/
/*
			case KEY_GENDU:
//            SDKLoadCode( MMI_REREAD_KEYGENDU_CODE );
//            gendu_key_press_deal();
  			recorder_mode = 1;
  			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(REREAD_APPLI);
            break;
			
            case KEY_DUIBI:
//            SDKLoadCode( MMI_REREAD_KEYDUIBI_CODE );
//            duibi_key_press_deal();
			recorder_mode = 0;
  			SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(REREAD_APPLI);
            break;
*/
/*			case KEY_FANYI:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
            mode_key_long_press_deal(RECORDER_APPLI);
            break;


            case KEY_VOL_UP:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE15 );
            up_key_long_press_deal();
            break;

            case KEY_VOL_DOWN:
            SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE16 );
            down_key_long_press_deal();
            break;
*/			
         }
      }
      break;

      case EVT_TIMER:
      if( LSB(param) == s_ts_display_id )
      {
         ts_set_time( s_ts_display_id, RECORDER_TIME );
      //   SDKLoadCode( MMI_REREAD_DISPTIME_CODE );
      //   mmirecorder_display_time(PLAY_CURRENT_TIME);
         SDKLoadCode( MMI_REREAD_PLAYTIME_CODE );
         mmirecorder_disrecord_time();
         break;
      }
      else if( LSB(param) == mmirecorder_play_id_timerdisplay )
      {
        // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
         if( rec_play_state != REC_PLAY_ERROR )
            break;
         if( MMI_PLAYER_STATE_PLAY != srvplayer_g_player_state )
            break;
//         SDKLoadCode( MMI_REREAD_DISPTIME_CODE );
         //srvplayer_gettime();
        // mmirecorder_play_time();
         mmirecorder_display_time(PLAY_CURRENT_TIME);
      }
      else
      {
         Mmgr_krn_forward_current_event();
      }
      break;

      case EVT_RECORDER:
      SDKLoadCode( MMI_RECORDER_TEMP_CODE12 );
      record_eof_deal();
      break;

      case EVT_PLAYER:
      if( g_b_recorder_on )
         break;
	  //SendString((U8 *)"\r\nrec_play_state=");
      //send_hex_u8(rec_play_state);
      SDKLoadCode( MMI_RECORDER_TEMP_CODE09 );
      mmi_recorder_player_eof_deal();
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_rec_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_rec_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_select_appli = POWER_OFF_APPLI;
      //goto mmi_recorder_exit;
mmi_recorder_exit:
      if( g_b_recorder_on )
      {
         srvrec_stop();
         Ts_free( s_ts_display_id );
      }
      //player_off();
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();
      Ts_free( mmirecorder_play_id_timerdisplay );
      //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
      //srvplayer_switch_off();  // Power off player
      Mmgr_kill_this_mmi_appli();
      //power_authorize_autopoweroff();
      break;
      
      case EVT_USB_POWERED:
      SDKLoadCode( MMI_RECORDER_USB_CODE );
      mmi_recorder_usb_deal();
      break;

	  case EVT_READY_RECORDER_APPLI:
	  	judge_current();
		if(reread_state == REREAD_REC){
			mail_send_event( EVT_REMOTE, REMOTE_REC_START);
			}
		if(reread_state == REREAD_PLAY){
			mail_send_event( EVT_REMOTE, REMOTE_REC_PLAY);
			}
		break;

	 case EVT_CHANGE_OID_TYPE:
	  	//SDKLoadCode(CHANGE_OIDTYPE_CODE);
		 /*
		if(g_bluetooth){
			g_bluetooth = FALSE;
			}
		else{
			g_bluetooth = TRUE;
			}*/
		g_bluetooth = ~g_bluetooth;
		SDKLoadCode(SET_OID_COMMAND);
		set_oid_type(g_bluetooth);
		lcd_goto_xy( OID_TYPE_FLAG_X, OID_TYPE_FLAG_Y);
        Ih_set_curr_font( IDC_FONT_08 );
		if(g_bluetooth){
	  	lcd_puts(( U8 *)"OID2");
	  	}
	  else{
	  	lcd_puts(( U8 *)"DPR3");
	  	}
	  	break;
		
	  case EVT_GOTO_OID_MODE:
	  	g_select_appli = IDLE_APPLI;
	  	goto mmi_recorder_exit;
	  	break;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      if( rec_play_state != REC_PLAY_ERROR )
         break;
      switch( LSB( param ))
      {
         case REMOTE_VOL_UP:
         //if( g_b_recorder_on )
         //   break;
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_recorder_display_volume();
         break;
      
         case REMOTE_VOL_DOWN:
         //if( g_b_recorder_on )
         //   break;
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_recorder_display_volume();
         break;
///*
         case REMOTE_VOL0:
         case REMOTE_VOL1:
         case REMOTE_VOL2:
         case REMOTE_VOL3:
         case REMOTE_VOL4:
         case REMOTE_VOL5:
         case REMOTE_VOL6:
         case REMOTE_VOL7:
/*		 	
         case REMOTE_VOL8:
         case REMOTE_VOL9:
         case REMOTE_VOL10:
         case REMOTE_VOL11:
         case REMOTE_VOL12:
         case REMOTE_VOL13:
         case REMOTE_VOL14:
//*/		 	
         //if( g_b_recorder_on )
         //   break;
         if(( LSB( param )- REMOTE_VOL0 )>( VOL_LEVEL - 1 ))
            break;
         g_volume = LSB( param )- REMOTE_VOL0;
         srvplayer_volume_send();
         mmi_recorder_display_volume();
         break;
//  */    
         case REMOTE_POWER_OFF:
         if( power_is_vbus_present())
            break;
         g_select_appli = POWER_OFF_APPLI;
         g_power_off_mode = POWER_OFF;
         g_b_remote_on = FALSE;
         goto mmi_recorder_exit;
		 
         case REMOTE_REC_START:
		 	/*
         if( !g_b_recorder_on )
         {
            SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
            srvplayer_stop();
            Ts_free( mmirecorder_play_id_timerdisplay );
            mail_send_event( EVT_START_RECORDER_APPLI, 0 );
         }
		 */
		 stop_rec_play();
		 mail_send_event( EVT_START_RECORDER_APPLI, 0 );
         break;
         
         case REMOTE_REC_STOP:
		 stop_rec_play();
         break;


         case REMOTE_REC_PLAY:
		 stop_rec_play();
         SDKLoadCode( RECORDER_REMOTE_TEMP_CODE00 );
         remote_rec_play_deal();
         break;
  /*    
         case REMOTE_MP3_PREV:
         SDKLoadCode( RECORDER_REMOTE_TEMP_CODE06 );
         remote_prev_deal();
         break;
      
         case REMOTE_MP3_NEXT:
         SDKLoadCode( RECORDER_REMOTE_TEMP_CODE09 );
         remote_next_deal();
         break;
 */     
         case REMOTE_CAL_MODE:
         g_select_appli = AP3_APPLI;
         bRemoteEnterCal = TRUE;
         goto mmi_recorder_exit;
        
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         g_select_appli = AP3_APPLI;
         bRemoteEnterCal = FALSE;
         goto mmi_recorder_exit;
         
         break;
      
         case REMOTE_MP3_MODE:
         g_select_appli = PLAYER_APPLI;
         goto mmi_recorder_exit;
         

#if( REMOTE_PASTER == ENABLE )
         case REMOTE_PASTER_MODE:
         g_select_appli = PASTER_APPLI;
         goto mmi_recorder_exit;
        
#endif   //#if( REMOTE_PASTER == ENABLE )

         case REMOTE_DIC_MODE:
         g_select_appli = DIC_APPLI;
         goto mmi_recorder_exit;
         
      }
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/
      
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

char *convert_U16( char *p, U16 i )
{
   if( i / 10 )
      p = convert_U16( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#if 0
void find_recod_dir(void)
{
/*
#if (UNICODE_TYPE == ENABLED)
        U16 RECORD_DIR_NAME[] = {0x5F55,0x97F3,0x6587,0x4EF6,0x0000};//录音文件
        //U16 MP3_DIR_NAME[] = {0x6587,0x4EF6,0x0000};//mp3文件
        U16 *p;
		U8 i;
		
   		p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\"), 3+1 );
      	
			//p = str_copy_u_n(( U16 *)tisiFileName, ( U16 *)RECORD_DIR_NAME, 4+1 );
		p = str_copy_u_n( p, ( U16 *)RECORD_DIR_NAME, 4+1 );
		p = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
		  SDKLoadCode( MMI_PLAYINTRO_CODE00 );
        ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
		 
		  for(i=0;i<(strlen(tisiFileShortpath)-2);i++){
		 	cur_recorder_path[i] = tisiFileShortpath[i+3];
		 	}
#endif 		

#if (UNICODE_TYPE == DISABLED)
		strcpy((char *)cur_recorder_path,(char *)RECORD_DIR);
#endif
*/
		 strcpy((char *)cur_recorder_path,(char *)DIY_REC_DIR);
		 SendString((U8 *)"\r\ncur_recorder_path=");
   		 SendString((U8 *)cur_recorder_path);
		// strcat((char *)cur_recorder_path,(char *)"\\");
}
#endif

Bool global_key_deal(U16 param)
{
	if(param == ((KEY_REC<<8)|KBD_KEY_PRESSED)){
		//g_select_appli = g_old_select_appli = PLAYER_APPLI;//DIC_APPLI;
		//Mmgr_kill_this_mmi_appli();
		//CommonLoadCode(C_PLAYER);
		SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
        mode_key_long_press_deal(PLAYER_APPLI);
		return TRUE;
		}
	if(param == ((KEY_MODE<<8)|KBD_KEY_PRESSED)){
		//g_select_appli = g_old_select_appli = PLAYER_APPLI;//DIC_APPLI;
		//Mmgr_kill_this_mmi_appli();
		//CommonLoadCode(C_PLAYER);
		SDKLoadCode( MMI_RECORDER_KEY_TEMP_CODE23 );
        mode_key_long_press_deal(IDLE_APPLI);
		return TRUE;
		}
	return FALSE;
}
void judge_current(void)
{
	reread_state = REREAD_IDLE;
	if(g_setting_list.struct_setting.bluetooth){
		//diy_min = OID2_DIY_RECORD_MIN;
		//diy_max = OID2_DIY_RECORD_MAX;
		if((Current_Index >= OID2_DIY_RECORD_MIN)&&(Current_Index <= OID2_DIY_RECORD_MAX)){
			if(Current_Index%2 == 0){
				reread_state = REREAD_PLAY;
				cur_reread_num = Current_Index - 1;
				}
			else{
				reread_state = REREAD_REC;
				cur_reread_num = Current_Index;
				}
			}
		}
	 else {
	 	if((Current_Index >= DPR3_DIY_RECORD_MIN)&&(Current_Index <= DPR3_DIY_RECORD_MAX)){
				reread_state = REREAD_REC;
				cur_reread_num = Current_Index;
			}
		if((Current_Index >= DPR3_DIY_PLAY_MIN)&&(Current_Index <= DPR3_DIY_PLAY_MAX)){
				reread_state = REREAD_PLAY;
				cur_reread_num = Current_Index - DPR3_DIY_OFFSET;
			}
	 	}
	SendString((U8 *)"\r\ni:s:n=");
	send_hex_u16(Current_Index);
	SendString((U8 *)":");
	send_hex_u8(reread_state);
	SendString((U8 *)":");
	send_hex_u16(cur_reread_num);
}

void stop_rec_play(void)
{
	if( g_b_recorder_on )
		{
		   srvrec_stop();
		   Ts_free( s_ts_display_id );
		   //power_authorize_autopoweroff();
			SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
			lcd_clear_part_of_screen( 4, 32, 8, 8 );
		   mmi_recorder_display_icon();
 // 		 mmirecorder_play_time();
		}
			//if( srvplayer_g_player_state == MMI_PLAYER_STATE_PLAY )
		if( g_b_player_on )
		{
		   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
		   srvplayer_stop();	   // in case of
		   Ts_free( mmirecorder_play_id_timerdisplay );
		   mmi_recorder_display_icon();
//			 mmirecorder_play_time();
		}

}

#pragma CODE = MMI_RECORDER_TEMP_CODE00

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_recorder_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( "record.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   rec_play_state = REC_PLAY_MODE;
   return TRUE;
}


#pragma CODE = MMI_RECORDER_TEMP_CODE06

//the function is find the number of ap3 in the root;
Bool find_rec_file( U8 _MEM_TYPE_SLOW_ * sz_filterext )
{
   char temp_name[32];
   SendString((U8 *)"\r\nfind_rec_file!");
   SDKLoadCode( NAVIGATION_CODE );
   SDKLoadCode( NAVIGATION_TEMP_CODE00 );
   if( !nav_dir_root())
      return FALSE;
   
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_ascii();  // ASCII mode for name of copy file
#endif

   // Make record folder
   //sprintf( g_string, "Recorder" );
   SDKLoadCode( NAVIGATION_TEMP_CODE03 );
   if( !nav_filelist_findname(( U8 *)"DIYREC", FALSE ))
   {
#if 0
      goto find_rec_file_error;
#else
      SDKLoadCode( NAVIGATION_TEMP_CODE02 );
      if( !nav_dir_make(( U8 *)"DIYREC" ) )
      {
         if( FS_ERR_FILE_EXIST != fs_g_status )
         {
            SDKLoadCode( NAVIGATION_TEMP_CODE04 );
            ( void )nav_file_del( TRUE );
            goto find_rec_file_error;
         }
      }
      strcpy( recorderpath, (char *)DIY_REC_DIR);
    //  strcpy( recorderpath, (char *)"0:\\");
     // strcat( recorderpath, (char *)&cur_recorder_path);
      //strcat( recorderpath, (char *)"\\");
      
#endif
   }
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif

   SDKLoadCode( NAVIGATION_TEMP_CODE01 );
   if( !nav_dir_cd() )
      return FALSE;
#if 1
   strcpy( tisiFileName, DIY_REC_DIR);
#else   
   strcpy( tisiFileName, (char *)"0:\\");
   strcat( tisiFileName, (char *)&cur_recorder_path);
   strcat( tisiFileName, (char *)"\\");
#endif   
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
   strcpy( recorderpath, tisiFileShortpath );

  // SendString((U8 *)"\r\ndir_path=");
  // SendString((U8 *)recorderpath);
   SDKLoadCode( NAVIGATION_CODE );
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext )
            && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE ))
         {
            //strcpy( tisiFileShortpath, REC_PATH );
            //strcpy( tisiFileShortpath, recorderpath );
            //strcat( tisiFileShortpath, "\\" );
           // strcpy( tisiFileShortpath, RECORD_PATH);
			strcpy( tisiFileShortpath, (char *)recorderpath);
  			 //strcat( tisiFileShortpath, (char *)&cur_recorder_path);
   				strcat( tisiFileShortpath, (char *)"\\");
  	          strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );
			SendString((U8 *)"\r\nfind!");
			strcpy(temp_name,( char *)fat_g_cur_nav->name );
			temp_name[1] = 'e';
			temp_name[2] = 'c';
			temp_name[6] = 'w';
			temp_name[7] = 'a';
			temp_name[8] = 'v';
			temp_name[9] = 0;
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
            //lcd_goto_xy( 15, 8 );
            lcd_goto_xy( 15, 26);
            Ih_set_curr_font( IDC_FONT_16 );
            //lcd_puts( g_string );
            lcd_puts((U8 *)temp_name);
            mmi_recorder_display_icon();
 //           mmirecorder_play_time();
 			// SendString((U8 *)"\r\nfind!");
            return TRUE;
         }
      }
   }
   SendString((U8 *)"\r\nnofind!");
   return FALSE;
find_rec_file_error:
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif
   return FALSE;
}


#pragma CODE = MMI_RECORDER_TEMP_CODE10

//the function is find the number of ap3 in the root;
Bool find_index_rec_file( U8 _MEM_TYPE_SLOW_ * sz_filterext )
{
   char temp_name[32];
   char file_name[6];
   U32  file_size;
   U8  i;
   __X FS_ERR err;
   //SendString((U8 *)"\r\nfind_rec_file!");
 /*  
   SDKLoadCode( NAVIGATION_CODE );
   SDKLoadCode( NAVIGATION_TEMP_CODE00 );
   if( !nav_dir_root())
      return FALSE;
   
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_ascii();  // ASCII mode for name of copy file
#endif

   // Make record folder
   //sprintf( g_string, "Recorder" );
   SDKLoadCode( NAVIGATION_TEMP_CODE03 );
   if( !nav_filelist_findname(( U8 *)"DIYREC", FALSE ))
   {
#if 1
      goto find_rec_file_error;
#else
      SDKLoadCode( NAVIGATION_TEMP_CODE02 );
      if( !nav_dir_make(( U8 *)"DIYREC" ) )
      {
         if( FS_ERR_FILE_EXIST != fs_g_status )
         {
            SDKLoadCode( NAVIGATION_TEMP_CODE04 );
            ( void )nav_file_del( TRUE );
            goto find_rec_file_error;
         }
      }
      strcpy( recorderpath, (char *)DIY_REC_DIR);
    //  strcpy( recorderpath, (char *)"0:\\");
     // strcat( recorderpath, (char *)&cur_recorder_path);
      //strcat( recorderpath, (char *)"\\");
      
#endif
   }
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif

   SDKLoadCode( NAVIGATION_TEMP_CODE01 );
   if( !nav_dir_cd() )
      return FALSE;
*/   
#if 1
  // strcpy( tisiFileName, DIY_REC_DIR);
#else   
   strcpy( tisiFileName, (char *)"0:\\");
   strcat( tisiFileName, (char *)&cur_recorder_path);
   strcat( tisiFileName, (char *)"\\");
#endif   
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
		nav_string_ascii();  // ASCII mode for name of copy file
#endif

	 // strcat( recorder_path, (char *)&cur_recorder_path);
	 // strcat( recorder_path, (char *)"\\");
	  // Search last record file
	  //for( u8_i = 1; u8_i < 100; u8_i++ )
	  //for( u8_i = last_record_number; u8_i < 100; u8_i++ )
	 // last_record_number = cur_reread_num;
	  //{
		 //strcpy( recorder_path, (char *)"0:\\");
		 //strcat( recorder_path, (char *)&cur_recorder_path);
		 //strcat( recorder_path, (char *)"\\");
		 strcpy( recorder_path,(char *)DIY_REC_DIR);
		// strcat( recorder_path, (char *)"\\");
		 //sprintf( g_string, "rec%02bu.wav" , u8_i );
		 memset( recorder_file_name, 0, 13 );
		 //strcpy(( char *)recorder_file_name, "Rec");
		 strcpy(( char *)recorder_file_name, "Rec");
		 memset( file_name, 0, 6 );
		 convert_U16(( char *)file_name, cur_reread_num );
		 i = strlen(( char *)file_name );
		 for( ; i < 5; i++ )
			strcat(( char *)recorder_file_name, "0" );
		 strcat(( char *)recorder_file_name, ( char *)file_name );
		 strcat(( char *)recorder_file_name, ".wav" );

		 //strcpy(temp_name,recorder_file_name);
		 //strcat(( char *)recorder_path, "\\" );
		 strcat(( char *)recorder_path, recorder_file_name );
		 //ADPCM_File = ( __X FCFILE *)FSOpen( recorder_path, FS_MODE_READ, &err );
		 //FSClose(( __X FCFILE *)ADPCM_File );
		  
		// strcpy( tisiFileName, recorder_path );
		 SendString((U8 *)"\r\nrec_file=");
		 SendString((U8 *)recorder_path);
		 SDKLoadCode( MMI_PLAYINTRO_CODE00 );
#if 1//(UNICODE_TYPE == DISABLE)	  
		 if( !find_path_a(( U8 *)recorder_path, (U8 *)tisiFileShortpath ))
#endif	 
#if 0//(UNICODE_TYPE == ENABLE)	  
		 if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
#endif
		 {
			
			//goto find_rec_file_error;
			SendString((U8 *)"\r\nnofile!");
			return FALSE;
		   
		 }
	   else{
	   	
		 file_size = FSGetFileSize( tisiFileShortpath, &err );
		 /*
		 SendString((U8 *)"\r\ns:n=");
		 SendString((U8 *)tisiFileShortpath);
		 SendString((U8 *)":");
		 send_hex_u32(file_size );
		 */
		 if( 0 != file_size )
		 {
			SendString((U8 *)"\r\nfind!");
			//strcpy(temp_name,( char *)fat_g_cur_nav->name );
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
            //lcd_goto_xy( 15, 8 );
            lcd_goto_xy( 15, 26);
            Ih_set_curr_font( IDC_FONT_16 );
            //lcd_puts( g_string );
            lcd_puts((U8 *)recorder_file_name);
            mmi_recorder_display_icon();
			return TRUE;
		 }
		 else{
		 	//SendString((U8 *)"\r\nnodata!");
		 	return FALSE;
		 	}
	   }
		 //recorder_path[ 6 ] = 0;
	  //}
	  //return FALSE;


/*

   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
   strcpy( recorderpath, tisiFileShortpath );

  // SendString((U8 *)"\r\ndir_path=");
  // SendString((U8 *)recorderpath);
   SDKLoadCode( NAVIGATION_CODE );
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext )
            && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE ))
         {
            //strcpy( tisiFileShortpath, REC_PATH );
            //strcpy( tisiFileShortpath, recorderpath );
            //strcat( tisiFileShortpath, "\\" );
           // strcpy( tisiFileShortpath, RECORD_PATH);
			strcpy( tisiFileShortpath, (char *)recorderpath);
  			 //strcat( tisiFileShortpath, (char *)&cur_recorder_path);
   				strcat( tisiFileShortpath, (char *)"\\");
  	          strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );
			SendString((U8 *)"\r\nfind!");
			strcpy(temp_name,( char *)fat_g_cur_nav->name );
			temp_name[1] = 'e';
			temp_name[2] = 'c';
			temp_name[6] = 'w';
			temp_name[7] = 'a';
			temp_name[8] = 'v';
			temp_name[9] = 0;
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
            //lcd_goto_xy( 15, 8 );
            lcd_goto_xy( 15, 26);
            Ih_set_curr_font( IDC_FONT_16 );
            //lcd_puts( g_string );
            lcd_puts((U8 *)temp_name);
            mmi_recorder_display_icon();
 //           mmirecorder_play_time();
 			// SendString((U8 *)"\r\nfind!");
            return TRUE;
         }
      }
   }
 */  
 /*
   SendString((U8 *)"\r\nnofind!");
   return FALSE;
find_rec_file_error:
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
   nav_string_unicode();
#endif
   return FALSE;
*/
}


#pragma CODE = MMI_RECORDER_TEMP_CODE07

Bool find_next_rec_file( U8 _MEM_TYPE_SLOW_ * sz_filterext )
{
   Bool find_flag = FALSE;
   char temp_name[32];
   SDKLoadCode( NAVIGATION_CODE );
find_next_file:
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext )
            && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE ))
         {
            //strcpy( tisiFileShortpath, REC_PATH );
            //strcpy( tisiFileShortpath, recorderpath );
            //strcat( tisiFileShortpath, "\\" );
           // strcpy( tisiFileShortpath, RECORD_PATH);
           strcpy( tisiFileShortpath, (char *)recorderpath);
  			// strcat( tisiFileShortpath, (char *)&cur_recorder_path);
  			 strcat( tisiFileShortpath, (char *)"\\");
            strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );

			strcpy(temp_name,( char *)fat_g_cur_nav->name );
			temp_name[1] = 'e';
			temp_name[2] = 'c';
			temp_name[6] = 'w';
			temp_name[7] = 'a';
			temp_name[8] = 'v';
			temp_name[9] = 0;
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
            //lcd_goto_xy( 15, 8 );
            lcd_goto_xy( 15, 26);
            Ih_set_curr_font( IDC_FONT_16 );
           // lcd_puts( g_string );
            lcd_puts((U8 *)temp_name);
            mmi_recorder_display_icon();
 //           mmirecorder_play_time();
            return TRUE;
         }
      }
   }
   ( void )nav_filelist_reset();
   if( !find_flag )
   {
      find_flag = TRUE;
      goto find_next_file;
   }
   return FALSE;
}


#pragma CODE = MMI_RECORDER_TEMP_CODE08

Bool find_prev_rec_file( U8 _MEM_TYPE_SLOW_ * sz_filterext )
{
   Bool find_flag = FALSE;

   char temp_name[32];

   SDKLoadCode( NAVIGATION_CODE );
   while( nav_filelist_set( 0, FS_FIND_PREV ))
   {
find_prev_file:
      if( !nav_file_isdir())
      {
         //if( nav_file_checkext( sz_filterext )
         //   && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE ))
         if( nav_file_checkext( sz_filterext ))
         {
            if( !nav_filelist_set( 0, FS_FIND_PREV ))
               ( void )nav_filelist_reset();
            ( void )nav_filelist_set( 0, FS_FIND_NEXT );
            ( void )nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
            
            //strcpy( tisiFileShortpath, REC_PATH );
            //strcpy( tisiFileShortpath, recorderpath );
            //strcat( tisiFileShortpath, "\\" );
            //strcpy( tisiFileShortpath, RECORD_PATH);
            
            strcpy( tisiFileShortpath, (char *)DIY_REC_DIR);
            strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );

			//memset(temp_name,0,sizeof(temp_name));
			strcpy(temp_name,( char *)fat_g_cur_nav->name );
			temp_name[1] = 'e';
			temp_name[2] = 'c';
			temp_name[6] = 'w';
			temp_name[7] = 'a';
			temp_name[8] = 'v';
			temp_name[9] = 0;
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
            //lcd_goto_xy( 15, 8 );
            lcd_goto_xy( 15, 26 );
            Ih_set_curr_font( IDC_FONT_16 );
            // lcd_puts( g_string );
            lcd_puts((U8 *)temp_name);
            mmi_recorder_display_icon();
//            mmirecorder_play_time();
            return TRUE;
         }
      }
   }
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ));
   if( !find_flag )
   {
      find_flag = TRUE;
      goto find_prev_file;
   }
   
   return FALSE;
}


#pragma CODE = MMI_RECORDER_TEMP_CODE09

void mmi_recorder_player_eof_deal( void )
{
   
   if( rec_play_state != REC_PLAY_ERROR )
   {
      switch( rec_play_state )
      {
         case REC_PLAY_MODE:
         rec_play_state = REC_PLAY_ERROR;
         mail_send_event( EVT_STOP_RECORDER_APPLI, 0 );
         break;

		 case REC_PLAY_BAT_WARN:
    	 rec_play_state = REC_PLAY_ERROR;
		 scan_oid_enable();
		 Kbd_set_evt_enabled();
		 break;
		 
         default:
         rec_play_state = REC_PLAY_ERROR;
         break;
      }
   }
   else
   {
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();       // in case of
      Ts_free( mmirecorder_play_id_timerdisplay );
      mmi_recorder_display_icon();
//      mmirecorder_play_time();
   }
}


#if( LCD_PRESENT == ENABLE )

#pragma CODE = MMI_RECORDER_TEMP_CODE01

//! Display background
//!
void mmirecorder_display_background( void )
{
   lcd_turn_on();
   Lcd_start_backlight_timer();
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //  lcd_clear_screen();
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
   lcd_clear_part_of_screen(20, 0,64, 8);
   lcd_goto_xy( 20, 0 );
   Ih_set_curr_font( IDC_FONT_08);
   lcd_puts(( U8 *)"RECORD" );
   
   lcd_goto_xy( 32, 18 );
   Ih_set_curr_font( IDC_FONT_16 );
   if(g_setting_list.struct_setting.language_ihm == LANG_ENGLISH){
   lcd_puts(( U8 *)"Recorder" );
   	}
   else{
   	lcd_puts(( U8 *)"自主录音" );
   	}
   //lcd_goto_xy( 46, 0 );
   //Ih_set_curr_font( IDC_FONT_08 );
   //lcd_puts(( U8 *)"RECORD" );
   status_display();

   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
   mmi_recorder_display_volume();

   s_ts_display_id                  = UNDEFINED_TIMER;
   mmirecorder_play_id_timerdisplay = UNDEFINED_TIMER;
}


#pragma CODE = MMI_RECORDER_TEMP_CODE02

//! Display preparing message
//!
void mmirecorder_display_preparing( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   
   Ih_set_curr_font( IDC_FONT_16 );
   if( g_b_battery_low )
   {
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"Low voltage" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"电量不足!" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"筿秖ぃì!" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
	  lcd_goto_xy( 15, 26 );
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
   //      lcd_goto_xy( 20, 12 );
         lcd_puts(( U8 *)"Low voltage" );
         break;

         case LANG_CHINESE_SIMP:
 //        lcd_goto_xy( 28, 12 );
         lcd_puts(( U8 *)"电量不足!" );
         break;

         case LANG_CHINESE_TRAD:
  //       lcd_goto_xy( 28, 12 );
         lcd_puts(( U8 *)"筿秖ぃì!" );
         break;
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      return;
   }
   
   //lcd_puts( MMIRECORDER_MSG_PREPARING );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 28, 12 );
   lcd_puts( "Preparing" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"准备录音" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"称魁" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
	lcd_goto_xy(15, 26 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
 //     lcd_goto_xy( 28, 12 );
      lcd_puts(( U8 *)"Preparing" );
      break;
      
      case LANG_CHINESE_SIMP:
//      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"准备录音" );
      break;
   
      case LANG_CHINESE_TRAD:
 //     lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"称魁" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_REREAD_PLAYTIME_CODE

//! Display record name + patch
//!
void mmirecorder_disrecord_time( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 5+4 ], i;
   U32 time;

   SDKLoadCode( RECORDER_TEMP_CODE04 );
   time = record_gettime();
   //sprintf( string , "%02bu:%02bu:%02bu", MSB1( time ), MSB2( time ), MSB3( time ));
   lcd_goto_xy( 4, 44 );
   Ih_set_curr_font( IDC_FONT_08 );   
   memset( &string[ 5 ], 0, 4 );
   convert_U8(( char *)&string[ 5 ], MSB2( time ));
   i = strlen(( char *)&string[ 5 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 5 ] );
   strcat(( char *)string, ":" );

   memset( &string[ 5 ], 0, 4 );
   convert_U8(( char *)&string[ 5 ], MSB3( time ));
   i = strlen(( char *)&string[ 5 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 5 ] );
   lcd_puts( string );
}


//#pragma CODE = MMI_REREAD_DISPTIME_CODE


#pragma CODE = MMI_RECORDER_CODE

#if 1

//! Display time
//!
void mmirecorder_display_time( U8 time_flag )
{  
#if 1
   _MEM_TYPE_SLOW_ U8 string[6+4], i;

   U32 temp_data;
//   if(!mmi_player_display_is_autorize())  return;
#if 0
   SendString((U8 *)"\r\nflag:Cu_t = ");
   send_hex_u8(time_flag);SendString((U8 *)":");
   send_hex_u32(current_play_time);
#endif   
   if(time_flag == PLAY_BEGIN_TIME){
   	temp_data = 0;
   	}
  else if(time_flag == PLAY_ALL_TIME){
  	
  	if(audio_is_wav == 0){
	     temp_data = media_paly_all_time/1000;
	   	}
	   else{
	   	temp_data = media_paly_all_time/(U32)size_psec_wav;
		//temp_data = media_paly_all_time;
	   	}
	   /*
	   if(temp_data<=2){
	   	count_AB = 0x80;
	   	}
	   else{
	   	count_AB = 0;
	   	}*/
	   
  	}
  else{
	   if(audio_is_wav == 0){
	     temp_data = current_play_time/1000;
	   	}
	   else{
//	   	temp_data = Frame_Count;
		temp_data = current_play_time/(U32)size_psec_wav;
	   	//temp_data = (media_paly_all_time - temp_data<<8)/size_psec_wav;
	   // temp_data = media_paly_all_time - temp_data;
	   	}
   	}
  
   srvplayer_all_time.seconds = temp_data%60;
 //  temp_data = temp_data/60;
   srvplayer_all_time.minutes = temp_data/60;
//   temp_data = temp_data/60;
//    srvplayer_all_time.hours = temp_data%60;
//	return;
   // Draw time
   Lcd_unset_merging();
   Ih_set_curr_font( IDC_FONT_08 );
   memset( string, 0, 6+4 );
   if(time_flag == PLAY_ALL_TIME){
   	lcd_goto_xy( 20, 44 );
	strcat(( char *)string, "/" );
   	}
   else{
	  lcd_goto_xy( 4, 44 );
   	}
  
   //sprintf( string , "%02bu:%02bu:%02bu", srvplayer_g_arg.start_time.hours, srvplayer_g_arg.start_time.minutes, srvplayer_g_arg.start_time.seconds );
/*  
   convert_U8(( char *)&string[ 10 ], srvplayer_all_time.hours );
   i = strlen(( char *)&string[ 10 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 10 ] );
   strcat(( char *)string, ":" );
   
   memset( &string[ 6 ], 0, 4 );
 */  
   convert_U8(( char *)&string[ 6 ], srvplayer_all_time.minutes );
   i = strlen(( char *)&string[ 6 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 6 ] );
   strcat(( char *)string, ":" );

   memset( &string[ 6 ], 0, 4 );
   convert_U8(( char *)&string[ 6 ], srvplayer_all_time.seconds );
   i = strlen(( char *)&string[ 6 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 6 ] );
   lcd_puts( string );
#else
   _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;
   U32 time;

   SDKLoadCode( RECORDER_TEMP_CODE04 );
   time = record_gettime();
   //sprintf( string , "%02bu:%02bu:%02bu", MSB1( time ), MSB2( time ), MSB3( time ));
   lcd_goto_xy( 4, 44 );
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
#endif   
}

#else
void mmirecorder_play_time( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 4, 44 );
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
#endif

//! @brief Display volume
//!
void mmi_recorder_display_volume( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 6, 0 );
   //sprintf( string , "%02bu", srvplayer_g_arg.volume );
   memset( string, 0, 8 );
   convert_U8(( char *)&string[ 4 ], g_volume );
   i = strlen(( char *)&string[ 4 ] );
   for( ; i < 2; i++ )
      strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&string[ 4 ] );
   lcd_puts( string );
}


//! @brief Display state icon
//!
void mmi_recorder_display_icon( void )
{
   lcd_draw_picture(4,56, mmirecorder_state_icon[ srvplayer_g_player_state ] );
}


#pragma CODE = MMI_RECORDER_TEMP_CODE05

void mmi_recorder_no_file( void )
{
   SendString((U8 *)"\r\nrecord_nofile!");
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 8, 12 );
   lcd_puts(( U8 *)"No record file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 24, 12 );
   lcd_puts(( U8 *)"无录音文件" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 24, 12 );
   lcd_puts(( U8 *)"礚魁ゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
	lcd_goto_xy( 15, 26 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
//      lcd_goto_xy( 8, 12 );
      lcd_puts(( U8 *)"No record file" );
      break;

      case LANG_CHINESE_SIMP:
//      lcd_goto_xy( 24, 12 );
      lcd_puts(( U8 *)"无录音文件" );
      break;

      case LANG_CHINESE_TRAD:
//      lcd_goto_xy( 24, 12 );
      lcd_puts(( U8 *)"礚魁ゅン" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
   recorder_file = FALSE;
}

#endif  //#if( LCD_PRESENT == ENABLE )


//#pragma CODE = MMI_RECORDER_TEMP_CODE10
#pragma CODE = MMI_RECORDER_CODE

//! Start the record
//!
Bool srvrec_start( void )
{
   U8 u8_i;
   __X FS_ERR err;
   __X U32 file_size;
   U8 file_name[ 4 ], i;
   U8 mode;

   SDKLoadCode( SRVREC_TEMP_CODE00 );
   if( !srv_rec_find_file( TRUE ))
      return FALSE;

 
	
   SDKLoadCode( SRVREC_TEMP_CODE01 );
   if( !srv_rec_creat_file())
      return FALSE;

 
   
   SDKLoadCode( RECORDER_TEMP_CODE00 );
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
   //recorder_task_start();    // Enable event of recorder task
   power_forbid_autopoweroff();
   record_result = RECORD_OK;
   //mmirecorder_display_background();
   //mmirecorder_display_name();
   //lcd_draw_picture( 4, 13, IDP_PLAYER_RECORD );
   lcd_draw_picture( 4, 32, IDP_PLAYER_RECORD );
   //status_inhib_display();
   s_ts_display_id = ts_alloc();
   ts_set_time( s_ts_display_id, RECORDER_TIME );
   power_forbid_autopoweroff();
   recorder_file = TRUE;
   recorder_wait_time = Timer_set_timer( TIMER_MS( 500 ));
   green_led_stop_timer();
   red_led_stop_timer();
   red_led_on();
   green_led_on();
   return TRUE;
}


#pragma CODE = SRVREC_TEMP_CODE00

Bool srv_rec_find_file( Bool flag )
{
   U16 u8_i;
   __X FS_ERR err;
   __X U32 file_size;
   U8 file_name[ 6 ], i;

   record_result = RECORD_ERROR;

   if( sd_format_error )
      return FALSE;

  // strcpy(( char *)recorder_path, RECORD_PATH );
  //strcpy( recorder_path, (char *)"0:\\");
  // strcat( recorder_path, (char *)&cur_recorder_path);
  // strcat( recorder_path, (char *)"\\");
   // Search last record file
   //for( u8_i = 1; u8_i < 100; u8_i++ )
   //for( u8_i = last_record_number; u8_i < 100; u8_i++ )
  // last_record_number = cur_reread_num;
   {
   	  //strcpy( recorder_path, (char *)"0:\\");
      //strcat( recorder_path, (char *)&cur_recorder_path);
      //strcat( recorder_path, (char *)"\\");
      strcpy( recorder_path,(char *)DIY_REC_DIR);
     // strcat( recorder_path, (char *)"\\");
      //sprintf( g_string, "rec%02bu.wav" , u8_i );
       memset( recorder_file_name, 0, 6 );
      strcpy(( char *)recorder_file_name, "Rec");
      memset( file_name, 0, 6 );
      convert_U16(( char *)file_name, cur_reread_num );
      i = strlen(( char *)file_name );
      for( ; i < 5; i++ )
         strcat(( char *)recorder_file_name, "0" );
      strcat(( char *)recorder_file_name, ( char *)file_name );
      strcat(( char *)recorder_file_name, ".wav" );

      //strcat(( char *)recorder_path, "\\" );
      strcat(( char *)recorder_path, recorder_file_name );
      //ADPCM_File = ( __X FCFILE *)FSOpen( recorder_path, FS_MODE_READ, &err );
      //FSClose(( __X FCFILE *)ADPCM_File );
	   
      strcpy( tisiFileName, recorder_path );
	  //SendString((U8 *)"\r\nrec_file=");
	  //SendString((U8 *)tisiFileName);
      SDKLoadCode( MMI_PLAYINTRO_CODE00 );
#if 1//(UNICODE_TYPE == DISABLE)	  
      if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath ))
#endif	 
#if 0//(UNICODE_TYPE == ENABLE)	  
      if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
#endif
      {
      	 
         file_mode = FS_MODE_CREATE;
		 if(!flag)return FALSE;
        // break;
      }
	else{
      file_size = FSGetFileSize( tisiFileShortpath, &err );
      if( !file_size )
      {
         file_mode = FS_MODE_READ_WRITE;
         //break;
      }
	}
      //recorder_path[ 6 ] = 0;
   }
   /*
   last_record_number = u8_i;
   if( u8_i >= 100 )
   {
      record_result = RECORD_FILE_FULL;
      return FALSE;
   }
   */
   return TRUE;
}


#pragma CODE = SRVREC_TEMP_CODE01

Bool srv_rec_creat_file( void )
{
   __X FS_ERR err;
   __X U32 file_size;


	file_size = MAX_LENGTH;

   if( file_mode == FS_MODE_READ_WRITE )
   {
      ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
   }
   else
   {
      //SendString((U8 *)"\r\ncreat");
      /*strcpy( tisiFileName, "0:\\Recorder\\" );
      ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
      ADPCM_File = ( __X FCFILE *)FSOpenForUnicode2(( char *)tisiFileShortpath, ( char *)recorder_file_name, ( U8 *)atou(( U8 *)recorder_file_name ), FS_MODE_CREATE, &err );*/
 	  SendString((U8 *)"\r\ndir:name:short_path=");
	  SendString((U8 *)recorderpath);
	  SendString((U8 *)":");
	  SendString((U8 *)recorder_file_name);

	  //strcpy( tisiFileShortpath, recorderpath );
     // strcat( tisiFileShortpath, "\\" );
     // strcat( tisiFileShortpath, recorder_file_name );
      //ADPCM_File = ( __X FCFILE *)FSOpenForUnicode2(( char *)recorderpath, ( char *)recorder_file_name, ( U8 *)atou(( U8 *)recorder_file_name ), FS_MODE_CREATE, &err );
      ADPCM_File = ( __X FCFILE *)FSOpen( ( char *)tisiFileName, FS_MODE_CREATE, &err );
		SendString((U8 *)"\r\nerr==");
	   send_hex_u8(err);
      /*SendString((U8 *)"resize=");
      send_hex_u8(err);
      SendReturn();
      //return FALSE;

      if( err )
      {
         FSClose(( __X FCFILE *)ADPCM_File );
         return FALSE;
      }*/
      
      SDKLoadCode( KB_RESIZE_CODE );
      kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
      FSClose(( __X FCFILE *)ADPCM_File );

      /*SendString((U8 *)"find rec\r\n");
      SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
      ( void )find_rec_file( rec_ext );
      SDKLoadCode( MMI_RECORDER_TEMP_CODE08 );
      ( void )find_prev_rec_file( rec_ext );*/
#if 1
		  //strcpy( tisiFileShortpath, (char *)recorderpath);
		   // strcat( tisiFileShortpath, (char *)&cur_recorder_path);
			//strcat( tisiFileShortpath, (char *)"\\");
		   //strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );

		  // strcpy(temp_name,( char *)fat_g_cur_nav->name );
		  // temp_name[1] = 'e';
		  // temp_name[2] = 'c';
		  // temp_name[6] = 'w';
		  // temp_name[7] = 'a';
		  // temp_name[8] = 'v';
		  // temp_name[9] = 0;
		   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
		   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
		   //lcd_goto_xy( 15, 8 );
		   lcd_goto_xy( 15, 26);
		   Ih_set_curr_font( IDC_FONT_16 );
		  // lcd_puts( g_string );
		   lcd_puts((U8 *)recorder_file_name);
		   mmi_recorder_display_icon();
//			 mmirecorder_play_time();
		  // return TRUE;

#else
      SDKLoadCode( MMI_RECORDER_TEMP_CODE07 );
      ( void )find_next_rec_file( rec_ext );
#endif	  
      //SDKLoadCode( REC_DIR_END_TEMP_CODE00 );
      //( void )reload_rec_file_dir();

      SendString((U8 *)"\r\nopen rec");
    

	 // SendString((U8 *)"\r\nopen_name=");SendString((U8 *)tisiFileShortpath);
      ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileName, FS_MODE_READ_WRITE, &err );
	  SendString((U8 *)"\r\nerr==");
	  send_hex_u8(err);
   }
   if( err )
   {
      FSClose(( __X FCFILE *)ADPCM_File );
      return FALSE;
   }

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
   Ih_set_curr_font( IDC_FONT_16 );
   //lcd_goto_xy( 15, 8 );
   lcd_goto_xy( 15, 26 );
   lcd_puts(( U8 *)recorder_file_name );

//   strcpy( tisiFileShortpath, recorderpath );
//   strcat( tisiFileShortpath, "\\" );
//   strcat( tisiFileShortpath, recorder_file_name );

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
   strcpy( tisiFileShortpath, tisiFileName );
   return TRUE;
}

#pragma CODE = REREAD_MENU_INIT_CODE
void reread_menu_init(void)
{
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
  	lcd_goto_xy( 0, 8);
	Ih_set_curr_font( IDC_FONT_16 );
//lcd_puts(( U8 *)"MP3播放器" );
    switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
	      lcd_puts(( U8 *)"Recorder");
	      break;
	      
	      case LANG_CHINESE_SIMP:
	      lcd_puts(( U8 *)"自主录音");
	      break;
	   }
}

#pragma CODE = MMI_RECORDER_TEMP_CODE11

void record_fail_deal( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   Ih_set_curr_font( IDC_FONT_16 );
   switch( record_result )
   {
      case RECORD_ERROR:
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy( 16, 12 );
      lcd_puts(( U8 *)"Record error" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"录音错误" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"魁岿粇" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
	  lcd_goto_xy( 15, 26 );
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
//         lcd_goto_xy( 16, 12 );
         lcd_puts(( U8 *)"Record error" );
         break;

         case LANG_CHINESE_SIMP:
//         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"录音错误" );
         break;
/*
         case LANG_CHINESE_TRAD:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"魁岿粇" );
         break;
*/		 
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      break;

      case RECORD_FILE_FULL:
      case RECORD_FILE_ERROR:
      case RECORD_MEMORY_FULL:
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"空间不足" );
#elif( LANGUAGE == Traditional )
      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"丁ぃì" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
      lcd_goto_xy( 15, 26 );
      switch( g_setting_list.struct_setting.language_ihm )
      {
         case LANG_ENGLISH:
//         lcd_goto_xy( 20, 12 );
         lcd_puts(( U8 *)"Memory full" );
         break;

         case LANG_CHINESE_SIMP:
//         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"空间不足" );
         break;
/*
         case LANG_CHINESE_TRAD:
         lcd_goto_xy( 32, 12 );
         lcd_puts(( U8 *)"丁ぃì" );
         break;
*/		 
      }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
      break;
   }
}


#pragma CODE = MMI_RECORDER_TEMP_CODE12

void record_eof_deal( void )
{
   record_result = RECORD_MEMORY_FULL;
   Ts_free( s_ts_display_id );
   //power_authorize_autopoweroff();
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8) );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 20, 12 );
   lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"空间不足" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 32, 12 );
   lcd_puts(( U8 *)"丁ぃì" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   lcd_goto_xy( 15, 26 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
//      lcd_goto_xy( 20, 12 );
      lcd_puts(( U8 *)"Memory full" );
      break;

      case LANG_CHINESE_SIMP:
//      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"空间不足" );
      break;

      case LANG_CHINESE_TRAD:
//      lcd_goto_xy( 32, 12 );
      lcd_puts(( U8 *)"丁ぃì" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )#endif
}

#endif



#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE00

void power_key_press_deal( void )
{
   if( g_b_recorder_on ){
      return;
   	}
   
   switch( srvplayer_g_player_state )
   {
      case MMI_PLAYER_STATE_PLAY :
      SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
      ( void )srvplayer_pause();
      mmi_recorder_display_icon();
      power_authorize_autopoweroff();
      break;

      case MMI_PLAYER_STATE_PAUSE :
      SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
      ( void )srvplayer_restart_play();
      mmi_recorder_display_icon();
      power_forbid_autopoweroff();
      break;
      
      case MMI_PLAYER_STATE_STOP :
	  	SendString((U8 *)"\r\n1915");
      if( recorder_file == FALSE )
         break;
      if( record_result != RECORD_OK )
      {
          //SendString((U8 *)"\r\n1920");
         record_result = RECORD_OK;
         //SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
         //srvplayer_stop();       // in case of
         Ts_free( mmirecorder_play_id_timerdisplay );
         SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
         if( !find_rec_file( rec_ext ))
         {
            SendString((U8 *)"\r\n1928");
            SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
            mmi_recorder_no_file();
            break;
         }
         break;
      }
      SendString((U8 *)"\r\nplay_file=");
	  SendString((U8 *)tisiFileShortpath);
      audio_is_wav = 1;
      SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
      if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
      {
         //mmiplayer_b_restartplay = TRUE;
         mmirecorder_play_id_timerdisplay = ts_alloc();
         // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
         mmi_recorder_display_icon();
         //mmirecorder_play_time();
         mmirecorder_display_time(PLAY_BEGIN_TIME);
		  mmirecorder_display_time(PLAY_ALL_TIME);
      }
      break;
   }
}


#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE15

void up_key_long_press_deal( void )
{
   if( g_b_recorder_on )
      return;
   
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   Ts_free( mmirecorder_play_id_timerdisplay );
   //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   if( record_result != RECORD_OK )
   {
      record_result = RECORD_OK;
      SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
      if( !find_rec_file( rec_ext ))
      {
         SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
         mmi_recorder_no_file();
         return;
      }
      goto up_key_long_play;
   }

   SDKLoadCode( MMI_RECORDER_TEMP_CODE07 );
   if( find_next_rec_file( rec_ext ))
   {
      goto up_key_long_play;
   }
   return;

up_key_long_play:
   audio_is_wav = 1;
   SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
   if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
   {
      //mmiplayer_b_restartplay = TRUE;
      mmirecorder_play_id_timerdisplay = ts_alloc();
      // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
      mmi_recorder_display_icon();
 //     mmirecorder_play_time();
 		mmirecorder_display_time(PLAY_BEGIN_TIME);
	    mmirecorder_display_time(PLAY_ALL_TIME);
   }
}


#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE16

void down_key_long_press_deal( void )
{
   if( g_b_recorder_on )
      return;
   
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   Ts_free( mmirecorder_play_id_timerdisplay );
   //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   if( record_result != RECORD_OK )
   {
      record_result = RECORD_OK;
      SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
      if( !find_rec_file( rec_ext ))
      {
         SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
         mmi_recorder_no_file();
         return;
      }
      goto down_key_long_play;
   }

   SDKLoadCode( MMI_RECORDER_TEMP_CODE08 );
   if( find_prev_rec_file( rec_ext ))
   {
      goto down_key_long_play;
   }

down_key_long_play:
   audio_is_wav = 1;
   SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
   if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
   {
      //mmiplayer_b_restartplay = TRUE;
      mmirecorder_play_id_timerdisplay = ts_alloc();
      // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
      mmi_recorder_display_icon();
//      mmirecorder_play_time();
		mmirecorder_display_time(PLAY_BEGIN_TIME);
		mmirecorder_display_time(PLAY_ALL_TIME);
   }
}




#pragma CODE = MMI_RECORDER_USB_CODE

void mmi_recorder_usb_deal( void )
{
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_ts_display_id );
   }
   //player_off();
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();
   Ts_free( mmirecorder_play_id_timerdisplay );
   //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
   //srvplayer_switch_off();  // Power off player
   //g_select_appli = MSTORAGE_APPLI;
   //Mmgr_kill_this_mmi_appli();
   //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
   SDKLoadCode( AUADC_COMMON_CODE );
   AUADC_Power_Down();
   CommonLoadCode( C_USB );
}


#pragma CODE = RECORDER_REMOTE_TEMP_CODE00

void remote_rec_play_deal( void )
{
	SendString((U8 *)"\r\nremote_rec_play_deal");
   if( g_b_recorder_on )
      return;

#if 1

	switch( srvplayer_g_player_state )
	{
	   case MMI_PLAYER_STATE_PLAY :
	   	/*
	   SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
	   ( void )srvplayer_pause();
	   mmi_recorder_display_icon();
	   power_authorize_autopoweroff();
	   */
	   break;

	   case MMI_PLAYER_STATE_PAUSE :
	   	/*
	   SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
	   ( void )srvplayer_restart_play();
	   mmi_recorder_display_icon();
	   power_forbid_autopoweroff();
	   */
	   break;
	   
	   case MMI_PLAYER_STATE_STOP :
		 SendString((U8 *)"\r\n2444");
	  // if( recorder_file == FALSE )
		//  break;
	  // if( record_result != RECORD_OK )
	   {
		   //SendString((U8 *)"\r\n1920");
		  record_result = RECORD_OK;
		  //SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
		  //srvplayer_stop();		// in case of
		  Ts_free( mmirecorder_play_id_timerdisplay );
		 //SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
         //if( !find_rec_file( rec_ext ))
         SDKLoadCode( MMI_RECORDER_TEMP_CODE10 );
         if( !find_index_rec_file( rec_ext ))
		  {
			 SendString((U8 *)"\r\n2457");
			 SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
			 mmi_recorder_no_file();
			 break;
		  }
		  //break;
	   }
	   SendString((U8 *)"\r\nplay_file=");
	   SendString((U8 *)tisiFileShortpath);
	   audio_is_wav = 1;
	   SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
	   if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
	   {
		  //mmiplayer_b_restartplay = TRUE;
		  mmirecorder_play_id_timerdisplay = ts_alloc();
		  // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
		  ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
		  mmi_recorder_display_icon();
		  //mmirecorder_play_time();
		  mmirecorder_display_time(PLAY_BEGIN_TIME);
		   mmirecorder_display_time(PLAY_ALL_TIME);
	   }
	   break;
	}

#else
   switch( srvplayer_g_player_state )
   {
      /*case MMI_PLAYER_STATE_PLAY :
      srvplayer_pause();
      mmi_recorder_display_icon();
      power_authorize_autopoweroff();
      break;

      case MMI_PLAYER_STATE_PAUSE :
      srvplayer_restart_play();
      mmi_recorder_display_icon();
      power_forbid_autopoweroff();
      break;*/
         
      case MMI_PLAYER_STATE_STOP :
      //if( recorder_file == FALSE )
      //   break;
     // if( record_result != RECORD_OK )
      {
         record_result = RECORD_OK;
         //SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
         //srvplayer_stop();       // in case of
        // Ts_free( mmirecorder_play_id_timerdisplay );
         //SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
         //if( !find_rec_file( rec_ext ))
         SDKLoadCode( MMI_RECORDER_TEMP_CODE10 );
         if( !find_index_rec_file( rec_ext ))
         {
            SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
            mmi_recorder_no_file();
            break;
         }
         break;
      }
	  SendString((U8 *)"\r\nplay_file=");
	  SendString((U8 *)tisiFileShortpath);
      audio_is_wav = 1;
      SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
      if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
      {
         //mmiplayer_b_restartplay = TRUE;
         mmirecorder_play_id_timerdisplay = ts_alloc();
         // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
         mmi_recorder_display_icon();
 //        mmirecorder_play_time();
 		mmirecorder_display_time(PLAY_BEGIN_TIME);
		  mmirecorder_display_time(PLAY_ALL_TIME);
      }
      else
         mail_send_event( EVT_PLAYER, PLAYER_EOF );
      break;
   }
#endif   
}


#pragma CODE = RECORDER_REMOTE_TEMP_CODE06

void remote_prev_deal( void )
{
   if( g_b_recorder_on )
      return;

   switch( srvplayer_g_player_state )
   {
      case MMI_PLAYER_STATE_PLAY:
      case MMI_PLAYER_STATE_STOP:
      case MMI_PLAYER_STATE_PAUSE:
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();       // in case of
      Ts_free( mmirecorder_play_id_timerdisplay );
      //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
      if( record_result != RECORD_OK )
      {
         record_result = RECORD_OK;
         SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
         if( !find_rec_file( rec_ext ))
         {
            SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
            mmi_recorder_no_file();
            break;
         }
         break;
      }

      SDKLoadCode( MMI_RECORDER_TEMP_CODE08 );
      if( find_prev_rec_file( rec_ext ))
      {
         audio_is_wav = 1;
         SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
         if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
         {
            //mmiplayer_b_restartplay = TRUE;
            mmirecorder_play_id_timerdisplay = ts_alloc();
            // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
            mmi_recorder_display_icon();
//            mmirecorder_play_time();
			mmirecorder_display_time(PLAY_BEGIN_TIME);
		   mmirecorder_display_time(PLAY_ALL_TIME);
         }
      }
      break;
   }
}


#pragma CODE = RECORDER_REMOTE_TEMP_CODE09

void remote_next_deal( void )
{
   if(g_b_recorder_on )
      return;

   switch( srvplayer_g_player_state )
   {
      case MMI_PLAYER_STATE_PLAY:
      case MMI_PLAYER_STATE_STOP:
      case MMI_PLAYER_STATE_PAUSE:
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();       // in case of
      Ts_free( mmirecorder_play_id_timerdisplay );
      //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
      if( record_result != RECORD_OK )
      {
         record_result = RECORD_OK;
         SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
         if( !find_rec_file( rec_ext ))
         {
            SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
            mmi_recorder_no_file();
            break;
         }
         break;
      }

      SDKLoadCode( MMI_RECORDER_TEMP_CODE07 );
      if( find_next_rec_file( rec_ext ))
      {
         audio_is_wav = 1;
         SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
         if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
         {
            //mmiplayer_b_restartplay = TRUE;
            mmirecorder_play_id_timerdisplay = ts_alloc();
            // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
            mmi_recorder_display_icon();
//            mmirecorder_play_time();
			mmirecorder_display_time(PLAY_BEGIN_TIME);
		    mmirecorder_display_time(PLAY_ALL_TIME);
         }
      }
      break;
   }
}


//#pragma CODE = JUDGE_INDEX_ACT_CODE

#pragma CODE = RECORDER_STAR_TEMP_CODE00

void recorder_star_appli_deal( void )
{
   //SendString((U8 *)"recorder\r\n");
   power_authorize_autopoweroff();
   mmgr_set_appli_full_screen();
   SDKLoadCode( MMI_RECORDER_TEMP_CODE01 );
   mmirecorder_display_background();
   SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
   unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));

#if( OID == ENABLE )
 SendString((U8 *)"\r\noid init!");
  // b_oid_init_flag = FALSE;
   if( !b_oid_init_flag )
   {
      reinit_oid_pen();
   }
#endif   //#if( OID == ENABLE )

   Kbd_set_evt_enabled();
   if( Kbd_is_event_triggered())
      Kbd_set_evt_disabled();
   
   scan_oid_disable();
   //scan_oid_enable();
   record_result = RECORD_OK;
   rec_play_state = REC_PLAY_ERROR;

	
/*   
   SendString((U8 *)"\r\nmode=");
   send_hex_u8(reread_state);
   if( reread_state == REREAD_REC )
   {
      //SDKLoadCode( MMI_PLAYINTRO_CODE00 );
      // Search file to play
      //search_subdir_mp3_file( "record.mp3" );
      mail_send_event( EVT_START_RECORDER_APPLI, 1 );
      //break;
   }
   else
   {
      SDKLoadCode( MMI_RECORDER_TEMP_CODE00 );
      if( !mmi_recorder_playintro())
      {
         mail_send_event( EVT_STOP_RECORDER_APPLI, 0 );
      }
   }*/
    SDKLoadCode( MMI_RECORDER_TEMP_CODE00 );
      if( !mmi_recorder_playintro())
      {
         mail_send_event( EVT_STOP_RECORDER_APPLI, 0 );
      }
   recorder_mode = 0;
}


#pragma CODE = RECORDER_STAR_TEMP_CODE01

void recorder_appli_init_deal( U8 mode )
{
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
   bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )

   SDKLoadCode( NAVIGATION_TEMP_CODE05 );
   file_init();
   SDKLoadCode( SRV_PLAYER_TEMP_CODE00 );
   ( void )srvplayer_switch_on();
   
   scan_oid_enable();
   power_authorize_autopoweroff();
   //Kbd_set_evt_enabled();
   recorder_file = TRUE;


   
   SDKLoadCode( MMI_RECORDER_TEMP_CODE06 );
   if( !find_rec_file( rec_ext ))
   {
      SDKLoadCode( MMI_RECORDER_TEMP_CODE05 );
      mmi_recorder_no_file();
   }
   
    Current_Index = reread_num;
   //(void)judge_current();
 #if 1
 	mail_send_event( EVT_READY_RECORDER_APPLI, 0);
 #else
   if( mode == 1 )
      mail_send_event( EVT_START_RECORDER_APPLI, 0 );
 #endif  
}


#pragma CODE = MMI_REREAD_KEYGENDU_CODE

void gendu_key_press_deal( void )
{
#if 1
	if(g_b_recorder_on){
		if( recorder_wait_time && !Timer_is_timeout( recorder_wait_time ))
         return;
		  srvrec_stop();
		  Ts_free( s_ts_display_id );
		  //power_authorize_autopoweroff();
		  mmi_recorder_display_icon();
		  //status_authorize_display();
		  //Mmgr_kill_this_mmi_appli();
		  SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
		 // lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8) );
		  lcd_clear_part_of_screen( 4, 32, 8, 8 );
//		  mmirecorder_disrecord_time();
		  return;
		}
	SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
    srvplayer_stop();
    Ts_free( mmirecorder_play_id_timerdisplay );
    mail_send_event( EVT_START_RECORDER_APPLI, 0 );
#else
   if( !g_b_recorder_on )
   {
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();
      Ts_free( mmirecorder_play_id_timerdisplay );
      mail_send_event( EVT_START_RECORDER_APPLI, 0 );
   }
   else
   {
      if( recorder_wait_time && !Timer_is_timeout( recorder_wait_time ))
         return;
      srvrec_stop();
      Ts_free( s_ts_display_id );
      //power_authorize_autopoweroff();
      mmi_recorder_display_icon();
      //status_authorize_display();
      //Mmgr_kill_this_mmi_appli();
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
     // lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8) );
      lcd_clear_part_of_screen( 4, 23, 8, 8 );
      mmirecorder_disrecord_time();
	  
   }
#endif 
}
#pragma CODE = MMI_REREAD_KEYDUIBI_CODE
void duibi_key_press_deal( void )
{
//   SendString((U8 *)"\r\n2297");
   if( g_b_recorder_on )
   {
      if( recorder_wait_time && !Timer_is_timeout( recorder_wait_time ))
         return;
      srvrec_stop();
      Ts_free( s_ts_display_id );
      //power_authorize_autopoweroff();
      mmi_recorder_display_icon();
      //status_authorize_display();
      //Mmgr_kill_this_mmi_appli();
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
     // lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8) );
      lcd_clear_part_of_screen( 4, 32, 8, 8 );
//      mmirecorder_disrecord_time();
//	  SendString((U8 *)"\r\n2298");
   }
   
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   Ts_free( mmirecorder_play_id_timerdisplay );
   //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
   audio_is_wav = 1;
//   SendString((U8 *)"\r\n2306");
   SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
   if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
   {
      //mmiplayer_b_restartplay = TRUE;
//      SendString((U8 *)"\r\n2311");
      mmirecorder_play_id_timerdisplay = ts_alloc();
      // ts_set_time( mmirecorder_play_id_timerdisplay, PLAYER_TIME );
         ts_set_time( mmirecorder_play_id_timerdisplay, TS_FLASH_TIME);
      mmi_recorder_display_icon();
 //     mmirecorder_play_time();
 		mmirecorder_display_time(PLAY_BEGIN_TIME);
	    mmirecorder_display_time(PLAY_ALL_TIME);
   }
   
}
#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE21

void power_key_long_press_deal( void )
{
   if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_ts_display_id );
   }
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();
   Ts_free( mmirecorder_play_id_timerdisplay );
   //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
   //srvplayer_switch_off();  // Power off player
   //power_authorize_autopoweroff();

   if( power_is_vbus_present())
   {
      //if( Usb_connect )
         mail_send_event( EVT_USB_POWERED, 0 );
      return;
   }
   g_select_appli = POWER_OFF_APPLI;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   g_power_off_mode = POWER_OFF;

   Mmgr_kill_this_mmi_appli();
}


#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE22

void men_key_long_press_deal( void )
{
 //  g_select_appli = PASTER_APPLI;
   g_select_appli = IDLE_APPLI;
   if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_ts_display_id );
   }
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();
   Ts_free( mmirecorder_play_id_timerdisplay );
   //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
   //srvplayer_switch_off();  // Power off player
   //power_authorize_autopoweroff();
   Mmgr_kill_this_mmi_appli();
}


#pragma CODE = MMI_RECORDER_KEY_TEMP_CODE23

void mode_key_long_press_deal( U8 appli_id )
{
#if 1
 if( g_b_recorder_on )
   {
      srvrec_stop();
      Ts_free( s_ts_display_id );
   }
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();
   Ts_free( mmirecorder_play_id_timerdisplay );

 

   
   	g_select_appli = appli_id;
    Mmgr_kill_this_mmi_appli();
#else
   if( !g_b_recorder_on )
   {
#if( PASTER_MODE == ENABLE )
      g_select_appli = PASTER_APPLI;
#else
      //g_select_appli = SETTING_APPLI;
      g_select_appli = IDLE_APPLI;
#endif   //#if( PASTER_MODE == ENABLE )
      //goto mmi_recorder_exit;
      SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
      srvplayer_stop();
      Ts_free( mmirecorder_play_id_timerdisplay );
      //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
      //srvplayer_switch_off();  // Power off player
      //power_authorize_autopoweroff();
      //g_select_appli = AP3_APPLI;
      //g_select_appli = DIC_APPLI;
      Mmgr_kill_this_mmi_appli();
   }
#endif   
}


#pragma CODE = MMI_AP4_BAT_LOW_CODE

void bat_low_waring_deal( U16 param )
{
#if 1
	U8 temp_buf[32];
	
	if(g_b_recorder_on ){
	   srvrec_stop();
	   Ts_free( s_ts_display_id );
	}
	
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   Ts_free( mmirecorder_play_id_timerdisplay );
   
   // Search file to play

   if( 0 == param ){
   	SendString((U8 *)"\r\nW!");
	strcpy((char *)temp_buf,(char *)LOW_BAT_WARNING);
   	}
   
   else 
   		return;

   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   if( !search_subdir_mp3_file( (char *)temp_buf ))
   {
      return;
   }
   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
   {
      return ;
   }
   rec_play_state = REC_PLAY_BAT_WARN;
   Kbd_set_evt_disabled();
   scan_oid_disable();
#else
   if( !g_b_recorder_on )
      return;
   srvrec_stop();

   Ts_free( s_ts_ap3record_display_id );
   mmi_ap3_kbd_enable();
   scan_oid_enable();
#endif   
}

