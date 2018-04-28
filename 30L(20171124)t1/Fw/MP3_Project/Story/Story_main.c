//#define SD1_MP3_PATH "0:\\MP3"
//#define SD2_MP3_PATH "1:\\MP3"
#define ROOT_PATH "0:"
//#define MP3_EXT	 "MP3"

#define FIRST_MUSIC_NUMBER 0

#pragma CODE= MAIN_CODE
#pragma DATA= MAIN_RAM

#include <KBDM9_DDK.H>
#include <sfr.h>
#include <string.h>
#include <ctype.h> 
//#include "fs.h"
#include "string.h"
#include "MP3_API.H"
#include "compiler.h"
//#include "mp3_include.h"
//#include "Music_Player.H"
#include "ADC.h"
#include "SYSTEM.H"
#include "keypad.h"

#include "FileSearch.h"
//#include "BinaryImage.h"
#include "com.h"
#include "oid.h"
#include "conf_kb.h"
//#include "rec.h"
//--------------------------------------------------------------------
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(MP3_COMMON_CODE);
//ImageDeclareType(TEST_IRAM);
ImageDeclareType(SYSTEM_TIMER3_INIT_CODE);
enum{
	MP3_STOP = 0,
	MP3_PAUSE,
	MP3_PLAY
}MP3_state;

//tLED LED_state;

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
//const  U8       root[] = "0:\\";
//const  U8       AP4[] = "ap4";
//const  U8		subdir[] = "subdir";
#endif

/*
#ifdef UNCOIDE_FORMAT
const U16 MP3_FILE[] = {'m','p','3','p','l','a','y','e','r','.','m','p','3','\0'};
const U16 NO_MP3[] = {'n','o','m','p','3','f','i','l','e','.','m','p','3','\0'};
const U16 POWEROFF_FILE[] = {'p','o','w','e','r','o','f','f','.','m','p','3','\0'};
const U16 BATTERY_LOW_FILE[] = {'b','a','t','t','e','r','y','l','o','w','.','m','p','3','\0'};
const U16 DEL[] = {'d','e','l','.','m','p','3','\0'};
const U16 ERROR[] = {'e','r','r','o','r','.','m','p','3','\0'};
#endif

#ifdef ASCII_FORMAT
const U8 MP3_FILE[] = "mp3player.mp3";
const U8 NO_MP3[] = "nomp3file.mp3";
const U8 POWEROFF_FILE[] = "poweroff.mp3";
const U8 BATTERY_LOW_FILE[] = "batterylow.mp3";
#endif
*/
/*
#define  REMOTE_INDEX_MAX                 32
#define  REMOTE_VOL_UP                    0
#define  REMOTE_VOL_DOWN                  ( REMOTE_VOL_UP + 1 )
#define  REMOTE_POWER_OFF                 ( REMOTE_VOL_UP + 2 )
#define  REMOTE_EXIT_MP3                  ( REMOTE_VOL_UP + 3 )
#define  REMOTE_MP3_PLAY_PAUSE            ( REMOTE_VOL_UP + 4 )
#define  REMOTE_MP3_PREV                  ( REMOTE_VOL_UP + 5 )
#define  REMOTE_MP3_NEXT                  ( REMOTE_VOL_UP + 6 )
#define  REMOTE_AP4_MODE                  ( REMOTE_VOL_UP + 7 )
#define  REMOTE_MP3_MODE                  ( REMOTE_VOL_UP + 8 )
#define  REMOTE_REC_MODE                  ( REMOTE_VOL_UP + 9 )
#define  REMOTE_PASTER_MODE               ( REMOTE_VOL_UP + 10 )
#define  REMOTE_DIC_MODE                  ( REMOTE_VOL_UP + 11 )
#define  REMOTE_CAL_MODE                  ( REMOTE_VOL_UP + 12 )
#define  REMOTE_REC_START                 ( REMOTE_VOL_UP + 13 )
#define  REMOTE_REC_PLAY                  ( REMOTE_VOL_UP + 14 )
#define  REMOTE_REC_STOP                  ( REMOTE_VOL_UP + 15 )
#define  REMOTE_OK                  	  ( REMOTE_VOL_UP + 16 )
#define  REMOTE_V_0                  	  ( REMOTE_VOL_UP + 17 )
#define  REMOTE_V_1                  	  ( REMOTE_VOL_UP + 18 )
#define  REMOTE_V_2                  	  ( REMOTE_VOL_UP + 19 )
#define  REMOTE_V_3                  	  ( REMOTE_VOL_UP + 20 )
#define  REMOTE_V_4                  	  ( REMOTE_VOL_UP + 21 )
#define  REMOTE_V_5                  	  ( REMOTE_VOL_UP + 22 )
#define  REMOTE_V_6                  	  ( REMOTE_VOL_UP + 23 )
#define  REMOTE_V_7                  	  ( REMOTE_VOL_UP + 24 )
#define  REMOTE_V_8                  	  ( REMOTE_VOL_UP + 25 )
#define  REMOTE_V_9                  	  ( REMOTE_VOL_UP + 26 )
*/
//DEFINE  ALL BITS OF AP4_MP4_FLAG 
//#define BANK_FORM_MP3       	0X8000
//#define BANK_FORM_GAME          0X4000
//#define BANK_HAVE_INDEX     	0X2000
//#define CURRENT_CONTROL_GAME 	0X1000
//#define FLAG_PLAY_CONTROL_MODE  0X0800
//unsigned char key_mode_flag;

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

//extern __X int Queue_Left;
//extern __X int Queue_Right;

//extern unsigned int ap4_mp3_flag;

//extern   unsigned int ap3_file_number;
//extern   unsigned int volume_inc_flag;
//extern   unsigned int remote_index_tab[16];
//extern unsigned long TimerCount;
unsigned int all_wav_num;
unsigned int play_wav_num;
unsigned int lFileMax;
unsigned int lFileNow;
unsigned int lFileMax_SD1_MP3;
unsigned int lFileMax_SD2_MP3;
unsigned int lFileMax_SD1_WAV;
unsigned int lFileMax_SD2_WAV;

//extern unsigned int prev_task;

//extern unsigned int wav_action;

unsigned char flag_auto_poweroff;
unsigned long auto_poweroff_timer;

//extern _MEM_TYPE_SLOW_  U16     Current_Index;
//__X  Bool       		 index_flag;
__X  Bool       		 flag_lowpower;

FCFILE __X **	fat_g_cur_file;
//Bool OID_PLAY;
//======================================================

typedef enum {
    REC_STOP = 0,
    REC_RECODING,
    REC_PAUSE,
    REC_ERROR,
    WAV_STOP,
    WAV_PLAYING,
    WAV_PAUSE,
    WAV_ERROR
}tWAV_REC_State;

#define MIN_LENGTH  (1UL*1024*1024)
#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE      ((PCM_BUF_SIZE-1)/4+2)          // word
#define ADPCM_BUF_CNT       6

//int PCM_CACHE_Buf[PCM_BUF_SIZE];
//__X int ADPCM_Buf[ADPCM_BUF_SIZE];
__X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
__X int PCM_Buf1[PCM_BUF_SIZE];
__X int PCM_Buf2[PCM_BUF_SIZE];
__X int NotifyFlag, NotifyError;
//int *NotifyBuf;

