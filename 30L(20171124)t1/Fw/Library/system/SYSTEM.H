/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: SYSTEM.H
 * Author		: Nick
 * Create Date	: December 2, 2008
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <KBDM9_DDK.H>
#include "compiler.h"
#include "conf_kb.h"
#include "com.h"
#include "globalvariable.h"
#include "power.h"
#include "GPIO_bitref.H"
#include "BinaryImage.h"

#define TICK_PERIOD              2
#define TIME_TICK 	TICK_PERIOD
#define VOL_NUM		(5)

#define SYS_CLOCK	FEQ_60M

#define ROOT_DENTRY		"0:"

#define ROOT			"0:\\"
#define SUBDIR			"SUBDIR"

#define AP4_EXT			"MEP"
//#define AP4_EXT			"WYX"
#define WAV_EXT			"WAV"
#define MP3_EXT	 		"MP3"
#define DIC_EXE			"DIC"
#define DIYDIR			"DIY"
#define POINT_MP3_EXT	".MP3"

#if 0
#define SD1_AP4_DIR 	"0:\\AP4~1"
#define SD2_AP4_DIR 	"1:\\AP4~1"
#define SD1_MP3_DIR 	"0:\\MP3~1"
#define SD2_MP3_DIR 	"1:\\MP3~1"
#else
#define SD1_AP4_DIR 	"0:\\AP4"
#define SD2_AP4_DIR 	"1:\\AP4"
#define SD1_MP3_DIR 	"0:\\MP3"
#define SD2_MP3_DIR 	"1:\\MP3"
#endif

#define	 SD1_DIC_DIR  			"0:\\DIC"
#define  SD1_DEFAULT_DIC_FILE 	"0:\\DIC\\DEFAULT.DIC"

#define	 SD2_DIC_DIR  			"1:\\DIC"
#define  SD2_DEFAULT_DIC_FILE 	"1:\\DIC\\DEFAULT.DIC"

#define SD1_STORY_FILE 	"0:\\STORY"
#define SD2_STORY_FILE 	"1:\\STORY"

#define SD1_BOOK_SYS 	"0:\\BOOK.SYS"
#define SD2_BOOK_SYS 	"1:\\BOOK.SYS"

#if 0
#define SD1_PASTER_DIR  "0:\\REC~1\\"
#define SD2_PASTER_DIR  "1:\\REC~1\\"

#define SD1_PASTER_DENTRY  "0:\\REC~1"
#define SD2_PASTER_DENTRY  "1:\\REC~1"

#else
#define SD1_PASTER_DIR  "0:\\RECORD\\"
#define SD2_PASTER_DIR  "1:\\RECORD\\"

#define SD1_PASTER_DENTRY  "0:\\RECORD"
#define SD2_PASTER_DENTRY  "1:\\RECORD"
#endif
///*
//#define INDEX_NORMAL_MIN		1
//#define INDEX_NORMAL_MAX		59999

#define INDEX_BEGIN_RECORD		(U16)(61499)
#define INDEX_BEGIN_PLAY		(U16)(61498)

#define ALL_OFFSET				(U32)(0x2000)


#define  NO_AP4_FILE       	"noap4.mp3"
#define  SYS_OPEN_FILE		"startup.mp3"
#define  BATTERY_WARN_FILE	"batwarning.mp3"
#define  BATTERY_LOW_FILE	"batterylow.mp3"
#define  POWER_OFF_FILE		"poweroff.mp3"
#define  RECORDSTART		"recordstart.mp3"
#define  RECORDEND			"recordend.mp3"
#define  PLAYSTART			"playstart.mp3"
#define  PLAYEND			"playend.mp3"
#define  DISKFULL			"diskfull.mp3"
#define  SIMPLE				"simple.mp3"
#define  NO_FINDAP4			"notfindap4.mp3"
#define  REREAD				"reread.mp3"
#define  MIN_TISHI			"waitprompt.mp3"
#define  STOP_MP3			"stop.mp3"
#define  REPEAT_MP3			"repeat.mp3"
#define  REREAD_MP3			"reread.mp3"
#define  VOLUME_MP3			"vol.mp3"
#define  MINVOL_MP3			"minvolume.mp3"
#define  MAXVOL_MP3			"maxvolume.mp3"
#define  MP3_FILE			"mp3player.mp3"
#define  NO_MP3				"nomp3file.mp3"
#define  GO_DIC				"dictionary.mp3"
#define  NO_DIC				"nodic.mp3"
#define  ERROR_FILE			"error.mp3"
#define  STORY_FILE			"story.mp3"
#define  NO_STORY			"nostory.mp3"



