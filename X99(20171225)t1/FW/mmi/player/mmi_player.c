//!
//! @file mmi_player.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 2.7 snd3-dvk-1_9_5 $Id: mmi_player.c,v 2.7 2007/10/15 09:59:28 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_player_c_
#define _mmi_player_c_

//_____  I N C L U D E S ___________________________________________________
#include "lib_mcu\compiler.h"
#include "conf\conf_lcd.h"

#include <stdio.h>
#include <string.h>
#include "modules\power\power.h"
//#include "SSSMP3_Regs.h"
#include "modules\file_system\explorer.h"
#include "modules\file_system\file.h"           // for file management 
#include "sio\com.h"
#include "modules\player\player_task.h"
#include "modules\power\power_task.h"
#include "lib_system\mailbox\mail_evt.h"
#include "conf\conf_oid.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\status\mmi_status.h"
#include "mmi\kbd\keypad.h"
#include "mmi_player.h"
#include "srv_player.h"

#include "modules\mmi_manager\mmgr_kernel.h"
#include "modules\player\player.h"
#include "modules\control_access\ctrl_access.h"
#include "modules\file_system\fat.h"
#include "mmi\ap3\mmi_ap3.h"
#include "drivers\mmi\img_handler.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

#include "drivers\oid\oid.h"
//#include "lib_system\utilities\string.h"
#include "mmi\startup\mmi_startup.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "modules\search\search_remote.h"
#include "lib_mcu\timer\timer_drv.h"

#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"

#include "mmi\player\srv_player.h"
#include "conf\conf_mmi.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "modules\file_system\navigation.h"
#include "fs.h"
#include "USB_MAssStorage.h"
#include "mmi\status\mmi_status.h"

#include "point_g.h"


#pragma DATA = MMI_PLAYER_DATA
#pragma CODE = MMI_PLAYER_CODE

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE00 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE01 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE03 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE04 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE07 );