//Bool flag_wav_play;
char dir_path[128];
extern char	__X	g_full_path[128];
char current_shortFileName[128];
__X int idx_L,idx_R;
__X FS_ERR REC_fserr;
Bool flag_prev = FALSE;
Bool flag_repeat = FALSE;
//__X FCFILE *ADPCM_File;  
//extern __X FCFILE *ADPCM_File;
__X tWAV_REC_State WavRecState;
__X unsigned long  Frame_Count;
__X unsigned int PCM_Buffer_Size, ADPCM_Buffer_Size;
extern unsigned int current_wav_num;
Bool find_wav(unsigned int num,Bool flag);
Bool WavPlayStart(void);
void WavPlayStop(void);
extern void DAC_Stereo_to_Mono(unsigned Option);
//======================================================
char tisiFileName[128];
unsigned int temp_string2[30];
void main_loop(void);
void tishi_mp3(U8 flag);
U16  oid_remote(void);
void wav_process(Bool flag);
//extern void judge_op(void);
extern void ADC_INIT( void );
Bool WavDel(void);
Bool find_del_wav(Bool flag);
extern void scan_oid(void);
void Main_system_init(void);
//extern int	file_get_ext_count(char * dir_path,char * ext);
//extern Bool file_get_path(char * dir_path,char * ext,int index);

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
/*
void save_wav_num(void)
{

    __X FS_ERR err;
  unsigned long temp_len;

  __X FCFILE *Temp_File;
   char temp_file[]="0:\\rec\\pos.dat";
   
	Temp_File = FSOpen((char *)temp_file,FS_MODE_READ_WRITE,&err);   
	FSSeek(Temp_File,0);
	FSWrite(Temp_File, (void *)&current_wav_num,2,&err);
	FSClose(Temp_File);
	FSSetAttribute((char *)temp_file,FS_ATTR_HIDDEN);
}
*/
void AudioMP3FileOpenFirst(char *file)
{
	FS_ERR fs_err;
	if(OID_PLAY){	
		}
	else{
		AudioMP3Handle.FSHandle = FSOpen( file, FS_MODE_READ, &fs_err );
		AudioMP3Offset  = 0;
		AudioMP3FileLen = AudioMP3Handle.FSHandle->size;
		}
}
MP3API_STATUS AudioMP3FileCloseAfter( void )
{
   	if(OID_PLAY) return MP3API_NO_ERR;	
	if( FSClose(AudioMP3Handle.FSHandle) != MP3_RUN_NRR )
		return MP3_RUN_ERR;
	return MP3API_NO_ERR;
}

void main(void)
{	
	tSDKLoadCodeStatus LoadCodeStatus;

	IP_INIT();

 	ADC_init();
	SystemClock(SYS_CLOCK);	
	SDKLoadCodInit();	
//	key_mode_flag = TRUE;    
	flag_auto_poweroff = FALSE;
//	key_begin_flag = FALSE;
	//LoadCodeStatus=SDKLoadCode(TEST_IRAM);
	BlueLed_ON();
	RedLed_OFF();
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB mp3 begin!");
#endif	
	Main_system_init();
	main_loop();
}




int CheckPlaying(Bool flag)
{
/*
    tMusicStatus status;
    int i = 0;

	if(DAC_CCMC_Status != 0)
	{
#ifdef  TX_DEBUG	
		SendString("dac reset\r\n");
#endif
    	DAC_Headphone_Reset();
    //	DAC_Enable_HeadPhone();
    //	DAC_HeadPhone_Volume(VolTbl[vol_idx]); // 此處的Vol為UI之前設定的音量值
    //	DAC_Go();
	}
*/
    SpeakDetect(FALSE);
/*
    status = AudioMP3Status();
    while(1)
    {
        if(PlayStatusTable[i] == (-1))
            break;
        else if(PlayStatusTable[i] == status)
            return (-1);
        else
            i++;
    }
    return 0;
*/	
	return AudioMP3ChkPlaying(); 
}


#pragma CODE = MAIN_LOOP_CODE
void Main_system_init(void);
void Key_Process(INT16U Key_Code,Bool flag);
Bool PlayMP3Music(void);
void StopMP3Music(void);

extern int set_vol_;

void ready_back_mp3(U16 go_project)
{
	
	current_playmp3_number = lFileNow;
	
	g_b_secondgo_mp3 = TRUE;
	b_back_from_mp3 = TRUE;
	CommonLoadCode(go_project);
}



void playing_dowith(void)
{
	INT16U temp_key=0;
    	MP3_state = MP3_PLAY;	
#ifdef NIEQING_DIANCHI
		Charge_Check();
#endif
		if(rec_led_is_timer_timeout())
			{
			 BlueLed_FLASH();
			 RedLed_OFF();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			}
			adc_get();
    		
			if(sys_led_is_timer_timeout()){
				sys_led_start_timer(SYS_LED_ON_TIME);
#if 0//def TX_DEBUG 
		   	    SendString("\r\nadc0=");
		   	    send_hex_u16(adc0_value);
#endif					
				op_time_dowith();
				if(CheckUSB()){
					AudioMP3Stop();
					AudioMP3FileCloseAfter();
					b_find_usb_flag = TRUE;
          			//CommonLoadCode(C_POWEROFF);
          			ready_back_mp3(C_POWEROFF);
					}
				if(BatteryDetect())
		    			{
		    				AudioMP3Stop();
							AudioMP3FileCloseAfter();
//							vol_idx = 0;
							tishi_mp3(FLAG_BATTERY_LOW_FILE);
							OP_CLOSE();
							//CommonLoadCode(C_POWEROFF);
							ready_back_mp3(C_POWEROFF);
		    			}
			  }
			


		   (void)oid_remote();
/*
#if 1
		   if(0!=temp_key)
		   	{                           
#ifdef TX_DEBUG 
		   	    SendString("\r\noid_key=");
		   	    send_hex_u16(temp_key);
#endif				
		   		Key_Process(temp_key,TRUE);
		   	}	
#endif		 
*/
		   kbd_process();
		   temp_key = kbd_key_read();
		   if(0!=temp_key)
		   	{                           
#ifdef TX_DEBUG 
		   	    SendString("\r\ntemp_key=");
		   	    send_hex_u16(temp_key);
#endif				
		   		Key_Process(temp_key,TRUE);
		   	}
    	    //UI Code  
  	    
			if(flag_auto_poweroff)
				{
#if 0//def TX_DEBUG  
    	    		SendString("\r\nTimerCount=");
					send_hex_u32(g_timer_tick);
					SendString("\r\nauto_poweroff_timer=");
					send_hex_u32(auto_poweroff_timer);
#endif	//#ifdef TX_DEBUG 					
				}
  				
    	    if((g_timer_tick>=auto_poweroff_timer)&&flag_auto_poweroff)
    	    	{
#ifdef TX_DEBUG    	    	
    	    			SendString("mp3 auto close!\r\n");
#endif //#ifdef TX_DEBUG   
    	    		    
			        	AudioMP3Stop();
						AudioMP3FileCloseAfter();
//						strcpy(tisiFileName,POWEROFF_FILE);
						tishi_mp3(FLAG_POWER_OFF_FILE);
						OP_CLOSE();
						//save_wav_num();
						//PowerTurnOff();
						//if(USB_DET())
						//	{
						//		CommonLoadCode(C_OPEN);
						//	}
						//while(PWD_Key_Press());
						//Delay_ms(5);
						//CommonLoadCode(C_OPEN);
						//CommonLoadCode(C_POWEROFF);
						ready_back_mp3(C_POWEROFF);
    	    	}  
}

