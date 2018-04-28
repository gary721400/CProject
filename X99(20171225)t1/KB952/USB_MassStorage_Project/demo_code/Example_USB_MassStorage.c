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
//#include "SYSTEM.H"
#include "ADC.H"
#include "SDImg.h"
#include "config.h"
#include "conf_kb.h"
#include "power.h"
//#include ""


//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//#error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
extern unsigned int gflag;
#define OID_Off()		_bitCLR(CTL_OID_OD)
#define PowerTurnOff()	_bitCLR(PWR_CTRL_OD)


extern unsigned int ap4_mp3_flag;
unsigned int g_exit_usb;
unsigned long time_count;
unsigned long usb_attach_time;
unsigned int flag_chongdian;

void Delay_ms(int count);
//=============================================================================
// PROGRAM
//=============================================================================
#ifdef NIEQING_DIANCHI

//const unsigned long TIMER_COUNT = ((U32)(2)*60000/(TICK_PERIOD));
const unsigned long TIMER_COUNT = 0x30;

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



void Charge_Init(void)
{

   	Charge_EN_IN();
	time_count=0;
}

void Charge_Check(void)
{
#if 0
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
				if(TimerCount%4==0){
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

   	  if(!Charge_State()){
		  	if(TimerCount>time_count){
				time_count=TimerCount+TIMER_COUNT;
			 	RedLed_FLASH();
				BlueLed_OFF();
			 	}
				return;
			}
	  else{
	  	    RedLed_ON();
			BlueLed_OFF();
	  	  }

/*
		if(Charge_State()){
			if(flag_chongdian==0){
				flag_chongdian=1;
				time_count=TimerCount+TIMER_COUNT;
				}
			 if(time_count>TIMER_COUNT){
			 	Charge_Disable();
			 	}
			}
		else{
			 	flag_chongdian=0;
			}
		*/
#endif
/*
	if((Charge_State())&&(charge_state==TRUE))
	{
		if(TimerCount%50==0)
		{
			TimerCount++;
			h_count +=1;
			l_count=0;
		}
			
		if(h_count>50)
		{
			h_count=0;
			charge_state=FALSE;
			Charge_Disable();			
		}
	}
	if((!Charge_State())&&(charge_state==TRUE))
	{
		if(TimerCount%50==0)
		{	
			TimerCount++;
			h_count =0;
			l_count++;
		}
		if(l_count>50)
		{
			l_count=0;
			charge_state=FALSE;
			Charge_Disable();			
		}
			
	}
*/	
}


#endif



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

Bool  key_down(void)
{

	unsigned int count;
	unsigned int adc_value;
	count = 0;	
#ifdef NIEQING_DIANCHI
//	Charge_Check();
#endif	

		adc_value= Get_ADCValue(2);
//		SendString("\radc_value=");
//		send_hex_u16(adc_value);
		if((adc_value >= 600) && ( adc_value <= 730))
		{
			flag_chongdian++;
			
		}
		else{
			flag_chongdian=0;
			}
		if(flag_chongdian>=100){
			MassStorage_Exit();
       		CommonLoadCode(C_USB_HID);
			}
		
	if(Lidian_ok()){
		time_count ++;
		if(time_count >= 10000)
		BlueLed_ON();
		RedLed_OFF();
		}
	else{
		time_count = 0;
		}

	while(PWD_Key_Press())
	{
		if(count++>100){
			return TRUE;
			}
	}




	
  return FALSE;
}

void main(void)
{
    //Variable Declare
    int                 lun;
	
	FS_ERR fserr;
	char temp_lab[12];
	char sys_lab[12];
	char digi[6];
	__asm{BCLR USBCON.14};	
	//System Initial
    IP_INIT();
//	GPIO_init();
	__asm{
		BCLR PBEN.12
		BSET PBDIR.12
		};	
	
	ADC_init();
    SystemClock(FEQ_60M);
	OP_CLOSE();
    PowerTurnOff();
    BlueLed_ON();
//	BlueLed_OFF();
	RedLed_ON();
	ap4_mp3_flag = 0;
	g_exit_usb=0;
	flag_chongdian = 0;
#ifdef NIEQING_DIANCHI
	System_Timer_Init(4,10); 
	Charge_Init();
#endif	
	FSInit(1);
#if USE_SD
	fserr = FS_mount(&SD_Drive);
	//fserr = FS_mount(&NF_Drive);	
	if( fserr != FS_DRIVE_ERROR ){

    digi[0] = HARD_VERSION;
	digi[1] = MAJOR_FW_VERSION;
	digi[2] = MINOR_FW_VERSION;
	digi[3] = 0;
	memset(sys_lab,0,12);
	strcpy(sys_lab,SYS_LAB);
	strcat(sys_lab,digi);


    FSVolume(0,temp_lab,0);
	if(strcmp(temp_lab,sys_lab))
		{
	     FSVolume(0,sys_lab,1);
		}
/*
    FSVolume(0,temp_lab,0);
	if(strcmp(temp_lab,SYS_LAB))
		{
	     FSVolume(0,SYS_LAB,1);
		}
*/
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


		usb_attach_time = 0x180000;
//  	USBStoragePoweroff();
    //USB MassStorage Initial
    MassStorage_Init();
   


USB_Attach:
        
   	USBSetDebounceCount(usb_attach_time);
//	while( !USBAttachCheck() ){
		// check attached or not
		// do your things, for example: mp3 play
//	}
		
	// USB Storage Initial
	lun=0;

    MassStorage_Registers();
	__asm{BSET USBCON.14};
    while(1)
    {		
	if( MassStorage_Loop()==USB_DISCONN || key_down())	
       	{
    		break;
       	}	
   	}
    // un-init USB
	MassStorage_Exit();
	gflag |= 0x0001; 

	if(!USB_DET()){
		 __asm("reset");
		}

	CommonLoadCode(C_OID);	
//	CommonLoadCode(C_MP3);
//	CommonLoadCode(C_REC_PASTER);

   	goto USB_Attach;
    while(1);
}

//=================================================================================
// EOF
//=================================================================================

