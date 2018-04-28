//!
//! @file mmi_poweroff.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!				
//! @brief This file defines the MMI application status
//!
//! Contains poweroff and animation management
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: mmi_poweroff.c,v 2.3 2007/09/24 09:46:24 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_poweroff_c_
#define _mmi_poweroff_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "conf\conf_lcd.h"

#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\auto_disp\auto_disp.h"
#include "lib_system\timer_soft\timer_soft.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\kbd\keypad.h"
#include "mmi_poweroff.h"

#include <stdio.h>
#include "sio\com.h"

#include "modules\mmi_manager\mmgr_kernel.h"

#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

#include "drivers\oid\oid.h"
//#include "mmi\paster\mmi_paster.h"
//#include "mmi\recorder\mmi_recorder.h"
#include "modules\power\power.h"             // Power module definition
#include "modules\file_system\file.h"           // for file management 
#include "mmi\player\mmi_player.h"
#include "modules\player\player_task.h"
#include "mmi\startup\mmi_startup.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fs.h"
#include "USB_MAssStorage.h"
#include "modules\file_system\navigation.h"
#include "string.h"

#pragma DATA = MMI_POWEROFF_DATA
#pragma CODE = MMI_POWEROFF_CODE

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE04 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );


//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
// ID of the timer soft used by this MMI appli 
static _MEM_TYPE_SLOW_ U8 s_ts_id;
static _MEM_TYPE_MEDSLOW_ U8 s_test_usb_ts_id;

_MEM_TYPE_SLOW_   U8    display_battery_low_num /*= 0*/;

// EXEMPLE PLAY ONE FILE
#include "mmi\player\srv_player.h"
#include "modules\player\player.h"
Bool  mmi_poweroff_playintro        ( void );
Bool  mmi_batterylow_playintro      ( void );
Bool  mmi_paster_poweroff_playintro ( void );


//_____ D E F I N I T I O N S ______________________________________________


