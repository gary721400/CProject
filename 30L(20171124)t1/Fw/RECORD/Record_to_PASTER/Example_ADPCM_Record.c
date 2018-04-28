// This is KBDM9 Template File. 
/***************************************************************************************
 * Copyright (c)
 * King Billion Electronics Co., Ltd.
 * File Name	: 
 * Author		:  Robin Lin
 * Create Date	: 2009/10/30 
 * History		: 
 * Comment      :  Example code for IMA-ADPCM Encoder and Audio ADC.
                   Recording voice from microphone, converted by Audio ADC.
                   Then compressed by ADPCM encoder and stored wave file to SD card.
 **************************************************************************************/
#include <KBDM9_DDK.H>
#include <fs.h>
#include <string.h>
#include <ctype.h> 
#include "ADC.h"
//#include "KEY_SCAN.H"
#include "SYSTEM.H"
//#include "BinaryImage.h"
#include "com.h"
#include "MP3_API.H"
//#include "mp3_include.h"
#include "keypad.h"
#include "oid.h"
#include "conf_oid.h"
//#include "apf.h"
#include "conf_kb.h"
//#include "agc.h"
#include "fc_internal.h"
#include "filesearch.h"

//0#define SYSTEM_OPEN_FILE		"0:\\subdir\\startup.mp3"
//1#define PASTER_WAV_FILE			"0:\\subdir\\recording.mp3"
//2#define RECORD_BEGIN_WAV_FILE	"0:\\subdir\\recordstart.mp3"
//3#define RECORD_OVER_WAV_FILE	"0:\\subdir\\recordend.mp3"
//4#define PLAY_BEGIN_WAV_FILE		"0:\\subdir\\playstart.mp3"
//5#define PLAY_OVER_WAV_FILE		"0:\\subdir\\playend.mp3"
//6#define NOAPF_WAV_FILE 			"0:\\subdir\\noapffil.mp3"
//7#define POWEROFF_FILE			"0:\\subdir\\poweroff.mp3"
//8#define LOWPOWER_FILE			"0:\\subdir\\batterylow.mp3"


ImageDeclareType(KB_RESIZE_CODE);
ImageDeclareType(SYSTEM_TIMER3_INIT_CODE);



//#define FLAG_RECORD   	0X1111
//#define FLAG_PLAY		0X2222

//DEFINE  ALL BITS OF AP4_MP4_FLAG 
//#define BANK_FORM_MP3       	0X8000
//#define BANK_FORM_RP            0X4000
//#define BANK_HAVE_INDEX     	0X2000
//#define CURRENT_CONTROL_GAME 	0X1000


#define NULL_CMD  0xFFFF
/*
#define INDEX_RECORD_MIN		29001
#define INDEX_RECORD_MAX		30000

#define INDEX_INC_VOL			65321
#define INDEX_DEC_VOL			65524

#define INDEX_GO_MP3			65384
#define INDEX_NEXT_MP3			65385
#define INDEX_PREV_MP3			65386

#define INDEX_START_RECORD		65344
#define INDEX_STOP_RECORD		65345
#define INDEX_PLAY_RECORD		65346
#define INDEX_DEL_RECORD		65347

#define INDEX_PAUSE_PLAY		65387
#define INDEX_GO_AP4			65388
#define INDEX_FIRST_VOL			65389

#define INDEX_GO_GAME_MIN		60001
#define INDEX_GO_GAME_MAX		61438

#define INDEX_EXIT_GAME			61439

#define INDEX_LANG_MIN			65001
#define INDEX_LANG_MAX			65020

#define INDEX_CHANGE_AP4_MIN	61440
#define RANG_CHANGE_AP4			2047

#define INDEX_GO_DIC			65396
*/

#define MAIN_FILE_NAME "rec_"
#define MAX_LENGTH	(32UL*1024*1024)
#define MIN_DISK_SPACE  (1UL*1024*1024)
#define BEGIN_RECORD_SPACE  (2UL*1024*1024)

//#define MAX_WAV_NUM	  200

#define Wav_Pause() 	DAC_Pause()
#define Wav_Continue()  DAC_Continue()

#define Record_Pause()		AUADC_Stop()
//#define Record_Continue()	AUADC_Start()

#pragma CODE = MAIN_CODE
#pragma DATA = MAIN_DATA

#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE		((PCM_BUF_SIZE-1)/4+2)			// word

enum
{
	OID_CMD_RECORD,
	OID_CMD_PLAY,
	OID_CMD_PAUSE,
	OID_CMD_STOP,
	OID_CMD_COPY,
	OID_CMD_BACK,

	OID_CMD_NUM,
	OID_CMD_NONE = OID_CMD_NUM
};

enum{
	IDLE = 0,
    WAV_INIT = 1,
    WAV_RECORD_BEGIN =2,
    WAV_REVCORD_OVER = 3,
    WAV_RECORD_PAUSE = 4,
    WAV_RECORD_CONTIN =5,
    WAV_RECORD_OVER =6,
    WAV_PLAY_BEGIN =7,
    WAV_PLAY_OVER = 8,
    WAV_PLAY_PAUSE = 9,
    WAV_PLAY_CONTIN =10,
    WAV_STOP		=11,
    WAV_READY_PLAY = 12,
};

typedef enum {
	STOP = 0,
	REC,
	PLAY
}tREC_State;

//tLED LED_state;

#define  FS_ATTR_LFN_ENTRY    ((Byte)0x0F)   // LFN entry attribute
/*
extern unsigned int gflag;
extern unsigned int vol_idx;
extern unsigned int ap4_mp3_flag;
extern unsigned int volume_inc_flag;
extern unsigned int bak_current_index;
extern unsigned int begin_R;
extern unsigned int over_R;
extern unsigned int begin_P;
extern unsigned int over_P;
extern unsigned int g_string2[30];
extern   unsigned int remote_index_tab[16];
extern unsigned int systerm_flag;

extern unsigned int wav_action;
*/
extern const tFSDrvHandle SD_Cache_Drive;



#ifdef UNCOIDE_FORMAT
const  U16      root[] = {'0',':','\\','\0'};
const  U8		subdir[] = {0x7C,0xFB,0x7E,0xDF,0x65,0x87,0x4E,0xF6,0x00,0x00};
#endif

#ifdef ASCII_FORMAT
//const  U8       root[] = "0:\\";
//const  U8		subdir[] = "subdir";
#endif

/*
#ifdef UNCOIDE_FORMAT
const U16 SYS_OPEN_FILE[] = {'s','t','a','r','t','u','p','.','m','p','3','\0'};
const U16 PASTER_WAV_FILE[] = {'r','e','c','o','r','d','i','n','g','.','m','p','3','\0'};
const U16 RECORD_BEGIN_WAV_FILE[] = {'r','e','c','o','r','d','.','m','p','3','\0'};
const U16 RECORD_OVER_WAV_FILE[] = {'s','t','o','p','r','e','c','.','m','p','3','\0'};
const U16 PLAY_BEGIN_WAV_FILE[] = {'p','l','a','y','s','t','a','r','t','.','m','p','3','\0'};
const U16 PLAY_OVER_WAV_FILE[] = {'p','l','a','y','e','n','d','.','m','p','3','\0'};
const U16 NOAPF_WAV_FILE[] = {'n','o','a','p','f','f','i','l','e','.','m','p','3','\0'};
const U16 POWEROFF_FILE[] = {'p','o','w','e','r','o','f','f','.','m','p','3','\0'};
const U16 BATTERY_LOW_FILE[] = {'b','a','t','t','e','r','y','l','o','w','.','m','p','3','\0'};
const U16 VOLUME_FILE[] = {'v','o','l','u','m','e','.','m','p','3','\0'};
const U16 DISKFULL[] = {'d','i','s','k','f','u','l','l','.','m','p','3','\0'};
const U16 DELFULL[] = {'d','e','l','.','m','p','3','\0'};
const U16 ERROR[] = {'e','r','r','o','r','.','m','p','3','\0'};
const U16 FILEFULL[] = {'f','i','l','e','f','u','l','l','.','m','p','3','\0'};
const U16 VOL[] = {'v','o','l','.','m','p','3','\0'};
#endif

#ifdef ASCII_FORMAT
const U8 SYS_OPEN_FILE[] = "startup.mp3";
const U8 PASTER_WAV_FILE[] = "recording.mp3";
const U8 RECORD_BEGIN_WAV_FILE[] = "recordstart.mp3";
const U8 RECORD_OVER_WAV_FILE[] = "recordend.mp3";
const U8 PLAY_BEGIN_WAV_FILE[] = "playstart.mp3";
const U8 PLAY_OVER_WAV_FILE[] = "playend.mp3";
const U8 NOAPF_WAV_FILE[] = "noapffile.mp3";
const U8 POWEROFF_FILE[] = "poweroff.mp3";
const U8 BATTERY_LOW_FILE[] = "batterylow.mp3";
const U8 BATTERY_WARN_FILE[] = "batwarning.mp3";
const U8 VOLUME_FILE[] = "volume.mp3";
const U8 DISKFULL_FILE[] = "diskfull.mp3";
const U8 MIN_TISHI[] = "waitprompt1.mp3";
const U8 STOP_MP3[] = "stop.mp3";

#endif
*/
#if 0//def NIEQING_DIANCHI
__X unsigned long nieqing_time;
__X U8  nieqing_count;
#endif

#define MIN_LENGTH  (1UL*1024*1024)

unsigned int 	WRITE_SIZE;
//__X int ADC_Buf1[PCM_BUF_SIZE];
//__X int ADC_Buf2[PCM_BUF_SIZE];

//__X int PCM_Buf[PCM_BUF_SIZE];
//__X int ADPCM_Buf[ADPCM_BUF_SIZE];

extern __X int MbufL1[], MbufL2[], MbufR1[], MbufR2[];
extern __X int AudioMP3FileCatchBuf[];
extern __X int MbufHeap[];

#define  PCM_Buf1      		(( __X int *)MbufL1 )
#define  PCM_Buf2      		(( __X int *)MbufL2 )
#define  ADPCM_Buf			(( __X int *)MbufR1 )

//#define REC_ADPCM_Buf		(( __X int (*)[ ADPCM_BUF_SIZE ]))
#define ADVACE_ADPCM_Buf	(( __X int (*)[ ADPCM_BUF_SIZE ])MbufHeap )

