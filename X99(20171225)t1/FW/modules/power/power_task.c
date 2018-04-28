//!
//! @file power_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the power task.
//!
//! @version 1.26 snd3-dvk-1_9_5 $Id: power_task.c,v 1.26 2007/07/24 09:33:14 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _power_task_c_
#define _power_task_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"                          // System definition
#include "stdio.h"
#include <string.h>
#include "lib_mcu\compiler.h"                          // System definition
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_mcu\timer\timer_drv.h"

#include "modules\power\power.h"             // Power module definition

#include "mmi\kbd\keypad.h"
#include "mmi\shared\com_var.h"
#include "modules\power\power_task.h"
//#include "lib_mem\mmc\mmc_sd.h"
//#include "SSSMP3_regs.h"
//#include "modules\search\search_task.h"
#include "modules\player\player_task.h"
#include "sio\com.h"
#include "conf\conf_oid.h"
#include "drivers\oid\oid.h"
#include "mmi\shared\com_evt.h"
#include "mmi\images\tab_pict.h"
#include "mmi\status\mmi_status.h"

#include "conf\conf_lcd.h"
#include "lib_mcu\lcd\lcd_drv.h"
#include "drivers\lcd\lcd_SSD1303.h"
#include "mmi\images\tab_pict.h"
#include "mmi\shared\com_appli.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "adc.h"
#include "conf\conf_access.h"
#include "drivers\mmi\img_handler.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE


#pragma DATA = POWER_TASK_DATA
#pragma CODE = POWER_TASK_CODE


ImageDeclareType( POWER_TEMP_CODE00 );
ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( POWER_TASK_TEMP_CODE00 );
ImageDeclareType( POWER_TASK_TEMP_CODE01 );
ImageDeclareType( POWER_TASK_TEMP_CODE02 );
ImageDeclareType( POWER_TASK_TEMP_CODE03 );
ImageDeclareType( POWER_TASK_TEMP_CODE04 );
ImageDeclareType( POWER_TASK_TEMP_CODE05 );
ImageDeclareType( SYSTEM_POWER_OFF_CODE );


//_____ M A C R O S ________________________________________________________


//_____ P R I V A T E    D E C L A R A T I O N S ___________________________
//extern _MEM_TYPE_SLOW_ U32 Detect_voltage;
//extern _MEM_TYPE_SLOW_ U16 Det_value;
Bool g_b_recorder_on       /*= FALSE*/;

//extern            Bool  g_b_recorder_on;
//extern            Bool  g_b_player_on;

_MEM_TYPE_SLOW_   U32   s_batt_timer;
                  //Bool  s_b_batt_evt_low_sent;
                  Bool  s_b_vbus_present;

//_MEM_TYPE_MEDSLOW_   U8    battery_low_counter = 0;
//_MEM_TYPE_MEDSLOW_   U8    battery_level_counter = 0;
//_MEM_TYPE_MEDSLOW_   U32   batt_low_warning_timer = 0;

_MEM_TYPE_MEDSLOW_   U32   battery_timer_count;

//_MEM_TYPE_MEDSLOW_   U32   usb_charge_time = 0;

_MEM_TYPE_MEDSLOW_   U32   u32_vbus_time;
_MEM_TYPE_MEDSLOW_   U32   u32_sd_card_time;
_MEM_TYPE_MEDSLOW_   U32   u32_earphone_time;
_MEM_TYPE_MEDSLOW_   U8    u8_date[4];


//_____ D E F I N I T I O N S ______________________________________________
//#define Detect_voltage              Det_value
//#define POWER_BATTERY_LEVEL_ALERT   1568

//#define POWER_BATTERY_LOW_WARNING   1705

void  sd_vbus_check           ( void );
void  battery_status_updata   ( void );
void  battery_low_check       ( void );
void  led_set_deal            ( void );
void  system_power_deal       ( void );
void  system_power_off        ( void );
void  lcd_display_time		  ( Bool );
void dowith_sd_vbus_check(void);
void dowith_battery_status(void);
void dowith_battery_low_1(void);
void dowith_battery_low_2(void);
/***************************** WARNING *****************************************/
/* All Power events are routed directly to the first launched appli MMI_STATUS */
/*******************************************************************************/

