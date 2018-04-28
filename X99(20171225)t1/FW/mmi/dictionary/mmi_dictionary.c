#ifndef _mmi_dictionary_c_
#define _mmi_dictionary_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"

#include "lib_system\unicode\unicode.h"

#include "modules\file_system\fs_com.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "mmi\kbd\keypad.h"

#include "modules/text/text.h"
#include "modules/text/text_base.h"
#include "modules/text/text_viewer.h"
#include "modules/text/text_task.h"

#include "mmi_dictionary.h"
#include "mmi/shared/com_var.h"

#include "drivers\oid\oid.h"
#include "mmi\player\srv_player.h"
#include "modules\player\player.h"
#include "mmi\status\mmi_status.h"
#include "modules\player_ap3\player_ap3.h"


#include "conf\conf_lcd.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_sio.h"
//-------------C标准库-------------
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "modules\search\search_remote.h"
#include "modules\power\power.h"             // Power module definition
#include "mmi\startup\mmi_startup.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#include "point_g.h"


#pragma DATA = MMI_DIC_DATA
#pragma CODE = MMI_DIC_CODE

ImageDeclareType( MMI_DIC_TEMP_CODE00 );
ImageDeclareType( MMI_DIC_TEMP_CODE01 );
ImageDeclareType( MMI_DIC_TEMP_CODE02 );
ImageDeclareType( MMI_DIC_TEMP_CODE03 );
ImageDeclareType( MMI_DIC_TEMP_CODE04 );

ImageDeclareType( MMI_DIC_START_INIT_CODE );
ImageDeclareType( MMI_DIC_EXIT_CODE );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( MMI_DIC_KEY_PRESS_CODE );
ImageDeclareType( MMI_DIC_PLAY_EOF_CODE );
ImageDeclareType( MMI_DIC_REMOTE_CODE );
ImageDeclareType( MMI_DIC_USB_CODE );
ImageDeclareType( MMI_DIC_POWEROFF_CODE );
ImageDeclareType( POWER_TASK_TEMP_CODE05 );
ImageDeclareType( MMI_DIC_VOL_CODE );
ImageDeclareType( TEXT_CH_DISPLAY_CODE );
ImageDeclareType( T01_COMMAND_CODE );
ImageDeclareType( GLOBEKEY_DOWITH_CODE );
#if( DIC == ENABLE )
//_____ P R I V A T E   D E F I N I T I O N S ______________________________
typedef enum
{
   DIC_PLAY_MODE
,  DIC_PLAY_NO_FILE
,  DIC_PLAY_ERROR
,  DIC_PLAY_BAT_WARN
}DIC_STATE;
_MEM_TYPE_SLOW_   U8    dic_play_state;

                  Bool  b_kbd_process_disable;

// ID of the timer soft used by this MMI appli
static _MEM_TYPE_SLOW_  U8       s_ts_id;

Bool  mmi_dic_playintro          ( void );
Bool  mmi_dicno_file_playintro   ( void );
void  dic_no_adb_file_deal       ( void );
void  mmi_dic_no_file_display    ( void );
void bat_low_waring_deal( U16 param );
#if( LCD_PRESENT == ENABLE )
void  mmidic_display_backgroung  ( void );
void  mmi_dic_display_volume     ( void );
#else
#define mmidic_display_backgroung()
#define mmi_dic_display_volume()
#endif   //#if( LCD_PRESENT == ENABLE )

char *convert_U8                 ( char *p, U8 i );
void  dic_start_appli_init       ( void );
void  dic_start_deal             ( void );
void  dic_exit_deal              ( void );
void  mmi_dic_key_press_deal     ( U16 param );
void  mmi_dic_player_eof_deal    ( void );
void  mmi_dic_remote_deal        ( U8 param );
void  mmi_dic_usb_deal           ( void );
void  mmi_dic_poweroff_deal      ( void );
void  dic_vol_dowith			 (U16 param);


extern   U8    _MEM_TYPE_SLOW_   dictext_g_cmd_id;
extern U8       _MEM_TYPE_SLOW_   dictext_g_state;
extern void  lcd_display_time		  ( Bool flag);
extern Bool text_chinese_display( U32 fan_add );
extern void set_oid_type(Bool set_type);
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________



