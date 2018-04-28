//!
//! @file mmi_ap3.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI ap3
//!
//! @version 1.26 snd3b-dvk-1_0_33 $Id: mmi_ap3.c,v 1.26 2007/3/15 9:20:06 Gary_liu $
//!
//! @todo
//! @bug
//!

#pragma CODE = MMI_AP3_CODE
#pragma DATA = MMI_AP3_DATA


#ifndef _mmi_ap3_c_
#define _mmi_ap3_c_

//_____  I N C L U D E S ___________________________________________________
#include "compiler.h"
#include <stdio.h>
#include <string.h>
#include "fc_internal.h"
//#include "mail_evt.h"
//#include "system.h"
//#include "timer_soft.h"
#include "mmgr_kernel.h"
//#include "com_appli.h"
//#include "com_evt.h"

#include "keypad.h"
#include "keypad_drv.h"
#include "conf_kb.h"
#include "mmi_ap3.h"
#include "file.h"
//#include "navigation.h"
//#include "unicode.h"
#include "search.h"
#include "search_task.h"
//#include "mmi_status.h"
//#include "srv_player.h"
//#include "com_var.h"

#include "com.h"
#include "conf_oid.h"
#include "player_ap3.h"
//#include "mmi_player.h"
//#include "recorder.h"
//#include "srv_recorder.h"
#include "oid.h"
//#include "recorder_task.h"
//#include "search_game.h"
#include "search_remote.h"
//#include "player.h"
//#include "string_1.h"
//#include "mmi_startup.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "player_task.h"

#include "FileSearch.h"
//#include "power.h"             // Power module definition
//#include "BinaryImage.h"


//#include "USB_Detect.h"

#if 1//( AP3 == ENABLE )
//_____ P R I V A T E   D E F I N I T I O N S _____________________________
   _MEM_TYPE_SLOW_   AP4_Mode ap3_mode;
   _MEM_TYPE_SLOW_   U8       Ap3_MenuPath;
//   _MEM_TYPE_SLOW_   U16      ap3_file_number;
//const U8       Ext[] = "ap4";

const U8	 player_dir[]= "Mp3";
//const U8	 rec_dir[]= "REC";

#ifdef UNCOIDE_FORMAT
const  U16      root[] = {'0',':','\\','\0'};
const  U8       AP4[] = {0x70,0xB9,0x8B,0xFB,0x65,0x87,0x4E,0xF6,0x00,0x00};//点读文件
const  U16      MP3_FILE[] = {'m','p','3','\0'};
const  U16      DIC_FILE[] = {'d','i','c','\0'};
const  U8		subdir[] = {0x7C,0xFB,0x7E,0xDF,0x65,0x87,0x4E,0xF6,0x00,0x00};//\\系统文件
//const  U16		AP4_GENDU_FILE[] = {'0',':','\\','a','p','4','\\','r','e','c','o','r','d','.','w','a','v','\0'};
const  U16     GENDU_FILE[] = {'r','e','c','o','r','d','.','w','a','v','\0'};  
#endif 

#ifdef ASCII_FORMAT
//const  U8       root[] = "0:\\";
//const  U8       AP4[] = "ap4";
//const  U8		subdir[] = "subdir";
const  U8       AP4_GENDU_FILE[] = "0:\\ap4\\record.wav";
#endif

//const U8	    REC_FILE[] = "0:\\rec\\record.wav";
char REC_FILE[64];

//#define AP4_GENDU_FILE   "0:\\ap4\\record.wav"

char tisiFileName[256];
//char  _MEM_TYPE_SLOW_   tisiFileShortpath[ 128 ];
 UnicodeBuffer g_string   [G_STRING_SIZE ];
// UnicodeBuffer g_string2  [G_STRING_SIZE2];
   _MEM_TYPE_SLOW_   U8       ap4_play_state;

//   _MEM_TYPE_SLOW_   U16                  RemoteNumber;
//   _MEM_TYPE_SLOW_   MMI_AP3_State  ap3_poweron_state;
                     Bool           g_b_open_file_error;

   _MEM_TYPE_SLOW_   AP3_CHANGE_FILE   ap4_change_file;

//  _MEM_TYPE_SLOW_     Fs_clusterlist_cache fs_g_cache_clusterlist[ FS_NB_CACHE_CLUSLIST ];
//  _MEM_TYPE_SLOW_     Fs_clusterlist_cache fs_g_cache_clusterlist_bak[ FS_NB_CACHE_CLUSLIST ];

//extern   xdata    AL_Variables     	varAl;

   _MEM_TYPE_SLOW_   Bool      poweroff_falg;
   _MEM_TYPE_SLOW_   Bool      flag_load_mp3_dac;

  _MEM_TYPE_SLOW_   Bool      root_dir_falg;


//void Delay_ms(int count);
//extern void SpeakDetect(void);

// Coordinate of the setting window title "My settings"
#define POSX_WND_TITLE                            60
#define POSY_WND_TITLE                            8


//** To manage display ** 

// Position of graphical elements
#define MMIAP3_POSX_TITLE                         40
#define MMIAP3_POSY_TITLE                         4

#define MMIAP3LIST_POSX_NAME_CURRENT_FILE         10
#define MMIAP3LIST_POSX_ICON_CURSOR               2
#define MMIAP3LIST_POSY_NAME_CURRENT_FOLDER       3

//DEFINE  ALL BITS OF AP4_MP4_FLAG 
//#define BANK_FORM_MP3       	0X8000
//#define BANK_FORM_GAME          0X4000
//#define BANK_HAVE_INDEX     	0X2000
//#define CURRENT_CONTROL_GAME 	0X1000


/*
extern unsigned int gflag;
extern unsigned int ap4_mp3_flag;

extern unsigned int bak_ap4_num;
extern unsigned int bak_book_num;
extern unsigned int bak_page_num;
extern unsigned int bak_control_mode_num;

extern unsigned int begin_R;
extern unsigned int over_R;
extern unsigned int begin_P;
extern unsigned int over_P;
extern unsigned int prev_task;
*/
extern Bool OID_PLAY;
//extern FILE_INFO __X	*	fat_g_cur_nav;
extern __X char CurrentShortFilePath[128];
extern		FCFILE __X *	fat_g_cur_file;
//extern void swap_code( U16 flag);
extern void judge_op(void);
//extern Bool flag_wav_play;
//extern void  delay1ms    ( int count );
//***********************************************
// ID of the timer soft used by this MMI appli
_MEM_TYPE_SLOW_ U8  s_ts_ap3record_display_id;

_MEM_TYPE_SLOW_      U8                ap4_short_name[ SHORT_NAME_SIZE + 1 ];
_MEM_TYPE_SLOW_      U8                ap4_long_name[ G_STRING_SIZE ];

#define AP3RECORDER_TIME   TIMER_MS( 500 ) 


#define  G_STRING_SIZE1    (127 *2)
U8 _MEM_TYPE_SLOW_   g_string1   [G_STRING_SIZE1 ];
//extern U16		swap_code_flag;


                  Bool  g_b_search_record;
Bool       b_kbd_process_disable;
   __X FCFILE *Temp_File;
//========================================================================
//char REC_FILENAME[128];
//U16  record_ap4_num[1];
   _MEM_TYPE_SLOW_   U16	record_ap4_num;
   _MEM_TYPE_SLOW_   U16	record_ap4_id;
      _MEM_TYPE_SLOW_   U16	 record_sd_id;
//U16  bak_last_num[5];
U16  cur_ap4_num;
U16  cur_type;
//U16  cur_sd;
//U16  Range_group[10];
//U16  NP_group[10];
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
U16   Get_File_Number            (  U8  *sz_filterext );

//Bool  ap3srvrec_ready                  ( void );
//Bool  ap3srvrec_reread_on              ( void );
//Bool  mmi_ap3_no_ap4_playintro         ( void );
//Bool  mmi_ap3_sd_remove_playintro      ( void );
//Bool  mmi_ap3_only_simple_playintro    ( void );
//Bool  mmi_ap3_only_sequence_playintro  ( void );
//Bool  mmi_ap3_simple_playintro         ( void );
//Bool  mmi_ap3_sequence_playintro       ( void );
//Bool  mmi_ap3_not_find_playintro       ( void );

U16   get_name_lead                    ( void );
void  create_gendu_file				   ( void );

/*********************** Menus **********************/

//_MEM_TYPE_SLOW_ U8  current_api;

