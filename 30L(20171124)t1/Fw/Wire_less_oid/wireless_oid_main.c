

#pragma CODE= MAIN_CODE
#pragma DATA= MAIN_RAM
#include "KBDM9_DDK.h"
#include <string.h>
#include <sfr.h>
#include <string.h>
#include <ctype.h> 
#include "fs.h"
#include "ADC.h"
#include "power.h"
#include "SYSTEM.H"

#include "conf_kb.h"
#include "oid.h"
#include "keypad_drv.h"                      // Keypad definition
#include "keypad.h"
#include "FileSearch.h"
#include "BinaryImage.h"
#include "com.h"


//#include "WAV_API.H"
//#include "mp3_apf.h"
#include "wireless\wireless.h"
#include "wireless\E2radio.h"

//#include "timer_drv.h"

//#include "cis.h"
//#include "mbc_decode.h"
//#include "infrared.h"
//--------------------------------------------------------------------
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(MP3_COMMON_CODE);
#define MP3_PATH "0:\\MP3"
#define FIRST_MUSIC_NUMBER 0
#define  FS_ATTR_LFN_ENTRY    ((Byte)0x0F)   // LFN entry attribute

//#define MP3_FILE			"0:\\subdir\\mp3playe.mp3"
//#define POWEROFF_FILE		"0:\\subdir\\poweroff.mp3"
//#define BATTERY_FILE		"0:\\subdir\\batteryl.mp3"

#ifdef UNCOIDE_FORMAT
const  U16      root[] = {'0',':','\\','\0'};
//const  U8       AP4[] = {0x70,0xB9,0x8B,0xFB,0x65,0x87,0x4E,0xF6,0x00,0x00};
const  U8		subdir[] = {0x7C,0xFB,0x7E,0xDF,0x65,0x87,0x4E,0xF6,0x00,0x00};
#endif

#ifdef ASCII_FORMAT
const  U8       root[] = "0:\\";
//const  U8       AP4[] = "ap4";
const  U8		subdir[] = "subdir";
#endif


#ifdef UNCOIDE_FORMAT
const U16 WIRELESS_MODE[] = {'w','i','r','e','l','e','s','s','.','m','p','3','\0'};
const U16 POWEROFF_FILE[] = {'p','o','w','e','r','o','f','f','.','m','p','3','\0'};
#endif

#ifdef ASCII_FORMAT
const U8 POWEROFF_FILE[] = "poweroff.mp3";
const U8 WIRELESS_MODE[] = "wireless.mp3";
#endif


/*
#define 	POWER_S_PRESS	64007
#define 	POWER_L_S_PRESS	64001
#define 	POWER_L_O_PRESS	64002

#define 	MODE_S_PRESS	64003
#define 	MODE_L_PRESS	

#define 	VOL_I_S_PRESS	64004
#define 	VOL_I_L_PRESS	

#define 	VOL_D_S_PRESS	64005
#define 	VOL_D_L_PRESS	

#define 	LOW_BATTAAY_V	64006

#define 	INFR_ADDR   68

#define     INFR_COM_OPEN  82
#define     INFR_COM_REC   01
*/

//unsigned char key_mode_flag;

#define PLAYING_MP3 1
#define PLAYING_WAV 2
U8 play_mode ;
//extern __X int Queue_Left;
//extern __X int Queue_Right;

//extern unsigned int ap4_mp3_flag;

//extern   unsigned int ap3_file_number;
//extern   unsigned int volume_inc_flag;
//extern   unsigned int remote_index_tab[16];
//extern unsigned int current_wav_num;
extern unsigned int ena_rec_mode;
extern unsigned int mp3_playing_num;
extern 	U32  packet_counter;
//extern unsigned int wav_playing_num;

extern Bool b_oid_init_flag;
extern Bool g_b_oid_data_flag;
extern  Bool   g_b_wireless_bind_flag;
//unsigned long g_timer_tick;
extern _MEM_TYPE_SLOW_  U16     Current_Index_bak;

