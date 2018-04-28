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
//#include <sfr.h>
#include "system.h"
//#include "SFR.h"
#include "ADC.h"
//#include "KEY_SCAN.H"
#include "com.h"
#include "mp3_api.h"
//#include "USB_Detect.h"
//------------------------------------------------------------------------------
#pragma CODE= SYSTEM_CODE
#pragma DATA= SYSTEM_DATA

//------------------------------------------------------------------------------
//void LED_Callback(void);

//------------------------------------------------------------------------------
U32 g_timer_tick;
U32 mp3_begin_add,play_file_size;
U8 gOidXOR_Key;
unsigned long led_timer;
U32			system_check_timer;
U32 _MEM_TYPE_SLOW_ op_close_timer;
U32 _MEM_TYPE_SLOW_ rec_led_timer;
U32 _MEM_TYPE_SLOW_ sys_led_timer;
__X   U32   Start_Sector, Length;
__X   U8    Buf[ 512 ];
unsigned int DAC_Out; // 0: Headphone, 1: Speaker
//unsigned int LED_param;
U8 battery_count;
U8 adc_channel;
U32 adc_timer;
unsigned int vol_idx_bak;
//const int VolTbl[5] = {7,13,19,25,29}; 
//const int VolTbl[VOL_NUM] = {4,8,12,16,20,24,28,31};
const int VolTbl[VOL_NUM] = {7,13,20,28,31};
const int SpeakerTbl[VOL_NUM] = {4,5,6,8,10};

unsigned int speaker_vol;

unsigned int sleep_mode_count;
 _MEM_TYPE_SLOW_  U32                  Current_Index; 
 _MEM_TYPE_SLOW_  U32                  changedIndex; 


Bool flag_wav_play;
__X FCFILE *ADPCM_File; 
Bool flag_creat;
//#define _USBCON_b0 ((_bitref)0x410)
_MEM_TYPE_SLOW_   Bool  g_b_autopoweroff_forbidden;   // Modified by tasks except to mmi manager task
_MEM_TYPE_SLOW_   Bool  g_b_poweroff_activated;
_MEM_TYPE_SLOW_   U32   g_autopoweroff_timer;
unsigned int heigh_num;
unsigned int low_num;
unsigned long qieqing_timer;	
unsigned long sleep_mode_timer;
unsigned long g_system_clk_timer;
Bool flag_tishi;
Bool OID_PLAY;
#if 0//def NIEQING_DIANCHI
 _MEM_TYPE_SLOW_ U32 nieqing_time;
_MEM_TYPE_SLOW_ U8 nieqing_count;
#endif

//extern unsigned char key_mode_flag;
extern void DAC_Stereo_to_Mono(unsigned Option);
extern void save_wav_num(void);
//------------------------------------------------------------------------------
void system_reset(void)
{
#ifdef TX_DEBUG
	SendString("\r\nsystem reset!");
#endif
	BlueLed_ON();
	RedLed_OFF();
	PowerTurnOff();
	RTCWDT = 0xBFFFU;
	while(1)
	{
	}
}

void __interrupt Exception(void)
{
    system_reset();	
}

//! @brief timer_get_time.
//!
//! This function just give the current tick value.
//!
//! @param  none
//! @return the time
U32 timer_get_time( void )
{
   U32 time;
   
//   Int_t3_disable();
   time = g_timer_tick;
//   Int_t3_enable();
   return time;
}

//! @brief timer_check_timeout
//!
//! This function test if a timeout occured on the timer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U32 timer: variable used for timer to compare
//!
//! @return TRUE if timeout occured, else FALSE.
Bool timer_check_timeout(U32 timer)
{
   if( timer_get_time()>=timer ) { return TRUE;  }
   else                          { return FALSE; }
}



///*
#if(CHECKT_SD_DET == ENABLE)	
void __interrupt pb_sd_isr( void )
{
#ifdef TX_DEBUG
	SendString("\r\nsdremove!");
#endif
   if( sd_det_pif())
   {
    //  clr_sd_det_pif();
    //  if( b_card_state == Mmc_card_present())
    //     return;
      PBICR = 0x00;
     // sd_det_disable();
      //RESET_SYS();
      //RTCWDT = 0xBFFFu;
      system_reset();
   }
   PBPIF = 0;
}
#endif
//*/