//_____ D E F I N I T I O N S ______________________________________________
//!
//! dictionary_mmi_appli
//!
//! Manage the application dictionary
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void dic_mmi_appli( U8 event, U16 param )
{
   switch( event )
   {
      case EVT_START_APPLI:
      SDKLoadCode( MMI_DIC_START_INIT_CODE );
      dic_start_appli_init();
#if 0
      mmgr_set_appli_full_screen();
      SDKLoadCode( MMI_DIC_TEMP_CODE02 );
      mmidic_display_backgroung();

      s_ts_id = UNDEFINED_TIMER;
      
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
      
      SendString(( U8 *)"dic\r\n");
      scan_oid_disable();
      mmi_dic_kbd_disable();
      dic_play_state = DIC_PLAY_ERROR;
      SDKLoadCode( MMI_DIC_TEMP_CODE00 );
      if( !mmi_dic_playintro())
      {
         mail_send_event( EVT_START_DIC_APPLI, 0 );
      }
      //else
      //   dic_play_state = DIC_PLAY_MODE;
#endif
      break;

      case EVT_START_DIC_APPLI:
      SDKLoadCode( MMI_DIC_TEMP_CODE04 );
      dic_start_deal();
#if 0
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
      //srvplayer_volume_send();

      //mmi_file_flag &= ~ADB_NO_FILE_FLAG;
#if( PASTER_MODE == DISABLE )
      //mmi_file_flag |= APF_NO_FILE_FLAG;
#endif   //#if( PASTER_MODE == DISABLE )

      power_authorize_autopoweroff();
      SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
      unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
      Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));
      
      dictext_task_start();
      dictext_evt_init();
      dictext_send_cmd( DICTEXT_Cmd_Start, 0 );
      //scan_oid_enable();
      dic_file_open_flag = FALSE;
      // TO DO
#endif
      break;
	  case EVT_GOTO_OID_MODE:
	  	//exit_dictext_to_appli = AP3_APPLI;
	  	 g_select_appli = exit_dictext_to_appli;
	  	mail_send_event( EVT_APPLI_KILLED, ((( U16 )TASK_DIC )<< 8 )|( U16 )exit_dictext_to_appli );
   		dictext_g_state = DICTEXT_State_Idle;
   		break;
      case EVT_APPLI_KILLED:
      // TO DO
      if( MSB( param )== TASK_DIC )
      {
         SDKLoadCode( MMI_DIC_EXIT_CODE );
         dic_exit_deal();
#if 0
         Ts_free( s_ts_id );
         if( player_ap3_is_busy())
            player_ap3_stop();
         dictext_task_stop();
         scan_oid_disable();
         SDKLoadCode( FILE_TEMP_CODE01 );
         file_close();
         switch( exit_dictext_to_appli )
         {
            case MSTORAGE_APPLI:
            //g_select_appli = exit_dictext_to_appli;
            //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
            CommonLoadCode( C_USB );
            break;

            case RECORDER_APPLI:
            //g_select_appli = g_old_select_appli = RECORDER_APPLI;
            //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
            recorder_mode = 1;
            break;

            default:
            g_select_appli = exit_dictext_to_appli;
            break;
         }
         Mmgr_kill_this_mmi_appli();
#endif
      }
      break;

      /*case EVT_BACK_TO_TOP:
      // TO DO
      break;*/

	   case EVT_DIC_FLUSH_TEXT:
	 // SendString((U8 *)"\r\nflushtext");
	  SDKLoadCode( TEXT_CH_DISPLAY_CODE );
	  (void)text_chinese_display(ch_display_address);
	  //SendString((U8 *)"\r\naddr:page_num:off=");
	 // send_hex_u32(ch_display_address);SendString((U8 *)":");
     //send_hex_u16(Display_Page_Num);SendString((U8 *)":");
	 //send_hex_u16(text_off_addr[Display_Page_Num]);
	  break;

	   case EVT_BAT_WARING:
      SDKLoadCode( MMI_AP4_BAT_LOW_CODE );
      bat_low_waring_deal(param);
      break;

	  case EVT_DIC_VOL_CHANGE:
	  srvplayer_volume_send();
	  mmi_dic_display_volume();
	  
	  if(param != 0){
	 	 mail_send_event(EVT_DIC_VOL_PLAY,0);
	  	}
	  break;	
	  
	  case EVT_DIC_VOL_PLAY:
	  SDKLoadCode( MMI_DIC_VOL_CODE );
      dic_vol_dowith(param);
	  break;
	  case EVT_CHANGE_OID_TYPE:
			//SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
			//ap4_play_stop();
		if( player_ap3_is_busy()){
   			player_ap3_stop();
   			}
		 /*
		if(g_bluetooth){
			g_bluetooth = FALSE;
			}
		else{
			g_bluetooth = TRUE;
			}*/
		g_bluetooth = ~g_bluetooth;
		 SDKLoadCode(T01_COMMAND_CODE);
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


	  
      case EVT_KEY_PRESSED:
	  if( mmi_dic_kbd_is_disable())return;
      SDKLoadCode( MMI_DIC_KEY_PRESS_CODE );
      mmi_dic_key_press_deal( param );
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
      if( dic_play_state != DIC_PLAY_ERROR )
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
         if( mmi_dic_kbd_is_disable())
            break;
         mail_send_command(((( U16 )TASK_DIC )<< 8 )|( U16 )event, param );
         break;
      }
      if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_VOL_UP:
            srvplayer_volume_change( SRVPLAYER_VOL_UP );
            //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            mmi_dic_display_volume();
            break;

            case KEY_VOL_DOWN:
            srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            //srvplayer_volume_change( SRVPLAYER_VOL_UP );
            mmi_dic_display_volume();
            break;
         }
      }
