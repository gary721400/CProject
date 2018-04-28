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
#pragma CODE = DIC_KERNEL_CODE
#pragma DATA = DIC_KERNEL_DATA


#ifndef _dic_kernel_c_
#define _dic_kernel_c_

//_____  I N C L U D E S ___________________________________________________

//#include "config.h"
#include "dic_kernel.h"
#include <string.h>
#include "power.h"
#include "keypad.h"

#include "conf_oid.h"
#include "oid.h"
#include "conf_kb.h"
#include "com.h"
#include "system.h"
//#include "intrins.h"
#include "dic_kernel.h"
//#include "int_drv.h"                // Interrupt definitions
//#include "player_ap3.h"
//#include "search_game.h"
//#include "mmi_ap3.h"
//#include "header.h"
#include <KBDM9_DDK.H>
#include "power.h"
#include "dic_ap3.h"
#include "dic_search.h"
#include "com.h"
#include "dic_remote.h"
#include "conf_kb.h"
#include "dic_file.h"
//#include "power_charge.h"
//_____ D E C L A R A T I O N ______________________________________________
#if 1//( AP3 == ENABLE )
extern Bool g_b_search_on;
#endif
//extern   xdata USB_Variables  USB;
//extern   xdata SDDEVICE_VAR   SDVar;

//#if(OID_VERSION == OID1)
extern Bool bPenOn;
extern Bool g_b_player_on;

//#endif
extern unsigned int cal_flag;
//extern unsigned int vol_idx;

//extern __X U32  g_timer_tick;

extern _MEM_TYPE_SLOW_   Bool      flag_load_mp3_dac;
extern unsigned long free_space;

// xdata  U32  usb_nosd_timer;
_MEM_TYPE_SLOW_  U16 current_adb_sd;
//extern unsigned int systerm_flag;
//extern Bool flag_wav_play;
// xdata U8 current_api;
//Bool op_open_flag = FALSE;
//unsigned int DAC_Out; // 0: Headphone, 1: Speak
//extern __X int *NotifyBuf;
//extern _MEM_TYPE_SLOW_   U8  dic_flag  ;
/*
extern unsigned int vol_idx;
extern unsigned int gflag;
extern unsigned int volume_inc_flag;
extern unsigned int charge_state;
extern unsigned int adc0_value;
extern unsigned int adc2_value;
extern unsigned int adc3_value;
extern unsigned int adc7_value;
*/
//xdata U8	state_level;
xdata U8	charge_count;
__X unsigned int Cache[256*CacheNum];
//extern Bool dic_play;
//extern unsigned int adc7_value;
//U16		swap_code_flag;
//U16		battery_count;
//Bool OID_PLAY;
//void BatteryDetect(void);
//void swap_code(U16 flag);

//extern void  create_gendu_file				   ( void );
//void Charge_Detect(void);

//void Charge_Init(void);

//extern void vol_inc(void);
//extern void judge_op(void);
extern void search_task( void );
void ledFlash(void);
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

//ImageDeclareType(AP3_OPEN_FILE_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(AUADC_COMMON_CODE);
//ImageDeclareType(REMOTE_TASK_INIT_CODE);
//ImageDeclareType(REMOTE_TASK_CODE);
//ImageDeclareType(AP3_OPEN_FILE_CODE);

//ImageDeclareType(IMA_CODEC_CODE);
//ImageDeclareType(FILE_EXTERN_CODE);
ImageDeclareType(SYSTEM_TIMER3_INIT_CODE);

#if 1
void dowith_bat(void)
{

	BlueLed_OFF();	
    RedLed_ON();
	if( player_ap3_is_busy())
 		player_ap3_stop();
 		init_array_v();
//					OP_CLOSE();
		scan_oid_disable();
 		Kbd_set_evt_disabled();
		battery_count=0x80;
		poweroff_falg = TRUE;
		tishi_mp3(FLAG_BATTERY_LOW_FILE);
}
#else
void BatteryDetect(void)
{
	unsigned int count,temp_Value;
	count = 0;	
	temp_Value = Get_ADCValue(0);
//	SendString("\r\ntemp_Value=");
//	send_hex_u16(temp_Value);
//	SendString("\r\n");
	while((temp_Value < LVT_VALUE)&&!poweroff_falg)
	{
//	    SendString("begin close!\r\n");
		count++;
		if(count==10)
			{
//			 swap_code(AP3_OPEN_FILE_CODE_FLAG);
//			 swap_code(FILE_EXTERN_CODE_FLAG);
			// tishi_mp3(2);
			// return ;
			 
#if 0			
			    key_scan.key_write_point = key_scan.key_read_point = 0;
				key_scan.key_word[key_scan.key_write_point].key_type = KBD_KEY_LONGPRESSED;
				key_scan.key_word[key_scan.key_write_point].key_num = KEY_POWER;
				key_scan.key_write_point++;
#else
#ifndef TX_DEBUG
				BlueLed_OFF();	
			    RedLed_ON();
#endif
				if( player_ap3_is_busy())
			 		player_ap3_stop();
			 		init_array_v();
//					OP_CLOSE();
					scan_oid_disable();
			 		Kbd_set_evt_disabled();
					poweroff_falg = TRUE;

//					 vol_idx = 1;
//					 swap_code(AP3_OPEN_FILE_CODE_FLAG);
					 tishi_mp3(2);
					
#endif	
				return;
			}
	}
}
#endif

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
#ifdef TX_DEBUG		
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
					   BlueLed_OFF();
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

   _MEM_TYPE_SLOW_ U16 temp_key=0;

	adc_get();

	if(sys_led_is_timer_timeout()){
		sys_led_start_timer(SYS_LED_ON_TIME);
		op_time_dowith();
		if(!player_ap3_is_busy())clk_time_dowith();
		if(CheckUSB())dowith_USB();
		if(BatteryDetect())dowith_bat();
		}
