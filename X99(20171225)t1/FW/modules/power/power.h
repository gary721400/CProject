//! @file power.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the power management driver definitions.
//!
//! @version 1.13 snd3-dvk-1_9_5 $Id: power.h,v 1.13 2007/05/25 14:26:38 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _POWER_H_
#define _POWER_H_

#include "conf_sio.h"
//#include "KBDM9_DDK.H"
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _power_c_)
   #define _GLOBEXT_ extern
#else                                        
   #define _GLOBEXT_ extern
#endif

#define  Begin_Offset	(0x0008)
#define  Level_Offset	(Begin_Offset+Begin_Offset)
#define  Power_Offset	(0x0349 - 0x0307)

#if 1
#define SW2_MIN		(Begin_Offset - Begin_Offset)
#define SW5_MIN		(0x00D1 - 0x0010)
#define SW8_MIN		(0x0112 - 0x0010)
#define SW1_MIN		(0x0219 - 0x0010)	
//#define SW4_MIN		(0x035A - Begin_Offset)
#define SW6_MIN		(0x0294 - 0x0010)
#define SW7_MIN		(0x0388 - 0x0010)
//#define SW3_MIN		(0x0349 - 0x0010)
#define SW9_MIN		()
#define SWA_MIN		()
#define SWB_MIN		()
#define SWC_MIN		()
#define SWD_MIN		()

#define SW2_MAX		(Begin_Offset + Begin_Offset)
#define SW5_MAX		(0x00D1 + 0x0010)
#define SW8_MAX		(0x0112 + 0x0010)
#define SW1_MAX		(0x0219 + 0x0010)	
//#define SW4_MAX		(0x035A + 0x0010)
#define SW6_MAX		(0x0294 + 0x0010)
#define SW7_MAX		(0x0388 + 0x0010)
//#define SW3_MAX		(0x0349 + Begin_Offset)  //2e4
#define SW9_MAX		()
#define SWA_MAX		()
#define SWB_MAX		()
#define SWC_MAX		()
#define SWD_MAX		()



#define SW3_V1_MIN		(0x0349 - 0x0010)
#define SW3_V1_MAX		(0x0349 + Begin_Offset)  //2e4

#define SW4_V1_MIN		(0x035A - Begin_Offset)
#define SW4_V1_MAX		(0x035A + 0x0010)


#define SW3_V2_MIN		(0x0349 - 0x0010 - Power_Offset) //2F7
//#define SW3_V2_MAX		(0x0349 + 0x0010 - Power_Offset)  //2e4
#define SW3_V2_MAX		(0x0349 + 0x0010 - 0x22)  //2e4

#define SW4_V2_MIN		(0x035A - 0x0010)       //34A
#define SW4_V2_MAX		(0x035A + 0x0010)       //36A






#else
#define SW2_MIN		(Begin_Offset - Begin_Offset)
#define SW5_MIN		(0x00D1 - 0x0010)
#define SW8_MIN		(0x0112 - 0x0010)
#define SW1_MIN		(0x0219 - 0x0010)	
#define SW4_MIN		(0x035A - 0x0010)
#define SW6_MIN		(0x0294 - 0x0010)
#define SW7_MIN		(0x0388 - 0x0010)
#define SW3_MIN		(0x0307 - 0x0010)
#define SW9_MIN		()
#define SWA_MIN		()
#define SWB_MIN		()
#define SWC_MIN		()
#define SWD_MIN		()

#define SW2_MAX		(Begin_Offset + Begin_Offset)
#define SW5_MAX		(0x00D1 + 0x0010)
#define SW8_MAX		(0x0112 + 0x0010)
#define SW1_MAX		(0x0219 + 0x0010)	
#define SW4_MAX		(0x035A + 0x0010)
#define SW6_MAX		(0x0294 + 0x0010)
#define SW7_MAX		(0x0388 + 0x0010)
#define SW3_MAX		(0x0307 + 0x0010)  //2e4
#define SW9_MAX		()
#define SWA_MAX		()
#define SWB_MAX		()
#define SWC_MAX		()
#define SWD_MAX		()