void power_task(void)
{
   // The first thing that every task need to do is to check if an
   // eventual command is available for it or not
   Msg _MEM_TYPE_MALLOC_MAIL_ vl_Cmd;
   
   if ( PASS==mail_get_command(TASK_POWER, &vl_Cmd) )
   {
      // Treat special message
#if 0
     if(CMD_POWER_OFF == vl_Cmd.id){
        SDKLoadCode( SYSTEM_POWER_OFF_CODE );
        system_power_off();
     }else if(CMD_GET_VBUS_STATE  == vl_Cmd.id){
        mail_send_event( EVT_POWER_VBUS_STATE, power_is_vbus_present());
     }else if(CMD_BATTERY_CHARGE_DISABLE  == vl_Cmd.id){
         power_stop_batt_charging();      // send EVT_BATTERY_STATE
     }else if(CMD_BATTERY_CHARGE_ENABLE  == vl_Cmd.id){
        power_activate_batt_charging();  // send EVT_BATTERY_STATE
     }else{
     }
#else
      switch(/*(U8)*/vl_Cmd.id)
      {
         case CMD_POWER_OFF:
            SDKLoadCode( SYSTEM_POWER_OFF_CODE );
            system_power_off();
#if 0
            Power_set_poweroff_activated();
            
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_screen();
            //Display off
            lcd_turn_off();
            // Reset the OLED controller
            Lcd_reset_on();
            
            SDKLoadCode( POWER_TEMP_CODE00 );
            power_system_off( TRUE );
            /*Power_unset_poweroff_activated();
#if (POWER_OFF_AUTO == ENABLE)
            power_rearm_autopoweroff_delay();
#endif
            // Disable keypad events to avoid spurious ones in the new top appli
            kbd_disable_event(TRUE);
            // If turned in power-down mode, the restart is done here
            mail_send_event(EVT_POWER_ON, 0);*/
#endif
            break;
#if 0//(POWER_OFF_AUTO == ENABLE)
         case CMD_AUTO_POWER_OFF_DISABLE:
            g_b_autopoweroff_inhibited = TRUE;
            break;
         case CMD_AUTO_POWER_OFF_ENABLE:
            g_b_autopoweroff_inhibited = FALSE;
            power_rearm_autopoweroff_delay();
            break;
         case CMD_POWER_SETTING_LOADED:
            g_b_autopoweroff_initialized = TRUE;
            power_rearm_autopoweroff_delay();
            break;
#endif
         case CMD_GET_VBUS_STATE:
            mail_send_event( EVT_POWER_VBUS_STATE, power_is_vbus_present());  // give state no power from cable
            break;
         case CMD_BATTERY_CHARGE_DISABLE:
            power_stop_batt_charging();      // send EVT_BATTERY_STATE
            break;
         case CMD_BATTERY_CHARGE_ENABLE:
            power_activate_batt_charging();  // send EVT_BATTERY_STATE
            break;
         /*default:
         {
            // IT IS JUST AN EXAMPLE!!! IT IS JUST AN EXAMPLE
            // Send the command Dummy
            // mail_send_command(CMD_DUMMY, NULL);
            // END EXAMPLE
         }break;*/
      }
#endif
   }
   adc_get();
   /*
   if(b_flag_rtc){
   	b_flag_rtc = FALSE;
	SDKLoadCode(POWER_TASK_TEMP_CODE05);
	lcd_display_time(TRUE);
   	}
 */  
   if( Power_is_poweroff_activated() )    { return; }

   //SDKLoadCode( POWER_TASK_TEMP_CODE00 );
   sd_vbus_check();
#if 0
   // Test the presence or not of the Vbus (USB power)
   if( s_b_vbus_present != power_is_vbus_present())
   {
      s_b_vbus_present = power_is_vbus_present();
      //mail_send_event( EVT_POWER_VBUS_STATE, s_b_vbus_present );
      if( s_b_vbus_present )
         mail_send_event( EVT_USB_CTRL_POWERED, 0 );
      else
         mail_send_event( EVT_USB_CTRL_UNPOWERED, 0 );
   }
#endif

//***********************************************
#if 0//( BAT_CHARGER_PART_NUMBER == BAT_CHARGER_NONE )
   if( power_is_vbus_present())
   {
      if( !g_b_recorder_on )
      {
         if( usb_sd_timer_timeout())
            usb_sd_timer_stop();
         if( green_led_is_timer_stopped()&& usb_sd_timer_is_stopped())
         {
            green_led_on();
            green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
         }
         if( red_led_is_timer_stopped())
         {
            red_led_off();
            red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
         }
      }
   }
   else
   {
      if( !g_b_recorder_on )
      {
         if( green_led_is_timer_stopped())
         {
            green_led_on();
            green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
         }
         if( g_b_battery_low )
         {
            if( red_led_is_timer_stopped())
            {
               red_led_on();
               red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
            }
         }
         else
         {
            if( green_led_is_timer_stopped())
            {
               green_led_on();
               green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
            }
            if( red_led_is_timer_stopped())
            {
               red_led_off();
               red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
            }
         }
      }
   }
#endif   //#if( BAT_CHARGER_PART_NUMBER == BAT_CHARGER_NONE )
//***********************************************

   // Update the battery status
   //SDKLoadCode( POWER_TASK_TEMP_CODE01 );
   battery_status_updata();
#if 0
   if( Timer_is_timeout(s_batt_timer) )
   {
      s_batt_timer = Timer_set_timer(TIMER_S(POWER_BATTERY_SCAN_PERIOD));
      if( s_b_vbus_present )
      {
         g_b_battery_low = FALSE;
         if( !Bat_is_charge_on())
            mail_send_event( EVT_BATTERY_LEVEL, 4 );
         else
         {
            if( Bat_is_fully_charged())
            {
               bat_charge_counter++;
               if( bat_charge_counter >= 2 )
               {
                  bat_charge_counter = 0;
                  power_stop_batt_charging();
                  //SendString((U8 *)"stop charge\r\n");
               }
            }
            else
               bat_charge_counter = 0;
         }
      }
      else
      {
         /*SendString((U8 *)"bat=");
         send_hex_u16(battery_value);
         SendString((U8 *)"\r\n");*/
         if( battery_value >= BAT_FULL )
            mail_send_event( EVT_BATTERY_LEVEL, 4 );
         else if( battery_value >= BAT_MED )
            mail_send_event( EVT_BATTERY_LEVEL, 3 );
         else if( battery_value >= BAT_LOW )
            mail_send_event( EVT_BATTERY_LEVEL, 2 );
         else
            mail_send_event( EVT_BATTERY_LEVEL, 1 );
      }
#if 0
      if( !g_b_recorder_on/* && !g_b_player_on */)
      {
         Det_LineIn_voltage();
         mail_send_event( EVT_BATTERY_LEVEL, Detect_voltage );
      }
#endif
   }
#endif

   //SDKLoadCode( POWER_TASK_TEMP_CODE02 );
   battery_low_check();
#if 0
   if( !s_b_vbus_present )
   {
#if 0
      if( Timer_is_timeout( s_batt_timer ))
      {
         //s_batt_timer = Timer_set_timer( TIMER_S( POWER_BATTERY_SCAN_PERIOD ));
         s_batt_timer = Timer_set_timer( TIMER_MS( 2500 ));
         if( !g_b_recorder_on/* && !g_b_player_on */)
         {
            Det_LineIn_voltage();
            if( !s_b_batt_evt_low_sent )
            {
               if( Detect_voltage < POWER_BATTERY_LOW_WARNING )
               {
                  if( !g_b_battery_low )
                  {
                     battery_low_counter++;
                     if( battery_low_counter >= 3 )
                     {
                        battery_low_counter = 0;
                        g_b_battery_low = TRUE;
                        red_led_on();
                        green_led_off();
                        red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
                        batt_low_warning_timer = Timer_set_timer( TIMER_MIN( 3 ));
                     }
                  }
               }
               else
                  battery_low_counter = 0;

               if( Detect_voltage <= POWER_BATTERY_LEVEL_ALERT ) 
               {
                  battery_level_counter++;
                  if( battery_level_counter >= 3 )
                  {
                     battery_level_counter = 0;
                     // Lock to sent once the events
                     s_b_batt_evt_low_sent = TRUE;
                     // Event "EVT_BATTERY_ALERT_LEVEL"
                     mail_send_event( EVT_BATTERY_LEVEL_ALERT, 0 );
                  }
               }
               else
                  battery_level_counter = 0;
            }
            /*else
            {
               // Current value above the threshold, so unlock
               if( Detect_voltage > POWER_BATTERY_LEVEL_ALERT )
               {
                  s_b_batt_evt_low_sent = FALSE;
               }
            }*/
         }
      }
#endif
      if( !g_b_battery_low )
      {
#if 1
         /*SendString((U8 *)"bat=");
         send_hex_u16(battery_value);
         SendString((U8 *)"\r\n");*/
         if( battery_timer_count && Timer_is_timeout( battery_timer_count ))
         {
            battery_timer_count = Timer_set_timer( TIMER_MS( 1500 ));
            if( battery_value >= BAT_V_LOW )
            {
               if( battery_low_counter )
                  battery_low_counter--;
            }
            
            if( battery_value <= LVT_VALUE )
            {
               battery_low_counter++;
               if( battery_low_counter >= 5 )
               {
                  battery_low_counter = 0;
                  g_b_battery_low = TRUE;
                  red_led_on();
                  green_led_stop_timer();
                  red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
                  batt_low_warning_timer = Timer_set_timer( TIMER_S( 2 ));
                  //batt_low_warning_timer = Timer_set_timer( TIMER_MIN( 3 ));

                  mail_send_event( EVT_BAT_WARING, 0 );
                  mail_send_event( EVT_BATTERY_LEVEL, 1 );
               }
            }
            //else
            //   battery_low_counter = 0;
         }
#else
         extern Bool b_oid_pen;
         extern Bool b_oid_bat_lower;
         if( b_oid_pen )
         {
            b_oid_pen = FALSE;
            if( b_oid_bat_lower )
            {
               battery_low_counter++;
               if( battery_low_counter >= 2 )
               {
                  battery_low_counter = 0;
                  g_b_battery_low = TRUE;
                  red_led_on();
                  red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
                  batt_low_warning_timer = Timer_set_timer( TIMER_S( 10 ));

                  mail_send_event( EVT_BAT_WARING, 0 );
               }
            }
            else
               battery_low_counter = 0;
         }
#endif
      }
      else
      {
         if( batt_low_warning_timer && Timer_is_timeout( batt_low_warning_timer ))
         {
            // Lock to sent once the events
            //s_b_batt_evt_low_sent = TRUE;
            // Event "EVT_BATTERY_ALERT_LEVEL"
            mail_send_event( EVT_BATTERY_LEVEL_ALERT, 0 );
            batt_low_warning_timer = Timer_set_timer( TIMER_S( 10 ));
         }
      }
      //goto set_led_status;
   }
#endif
#if 0
   else
   {
      g_b_battery_low = FALSE;
      /*if( g_select_appli == MSTORAGE_APPLI )
      {
         if( usb_sd_timer_timeout())
         {
            usb_sd_timer_stop();
            green_led_on();
            if( Bat_is_charge_on())
               red_led_on();
            green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
         }
      }*/
      if( battery_timer_count && Timer_is_timeout( battery_timer_count ))
      {
         battery_timer_count = Timer_set_timer( TIMER_MS( 1500 ));
         if( Bat_is_charge_on()&& Bat_is_fully_charged())
         {
            bat_charge_counter++;
            if( bat_charge_counter >= 3 )
            {
               bat_charge_counter = 0;
               power_stop_batt_charging();
               //SendString((U8 *)"stop charge\r\n");
            }
         }
         else
            bat_charge_counter = 0;
      }
#if 0
set_led_status:
      if( !g_b_recorder_on )
      {
         if( g_b_set_led )
         {
            g_b_set_led = FALSE;
            if( green_led_is_timer_stopped())
            {
               green_led_on();
               green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
            }
            if( red_led_is_timer_stopped())
            {
               if( g_b_battery_low )
               {
                  red_led_on();
                  green_led_off();
                  green_led_stop_timer();
               }
               else
               {
                  if( Bat_is_charge_on())
                     red_led_on();
                  else
                     red_led_off();
               }
               red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
            }
         }
      }
#endif
   }
#endif

   SDKLoadCode( POWER_TASK_TEMP_CODE04 );
   system_power_deal();
#if 0
#if (POWER_OFF_AUTO == ENABLE)
   if( !g_b_autopoweroff_forbidden )
   {
      if( Timer_is_timeout( g_autopoweroff_timer ))
      {
         power_rearm_autopoweroff_delay();   // To avoid to enter again here after the timeout
         if( power_is_vbus_present())
         {  // Not enter in power off if USB power
            power_rearm_autopoweroff_delay();
         }
         else
         {
            mail_send_event( EVT_POWER_ENTER_IN_POWEROFF, 0 );
         }
      }
   }
#endif
#endif

//***********************************************
   if( !g_b_recorder_on )
   {
      SDKLoadCode( POWER_TASK_TEMP_CODE03 );
      led_set_deal();
   }
#if 0//( OID == ENABLE )
   if( !g_b_set_led )
   {
      if( green_led_is_timer_timeout())
      {
#if 1
         U16 led_time;

         if( Bat_is_charge_on()
            &&( g_select_appli == MSTORAGE_APPLI ))
         {
            if( !green_led_is_off())
            {
               green_led_off();
               red_led_off();
            }
            else
            {
               green_led_on();
               red_led_on();
            }
            led_time = 150;
            //green_led_start_timer( 150 );
         }
         else
         {
            if( !green_led_is_off())
            {
               green_led_off();
               if( bPenLeave )
               {
                  //green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );
                  led_time = GREEN_LED_OFF_NORMAL_TIME;
               }
               else
               {
                  //green_led_start_timer( GREEN_LED_OFF_READ_TIME );
                  led_time = GREEN_LED_OFF_READ_TIME;
               }
            }
            else
            {
               green_led_on();
               if( bPenLeave )
               {
                  //green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
                  led_time = GREEN_LED_ON_NORMAL_TIME;
               }
               else
               {
                  //green_led_start_timer( GREEN_LED_ON_READ_TIME );
                  led_time = GREEN_LED_ON_READ_TIME;
               }
            }
         }
         green_led_start_timer( led_time );
#else
         if( !green_led_is_off())
         {
            green_led_off();
            if( Bat_is_charge_on())
            {
               red_led_off();
               green_led_start_timer( 150 );
            }
            else
            {
               if( bPenLeave )
                  green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );
               else
                  green_led_start_timer( GREEN_LED_OFF_READ_TIME );
            }
         }
         else
         {
            green_led_on();
            if( Bat_is_charge_on())
            {
               red_led_on();
               green_led_start_timer( 150 );
            }
            else
            {
               if( bPenLeave )
                  green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
               else
                  green_led_start_timer( GREEN_LED_ON_READ_TIME );
            }
         }
#endif
      }
#if 1
      if( g_b_battery_low )
      {
         green_led_stop_timer();
         if( red_led_is_timer_timeout())
         {
            if( !green_led_is_off())
               green_led_off();
            if( !red_led_is_off())
            {
               red_led_off();
               red_led_start_timer( RED_LED_OFF_BATTERY_LEVEL_LOW_TIME );
            }
            else
            {
               red_led_on();
               red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
            }
         }
      }
#endif
   }