//add by xuanyu
extern _MEM_TYPE_SLOW_  U16    Point_Read;
extern _MEM_TYPE_SLOW_  U16    Point_Write;
//extern _MEM_TYPE_SLOW_  U8     flag_gendu;
extern   xdata 		U8 				current_api;
extern Bool g_b_search_on;

//extern unsigned int vol_idx;
//extern const int VolTbl[VOL_NUM];
//extern unsigned int volume_inc_flag;
//extern  void  player_stop_stream    ( void );
//extern  void  player_codec_stop     ( void );
extern FILE_INFO __X	*	fat_g_cur_nav;
//extern __X char CurrentShortFilePath[256];
extern FS_ENTRY_INFO	__X *	fat_g_cur_entry;
extern __X  Bool flag_reread;
extern Bool flag_change_ap4;
extern    _MEM_TYPE_SLOW_  U32			  addr_novoice;
///=========================================================================
//extern void vol_inc(void);
extern void DAC_Stereo_to_Mono(unsigned Option);
extern void read_control_mode_news( U16 num_control );
extern void ledFlash(void);
///=========================================================================

ImageDeclareType(AP3_OPEN_FILE_CODE);
ImageDeclareType(POWER_SYSTEM_OFF_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(AUADC_COMMON_CODE);
//ImageDeclareType(FILE_EXTERN_CODE);
ImageDeclareType(LINK_FILENAME_CODE);
ImageDeclareType(GET_AP4NAME_CODE);
ImageDeclareType(KB_RESIZE_CODE);
ImageDeclareType(MP3_COMMON_CODE);
ImageDeclareType(TISHI_MP3_CODE);
ImageDeclareType(FIND_PATH_CODE);
ImageDeclareType(FILESEARCH_CODE);
///==========================================================================
void	strcpy_u(U16 * dst,U16 * src);
void	strcat_u(U16 * dst,U16 * src);
void    link_filename(U8 link_flag);
Bool Get_File_Name(  void );
FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );
void keylong_vol(U8 key_data);
void AudioMP3FileOpenFirst(char *file);
MP3API_STATUS AudioMP3FileCloseAfter( void );
void change_control_mode(void);
U16 get_book_id( void );
void get_area_num( void );
void display_filenews(void);
void translateFunction(void);
//===========================================================================
/*
void bak_to_news(void)
{
	 bak_ap4_num = Current_Record.Ap3_num;
     bak_book_num = Current_Record.Book_num;
     bak_control_mode_num = Current_Record.Control_num;
     bak_page_num = Current_Record.Page_num;
}

void bak_from_news(void)
{
	Current_Record.Ap3_num = bak_ap4_num;
	Current_Record.Book_num = bak_book_num;
	Current_Record.Control_num = bak_control_mode_num;
	Current_Record.Page_num = bak_page_num;
}

/*
void clear_RP(void)
{
	 begin_R = 0;
	 over_R  = 0;
	 begin_P = 0;
	 over_P  = 0;
}

void set_RP(void)
{
	 begin_R = Ap4_Record_Range.Record_Index_Min;
	 over_R  = Ap4_Record_Range.Record_Index_Max;
	 begin_P = Ap4_Record_Range.Play_Index_Min;
	 over_P  = Ap4_Record_Range.Play_Index_Max;
}
*/





//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Ap4_Key_Process(U16 Key_Code)
{
    
    switch(Key_Code)
    {
#if 1
    	case (STATE_KEY_RELEASED|KEY_POWER): // ap4 pause
    	//case (STATE_KEY_PRESSED|KEY_REC): // ap4 pause
			
			 if((!g_b_player_on)&&(b_play_pause))
        	{
        	    g_b_player_on = TRUE;
				b_play_pause = FALSE;
				SDKLoadCode(DAC_COMMON_CODE);
				SDKLoadCode(MP3_COMMON_CODE);
				vol_idx_bak = VOL_NUM;
				op_stop_timer();
				state_search_to_idle();
        		AudioMP3Continue();
				clkup_time(FALSE);
				return;
			}
			
          if((g_b_player_on)&&(!b_play_pause))
        	{
        		g_b_player_on = FALSE;
				b_play_pause = TRUE;
				//flag_ap4_play_pause = 0x55;
				state_search_to_goonplay();
				AudioMP3Pause();
				//OP_CLOSE();
				return;
			}
		  init_array_v();
		   write_media_addr(lastSpeech.Addr_Speech, lastSpeech.Flag_Speech);
	   	   state_search_to_play();
//#else
			
			//interrupt_play();
			/*
			 if( player_ap3_is_busy())
			 player_ap3_stop();
			 file_close();
			 bak_to_news();
			 prev_task=C_OID;
			 CommonLoadCode(C_MP3);
			 */
	 
            break;	
#endif	

		case (STATE_KEY_RELEASED|KEY_REC): //
			 interrupt_play();
			 clear_bigflag();
			 clear_smallflag();
			 //SDKLoadCode(CHANGE_CON_MODE_CODE);
			 //change_control_mode();
			 SDKLoadCode(TRANSLATE_CODE);
			 translateFunction();
			 break;
/*		 
		//case (STATE_KEY_PRESSED|KEY_MODE): // 
		case (STATE_KEY_RELEASED|KEY_POWER): // 
			 interrupt_play();
			 //file_close();
			// bak_to_news();
			 //CommonLoadCode(C_MP3);
			  //Point_Read = 0;
			  init_array_v();
			 write_media_addr(lastSpeech.Addr_Speech, lastSpeech.Flag_Speech);
	   		state_search_to_play();
            break;	
			
		case (STATE_KEY_RELEASED|KEY_REC): // 
			 interrupt_play();
			 file_close();
			// bak_to_news();
			 wav_action = WAV_RECORD_FLAG;
			 CommonLoadCode(C_REC_PASTER);
            break;	
//*/            
		case (STATE_KEY_RELEASED|KEY_VOL_INC):
			//if(g_b_player_on&&b_vol_play){
				//interrupt_play();
			//	}
			if(vol_idx < (VOL_NUM-1)){
				vol_idx++;
				}
			if(!g_b_player_on)
				{
					(void)act_do(All_Control.Addr_Antion_key, 0);
					state_search_to_play();
				}
			/*
			if(!g_b_player_on){
				if(vol_idx == (VOL_NUM-1)){
					//interrupt_play();
					tishi_mp3(FLAG_MAXVOL);
					}
				else{
					//(void)act_do(All_Control.Addr_Antion_key, 0);
					//state_search_to_play();
					tishi_mp3(FLAG_VOLUME);
					}
				}
			//*/
			break;
		case (STATE_KEY_RELEASED|KEY_VOL_DEC):
			//if(g_b_player_on&&b_vol_play){
				//interrupt_play();
			//	}
				if(vol_idx > 0){
					vol_idx--;
				}
				if(!g_b_player_on)
				{
					(void)act_do(All_Control.Addr_Antion_key, 0);
					state_search_to_play();
				}
			/*	
			if(!g_b_player_on){
				if(vol_idx == 0){
					//interrupt_play();
					tishi_mp3(FLAG_MINVOL);
					}
				else{
					//(void)act_do(All_Control.Addr_Antion_key, 0);
					//state_search_to_play();
					tishi_mp3(FLAG_VOLUME);
					}
				}
			//*/
		break;

//		 case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC): // ap4 Volume change
/*
        case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // ap4 Volume change
				if( vol_idx > 0 )
					{
						vol_idx--;
					}
				if(!g_b_player_on)
				{
					(void)act_do(All_Control.Addr_Antion_key, 0);
					state_search_to_play();
				}
            break;
			*/

		case (STATE_KEY_LONGPRESSED|KEY_POWER):
			 interrupt_play();
			 scan_oid_disable();
			 Kbd_set_evt_disabled();
///*			 
			 if(All_Control.Addr_Action_close)
			 	{
			 	    init_array_v();
			 	    poweroff_falg = TRUE;
			 		(void)act_do(All_Control.Addr_Action_close,0);
					if(Point_Read!=Point_Write){
					state_search_to_play();
					return;
					}
			 	}
//*/
			 tishi_mp3(FLAG_POWER_OFF_FILE,false);
		 	 CommonLoadCode(C_POWEROFF);
			 break;
		 
			 
		case (STATE_KEY_LONGPRESSED|KEY_VOL_DEC):
			interrupt_play();

			// file_close();
			// bak_to_news();
			// CommonLoadCode(C_MP3);
#if 0//def TX_DEBUG
	 		SendString("\r\nreread=");   
			send_hex_u16(sys_flag.value);
#endif				
			SDKLoadCode(KEYLONG_VOL_CODE);
			keylong_vol(KEY_VOL_DEC);
			break;
			
		case (STATE_KEY_LONGPRESSED|KEY_VOL_INC):
			//interrupt_play();
			//file_close();
			// bak_to_news();
			 //CommonLoadCode(C_MP3);
#if 0//def TX_DEBUG
	 		SendString("\r\nsleep_mode=");   
			send_hex_u16(sys_flag.value);
#endif			
			 interrupt_play();
			 SDKLoadCode(KEYLONG_VOL_CODE);
			 keylong_vol(KEY_VOL_INC);
			 break;	

		case (STATE_KEY_LONGPRESSED|KEY_REC): // 
			 interrupt_play();
			 file_close();
			 //bak_to_news();
			 wav_action = WAV_RECORD_FLAG;
			 readyGoNewPorject(C_REC_PASTER);
            break;	

		case (STATE_KEY_DOUBLE|KEY_REC): // 
			 interrupt_play();
			 file_close();
			 //bak_to_news();
			 wav_action = WAV_PLAY_FLAG;
			 CommonLoadCode(C_REC_PASTER);
            break;	
		case (STATE_KEY_PRESSED|KEY_MODE):	
			interrupt_play();
			file_close();
			readyGoNewPorject(C_MP3);
			break;
			
	
/*
			 case (KBD_KEY_LONGPRESSED<<8|KEY_MODE):
				 interrupt_play();;
				 file_close();
				 bak_to_news();
				 //prev_task=C_OID;
				 //wav_action = WAV_RECORD_FLAG;
				// CommonLoadCode(C_MP3);	
				CommonLoadCode(C_REC_PASTER);	
			 	break;
				*/
        default:
            break;
    }
//*/	
}