#endif
      break;

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/

      case EVT_USB_POWERED:
      SDKLoadCode( MMI_DIC_USB_CODE );
      mmi_dic_usb_deal();
#if 0
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      Ts_free( s_ts_id );
      if( player_ap3_is_busy())
         player_ap3_stop();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      dictext_task_stop();
      scan_oid_disable();
      //g_select_appli = MSTORAGE_APPLI;
      //Mmgr_kill_this_mmi_appli();
      //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      CommonLoadCode( C_USB );
#endif
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_dic_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_dic_power_off:
      SDKLoadCode( MMI_DIC_POWEROFF_CODE );
      mmi_dic_poweroff_deal();
#if 0
      Ts_free( s_ts_id );
      if( player_ap3_is_busy())
         player_ap3_stop();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      dictext_task_stop();
      scan_oid_disable();
      g_select_appli = POWER_OFF_APPLI;
      Mmgr_kill_this_mmi_appli();
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif
      break;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      SDKLoadCode( MMI_DIC_REMOTE_CODE );
      mmi_dic_remote_deal( LSB( param ));
#if 0
      if( dic_play_state != DIC_PLAY_ERROR )
         break;
      switch( LSB( param ))
      {
         case REMOTE_VOL_UP:
         if( dic_file_open_flag )
            break;
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_dic_display_volume();
         break;
         
         case REMOTE_VOL_DOWN:
         if( dic_file_open_flag )
            break;
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_dic_display_volume();
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
         if(( LSB( param )- REMOTE_VOL0 )>( SRVPLAYER_VOL_LEVEL - 1 ))
            break;
         g_volume = LSB( param )- REMOTE_VOL0;
         srvplayer_volume_send();
         mmi_dic_display_volume();
         break;
         
         case REMOTE_POWER_OFF:
         if( dic_file_open_flag )
            break;
         if( power_is_vbus_present())
            break;
         Ts_free( s_ts_id );
         if( player_ap3_is_busy())
            player_ap3_stop();
         SDKLoadCode( FILE_TEMP_CODE01 );
         file_close();
         dictext_task_stop();
         scan_oid_disable();
         g_b_remote_on = FALSE;
         g_select_appli = POWER_OFF_APPLI;
         g_power_off_mode = POWER_OFF;
         Mmgr_kill_this_mmi_appli();
         break;
         
         /*case REMOTE_EXIT_MP3:
         case REMOTE_MP3_PLAY_PAUSE:
         case REMOTE_MP3_PREV:
         case REMOTE_MP3_NEXT:
         case REMOTE_DIC_MODE:
         case REMOTE_REC_START:
         case REMOTE_REC_PLAY:
         case REMOTE_REC_STOP:
         break;*/
         
         case REMOTE_CAL_MODE:
         bRemoteEnterCal = TRUE;
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
         exit_dictext_to_appli = AP3_APPLI;
         break;
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         bRemoteEnterCal = FALSE;
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
         exit_dictext_to_appli = AP3_APPLI;
         break;

#if( REMOTE_REC == ENABLE )
         case REMOTE_REC_MODE:
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
         exit_dictext_to_appli = RECORDER_APPLI;
         recorder_mode = 0;
         break;
#endif   //#if( REMOTE_REC == ENABLE )

#if( REMOTE_MP3 == ENABLE )
         case REMOTE_MP3_MODE:
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
         exit_dictext_to_appli = PLAYER_APPLI;
         break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
         case REMOTE_PASTER_MODE:
         dictext_g_cmd_id = DICTEXT_Cmd_Exit;
         exit_dictext_to_appli = PASTER_APPLI;
         break;
#endif   //#if( REMOTE_PASTER == ENABLE )
      }
