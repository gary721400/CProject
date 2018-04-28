//! @file mmi_storage.c,v
//!
//! Copyright (c) 2006
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief Application MMI mass storage
//!
//! @version 2.4 snd3-dvk-1_9_5 $Id:
//!
//! @todo
//! @bug
#ifndef _mmi_storage_c_
#define _mmi_storage_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "conf\conf_lcd.h"

#include <stdio.h>
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\status\mmi_status.h"
#include "mmi\shared\com_var.h"
#include "mmi\kbd\keypad.h"
//#include "mmi\player\mmi_player.h"
#include "mmi_storage.h"

#include "modules\mmi_manager\mmgr_kernel.h"

#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"

#include "conf\conf_lcd.h"
#include "conf\conf_oid.h"
#include "drivers\oid\oid.h"
#include "modules\power\power.h"             // Power module definition

#include "sio\com.h"
//#include "modules\search\search_remote.h"
//#include "lib_mcu\mmc\mmc_drv.h"
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
//#include "modules\file_system\file.h"
#include "mmi\startup\mmi_startup.h"
//#include "mmi\player\srv_player.h"
//#include "modules\file_system\setting.h"
#include "mmi\ap3\mmi_ap3.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "USB_Detect.h"

#include "string.h"
#include "date.h"

#include "point_g.h"


extern unsigned char product[16];
extern unsigned char revision[4];

#pragma DATA = MMI_STORAGE_DATA
#pragma CODE = USB_STORAGE_CODE

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
// Selected appli variable
//static Storage_applis_in_scroller _MEM_TYPE_SLOW_ s_selected_appli = HOME ;
void ready_version(void);
#if 0//( LCD_PRESENT == ENABLE )
#  define NB_POINTS 9
static Point_data code storage_point_data[NB_POINTS] =
{
   {0,8, IDP_MSTORAGE_POINT}
,  {3,6, IDP_MSTORAGE_POINT}
,  {8,4, IDP_MSTORAGE_POINT}
,  {15,2,IDP_MSTORAGE_POINT}
,  {16,6,IDP_MSTORAGE_POINT}
,  {19,6,IDP_MSTORAGE_POINT}
,  {22,4,IDP_MSTORAGE_POINT}
,  {26,2,IDP_MSTORAGE_POINT}
,  {30,0,IDP_MSTORAGE_POINT}
};
#endif  //#if( LCD_PRESENT == ENABLE )
/*
// ID of the timer soft used by this MMI appli
static U8   _MEM_TYPE_SLOW_   s_ts_progress_id  = UNDEFINED_TIMER;
static U8   _MEM_TYPE_SLOW_   s_ts_idle_id      = UNDEFINED_TIMER;
static U8   _MEM_TYPE_SLOW_   s_ts_error_id     = UNDEFINED_TIMER;
static U8   _MEM_TYPE_SLOW_   s_point_idx = 0;
static Bool s_evt_RW = FALSE;
//***********************************************
//add by xuanyu
static  U8    _MEM_TYPE_SLOW_   s_ts_com_id       = UNDEFINED_TIMER;
static  Bool                    s_evt_com         = FALSE;
//***********************************************
static Bool s_evt_suspend = FALSE;
*/
         Bool                             b_msstorage_flag = FALSE;

//extern   Bool                             b_card_ready;
//extern   AL_Variables   xdata             varAl;
//extern   Bool                             gfSysUSBMP3Mode;
extern   Bool                             g_b_usb_on;

//extern   USB_Variables  xdata             USB;
//extern   SDDEVICE_VAR   xdata             SDVar;

//extern   _MEM_TYPE_MEDSLOW_   U32            usb_charge_time;

//extern   Bool                             Usb_connect;

extern   Bool  b_enter_usb;
extern   Bool  b_usb_detection;

int usb_core_status;

Bool  usb_key;