#endif   //#if( OID == ENABLE )

#if 0
   if( usb_charge_time && Timer_is_timeout( usb_charge_time ))
   {
      usb_charge_time = 0;
      mail_send_command( CMD_BATTERY_CHARGE_ENABLE, 0 );
   }
#endif
}
void sd_vbus_check( void )
{
//#ifndef TX_DEBUG
   if( s_b_vbus_present != power_is_vbus_present())
   {
	 SDKLoadCode(POWER_TASK_TEMP_CODE00);
	 dowith_sd_vbus_check();
   }
//#endif
#if 0//( MEM_MMC != DISABLED )
   if( b_card_state != Mmc_card_present())
   {
      /*if( !u32_sd_card_time )
      {
         u32_sd_card_time = Timer_set_timer( TIMER_MS( 50 ));
      }
      if( Timer_is_timeout( u32_sd_card_time ))*/
      {
         //u32_sd_card_time = 0;
         //if( b_card_state != Mmc_card_present())
         {
            SendString((U8 *)"remove\r\n");
            RESET_SYS();
         }
      }
   }
#endif
}
void battery_status_updata( void )
{
   if( Timer_is_timeout(s_batt_timer) )
   {
//    if(s_b_vbus_present)return;
    SDKLoadCode(POWER_TASK_TEMP_CODE01);
	dowith_battery_status();
   }
}
void battery_low_check( void )
{
   if( !s_b_vbus_present )
   {
#if 1

     
         /*SendString((U8 *)"bat=");
         send_hex_u16(battery_value);
         SendString((U8 *)"\r\n");*/
         if( /*battery_timer_count && */Timer_is_timeout( battery_timer_count ))
         {
         	  SDKLoadCode(POWER_TASK_TEMP_CODE02);
         	 if( !g_b_battery_low ){
		 		 dowith_battery_low_1();
         	 }
			 else{
            		dowith_battery_low_2();
			 	}
         }
#else
      if( !g_b_battery_low )
      {
         /*SendString((U8 *)"bat=");
         send_hex_u16(battery_value);
         SendString((U8 *)"\r\n");*/
         if( /*battery_timer_count && */Timer_is_timeout( battery_timer_count ))
         {
         SDKLoadCode(POWER_TASK_TEMP_CODE02);
		 dowith_battery_low_1();
         }
      }
      else
      {
         if( /*batt_low_warning_timer && */Timer_is_timeout( batt_low_warning_timer ))
         {
            SDKLoadCode(POWER_TASK_TEMP_CODE02);
            dowith_battery_low_2();
         }
      }

#endif	  
   }
}