void mp3_play(void)
{
	tAudioErr   Audio_err;
	clkup_time(TRUE);
	AudioMP3FileOpenFirst((char *)current_shortFileName);
    Audio_err = AudioMP3Init(0);
    if(Audio_err != AUDIO_ERROR_OK)
    	{
    	return;
//            ErrHappen(0,"The MP3 play initial fail.");
    	}
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nnum:shortFileName=");
	send_hex_u16(lFileNow);
	SendString(( char *)":");
	SendString(current_shortFileName);
#endif    
    Audio_err = AudioMP3Open((char *)current_shortFileName);
    if(Audio_err != AUDIO_ERROR_OK)
    	{
    	(void)AudioMP3Uninit();
    	return;
//            ErrHappen(0,"The MP3 play initial fail.");
    	}
   // AudioMP3SetEQ(EQ_NOMAL);
//        total_time = AudioMP3GetPlayTime(TOTAL_TIME_MODE);		
	vol_idx_bak = VOL_NUM;
	SpeakDetect(TRUE);
    //DAC_FadeIn();
    flag_lowpower = FALSE;
	// DAC_Stereo_to_Mono(1);  
    Audio_err = AudioMP3Play();
    if(Audio_err != AUDIO_ERROR_OK)
    	{
    	(void)AudioMP3Close();
    	(void)AudioMP3Uninit();
    	return;
//            ErrHappen(0,"The MP3 play initial fail.");
    	}

	 op_stop_timer();
	  clkup_time(FALSE);
	while (CheckPlaying(FALSE))
	{    	
    	playing_dowith();
	}
	
	MP3_state = MP3_STOP;
	//DAC_FadeOut();
	Audio_err = AudioMP3Stop();
	Audio_err = AudioMP3Close();
    Audio_err = AudioMP3Uninit();
	AudioMP3FileCloseAfter();
	op_start_timer();
	clkup_time(TRUE);
}
void main_loop(void)
{
	unsigned int longFileName[129]; // Word
	//char shortFileName[250];
	tSDKLoadCodeStatus LoadCodeStatus;
	tAudioErr   Audio_err;
    unsigned int i;
	Bool flag_have_wav;
	char current_mp3_path[16];
//	char *temp_char;
	
	unsigned char temp_data = 0;
    flag_lowpower = FALSE;
	play_wav_num = 0;
	flag_have_wav = FALSE;
//	LoadCodeStatus = SDKLoadCode(DAC_COMMON_CODE);
//	LoadCodeStatus = SDKLoadCode(MP3_COMMON_CODE);
	//LoadCodeStatus = SDKLoadCode(TEST_IRAM);
	

	tishi_mp3(FLAG_GO_STORY);
	
	MP3_state = MP3_STOP;
//	LED_param = 200;
	lFileMax_SD1_MP3 = 0;
	lFileMax_SD2_MP3 = 0;
	lFileMax_SD1_WAV = 0;
	lFileMax_SD2_WAV = 0;
//	Find_Initial( FILE_LEVEL, ".mp3" );	// Set search all MP3 file.
//	Find_PathFile( FILE_MODE_ALL, MP3_PATH );
//	Find_PathFile( FILE_MODE_LOCAL, MP3_PATH );
	memset(current_mp3_path,0,16);
	if(b_sd1_ok){
		strcpy(current_mp3_path,SD1_STORY_FILE);
		lFileMax_SD1_WAV = file_get_ext_count(current_mp3_path, WAV_EXT);
		lFileMax_SD1_MP3 = file_get_ext_count(current_mp3_path, MP3_EXT);
		}
	if(b_sd2_ok){
		strcpy(current_mp3_path,SD2_STORY_FILE);
		lFileMax_SD2_WAV = file_get_ext_count(current_mp3_path, WAV_EXT);
		lFileMax_SD2_MP3 = file_get_ext_count(current_mp3_path, MP3_EXT);
		}
#if 1
	lFileNow = FIRST_MUSIC_NUMBER; // first music number.
	play_wav_num = 0;
#else
	if(lFileMax==0){
		strcpy(current_mp3_path,ROOT_PATH);
		lFileMax = file_get_ext_count(current_mp3_path, MP3_EXT);
		}
	else{
		lFileNow = FIRST_MUSIC_NUMBER; // first music number.
		}
#endif	
#ifdef TX_DEBUG
		SendString("\r\nsd1_mp3:sd1_wav:sd2_mp3:sd2_wav=");
		send_hex_u16(lFileMax_SD1_MP3);
		SendString(":");
		send_hex_u16(lFileMax_SD1_WAV);
		SendString(":");
		send_hex_u16(lFileMax_SD2_MP3);
		SendString(":");
		send_hex_u16(lFileMax_SD2_WAV);
#endif			

    if((lFileMax_SD1_WAV==0)&&(lFileMax_SD1_MP3==0)&&(lFileMax_SD2_WAV==0)&&(lFileMax_SD2_MP3==0))
    	{
#if 1    	
/*
next_play_wav:
		    while(play_wav_num<100){
				if(find_wav(play_wav_num)){
					if(WavPlayStart())flag_have_wav=TRUE;
					}
					play_wav_num++;
				}
			  if(flag_have_wav){
			  	play_wav_num=0;
				goto next_play_wav;
			  	}
*/			  
			g_b_nomp3 = TRUE;
			tishi_mp3(FLAG_NO_STORYFILE);
    		if(g_b_noap4)
    			{
    			    
					OP_CLOSE();
					//save_wav_num();
    				//while(1)
					//{
					//	PowerTurnOff();
					//}
					CommonLoadCode(C_POWEROFF);
    			}
			else{
				b_back_from_mp3 = TRUE;
				CommonLoadCode(C_OID);
				}
		
#else
				   CommonLoadCode(C_REC_PASTER);
		
#endif
    	}

	index_flag = FALSE;
	scan_oid_enable();
	  if(g_b_secondgo_mp3){
	  	if(current_mp3_sd == FLAG_SD1_CON){
			lFileNow = current_playmp3_number;
			goto begin_sd1_mp3;
	  		}
		else{
			lFileNow = current_playmp3_number;
			goto begin_sd2_mp3;
			}
	  	}


//	SDKLoadCode(DAC_COMMON_CODE);
//	SDKLoadCode(MP3_COMMON_CODE);
	while(1)
	{
    	INT32U play_time;
	    INT32U total_time;
	    FS_ERR fs_err;
		INT16U temp_key=0;


	
		//(void)CheckUSB();
/*		
		Find_FileName(longFileName); // Get MP3 long file name, The short file name is global variable "cFileName".
		if( ++lFileNow >= lFileMax ){
		    lFileNow = 0;
			wav_process();
			}
    	strcpy(shortFileName,cFileName);
*/		
		lFileNow = 0;
begin_sd1_mp3:		
		current_mp3_sd = FLAG_SD1_CON;
		lFileMax = lFileMax_SD1_MP3;
		strcpy(current_mp3_path,SD1_STORY_FILE);
		while(lFileNow<lFileMax){
sd1_mp3:			
			if(file_get_path(current_mp3_path, MP3_EXT, lFileNow)){
				strcpy((char *)current_shortFileName,(char *)g_full_path);
				}
				
				mp3_play();
				if(flag_repeat){
					flag_repeat = FALSE;
					goto sd1_mp3;
					}
				 if(flag_prev){
					flag_prev = FALSE;
					
					if(lFileNow == 0){
						if(lFileMax_SD2_MP3!=0){
							lFileNow = lFileMax_SD2_MP3 - 1;
							goto begin_sd2_mp3;
							}
						else{
							lFileNow = lFileMax_SD1_MP3 - 1;
							goto sd1_mp3;
							}
						}
					else{
						lFileNow --;
						goto sd1_mp3;
						}
			    	}
				lFileNow++;
			}
	   
		
	//all_wav_num= lFileMax_SD1_WAV;
	//wav_process(FALSE);

	lFileNow = 0;
begin_sd2_mp3:	
	current_mp3_sd = FLAG_SD2_CON;
	lFileMax = lFileMax_SD2_MP3;
	strcpy(current_mp3_path,SD2_STORY_FILE);
	while(lFileNow<lFileMax){
sd2_mp3:		
		if(file_get_path(current_mp3_path, MP3_EXT, lFileNow)){
			strcpy((char *)current_shortFileName,(char *)g_full_path);
			}
		
			mp3_play();	
			if(flag_repeat){
					flag_repeat = FALSE;
					goto sd2_mp3;
					}
			if(flag_prev){
					flag_prev = FALSE;
					if(lFileNow == 0){
						if(lFileMax_SD1_MP3!=0){
							lFileNow = lFileMax_SD1_MP3 - 1;
							goto begin_sd1_mp3;
							}
						else{
							lFileNow = lFileMax_SD2_MP3 - 1;
							goto sd2_mp3;
							}
						}
					else{
						lFileNow --;
						goto sd2_mp3;
						}
	    		}
			lFileNow++;
		}
  	
	//all_wav_num= lFileMax_SD2_WAV;
	//wav_process(TRUE);

    }  
}