int NotifyFlag, NotifyError;
__X int *NotifyBuf;

__X FS_ERR REC_fserr;
unsigned long WavWriteSize;
__X FS_ERR fserr;
//__X FCFILE *ADPCM_File;   
tAUADC_Err Error;
__X tREC_State REC_state;

//__X int PCM_Buf1[PCM_BUF_SIZE];
//__X int PCM_Buf2[PCM_BUF_SIZE];
unsigned long  Frame_Count;
unsigned int PCM_Buffer_Size, ADPCM_Buffer_Size, Sample_Rate;

//extern unsigned long free_space;
unsigned long wav_space;
//unsigned int  Wav_Count;
		 char dir_path[128];
//char 	 tishi_file_name[128];
char tisiFileName[64];
//extern int DAC_Out; // 0: Headphone, 1: Speaker
Bool first_record_data;

unsigned int temp_string2[30];
char pasterDirName[16];


__X   U32               recorder_wait_time;

//__X  Bool       		 index_flag;
//__X  U16                 Current_Index;
__X  U16 				 State_RP_process;
__X  U16				 Next_CMD;
__X  U16                 paster_index;
 __X	 U16		dac_rate_bak;

__X  U16			wav_num;
//extern unsigned long TimerCount;

unsigned char flag_auto_poweroff;
unsigned long auto_poweroff_timer;
unsigned long current_play_size;
unsigned long wavfile_size;
Bool flag_bigfile;
Bool first_record_data;
Bool flag_allow_record;
Bool flag_apf_ok;
Bool flag_noname;
//char ap4_dir_path_bak[64];
//unsigned int temp_string2[30];
//Bool flag_wav_play;
 FCFILE __X *	fat_g_cur_file;
// Bool OID_PLAY;
#define REC_REMOVE_SEC	8192UL//ZOE_MODIFY //12288UL:1.5UL*8UL*1024UL 8192:1sec 4096:0.5sec

#define MAX_WAV_NUM		(U16)(99)

U16   _MEM_TYPE_SLOW_   g_u16_oid_start;
U16   _MEM_TYPE_SLOW_   g_u16_oid_division;

int idx_L,idx_R;
#define ADPCM_BUF_CNT       2
//__X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
__X  U16     paster_play_number;
__X unsigned int Cache[256*CacheNum];
ImageDeclareType(APF_CODE);

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size);
void Key_Process(U16 Key_Code);    
Bool RecStart(void);
void RecProcess(void);
void RecStop(void);
Bool WavPlay(void);
void WavStop(void);
void FillBuffer(void);
void FirstDecode(void);
U16	 Paster_judge(void);
void Rec_process_init(void);
void wav_play_process_init(void);
void Paster_process(void);
//void tishi_wav_play(void);
void tishi_mp3(U8 flag);
Bool dir_create(Bool flag_create);
void led_control(void);
void oid_remote(void);
void Record_Continue(void);
void WavPlayProcess(void);
Bool find_wav(U16 num);
void readyPasterDirName(void);
extern int	file_get_ext_count(char * dir_path,char * ext);
//Bool find_wav(Bool flag,U16 temp_num);
//Bool fix_index(void);
//Bool WavDel(void);
//void judge_op(void);
extern void DAC_Stereo_to_Mono(unsigned Option);
FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );
//void Wav_Continue(void);
//void SpeakDetect(void);
/*void ErrHappen(const char *string,unsigned int fserr)
{
    while(1);
}*/

Bool fs_seek(unsigned long addr)
{
	return FALSE;
}
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




void RecProcess(void)
{
#if 1
	clkup_time(FALSE);
    while(idx_L != idx_R)
    {
        WavWriteSize += FSWrite(ADPCM_File, (void *)ADVACE_ADPCM_Buf[idx_L++], ADPCM_BUF_SIZE*2, &fserr);
        if(idx_L > (ADPCM_BUF_CNT-1))
            idx_L = 0;
    }

    if((wav_space-WavWriteSize)<512UL*1024UL)
    	{
    		wav_space += BEGIN_RECORD_SPACE;
//			FSResize(ADPCM_File, wav_space);
			SDKLoadCode(KB_RESIZE_CODE);
			fserr = kb_fat_resize(ADPCM_File, wav_space);
			if(fserr != FS_NO_ERROR){
				State_RP_process = WAV_RECORD_OVER;
				}
			//if (((free_space - wav_space)< 1024UL*32UL)) {
			//	State_RP_process = WAV_RECORD_OVER;
			//}
    	}

		BlueLed_OFF();
    	RedLed_ON();

#else
	if (NotifyFlag > 0)
	{
		NotifyFlag--;
	 
		memcpy(PCM_Buf, NotifyBuf, PCM_BUF_SIZE*2);
		IMA_ADPCM_Encoder(PCM_Buf, ADPCM_Buf);	
	
		WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_Buf, ADPCM_BUF_SIZE*2, &fserr);      

		if (((free_space-WavWriteSize)< 1024UL*32UL)) {
//		if ((WavWriteSize > 1024UL*61440UL)|| ((free_space-WavWriteSize)< 1024UL*32UL)) {
//			RecStop();
//			ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
//			CommonLoadCode(C_OID);
			State_RP_process = WAV_RECORD_OVER;
		}				    
	}
#endif	

}

void RecStop(void)
{

	AUADC_Stop();
    
//	FSResize(ADPCM_File, WavWriteSize);   
    SDKLoadCode(KB_RESIZE_CODE);
	fserr = kb_fat_resize(ADPCM_File, WavWriteSize);

	FSSeek(ADPCM_File, 0);
		
	FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);	
	FSFlush(( __X FCFILE *)ADPCM_File );     
	fserr = FSClose(ADPCM_File);

	clr_ex();
	
	//free_space = free_space - WavWriteSize;

	REC_state = STOP;
	flag_auto_poweroff=TRUE;
#ifdef  TX_DEBUG  
	if(fserr){
		SendString("\r\nClose file(SD) is fail.\r\n");
		}
#endif	
	//Delay_ms(50);
//	save_wav_num();
    recorder_wait_time = 0;
	op_start_timer();
	clkup_time(TRUE);
	paster_play_number = current_wav_num;
}
/*
U16 oid_cmd_trans( U16 oid_value )
{
   U8 iter = 0;
   if( !oid_value )
      return OID_CMD_NUM;
   for( iter = 0; iter < OID_CMD_NUM; ++iter )
   {
      if( oid_value == g_oid_cmd[ iter ])
         return iter;
   }
   return OID_CMD_NUM;
}
*/
U16	Paster_judge(void)
{
if(flag_apf_ok){
	if((paster_index >= g_u16_oid_start)&&(paster_index <= (g_u16_oid_start+g_u16_oid_division-1)))
		{
		flag_noname=TRUE;
		 return OID_CMD_RECORD;
		}
		
	if((paster_index >= (g_u16_oid_start+g_u16_oid_division))&&(paster_index <= (g_u16_oid_start+g_u16_oid_division+g_u16_oid_division-1)))
		{
		 paster_index = paster_index-g_u16_oid_division;
		 flag_noname=TRUE;
		 return OID_CMD_PLAY;
		}
	
	if(flag_allow_record){

	 	 return OID_CMD_RECORD;	
	 	}
	 else{

	 	return OID_CMD_PLAY;
	 	}
	//return oid_cmd_trans(paster_index);
	}
else{
	 if(flag_allow_record){
	 	 return OID_CMD_RECORD;	
	 	}
	 else{
	 	return OID_CMD_PLAY;
	 	}
	}
}

void Buffer_Full_Func(__X int* buffer, unsigned int buf_size)
{	
#if 1
    if (NotifyFlag > 0){
        
        NotifyError = 1;
    }   
/*
#if ( OID_VERSION == OID1 )
    if(	first_record_data )
    	{
			first_record_data = FALSE;
			return;
    	}
#endif	
*/
	IMA_ADPCM_Encoder(buffer, ADVACE_ADPCM_Buf[idx_R++]);
	if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;


	Frame_Count++;
//	if(Frame_Count > 32){
//		  AGC(buffer);
//	}
	
#else
	if (NotifyFlag > 0){
		
		NotifyError = 1;
	}	
	NotifyFlag++;
	
	NotifyBuf = buffer;
#endif	
}


/*
void inc_volume(void)
{
if(SpeakerDET())
	{
		OP_OPEN();
		if(flag_wav_play){
			DAC_Speaker_Volume(WAV_SPEAKER_VOLUME_INCREASE);
			}
		else{	
			DAC_Speaker_Volume(MP3_SPEAKER_VOLUME_INCREASE);
			}
	}
    else
	{
		OP_CLOSE();
		DAC_Speaker_Volume(4);
	}
}
*/

void change_paster(void)
{
	switch(Paster_judge())
		{
			case OID_CMD_RECORD:
				if(REC_state == PLAY)
					{
					 State_RP_process = WAV_PLAY_OVER;
					 Next_CMD = WAV_RECORD_BEGIN;
					}
				if(REC_state == REC)
					{
					 State_RP_process = WAV_RECORD_OVER;
					 Next_CMD = WAV_RECORD_BEGIN;
					}
				if(REC_state == STOP )
					{
					State_RP_process = WAV_RECORD_BEGIN;
					}
				break;
			case OID_CMD_PLAY:
				if(REC_state == PLAY)
					{
					 State_RP_process = WAV_PLAY_OVER;
					 Next_CMD = WAV_PLAY_BEGIN;
					}
				if(REC_state == REC)
					{
					 State_RP_process = WAV_RECORD_OVER;
					 Next_CMD = WAV_PLAY_BEGIN;
					}
				if(REC_state == STOP )
					{
					State_RP_process = WAV_PLAY_BEGIN;
					}
				break;
			case OID_CMD_STOP:
				State_RP_process = WAV_STOP;
				break;
			case OID_CMD_PAUSE:
			if(State_RP_process == WAV_RECORD_PAUSE)
				{
				 State_RP_process = WAV_RECORD_CONTIN;
				 Record_Continue();
				 break;
				}
			if(State_RP_process == WAV_RECORD_CONTIN)
				{
				 State_RP_process = WAV_RECORD_PAUSE;
				 Record_Pause();
				 break;
				}
		   if(State_RP_process == WAV_PLAY_PAUSE)
				{
				 State_RP_process = WAV_PLAY_CONTIN;
				 Wav_Continue();
				 break;
				}
			if(State_RP_process == WAV_PLAY_CONTIN)
				{
				 State_RP_process = WAV_PLAY_PAUSE;
				 Wav_Pause();
				 break;
				}
				break;
			default:
				break;
		}
}

