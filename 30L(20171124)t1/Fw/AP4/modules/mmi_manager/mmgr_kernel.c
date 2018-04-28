//!
//! @file mmgr_kernel.c,v
//!
//! Copyright (c) 2004 Atmel.
//!mailbox
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the mmi_manager
//!
//! The mmi_manager is the task that manage the executing of MMI applications.
//! The kernel of the mmi_manager is organized around an LIFO stack which stores all applications
//! being launching and running at the same time.
//! The last application lauched (or pushed) is placed at the top of the stack and has in consequence the focus
//! in the interaction human - machine.
//!
//! @version 1.61 snd3-dvk-1_9_5 $Id: mmgr_kernel.c,v 1.61 2007/07/26 14:41:50 sguyon Exp $
//!
//! @todo
//! @bug
//!
#pragma CODE = MMGR_KERNEL_CODE
#pragma DATA = MMGR_KERNEL_DATA


#ifndef _mmgr_kernel_c_
#define _mmgr_kernel_c_

//_____  I N C L U D E S ___________________________________________________
#include <sfr.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <KBDM9_DDK.H>
#include <ctype.h>

#include "power.h"
#include "config.h"
#include "adc.h"
#include "conf_kb.h"
#include "com.h"
#include "system.h"


#include "mmgr_kernel.h"
#include <fs.h>
#include "keypad.h"
#include "conf_oid.h"
#include "oid.h"
//#include "int_drv.h"                // Interrupt definitions
#include "player_ap3.h"
//#include "search_game.h"
#include "mmi_ap3.h"
#include "mmi_ap3.h"
#include "search.h"
#include "file.h"
#include "search_remote.h"
#include "REC_API.H"
#include "AGC.h"
#include "kbfat.h"
#include "FileSearch.h"
//#include "conf_lcd.h"
//#include "lcd_drv.h"
//#include "lcd_SSD1303.h"
//#include "tab_pict.h"
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(MP3_COMMON_CODE);


//void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
//_____ D E C L A R A T I O N ______________________________________________
#if( AP3 == ENABLE )
extern Bool g_b_search_on;
#endif
//extern   xdata USB_Variables  USB;
//extern   xdata SDDEVICE_VAR   SDVar;

//#if(OID_VERSION == OID1)
//extern Bool bPenOn;
//#endif

//extern unsigned int vol_idx;
//extern unsigned int adc7_value;
//extern unsigned int current_wav_num;

//extern __X U32  g_timer_tick;
extern     _MEM_TYPE_SLOW_  U32			  addr_novoice;
extern _MEM_TYPE_SLOW_   Bool      flag_load_mp3_dac;
__X unsigned int Cache[256*CacheNum];
 xdata  U32  usb_nosd_timer;
//extern unsigned long free_space;
//extern unsigned int systerm_flag;
//extern Bool flag_wav_play;
// xdata U8 current_api;
//Bool op_open_flag = FALSE;
//unsigned int DAC_Out; // 0: Headphone, 1: Speak

#if 0//ndef NF_DISK 
extern unsigned long  Frame_Count;

extern __X int NotifyFlag, NotifyError;
//extern __X int *NotifyBuf;
extern __X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
extern __X int idx_L,idx_R;
#endif

#ifdef NIEQING_DIANCHI
extern void Charge_Init(void);
#endif

//U16		swap_code_flag;

void dowith_bat(void);
//void swap_code(U16 flag);
void read_wav_num(void);
//extern void  create_gendu_file				   ( void );
extern void ADC_INIT( void );

extern void player_task_init(void);
extern void  scan_oid       ( void );
extern void readyAp4DirName(void);
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
/*
ImageDeclareType(AP3_OPEN_FILE_CODE);
//ImageDeclareType(MP3_COMMON_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(AUADC_COMMON_CODE);
ImageDeclareType(REMOTE_TASK_INIT_CODE);
ImageDeclareType(REMOTE_TASK_CODE);
ImageDeclareType(AP3_OPEN_FILE_CODE);
ImageDeclareType(GAME_INITIAL_CODE);
ImageDeclareType(GAME_SEARCH_1_CODE);
ImageDeclareType(GAME_Script_CODE);
//ImageDeclareType(IMA_CODEC_CODE);
ImageDeclareType(FILE_EXTERN_CODE);
ImageDeclareType(RP_RECORD_CODE);
ImageDeclareType(POWER_SYSTEM_OFF_CODE);
ImageDeclareType(MAKE_SD_DIR_CODE);
ImageDeclareType(SYSTEM_TIMER3_INIT_CODE);
*/
ImageDeclareType(KBFAT_CODE);
ImageDeclareType(INIT_OID_CODE);
//static _MEM_TYPE_SLOW_ U8 s_false = FALSE;