ImageDeclareType( MMI_PLAYER_TEMP_CODE00 );
ImageDeclareType( MMI_PLAYER_TEMP_CODE01 );
ImageDeclareType( MMI_PLAYER_TEMP_CODE02 );
ImageDeclareType( MMI_PLAYER_TEMP_CODE03 );

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( EXPLORER_TEMP_CODE00 );
ImageDeclareType( EXPLORER_TEMP_CODE01 );
ImageDeclareType( EXPLORER_TEMP_CODE02 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( MMI_PLAYER_BROWSER_CODE );
ImageDeclareType( READY_GOTODIR_CODE );
ImageDeclareType( PLAYER_WAV_CODE );
ImageDeclareType( FIND_PLAYDIR_CODE );
ImageDeclareType( LINK_PLAYDIR_CODE );
//ImageDeclareType( DISPLAY_MODE_CODE );
ImageDeclareType(SET_OID_COMMAND);
_MEM_TYPE_MEDSLOW_   U8       ap4_play_state;
//________________ D E C L A R A T I O N S ___________________
#define  FIRST_MUSIC_NUMBER     0
#define  PLAYER_LINE_PAGE  		2
#define NB_PLAYER_MENU			1
const char *s_player_menu_english =
{
  //"Record/Mp3"
  "Mp3 Player"
};

const char *s_player_menu_chinese =
{
  //"录音/MP3" 
  "MP3播放器"
};

#define NB_PLAYER_DIR			2
const char *s_player_dir_english[ ] =
{
  "Rec File"
, "Mp3 File"  	
};

const char *s_player_dir_chinese[ ] =
{
  "录音文件"
, "Mp3 文件"  
};
//extern   Bool  Usb_connect;

//** DECLARATION TO MANAGE THE BEHAVIOR

// Last state of player (PLAY or STOP)
Bool  mmiplayer_b_restartplay;
Bool  bNPRepeat;


Bool  b_kbd_process_disable;

_MEM_TYPE_SLOW_   U8                   play_current_mark;
_MEM_TYPE_SLOW_   U8				   state_brower;
//_MEM_TYPE_SLOW_   U8				   idle_current_page_num;
_MEM_TYPE_SLOW_   U8                   play_num;

_MEM_TYPE_SLOW_   U8                   play_dir[12];

//const    U8    mp3_dir[] = "0:\\MP3\\";
//const    U8    rev_dir[] = "0:\\RECORDER\\";
//const    U8    ext_rec[] = "WAV";
//const    U8    ext_mp3[] = "MP3";
//extern   xdata    AL_Variables     	varAl;

// Icon for each player state
_CONST_TYPE_ U32 mmiplayer_state_icon[] =
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

// Icon for each player mode
_CONST_TYPE_ U32 mmiplayer_mode_icon[] =
{
   IDP_PLAYER_SINGLE
,  IDP_PLAYER_NORMAL
,  IDP_PLAYER_REPEAT_SINGLE
,  IDP_PLAYER_REPEAT_ALL
};

// ID of the timer soft used by this MMI appli
_MEM_TYPE_SLOW_  U8  mmiplayer_id_timerdisplay;

typedef enum
{
   MP3_PLAYER_MODE
,  MP3_PLAYER_NO_FILE
,  MP3_PLAYER_ERROR
}PLAYER_STATE;
//_MEM_TYPE_SLOW_   U8    mp3_player_state;

// ID of the timer soft used by this MMI appli 
static _MEM_TYPE_SLOW_ U8 s_ts_id;

char  _MEM_TYPE_SLOW_   mp3_path[ 13 ];

//** ROUTINES TO MANAGE THE BEHAVIOR
void mmi_player_manage_evt             ( U8 event );
void mmi_player_change                 ( Bool b_direction, Bool b_force_repeat, Bool b_resume_current );
void mmi_player_switchoff              ( Bool b_poweroff );



//** ROUTINES TO MANAGE THE DISPLAY
#if( LCD_PRESENT == ENABLE )
void mmi_player_display_init_new_title ( void );
void mmi_player_display_backgroung     ( void );
void mmi_player_display_volume         ( void );
void mmi_player_display_title_id3      ( void );
void mmi_player_display_codec_type     ( void );
//void mmi_player_display_dynamic_infos  ( void );
void mmi_player_display_icon           ( void );
void mmi_player_display_mode           ( void );
char *convert_U8                       ( char *p, U8 i );

void mmi_player_display_volume         ( void );
//#define  mmi_player_display_volume()  mmi_ap3_display_volume()
void player_mark_display(U8 mark);
void player_menu_display(U8 display_num_mark);
void mmi_player_display_dynami_all_time( U8 time_flag);
void play_browser_init(void);
void ready_goto_playdir(void);
#else
#define mmi_player_display_init_new_title()
#define mmi_player_display_backgroung()
#define mmi_player_display_volume()
#define mmi_player_display_title_id3()
#define mmi_player_display_codec_type()
//#define mmi_player_display_dynamic_infos()
#define mmi_player_display_icon()
#define mmi_player_display_mode()
#endif  //#if( LCD_PRESENT == ENABLE )
Bool mmi_player_display_is_autorize    ( void );

Bool  mmi_player_playintro             ( void );
Bool  mmi_playerno_file_playintro      ( void );
void  player_no_file_deal              ( void );


extern void gendu_tiaozhen(void);
extern void set_oid_type(Bool set_type);
#if (UNICODE_TYPE == ENABLED)
void find_play_dir(void);
void link_play_dir(void);
#endif
//_____ D E F I N I T I O N S ______________________________________________
//! Manage of event and behavior
//!
//! @param U8  event
//!        U16 param
//!
void player_mmi_appli( U8 event, U16 param )
{
   switch( event )
   {
      case EVT_PLAYER:
      if( mp3_player_state != MP3_PLAYER_ERROR )
      {
         //if( param == PLAYER_EOF )
         {
            //power_authorize_autopoweroff();
            if( mp3_player_state == MP3_PLAYER_MODE )
            {
               //mp3_player_state = MP3_PLAYER_ERROR;
               //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
               //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
               //srvplayer_switch_off();  // Power off player
               mail_send_event( EVT_INT_BROWSER_PLAYER_APPLI, 0 );
            }
            else if( mp3_player_state == MP3_PLAYER_NO_FILE )
            {
               mp3_player_state = MP3_PLAYER_ERROR;
               g_select_appli = IDLE_APPLI;
               mmi_player_switchoff( FALSE );
              // mail_send_event( EVT_INT_BROWSER_PLAYER_APPLI, 0 );
            }
            mp3_player_state = MP3_PLAYER_ERROR;
         }
      }
      else
      {
         SendString((U8 *)"\r\nplay error!");
         mp3_player_state = MP3_PLAYER_ERROR;
         mmi_player_manage_evt( param );
      }
      break;
      
      case EVT_START_APPLI:
      SendString((U8 *)"\r\nplayer");
      mmgr_set_appli_full_screen();
      SDKLoadCode( MMI_PLAYER_TEMP_CODE00 );
      mmi_player_display_backgroung();
      state_brower = 0;
      mmiplayer_id_timerdisplay  = UNDEFINED_TIMER;
      s_ts_id                    = UNDEFINED_TIMER;
	  bNPRepeat = FALSE;
      
      
      //file_init();
      //nav_dir_root();
      srvplayer_g_player_state      = MMI_PLAYER_STATE_STOP;

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
      mmi_player_kbd_disable();
      //mmi_file_flag &= ~MP3_NO_FILE_FLAG;
#if( PASTER_MODE == DISABLE )
      //mmi_file_flag |= APF_NO_FILE_FLAG;
#endif   //#if( PASTER_MODE == DISABLE )
#if( DIC == DISABLE )
      //mmi_file_flag |= ADB_NO_FILE_FLAG;
#endif   //#if( DIC == DISABLE )
//***********************************************

      mp3_player_state = MP3_PLAYER_ERROR;
      SDKLoadCode( MMI_PLAYER_TEMP_CODE01 );
      if( !mmi_player_playintro())
      {
         mail_send_event( EVT_INT_BROWSER_PLAYER_APPLI, 0 );
      }
      //else
      //   mp3_player_state = MP3_PLAYER_MODE;
      break;
	  case EVT_INT_BROWSER_PLAYER_APPLI:
	  	mmi_player_kbd_enable();
		SDKLoadCode(MMI_PLAYER_BROWSER_CODE);
	  	play_browser_init();
	  	
	  	break;
	  case EVT_BROWSER_PLAYER_APPLI:
	  	play_current_mark = 1;
	  	SDKLoadCode(READY_GOTODIR_CODE);
		ready_goto_playdir();
		
#if ( UNICODE_TYPE == DISABLED )	
	if(play_current_mark == 0){
		strcpy((char *)play_dir,(char *)RECORD_DIR);
		}
	else{
		strcpy((char *)play_dir,(char *)MP3_DIR);
		}
#endif	//#if ( UNICODE_TYPE == DISABLED )	
#if (UNICODE_TYPE == ENABLED)
	 SDKLoadCode( LINK_PLAYDIR_CODE );
	 link_play_dir();
     SDKLoadCode(FIND_PLAYDIR_CODE);
	 find_play_dir();
	 //strcpy((char *)play_dir,(char *)tisiFileShortpath);
	  
#endif	//#if (UNICODE_TYPE == ENABLED) 
	  	mail_send_event( EVT_START_PLAYER_APPLI, 0 );
	  	break;
//	  case EVT_ENTER_PLAYER_APPLI:
//	  	break;
      case EVT_START_PLAYER_APPLI:
//***********************************************
      //SendString((U8 *)"star player\r\n");
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
      scan_oid_enable();
      mmi_player_kbd_enable();
      SDKLoadCode( NAVIGATION_TEMP_CODE05 );
      SDKLoadCode( NAVIGATION_CODE );
      file_init();
      SDKLoadCode( NAVIGATION_TEMP_CODE00 );
      if( nav_dir_root())
      {
         //nav_filelist_reset();
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
         nav_string_ascii();
#endif
         SDKLoadCode( NAVIGATION_TEMP_CODE03 );
         if( !nav_filelist_findname(( U8 *)play_dir, FALSE ))
         {
/*         
            SDKLoadCode( NAVIGATION_TEMP_CODE02 );
            if( !nav_dir_make(( U8 *)play_dir ))
            {
               if( fs_g_status != FS_ERR_FILE_EXIST )
               {
                  SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                  ( void )nav_file_del( TRUE );
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
                  nav_string_unicode();
#endif
                  mail_send_event( EVT_PLAYER_NO_FILE, 0 );
                  break;
               }
            }
*/			
			 mail_send_event( EVT_PLAYER_NO_FILE, 0 );
             break;
         }
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
         nav_string_unicode();
#endif
         SDKLoadCode( NAVIGATION_TEMP_CODE01 );
         if( !nav_dir_cd())
         {
            goto no_mp3_file_next;
            /*mail_send_event( EVT_PLAYER_NO_FILE, 0 );
            break;*/
         }
      }
      else
      {
         goto no_mp3_file_next;
         //SendString("player disk error\r\n");
         /*mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         break;*/
      }
//***********************************************
		play_current_mark = 1;
#if (UNICODE_TYPE == DISABLED)
#if 0
      strcpy((char *)tisiFileName,(char *)MP3_PATH );
#else      
      	if(play_current_mark == 0){
			strcpy((char *)tisiFileName,(char *)RECORD_PATH );
			}
		else{
			strcpy((char *)tisiFileName,(char *)MP3_PATH );
			}
#endif	
 //     SDKLoadCode( MMI_PLAYINTRO_CODE00 );
//      ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
#endif //#if (UNICODE_TYPE == DISABLED)
#if (UNICODE_TYPE == ENABLED)
	 //SDKLoadCode(FIND_PLAYDIR_CODE);
	// find_play_dir();
	 strcpy((char *)tisiFileName,(char *)"0:\\" );
	 strcat((char *)tisiFileName,(char *)play_dir);
	 strcat((char *)tisiFileName,(char *)"\\");
	  
#endif	//#if (UNICODE_TYPE == ENABLED)  
      SDKLoadCode( MMI_PLAYINTRO_CODE00 );
      ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
      strcpy( mp3_path, tisiFileShortpath );
      SendString((U8 *)"\r\nmp3_path=");
	  SendString((U8 *)mp3_path);
      // Open explorer
      //exp_init( ext_filemusic );
      SDKLoadCode( EXPLORER_TEMP_CODE00 );
      SDKLoadCode( NAVIGATION_CODE );
      if( !exp_open( 0, 0 ))
      {
no_mp3_file_next:
         mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         return;
      }
      //SendString((U8 *)"play mp3 file\r\n");
      // Init display
      power_forbid_autopoweroff();
      //srvplayer_g_arg.repeat = g_setting_list.player_repeat;
      //srvplayer_g_arg.repeat = NORMAL;//REPEAT_ALL;//
      //srvplayer_g_arg.repeat = REPEAT_ALL;
      mmiplayer_id_timerdisplay = ts_alloc();
      ts_set_time(mmiplayer_id_timerdisplay, TS_FLASH_TIME);
      // Launch play
      mmiplayer_b_restartplay = TRUE;     // automatic start play

      SDKLoadCode( SRV_PLAYER_TEMP_CODE00 );
      ( void )srvplayer_switch_on();
      mmi_player_change( FS_FIND_NEXT, TRUE, TRUE );
      
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
      break;

      /*case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;*/

      case EVT_TIMER:
#if 1
      if( LSB(param) == s_ts_id )
      {
         Ts_free( s_ts_id );
#if 0
         if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
            ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         {
            if( power_is_vbus_present())
            {
               g_select_appli = IDLE_APPLI;
               mmi_player_switchoff( FALSE );
            }
            else
            {
               g_power_off_mode = POWER_OFF;
               mmi_player_switchoff( TRUE );
            }
         }
         else
         {
//#if( RECORDER == ENABLE )
            //g_select_appli = RECORDER_APPLI;
#if( PASTER_MODE == ENABLE )
            g_select_appli = PASTER_APPLI;
#else
#if( DIC == DISABLE )
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //
            mmi_player_switchoff( FALSE );
         }
#else
         g_select_appli = IDLE_APPLI;
         mmi_player_switchoff( FALSE );
#endif
         break;
      }
#endif
      if( LSB(param) != mmiplayer_id_timerdisplay )
      { 
         // SendString((U8 *)"\r\n492");
         Mmgr_krn_forward_current_event();
         break;
      }
      ts_set_time(mmiplayer_id_timerdisplay, TS_FLASH_TIME);

      if( mp3_player_state != MP3_PLAYER_ERROR )
         break;
      if( MMI_PLAYER_STATE_PLAY != srvplayer_g_player_state )
         break;
///*
      SDKLoadCode( SRV_PLAYER_TEMP_CODE07 );
      srvplayer_gettime();
//*/
      //if( MMI_PLAYER_STATE_SEARCH_A != srvplayer_g_player_state)
    //     mmi_player_display_dynamic_infos();
      mmi_player_display_dynami_all_time(PLAY_CURRENT_TIME);
      break;

      case EVT_KEY_PRESSED:
	  SendString((U8 *)"\r\nmp3_player_state:player_key = ");
	  send_hex_u8(mp3_player_state);
	  SendString((U8 *)":");
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
      if( mp3_player_state != MP3_PLAYER_ERROR )
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
         if( mmi_player_kbd_is_disable())
            break;
      }
      if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
            if( power_is_vbus_present())
            {
               //if( Usb_connect )
                  mail_send_event( EVT_USB_POWERED, 0 );
               break;
            }
            g_power_off_mode = POWER_OFF;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_select_appli = IDLE_APPLI;
            mmi_player_switchoff( TRUE );
            break;

            case KEY_VOL_UP:
         
            break;

            case KEY_VOL_DOWN:
          
            break;

           // case KEY_FANYI:
            case KEY_BAK:
            mmi_player_switchoff( FALSE );
            //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
            recorder_mode = 1;
            g_select_appli = RECORDER_APPLI;
            break;

#if( PASTER_MODE == ENABLE )
            case KEY_MEN:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_select_appli = g_old_select_appli = PASTER_APPLI;
            mmi_player_switchoff( FALSE );
            //mmgr_activate_mmi_appli( MMI_APPLI_PASTER, 0 );
            g_select_appli = PASTER_APPLI;
            break;
#endif   //#if( PASTER_MODE == ENABLE )

            case KEY_MODE:
back_player:				
            mmi_player_switchoff( FALSE );
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#if 1//( PASTER_MODE == ENABLE )
            //g_select_appli = PASTER_APPLI;
            //bRemoteEnterCal = FALSE;
            //g_select_appli = AP3_APPLI;
            //g_select_appli = RECORDER_APPLI;
            g_select_appli = IDLE_APPLI;
            recorder_mode = 0;
#else
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#if( DIC == DISABLE )
            bRemoteEnterCal = FALSE;
            g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
            g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //#if( PASTER_MODE == ENABLE )
            break;
         }
      }
      else if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_POWER:
			if(state_brower == EVT_BROWSER_PLAYER_APPLI){
				mail_send_event( EVT_BROWSER_PLAYER_APPLI, 0 );
				break;
				}
            switch( srvplayer_g_player_state )
            {
               case MMI_PLAYER_STATE_PLAY :
               SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
               ( void )srvplayer_pause();
               mmi_player_display_icon();
               power_authorize_autopoweroff();
               break;

               case MMI_PLAYER_STATE_PAUSE :
               SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
               ( void )srvplayer_restart_play();
			   srvplayer_volume_send();
               mmi_player_display_icon();
               power_forbid_autopoweroff();
               break;
               
               case MMI_PLAYER_STATE_STOP :
               SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
               if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
               {
                  mmiplayer_b_restartplay = TRUE;
                  mmi_player_display_icon();
                  mmi_player_display_codec_type();
               }
               else
               {
                  mmiplayer_b_restartplay = TRUE;
                  mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
               }
               break;
            }
            break;
            
            case KEY_VOL_DOWN:
            srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            //srvplayer_volume_change( SRVPLAYER_VOL_UP );
            mmi_player_display_volume();
            break;
            
            case KEY_VOL_UP:
            srvplayer_volume_change( SRVPLAYER_VOL_UP );
            //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            mmi_player_display_volume();
            break;

			case KEY_NEXT:
				if(state_brower == EVT_BROWSER_PLAYER_APPLI){
					if(play_current_mark == (NB_PLAYER_DIR - 1))return;
					play_current_mark ++;
					player_mark_display(play_current_mark); 
					}
				else{

					if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
						{
						   mmiplayer_b_restartplay = TRUE;
						}
					  if((REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat)||
					  	(SINGLE	== g_setting_list.struct_setting.player_repeat)){
					  	SendString((U8 *)"\r\nrepeat next");
						bNPRepeat = true;
						}
					   mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );

						/*
					   if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
			            {	
			               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
						   
			            }
			            else
			            {
			               SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
			               if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
			               {
			                  mmiplayer_b_restartplay = TRUE;
			                  mmi_player_display_icon();
			                  mmi_player_display_codec_type();
			               }
			               else
			               {
			                  mmiplayer_b_restartplay = TRUE;
							  if(REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat){
						  		bNPRepeat = true;
							  	}
			                  mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
			                 // mmi_player_change( FS_FIND_NEXT, TRUE, TRUE);
			               }
			            }	
						*/	
					}
				break;
			case KEY_PREV:
				if(state_brower == EVT_BROWSER_PLAYER_APPLI){
				   	if(play_current_mark == 0)return;
					play_current_mark --;
					player_mark_display(play_current_mark); 
					}
				else{
				
					  if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
			            {
			               mmiplayer_b_restartplay = TRUE;
			            }
					  if((REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat)||
					  	(SINGLE	== g_setting_list.struct_setting.player_repeat)){
					  	SendString((U8 *)"\r\nrepeat prev");
						bNPRepeat = true;
						}
		               mmi_player_change( FS_FIND_PREV, TRUE, FALSE );
					}
			break;
            //case KEY_MEN:
/*            
            case KEY_FANYI:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_select_appli = g_old_select_appli = RECORDER_APPLI;
            mmi_player_switchoff( FALSE );
            //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
            g_select_appli = RECORDER_APPLI;
            recorder_mode = 1;
            break;
*/
            case KEY_GENDU:
            mmi_player_switchoff( FALSE );
            //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
            g_select_appli = REREAD_APPLI;
            recorder_mode = 1;
            break;

			case KEY_DUIBI:
            mmi_player_switchoff( FALSE );
            //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
            g_select_appli = REREAD_APPLI;
            recorder_mode = 0;
            break;

			case KEY_FUDU:
			//case KEY_MODE:
#if 0//(MP3_AB == ENABLE)			
			if(srvplayer_g_player_state != MMI_PLAYER_STATE_PLAY)goto back_break;
			if(count_AB > 0x7F)goto back_break;
			if(count_AB == 3){
				SendString((U8*)"\r\n3");
				count_AB = 0;
				if( audio_is_wav == 0 ){
					 
					 //AudioMP3SetPlayTime(point_A);
					 //AudioMP3Play();
					 gendu_tiaozhen();
					 SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  				     lcd_clear_part_of_screen(24, 56, 16, 8);
					}
				else{
///*					
					DAC_Pause();
					SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  				    lcd_clear_part_of_screen(24, 56, 16, 8);
					FSSeek(ADPCM_File,(((media_paly_all_time - point_A)<<8)+60));
					Frame_Count = point_A;
					SDKLoadCode( PLAYER_WAV_CODE );
					DAC_Continue();
					
//*/			
		//			count_AB = 4;
					}
				 
				
				 break;
				}
			if(count_AB == 2){
				SendString((U8*)"\r\n2");
				 count_AB ++;
				 if( audio_is_wav == 0 ){
				 	 if((point_B - point_A)<2000){
					 	 count_AB = 0;
						 SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  				 		 lcd_clear_part_of_screen(24, 56, 16, 8);
						 break;
				 	 	}
					 else{
					 		 //AudioMP3SetPlayTime(point_A);
					 		//AudioMP3Play();
					 		gendu_tiaozhen();
					 	}
				 	}
				 else{
				 	if((point_B - point_A)<(size_psec_wav<<1)){
					 	 count_AB = 0;
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
				SendString((U8*)"\r\n1");
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
				 SendString((U8*)"\r\n0");
				 if( audio_is_wav == 0 ){
				 	point_A = current_play_time;
				 	}
				 else{
				 	point_A = Frame_Count;
				 	}
				 count_AB ++;
				 lcd_draw_picture(24, 56, IDP_PLAYER_MARK_A);
				}	
back_break:
#endif //#ifdef (MP3_AB == ENABLE)	
			break;
            case KEY_MODE:
           // case KEY_FUDU:
            mmi_player_switchoff( FALSE );
            g_select_appli = IDLE_APPLI;
            recorder_mode = 0;
           /*
			if(state_brower == EVT_BROWSER_PLAYER_APPLI){
				goto back_player;
				}
			else{
				SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
				 srvplayer_stop();       // in case of
				   //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
				   //srvplayer_switch_off();

				 Ts_free( s_ts_id );
				 Ts_free( mmiplayer_id_timerdisplay );
				 mail_send_event( EVT_INT_BROWSER_PLAYER_APPLI, 0 );
				}
			//*/
            /*
            g_setting_list.struct_setting.player_repeat++;
            if( NB_REPEAT == g_setting_list.struct_setting.player_repeat )
               g_setting_list.struct_setting.player_repeat = SINGLE;
            //mmi_player_display_is_autorize();
            mmi_player_display_mode();
			*/			
            break;
         }
      }
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_player_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_player_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_select_appli = IDLE_APPLI;
      mmi_player_switchoff( TRUE );
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
	  mmi_player_switchoff( FALSE );
      //g_select_appli = AP3_APPLI;
	  break;
