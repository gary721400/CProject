//!
//! @file mmi_setting.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI setting
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: mmi_setting.c,v 2.3 2007/10/05 13:40:08 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_setting_c_
#define _mmi_setting_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_lcd.h"
#include <string.h>
#include <stdio.h>
//#include "DP8051.H"
//#include "define.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\status\mmi_status.h"
#include "mmi\kbd\keypad.h"
#include "mmi_setting.h"

#include "sio\com.h"

#include "modules\control_access\ctrl_access.h"
#include "modules\mmi_manager\mmgr_kernel.h"
//#include "modules\file_aux\file_aux.h"
//#include "modules\player_ap3\player_ap3.h"
//#include "modules\paster\paster_base.h"
//#include "modules\file_system\setting.h"

#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
//#include "lib_system/utilities/string.h"

#include "conf\conf_sio.h"
#include "drivers\oid\oid.h"
#include "mmi\player\srv_player.h"
#include "modules\search\search_remote.h"

#include "lib_mcu\lcd\lcd_drv.h"
//#include "mmi\rw_nor\rw_nor.h"
//#include "drivers\lcd\lcd_SSD1303.h"
#include "mmi\images\tab_pict.h"
#include "modules\power\power.h"             // Power module definition
#include "mmi\startup\mmi_startup.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
//#include "date.h"


#include "point_g.h"

#pragma DATA = MMI_SETTING_DATA
#pragma CODE = MMI_SETTING_CODE

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

//_____ P R I V A T E   D E F I N I T I O N S _____________________________
//_____ D E F I N I T I O N S ______________________________________________
#if( LCD_PRESENT == ENABLE )
/*
//POSITION OF ALL ELEMENTS ON LCD
#define SETTING_POSX_ICON_CURSOR         2
#define SETTING_POSX_NAME_CURRENT_FILE   10

#define MENU_NB_LINE 4
code U8  MenuYPos[MENU_NB_LINE]   = { 15,25,35,45};

//Menu mode,press 
typedef enum
{
   SELECT = 0
,  NEXT
,  PREV
}MenuDirection;

typedef struct Menu_Contents
{
   U8 MenuIndex;
   U8 MenuNum;
   //U8 code * Title;     // head text
   //U32 IDP_Head;   //head icon
   U8 code ** MenuTxt;
};

//_MEM_TYPE_SLOW_ struct Menu_Contents menu_contents;
*/


//_____ D E C L A R A T I O N S ____________________________________________

void disp_Menu          ( U8 dir );
#else
#define disp_Menu( dir )
#endif  //#if( LCD_PRESENT == ENABLE )

/*********************** Menus **********************/

// Memory saving the parent path
// 6 menu levels : main menu/first sub-menu/second sub-menu
//   4 bits are reserved for the 1st level (main menu)   0x01->0x0F  (can launch 15 sub-menus)
//   3 bits for the 2nd level                            0x01->0x07  (can launch 7 sub-menus)
//   3 bits for the 3rd sub-menu                         0x01->0x07  (can launch 7 sub-menus)
//   3 bits for the 4th sub-menu                         0x01->0x07  (can launch 7 sub-menus)
//   3 bits for the 5th sub-menu                         0x01->0x07  (can launch 7 sub-menus)
//   value 0 kept to inform the root
// A shifting of this memory enables the storage of the menus path
#define NB_BIT_SUB_MENU    3
#define MSK_SUB_MENU       0xF8
#define Menu_start_ID(ParentMenuID)   ((ParentMenuID<<NB_BIT_SUB_MENU)+1)


#define SETTING_LINE_PAGE  2

#define CURRENT_POWEROFFVALUE     g_setting_list.struct_setting.poweroff_value
#define CURRENT_BLACKLIGHTTIME    g_setting_list.struct_setting.backlight_timeout
#define CURRENT_PLAYMODE		  g_setting_list.struct_setting.player_repeat
#define CURRENT_BLUETOOTH         g_setting_list.struct_setting.bluetooth
#define CURRENT_LANGUAGE          g_setting_list.struct_setting.language_ihm
#define CURRENT_VOLUME		      g_setting_list.struct_setting.player_volume
#define CURRENT_YEAR			  g_setting_list.struct_setting.year
#define CURRENT_MONTH   		  g_setting_list.struct_setting.month
#define CURRENT_DAY		          g_setting_list.struct_setting.day
#define CURRENT_HOUR    		  g_setting_list.struct_setting.hour
#define CURRENT_MIN  	          g_setting_list.struct_setting.min
//static _MEM_TYPE_SLOW_ U16 setting_menu_path;
//static _MEM_TYPE_SLOW_ U16 val;
//static _MEM_TYPE_SLOW_ U16 val2;

// Main menu (have sub-menus)
typedef enum
{
   SELECT_BLUETOOTH	 = 0x01
,  SELECT_POWEROFF_MENU
,  SELECT_BACKLIGHTTIMEOUT_MENU

/*
#if( SELECT_LANGUAGE == ENABLE )
,  SELECT_LANGUAGE_MENU
#endif   //#if( SELECT_LANGUAGE == ENABLE )
*/
//,  SELECT_WAV_PERCENT
,  SELECT_PLAYMODE
//,  SELECT_BLUETOOTH
//,  SELECT_DATE
//,  SELECT_TIME
//,  SELECT_YUYAN
,  SELECT_VOLUME_MENU
,  SELECT_TEL
,  SELECT_VERSION
//,  SELECT_DISKS_PROPERTIES
,  SELECT_NUM
/*
#if (ENABLE == FUNC_UPDATE)
,  SELECT_UPDATE
#endif
,  SELECT_UPDATE_SETTING
,  SELECT_ISP_SETTING
,  SELECT_MAIN_EXIT*/
}Setting_Main_Menu;

_MEM_TYPE_SLOW_   Setting_Main_Menu    MenuIndex;
_MEM_TYPE_SLOW_   U8                   setting_current_mark;
_MEM_TYPE_SLOW_   U8				   setting_page_num;
_MEM_TYPE_SLOW_   U8				   setting_current_page_num;
_MEM_TYPE_SLOW_   U8                   Menunum;
//_MEM_TYPE_SLOW_   U8                   Setting_Buf[128];


_MEM_TYPE_SLOW_   U8                   setting_port_mark;
_MEM_TYPE_SLOW_   U8				   setting_port_num;
_MEM_TYPE_SLOW_   U8				   setting_current_port_num;

_MEM_TYPE_SLOW_   U8				   display_mark;

_MEM_TYPE_SLOW_   U8				   setting_powerofftime;

_MEM_TYPE_SLOW_   U8				   setting_bluetooth;
// define the state of the seacher
#define Start_Setting                   ( 0x00 )
#define Bluetooth_Setting               ( Start_Setting + 1 )
#define Poweroff_Setting                ( Start_Setting + 2 )
#define Blacklight_Setting         		( Start_Setting + 3 )
#define PlayMode_Setting                ( Start_Setting + 4 )

//#define Date_Setting                	( Start_Setting + 5 )
//#define Time_Setting                  	( Start_Setting + 6 )
#define Language_Setting                ( Start_Setting + 20 )
#define Volume_Setting                  ( Start_Setting + 5 )

#define Telphone_Setting                ( Start_Setting + 6 )
#define Version_Setting                 ( Start_Setting + 7)

#define Currentstate_Setting            ( Start_Setting + 8 )
#define Idle_Setting            		( Start_Setting + 9 )
#define Init_Setting                    ( Start_Setting + 10 )
#define Browser_Setting                 ( Start_Setting + 11 )
#define Backtotop_Setting               ( Start_Setting + 12 )


U8 poweroff_setting[4] = {0,5,10,30};

_MEM_TYPE_SLOW_   U8                   State_setting;

#if( LCD_PRESENT == ENABLE )
#if( SELECT_LANGUAGE == DISABLE )
#define NB_SETTING_MAIN_MENU  4//5//(sizeof(s_setting_name)/sizeof (U8 code *))
#if( LANGUAGE == ENGLISH )
const char *s_setting_name[]=
{
   "Poweroff(min):"
,  "Backlight(m):"
,  "Volume(level):"
//,  "Wav_Percent:"
};
#elif( LANGUAGE == Simplified )
const char *s_setting_name[]=
{
   "关机(分钟):"
,  "背光(分钟):"
,  "音量(预设):"
//,  "wav百分比:"
/*,  "Version"
#if (ENABLE == FUNC_UPDATE)
,  "Update system"
#endif3
,  "Update setting"
,  "ISP"
,  "Exit"*/
};
#elif( LANGUAGE == Traditional )
const char *s_setting_name[]=
{
   "闽诀(だ牧):"
,  "璉(だ牧):"
,  "秖(箇砞):"
//,  "wavκだゑ :"
};
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
#define NB_SETTING_MAIN_MENU  (SELECT_NUM-1)//8
const char *s_setting_name_english[]=
{
   "Poweroff(min)"
,  "Backlight(min)"
//,  "Language:"
,  "PlayMode"
#if 0//( WIRELESS == ENABLE)
,  "Wireless"
#endif
#if 1//( BLUETOOTH == ENABLE)
,  "OidType"
#endif

//,  "Date:"
//,  "Time:"
//,  "language"
,  "Volume(level)"
,  "Telephone"
,  "Version"
//,  "Wav_Percent:"
};
const char *s_setting_name_chinese_simp[]=
{
  "编码类型"
,  "关机时间"
,  "背光调整"
//,  "语言:"
,  "播放模式"
#if 0//( WIRELESS == ENABLE)
,  "无 线"
#endif
#if 1//( BLUETOOTH == ENABLE)

#endif
//,  "日  期"
//,  "时  间"
//,  "语 言"
,  "默认音量"
,  "售后电话"
,  "系统版本"
//,  "wav百分比:"
};
const char *s_setting_name_chinese_trad[]=
{
   "闽诀(だ牧):"
,  "璉(だ牧):"
,  "秖(箇砞):"
,  "粂ē:"
,  "冀家Α:"
,  "屡:"
,  "緓牧:"//,  "wavκだゑ:"
,  " セ:"
,  "扳筿杠:"

};
#endif   //#if( SELECT_LANGUAGE == ENABLE )
#endif  //#if( LCD_PRESENT == ENABLE )
#define NB_SETTING_POWEROFF   4//(sizeof(s_setting_poweroff_list)/sizeof (char *))
#define PAGE_SETTING_POWEROFF  ((NB_SETTING_POWEROFF+1)%SETTING_LINE_PAGE)
const char * s_setting_poweroff_chm_list[]=
{
   "关"
,  "5分钟"
,  "10分钟"
,  "30分钟"

};
const char * s_setting_poweroff_en_list[]=
{
   "Close"
,  "5Min"
,  "10Min"
,  "30Min"

};
#define NB_SETTING_BACKLIGHTTIMEOUT  4//(sizeof(s_setting_backlighttimeout_list)/sizeof (char *))
#define PAGE_SETTING_BACKLIGHTTIMEOUT  ((NB_SETTING_BACKLIGHTTIMEOUT+1)%SETTING_LINE_PAGE)
const char * s_setting_backlighttimeout_chm_list[]=
{
   //"关"
   "背光常亮"
,  "1 分钟"
,  "2 分钟"
,  "3 分钟"
};

