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

#include "conf_kb.h"
#include "compiler.h"
#include <KBDM9_DDK.H>
#include <SFR.H>
#include "GPIO_bitref.H"
//#include "KBDM952.H"
//#include "timer_drv.h"

//_____ M A C R O S ________________________________________________________

#define  Begin_Offset	(0x0020)
#define  Level_Offset	(Begin_Offset+Begin_Offset)

#define SW0_DATA      	//0x00
#define SW1_DATA      	(0x0353)
#define SW2_DATA      	(0x02FB)
#define SW3_DATA      	(0x0291)
#define SW4_DATA    	(0x0000)
#define SW5_DATA       	//(0x01AB)
#define SW6_DATA	    //(0x010C)
#define SW7_DATA        //(Begin_Offset - Begin_Offset)//0x0005
#define SW8_DATA	    //0x08
#define SW9_DATA       	//0x09
#define SW10_DATA	   	//0x0A
#define SW11_DATA		//0x0B


//#define POWER_DATA	 	SW4_DATA
#define MODE_DATA	 	SW2_DATA
#define VOL_INC_DATA 	SW3_DATA
#define VOL_DEC_DATA	SW4_DATA
/*
#define INTERNAL_VLV_DC_DC             0x00
#define EXTERNAL_VLV_REGULATOR         0x01
#define EXTERNAL_LV_DC_DC              0x02
#define EXTERNAL_LV_REGULATOR          0x03

//KB REG
//----------------------------------------------

#define _PBEN_b0    ((_bitref)(0x420U))
#define _PBDIR_b0   ((_bitref)(0x430U))
#define _PBOD_b0    ((_bitref)(0x440U))
//#define _PBDS_b0    ((_bitref)(0x450U))
#define _PBID_b0    ((_bitref)(0x460U))
#define _PBODN_b0   ((_bitref)(0x470U))
#define _PBPH_b0    ((_bitref)(0x480U))
#define _PBPIE_b0   ((_bitref)(0x490U))
#define _PBPIP_b0   ((_bitref)(0x4A0U))
#define _PBPIF_b0   ((_bitref)(0x4B0U))
#define _PBICR_b0   ((_bitref)(0xD90U))

#define _PCEN_b0    ((_bitref)(0x4C0U))
#define _PCDIR_b0   ((_bitref)(0x4D0U))
#define _PCOD_b0    ((_bitref)(0x4E0U))
#define _PCDS_b0    ((_bitref)(0x4F0U))
#define _PCID_b0    ((_bitref)(0x500U))
#define _PCPH_b0    ((_bitref)(0x510U))
#define _PCPIE_b0   ((_bitref)(0x520U))
//#define _PCPIP_b0   ((_bitref)(0x530U))
#define _PCPIF_b0   ((_bitref)(0x540U))
#define _PCICR_b0   ((_bitref)(0xDA0U))

#define _PDEN_b0    ((_bitref)(0x550U))
#define _PDDIR_b0   ((_bitref)(0x560U))
#define _PDOD_b0    ((_bitref)(0x570U))
#define _PDDS_b0    ((_bitref)(0x580U))
#define _PDID_b0    ((_bitref)(0x590U))
#define _PDPH_b0    ((_bitref)(0x5A0U))

#define _SP1CR_b0   ((_bitref)(0x8A0U))
#define _SP1SR_b0   ((_bitref)(0x8B0U))
#define _SP2CR_b0   ((_bitref)(0x8D0U))
#define _SP2SR_b0   ((_bitref)(0x8E0U))
*/

//_____ M A C R O S ________________________________________________________
#define POWER_OFF_AUTO        ENABLE//DISABLE//

//! --- Battery scan period ---
//! Value in seconds
#define POWER_BATTERY_SCAN_PERIOD   5







#define PWR_CTRL_DIR (_PDDIR_b0+6)
#define PWR_CTRL_OD (_PDOD_b0+6)