//***********************************************
      case EVT_USB_POWERED:
      mmi_player_switchoff( FALSE );
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_select_appli = MSTORAGE_APPLI;
      //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      CommonLoadCode( C_USB );
      break;

#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      if( mp3_player_state != MP3_PLAYER_ERROR )
         break;
      switch( LSB( param ))
      {
         case REMOTE_VOL_UP:
         srvplayer_volume_change( SRVPLAYER_VOL_UP );
         mmi_player_display_volume();
         break;

         case REMOTE_VOL_DOWN:
         srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
         mmi_player_display_volume();
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
         mmi_player_display_volume();
         break;

         case REMOTE_POWER_OFF:
         if( power_is_vbus_present())
            break;
         g_power_off_mode = POWER_OFF;
         g_b_remote_on = FALSE;
         mmi_player_switchoff( TRUE );
         break;

#if( REMOTE_MP3 == ENABLE )
         case REMOTE_EXIT_MP3:
         //bRemoteExitMp3 = TRUE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;

         case REMOTE_MP3_PLAY_PAUSE:
         switch( srvplayer_g_player_state )
         {
            case MMI_PLAYER_STATE_PLAY :
            SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
            ( void )srvplayer_pause();
            mmi_player_display_icon();
            power_authorize_autopoweroff();
            break;

            case MMI_PLAYER_STATE_PAUSE :
            SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
            ( void )srvplayer_restart_play();
            mmi_player_display_icon();
            power_forbid_autopoweroff();
            break;
               
            case MMI_PLAYER_STATE_STOP :
            SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
            if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_display_icon();
               mmi_player_display_codec_type();
            }
            else
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
            }
            break;
         }
         break;

         case REMOTE_MP3_PREV:
         if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
         {
            mmiplayer_b_restartplay = TRUE;
         }
		 if((REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat)||
					  	(SINGLE	== g_setting_list.struct_setting.player_repeat)){
					  	SendString((U8 *)"\r\nrepeat prev");
						bNPRepeat = true;
						}
         mmi_player_change( FS_FIND_PREV, TRUE, FALSE );
         break;

         case REMOTE_MP3_NEXT:
		 if( srvplayer_g_player_state == MMI_PLAYER_STATE_STOP )
         {
            mmiplayer_b_restartplay = TRUE;
         }
		 if((REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat)||
		  	(SINGLE	== g_setting_list.struct_setting.player_repeat)){
		  	SendString((U8 *)"\r\nrepeat prev");
			bNPRepeat = true;
			}
		 mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
		 	/*
         if( srvplayer_g_player_state != MMI_PLAYER_STATE_STOP )
         {
            mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
         }
         else
         {
            SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
            if( SRVPLAYER_PLAY_OK == srvplayer_play( FALSE ))
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_display_icon();
               mmi_player_display_codec_type();
            }
            else
            {
               mmiplayer_b_restartplay = TRUE;
               mmi_player_change( FS_FIND_NEXT, TRUE, FALSE );
            }
         }*/
         break;