#pragma CODE = TASK_INIT_CODE

void power_task_init(void)
{
   // Power off 
   Power_unset_poweroff_activated();
#if (POWER_OFF_AUTO == ENABLE)
   Power_init_autopoweroff();
#endif

   // Battery
   //s_b_batt_evt_low_sent = FALSE;
   s_batt_timer = 0;     // timer set to 0 to have a time-out right now
   //g_b_battery_low = FALSE;
   if( g_b_battery_low )
   {
      red_led_on();
      green_led_stop_timer();
      red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
      batt_low_warning_timer = Timer_set_timer( TIMER_S( 2 ));

      mail_send_event( EVT_BATTERY_LEVEL, 1 );
   }
//   adc2_value = Get_ADCValue(2);
   if(adc2_value>100){
   	s_b_vbus_present = TRUE;
   	}
   else{
   	s_b_vbus_present = FALSE;
   	}
   u32_op_delay_off_time = 0;

   battery_timer_count = Timer_set_timer( TIMER_MS( 1500 ));

//   if( usb_charge_time )
//      usb_charge_time = Timer_set_timer( TIMER_MS( 500 ));

   //battery_value = Get_ADCValue( 0 );
   battery_low_counter = 0;

   u32_sd_card_time = 0;
   u32_vbus_time = 0;
   u32_earphone_time = 0;

   g_b_recorder_on = FALSE;
   g_b_set_led = FALSE;
  
   exit_flag = FALSE;
   g_b_oid_enable = FALSE;
   u32_oid_read_timeout = 0;
   u32_oid_read_time = 0;
   bPenLeave = TRUE;
   g_oid = FALSE;

   b_g_index_flag = FALSE;
   g_b_animation = FALSE;

   lcd_g_viewport.right_x = LCD_WIDTH - 1;
   g_backlight_en = TRUE;
   g_mmi_lcd_backlight_off = FALSE;
   lcd_g_inverse = FALSE;
   g_b_lcd_backlight_off = FALSE;

   b_flag_rtc = FALSE;
   b_flush_lcd = FALSE;
   count_AB = 0;
  
}