#define O_BEGIN		 ( 0 )

#define FLAG_NOAP4FILE	  				( O_BEGIN + 0 )
#define FLAG_SYS_OPEN_FILE	  			( O_BEGIN + 1 )
#define FLAG_BATTERY_LOW_FILE	  		( O_BEGIN + 2 )
#define FLAG_BATTERY_WARN_FILE	  		( O_BEGIN + 3 )
#define FLAG_RECORDSTART	  			( O_BEGIN + 4 )
#define FLAG_RECORDEND	  				( O_BEGIN + 5 )
#define FLAG_PLAYSTART	  				( O_BEGIN + 6 )
#define FLAG_PLAYEND	  				( O_BEGIN + 7 )
#define FLAG_DISKFULL	  				( O_BEGIN + 8 )
#define FLAG_SIMPLE	  					( O_BEGIN + 9 )
#define FLAG_NO_FINDAP4	  				( O_BEGIN + 10 )
#define FLAG_MIN_TISHI	  				( O_BEGIN + 11 )
#define FLAG_STOP_MP3	  				( O_BEGIN + 12 )
#define FLAG_POWER_OFF_FILE	  			( O_BEGIN + 13 )
#define FLAG_GO_DIC						( O_BEGIN + 14 )
#define FLAG_GO_MP3						( O_BEGIN + 15 )
#define FLAG_NO_MP3FILE					( O_BEGIN + 16 )
#define FLAG_VOLUME						( O_BEGIN + 17 )
#define FLAG_NOAPF						( O_BEGIN + 18 )
#define FLAG_RECORDING					( O_BEGIN + 19 )
#define FLAG_MINVOL						( O_BEGIN + 20 )
#define FLAG_MAXVOL						( O_BEGIN + 21 )
#define FLAG_REREAD						( O_BEGIN + 22 )
#define FLAG_REPEAT						( O_BEGIN + 23 )
#define FLAG_NODIC						( O_BEGIN + 24 )
#define FLAG_ERROR						( O_BEGIN + 25 )
#define FLAG_GO_STORY					( O_BEGIN + 26 )
#define FLAG_NO_STORYFILE				( O_BEGIN + 27 )

	
#define INDEX_INC_VOL			(U16)(60001)
#define INDEX_DEC_VOL			(U16)(60002)
#define INDEX_STOP				(U16)(60003)

#define INDEX_NEXT_BOOK			(U16)(60004)
#define INDEX_PREV_BOOK			(U16)(60005)
#define INDEX_REREAD			(U16)(60007)
#define INDEX_FUDU				(U16)(60008)


#define INDEX_GO_MP3			(U16)(61505)
#define INDEX_PLAY_MP3			(U16)(61502)
#define INDEX_PAUSE_MP3			(U16)(61501)
#define INDEX_STOP_MP3			(U16)(61500)
//#define INDEX_PAUSE_PLAY

#define INDEX_NEXT_MP3			(U16)(61503)
#define INDEX_PREV_MP3			(U16)(61504)

#define INDEX_START_RECORD		(U16)(61499)
#define INDEX_PLAY_RECORD		(U16)(61498)

#define INDEX_DEL_RECORD		(U16)(65347)