//! Store the future index pointing the top application.
//! This variable computes the future index according to the new applications pushed during the task process.
//! Indeed, these new applications can't be directly processed since they haven't started yet.
//! This variable prevents application stack overflow.
//! Each event "EVT_SART_APPLI" makes it possible to retrieve the pushed applications to launch.
//static _MEM_TYPE_SLOW_ U8 s_future_idx_top_appli;


//_____ P R I V A T E   D E C L A R A T I O N S ____________________________


//_____ D E F I N I T I O N S ______________________________________________


#if 1
void dowith_USB(void)
{     
		  interrupt_play();
          file_close();
		  b_find_usb_flag = TRUE;
          CommonLoadCode(C_POWEROFF);

}
void dowith_tishi_bat(void)
{
	interrupt_play();
	//swap_code(AP3_OPEN_FILE_CODE_FLAG);
	//SDKLoadCode(AP3_OPEN_FILE_CODE);
	tishi_mp3(FLAG_MIN_TISHI,false);
}
void dowith_bat(void)
{
		
		interrupt_play();
		scan_oid_disable();
 		Kbd_set_evt_disabled();
		poweroff_falg = TRUE;
		if(Current_Special_Action)
			{
				(void)act_do(Current_Special_Action,0);
			}
		else
			{
				 //SDKLoadCode(AP3_OPEN_FILE_CODE);
				 tishi_mp3(FLAG_BATTERY_LOW_FILE,false);
			}
}
#else
void BatteryDetect(void)
{
	unsigned int count,temp_Value;
	if(battery_count>=0x7F)return;
	temp_Value = Get_ADCValue(0);
//	SendString("\r\nGet_ADCValue(0)=");
//	send_hex_u16(temp_Value);
//	SendString("\r\n");
//		if((temp_Value < LVT_VALUE)&&!poweroff_falg)
		if((temp_Value < LVT_VALUE)&&!poweroff_falg)
		{
		    SendString("begin close!\r\n");
			battery_count++;
			if(battery_count>=10)
				{
//#ifndef TX_DEBUG
				BlueLed_OFF();	
			    RedLed_ON();
//#endif
				if( player_ap3_is_busy())
			 		player_ap3_stop();
			 		init_array_v();
//					OP_CLOSE();
					scan_oid_disable();
			 		Kbd_set_evt_disabled();
					battery_count=0x80;
					poweroff_falg = TRUE;
					if(Current_Special_Action)
						{
//						    vol_idx = 0;
							act_do(Current_Special_Action,0);
						}
					else
						{
//							 vol_idx = 0;
//							 swap_code(AP3_OPEN_FILE_CODE_FLAG);
							 SDKLoadCode(AP3_OPEN_FILE_CODE);
							 tishi_mp3(2);
						}
				return;
			}
	}
	else{
		battery_count=0;
		}
}
#endif



#ifdef NIEQING_DIANCHI
void Charge_Check(void)
{

	if(qieqing_is_timer_timeout()){
		qieqing_start_timer(QIEQING_TIME);

		if(Charge_State()){
			heigh_num++;
			}
		else{
			low_num++;
			}
		if((heigh_num>0)&&(low_num>0)){
			heigh_num = 0;
			low_num = 0;
			}
#if 0//def TX_DEBUG		
	 SendString("\r\nheight:low=");
	 send_hex_u16(heigh_num);
	  SendString(":");
	 send_hex_u16(low_num);
#endif			
		if(((heigh_num>30)&&(low_num==0))||((heigh_num==0)&&(low_num>=30))){
			Charge_Disable();
			charge_state_output();
			charge_state_islow();
			qieqing_stop_timer();
			}
		
		}

}

#endif


