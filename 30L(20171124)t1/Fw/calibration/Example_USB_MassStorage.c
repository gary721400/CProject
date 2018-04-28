/******************************************************************************
** Copyright (c) 2010
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE

#include <sfr.h>
#include <string.h>
#include "KBDM9_DDK.h"
#include "Example_USB_MAssStorage.h"
//#include "SDImg.h"
#include "system.h"
#include "ADC.h"
#include "GPIO_bitref.H"
#include "power.h"
#include "Calibration.h"
//#include "globalvariable.h"
#include "com.h"
//#include "LED.h"
//=============================================================================
// VERSION
//=============================================================================
//__X unsigned int gflag @ 0x6FF0;
//=============================================================================
// OPTION
extern unsigned long t01_calib_data1;
extern unsigned long t01_calib_data2;
extern unsigned int adc3_value;
extern unsigned int adc2_value;

unsigned int vol_key = 0;
unsigned int time_count = 0;
extern  _MEM_TYPE_SLOW_   U32   usb_nosd_timer;
//=============================================================================
extern unsigned int Calib_IR( void );
extern void Nib_Rectify( void );
//=============================================================================
// PROGRAM
//=============================================================================
void USB_BatteryCharged(void)
{
	//Battery charged
}
//-----------------------------------------------------------------------------
Bool key_down(void)
{
#if 1
		if(adc3_value < 0x03A0){
			//if(Mode_Key_Press()){
			if(vol_key++>=3){
#ifdef  TX_DEBUG	
				SendString(( char *)"\r\nmode key");
#endif		
				if((t01_calib_data1 == 0)&&(t01_calib_data2 == 0)){
			  		if(calib_state == CALIB_IDLE)calib_state = CALIB_START;
					}
				}
			}
		else{
			vol_key = 0;
			}
	

		if(PWD_Key_Press())
			{
				if(time_count++>5){
#ifdef  TX_DEBUG	
				SendString(( char *)"\r\npower key");
#endif
				  return TRUE;
					}
			}
			else{
				time_count = 0;
				}
		return FALSE;	
#else
	if(PWD_Key_Press())
	{
#ifdef  TX_DEBUG	
		SendString(( char *)"\r\npower key");
#endif
	  return TRUE;
	}
	if(((adc2_value >= 300)&&(adc2_value <= 430)))
	{
#ifdef  TX_DEBUG	
		SendString(( char *)"\r\nmode key");
#endif		
		if((t01_calib_data1 == 0)&&(t01_calib_data2 == 0)){
	  		if(calib_state == CALIB_IDLE)calib_state = CALIB_START;
			}
	}
	return FALSE;
#endif	
}

//=============================================================================
void main(void)
{
    int lun;
	unsigned int value;
	unsigned long Cnt, FullCnt;
	unsigned int PlayValue, PlayKeyCnt, PlayCnt;
	unsigned int USBADCValue, USBCnt, USBMoveCnt;
	unsigned long i;

//    __asm{BCLR USBCON.14};	
// if both SD card and NAND Flash are used for storages
#if  0//USE_SD & USE_NF
	SetVector(SD_DELAY_CALLBACK_TRAP, SD_Busy_Check);
#endif
    IP_INIT();
    SystemClock(SYS_CLOCK);

#if 0//def SPIBOOT_KBDM9
	SPIF_Set_Clock(SPI_20M);	//(SPI_30M);
#endif
    //GPIO_init();
	ADC_init();
//	PowerTurnOff();
	BlueLed_ON();
	RedLed_ON(); 
	usb_nosd_timer = 0;
	t01_calib_data1 = 0;
	t01_calib_data2 = 0;
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB cab begin!");
#endif	
//	SET_PD6
//	SET_POWER
//	SET_AMPLIFILER
//    _bitCLR(BLUE_LED_DIR);
//	_bitCLR(RED_LED_DIR);

//	ADC_init();
//	LED_Init();
	//System_Timer_Init(3, 2);
	System_Timer3_Init();
//	LED_R_Set(1);
//	LED_G_Set(0);
//	LED_B_Set(0);

//USB MassStorage Initial
 //   MassStorage_Init();
USB_Attach:
 //   USBSetDebounceCount(0x180000);
/*
	while( !USBAttachCheck() ){
		// check attached or not
		// do your things, for example: mp3 play
	}
*/
// USB Storage Initial
	//lun=0;
   // MassStorage_Registers();
	usb_nosd_timer = Timer_set_timer(TIMER_MS(2000));
	//__asm{BSET USBCON.14};
	
    while(1)
    {
	 if( usb_nosd_timer && Timer_is_timeout( usb_nosd_timer ))
         {
			usb_nosd_timer = Timer_set_timer(TIMER_MS(50));
			//adc3_value = Get_ADCValue(3);
			adc_get();
			if(key_down()){
				break;
				}
         }
       //	if(MassStorage_Loop()== USB_DISCONN )
      // 	{
		//	break;
      // 	}

	Nib_Rectify();
//Check Battery
	

//Check play Key , Exit USB Mode


//usb if remove

   	}
    // un-init USB
//	MassStorage_Exit();
//	LED_R_Set(0);
//	LED_G_Set(0);
//	LED_B_Set(0);

//	gflag |= 0x0001;

	RTCWDT = 0xBFFFu;
   	goto USB_Attach;

	while(1);
	
}
//=================================================================================
// EOF
//=================================================================================