//------------------------------------------------------------------------------
#if 1
//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Key_Process(INT16U Key_Code,Bool flag)
{
/*
	if(Key_Code == (STATE_KEY_PRESSED|KEY_LOCK)){
		if(b_lock_key){
#ifdef TX_DEBUG
		   SendString("\r\nlock release!");
#endif
			b_lock_key = FALSE;
			}
		else{
#ifdef TX_DEBUG
		   SendString("\r\nlock key!");
#endif			
			b_lock_key = TRUE;
			}
		}
	if(b_lock_key)return;  */ 
    switch(Key_Code)
    {
    	/*
    	case (KBD_KEY_RELEASED<<8|KEY_POWER):
			if(AudioMP3Status() == PAUSE_STATUS)
        	{
        		MP3_state = MP3_PLAY;
//        		LED_param = 200;
				flag_auto_poweroff = FALSE;
				auto_poweroff_timer = 0;
				op_stop_timer();
				vol_idx_bak = VOL_NUM;
        		AudioMP3Continue();
			}
        	else
        	{
        		MP3_state = MP3_PAUSE;
//        		LED_param = 600;
				flag_auto_poweroff = TRUE;
				//auto_poweroff_timer = Timer_set_timer(TIMER_MIN(3));
				auto_poweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME));
				op_start_timer();
				AudioMP3Pause();
			}			
            break;
	//*/		
    	case (STATE_KEY_RELEASED|KEY_MODE): // mode
    		if(flag){
				AudioMP3Stop();
				flag_repeat = TRUE;
				//AudioMP3FileCloseAfter();
    			}
			else{
				WavPlayStop();
				}
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_OID);
		break;	
		
        case (STATE_KEY_RELEASED|KEY_VOL_INC):
			if(vol_idx < (VOL_NUM -1)){
				vol_idx++;
				}
		break;
		case (STATE_KEY_RELEASED|KEY_VOL_DEC):
				if(vol_idx > 0){
					vol_idx--;
				}
		break;


		case (STATE_KEY_LONGPRESSED|KEY_VOL_INC):
			AudioMP3Stop();
			/*
			if(!b_sleep_mode){
				b_sleep_mode = TRUE;
#ifdef  TX_DEBUG
				SendString(( char *)"\r\nopen sleep mode!");
#endif				
				}
			else{
#ifdef  TX_DEBUG
				SendString(( char *)"\r\nopen sleep mode!");
#endif					
				b_sleep_mode = FALSE;
				}*/
		break;
		case (STATE_KEY_LONGPRESSED|KEY_VOL_DEC):
			AudioMP3Stop();
			flag_prev = TRUE;
		break;
#if 1	
/*		
        case (KBD_KEY_RELEASED<<8|KEY_POWER): // Pause or Continue
 
        	if(AudioMP3Status() == PAUSE_STATUS)
        	{
        		MP3_state = MP3_PLAY;
//        		LED_param = 200;
				flag_auto_poweroff = FALSE;
				auto_poweroff_timer = 0;
				op_stop_timer();
				vol_idx_bak = VOL_NUM;
        		AudioMP3Continue();
			}
        	else
        	{
        		MP3_state = MP3_PAUSE;
//        		LED_param = 600;
				flag_auto_poweroff = TRUE;
				//auto_poweroff_timer = Timer_set_timer(TIMER_MIN(3));
				auto_poweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME+AUTO_CLOSE_TIME));
				op_start_timer();
				AudioMP3Pause();
			}
		
            break;
	*/			

#else		
        case (KBD_KEY_RELEASED<<8|KEY_POWER): // Pause or Continue
			AudioMP3Stop();
//			OP_CLOSE();
//			ap4_mp3_flag = 0;
			//ap4_mp3_flag |= BANK_FORM_MP3;
			b_back_from_mp3 = TRUE;
			CommonLoadCode(C_OID);
			//CommonLoadCode(C_WR_TX);
            break;
/*			
        case (KBD_KEY_RELEASED<<8|KEY_MODE):
			AudioMP3Stop();
			if(lFileNow>=1)
				{
				lFileNow -= 1;
				}
			else
				{
				lFileNow = lFileMax+lFileNow-1;
				}
            break;
*/			
#endif
#if 0
		//case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC):
		case (STATE_KEY_LONGPRESSED|KEY_VOL_DEC):		//NEXT
			if(flag){
				AudioMP3Stop();
					//AudioMP3FileCloseAfter();
				flag_prev = TRUE;
				}
			else{
				WavPlayStop();
				if(play_wav_num>=2)
					{
					play_wav_num -= 2;
					}
				else
					{
					play_wav_num = all_wav_num+play_wav_num-2;
					}
				}
            break;
        case (STATE_KEY_LONGPRESSED|KEY_VOL_INC):		//NEXT
			if(flag){
				AudioMP3Stop();
					//AudioMP3FileCloseAfter();
    			}
			else{
				WavPlayStop();
				}
			/*s
			if(lFileNow>=1)
				{
				lFileNow -= 1;
				}
			else
				{
				lFileNow = lFileMax+lFileNow-1;
				}*/
            break;
#endif			
        case (KBD_KEY_LONGPRESSED<<8|KEY_POWER): // POWER
        	if(flag){
				AudioMP3Stop();
					//AudioMP3FileCloseAfter();
    			}
			else{
				WavPlayStop();
				}
			tishi_mp3(FLAG_POWER_OFF_FILE);
			OP_CLOSE();
//			BlueLED(OFF);
//			RedLED(OFF);
			//BlueLed_OFF();
			//RedLed_OFF();
			//save_wav_num();
			//PowerTurnOff();
			//if(USB_DET())
			//	{
			//		CommonLoadCode(C_OPEN);
			//	}
			//while(PWD_Key_Press());
			//Delay_ms(5);
			//CommonLoadCode(C_OPEN);
			CommonLoadCode(C_POWEROFF);
//			while(1);
            break;
		//case (KBD_KEY_PRESSED<<8|KEY_MEN):
		case (STATE_KEY_RELEASED|KEY_POWER):
			AudioMP3Stop();
			AudioMP3FileCloseAfter();
			b_back_from_mp3 = TRUE;
			//lFileNow = 0;
			//b_back_have_key = TRUE;
			//bak_current_index = STATE_KEY_RELEASED|KEY_MEN;
			ready_back_mp3(C_OID);
			break;
		 case (KBD_KEY_RELEASED<<8|KEY_REC):
		 	AudioMP3Stop();
			AudioMP3FileCloseAfter();
		 	wav_action = WAV_RECORD_FLAG;
			//CommonLoadCode(C_REC_PASTER);
			ready_back_mp3(C_REC_PASTER);
		 	break;
		case (KBD_KEY_LONGPRESSED<<8|KEY_REC):
		 	AudioMP3Stop();
			AudioMP3FileCloseAfter();
		 	wav_action = WAV_PLAY_FLAG;
			//CommonLoadCode(C_REC_PASTER);
			ready_back_mp3(C_REC_PASTER);
		 	break;	
			/*
        case (KBD_KEY_LONGPRESSED<<8|KEY_MODE):		//NEXT
			AudioMP3Stop();
			//wav_action = WAV_RECORD_FLAG;
			b_back_from_mp3 = TRUE;
			CommonLoadCode(C_OID);
            break;
			*/
        default:
            break;
    }
}

//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
/*
void Wav_Key_Process(INT16U Key_Code)
{
    
    switch(Key_Code)
    {
        case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // Volume
				vol_idx++;
				if(vol_idx > (VOL_NUM-1))
					{
						vol_idx = 0;
					}		
//			AudioMP3SetVol(VolTbl[vol_idx]);
            break;
		
        case (KBD_KEY_RELEASED<<8|KEY_POWER): // Pause or Continue
			WavPlayStop();
//			OP_CLOSE();
//			ap4_mp3_flag = 0;
			//ap4_mp3_flag |= BANK_FORM_MP3;
			b_back_from_mp3 = TRUE;
			//CommonLoadCode(C_OID);
			CommonLoadCode(C_WR_TX);
            break;
			
        case (KBD_KEY_RELEASED<<8|KEY_MODE):
			//WavPlayStop();
			WavRecState = WAV_STOP;
			if(play_wav_num>=1)
				{
				play_wav_num -= 1;
				}
			else
				{
				play_wav_num = 99+play_wav_num-1;
				}
            break;

        case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC): // PREV
			//WavPlayStop();
			WavRecState = WAV_STOP;
			if(play_wav_num>=2)
				{
				play_wav_num -= 2;
				}
			else
				{
				play_wav_num = 99+play_wav_num-2;
				}
            break;
        case (KBD_KEY_LONGPRESSED<<8|KEY_MODE):		//NEXT
			//WavPlayStop();
			WavRecState = WAV_STOP;
            break;
        case (KBD_KEY_LONGPRESSED<<8|KEY_POWER): // POWER
        	WavPlayStop();
			tishi_mp3(1);
			OP_CLOSE();
			//BlueLED(OFF);
			//RedLED(OFF);
			//BlueLed_OFF();
			//RedLed_OFF();
			//save_wav_num();
			//PowerTurnOff();
			//if(USB_DET())
			//	{
			//		CommonLoadCode(C_OPEN);
			//	}
			//while(PWD_Key_Press());
			//Delay_ms(5);
			//CommonLoadCode(C_OPEN);
			CommonLoadCode(C_POWEROFF);
//			while(1);
            break;
        default:
            break;
    }
}
*/

#endif
///================================================
Bool fix_index_01(void)
{

	if(Current_Index==INDEX_PLAY_MP3){
        if(AudioMP3Status() == PAUSE_STATUS)
        	{
        		kbd_begin_write(KBD_KEY_RELEASED, KEY_POWER);
			}
        	return TRUE;
		}

	if(Current_Index==INDEX_PAUSE_MP3){
		 if(AudioMP3Status() == PLAYING_STATUS){
      		 kbd_begin_write(KBD_KEY_RELEASED, KEY_POWER);
			}
        return TRUE;
		}

	if(Current_Index==INDEX_NEXT_MP3){
       kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_VOL_INC);
        return TRUE;
		}
	if(Current_Index==INDEX_PREV_MP3){
       kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_VOL_DOWN);
        return TRUE;
		}
	if(Current_Index==INDEX_GO_MP3){
       
        return TRUE;
		}
	if(Current_Index == INDEX_STOP){
		
		//tishi_addr(rec_stop_addr);
		return TRUE;
		}
		
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
		{
		 vol_idx=Current_Index-INDEX_FIRST_VOL;
		 return TRUE;
		}
	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
				}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;

			}
         return TRUE;
		}

	
	return FALSE;
}