#define INDEX_AP4_PAUSEPLAY		(U16)(61520)
#define INDEX_AP4_SXPLAY		(U16)(61521)
#define INDEX_GO_AP4			(U16)(65388)

#define INDEX_GO_DIC			(U16)(65396)

#define INDEX_FIRST_VOL			(U16)(61506)
#define INDEX_LAST_VOL			(U16)(61510)

//#define INDEX_GO_GAME_MIN		(U16)(60200)
//#define INDEX_GO_GAME_MAX		(U16)(60600)

#define INDEX_EXIT_GAME			(U16)(61439)

#define INDEX_LANG_MIN			(U16)(60011)
#define INDEX_LANG_MAX			(U16)(60025)

#define INDEX_CHANGE_AP4_MIN	(U16)(62000)
#define INDEX_CHANGE_AP4_MAX	(U16)(65534)
#define RANG_CHANGE_AP4			(U16)(65534-62000)


#define INDEX_NORMAL_MIN		(U16)(62000)
#define INDEX_NORMAL_MAX		(U16)(63999)
#define BEGIN_NORMAL			(INDEX_NORMAL_MIN - INDEX_CHANGE_AP4_MIN)
#define OVER_NORMAL				(INDEX_NORMAL_MAX - INDEX_CHANGE_AP4_MIN)
#define RANGE_NORMAL			(OVER_NORMAL - BEGIN_NORMAL + 1)
#define INDEX_NORMAL_NEXT		(U16)(60004)
#define INDEX_NORMAL_PREV		(U16)(60005)

#define INDEX_RECITATIONS_MIN		(U16)(64000)
#define INDEX_RECITATIONS_MAX		(U16)(64799)
#define BEGIN_RECITATIONS			(INDEX_RECITATIONS_MIN - INDEX_CHANGE_AP4_MIN)
#define OVER_RECITATIONS			(INDEX_RECITATIONS_MAX - INDEX_CHANGE_AP4_MIN)
#define RANGE_RECITATIONS			(OVER_RECITATIONS - BEGIN_RECITATIONS + 1)
#define INDEX_RECITATIONS_NEXT		(U16)(60026)
#define INDEX_RECITATIONS_PREV		(U16)(60027)

#define INDEX_TRANSLATIONS_MIN		(U16)(64800)
#define INDEX_TRANSLATIONS_MAX		(U16)(64999)
#define BEGIN_TRANSLATIONS			(INDEX_TRANSLATIONS_MIN - INDEX_CHANGE_AP4_MIN)
#define OVER_TRANSLATIONS			(INDEX_TRANSLATIONS_MAX - INDEX_CHANGE_AP4_MIN)
#define RANGE_TRANSLATIONS			(OVER_TRANSLATIONS - BEGIN_TRANSLATIONS + 1)
#define INDEX_TRANSLATIONS_NEXT		(U16)(60028)
#define INDEX_TRANSLATIONS_PREV		(U16)(60029)



#define INDEX_EXPLANTION_MIN		(U16)(65000)
#define INDEX_EXPLANTION_MAX		(U16)(65299)
#define BEGIN_EXPLANTION			(INDEX_EXPLANTION_MIN - INDEX_CHANGE_AP4_MIN)
#define OVER_EXPLANTION				(INDEX_EXPLANTION_MAX - INDEX_CHANGE_AP4_MIN)
#define RANGE_EXPLANTION			(OVER_EXPLANTION - BEGIN_EXPLANTION + 1)
#define INDEX_EXPLANTION_NEXT		(U16)(60030)
#define INDEX_EXPLANTION_PREV		(U16)(60031)

#define INDEX_OTHER_MIN			(U16)(65300)
#define INDEX_OTHER_MAX			(U16)(65535)
#define BEGIN_OTHER				(INDEX_OTHER_MIN - INDEX_CHANGE_AP4_MIN)
#define OVER_OTHER				(INDEX_OTHER_MAX - INDEX_CHANGE_AP4_MIN)
#define RANGE_OTHER				(OVER_OTHER - BEGIN_OTHER + 1)
#define INDEX_OTHER_NEXT		(U16)(60032)
#define INDEX_OTHER_PREV		(U16)(60033)

