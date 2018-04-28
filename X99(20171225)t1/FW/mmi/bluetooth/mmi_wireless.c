//!
//! @file mmi_wireless.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application IDLE
//!
//! Is the base application of the MMI.
//! Must be lauched first by the kernel.
//!
//! @version 2.4 snd3-dvk-1_9_5 $Id: mmi_idle.c,v 2.4 2007/07/26 14:59:08 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_wireless_c_
#define _mmi_wireless_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "lib_mcu\compiler.h"
#include "conf\conf_lcd.h"
#include "drivers\mmi\img_handler.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "drivers\lcd\lcd_SSD1303.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
//#include "mmi\status\mmi_status.h"
//#include "mmi\player\mmi_player.h"
//#include "mmi\player\srv_player.h"

#include "mmi\kbd\keypad.h"

#include "mmi_wireless.h"

#include "conf\conf_oid.h"
#include "conf\conf_sio.h"
#include "modules\player\player.h"
#include "sio\com.h"
#include "mmi\shared\com_var.h"
#include <stdio.h>
#include "modules\search\search_remote.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "drivers\oid\oid.h"
#include "modules\power\power.h"             // Power module definition

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
//#include "i2c_drv.h"


//#include LCD_CONTROLLER_INCLUDE_NAME_FILE




#pragma DATA = MMI_WIRELESS_DATA
#pragma CODE = MMI_WIRELESS_CODE

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );
//_____ D E F I N I T I O N S ______________________________________________
extern Bool g_b_wireless_flag;
//-----------------------------------------------------------------------
void wireless_lcd_init( void );
void wireless_index_display(U16 temp_data);
////////////////////////////////////////////////////////////////////////////
//!
//! wireless_mmi_appli
//!
//! Manage the application idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void wireless_mmi_appli(U8 event, U16 param)
{
   switch( event )
   {
      case EVT_START_APPLI:
      SendString((U8 *)"mmi wireless\r\n");
      mmgr_set_appli_full_screen();
      power_authorize_autopoweroff();
      wireless_lcd_init();
//	  u16_current_index = 0x1234;
//	  wireless_index_display(u16_current_index);
#if( OID == ENABLE )
      //b_oid_init_flag = FALSE;
      //if( !b_oid_init_flag )
      {
         reinit_oid_pen();
		 b_oid_init_flag = TRUE;
      }
#endif   //#if( OID == ENABLE )

      Kbd_set_evt_enabled();
      if( Kbd_is_event_triggered())
         Kbd_set_evt_disabled();

      //g_b_wireless_flag = TRUE;
      
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
      bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
      scan_oid_enable();
      g_b_wireless_flag = TRUE;
      break;

      /*case EVT_APPLI_KILLED:
      break;

      case EVT_BACK_TO_TOP:
      break;*/

      case EVT_KEY_PRESSED:
      // Only if feature keyboard is activated
      SendString((U8 *)"\r\nwr_key=");
      send_hex_u16(param);
      if( KBD_KEY_LONGPRESSED == KEY_STATUS( param ))
      {
         switch( KEY_ID( param ) )
         {
		      case KEY_POWER:
		      if( power_is_vbus_present())
		      {
		      
		         if( Usb_connect )
		         {
		            g_select_appli = MSTORAGE_APPLI;
		            //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
		            //CommonLoadCode( C_USB );
		         }
		         //break;
		      }
		      else
		      {
		         /*g_select_appli = POWER_OFF_APPLI;
		         g_power_off_mode = POWER_OFF;
		         ( void )mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
		         CommonLoadCode( C_POWEROFF );*/
		         green_led_off();
                 red_led_off();
               //EXT_PWR_CTL_OFF();
               //EXT_DC_PWR_OFF();
               //PWR_OFF();
 //              while( power_key_is_press());
               //CommonLoadCode( C_POWEROFF );
               g_power_off_mode = POWER_OFF;
               g_select_appli = POWER_OFF_APPLI;
			   
		      }
		       Mmgr_kill_this_mmi_appli();
			   break;
			 case KEY_MODE:
			   //Mmgr_kill_this_mmi_appli();
			    g_b_bind_flag = TRUE;
            	SendString((U8 *)"bind key\r\n");

                SendString((U8 *)"packet=");
//              send_hex_u16( i2c_data_packet_count );
                SendString((U8 *)"\r\n");
			   break;
			   
          }
      }
      else if( KBD_KEY_RELEASED == KEY_STATUS( param ))
      {
         if( KEY_ID( param )== KEY_MODE )
         {
         /*
            g_b_bind_flag = TRUE;
            SendString((U8 *)"bind key\r\n");

            SendString((U8 *)"packet=");
//            send_hex_u16( i2c_data_packet_count );
            SendString((U8 *)"\r\n");
*/
			 g_power_off_mode = POWER_OFF;
			g_select_appli = IDLE_APPLI;
			Mmgr_kill_this_mmi_appli();
            break;
         }
      }
      
      //packet_counter++;
//      g_b_kbd_data_flag = TRUE;
      break;

      case EVT_ENTER_IN_POWEROFF:
      g_power_off_mode = POWER_OFF;
      goto mmi_idle_power_off;
      
      case EVT_ALERT_BATTERY_LOW:
      g_power_off_mode = BATTERY_LOW;
mmi_idle_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
      /*g_select_appli = POWER_OFF_APPLI;
      ( void )mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
      Mmgr_kill_this_mmi_appli();
      CommonLoadCode( C_POWEROFF );*/
     // green_led_off();
      //red_led_off();
      EXT_PWR_CTL_OFF();
      EXT_DC_PWR_OFF();
     // PWR_OFF();
//      while( power_key_is_press());
      //CommonLoadCode( C_STARTUP );
      g_select_appli = POWER_OFF_APPLI;
      Mmgr_kill_this_mmi_appli();
      break;

      default:
      Mmgr_krn_forward_current_event();
      break;
   }
}