Bool fix_index(void){
	if(Current_Index==INDEX_GO_AP4){
		AudioMP3Stop();
		AudioMP3FileCloseAfter();
//		
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_OID);
		ready_back_mp3(C_REC_PASTER);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_DIC){
		AudioMP3Stop();
		AudioMP3FileCloseAfter();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_DIC);
		ready_back_mp3(C_REC_PASTER);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_MP3){
		AudioMP3Stop();
		AudioMP3FileCloseAfter();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_MP3);
		ready_back_mp3(C_MP3);
		return TRUE;
		}
	/*
	if(Current_Index==INDEX_PAUSE_PLAY){
        	if(AudioMP3Status() == PAUSE_STATUS)
        	{
        		MP3_state = MP3_PLAY;
				flag_auto_poweroff = FALSE;
				auto_poweroff_timer = 0;
        		AudioMP3Continue();
				return TRUE;
			}
        	if(AudioMP3Status() == PLAYING_STATUS)
        	{
        		MP3_state = MP3_PAUSE;
				flag_auto_poweroff = TRUE;
				auto_poweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME));
				AudioMP3Pause();
				return TRUE;
			}
		}
	*/
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_FIRST_VOL+6)))
		{
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 //AudioMP3SetVol(VolTbl[vol_idx]);
		 return TRUE;
		}

	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
//					 AudioMP3SetVol(VolTbl[vol_idx]);
				}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;
//				 AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}

       if((Current_Index==INDEX_PREV_MP3)){
			AudioMP3Stop();
					//AudioMP3FileCloseAfter();
			if(lFileNow>=2)
				{
				lFileNow -= 2;
				}
			else
				{
				lFileNow = lFileMax;
				//lFileNow = lFileMax+lFileNow-2;
				}
            return TRUE;
       		}
	   
         if((Current_Index==INDEX_NEXT_MP3)){
			AudioMP3Stop();
            return TRUE;
         	}
/*
	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX)){
		AudioMP3Stop();
		wav_action=Current_Index;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}
*/
	if(Current_Index==INDEX_START_RECORD){
		AudioMP3Stop();
		wav_action=WAV_RECORD_FLAG;
		//CommonLoadCode(C_REC_PASTER);
		ready_back_mp3(C_REC_PASTER);
		return TRUE;
		}
	if(Current_Index==INDEX_PLAY_RECORD)
		{
		AudioMP3Stop();
		AudioMP3FileCloseAfter();
#if 0		
		wav_process();
#else
		wav_action=WAV_PLAY_FLAG;
		//CommonLoadCode(C_REC_PASTER);
		ready_back_mp3(C_REC_PASTER);
#endif
		return TRUE;
		}
	if(Current_Index==INDEX_DEL_RECORD)
		{
		AudioMP3Stop();
					AudioMP3FileCloseAfter();
		(void) WavDel();
//		 OP_CLOSE();
		 //ap4_mp3_flag |= BANK_FORM_MP3;
		 //b_back_from_mp3 = TRUE;
		 //CommonLoadCode(C_OID);
		 ready_back_mp3(C_OID);
		 return TRUE;
		}
	return FALSE;
}

///================================================