const char * s_setting_backlighttimeout_en_list[]=
{
   "Close"
,  "1Min"
,  "2Min"
,  "3Min"
};
#define NB_SETTING_PLAYMODE  4//(sizeof(s_setting_playmode_list)/sizeof (char *))
#define PAGE_SETTING_PLAYMODE  ((NB_SETTING_PLAYMODE+1)%SETTING_LINE_PAGE)
const char * s_setting_playmode_chm_list[]=
{
   "单曲播放"
,  "顺序播放"
,  "单曲循环"
,  "全部循环"
//,  "随机播放"
};

const char * s_setting_playmode_en_list[]=
{
   "Single"
,  "All"
,  "Repeat Single"
,  "Repeat All"
//,  "随机播放"
};

#define NB_SETTING_BLUETOOTH  2//(sizeof(s_setting_bluetooth_list)/sizeof (char *))
#define PAGE_SETTING_BLUETOOTH  ((NB_SETTING_BLUETOOTH+1)%SETTING_LINE_PAGE)
const char * s_setting_bluetooth_chm_list[]=
{
#if 1
   "Dpr3(新东方)"
,  "Oid2(其他)"
#else
   "关"
,  "开"
#endif
};
const char * s_setting_bluetooth_en_list[]=
{
#if 1
   "Dpr3"
,  "Oid2"
#else
   "Close"
,  "Open"
#endif
};
#define NB_SETTING_LANGUAGE  2//(sizeof(s_setting_bluetooth_list)/sizeof (char *))
#define PAGE_SETTING_LANGUAGE  ((NB_SETTING_LANGUAGE+1)%SETTING_LINE_PAGE)
const char * s_setting_language_chm_list[]=
{
   "英文"
,  "中文"

};
const char * s_setting_language_en_list[]=
{
   "English"
,  "Chinese"

};
#if 0
// Power menu (have sub-menus)
typedef enum {
   SELECT_POWEROFF_MENU = Menu_start_ID(SELECT_POWER_MENU)
,  SELECT_BACKLIGHTOFF_MENU
,  SELECT_BACKLIGHTTIMEOUT_MENU
,  SELECT_POWER_EXIT
}Setting_Power_Menu;

#define NB_SETTING_POWER      (sizeof(s_setting_power_list)/sizeof (U8 code *))
static code U8 code * s_setting_power_list[]=
{
   "Power off"
,  "Backlight"
,  "Backlight timeout"
,  "Exit"
};


// Language menu (have no sub-menus)
typedef enum
{
   SELECT_LANGUAGE = Menu_start_ID(SELECT_LANGUAGE_MENU)
}Setting_Language_Value;

#define NB_SETTING_LANGUAGE   (sizeof(s_setting_language_list)/sizeof (U8 code *))
static code U8 code * s_setting_language_list[]=
{
   "English"
,  "French"
,  "Chinese simplified"
,  "Chinese traditional"
};


// Power-off menu (have no sub-menus)
typedef enum {
   SELECT_POWEROFF = Menu_start_ID(SELECT_POWEROFF_MENU)
}Setting_PowerOff_Value;



// Backlight-off menu (have no sub-menus)
typedef enum {
   SELECT_BACKLIGHTOFF = Menu_start_ID(SELECT_BACKLIGHTOFF_MENU)
}Setting_Backlight_Off_Value;

#define NB_SETTING_BACKLIGHTOFF  (sizeof(s_setting_backlightoff_list)/sizeof (U8 code *))
static code U8 code * s_setting_backlightoff_list[]=
{
   "Always"
,  "After key"
};

// Backlight-timeout menu (have no sub-menus)
typedef enum {
   SELECT_BACKLIGHTTIMEOUT = Menu_start_ID(SELECT_BACKLIGHTTIMEOUT_MENU)
}Setting_Backlight_Timeout_Value;




// Format
typedef enum {
   SELECT_FORMAT= Menu_start_ID(SELECT_FORMAT_MENU)
}Setting_Format_Value;

typedef enum {
   EXIT_FORMAT  = Menu_start_ID(SELECT_FORMAT)
}Setting_ExitFormat_Value;

#define NB_SETTING_FORMAT     (sizeof(s_setting_format_list)/sizeof (U8 code *))
static code U8 code * s_setting_format_list[]=
{
   "yes"
,  "no"
};

// Version
typedef enum {
   EXIT_VERSION= Menu_start_ID(SELECT_VERSION)
}Setting_Version_Value;

typedef enum
{
   VERSION_FCT          = 1
}High_setting_fct_ID;
#endif


//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
void  mmi_setting_back_to_main_menu    ( void );
void  mmi_setting_display_version      ( void );
void  mmi_setting_exit                 ( void );
void  setting_version                  ( Bool b_present, Bool b_full_screen );
extern void asm_isp_jmp(void);

void  setting_purcent_bar_init         ( void );
void  setting_percent_bar              ( U8 u8_percent );
void setting_state_change(void);
void setting_mark_display(U8 mark);
void setting_menu_display(U16 display_num_mark);
void ready_setting_all_display(void);
void change_setting_all_display(Bool temp_flag);
void display_volume(U8 temp_data);
void display_date(void);
void display_bcd(Bool flag_inverse,U8 temp_data );
void repair_date(Bool direction_flag);
void display_time(void);
void repair_time(Bool direction_flag);
void mmi_setting_display_volume( void );
//_____ second level of mmi_update routine (private)
#if( LCD_PRESENT == ENABLE )
void  display_menu                     ( void );
void  setting_hbar                     ( const U8 x0, const U8 y0 );
void  display_disk_properties          ( void );
void  display_bluetooth                ( void );
void  display_alarm                    ( void );
void  display_playmode                 ( void );
void  display_tel(void);
void  display_version(void);
#else
#define display_menu()
#define setting_hbar( x0, y0 )
#define display_disk_properties()
#define display_bluetooth()                
#define display_alarm()                   
#define display_playmode()                
#endif

char  *convert_U8                      ( char *p, U8 i );
char  *convert_U16                     ( char *p, U16 i );


//_____ D E F I N I T I O N S ______________________________________________
Bool global_key_deal(U16 param)
{
	if(param == ((KEY_REC<<8)|KBD_KEY_PRESSED)){
		//g_select_appli = IDLE_APPLI;
	    g_select_appli = PLAYER_APPLI;
	    mmi_setting_exit();
		return TRUE;
		}
	return FALSE;
}
//!
//! setting_mmi_appli
//!
//! Manage the application playlist
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void setting_mmi_appli( U8 event, U16 param )
{
  // param = param;
   switch( event )
   {
      case EVT_START_APPLI:
      mmgr_set_appli_full_screen();
	   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
     lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
     lcd_clear_part_of_screen(20, 0,64, 8);
	lcd_goto_xy( 20, 0 );
    Ih_set_curr_font( IDC_FONT_08);
     lcd_puts(( U8 *)"SYS" );
#if 0//( LCD_PRESENTABLE )
      menu_contents.Title     = "Setting";
      //menu_contents.IDP_Head = -1;
      menu_contents.MenuNum   = == EN NB_SETTING_MAIN_MENU;
      menu_contents.MenuTxt   = s_setting_name;
      menu_contents.MenuIndex = 0;
      //setting_menu_path = 0;
#endif  //#if( LCD_PRESENT == ENABLE )
#if( OID == ENABLE )
      //b_oid_init_flag = FALSE;
      if( !b_oid_init_flag )
      {
         reinit_oid_pen();
      }
#endif   //#if( OID == ENABLE )
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )

      Kbd_set_evt_enabled();
      if( Kbd_is_event_triggered())
         Kbd_set_evt_disabled();
      
      scan_oid_enable();
	 
      //setting_purcent_bar_init();
      //setting_percent_bar(( U16 )g_setting_list.poweroff_value * 100 / 10 );
      State_setting = Init_Setting;
      break;

      case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_set_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_set_power_off:
      //mail_send_command(CMD_POWER_OFF, 0);
      g_select_appli = POWER_OFF_APPLI;
      mmi_setting_exit();
      break;
     
      // Running only if feature keyboard is activated
      case EVT_KEY_PRESSED:
	  SendString((U8 *)"\r\nsetting_key = ");
	  send_hex_u16(param);
#if 1
#if( LCD_PRESENT == ENABLE )
#  if(BACKLIGHT == ENABLE)
      if( g_mmi_lcd_backlight_off )
      {
         if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
            g_mmi_lcd_backlight_off = FALSE;
         break;
      }
#  endif //#if(BACKLIGHT == ENABLE)

	  if(global_key_deal(param))return;
#endif   //#if( LCD_PRESENT == ENABLE )
      if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KEY_MODE:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

	            bRemoteEnterCal = FALSE;
	            g_select_appli = IDLE_APPLI;
	            //g_select_appli = PLAYER_APPLI;
	            mmi_setting_exit();
            break;

            case KEY_POWER:
            if( power_is_vbus_present())
            {
               //if( Usb_connect )
                  mail_send_event( EVT_USB_POWERED, 0 );
               break;
            }
            //mail_send_command( CMD_POWER_OFF, 0 );
            g_select_appli = POWER_OFF_APPLI;
            g_power_off_mode = POWER_OFF;
            mmi_setting_exit();
            break;

			//case KEY_FANYI:
			case KEY_BAK:
			recorder_mode = 1;
			g_select_appli = RECORDER_APPLI;
            mmi_setting_exit();
			break;
/*
			case KEY_GENDU:
			recorder_mode = 1;
			g_select_appli = REREAD_APPLI;
            mmi_setting_exit();
			break;

			case KEY_DUIBI:
			recorder_mode = 0;
			g_select_appli = REREAD_APPLI;
            mmi_setting_exit();
			break;
*/
#if( PASTER_MODE == ENABLE )
            case KEY_MEN:
            g_select_appli = PASTER_APPLI;
            mmi_setting_exit();
            break;
#endif   //#if( PASTER_MODE == ENABLE )
         }
      }
      else if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
         	case KEY_MODE:
				if(State_setting == Browser_Setting){
//					bRemoteEnterCal = FALSE;
		            g_select_appli = IDLE_APPLI;
		            //g_select_appli = PLAYER_APPLI;
		            mmi_setting_exit();
					}
				else{
					if(State_setting == Volume_Setting){
						mmi_setting_display_volume();
						}
					SendString((U8 *)"\r\ngo back to top");
					State_setting = Backtotop_Setting;
					}
				break;
			case KEY_POWER:
				if((State_setting >= Bluetooth_Setting)&&(State_setting <= Volume_Setting)){
					SendString((U8 *)"\r\ngo back to top");
					State_setting = Backtotop_Setting;
					}
				if(State_setting == Browser_Setting){
//					MenuIndex = setting_current_mark;
					display_menu();
					State_setting = setting_current_mark+1;
					SendString((U8 *)"\r\ngoin port set,State_setting=");
					send_hex_u8(State_setting);
					ready_setting_all_display();
					}
				break;
            case KEY_NEXT:
			  SendString((U8 *)"\r\nState_setting:mark=");
			  send_hex_u8(State_setting);
			   SendString((U8 *)":");
			  send_hex_u8(setting_current_mark);
			  if(State_setting == Browser_Setting){	
			  if((setting_current_mark%SETTING_LINE_PAGE)==0){
			  	if((setting_current_mark+1)==Menunum)break;
			  	 setting_current_mark ++;
				 setting_mark_display(setting_current_mark);
			  	}
			  else{
			  	if(setting_current_page_num<setting_page_num){
					setting_current_page_num++;
					setting_current_mark ++;
					
					setting_menu_display(setting_current_page_num);
					setting_mark_display(setting_current_mark);
			  		}
			  	}
	    		State_setting = Browser_Setting;
			  	}
			else{
				if(State_setting != Volume_Setting){
					change_setting_all_display(TRUE);
					}
				}
            break;

			case KEY_PREV:
			SendString((U8 *)"\r\nState_setting:mark=");
			  send_hex_u8(State_setting);
			   SendString((U8 *)":");
			  send_hex_u8(setting_current_mark);
			if(State_setting == Browser_Setting){	
				  if((setting_current_mark)==0)break;		
				  if((setting_current_mark%SETTING_LINE_PAGE)!=0){
				  	setting_current_mark --;
					setting_mark_display(setting_current_mark);
				  	}
				  else{
				  	if(setting_current_page_num > 1){
						setting_current_page_num--;
						setting_current_mark --;
						setting_menu_display(setting_current_page_num);
						setting_mark_display(setting_current_mark);
				  		}
				  	}
			    State_setting = Browser_Setting;
				}
			else{
				if(State_setting != Volume_Setting){
			     	change_setting_all_display(FALSE);
					}
				}
            break;