U16 *	atou(U8 * astr)
{
	U16 * ustr;
	ustr = (U16 *)g_string2;
	while(*astr)
	{
		*ustr++ = *astr++;
	}
	*ustr = 0;
	return	(U16 *)g_string2;
}


U16 read_bnl_file(U16 off_t)
{
/*
	U16 offset,bid = 0;
	U32 temp_id,temp_off;
	//char tmpbuf[32];
	char rbuf[64];
	
	__X FS_ERR err;
 

	if(b_sd1_ok){
   		strcpy(tmpbuf,(char *)SD1_BOOK_SYS);
		}
	else{
		strcpy(tmpbuf,(char *)SD2_BOOK_SYS);
		}
#if 0//def	TX_DEBUG
   	SendString(( char *)"\n\rtemp_path=");
	SendString(( char *)tmpbuf);
#endif	
   Temp_File = FSOpen((char *)tmpbuf,FS_MODE_READ,&err);   
   if(err != FS_NO_ERROR)return 0xFFFF;
			 
	//打开bnl文件，读取数据到rbuf
	//GetFileData_rBuf(0, rbuf, 0xc0);//192bytes
	//read_data_from_nf(0, (U8 _MEM_TYPE_SLOW_ *)rbuf, 0xc0);

	temp_off = (U32)off_t;
	FSSeek(Temp_File, temp_off);
	FSRead(Temp_File, rbuf, 0x08, &err);
	//FSClose(Temp_File);
	MSB(record_ap4_num) = rbuf[0x00];
	LSB(record_ap4_num) = rbuf[0x01];
#if 0//def TX_DEBUG
	SendString(( char *)"\n\rbid=");
	send_hex_u16(record_ap4_num);
#endif	

	return record_ap4_num;
	*/
	return 0;
}

#pragma CODE = AP3_OPEN_FILE_CODE
//_____ D E F I N I T I O N S ______________________________________________
//!
//! ap3_mmi_appli
//!
//! Manage the application template 
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void ap3_mmi_appli( void )
{
	U8 i;
	U32 temp_id,temp_off;
	char tmpbuf[32];

	
	__X FS_ERR err;
	
   first_go_ap4 = 0;
    g_b_noap4 = FALSE;
   g_b_autopoweroff_forbidden = FALSE;
  // Search_Record.news_sys.flag_index = FALSE;
  	index_flag = FALSE;
	flag_ap4_play_pause = 0;
	b_play_pause = FALSE;
	flag_wav_play = FALSE;
	bRemoteChangeAp3 = FALSE;
	OID_PLAY = TRUE;
	Addr_Fanyi = 0;
	flag_fanyi=TRUE;
	addr_novoice=0;
     poweroff_falg = FALSE;
     root_dir_falg = FALSE;
//	 flag_gendu = 0;
	 flag_reread = FALSE;

	 g_b_play_afx = FALSE;
	 g_b_record = FALSE;
	 clear_bigflag();
	 clear_smallflag();
//	 addr_num = 0;
#ifdef USE_AUADO	 
     create_gendu_file();
#endif

#if 0//def TX_DEBUG
	 SendString("\r\narea_range=");
	for(i=0;i<NUM_AREA;i++)
		{
		send_hex_u16(CUR_RANGE_AREA[i].min);
		SendString(":");
		send_hex_u16(CUR_RANGE_AREA[i].max);
		SendString(":");
		}
#endif
    	
      (void)scank_disk(root_dir_falg);
      //{
	  	/*
      	root_dir_falg = TRUE;
		if( scank_disk(root_dir_falg))goto find_lce;
		*/
         //SendString("error\r\n");
//         mmi_ap3_no_ap4_exit();
//         RESET_SYS();
//	   SendString("\r\n202\r\n");
//		strcpy(tisiFileName,NO_AP4_FILE);
		//tishi_mp3(0);
       //ap3_file_number = 0;
	  // CommonLoadCode(C_REC_PASTER);
 //		CommonLoadCode(C_MP3);
     // }
//find_lce:	  
//	SendString("\r\n206\r\n");
      //SendString("disk ok\r\n");
     // search_task_start();
     g_b_search_on = TRUE;
   wav_action=0;
	flag_record_ap4_num = FALSE;
		
/*
	 Ap4_Record_Range.Play_Index_Max=0;
	 Ap4_Record_Range.Play_Index_Min=0;
	 Ap4_Record_Range.Record_Index_Max=0;
	 Ap4_Record_Range.Record_Index_Min=0;
*/  
   
//      ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
      State_Search = Open_File_Search;
      g_b_open_file_error = FALSE;
      flag_change_ap4 = TRUE;
//      Ap3_MenuPath = AP3_MENU_PATH;
      ap3_mode = SINGLE_PLAY_MODE;
///*	  
	  if(b_back_from_mp3)
	  	{
	  	   //flag_load_mp3_dac = TRUE;
	  	   //tishi_mp3(9);
	  	   scan_oid_disable();
		   Kbd_set_evt_disabled();
	  	   //bak_from_news();
		  
	  	}
	  else //*/
	  	{
	      
		  Current_Record.Book_num = 0;
		  Current_Record.Control_num = 0;
		  Current_Record.Page_num = 0;
		  /*
		  Current_Record.Ap3_num = 0;
		  if(ap3_file_number != 0){
		  	//current_sd = FLAG_SD1_CON;
		  	}
		  else if(sd2_ap3_number != 0){
		  	current_sd = FLAG_SD2_CON;
		  	}
		  else{
		    g_b_noap4 = TRUE;
		  	}*/
	  	}
//      Search_Record.value = 0;
      g_b_search_record = FALSE;



/*
 if(b_sd1_ok){
   		strcpy(tmpbuf,(char *)SD1_BOOK_SYS);
		}
	else{
		strcpy(tmpbuf,(char *)SD2_BOOK_SYS);
		}
#if 0//def	TX_DEBUG
   	SendString(( char *)"\n\rtemp_path=");
	SendString(( char *)tmpbuf);
#endif	
   Temp_File = FSOpen((char *)tmpbuf,FS_MODE_READ,&err);   
   if(err != FS_NO_ERROR){

#ifdef TX_DEBUG
	 SendString("\r\nopen book.sys fail!");   	
#endif
   	//return 0xFFFF;
   	Exception();
   	}
   */
}



/*
//the function is get the file name;
Bool Get_File_Name(  U8 * sz_filterext,  U16 Number )
{
#if 1
   U16 _MEM_TYPE_SLOW_ Number_Temp = 0;
   nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext )
            && nav_file_name(g_string2,sizeof(g_string2), FS_NAME_GET, FALSE ))
         {
            Number_Temp++;
         }
         if(( Number + 1 )== Number_Temp )
         {
 //        	SendString((char *)g_string2);
            return TRUE;
         }
      }
   }
   return FALSE;
#endif   
}
*/

