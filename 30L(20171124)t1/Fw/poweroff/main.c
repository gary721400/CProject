#pragma DATA = OPEN_MAIN_DATA
#pragma CODE = OPEN_MAIN_CODE
#include <sfr.h>
#include <stdlib.h>
#include <math.h>
#include <KBDM9_DDK.H>
#include "com.h"
#include "power.h"
//#include "USB_Detect.h"
#include "adc.h"
#include "conf_kb.h"
#include "system.h"
//nclude "globalvariable.h"
#include "BinaryImage.h"
#include <string.h>
/*
#define _USBCON_b0 ((_bitref)0x410)
//void Delay_ms(int count);

#define OP_DET_DIR (_PCDIR_b0+2)
#define OP_DET_PH (_PCPH_b0+2)
#define OP_DET_OD (_PCOD_b0+2)
*/
extern int OtpVerify128Bit(__X unsigned int *ptr);
/*
extern unsigned int gflag;
extern unsigned int ap4_mp3_flag;
extern unsigned int vol_idx;
extern unsigned int volume_inc_flag;
extern unsigned int systerm_flag;
extern unsigned int prev_task;
extern unsigned int current_wav_num;
extern unsigned int adc0_value;
extern unsigned int adc2_value;
extern unsigned int adc3_value;
extern unsigned int adc7_value;
*/
#ifdef NIEQING_DIANCHI
unsigned long nieqing_time_count;
_MEM_TYPE_SLOW_ U8  nieqing_count;
const unsigned long TIMER_COUNT = 300;
#endif
//extern unsigned int bak_ap4_num;
//extern unsigned int bak_book_num;
//extern unsigned int bak_page_num;
//extern unsigned int bak_control_mode_num;

//__X unsigned int	otpdata128[8] = {0x0007u,0x5555u,0x5115u,0x0010u,0x6AD8u,0x0D91u,0x0000u,0x1E51u};
//__X unsigned int	otpdata128_test[8] = {0x0001u,0xAAAAu,0x4321u,0x1999u,0x9527u,0x0097u,0xFFFFu,0x9797u};

//__X unsigned int	otpdata128[8] = {0xE000u,0xAAAAu,0xA88A,0x0800u,0x1B56u,0x89B0u,0x0000u,0x8A78u};
const unsigned int	otpdata128_test[8] = {0x0001u,0x5555u,0x4321u,0x0800u,0x9527u,0x0097u,0xFFFFu,0xB458u};
const unsigned int	otpdata128_test_2[8] = {0xFFFEu,0xFFF0u,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu};
/*****************************************************************************/

void Delay_ms(int count);
//------------------------------------------------------------------------------
/*
void GPIO_init(void)
{
	PBEN = 0;
	PCEN = 0;
//	PDEN = 0;

	_bitSET(Lidian_EN_DIR);//锂电充电检查pin
	
	_bitCLR(PWR_CTRL_DIR); // Set PWR_CTRL with Output.

	_bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.

    _bitCLR(BLUE_LED_DIR);
	_bitCLR(RED_LED_DIR);


	_bitCLR(BLUE2_LED_DIR);
	_bitCLR(RED2_LED_DIR);	
	
	_bitCLR(CTL_OID_DIR);
	_bitCLR(CTL_OID_OD);
	
	_bitSET(HP_DET_DIR);
	_bitSET(HP_DET_PH);

	_bitSET(USB_DET_DIR);
	_bitCLR(USB_DET_PH);
	_bitCLR(OP_DET_DIR);
	_bitSET(OP_DET_PH);

}
*/



#ifdef NIEQING_DIANCHI

void Charge_Init(void)
{
	if(USB_DET()){
		Charge_EN_OUT();  //set output
		Charge_Enable();//使能充电
		Charge_STA_IN();
		nieqing_time_count=0;
		}
	else{
		Charge_EN_OUT();  //set output
		Charge_Disable();//使能充电
		charge_state_output();
		charge_state_islow();
		}
}