#if( LCD_PRESENT == ENABLE )
const char  *s_usb_connect_name =
{
  //English
  "USB Connect"
};
const char *s_usb_charger_name =
{
  //English
  "USB Charger"
};
#endif   //#if( LCD_PRESENT == ENABLE )

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
#if( LCD_PRESENT == ENABLE )
static  void storage_display( U8,U8 );
#else
#define storage_display( entire_draw, RWFlag )
#endif  //#if( LCD_PRESENT == ENABLE )
//_____ D E F I N I T I O N S ______________________________________________
//!
//! @brief MMIAppli Mass storage
//!
//! Application MMI use to perform mass storage
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void storage_mmi_appli( U8 event, U16 param )
{
   switch( event )
   {
      case EVT_START_APPLI:
      // Request to know if USB cable connected to the device
      SendString((U8 *)"storage\r\n");
      power_forbid_autopoweroff();
      //Lcd_backlight_enable();
      //lcd_draw_picture(120, 0, IDP_STATUS_BATT_CHARGE);
      Lcd_rearm_backlight_on();
      Lcd_backlight_disable();
      {
         SPIF_ERR SPI_STATE;

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
         }
      }
	  ready_version();
      {
      U32 timer;
      timer = Timer_set_timer( TIMER_MS( 300 ));
      while( !Timer_is_timeout( timer )); //delay 17ms
      }
      mail_send_command( CMD_GET_VBUS_STATE, 0 );
      scan_oid_disable();
      //power_stop_batt_charging();
#if 0//( WIRELESS == ENABLE )
      g_b_wireless_flag = FALSE;
#endif   //#if( WIRELESS == ENABLE )
#if( OID == ENABLE )
      if( b_oid_init_flag )
      {
         turn_off_oid();
         pen_disable();
         oid_power_off();
         oid_dc_dc_off();
      }
      b_oid_init_flag = FALSE;
	  g_b_booksys_flag = FALSE;
      bPenLeave = TRUE;
      //u32_oid_read_timeout = 0;
#endif   //#if( OID == ENABLE )
      //g_b_storage_bulid_on = FALSE;
      b_msstorage_flag = TRUE;

      Kbd_set_evt_enabled();
      if( Kbd_is_event_triggered())
         Kbd_set_evt_disabled();
      break;

      case EVT_VBUS_STATE:
      // USB cable is not connected
      if( 0 == LSB(param) )
      {
exit_mmi_storage:
         /*g_select_appli = g_old_select_appli;
         //g_select_appli = DEFAULT_APPLI;
         mail_send_command( CMD_BATTERY_CHARGE_DISABLE, 0 );
         usb_charge_time = Timer_set_timer( TIMER_S( 2 ));
         Mmgr_kill_this_mmi_appli();*/

         {
         U16 param;

         KEY_ID( param ) =  KEY_POWER;
         KEY_STATUS( param ) = KBD_KEY_LONGPRESSED;
         SendString((U8 *)"not usb attached\r\n");
         //g_b_usb_on = FALSE;
         mail_send_event( EVT_KEY_PRESSED, param );
         }
      }
      // USB cable is connected
      else
      {
         oid_dc_dc_off();
         
         mmgr_set_appli_full_screen();
         // Disable the charge of the battery during mass-storage
         mail_send_command( CMD_BATTERY_CHARGE_DISABLE, 0 );
         usb_charge_time = Timer_set_timer( TIMER_S( 5 ));

         storage_display( TRUE, 0 );
         
         //Usb_connect = TRUE;
         //if( !ChangeToUSBMode())
         b_enter_usb = FALSE;
         if( !b_usb_first_check )
         {
            b_usb_first_check = TRUE;
            b_enter_usb = TRUE;
            USBStoragePoweroff();
         }
         else
         {
            if( Usb_connect )
               b_enter_usb = TRUE;
         }

         if( !b_enter_usb )
         {
            /*SendString((U8 *)"not usb masstorage\r\n");
            Usb_connect = FALSE;*/
            //goto exit_mmi_storage;
            mail_send_event( EVT_USB_DEVICE_CHARGE, 0 );
            //g_b_usb_on = FALSE;
            break;
         }
         
         // Before start mass storage, flush cache of file system and reset this one
         //fat_cache_flush();
         //nav_reset();
         // Send to the operating task the command to start the mass transfert
         mail_send_command(CMD_USB_DEVICE_START, 0);
         //storage_display( TRUE, 0 );
/*
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
         ap3_poweron_state = MMI_AP3_USB_WRITE;
*/
      }
      break;

      case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      storage_display( TRUE, 0 );
      break;
#if 0
      case EVT_USB_DEVICE_SEND :
      case EVT_USB_DEVICE_RECEIV :
      if( !g_b_storage_bulid_on )
      {
         g_b_storage_bulid_on = TRUE;
         //mail_send_command(CMD_BATTERY_CHARGE_DISABLE, 0);
      }
      //red_led_flash();
      //ts_set_time( s_ts_idle_id, STORAGE_IDLE_TIMER );
      if( FALSE == s_evt_RW )
      {
         s_evt_RW = TRUE;
         s_point_idx++;
         storage_display( FALSE,1 );
         //ts_set_time( s_ts_progress_id, STORAGE_ARROW_TIMER );
         g_b_storage_bulid_on = TRUE;
      }
//***********************************************
      //add by xuanyu
      /*if( s_evt_com )
      {
         s_evt_com = FALSE;
         ts_stop( s_ts_com_id );
         Ts_free( s_ts_com_id );
      }*/
//***********************************************
      break;

      case EVT_USB_DEVICE_CDC_RECEIV:
      break;
      
      case EVT_TIMER:
      if( LSB(param) == s_ts_idle_id )
      {
         if( s_evt_suspend )
         {
            mail_send_command(CMD_USB_DEVICE_STOP, 0);
            mail_send_command(CMD_BATTERY_CHARGE_ENABLE, 0);
            Ts_free( s_ts_idle_id );
            Ts_free( s_ts_progress_id );
//***********************************************
            //add by xuanyu
            g_b_storage_bulid_on = FALSE;
//***********************************************
            g_select_appli = g_old_select_appli;
            b_usb_powered = FALSE;
            Mmgr_kill_this_mmi_appli();
            break;
         }
         storage_display( FALSE,0 );
         s_evt_RW = FALSE;
         ts_stop( s_ts_idle_id );
         ts_stop( s_ts_progress_id );
//***********************************************
         //add by xuanyu
         mail_send_command( CMD_BATTERY_CHARGE_ENABLE, 0 );
         g_b_storage_bulid_on = FALSE;
//***********************************************
      }
      //else test idle timer
      else if( LSB(param) == s_ts_progress_id )
      {
         s_evt_RW = FALSE;
         ts_stop( s_ts_progress_id );
      }
      else if ( LSB(param) == s_ts_error_id )
      {
         ts_stop( s_ts_error_id );
         Ts_free( s_ts_error_id );
//***********************************************
         //add by xuanyu
         g_b_storage_bulid_on = FALSE;
//***********************************************
         g_select_appli = g_old_select_appli;
         b_usb_powered = FALSE;
         Mmgr_kill_this_mmi_appli();
      }
      else
      { Mmgr_krn_forward_current_event(); }
      break;
#endif

      case EVT_KEY_PRESSED:
	  	///*
	  SendString((U8 *)"\r\nusb_key=");
	  send_hex_u16(param);
	 // break;
	  //*/
      // Check which pen status we have:pen down, pen up or long press
      if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ))
         {
         	case KEY_MODE:
           // case KEY_POWER: // Same action whether simple release or release after repeat(s).
            if( b_usb_detection )
               break;
			if(!g_b_V1Hard_Flag)goto new_mode;
            //if( !usb_key_is_enable())
            //   break;
new_power:            
            if( Usb_connect )
               mail_send_command( CMD_USB_DEVICE_STOP, 0 );
            g_b_usb_on = FALSE;
            usb_charge_time = Timer_set_timer( TIMER_S( 2 ));
            g_select_appli = g_old_select_appli;
            //g_select_appli = DEFAULT_APPLI;
            Mmgr_kill_this_mmi_appli();
            break;

			case KEY_REC:
           // case KEY_POWER: // Same action whether simple release or release after repeat(s).
            if( b_usb_detection )
               break;
            //if( !usb_key_is_enable())
            //   break;
            if( Usb_connect )
               mail_send_command( CMD_USB_DEVICE_STOP, 0 );
            g_b_usb_on = FALSE;
            usb_charge_time = Timer_set_timer( TIMER_S( 2 ));
            //g_select_appli = g_old_select_appli = C_USB_HID;
             g_old_select_appli = C_CAB; 
             g_select_appli = g_old_select_appli;
            //g_select_appli = DEFAULT_APPLI;
            Mmgr_kill_this_mmi_appli();
            break;

			//case KEY_MODE:
            case KEY_POWER: // Same action whether simple release or release after repeat(s).
            if( b_usb_detection )
               break;
            //if( !usb_key_is_enable())
            //   break;
            if(!g_b_V1Hard_Flag)goto new_power;
new_mode:            
            if( Usb_connect )
               mail_send_command( CMD_USB_DEVICE_STOP, 0 );
            g_b_usb_on = FALSE;
            usb_charge_time = Timer_set_timer( TIMER_S( 2 ));
           
#if( T01OID_CAB == ENABLE)
			g_old_select_appli = C_CAB;
#endif
			g_select_appli = g_old_select_appli;

            //g_select_appli = DEFAULT_APPLI;
            Mmgr_kill_this_mmi_appli();
            break;
         }
      }
      break;

      /*case EVT_USB_DEVICE_SUSPEND :
      //nf_sync();  // in case of detach USB power (=POWER DOWN), flush write sequence to accelerate the NF rebuild in restart time
      break;

      case EVT_USB_CTRL_POWERED :
      break;

      case EVT_USB_CTRL_UNPOWERED :
      case EVT_USB_DEVICE_STOP :
      SendString("USB UnPower STOP\r\n");
      //mail_send_command( CMD_USB_DEVICE_STOP, 0 );
      g_select_appli = g_old_select_appli;
      //b_usb_powered = FALSE;
      Mmgr_kill_this_mmi_appli();
      break;*/

      /*case EVT_USB_DEVICE_WAKE_UP :
      // USB waked up after the suspend
      //s_evt_suspend = FALSE;
      // Restart our animation
      //ts_set_time( s_ts_progress_id, STORAGE_ARROW_TIMER );
      //ts_stop( s_ts_idle_id );
      break;*/

      /*case EVT_USB_DEVICE_RESUME :
      break;

      case EVT_USB_DEVICE_ENUM_HIGH :
      // USB in hight speed mode
      break;

      case EVT_USB_DEVICE_ENUM_FULL :
      // USB in full speed
      break;*/

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      IE = 0;
      aDEVCSR[ 0 ] |= 0x40;   //UTMI PHY reset
      aUSBR = 0x0;			//USB Cable Attachment detection Disable*/
      /*CLASS_D_POP_CTL = 0;
      CLASS_D_PWM_CTL = 0;
      P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
      (*(void(*)())0)();
      break;*/

      case EVT_USB_DEVICE_RECEIV:
      //case EVT_EXP_MEMORY_MMCSD_CHANGE:
      ap3_poweron_state = MMI_AP3_USB_WRITE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      last_record_number = 1;

      b_clusterlist = FALSE;
      break;

      case EVT_USB_DEVICE_CHARGE:
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
	  //lcd_goto_xy( 16, 0 );
      //Ih_set_curr_font( IDC_FONT_08);
      //lcd_puts((U8 *)"usb" );
	  
      lcd_goto_xy( 20, 12 );
      Ih_set_curr_font( IDC_FONT_16);
      lcd_puts(( U8 *)s_usb_charger_name );
      break;

      default:
      Mmgr_krn_forward_current_event();
      break;
   }
}