/*
//the function is find the number of ap3 in the root;
U16 Get_File_Number(  U8  * sz_filterext )
{
   U16 _MEM_TYPE_SLOW_ Number = 0;
   
 
  ( void ) nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( sz_filterext ))
         {
            Number++;
         }
      }
   }
   return Number;
}
*/
#ifdef USE_AUADO
///*
void create_gendu_file(void)
{   

  __X FS_ERR err;
  __X FCFILE *REC_File;
/*  
#ifdef UNCOIDE_FORMAT 
  U16	m[] = {'\\','\0'};
  strcpy_u((U16 *)REC_FILE,(U16 *)&root);
  strcat_u((U16 *)REC_FILE,(U16 *)&subdir);
  strcat_u((U16 *)REC_FILE,(U16 *)&m);
  strcat_u((U16 *)REC_FILE,(U16 *)&GENDU_FILE);
#endif  
#ifdef ASCII_FORMAT
	strcpy(REC_FILE,(char*)"0:\\ap4\\record.wav");  
#endif
*/	
	  if(b_sd1_ok){
	  	strcpy(REC_FILE,(char*)"0:\\record\\record.wav"); 
	  	//strcpy(REC_FILE,(char*)"0:\\ap4\\record.wav");
	  	}
	  else{
	  	strcpy(REC_FILE,(char*)"1:\\record\\record.wav"); 
		//strcpy(REC_FILE,(char*)"1:\\ap4\\record.wav"); 
	  	}
	  
   	   if((FSGetFileSize((char *)REC_FILE, &err))){
		}else{
    		 REC_File = FSOpen((char *)REC_FILE,FS_MODE_CREATE,&err);   
//			 FSResize(REC_File,2UL*1024*1024);
			 SDKLoadCode(KB_RESIZE_CODE);
			 err = kb_fat_resize(REC_File, 2UL*1024*1024);
			 FSClose(REC_File);
			}	

}
//*/
#endif

//=============================================================================


//
Bool scank_disk( Bool root_flag )
{
#ifdef TX_DEBUG
   	SendString("\r\nsd1_number:sd2_num=");
   	send_hex_u16(ap3_file_number);
	SendString(":");
   	send_hex_u16(sd2_ap3_number);
#endif
   return TRUE;
}

#pragma CODE = OPEN_FILE_CODE

//the funcion is open the first file of changed file;
Bool open_file( void )
{
   U16 _MEM_TYPE_SLOW_ Number = 0;
   char temp_buf[32];
   
#if 1
	if(FLAG_SD2_CON == current_sd){
		strcpy(temp_buf,(char *)SD2_AP4_DIR);
		}
	else{
		strcpy(temp_buf,(char *)SD1_AP4_DIR);
		//strcpy(temp_buf,ap4_dirname);
		}
#ifdef TX_DEBUG
 		SendString("\r\nopen:sd:num=");send_hex_u16(current_sd);SendString(":");send_hex_u16(Current_Record.Ap3_num);
#endif
	 SDKLoadCode(FILESEARCH_CODE);
	 if(file_get_path((char *)temp_buf, (char *)AP4_EXT, Current_Record.Ap3_num)){
	 	 strcpy(CurrentShortFilePath,g_full_path);
#ifdef  TX_DEBUG
	SendString(( char *)"\r\ncur_path:size=");
	SendString(CurrentShortFilePath);
#endif			 
	 	 if( file_open( FOPEN_MODE_R ))
               {
               	  size_current_ap4 = file_getsize();
#ifdef  TX_DEBUG
					SendString(( char *)":");
					send_hex_u32(size_current_ap4);
#endif				  
                  if( size_current_ap4 > 512*2 )
                  {
				  SDKLoadCode(GET_AP4NAME_CODE);
				  (void)Get_File_Name();
				  (void)get_book_id();
				  //Current_lastbook[current_area].Book_num = Current_Record.Ap3_num;
				  //Current_lastbook[current_area].Book_id = record_ap4_id;
				  //Current_lastbook[current_area].Sd_num = current_sd;
				  //get_area_num();
				  //display_filenews();
                  return TRUE;
                  }

				  file_close();
                  return FALSE;
               }
	 	}
	 return FALSE;
#else
    SDKLoadCode(FILE_EXTERN_CODE);
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
//   	 SendString((char *)fat_g_cur_nav->name);
//	 SendString("\r\n");
      if( !nav_file_isdir())
      {
         if( nav_file_checkext( (unsigned char *)Ext ))
         {
            if( Number == Current_Record.Ap3_num )
            {
               if( file_open( FOPEN_MODE_R ))
               {
               		size_current_ap4 = file_getsize();
                  if( size_current_ap4 > FS_512B*2 )
                  {
 //				  SendString("\r\n0k!\r\n");
				  SDKLoadCode(GET_AP4NAME_CODE);
				  (void)Get_File_Name();
                  return TRUE;
                  }

				  file_close();
                  return FALSE;
               }
//			   SendString("\r\n2!\r\n");
               return FALSE;
            }
            Number++;
         }
      }
   }
//   SendString("\r\n3!\r\n");
   return FALSE;
#endif
}
//===============================================


#pragma CODE = AP4_CHOOSE_PRE_NEXT

U16 check_no_ff(U16 seek1,U16 seek2)
{
/*
	U16 temp;
	temp = read_bnl_file(seek1);
	if(record_ap4_num != (U16)(0xFFFF))
	{
#ifdef TX_DEBUG
	SendString("\r\n1s_r_t=");
	send_hex_u16(seek1);
	SendString(":");
	send_hex_u16(record_ap4_num);
	SendString(":");
	send_hex_u16(temp);
#endif		
	return FLAG_SD1_CON;
	}
	temp = read_bnl_file(seek2);
	if(record_ap4_num != (U16)(0xFFFF)){
#ifdef TX_DEBUG
	SendString("\r\n2s_r_t=");
	send_hex_u16(seek1);
	SendString(":");
	send_hex_u16(record_ap4_num);
	SendString(":");
	send_hex_u16(temp);
#endif		
	return FLAG_SD2_CON;
	}
*/	
 return 0;
}

Bool check_have_book_next(U16 start_id,U16 end_id)
{
/*
	int i;
	int sdcard=0;
	BlueLed_ON();
#ifdef TX_DEBUG
	SendString("\r\nnext_b_o=");
	send_hex_u16(start_id);
	SendString(":");
	send_hex_u16(end_id);
#endif	
	for(i=start_id;i<=end_id;i++)
		{
		sdcard = check_no_ff((i<<1),((i<<1)+0x2000));
		if(sdcard!=0)
			{
#ifdef TX_DEBUG
		SendString("\r\nnext_id_num_flag=");
		send_hex_u16(i);
		SendString(":");
		send_hex_u16(record_ap4_num);
		SendString(":");
		send_hex_u16(sdcard);
#endif				
			//cur_ap4_num = record_ap4_num[0];
			//Current_Record.Ap3_num = record_ap4_num;
			record_ap4_id = i;
			record_sd_id = sdcard;
			return TRUE;
			}
		}
	*/
	return FALSE;
}
Bool check_have_book_prev(U16 start_id,U16 end_id)
{
/*
	int i;
	int sdcard=0;
	BlueLed_ON();
#ifdef TX_DEBUG
	SendString((char *)"\r\nprev_b_o=");
	send_hex_u16(start_id);
	SendString((char *)":");
	send_hex_u16(end_id);
#endif		
	for(i=end_id;i>=start_id;i--)
		{
		sdcard = check_no_ff((i<<1),((i<<1)+0x2000));
		if(sdcard!=0)
			{
#ifdef TX_DEBUG
		SendString("\r\nprev_id_num_flag=");
		send_hex_u16(i);
		SendString(":");
		send_hex_u16(record_ap4_num);
		SendString(":");
		send_hex_u16(sdcard);
#endif				
			//Current_Record.Ap3_num = record_ap4_num;
			record_ap4_id = i;
			record_sd_id = sdcard;
			return TRUE;
			}			
		}
	*/
	return FALSE;
}