#endif


#define POWER_DATA		SW8_DATA
#define VOL_INC_DATA	SW4_DATA



#define SYSTEM_CLOCK			FEQ_60M

//#define RESET_SYS()              __asm( "reset" );
void  RESET_SYS   ( void );

#define  SYS_CLK_LOW_TIME        ( TIMER_MS( 3000 ))

_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   u32_vbus_time;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   u32_sd_card_time;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   u32_earphone_time;

#define USB_DET_DIR              //( _PCDIR_b0 + 10 )
#define USB_DET_PH               //( _PCPH_b0 + 10 )
#define USB_DET                  //( _PCID_b0 + 10 )
//#define Pow_is_vbus_present()    (Get_ADCValue(2)>100)//( _bit_get( USB_DET ))

//_____ M A C R O S ________________________________________________________
//! Enable the battery charging
//  EN# input is connected to P2.0
#define Bat_enable_charge()         //( P2DIR |= BIT0, P2_0 = 1 ) 

//! Check if battery end of charge status
//! return: TRUE:  end of charge reached
//!         FALSE: battery under charge
//  CHG# output is connected to P2.1
#if 1
#define BAT_nEOC_DIR                ( _PBDIR_b0 + 9 )
#define BAT_nEOC_IN                 ( _PBID_b0 + 9 )
#define BAT_nEOC_PH                 ( _PBPH_b0 + 9 )
#else
#define BAT_nEOC_DIR                ( _PBDIR_b0 + 14 )
#define BAT_nEOC_IN                 ( _PBID_b0 + 14 )
#define BAT_nEOC_PH                 ( _PBPH_b0 + 14 )
#endif
#define Bat_is_fully_charged()      ( _bit_get( BAT_nEOC_IN ))

//! Disable the battery charging
//  EN# input is connected to P2.0
#define Bat_disable_charge()        //( P2DIR |= BIT0, P2_0 = 0 )

//! Check if battery charge is on 
//  EN# input is connected to P2.0
//#define Bat_is_charge_on()          ( P2_0 == 1 )
#define Bat_is_charge_on()          ( b_bat_charge_on == TRUE )

/*_GLOBEXT_ Bool b_bat_charge_on
#if 0//def _power_c_
   = FALSE
#endif
;*/
/*_GLOBEXT_ U8 _MEM_TYPE_MEDSLOW_ bat_charge_counter
#if 0//def _power_c_
   = 0
#endif
;*/
_GLOBEXT_ U8 _MEM_TYPE_MEDSLOW_  bat_counter;
#define  bat_charge_counter      bat_counter
#define  battery_low_counter     bat_counter

#define POWER_OFF_AUTO        ENABLE//DISABLE//
/*
_GLOBEXT_   Bool  g_system_clock_is_30MHZ
#ifdef _power_c_
   = FALSE
#endif
;
*/
//X99测试，有卡为0，没卡为1
#define SD_DET_PIE                     ( _PBPIE_b0 + 11 )
#define SD_DET_PIP                     ( _PBPIP_b0 + 11 )
#define SD_DET_PIF                     ( _PBPIF_b0 + 11 )
#define sd_det_enable()                ( _bitSET( SD_DET_PIE ))
#define sd_det_disable()               ( _bitCLR( SD_DET_PIE ))

#define sd_det_rise()                  ( _bitSET( SD_DET_PIP ))
#define sd_det_fall()                  ( _bitCLR( SD_DET_PIP ))

#define sd_det_pif()                   ( _bit_get( SD_DET_PIF ))
#define clr_sd_det_pif()               ( _bitCLR( SD_DET_PIF ))