#define BLUE_LED_DIR (_PBDIR_b0+4)
#define BLUE_LED_OD (_PBOD_b0+4)

#define RED_LED_DIR     (_PBDIR_b0+5)
#define RED_LED_OD      (_PBOD_b0+5)

#define BLUE2_LED_DIR   // (_PDDIR_b0+6)
#define BLUE2_LED_OD    // (_PDOD_b0+6)

#define RED2_LED_DIR    // (_PDDIR_b0+7)
#define RED2_LED_OD     // (_PDOD_b0+7)

#define CTL_OID_DIR      (_PCDIR_b0+5)
#define CTL_OID_OD       (_PCOD_b0+5)


#define KBD_PWR_DIR	(_PCDIR_b0+9)
#define KBD_PWR_PH	(_PCPH_b0+9)
#define KBD_PWR_ID	(_PCID_b0+9)

#define OP_DET_DIR 			(_PDDIR_b0+5)
#define OP_DET_PH 			(_PDPH_b0+5)
#define OP_DET_OD 			(_PDOD_b0+5)







#define HP_DET_DIR   (_PCDIR_b0+10)
#define HP_DET_PH    (_PCPH_b0+10)
#define HP_DET_ID    (_PCID_b0+10)

#define EX_DET_DIR 			(_PBDIR_b0+8)
#define EX_DET_PH 			(_PBPH_b0+8)
#define EX_DET_OD 			(_PBOD_b0+8)




#define Lidian_EN_DIR	(_PDDIR_b0+7)
#define Lidian_EN_PH	(_PDPH_b0+7)
#define Lidian_EN_OD	(_PDOD_b0+7)
#define Lidian_EN_ID	(_PDID_b0+7)


#if (LI_DIANCHI_DIANYA == ENABLE )
//#define LVT_VALUE_BEGIN 	0x01A9    //2.3v
//#define LVT_VALUE 			0x0195    //2.2v

//#define LVT_VALUE_BEGIN 	0x021D    //3.5v
//#define LVT_VALUE 			0x0216    //3.4v
#define LVT_VALUE_BEGIN 	0x5C // //3.5v    
#define LVT_VALUE 			0x59 //   //3.4v 
#define LVT_TISHI_VALUE 	0X5A////3.43v
#endif
//#ifdef NIEQING_DIANCHI_DIANYA
#if (LI_DIANCHI_DIANYA == DISABLE)
//#define LVT_VALUE_BEGIN 	0x0195    //2.2v
//#define LVT_VALUE 			0x016D    //2.0v
#define LVT_VALUE_BEGIN 	0x47//0x3B//0x47//0x01A9    //2.3v
#define LVT_VALUE 			0X3D//0x39//0X3D//0x016D    //2.0v
#define LVT_TISHI_VALUE 	0X45//0x33//0X45//0x0195    //2.2v
#endif

#define PowerKeepOn()	_bitSET(PWR_CTRL_OD)
#define PowerTurnOff()	_bitCLR(PWR_CTRL_OD)

#define OID_On()		 //_bitSET(CTL_OID_OD)
#define OID_Off()		 //_bitCLR(CTL_OID_OD)

#define SpeakerDET()	 _bit_get(HP_DET_ID)
//#define USB_DET()		_bit_get(USB_DET_ID)
//#define PWD_Key_Press()	  (_bit_get(KBD_PWR_ID))
#define USB_DET()			 (adc2_value>100)
#define PWD_Key_Press()		((adc3_value < (SW1_DATA+Begin_Offset))&&(adc3_value > (SW1_DATA-Begin_Offset)))
//#define Mode_Key_Press()	((adc3_value < (MODE_DATA+Begin_Offset))&&(adc3_value > (MODE_DATA-Begin_Offset)))
#define Vol_Inc_Press()		((adc3_value < (VOL_INC_DATA+Begin_Offset))&&(adc3_value > (VOL_INC_DATA-Begin_Offset)))
#define Vol_Dec_Press()		((adc3_value < (VOL_DEC_DATA+Begin_Offset))&&(adc3_value > (VOL_DEC_DATA-Begin_Offset)))
//#define PWD_Key_Press()		(!_bit_get(KBD_PWR_ID))
//#define PWD_Key_Press()		(adc2_value>100)