unsigned char flag_auto_poweroff;
unsigned long auto_poweroff_timer;
unsigned int global_key_value ;
//_MEM_TYPE_SLOW_  U16     Current_Index;
//__X  Bool       		 index_flag;
__X  unsigned char  	 flag_lowpower;
__X	 unsigned int		 count_lowpower;
__X  Bool       		 mount_sd_falg;

//U8	count_battary;

//__X  unsigned int wav_number;
char tisiFileName[128];
unsigned int temp_string2[30];
void main_loop(void);
//void wireless_tishi_mp3(U8 flag);
U16  oid_remote(void);
//void save_wav_num(void);
void ex_rc_init(void);
//extern char GetID(void);
//extern void init_getid(void);

//extern void  delay1ms    ( int count );
//extern void  delay10us   ( void );
//extern void  delay100us  ( void );
//#define delay3us()      _nop_()//;_nop_();_nop_()
//extern void  delay3us	  ( void );
extern MOUSE_STATES mouse_state;
extern void protocol_init(void);
void Key_RF_Process(INT16U Key_Code);


extern void wireless_task_init( void );
//extern void judge_op(void);
//extern void DAC_Stereo_to_Mono(unsigned Option);




void main(void)
{	
	tSDKLoadCodeStatus LoadCodeStatus;

	IP_INIT();
// 	GPIO_init();
 	ADC_init();
//	OP_CLOSE();
//	PowerKeepOn();
	SystemClock(FEQ_60M);	
//	SystemClock(0x03);	//0x02---------80M 0x03--------60M
	SDKLoadCodInit();	
//	key_mode_flag = TRUE;    
	flag_auto_poweroff = FALSE;
//	key_begin_flag = FALSE;
	//LoadCodeStatus=SDKLoadCode(TEST_IRAM);
	BlueLed_ON();
	RedLed_ON();
//	ex_rc_init();
//	PowerKeepOn();
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"KB wireless begin!\r\n");
#endif
/*
	while(1){
		SendString(( char *)"\r\npower=");
		send_hex_u16(Get_ADCValue(0));
		delay1ms(2000);
		}
	*/
	/*
	while(1){
	   	delay1ms(1000); 
	   	RedLed_FLASH();
	   	}

	*/
	main_loop();
}


//#pragma CODE = MAIN_LOOP_CODE
void Main_system_init(void);
void Key_Process(INT16U Key_Code);
//Bool PlayMP3Music(void);
//void StopMP3Music(void);
//void play_mp3_file(void);
//extern void WavPlayStart(void);
//extern int set_vol_;
//extern Bool wav_flag ;
//extern __X U16 playwav_num;
//U8 wav_status ;