#if 1
Bool choose_next(void)
{
/*
	U16 temp_id,temp_num;
	U32 temp_data;
	if(Current_booknews[current_area].Max_num == 0)return FALSE;
	Current_booknews[current_area].Cur_num++;
	if(Current_booknews[current_area].Cur_num >= Current_booknews[current_area].Max_num){
		Current_booknews[current_area].Cur_num = 0;
		}
	temp_data = OFFSET_AREA[current_area] + (Current_booknews[current_area].Cur_num<<2);
	(void)read_bnl_file(temp_data);
	record_sd_id = record_ap4_num;
	(void)read_bnl_file((temp_data+2));
#ifdef TX_DEBUG
	SendString("\r\nnext_off_sd_num_cur_max=");
	send_hex_u16(temp_data);
	SendString(":");
	send_hex_u16(record_sd_id);
	SendString(":");
	send_hex_u16(record_ap4_num);
	SendString(":");
	send_hex_u16(Current_booknews[current_area].Cur_num);
	SendString(":");
	send_hex_u16(Current_booknews[current_area].Max_num);
#endif
*/
	return TRUE;
}
#else
Bool choose_next(U16 start_id,U16 end_id,U16 min_id)
{
#ifdef TX_DEBUG
	SendString("\r\nnext_b_o_m=");
	send_hex_u16(start_id);
	SendString(":");
	send_hex_u16(end_id);
	SendString(":");
	send_hex_u16(min_id);
#endif	
	if(!check_have_book_next((min_id+1),end_id))
		{
		if(!check_have_book_next(start_id,min_id))
			{
			return FALSE;
			}
		}
	return TRUE;	
}
#endif


#if 1
Bool choose_prev(void)
{
/*
	U16 temp_id,temp_num;
	U32 temp_data;


	if(Current_booknews[current_area].Max_num == 0)return FALSE;
	if(Current_booknews[current_area].Cur_num == 0){
		Current_booknews[current_area].Cur_num = Current_booknews[current_area].Max_num - 1;
		}
	else{
		Current_booknews[current_area].Cur_num --;
		}
	temp_data = OFFSET_AREA[current_area] + (Current_booknews[current_area].Cur_num<<2);
	(void)read_bnl_file(temp_data);
	record_sd_id = record_ap4_num;
	(void)read_bnl_file((temp_data+2));
#ifdef TX_DEBUG
	SendString("\r\nprev_off_id_num_cur_max=");
	send_hex_u16(temp_data);
	SendString(":");
	send_hex_u16(record_sd_id);
	SendString(":");
	send_hex_u16(record_ap4_num);
	SendString(":");
	send_hex_u16(Current_booknews[current_area].Cur_num);
	SendString(":");
	send_hex_u16(Current_booknews[current_area].Max_num);
#endif	
*/
	return TRUE;
}
#else
Bool choose_prev(U16 start_id,U16 end_id,U16 min_id)
{
#ifdef TX_DEBUG
	SendString("\r\nprev_b_o_m=");
	send_hex_u16(start_id);
	SendString(":");
	send_hex_u16(end_id);
	SendString(":");
	send_hex_u16(min_id);
#endif
	if(min_id==start_id){
		if(!check_have_book_prev(start_id,end_id))
			{
			return FALSE;
			}
		}
	else{
		if(!check_have_book_prev(start_id,(min_id-1)))
			{
			if(!check_have_book_prev(min_id,end_id))
				{
				return FALSE;
				}
			}
		}
	return TRUE;	
}
#endif
/*
Bool change_ap4_choose_pre_next(U16 start_id,U16 end_id,U16 min_id,U16 max_id,U8 type)
{
	if(type==1)
		{
		return choose_next(start_id,end_id,min_id,max_id);
		}
	else if(type==2)
		{
		return choose_pre(start_id,end_id,min_id,max_id);
		}	
}
*/

  

#pragma CODE = DO_CHANGE_AP4_CODE
/*
Bool remote_search_file( void )
{
   Bool find_flag = FALSE;
   FS_ERR fs_err;
   U16 i;
   U16 ap4_num;
   U8 sdcard = 0x3;
   Bool ret=FALSE;
   U16 search_num = RemoteNumber-Range_group[0];
   U16 SD1_seek=search_num*2;
   U16 SD2_seek=search_num*2+0x2000;
   U16 bak_type=cur_type;
   if(RemoteNumber>=Range_group[0]&&RemoteNumber<=Range_group[9])
   	{
	for(i=0;i<5;i++)
		{
		if(RemoteNumber>=Range_group[2*i]&&RemoteNumber<=Range_group[2*i+1])
			{
			sdcard = check_no_ff(SD1_seek,SD2_seek);
			if(sdcard!=0)
				{
				cur_sd=sdcard;
				cur_type=i;
				cur_ap4_num=record_ap4_num[0];
				return TRUE;
				}
			else 
				{
				return FALSE;
				}
			}
		}
   	}
	for(i=0;i<10;i++)
		{
		if(RemoteNumber==NP_group[i])
			{
			cur_type=i;
			break;
			}
		}   
		if(cur_type!=0xFF&&(cur_type/2)!=bak_type)
			{
			if(bak_last_num[cur_type/2]!=0x00)
				{
				bak_last_num[bak_type]=cur_ap4_num;
				cur_ap4_num=bak_last_num[cur_type];
				return TRUE;
				}
		else
			{
			ret = change_ap4_choose_pre_next(
				Range_group[(cur_type/2)*2],
				Range_group[(cur_type/2)*2+1],
				Range_group[(cur_type/2)*2],
				Range_group[(cur_type/2)*2+1],
				1);
			}
		}
		else if(bak_last_num[cur_type/2]!=0x0000)
			{
			if(cur_type%2==0)
				{
				ret = change_ap4_choose_pre_next(
					Range_group[(cur_type/2)*2],
					bak_last_num[cur_type/2],
					Range_group[(cur_type/2)*2],
					Range_group[(cur_type/2)*2+1],
					2);
				}
			else if(cur_type%2==1)
				{
				ret = change_ap4_choose_pre_next(
					bak_last_num[cur_type/2],
					Range_group[(cur_type/2)*2+1],
					Range_group[(cur_type/2)*2],
					Range_group[(cur_type/2)*2+1],
					1);
				}				
   		}
		return ret;
} 
//*/
///*
Bool remote_search_file( void )
{
   Bool find_flag = FALSE;
   FS_ERR fs_err;
   U16 i;
#if 1
	SDKLoadCode(FILESEARCH_CODE);
	if(b_sd1_ok){
		if(search_path_file((char *)ap4_dirname, (char *)AP4_EXT, RemoteNumber)){
			current_sd = FLAG_SD1_CON;
			Current_Record.Ap3_num = current_find_num;
			return TRUE;
			}
		}

	if(b_sd2_ok){
		if(search_path_file((char *)SD2_AP4_DIR, (char *)AP4_EXT, RemoteNumber)){
			current_sd = FLAG_SD2_CON;
			Current_Record.Ap3_num = current_find_num;
			return TRUE;
			}
		}
	
#else
   SDKLoadCode(FILESEARCH_CODE);
   for(i=0;i<ap3_file_number;i++){
   	if(file_get_path((char *)SD1_AP4_DIR, (char *)AP4_Ext, i)){
		  strcpy(( char *)( g_string ),(char *)fat_g_info.name);
		 if( RemoteNumber == get_name_lead()){
		 	current_sd = FLAG_SD1_CON;
			Current_Record.Ap3_num=i;
			return TRUE;
		 	}
   		}
   	}
    
    for(i=0;i<sd2_ap3_number;i++){
   	if(file_get_path((char *)SD2_AP4_DIR, (char *)AP4_Ext, i)){
		  strcpy(( char *)( g_string ),(char *)fat_g_info.name);
		 if( RemoteNumber == get_name_lead()){
		 	current_sd = FLAG_SD2_CON;
			Current_Record.Ap3_num=i;
			return TRUE;
		 	}
   		}
   	}
#endif	
	return FALSE;
  
} 
//*/