Bool remote_index(void)
{
	if(Current_Index==remote_index_tab[REMOTE_EXIT_MP3]){
		AudioMP3Stop();
					AudioMP3FileCloseAfter();
//		
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_OID);
		ready_back_mp3(C_OID);
		return TRUE;
		}
	if(Current_Index==remote_index_tab[REMOTE_DIC_MODE]){
		AudioMP3Stop();
		AudioMP3FileCloseAfter();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_DIC);
		ready_back_mp3(C_DIC);
		return TRUE;
		}
	/*
	if(Current_Index==INDEX_GO_MP3){
		AudioMP3Stop();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_MP3);
		return TRUE;
		}
	*/
	if(Current_Index==remote_index_tab[REMOTE_MP3_PLAY_PAUSE]){
        	if(AudioMP3Status() == PAUSE_STATUS)
        	{
        		MP3_state = MP3_PLAY;
				flag_auto_poweroff = FALSE;
				auto_poweroff_timer = 0;
        		AudioMP3Continue();
				vol_idx = VOL_NUM;
				return TRUE;
			}
        	if(AudioMP3Status() == PLAYING_STATUS)
        	{
        		MP3_state = MP3_PAUSE;
				flag_auto_poweroff = TRUE;
				auto_poweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME+AUTO_CLOSE_TIME));
				AudioMP3Pause();
				op_start_timer();
				clkup_time(TRUE);
				return TRUE;
			}
		}
	
	if((Current_Index>=remote_index_tab[REMOTE_V_0])&&(Current_Index<=remote_index_tab[REMOTE_V_4])){
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 //AudioMP3SetVol(VolTbl[vol_idx]);
		 return TRUE;
		}

	if(Current_Index==remote_index_tab[REMOTE_VOL_UP]){
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
//					 AudioMP3SetVol(VolTbl[vol_idx]);
				}
		 return TRUE;
		}
	if(Current_Index==remote_index_tab[REMOTE_VOL_DOWN]){
	 	   if(vol_idx > 0)
			{
			     vol_idx--;
//				 AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}

       if(Current_Index==remote_index_tab[REMOTE_MP3_PREV]){
			AudioMP3Stop();
			AudioMP3FileCloseAfter();
			if(lFileNow>=2)
				{
				lFileNow -= 2;
				}
			else
				{
				//lFileNow = lFileMax+lFileNow-2;
				flag_prev = TRUE;
				}
            return TRUE;
       		}
	   
         if(Current_Index==remote_index_tab[REMOTE_MP3_NEXT]){
			AudioMP3Stop();
			AudioMP3FileCloseAfter();
            return TRUE;
         	}
/*
	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX)){
		AudioMP3Stop();
		wav_action=Current_Index;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}

	if(Current_Index==INDEX_START_RECORD){
		AudioMP3Stop();
		wav_action=WAV_RECORD_FLAG;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}
	if(Current_Index==INDEX_PLAY_RECORD)
		{
		AudioMP3Stop();
#if 0		
		wav_process();
#else
		wav_action=WAV_PLAY_FLAG;
		CommonLoadCode(C_REC_PASTER);
#endif
		return TRUE;
		}
	if(Current_Index==INDEX_DEL_RECORD)
		{
		 AudioMP3Stop();
		(void) WavDel();
//		 OP_CLOSE();
		 //ap4_mp3_flag |= BANK_FORM_MP3;
		 b_back_from_mp3 = TRUE;
		 CommonLoadCode(C_OID);
		 return TRUE;
		}
*/	
	return FALSE;
}


Bool WavDel(void)
{
 unsigned int i;
#if 0
	BlueLed_ON();
	RedLed_ON();
	i=100;
	while(i>0){
		if(find_wav((i-1))){
			FSDelete(dir_path);
			tishi_mp3(13);
			return TRUE;
			}
		i--;
		}
	tishi_mp3(11);
	return FALSE;
#else
	BlueLed_ON();
	RedLed_ON();
	while(current_wav_num>0){
		if(find_del_wav(FALSE)){
			FSDelete(dir_path);
			tishi_mp3(13);
			return TRUE;
			}
		current_wav_num--;
		}
	if(current_wav_num==0){
		if(find_del_wav(FALSE)){
			FSDelete(dir_path);
			tishi_mp3(13);
			return TRUE;
			}
		}
	tishi_mp3(11);
	return FALSE;
#endif	
}



///=================================================
U16 oid_remote(void)
{
    unsigned int i;
//	return 0;
    scan_oid();
	if(index_flag)
		{
			index_flag = FALSE;
			clkup_time(FALSE);
			if(fix_index_01())return 0;
			//if(remote_index())return 0;
			AudioMP3Stop();
			AudioMP3FileCloseAfter();
		 	b_back_from_mp3 = TRUE;
			b_back_have_index = TRUE;
			bak_current_index = Current_Index;
			//CommonLoadCode(C_OID);
			ready_back_mp3(C_OID);
			
#if 0			
			if(fix_index())return 0;
			AudioMP3Stop();
//			OP_CLOSE();
			//ap4_mp3_flag |= BANK_FORM_MP3;
			b_back_from_mp3 = TRUE;
			CommonLoadCode(C_OID);
			return 0;
#endif			
		}
	return 0;
}

//===============================================
void	strcpy_u(U16 * dst,U16 * src)
{
	while(*src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
}
void	strcat_u(U16 * dst,U16 * src)
{
	while(*dst)	++dst;
	while(*src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
}

U16 *	atou(U8 * astr)
{
	U16 * ustr;
	ustr = (U16 *)temp_string2;
	while(*astr)
	{
		*ustr++ = *astr++;
	}
	*ustr = 0;
	return	(U16 *)temp_string2;
}


int	str_comp_u(U16  * sl,U16  * sr,U8 use_case)
{
	if(use_case)
	{
		while(*sl&&*sl==*sr)
		{
			++sl;
			++sr;
		}
		return	*sl - *sr;
	}
	else
	{
		while(*sl&&tolower(*sl)==tolower(*sr))
		{
			++sl;
			++sr;
		}
		return	tolower(*sl)-tolower(*sr);
	}
}

void	local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size)
{
	while(*local_str)
	{
		*unicode_str++=*local_str++;
		buffer_size -= 2;
		if(buffer_size<=3)
		{
			*unicode_str = 0;
			return;
		}
	}
	*unicode_str = 0;
}

#define	find_path_a(a,b)	find_path_u(atou(a),b)   

Bool find_entry( U8 * spec, U16 *name, Bool is_dir, FILE_INFO __X *finfo, FS_ENTRY_INFO __X *entry )
{
   U16 buffer[ 25 ];
   
   //if( FSGetEntry(( char *)spec, 0, finfo, entry ) != FS_NO_ERROR )
   //   return FALSE;

   //change 2013-05-22 zoe
   unsigned int i;
   FS_ERR fserr;
   for( i = 0; ; ++i )
   {
      fserr = FSGetEntry(( char *)spec, i, finfo, entry );
      if( fserr == FS_NO_ERROR )
         break;
      if( fserr == FS_FILE_NOT_FOUND )
         continue;
      return FALSE;
   }

   do
   {
      if(( finfo->attribute & FS_ATTR_LFN_ENTRY )!= FS_ATTR_LFN_ENTRY )
      {
         local_to_unicode( finfo->name, buffer, sizeof( buffer ));
         if( str_comp_u(( U16 *)finfo->lfn, name, FALSE )== 0
            || str_comp_u( buffer, name, FALSE )== 0 )
         {
            if( is_dir )
            {
               if( finfo->attribute & FS_ATTR_DIRECTORY )
                  return TRUE;
            }
            else
            {
               if( !( finfo->attribute & FS_ATTR_DIRECTORY ))
                  return TRUE;
            }
         }
      }

      //change 2013-05-22 zoe
      fserr = FSGetNextEntry( finfo, entry );
      if(( fserr != FS_NO_ERROR )&&( fserr != FS_FILE_NOT_FOUND ))
         break;
   }
   //while( FSGetNextEntry( finfo, entry )== FS_NO_ERROR );
   while( 1 );
   
   return FALSE;
}











Bool	find_path_u(U16 * path_str,U8 * short_path)
{
	U16	left ,right;
	U16	iter;
	U16	old_char;
	FILE_INFO	__X	info;
	FS_ENTRY_INFO __X entry;
	left = 3,right = 3;

		//strcpy((char *)short_path,"0:");
	strcpy((char *)short_path,(char* )ROOT_DENTRY);
	memset(&info,0,sizeof(info));
	entry.drive = 0;
	FSGetEntry((char *)short_path,0,&info,&entry);
	if(path_str[left] == '\0')
		return	TRUE;
	while(path_str[left]!='\0')
	{
		while(path_str[right]!='\\'&&path_str[right]!='/'&&path_str[right]!='\0')
			++right;
		if(path_str[right] == '\\'||path_str[right] == '/')
		{
			old_char = path_str[right];
			path_str[right] = 0;
			if(find_entry(short_path,(U16 *)&path_str[left],TRUE,&info,&entry))
			{
		//	if(nav_filelist_findname((FS_STRING)(&path_str[left]),FALSE)&&nav_file_isdir())
				strcat((char *)short_path,(char *)"\\");
				strcat((char *)short_path,(char *)info.name);
				if(FSGetEntry((char *)short_path,0,&info,&entry)!=FS_NO_ERROR)
					goto	__bad_end;
			}
			else
				goto	__bad_end;
			path_str[right] = old_char;
			++right;
			left = right;
		}
		else
		{
			if(find_entry(short_path,(U16 *)&path_str[left],FALSE,&info,&entry))
			{
				strcat((char *)short_path,"\\");
				strcat((char *)short_path,(char *)info.name);
				goto	__end;
			}
			else
				goto	__bad_end;
		}
	}
__end:
	return	TRUE;
__bad_end:
	return	FALSE;
}


//==============================================
void tishi_mp3(U8 flag)
{
	    FS_ERR fs_err;
	    tAudioErr   Audio_err;
		unsigned long len=0;
		char path[64];
        U16	m[] = {'\\','\0'};
		//judge_op();
#ifdef UNCOIDE_FORMAT
		strcpy_u((U16 *)tisiFileName,(U16 *)&root);
		strcat_u((U16 *)tisiFileName,(U16 *)&subdir);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		switch (flag)
			{
				case 0:
					strcat_u((U16 *)tisiFileName,(U16 *)&MP3_FILE);
					break;
				case 1:
					strcat_u((U16 *)tisiFileName,(U16 *)&POWEROFF_FILE);
					break;
				case 2:
					strcat_u((U16 *)tisiFileName,(U16 *)&BATTERY_LOW_FILE);
					break;
				case 3:
					strcat_u((U16 *)tisiFileName,(U16 *)&NO_MP3);
					break;
				case 4:
					strcat_u((U16 *)tisiFileName,(U16 *)&ERROR);	
					break;	
				case 5:
					strcat_u((U16 *)tisiFileName,(U16 *)&DEL);	
					break;	
				default:
					return;
					break;
			}
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)path))return;
#endif

#ifdef ASCII_FORMAT
		strcpy(tisiFileName,(char*)ROOT);
		strcat(tisiFileName,(char *)SUBDIR);
		strcat(tisiFileName,"\\");
		switch (flag)
			{
				case FLAG_GO_MP3:
					strcat(tisiFileName,(char *)&MP3_FILE);
					break;
				case FLAG_POWER_OFF_FILE:
					strcat(tisiFileName,(char *)&POWER_OFF_FILE);
					break;
				case FLAG_BATTERY_LOW_FILE:
					strcat(tisiFileName,(char *)&BATTERY_LOW_FILE);
					break;
				case FLAG_NO_MP3FILE:
					strcat(tisiFileName,(char *)&NO_MP3);
					break;	
				case FLAG_NO_STORYFILE:
					strcat(tisiFileName,(char *)&NO_STORY);
					break;	
				case FLAG_GO_STORY:
					strcat(tisiFileName,(char *)&STORY_FILE);
					break;	
				default:
					return;
					break;
			}
		if(!find_path_a((U8 *)tisiFileName,(U8 *)path))return;
#endif		
		len = FSGetFileSize((char *)path, &fs_err);
		if(0==len)return;



//		char path[32];
//		len = FSGetFileSize((char *)tisiFileName, &fs_err);
//		if(0==len)return;
		//sysup_opstart();
		//SysClkUpgrade();
		//clk_stop_timer();
		clkup_time(TRUE);
		AudioMP3FileOpenFirst((char *)path);
       Audio_err = AudioMP3Init(0);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 return;
        	}
		//flag_wav_play = FALSE;
		//DAC_HeadPhone_Volume(0);
		//judge_op();
		//vol_inc();	
		//DAC_Stereo_to_Mono(1);
		
        Audio_err = AudioMP3Open((char *)path);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        		Audio_err = AudioMP3Uninit();
				return;
        	}
		
 /*       AudioMP3SetEQ(EQ_NOMAL);
//		SpeakDetect();
		DAC_HeadPhone_Volume(0);
		judge_op();
		vol_inc();
		
	    AudioMP3SetEQ(EQ_NOMAL);
		Audio_err = AudioMP3SetVol(VolTbl[vol_idx]);  
       // if(Audio_err != AUDIO_ERROR_OK)
       //     ErrHappen(0,"The MP3 Volume initial fail.");   
       	SpeakDetect();
*/
		vol_idx_bak = VOL_NUM;
 		SpeakDetect(TRUE);