//extern Bool bPenOn ;
void main_loop(void)
{
	U32 close_time;
	tSDKLoadCodeStatus LoadCodeStatus;
	
    unsigned int i;
	unsigned int  temp_key=0;
	
	unsigned char temp_data = 0;
	unsigned int temp_count=0;
    flag_lowpower = 0;
	count_lowpower=0;
//	LoadCodeStatus = SDKLoadCode(DAC_COMMON_CODE);
//	LoadCodeStatus = SDKLoadCode(MP3_COMMON_CODE);
	//LoadCodeStatus = SDKLoadCode(TEST_IRAM);
#if 0
	init_infrared();
//	test_38K();
	i=0;
	while(i<10){
//		/*
			Infrared_Send_data(INFR_ADDR,INFR_COM_OPEN);
			delay1ms(1000);
			BlueLed_FLASH();
			i++;
//			*/
			}
//		Clr_red_clk();
#ifdef  TX_DEBUG	
	SendString((char *)"\r\nlow!");
#endif
//		delay1ms(1000);
//		Set_red_clk();
//		delay1ms(1000);
//		}
#endif
	Main_system_init();
//	wireless_tishi_mp3(0);
//	_bitCLR(RF_RST_OD);

    mouse_state = MOUSE_STATE_DISCONNECTED;   
   
    protocol_init();
#ifdef  TX_DEBUG	
	SendString((char *)"\r\nafter protocol_init!");
#endif
	rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
	BlueLed_ON();
	RedLed_ON();
	global_key_value = 0 ;
	flag_auto_poweroff = FALSE ;

	g_b_oid_data_flag=FALSE;
	scan_oid_enable();

/*	Current_Index = POWER_L_S_PRESS;

    while(1){
		
			delay1ms(100);
		    if(BatteryDetect())
			{
#ifdef  TX_DEBUG	
			SendString((char *)"\r\ngame over!");
#endif			
			 flag_lowpower = 0x55;
			}
    	}
*/
//	Current_Index=10;

	while(1)
	{	
		 adc_get();
		 
	     if( b_oid_init_flag )
		 {
			//GetID();
			scan_oid();
#ifdef  TX_DEBUG	
			if(index_flag){
				index_flag = FALSE;
				SendString((char *)"\r\ncurrent_index=");
				send_hex_u16(Current_Index);
				if(64766 == Current_Index)//if(64766 == data_from_pen)
				   {
				     	g_b_wireless_bind_flag = TRUE;
				   }
				else{
						packet_counter++;
						g_b_oid_data_flag=TRUE;
					}
				
				}
#endif			
		 }

		 kbd_process();
   		 temp_key = kbd_key_read();
		
		 if(temp_key)
	 	 {
	 	 	flag_auto_poweroff = FALSE ;
	 		Key_RF_Process( temp_key);
	 	 }
		 
		 wireless_evt();
		 
		 if(temp_key)
	 	 {
	 	 	flag_auto_poweroff = FALSE ;
	 		Key_Process( temp_key);
	 	 }

///*
		 if((g_timer_tick%(1000/TICK_PERIOD) == 0)&&(flag_lowpower==0))
//		if(g_timer_tick%(1000) == 0)
		 {
#ifdef  TX_DEBUG	
//			SendString((char *)"\r\ng_timer_tick=");
//			send_hex_u32(g_timer_tick);
#endif			
			if(CheckUSB()){
				CommonLoadCode(C_USB);
				}
    		if(BatteryDetect())
			{
#ifdef  TX_DEBUG	
			SendString((char *)"\r\ngame over!");
#endif			
			 flag_lowpower = 0x55;
			//	AudioMP3Stop();
//				vol_idx = 1;
//				OP_CLOSE();
				
//				delay1ms(1100);

			}
		}
//*/


	//		RedLed_OFF();
	//		BlueLed_ON();
		if(flag_lowpower!=0){
			b_oid_init_flag=FALSE;
			Kbd_set_evt_disabled();
			//Delay_ms(300);
			//PowerTurnOff();
			//	while(1){
			//		Delay_ms(1000);
			//		}
			CommonLoadCode(C_POWEROFF);

	    /*	
			if(count_lowpower<2){
				g_b_oid_data_flag=TRUE;
				if(flag_lowpower==0x55){
					Current_Index = 64006;
					}
				else{
					Current_Index = POWER_L_O_PRESS;
					}
				packet_counter++;
				count_lowpower++;
				}
			else{
				PowerTurnOff();
				while(1){
					delay1ms(1000);
					}
				}
//			*/
			}

///*		
		if((!flag_auto_poweroff)|| index_flag)
		{
			flag_auto_poweroff = TRUE ;
			index_flag = FALSE ;
			auto_poweroff_timer = Timer_set_timer(TIMER_MIN(30));
		}
		if((g_timer_tick>=auto_poweroff_timer)&&flag_auto_poweroff)
    	{
    //    	AudioMP3Stop();
	//		tishi_mp3(1);
	//		OP_CLOSE();
			//PowerTurnOff();
	
			//if(USB_DET())
			//{
			//	CommonLoadCode(C_OPEN);
			//}
			
			//while(PWD_Key_Press());
			//Delay_ms(5);
			//CommonLoadCode(C_OPEN);
			CommonLoadCode(C_POWEROFF);
    	}  
//*/		

				if(rec_led_is_timer_timeout()){
					RedLed_OFF();
					BlueLed_FLASH();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);

					}

	}
}