void ledFlash(void)
{

#if 0		
		if(rec_led_is_timer_timeout())
			{
				BlueLed_FLASH();
				RedLed_OFF();
				rec_led_start_timer(LED_NORMAL_TIME);
			}
#else		
		if(rec_led_is_timer_timeout())
			{
#if(QUICKLY_FLASH == ENABLE)
				if(b_flash_led){
					BlueLed_ON();
					if(bPenOn){
						rec_led_start_timer(REC_OID_FLASH_TIME);
						}
					else{
						//BlueLed_ON();
						rec_led_start_timer(REC_LED_ON_QUICK_TIME);
						}
					}
				else{
					   BlueLed_ON();//BlueLed_OFF();
						if(bPenOn){
						rec_led_start_timer(REC_OID_FLASH_TIME);
						}
					else{
						//BlueLed_ON();
						rec_led_start_timer(REC_LED_OFF_NORMAL_TIME);
						}
					}
				//BlueLed_FLASH();
				b_flash_led = ~b_flash_led;
				RedLed_OFF();
		
#endif			
#if(QUICKLY_FLASH == DISABLE)

				BlueLed_FLASH();
				RedLed_OFF();
				if(bPenOn){
					rec_led_start_timer(REC_OID_FLASH_TIME);
					}
				else{
					//BlueLed_ON();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
					}
#endif				
			}
#endif

}

//! @brief The mmi_manager task in charge of managing the entire MMI.
//!
//! The mmi_manager task is running on messages posted in the mailbox by tasks.
//! These messages can be distributed to the different launched applications
//! in the processing order defined by the LIFO architecture of the stack.
//! An event can be passed from the top application towards the bottom application
//! to the destination application.
//! Each mmi application can catch an event that is destinated to it
//! or forward it to the next lowest-priority application.
//! If any application catch the event, the mmi_manager destroyes it.
//! The first MMI application launched, placed at the bottom of the application stack,
//! is called : mmi_idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none

void mmgr_task(void)
{
   // Boolean "b_evt_back_to_top_stacked" to prevent consecutive posted events "EVT_BACK_TO_TOP"
   // when several applications are killed at the same time
   // This avoids multiple refeshing of a same screen.
//   static Bool b_evt_back_to_top_stacked = FALSE;
//   _MEM_TYPE_SLOW_ Msg evt;

   

//	if(g_timer_tick%(150) == 0)
//		BlueLed_FLASH();

	adc_get();


/*
	if((g_timer_tick%(500/TICK_PERIOD) == 0))
    if(CheckUSB()){
		dowith_USB();
    	}

	if((KBD_IDLE == kbd_state)&&(g_timer_tick%(500/TICK_PERIOD) == 0))
	BatteryDetect();
*/	


#ifdef NIEQING_DIANCHI
	 Charge_Check();
#endif	
 
   // Keypad process
   kbd_process();

   
///*
   scan_oid();
//   SendString("\r\nCurrent_Index =");
//   send_hex_u16(Current_Index);
   
//#if( OID_VERSION == OID1 )
//*/
///*
	if(b_low_battary){
		}
	else{
		 ledFlash();
		}
//	  	*/
//#endif

//	if((g_timer_tick%(500/TICK_PERIOD) == 0)&&!bPenOn)
//	  	BlueLed_FLASH();

//    SendString("\r\nsearch_task=");
//    send_hex_u8(g_b_search_on);
//	send_hex_u8(g_b_poweroff_activated);

//#if( AP3 == ENABLE )
   // search_task();

	if(sys_led_is_timer_timeout()){
		sys_led_start_timer(SYS_LED_ON_TIME);
		op_time_dowith();
		if(!player_ap3_is_busy())clk_time_dowith();
		if(CheckUSB())dowith_USB();
		//if(tishi_BatteryDetect())dowith_tishi_bat();
		if(BatteryDetect())dowith_bat();
		
		}

   search_evt();
//#endif   //#if( AP3 == ENABLE )


   // Timers software process
 //  ts_process();

//   SendString("current_api=");
//	  send_hex_u8(current_api);
//	 SendString("\r\n");  
   // Get the eventual event present in the mailbox.

//if(g_timer_tick%(5000/TICK_PERIOD) == 0)
//		    BatteryDetect();
  
/*
#if( EARPHONE_DET == ENABLE )
	if(SpeakerDET())
	{
		DAC_Out = SPEAKER;
	}
    else
	{
		DAC_Out = HEADER_PHONE;
	}
#endif   //#if( EARPHONE_DET == ENABLE )
//*/
}
/*
void __interrupt Exception(void)
{
    while(1){
			RESET_SYS();
			}
//	while(1);	
}
*/
/*
#ifndef NF_DISK 

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
{   
    if (NotifyFlag > 0){
        
        NotifyError = 1;
    }   
	IMA_ADPCM_Encoder(buffer, ADVACE_ADPCM_Buf[idx_R++]);
	if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;

    Frame_Count++;
//	if(Frame_Count > 32){
//		  AGC(buffer);
//	}	
//    NotifyFlag++;
    
//    NotifyBuf = buffer;
}

#endif
//*/