//! Manage the application startup
//!
//! @param U8  event
//!        U16 param
//!
void poweroff_mmi_appli(U8 event, U16 param)
{
   switch( event )
   {
      case EVT_START_APPLI:
      SendString((U8 *)"\r\nstart mmi_poweroff");
      display_battery_low_num = 0;

      lcd_turn_on();
      Lcd_start_backlight_timer();
      
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //     lcd_clear_screen();
  
     lcd_clear_part_of_screen(20, 0,64, 8);
 	  lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
      s_test_usb_ts_id = ts_alloc();
      ts_set_time( s_test_usb_ts_id, TIMER_MS( 200 ));
      scan_oid_disable();

	  //SPIF_Set_Clock(SPI_30M); 
	  if(g_power_off_mode != BATTERY_LOW){
	  	g_power_off_mode = POWER_OFF;
	  	}
	  SendString((U8 *)"\r\ng_power_off_mode=");
	  send_hex_u8(g_power_off_mode);
      /*if(( g_setting_list.player_volume != srvplayer_g_arg.volume )
         ||( g_setting_list.player_repeat != srvplayer_g_arg.repeat )
            ||( g_setting_list.language_ihm != setting_language_ihm )
               ||( g_setting_list.poweroff_value != setting_poweroff_value )
                  ||( g_setting_list.backlight_mode != setting_backlight_mode )
                     ||( g_setting_list.backlight_timeout != setting_backlight_timeout ))
      {
         g_setting_list.language_ihm = setting_language_ihm;
         g_setting_list.poweroff_value = setting_poweroff_value;
         g_setting_list.backlight_mode = setting_backlight_mode;
         g_setting_list.backlight_timeout = setting_backlight_timeout;
         g_setting_list.player_volume = srvplayer_g_arg.volume;
         g_setting_list.player_repeat = srvplayer_g_arg.repeat;
         setting_save( (U8*)&g_setting_list );
      }*/
      {
      
         SPIF_ERR SPI_STATE;

         GetDataIdInfo( D_SETTINGS, &Start_Sector, &Length );
         SPI_STATE = spif_resv_read( Start_Sector, ( __X void *)Buf );
         SendString((U8 *)"\r\nread");
         if( SPI_STATE == SDTL_No_Err )
         {
            SendString((U8 *)"\r\ncompare");
            if( memcmp(( U8 *)g_setting_list.buffer, &Buf[ 1 + SETTING_VERSION_LEN ] , sizeof( SETTING_NEW )))
            {
               SendString((U8 *)"\r\nready save");
               Buf[ 0 ] = 1 + sizeof(SETTING_VERSION) + sizeof( SETTING_NEW );
               //memcpy( &Buf[ 1 ], SETTING_VERSION , sizeof(SETTING_VERSION) );
               //memcpy( &Buf[ 1 + SETTING_VERSION_LEN ], ( U8 *)g_setting_list.buffer, sizeof( SETTING_NEW ));
			   memcpy( &Buf[ 1 ], SETTING_VERSION , sizeof(SETTING_VERSION) );
               memcpy( &Buf[ 1 + sizeof(SETTING_VERSION) ], ( U8 *)g_setting_list.buffer, sizeof( SETTING_NEW ));
			   SendString((U8 *)"\r\nbegin save");
               spif_resv_write( Start_Sector, ( __X void *)Buf );
			   SendString((U8 *)"\r\nover save");
			   SendString((U8 *)"\r\nblue=");
			   send_hex_u8(g_setting_list.struct_setting.bluetooth);
            }
         }
      }
	  
      switch( g_power_off_mode )
      {
         case POWER_OFF:
         lcd_goto_xy(30, 22 );
         //lcd_goto_xy( 32, 8 );
         Ih_set_curr_font( IDC_FONT_24 );
         lcd_puts(( U8 *)"Bye-bye!" );

         // EXEMPLE PLAY ONE FILE
         if( !mmi_poweroff_playintro() )
         {
            // Play fail then startup finish
            //Mmgr_kill_this_mmi_appli();
            //mail_send_command(CMD_POWER_OFF, 0);
            s_ts_id = ts_alloc();
            ts_set_time( s_ts_id, SPLASH_TIME );
         }
         break;

         case BATTERY_LOW:
         //Ih_set_curr_font( IDC_FONT_24 );
		  Ih_set_curr_font( IDC_FONT_16);
         lcd_goto_xy( 16, 8 );
		 if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
		 	lcd_puts((U8 *)"电量低请充电");
		 	}
		 else{
         		lcd_puts(( U8 *)"Low voltage" );
		 	}
         
#if 0//( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
         //lcd_goto_xy( 0, 8 );
         lcd_goto_xy( 20, 8 );
         lcd_puts( "Low voltage" );
#elif( LANGUAGE == Simplified )
         //lcd_goto_xy( 10, 8 );
         lcd_goto_xy( 28, 8 );
         lcd_puts( "电量不足!" );
#elif( LANGUAGE == Traditional )
         //lcd_goto_xy( 10, 8 );
         lcd_goto_xy( 28, 8 );
         lcd_puts( "筿秖ぃì!" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if 0//( SELECT_LANGUAGE == ENABLE )
         switch( g_setting_list.language_ihm )
         {
            case LANG_ENGLISH:
            //lcd_goto_xy( 0, 8 );
            lcd_goto_xy( 20, 8 );
            lcd_puts( "Low voltage" );
            break;
                        
            case LANG_CHINESE_SIMP:
            //lcd_goto_xy( 10, 8 );
            lcd_goto_xy( 28, 8 );
            lcd_puts( "电量不足!" );
            break;

            case LANG_CHINESE_TRAD:
            //lcd_goto_xy( 10, 8 );
            lcd_goto_xy( 28, 8 );
            lcd_puts( "筿秖ぃì!" );
            break;
         }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
         s_ts_id = ts_alloc();
         if( !mmi_batterylow_playintro() )
         {
            // Play fail then startup finish
            //Mmgr_kill_this_mmi_appli();
            //mail_send_command(CMD_POWER_OFF, 0);
            ts_set_time( s_ts_id, TIMER_MS( 500 ));
            display_battery_low_num = 20;
         }
         break;
      }
      break;

      case EVT_APPLI_KILLED:
      case EVT_BACK_TO_TOP:
      break;

      case EVT_TIMER:
	  SendString((U8 *)"\r\nEVT_TIMER");
      if( LSB(param) == s_test_usb_ts_id )
      {
       	SendString((U8 *)"\r\ns_test_usb_ts_id");
         ts_set_time( s_test_usb_ts_id, TIMER_MS( 200 ));
		 /*
         if( power_is_vbus_present())
         {
            Ts_free( s_test_usb_ts_id );
            SDKLoadCode( SRV_PLAYER_TEMP_CODE04 );
            srvplayer_stop();       // in case of
            //srvplayer_switch_off();  // Power off player
            power_system_off( FALSE );
            //break;
         }*/
         break;
      }
      else if( LSB( param ) == s_ts_id )
      {
      	SendString((U8 *)"\r\ns_ts_id");
         if( g_power_off_mode == POWER_OFF )
         {
            Ts_free( s_ts_id );
            Mmgr_kill_this_mmi_appli();
            mail_send_command( CMD_POWER_OFF, 0 );
         }
         else if( g_power_off_mode == BATTERY_LOW )
         {
            ts_set_time( s_ts_id, TIMER_MS( 500 ));
            if( display_battery_low_num )
            {
               display_battery_low_num--;
               if( !display_battery_low_num )
               {
                  Ts_free( s_ts_id );
                  Mmgr_kill_this_mmi_appli();
                  mail_send_command(CMD_POWER_OFF, 0);
               }
               else
               {
                  //if( display_battery_low_num % 2 )
                  if( display_battery_low_num & 0x01 )
                  {
                     SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
                     lcd_clear_screen();
                  }
                  else
                  {
                     Ih_set_curr_font( IDC_FONT_16);
			         lcd_goto_xy( 16, 8 );
					 if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP){
					 	lcd_puts((U8 *)"电量低请充电");
					 	}
					 else{
			         		lcd_puts(( U8 *)"Low voltage" );
					 	}
#if 0//( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
                     //lcd_goto_xy( 0, 8 );
                     lcd_goto_xy( 20, 8 );
                     lcd_puts( "Low voltage" );
#elif( LANGUAGE == Simplified )
                     //lcd_goto_xy( 10, 8 );
                     lcd_goto_xy( 28, 8 );
                     lcd_puts( "电量不足!" );
#elif( LANGUAGE == Traditional )
                     //lcd_goto_xy( 10, 8 );
                     lcd_goto_xy( 28, 8 );
                     lcd_puts( "筿秖ぃì!" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if 0//( SELECT_LANGUAGE == ENABLE )
                     switch( g_setting_list.language_ihm )
                     {
                        case LANG_ENGLISH:
                        //lcd_goto_xy( 0, 8 );
                        lcd_goto_xy( 20, 8 );
                        lcd_puts( "Low voltage" );
                        break;
                        
                        case LANG_CHINESE_SIMP:
                        //lcd_goto_xy( 10, 8 );
                        lcd_goto_xy( 28, 8 );
                        lcd_puts( "电量不足!" );
                        break;

                        case LANG_CHINESE_TRAD:
                        //lcd_goto_xy( 10, 8 );
                        lcd_goto_xy( 28, 8 );
                        lcd_puts( "筿秖ぃì!" );
                        break;
                     }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
                  }
               }
            }
         }
      }
      break;
      
      case EVT_PLAYER:
	  SendString((U8 *)"\r\nevt_player");
      if( param == PLAYER_EOF )
      {
         //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
         //srvplayer_switch_off();  // Power off player
         if( g_power_off_mode == POWER_OFF )
         {
            Mmgr_kill_this_mmi_appli();
            mail_send_command(CMD_POWER_OFF, 0);
         }
         else if( g_power_off_mode == BATTERY_LOW )
         {
            ts_set_time( s_ts_id, TIMER_MS( 500 ));
            display_battery_low_num = 20;
         }
      }
      break;

      /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
      power_system_off( !power_is_vbus_present());
      break;*/

      //default:
      //break;
   }
}

// EXEMPLE PLAY ONE FILE

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_poweroff_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( "poweroff.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_batterylow_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE00 );
   // Search file to play
   if( !search_subdir_mp3_file( "batterylow.mp3" ))
      return FALSE;

   SDKLoadCode( MMI_PLAYINTRO_CODE01 );
   if( !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}

#if 0
//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_paster_poweroff_playintro( void )
{
   SDKLoadCode( MMI_PLAYINTRO_CODE );
   // Search file to play
   if( !search_subdir_mp3_file( "recclose.mp3" )
      || !play_sbdir_mp3_file())
      return FALSE;
   return TRUE;
}
#endif
#endif // _mmi_startup_c_