//*/

#define WAV_RECORD_FLAG			0x1111
#define WAV_PLAY_FLAG			0x2222

#define FLAG_SD1_CON			0x5555
#define FLAG_SD2_CON			0xAAAA

#define AREA_NORMAL				0x00
#define AREA_RECITATION			0x01
#define AREA_TRANSLATION		0x02
#define AREA_EXPLANTION			0x03
#define AREA_OTHER				0x04

#define OFFSET_NORMAL			(U16)(0x4000)
#define OFFSET_RECITATIONS		(U16)(0x6000)
#define OFFSET_TRANSLATIONS		(U16)(0x7000)
#define OFFSET_EXPLANATION		(U16)(0x8000)
#define OFFSET_OTHER			(U16)(0x9000)

#define  SYS_CLK_LOW_TIME        ( TIMER_MS( 4000 ))

enum{
	HEADER_PHONE = 0,
	SPEAKER	= 1
};
//_____ M A C R O S ________________________________________________________
// Select the oscillator clock divided by 2 as timer 0 clock input (X2 independent)
#define Int_enable()          _bitSET(_SOCR_b0+15)  // Enable SOCR.15
#define Int_disable()		  _bitCLR(_SOCR_b0+15)  // Disable SOCR.15
/*
#define _TM0ICR_b0             ((_bitref)0xD00U)
#define _TM1ICR_b0             ((_bitref)0xD10U)
#define _TM2ICR_b0             ((_bitref)0xD20U)
#define _TM3ICR_b0             ((_bitref)0xD30U)
#define _TM4ICR_b0             ((_bitref)0xD40U)
*/
#define Int_t0_disable()      _bitCLR(_TM0ICR_b0+0);
#define Int_t0_enable()       _bitSET(_TM0ICR_b0+0);
#define Int_t1_disable()      _bitCLR(_TM1ICR_b0+0);
#define Int_t1_enable()       _bitSET(_TM1ICR_b0+0);
#define Int_t2_disable()      _bitCLR(_TM2ICR_b0+0);
#define Int_t2_enable()       _bitSET(_TM2ICR_b0+0);
#define Int_t3_disable()      _bitCLR(_TM3ICR_b0+0);
#define Int_t3_enable()       _bitSET(_TM3ICR_b0+0);
#define Int_t4_disable()      _bitCLR(_TM4ICR_b0+0);
#define Int_t4_enable()       _bitSET(_TM4ICR_b0+0);

//_____ M A C R O S ________________________________________________________

//! Timer_set_timer
//! Set a timer variable.
//! Ex: my_timer = Timer_set_timer(TIMER_4_S);  
//! @param  delay: delay in ms before timeout
//! @return value to store in the timer variable (x bits)
#define Timer_set_timer(delay)  (timer_get_time() + (U32)(delay) +1)

//! Timer_is_timeout
//! Test if a timeout occurs on a timer.
//! Ex:  if (Timer_is_timeout(my_timer,TIMER_1_S))    // action 
//!      else // wait
//! @param  timer: variable used for timer
//!         delay: delay in ms to compare
//! @return TRUE if timeout occured, else FALSE.
#define Timer_is_timeout(timer)         ( timer_check_timeout(timer) )

//! Timer_reset_timer
//! Force a timeout on timer.
//! Ex:  my_timer = Timer_set_timer(TIMER_1_S);
//!      Timer_reset_timer(my_timer);
//!      if (Timer_is_timeout(my_timer))
//!         ... // Always true due to Timer_reset_timer
//! @param  timer: variable used for timer
//! @return the new value of timer
#define Timer_reset_timer(timer)   ((timer) = timer_get_time())

