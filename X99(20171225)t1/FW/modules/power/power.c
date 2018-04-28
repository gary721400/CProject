//! @file power.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the player management routines
//!
//! @version 1.22 snd3-dvk-1_9_5 $Id: power.c,v 1.22 2007/05/25 14:26:37 sguyon Exp $
//!
//! @configuration
//!  This module must be configured in conf_power.h header file
//!
//! @todo
//!  Microphone bias management when microphone input is selected
//!  Codec information BR, FS, Channel reporting
//!
//! @bug
#ifndef _power_c_
#define _power_c_


//_____  I N C L U D E S ___________________________________________________

#include "lib_mcu\compiler.h"                          // System definition
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_mcu\kbd\keypad_drv.h"          // Keypad driver definition
#include "modules\power\power.h"             // Power module definition
#include "lib_mcu\timer\timer_drv.h"
#include "conf\conf_oid.h"
#include "mmi\shared\com_appli.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#if( OID == ENABLE )
#include "drivers\oid\oid.h"
#endif
#include "sio\com.h"
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "mmi\shared\com_var.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "adc.h"


#pragma DATA = POWER_DATA
#pragma CODE = POWER_CODE

ImageDeclareType( POWER_TEMP_CODE01 );
ImageDeclareType(SYSCLK_TEMP_CODE01);

//_____ M A C R O S ________________________________________________________

//_____ P R I V A T E    D E C L A R A T I O N _____________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________
//                     Bool  b_bat_charge_on = FALSE;
//_MEM_TYPE_MEDSLOW_   U8    bat_charge_counter = 0;
_MEM_TYPE_MEDSLOW_   U8    bat_counter/* = 0*/;

                     Bool  b_card_state;
					 Bool  b_flag_rtc;

#if( EARPHONE_DET == ENABLE )
                     Bool  g_b_earphone_present;
#endif   //#if( EARPHONE_DET == ENABLE )

                     //Bool  g_b_battery_low;

_MEM_TYPE_MEDSLOW_   U32   u32_op_delay_off_time/* = 0*/;

//_MEM_TYPE_MEDSLOW_   U32   u32_op_delay_off_time/* = 0*/;

                     Bool  g_b_set_led /*= FALSE*/;

_MEM_TYPE_MEDSLOW_   U32   green_led_timer;
_MEM_TYPE_MEDSLOW_   U32   red_led_timer;

//_MEM_TYPE_MEDSLOW_   U8    mmi_file_flag = 0;
_MEM_TYPE_SLOW_ 	U32 MSCOUNTER_5 = 0;

#if (!defined ASM_INCLUDE)
                     Bool  g_b_autopoweroff_forbidden;   // Modified by tasks except to mmi manager task
                     Bool  g_b_poweroff_activated;
_MEM_TYPE_MEDSLOW_   U32   g_autopoweroff_timer;
_MEM_TYPE_MEDSLOW_   U32   g_system_clk_timer;
#endif

//_MEM_TYPE_MEDSLOW_   U16   battery_value;

extern               Bool  g_b_recorder_on;

void  program_reset  ( void );

//_____ F U N C T I O N S __________________________________________________
#if 0
//! Power supply switch on
//!
void power_system_on(void)
{
}
#endif


#pragma CODE = POWER_TEMP_CODE00