#pragma CODE = MMGR_KERNEL_INIT_CODE
#ifdef NIEQING_DIANCHI

void Charge_Init(void)
{
	if(USB_DET()){
		Charge_EN_OUT();  //set output
	//	Charge_STA_IN();  //input
		//Charge_Disable();
		//Charge_NO_PH();//state input no_pullup 
		Charge_Enable();//使能充电
		Charge_STA_IN();
		heigh_num = 0;
		low_num = 0;
		qieqing_stop_timer();
		qieqing_start_timer(1000);
		}
	else{
		Charge_EN_OUT();  //set output
		Charge_Disable();//使能充电
		charge_state_output();
		charge_state_islow();
		qieqing_stop_timer();
		}
}
#endif


void ready_mount_sd(void)
{
	FS_ERR fserr;
	U8 i;
	//char temp_lab[12];
	//char sys_lab[12];
	//char digi[6];
	b_sd1_ok = FALSE;
	b_sd2_ok = FALSE;
	FSInit();
#if(USE_SD1 == 1)
	fserr = FS_mount( 0, &SD1_Drive_Cache );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 fail!");
#endif	
		}
	else{
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 ok!");
#endif		
		SDCKCR = 0x03;
		SD1_Cache_Open( Cache, CacheNum );
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
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd2 ok!");
#endif			
		b_sd2_ok = TRUE;
		SDCKCR = 0x03;
		SD2_Cache_Open( Cache, CacheNum );
		}
#endif
}