//------------------------------------------------------------------------------


#if 1
//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
extern Bool g_b_kbd_data_flag;
void Key_RF_Process(INT16U Key_Code)
{
	unsigned int tmp_key_val;
//	if(change_mode_flag )
//		return ;
	 g_b_kbd_data_flag = FALSE;
	 g_b_oid_data_flag = TRUE;
//	 g_b_oid_data_flag = FALSE;
#ifdef  TX_DEBUG
	SendString((char *)"\r\nwf_key_code=");
	send_hex_u16(Key_Code);

#endif	

	if(g_b_oid_data_flag)
	{
		global_key_value = ( Key_Code << 8 ) | ( Key_Code >> 8) ;
		packet_counter ++ ;
	}

	
}

void Key_Process(INT16U Key_Code)
{
	unsigned int tmp_key_val;
//	if(change_mode_flag )
//		return ;
//	 g_b_kbd_data_flag = FALSE;
//	 g_b_oid_data_flag = TRUE;
//	 g_b_oid_data_flag = FALSE;
#ifdef  TX_DEBUG
	SendString((char *)"\r\nkey_code=");
	send_hex_u16(Key_Code);

#endif	
    switch(Key_Code)
    {
        case (KBD_KEY_PRESSED<<8|KEY_POWER): // Pause or Continue
#ifdef  TX_DEBUG				
			SendString((char *)"\r\npower release!");
#endif        

            break;    
        case (KBD_KEY_RELEASED<<8|KEY_POWER): // Pause or Continue
#ifdef  TX_DEBUG				
			SendString((char *)"\r\npower release!");
#endif        

			b_back_from_mp3 = TRUE;
			CommonLoadCode(C_OID);
            break;
			
        case (KBD_KEY_RELEASED<<8|KEY_MODE):
			
            break;

		 case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // Volume dec

            break;
		 case (KBD_KEY_RELEASED<<8|KEY_VOL_INC): // Volume inc

		 	break;
//		 case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_INC): // PRE
//         case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC): // Next
//		 case (KBD_KEY_LONGPRESSED<<8|KEY_MODE):
         case (KBD_KEY_LONGPRESSED<<8|KEY_POWER): // POWER
#ifdef  TX_DEBUG         
			SendString("\r\npoweroff");
#endif
			g_b_oid_data_flag = FALSE;
			flag_lowpower = 0xAA;
//			Current_Index=POWER_L_O_PRESS;
//			flag_auto_poweroff = TRUE ;
//			index_flag = FALSE ;
//			auto_poweroff_timer = 0;
/*			
			OP_CLOSE();
			BlueLed_OFF();
			RedLed_OFF();
			PowerTurnOff();
			while(PWD_key_check());
			delay1ms(5);
			CommonLoadCode(C_OPEN);
*/
            break;

        default:

            break;
    }
}
#endif


void Main_system_init(void)
{	
	FS_ERR fserr;
	int drive;
	unsigned int ch0;
	unsigned int ch2;
	
 	SetVector(EXCEPTION_TRAP,Exception);

    // Keyboard I/O port initial
    //g_timer_tick=0;
	//System_Timer_Init(3,TICK_PERIOD);  
	System_Timer3_Init();
	kbd_init_process();
	rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
//    reinit_oid_pen();
//#ifdef NIEQING_DIANCHI
/*
	init_getid();
	
	init_raw_data();
	CIS_Init();
	b_oid_init_flag=TRUE;

*/	
//	init_getid();
//	CIS_Init();
//	OPAR_Init();
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nready to init oid");
#endif
if(!b_oid_ok){
	reinit_oid_pen();
	}
	b_oid_init_flag=TRUE;
#ifdef  TX_DEBUG
	SendString(( char *)"\r\ninit oid over\r\n");
#endif	
	index_flag = FALSE;
	mount_sd_falg = TRUE ;
	b_sd1_ok = FALSE;
	b_sd2_ok = FALSE;
	FSInit();
#ifdef USE_SD1
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
#ifdef USE_SD2
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
//*/
   wireless_task_init();
}