#pragma CODE = POWER_TASK_TEMP_CODE00
void dowith_sd_vbus_check(void)
{
	if( !u32_vbus_time )
	 u32_vbus_time = Timer_set_timer( TIMER_MS( 150 ));
	if( Timer_is_timeout( u32_vbus_time ))
	{
	 u32_vbus_time = 0;
	  SendString((U8 *)"\r\nb_battery_charging = ");
      send_hex_u8(b_battery_charging);
	 if( s_b_vbus_present != power_is_vbus_present())
	 {
//	 	SendString((U8 *)"\r\n810");
	    s_b_vbus_present = power_is_vbus_present();
	    //mail_send_event( EVT_POWER_VBUS_STATE, s_b_vbus_present );
//	 	SendString((U8 *)"\r\ns_b_vbus_present:b_battery_charging=");
//		send_hex_u8(s_b_vbus_present);SendString((U8 *)":");send_hex_u16(adc2_value);
	    if( s_b_vbus_present )
	    {
	       mail_send_event( EVT_USB_CTRL_POWERED, 0 );
	       usb_charge_time = Timer_set_timer( TIMER_S( 2 ));
	    }
	    else
	       mail_send_event( EVT_USB_CTRL_UNPOWERED, 0 );
	 }
	}
//	 SendString((U8 *)"\r\nadc2_value = ");
//      send_hex_u16(adc2_value);
}