#if 1
//change ap3 file
Bool do_change_ap3( void )
{
   //_MEM_TYPE_SLOW_ U16 i;
   	  file_close();
//   	  bak_to_news();
//      init_array_v();
#if 0//def UNCOIDE_FORMAT
	( void )scank_disk(root_dir_falg);
#endif
   	State_Search = Open_File_Search;
 //     ap4_change_file = MMI_AP3_REMOTE_CHANGE_FILE;
  
   //Remote_Search_first_Ap3_file_num = Current_Record.Ap3_num;
   Temp_Ap3_file_num = Current_Record.Ap3_num;
   Temp_Search_record_value = Search_Record.value;

   Remote_Search_first_Ap3_file_num = 0;
//   SendString((char *)CurrentShortFilePath);
//   SendString("\r\n");
   if( remote_search_file())
   	{
	 SendString("\r\nremote:sd:num=");send_hex_u16(current_sd);SendString(":");send_hex_u16(Current_Record.Ap3_num);
		
   	  Search_Record.value = 0;
	  State_Search = Open_File_Search;
//      ap4_change_file = MMI_AP3_KEY_NEXT_CHANGE_FILE;
      g_b_open_file_error = TRUE;
      init_array_v();
      return TRUE;
   	}
    SendString("\r\nnofindap4!");
//	SendString((char *)CurrentShortFilePath);
//	SendString("\r\n");

   	Current_Record.Ap3_num = Temp_Ap3_file_num;
//	bak_from_news();
//  	Get_File_Name( (unsigned char *)Ext, Current_Record.Ap3_num );
// 	open_file();
   return FALSE;
}

 
//get ap3 file name's header
U16 get_name_lead( void )
{
#if 0
#ifdef UNCOIDE_FORMAT
   U16 *p = ( U16 *)( g_string );
#else
   U8 *p = ( U8 *)( g_string );
#endif
#else
	U8 *p = ( U8 *)( g_string );
#endif
   _MEM_TYPE_SLOW_ U16 iNameLead;

   //bRemoteChoiceAp3 = FALSE;
   iNameLead = 0;
   if( !((*p >= '0' )&&( *p <= '9')))
   {
      //bRemoteChoiceAp3 = TRUE;
      return ( RemoteNumber + 1 );
   }
   while((*p >= '0' )&&( *p <= '9'))
   {
      iNameLead = iNameLead * 10 + *p - '0';
      p++;
   }
   SendString((char *)"\r\nnamelead=");
   send_hex_u16(iNameLead);
   return iNameLead;
}
#endif


#pragma CODE = FIND_PATH_CODE

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
/*
U8 *	str_copy(U8 * dst,U8 * src)
{
	U8 *	old_ptr = dst;
	while(*dst++=*src++);
	return	old_ptr;
}

U16 *	str_copy_u(U16 * dst,U16 * src)
{
	U16 *	old_ptr = dst;
	while(*dst++=*src++);
	return	old_ptr;
}
*/

/*
//unicode字符串转化为本地字符串(ascii字符串)
void	unicode_to_local(U16 * unicode_str,U8 * local_str,U16 buffer_size)
{
	while(*unicode_str)
	{
		if(*unicode_str<256)
		{
			*local_str++=*unicode_str++;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
		}
		else
		{
			*local_str++=*unicode_str;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			*local_str++=((*unicode_str)&0xFF)>>8;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			++unicode_str;
		}
	}
	*local_str = 0;
}
*/
//本地字符串(ascii字符串)转化为unicode字符串
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


int	str_comp_a(U8 * sl,U8 * sr,U8 use_case)
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
/*
U8 *	str_copy_n(U8 * dst,U8 * src,U16 num)
{
	U8 *	old_ptr = dst;
	while(num)
	{
		*dst++ = *src++;
		if(*src == 0)
			break;
		--num;
	}
	if(num)
		*dst = 0;
	return	old_ptr;
}
*/
#define	find_path_a(a,b)	find_path_u(atou(a),b) 
Bool find_entry( U8 * spec, U16 *name, Bool is_dir, FILE_INFO __X *finfo, FS_ENTRY_INFO __X *entry )
{
   U16 buffer[ 25 ];
   
   //if( FSGetEntry(( char *)spec, 0, finfo, entry ) != FS_NO_ERROR )
   //   return FALSE;

   //change 2013-05-22 zoe
   unsigned int i;
   FS_ERR fserr;
   //SDKLoadCode(FILE_EXTERN_CODE);
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

#pragma CODE = GET_AP4NAME_CODE
U16 *	str_copy_u_n(U16 * dst,U16 * src,U16 num)
{
	U16 *	old_ptr = dst;
	while(num)
	{
		*dst++ = *src++;
		if(*src == 0)
			break;
		--num;
	}
	if(num)
		*dst = 0;
	return	old_ptr;
}
void	file_local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size)
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

/*
U8 test_area(U16 area_num)
{
	if((area_num<=OVER_NORMAL)){
		return AREA_NORMAL;
		}
	if((area_num >= BEGIN_RECITATIONS)&&(area_num<=OVER_RECITATIONS)){
		return AREA_RECITATION;
		}
	if((area_num >= BEGIN_TRANSLATIONS)&&(area_num<=OVER_TRANSLATIONS)){
		return AREA_TRANSLATION;
		}
	if((area_num >= BEGIN_EXPLANTION)&&(area_num<=OVER_EXPLANTION)){
		return AREA_EXPLANTION;
		}
	if((area_num >= BEGIN_OTHER)&&(area_num<=OVER_OTHER)){
		return AREA_OTHER;
		}
	return 0xFF;
}
*/
//get ap3 file name's header
void get_area_num( void )
{
/*
	U16 i,j;
	U16 temp_data,temp_off;
	//current_area = test_area(Current_lastbook[].Book_id);
	for(i=0;i<Current_booknews[current_area].Max_num;i++){
		temp_off = OFFSET_AREA[current_area] + i*4;
		(void)read_bnl_file(temp_off);
		temp_data = record_ap4_num;
		if(temp_data == Current_Record.Ap3_num){
			Current_booknews[current_area].Cur_num = i;
			return;
			}
		}
	*/
}

void display_filenews(void)
{
#ifdef TX_DEBUG
	SendString("\r\nid:sd:num:area:cur:max=");
	send_hex_u16(Current_lastbook[current_area].Book_id);SendString(":");
	send_hex_u16(Current_lastbook[current_area].Sd_num);SendString(":");
	send_hex_u16(Current_lastbook[current_area].Book_num);SendString(":");
	send_hex_u16(current_area);SendString(":");
	send_hex_u16(Current_booknews[current_area].Cur_num);SendString(":");
	send_hex_u16(Current_booknews[current_area].Max_num);
#endif
}
 
//get ap3 file name's header
U16 get_book_id( void )
{

	
   _MEM_TYPE_SLOW_ U16 iNameLead;
	U8 *p = ( U8 *)( ap4_short_name );
   //bRemoteChoiceAp3 = FALSE;
   iNameLead = 0;
   if( !((*p >= '0' )&&( *p <= '9')))
   {
      //bRemoteChoiceAp3 = TRUE;
      return 0xFFFF;
   }
   while((*p >= '0' )&&( *p <= '9'))
   {
      iNameLead = iNameLead * 10 + *p - '0';
      p++;
   }
   SendString((char *)"\r\nnamelead=");
   send_hex_u16(iNameLead);
   record_ap4_id = iNameLead;
   return iNameLead;
}

///*
//the function is get the file name;
Bool Get_File_Name(  void )
{
	//FILE_INFO	__X	finfo;
	//FS_ENTRY_INFO	__X	fentry;
	U8 _MEM_TYPE_SLOW_ dot_index;
    U16 *p1, *p2;
	unsigned char	_MEM_TYPE_SLOW_	buffer[FS_MAX_NAME_BUFFER];
	for( dot_index = 0; dot_index < SHORT_NAME_SIZE; ++dot_index )
        {
           if( fat_g_info.name[ dot_index ] == '.' )
              break;
           if( fat_g_info.name[ dot_index ] == 0 )
              break;
           ap4_short_name[ dot_index ] = fat_g_info.name[ dot_index ];
        }
        ap4_short_name[ dot_index ] = 0;


			if(*(U16 *)fat_g_info.lfn)
			{
				str_copy_u_n((U16 *)g_string1,(U16 *)fat_g_info.lfn,min(sizeof(g_string1),sizeof(buffer)));
			}
			else
			{
				file_local_to_unicode((U8 *)fat_g_info.name,(U16 *)buffer,sizeof(buffer));
				str_copy_u_n((U16 *)g_string1,(U16 *)buffer,min(13,sizeof(buffer)));
			}

        	p1 = ( U16 *)g_string1;
	        p2 = ( U16 *)ap4_long_name;
	        for( dot_index = 0; dot_index <( G_STRING_SIZE1 / 2 - 1 ); ++dot_index )
	        {
	           if( *p1 == '.' )
	              break;
	           if( *p1 == 0 )
	              break;
	           *p2++ = *p1++;
	        }
	        *p2 = 0;
#ifdef TX_DEBUG
		SendString((char *)"\r\ns:l=");
		SendString((char *)ap4_short_name);
		SendString((char *)":");
		SendString((char *)ap4_long_name);
#endif
		return TRUE;
 
}