/*	
	if((g_timer_tick%(500/TICK_PERIOD) == 0))
    if(CheckUSB()){
		dowith_USB();
    	}

	if((KBD_IDLE == kbd_state)&&(g_timer_tick%(500/TICK_PERIOD) == 0))
		if(BatteryDetect()){
			dowith_bat();
			}
*/

#ifdef NIEQING_DIANCHI
//	 Charge_Check();
#endif	
 
   // Keypad process
   kbd_process();
   temp_key = kbd_key_read();
   if(0!=temp_key)
   	{                           
	g_b_autopoweroff_forbidden = TRUE; 
	//sysup_opstart();
	clkup_time(TRUE);
   	Ap4_Key_Process(temp_key);
   	}
   scan_oid();
   
	if(b_low_battary){
		}
	else{
		 ledFlash();
		}

     search_evt();
}





void ready_mount_sd(void)
{
	FS_ERR fserr;
#ifdef TX_DEBUG
		SendString(( char *)"\r\nbegin mount sd!");
#endif		
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
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 ok!");
#endif		
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
		}
#endif
}


void mmgr_task_init( void )
{

	FS_ERR fserr;
	int drive;
	unsigned int i,j;
	unsigned int ch0;
	unsigned int ch2;
	char temp_lab[12];

	//swap_code_flag = 0;
	//code_flag = 0;
//	dic_flag = 1 ;
 	//SetVector(EXCEPTION_TRAP,Exception);

		ADC_init();
//	PowerKeepOn();


 //  	 PWR_ON();
 //    OID_PWR_ON();  
	 BlueLed_ON();
     RedLed_OFF();
    //g_timer_tick = 0;
	//battery_count  = 0;
	//System_Timer_Init(3,TICK_PERIOD); 
	SDKLoadCode(SYSTEM_TIMER3_INIT_CODE);
	System_Timer3_Init();
//	swap_code(IMA_CODEC_CODE_FLAG);
//	swap_code(AP3_OPEN_FILE_CODE_FLAG);
// 	swap_code(FILE_EXTERN_CODE_FLAG);

#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB dic begin!\r\n");
#endif

/*
#ifdef UNCOIDE_FORMAT
	nav_string_unicode();
#endif

#ifdef ASCII_FORMAT
	nav_string_ascii();
#endif	
*/
	
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
//	SD1_fast_mode();



	
#ifdef NIEQING_DIANCHI
//	 Charge_Init();
#endif	

 	kbd_init_process();
    if(!b_oid_ok){
	    reinit_oid_pen();
		b_oid_ok = TRUE;
		}
	   bPenOn = FALSE;
	   index_flag = FALSE;
	   scan_oid_disable();

    	tishi_mp3(FLAG_GO_DIC);


  // file_init();



   //first_go_ap4 = 0;
  // 	Addr_Fanyi = 0;
   g_b_autopoweroff_forbidden = FALSE;
   Search_Record.news_sys.flag_index = FALSE;
	flag_ap4_play_pause = 0;
	g_b_player_on = FALSE;
//	flag_wav_play = FALSE;
	bRemoteChangeAp3 = FALSE;
	index_flag = FALSE;
	OID_PLAY = TRUE;
//	dic_play = 0 ;
//	init_check_sd();
	ap3_mmi_appli();
	clkup_time(TRUE);
	SetVector(EXCEPTION_TRAP,Exception);
///	if(gflag & 0x01 == 0x01)
	//	Charge_Init();
}
#endif // _mmgr_kernel_c_