#endif
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      case EVT_PLAYER:
      SDKLoadCode( MMI_DIC_PLAY_EOF_CODE );
      mmi_dic_player_eof_deal();
#if 0
      //if( param == PLAYER_EOF )
      {
         if( dic_play_state != DIC_PLAY_ERROR )
         {
            //power_authorize_autopoweroff();
            switch( dic_play_state )
            {
               case DIC_PLAY_MODE:
               dic_play_state = DIC_PLAY_ERROR;
               //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               //srvplayer_switch_off();  // Power off player
               mail_send_event( EVT_START_DIC_APPLI, 0 );
               break;

               case DIC_PLAY_NO_FILE:
               dic_play_state = DIC_PLAY_ERROR;
               //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               //srvplayer_switch_off();  // Power off player
               //if( player_ap3_is_busy())
               //   player_ap3_stop();
               //file_close();
               dictext_task_stop();
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
                  g_select_appli = SETTING_APPLI;
               bRemoteEnterCal = FALSE;
#else
               g_select_appli = IDLE_APPLI;
#endif
               Mmgr_kill_this_mmi_appli();
               break;
               
               default:
               dic_play_state = DIC_PLAY_ERROR;
               break;
            }
         }
      }
#endif
      break;

      case EVT_DIC_NO_FILE:
      SDKLoadCode( MMI_DIC_TEMP_CODE01 );
      dic_no_adb_file_deal();
      break;

      case EVT_TIMER:
      if( LSB( param ) == s_ts_id )
      {
         Ts_free( s_ts_id );
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //file_close();
         dictext_task_stop();
         scan_oid_disable();
         g_select_appli = IDLE_APPLI;
         Mmgr_kill_this_mmi_appli();
      }
      else
      {
         Mmgr_krn_forward_current_event();
      }
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