//! To ease the setting of timers
#define TIMER_MS(x)    ((U32)(x)/(TICK_PERIOD))         //! Wait for 'x' ms
#define TIMER_S(x)     ((U32)(x)*1000/(TICK_PERIOD))    //! Wait for 'x' sec
#define TIMER_MIN(x)   ((U32)(x)*60000/(TICK_PERIOD))   //! Wait for 'x' minutes


extern U32 _MEM_TYPE_SLOW_ rec_led_timer;
#define REC_LED_ON_QUICK_TIME        30
#define REC_LED_ON_NORMAL_TIME       500
#define REC_LED_OFF_NORMAL_TIME      1200//900
#define REC_LED_ON_READ_TIME         50
#define REC_LED_OFF_READ_TIME        50

#define REC_OID_FLASH_TIME			 REC_LED_ON_READ_TIME

#define rec_led_start_timer( value ) ( rec_led_timer = Timer_set_timer( TIMER_MS( value )))
#define rec_led_stop_timer()         ( rec_led_timer = 0 )
#define rec_led_is_timer_timeout()   ( rec_led_timer && Timer_is_timeout( rec_led_timer ))
#define rec_led_is_timer_stopped()   ( rec_led_timer == 0 )


extern U32 _MEM_TYPE_SLOW_ sys_led_timer;
#define SYS_LED_ON_TIME    					100
#define SYS_LED_OFF_TIME   					300
#define sys_led_start_timer( value )         ( sys_led_timer = Timer_set_timer( TIMER_MS( value )))
#define sys_led_stop_timer()                 ( sys_led_timer = 0 )
#define sys_led_is_timer_timeout()           ( sys_led_timer && Timer_is_timeout( sys_led_timer ))
#define sys_led_is_timer_stopped()           ( sys_led_timer == 0)


extern U32 _MEM_TYPE_SLOW_ op_close_timer;

#define OP_CLOSE_TIME   3000
#define op_start_timer( )         			( op_close_timer = Timer_set_timer( TIMER_MS( OP_CLOSE_TIME )))
#define op_stop_timer()                 	( op_close_timer = 0 )
#define op_is_timer_timeout()           	( op_close_timer && Timer_is_timeout( op_close_timer ))
#define op_is_timer_stopped()           	( op_close_timer == 0)


extern unsigned long qieqing_timer;	
#define QIEQING_HALF_TIME    				 100
#define QIEQING_TIME    				     200

#define qieqing_start_timer( value )         ( qieqing_timer = Timer_set_timer( TIMER_MS( value )))
#define qieqing_stop_timer()                 ( qieqing_timer = 0 )
#define qieqing_is_timer_timeout()           ( qieqing_timer && Timer_is_timeout( qieqing_timer ))
#define qieqing_is_timer_stopped()           ( qieqing_timer == 0)


extern unsigned long sleep_mode_timer;	
#define sleep_mode_start_timer( value )         ( sleep_mode_timer = Timer_set_timer( TIMER_MIN( 5 )))
#define sleep_mode_stop_timer()                 ( sleep_mode_timer = 0 )
#define sleep_mode_is_timer_timeout()           ( sleep_mode_timer && Timer_is_timeout( sleep_mode_timer ))
#define sleep_mode_is_timer_stopped()           ( sleep_mode_timer == 0)


extern unsigned long g_system_clk_timer;


#define clk_start_timer( )         			( g_system_clk_timer = Timer_set_timer( SYS_CLK_LOW_TIME ))
#define clk_stop_timer()                 	( g_system_clk_timer = 0 )
#define clk_is_timer_timeout()           	( g_system_clk_timer && Timer_is_timeout( g_system_clk_timer ))
#define clk_is_timer_stopped()           	( g_system_clk_timer == 0)