/*			
            //case KEY_MEN:
            case KEY_FANYI:
			g_select_appli = RECORDER_APPLI;
            mmi_setting_exit();
			break;
*/
			case KEY_GENDU:
			recorder_mode = 1;
			g_select_appli = REREAD_APPLI;
            mmi_setting_exit();
			break;

			case KEY_DUIBI:
			recorder_mode = 0;
			g_select_appli = REREAD_APPLI;
            mmi_setting_exit();
			break;


            case KEY_VOL_UP:
					
            //setting_change_flag = TRUE;
            if(State_setting == Volume_Setting){
				change_setting_all_display(TRUE);
				break;
            	}
			 //SendString((U8 *)"\r\n755_setting_current_page_num=");
		    // send_hex_u16(setting_current_page_num);
		   	if(CURRENT_VOLUME<(VOL_LEVEL-1)){
				CURRENT_VOLUME ++;
				mmi_setting_display_volume();
				break;
		   		}
		   //if(State_setting == Browser_Setting)return;
			/*
			if(State_setting == Date_Setting){
				repair_date(FALSE);
				}
			if(State_setting == Time_Setting){
				repair_time(FALSE);
				}*/
            break;

            case KEY_VOL_DOWN:
            //setting_change_flag = TRUE;
					
			if(State_setting == Volume_Setting){
				SendString((U8 *)"\r\nvolume_setting");
				change_setting_all_display(FALSE);
				break;
            	}
			
		   	if(CURRENT_VOLUME>0){
				SendString((U8 *)"\r\nkey_volume_setting");
				CURRENT_VOLUME --;
				mmi_setting_display_volume();
				//State_setting = Currentstate_Setting;
				break;
		   		}
		   	//if(State_setting == Browser_Setting)return;
/*			
			if(State_setting == Date_Setting){
				repair_date(TRUE);
				}
			if(State_setting == Time_Setting){
				repair_time(TRUE);
				}
*/			
            break;
         }
      }
      else if( KBD_KEY_REPEAT == KEY_STATUS( param ))
      {
      //	 State_setting = Idle_Setting;
#if 0          
         switch( KEY_ID( param ))
         {
            case KEY_VOL_UP:
            //setting_change_flag = TRUE;
            switch( MenuIndex )
            {
               case SELECT_POWEROFF_MENU:
               if( g_setting_list.struct_setting.poweroff_value < 10 )
                  g_setting_list.struct_setting.poweroff_value++;
               disp_Menu( g_setting_list.struct_setting.poweroff_value );
               break;

               case SELECT_BACKLIGHTTIMEOUT_MENU:
               if( g_setting_list.struct_setting.backlight_timeout < 5 )
               {
                  g_setting_list.struct_setting.backlight_timeout++;
                  g_setting_list.struct_setting.backlight_mode = BACKLIGHT_MODE_FULL_OFF;
               }
               disp_Menu( g_setting_list.struct_setting.backlight_timeout );
               break;

               case SELECT_VOLUME_MENU:
               if( g_setting_list.struct_setting.player_volume <( SRVPLAYER_VOL_LEVEL - 1 ))
                  g_setting_list.struct_setting.player_volume++;
               disp_Menu( g_setting_list.struct_setting.player_volume );
               break;

               /*case SELECT_WAV_PERCENT:
               if( g_setting_list.play_precent <( SRVPLAYER_VOL_LEVEL ))
                  g_setting_list.play_precent++;
               disp_Menu( g_setting_list.play_precent );
               break;*/

               /*default:
               setting_change_flag = FALSE;
               break;*/
            }
            break;

            case KEY_VOL_DOWN:
            //setting_change_flag = TRUE;
            switch( MenuIndex )
            {
               case SELECT_POWEROFF_MENU:
               if( g_setting_list.struct_setting.poweroff_value )
                  g_setting_list.struct_setting.poweroff_value--;
               disp_Menu( g_setting_list.struct_setting.poweroff_value );
               break;

               case SELECT_BACKLIGHTTIMEOUT_MENU:
               if( g_setting_list.struct_setting.backlight_timeout )
               {
                  g_setting_list.struct_setting.backlight_timeout--;
                  if( !g_setting_list.struct_setting.backlight_timeout )
                     g_setting_list.struct_setting.backlight_mode = BACKLIGHT_MODE_OFF;
               }
               disp_Menu( g_setting_list.struct_setting.backlight_timeout );
               break;

               case SELECT_VOLUME_MENU:
               if( g_setting_list.struct_setting.player_volume )
                  g_setting_list.struct_setting.player_volume--;
               disp_Menu( g_setting_list.struct_setting.player_volume );
               break;

               /*case SELECT_WAV_PERCENT:
               if( g_setting_list.play_precent > 0 )
                  g_setting_list.play_precent--;
               disp_Menu( g_setting_list.play_precent );
               break;*/

               /*default:
               setting_change_flag = FALSE;
               break;*/
            }
            break;
         }
#endif		 
      }
#if 0
      if( KBD_KEY_PRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
            case KBD_MMI_F:
            mmi_setting_exit();
            break;

            case KBD_MMI_NEXT:
            case KBD_MMI_SELECT:
            //menu_contents.IDP_Head = -1;  // default value
            setting_menu_path |= (menu_contents.MenuIndex+1);
            switch( setting_menu_path )
            {
               case SELECT_LANGUAGE_MENU:
               menu_contents.Title     = "Setting Language";
               menu_contents.MenuNum   = NB_SETTING_LANGUAGE;
               menu_contents.MenuTxt   = s_setting_language_list;
               menu_contents.MenuIndex = g_setting_list.language_ihm;
               break;

               case SELECT_POWER_MENU:
               menu_contents.Title     = "Setting Power";
               menu_contents.MenuNum   = NB_SETTING_POWER;
               menu_contents.MenuTxt   = s_setting_power_list;
               menu_contents.MenuIndex = 0;
               break;

               case SELECT_POWEROFF_MENU:
               menu_contents.Title     = "Setting Power Off";
               menu_contents.MenuNum   = NB_SETTING_POWEROFF;
               menu_contents.MenuTxt   = s_setting_poweroff_list;
               menu_contents.MenuIndex = g_setting_list.poweroff_value;
               break;

               case SELECT_BACKLIGHTOFF_MENU:
               menu_contents.Title     = "Setting Backlight Off";
               menu_contents.MenuNum   = NB_SETTING_BACKLIGHTOFF;
               menu_contents.MenuTxt   = s_setting_backlightoff_list;
               menu_contents.MenuIndex = g_setting_list.backlight_mode;
               break;

               case SELECT_BACKLIGHTTIMEOUT_MENU:
               menu_contents.Title     = "Setting Backlight Timeout";
               menu_contents.MenuNum   = NB_SETTING_BACKLIGHTTIMEOUT;
               menu_contents.MenuTxt   = s_setting_backlighttimeout_list;
               menu_contents.MenuIndex = (g_setting_list.backlight_timeout / 10)-1;
               break;

               case SELECT_FORMAT_MENU:
               menu_contents.Title     = "Really want to format?";
               menu_contents.MenuNum   = NB_SETTING_FORMAT;
               menu_contents.MenuTxt   = s_setting_format_list;
               menu_contents.MenuIndex = 1;
               break;

#if (ENABLE == FUNC_UPDATE)
               case SELECT_UPDATE:
               // Launch mmi_update through the explorer 
               mmgr_activate_mmi_appli(MMI_APPLI_EXPLORER, EXP_FILTER_UPDATE |EXP_FILTER_DIR|(EXP_OPT_POS_CURRENT<<8));
               mmi_setting_exit();               
               break;
#endif
               case SELECT_UPDATE_SETTING:
               lcd_draw_picture( 0, 0, IDP_BACKGROUND );
               lcd_goto_xy (25,30);

               if( setting_update() )
               {
                  lcd_puts ("DONE");
               }else{
                  lcd_puts ("FAIL");
               }
               goto out_without_disp;
               break;

               case SELECT_ISP_SETTING:
               lcd_draw_picture( 0, 0, IDP_BACKGROUND );
               lcd_goto_xy (25,30);
               lcd_puts ("ISP ON");
               asm_isp_jmp();
               goto out_without_disp;
               break;

               case SELECT_VERSION:
               mmi_setting_display_version();
               goto out_without_disp;
               break;

               case SELECT_MAIN_EXIT:
               mmi_setting_exit();
               goto out_without_disp;
               break;

               default:
               switch( ((setting_menu_path&(U16)MSK_SUB_MENU)+1) )
               { 
                  case SELECT_LANGUAGE:
                  g_setting_list.language_ihm = menu_contents.MenuIndex;
                  mmi_setting_back_to_main_menu();
                  break;

                  case SELECT_POWEROFF:
                  g_setting_list.poweroff_value = menu_contents.MenuIndex;
                  mmi_setting_back_to_main_menu();
                  break;

                  case SELECT_BACKLIGHTOFF:
                  g_setting_list.backlight_mode = menu_contents.MenuIndex;
                  mmi_setting_back_to_main_menu();
                  break;

                  case SELECT_BACKLIGHTTIMEOUT:
                  g_setting_list.backlight_timeout = 10*(menu_contents.MenuIndex+1);
                  mmi_setting_back_to_main_menu();
                  break;

                  case SELECT_FORMAT:
                  if( 0==menu_contents.MenuIndex )
                  {  // Yes
                     lcd_draw_picture( 0, 0, IDP_BACKGROUND );
                     lcd_goto_xy (25,30);
                     lcd_puts("Formatting...");
                     if( nav_drive_set( LUN_ID_NF_DISKMASS )&& nav_drive_format( FS_FORMAT_DEFAULT ))
                     {
                        lcd_draw_picture( 0, 0, IDP_BACKGROUND );
                        lcd_goto_xy( 25, 30 );
                        lcd_puts( "DONE" );
                     }
                     else
                     {
                        lcd_draw_picture( 0, 0, IDP_BACKGROUND );
                        lcd_goto_xy( 25, 30 );
                        lcd_puts( "FAIL" );
                     }
                     goto out_without_disp;
                  }
                  // No break here

                  case EXIT_FORMAT:
                  case EXIT_VERSION:
                  default:
                  mmi_setting_back_to_main_menu();
                  break;
               }
               break;
            }
            disp_Menu( SELECT );
out_without_disp:
            setting_menu_path <<= NB_BIT_SUB_MENU;
            break;

            case KBD_MMI_PREV:
            if( 0==(setting_menu_path & (U16)MSK_SUB_MENU))
            {
               // Main menu, then exit setting
               mmi_setting_exit();
               break;
            }
            mmi_setting_back_to_main_menu();
            disp_Menu( SELECT );
            break;

            case KBD_MMI_UP:
            if((setting_menu_path != (SELECT_FORMAT<<NB_BIT_SUB_MENU) )
            && (setting_menu_path != (SELECT_VERSION<<NB_BIT_SUB_MENU)))
            {
               disp_Menu(PREV);
            }
            break;
      
            case KBD_MMI_DOWN:
            if((setting_menu_path != (SELECT_FORMAT<<NB_BIT_SUB_MENU) )
            && (setting_menu_path != (SELECT_VERSION<<NB_BIT_SUB_MENU)))
            {
               disp_Menu(NEXT);
            }
            break;
         }
      }