//! @brief Draw the background
//!
void wireless_lcd_init( void )
{
//   SendString((U8 *)"\r\n214");
   lcd_turn_on();
//   SendString((U8 *)"\r\n216");
   Lcd_start_backlight_timer();
//   SendString((U8 *)"\r\n218");
   // Draw the main screen
   //lcd_draw_picture( 0, 0, IDP_BACKGROUND );
   //lcd_draw_picture( 0, 0, IDP_PLAYER_WND_ICON );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_screen();
//   SendString((U8 *)"\r\n224");
   lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
 //  SendString((U8 *)"\r\n226");
   lcd_goto_xy( 0, 8 );
   Ih_set_curr_font( IDC_FONT_16 );
//   SendString((U8 *)"\r\n229");
  if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
		lcd_puts(( U8 *)"无线模组:" );
		}
	else{
   		 lcd_puts(( U8 *)"Wireless" );
		}
   
//   SendString((U8 *)"\r\n231");
   //lcd_goto_xy( 49, 0 );
   //Ih_set_curr_font( IDC_FONT_08 );
   //lcd_puts(( U8 *)"MUSIC" );
//   status_display();
}

//! @brief Draw the background
//!
void wireless_lcd_bind(U8 temp_flag)
{
 //  return;
 #if 1  
   //lcd_turn_on();
   //Lcd_start_backlight_timer();
   // Draw the main screen
   //lcd_draw_picture( 0, 0, IDP_BACKGROUND );
   //lcd_draw_picture( 0, 0, IDP_PLAYER_WND_ICON );
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_screen();
   lcd_clear_part_of_screen(0, 26, LCD_WIDTH, (LCD_HEIGHT-26));
   lcd_goto_xy( 12, 26 );
   Ih_set_curr_font( IDC_FONT_16 );
   switch(temp_flag)
   	{
   	 case 0:
	 	if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
	 		lcd_puts(( U8 *)"绑定中.." );
	 		}
		else{
			lcd_puts(( U8 *)"Binding.." );
			}
	 	break;
	 case 1:
	 	if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
	 		lcd_puts(( U8 *)"绑定成功" );
	 		}
		else{
			lcd_puts(( U8 *)"Bind OK!" );
			}
	 	
	 	break;
	 case 2:
	 	if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
	 		lcd_puts(( U8 *)"绑定失败" );
	 		}
		else{
			lcd_puts(( U8 *)"Binding Fail!" );
			}
	 	break;
	 default:
	 	if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
	 		lcd_puts(( U8 *)"无效状态" );
	 		}
		else{
			lcd_puts(( U8 *)"Invalid state" );
			}
	 	break;
   	}
 #endif  
   //lcd_goto_xy( 49, 0 );
   //Ih_set_curr_font( IDC_FONT_08 );
   //lcd_puts(( U8 *)"MUSIC" );

}
#if 0
char *convert_U16( char *p, U16 i )
{
   if( i / 16 )
      p = convert_U16( p, i / 16 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 16 )+ '0', p + 1;
}
#endif
char *convert_U8( char *p, U16 i )
{
   if( i / 10 )
      p = convert_U8( p, i / 10 );
   //else if( i < 0 )
   //   *p++ = '-';
   return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}