#endif   //#if( REMOTE_MP3 == ENABLE )

         case REMOTE_CAL_MODE:
         bRemoteEnterCal = TRUE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         bRemoteEnterCal = FALSE;
         g_select_appli = AP3_APPLI;
         mmi_player_switchoff( FALSE );
         break;

#if( REMOTE_REC == ENABLE )
         case REMOTE_REC_MODE:
         g_select_appli = RECORDER_APPLI;
         recorder_mode = 0;
         mmi_player_switchoff( FALSE );
         break;
#endif   //#if( REMOTE_REC == ENABLE )

         /*case REMOTE_MP3_MODE:
         case REMOTE_REC_START:
         case REMOTE_REC_PLAY:
         case REMOTE_REC_STOP:
         break;*/

#if( REMOTE_PASTER == ENABLE )
         case REMOTE_PASTER_MODE:
         g_select_appli = PASTER_APPLI;
         mmi_player_switchoff( FALSE );
         break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
         case REMOTE_DIC_MODE:
         g_select_appli = DIC_APPLI;
         mmi_player_switchoff( FALSE );
         break;
#endif   //#if( REMOTE_DIC == ENABLE )
      }
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      case EVT_PLAYER_NO_FILE:
      SendString((U8 *)"\r\nNO FILE");
      SDKLoadCode( MMI_PLAYER_TEMP_CODE02 );
      player_no_file_deal();
#if 0
#endif
      break;
//***********************************************

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/

      default:
      Mmgr_krn_forward_current_event();
      break;
   }
}


//! Process the player events
//!
void mmi_player_manage_evt( U8 event )
{
   switch( event )
   {
      case PLAYER_EOF:                    // End of file, the player is stopped
      case PLAYER_BOF:                    // Beginning of file, the player is stopped
      // End of play then reset state of player
      srvplayer_g_player_state            = MMI_PLAYER_STATE_STOP;
      mmi_player_change( (PLAYER_EOF==event)? FS_FIND_NEXT : FS_FIND_PREV , FALSE, FALSE );
      break;
   }
}