#endif
#endif
      break;

      case EVT_USB_POWERED:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      //g_select_appli = MSTORAGE_APPLI;
      mmi_setting_exit();
      //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
      CommonLoadCode( C_USB );
      break;


	 case EVT_GOTO_OID_MODE:
	 	//g_select_appli = AP3_APPLI;
        mmi_setting_exit();
	 	break;

#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      case EVT_REMOTE:
      switch( LSB( param ))
      {
         /*case REMOTE_VOL_UP:
         case REMOTE_VOL_DOWN:
         case REMOTE_EXIT_MP3:
         case REMOTE_MP3_PLAY_PAUSE:
         case REMOTE_MP3_PREV:
         case REMOTE_MP3_NEXT:
         case REMOTE_REC_START:
         case REMOTE_REC_PLAY:
         case REMOTE_REC_STOP:
         break;*/
      
         case REMOTE_POWER_OFF:
         if( power_is_vbus_present())
            break;
         g_b_remote_on = FALSE;
         g_select_appli = POWER_OFF_APPLI;
         g_power_off_mode = POWER_OFF;
         mmi_setting_exit();
         break;
      
         case REMOTE_CAL_MODE:
         bRemoteEnterCal = TRUE;
         g_select_appli = AP3_APPLI;
         mmi_setting_exit();
         break;
         
         case REMOTE_AP4_MODE:
         //g_b_remote_on = FALSE;
         bRemoteEnterCal = FALSE;
         g_select_appli = AP3_APPLI;
         mmi_setting_exit();
         break;
      
         case REMOTE_REC_MODE:
         g_select_appli = RECORDER_APPLI;
         recorder_mode = 0;
         mmi_setting_exit();
         break;
      
         case REMOTE_MP3_MODE:
         g_select_appli = PLAYER_APPLI;
         mmi_setting_exit();
         break;

#if( REMOTE_PASTER == ENABLE )
         case REMOTE_PASTER_MODE:
         g_select_appli = PASTER_APPLI;
         mmi_setting_exit();
         break;
#endif   //#if( REMOTE_PASTER == ENABLE )

         case REMOTE_DIC_MODE:
         g_select_appli = DIC_APPLI;
         mmi_setting_exit();
         break;
      }
      break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/
      
      default:
      event = event;
      Mmgr_krn_forward_current_event();
      break;
   }


   setting_state_change();
   
}

#if 0
//! mmi_setting_back_to_main_menu()
//! Retrieve the position in the amin menu from the path
void mmi_setting_back_to_main_menu()
{
   do
   {
      setting_menu_path >>= NB_BIT_SUB_MENU;
   }
   while( 0!=(setting_menu_path & (U16)MSK_SUB_MENU));

   menu_contents.MenuIndex = LSB(setting_menu_path) -1;
   setting_menu_path = 0;

   menu_contents.Title     = "Setting Menu";
   //menu_contents.IDP_Head = -1;
   menu_contents.MenuNum   = NB_SETTING_MAIN_MENU;
   menu_contents.MenuTxt   = s_setting_name;
}

void mmi_setting_display_version( void )
{
   _MEM_TYPE_SLOW_ U8 buf[8];
   U8 u8_i, id_codec;

   Ih_set_curr_font( IDC_FONT_16 );
   // Draw back ground
   lcd_draw_picture( 0, 0, IDP_BACKGROUND );
   lcd_goto_xy (4,POSY_WND_TITLE);
   Lcd_set_merging();
   // Write firmware version
   lcd_puts( WP_NAME );
   // Write nand flash ID
   sprintf(buf, " %02bX %02bX", G_DEV_MAKER, G_DEV_ID);
   lcd_puts( buf );
   Lcd_set_merging();
 
   // Display codec version
   for( u8_i=0; u8_i<5 ; u8_i++ )
   {
      switch( u8_i )
      {
         case 0:
         sprintf(buf, "MP3 " );
         id_codec = CODEC_MP3;
         break;
         case 1:
         sprintf(buf, "MP2 " );
         id_codec = CODEC_MP2;
         break;
         case 2:
         sprintf(buf, "WMA " );
         id_codec = CODEC_WMA;
         break;
         case 3:
         sprintf(buf, "WAV " );
         id_codec = CODEC_WAV;
         break;
         case 4:
         sprintf(buf, "G726 " );
         id_codec = CODEC_REC_G726;
         break;
      }
      lcd_goto_xy( 35, 15 + (u8_i*9) );
      lcd_puts( buf );
      if( PASS == codec_version( id_codec ,  g_string ))
      {
         lcd_puts("V");
         lcd_puts(g_string);
      }else{
         lcd_puts("not load");
      }
   }
}
#endif


#if( LCD_PRESENT == ENABLE )
void display_menu( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8) );
   lcd_goto_xy( 0, 12 );
   Ih_set_curr_font( IDC_FONT_16 );
    switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
		  lcd_puts(( U8 *)s_setting_name_english[ setting_current_mark ]);
	      break;
	      
	      case LANG_CHINESE_SIMP:
		  lcd_puts(( U8 *)s_setting_name_chinese_simp[ setting_current_mark ]);
	      break;

	      case LANG_CHINESE_TRAD:
	      break;
	   }
   
/*   
#if( SELECT_LANGUAGE == DISABLE )
   lcd_puts(( U8 *)s_setting_name[ MenuIndex - 1 ]);
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_puts(( U8 *)s_setting_name_english[ MenuIndex - 1 ]);
      break;
      
      case LANG_CHINESE_SIMP:
      lcd_puts(( U8 *)s_setting_name_chinese_simp[ MenuIndex - 1 ]);
      break;

      case LANG_CHINESE_TRAD:
      lcd_puts(( U8 *)s_setting_name_chinese_trad[ MenuIndex - 1 ]);
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
*/
}


void disp_Menu( U8 dir )
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

   if( g_setting_list.struct_setting.language_ihm == LANG_ENGLISH )
      lcd_goto_xy( 112, 12 );
   else
      lcd_goto_xy( 88, 12 );
   //sprintf( string , "%02bu", dir );
   if(dir != 0xAA){
	   memset( string, 0, 8 );
	   convert_U8(( char *)&string[ 4 ], dir );
	   i = strlen(( char *)&string[ 4 ] );
	   for( ; i < 2; i++ )
	      strcat(( char *)string, "0" );
	   strcat(( char *)string, ( char *)&string[ 4 ] );
	   Ih_set_curr_font( IDC_FONT_16 );
	   lcd_puts( string );
   	}
   else{
   	   Ih_set_curr_font( IDC_FONT_16 );
	   switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
		  lcd_puts(( U8 *)"Close" );
	      break;
	      
	      case LANG_CHINESE_SIMP:
		  lcd_puts(( U8 *)"关" );
	      break;

	      case LANG_CHINESE_TRAD:
		  lcd_puts(( U8 *)"闽" );
	      break;
	   }
   	}