//================================================================================
/*
void __interrupt Exception(void)
{
#ifdef TX_DEBUG
	SendString("\r\nException");
#endif
	system_reset();
}
*/
//------------------------------------------------------------------------------


#if 1

void __interrupt TIMER0_ISR(void)
{
	g_timer_tick++;
}
/*
//=====================================================================
void judge_op(void)
{
	if(SpeakerDET()){
		//MUT_OPEN();
		OP_CLOSE();
		//EX_RC_EN_CLOSE();
		
		}
	else{
		OP_OPEN();
		//EX_RC_EN_OPEN();
		//MUT_CLOSE();
		}
}
void SpeakDetect(void)
{

  int temp_vol;
  judge_op();
  if(vol_idx!=vol_idx_bak){

  		PCMv2_VoiceVolume(vol_idx);
		vol_idx_bak = vol_idx;
  	}
}
*/
void adc_get(void)
{
  int value;
 if( !(Timer_is_timeout( adc_timer )))
   {
      return;
   }

   adc_timer = Timer_set_timer( TIMER_MS( 20 ));
   value = Get_ADCValue(( U16 )adc_channel );
#if 1
	if(adc_channel == 0){
		adc0_value = value;		
		adc_channel = 2;
		return;
		}
	
	if(adc_channel == 2){
		adc2_value = value;
		adc_channel = 3;
		return;
		}
	///*
	if(adc_channel == 3){
		adc3_value = value;
		adc_channel = 7;
		return;
		}
	//*/
	if(adc_channel == 7){
		adc7_value = value;
		adc_channel = 0;
		return;
		}
	adc_channel = 7;
#else
   switch(adc_channel)
   	{
   		case 0:
			adc0_value = value;
			adc_channel = 2;
			break;
		case 2:
			adc2_value = value;
			adc_channel = 3;
			break;
		case 3:
			adc3_value = value;
			adc_channel = 7;
			break;
		case 7:
			adc7_value = value;
			adc_channel = 0;
			break;
		default:
			adc_channel = 0;
			break;
		
   	} 
#endif	
}
//------------------------------------------------------------------------------

void  op_time_dowith( void )
{
	if(op_is_timer_timeout()){
		op_stop_timer();
		OP_CLOSE();
		}
}
void  clk_time_dowith( void )
{
	if(clk_is_timer_timeout()){
		clk_stop_timer();
		SysClkDowngrade();
		}
}
void vol_dec(void)
{
#if 1
	return;
#else
 unsigned int TempVolume,TempV;
  TempVolume=VolTbl[vol_idx];
       while(TempVolume>0)
        {
          for(TempV=0;TempV<=10000;TempV++)
             __asm("nop");
            TempVolume--;
			DAC_HeadPhone_Volume(TempVolume); 
        }	   
#endif		
}
void vol_inc(void)
{
#if 1
	return;
#else
 unsigned int TempVolume,TempV;
      TempVolume=0;
       while(TempVolume<VolTbl[vol_idx])
        {
          for(TempV=0;TempV<=10000;TempV++)
             __asm("nop");
            TempVolume++;
			DAC_HeadPhone_Volume(TempVolume); 
        }
#endif		
}

#define FIVE_MINUTES_COUNT   (6UL)
/*
void sleep_mode_dowith(void)
{
	sleep_mode_count ++;
	if(sleep_mode_count < 6){
		if(speaker_vol > 0){
			speaker_vol --;
			vol_idx_bak = VOL_NUM;
			}
		}
	else{
		//kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_POWER);
		}
}
*/
void judge_op(void)
{
	if(SpeakerDET()!=b_headphone_flag){
		vol_idx_bak = VOL_NUM;
		}
}
void inc_volume(Bool flag)
{

  	b_headphone_flag = SpeakerDET();
	
	if(!b_headphone_flag)
	{
		OP_CLOSE();
		//DAC_Speaker_Volume(4);
		DAC_Speaker_Volume(5);
		DAC_Stereo_to_Mono(0);
	}
    else
	{
		//DAC_HeadPhone_Volume(0);
		OP_OPEN();
		DAC_Stereo_to_Mono(1);
		//vol_inc();
		if(flag_wav_play){
			DAC_Speaker_Volume(WAV_SPEAKER_VOLUME_INCREASE);
			}
		else{	
			//if(b_sleep_mode){
			//	DAC_Speaker_Volume(SpeakerTbl[speaker_vol]);
			//	}
			//else{
				DAC_Speaker_Volume(MP3_SPEAKER_VOLUME_INCREASE);
			//	}
			}
	}	
}