//! Manage the status of the file(s) and play
//!
void mmi_player_change( Bool b_direction, Bool b_force_repeat, Bool b_resume_current )
{
   U8 u8_stateplay;
   U8 u8_loop = 0;
   //unsigned int longFileName[ 129 ];   // Word

   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 26, LCD_WIDTH, (LCD_HEIGHT - 26) );
   //mmi_player_display_dynamic_infos();
   mmi_player_display_dynami_all_time(PLAY_BEGIN_TIME);
   mmi_player_display_icon();
   mmi_player_display_mode();

   SendString((U8 *)"\r\nchange");
   if( b_resume_current )
      goto mmi_player_change_playfile;

   while(1)
   {
      //*** Go to next file
      // When you are on repeat single mode you play the same file
      /*if((REPEAT_SINGLE == srvplayer_g_arg.repeat)
      || (SINGLE        == srvplayer_g_arg.repeat))*/
      if((REPEAT_SINGLE	== g_setting_list.struct_setting.player_repeat)
      || (SINGLE == g_setting_list.struct_setting.player_repeat))
      {
      	 if(!bNPRepeat){
         	u8_loop++;
         	goto mmi_player_change_playfile;
      	 	}
      }

      SDKLoadCode( NAVIGATION_CODE );
      // Mov in explorer API
      SDKLoadCode( EXPLORER_TEMP_CODE01 );
      switch( exp_mov(b_direction) )
      {
         case EXP_MOV_OK_LOOP:
         u8_loop++;
         case EXP_MOV_OK:
         break;
   
         // Open loop error or empty
         case EXP_MOV_EMPTY:
         //mmi_player_switchoff(FALSE);
         //return;
         case EXP_MOV_DISKNOPRESENT:
         //mmi_player_switchoff(FALSE);
         //return;
         case EXP_MOV_DISKERROR:
         /*g_select_appli = IDLE_APPLI;
         mmi_player_switchoff(FALSE);*/
         mail_send_event( EVT_PLAYER_NO_FILE, 0 );
         return;
      }
mmi_player_change_playfile:
      if( u8_loop )
      {
         /*if((REPEAT_SINGLE != srvplayer_g_arg.repeat )
         && (REPEAT_ALL    != srvplayer_g_arg.repeat )
         && (!b_force_repeat) )*/
         if((REPEAT_SINGLE != g_setting_list.struct_setting.player_repeat )
         && (REPEAT_ALL    != g_setting_list.struct_setting.player_repeat )
         && (!b_force_repeat) )
         
         {
            mmiplayer_b_restartplay = FALSE;  // Stop play
            power_authorize_autopoweroff();
         }
         if( 1 < u8_loop )
         {
            // Loop empty or all fail
            /*g_select_appli = IDLE_APPLI;
            mmi_player_switchoff(FALSE);*/
            mail_send_event( EVT_PLAYER_NO_FILE, 0 );
            return;
         }
//***********************************************
         //add 2011-04-19, because long file name is lost
         /*if((REPEAT_SINGLE == srvplayer_g_arg.repeat)
            || (SINGLE        == srvplayer_g_arg.repeat))*/
         if((REPEAT_SINGLE == g_setting_list.struct_setting.player_repeat)
            || (SINGLE        == g_setting_list.struct_setting.player_repeat))  
         {
          if(!bNPRepeat){
         	goto mmi_player_change_playfile_single;
      	 	}
            
         }
//***********************************************
      }
      SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
      srvplayer_file_getinfos();
mmi_player_change_playfile_single:
      //Find_FileName( longFileName );   // Get MP3 long file name, The short file name is global variable "cFileName".
      //strcpy( tisiFileShortpath, "0:\\MP3\\" );
      strcpy( tisiFileShortpath, mp3_path );
      strcat( tisiFileShortpath, "\\" );
      
      strcat( tisiFileShortpath, ( char *)fat_g_cur_nav->name );
      
//***********************************************
      //add by xuanyu, because cannot codec if the file size is 0
      //if( fs_g_nav_entry.u32_size < 1024 )
      //   continue;
//***********************************************
      //mmi_player_display_init_new_title();
      SDKLoadCode( MMI_PLAYER_TEMP_CODE03 );
      mmi_player_display_title_id3();
      if( mmiplayer_b_restartplay )
      {
         // Restart play
         SDKLoadCode( SRV_PLAYER_TEMP_CODE03 );
         u8_stateplay = srvplayer_play( b_resume_current );
         b_resume_current = FALSE;
         if( SRVPLAYER_PLAY_OK != u8_stateplay )
         {
            continue;   // Error jump to other file
         }
         mmi_player_display_codec_type();
      }
      break;   // File change
   }
 	bNPRepeat = FALSE;
   mmi_player_display_icon();
   mmi_player_display_mode();
//   mmi_player_display_dynamic_infos  ();
   mmi_player_display_dynami_all_time(PLAY_BEGIN_TIME);
   mmi_player_display_dynami_all_time(PLAY_ALL_TIME);
}


//! @brief Turn off the player MMI
//!
void mmi_player_switchoff( Bool b_poweroff )
{
   SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
   srvplayer_stop();       // in case of
   //SDKLoadCode( SRV_PLAYER_TEMP_CODE01 );
   //srvplayer_switch_off();

   Ts_free( s_ts_id );
   Ts_free( mmiplayer_id_timerdisplay );
   Mmgr_kill_this_mmi_appli();
   power_authorize_autopoweroff();
   if( b_poweroff )
   {
      // Send command to power OFF player
      //mail_send_command(CMD_POWER_OFF, 0);
      //SendString((U8 *)"mp3 poweroff\r\n");
      g_select_appli = POWER_OFF_APPLI;
   }
}

#if (UNICODE_TYPE == ENABLED)
#pragma CODE = LINK_PLAYDIR_CODE
void link_play_dir(void)
{
	U16 RECORD_DIR_NAME[] = {0x5F55,0x97F3,0x6587,0x4EF6,0x0000};//录音文件
    U16 MP3_DIR_NAME[] = {0x6587,0x4EF6,0x0000};//mp3文件
    U16 *p;
    
		
   		p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\"), 3+1 );
      	if(play_current_mark == 0){
			//p = str_copy_u_n(( U16 *)tisiFileName, ( U16 *)RECORD_DIR_NAME, 4+1 );
			p = str_copy_u_n( p, ( U16 *)RECORD_DIR_NAME, 4+1 );
			}
		else{
			//strcpy((char *)tisiFileName,(char *)MP3_PATH );
			 //p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"mp3"), 4 );
			 p = str_copy_u_n( p, atou(( U8 *)"mp3"), 4 );
   			 p = str_copy_u_n( p, ( U16 *)MP3_DIR_NAME, 2+1 );
			}
		  p = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
}
#pragma CODE = FIND_PLAYDIR_CODE
void find_play_dir(void)
{
         U8 i;
		//SendString((U8 *)"\r\ntisiFileName=");
   		 //SendString((U8 *)tisiFileName);
   		  
		  SDKLoadCode( MMI_PLAYINTRO_CODE00 );
        ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
		 
		 //SendString((U8 *)"\r\ntisiFileShortpath=");
   		 //SendString((U8 *)tisiFileShortpath);
		 for(i=0;i<(strlen(tisiFileShortpath)-2);i++){
		 	play_dir[i] = tisiFileShortpath[i+3];
		 	}
		// SendString((U8 *)"\r\nplay_dir=");
   		 //SendString((U8 *)play_dir);
		//strcat((char *)tisiFileShortpath,(char *)"\\");
}
#endif
//--------------   mmi payer DISPLAY routines ------------------------------------
#if( LCD_PRESENT == ENABLE )
#if 0
//! @brief Initialize the scrolling Title
//!
void mmi_player_display_init_new_title( void )
{
   U8 nb_char;
   //U8 text_area_end_x;

   // Init scrolling
   nb_char = Unicode_get_char_space(g_string);
   if( nb_char < (unicode_get_string_length(g_string)+1+3) )   //+1 = NULL terminaison, +3 = "   "
   {
      Unicode_char(g_string, nb_char-3) = 0x0000;              // cut the end of name
   }
   unicode_append_ascii_string_to_buffer(g_string, "   ");
   
   if(!mmi_player_display_is_autorize())  return;

}
#endif
#pragma CODE = MMI_PLAYER_CODE
void player_mark_display(U8 mark)
{
	U16 i,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 28, 30, (LCD_HEIGHT-28));
	lcd_draw_picture(12,32+20*(mark%PLAYER_LINE_PAGE), IDP_PLAYER_PLAY);
}