/*
#define clear_bookarer()           			( bookarer_record = 0)
#define Set_normal_arer()           		( bookarer_record = bookarer_record|0x0001)
#define Set_recitations_arer()           	( bookarer_record = bookarer_record|0x0002)
#define Set_translations_arer()         	( bookarer_record = bookarer_record|0x0004)
#define Set_explanation_arer()           	( bookarer_record = bookarer_record|0x0008)
#define Set_other_arer()           			( bookarer_record = bookarer_record|0x0010)

#define Test_normal_arer()           		(bookarer_record&0x0001)
#define Test_recitations_arer()           	(bookarer_record&0x0002)
#define Test_translations_arer()         	(bookarer_record&0x0004)
#define Test_explanation_arer()           	(bookarer_record&0x0008)
#define Test_other_arer()           		(bookarer_record&0x0010)
*/

/*
extern  unsigned long led_timer;
#define LED_NORMAL_TIME       		625
#define LED_QUICKLY_TIME         	50
#define led_start_timer( value ) 	( led_timer = Timer_set_timer( TIMER_MS( value )))
#define led_stop_timer()         	( led_timer = 0 )
#define led_is_timer_timeout()   	( led_timer && Timer_is_timeout( led_timer ))
#define led_is_timer_stopped()   	( led_timer == 0 )


extern unsigned int gflag;
//extern unsigned long TimerCount;
extern const int VolTbl[];
extern unsigned int vol_idx;
extern unsigned int LED_param;
extern unsigned int adc0_value;
extern unsigned int adc2_value;
extern unsigned int adc7_value;
*/
extern U32 g_timer_tick;
extern unsigned int vol_idx_bak;
extern const int VolTbl[VOL_NUM];
extern U8 battery_count;
extern Bool flag_wav_play;
extern unsigned int DAC_Out; // 0: Headphone, 1: Speaker
extern U32			system_check_timer;
//extern unsigned long g_system_clk_timer;
extern unsigned int heigh_num;
extern unsigned int low_num;
extern Bool OID_PLAY;
extern Bool flag_creat;
extern __X FCFILE *ADPCM_File;   
extern U16		tishi_count;
extern U32 mp3_begin_add,play_file_size;
extern U8 gOidXOR_Key;

extern  _MEM_TYPE_SLOW_  U32                  Current_Index; 
extern  _MEM_TYPE_SLOW_  U32                  changedIndex; 

//extern _MEM_TYPE_SLOW_   U16   oid_index_type;
extern __X   U32   Start_Sector, Length;
extern __X   U8    Buf[  ];
//extern __Y   U8    Buf[  ];

extern   _MEM_TYPE_SLOW_   Bool  g_b_autopoweroff_forbidden;   // Modified by tasks except to mmi manager task
extern    _MEM_TYPE_SLOW_  Bool  g_b_poweroff_activated;
extern   _MEM_TYPE_SLOW_   U32   g_autopoweroff_timer;




ImageDeclareType(SYSTEM_TIMER3_INIT_CODE);



Bool    timer_check_timeout(U32);
U32     timer_get_time(      void);
void ErrHappen(int ErrCode,char *string);
Bool BatteryDetect(void);
Bool tishi_BatteryDetect(void);
//void GPIO_init(void);
Bool  CheckUSB( void );
void judge_op(void);
void SpeakDetect(Bool flag);
void Delay_ms(int count);
void inc_volume(Bool flag);
void vol_dec(void);
void vol_inc(void);
void __interrupt Exception(void);
void System_Timer3_Init(void);
void system_reset(void);
//void __interrupt TimerISR_RP(void);
//void __interrupt TimerISR_ADPCM(void);
void  power_forbid_autopoweroff        ( void );
void  power_authorize_autopoweroff     ( void );
void  power_rearm_autopoweroff_delay   (void);
void  op_time_dowith( void );
void adc_get(void);
void  SysClkUpgrade( void );
void  SysClkDowngrade( void );
void  sysup_opstart(void);
void  clk_time_dowith( void );
void  clkup_time(Bool flag_on);
#endif // _SYSTEM_H_
//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------