//		DAC_FadeIn();
		op_stop_timer();
		clkup_time(FALSE);
		DAC_Stereo_to_Mono(1);
        Audio_err = AudioMP3Play();
		MP3_state = MP3_PLAY;
    	while (CheckPlaying(FALSE))
    	{    		
    		adc_get();
    		MP3_state = MP3_PLAY;
			if(rec_led_is_timer_timeout())
			{
			 BlueLed_FLASH();
			 RedLed_OFF();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			}
    	}
    	MP3_state = MP3_STOP;
//    	DAC_FadeOut();
		Audio_err = AudioMP3Stop();
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();
		AudioMP3FileCloseAfter();
		op_start_timer();
		clkup_time(TRUE);
}


//==========================================================================

//==============================================
//#pragma CODE = WAV_PLAY_CODE

Bool find_wav(unsigned int num,Bool flag)
{
#if 1
	char current_wav_path[16];
	if(flag){
		strcpy(current_wav_path,SD2_STORY_FILE);
		}
	else{
		strcpy(current_wav_path,SD1_STORY_FILE);
		}
	
	if(file_get_path(current_wav_path, WAV_EXT, play_wav_num)){
			strcpy((char *)tisiFileName,(char *)g_full_path);
			 if(FSGetFileSize(tisiFileName,&REC_fserr)==0)
				{
					return FALSE;
				}
	   		return TRUE;
		}
	return FALSE;
#else
	char digi[4];
	if(num>=1000)return FALSE;
	strcpy(tisiFileName,"0:\\REC\\");
		digi[0] = '0'+num/100;
		digi[1] = '0'+((num -(digi[0]-'0')*100)/10);
		digi[2] = '0'+num%10;
		digi[3] = 0;			
		strcat(tisiFileName,"rec");
 		strcat(tisiFileName,digi);
		strcat(tisiFileName,".wav");			
	  if(FSGetFileSize(tisiFileName,&REC_fserr)==0)
		{
			return FALSE;
		}
	   return TRUE;
#endif	   
}

Bool find_del_wav(Bool flag)
{
	char digi[4];
	unsigned long temp_leng=0;
	
	strcpy(dir_path,"0:\\REC\\");
	digi[0] = '0'+current_wav_num/100;
	digi[1] = '0'+((current_wav_num -(digi[0]-'0')*100)/10);
	digi[2] = '0'+current_wav_num%10;
	digi[3] = 0;		
	strcat(dir_path,"rec");
	strcat(dir_path,digi);
	strcat(dir_path,".wav");	
	temp_leng = FSGetFileSize(dir_path,&REC_fserr);
	if(flag){
		 if(temp_leng==0){
				return FALSE;
			}
		 else{
	         return TRUE;
		 	}
		}
	else{
			if(temp_leng==0)
			{
				return FALSE;
			}
	   		return TRUE;
		}

}


///*
void wav_process(Bool flag)
{	
	play_wav_num=0;
	while(play_wav_num<all_wav_num){
	if(find_wav(play_wav_num,flag)){
		(void)WavPlayStart();
		}
		play_wav_num++;
	}

/*
    while(play_wav_num<100){
	if(find_wav(play_wav_num)){
		(void)WavPlayStart();
		}
		play_wav_num++;
	}
	
//*/	
}



static const unsigned int SR2PCMSR_TBL[][2] ={{(unsigned int)96000,DAC_SAMPLE_RATE_96K},
                                             {(unsigned int)48000,DAC_SAMPLE_RATE_48K}, 
                                             {(unsigned int)44100,DAC_SAMPLE_RATE_44P1K}, 
                                             {(unsigned int)32000,DAC_SAMPLE_RATE_32K}, 
                                             {(unsigned int)24000,DAC_SAMPLE_RATE_24K}, 
                                             {(unsigned int)22050,DAC_SAMPLE_RATE_22P05K}, 
                                             {(unsigned int)16000,DAC_SAMPLE_RATE_16K}, 
                                             {(unsigned int)12000,DAC_SAMPLE_RATE_12K}, 
                                             {(unsigned int)11025,DAC_SAMPLE_RATE_11P025K}, 
                                             {(unsigned int)9600,DAC_SAMPLE_RATE_9P6K},
                                             {(unsigned int)8000,DAC_SAMPLE_RATE_8K}};
                                    
static unsigned int SmpRate2PCMSR(unsigned int smp_rate)
{
    int i;
    for (i=0; i<sizeof(SR2PCMSR_TBL)/sizeof(SR2PCMSR_TBL[0]); i++)
    {
        if (SR2PCMSR_TBL[i][0] == smp_rate)
            return SR2PCMSR_TBL[i][1];
    }
    return DAC_SAMPLE_RATE_44P1K;
}

///================================================

Bool wav_fix_index(void){

	if(Current_Index==INDEX_GO_AP4){
		WavPlayStop();
//		OP_CLOSE();
//		ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_OID);
		ready_back_mp3(C_OID);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_MP3){
		WavPlayStop();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_MP3);
		ready_back_mp3(C_MP3);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_DIC){
		WavPlayStop();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		//CommonLoadCode(C_DIC);
		ready_back_mp3(C_DIC);
		return TRUE;
		}
	/*
	if(Current_Index==INDEX_PAUSE_PLAY){
			
			return TRUE;
		}
*/
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_FIRST_VOL+6)))
		{
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 return TRUE;
		}

	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
//					 AudioMP3SetVol(VolTbl[vol_idx]);
				}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;
//				 AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}

       if((Current_Index==INDEX_PREV_MP3)){
			WavPlayStop();
			if(play_wav_num>=2)
				{
				play_wav_num -= 2;
				}
			else
				{
				play_wav_num = all_wav_num +play_wav_num-2;
				}
            return TRUE;
       		}
	   
         if((Current_Index==INDEX_NEXT_MP3)){
			WavPlayStop();
            return TRUE;
         	}

	if(Current_Index==INDEX_START_RECORD){
		WavPlayStop();
		wav_action = WAV_RECORD_FLAG;
		//CommonLoadCode(C_REC_PASTER);
		ready_back_mp3(C_REC_PASTER);
		return TRUE;
		}
	
	if(Current_Index==INDEX_PLAY_RECORD)
		{
		WavPlayStop();
		wav_action = WAV_PLAY_FLAG;
		//CommonLoadCode(C_REC_PASTER);
		ready_back_mp3(C_REC_PASTER);
		return TRUE;
		}
	if(Current_Index==INDEX_DEL_RECORD)
		{
		 WavPlayStop();
		(void) WavDel();
//		 OP_CLOSE();
		// ap4_mp3_flag |= BANK_FORM_MP3;
		//b_back_from_mp3 = TRUE;
		 //CommonLoadCode(C_OID);
		 ready_back_mp3(C_OID);
		 return TRUE;
		}
/*	
	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX)){
		WavPlayStop();
		wav_action=Current_Index;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}	
*/	
	return FALSE;
}


static void FillBuffer(void)
{
    unsigned long len;
    __X int *BufPtr;
    void (*Fill_Buf)(void);
    
    if(Frame_Count == 0)
    {
        WavRecState = WAV_STOP;
        return; 
    }
    Frame_Count-- ;
    if(DAC_Buffer1Empty()){
        BufPtr = DAC_LChannelBuffer1 ;
      Fill_Buf = DAC_FillBuffer1;       
    }else if(DAC_Buffer2Empty()) {
        BufPtr = DAC_LChannelBuffer2 ;
        Fill_Buf = DAC_FillBuffer2;
    }
    
    IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[idx_R++], BufPtr);
    if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;
    
    (*Fill_Buf)();
}               

void FirstDecode(void)
{
    unsigned long len;
    int i;
    
    len = FSRead(ADPCM_File, (void *)ADVACE_ADPCM_Buf[0], ADPCM_Buffer_Size*2, &REC_fserr);
    IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[0], DAC_LChannelBuffer1);
    for(i=0;i<ADPCM_BUF_CNT;i++)
    {
        len = FSRead(ADPCM_File, (void *)ADVACE_ADPCM_Buf[i], ADPCM_Buffer_Size*2, &REC_fserr);
        if(len == 0)
            break;
    }                      
    DAC_FillBuffer1();
}