//! Power supply switch off
//!
void power_system_off( Bool b_power_off )
{
   U16 bat_val,temp_val;
   audio_off();
#if( OID == ENABLE )
   turn_off_oid();
   pen_disable();
   oid_power_off();
   oid_dc_dc_off();
#endif

   //CLASS_D_POP_CTL = 0;
   //CLASS_D_PWM_CTL = 0;
   green_led_off();
   red_led_off();
   op_disable();
   speak_power_off();
   //Bat_disable_charge();
   EXT_PWR_CTL_OFF();
   EXT_DC_PWR_OFF();
   PWR_OFF();
   SendString((U8 *)"\r\npower off");
   if( !b_power_off )
   {
      SendString((U8 *)"\r\nrestart");
      //P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
      //(*(void(*)())0)();
      //RESET_SYS();
   }
   temp_val = 0;
   while(1){
   	delay1ms(100);
   	bat_val = Get_ADCValue( 3 );
	SendString((U8 *)"\r\nadc3=");send_hex_u16(bat_val);
#if 1
if((bat_val <= 0x03A0)){
		temp_val = 0;
		}
	else{
		temp_val ++;
		if(temp_val >= 5)break;
		}

#else
	if((bat_val >= (POWER_DATA - Begin_Offset))&&(bat_val <= (POWER_DATA + Begin_Offset))){
		temp_val = 0;
		}
	else{
		temp_val ++;
		if(temp_val >= 5)break;
		}
#endif		
   	}
   SendString((U8 *)"power key release\r\n");
   RESET_SYS();
   while( 1 );
}


#pragma CODE = POWER_CODE
//test usb port is linking or not

Bool Pow_is_vbus_present(void)
{
	if(adc2_value>100){
	b_readySysUP = TRUE;
	return TRUE;
	}
	return FALSE;
}


//! power_forbid_autopoweroff
//!
//! Forbid the auto power off by the system part
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return U8 : voltage level 0x00 -> 0x10
//!
void power_forbid_autopoweroff(void)
{
#if (POWER_OFF_AUTO == ENABLE)
   g_b_autopoweroff_forbidden = TRUE;
#endif
   clk_stop_timer();
}

//! power_authorize_autopoweroff
//!
//! Autorizes the auto power off by the system part
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return U8 : voltage level 0x00 -> 0x10
//!
void power_authorize_autopoweroff(void)
{
#if (POWER_OFF_AUTO == ENABLE)
   g_b_autopoweroff_forbidden = FALSE;
   power_rearm_autopoweroff_delay();
#endif
    clk_start_timer();
}

#if (POWER_OFF_AUTO == ENABLE)
//! power_rearm_autopoweroff_delay
//!
//! Rearm the timer to delay the autopoweroff
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @param none
//!
void power_rearm_autopoweroff_delay(void)
{          
#if 0
   U32 delay;
   switch( g_setting_list.poweroff_value )
   {
      case 1:
      delay = TIMER_MIN(2);   // 2 min
      break;
      case 2:
      delay = TIMER_MIN(10);  // 10 min
      break;
      default:
      delay = TIMER_MIN(60);   // To not have poweroff few seconds after the init when no keypressed
      break;
   }
   g_autopoweroff_timer = Timer_set_timer(delay);
#endif
   //g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(3));
   
   if( g_setting_list.struct_setting.poweroff_value )
      g_autopoweroff_timer = Timer_set_timer( TIMER_MIN( g_setting_list.struct_setting.poweroff_value ));
   else
      g_autopoweroff_timer = Timer_set_timer( TIMER_MIN( 60 ));
}
#endif

#if 0
//! power_get_batt_voltage_level
//!
//! To get battery voltage level
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return U8 : voltage level 0x00 -> 0x10
//!
U8 power_get_batt_voltage_level(void)
{
   U8 level;
   U32 timer;

   Mcu_set_sfr_page_power();
   Pow_vbat_power_enable();   // VBPEN =1 ( <=> P1.6 of PCON reg)
   
   // Delay of 1ms required
   timer = Timer_set_timer(TIMER_MS(2));
   while (!Timer_is_timeout(timer));

   Mcu_set_sfr_page_power();
   Pow_vbat_clock_enable();   // VBCEN =1 ( <=> P1.6 of PCON reg)

   // Delay of 16 clk period required, done with the 2 following code lines
   level = 0x00; // Takes 1 cyc -> 12 clk
   level = 0x00; // Takes 1 cyc -> 12 clk

   Pow_vbat_conv_start();            
   while(Pow_is_vbat_not_ready());
   level = VBAT;

   Pow_vbat_switch_off(); 

   if( level > 0x10 )   { level = 0; }
   return(level);
}
#endif

