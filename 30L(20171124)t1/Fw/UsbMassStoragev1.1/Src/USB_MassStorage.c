//------------------------------------------------------------------------------
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
#include "SYSTEM.H"
#include "ADC.H"
#include "BinaryImage.h"
#include "config.h"
#include "conf_kb.h"
#include "com.h"
#include <string.h>
#include <fs.h>
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
unsigned int vol_key = 0;
Bool bookSys;
extern unsigned char writeFlag;

unsigned char product[16];
unsigned char revision[4];
unsigned char usb_volume[8];

void ready_mount_sd(void);
void delBookSys(void);

void ready_version(void)
{
   //product[0] = HARD_VERSION;USB_DEVICE_MS_PRODUCT         "OID4 V2.0.4.2B  "
   //const unsigned char product[16]={' ',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE_VERSION,'-',H_YEAR,L_YEAR,H_MONTH,L_MONTH,H_DAY,L_DAY,' ',' ',' ',' '};
   memset(usb_volume,0,sizeof(8));
    strcpy((char *)usb_volume,(char *)USB_DEVICE_MS_VENDOR);
   memset(product,0,sizeof(16));
   //strcpy((char *)product,(char *)HARD_VERSION);
   //strcat((char *)product,(char *)MAJOR_FW_VERSION);
   //strcat((char *)product,(char *)MINOR_FW_VERSION);
   //strcat((char *)product,(char *)OID_TYPE);
   strcpy((char *)product,(char *)" ");
   strcat((char *)product,(char *)"G");
   strcat((char *)product,(char *)" ");
   //strcat((char *)product,(char *)NOW_DATA);
   strcat((char *)product,(char *)__DATE__);
 //  strcat((char *)product,(char *)" ");
   SendString((char *)"\r\nproduct=");
   SendString((char *)&product);

   memset(revision,0,sizeof(4));
   strcpy((char *)revision,(char *)HARD_VERSION);
   strcat((char *)revision,(char *)MAJOR_FW_VERSION);
   strcat((char *)revision,(char *)MINOR_FW_VERSION);
   strcat((char *)revision,(char *)OID_TYPE);
   
}


//*/

void deleteRereadfile(void)
{   

  __X FS_ERR err;
  __X FCFILE *REC_File;
  char fileName[64];
  __X unsigned long tempLeng;
	  if(b_sd1_ok){
	  	strcpy(fileName,(char*)"0:\\record.wav"); 
	  	//strcpy(REC_FILE,(char*)"0:\\ap4\\record.wav");
	  	//strcpy(REC_FILE,ap4_dirname);
		//strcat(REC_FILE,(char *)"\\record.wav");
	  	}
	  else{
	  	//strcpy(REC_FILE,(char*)"1:\\record\\record.wav"); 
		strcpy(fileName,(char*)"1:\\ap4\\record.wav"); 
	  	}

	   		tempLeng = FSGetFileSize((char *)fileName, &err);
   	   		if(tempLeng >= 0){
#ifdef TX_DEBUG
				SendString(( char *)"\r\ndel record.wav");
#endif
				FSDelete((char *)fileName);
		}	

}


Bool  key_down(void)
{

	unsigned int count;
	unsigned int adc_value =0;
	count = 0;	
#ifdef NIEQING_DIANCHI
//	Charge_Check();
#endif	
		adc_get();
		if(rec_led_is_timer_timeout()){
			//rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			//adc3_value= Get_ADCValue(3);
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
				}*/
//*/
#if(QUICKLY_FLASH == ENABLE)
				if(b_flash_led){
					BlueLed_OFF();
					RedLed_ON();
					rec_led_start_timer(REC_LED_ON_QUICK_TIME);
					}
				else{
					BlueLed_OFF();
					RedLed_ON();
					rec_led_start_timer(REC_LED_OFF_NORMAL_TIME);

					}
				//BlueLed_FLASH();
				b_flash_led = ~b_flash_led;
				//RedLed_OFF();
				//BlueLed2_OFF();
		
#endif
			
#if(QUICKLY_FLASH == DISABLE)			
			 BlueLed_OFF();
			 RedLed_FLASH();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
#endif
#if(LI_DIANCHI_CHONGDIAN == ENABLE)
		 if(lidian_flag){
		 	BlueLed_ON();
			RedLed_OFF();
		 	}
		 else{
			 if(Lidian_ok()){
				//BlueLed_OFF();
				//RedLed_OFF();
				lidian_flag = TRUE;
				}
			
		 	}

#endif

   }

		if(sys_led_is_timer_timeout()){
			sys_led_start_timer(REC_LED_ON_NORMAL_TIME);
#if 1//( T01OID_CAB == ENABLE )
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

	//__asm{ BCLR PBDIR.6 BCLR PBDIR.7 BCLR PBDIR.8 BCLR PBDIR.9 }
	ADC_init();
    SystemClock(SYS_CLOCK);
	OP_CLOSE();
    PowerTurnOff();
  //  BlueLed_ON();
	BlueLed_OFF();
	RedLed_ON();
	//ap4_mp3_flag = 0;
	//g_exit_usb=0;
	//flag_chongdian = 0;
	//lidian_flag = FALSE;
	bookSys = false;
	System_Timer3_Init();
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nusb double begin!");
#endif
	ready_version();
	MassStorage_Init();

	USBStoragePoweroff();
	USBSetDebounceCount(0x18000);
	//while( !USBAttachCheck() )
	//{
		// check attached or not
		// do your things, for example: mp3 play	
	//}
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
	MassStorage_Registers();
	while(1)
	{
		if( MassStorage_Loop()==USB_DISCONN )
			break;
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
	CommonLoadCode(C_READBNL);	
	while(1);
batterycharge_code:
	while(1){
		if( key_down())goto usb_exit;
		}
}

//-------------------------------------------------------------------------









void ready_mount_sd(void)
{
	FS_ERR fserr;
#ifdef TX_DEBUG
		SendString(( char *)"\r\nbegin mount sd!");
#endif		
	FSInit();
#if(USE_SD1 == 1)

	fserr = FS_mount( 0, &SD1_Drive );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 fail!");
#endif	
		}
	else{
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 ok!");
#endif		
		}
#endif	
}

void delBookSys(void){
	FS_ERR fserr;
	//if(bookSys)return;
	//	bookSys = true;
	if((FSGetFileSize((char *)SD1_BOOK_SYS, &fserr))){
#ifdef TX_DEBUG
		SendString(( char *)"\r\ndel booksys");
#endif
		FSDelete((char *)SD1_BOOK_SYS);
		}	

}

//------------------------------------------------------------------------------
