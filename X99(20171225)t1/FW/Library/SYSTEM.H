/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: SYSTEM.H
 * Author		: Nick
 * Create Date	: December 2, 2008
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <KBDM9_DDK.H>
#include "GPIO_Bitref.H"

#define HP_DET_DIR (_PCDIR_b0+1)
#define HP_DET_PH (_PCPH_b0+1)
#define HP_DET_ID (_PCID_b0+1)

#define PWR_CTRL_DIR (_PBDIR_b0+10)
#define PWR_CTRL_OD (_PBOD_b0+10)

#define BLUE_LED_DIR (_PBDIR_b0+13)
#define BLUE_LED_OD (_PBOD_b0+13)

#define BlueLed_ON()	    _bitCLR(BLUE_LED_OD)
#define BlueLed_OFF()	    _bitSET(BLUE_LED_OD)
#define BlueLed_FLASH()		_bitXOR(BLUE_LED_OD,1)


#define RED_LED_DIR (_PBDIR_b0+5)
#define RED_LED_OD (_PBOD_b0+5)

#define RedLed_ON()	    	_bitCLR(RED_LED_OD)
#define RedLed_OFF()	    _bitSET(RED_LED_OD)
#define RedLed_FLASH()		_bitXOR(RED_LED_OD,1)



#define CTL_OID_DIR (_PBDIR_b0+6)
#define CTL_OID_OD (_PBOD_b0+6)

#define KBD_PWR_DIR	(_PCDIR_b0+9)
#define KBD_PWR_PH	(_PCPH_b0+9)
#define KBD_PWR_ID	(_PCID_b0+9)

#define USB_DET_DIR	(_PCDIR_b0+10)
#define USB_DET_PH	(_PCPH_b0+10)
#define USB_DET_ID	(_PCID_b0+10)

#define TIME_TICK 2
#define LVT_VALUE 450

#define PowerKeepOn()	_bitSET(PWR_CTRL_OD)
#define PowerTurnOff()	_bitCLR(PWR_CTRL_OD)
#define OID_On()		_bitSET(CTL_OID_OD)
#define OID_Off()		_bitCLR(CTL_OID_OD)
#define SpeakerDET()	_bit_get(HP_DET_ID)
#define USB_DET()		_bit_get(USB_DET_ID)
#define PWD_Key_Press()	(!_bit_get(KBD_PWR_ID))


#define OP_DET_DIR (_PCDIR_b0+2)
#define OP_DET_PH (_PCPH_b0+2)
#define OP_DET_OD (_PCOD_b0+2)

#define OP_OPEN()		_bitSET(OP_DET_OD)
#define OP_CLOSE()		_bitCLR(OP_DET_OD)

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

enum{
    OID_MODE = 0,
    MP3_MODE = 1
};

enum{
	HEADER_PHONE = 0,
	SPEAKER	= 1
};

typedef enum{
	TIMER0			=	(int)0xF2A0,	/**<IRQ source為Timer 0 , 所設定的內容值為KBMP3所定的T0ICR之register的位置*/
	TIMER1			=	(int)0xF2A2,	/**<IRQ source為Timer 1 , 所設定的內容值為KBMP3所定的T1ICR之register的位置*/
	TIMER2			=	(int)0xF2A4,	/**<IRQ source為Timer 2 , 所設定的內容值為KBMP3所定的T2ICR之register的位置*/
	TIMER3			=	(int)0xF2A6,	/**<IRQ source為Timer 3 , 所設定的內容值為KBMP3所定的T3ICR之register的位置*/
	TIMER4			=	(int)0xF2A8,	/**<IRQ source為Timer 4 , 所設定的內容值為KBMP3所定的T4ICR之register的位置*/
}tKBMP3_IRQSource;

typedef enum{
	OFF	= 0,
	ON	= 1
}tLED;

typedef enum{
	NO_KEY = 0,
	SW1,
	SW2,
	KBD_PWR
}tKEY_SW;

typedef struct {
	unsigned int value;
	tKEY_SW sw;
}tKEY;

extern unsigned int gflag;
extern unsigned long TimerCount;
extern const int VolTbl[];
extern unsigned int vol_idx;
//extern unsigned int LED_param;

void __interrupt Exception(void);
void ErrHappen(int ErrCode,char *string);
void System_Timer_Init( int TimerSource,
                    	unsigned int time);
void BatteryDetect(void);
void BlueLED(tLED status);
void RedLED(tLED status);
int Key_Phy_GetKey(void);
void GPIO_init(void);
void CheckUSB(void);
void SpeakDetect(void);

#endif // _SYSTEM_H_
//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------