#define SD_DET_DIR                     ( _PBDIR_b0 + 11 )
#define SD_DET_IN                      ( _PBID_b0 + 11 )
#define SD_DET_PH                      ( _PBPH_b0 + 11 )
#define Mmc_card_present()             ( _bit_get( SD_DET_IN ))
_GLOBEXT_ Bool b_card_state;

//! --- Battery scan period ---
//! Value in seconds
#define POWER_BATTERY_SCAN_PERIOD   5

#define PWRON_DIR             ( _PBDIR_b0 + 10 )
#define PWRON                 ( _PBOD_b0 + 10 )
#define PWR_ON()              ( _bitSET( PWRON ))
#define PWR_OFF()             ( _bitCLR( PWRON ))

#define PWR_CTRL              //P1_3
#define EXT_DC_PWR_ON()       //( P1DIR |= BIT3, PWR_CTRL = 1 )
#define EXT_DC_PWR_OFF()      //( P1DIR |= BIT3, PWR_CTRL = 0 )

//#define power_key_is_press()  (Get_ADCValue( 3 ) < 80)

#define key_power_is_pressed()   TRUE//( !_bit_get( KBD_PWR ))//

#define EXT_PWR_CTL_ON()      //( /*P4DIR |= BIT1, */P4 |= BIT1 )
#define EXT_PWR_CTL_OFF()     //( /*P4DIR |= BIT1, */P4 &= ~BIT1 )

//#define BAT_LOW_DET           ( P4 & BIT4 )

//#define AUDIO_CTL             //P1_6
#define audio_on()            //( P4 &= ~BIT0 )//( AUDIO_CTL = 0 )//
#define audio_off()           //( P4 |= BIT0 )//( AUDIO_CTL = 1 )//

#if( EARPHONE_DET == ENABLE )
#if 0
#define HP_DET_DIR                     ( _PBDIR_b0 + 0 )
#define HP_DET_IN                      ( _PBID_b0 + 0 )
#define HP_DET_PH                      ( _PBPH_b0 + 0 )
//#else
#define HP_DET_DIR                     ( _PBDIR_b0 + 5 )
#define HP_DET_IN                      ( _PBID_b0 + 5 )
#define HP_DET_PH                      ( _PBPH_b0 + 5 )
#endif
//#define earphone_is_present()          ( !_bit_get( HP_DET_IN ))
_GLOBEXT_ Bool g_b_earphone_present;

#define OP_CTL_DIR                     ( _PCDIR_b0 + 1 )
#define OP_CTL                         ( _PCOD_b0 + 1 )
#define op_enable()                    ( _bitSET( OP_CTL ))
#define op_disable()                   ( _bitCLR( OP_CTL ))

#define EX_RC_EN_DIR                   ( _PBDIR_b0 + 13 )
#define EX_RC_EN                       ( _PBOD_b0 + 13 )
#define exrcen_enable()                //( _bitSET( EX_RC_EN ))
#define exrcen_disable()               //( _bitCLR( EX_RC_EN ))



#define CHECK_DET_DIR                     ( _PBDIR_b0 + 13 )
#define CHECK_DET_OUT                     ( _PBOD_b0 + 13 )

#define CHECK_DET_IN                      ( _PBID_b0 + 13 )
#define CHECK_DET_PH                      ( _PBPH_b0 + 13 )

#define CHECK_READ_FLAG()                 ( _bit_get( CHECK_DET_IN ))


#define HP_V1_DET_DIR                     ( _PBDIR_b0 + 13 )
#define HP_V1_DET_IN                      ( _PBID_b0 + 13 )
#define HP_V1_DET_PH                      ( _PBPH_b0 + 13 )
#define HP_V1_FLAG()                 	  ( !_bit_get( HP_V1_DET_IN ))


#define HP_V2_DET_DIR                     ( _PBDIR_b0 + 5 )
#define HP_V2_DET_IN                      ( _PBID_b0 + 5 )
#define HP_V2_DET_PH                      ( _PBPH_b0 + 5 )
#define HP_V2_FLAG()                 	  ( !_bit_get( HP_V2_DET_IN ))