//!
//! @brief Initialisation of the mmi_manager
//!
//! Set to 0 the general LIFO stack
//! And activate the first MMI application : the idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//!
//! @return none
void mmgr_task_init( void )
{
   // Init the LIFO
   
//    U32 timer;

	FS_ERR fserr;
	int drive;
	unsigned int i,j;
	unsigned int ch0;
	unsigned int ch2;
	char temp_lab[12];
//	swap_code_flag = 0;
//	code_flag = 0;

 	
//	PowerKeepOn();
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB OID begin!");
#endif
	//if(!g_b_readbnl){
	//	CommonLoadCode(C_READBNL);
	//	}
#if 0
//  if( (!PWD_Key_Press())&&(gflag & 0x0001) == 0)
//  if( PWD_Key_Press())
if((gflag & 0x0001) == 0)
   {
//     SendString("121\r\n");
   	  if(!PWD_Key_Press())
   	  	{
//   	  	  SendString("124\r\n");
		  PWR_ON();   	     
//	      OID_PWR_OFF();
	      CommonLoadCode(C_USB);
//          CommonLoadCode(C_MP3);
   	  	}
   }
//     gflag = 0;
//     SendString("131\r\n");
#endif
//   	 PWR_ON();
     OID_PWR_ON();  
	 BlueLed_ON();
#ifndef TX_DEBUG	
     RedLed_OFF();
#endif
	// ready_mount_sd();

	SetVector(EXCEPTION_TRAP,Exception);

	ADC_init();
//     Delay_ms(100);
//	 BlueLed_ON();
//	 sys_led_on();
//   CommonLoadCode(C_MP3);
//     scheduler();
	player_task_init();
   // g_timer_tick = 0;
	
	//System_Timer_Init(3,TICK_PERIOD); 
	SDKLoadCode(SYSTEM_TIMER3_INIT_CODE);
	System_Timer3_Init();
 // SDKLoadCode(DAC_COMMON_CODE);
//	SDKLoadCode(MP3_COMMON_CODE);
//	SDKLoadCode(AP3_OPEN_FILE_CODE);

#if 0//def USE_AUADO
#ifndef NF_DISK 
//	swap_code(IMA_CODEC_CODE_FLAG);
	SDKLoadCode(IMA_CODEC_CODE);
	ADC_INIT();
#endif
#endif

//	swap_code(AP3_OPEN_FILE_CODE_FLAG);
// 	swap_code(FILE_EXTERN_CODE_FLAG);
//	SDKLoadCode(AP3_OPEN_FILE_CODE);
	SDKLoadCode(FILE_EXTERN_CODE);

#ifdef UNCOIDE_FORMAT
	nav_string_unicode();
#endif

#ifdef ASCII_FORMAT
	nav_string_ascii();
#endif	


	//FSInit(1);
	//fserr = FS_mount(&SD_Drive);

	//fserr = FS_mount(&NF_Drive);	
	ready_mount_sd();
	if((!b_sd1_ok)&&(!b_sd2_ok)){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nno sd!");
#endif		
		  Exception();
	 }
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nmount sd ok!");
#endif




#if 0//USE_NF
	fserr = FS_mount(&NF_Drive);	
	if( fserr == FS_DRIVE_ERROR ){ 
//		SendString(( char *)"The File system initial fail.");
		while(1){
			RESET_SYS();
			}
		}
#endif


	 //SDKLoadCode(KBFAT_CODE);
	 //show_subdir();

//	SDKLoadCode(AP3_OPEN_FILE_CODE);
//	SDKLoadCode(FILE_EXTERN_CODE);
#ifdef NIEQING_DIANCHI
	 Charge_Init();
#endif	

 	kbd_init_process();
//#ifndef NF_DISK 	
//    SDKLoadCode(DAC_COMMON_CODE);
//#endif
//	SDKLoadCode(MP3_COMMON_CODE);
	
    
//    mmc_sd_all_mode_dir_make();
	if(!b_oid_ok){
		SDKLoadCode(INIT_OID_CODE);
    	reinit_oid_pen();
		b_oid_ok = TRUE;
		}
	   bPenOn = FALSE;
	scan_oid_disable();
	flag_load_mp3_dac = FALSE;
#if 1
	 if(!b_startup_play){
	 	//b_startup_play = TRUE;
//		strcpy(tisiFileName,SYS_OPEN_FILE);
		tishi_mp3(FLAG_SYS_OPEN_FILE,false);
		//SDKLoadCode(MAKE_SD_DIR_CODE);
   		//mmc_sd_all_mode_dir_make();
		//read_wav_num();
	 	}
	 else{
	 	tishi_mp3(FLAG_SIMPLE,false);
	 	}
#endif
	SDKLoadCode(MAKE_SD_DIR_CODE);
	readyAp4DirName();
   if(b_back_from_mp3){
	//  	    tishi_mp3(FLAG_SIMPLE);
  	 }
//   swap_code(FILE_EXTERN_CODE_FLAG);
//	SDKLoadCode(FILE_EXTERN_CODE);
//   file_init();
	if(g_b_noap4){
		tishi_mp3(FLAG_NOAP4FILE,false);
	    readyGoNewPorject(C_MP3);
	    //system_reset();
		}
	
//    create_gendu_file();
	SDKLoadCode(AP3_OPEN_FILE_CODE);
	ap3_mmi_appli();
	
	power_authorize_autopoweroff();
}
/*
void read_wav_num(void)
{

    __X FS_ERR err;
  unsigned long temp_len;

  __X FCFILE *Temp_File;
  char temp_file[]="0:\\rec\\pos.dat";
  
  current_wav_num = 0;
  temp_len = FSGetFileSize((char *)temp_file, &err);
	if(temp_len>0){ 
	   	Temp_File = FSOpen((char *)temp_file,FS_MODE_READ,&err);   
		FSSeek(Temp_File,0);
		FSRead(Temp_File, (void *)&current_wav_num,2,&err);
		FSClose(Temp_File);
		if(current_wav_num>999)current_wav_num=0;
		}	
	else{
		Temp_File = FSOpen((char *)temp_file,FS_MODE_CREATE,&err);   
		FSResize(Temp_File,2);
		FSClose(Temp_File);
		}
}
*/
#endif // _mmgr_kernel_c_