#pragma CODE = POWER_TASK_TEMP_CODE01

void dowith_battery_status(void)
{
  s_batt_timer = Timer_set_timer(TIMER_S(POWER_BATTERY_SCAN_PERIOD));
  if( s_b_vbus_present )
  {
     g_b_battery_low = FALSE;
     if( !Bat_is_charge_on())
        mail_send_event( EVT_BATTERY_LEVEL, 4 );
     else
     {
        if( Bat_is_fully_charged())
        {
           bat_charge_counter++;
           if( bat_charge_counter >= 2 )
           {
              //bat_charge_counter = 0;
              power_stop_batt_charging();
              //SendString((U8 *)"stop charge\r\n");
           }
        }
        else
           bat_charge_counter = 0;
     }
  }
  else
  {
     /*SendString((U8 *)"bat=");
     send_hex_u16(battery_value);
     SendString((U8 *)"\r\n");*/
     if( battery_value >= BAT_FULL )
        mail_send_event( EVT_BATTERY_LEVEL, 4 );
     else if( battery_value >= BAT_MED )
        mail_send_event( EVT_BATTERY_LEVEL, 3 );
     else if( battery_value >= BAT_LOW )
        mail_send_event( EVT_BATTERY_LEVEL, 2 );
     else
        mail_send_event( EVT_BATTERY_LEVEL, 1 );
  }
}