#pragma CODE = LINK_FILENAME_CODE
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
void link_filename(U8 link_flag)
{
	    U16	m[] = {'\\','\0'};
#ifdef UNCOIDE_FORMAT
		strcpy_u((U16 *)tisiFileName,(U16 *)&root);
		strcat_u((U16 *)tisiFileName,(U16 *)&subdir);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		switch (link_flag)
			{
				case 0:
					strcat_u((U16 *)tisiFileName,(U16 *)&NO_AP4_FILE);
					break;
				case 1:
					strcat_u((U16 *)tisiFileName,(U16 *)&SYS_OPEN_FILE);
					break;
				case 2:
					strcat_u((U16 *)tisiFileName,(U16 *)&BATTERY_LOW_FILE);
					break;
				case 3:
					strcat_u((U16 *)tisiFileName,(U16 *)&POWER_OFF_FILE);
					break;	
				case 4:
					strcat_u((U16 *)tisiFileName,(U16 *)&RECORDSTART);
					break;
				case 5:
					strcat_u((U16 *)tisiFileName,(U16 *)&RECORDEND);
					break;
				case 6:
					strcat_u((U16 *)tisiFileName,(U16 *)&PLAYSTART);
					break;
				case 7:
					strcat_u((U16 *)tisiFileName,(U16 *)&PLAYEND);
					break;	
				case 8:
					strcat_u((U16 *)tisiFileName,(U16 *)&DISKFULL);
					break;	
				case 9:
					strcat_u((U16 *)tisiFileName,(U16 *)&SIMPLE);
					break;		
				case 10:
					strcat_u((U16 *)tisiFileName,(U16 *)&NO_FINDAP4);
					break;	
				case 11:
					strcat_u((U16 *)tisiFileName,(U16 *)&STOPREC);
					break;
				case 12:
					strcat_u((U16 *)tisiFileName,(U16 *)&DEL);
					break;
				case 13:
					strcat_u((U16 *)tisiFileName,(U16 *)&ERROR);	
					break;	
				case 14:
					strcat_u((U16 *)tisiFileName,(U16 *)&FILEFULL);	
					break;	
				case 15:
					strcat_u((U16 *)tisiFileName,(U16 *)&REREAD);	
					break;		
				default:
					return;
					break;
			}
#endif

#ifdef ASCII_FORMAT
		strcpy(tisiFileName,(char*)ROOT);
		strcat(tisiFileName,(char *)SUBDIR);
		strcat(tisiFileName,"\\");
		switch (link_flag)
			{
				case FLAG_NOAP4FILE:
					strcat(tisiFileName,(char *)NO_AP4_FILE);
					break;
				case FLAG_SYS_OPEN_FILE:
					strcat(tisiFileName,(char *)SYS_OPEN_FILE);
					break;
				case FLAG_BATTERY_LOW_FILE:
					strcat(tisiFileName,(char *)BATTERY_LOW_FILE);
					break;
				case FLAG_POWER_OFF_FILE:
					strcat(tisiFileName,(char *)POWER_OFF_FILE);
					break;	
				case FLAG_RECORDSTART:
					strcat(tisiFileName,(char *)RECORDSTART);
					break;	
				case FLAG_RECORDEND:
					strcat(tisiFileName,(char *)RECORDEND);
					break;	
				case FLAG_PLAYSTART:
					strcat(tisiFileName,(char *)PLAYSTART);
					break;	
				case FLAG_PLAYEND:
					strcat(tisiFileName,(char *)PLAYEND);
					break;	
				case FLAG_DISKFULL:
					strcat(tisiFileName,(char *)DISKFULL);
					break;	
				case FLAG_SIMPLE:
					strcat(tisiFileName,(char *)SIMPLE);
					break;	
				case FLAG_NO_FINDAP4:
					strcat(tisiFileName,(char *)NO_FINDAP4);
					break;	
				case FLAG_MIN_TISHI:
					strcat(tisiFileName,(char *)MIN_TISHI);
					break;	
				case FLAG_STOP_MP3:
					strcat(tisiFileName,(char *)STOP_MP3);
					break;	
				case FLAG_REPEAT:
					strcat(tisiFileName,(char *)REPEAT_MP3);
					break;	
				case FLAG_MINVOL:
					strcat(tisiFileName,(char *)MINVOL_MP3);
					break;
				case FLAG_MAXVOL:
					strcat(tisiFileName,(char *)MAXVOL_MP3);
					break;
				case FLAG_VOLUME:
					strcat(tisiFileName,(char *)VOLUME_MP3);
					break;	
				case FLAG_REREAD:
					strcat(tisiFileName,(char *)REREAD_MP3);
					break;	
				default:
					return;
					break;
			}
#endif		
}
#pragma CODE = MAKE_SD_DIR_CODE
void readyAp4DirName(void)
{
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
	U16	ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,0x0000};//有声课本
	//U16	record[] = {0x5F55,0x97F3,0x0000};//录音


	U16   sd1_root[] = {'0',':','\\','\0'};

	memset(tisiFileName,0,sizeof(tisiFileName));
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
//		judge_op();
// 		swap_code(FILE_EXTERN_CODE_FLAG);
//		SDKLoadCode(FILE_EXTERN_CODE);
		SDKLoadCode(LINK_FILENAME_CODE);
		strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
		strcat_u((U16 *)tisiFileName,(U16 *)&ap4_book);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);

		SDKLoadCode(FIND_PATH_CODE);
		(void)find_path_u((U16 *)&tisiFileName,(U8 *)ap4_dirname);
		//*/
#else		
		strcpy(ap4_dirname,(char *)SD1_AP4_DIR);
#endif		
}
#if 0
void mmc_sd_all_mode_dir_make( void )
{
	FS_ERR fs_err;
	U8 j;
	U16	m[] = {'\\','\0'};
/*
		memset(tisiFileName,0,128);
		for(j=0;j<8;j++){
			tisiFileName[j]=subdir[j];
			}
		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)subdir);


		memset(tisiFileName,0,128);
		for(j=0;j<8;j++){
			tisiFileName[j]=AP4[j];
			}
		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)AP4);
*/
//		strcpy(tisiFileName,"点读文件");
//		fs_err = FSDirectory(tisiFileName);
//		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)AP4);

//		strcpy(tisiFileName,"系统文件");
//		fs_err = FSDirectory(tisiFileName);
//		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)subdir);

		strcpy(tisiFileName,"0:\\ap4");
		fs_err = FSDirectory(tisiFileName);
		
		//strcpy(tisiFileName,"0:\\dic");
		//fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,"0:\\mp3");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,"0:\\record");
		fs_err = FSDirectory(tisiFileName);		
#ifdef USE_SD2
		strcpy(tisiFileName,"1:\\AP4");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,"1:\\mp3");
		fs_err = FSDirectory(tisiFileName);

//		strcpy(tisiFileName,"0:\\rec");
//		fs_err = FSDirectory(tisiFileName);		

		strcpy(tisiFileName,"1:\\record");
		fs_err = FSDirectory(tisiFileName);
#endif

#if 0
		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)subdir);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)AP4);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)MP3_FILE);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		
		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)DIC_FILE);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);
#endif
}
#endif
//==============================================
#pragma CODE = TISHI_MP3_CODE
void tishikey_dowith(U16 temp_key)
{
	
}
void tishi_play_mp3(boolean flag)
{

	    FS_ERR fs_err;
	    tAudioErr   Audio_err;
		U16 temp_key;
		unsigned long len=0;
		//char path[64];
       // U16	m[] = {'\\','\0'};
		clkup_time(true);

//		swap_code(DAC_CODE_FLAG);
		SDKLoadCode(MP3_COMMON_CODE);
	    SDKLoadCode(DAC_COMMON_CODE);
		
//		flag_wav_play = FALSE;
        Audio_err = AudioMP3Init1k0(0);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	
        	 return;
        	}
 //       Audio_err = AudioMP3Open((signed char *)tisiFileName);

#if 1
			//OP_CLOSE();
			//DAC_Enable_HeadPhone();
			//DAC_HeadPhone_Volume(VolTbl[vol_idx]);
			vol_idx_bak = VOL_NUM;
			