#pragma CODE = MMI_PLAYER_TEMP_CODE00

//! @brief Draw the background
//!
void mmi_player_display_backgroung( void )
{
   lcd_turn_on();
   Lcd_start_backlight_timer();
   
   if(!mmi_player_display_is_autorize())  return;
   
   // Draw the main screen
   //lcd_draw_picture( 0, 0, IDP_BACKGROUND );
   //lcd_draw_picture( 0, 0, IDP_PLAYER_WND_ICON );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_screen();
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
	  lcd_clear_part_of_screen(20, 0,64, 8);
   lcd_goto_xy( 20, 0 );
   Ih_set_curr_font( IDC_FONT_08);
   lcd_puts(( U8 *)"MP3" );

#if 1
	lcd_goto_xy( 32, 16 );
    Ih_set_curr_font( IDC_FONT_16 );
    //lcd_puts(( U8 *)"MP3播放器" );
    switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
	      lcd_puts(( U8 *)s_player_menu_english);
	      break;
	      
	      case LANG_CHINESE_SIMP:
	      lcd_puts(( U8 *)s_player_menu_chinese);
	      break;
	   }
#else
   lcd_goto_xy( 0, 8 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts(( U8 *)"MP3播放:" );
   //lcd_goto_xy( 49, 0 );
   //Ih_set_curr_font( IDC_FONT_08 );
   //lcd_puts(( U8 *)"MUSIC" );
#endif   
   status_display();

   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
   mmi_player_display_volume();
}


#pragma CODE = MMI_PLAYER_CODE

//! @brief Display volume
//!
void mmi_player_display_volume( void )
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


#pragma CODE = MMI_PLAYER_TEMP_CODE03

//! @brief Display title and ID3 informations
//!
void mmi_player_display_title_id3( void )
{
   //_MEM_TYPE_SLOW_ U8 string[9];
   U8 text_area_end_x;


   //if ( Scroller_is_inactive() )
      { text_area_end_x = SCROLLING_AREA_END_X; }
   /*else
      { text_area_end_x = SCROLLER_PATTERN_X; }*/
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 26, LCD_WIDTH, (LCD_HEIGHT-26) );
   // Draw Title
   //lcd_goto_xy( SCROLLING_AREA_START_X, 26 );
   lcd_goto_xy( SCROLLING_AREA_START_X, 26 );
   //Lcd_set_txt_viewport_right(text_area_end_x);
   //lcd_clear_text_line_area( text_area_end_x );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( g_string );
#if 0

#endif
   Lcd_set_txt_viewport_right(SCROLLING_AREA_END_X);  // Reset view range
}


#pragma CODE = MMI_PLAYER_CODE

//! @brief Display codec type
//!
void mmi_player_display_codec_type( void )
{
   if(!mmi_player_display_is_autorize())  return;
 /* 
   lcd_goto_xy( 26, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   // Draw codec type
   if( audio_is_wav )
      lcd_puts(( U8 *)"WAV" );
   else
      lcd_puts(( U8 *)"MP3" );
*/   
}


//! @brief Display time and informations about play
//!
/*
void mmi_player_display_dynamic_infos( void )
{
   _MEM_TYPE_SLOW_ U8 string[9+4], i;
   
   if(!mmi_player_display_is_autorize())  return;

   // Draw time
   Lcd_unset_merging();
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
#if 0
   // Draw total time
   {
   U16 u16_tmp;
   u16_tmp = player_get_timetotal();
   srvplayer_g_total_time.minutes = MSB(u16_tmp );
   srvplayer_g_total_time.seconds = LSB(u16_tmp );
   }
   lcd_goto_xy( 88, 22 );
   sprintf( string , "%2bu:%02bu", srvplayer_g_total_time.minutes , srvplayer_g_total_time.seconds ); 
   lcd_puts( string );
#endif
}
*/



//! @brief Display time and informations about play
//!
void mmi_player_display_dynami_all_time(U8 time_flag)
{
   _MEM_TYPE_SLOW_ U8 string[6+4], i;

   U32 temp_data;
   if(!mmi_player_display_is_autorize())  return;
  // SendString((U8 *)"\r\nCurrent_play_time = ");
  // send_hex_u32(current_play_time);
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
	   if(temp_data<=2){
	   	count_AB = 0x80;
	   	}
	   else{
	   	count_AB = 0;
	   	}
	   
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
   	lcd_goto_xy( (RUNTIME_BEGIN_X + 16), 44 );
	strcat(( char *)string, "/" );
   	}
   else{
	  lcd_goto_xy( RUNTIME_BEGIN_X, 44 );
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
#if 0
   // Draw total time
   {
   U16 u16_tmp;
   u16_tmp = player_get_timetotal();
   srvplayer_g_total_time.minutes = MSB(u16_tmp );
   srvplayer_g_total_time.seconds = LSB(u16_tmp );
   }
   lcd_goto_xy( 88, 22 );
   sprintf( string , "%2bu:%02bu", srvplayer_g_total_time.minutes , srvplayer_g_total_time.seconds ); 
   lcd_puts( string );
#endif
}


//! @brief Display state icon
//!
void mmi_player_display_icon( void )
{
   if(!mmi_player_display_is_autorize())  return;
   lcd_draw_picture(RUN_STATE_X,56, mmiplayer_state_icon[ srvplayer_g_player_state ] );
}






//! @brief Check if display autorize
//!
Bool mmi_player_display_is_autorize( void )
{
   return (Mmgr_is_this_appli_at_the_top()/* || mmgr_is_appli_at_the_top(scroller_g_id_appli)*/);
}