void ready_version(void)
{
   //product[0] = HARD_VERSION;USB_DEVICE_MS_PRODUCT         "OID4 V2.0.4.2B  "
   //const unsigned char product[16]={' ',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE_VERSION,'-',H_YEAR,L_YEAR,H_MONTH,L_MONTH,H_DAY,L_DAY,' ',' ',' ',' '};
   memset(product,0,sizeof(16));
   strcpy((char *)product,(char *)HARD_VERSION);
   strcat((char *)product,(char *)MAJOR_FW_VERSION);
   strcat((char *)product,(char *)MINOR_FW_VERSION);
   strcat((char *)product,(char *)OID_TYPE_VERSION);
   strcat((char *)product,(char *)" ");
   //strcat((char *)product,(char *)NOW_DATA);
   strcat((char *)product,(char *)__DATE__);
 //  strcat((char *)product,(char *)" ");
   SendString((U8 *)"\r\nproduct=");
   SendString((U8 *)product);

   memset(revision,0,sizeof(4));
   strcpy((char *)revision,(char *)HARD_VERSION);
   strcat((char *)revision,(char *)MAJOR_FW_VERSION);
   strcat((char *)revision,(char *)MINOR_FW_VERSION);
   strcat((char *)revision,(char *)OID_TYPE_VERSION);
   
}