#pragma CODE = POWER_TASK_TEMP_CODE02
void dowith_battery_low_1(void)
{
    battery_timer_count = Timer_set_timer( TIMER_MS( 1500 ));
    if( battery_value > BAT_V_LOW )
   {
       if( battery_low_counter )
         battery_low_counter--;
    }

	if(!g_b_warning_tishi){
		if( battery_value <= BAT_V_LOW ){
			g_b_warning_tishi = TRUE;
			//SendString((U8 *)"\r\nWARING!");
          	mail_send_event( EVT_BAT_WARING, 0 );
		  }
		} 	
    if( battery_value <= LVT_VALUE )
    {
       battery_low_counter++;
       if( battery_low_counter >= 3 )
       {
          //battery_low_counter = 0;
          g_b_battery_low = TRUE;
          red_led_on();
          green_led_stop_timer();
          red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
          batt_low_warning_timer = Timer_set_timer( TIMER_S( 2 ));
          //batt_low_warning_timer = Timer_set_timer( TIMER_MIN( 3 ));
          //SendString((U8 *)"\r\nALERT_1!");
          mail_send_event( EVT_BATTERY_LEVEL, 1 );
       }
    }
}

void dowith_battery_low_2(void)
{
	//SendString((U8 *)"\r\nALERT!");
 	mail_send_event( EVT_BATTERY_LEVEL_ALERT, 0 );
    batt_low_warning_timer = Timer_set_timer( TIMER_S( 10 ));
}


#pragma CODE = POWER_TASK_TEMP_CODE03

void led_set_deal( void )
{
#if 1//( OID == ENABLE )
   if( !g_b_set_led )
   {
      if( green_led_is_timer_timeout())
      {
         U16 led_time;

         if( Bat_is_charge_on()
            &&( g_select_appli == MSTORAGE_APPLI ))
         {
            if( !green_led_is_off())
            {
               green_led_off();
               red_led_off();
            }
            else
            {
               green_led_on();
               red_led_on();
            }
            led_time = 150;
            //green_led_start_timer( 150 );
         }
         else
         {
            if( !green_led_is_off())
            {
               green_led_off();
               if( bPenLeave )
               {
                  //green_led_start_timer( GREEN_LED_OFF_NORMAL_TIME );
                  led_time = GREEN_LED_OFF_NORMAL_TIME;
               }
               else
               {
                  //green_led_start_timer( GREEN_LED_OFF_READ_TIME );
                  led_time = GREEN_LED_OFF_READ_TIME;
               }
            }
            else
            {
               green_led_on();
               if( bPenLeave )
               {
                  //green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
                  led_time = GREEN_LED_ON_NORMAL_TIME;
               }
               else
               {
                  //green_led_start_timer( GREEN_LED_ON_READ_TIME );
                  led_time = GREEN_LED_ON_READ_TIME;
               }
            }
         }
         green_led_start_timer( led_time );
      }
#if 0
      if( g_b_battery_low )
      {
         green_led_stop_timer();
         if( red_led_is_timer_timeout())
         {
            if( !green_led_is_off())
               green_led_off();
            if( !red_led_is_off())
            {
               red_led_off();
               red_led_start_timer( RED_LED_OFF_BATTERY_LEVEL_LOW_TIME );
            }
            else
            {
               red_led_on();
               red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
            }
         }
      }
#endif
   }
#endif   //#if( OID == ENABLE )
}


#pragma CODE = POWER_TASK_TEMP_CODE04