#pragma CODE = GLOBEKEY_DOWITH_CODE
Bool globekey_dowith(U16 param)
{
#if 0
#else
  if(param == ((KEY_POWER<<8)|KBD_KEY_LONGPRESSED)){
  	exit_dictext_to_appli = POWER_OFF_APPLI;
	goto new_panduan;
  	}
  else if(param == ((KEY_MODE<<8)|KBD_KEY_LONGPRESSED)){
  	exit_dictext_to_appli = IDLE_APPLI;
	goto new_panduan;
  	}
  else if(param == ((KEY_FANYI<<8)|KBD_KEY_LONGPRESSED)){
  	exit_dictext_to_appli = RECORDER_APPLI;
	goto new_panduan;
  	}/*
  else if(KEY_ID(param)== KEY_GENDU){
  	recorder_mode = 1;
	exit_dictext_to_appli = REREAD_APPLI;
	goto new_panduan;
  	}*/
  else if(KEY_ID(param)== KEY_REC){
  	//recorder_mode = 1;
	exit_dictext_to_appli = PLAYER_APPLI;
	goto new_panduan;
  	}/*
  else if(KEY_ID(param)== KEY_DUIBI){
  	recorder_mode = 0;
	exit_dictext_to_appli = REREAD_APPLI;
	goto new_panduan;
  	}*/
  else if(param == ((KEY_VOL_UP<<8)|KBD_KEY_RELEASED)){
  	srvplayer_volume_change( SRVPLAYER_VOL_UP);
	mmi_dic_display_volume();
	mail_send_event(EVT_DIC_VOL_PLAY,0);
	return TRUE;
  	}
  else if(param == ((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED)){
  	srvplayer_volume_change( SRVPLAYER_VOL_DOWN);
	mmi_dic_display_volume();
	mail_send_event(EVT_DIC_VOL_PLAY,0);
	return TRUE;
  	}
  else
  		return FALSE;
  
#endif  
new_panduan:
   mail_send_event( EVT_APPLI_KILLED, ((( U16 )TASK_DIC )<< 8 )|( U16 )exit_dictext_to_appli );
   dictext_g_state = DICTEXT_State_Idle;
   return TRUE;
}





#pragma CODE = MMI_DIC_TEMP_CODE00

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_dic_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
   // Search file to play
   if( !search_subdir_mp3_file( "dictionary.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   dic_play_state = DIC_PLAY_MODE;
   return TRUE;
}


#pragma CODE = MMI_DIC_TEMP_CODE01

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_dicno_file_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
   // Search file to play
   if( !search_subdir_mp3_file( "nodicfile.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   dic_play_state = DIC_PLAY_NO_FILE;
   return TRUE;
}



void dic_no_adb_file_deal( void )
{
   SDKLoadCode( MMI_DIC_TEMP_CODE03 );
   mmi_dic_no_file_display();
#if 0
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( DICTEXT_Content_PosX, DICTEXT_Content_PosY, DICTEXT_Screen_Width, 24 );
   Ih_set_curr_font( DICTEXT_BigFont );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
   lcd_puts(( U8 *)"No ADB file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
   lcd_puts(( U8 *)"无ADB文件" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
   lcd_puts(( U8 *)"礚ADBゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"No ADB file" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"无ADB文件" );
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"礚ADBゅン" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
#endif
   scan_oid_disable();
   mmi_dic_kbd_disable();
   dic_play_state = DIC_PLAY_ERROR;
   //mmi_file_flag |= ADB_NO_FILE_FLAG;
   if( !mmi_dicno_file_playintro())
   {
#if 0
      //if( player_ap3_is_busy())
      //   player_ap3_stop();
      //file_close();
      dic_task_stop();
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
         g_select_appli = AP3_APPLI;
      bRemoteEnterCal = FALSE;
      Mmgr_kill_this_mmi_appli();
#else
      s_ts_id = ts_alloc();
      ts_set_time( s_ts_id, SPLASH_TIME );
#endif
   }
   //else
   //   dic_play_state = DIC_PLAY_NO_FILE;
}


#pragma CODE = MMI_DIC_TEMP_CODE03

void mmi_dic_no_file_display( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_part_of_screen( DICTEXT_Content_PosX, DICTEXT_Content_PosY, DICTEXT_Screen_Width, (DICTEXT_Screen_Width - DICTEXT_Content_PosY) );
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
   Ih_set_curr_font( DICTEXT_BigFont );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
   lcd_puts(( U8 *)"No ADB file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
   //lcd_puts(( U8 *)"无ADB文件" );
    lcd_puts(( U8 *)"无词典数据文件" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
   lcd_puts(( U8 *)"礚ADBゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   lcd_goto_xy( 15, 24 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
//      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"No ADB file" );
      break;

      case LANG_CHINESE_SIMP:
      //lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      //lcd_puts(( U8 *)"无ADB文件" );
      lcd_puts(( U8 *)"无词典数据文件" );
      break;

      case LANG_CHINESE_TRAD:
      //lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"礚ADBゅン" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}

#if( LCD_PRESENT == ENABLE )

#pragma CODE = MMI_DIC_TEMP_CODE02

//! Draw the background
//!
void mmidic_display_backgroung( void )
{
   lcd_turn_on();
   Lcd_start_backlight_timer();
   
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   //lcd_clear_screen();
   lcd_clear_part_of_screen(0, 8,LCD_WIDTH, (LCD_HEIGHT-8));
   lcd_clear_part_of_screen(20, 0,64, 8);
   lcd_goto_xy( 20, 0 );
   Ih_set_curr_font( IDC_FONT_08);
   lcd_puts(( U8 *)"DIC" );
   
   lcd_goto_xy( 24, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 20, FILEBROWSER_PosFile_StartY );
       lcd_puts(( U8 *)"Dictionary" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( FILEBROWSER_PosFile_StartX + 28, FILEBROWSER_PosFile_StartY );
      lcd_puts(( U8 *)"电子词典" );
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )   
  
   
/*
   lcd_goto_xy( 34, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"DICTIONARY" );
 */  
   status_display();

   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
   mmi_dic_display_volume();

   SDKLoadCode(POWER_TASK_TEMP_CODE05);
   lcd_display_time(FALSE);
}


#pragma CODE = MMI_DIC_CODE

//! @brief Display volume
//!
void mmi_dic_display_volume( void )
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
   strcat(( char *)string, ( char *)&string[ 4 ]);
   lcd_puts( string );
}

#endif   //#if( LCD_PRESENT == ENABLE )


#pragma CODE = MMI_DIC_START_INIT_CODE

void dic_start_appli_init( void )
{
   mmgr_set_appli_full_screen();
   SDKLoadCode( MMI_DIC_TEMP_CODE02 );
   mmidic_display_backgroung();

   s_ts_id = UNDEFINED_TIMER;

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

   SendString(( U8 *)"\r\ndic");
   b_clusterlist = FALSE;
   scan_oid_disable();
   mmi_dic_kbd_disable();
   dic_play_state = DIC_PLAY_ERROR;
   SDKLoadCode( MMI_DIC_TEMP_CODE00 );
   if( !mmi_dic_playintro())
   {
      mail_send_event( EVT_START_DIC_APPLI, 0 );
   }
   //else
   //   dic_play_state = DIC_PLAY_MODE;
}


#pragma CODE = MMI_DIC_TEMP_CODE04

void dic_start_deal( void )
{
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
   bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
   //srvplayer_volume_send();

   //mmi_file_flag &= ~ADB_NO_FILE_FLAG;
#if( PASTER_MODE == DISABLE )
   //mmi_file_flag |= APF_NO_FILE_FLAG;
#endif   //#if( PASTER_MODE == DISABLE )
   power_authorize_autopoweroff();
   SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
   unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
   Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));

   dictext_task_start();
   dictext_evt_init();
   dictext_send_cmd( DICTEXT_Cmd_Start, 0 );
   //scan_oid_enable();
   dic_file_open_flag = FALSE;
}


#pragma CODE = MMI_DIC_EXIT_CODE

void dic_exit_deal( void )
{
   Ts_free( s_ts_id );
   if( player_ap3_is_busy())
      player_ap3_stop();
   dictext_task_stop();
   scan_oid_disable();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   switch( exit_dictext_to_appli )
   {
      case MSTORAGE_APPLI:
      //g_select_appli = exit_dictext_to_appli;
      //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      CommonLoadCode( C_USB );
      break;

      //case RECORDER_APPLI:
      //g_select_appli = g_old_select_appli = RECORDER_APPLI;
      //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
      //recorder_mode = 1;
      //break;

      default:
      g_select_appli = exit_dictext_to_appli;
      break;
   }
   Mmgr_kill_this_mmi_appli();
}


#pragma CODE = MMI_DIC_KEY_PRESS_CODE
void mmi_dic_key_press_deal( U16 param )
{
#if( LCD_PRESENT == ENABLE )
#  if(BACKLIGHT == ENABLE)
   if( g_mmi_lcd_backlight_off )
   {
      if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
         g_mmi_lcd_backlight_off = FALSE;
      return;
   }
#  endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
  	
   SendString((U8 *)"\r\ndic_key = ");
   send_hex_u16(param);
   SDKLoadCode(GLOBEKEY_DOWITH_CODE);
   if(globekey_dowith(param))return;
   if( dic_play_state != DIC_PLAY_ERROR )
   {
      if(( KBD_KEY_PRESSED | KBD_KEY_LONGPRESSED | KBD_KEY_REPEAT | KBD_KEY_REPEAT_RELEASED )
         & KEY_STATUS( param ))
      {
         return;
      }
      if( KBD_KEY_RELEASED & KEY_STATUS( param ))
      {
         if(( KEY_ID( param )!= KEY_VOL_UP )
            &&( KEY_ID( param )!= KEY_VOL_DOWN ))
            return;
      }
   }
   else
   {
      if( mmi_dic_kbd_is_disable())
         return;
	  
      mail_send_command(((( U16 )TASK_DIC )<< 8 )|( U16 )EVT_KEY_PRESSED, param );
      return;
   }
/*
   if( KBD_KEY_RELEASED == KEY_STATUS( param ))
   {
      switch( KEY_ID( param ))
      {
         case KEY_VOL_UP:
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_dic_display_volume();
         break;

         case KEY_VOL_DOWN:
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         //srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_dic_display_volume();
         break;

		 case KEY_FANYI:
			g_select_appli = RECORDER_APPLI;
			goto ready_back;
			break;

			case KEY_GENDU:
			recorder_mode = 1;
			g_select_appli = REREAD_APPLI;
			goto ready_back;
			break;

			case KEY_DUIBI:
			recorder_mode = 0;
			g_select_appli = REREAD_APPLI;
ready_back:			
			 Mmgr_kill_this_mmi_appli();
			break;

		 case KEY_NEXT:
		 	break;
		 case KEY_PREV:
		 	break;
			
      }
   }
 */
}


#pragma CODE = MMI_DIC_PLAY_EOF_CODE

void mmi_dic_player_eof_deal( void )
{
   if( dic_play_state != DIC_PLAY_ERROR )
   {
      //power_authorize_autopoweroff();
      switch( dic_play_state )
      {
         case DIC_PLAY_MODE:
         dic_play_state = DIC_PLAY_ERROR;
         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //srvplayer_switch_off();  // Power off player
         mail_send_event( EVT_START_DIC_APPLI, 0 );
         break;

         case DIC_PLAY_NO_FILE:
         dic_play_state = DIC_PLAY_ERROR;
         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //srvplayer_switch_off();  // Power off player
         //if( player_ap3_is_busy())
         //   player_ap3_stop();
         //file_close();
         dictext_task_stop();
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
            g_select_appli = SETTING_APPLI;
         bRemoteEnterCal = FALSE;
#else
         g_select_appli = IDLE_APPLI;
#endif
         Mmgr_kill_this_mmi_appli();
         break;

		 case DIC_PLAY_BAT_WARN:
		 	
		 SendString((U8 *)"\r\n1105!");
		 dic_play_state = DIC_PLAY_ERROR;
		 mmi_dic_kbd_enable();
         scan_oid_enable();
		 break;	
         default:
         dic_play_state = DIC_PLAY_ERROR;
         break;
      }
   }
}


#pragma CODE = MMI_DIC_REMOTE_CODE

void mmi_dic_remote_deal( U8 param )
{
   if( dic_play_state != DIC_PLAY_ERROR )
      return;
   SendString((U8 *)"\r\nr_p=");
   send_hex_u8(param);
   switch( param )
   {
      case REMOTE_VOL_UP:
      if( dic_file_open_flag )
         break;
      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      mmi_dic_display_volume();
	 // mail_send_event(EVT_DIC_VOL_PLAY,0);
      break;
      
      case REMOTE_VOL_DOWN:
      if( dic_file_open_flag )
         break;
      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      mmi_dic_display_volume();
	  //mail_send_event(EVT_DIC_VOL_PLAY,0);
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
  	
      if(( LSB( param )- REMOTE_VOL0 )>( VOL_LEVEL - 1 ))
         break;
      g_volume = LSB( param )- REMOTE_VOL0;
      srvplayer_volume_send();
      mmi_dic_display_volume();
	 // mail_send_event(EVT_DIC_VOL_PLAY,0);
      break;
      
      case REMOTE_POWER_OFF:
      if( dic_file_open_flag )
         break;
      if( power_is_vbus_present())
         break;
      Ts_free( s_ts_id );
      if( player_ap3_is_busy())
         player_ap3_stop();
      SDKLoadCode( FILE_TEMP_CODE01 );
      file_close();
      dictext_task_stop();
      scan_oid_disable();
      g_b_remote_on = FALSE;
      g_select_appli = POWER_OFF_APPLI;
      g_power_off_mode = POWER_OFF;
      Mmgr_kill_this_mmi_appli();
      break;
      
      /*case REMOTE_EXIT_MP3:
      case REMOTE_MP3_PLAY_PAUSE:
      case REMOTE_MP3_PREV:
      case REMOTE_MP3_NEXT:
      case REMOTE_DIC_MODE:
      case REMOTE_REC_START:
      case REMOTE_REC_PLAY:
      case REMOTE_REC_STOP:
      break;*/
      
      case REMOTE_CAL_MODE:
      bRemoteEnterCal = TRUE;
      dictext_g_cmd_id = DICTEXT_Cmd_Exit;
      exit_dictext_to_appli = AP3_APPLI;
      break;
      
      case REMOTE_AP4_MODE:
      //g_b_remote_on = FALSE;
      bRemoteEnterCal = FALSE;
      dictext_g_cmd_id = DICTEXT_Cmd_Exit;
      exit_dictext_to_appli = AP3_APPLI;
      break;

#if( REMOTE_REC == ENABLE )
      case REMOTE_REC_MODE:
      dictext_g_cmd_id = DICTEXT_Cmd_Exit;
      exit_dictext_to_appli = RECORDER_APPLI;
      recorder_mode = 0;
      break;
#endif   //#if( REMOTE_REC == ENABLE )

#if( REMOTE_MP3 == ENABLE )
      case REMOTE_MP3_MODE:
      dictext_g_cmd_id = DICTEXT_Cmd_Exit;
      exit_dictext_to_appli = PLAYER_APPLI;
      break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
      case REMOTE_PASTER_MODE:
      dictext_g_cmd_id = DICTEXT_Cmd_Exit;
      exit_dictext_to_appli = PASTER_APPLI;
      break;
#endif   //#if( REMOTE_PASTER == ENABLE )
   }
}


#pragma CODE = MMI_DIC_USB_CODE

void mmi_dic_usb_deal( void )
{
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   Ts_free( s_ts_id );
   if( player_ap3_is_busy())
      player_ap3_stop();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   dictext_task_stop();
   scan_oid_disable();
   //g_select_appli = MSTORAGE_APPLI;
   //Mmgr_kill_this_mmi_appli();
   //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
   CommonLoadCode( C_USB );
}


#pragma CODE = MMI_DIC_POWEROFF_CODE

void mmi_dic_poweroff_deal( void )
{
   Ts_free( s_ts_id );
   if( player_ap3_is_busy())
      player_ap3_stop();
   SDKLoadCode( FILE_TEMP_CODE01 );
   file_close();
   dictext_task_stop();
   scan_oid_disable();
   g_select_appli = POWER_OFF_APPLI;
   Mmgr_kill_this_mmi_appli();
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
   g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
}




#pragma CODE = MMI_DIC_VOL_CODE

void dic_vol_dowith( U16 param )
{
#if 1
	U8 temp_buf[32];
	
   if( player_ap3_is_busy()){
   	player_ap3_stop();
   	}
  // if(Point_Read != Point_Write)return;
//SendString((U8 *)"\r\nvol.mp3!");
   init_array_v();
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   
	strcpy((char *)temp_buf,(char *)"vol.mp3");
   	
   
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( (char *)temp_buf ))return;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())return ;
   scan_oid_disable();
   mmi_dic_kbd_disable();
   
   SendString((U8 *)"\r\nvol.mp3!");
   dic_play_state = DIC_PLAY_BAT_WARN;
#else
   
	  if( player_ap3_is_busy()){
	   player_ap3_stop():
	   }
	init_array_v();

   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   
   // Search file to play
   if( !search_subdir_mp3_file( "vol.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   dic_play_state = DIC_PLAY_BAT_WARN;
   
#endif   
}

#pragma CODE = MMI_AP4_BAT_LOW_CODE

void bat_low_waring_deal( U16 param )
{
#if 1
	U8 temp_buf[32];
	
    if( player_ap3_is_busy())
      {
         player_ap3_stop();
      }
   init_array_v();
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play

   if( 0 == param ){
   	SendString((U8 *)"\r\nW!");
	strcpy((char *)temp_buf,(char *)LOW_BAT_WARNING);
   	}
   else 
   		return;
   
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( (char *)temp_buf ))return;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())return ;
   scan_oid_disable();
   mmi_dic_kbd_disable();
   dic_play_state = DIC_PLAY_BAT_WARN;
#else
   if( !g_b_recorder_on )
      return;
   srvrec_stop();

   Ts_free( s_ts_ap3record_display_id );
   mmi_ap3_kbd_enable();
   scan_oid_enable();
#endif   
}

#endif   //#if( DIC == ENABLE )

#endif   //_mmi_dictionary_c_