#if( LCD_PRESENT == ENABLE )
//! @brief Display the storage screen
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 entire_draw: Draw the screen entirely
//!
//! @return none
static void storage_display( U8 entire_draw , U8 RWFlag)
{
   RWFlag = RWFlag;

   if( TRUE == entire_draw )
   {
      lcd_turn_on();
      Lcd_start_backlight_timer();
      
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//      lcd_clear_screen();
	  lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
	  lcd_clear_part_of_screen(20, 0,98, 8);
      Lcd_set_merging();
//      lcd_goto_xy(28, 0);
 //     Ih_set_curr_font( IDC_FONT_08 );
//      lcd_puts(( U8 *)WP_NAME );

	  //lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
	   //lcd_clear_part_of_screen( 64, 0, 54, 8);
	  //lcd_clear_part_of_screen(20, 0,64, 8);
	  lcd_goto_xy( 20, 0 );
      Ih_set_curr_font( IDC_FONT_08);
      lcd_puts((U8 *)"USB" );

      lcd_goto_xy( 20, 12 );
      Ih_set_curr_font( IDC_FONT_16 );
      lcd_puts(( U8 *)s_usb_connect_name );
      status_display();
   }
}
#endif  //#if( LCD_PRESENT == ENABLE )


#endif // _mmi_storage_c_