void system_power_deal( void )
{
#if (POWER_OFF_AUTO == ENABLE)
   if( !g_b_autopoweroff_forbidden )
   {
      if( Timer_is_timeout( g_autopoweroff_timer ))
      {
         power_rearm_autopoweroff_delay();   // To avoid to enter again here after the timeout
         /*if( power_is_vbus_present())
         {  // Not enter in power off if USB power
            power_rearm_autopoweroff_delay();
         }
         else*/
         if( !power_is_vbus_present())
         {
            b_readySysUP = true;
         	SendString((U8 *)"\r\nsystem_power_deal\r\n");
            mail_send_event( EVT_POWER_ENTER_IN_POWEROFF, 0 );
         }
      }
   }
#endif

   if( usb_charge_time && Timer_is_timeout( usb_charge_time ))
   {
      usb_charge_time = 0;
      mail_send_command( CMD_BATTERY_CHARGE_ENABLE, 0 );
   }
}
#pragma CODE = POWER_TASK_TEMP_CODE05
/*
char *date_convert_U8( char *p, U8 i )
{
   if( i / 10 )
      p = date_convert_U8( p, i / 10 );
   return *( char *)p = (i % 10 )+ '0', p + 1;
}

void change_date(void)
{
  SendString((U8 *)"\r\nEVT_DATE");
	g_setting_list.struct_setting.min ++;
	if(g_setting_list.struct_setting.min >= 60){
		g_setting_list.struct_setting.min = 0;
		g_setting_list.struct_setting.hour ++;
		if(g_setting_list.struct_setting.hour>=24){
			g_setting_list.struct_setting.hour = 0;
			g_setting_list.struct_setting.day ++;
			if(g_setting_list.struct_setting.day>=30){
				g_setting_list.struct_setting.day = 1;
				g_setting_list.struct_setting.month ++;
				if(g_setting_list.struct_setting.month >= 12){
					g_setting_list.struct_setting.month = 1;
					g_setting_list.struct_setting.year ++;
					}
				}
			}
		} 
}

void date_change(U8 date_data)
{
  _MEM_TYPE_SLOW_  i;
   //memset( temp_string, 0, 3 );
    memset( u8_date, 0, 4 );
	i = date_data/10;
	if(i){
		
//		strcat(( char *)u8_date, (char *)('0'+i) );
		u8_date[0] = '0'+i;
		}
	else{
//		strcat(( char *)u8_date, "0" );
		u8_date[0] = '0';
		}
	i = date_data%10;
	if(i){
	//	strcat(( char *)u8_date, ('0'+i) );
		u8_date[1] = '0'+i;
		}
	else{
	//	strcat(( char *)u8_date, "0" );
		u8_date[1] = '0';
		}
	  
   //strcat(( char *)u8_date, "/" );
    u8_date[2] = '/';
}
//*/
//*/
//! Display time
//!
void lcd_display_time( Bool flag )
{  
/*
   _MEM_TYPE_SLOW_ U8 string[ 17 ], temp_string[3],i;
   memset( string, 0, 17 );
   memset( temp_string, 0, 3 );


   if(flag)change_date();
   
   strcat(( char *)string, "20" );
#if 0
   date_change(g_setting_list.struct_setting.year%100);
   strcat(( char *)string, ( char *)&u8_date);
   date_change(g_setting_list.struct_setting.month);
   strcat(( char *)string, ( char *)&u8_date);
   date_change(g_setting_list.struct_setting.day);
   strcat(( char *)string, ( char *)&u8_date);
   date_change(g_setting_list.struct_setting.hour);
   strcat(( char *)string, ( char *)&u8_date);
   date_change(g_setting_list.struct_setting.min);
   strcat(( char *)string, ( char *)&u8_date);
//#else
   date_convert_U8(( char *)&temp_string, g_setting_list.struct_setting.year%100);
   i = strlen(( char *)&temp_string );
   if(i <= 1)strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&temp_string );
   strcat(( char *)string, "/" );
   
   memset( temp_string, 0, 3 );
   date_convert_U8(( char *)&temp_string, g_setting_list.struct_setting.month);
   i = strlen(( char *)&temp_string );
   if(i <= 1)strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&temp_string );
   strcat(( char *)string, "/" );
   
   memset( temp_string, 0, 3 );
   date_convert_U8(( char *)&temp_string, g_setting_list.struct_setting.day);
   i = strlen(( char *)&temp_string );
   if(i <= 1)strcat(( char *)string, "0" );
   strcat(( char *)string, ( char *)&temp_string );
   strcat(( char *)string, "/" );
   
   memset( temp_string, 0, 3 );
   date_convert_U8(( char *)&temp_string, g_setting_list.struct_setting.hour);
   i = strlen(( char *)&temp_string );
   if(i == 0){
   	strcat(( char *)string, "00" );
   	}
   if(i == 1){ 
   	strcat(( char *)string, "0" );
   	strcat(( char *)string, ( char *)&temp_string);
   	}
   if(i == 2){ 
   	strcat(( char *)string, ( char *)&temp_string);
   	}
   strcat(( char *)string, "/" );

//  SendString((U8 *)"\r\nmin = ");
//  send_hex_u8(g_setting_list.struct_setting.min);
   memset( temp_string, 0, 3 );
   date_convert_U8(( char *)&temp_string, g_setting_list.struct_setting.min);
   i = strlen(( char *)&temp_string );
   if(i == 0){
   	strcat(( char *)string, "00" );
   	}
   if(i == 1){ 
   	strcat(( char *)string, "0" );
   	strcat(( char *)string, ( char *)&temp_string);
   	}
   if(i == 2){ 
   	strcat(( char *)string, ( char *)&temp_string);
   	}
#endif   
//   strcat(( char *)string, NULL );
	string[16] = '\0';

//   SendString((U8 *)"\r\nstring = ");
//   SendString(string);
   lcd_goto_xy( 70, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts( string );
//*/   
}

#pragma CODE = SYSTEM_POWER_OFF_CODE

void system_power_off( void )
{
   Power_set_poweroff_activated();
   
   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_screen();
   //Display off
   lcd_turn_off();
   // Reset the OLED controller
   Lcd_reset_on();
   
   SDKLoadCode( POWER_TEMP_CODE00 );
   power_system_off( TRUE );
}

#endif   // _power_task_c_