#if 0
   _MEM_TYPE_SLOW_ U8 i,Pos, PosIndex,TotalPage,CurrPage;

   if (PREV == dir)
   {
      if ( menu_contents.MenuIndex != 0 )
      {
         menu_contents.MenuIndex--;
         if(( menu_contents.MenuIndex % MENU_NB_LINE )!=( MENU_NB_LINE - 1 ))
            goto disp_Menu_display_cursor;  // just display cursor
      }
      else
      {
         menu_contents.MenuIndex = menu_contents.MenuNum -1;
//***********************************************
         //add by xuanyu
         if( menu_contents.MenuNum <= MENU_NB_LINE )
            goto disp_Menu_display_cursor;  // just display cursor
//***********************************************
      }
   }
   if ( NEXT == dir )
   {
      if ( menu_contents.MenuIndex == menu_contents.MenuNum-1 )
      {
         menu_contents.MenuIndex = 0;
//***********************************************
         //add by xuanyu
         if( menu_contents.MenuNum <= MENU_NB_LINE )
            goto disp_Menu_display_cursor;  // just display cursor
//***********************************************
      }
      else
      {
         menu_contents.MenuIndex ++;
         if( 0 != (menu_contents.MenuIndex%MENU_NB_LINE) )
            goto disp_Menu_display_cursor;  // just display cursor
      }
   }

   TotalPage = menu_contents.MenuNum / MENU_NB_LINE;
   CurrPage = menu_contents.MenuIndex / MENU_NB_LINE;

   //lcd_draw_picture( 0, 0, IDP_BACKGROUND );
   lcd_clear_screen();

   lcd_goto_xy( POSX_WND_TITLE, POSY_WND_TITLE );
   Lcd_set_merging();
   lcd_puts(menu_contents.Title);
   Lcd_unset_merging();

   /*if (menu_contents.MenuNum>MENU_NB_LINE)
   {
      if ( CurrPage == TotalPage )
         Pos = menu_contents.MenuNum % MENU_NB_LINE;
      else
         Pos = MENU_NB_LINE;
      for (i=0;i<=Pos-1;i++)
      {
         lcd_goto_xy( SETTING_POSX_NAME_CURRENT_FILE, MenuYPos[i] );
         lcd_puts( menu_contents.MenuTxt[CurrPage*MENU_NB_LINE+i] );
      }
   }
   else
   {
      for (i=0;i<=menu_contents.MenuNum-1;i++)
      {
         lcd_goto_xy( SETTING_POSX_NAME_CURRENT_FILE, MenuYPos[i] );
         lcd_puts( menu_contents.MenuTxt[i] );
      }
   }*/
   
   status_display();

/*disp_Menu_display_cursor:
   // Display cursor
   PosIndex = menu_contents.MenuIndex % MENU_NB_LINE;
   lcd_clear_part_of_screen( SETTING_POSX_ICON_CURSOR, 18, 5, 40 );
   lcd_draw_picture( SETTING_POSX_ICON_CURSOR, MenuYPos[ PosIndex ]+ 3, IDP_CURSOR );*/
#endif
}




/*
void display_playmode(void)
{
 	U16 i,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, 30, (LCD_HEIGHT-24));
	lcd_draw_picture(12,32+20*(mark%SETTING_LINE_PAGE), IDP_PLAYER_PLAY);
    return;
}
*/



void setting_menu_display(U16 display_num_mark)
{
	U16 i,j,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   temp_file = (display_num_mark-1)*SETTING_LINE_PAGE;
   if((Menunum- temp_file) >= SETTING_LINE_PAGE ){
   	j = SETTING_LINE_PAGE;
   	}
   else{
   	j = SETTING_LINE_PAGE-1;
   	}
   Ih_set_curr_font( IDC_FONT_16 );
  for(i=0;i<j;i++ ){
 	  lcd_goto_xy( 32, 28+i*20 );  
	  switch( g_setting_list.struct_setting.language_ihm )
       {
          case LANG_ENGLISH:
          lcd_puts( ( U8 *)s_setting_name_english[temp_file ] );
          break;
          
          case LANG_CHINESE_SIMP:
           lcd_puts( ( U8 *)s_setting_name_chinese_simp[temp_file ] );
          break;

          case LANG_CHINESE_TRAD:
          break;
       }
	  
	  temp_file ++;
  	}
}

void setting_mark_display(U8 mark)
{
	U16 i,temp_file;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 28, 30, (LCD_HEIGHT-28));
	lcd_draw_picture(12,32+20*(mark%SETTING_LINE_PAGE), IDP_PLAYER_PLAY);
}


void setting_port_display(U8 temp_port)
{
	U16 i,j,temp_file,temp_num,temp_line;
	SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 32, 28, (LCD_WIDTH-32), (LCD_HEIGHT-28));
 /*  
   if(temp_port == 0){
   	temp_file = 0;
   	}
   else{
   	 temp_file = (temp_port-1)*SETTING_LINE_PAGE;
   	}
   */
   temp_file = temp_port+1;
   if(State_setting == Poweroff_Setting){
   	temp_num = NB_SETTING_POWEROFF;
	temp_line = 2;
   	}
    if(State_setting == Blacklight_Setting){
   	temp_num = NB_SETTING_BACKLIGHTTIMEOUT;
	temp_line = 2;
   	}

    if(State_setting == PlayMode_Setting){
   	temp_num = NB_SETTING_PLAYMODE;
	temp_line = 2;
	/*
    if(temp_file>2){
		temp_line = 1;
    	}
	else{
		temp_line = 2;
		}
	*/
   	}

   if(State_setting == Bluetooth_Setting){
   	temp_num = NB_SETTING_BLUETOOTH;
	temp_line = 2;
   	}

    if(State_setting == Language_Setting){
   	temp_num = NB_SETTING_LANGUAGE;
	temp_line = 2;
   	}

   if(temp_port >= 2){
   	temp_port = 2;
   	}
   else{
   	temp_port = 0;
   	}
#if 1   
    	j = temp_line;
#else
    if(temp_file <= 2 ){
   	j = SETTING_LINE_PAGE;
   	}
   else{
   	 if(temp_num - temp_file>=2){
	 	j = SETTING_LINE_PAGE;
   	 	}
	 else{
   			j = SETTING_LINE_PAGE-1;
	 	}
   	}
 #endif  
  Ih_set_curr_font( IDC_FONT_16 );
  for(i=0;i<j;i++ ){
 	  lcd_goto_xy( 32, 28+i*20 );  

	switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
			  	if(State_setting == Poweroff_Setting){
 		 	   	lcd_puts( ( U8 *)s_setting_poweroff_en_list[temp_port ] );
	   			}
               if(State_setting == Blacklight_Setting){
			   	lcd_puts( ( U8 *)s_setting_backlighttimeout_en_list[temp_port ] );
			   	}

			    if(State_setting == PlayMode_Setting){
			   	lcd_puts( ( U8 *)s_setting_playmode_en_list[temp_port ] );
			   	}

			   if(State_setting == Bluetooth_Setting){
			   	lcd_puts( ( U8 *)s_setting_bluetooth_en_list[temp_port ] );
			   	}
               if(State_setting == Language_Setting){
			   	lcd_puts( ( U8 *)s_setting_language_en_list[temp_port ] );
			   	}
              break;
              
              case LANG_CHINESE_SIMP:
			  if(State_setting == Poweroff_Setting){
 		 	   	lcd_puts( ( U8 *)s_setting_poweroff_chm_list[temp_port ] );
	   			}
               if(State_setting == Blacklight_Setting){
			   	lcd_puts( ( U8 *)s_setting_backlighttimeout_chm_list[temp_port ] );
			   	}

			    if(State_setting == PlayMode_Setting){
			   	lcd_puts( ( U8 *)s_setting_playmode_chm_list[temp_port ] );
			   	}

			   if(State_setting == Bluetooth_Setting){
			   	lcd_puts( ( U8 *)s_setting_bluetooth_chm_list[temp_port ] );
			   	}
			    if(State_setting == Language_Setting){
			   	lcd_puts( ( U8 *)s_setting_language_chm_list[temp_port ] );
			   	}
              break;
			  
              default:
              break;
           }
	   
	   
	  temp_port ++;
  	}
}

void display_disk_properties( void )
{
   U16 u16_tmp1, u16_tmp2;
   __X FS_ERR err;
   U32 free_space, total_space;
   char string[ 16 + 6 ];

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_goto_xy( 4, 8 );
   lcd_puts (( U8 *)"Disk Properties");
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 43, 24 );
   lcd_puts (( U8 *)"Wait...");

   free_space = FSFreeSpace( 0, &err );
   total_space = sdtl_get_size();
   if( err == FS_NO_ERROR )
   {
      u16_tmp1 = ( U16 )( total_space /( 1024uL * 2 ));      // Size Mo
      u16_tmp2 = ( U16 )( free_space * 100 / total_space );   // %
   }
   else
   {
      u16_tmp1 = 0;
      u16_tmp2 = 0;
   }

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_goto_xy( 0, 8 );
   lcd_puts (( U8 *)"Disk(s): SD");
   lcd_goto_xy( 0, 16 );
   //sprintf( g_string, "Size(Mo): %u", u16_tmp1 ); 
   //lcd_puts( g_string );
   strcpy( string, "Size(Mo): " );
   memset( &string[ 16 ], 0, 6 );
   convert_U16(( char *)&string[ 16 ], u16_tmp1 );
   strcat( string, ( char *)&string[ 16 ] );
   lcd_puts(( U8 *)string );

   lcd_goto_xy( 0, 24 );
   //sprintf( g_string, "Free(%%) : %u", u16_tmp2 ); 
   //lcd_puts( g_string );
   strcpy( string, "Free(%) : " );
   memset( &string[ 16 ], 0, 6 );
   convert_U16(( char *)&string[ 16 ], u16_tmp2 );
   strcat( string, ( char *)&string[ 16 ] );
   lcd_puts(( U8 *)string );
}

Bool display_custemtel( void )
{
   FS_ERR fs_err;
   __X U32 file_size;
   __X FCFILE *Temp_File;
  unsigned long temp_len;

  char temp_file[32];	
  
//  char temp_file[]="0:\\subdir\\tel.sys";
  strcpy( temp_file,SUBDIR_PATH );
   strcat( temp_file, "tel.sys" );
   
  
  temp_len = FSGetFileSize((char *)temp_file, &fs_err);
	if(temp_len>0){ 
	   	Temp_File = FSOpen((char *)temp_file,FS_MODE_READ,&fs_err);   
		FSSeek(Temp_File,0);
		memset(g_string,0,G_STRING_SIZE);
		FSRead(Temp_File, (void *)&g_string,temp_len,&fs_err);
		FSClose(Temp_File);
		return TRUE;
		}	
	else{
		return FALSE;
		}
}


/*
void ready_version(void)
{
   //product[0] = HARD_VERSION;USB_DEVICE_MS_PRODUCT         "OID4 V2.0.4.2B  "
   //const unsigned char product[16]={' ',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE_VERSION,'-',H_YEAR,L_YEAR,H_MONTH,L_MONTH,H_DAY,L_DAY,' ',' ',' ',' '};
   memset(product,0,sizeof(16));
   strcpy((char *)product,(char *)HARD_VERSION);
   strcat((char *)product,(char *)MAJOR_FW_VERSION);
   strcat((char *)product,(char *)MINOR_FW_VERSION);
   strcat((char *)product,(char *)OID_TYPE_VERSION);
   strcat((char *)product,(char *)"-");
   strcat((char *)product,(char *)NOW_DATA);
   strcat((char *)product,(char *)" ");
   SendString((U8 *)"\r\nproduct=");
   SendString((U8 *)product);
}
//*/