//! power_activate_batt_charging
//!
//! To activate the battery charging
//!
//! @param none
//! @return none
//!
void power_activate_batt_charging(void)
{
   // Test the presence or not of the Vbus (USB power)
   if( !power_is_vbus_present() )
      return;
   
   // Enable the charging if battery is not full
   Bat_enable_charge();
   bat_charge_counter = 0;
   b_bat_charge_on = TRUE;
   mail_send_event(EVT_BATTERY_CHARGING, TRUE);
   if( g_select_appli == MSTORAGE_APPLI )
   {
      red_led_on();
      green_led_on();
   }
   green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
}

//! power_stop_batt_charging
//!
//! To activate the battery charging
//!
//! @param none
//! @return none
//!
void power_stop_batt_charging(void)
{
   Bat_disable_charge();
   b_bat_charge_on = FALSE;
   mail_send_event(EVT_BATTERY_CHARGING, FALSE);
   if( !g_b_recorder_on )
   {
      red_led_off();
      green_led_on();
      green_led_start_timer( GREEN_LED_ON_NORMAL_TIME );
   }
   //SendString((U8 *)"stop charging\r\n");
}

//! power_is_vbus_present
//!
//! @return VBUS level
//!
/*Bool power_is_vbus_present( void )
{
   Bool b_value;
   b_value = Pow_is_vbus_present();
   return b_value;
}*/

Bool earphone_is_present(void)
{
	if(g_b_V1Hard_Flag)return HP_V1_FLAG();
		return HP_V2_FLAG();
}

void RESET_SYS( void )
{
   //SDKLoadCode( POWER_TEMP_CODE01 );
   //program_reset();
    RTCWDT = 0xBFFFu;
   
}
/*
Bool power_key_is_press( void )
{
   delay1ms( 15 );
   if( Get_ADCValue( 2 )> 80 )
      return FALSE;
   return TRUE;
}
*/

#pragma CODE = SYSCLK_TEMP_CODE01
void syscls_set(Bool flag)
{
   if(flag){
	   if( g_b_system_clk_low ){
	   /*
          SystemClock( FEQ_24M );
          SystemClock( SYSTEM_CLOCK );
          SPIF_Set_Clock( SPI_30M );
          //*/
          g_b_system_clk_low = FALSE;
#ifdef TX_DEBUG
    	 SendString((U8 *)"\r\nsys up!");
#endif	
	   }
   	}
   else{
   	  if( !g_b_system_clk_low ){
#ifdef TX_DEBUG
	     SendString((U8 *)"\r\nsys low!");
#endif	
   /*
      SPIF_Set_Clock( SPI_3M );
      __asm
      {
         BCLR MODCON.12
         REP #45
   		 NOP
      }
  	//*/
      g_b_system_clk_low = TRUE;
	 }
   	}
  // g_system_clk_timer = Timer_set_timer( SYS_CLK_LOW_TIME );
}
#pragma CODE = POWER_CODE
void SysClkUpgrade( void )
{
      /*
	  SDKLoadCode(SYSCLK_TEMP_CODE01);
	  syscls_set(TRUE);
	  //*/
}


//------------------------------------------------------------------------------
void SysClkDowngrade( void )
{
    /*
 	SDKLoadCode(SYSCLK_TEMP_CODE01);
    syscls_set(FALSE);
    //*/
}


void clkup_time(Bool flag_on)
{
/*
	SysClkUpgrade();
	if(flag_on){
		clk_start_timer();
		}
	else{
		clk_stop_timer();
		}
//*/		
}


void  clk_time_dowith( void )
{
/*
	if(clk_is_timer_timeout()){
		clk_stop_timer();
		SysClkDowngrade();
		}
//*/		
}



#pragma CODE = POWER_TEMP_CODE01
void program_reset( void )
{
   if( power_is_vbus_present())
      CommonLoadCode( C_STARTUP );
   CommonLoadCode( C_BOOT );
}

#endif   // _power_c_
