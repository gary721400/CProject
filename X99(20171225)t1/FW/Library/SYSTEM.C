/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: SYSTEM.C
 * Author		: Nick
 * Create Date	: December 2, 2008
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

//------------------------------------------------------------------------------
#include <sfr.h>
#include "system.h"
#include "SFR.h"
#include "ADC.h"
#include "KEY_SCAN.H"
#include "SDImg.h"
//------------------------------------------------------------------------------
#pragma CODE= SYSTEM_CODE
#pragma DATA= SYSTEM_DATA

//------------------------------------------------------------------------------
//void LED_Callback(void);

//------------------------------------------------------------------------------
unsigned long TimerCount;
//int DAC_Out; // 0: Headphone, 1: Speaker
//unsigned int LED_param;
//const int VolTbl[5] = {7,13,19,25,31}; 
extern const int VolTbl[]; 
//------------------------------------------------------------------------------
void __interrupt Exception(void)
{
    //__asm{RESET}
	while(1);	
}

//------------------------------------------------------------------------------
void ErrHappen(int ErrCode,char *ErrMessage)
{
    //__asm{RESET}
	while(1);
}

//------------------------------------------------------------------------------
#define CLOCK (12000/32)   // Kilo Hz
#define TMRxPRD(T) (CLOCK*(T)-1)    // millisecond

#define TMR_SRC_NUM 5
#define TMR_RGS_NUM 4
static const unsigned int TimerSourceTBL[TMR_SRC_NUM][TMR_RGS_NUM] =
{{(unsigned int)TIMER0,(unsigned int)&TMR0CR,(unsigned int)&TM0PRD,TIMER0_INT_TRAP},
 {(unsigned int)TIMER1,(unsigned int)&TMR1CR,(unsigned int)&TM1PRD,TIMER1_INT_TRAP},
 {(unsigned int)TIMER2,(unsigned int)&TMR2CR,(unsigned int)&TM2PRD,TIMER2_INT_TRAP},
 {(unsigned int)TIMER3,(unsigned int)&TMR3CR,(unsigned int)&TM3PRD,TIMER3_INT_TRAP},
 {(unsigned int)TIMER4,(unsigned int)&TMR4CR,(unsigned int)&TM4PRD,TIMER4_INT_TRAP}};

//------------------------------------------------------------------------------
void __interrupt TimerISR(void)
{
	TimerCount++;
//    KeyScan_CallBack();
//    if(TimerCount%(LED_param/TIME_TICK) == 0)
//    	LED_Callback();
}

//------------------------------------------------------------------------------
void System_Timer_Init(int TimerSource,
                       unsigned int time)
{
	TimerCount = 0;
//	LED_param = 5000;
	// clock source is 12Mhz, pre-scale is divided by 32.
    *((__X unsigned int *)TimerSourceTBL[TimerSource][1]) = 0xD005u;
    *((__X unsigned int *)TimerSourceTBL[TimerSource][2]) = TMRxPRD(time);
    SetVector(TimerSourceTBL[TimerSource][3],TimerISR);
    *((__X unsigned int *)TimerSourceTBL[TimerSource][0]) = 0x0001;    
}

//------------------------------------------------------------------------------
void BlueLED(tLED status)
{
/*
	if(status == ON)
	{
	    _bitCLR(BLUE_LED_OD);
	}
	else
	{
	    _bitSET(BLUE_LED_OD);
	}
	*/
}

//------------------------------------------------------------------------------

void RedLED(tLED status)
{
/*
	if(status == ON)
	{
	    _bitCLR(RED_LED_OD);
	}
	else
	{
	    _bitSET(RED_LED_OD);
	}
	*/
}

//------------------------------------------------------------------------------
static const tKEY ADCKeyLevel[] = {{900,NO_KEY},{600,SW2},{300,SW1}};
int Key_Phy_GetKey(void)
{
	static unsigned int LastKeyState;
	unsigned int value;
	int i;
	
	if(PWD_Key_Press())
	{
		return KBD_PWR;
	}
	value = Get_ADCValue(2);
	if(value == 0xFFFFu)
		return LastKeyState;
	for(i=0;i<sizeof(ADCKeyLevel)/sizeof(tKEY);i++)
	{
		if(value >= ADCKeyLevel[i].value)
		{
			LastKeyState = ADCKeyLevel[i].sw;
			return LastKeyState;
		}
	}
	LastKeyState = NO_KEY;
    return NO_KEY;
}

//------------------------------------------------------------------------------
void GPIO_init(void)
{
	_bitCLR(PWR_CTRL_DIR); // Set PWR_CTRL with Output.

	_bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.

    _bitCLR(BLUE_LED_DIR);
	_bitCLR(RED_LED_DIR);
	
	_bitCLR(CTL_OID_DIR);
	_bitCLR(CTL_OID_OD);
	
	_bitSET(HP_DET_DIR);
	_bitSET(HP_DET_PH);

	_bitSET(USB_DET_DIR);
	_bitCLR(USB_DET_PH);

#if 1
	_bitCLR(OP_DET_DIR);
	_bitSET(OP_DET_PH);
#endif
	
}

//--------------------------------------------------------------------
extern unsigned int gflag;

//------------------------------------------------------------------------------
void CheckUSB(void)
{
#if 1
	unsigned int count;

	count = 0;	
	while(USB_DET())
	{
		if((gflag & 0x0001) == 0 && count++ > 100)
		{
			OID_Off();
			CommonLoadCode(C_USB);			
		}
	}
	gflag = 0;
#else
	if(USB_DET()) // Check USB VBUS
    {
        if((gflag & 0x0001) == 0)
        {     
            Delay_ms(500); // VBUS(5V) Debounce
            if(USB_DET()) // Check USB VBUS
            {
                if(ChangeToUSBMode())
                	{
                    	CommonLoadCode(C_USB);
                	}
                // Is not connect with USB Host.
                gflag |= 0x0001; 
            }
        }
    }
    else
    {
        gflag &= 0xFFFEu;
    }
#endif	
}

//------------------------------------------------------------------------------
void BatteryDetect(void)
{
	unsigned int count;
	
	count = 0;	
	while(Get_ADCValue(0) < LVT_VALUE)
	{
		if(count++>100)
			PowerTurnOff();
	}
}
//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------