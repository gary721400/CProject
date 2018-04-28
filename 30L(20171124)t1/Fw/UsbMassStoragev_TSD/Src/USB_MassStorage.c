//------------------------------------------------------------------------------
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"

#include "ADC.H"
#include "BinaryImage.h"
#include "config.h"
#include "conf_kb.h"

//------------------------------------------------------------------------------
#pragma DATA = USB_MS_XRAM
#pragma CODE = USB_MS_CODE
//------------------------------------------------------------------------------
unsigned int g_exit_usb;
unsigned long time_count;
unsigned long usb_attach_time;
unsigned int flag_chongdian;
unsigned int vid_data;
Bool lidian_flag = FALSE;
Bool adpate_flag;
_MEM_TYPE_SLOW_   U32   usb_nosd_timer;

_MEM_TYPE_SLOW_   U8    usb_count;
Bool  b_usb_detection;	
int usb_core_status;

Bool  key_down(void)
{

	unsigned int count;
	unsigned int adc_value =0;
	count = 0;	
#ifdef NIEQING_DIANCHI
//	Charge_Check();
#endif	

		if(rec_led_is_timer_timeout()){
			rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			adc3_value= Get_ADCValue(3);
				//SendString("\r\nadc_value=");
				//send_hex_u16(adc3_value);
/*		
			if((adc_value >= 600) && ( adc_value <= 730))
			{
				flag_chongdian++;
				
			}
			else{
				flag_chongdian=0;
				}
			
			if(flag_chongdian>=4){
				MassStorage_Exit();
		   		CommonLoadCode(C_USB_HID);
				}
*/
#ifdef LI_DIANCHI
		 if(lidian_flag){
		 	BlueLed_OFF();
			RedLed_OFF();
		 	}
		 else{
			 if(Lidian_ok()){
				BlueLed_OFF();
				RedLed_OFF();
				lidian_flag = TRUE;
				}
			else{
				BlueLed_ON();
				RedLed_ON();
				}
		 	}

#endif
#if 0//( T01OID_CAB == ENABLE )
		if(Vol_Inc_Press()){
			if(vol_key++>=3){
				MassStorage_Exit();
				//gflag |= 0x0001; 
				b_usb = TRUE;
				CommonLoadCode(C_CAB);
				}
			}
		else{
			vol_key = 0;
			}
#endif	

		if(PWD_Key_Press())
			{
				if(time_count++>2){
					return TRUE;
					}
			}
			else{
				time_count = 0;
				}
   }


			
	
  return FALSE;
}


void main(void)
{
	unsigned int flag;
	unsigned long i;
	IP_INIT();
	//__asm{BCLR USBCON.14};
	//__asm{ BCLR PBDIR.6 BCLR PBDIR.7 BCLR PBDIR.8 BCLR PBDIR.9 }
	ADC_init();
    SystemClock(FEQ_60M);
	OP_CLOSE();
    PowerTurnOff();
    BlueLed_ON();
//	BlueLed_OFF();
	RedLed_ON();
	//ap4_mp3_flag = 0;
	//g_exit_usb=0;
	//flag_chongdian = 0;
	//lidian_flag = FALSE;
	System_Timer3_Init();
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nKB usb begin!");
#endif
	 // if(!b_usb)goto batterycharge_code;
	MassStorage_Init();

	USBStoragePoweroff();
	USBSetDebounceCount(0x18000);
	//while( !USBAttachCheck() )
	//{
		// check attached or not
		// do your things, for example: mp3 play	
	//}
///*	
	for (i = 0; i <= 0x100000; i++)
	{
		flag = USBAttachCheck();
		// check attached or not
		// do your things, for example: mp3 play
		if(flag) break;
	}
	if(flag == 0)
	{
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nusb adpeat!");
#endif
		goto batterycharge_code;
	}
//*/	
   usb_nosd_timer = Timer_set_timer( TIMER_MS( 4000 ));
   
   //Usb_connect = TRUE;
   usb_core_status = 0;
   usb_count = 0;
   b_usb_detection = TRUE;
   adpate_flag = FALSE;
	MassStorage_Registers();
	
	//__asm{BSET USBCON.14};
	while(1)
	{
		if( MassStorage_Loop()==USB_DISCONN )
			break;
		if(adpate_flag)goto batterycharge_code;
		if( key_down()){
		break;
		}
	}
	MassStorage_Exit();
usb_exit:	
	b_usb = TRUE;
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nusb exit!");
#endif	
	CommonLoadCode(FIRST_PROJECT);	
	while(1);
batterycharge_code:
#ifdef  TX_DEBUG
	SendString("\r\nusb adpate!");
#endif
	while(1){
		if( key_down())goto usb_exit;
		}
}
//------------------------------------------------------------------------------