#define BlueLed_ON()	    _bitCLR(BLUE_LED_OD)
#define BlueLed_OFF()	    _bitSET(BLUE_LED_OD)     //_bitSET(BLUE_LED_OD)
#define BlueLed_FLASH()		_bitXOR(BLUE_LED_OD,1)   //_bitXOR(BLUE_LED_OD,1)

#define RedLed_ON()	    	 _bitCLR(RED_LED_OD)//_bitCLR(RED_LED_OD)
#define RedLed_OFF()	     _bitSET(RED_LED_OD)//_bitSET(RED_LED_OD)
#define RedLed_FLASH()		 _bitXOR(RED_LED_OD,1)//_bitXOR(RED_LED_OD,1)


#define BlueLed2_ON()	    //_bitCLR(BLUE2_LED_OD)
#define BlueLed2_OFF()	    //_bitSET(BLUE2_LED_OD)
#define BlueLed2_FLASH()	//_bitXOR(BLUE2_LED_OD,1)

#define RedLed2_ON()	    //_bitCLR(RED2_LED_OD)
#define RedLed2_OFF()	    //_bitSET(RED2_LED_OD)
#define RedLed2_FLASH()		//_bitXOR(RED2_LED_OD,1)









#define set_ex()			_bitSET(EX_DET_OD)
#define clr_ex()			_bitCLR(EX_DET_OD)



#define OP_OPEN()		_bitSET(OP_DET_OD)
#define OP_CLOSE()		_bitCLR(OP_DET_OD)


#define Lidian_ok()	    (_bit_get(Lidian_EN_ID))

#define _USBCON_b0 		((_bitref)0x410)

	
#define SD2_DET_PIN						9


#define SD_DET_PIE                     ( _PBPIE_b0 + SD2_DET_PIN )
#define SD_DET_PIP                     ( _PBPIP_b0 + SD2_DET_PIN )
#define SD_DET_PIF                     ( _PBPIF_b0 + SD2_DET_PIN )
#define sd_det_enable()                ( _bitSET( SD_DET_PIE ))
#define sd_det_disable()               ( _bitCLR( SD_DET_PIE ))

#define sd_det_rise()                  ( _bitSET( SD_DET_PIP ))
#define sd_det_fall()                  ( _bitCLR( SD_DET_PIP ))

#define sd_det_pif()                   ( _bit_get( SD_DET_PIF ))
#define clr_sd_det_pif()               ( _bitCLR( SD_DET_PIF ))

#define SD_DET_DIR                     ( _PBDIR_b0 + SD2_DET_PIN )
#define SD_DET_IN                      ( _PBID_b0 + SD2_DET_PIN )
#define SD_DET_PH                      ( _PBPH_b0 + SD2_DET_PIN )
#define Mmc_card_present()             ( _bit_get( SD_DET_IN ))
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#define OID_PWR_OFF()      OID_Off()
#define OID_PWR_ON()       OID_On()


#define PWR_ON()           PowerKeepOn()
#define PWR_OFF()          PowerTurnOff()


//#define RESET_SYS()        __asm("reset");

//#define battery_power_low()     (Get_ADCValue(0) < LVT_VALUE)
//#define battery_power_normal()  (Get_ADCValue(0) > LVT_VALUE)
/*
enum{
	HEADER_PHONE = 0,
	SPEAKER	= 1
};
*/
#ifdef NIEQING_DIANCHI
//***************************************
// CHG_EN 充电使能 CHG濉CHG_STA 充电状态
#define Charge_EN_DIR	(_PBDIR_b0+12)
#define Charge_EN_OD	(_PBOD_b0+12)