//! @brief Draw the background
//!
void wireless_index_display(U16 temp_data)
{
   _MEM_TYPE_SLOW_ U8 string[ 7 ], temp_string[6],i;
  char p;
  SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_screen();
   lcd_clear_part_of_screen(0, 26, LCD_WIDTH, (LCD_HEIGHT-26));
   
   memset( string, 0, 7 );
   strcat(( char *)string, "0X" );
  
   memset( temp_string, 0, 6 );
   temp_string[1] = '\0';
   /*
   convert_U16(( char *)&temp_string[ 0 ], temp_data);
   i = strlen(( char *)&temp_string[ 0 ] );
   for( ; i < 5; i++ )
      strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&temp_string[ 0 ] );
	string[6] = '\0';
   */
   i = temp_data>>12;
//   SendString((U8 *)"\r\ni=");
   if((i%16)>9){
   	temp_string[0] = (i%16) - 10 + 'A';
   	}
   else{
   	temp_string[0] = (i%16) + '0';
   	}
//   SendString((U8 *)&temp_string);
//   SendString((U8 *)"\r\n");
   strcat(( char *)string, (char *)temp_string );
   
   i = (U8)(temp_data>>8);
   i = i&0x0F;
    if((i%16)>9){
   	temp_string[0] = (i%16) - 10 + 'A';
   	}
   else{
   	temp_string[0] = (i%16) + '0';
   	}
//  SendString((U8 *)&temp_string);
//  SendString((U8 *)"\r\n");
   strcat(( char *)string, (char *)&temp_string  );
   i = (U8)(temp_data>>4);
    i = i&0x0F;
    if((i%16)>9){
   	temp_string[0] = (i%16) - 10 + 'A';
   	}
   else{
   	temp_string[0] = (i%16) + '0';
   	}
//   SendString((U8 *)&temp_string);
 //  SendString((U8 *)"\r\n");
   strcat(( char *)string, (char *)&temp_string  );
   i = (U8)temp_data;
    i = i&0x0F;
    if((i%16)>9){
   	temp_string[0] = (i%16) - 10 + 'A';
   	}
   else{
   	temp_string[0] = (i%16) + '0';
   	}
//   SendString((U8 *)&temp_string);
//   SendString((U8 *)"\r\n");
   strcat(( char *)string, (char *)&temp_string  );
//   strcat(( char *)string, (char *)&temp_data);
  string[6] = '\0';
 //  strcat(( char *)string, (char)temp_data );
//   strcat(( char *)string, NULL );

   lcd_goto_xy( 10, 26 );
   lcd_puts( (U8*)"Hex:" );
   lcd_goto_xy( 26, 26 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( string );

   memset( string, 0, 7 );
   memset( temp_string, 0, 6 );
   convert_U8(( char *)&temp_string[ 0 ], temp_data);
   i = strlen(( char *)&temp_string[ 0 ] );
   for( ; i < 5; i++ )
      strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&temp_string[ 0 ] );
	string[6] = '\0';

//   SendString((U8 *)"\r\nstring = ");
//   SendString(string);
   lcd_goto_xy( 10, 44 );
   lcd_puts( (U8*)"Dec:" );
   lcd_goto_xy( 26, 44 );
   Ih_set_curr_font( IDC_FONT_16 );
   lcd_puts( string );

   SendString((U8 *)"display index ok!");

}
#endif // _mmi_wirless_c_