void display_version(void)
{
	char temp[32];
	memset(temp,0,sizeof(temp));
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
	lcd_clear_part_of_screen(0, 28, LCD_WIDTH, (LCD_HEIGHT-28));
	Ih_set_curr_font( IDC_FONT_16 );
#if 1
	lcd_goto_xy( 40, 30 );
	strcpy(temp,(char *)HARD_VERSION);
	strcat(temp,(char *)".");
	strcat(temp,(char *)MAJOR_FW_VERSION);
	strcat(temp,(char *)".");
	strcat(temp,(char *)MINOR_FW_VERSION);
	strcat(temp,(char *)".");
	strcat(temp,(char *)OID_TYPE_VERSION);
	strcat(temp,(char *)" ");
	//strcat(temp,(char *)NOW_DATA);
	//strcat(temp,(char *)__DATE__);
	lcd_puts(( U8 *)temp );
#else
	lcd_goto_xy( 40, 30 );
	strcpy(temp,(char *)HARD_VERSION);
	strcat(temp,(char *)'.');
	strcat(temp,(char *)MAJOR_FW_VERSION);
	strcat(temp,(char *)'.');
	strcat(temp,(char *)MINOR_FW_VERSION);
	strcat(temp,(char *)'.');
	strcat(temp,(char *)OID_TYPE_VERSION);
	lcd_puts(( U8 *)VERSION_NAME );
#endif

}

void display_tel(void)
{
  SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  lcd_clear_part_of_screen(0, 28, LCD_WIDTH, (LCD_HEIGHT-28));
  Ih_set_curr_font( IDC_FONT_16 );
  lcd_goto_xy( 40, 30 );
  if(display_custemtel()){
  	lcd_puts(( U8 *)g_string);
  	}
  else{
 	 lcd_puts(( U8 *)"400-169-8377" );
  	}
}

void display_readme(void)
{
  SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
  lcd_clear_part_of_screen(70, 12, (LCD_WIDTH-70), 16);
  Ih_set_curr_font( IDC_FONT_16 );
  lcd_goto_xy( 70, 12 );
  //lcd_puts((U8 *)"(按音量键调整)");
  switch( g_setting_list.struct_setting.language_ihm )
       {
          case LANG_ENGLISH:
          lcd_puts((U8 *)"(V+- repair)");
          break;
          
          case LANG_CHINESE_SIMP:
          lcd_puts((U8 *)"(按音量键调整)");
          break;

          case LANG_CHINESE_TRAD:
          break;
       }
}


void ready_setting_all_display(void)
{
	 switch( State_setting )
        {
           case Poweroff_Setting:	
//		   setting_port_mark = g_setting_list.struct_setting.poweroff_value;
//			SendString((U8 *)"\r\nINIT_CURRENT_POWEROFFVALUE=");
//			send_hex_u8(CURRENT_POWEROFFVALUE);
//           setting_port_display( CURRENT_POWEROFFVALUE );
//		   setting_mark_display( CURRENT_POWEROFFVALUE ); 
 		   setting_port_display( setting_powerofftime );
		   setting_mark_display( setting_powerofftime ); 
           break;

           case Blacklight_Setting:
		   	SendString((U8 *)"\r\nINIT_CURRENT_BLACKLIGHT=");
			send_hex_u8(CURRENT_BLACKLIGHTTIME);
		   setting_mark_display(CURRENT_BLACKLIGHTTIME);
           setting_port_display(CURRENT_BLACKLIGHTTIME);
           break;

           case Volume_Setting:
           //disp_Menu( g_setting_list.struct_setting.player_volume );
           display_readme();
           display_volume(CURRENT_VOLUME);
           break;
		   
		   case PlayMode_Setting:
		   	SendString((U8 *)"\r\nINIT_CURRENT_PLAYMODE=");
			send_hex_u8(CURRENT_PLAYMODE);
		   setting_mark_display( CURRENT_PLAYMODE );	
		   setting_port_display( CURRENT_PLAYMODE );
		   break;
			
		   case Bluetooth_Setting:
		   //setting_mark_display( CURRENT_BLUETOOTH );
 		   //setting_port_display( CURRENT_BLUETOOTH );
 		   setting_mark_display( setting_bluetooth );
 		   setting_port_display( setting_bluetooth );
           break;
/*
		   case Date_Setting:
           display_readme();
		   display_date();
           break;
		   case Time_Setting:
           //display_alarm();
           display_readme();
		   display_time();
           break;
*/
		   case Language_Setting:
		   setting_mark_display( CURRENT_LANGUAGE );
 		   setting_port_display( CURRENT_LANGUAGE );
           break;

		   case Telphone_Setting:
		   	display_tel();
           break;

		   case Version_Setting:
		   display_version();
           break;
/*
#if( SELECT_LANGUAGE == ENABLE )
           case SELECT_LANGUAGE_MENU:
           switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
              break;
              
              case LANG_CHINESE_SIMP:
              g_setting_list.struct_setting.language_ihm = LANG_CHINESE_TRAD;
              break;

              case LANG_CHINESE_TRAD:
              g_setting_list.struct_setting.language_ihm = LANG_ENGLISH;
              break;

              default:
              //g_setting_list.language_ihm = LANG_CHINESE_SIMP;
              g_setting_list.struct_setting.language_ihm = LANGUAGE_DEFAULT;
              break;
           }
           display_menu();
           Ih_set_curr_font( IDC_FONT_16 );
           switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              lcd_goto_xy( 72, 12 );
              lcd_puts(( U8 *)"English" );
              break;
              
              case LANG_CHINESE_SIMP:
              lcd_goto_xy( 40, 12 );
              lcd_puts(( U8 *)"简体中文" );
              break;

              case LANG_CHINESE_TRAD:
              lcd_goto_xy( 40, 12 );
              lcd_puts(( U8 *)"羉砰いゅ");
              break;
           }
           break;
#endif   //#if( SELECT_LANGUAGE == ENABLE )
*/
        }
}



void change_setting_all_display(Bool temp_flag)
{
	 switch( State_setting )
        {
           case Poweroff_Setting:	
//		   	SendString((U8 *)"\r\nCURRENT_POWEROFFVALUE=");
//			send_hex_u8(CURRENT_POWEROFFVALUE);
		   if(temp_flag){
		   	if(setting_powerofftime == (NB_SETTING_POWEROFF - 1))break;
				  	setting_powerofftime ++;
					setting_mark_display(setting_powerofftime); 
					setting_port_display( setting_powerofftime );
		   	}
		   else{
		   	if(setting_powerofftime == 0)return;
				  	setting_powerofftime --;
					setting_port_display( setting_powerofftime );
					setting_mark_display( setting_powerofftime ); 
		   	}
           break;

           case Blacklight_Setting:
		   	SendString((U8 *)"\r\nCURRENT_BLACKLIGHTTIME=");
			send_hex_u8(CURRENT_BLACKLIGHTTIME);
		    if(temp_flag){
		   	if(CURRENT_BLACKLIGHTTIME >= (NB_SETTING_BACKLIGHTTIMEOUT - 1))break;
				  	CURRENT_BLACKLIGHTTIME ++;
					setting_port_display( CURRENT_BLACKLIGHTTIME );
					setting_mark_display(CURRENT_BLACKLIGHTTIME); 
		
		   	}
		   else{
		   	if(CURRENT_BLACKLIGHTTIME == 0)break;
				  	CURRENT_BLACKLIGHTTIME --;
						setting_port_display( CURRENT_BLACKLIGHTTIME );
						setting_mark_display(CURRENT_BLACKLIGHTTIME); 
		   	}
		   if(CURRENT_BLACKLIGHTTIME){
		   	g_setting_list.struct_setting.backlight_mode    = BACKLIGHT_MODE_FULL_OFF;
		   	}
		   else{
		   	g_setting_list.struct_setting.backlight_mode    = BACKLIGHT_MODE_OFF;
		   	}
           break;

           case Volume_Setting:
 //          disp_Menu( g_setting_list.struct_setting.player_volume );
 		   if(temp_flag){
		   	if(CURRENT_VOLUME<(VOL_LEVEL-1)){
				CURRENT_VOLUME ++;
				display_volume(CURRENT_VOLUME);
		   		}
 		   	}
		   else{
		   	if(CURRENT_VOLUME>0){
				CURRENT_VOLUME --;
				display_volume(CURRENT_VOLUME);
		   		}
		   	}
           break;
		   
		   case PlayMode_Setting:
		   	SendString((U8 *)"\r\nCURRENT_PLAYMODE=");
			send_hex_u8(CURRENT_PLAYMODE);
		    if(temp_flag){
		   	if(CURRENT_PLAYMODE == (NB_SETTING_PLAYMODE - 1))return;
		  	CURRENT_PLAYMODE ++;
			setting_mark_display(CURRENT_PLAYMODE); 
			setting_port_display( CURRENT_PLAYMODE );
		   	}
		   else{
		   	if(CURRENT_PLAYMODE == 0)return;
			CURRENT_PLAYMODE --;
			setting_port_display( CURRENT_PLAYMODE );
			setting_mark_display(CURRENT_PLAYMODE); 
		   	}
		   break;
			
		   case Bluetooth_Setting:
		    if(temp_flag){
		   	if(setting_bluetooth == (NB_SETTING_BLUETOOTH - 1))return;
			setting_bluetooth ++;
			setting_mark_display(setting_bluetooth); 
		   	}
		   else{
		   	if(setting_bluetooth == 0)return;
			setting_bluetooth --;
			setting_mark_display(setting_bluetooth); 
		   	}
           break;
/*
		   case Date_Setting:
		   if(temp_flag){
		   	if(display_mark>=2)return;
			display_mark++;
			goto display_1;
		   	}
		   else{
		   	if(display_mark==0)return;
			display_mark--;
		   	}
display_1:		   
			switch(display_mark)
				{
				case 0:
					display_mark = 0;
					display_bcd(TRUE,CURRENT_YEAR);
					display_mark++;
					display_bcd(FALSE,CURRENT_MONTH);
					display_mark++;
					display_bcd(FALSE,CURRENT_DAY);
					display_mark = 0;
					break;
				case 1:
					display_mark = 0;
					display_bcd(FALSE,CURRENT_YEAR);
					display_mark++;
					display_bcd(TRUE,CURRENT_MONTH);
					display_mark++;
					display_bcd(FALSE,CURRENT_DAY);
					display_mark = 1;
					break;
				case 2:
					display_mark = 0;
					display_bcd(FALSE,CURRENT_YEAR);
					display_mark++;
					display_bcd(FALSE,CURRENT_MONTH);
					display_mark++;
					display_bcd(TRUE,CURRENT_DAY);	
					display_mark = 2;
					break;
				}   
           //display_alarm();
           break;
		   case Time_Setting:
           if(temp_flag){
		   	if(display_mark>=1)return;
			display_mark++;
			goto display_2;
		   	}
		   else{
		   	if(display_mark==0)return;
			display_mark--;
		   	}
display_2:		   
		   	switch(display_mark)
				{
				case 0:
					display_mark = 0;
					display_bcd(TRUE,CURRENT_HOUR);
					display_mark++;
					display_bcd(FALSE,CURRENT_MIN);
					display_mark = 0;
					break;
				case 1:
					display_mark = 0;
					display_bcd(FALSE,CURRENT_HOUR);
					display_mark++;
					display_bcd(TRUE,CURRENT_MIN);
					display_mark = 1;
					break;
				} 
           break;
*/

		 case Language_Setting:
		    if(temp_flag){
		   	if(CURRENT_LANGUAGE== (NB_SETTING_LANGUAGE- 1))return;
			CURRENT_LANGUAGE ++;
			setting_mark_display(CURRENT_LANGUAGE); 
		   	}
		   else{
		   	if(CURRENT_LANGUAGE == 0)return;
			CURRENT_LANGUAGE --;
			setting_mark_display(CURRENT_LANGUAGE); 
		   	}
           break;

		   case Telphone_Setting:
//		   	display_tel();
           break;

		   case Version_Setting:
//		   display_version();
           break;
/*
#if( SELECT_LANGUAGE == ENABLE )
           case SELECT_LANGUAGE_MENU:
           switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
              break;
              
              case LANG_CHINESE_SIMP:
              g_setting_list.struct_setting.language_ihm = LANG_CHINESE_TRAD;
              break;

              case LANG_CHINESE_TRAD:
              g_setting_list.struct_setting.language_ihm = LANG_ENGLISH;
              break;

              default:
              //g_setting_list.language_ihm = LANG_CHINESE_SIMP;
              g_setting_list.struct_setting.language_ihm = LANGUAGE_DEFAULT;
              break;
           }
           display_menu();
           Ih_set_curr_font( IDC_FONT_16 );
           switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              lcd_goto_xy( 72, 12 );
              lcd_puts(( U8 *)"English" );
              break;
              
              case LANG_CHINESE_SIMP:
              lcd_goto_xy( 40, 12 );
              lcd_puts(( U8 *)"简体中文" );
              break;

              case LANG_CHINESE_TRAD:
              lcd_goto_xy( 40, 12 );
              lcd_puts(( U8 *)"羉砰いゅ");
              break;
           }
           break;
#endif   //#if( SELECT_LANGUAGE == ENABLE )
*/
        }
}