///================================================
Bool fix_index_01(void)
{
/*
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
	*/
	if(Current_Index==INDEX_GO_MP3){
        kbd_begin_write(KBD_KEY_PRESSED, KEY_MODE);
        return TRUE;
		}
///*	
	if(Current_Index == INDEX_STOP){
		
		//tishi_addr(rec_stop_addr);
		if(REC_state == PLAY)WavStop();
		if(REC_state == REC)RecStop();
		tishi_mp3(FLAG_STOP_MP3);
		State_RP_process = IDLE;
		return TRUE;
		}
//*/		
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
		{
		 vol_idx=Current_Index-INDEX_FIRST_VOL;
		 if(REC_state == STOP)tishi_mp3(FLAG_VOLUME);
		 return TRUE;
		}
	if((Current_Index==INDEX_INC_VOL))
		{
	   	 kbd_begin_write(KBD_KEY_RELEASED, KEY_VOL_INC);	
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	  kbd_begin_write(KBD_KEY_RELEASED, KEY_VOL_DEC);  
         return TRUE;
		}

	if((Current_Index==INDEX_PLAY_RECORD))
		{
	 	 kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_REC);  
         return TRUE;
		}
	if((Current_Index==INDEX_START_RECORD))
		{
	 	 kbd_begin_write(KBD_KEY_RELEASED, KEY_REC);  
         return TRUE;
		}

	return FALSE;
}



const int PlayStatusTable[] = {PLAYING_STATUS,
                         PAUSE_STATUS,
                         AB_REPEAT_STATUS,
                         A_STAGE_STATUS,
                         FWRWD_PROLOG_STATUS,
                         FORWARD_STATUS,
                         REWIND_STATUS,
                         FWRWD_EPILOG_STATUS,
                         (-1)}; 




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



void main(void)
{
	int drive, status;
	unsigned int i;
    U16 temp_key=0;
    unsigned int count;
    
	char apf_file[64];
    
	
	IP_INIT();
// 	GPIO_init();
//	PDEN = 0;
	BlueLed_ON();
	RedLed_OFF();
 	ADC_init();
//	PowerKeepOn();
	SystemClock(SYS_CLOCK);
	SDKLoadCodInit();  
	SetVector(EXCEPTION_TRAP,Exception);
//    Wav_Count = 0;
//	RP_flag = 0;
#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"\r\nKB paster record begin!");
#endif
	
	recorder_wait_time = 0;
//	flag_wav_play = TRUE;
//	begin_P = 65535;
//	begin_P = 65525;
//	bak_current_index = 65530;
//	strcpy((char *)g_string2,"test.ap4");
	/*
	for(i=0; i<PCM_BUF_SIZE; i++){
		ADC_Buf1[i] = 0;
		ADC_Buf2[i] = 0; 	
	}  
*/
	
//	System_Timer_Init(3,TIME_TICK);  
	
	SDKLoadCode(SYSTEM_TIMER3_INIT_CODE);
	System_Timer3_Init();
	  
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
	

 	//b_sd2_ok = FALSE;
	// Initial Audio ADC
//	SDKLoadCode(AUADC_COMMON_CODE); 
	kbd_init_process();
///*
	if(!b_oid_ok){
	    reinit_oid_pen();
		b_oid_ok = TRUE;
		}
	scan_oid_disable();
//*/	
	index_flag=FALSE;
#if 0
	 strcpy(dir_path,PASTER_DIR);
	 fserr = FSDirectory(dir_path);


	 State_RP_process = IDLE;
	 REC_state = STOP;
	 Next_CMD = NULL_CMD;
#endif	
	 flag_auto_poweroff = TRUE;
	readyPasterDirName();
	/*
	 if(b_sd2_ok){
	 	strcpy(dir_path,(char *)SD2_PASTER_DIR);
	 	}
	 else{
	 	//strcpy(dir_path,(char *)SD1_PASTER_DIR);
	 	strcpy(dir_path,pasterDirName);
		strcat(dir_path,"\\");
	 	}*/

	 
	 //fserr = FSDirectory(dir_path);

#ifdef  TX_DEBUG  			
	SendString("\r\ndir_path=");
	SendString(dir_path);
#endif
     recorder_wait_time = 0;
	 State_RP_process = IDLE;
	 REC_state = STOP;
	 Next_CMD = NULL_CMD;
	 flag_auto_poweroff = TRUE;
	 flag_allow_record = FALSE;
	 flag_apf_ok = FALSE;
	 //led_start_timer(LED_NORMAL_TIME);
  

	Sample_Rate = 16000;

//	DAC_SampleRate = DAC_SAMPLE_RATE_16K;
	dac_rate_bak	= DAC_SAMPLE_RATE_16K;


#ifdef NIEQING_DIANCHI
	Charge_Init();
#endif

///*
	 if(!b_startup_play){
	 	b_startup_play = TRUE;
//	 	strcpy(tishi_file_name,SYSTEM_OPEN_FILE);
//	 	tishi_wav_play();
		tishi_mp3(FLAG_SYS_OPEN_FILE);
	 	}

//	 strcpy(tishi_file_name,PASTER_WAV_FILE);
//	 tishi_wav_play();
	// tishi_mp3(FLAG_RECORDING);

	flag_apf_ok = FALSE;
/*
	SDKLoadCode(APF_CODE);
	 if(!apf_init())
	 	{
//	 		strcpy(tishi_file_name,NOAPF_WAV_FILE);
//	 		tishi_wav_play();
			//tishi_mp3(FLAG_NOAPF);
			flag_apf_ok = FALSE;
#if 0	 	
			ap4_mp3_flag = 0;
			ap4_mp3_flag |= 0x8000;
			CommonLoadCode(C_OID);
//#else
			volume_inc_flag |= 0x8000;
			CommonLoadCode(C_OPEN);
#endif
	 	}
	  else{
	 	flag_apf_ok = TRUE;
	 	}*/

 if(b_sd2_ok){
	 	strcpy(dir_path,(char *)SD2_PASTER_DENTRY);
	 	}
	 else{
	 	//strcpy(dir_path,(char *)SD1_PASTER_DENTRY);
	 	strcpy(dir_path,pasterDirName);
	 	}
		wav_num = (U16)file_get_ext_count((char *)dir_path, (char *)WAV_EXT);
#ifdef TX_DEBUG
	SendString("\r\nwav_num=");
	send_hex_u16(wav_num);
#endif		
	for(i= 0;i < wav_num ;i++){
		if(file_get_path((char *)dir_path, (char *)WAV_EXT,i)){
			SendString("\r\nnum:path=");
			send_hex_u16(i);
			SendString(":");
			SendString(g_full_path);
			}
		}
	if( wav_num != current_wav_num){
		current_wav_num = wav_num;
		}
	else{
		}

		
//*/
//		tishi_mp3(3);

//#if ( OID_VERSION == OID1 )
	   
	//   scan_oid();
	   index_flag=FALSE;
	   
	   REC_state = STOP;
//	   paster_index=INDEX_RECORD_MIN+1;
 	   paster_index=0;
//#endif 
		Next_CMD=NULL_CMD;
///*
	 if(wav_action==WAV_RECORD_FLAG){	
	 		State_RP_process = WAV_RECORD_BEGIN;
	 	}
	 else if(wav_action==WAV_PLAY_FLAG){
	 	State_RP_process = WAV_PLAY_BEGIN;
	 	}
	 else{
		State_RP_process = IDLE;
	 	}
	wav_action = 0;
//*/	

//

	if(current_wav_num == 0){
		paster_play_number = 1;
		}
	else{
		paster_play_number = current_wav_num;
		}
#ifdef TX_DEBUG
	SendString("\r\npaster_play_number=");
	send_hex_u16(paster_play_number);
#endif
	//
//	DAC_Disable_Speaker();
//	DAC_Enable_HeadPhone();
	scan_oid_enable();
//	scan_oid_disable();
	index_flag = FALSE;
	clkup_time(TRUE);
	while(1)
	{
		adc_get();
#if 1

		if(sys_led_is_timer_timeout()){
			sys_led_start_timer(SYS_LED_ON_TIME);
			op_time_dowith();
			clk_time_dowith();
			if(CheckUSB()){
				if(REC_state == PLAY)WavStop();
				if(REC_state == REC)RecStop();
				//CommonLoadCode(C_USB);
				b_find_usb_flag = TRUE;
         		 CommonLoadCode(C_POWEROFF);
				}
			if(BatteryDetect())
    			{
    				clkup_time(FALSE);
    				if(REC_state == PLAY)WavStop();
					if(REC_state == REC)RecStop();
					vol_idx = 0;
					tishi_mp3(FLAG_BATTERY_LOW_FILE);
					OP_CLOSE();
					AUADC_Power_Down();
					CommonLoadCode(C_POWEROFF);
    			}
		  }

			if((REC_state == STOP)&&flag_auto_poweroff)
				{
#ifdef TX_DEBUG
				SendString("\r\nautoclose!");
#endif					
				  flag_auto_poweroff=FALSE;
				  g_b_tishi_2 = FALSE;
				  //b_2min_tishi = TRUE;
				  auto_poweroff_timer = Timer_set_timer(TIMER_MIN(TISHI_TIME));
				  op_start_timer();
				  clkup_time(TRUE);
				}

			
    	    if((g_timer_tick>=auto_poweroff_timer)&&!flag_auto_poweroff&&(REC_state == STOP))
    	    	{   
						//if(REC_state == PLAY)WavStop();
						//if(REC_state == REC)RecStop();
						clkup_time(TRUE);
						if(!g_b_tishi_2){
							tishi_mp3(FLAG_MIN_TISHI);
							g_b_tishi_2 = TRUE;
							op_start_timer();
							auto_poweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME));
							}
						else{
							
							tishi_mp3(FLAG_POWER_OFF_FILE);
							OP_CLOSE();
							AUADC_Power_Down();
							//save_wav_num();
							//PowerTurnOff();
							//CommonLoadCode(C_OPEN);
							CommonLoadCode(C_POWEROFF);
							}
    	    	} 
		