void wav_play_process_init(void)
{

	   // Initial DAC 

    DAC_Init(DTU3, DTU2, FillBuffer, 0);
  
    DAC_LChannelBuffer1 = PCM_Buf1;
    DAC_RChannelBuffer1 = PCM_Buf1;
    DAC_LChannelBuffer2 = PCM_Buf2;
    DAC_RChannelBuffer2 = PCM_Buf2;    
    DAC_FrameSize = PCM_Buffer_Size;     

    idx_L = 0;
    idx_R = 0;                                                    
    FirstDecode();
  	flag_wav_play = TRUE;
    //DAC_HeadPhone_Volume(30); 
    //DAC_Enable_HeadPhone();         //  Headphone output
	//judge_op();
	//DAC_Speaker_Volume(WAV_SPEAKER_VOLUME_INCREASE);
	vol_idx_bak = VOL_NUM;
	SpeakDetect(TRUE);

	WavRecState = WAV_PLAYING;
    DAC_Go();
}

void WavPlayProcess(void)
{
    int len;
    if(Frame_Count==1){
		if(idx_L == idx_R){
			WavRecState = WAV_STOP;
			return;
			}
    	}
	else
		{
	    while(idx_L != idx_R)
	    {
	        len = FSRead(ADPCM_File, ADVACE_ADPCM_Buf[idx_L], ADPCM_Buffer_Size*2, &REC_fserr);
	        if(len == 0)
	        	{
//	        	    WavRecState = WAV_STOP;
					Frame_Count=1;
	            	break;
	        	}
	        idx_L++;
	        if(idx_L > ADPCM_BUF_CNT-1)
	            idx_L = 0;
	    }
		}
}

void WavPlayStop(void)
{
    DAC_Pause();
    DAC_Mute();

    FSClose(ADPCM_File);
    WavRecState = WAV_STOP;
}

Bool WavPlayStart(void)
{
    int i;
    int error;
    unsigned long read_length;
    tIMA_INFO ADPCM_Info;
	FS_ERR err;
	unsigned int temp_key = 0;


    ADPCM_File = FSOpen(tisiFileName,FS_MODE_READ,&REC_fserr);
    if(REC_fserr)
    {
        WavRecState = WAV_ERROR;
        return FALSE;
    }

	    for(i=0; i<PCM_BUF_SIZE; i++){
	    PCM_Buf1[i] = 0;
	    PCM_Buf2[i] = 0;    
    }  
    PCM_Buffer_Size = PCM_BUF_SIZE;
    ADPCM_Buffer_Size = ADPCM_BUF_SIZE; 
//    SDKLoadCode(DAC_COMMON_CODE);
//	swap_code(DAC_CODE_FLAG);
 //read the 60-byte file header 
    read_length = FSRead(ADPCM_File, (void *)ADPCM_File_Header, 60, &REC_fserr);
 
 // Initial ADPCM  Decoder 
 
    error = IMA_ADPCM_Decoder_Init(&ADPCM_Info);  
    if(error != 0)
    {
        WavRecState = WAV_ERROR;
        return FALSE;
    }
        
    DAC_SampleRate = SmpRate2PCMSR(ADPCM_Info.IMA_SR);
//	DAC_SampleRate = dac_rate_bak;
    PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;               // buffer size for output PCM
    ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;       // buffer size for input ADPCM
    Frame_Count = ADPCM_Info.IMA_Frame_No;                          // Tell the user how many frame shall be decoded.

    wav_play_process_init();
    while(1)
    {
    	
        WavPlayProcess();
#ifdef NIEQING_DIANCHI
		Charge_Check();
#endif
		if(rec_led_is_timer_timeout())
			{
			 BlueLed_FLASH();
			 RedLed_OFF();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			}
//    		MP3_state = MP3_PLAY;
			if(sys_led_is_timer_timeout()){
				sys_led_start_timer(SYS_LED_ON_TIME);
				if(CheckUSB()){
					WavPlayStop();;
					CommonLoadCode(C_POWEROFF);
					}
				if(BatteryDetect())
		    			{
		    				WavPlayStop();
//							vol_idx = 0;
							tishi_mp3(FLAG_BATTERY_LOW_FILE);
							OP_CLOSE();
							//save_wav_num();
							//while(1)
							//	{
							//		PowerTurnOff();
							//	}
							CommonLoadCode(C_POWEROFF);
		    			}
			  }
#if 1
		    scan_oid();
			if(index_flag)
				{
				    index_flag = FALSE;
					if(!wav_fix_index()){
						WavPlayStop();
//						OP_CLOSE();
						//ap4_mp3_flag |= BANK_FORM_MP3;
						//b_back_from_mp3 = TRUE;
						//CommonLoadCode(C_OID);
						ready_back_mp3(C_OID);
						}
				}
#endif		   
 			adc_get();
		   kbd_process();
		   temp_key = kbd_key_read();
		   if(0!=temp_key)
		   	{                           
#ifdef TX_DEBUG 
		   	    SendString("\r\ntemp_key=");
		   	    send_hex_u16(temp_key);
#endif			
		   		//Wav_Key_Process(temp_key);
		   		Key_Process(temp_key,FALSE);
		   	}
    	    //UI Code  
    	    
    	    if((g_timer_tick>=auto_poweroff_timer)&&flag_auto_poweroff)
    	    	{
		        	WavPlayStop();
					tishi_mp3(FLAG_POWER_OFF_FILE);
					OP_CLOSE();
					//save_wav_num();
					//PowerTurnOff();
					//if(USB_DET())
					//	{
					//		CommonLoadCode(C_OPEN);
					//	}
					//while(PWD_Key_Press());
					//Delay_ms(5);
					//CommonLoadCode(C_OPEN);
					CommonLoadCode(C_POWEROFF);
    	    	}  	   
		
		SpeakDetect(FALSE);
        if(WavRecState == WAV_STOP)
        {
            WavPlayStop();
            break;
        }
    }

	return TRUE;
}

//*/

 Bool fs_seek( FCFILE __X* pfile, U32 addr )
 {
 	return TRUE;
 }
//--------------------------------------------------------------------
void test_sd(void)
{
	U16 i, temp_data;
	SendString("\r\ntest sd!");
	temp_data = file_get_ext_count((char *)"0:\\MP3",(char *)"MP3");
	for(i=0;i<temp_data;i++){
		if(file_get_path((char *)"0:\\MP3",(char *)"MP3", i)){
			SendString("\r\ni:current_paht=");
			send_hex_u16(i);
			SendString(":");
			SendString(g_full_path);
			}
		}
	temp_data = file_get_ext_count((char *)"1:\\MP3",(char *)"MP3");
	for(i=0;i<temp_data;i++){
		if(file_get_path((char *)"1:\\MP3",(char *)"MP3", i)){
			SendString("\r\ni:current_paht=");
			send_hex_u16(i);
			SendString(":");
			SendString(g_full_path);
			}
		}	
	SendString("\r\ntest sd ok!");
}


void ready_mount_sd(void)
{
	FS_ERR fserr;
	//U8 i;
	//char temp_lab[12];
	//char sys_lab[12];
	//char digi[6];
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
}

//=====================================================
//#pragma CODE = MP3_COMMON_CODE
void Main_system_init(void)
{	
	FS_ERR fserr;
	int drive;
	unsigned int ch0;
	unsigned int ch2;
	
 	SetVector(EXCEPTION_TRAP,Exception);

    // Keyboard I/O port initial
	//System_Timer_Init(3,TIME_TICK);  
	SDKLoadCode(SYSTEM_TIMER3_INIT_CODE);
	System_Timer3_Init();
	kbd_init_process();


	if(!b_oid_ok){
	    reinit_oid_pen();
		b_oid_ok = TRUE;
		}
#ifdef NIEQING_DIANCHI
	Charge_Init();
#endif
	index_flag = FALSE;
	OID_PLAY = FALSE;

   //
	//FSInit(1);
	//fserr = FS_mount(&SD_Drive);
/*	
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
		b_sd2_ok = TRUE;
		}
#endif
*/
	//fserr = FS_mount(&NF_Drive);	
	ready_mount_sd();
	if((!b_sd1_ok)&&(!b_sd2_ok)){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nno sd!");
#endif		
		  Exception();
	 }

	
//	test_sd();
//	SD1_fast_mode();


/*
#if USE_NF
	fserr = FS_mount(&NF_Drive);	
#ifdef  TX_DEBUG
		if( fserr == FS_DRIVE_ERROR ){ 
			SendString(( char *)"The File system initial fail.");
			RESET_SYS();
			}
#endif
#endif
*/
//		 ADC_INIT( );
		//clk_start_timer();
		clkup_time(TRUE);
		op_start_timer();
}