#else
		if(!flag_load_mp3_dac){
	   		AudioMP3SetEQ(EQ_NOMAL);
			judge_op();
			DAC_Disable_Speaker();
			DAC_Enable_HeadPhone();
			DAC_HeadPhone_Volume(VolTbl[vol_idx]);
			flag_load_mp3_dac = TRUE;
			}
	    else{
			DAC_HeadPhone_Volume(0);
			judge_op();
			vol_inc();
	    	}
#endif		

 		Audio_err = AudioMP3Open((char *)tisiFileName);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        		Audio_err = AudioMP3Uninit();
				return;
        	}



		//DAC_FadeIn();
		//DAC_Stereo_to_Mono(1);
		flag_wav_play = FALSE;
		SpeakDetect(TRUE);
		op_stop_timer();
		clkup_time(FALSE);
        Audio_err = AudioMP3Play();
		bPenOn = false;
    	while (CheckPlaying(FALSE))
    	{    		
    		ledFlash();
    		adc_get();
			kbd_process();
			temp_key = kbd_key_read();
			if(temp_key!=0){
				tishikey_dowith(temp_key);
				}
		if(flag){
			scan_oid();
			if(index_flag){
				if(Current_Index == INDEX_STOP)break;
					}
				}
			
    	}
		//DAC_FadeOut();
		//flag_load_mp3_dac = TRUE;
 //   	OP_CLOSE();
		Audio_err = AudioMP3Stop();
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();
		AudioMP3FileCloseAfter();
		op_start_timer();
		clkup_time(TRUE);
		kbd_init_process();
//		swap_code_flag &= ~SYSTEM_CODE_FLAG;
}

#pragma CODE = MMI_AP3_CODE
void tishi_mp3(U8 numFlag,Bool flag)
{

	    FS_ERR fs_err;
	    tAudioErr   Audio_err;

		unsigned long len=0;
		char path[64];
        U16	m[] = {'\\','\0'};
		clkup_time(TRUE);
		memset(tisiFileName,0,sizeof(tisiFileName));
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
//		judge_op();
// 		swap_code(FILE_EXTERN_CODE_FLAG);
//		SDKLoadCode(FILE_EXTERN_CODE);
		SDKLoadCode(LINK_FILENAME_CODE);
		link_filename(numFlag);
#if 0//def TX_DEBUG
		SendString("\r\ntishimp3=");
		SendString(tisiFileName);
#endif
		SDKLoadCode(FIND_PATH_CODE);
#ifdef UNCOIDE_FORMAT
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)path))return;
#endif

#ifdef ASCII_FORMAT
		if(!find_path_a((U8 *)tisiFileName,(U8 *)path))return;
#endif		
		len = FSGetFileSize((char *)path, &fs_err);
		if(0==len)return;
		strcpy(tisiFileName,path);
#ifdef TX_DEBUG
		SendString("\r\ntishimp3=");
		SendString(tisiFileName);
#endif			
		if(numFlag == FLAG_SYS_OPEN_FILE){
			b_startup_play = TRUE;
			}
		OID_PLAY = FALSE;
		g_b_play_afx = FALSE;
		AudioMP3FileOpenFirst( (char *)tisiFileName );
		SDKLoadCode(TISHI_MP3_CODE);
		tishi_play_mp3(flag);
}
#pragma CODE = TRANSLATE_CODE
void translateFunction(void)
{
	if(Addr_Fanyi){
		if(Num_Cur_Fanyi>=Num_Fanyi)
			{
			 if(Num_Fanyi==1){
			 	Num_Cur_Fanyi=1;
			 	}else{
					 Num_Cur_Fanyi = 2;
			 		}
			}
		else{
				Num_Cur_Fanyi++;
			}
		write_media_addr( (Addr_Fanyi+(Num_Cur_Fanyi-1)*9), 0 );
		state_search_to_play();
	}
}

#pragma CODE = CHANGE_CON_MODE_CODE
void change_control_mode(void)
{
	if(b_current_control_game)return;
	 SDKLoadCode(READ_CONTR_NEWS_CODE);
	if(News_Go.Num_ControlTab){
		Temp_Control_num = Current_Record.Control_num;
		 Current_Record.Control_num++;
		while(Current_Record.Control_num < News_Go.Num_ControlTab){
			 SDKLoadCode(READ_CONTR_NEWS_CODE);
      		  read_control_mode_news( Current_Record.Control_num );
			  if(News_Control.Type_Control != GAME_TYPE ){
			  	State_Search = Control_Mode_Search;	
				return;
			  	}
			  Current_Record.Control_num++;
			}
		Current_Record.Control_num = 0;
		while(Current_Record.Control_num <= Temp_Control_num){
			 
      		  read_control_mode_news( Current_Record.Control_num );
			  if(News_Control.Type_Control != GAME_TYPE ){
			  	State_Search = Control_Mode_Search;	
				return;
			  	}
			  Current_Record.Control_num++;
			}
		Current_Record.Control_num = Temp_Control_num;
		State_Search = Control_Mode_Search;	
		//Search_Record.news_sys.table_flag = DEFAULT_TABLE;
		return;
		}
	
/*
    if(( Ap3_MenuPath >= CONTROL_MENU_PATH )
       && News_Go.Num_ControlTab )
    {
       Temp_Control_num = Current_Record.Control_num;
       if( ++Current_Record.Control_num >= News_Go.Num_ControlTab )
          Current_Record.Control_num = 0;
	   
       State_Search = Control_Mode_Search;
//       ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
    }
*/	
}
#pragma CODE = KEYLONG_VOL_CODE
void keylong_vol(U8 key_data)
{
#if 0
	U16 temp_ap4;
     Search_Record.value = 0;
	 file_close();
	 //temp_ap4 = Current_Record.Ap3_num;
	 temp_ap4 = Current_lastbook[current_area].Book_id;
	 SDKLoadCode(AP4_CHOOSE_PRE_NEXT);
	 if(KEY_VOL_INC == key_data){
	 	if(!choose_next())goto reopen;
	 	}
	 else{
		if(!choose_prev())goto reopen;
	 	}
	 	Current_Record.Ap3_num = record_ap4_num;
		current_sd = record_sd_id;
		
	 	Current_lastbook[current_area].Book_num = Current_Record.Ap3_num ;
		Current_lastbook[current_area].Sd_num = current_sd;
		Current_lastbook[current_area].Book_id = record_ap4_id;
reopen:		
 		g_b_open_file_error = TRUE;
	  	first_go_ap4 = 0;
  		b_back_from_mp3 = FALSE;
		//b_back_from_rp = FALSE;
		b_back_have_index = FALSE;
		b_current_control_game = FALSE;
  		State_Search = Open_File_Search;
#else
    Search_Record.value = 0;
	 file_close();
	 if(KEY_VOL_INC == key_data){
	  Current_Record.Ap3_num++;
	   if( Current_Record.Ap3_num >= ap3_file_number )Current_Record.Ap3_num = 0;
	  /*
		  if(FLAG_SD1_CON == current_sd){
		  	 if( Current_Record.Ap3_num >= ap3_file_number ){
	            Current_Record.Ap3_num = 0;
				current_sd = FLAG_SD2_CON;
	         }
		  	}
		  else{
		  	if( Current_Record.Ap3_num >= sd2_ap3_number ){
	            Current_Record.Ap3_num = 0;
				current_sd = FLAG_SD1_CON;
		  	}
		  }
	 */
	 	}
	 else{
				if( Current_Record.Ap3_num == 0 ){
		            Current_Record.Ap3_num = sd2_ap3_number-1;
		         }
			  else{
			  	Current_Record.Ap3_num--;
			  	}
		
	 	/*
		  if(FLAG_SD1_CON == current_sd){
			  	if( Current_Record.Ap3_num == 0 ){
		            Current_Record.Ap3_num = sd2_ap3_number-1;
					current_sd = FLAG_SD2_CON;
		         }
			  else{
			  	Current_Record.Ap3_num--;
			  	}
		  	}
		  else{
		  	}
		  if( Current_Record.Ap3_num == 0 ){
	            Current_Record.Ap3_num = ap3_file_number-1;
				current_sd = FLAG_SD1_CON;
	         }
		  else{
		  	Current_Record.Ap3_num--;
		  	}
		  */
	 	}
 		//g_b_open_file_error = TRUE;
	  	//first_go_ap4 = 0;
  		//b_back_from_mp3 = FALSE;
		//b_back_from_rp = FALSE;
		//b_back_have_index = FALSE;
		b_current_control_game = FALSE;
  		//State_Search = Open_File_Search;
#endif		
}
#endif  //#if( AP3 == ENABLE )

#endif // _mmi_ap3_c_