#endif

		led_control();
// Keypad process
//	   CheckUSB();
///*	   
#ifdef  TX_DEBUG  			
//		SendString("\r\n825");
#endif
	
	   scan_oid();
#ifdef  TX_DEBUG  			
//		SendString("\r\n829");
#endif
	   if(index_flag)
	   	{
#if 0//def  TX_DEBUG  			
			SendString("\r\ncurrent_index=");
			send_hex_u16(Current_Index);
#endif	   	
	   		index_flag=FALSE;
			
			if( recorder_wait_time && !Timer_is_timeout( recorder_wait_time )){
				 	goto record_loop;
					}
			//SysClkUpgrade();
			//sysup_opstart();
			clkup_time(TRUE);
			recorder_wait_time = 0;
			paster_index = Current_Index;
			//if(!fix_index_01()){
				if(REC_state == PLAY)WavStop();
				if(REC_state == REC)RecStop();
				bak_current_index = Current_Index;
				b_back_from_mp3 = TRUE;
				b_back_have_index = TRUE;
				
				AUADC_Power_Down();
				CommonLoadCode(C_OID);
				//}
			//(void)change_paster();
//			paster_index = Current_Index;
	//		State_RP_process = WAV_RECORD_OVER;
	/*
			if(!fix_index()){
				 if(REC_state == REC){
		    		RecStop();
//					tishi_mp3(3);
		    		}
		    	if(REC_state == PLAY){
		    		WavStop();
		    		}
				AUADC_Power_Down();
				wav_action = 0;
				State_RP_process = IDLE;
//				OP_CLOSE();
				//ap4_mp3_flag |= BANK_FORM_MP3;
				b_back_from_mp3 = TRUE;
				CommonLoadCode(C_OID);
				*/

			
#if 0			
			oid_remote();
#endif



	   	}
#if 1   
	   kbd_process();
	   temp_key = kbd_key_read();
	   if(0!=temp_key){ 
	   	clkup_time(TRUE);
	   	if( recorder_wait_time && !Timer_is_timeout( recorder_wait_time )){
			goto record_loop;
			}
		    recorder_wait_time = 0;
			//SysClkUpgrade();
			
#ifdef  TX_DEBUG   	
	   	    SendString("\r\ntemp_key=");
	   	    send_hex_u16(temp_key);
#endif		
	   		Key_Process(temp_key);
	   	}
#endif

record_loop:

#if 0	   
		if(REC_state == REC)
        	RecProcess();
        else if(REC_state == PLAY)
        {
	        SpeakDetect();
	        if(Frame_Count == 0)
	        	{
	        		WavStop();
					ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
					CommonLoadCode(C_OID);
	        	}
		}	
#else
		Paster_process();
#endif		
	}   
}	

void Paster_process(void)
{
	switch(State_RP_process)
		{
			case IDLE:
				{
					break;
				}
			case WAV_RECORD_BEGIN:
//				strcpy(tishi_file_name,RECORD_BEGIN_WAV_FILE);
//				tishi_wav_play();
				tishi_mp3(FLAG_RECORDSTART);
				if(RecStart())
					{
#ifdef  TX_DEBUG  			
					SendString("\r\nrecord begin!");
#endif					
					State_RP_process = WAV_RECORD_CONTIN;
					}
				else
					{
					State_RP_process = IDLE;
					}
				break;
			case WAV_RECORD_PAUSE:
				break;
			case WAV_RECORD_CONTIN:
				RecProcess();
				break;
			case WAV_RECORD_OVER:
				RecStop();
#ifdef  TX_DEBUG  			
					SendString("\r\nrecord over!");
#endif					
				tishi_mp3(FLAG_RECORDEND);
				if(Next_CMD!=NULL_CMD)
					{
					State_RP_process = Next_CMD;
					Next_CMD = NULL_CMD;
					}
				else
					{
					State_RP_process = IDLE;
					}
				break;
			case WAV_PLAY_BEGIN:
				tishi_mp3(FLAG_PLAYSTART);
				if(WavPlay())
					{
#ifdef  TX_DEBUG  			
					SendString("\r\nplay begin!");
#endif						
					State_RP_process = WAV_PLAY_CONTIN;
					}
				else
					{
					 REC_state = STOP;
					 State_RP_process = IDLE;
					 //State_RP_process = WAV_READY_PLAY;
					}
				break;
			case WAV_PLAY_PAUSE:
				break;
			case WAV_PLAY_CONTIN:
			     SpeakDetect(FALSE);
				 clkup_time(FALSE);
				 WavPlayProcess();
			     if(Frame_Count == 0)
		        	{
		        	 State_RP_process = WAV_PLAY_OVER;
		        	}
				break;
			case WAV_PLAY_OVER:
				WavStop();
#ifdef  TX_DEBUG  			
					SendString("\r\nplay over!");
#endif	
				tishi_mp3(FLAG_PLAYEND);
				if(Next_CMD!=NULL_CMD)
					{
					State_RP_process = Next_CMD;
					Next_CMD = NULL_CMD;
					}
				else
					{
					State_RP_process = IDLE;
					}
				//State_RP_process = WAV_READY_PLAY;
				break;
			case WAV_STOP:
				{
				if(REC_state == REC)
	        		{
	        			RecStop();
//						strcpy(tishi_file_name,RECORD_OVER_WAV_FILE);
//						tishi_wav_play();
					}
	             if(REC_state == PLAY)
	             	{
	        			WavStop();
	             	}
//				strcpy(tishi_file_name,RECORD_OVER_WAV_FILE);
//				tishi_wav_play();
				//tishi_mp3(3);
				State_RP_process = IDLE;
					
				}
				break;
			case WAV_READY_PLAY:

				if(paster_play_number>1){
					paster_play_number--;
					}
				else{
					paster_play_number = MAX_WAV_NUM;
					}
				State_RP_process = WAV_PLAY_BEGIN;
				break;
			default:
				break;
		}
}

///=================================================
void oid_remote(void)
{
    unsigned int i;

	for(i=0;i<REMOTE_INDEX_MAX;i++)
		{
			if(paster_index == remote_index_tab[i])
				{
					switch (i)
						{
							case REMOTE_VOL_UP:
								if(REC_state == PLAY)
									{
										if(vol_idx<(VOL_NUM-1))vol_idx++;
										DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
										DAC_Enable_HeadPhone();
									}
								break;
							case REMOTE_VOL_DOWN:
								if(REC_state == PLAY)
									{
										if(vol_idx>0)vol_idx--;
										DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
										DAC_Enable_HeadPhone();
									}
								break;
							case REMOTE_POWER_OFF:
								if(REC_state == REC){
					        		RecStop();
									}
					        	if(REC_state == PLAY){
					        		WavStop();
					        		}
								//save_wav_num();
								AUADC_Power_Down();
								 //PowerTurnOff();
								 //if(USB_DET()){
								//	CommonLoadCode(C_OPEN);
								//	}
								CommonLoadCode(C_POWEROFF);
								break;

							case REMOTE_AP4_MODE:
								{
								 if(REC_state == REC){
					        		RecStop();
								 }
					        	if(REC_state == PLAY){
					        		WavStop();
					        	 }
								 AUADC_Power_Down();
								 //ap4_mp3_flag = 0;
								// ap4_mp3_flag |= 0x8000;
								b_back_from_mp3 = TRUE;
								 CommonLoadCode(C_OID);
								}
								break;
							case REMOTE_EXIT_MP3:
							case REMOTE_MP3_PLAY_PAUSE:
							case REMOTE_MP3_PREV:
							case REMOTE_MP3_NEXT:
								break;
							case REMOTE_MP3_MODE:
								{
								 if(REC_state == REC){
					        		RecStop();
								 	}
					        	if(REC_state == PLAY){
					        		WavStop();
					        		}
								 AUADC_Power_Down();
								// ap4_mp3_flag = 0;
								 //ap4_mp3_flag |= 0x8000;
								 b_back_from_mp3 = TRUE;
								 CommonLoadCode(C_MP3);
								}
								break;
							case REMOTE_REC_MODE:
								break;
							case REMOTE_PASTER_MODE:
								break;
							case REMOTE_REC_START:
								break;
							case REMOTE_REC_PLAY:
								break;
							case REMOTE_REC_STOP:
								break;

						}
				}
		}

	
}




Bool dir_create(Bool flag_create)
{
	U16 i,current_dir_mask,mask,temp_data_1;
	char digi[6];
	U16 temp_index;
	U32 temp_file_size=0;
	temp_index = paster_index;
//	memset(dir_path,0,512);
/*
	if(flag_apf_ok&&flag_noname){
			flag_noname=FALSE;
			strcpy(dir_path,PASTER_DIR);
		}
	else{

			if(strcmp(ap4_dir_path,ap4_dir_path_bak)!=0){
				strcpy(ap4_dir_path,ap4_dir_path_bak);
				}
			strcpy(dir_path,ap4_dir_path);
		}
*/	
//strcpy(dir_path,PASTER_DIR);
 if(b_sd2_ok){
	 	strcpy(dir_path,(char *)SD2_PASTER_DIR);
	 	}
	 else{
	 	//strcpy(dir_path,(char *)SD1_PASTER_DIR);
	 	strcpy(dir_path,pasterDirName);
		strcat(dir_path,"\\");
	 	}
if(flag_create)
	{	
		 current_dir_mask = 0xF000;
		 mask = 0xF000 >> 4;
		 while(mask)
			{
			        temp_data_1 = current_dir_mask&temp_index;
					digi[0] = '0'+temp_data_1/10000;
					digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
					digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
					digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
					digi[4] = '0'+temp_data_1%10;
					digi[5] = 0;

					strcat(dir_path,"REC");
		    		strcat(dir_path,digi);
//					strcat(dir_path,"\\");
#ifdef  TX_DEBUG 	
					SendString("\r\ndir_path=");
					SendString(dir_path);
#endif					
					fserr = FSDirectory(dir_path);
					current_dir_mask |= mask;
		      		mask >>= 4;
					strcat(dir_path,"\\");
			}
 			temp_data_1 = current_dir_mask&temp_index;
			digi[0] = '0'+temp_data_1/10000;
			digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+temp_data_1%10;
			digi[5] = 0;
			strcat(dir_path,"rec");
	 		strcat(dir_path,digi);
			strcat(dir_path,".wav");
#ifdef  TX_DEBUG 	
					SendString("\r\ncreate_dir_path=");
					SendString(dir_path);
#endif				
			temp_file_size = FSGetFileSize(dir_path,&fserr);
			if(temp_file_size>0)
				{
				   //free_space += temp_file_size;
				   return TRUE;
				}
			else
				{
					return FALSE;
				}
	}
	else
	{
		 current_dir_mask = 0xF000;
		 mask = 0xF000 >> 4;
		 while(mask)
			{
			        temp_data_1 = current_dir_mask&temp_index;
					digi[0] = '0'+temp_data_1/10000;
					digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
					digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
					digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
					digi[4] = '0'+temp_data_1%10;
					digi[5] = 0;

					strcat(dir_path,"rec");
		    		strcat(dir_path,digi);
					strcat(dir_path,"\\");
#ifdef  TX_DEBUG 	
					SendString("\r\ndir_path=");
					SendString(dir_path);
#endif					
					current_dir_mask |= mask;
		      		mask >>= 4;
			}
 			temp_data_1 = current_dir_mask&temp_index;
			digi[0] = '0'+temp_data_1/10000;
			digi[1] = '0'+((temp_data_1 -(digi[0]-'0')*10000)/1000);
			digi[2] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000))/100;
			digi[3] = '0'+((temp_data_1 -(digi[0]-'0')*10000-(digi[1]-'0')*1000-(digi[2]-'0')*100))/10;
			digi[4] = '0'+temp_data_1%10;
			digi[5] = 0;
			strcat(dir_path,"rec");
	 		strcat(dir_path,digi);
			strcat(dir_path,".wav");