void SpeakDetect(Bool flag)
{
  //int temp_vol;
  judge_op();
  if(vol_idx!=vol_idx_bak){
		vol_idx_bak = vol_idx;

		DAC_Disable_Speaker();
		DAC_Enable_HeadPhone();

		//if(b_sleep_mode){
		//	DAC_HeadPhone_Volume(VolTbl[0]);
		//	}
		//else{
			DAC_HeadPhone_Volume(VolTbl[vol_idx]);
			//}
		inc_volume(flag);
  	}
/*
//        DAC_Disable_Speaker();
//		DAC_Enable_HeadPhone();
		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
		judge_op();
		inc_volume();
//		DAC_Unmute();
*/
}


Bool  CheckUSB( void )
{
 if(b_usb)return FALSE;
 if(USB_DET()){
 	clkup_time(FALSE);
#ifdef TX_DEBUG
	SendString((char *)"\r\nfind usb!");
#endif 	
 	sys_led_stop_timer();
 	return TRUE;
 	}
 return FALSE;
}

void power_forbid_autopoweroff(void)
{
   g_b_autopoweroff_forbidden = TRUE;
}

//! power_authorize_autopoweroff
//!
//! Autorizes the auto power off by the system part
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return U8 : voltage level 0x00 -> 0x10
//!
void power_authorize_autopoweroff(void)
{
 
 g_b_autopoweroff_forbidden = FALSE;
 //g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(TISHI_TIME));
 g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME));
  g_b_wait_time = FALSE;
 g_b_tishi_2 = FALSE;

}



//=====================================================================
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
void SysClkUpgrade( void )
{
   if( g_b_system_clk_low )
   {
      SystemClock( FEQ_24M );
      SystemClock( SYS_CLOCK );
      SPIF_Set_Clock( SPI_30M );
      g_b_system_clk_low = FALSE;
#ifdef TX_DEBUG
	 SendString("\r\nsys up!");
#endif	  
   }
  // g_system_clk_timer = Timer_set_timer( SYS_CLK_LOW_TIME );
}


//------------------------------------------------------------------------------
void SysClkDowngrade( void )
{
   if( !g_b_system_clk_low )
   {
#ifdef TX_DEBUG
	 SendString("\r\nsys low!");
#endif	
      SPIF_Set_Clock( SPI_3M );
      __asm
      {
         BCLR MODCON.12
         REP #45
   		NOP
      }
  
      g_b_system_clk_low = TRUE;
   }
  // g_system_clk_timer = Timer_set_timer( SYS_CLK_LOW_TIME );
}

void clkup_time(Bool flag_on)
{
	SysClkUpgrade();
	if(flag_on){
		clk_start_timer();
		//clk_stop_timer();
		}
	else{
		clk_stop_timer();
		}
}
//=====================================================================
#pragma CODE = SYSTEM_TIMER3_INIT_CODE

#if(CHECKT_SD_DET == ENABLE)	
void check_sd(void)
{
 	sd_det_disable();
	_bitSET( SD_DET_DIR );
  	_bitSET( SD_DET_PH );
	PBPIF = 0x00;
   b_card_state = Mmc_card_present();
   sd_det_enable();
   if( !b_card_state )
      sd_det_rise();
   else
      sd_det_fall();
   PBICR = 0x31;
   SetVector( PORT_B_INT_TRAP, pb_sd_isr );
}

#endif

void time3_init(void)
{
#if 1
 	SetVector(TIMER3_INT_TRAP,TIMER0_ISR);
 	TMR3CR = 0xD005;	    // Clock = 12000, Div = 32
 	//TM3PRD = 9675; 		// 50ms*24M/128
 	TM3PRD = 750; 		// 2ms*24M/128
 	TM3ICR = 0x01;
	g_timer_tick = 0;
#else
	
 	SetVector(TIMER0_INT_TRAP,TIMER0_ISR);
 	TMR0CR = 0xD005;	// Clock = 12M, Div = 32
// 	TM3PRD = 9675; 		// 50ms*12M/128
 	TM0PRD = 750; 		// 2ms*12M/128
 	TM0ICR = 0x01;
	g_timer_tick = 0;
//	g_key_clock = 0;
 
#endif	
}