void find_powerofftime(void)
{
	U8 i;
	for(i=0;i<4;i++){
		if(CURRENT_POWEROFFVALUE == poweroff_setting[i]){
			setting_powerofftime = i;
			return;
			}
		}
	setting_powerofftime = 2;
}

void setting_state_change(void)
{

    switch(State_setting){
   	 case Init_Setting:
	  
	  find_powerofftime(); 
      lcd_turn_on();
      Lcd_start_backlight_timer();
#if 1
	 if(CURRENT_BLUETOOTH > 0){
	 	 setting_bluetooth  = 1;
	 	}
	 else{
	 	 setting_bluetooth  = 0;
	 	}
#else
      setting_bluetooth  = CURRENT_BLUETOOTH;
#endif	  
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//      lcd_clear_screen();
	  lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
      lcd_goto_xy( 0, 8 );
      Ih_set_curr_font( IDC_FONT_16 );
	  switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              lcd_puts(( U8 *)"Setting" );
              break;
              
              case LANG_CHINESE_SIMP:
               lcd_puts(( U8 *)"系统设置" );
              break;

              case LANG_CHINESE_TRAD:
              break;
           }
     
/*	  
      lcd_goto_xy( POSX_WND_TITLE, POSY_WND_TITLE );
      Ih_set_curr_font( IDC_FONT_08 );
      lcd_puts(( U8 *)"SETTING" );
*/	  
      
	  //Menunum = 8;
	  Menunum = NB_SETTING_MAIN_MENU;
	  setting_current_mark = 0;
	  setting_current_page_num = 1;
	  setting_page_num = (Menunum+1)/2;
      MenuIndex = SELECT_POWEROFF_MENU;
//    display_menu();
//     disp_Menu( g_setting_list.struct_setting.poweroff_value );
	  status_display();
//	  State_setting = Currentstate_Setting;
//	 	break;
	 case Currentstate_Setting:
	 	SendString((U8 *)"\r\nCurrentstate_Setting=");
		send_hex_u16(setting_current_page_num);
	 	setting_menu_display(setting_current_page_num);
		setting_mark_display(setting_current_mark);
		State_setting = Browser_Setting;
		break;
	 case Browser_Setting:
	 	SendString((U8 *)"\r\nBrowser_Setting");
//	 	setting_menu_display(setting_current_page_num);
//		setting_mark_display();
	 	break;
	 case Poweroff_Setting:
	 	break;
		
	 case Blacklight_Setting:
	 	break;
	 case Volume_Setting:
	 	break;
	 case PlayMode_Setting:
	 	break;
	 case Bluetooth_Setting:
	 	break;
/*		
	 case Date_Setting:
	 	break;
	 case Time_Setting:
	 	break;
*/		
	 case Language_Setting:
	 	break;
		
	 case Telphone_Setting:
       
	 	break;
	 case Version_Setting:
       
	 	break;
	 case Backtotop_Setting:
	  SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
	  lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
      lcd_goto_xy( 0, 8 );
      Ih_set_curr_font( IDC_FONT_16 );
 //     lcd_puts(( U8 *)"系统设置" );
	  switch( g_setting_list.struct_setting.language_ihm )
           {
              case LANG_ENGLISH:
              lcd_puts(( U8 *)"Setting" );
              break;
              
              case LANG_CHINESE_SIMP:
               lcd_puts(( U8 *)"系统设置" );
              break;

              case LANG_CHINESE_TRAD:
              break;
           }
//	  SendString((U8 *)"\r\nCurrentstate_Setting");
	  setting_menu_display(setting_current_page_num);
	  setting_mark_display(setting_current_mark);
	  State_setting = Browser_Setting;
	 default:
	 	break;
   	}
}

#endif  //#if( LCD_PRESENT == ENABLE )

#if 1
//! Exit this mmi appli properly
//!
void mmi_setting_exit( void )
{
   //if( setting_change_flag )
   /*if(( g_setting_list.player_volume != srvplayer_g_arg.volume )
      ||( g_setting_list.player_repeat != srvplayer_g_arg.repeat )
         ||( g_setting_list.language_ihm != setting_language_ihm )
            ||( g_setting_list.poweroff_value != setting_poweroff_value )
               ||( g_setting_list.backlight_mode != setting_backlight_mode )
                  ||( g_setting_list.backlight_timeout != setting_backlight_timeout )
                     || setting_change_flag )
   {
      srvplayer_g_arg.volume = g_setting_list.player_volume;
      g_setting_list.player_repeat = srvplayer_g_arg.repeat;
      setting_language_ihm = g_setting_list.language_ihm;
      setting_poweroff_value = g_setting_list.poweroff_value;
      setting_backlight_mode = g_setting_list.backlight_mode;
      setting_backlight_timeout = g_setting_list.backlight_timeout;
      setting_save( (U8*)&g_setting_list );  // Save setting
      setting_change_flag = FALSE;
   }*/
   /*SPIF_ERR SPI_STATE;

   GetDataIdInfo( D_SETTINGS, &Start_Sector, &Length );
   SPI_STATE = spif_resv_read( Start_Sector, ( __X void *)Buf );
   SendString((U8 *)"read\r\n");
   if( SPI_STATE == SDTL_No_Err )
   {
      SendString((U8 *)"compare\r\n");
      if( memcmp(( U8 *)g_setting_list.buffer, &Buf[ 1 + SETTING_VERSION_LEN ] , sizeof( SETTING_NEW )))
      {
         SendString((U8 *)"save\r\n");
         Buf[ 0 ] = 1 + SETTING_VERSION_LEN + sizeof( SETTING_NEW );
         memcpy( &Buf[ 1 ], SETTING_VERSION , sizeof(SETTING_VERSION) );
         memcpy( &Buf[ 1 + SETTING_VERSION_LEN ], ( U8 *)g_setting_list.buffer, sizeof( SETTING_NEW ));
         spif_resv_write( Start_Sector, ( __X void *)Buf );
      }
   }*/
   
   if(setting_powerofftime < 4){
   	}
   else{
   	  setting_powerofftime = 2;
   	}
   CURRENT_POWEROFFVALUE = poweroff_setting[setting_powerofftime];

	if(setting_bluetooth > 0){
		 setting_bluetooth  = 0xFF;
		}
	else{
		 setting_bluetooth  = 0x00;
		}
  
   if(setting_bluetooth == g_setting_list.struct_setting.bluetooth){
   	}
   else{
   if(setting_bluetooth){
   	    SendString((U8 *)"\r\nready oid2");
		if(set_t01_one_mode(OID_SONIX_2_COMMAND,OID_SONIX_2_COMMAND_ACK)){
			//set_t01_mode(UserCmd_SetMode_Waiyan, UserCmd_SetModeParam1_Waiyan, UserCmd_SetModeParam2_Waiyan,OIDCmd_SetMode_Ack_Waiyan);
			(void)set_t01_one_mode(UserCmd_Waiyan_Command_01_Param1,UserCmd_Waiyan_Ack_01);
		set_t01_mode(UserCmd_Waiyan_Command_02_Param1, UserCmd_Waiyan_Command_02_Param2, UserCmd_Waiyan_Command_02_Param3,UserCmd_Waiyan_Ack_02);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_03_Param1,UserCmd_Waiyan_Ack_03);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_04_Param1,UserCmd_Waiyan_Ack_04);
		set_t01_mode(UserCmd_Waiyan_Command_05_Param1, UserCmd_Waiyan_Command_05_Param2, UserCmd_Waiyan_Command_05_Param3,UserCmd_Waiyan_Ack_05);
		set_t01_mode(UserCmd_Waiyan_Command_06_Param1, UserCmd_Waiyan_Command_06_Param2, UserCmd_Waiyan_Command_06_Param3,UserCmd_Waiyan_Ack_06);
		set_t01_mode(UserCmd_Waiyan_Command_07_Param1, UserCmd_Waiyan_Command_07_Param2, UserCmd_Waiyan_Command_07_Param3,UserCmd_Waiyan_Ack_07);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_08_Param1,UserCmd_Waiyan_Ack_08);
			SendString((U8 *)"\r\nset to oid2");
			g_setting_list.struct_setting.bluetooth = setting_bluetooth;
			}
   		}
	else{
		SendString((U8 *)"\r\nready dpr3");
		if(set_t01_one_mode(OID_DPR3_COMMAND,OID_DPR3_COMMAND_ACK)){
			SendString((U8 *)"\r\nset to dpr3");
			g_setting_list.struct_setting.bluetooth = setting_bluetooth;
			}
		}
   	}
	//b_oid_init_flag = FALSE;
   Mmgr_kill_this_mmi_appli();
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