char *convert_U8( char *p, U8 i )
{
   if( i / 10 )
      p = convert_U8( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}
#endif  //#if( LCD_PRESENT == ENABLE )


//***********************************************
//#pragma CODE = DISPLAY_MODE_CODE

void mmi_player_display_single( void )
{
  // if(!mmi_player_display_is_autorize())  return;
   //lcd_draw_picture(RUN_STATE_X,56, mmiplayer_state_icon[ srvplayer_g_player_state ] );
   if(0 != g_setting_list.struct_setting.player_repeat)return;
   
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( (RUN_STATE_X +16), 56 );
   lcd_puts((U8 *)"1");
}


void mmi_player_display_mode( void )
{
   //lcd_draw_picture(84,24, mmiplayer_mode_icon[ srvplayer_g_arg.repeat ] );
   SendString((U8 *)"\r\nplay_mode=");
   send_hex_u8(g_setting_list.struct_setting.player_repeat);
   lcd_draw_picture((RUN_STATE_X +8),56, mmiplayer_mode_icon[ g_setting_list.struct_setting.player_repeat ] );

   mmi_player_display_single();
}

#pragma CODE = MMI_PLAYER_TEMP_CODE01

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_player_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( "mp3player.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;

   mp3_player_state = MP3_PLAYER_MODE;
   return TRUE;
}


#pragma CODE = MMI_PLAYER_TEMP_CODE02

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_playerno_file_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( "nomp3file.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}



void player_no_file_deal( void )
{
   Ts_free( mmiplayer_id_timerdisplay );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 26, LCD_WIDTH, (LCD_HEIGHT - 26) );
   Ih_set_curr_font( IDC_FONT_16 );
   //lcd_puts( MMIPLAYER_MSG_ERROR_NOFILEPLAY );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   lcd_goto_xy( 12, 12 );
   lcd_puts(( U8 *)"No music file" );
#elif( LANGUAGE == Simplified )
   lcd_goto_xy( 24, 12 );
   lcd_puts(( U8 *)"无MP3数据文件" );
#elif( LANGUAGE == Traditional )
   lcd_goto_xy( 24, 12 );
   lcd_puts(( U8 *)"礚贾ゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 12, 26 );
      lcd_puts(( U8 *)"No find file" );
      break;

      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 24, 26 );
      //lcd_puts(( U8 *)"无播放文件" );
      lcd_puts(( U8 *)"无MP3数据文件" );
      break;
/*
      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 24, 26 );
      lcd_puts(( U8 *)"礚贾ゅン" );
      break;
*/	  
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )

   scan_oid_disable();
   mmi_player_kbd_disable();
   mp3_player_state = MP3_PLAYER_ERROR;
   //mmi_file_flag |= MP3_NO_FILE_FLAG;
   //SDKLoadCode( MMI_PLAYER_TEMP_CODE02 );
   if( !mmi_playerno_file_playintro())
   {
#if 1
      s_ts_id = ts_alloc();
      ts_set_time( s_ts_id, SPLASH_TIME );
#else
      if(( mmi_file_flag &( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
         ==( AP4_NO_FILE_FLAG | MP3_NO_FILE_FLAG | APF_NO_FILE_FLAG | ADB_NO_FILE_FLAG ))
      {
         if( power_is_vbus_present())
         {
            g_select_appli = IDLE_APPLI;
            mmi_player_switchoff( FALSE );
         }
         else
         {
            g_power_off_mode = POWER_OFF;
            mmi_player_switchoff( TRUE );
         }
      }
      else
      {
         mmi_player_switchoff( FALSE );
#if( RECORDER == ENABLE )
         //g_select_appli = PASTER_APPLI;
         //g_current_appli = AP3_APPLI;
         g_select_appli = RECORDER_APPLI;
#else
#if( DIC == DISABLE )
         g_select_appli = AP3_APPLI;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
         g_select_appli = DIC_APPLI;
#endif   //#if( DIC == ENABLE )
#endif   //#if( PASTER_MODE == ENABLE )
      }
#endif
   }
   else
   {
      mp3_player_state = MP3_PLAYER_NO_FILE;
   }
}

#pragma CODE = READY_GOTODIR_CODE
void ready_goto_playdir(void)
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
	lcd_goto_xy( 0, 8 );
	Ih_set_curr_font( IDC_FONT_16 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_puts(( U8 *)s_player_dir_english[play_current_mark]);
      break;
      
      case LANG_CHINESE_SIMP:
      lcd_puts(( U8 *)s_player_dir_chinese[play_current_mark]);
      break;
   } 
    state_brower = 0;

   //SendString((U8 *)"\r\nplay_dir=");
   //SendString((U8 *)play_dir);
}

#pragma CODE = MMI_PLAYER_BROWSER_CODE


void player_menu_display(U8 display_num_mark)
{
	U16 i,j,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
//   temp_file = (display_num_mark-1)*PLAYER_LINE_PAGE;
	temp_file = 0;
    Ih_set_curr_font( IDC_FONT_16 );
  for(i=0;i<2;i++ ){
 	  lcd_goto_xy( 32, 28+i*20 );  
	  switch( g_setting_list.struct_setting.language_ihm )
       {
          case LANG_ENGLISH:
          lcd_puts( ( U8 *)s_player_dir_english[temp_file ] );
          break;
          
          case LANG_CHINESE_SIMP:
           lcd_puts( ( U8 *)s_player_dir_chinese[temp_file ] );
          break;

          case LANG_CHINESE_TRAD:
          break;
       }
	  
	  temp_file ++;
  	}
}



void play_browser_init(void)
{
/*
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
  	lcd_goto_xy( 0, 8);
	Ih_set_curr_font( IDC_FONT_16 );
//lcd_puts(( U8 *)"MP3播放器" );
    switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
	      lcd_puts(( U8 *)s_player_menu_english);
	      break;
	      
	      case LANG_CHINESE_SIMP:
	      lcd_puts(( U8 *)s_player_menu_chinese);
	      break;
	   }
  	play_current_mark = 0;
	state_brower = EVT_BROWSER_PLAYER_APPLI;
	player_menu_display(play_current_mark);
	player_mark_display(play_current_mark);
//		mail_send_event( EVT_BROWSER_PLAYER_APPLI, 0 );
    mp3_player_state = MP3_PLAYER_ERROR;
*/
	mail_send_event( EVT_BROWSER_PLAYER_APPLI, 0 );
}
//***********************************************

#endif // _mmi_player_c_