#define speak_power_on()
#define speak_power_off()
#else
#define earphone_input()
#define earphone_is_present()

#define speak_power_on()
#define speak_power_off()

#define op_enable()
#define op_disable()
#endif   //#if( EARPHONE_DET == ENABLE )

//_GLOBEXT_                     Bool  g_b_battery_low;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_ U32 u32_op_delay_off_time
#if 0//def _power_c_
   = 0
#endif
;

_GLOBEXT_ Bool g_b_set_led
#if 0//def _power_c_
   = FALSE
#endif
;

#define GREEN_LED_DIR         ( _PCDIR_b0 + 8 )
#define GREEN_LED             ( _PCOD_b0 + 8 )
//#define YELLOW_LED            P2_1
#define RED_LED_DIR           //( _PBDIR_b0 + 5 )
#define RED_LED               //( _PBOD_b0 + 5 )

#define GREEN_LED_ON()        ( _bitCLR( GREEN_LED ))
#define GREEN_LED_OFF()       ( _bitSET( GREEN_LED ))
#define GREEN_LED_SHINE()     ( _bitXOR( GREEN_LED, 1 ))

#define RED_LED_ON()          //( _bitCLR( RED_LED ))
#define RED_LED_OFF()         //( _bitSET( RED_LED ))
#define RED_LED_SHINE()       //( _bitXOR( RED_LED, 1 ))

//#define yellow_led_on()       ( P2DIR |= BIT1, YELLOW_LED = 0 )
//#define yellow_led_off()      ( P2DIR |= BIT1, YELLOW_LED = 1 )

#define green_led_on()        GREEN_LED_ON()
#define green_led_off()       GREEN_LED_OFF()
#define green_led_flash()     GREEN_LED_SHINE()
#define red_led_on()          RED_LED_ON()
#define red_led_off()         RED_LED_OFF()
#define red_led_flash()       RED_LED_SHINE()
/*
_GLOBEXT_ U32 _MEM_TYPE_SLOW_ usb_sd_timer
#ifdef _power_c_
   = 0
#endif
;
*/
#define ACCESS_LED_TOG()/*                                 \
{                                                        \
   GREEN_LED_SHINE();                                    \
   usb_sd_timer = Timer_set_timer( TIMER_MS( 2000 ));    \
}

#define usb_sd_timer_timeout()         ( usb_sd_timer && Timer_is_timeout( usb_sd_timer ))
#define usb_sd_timer_stop()            ( usb_sd_timer = 0 )
#define usb_sd_timer_is_stopped()      ( usb_sd_timer == 0 )
*/
#define green_led_is_off()    ( _bit_get( GREEN_LED ))
#define red_led_is_off()      TRUE//( _bit_get( RED_LED ))
//#define yellow_led_is_off()   ( YELLOW_LED == 1 )

_GLOBEXT_ U32 _MEM_TYPE_MEDSLOW_ green_led_timer;
#define GREEN_LED_ON_NORMAL_TIME       80
#define GREEN_LED_OFF_NORMAL_TIME      900
#define GREEN_LED_ON_READ_TIME         20
#define GREEN_LED_OFF_READ_TIME        50
#define green_led_start_timer( value ) ( green_led_timer = Timer_set_timer( TIMER_MS( value )))
#define green_led_stop_timer()         ( green_led_timer = 0 )
#define green_led_is_timer_timeout()   ( green_led_timer && Timer_is_timeout( green_led_timer ))
#define green_led_is_timer_stopped()   ( green_led_timer == 0 )