#ifdef  TX_DEBUG 	
					SendString("\r\ndir_path=");
					SendString(dir_path);
#endif				
		 wavfile_size =FSGetFileSize(dir_path,&fserr);
		  if(wavfile_size==0)
			{
				return FALSE;
			}
		  
		   if(wavfile_size>(REC_REMOVE_SEC*2)){
		   	  flag_bigfile=TRUE;//ZOE_CHANGE
		   }else{
		      flag_bigfile=FALSE;//ZOE_CHANGE
		      return FALSE;
		   }
		   
		   return TRUE;
	}
}




Bool find_wav(U16 num)
{
	U16 i,current_dir_mask,mask,temp_data_1;
	char digi[4];
	U16 temp_index;
	U32 temp_file_size=0;
	temp_index = num;
	//strcpy(dir_path,PASTER_DIR);
	 if(b_sd2_ok){
	 	strcpy(dir_path,(char *)SD2_PASTER_DIR);
	 	}
	 else{
	 	//strcpy(dir_path,(char *)SD1_PASTER_DIR);
	 	strcpy(dir_path,pasterDirName);
		strcat(dir_path,"\\");
	 	}
	digi[0] = '0';
	digi[1] = '0'+temp_index/10;
	digi[2] = '0'+temp_index%10;
	digi[3] = 0;
	strcat(dir_path,"REC");
	strcat(dir_path,digi);
	strcat(dir_path,".WAV");
#ifdef  TX_DEBUG 	
	SendString("\r\ndir_path:size=");
	SendString(dir_path);
#endif		

 	wavfile_size = FSGetFileSize((char *)dir_path,&fserr);
#ifdef  TX_DEBUG 	
	SendString(":");
	send_hex_u32(wavfile_size);
#endif	
  if(wavfile_size==0)
	{

		return FALSE;
	}
  
   if(wavfile_size>(REC_REMOVE_SEC*2)){
   	  flag_bigfile=TRUE;
   }else{
      flag_bigfile=FALSE;
   }   
   return TRUE;
}

/*
Bool dir_create(Bool flag_create)
{
	U16 i,current_dir_mask,mask,temp_data;
	char digi[4];
	U16 temp_index;
	unsigned long temp_leng=0;

	temp_index = paster_index-INDEX_RECORD_MIN+2;
	paster_index=0;
	strcpy(dir_path,PASTER_DIR);
	temp_data = temp_index/2;
	
	digi[0] = '0'+temp_data/100;
	digi[1] = '0'+((temp_data -(digi[0]-'0')*100)/10);
	digi[2] = '0'+temp_data%10;
	digi[3] = 0;
//			strcat(dir_path,"rec");
 	strcat(dir_path,digi);
	strcat(dir_path,".wav");
	temp_leng = FSGetFileSize(dir_path,&fserr);
if(flag_create)
	{	
			if(temp_leng>0)
				{
				   free_space += temp_leng;
				   return TRUE;
				}
			else
				{
					return FALSE;
				}
	}
	else
	{			
		
		  if(temp_leng==0)
			{
				return FALSE;
			}
		   return TRUE;
	}
}

void tishi_wav_play(void)
{

	    FS_ERR fs_err;
	    tAudioErr   Audio_err;
		U32 len;
		len = FSGetFileSize((char *)tishi_file_name, &fs_err);
		if(0==len)return;
		
        Audio_err = AudioMP3Init();
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 Audio_err = AudioMP3Uninit();
        	 return;
        	}
        Audio_err = AudioMP3Open((signed char *)tishi_file_name);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        		Audio_err = AudioMP3Uninit();
				return;
        	}
        AudioMP3SetEQ(EQ_NOMAL);
		SpeakDetect();
        Audio_err = AudioMP3Play();
    	while (CheckPlaying())
    	{    		
	    	
    	}
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();
}
*/
Bool RecStart(void)
{
//	char path[256];
//	char digi[6];
//	int file_count;
//	unsigned long size;
//	free_space = 0;
//	free_space = FSFreeSpace(0,&fserr);	
	unsigned char i;
	U16 temp_num;
	Bool flag_overlab = FALSE;
    //if(free_space<=(MIN_DISK_SPACE)) 
    //	{
//    		ap4_mp3_flag |= (BANK_FORM_MP3|BANK_FORM_RP);
//			CommonLoadCode(C_OID);
	//		tishi_mp3(FLAG_DISKFULL);
		//    return FALSE;
    	//}
	// Craete a file for writing.
//	file_count = 0;
//	if(RP_flag&0x8000)

//	OP_CLOSE();
	current_wav_num ++;
	 if(current_wav_num>MAX_WAV_NUM){
	 	current_wav_num = 1;
	 	}
	 temp_num = current_wav_num;

	if(!find_wav(temp_num))goto create_file;
	ADPCM_File = FSOpen(dir_path,FS_MODE_READ_WRITE,&fserr);
	goto begin;
create_file:
	ADPCM_File = FSOpen(dir_path,FS_MODE_CREATE,&fserr); 				 
begin:
	current_wav_num=temp_num;
    if(fserr)
    {
         return FALSE;
    }
//	wav_space = MIN_LENGTH;
//    FSResize(ADPCM_File, wav_space);
	
	index_flag = FALSE;	
//	ADPCM_File = FSOpen(dir_path,FS_MODE_CREATE,&fserr);    
#ifdef  TX_DEBUG	
	if(fserr)
		SendString("\r\nFail to open file from SD card.\r\n");
	SendString("\r\n rec wav = ");
	send_hex_u16(current_wav_num);
#endif	
#if 0
    if(free_space>(MAX_LENGTH/512)){  
		free_space = MAX_LENGTH;
//		FSResize(ADPCM_File, MAX_LENGTH);
    	}
	else 
		{
		 free_space = free_space*512U;
//		 FSResize(ADPCM_File, free_space*512U);
		}
#else	
//	free_space = free_space*512U;
	wav_space = BEGIN_RECORD_SPACE;
//	FSResize(ADPCM_File, wav_space);
    SDKLoadCode(KB_RESIZE_CODE);
	fserr = kb_fat_resize(ADPCM_File, wav_space);

	if(fserr != FS_NO_ERROR){
		tishi_mp3(FLAG_DISKFULL);
		return FALSE;
		}
#endif	
	

	
	REC_state = REC;
	flag_auto_poweroff=FALSE;
	BlueLed_OFF();
    RedLed_ON();
	set_ex();
	clkup_time(FALSE);
	OP_CLOSE();
	op_stop_timer();
#if ( OID_VERSION == OID1 )
	first_record_data = TRUE;
#endif
	Rec_process_init();
    recorder_wait_time = Timer_set_timer( TIMER_MS( 500 ));
	return TRUE;

}



Bool WavPlay(void)
{
//	char path[256];
//	char digi[6];
//	int file_count;
	unsigned int temp_data;
	int error;
	unsigned long read_length;
    tIMA_INFO ADPCM_Info;
    unsigned int i;



	wavfile_size=0;
	current_play_size=0; 

#if 1
	(void)find_wav(paster_play_number);
#else
	temp_data = paster_play_number;
	while(paster_play_number<=MAX_WAV_NUM){
		if(find_wav(paster_play_number))goto begin_play;
		paster_play_number++;
		}
		paster_play_number=1;
	while(paster_play_number<=temp_data){
		if(find_wav(paster_play_number))goto begin_play;
		paster_play_number++;
		}
	paster_play_number=temp_data;
	tishi_mp3(FLAG_STOP_MP3);
#ifdef  TX_DEBUG 	
	SendString("\r\nno find wav!");
#endif	
	return FALSE;

begin_play:
#endif	
		//paster_play_number=temp_data;
		//paster_play_number = paster_play_number;
	
    //(!dir_create(FALSE))
	//return FALSE;
#ifdef  TX_DEBUG 	
	SendString("\r\nwav_play=");
	SendString(dir_path);
#endif		
	PCM_Buffer_Size	= PCM_BUF_SIZE;
	ADPCM_Buffer_Size = ADPCM_BUF_SIZE;	
	
	ADPCM_File = FSOpen(dir_path,FS_MODE_READ,&fserr);

    if(fserr){
#ifdef  TX_DEBUG 	
		SendString("\r\nFail to open file from SD Card.\r\n");
#endif
		return FALSE;
    	}
	

 //read the 60-byte file header 
 	read_length = FSRead(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);
 
 // Initial ADPCM  Decoder 
 
	error = IMA_ADPCM_Decoder_Init(&ADPCM_Info);  
	        
	if(error == 0){
		Sample_Rate = ADPCM_Info.IMA_SR;										// Sampling rate for playing the decoded PCM samples 	
		PCM_Buffer_Size = ADPCM_Info.IMA_PCM_BufSize;				// buffer size for output PCM
		ADPCM_Buffer_Size = ADPCM_Info.IMA_ADPCM_BufSize;		// buffer size for input ADPCM
		Frame_Count = ADPCM_Info.IMA_Frame_No;							// Tell the user how many frame shall be decoded.
	}

#ifdef  TX_DEBUG 	
		SendString("\r\nFrame_Count=");
		send_hex_u32(Frame_Count);
#endif

	flag_auto_poweroff=FALSE;
	
	wav_play_process_init();
    REC_state = PLAY;  
	return TRUE;
	
}