void Charge_Check(void)
{
	if(USB_DET()){	
		while(nieqing_time_count<TIMER_COUNT){
				if(!Charge_State()){
					nieqing_time_count=0;
					return;
				}
				nieqing_time_count++;
				Delay_ms(10);
			}
		if(nieqing_time_count>=TIMER_COUNT){
			Charge_Disable();
			charge_state_output();
			charge_state_islow();
			}	
		}

}
#endif







void save_wav_num(void)
{
#if 1
    __X FS_ERR err;
  unsigned long temp_len;

  __X FCFILE *Temp_File;
   char temp_file[]="0:\\rec\\pos.dat";
   
	Temp_File = FSOpen((char *)temp_file,FS_MODE_READ_WRITE,&err);   
	FSSeek(Temp_File,0);
	FSWrite(Temp_File, (void *)&current_wav_num,2,&err);
	FSClose(Temp_File);
	FSSetAttribute((char *)temp_file,FS_ATTR_HIDDEN);
#endif	
}


///*
void write_nor_data( void )
{
   __X U8 TempBuf[ 512 ], i;
   __X U32 DataStartSector;
   __X U32 DataLength;
   __X U16 tempNum = 0;
   __X U8  bakVol = 0;

   

   memset(TempBuf,0,256);
   //TempBuf[ 0 ] = MSB(current_wav_num);
   //TempBuf[ 1 ] = LSB(current_wav_num);
   //TempBuf[ 2 ] = vol_idx;
//   TempBuf[ 2 ] = MSW( nor_mp3_bak );
//   TempBuf[ 3 ] = LSW( nor_mp3_bak );
   
   	   GetDataIdInfo( D_DATA, &DataStartSector, &DataLength );
#ifdef SDBOOT_KBDM9
       sdtl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SPIBOOT_KBDM9
        spif_resv_read( DataStartSector, TempBuf );
#endif

		MSB(tempNum) = TempBuf[0];
		LSB(tempNum) = TempBuf[1];
		bakVol = TempBuf[2];

		if((tempNum == current_wav_num)&&(bakVol == vol_idx))return;
#ifdef TX_DEBUG		
   	   SendString("\r\nbak spi data!");
#endif		
		memset(TempBuf,0,256);
		TempBuf[ 0 ] = MSB(current_wav_num);
		TempBuf[ 1 ] = LSB(current_wav_num);
		TempBuf[ 2 ] = vol_idx;


#if 0//def NFBOOT_KBDM9
   nftl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SDBOOT_KBDM9
   //sdtl_resv_read( DataStartSector, TempBuf );
	sdtl_resv_write( DataStartSector, TempBuf );
#endif
#ifdef SPIBOOT_KBDM9
   //spif_resv_read( DataStartSector, TempBuf );
   spif_resv_write( DataStartSector, TempBuf );
#endif

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




//! Power supply switch off
//!
void power_system_off( void )
{

	unsigned int value;
	unsigned int i;
	unsigned int temp_count=0;
#ifdef TX_DEBUG
   SendString("\r\nready to power off");
#endif
//	save_wav_num();
#if 1
    if(b_find_usb_flag)
   	{
   		b_find_usb_flag = FALSE;
#ifdef TX_DEBUG		
   	   SendString("\r\nusb_det power off");
#endif

   	 	BlueLed_OFF();
//	   OID_PWR_OFF();
//   		PWR_OFF();
#ifndef TX_DEBUG	
       RedLed_OFF();
#endif
/*
		if(ChangeToUSBMode()){
              CommonLoadCode(C_USB);
			}
		else{
			system_reset();
			}*/
/*

            // Is not connect with USB Host.
       while(1){
	   			Delay_ms(500);
//				SendString("power_1\r\n");
	   			if(PWD_Key_Press())
	   			CommonLoadCode(C_OID);
       			}
*/
   		
   	}
#else
    if(USB_DET()&&(!b_usb))
   	{
   	   SendString("usb_det power off\r\n");
   	 	BlueLed_OFF();
//	   OID_PWR_OFF();
//   		PWR_OFF();
#ifndef TX_DEBUG	
       RedLed_OFF();
#endif

            // Is not connect with USB Host.
       while(1){
	   			Delay_ms(500);
//				SendString("power_1\r\n");
	   			if(PWD_Key_Press())
	   			CommonLoadCode(C_OID);
       			}

   		
   	}
#endif	
#if 0//( OID == ENABLE )
   turn_off_oid();
   pen_disable();
   oid_power_off();
   oid_dc_dc_off();
#endif

   OID_PWR_OFF();
   PWR_OFF();
//   GPIO_init();
   BlueLed_OFF();
#ifndef TX_DEBUG	
     RedLed_OFF();
#endif
   SendString("\r\npower off\r\n");
 //  while(PWD_Key_Press());
 while(1){
			Delay_ms(100);temp_count ++;
#if 1		
			adc3_value = Get_ADCValue(3);
			
			
#ifdef  TX_DEBUG
			SendString(( char *)"\r\nadc3=");
			send_hex_u16(adc3_value);
#endif
			if(adc3_value > 0x3F0){
				if(temp_count >= 3)break;
			}else{
				temp_count = 0;
			}
#else
			if(!PWD_Key_Press()){
				if(temp_count >= 3)break;
				}
			else{
				temp_count = 0;
				}
#endif			
		}
    Delay_ms(500);
    BlueLed_OFF();
#ifndef TX_DEBUG	
     RedLed_OFF();
#endif
	RTCWDT = 0xBFFFU;
	while(1){
		}
  // CommonLoadCode(C_OPEN);
/*
   while( 1 )
   	{
   	 PWR_OFF();
	 
   	}
*/   
}
void remeber_mp3_number(void)
{
	
	 FS_ERR fs_err;
	 __X U32 file_size;
	__X FCFILE *REC_File;

	 //SDKLoadCode( MMI_PLAYINTRO_CODE00 );
	 //strcpy( tisiFileName, "0:\\count.bin" );
	  //if( find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath ))
	  {

		REC_File = FSOpen((char *)"0:\\count.bin",FS_MODE_READ_WRITE,&fs_err);	
		FSSeek(REC_File,0UL);
		FSWrite(REC_File,(__X void *)&all_mp3_num,2,&fs_err);
    	FSClose(REC_File);
	  }

	SendString("\r\nall_mp3_num=");
	send_hex_u16(all_mp3_num);
}

void main(void)
{

    FS_ERR fserr;
	IP_INIT();
	SystemClock(SYS_CLOCK);
	OP_CLOSE();
	ADC_init();
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nKB poweroff!\r\n");
#endif

	write_nor_data();
	//FSInit(1);
	//fserr = FS_mount(&SD_Drive);
	b_sd1_ok = FALSE;
	b_sd2_ok = FALSE;
	FSInit();
#if(USE_SD1 == 1)

	fserr = FS_mount( 0, &SD1_Drive );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 fail!");
#endif	
		}
	else{
		b_sd1_ok = TRUE;
		}
#endif	

#if(USE_SD2 == 1)


	fserr = FS_mount( 1, &SD2_Drive );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd2 fail!");
#endif	
		}
	else{
		b_sd2_ok = TRUE;
		}
#endif
	//fserr = FS_mount(&NF_Drive);	

	if((!b_sd1_ok)&&(!b_sd2_ok)){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nno sd!");
#endif		
		  Exception();
	 }
//	SD1_fast_mode();

	   //remeber_mp3_number();

	   BlueLed_ON();
	   RedLed_OFF();

	   BlueLed2_OFF();
	   RedLed2_OFF();

	   power_system_off();


}



//void LED_Callback(void)
//{
//}