void System_Timer3_Init(void)
{
	time3_init();
#if(CHECKT_SD_DET == ENABLE)	
	check_sd();
#endif	
	b_headphone_flag = SpeakerDET();
	battery_count = 0;
	adc_channel = 0;
    adc_timer = 1;
	
	//system_check_timer = 1;
	sys_led_stop_timer();
	sys_led_start_timer(SYS_LED_ON_TIME);
	flag_tishi = FALSE;
	op_stop_timer();
	rec_led_stop_timer();
	rec_led_start_timer(REC_LED_OFF_READ_TIME);
	flag_tishi = TRUE;
	
	sleep_mode_count = 0;
	sleep_mode_stop_timer();
	//b_2min_tishi = FALSE;
	  
}
#else

void __interrupt TimerISR(void)
{
	g_timer_tick++;
//	if((g_timer_tick%(500/TICK_PERIOD) == 0)&&!bPenOn)
//	  	BlueLed_FLASH();
}
//---------------------------------------------------------------
void System_Timer_Stop(int TimerSource)
{
    *((__X unsigned int *)TimerSourceTBL[TimerSource][1]) = 0x0u;  
}

//------------------------------------------------------------------------------

void System_Timer_Init(int TimerSource,
                       unsigned int time)
{
//	g_timer_tick = 0;
	// clock source is 12Mhz, pre-scale is divided by 32.
    *((__X unsigned int *)TimerSourceTBL[TimerSource][1]) = 0xD005u;//0xD005u;
    *((__X unsigned int *)TimerSourceTBL[TimerSource][2]) = TMRxPRD(time);
	if(TimerSource==4){    
    	SetVector(TimerSourceTBL[TimerSource][3],TimerISR);
    	}
/*	
	if(TimerSource==2){    
    	SetVector(TimerSourceTBL[TimerSource][3],TimerISR_ADPCM);
    	}
    if(TimerSource==3){    
    	SetVector(TimerSourceTBL[TimerSource][3],TimerISR_RP);
    	}	
//*/	
    *((__X unsigned int *)TimerSourceTBL[TimerSource][0]) = 0x0001;    
}

#endif

//--------------------------------------------------------------------
//#pragma CODE = USBCHECK_CODE

//--------------------------------------------------------------------
#pragma CODE = BATTERY_CODE
/*
Bool tishi_BatteryDetect(void)
{
	unsigned int temp_Value;
	U16  temp_data;
	if(USB_DET())return FALSE;
	if(flag_tishi)return FALSE;
	//if(battery_count>0x7f)return FALSE;
	//temp_Value = Get_ADCValue(0);
	//if (temp_Value == (-1))return FALSE;
	temp_Value = (unsigned int)((adc0_value*100UL/adc7_value));
	if((temp_Value < LVT_TISHI_VALUE))
	{
		battery_count++;
		if(battery_count>=20)
			{
#ifdef TX_DEBUG
				SendString((char *)"\r\nwarning battery!");
#endif
				flag_tishi = TRUE;
				battery_count=0;
				return TRUE;
			}
	}
	else{
		battery_count=0;
		b_low_battary = FALSE;
		}
	return FALSE;
}
*/
Bool BatteryDetect(void)
{
	unsigned int temp_Value;
	U16  temp_data;
	if(USB_DET())return FALSE;
	//if(!flag_tishi)return FALSE;
	//if(battery_count>0x7f)return FALSE;
	//temp_Value = Get_ADCValue(0);
	//if (temp_Value == (-1))return FALSE;
	temp_Value = (unsigned int)((adc0_value*100UL/adc7_value));
	if((temp_Value < LVT_VALUE))
	{
		battery_count++;
		if(battery_count>=20)
			{
#ifdef TX_DEBUG
				SendString((char *)"\r\nlow battery!");
#endif
				clkup_time(FALSE);
				BlueLed_OFF();	
			    RedLed_ON();
				sys_led_stop_timer();
				b_low_battary = TRUE;
				//battery_count=0x80;
				return TRUE;
			}
	}
	else{
		battery_count=0;
		b_low_battary = FALSE;
		}
	return FALSE;
}

//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------