///-----------------------------------
void led_control( void )
{
//if((REC_state == REC))
/*
if((WAV_RECORD_CONTIN== State_RP_process))
		{
			BlueLed_ON();
			RedLed_ON();
		}
	else
*/		
		{
		if(rec_led_is_timer_timeout())
			{
#if(QUICKLY_FLASH == ENABLE)
				if(b_flash_led){
					BlueLed_ON();
					rec_led_start_timer(REC_LED_ON_QUICK_TIME);
	
					}
				else{
					BlueLed_OFF();
					rec_led_start_timer(REC_LED_OFF_NORMAL_TIME);

					}
				//BlueLed_FLASH();
				b_flash_led = ~b_flash_led;
				RedLed_OFF();
		
#endif
			
#if(QUICKLY_FLASH == DISABLE)				
			 BlueLed_FLASH();
			 RedLed_OFF();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
#endif			 
			}
		}
}


//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Key_Process(U16 Key_Code)
{
	U16 temp_data;
    switch(Key_Code)
    {  
        case (STATE_KEY_LONGPRESSED|KEY_VOL_INC): // ap4 Volume change
			if(vol_idx < (VOL_NUM-1)){
				vol_idx++;
				}
			if(REC_state == STOP){
				if(vol_idx == (VOL_NUM-1)){
					tishi_mp3(FLAG_MAXVOL);
					}
				else{
					tishi_mp3(FLAG_VOLUME);
					}
				}
			break;
		case (STATE_KEY_LONGPRESSED|KEY_VOL_DEC): // ap4 Volume change
			if(vol_idx > 0){
					vol_idx--;
				}
			if(REC_state == STOP){
				if(vol_idx == 0){
					tishi_mp3(FLAG_MINVOL);
					}
				else{
					tishi_mp3(FLAG_VOLUME);
					}
				}
			break;
		case (KBD_KEY_RELEASED<<8|KEY_MEN):
        	if(REC_state == REC){
        		RecStop();
				tishi_mp3(FLAG_STOP_MP3);
        		}
        	else if(REC_state == PLAY){
        		WavStop();
        		}
			State_RP_process = IDLE;
			// tishi_mp3(7);
			 AUADC_Power_Down();
			b_back_from_mp3 = TRUE;
			b_back_have_key = TRUE;
			bak_current_index = STATE_KEY_RELEASED|KEY_MEN;
			CommonLoadCode(C_OID);
			 break;	
			 /*
		case (KBD_KEY_PRESSED<<8|KEY_MODE):
        	if(REC_state == REC){
        		RecStop();
				tishi_mp3(FLAG_STOP_MP3);
        		}
        	else if(REC_state == PLAY){
        		WavStop();
        		}
			State_RP_process = IDLE;
			// tishi_mp3(7);
			 AUADC_Power_Down();
			CommonLoadCode(C_MP3);
			 break;		 */
		case (STATE_KEY_PRESSED|KEY_MODE): 
			
			if(REC_state == REC){
        		RecStop();
        		}
        	else if(REC_state == PLAY){
        		WavStop();
        		}
			State_RP_process = IDLE;
			// tishi_mp3(7);
			 AUADC_Power_Down();
			b_back_from_mp3 = TRUE;
			CommonLoadCode(C_OID);
			break;
			 ///*
    	case (KBD_KEY_RELEASED<<8|KEY_POWER): // rec pause
           if((WAV_PLAY_PAUSE==State_RP_process)){
				State_RP_process = WAV_PLAY_CONTIN;
				Wav_Continue();
				return;
				}
		   if((WAV_PLAY_CONTIN==State_RP_process)){
				State_RP_process = WAV_PLAY_PAUSE;
				Wav_Pause();
				}
            break;
			//*/
		//case (KBD_KEY_RELEASED<<8|KEY_REC): // rec pause
		case (STATE_KEY_LONGPRESSED|KEY_REC): // rec pause
	         if(REC_state == REC){
	        	RecStop();
				tishi_mp3(FLAG_STOP_MP3);
				//State_RP_process = IDLE;
				paster_play_number++;
				//return;
	        	}
			 
			 if(REC_state == PLAY){
        		WavStop();
        		} 
			//paster_play_number = current_wav_num;
			State_RP_process = WAV_RECORD_BEGIN;
            break;

		//case (KBD_KEY_LONGPRESSED<<8|KEY_REC):
			case (STATE_KEY_DOUBLE|KEY_REC):
				if(REC_state == REC){
					RecStop();
					tishi_mp3(FLAG_STOP_MP3);
					//State_RP_process = WAV_PLAY_BEGIN;
					//return;
					}
				else if(REC_state == PLAY){
					WavStop();
					}
				 State_RP_process = WAV_PLAY_BEGIN;
				 //State_RP_process = WAV_READY_PLAY;
				 break;
				 
			case (STATE_KEY_RELEASED|KEY_REC):
				if(REC_state == REC){
					RecStop();
					tishi_mp3(FLAG_STOP_MP3);
					State_RP_process = IDLE;
					return;
					}
				/*
		     if((WAV_PLAY_PAUSE==State_RP_process)){
				State_RP_process = WAV_PLAY_CONTIN;
				Wav_Continue();
				return;
				}
		   if((WAV_PLAY_CONTIN==State_RP_process)){
				State_RP_process = WAV_PLAY_PAUSE;
				Wav_Pause();
				}*/
				break;

		case (STATE_KEY_LONGPRESSED|KEY_POWER):
        	if(REC_state == REC){
        		RecStop();
				State_RP_process = IDLE;
				tishi_mp3(FLAG_STOP_MP3);
        		}
        	else if(REC_state == PLAY){
        		WavStop();
				State_RP_process = IDLE;
        		}
	
			 tishi_mp3(FLAG_POWER_OFF_FILE);
			 AUADC_Power_Down();
			 CommonLoadCode(C_POWEROFF);
			 break;
			 
			 
			 
		case (STATE_KEY_RELEASED|KEY_VOL_DEC):
			if(REC_state == REC){
        		RecStop();
				//State_RP_process = IDLE;
				tishi_mp3(FLAG_STOP_MP3);
        		}
        	else if(REC_state == PLAY){
        		WavStop();
				//State_RP_process = IDLE;
        		}
			State_RP_process = IDLE;
			temp_data = paster_play_number;
			while(paster_play_number>0){
				paster_play_number--;
				if(find_wav(paster_play_number)){
					State_RP_process = WAV_PLAY_BEGIN;
					return;
					}
				}
				paster_play_number=temp_data;
				tishi_mp3(FLAG_STOP_MP3);
#ifdef  TX_DEBUG 	
				SendString("\r\nno find wav!paster_play_number=");
				send_hex_u16(paster_play_number);
#endif
			 break;
		case (STATE_KEY_RELEASED|KEY_VOL_INC):
						if(REC_state == REC){
							RecStop();
							//State_RP_process = IDLE;
							tishi_mp3(FLAG_STOP_MP3);
							}
						else if(REC_state == PLAY){
							WavStop();
							//State_RP_process = IDLE;
							}
						State_RP_process = IDLE;
						temp_data = paster_play_number;
						while(paster_play_number < MAX_WAV_NUM){
							paster_play_number++;
							if(find_wav(paster_play_number)){
								State_RP_process = WAV_PLAY_BEGIN;
								return;
								}
							}
							paster_play_number=temp_data;
							tishi_mp3(FLAG_STOP_MP3);
#ifdef  TX_DEBUG 	
							SendString("\r\nno find wav!paster_play_number=");
							send_hex_u16(paster_play_number);
#endif

			break;
			
			
        default:
            break;
    }	
}

//===========================================================================
///================================================

Bool fix_index( void )
{
/*
	if(Current_Index==INDEX_GO_AP4){
    	if(REC_state == REC){
    		RecStop();
    		}
    	if(REC_state == PLAY){
    		WavStop();
    		}
		State_RP_process = IDLE;
//		OP_CLOSE();
//		ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_OID);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_MP3){
    	if(REC_state == REC){
    		RecStop();
//			tishi_mp3(3);
    		}
    	if(REC_state == PLAY){
    		WavStop();
    		}
		State_RP_process = IDLE;
//		OP_CLOSE();
//		ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_MP3);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_DIC){
    	if(REC_state == REC){
    		RecStop();
//			tishi_mp3(3);
    		}
    	if(REC_state == PLAY){
    		WavStop();
    		}
		State_RP_process = IDLE;
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_DIC);
		return TRUE;
		}	
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_FIRST_VOL+6)))
		{
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 //DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
//		 DAC_Enable_HeadPhone();
			if(REC_state==STOP){
				tishi_mp3(14);
				}
		 return TRUE;
		}

	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
//					 DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
//		 			 DAC_Enable_HeadPhone();

				if(REC_state==STOP){
					tishi_mp3(14);
					}
				}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;
//				 DAC_HeadPhone_Volume(VolTbl[vol_idx]); 
//		 		 DAC_Enable_HeadPhone();
				if(REC_state==STOP){
					tishi_mp3(14);
					}
			}
         return TRUE;
		}

	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX))
		{
    	if(REC_state == REC){
    		RecStop();
//			tishi_mp3(3);
    		}
    	if(REC_state == PLAY){
    		WavStop();
    		}
		
		paster_index = Current_Index;
		if(paster_index%2==0){
			State_RP_process = WAV_PLAY_BEGIN;
			}
		else{
			State_RP_process = WAV_RECORD_BEGIN;
			}		
		return TRUE;
		}
	
	if(Current_Index==INDEX_START_RECORD){
		  if(REC_state == REC){
    		RecStop();
//			tishi_mp3(3);
    		}
    	if(REC_state == PLAY){
    		WavStop();
    		}
		State_RP_process = WAV_RECORD_BEGIN;
		return TRUE;
		}
	
	if(Current_Index==INDEX_STOP_RECORD){
		  if(REC_state == REC){
    		RecStop();
			tishi_mp3(3);
			State_RP_process = IDLE;
			return TRUE;
    		}
    	if(REC_state == PLAY){
    		WavStop();
			}
			tishi_mp3(11);
			State_RP_process = IDLE;
			return TRUE;
		}	
	
	if(Current_Index==INDEX_PLAY_RECORD)
		{
		  if(REC_state == REC){
        		RecStop();
//				tishi_mp3(3);
        		}
        	if(REC_state == PLAY){
        		WavStop();
        		}
		State_RP_process = WAV_PLAY_BEGIN;
		paster_index = 0;
		return TRUE;
		}
	
	if(Current_Index==INDEX_DEL_RECORD)
		{
		if(REC_state == REC){
			return TRUE;
			}
        if(REC_state == PLAY){
        		WavStop();
        		}
		(void)WavDel();
		paster_play_number = current_wav_num;
		return TRUE;
		}
	*/
	return FALSE;
}