#define Charge_State_DIR	(_PBDIR_b0+9)
#define Charge_State_PH	    (_PBPH_b0+9)
#define Charge_State_ID	    (_PBID_b0+9)
#define Charge_State_OD	    (_PBOD_b0+9)

#define Charge_EN_OUT()	     _bitCLR(Charge_EN_DIR)
#define Charge_STA_IN()	     _bitSET(Charge_State_DIR)

//#define Charge_STA_IN()	     _bitSET(Charge_State_DIR)
#define Charge_State()       _bit_get(Charge_State_ID)
//#define Charge_State()       _bit_get(_PBID_b0+12)
#define Charge_NO_PH()       _bitCLR(Charge_State_PH)



#define Charge_Enable()      _bitSET(Charge_EN_OD)
#define Charge_Disable()     _bitCLR(Charge_EN_OD)

#define charge_state_output()		_bitCLR(Charge_State_DIR); // Set _charge_state_dir with Output;
#define charge_state_islow()		_bitCLR(Charge_State_OD); // Set _charge_state_od is low;
//****************************************

//extern _MEM_TYPE_SLOW_ U32 nieqing_time;
//extern  _MEM_TYPE_SLOW_ U8  nieqing_count;

//#define nieqing_start_timer( value ) ( nieqing_time = Timer_set_timer( TIMER_MIN( value )))
//#define nieqing_stop_timer()         ( nieqing_time = 0 )
//#define nieqing_is_timer_timeout()   ( nieqing_time && Timer_is_timeout( nieqing_time ))

#endif


//#define Lidian_EN_DIR	(_PBDIR_b0+12)
//#define Lidian_EN_OD	(_PBOD_b0+12)
//#define Lidian_EN_ID	(_PBID_b0+12)



/*
_GLOBEXT_ U32 _MEM_TYPE_SLOW_ led_2_timer;	
#define lED_2_INDEX_TIME                     20
#define LED_2_PLAY_TIME    				     900
#define led_2_start_timer( value )         ( led_2_timer = Timer_set_timer( TIMER_MS( value )))
#define led_2_stop_timer()                 ( led_2_timer = 0 )
#define led_2_is_timer_timeout()           ( led_2_timer && Timer_is_timeout( led_2_timer ))
#define led_2_is_timer_stopped()           ( led_2_timer == 0)
*/

#define AP4_NO_FILE_FLAG               0x01
#define MP3_NO_FILE_FLAG               0x02
#define APF_NO_FILE_FLAG               0x04
#define ADB_NO_FILE_FLAG               0x08

//extern U8 _MEM_TYPE_SLOW_ mmi_file_flag;
/*
#define Power_init_autopoweroff()            \
{                                            \
   g_b_autopoweroff_forbidden = FALSE;       \
   power_rearm_autopoweroff_delay();         \
}
*/

#define Power_is_poweroff_activated()     ( g_b_poweroff_activated == TRUE )
#define Power_set_poweroff_activated()    ( g_b_poweroff_activated = TRUE )
#define Power_unset_poweroff_activated()  ( g_b_poweroff_activated = FALSE )


//_____ D E C L A R A T I O N S ____________________________________________

//extern                     Bool  g_b_autopoweroff_forbidden;   // Modified by tasks except to mmi manager task
//extern                     Bool  g_b_poweroff_activated;
//extern   _MEM_TYPE_SLOW_   U32   g_autopoweroff_timer;




//_____ D E C L A R A T I O N S ____________________________________________
#if (!defined ASM_INCLUDE)
void  power_system_on                  ( void );
//void power_system_off                  ( void );
U8    power_get_batt_voltage_level     ( void );

void  power_stop_batt_charging         ( void );
void  power_activate_batt_charging     ( void );
//Bool  power_is_vbus_present            ( void );
#define power_is_vbus_present()        FALSE
#endif

#endif  // _POWER_H_