_GLOBEXT_ U32 _MEM_TYPE_MEDSLOW_ red_led_timer;
#define RED_LED_ON_BATTERY_LEVEL_LOW_TIME    200
#define RED_LED_OFF_BATTERY_LEVEL_LOW_TIME   300
#define red_led_start_timer( value )         ( red_led_timer = Timer_set_timer( TIMER_MS( value )))
#define red_led_stop_timer()                 ( red_led_timer = 0 )
#define red_led_is_timer_timeout()           ( red_led_timer && Timer_is_timeout( red_led_timer ))
#define red_led_is_timer_stopped()           ( red_led_timer == 0)


#define clk_start_timer( )         			( g_system_clk_timer = Timer_set_timer( SYS_CLK_LOW_TIME ))
#define clk_stop_timer()                 	( g_system_clk_timer = 0 )
#define clk_is_timer_timeout()           	( g_system_clk_timer && Timer_is_timeout( g_system_clk_timer ))
#define clk_is_timer_stopped()           	( g_system_clk_timer == 0)


#define AP4_NO_FILE_FLAG               0x01u
#define MP3_NO_FILE_FLAG               0x02u
#define APF_NO_FILE_FLAG               0x04u
#define ADB_NO_FILE_FLAG               0x08u
/*_GLOBEXT_ U8 _MEM_TYPE_MEDSLOW_ mmi_file_flag
#if 0//def _power_c_
   = 0
#endif
;*/

#define Power_init_autopoweroff()            \
{                                            \
   g_b_autopoweroff_forbidden = FALSE;       \
   power_rearm_autopoweroff_delay();         \
}


#define Power_is_poweroff_activated()     ( g_b_poweroff_activated == TRUE )
#define Power_set_poweroff_activated()    ( g_b_poweroff_activated = TRUE )
#define Power_unset_poweroff_activated()  ( g_b_poweroff_activated = FALSE )


//_____ D E C L A R A T I O N S ____________________________________________
#if (!defined ASM_INCLUDE)
_GLOBEXT_                        Bool  g_b_autopoweroff_forbidden;   // Modified by tasks except to mmi manager task
_GLOBEXT_                        Bool  g_b_poweroff_activated;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   g_autopoweroff_timer;

_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   g_system_clk_timer;

#endif
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   MSCOUNTER_5;
_GLOBEXT_   Bool  b_flag_rtc;

/*
3.8v	0x2bc	0x76
3.7v	0x2ab	0x72
3.6v	0x295	0x6f
3.5v	0x284	0x6c
3.4v	0x274	0x69
3.3v	0x26b	0x66
3.2v	0x26c	0x63

*/
//#define  LVT_VALUE_BEGIN         0x0195    //2.2v
#define  LVT_VALUE               0x6b//0x0216      //3.4v

#define  BAT_FULL                0x78//0x0248      //3.8v
#define  BAT_MED                 0x75//0x023A      //3.7v
#define  BAT_LOW                 0x73//0x022C      //3.6V
#define  BAT_V_LOW               0x6f//0x021D      //3.5v
#define  BVT_VALUE               0x6f//0x021D      //3.5v

//_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U16   battery_value;
//------------------------------------------------------------------


//_____ D E C L A R A T I O N S ____________________________________________
#if (!defined ASM_INCLUDE)
void  power_system_on                  ( void );
//void  power_system_off                 ( void );
void power_system_off                  ( Bool b_power_off );
U8    power_get_batt_voltage_level     ( void );
void  power_forbid_autopoweroff        ( void );
void  power_authorize_autopoweroff     ( void );
void  power_rearm_autopoweroff_delay   (void);
void  power_stop_batt_charging         ( void );
void  power_activate_batt_charging     ( void );

void  SysClkUpgrade                    ( void );
void  SysClkDowngrade                  ( void );
void  clkup_time(Bool flag_on);
void  clk_time_dowith( void );

Bool earphone_is_present(void);

//Bool  power_key_is_press			   ( void );
Bool  Pow_is_vbus_present              ( void );
#define power_is_vbus_present()        Pow_is_vbus_present()
#endif

#endif  // _POWER_H_