//-------------------------------------------------------


//==========================================================================
/*
Bool find_wav(Bool flag,U16 temp_num)
{
	char digi[4];
	unsigned long temp_leng=0;
#ifdef TX_DEBUG
	SendString("\r\ncurrent_wav_num=");
	send_hex_u16(temp_num);
#endif
	strcpy(dir_path,PASTER_DIR);
	digi[0] = '0'+temp_num/100;
	digi[1] = '0'+((temp_num -(digi[0]-'0')*100)/10);
	digi[2] = '0'+temp_num%10;
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
		 	 free_space += temp_leng;
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
*/
//////////////////////////////////////////////////////////////////////////
//				rec process
/////////////////////////////////////////
const unsigned int SR2PCMSR_TBL[] ={(unsigned int)96000,
	                                  (unsigned int)48000, 
                                    (unsigned int)44100, 
                                    (unsigned int)32000, 
                                    (unsigned int)24000, 
                                    (unsigned int)22050, 
                                    (unsigned int)16000, 
                                    (unsigned int)12000, 
                                    (unsigned int)11025, 
                                    (unsigned int)9600,
                                    (unsigned int)8000  };
                                    
static unsigned int SmpRate2PCMSR(unsigned int smp_rate)
{
    int i;
    for (i=0; i<sizeof(SR2PCMSR_TBL)/sizeof(unsigned int); i++)
    {
        if (SR2PCMSR_TBL[i] == smp_rate)
            return i;
    }
    return DAC_SAMPLE_RATE_44P1K;
}

void Rec_process_init(void)
{

	idx_L = 0;
    idx_R = 0;
	// Initial ADPCM encoder
//	Sample_Rate = 8000;										// 8KHz	
//	Sample_Rate = 32000;
	WavWriteSize = 0;
			
	IMA_ADPCM_Encoder_Init(Sample_Rate);

	// write 60 bytes file header  
	WavWriteSize += FSWrite(ADPCM_File, (void *)ADPCM_File_Header, 60, &fserr);  

	NotifyError = 0;
	NotifyFlag = 0;   
	Frame_Count=0;

//	// Set Audio ADC parameters      
//	AUADC_Init(MIC_In, DTU1,	AUADC_SAMPLE_RATE_8K, Buffer_Full_Func);

    // Set Audio ADC parameters      
//	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, Buffer_Full_Func);
    
//	AUADC_MicGain(MIC_Gain_20dB);
//    AUADC_InputGain(AUADC_Gain_16P5dB);
  // Set Audio ADC parameters      
	AUADC_Init(MIC_In, DTU1, AUADC_SAMPLE_RATE_16K, Buffer_Full_Func);
    
	AUADC_MicGain(MIC_Gain_0dB);
	//AUADC_MicGain(MIC_Gain_20dB);
    AUADC_InputGain(AUADC_Gain_19P5dB);

	AUADC_Set_Buffer((__X int *)PCM_Buf1, (__X int *)PCM_Buf2, PCM_BUF_SIZE);
 
//	AUADC_MicGain(MIC_Gain_20dB);  
//	AUADC_InputGain(AUADC_Gain_22P5dB);
/*
	AGC_Init(PCM_BUF_SIZE);
    AGC_Sensitive(Sample_Rate/1000);
*/
	Error = AUADC_Start();
}

void Record_Continue(void)
{

	NotifyError = 0;
	NotifyFlag = 0;
#if ( OID_VERSION == OID1 )
	first_record_data = TRUE;
#endif
//	BlueLed_ON();
//    RedLed_ON();
		// Set Audio ADC parameters      

//	AUADC_Set_Buffer((__X int *)ADC_Buf1, (__X int *)ADC_Buf2, PCM_BUF_SIZE);
 
//	AUADC_MicGain(MIC_Gain_20dB);  
//	AUADC_InputGain(AUADC_Gain_22P5dB);
	AUADC_Start();
}



//////wav play process


//void Wav_Continue(void)
//{
//	wav_play_process_init();
//}
void wav_play_process_init(void)
{
	// Initial DAC 

  	DAC_Init(DTU3, DTU2, FillBuffer, 0);
  
//	DAC_LChannelBuffer1 = PCM_Buf1;
//	DAC_RChannelBuffer1 = PCM_Buf1;
	DAC_LChannelBuffer1 = PCM_Buf1;
	DAC_RChannelBuffer1 = PCM_Buf1;
	DAC_LChannelBuffer2 = PCM_Buf2;
	DAC_RChannelBuffer2 = PCM_Buf2;    
  	DAC_FrameSize = PCM_Buffer_Size;     
	op_stop_timer();
	clkup_time(FALSE);
	set_ex();
	idx_L = 0;
    idx_R = 0;													
  	FirstDecode();
    flag_wav_play = TRUE;
	vol_idx_bak = VOL_NUM;
	SpeakDetect(TRUE);


//	DAC_FadeIn();
	
    DAC_Go();
}


