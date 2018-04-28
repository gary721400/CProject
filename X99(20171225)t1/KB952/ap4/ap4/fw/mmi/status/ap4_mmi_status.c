//!
//! @file mmi_status.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application status
//!
//! Is the base application of the MMI.
//! Must be lauched first by the kernel.
//!
//! @version 2.5 snd3-dvk-1_9_5 $Id: mmi_status.c,v 2.5 2007/06/25 11:03:06 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_status_c_
#define _mmi_status_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"

#include "conf\conf_mmi.h"

#include <stdio.h>
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\auto_disp\auto_disp.h"

#include "modules\power\power.h"
#include "modules\mmi_manager\mmgr_kernel.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\kbd\keypad.h"
#include "mmi\status\mmi_status.h"
#include "sio\com.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include <string.h>
#include "modules\file_system\navigation.h"
#include "modules\player\player.h"
#include "modules\player\player_task.h"
#include "mmi\player\srv_player.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

#pragma DATA = MMI_STATUS_DATA
#pragma CODE = MMI_STATUS_CODE

ImageDeclareType( AUTO_DISP_TEMP_CODE00 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( TASK_INIT_CODE );

ImageDeclareType( AUADC_COMMON_CODE );

ImageDeclareType( MMI_STATUS_TEMP_CODE00 );
ImageDeclareType( MMI_STATUS_TEMP_CODE01 );

ImageDeclareType( CHANGE_APPLI_CODE );
//ImageDeclareType( LINK_NAME_CODE );
//_MEM_TYPE_SLOW_   U8    g_startup_id_appli = MMI_APPLI_NOT_DEFINED;

static Bool b_inhib_refresh_status;

// Decharging battery level
//static _MEM_TYPE_SLOW_ U8  s_battery_level;
// Battery in charging state or not
//static                 Bool  b_battery_charging = FALSE;
// Variable to store the id of the battery animation
static _MEM_TYPE_SLOW_ U8  s_id_batt_anim;


#pragma CODE = AUTO_DISP_TEMP_CODE00

// Parameter for the battery animation
static _MEM_TYPE_SLOW_ Ad_prm_animation s_prm_batt_anim = {115,0,IDP_BATT_CHARG,BATTERY_ANIM_TIME,AD_FOREVER|AD_DISP_OVER};

// USB host
//static Bool b_usb_host_mode   = FALSE;
//_CONST_TYPE_  U8 MMISTATUS_MSG_ERROR_USB_LIMITATION_HW[]  = "USB Limit HW";
//_CONST_TYPE_  U8 MMISTATUS_MSG_ERROR_USB_LIMITATION_SW[]  = "USB Limit SW";

#pragma CODE = MMI_STATUS_CODE

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
#if( LCD_PRESENT == ENABLE )
void status_display_battery( void );
//static void status_display_lock( void );
//static void status_display_host( void );
#define status_display_lock()
#define status_display_host()
#else
#define status_display_battery()
#define status_display_lock()
#define status_display_host()
#endif  //#if( LCD_PRESENT == ENABLE )

void  system_error_deal       ( void );
void  evt_update_start_deal   ( void );
void  change_appli			  (void);
//void link_filename( char *str );
//_____ D E F I N I T I O N S ______________________________________________
//!
//! status_mmi_appli
//!
//! Manage the application status
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void status_mmi_appli(U8 event, U16 param)
{
   switch( event )
   {
      case EVT_START_APPLI:
         //s_battery_level = 0;    // To make it possible a charging battery status display
         break;
#if 0
      case EVT_APPLI_KILLED:
         if( MSB(param) == g_startup_id_appli )
         {
            Mmgr_set_id_appli_not_defined( g_startup_id_appli );
            //mail_send_command(CMD_POWER_SETTING_LOADED, 0);    // Setting has just been loaded, so inform power task
            if( power_is_vbus_present())
            {
               mail_send_command( CMD_USB_START, 0 );
               /*g_select_appli = MSTORAGE_APPLI;
               mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );*/
               SendString((U8 *)"CMD_USB_START\r\n");
            }
            //s_id_batt_anim = ad_allocate(AD_ID_ANIMATION,(Ad_p_prm _MEM_TYPE_SLOW_ *)&s_prm_batt_anim);
            //status_authorize_display();
            //status_display();
         }
         break;
#endif
      case EVT_BACK_TO_TOP:
         // Only MMI_STATUS in the stack, so launching MMI_IDLE again
         //mmgr_activate_mmi_appli( MMI_APPLI_IDLE, 0 );
         //g_select_appli = IDLE_APPLI;
         SDKLoadCode(CHANGE_APPLI_CODE);
		 change_appli();
         break;

      case EVT_UPDATE_START:
         //if (LSB(param) == UPDATE_STOPPED)
         {  // The default application selected after power on
            SDKLoadCode( MMI_STATUS_TEMP_CODE01 );
            evt_update_start_deal();
            /*SDKLoadCode( TASK_INIT_CODE );
            ( void )mmgr_activate_mmi_appli(MMI_APPLI_AP3, 0);
            SDKLoadCode( AUTO_DISP_TEMP_CODE00 );
            s_id_batt_anim = ad_allocate(AD_ID_ANIMATION,(Ad_p_prm _MEM_TYPE_SLOW_ *)&s_prm_batt_anim);
            status_authorize_display();
            status_display();*/
         }
         break;

      case EVT_USB_CTRL_POWERED:
         //PWR_OFF();
         //EXT_DC_PWR_OFF();
         /*if( Mmgr_is_this_appli_at_the_top() || (MMI_APPLI_IDLE==mmgr_get_module_id_appli(1)) )
         {
#if (USB_DEVICE_AUTO_LAUNCH==ENABLE)
            //g_select_appli = MSTORAGE_APPLI;
            //SendString((U8 *)"mstorage\r\n");
            mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
            CommonLoadCode( C_USB );
#endif
         }
         else*/
         {
            //if( power_is_vbus_present())
            {
               mail_send_event( EVT_USB_POWERED, 0 );
            }
            /*else
            {
               //power_down();
               power_system_off( TRUE );
            }*/
         }
         break;

//***********************************************
      case EVT_USB_CTRL_UNPOWERED:
         //if( g_select_appli != MSTORAGE_APPLI )
         {
            //power_down();
            //power_system_off( TRUE );
            mail_send_command(CMD_POWER_OFF, 0);
         }
         break;
//***********************************************

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
         power_system_off( !power_is_vbus_present());
         break;*/
#if 0
      case EVT_USB_DEVICE_START:
         break;

      case EVT_USB_HOST_START:
         b_usb_host_mode   = TRUE;
         status_display_host();
         break;

      case EVT_USB_HOST_EXIT:
         b_usb_host_mode   = FALSE;
         status_display_host();
         break;
         
      case EVT_USB_HOST_MS_CHANGE:
      case EVT_USB_HOST_HUB_CHANGE:
      case EVT_USB_HOST_HID_CHANGE:
      case EVT_USB_HOST_CDC_CHANGE:
         status_display_host();
         break;

      case EVT_USB_HOST_LIMITATION_HW:
         info_put_text( MMISTATUS_MSG_ERROR_USB_LIMITATION_HW );
         mmgr_activate_mmi_appli(MMI_APPLI_INFO,INFO_ERROR);
         break;

      case EVT_USB_HOST_LIMITATION_SW:
         info_put_text( MMISTATUS_MSG_ERROR_USB_LIMITATION_SW );
         mmgr_activate_mmi_appli(MMI_APPLI_INFO,INFO_ERROR);
         break;

      case EVT_SWITCH:
         status_display_lock();
         break;
#endif
      /*case EVT_POWER_VBUS_STATE:
         // All power events as EVT_POWER_VBUS_STATE are routed directly to MMI_STATUS
         // So, another event must be sent so that another mmi appli gets this info
         mail_send_event( EVT_VBUS_STATE, LSB(param));
         break;*/

      case EVT_BATTERY_CHARGING:
         b_battery_charging = LSB(param);
         //status_display_battery();
         goto status_bat_display;
         //break;

      case EVT_BATTERY_LEVEL:
         if( power_is_vbus_present())
            s_battery_level = LSB(param);
         else
         {
            if( !s_battery_level )
               s_battery_level = LSB(param);
            else
            {
               if( s_battery_level >= LSB( param ))
                  s_battery_level = LSB(param);
            }
         }
         //s_battery_level = LSB(param);
status_bat_display:
         status_display_battery();
         break;

      case EVT_BATTERY_LEVEL_ALERT:
         mail_send_event(EVT_ALERT_BATTERY_LOW, LSB(param));
         break;

      case EVT_ALERT_BATTERY_LOW:
         // Not caught by MMI above because no specific action to do, so the power-off starts here
         //mail_send_command(CMD_POWER_OFF, 0);
         break;

      case EVT_POWER_ENTER_IN_POWEROFF:
         // All power events like EVT_POWER_ENTER_IN_POWEROFF are routed directly to MMI_STATUS
         // So, another event must be sent so that another mmi appli gets this info
         mail_send_event(EVT_ENTER_IN_POWEROFF, 0);
         break;

      case EVT_ENTER_IN_POWEROFF:
         // EVT_ENTER_IN_POWEROFF not catched by mmi applications above for a specific power off
         mail_send_command(CMD_POWER_OFF, 0);
         break;
#if 0
      case EVT_POWER_ON:
         // All power events like EVT_POWER_ON are routed directly to MMI_STATUS
         // So, another event must be sent so that another mmi appli gets this info
         mail_send_event(EVT_EXIT_POWEROFF, 0);
         break;
#endif
      case EVT_MAILBOX_CMD_FULL:
#if 0
         // Critical system error: one of the mailboxes becomes full. Thus, some
         // information are lost.
         lcd_clear_screen();
         lcd_goto_xy(16,  8); lcd_puts("System error");
         lcd_goto_xy(16, 20); lcd_puts("Command mailbox");
         lcd_goto_xy(16, 32); lcd_puts("is full.");
         while(1);
#endif
      case EVT_MAILBOX_EVT_FULL:
#if 0
         // Critical system error: one of the mailboxes becomes full. Thus, some
         // information are lost.
         lcd_clear_screen();
         lcd_goto_xy(16,  8); lcd_puts("System error");
         lcd_goto_xy(16, 20); lcd_puts("Event mailbox");
         lcd_goto_xy(16, 32); lcd_puts("is full.");
         while(1);
#endif
  		case EVT_TIMER_ALLOC_FAILURE:
#if 0
     		// Critical system error: failure in the allocation of a timer (see "timersoft.c/h")
     		// Too much timers require to run at the same time.
         // This case can appear when you forget to let free timers when you kill applications
         lcd_clear_screen();
         lcd_goto_xy(16,  8); lcd_puts("System error");
         lcd_goto_xy(16, 20); lcd_puts("No more");
         lcd_goto_xy(16, 32); lcd_puts("timer available.");
#endif
         SDKLoadCode( MMI_STATUS_TEMP_CODE00 );
         system_error_deal();
         /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
         lcd_clear_screen();
         Ih_set_curr_font( IDC_FONT_16 );
         lcd_goto_xy(16, 12); lcd_puts(( U8 *)"System error");
         while(1);*/

   }
}


#pragma CODE = MMI_STATUS_TEMP_CODE01

//! status_display
//!
//! Display the screen of the application status
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_display( void )
{
   // display icons of devices
   status_display_battery();
   status_display_lock();
   status_display_host();
}


#pragma CODE = MMI_STATUS_CODE

//! status_display_battery
//!
//! Display the icon battery
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_display_battery( void )
{
   U32 _MEM_TYPE_SLOW_ IDP;

   if( (!mmgr_is_top_appli_full_screen())||(b_inhib_refresh_status) )
   { return; }
   else
   {
      if( b_battery_charging )    
      {
 //     ad_start(s_id_batt_anim);
 		lcd_draw_picture(120, 0, IDP_STATUS_BATT_CHARGE);
	  }
      else 
      {
//         ad_stop(s_id_batt_anim);
         // Specific display for the battery according its level
         // Vmes = Vbat x 0.375 (3V-4.2V LiIon battery)
         if      (s_battery_level >= 4) IDP = IDP_STATUS_BATT_FULL;  // > ~3.8V
         else if (s_battery_level >= 3) IDP = IDP_STATUS_BATT_MED;   // > ~3.4V
         else if (s_battery_level >= 2) IDP = IDP_STATUS_BATT_LOW;   // > ~3.2V
         else if (s_battery_level >= 1) IDP = IDP_STATUS_BATT_V_LOW;
         else                           return;
         if( g_b_battery_low )
         {
           /*
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 109, 0 );
            lcd_puts(( U8 *)"!" );
            */
            s_battery_level = 1;
            IDP = IDP_STATUS_BATT_V_LOW;
         }
         //lcd_draw_picture(115, 0, IDP);
          lcd_draw_picture(120, 0, IDP);

         /*if( g_b_battery_low )
         {
            Ih_set_curr_font( IDC_FONT_08 );
            lcd_goto_xy( 109, 0 );
            lcd_puts(( U8 *)"!" );
         }*/
      }
   }
}

#if 0
//! status_display_lock
//!
//! Display the icon keyboard lock
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_display_lock()
{
   if( (!mmgr_is_top_appli_full_screen())
   ||  (b_inhib_refresh_status          ) )
      { return; }

   //Display or not the keyboard lock
//   if( TRUE == g_keypad_locked )  lcd_draw_picture(90,54,IDP_STATUS_LOCK);
//   else                           lcd_clear_part_of_screen(90,54,7,8);
}


//! status_display_lock
//!
//! Display the icon keyboard lock
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_display_host()
{
   U32 _MEM_TYPE_SLOW_ idp_host;

   if( (!mmgr_is_top_appli_full_screen())
   ||  (b_inhib_refresh_status          ) )
      { return; }

   //Display or not the HOST state of the USB
   if( TRUE == b_usb_host_mode )
   {
#if (USB_HOST_HUB_SUPPORT == ENABLE)
      if( 0 != g_host_hub_nb )
         idp_host = IDP_HOST_HUB;
      else 
#endif
         idp_host = IDP_HOST_CON;
#if (USB_HOST_MS_SUPPORT == ENABLE)
      if( 0 != usb_host_ms_getlun() )
         idp_host = IDP_HOST_MS;
#endif
#if (USB_HOST_HID_SUPPORT == ENABLE)
      if( 0 != g_host_hid_nb )
         idp_host = IDP_HOST_HID;
#endif
#if (USB_HOST_CDC_SUPPORT == ENABLE)
      if( 0 != usb_host_cdc_getcom() )
         idp_host = IDP_HOST_CDC;
#endif
   }
   else
   {
      return;  // Don't display icon "USB host no connected"
//      idp_host = IDP_HOST_DIS;
   }
   lcd_draw_picture(84-11, 2, idp_host);
}

//! status_inhib_display()
//!
//! Inhibit the display of the status icons
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_inhib_display( void )
{
   ad_stop(s_id_batt_anim);
   b_inhib_refresh_status = TRUE;
}
#endif


#pragma CODE = MMI_STATUS_TEMP_CODE01

//! status_authorize_display()
//!
//! Authorize the display refresh of the status icons
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
void status_authorize_display( void )
{
   b_inhib_refresh_status = FALSE;
}


void evt_update_start_deal( void )
{
   SDKLoadCode( TASK_INIT_CODE );
   ( void )mmgr_activate_mmi_appli(MMI_APPLI_AP3, 0);
   SDKLoadCode( AUTO_DISP_TEMP_CODE00 );
//   s_id_batt_anim = ad_allocate(AD_ID_ANIMATION,(Ad_p_prm _MEM_TYPE_SLOW_ *)&s_prm_batt_anim);
   status_authorize_display();
   status_display();
}


#pragma CODE = MMI_STATUS_TEMP_CODE00

void system_error_deal( void )
{
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   //lcd_clear_screen();
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_goto_xy(16, 12); lcd_puts(( U8 *)"System error");
   while(1);
}


#if 1
char  _MEM_TYPE_SLOW_   tisiFileName[ 512 ];
char  _MEM_TYPE_SLOW_   tisiFileShortpath[ 128 ];

char  _MEM_TYPE_SLOW_   subdirFileName[ 32 ];
char  _MEM_TYPE_SLOW_   subdirFileShortpath[ 32 ];

#pragma CODE = MMI_PLAYINTRO_CODE00

//#define  find_path_a( a, b )     find_path_u( atou( a ), b )
/*
void strcpy_u( U16 *dst, U16 *src )
{
   while( *src )
   {
      *dst++ = *src++;
   }
   *dst = 0;
}


void strcat_u( U16 *dst, U16 *src )
{
   while( *dst )
      ++dst;
   while( *src )
   {
      *dst++ = *src++;
   }
   *dst = 0;
}
*/


#pragma CODE = MMI_STATUS_CODE

U16 *atou( U8 *astr )
{
   U16 *ustr;

   ustr = ( U16 *)g_string2;
   while( *astr )
   {
      *ustr++ = *astr++;
   }
   *ustr = 0;
   return ( U16 *)g_string2;
}


#pragma CODE = MMI_PLAYINTRO_CODE00

Bool find_entry( U8 * spec, U16 *name, Bool is_dir, FILE_INFO __X *finfo, FS_ENTRY_INFO __X *entry )
{
   U16 buffer[ 25 ];
   
   //if( FSGetEntry(( char *)spec, 0, finfo, entry ) != FS_NO_ERROR )
   //   return FALSE;

   //change 2013-05-22 zoe
   unsigned int i;
   FS_ERR fserr;
   for( i = 0; ; ++i )
   {
      fserr = FSGetEntry(( char *)spec, i, finfo, entry );
      if( fserr == FS_NO_ERROR )
         break;
      if( fserr == FS_FILE_NOT_FOUND )
         continue;
      return FALSE;
   }

   do
   {
      if(( finfo->attribute & FS_ATTR_LFN_ENTRY )!= FS_ATTR_LFN_ENTRY )
      {
         local_to_unicode( finfo->name, buffer, sizeof( buffer ));
         if( str_comp_u(( U16 *)finfo->lfn, name, FALSE )== 0
            || str_comp_u( buffer, name, FALSE )== 0 )
         {
            if( is_dir )
            {
               if( finfo->attribute & FS_ATTR_DIRECTORY )
                  return TRUE;
            }
            else
            {
               if( !( finfo->attribute & FS_ATTR_DIRECTORY ))
                  return TRUE;
            }
         }
      }

      //change 2013-05-22 zoe
      fserr = FSGetNextEntry( finfo, entry );
      if(( fserr != FS_NO_ERROR )&&( fserr != FS_FILE_NOT_FOUND ))
         break;
   }
   //while( FSGetNextEntry( finfo, entry )== FS_NO_ERROR );
   while( 1 );
   
   return FALSE;
}


Bool find_path_u( U16 *path_str, U8 *short_path )
{
   U16	left, right;
   U16	iter;
   U16	old_char;
   FILE_INFO __X info;
   FS_ENTRY_INFO __X entry;
   left = 3, right = 3;

   strcpy(( char *)short_path, "0:" );
   memset( &info, 0, sizeof( info ));
   entry.drive = 0;
   FSGetEntry(( char *)short_path, 0, &info,&entry );
   if( path_str[ left ] == '\0' )
      return TRUE;
   while( path_str[ left ] != '\0' )
   {
      while( path_str[ right ] != '\\'
         && path_str[ right ] != '/'
            &&path_str[ right ] != '\0' )
         ++right;
      if( path_str[ right ] == '\\'
         || path_str[ right ] == '/' )
      {
         old_char = path_str[ right ];
         path_str[ right ] = 0;
         if( find_entry( short_path, ( U16 *)&path_str[ left ], TRUE, &info, &entry ))
         {
            //if( nav_filelist_findname(( FS_STRING )( &path_str[ left ]), FALSE ) && nav_file_isdir())
            strcat(( char *)short_path, ( char *)"\\" );
            strcat(( char *)short_path, ( char *)info.name );
            if( FSGetEntry(( char *)short_path, 0, &info, &entry )!= FS_NO_ERROR )
            {
               path_str[ right ] = old_char;
               goto __bad_end;
            }
         }
         else
         {
            path_str[ right ] = old_char;
            goto __bad_end;
         }
         path_str[ right ] = old_char;
         ++right;
         left = right;
      }
      else
      {
         if( find_entry( short_path, ( U16 *)&path_str[ left ], FALSE, &info, &entry ))
         {
            strcat(( char *)short_path, "\\" );
            strcat(( char *)short_path, ( char *)info.name );
            goto __end;
         }
         else
            goto __bad_end;
      }
   }
__end:
   return TRUE;
__bad_end:
   return FALSE;
}


Bool search_subdir_mp3_file( char *str )
{
   /*
   //strcpy( subdirFileName, SUBDIR_PATH );
   strcpy( subdirFileName, SUBDIR_PATH );
   strcat( subdirFileName, str );
   if( !find_path_a(( U8 *)subdirFileName, (U8 *)subdirFileShortpath ))
      return FALSE;

   return TRUE;
   */
   Bool flag;
#if 1//( UNICODE_TYPE == DISABLE )
   strcpy( tisiFileName, (char *)&system_dir);
   strcat( tisiFileName, str );
  // SendString((U8 *)tisiFileName);
   if( !find_path_a(( U8 *)tisiFileName, (U8 *)subdirFileShortpath ))
      return FALSE;

   return TRUE;
#endif   //#if( UNICODE_FORMAT == DISABLE )

#if 0//( UNICODE_TYPE == ENABLE )
   SDKLoadCode(LINK_NAME_CODE);
   link_filename(str);

   flag = find_path_u(( U16 *)tisiFileName, ( U8 *)tisiFileShortpath );
   SendString((U8 *)"\r\ntishi=");
   SendString((U8 *)tisiFileShortpath);
   return flag;
#endif   //#if( UNICODE_FORMAT == ENABLE )   
}

/*
#pragma CODE = LINK_NAME_CODE
void link_filename( char *str )
{
   U16 *p;
   U16 SYS_DIR_NAME[] = {0x7CFB,0x7EDF,0x6587,0x4EF6,0x0000};
   //nav_string_unicode();
   p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\"), 3+1 );
   p = str_copy_u_n( p, ( U16 *)SYS_DIR_NAME, 4+1 );
   p = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
   p = str_copy_u_n( p, atou(( U8 *)str ), strlen( str )+1 );

   
}
*/
#pragma CODE = CHANGE_APPLI_CODE
void change_appli(void)
{
  SDKLoadCode( AUADC_COMMON_CODE );
	 AUADC_Power_Down();
     SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
     lcd_clear_part_of_screen( 64, 0, 12, 8 );
	 if(g_select_appli == POWER_OFF_APPLI){
	 	CommonLoadCode( C_POWEROFF);
	 	}
	  if(g_select_appli == PLAYER_APPLI){
	 	CommonLoadCode( C_PLAYER);
	 	}
	  if(g_select_appli == RECORDER_APPLI){
	 	CommonLoadCode( C_RECORD);
	 	}
	  if(g_select_appli == REREAD_APPLI){
	 	CommonLoadCode( C_REREAD);
	 	}
	  if(g_select_appli == DIC_APPLI){
	 	CommonLoadCode( C_DIC );
	 	}
	 	CommonLoadCode( C_IDLE );
}

#pragma CODE = MMI_PLAYINTRO_CODE01

Bool play_sbdir_mp3_file( void )
{
   FS_ERR fs_err;
   extern Bool g_b_play_ap3;

   if( FSGetFileSize(( char *)subdirFileShortpath, &fs_err )== 0 )
      return FALSE;
   
   
   audio_is_wav = 0;
   count_AB = 0;
   //wav_encrypt = FALSE;
   //srvplayer_volume_send();
   g_b_play_ap3 = FALSE;
   if( !player_play_file( subdirFileShortpath ))
   {
      return FALSE;
   }
   /*power_forbid_autopoweroff();

   srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
   player_task_start();       // Enable event of player task*/
   return TRUE;
}
#endif

#endif // _mmi_status_c_