void display_volume(U8 temp_data)
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
	lcd_clear_part_of_screen(0, 28, LCD_WIDTH, (LCD_HEIGHT-28));
	Ih_set_curr_font( IDC_FONT_16 );
	lcd_goto_xy( 40, 30 );

	 memset( string, 0, 8 );
	 convert_U8(( char *)&string[ 4 ], temp_data );
	 i = strlen(( char *)&string[ 4 ] );
	 for( ; i < 2; i++ )
	      strcat(( char *)string, "0" );
	   strcat(( char *)string, ( char *)&string[ 4 ] );
	   Ih_set_curr_font( IDC_FONT_16 );
	   lcd_puts( string );
    //lcd_puts(( U8 *)VERSION_NAME );
}


/*

void repair_date(Bool direction_flag)
{
	switch(display_mark)
		{
		case 0:
			if(direction_flag){
				if(CURRENT_YEAR>100)return;
				CURRENT_YEAR++;
				}
			else{
				if(CURRENT_YEAR<=14)return;
				CURRENT_YEAR--;
				}
			display_bcd(TRUE,CURRENT_YEAR);
			break;
		case 1:
			if(direction_flag){
				if(CURRENT_MONTH >= 12)return;
				CURRENT_MONTH++;
				}
			else{
				if(CURRENT_MONTH<=1)return;
				CURRENT_MONTH--;
				}
			display_bcd(TRUE,CURRENT_MONTH);
			break;
		case 2:
			if(direction_flag){
				if(CURRENT_DAY>=31)return;
				CURRENT_DAY++;
				}
			else{
				if(CURRENT_DAY<=1)return;
				CURRENT_DAY--;
				}
			display_bcd(TRUE,CURRENT_DAY);
			break;
		default:
			break;
				
		}
}

void display_bcd(Bool flag_inverse,U8 temp_data )
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;
	switch(display_mark)
		{
		case 0:
			lcd_goto_xy( 34, 30 );
			break;
		case 1:
			lcd_goto_xy( 54, 30 );
			break;
		case 2:
			lcd_goto_xy( 74, 30 );
			break;
		default:
			return;
		}
	

	 memset( string, 0, 8 );
	 convert_U8(( char *)&string[ 4 ], temp_data );
	 i = strlen(( char *)&string[ 4 ] );
	 for( ; i < 2; i++ )
	      strcat(( char *)string, "0" );
	   strcat(( char *)string, ( char *)&string[ 4 ] );
	   Ih_set_curr_font( IDC_FONT_16 );
	   if(flag_inverse){
	   	lcd_g_inverse = TRUE;
	   	}
	   lcd_puts( string );
       lcd_g_inverse = FALSE;
}


void display_date(void)
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;
    
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
	lcd_clear_part_of_screen(0, 28, LCD_WIDTH, (LCD_HEIGHT-28));
	Ih_set_curr_font( IDC_FONT_16 );
	lcd_goto_xy( 26, 30 );
	lcd_puts((U8 *)"20");
	display_mark = 0;
    display_bcd(TRUE,CURRENT_YEAR);
	lcd_goto_xy( 42, 30 );
	lcd_puts((U8 *)"年");
    display_mark ++;
    display_bcd(FALSE,CURRENT_MONTH);

	lcd_goto_xy( 62, 30 );
	lcd_puts((U8 *)"月");
	 display_mark ++;
    display_bcd(FALSE,CURRENT_DAY);
	lcd_goto_xy( 82, 30 );
	lcd_puts((U8 *)"日");
	display_mark = 0;
}





void repair_time(Bool direction_flag)
{
	switch(display_mark)
		{
		case 0:
			if(direction_flag){
				if(CURRENT_HOUR>=23)return;
				CURRENT_HOUR++;
				}
			else{
				if(CURRENT_HOUR==00)return;
				CURRENT_HOUR--;
				}
			display_bcd(TRUE,CURRENT_HOUR);
			break;
		case 1:
			if(direction_flag){
				if(CURRENT_MIN>= 59)return;
				CURRENT_MIN++;
				}
			else{
				if(CURRENT_MIN<=1)return;
				CURRENT_MIN--;
				}
			display_bcd(TRUE,CURRENT_MIN);
			break;
		default:
			break;
				
		}
}




void display_time(void)
{
    _MEM_TYPE_SLOW_ U8 string[ 8 ], i;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
	lcd_clear_part_of_screen(0, 28, LCD_WIDTH, (LCD_HEIGHT-28));
	display_mark = 0;
	Ih_set_curr_font( IDC_FONT_16 );
    display_bcd(TRUE,CURRENT_HOUR);
	lcd_goto_xy( 42, 30 );
	lcd_puts((U8 *)"时");
    display_mark ++;
    display_bcd(FALSE,CURRENT_MIN);
	lcd_goto_xy( 62, 30 );
	lcd_puts((U8 *)"分");
	display_mark = 0;
}


void display_alarm(void)
{
	   display_menu();
	   Ih_set_curr_font( IDC_FONT_16 );
	   switch( g_setting_list.struct_setting.language_ihm )
	   {
	      case LANG_ENGLISH:
	      lcd_goto_xy( 82, 12 );
		  if( g_setting_list.struct_setting.alarm == 1 ){
	      	lcd_puts(( U8 *)"Open" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"Close" );
		  	}
	      break;
	      
	      case LANG_CHINESE_SIMP:
	      lcd_goto_xy( 40, 12 );
	      if( g_setting_list.struct_setting.alarm == 1 ){
	      	 lcd_puts(( U8 *)"开" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"关" );
		  	}
	      break;

	      case LANG_CHINESE_TRAD:
	      lcd_goto_xy( 40, 12 );
	      if( g_setting_list.struct_setting.alarm == 1 ){
	      	lcd_puts(( U8 *)"秨" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"闽");
		  	}
	      break;
	   }
	   return;
}
//*/
void display_bluetooth(void)
{
      
       Ih_set_curr_font( IDC_FONT_16 );
       switch( g_setting_list.struct_setting.language_ihm )
       {
          case LANG_ENGLISH:
          lcd_goto_xy( 82, 12 );
		  if( setting_bluetooth == 1 ){
          	lcd_puts(( U8 *)"Open" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"Close" );
		  	}
          break;
          
          case LANG_CHINESE_SIMP:
          lcd_goto_xy( 40, 12 );
          if( setting_bluetooth == 1 ){
          	 lcd_puts(( U8 *)"开" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"关" );
		  	}
          break;

          case LANG_CHINESE_TRAD:
          lcd_goto_xy( 40, 12 );
          if( setting_bluetooth == 1 ){
          	lcd_puts(( U8 *)"秨" );
		  	}
		  else{
		  	lcd_puts(( U8 *)"闽");
		  	}
          break;
       }
 return;
}

void display_playmode(void)
{
   display_menu();
   Ih_set_curr_font( IDC_FONT_16 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_goto_xy( 74, 12 );
	  if( g_setting_list.struct_setting.player_repeat == 0 ){
      	lcd_puts(( U8 *)"Singe" );
	  	}
	  else if( g_setting_list.struct_setting.player_repeat == 2 ){
	  	lcd_puts(( U8 *)"Random" );
	  	}
	  else{
	  	 lcd_puts(( U8 *)"Normal" );
	  	}
      break;
      
      case LANG_CHINESE_SIMP:
      lcd_goto_xy( 74, 12 );
      if( g_setting_list.struct_setting.player_repeat == 0 ){
      	 lcd_puts(( U8 *)"单曲" );
	  	}
	  else if( g_setting_list.struct_setting.player_repeat == 2 ){
	  	lcd_puts(( U8 *)"随机" );
	  	}
	  else{
	  	lcd_puts(( U8 *)"顺序" );
	  	}
      break;

      case LANG_CHINESE_TRAD:
      lcd_goto_xy( 74, 12 );
      if( g_setting_list.struct_setting.player_repeat == 0 ){
      	lcd_puts(( U8 *)"虫Ρ" );
	  	}
	  else if( g_setting_list.struct_setting.player_repeat == 2 ){
	  	lcd_puts(( U8 *)"繦诀");
	  	}
	  else{
	  	lcd_puts(( U8 *)"抖");
	  	}
      break;
   }
  return;
}
void mmi_setting_display_volume( void )
{
   _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
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
#if 0
void setting_purcent_bar_init( void )
{
   U8 u8_i;

   setting_last_percent = 0;   

   setting_hbar( BAR_X0, ( BAR_Y0 + 1 ));
   for( u8_i = BAR_X0; u8_i <( BAR_X0 + BAR_WIDTH ); u8_i++ )
   {   
      lcd_set_dot( u8_i, BAR_Y0 );
      lcd_set_dot( u8_i, ( BAR_Y0 + BAR_HEIGHT - 1 ));
   }
   setting_hbar(( BAR_X0 + BAR_WIDTH - 1 ), ( BAR_Y0 + 1 ));
}

void setting_percent_bar( U8 u8_percent )
{
   U8 u8_i;
      
   u8_percent++;

   for( u8_i = setting_last_percent; u8_i <= u8_percent; u8_i++ )
   {   
      setting_hbar( BAR_X0 + u8_i, BAR_Y0 + 1 );
   }
   setting_last_percent = u8_percent;   
}  


//_____ S E C O N D   L E V E L   O F   U P D A T E   R O U T I N E  ( P R I V A T E ) _________________
#if( LCD_PRESENT == ENABLE )
void setting_hbar( const U8 x0, const U8 y0 )
{ 
   U8 u8_i;
   for( u8_i = 0;u8_i <( BAR_HEIGHT - 2 ); u8_i++ )
   {   
      lcd_set_dot( x0, y0 + u8_i );     
   }
}
#endif   //#if( LCD_PRESENT == ENABLE )
#endif
#endif

#endif // _mmi_setting_c_