static void FirstDecode(void)
{
    unsigned long len;
    int i;
    DAC_SampleRate=SmpRate2PCMSR(Sample_Rate);
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


void FillBuffer(void)
{
    unsigned long len;
    __X int *BufPtr;
    void (*Fill_Buf)(void);
    
    if(Frame_Count == 0)
    {
       // WavRecState = WAV_STOP;
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
/*
	unsigned long len;
	__X int *BufPtr;
	void (*Fill_Buf)(void);

  if( !Frame_Count )
      return;
	if(DAC_Buffer1Empty()){
		BufPtr = DAC_LChannelBuffer1 ;
	  Fill_Buf = DAC_FillBuffer1;	    
	}else if(DAC_Buffer2Empty()) {
		BufPtr = DAC_LChannelBuffer2 ;
		Fill_Buf = DAC_FillBuffer2;
	}
	*/
#if 0
    IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[idx_R++], BufPtr);
    if(idx_R > ADPCM_BUF_CNT-1)
        idx_R = 0;

	 (*Fill_Buf)();
#else
/*
	len = FSRead(ADPCM_File, (void *)ADPCM_Buf, ADPCM_Buffer_Size*2, &fserr);

	if(( len == 0 )||( fserr != FS_NO_ERROR ))
	   {
	      Frame_Count = 0;
	      return;
	   }
	   
	
	IMA_ADPCM_Decoder(ADPCM_Buf, BufPtr);

	IMA_ADPCM_Decoder(ADVACE_ADPCM_Buf[idx_R++], BufPtr);
    if(idx_R > ADPCM_BUF_CNT-1){
     idx_R = 0;
    }
       
        
	(*Fill_Buf)();
	
	Frame_Count-- ;	*/
#endif	
}			

void WavPlayProcess(void)
{
    int len;
#if 0
	if(Frame_Count<=20){
		Frame_Count = 0;
		return;
		}
#else
	if(Frame_Count<=1){
		if(idx_L == idx_R){
			Frame_Count=0;
			return;
			}
		}
	else
		{
		    while(idx_L != idx_R)
		    {
		        len = FSRead(ADPCM_File, ADVACE_ADPCM_Buf[idx_L], ADPCM_Buffer_Size*2, &fserr);
		        if(len == 0)
		        	{
		        	    Frame_Count=0;
		            	break;
		        	}
		        idx_L++;
		        if(idx_L > ADPCM_BUF_CNT-1)
		            idx_L = 0;
		    }
		}
#endif	
}

void WavStop(void)
{
	//;	vol_dec();

//	DAC_FadeOut();
	DAC_Pause();
	DAC_Mute();
	clr_ex();
//	vol_dec(1);
//	DAC_Mute();
	
	FSClose(ADPCM_File);
	REC_state = STOP;
	flag_auto_poweroff=TRUE;
	Delay_ms(10);
	op_start_timer();
	clkup_time(TRUE);
//	save_wav_num();
//	LED_param = 5000;
	
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
	while(*local_str&&buffer_size>0)
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
   //FSGetEntry(( char *)short_path, 0, &info,&entry );
   {
   unsigned int i;
   FS_ERR fserr;
   for( i = 0; ; ++i )
   {
      fserr = FSGetEntry(( char *)short_path, i, &info,&entry );
      if( fserr == FS_NO_ERROR )
         break;
      if( fserr == FS_FILE_NOT_FOUND )
         continue;
      return FALSE;
   }
   }
   
   if( path_str[ left ] == '\0' )
      return TRUE;
   while( path_str[ left ] != '\0' )
   {
      while( path_str[ right ] != '\\'
         && path_str[ right ] != '/'
            && path_str[ right ] != '\0' )
         ++right;
      if( path_str[ right ] == '\\'
         || path_str[ right ] == '/' )
      {
         old_char = path_str[ right ];
         path_str[ right ] = 0;
         if( find_entry( short_path, ( U16 *)&path_str[ left ], TRUE, &info, &entry ))
         {
            //if( nav_filelist_findname(( FS_STRING )( &path_str[ left ]), FALSE ) && nav_file_isdir())
            strcat(( char *)short_path, ( char *)"\\" );
            strcat(( char *)short_path, ( char *)info.name );
            if( FSGetEntry(( char *)short_path, 0, &info, &entry )!= FS_NO_ERROR )
            {
               path_str[ right ] = old_char;
               goto __bad_end;
            }
         }
         else
         {
            path_str[ right ] = old_char;
            goto __bad_end;
         }
         path_str[ right ] = old_char;
         ++right;
         left = right;
      }
      else
      {
         if( find_entry( short_path, ( U16 *)&path_str[ left ], FALSE, &info, &entry ))
         {
            strcat(( char *)short_path, "\\" );
            strcat(( char *)short_path, ( char *)info.name );
            goto __end;
         }
         //else
            goto __bad_end;
      }
   }
__end:
   return TRUE;
__bad_end:
   return FALSE;
}

//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void tishi_keyborad(U16 Key_Code)
{ 
    switch(Key_Code)
    {
			
        case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // ap4 Volume change    
			if(1== volume_inc_flag)	
				{
					vol_idx++;
					if(vol_idx > 4)
						{
							vol_idx-=2;
							volume_inc_flag = 0;
						}
				}
			else
				{
					
					if(0 == vol_idx)
						{
							vol_idx = 1;
							volume_inc_flag = 1;
						}
					else
						{
							vol_idx--;
						}
				}
            break;
        default:
            break;
    }
}

//==============================================
void AudioMP3FileOpenFirst(char *file)
{
	FS_ERR fs_err;
	if(OID_PLAY){
			if (g_b_play_afx){
				AudioMP3Handle.FSHandle = ADPCM_File;
			}
		    else{
				AudioMP3Handle.FSHandle = fat_g_cur_file;
			}		
			AudioMP3Offset = mp3_begin_add;
			AudioMP3FileLen = play_file_size;
		
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
void tishi_mp3(U8 flag)
{
	    FS_ERR fs_err;
	    tAudioErr   Audio_err;
		unsigned long len=0;
		char path[64];
		U16 temp_key;
        U16	m[] = {'\\','\0'};
		memset(tisiFileName,0,128);
#ifdef UNCOIDE_FORMAT
		strcpy_u((U16 *)tisiFileName,(U16 *)&root);
		strcat_u((U16 *)tisiFileName,(U16 *)&subdir);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		switch (flag)
			{
				case 0:
					strcat_u((U16 *)tisiFileName,(U16 *)&SYS_OPEN_FILE);
					break;
				case 1:
					strcat_u((U16 *)tisiFileName,(U16 *)&PASTER_WAV_FILE);
					break;
				case 2:
					strcat_u((U16 *)tisiFileName,(U16 *)&RECORD_BEGIN_WAV_FILE);
					break;
				case 3:
					strcat_u((U16 *)tisiFileName,(U16 *)&RECORD_OVER_WAV_FILE);
					break;
				case 4:
					strcat_u((U16 *)tisiFileName,(U16 *)&PLAY_BEGIN_WAV_FILE);
					break;
				case 5:
					strcat_u((U16 *)tisiFileName,(U16 *)&PLAY_OVER_WAV_FILE);
					break;
				case 6:
					strcat_u((U16 *)tisiFileName,(U16 *)&NOAPF_WAV_FILE);
					break;
				case 7:
					strcat_u((U16 *)tisiFileName,(U16 *)&POWEROFF_FILE);
					break;
				case 8:
					strcat_u((U16 *)tisiFileName,(U16 *)&BATTERY_LOW_FILE);
					break;		
				case 9:
					strcat_u((U16 *)tisiFileName,(U16 *)&VOLUME_FILE);
					break;
				case 10:
					strcat_u((U16 *)tisiFileName,(U16 *)&DISKFULL);
					break;
				case 11:
					strcat_u((U16 *)tisiFileName,(U16 *)&ERROR);	
					break;	
				case 12:
					strcat_u((U16 *)tisiFileName,(U16 *)&FILEFULL);	
					break;	
				case 13:
					strcat_u((U16 *)tisiFileName,(U16 *)&DELFULL);	
					break;	
				case 14:
					strcat_u((U16 *)tisiFileName,(U16 *)&VOL);	
					break;		
				default:
					return;
					break;
			}
//		SendString(tisiFileName);
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)path))return;
#endif

#ifdef ASCII_FORMAT
		strcpy(tisiFileName,(char*)&ROOT);
		strcat(tisiFileName,(char *)&SUBDIR);
		strcat(tisiFileName,"\\");
		switch (flag)
			{
				case FLAG_SYS_OPEN_FILE:
					strcat(tisiFileName,(char *)&SYS_OPEN_FILE);
					break;
				//case FLAG_RECORDING:
				//	strcat(tisiFileName,(char *)&PASTER_WAV_FILE);
				//	break;
				case FLAG_RECORDSTART:
					strcat(tisiFileName,(char *)&RECORDSTART);
					break;
				case FLAG_RECORDEND:
					strcat(tisiFileName,(char *)&RECORDEND);
					break;
				case FLAG_PLAYSTART:
					strcat(tisiFileName,(char *)&PLAYSTART);
					break;
				case FLAG_PLAYEND:
					strcat(tisiFileName,(char *)&PLAYEND);
					break;
				//case FLAG_NOAPF:
				//	strcat(tisiFileName,(char *)&);
				//	break;
				case FLAG_POWER_OFF_FILE:
					strcat(tisiFileName,(char *)&POWER_OFF_FILE);
					break;
				case FLAG_BATTERY_LOW_FILE:
					strcat(tisiFileName,(char *)&BATTERY_LOW_FILE);
					break;				
				case FLAG_VOLUME:
					strcat(tisiFileName,(char *)&VOLUME_MP3);
					break;
				case FLAG_MIN_TISHI:
					strcat(tisiFileName,(char *)&MIN_TISHI);
					break;	
				case FLAG_STOP_MP3:
					strcat(tisiFileName,(char *)&STOP_MP3);
					break;	
				case FLAG_MINVOL:
					strcat(tisiFileName,(char *)MINVOL_MP3);
					break;
				case FLAG_MAXVOL:
					strcat(tisiFileName,(char *)MAXVOL_MP3);
					break;	
				default:
					return;
					break;
			}
		if(!find_path_a((U8 *)tisiFileName,(U8 *)path))return;
#endif		
#ifdef TX_DEBUG
		SendString("\r\ntishimp3=");
		SendString(tisiFileName);
#endif
		len = FSGetFileSize((char *)path, &fs_err);
		if(0==len)return;

		//SysClkUpgrade();
		clkup_time(TRUE);
//		judge_op();
//		char path[32];
//		len = FSGetFileSize((char *)tisiFileName, &fs_err);
//		if(0==len)return;
		OID_PLAY = FALSE;
		g_b_play_afx = FALSE;
		AudioMP3FileOpenFirst((char *)path);
        Audio_err = AudioMP3Init1k0(0);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 return;
        	}
//		DAC_HeadPhone_Volume(0);
//		judge_op();
//		vol_inc();
        Audio_err = AudioMP3Open((char *)path);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
				return;
        	}
		
        //AudioMP3SetEQ(EQ_NOMAL);
//		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
//		DAC_HeadPhone_Volume(0);
//		judge_op();
//		vol_inc();
/*
//		DAC_Enable_HeadPhone();
//		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
//		OP_CLOSE();
//		SpeakDetect();
		judge_op();
		DAC_HeadPhone_Volume(0);
		judge_op();
		vol_inc();
*/		
//		flag_wav_play = FALSE;
//		inc_volume();
		flag_wav_play = FALSE;
		vol_idx_bak = VOL_NUM;
		SpeakDetect(TRUE);
		//DAC_FadeIn();
		DAC_Stereo_to_Mono(1);
		op_stop_timer();
		clkup_time(FALSE);
        Audio_err = AudioMP3Play();
    	while (CheckPlaying(FALSE))
    	{    	
    	
/*    	
    	   kbd_process();
		   temp_key = kbd_key_read();
		   if(0!=temp_key)
		   	{    
		   	 tishi_keyborad(temp_key);
		   	}
*/ 
			if(rec_led_is_timer_timeout())
			{
			 BlueLed_FLASH();
			 RedLed_OFF();
			 rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
			}
    	}
		//DAC_FadeOut();
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();
		AudioMP3FileClose( );
		AudioMP3FileCloseAfter();
		op_start_timer();
		clkup_time(TRUE);
}



void readyPasterDirName(void)
{
#if 0//def TX_DEBUG
	SendString(( char *)"\r\nfind paster dir");
#endif

#if 1
	//	strcpy(ap4_dirname,(char *)"0:\\AP4~1");
///*
	//char dirpath[32];
	U16	m[] = {'\\','\0'};
	//U16		dictionary[] = {0x8BCD,0x5178, 0x0000};
	
	//U16	story[] = {0x6545,0x4E8B,0x0000};//故事
	//U16	erge[] = {0x513F,0x6B4C,0x0000};//儿歌
	//U16	music[] = {0x97F3,0x4E50,0x0000};//音乐
	//U16	mp3_book[] = {0x6709,0x58F0,0x4E66,0x0000};//有声书

	//U16	music[] = {0x97F3,0x4E50,0x6B23,0x8D4F,0x0000};//音乐欣赏
	//U16	ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,0x0000};//有声课本
	U16	record[] = {0x5F55,0x97F3,0x0000};//录音


	U16   sd1_root[] = {'0',':','\\','\0'};

	  memset(tisiFileName,0,sizeof(tisiFileName));
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
//		judge_op();
// 		swap_code(FILE_EXTERN_CODE_FLAG);
//		SDKLoadCode(FILE_EXTERN_CODE);
		//SDKLoadCode(LINK_FILENAME_CODE);
		
		strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
		strcat_u((U16 *)tisiFileName,(U16 *)&record);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);

#if 0//def TX_DEBUG
				SendString(( char *)"\r\ntisiFileName=");
				SendString(tisiFileName);
#endif	

		//SDKLoadCode(FIND_PATH_CODE);
		(void)find_path_u((U16 *)&tisiFileName,(U8 *)pasterDirName);
		//*/
		//strcpy(pasterDirName,(char *)"0:\\录音");
		//strcpy(pasterDirName,(char *)g_paster_dir);
#else		
		strcpy(ap4_dirname,(char *)SD1_AP4_DIR);
#endif		

#ifdef TX_DEBUG
		SendString(( char *)"\r\npasterDirName=");
		SendString((char *)pasterDirName);
#endif	


}


//==============================================

