/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE

#include "KBDM9_DDK.h"
#include <string.h>
#include "Example_USB_MAssStorage.h"
//#include "CodeImg.h"
#include "SYSTEM.H"
#include "ADC.H"
#include "BinaryImage.h"
#include "config.h"
#include "conf_kb.h"
#include "com.h"

#include "power.h"
//#include "keypad_drv.h"

//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//#error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
//extern unsigned int gflag;

#define FW_TO_BOOT	   0x02

#define PWR_VALUE 80

extern unsigned int ena_rec_mode;
extern unsigned int mp3_playing_num;
extern unsigned int wav_playing_num;

extern unsigned int systerm_flag ;
extern unsigned int ap4_mp3_flag;
//extern void send_data(void);
Bool updata_flag;
unsigned int g_exit_usb;
//unsigned long time_count;
unsigned long usb_attach_time;
//unsigned int flag_chongdian;
unsigned char PWD_key_check(void);
//void Delay_ms(int count);
//=============================================================================
// PROGRAM
//=============================================================================

//const unsigned long TIMER_COUNT = ((U32)(2)*60000/(TICK_PERIOD));
//const unsigned long TIMER_COUNT = 0x1000;

Bool key_down(void)
{

	unsigned int count;
	count = 0;	
//	return FALSE;
//#ifdef NIEQING_DIANCHI
#if 0
	Charge_Check();
#endif	
	while(PWD_Key_Press())
	{
		if(count++>100){
			return TRUE;
			}
	}
	
  return FALSE;
}
/*
__asm{
_Delay_ms::
    LINK #0
    PUSH R0
    PUSH R1

    MOV R0,[FP+#(6)]
$Loop2:
    MOV R1,#12000 ;// for fsys = 48Mhz
$Loop:
    CMPD1 R1,#0
    AJMP IF_NE,$Loop
    CMPD1 R0,#0
    AJMP IF_NE,$Loop2
    POP R1
    POP R0
    UNLINK_MACRO
    RETS
}
*/
/*

void Charge_Init(void)
{
	Charge_EN_OUT();  //set output
//	Charge_STA_IN();  //input
	//Charge_Disable();
	//Charge_NO_PH();//state input no_pullup 
	Charge_Enable();//Ê¹ÄÜ³äµç
//	time_count=0;
	TimerCount = 0;
	flag_chongdian=0;
}

void Charge_Check(void)
{
#if 1
	if(0==flag_chongdian)
		{	
		   if((TimerCount<0xBB8)){
		   		return;
		   	}
		   else{
		   		flag_chongdian=0x5555;
				TimerCount=0;
				return;
		   	}
			
		}
	else{
			while(TimerCount<=TIMER_COUNT){
				if(TimerCount%20==0){
//						RedLed_FLASH();
					if(!Charge_State()){
						TimerCount=0;
						return;
						}
					}
				}
			if(TimerCount>=TIMER_COUNT){
				Charge_Disable();
				}
		}
#else	
	if(usb_attach_time>0x100)return;
	while(1){
		Delay_ms(2000);
		RedLed_FLASH();
		}
	while(time_count<=TIMER_COUNT){
			if(!Charge_State()){
				time_count=0;
				return;
				}
			else{
				time_count++;	
				}
			Delay_ms(10);
		}
	if(time_count>=TIMER_COUNT){
		Charge_Disable();
		}
#endif
}
*/

void USB_BatteryCharged(void)
{
	//Battery charged
}

void ToDoCheckNandFlash(void)
{
    while(1);
}

void LED_Callback(void)
{
}

void main(void)
{
	unsigned int adc_value ;
    //Variable Declare
    int                 lun;
	FS_ERR fserr;
	char temp_lab[12];
	__asm{BCLR USBCON.14};	
	//System Initial
    IP_INIT();
//	GPIO_init();
    _bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.
	ADC_init();
    SystemClock(FEQ_60M);
	OP_OPEN();
//    BlueLed_ON();
	BlueLed_ON();
	RedLed_ON();
//	ap4_mp3_flag = 0;
//	g_exit_usb=0;
//	systerm_flag = 0 ;
//	System_Timer_Init(4,TICK_PERIOD); 
#if 0
//#ifdef NIEQING_DIANCHI
#if 0
	Charge_Init();
#endif	
	FSInit(1);
#if USE_SD
	fserr = FS_mount(&SD_Drive);
	//fserr = FS_mount(&NF_Drive);	
	if( fserr != FS_DRIVE_ERROR ){
#if 1
    FSVolume(0,temp_lab,0);
	if(strcmp(temp_lab,SYS_LAB))
		{
	     FSVolume(0,SYS_LAB,1);
		}
#endif
		}
/*
	else
		{
			System_Timer_Init(3,TICK_PERIOD); 
			while(1)
				{
				 if((TimerCount%500)==0)
				  RedLed_FLASH();
				}
		}
*/	
#endif

#if USE_NF
	fserr = FS_mount(&NF_Drive);	
	if( fserr != FS_DRIVE_ERROR ){ 
#if 1
    FSVolume(0,temp_lab,0);
	if(strcmp(temp_lab,SYS_LAB))
		{
	     FSVolume(0,SYS_LAB,1);
		}
#endif
		}
#endif
#endif
#ifdef TX_DEBUG
	UART_Init();
	SendString("\rUSB_Hid start!");
#endif
		usb_attach_time = 0x180000;
//  	USBStoragePoweroff();
    //USB MassStorage Initial
//    ena_rec_mode = 0 ;
//	wav_playing_num = 0 ;
//	mp3_playing_num = 0 ;
    MassStorage_Init();
//	PowerTurnON();
//	PowerTurnOff();
//	Delay_ms(100);
	updata_flag = FALSE ;
USB_Attach:
        
   	USBSetDebounceCount(usb_attach_time);
//	while( !USBAttachCheck() ){
		// check attached or not
		// do your things, for example: mp3 play
//	}
		
	// USB Storage Initial
	lun=0;

    MassStorage_Registers();
	__asm{ BSET USBCON.14 };

    while(1)
    {		
		if( MassStorage_Loop()==USB_DISCONN || key_down()  )	
       	{
       //		MassStorage_Exit();
       //		CommonLoadCode(C_USB_HID);

	   		
    		break;
       	}	
		///*
		if((updata_flag & FW_TO_BOOT))
		{
			Delay_ms(500);
			break ;
		}
	//*/
   	}
    // un-init USB
	MassStorage_Exit();
//	gflag |= 0x0001; 
	Delay_ms(100);
	BlueLed_OFF();
	RedLed_OFF();
	if(!USB_DET())
	{
	//	CommonLoadCode(C_OPEN);
	//	PowerTurnOff();
		__asm("reset");
	}
//	PowerTurnOff();
	b_usb = TRUE;
	CommonLoadCode(C_OID);
//	CommonLoadCode(C_DIC);	
//	CommonLoadCode(C_MP3);
//	CommonLoadCode(C_REC_PASTER);

   	goto USB_Attach;
    while(1);
}

//=================================================================================
// EOF
//=================================================================================

