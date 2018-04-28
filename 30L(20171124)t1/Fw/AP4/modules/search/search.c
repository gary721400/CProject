//!
//! @file search.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search.
//!
//! @version 1.53 snd3b-dvk-1_9_0 $Id: search.c,v 1.53 2007/04/03 09:49:23 sguyon Exp $
//!
//! @todo
//! @bug

#pragma DATA = SEARCH_DATA
#pragma CODE = SEARCH_CODE
#ifndef _search_c_
#define _search_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "stdio.h"

#include "search.h"
#include "search_game.h"
#include "search_remote.h"
#include "player_ap3.h"
#include "file.h"
#include "navigation.h"
//#include "unicode.h"

//#include "mail.h"
//#include "mail_evt.h"
#include "mmgr_kernel.h"
//#include "com_appli.h"
//#include "com_var.h"
#include "com.h"
#include "conf_oid.h"
#include "mmi_ap3.h"
//#include "string.h"
//#include "srv_player.h"
//#include "search_task.h"
//#include "mmi_player.h"
//#include "stdlib.h" // Contains Funktion "rand()"

//#include "system.h"
#include "oid.h"
#include "power.h"             // Power module definition

//#include "mmi_player.h"
#include "player_task.h"
//#include "power_task.h"
//#include "int_drv.h"
//#include "mmi_startup.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "keypad.h"
//#include "BinaryImage.h"
#include "REC_API.H"

#include "fs.h"
#include "fc_internal.h"
#if 1//( AP3 == ENABLE )
//extern   xdata    AL_Variables     	varAl;

//_____ M A C R O S ________________________________________________________

//define coordinate range
#if( OID_VERSION == OID1 )
#define Coor_Invalidation        ( 0 )       //0:invalidation
#define Coor_Normal              ( 3300 )    //1-3300:normal
#define Coor_Book                ( 3350 )    //3301-3350:book
#define Coor_Page                ( 3700 )    //3351-3700:page
#define Coor_Ctr                 ( 4000 )    //3701-4000:control mode
#define Coor_Remote              ( 4095 )    //4001-4095:remote mode
#elif( OID_VERSION == OID2 )
#define Coor_Invalidation        ( 0 )  //0:invalidation
#define Coor_Normal              ( 56000 )  //1-56000:normal
#define Coor_Book                ( 56500 )  //56001-56500:book
#define Coor_Page                ( 58000 )  //56501-58000:page
#define Coor_Ctr                 ( 59000 )  //58001-59000:control mode
#define Coor_Remote              ( 64000 )  //59001-60000:remote mode
#endif


 _MEM_TYPE_SLOW_  U32				   Current_Special_Action;
 _MEM_TYPE_SLOW_  Action_Sequence      Current_Act_Sequence;
 _MEM_TYPE_SLOW_  U32                  sequence_speech_address;
 _MEM_TYPE_SLOW_  U32                  sequence_text_address;
 _MEM_TYPE_SLOW_  U32                  sequence_speech_read_point;


//#define Current_Index                           u16_current_index

 //_MEM_TYPE_SLOW_  Record               Current_Record;
 _MEM_TYPE_SLOW_  News_All_Control     All_Control;
 _MEM_TYPE_SLOW_  News_Index_Range     Index_Range;
  _MEM_TYPE_SLOW_  Other_Range     		Current_Other_Range;
 _MEM_TYPE_SLOW_  Record_Range		   Ap4_Record_Range;
 _MEM_TYPE_SLOW_  Book_Table           News_Book;
 _MEM_TYPE_SLOW_  Go_Table             News_Go;
 _MEM_TYPE_SLOW_  Control_Table        News_Control;
 _MEM_TYPE_SLOW_  Coor                 Current_Coor;
 _MEM_TYPE_SLOW_  U8                   Coor_Type;
 _MEM_TYPE_SLOW_  U8                   Encrypt_Key;



 _MEM_TYPE_SLOW_  U16                  Temp_Control_num;
 _MEM_TYPE_SLOW_  U16                  Temp_Ap3_file_num;
 _MEM_TYPE_SLOW_  U16                  Temp_Search_record_value;
 _MEM_TYPE_SLOW_  U8         	        Remote_Search_first_Ap3_file_num;

 _MEM_TYPE_SLOW_  U8                   State_Search;

 _MEM_TYPE_SLOW_  U32                  Seed;

 _MEM_TYPE_SLOW_ SEARCH_STATUS 			Search_Record;

 _MEM_TYPE_SLOW_  U8                   first_go_ap4;
 _MEM_TYPE_SLOW_  U32 					Key_encrypt_mp3;

  _MEM_TYPE_SLOW_  U16 					flag_record_ap4_num;

_MEM_TYPE_SLOW_   U8					flag_ap4_play_pause;
 _MEM_TYPE_SLOW_  U32                  size_current_ap4;

  _MEM_TYPE_SLOW_  Record_Range		   Ap4_Record_Range_2;
// _MEM_TYPE_SLOW_ U32 file_size_count;
 //_MEM_TYPE_SLOW_ U32 play_file_size;


 _MEM_TYPE_SLOW_  U32 					Addr_Fanyi;  
  _MEM_TYPE_SLOW_ U8					Num_Fanyi;
  _MEM_TYPE_SLOW_ U8					Num_Cur_Fanyi;

   _MEM_TYPE_SLOW_   Bool									flag_fanyi;

  _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;
  _MEM_TYPE_SLOW_  U32				   mp3_over_clock;

  _MEM_TYPE_SLOW_  U32                    rec_timer;
    _MEM_TYPE_SLOW_  U32			  addr_novoice;
//==========================================================================
//__X ClusterListItem cluster_buf[16];
//_____ D E F I N I T I O N ________________________________________________

_MEM_TYPE_SLOW_  Action          Current_Act;
_MEM_TYPE_SLOW_  U16			 Point_Write;
_MEM_TYPE_SLOW_  U16              Point_Read;
//_MEM_TYPE_SLOW_  Speech          Array_V[ Length_v ];
__Y  			Speech          Array_V[ Length_v ];
_MEM_TYPE_SLOW_  U16             repeat_point;
_MEM_TYPE_SLOW_  U32    		prev_index;
_MEM_TYPE_SLOW_  U8				remember_num;
//_MEM_TYPE_SLOW_  U16		 	 mp3_addr_bak_flag_record;
//_MEM_TYPE_SLOW_  U16		 	 mp3_addr_bak_num_record;
//_MEM_TYPE_SLOW_  Speech		 	 mp3_addr_bak[ Length_v ];

_MEM_TYPE_SLOW_	  U32 					all_control_addr;
_MEM_TYPE_SLOW_	  U32 					 all_control_pw;

_MEM_TYPE_SLOW_  U16    num_repeat;
   _MEM_TYPE_SLOW_  U32			  action_addr_bak;
      Bool			  action_flag_bak;

_MEM_TYPE_SLOW_  Speech         lastSpeech;
_MEM_TYPE_SLOW_  char             ap4_dirname[ 32 ];
//***********************************************
#define AP3_MAX_RANGE_RAND       4       // 32 files
_MEM_TYPE_SLOW_  U8              g_ap3_rand_tab[ AP3_MAX_RANGE_RAND ];
//***********************************************
_MEM_TYPE_SLOW_ LASTBOOK    Current_lastbook[NUM_AREA];
_MEM_TYPE_SLOW_ BOOKNEWS    Current_booknews[NUM_AREA];
//*********************************************
extern Bool g_b_search_on;
extern U16		swap_code_flag;
extern __X char CurrentShortFilePath[256];

//#define AP4_ENCRYPT              ENABLE
#define AP4_ENCRYPT              DISABLE
/*
#ifdef UNCOIDE_FORMAT
const U16 NO_AP4_FILE[] = {'n','o','a','p','4','.','m','p','3','\0'};
const U16 SYS_OPEN_FILE[] = {'s','t','a','r','t','u','p','.','m','p','3','\0'};
const U16 BATTERY_LOW_FILE[] = {'b','a','t','t','e','r','y','l','o','w','.','m','p','3','\0'};
const U16 POWER_OFF_FILE[] = {'p','o','w','e','r','o','f','f','.','m','p','3','\0'};
//const U16 RECORDSTART[] = {'r','e','c','o','r','d','s','t','a','r','t','.','m','p','3','\0'};
const U16 RECORDSTART[] = {'r','e','c','o','r','d','.','m','p','3','\0'};
const U16 RECORDEND[] = {'r','e','c','o','r','d','e','n','d','.','m','p','3','\0'};
const U16 PLAYSTART[] = {'p','l','a','y','s','t','a','r','t','.','m','p','3','\0'};
const U16 PLAYEND[] = {'p','l','a','y','e','n','d','.','m','p','3','\0'};
const U16 DISKFULL[] = {'d','i','s','k','f','u','l','l','.','m','p','3','\0'};
const U16 SIMPLE[] = {'s','i','m','p','l','e','.','m','p','3','\0'};
const U16 NO_FINDAP4[] = {'n','o','t','f','i','n','d','a','p','4','.','m','p','3','\0'};
const U16 STOPREC[] = {'s','t','o','p','r','e','c','.','m','p','3','\0'};
const U16 DEL[] = {'d','e','l','.','m','p','3','\0'};
const U16 ERROR[] = {'e','r','r','o','r','.','m','p','3','\0'};
const U16 FILEFULL[] = {'f','i','l','e','f','u','l','l','.','m','p','3','\0'};
const U16 REREAD[] = {'r','e','r','e','a','d','.','m','p','3','\0'};
const U16 MIN_TISHI[] = {'w','a','i','t','p','r','o','m','p','t','1','.','m','p','3','\0'};
#endif
*/
#ifdef ASCII_FORMAT


#endif

#ifdef 	ALL_NEWS_KEY
//const  U8       all_news_key[] = {'S','A','R','S'};

//const U32 all_news_key = 0x48594E57;  //{'H','Y','N','W'};
#endif
const  U8       all_news_key[] = {'H','Y','N','W'};
const  RANGE_AREA CUR_RANGE_AREA[NUM_AREA] = {{BEGIN_NORMAL,OVER_NORMAL},{BEGIN_RECITATIONS,OVER_RECITATIONS},
								{BEGIN_TRANSLATIONS,OVER_TRANSLATIONS},{BEGIN_EXPLANTION,OVER_EXPLANTION},
								{BEGIN_OTHER,OVER_OTHER},};
const U16 OFFSET_AREA[NUM_AREA] = {OFFSET_NORMAL,OFFSET_RECITATIONS,OFFSET_TRANSLATIONS,
								OFFSET_EXPLANATION,OFFSET_OTHER};
#if( AP4_ENCRYPT == ENABLE )
//#define DECODE_NORMAL_FILE       ENABLE
#define DECODE_NORMAL_FILE       DISABLE
#endif   //#if( AP4_ENCRYPT == ENABLE )

//tern unsigned int vol_idx;
//tern const int VolTbl[VOL_NUM];
//tern unsigned int volume_inc_flag;
//tern unsigned int prev_task;
//tern unsigned int wav_action;
extern _MEM_TYPE_SLOW_ U32 ScriptCurrentAddress;
extern _MEM_TYPE_SLOW_ U32 ScriptCurrentAddress_bak;
//_____ D E C L A R A T I O N ______________________________________________
Bool  judge_coor_01                 ( void );
Bool  judge_coor_02                 ( void );
void  judge_key                     ( void );
Bool  search_twobytes_coordinate    ( U32 addr, U16 number );
void  read_book_news                ( U16 num_book );
void  read_control_mode_news        ( U16 num_control );
void  read_page_news                ( U16 num_page );
void  read_sgdb                     ( void );
void  repeat_open_file              ( void );
void  speech_play_end               ( void );
Bool  ap3_rand_table_is_full        ( U8 number );
void  voice_play_number             ( U32 address );
Bool  record_judge					( void );
void srand( U32 s );
U16 rand( void );
//***********************************************

//#define SIZE_G_BUF 128
//unsigned char g_buf[SIZE_G_BUF];

extern _MEM_TYPE_SLOW_ Bool                  g_b_player_on;
extern   _MEM_TYPE_SLOW_   U16	record_ap4_num;
extern   _MEM_TYPE_SLOW_   U16	record_ap4_id;
extern   _MEM_TYPE_SLOW_   U16	 record_sd_id;
Bool flag_change_ap4;
Bool g_b_search_on = FALSE;
//Bool flag_wav_play;
__X  Bool flag_reread;
_MEM_TYPE_SLOW_  U16    code_flag;
_MEM_TYPE_SLOW_  U8     flag_gendu;

extern FILE_INFO __X	fat_g_info;
extern   Bool     g_b_search_record;
extern __X  U16         rp_flag;
extern char	__X	g_full_path[];
//tern unsigned int bak_current_index;
extern FCFILE __X *	fat_g_cur_file;
//extern void swap_code(U16 flag);
extern void rp_task(void);
extern void player_task(void);

/////////////////////////////////////////////////////////////
//extern  ClusterListItem cluster_buf[16];
extern int	kb_fat_check_cluster(FSINFO * fs_info,U32 cluster);
extern Bool FSCreateBuf( FCFILE *pfile );
//Bool fix_index(void);
//Bool nostop_index(void);
void judge_coor_deal(void);
Bool read_page_other_news(void);
void act_read(U32 addr);
Bool play_ap3_deal(U32 addr, U8 flag);
Bool read_page_other_news(void);
Bool change_ap4(void);
void auto_close_dowith(void);
Bool search_open_ap4_00(void);
void search_open_ap4_01(void);
void remote_change_project(void);
void control_news_dowith(void);
Bool fix_index_01(void);
Bool fix_index_02(void);
Bool fix_index_03(void);
Bool fix_index_04(void);
Bool fix_index_05(void);
Bool fix_index_06(void);
Bool fix_index_07(void);
Bool fix_index_08(void);
Bool fix_index_09(void);
Bool fix_index_10(void);
Bool bak_dowith(void);
void random_ready(U32 address);
void ordea_ready(U32 address);
void remember_ready(U32 address);
void print_news_ap4(void);
Bool open_newAp4(void);
void bak_to_ap4(void);
Bool judge_coor_03( void );
void cleanStruct(void);
Bool judge_password(void);
void new_read_all_control_news( void );
void actSelect(U16 data);
/////////////////////////////////////////////////////////


//***********************************************

//_____ D E C L A R A T I O N ______________________________________________



//! Initialization of search task
//!

//! Start search task
//!


//! Stop search task
//!
//void search_task_stop( void )
//{
//   g_b_search_on = FALSE;
//}

//the function is realize the search

//the function is realize the search
void search_evt( void )
{
	//unsigned int	buf_offset = 0;
	unsigned char	t;
	_MEM_TYPE_SLOW_ U16 temp_key=0;
	temp_key = kbd_key_read();
   if(0!=temp_key)
   	{                           
		g_b_autopoweroff_forbidden = TRUE; 
		if(!player_ap3_is_busy())clkup_time(TRUE);
		//b_2min_tishi = FALSE;
#ifdef TX_DEBUG
   	    SendString("\r\ntemp_key=");
   	    send_hex_u16(temp_key);
#endif		
   		Ap4_Key_Process(temp_key);
   	}
   if( index_flag )
   	{
	  index_flag=FALSE;
#ifdef TX_DEBUG
   	    SendString("\r\nfind index!");
#endif		 
	  clkup_time(TRUE);
 	  SDKLoadCode(FIX_INDEX_CODE01);
	  if(fix_index_01())return;
	 /*
	  SDKLoadCode(FIX_INDEX_CODE02);
	  if(fix_index_02())return;
	   SDKLoadCode(FIX_INDEX_CODE03);
	  if(fix_index_03())return;

	  SDKLoadCode(FIX_INDEX_CODE04);
	  if(fix_index_04())return;
	  SDKLoadCode(FIX_INDEX_CODE05);
	  if(fix_index_05())return;

	  SDKLoadCode(FIX_INDEX_CODE06);
	  if(fix_index_06())return;
	   SDKLoadCode(FIX_INDEX_CODE07);
	  if(fix_index_07())return;

	  SDKLoadCode(FIX_INDEX_CODE08);
	  if(fix_index_08())return;
	  SDKLoadCode(FIX_INDEX_CODE09);
	  if(fix_index_09())return;
	   SDKLoadCode(FIX_INDEX_CODE10);
	  if(fix_index_10())return;
	*/	
	  SDKLoadCode(JUDGE_COOR01_CODE);
	  if(judge_coor_01())return;
	  ///*
	  SDKLoadCode(JUDGE_COOR02_CODE);
	  if(judge_coor_02()){
	  	if(b_cal_over){
			b_cal_over = FALSE;
	  		tishi_mp3(FLAG_PLAYSTART,false);
	  		}
#ifdef TX_DEBUG
   	    SendString("\r\n388!");
#endif			
	  	}
	  else{
	  	 SDKLoadCode(JUDGE_COOR03_CODE);
		 (void)judge_coor_03();
		 if(bInvalidFlag){
		 	bInvalidFlag = false;
		 	//tishi_mp3(FLAG_INVALIDFILE, false);
		 	(void)act_do( News_Go.Action_NoVoice, 0 );
		 	}
	  	}//*/
	  
   	}

   switch( State_Search )
   {
      case Open_File_Search:
		SDKLoadCode(SEARCH_OPEN_AP4_CODE00);
	    (void)search_open_ap4_00();
		SDKLoadCode(SEARCH_OPEN_AP4_CODE01);
	    search_open_ap4_01();  
		if(b_back_from_mp3){
			SDKLoadCode(BANKTOAP4_CODE);
			bak_to_ap4();
			//bak_current_index = 0xe6e3;
			//SDKLoadCode(BACK_BAK_DOWITH_CODE);
			//(void)bak_dowith();
			}
      break;

      case Book_Search:
	  SDKLoadCode(READ_BOOK_NEWS_CODE);
      read_book_news(( U16 ) Current_Record.Book_num );
#if 0
#endif	  
      break;

      case Page_Search:
	  SDKLoadCode(READ_PAGE_NEWS_CODE);
      read_page_news(( U16 ) Current_Record.Page_num );

       
//***********************************************
	  SDKLoadCode(READ_PAGE_OTHERCODE);
  	  if(read_page_other_news())return;
      break;

      case Control_Mode_Search:
	  //clear_bigflag();
	  //clear_smallflag();
	  SDKLoadCode(READ_CONTR_NEWS_CODE);
      read_control_mode_news( Current_Record.Control_num );
      if((1!=first_go_ap4)){
      	(void)act_do( News_Control.Addr_Action, 0 );
	  }
	  SDKLoadCode(CONTROL_NEWS_CODE);
	  control_news_dowith();
      break;

      case Normal_Search:
	 
	  SDKLoadCode(READ_SGDB_CODE);
      read_sgdb();
	  if(bInvalidFlag){
		 bInvalidFlag = false;
		 //tishi_mp3(FLAG_INVALIDFILE, false);
		 (void)act_do( News_Go.Action_NoVoice, 0 );
		 }
	  state_search_to_play();
      break;

      case Idle_Search:
	  if( !g_b_player_on )
      {
         if( Point_Read == Point_Write )
         {
            speech_play_end();
			if(State_Search == Open_File_Search)
				return;
         }
         state_search_to_play();
      }
      break;

	  case Wait_Goon_Play:
	  break;

      case Play_Search:
#if 0//def TX_DEBUG
   	    SendString("\r\nPlay_Search");
#endif	  	
//	  scan_oid_enable();
//	  Kbd_set_evt_enabled();
	  state_search_to_idle();
	  if(b_back_from_mp3)
	  	{
			SDKLoadCode(BACK_BAK_DOWITH_CODE);
			if(bak_dowith())break;
	  	}
      if( Point_Write != Point_Read )
      {
//         SendString(( char *)"player_ap3_start\r\n");
//		  SystemClock(FEQ_48M);
         if( player_ap3_start( Array_V[ Point_Read ].Addr_Speech, Array_V[ Point_Read ].Flag_Speech ))
         {
//            bSpeechEnd = FALSE;
            state_search_to_idle();
         }
         Point_Read++;
      }
      else
      {
         speech_play_end();
      }
      break;
	  
#if 0
      case Sequence_Search:
//      if( bSpeechEnd == FALSE )
//         break;
      if( sequence_speech_read_point > Current_Act_Sequence.Speech_End_Address )
      {
         State_Search = Null_state;
         break;
      }
      if( player_ap3_start( sequence_speech_read_point, 0 ))
      {
//         bSpeechEnd = FALSE;
      }
      if( Search_Record.news_sys.repeat_flag )
         break;
      sequence_speech_read_point += 9;
      break;
#endif      
      /*case Err_Search:
      break;

      case Null_state:
      default:
      break;*/
   }
 
#if 1  
   //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
   if(News_Control.Type_Control == GAME_TYPE)
   {
     if((1==game_timer_stop)|| bSkipNext || bSkipPrevious || bTipPlay || bGameTimeOverPlay)
     	{
	      check_game_time();
	      game_timeout_deal();
	  }
   }
#endif   
}


void speech_play_end( void )
{

  if(poweroff_falg)
  	{
  	    OP_CLOSE();
		file_close();
		//SDKLoadCode(POWER_SYSTEM_OFF_CODE);
  		//power_system_off();
  		CommonLoadCode(C_POWEROFF);
  	}
/*  
  if( Search_Record.news_sys.repeat_flag )
    {
       Point_Read = 0;
	   return;
    }
*/
	 if(bRmoteExitAp4)
	 	{
			SDKLoadCode(REMOTE_CHANGEPRO_CODE);
			remote_change_project();
	 	}
  
///*  
#ifdef USE_AUADO  

   if( Search_Record.news_sys.reread_flag )
  // if(g_b_reread)
    {
	 //  swap_code(IMA_CODEC_CODE_FLAG);
	   //SDKLoadCode(AP3_OPEN_FILE_CODE);
	   clkup_time(FALSE);
	   tishi_mp3(FLAG_RECORDSTART);
	    mp3_over_clock = timer_get_time();
	   SDKLoadCode(GENDU_CODEC_CODE);
	   BlueLed_ON();
	   RedLed_ON();
	   (void)WAV_Mode(WAV_REC_MODE);
	   Search_Record.news_sys.reread_flag = FALSE;
	    if(index_flag){
	   	flag_reread = FALSE;
	   	}
	   else{
	   		Point_Read = 0;
	   	}
	   RedLed_OFF();
	   op_start_timer();
	   clkup_time(TRUE);
	   //swap_code(AP3_OPEN_FILE_CODE_FLAG);
//	   SDKLoadCode(AP3_OPEN_FILE_CODE);
//		tishi_mp3(11);
	   return;
    }
	if( flag_reread )
    {

	   //  swap_code(IMA_CODEC_CODE_FLAG);
	   clkup_time(FALSE);
	   SDKLoadCode(GENDU_CODEC_CODE);
	   BlueLed_ON();
	   (void)WAV_Mode(WAV_PLAY_MODE);
	   power_authorize_autopoweroff();
	   op_start_timer();
	   clkup_time(TRUE);
	   return;
    }		
#endif
//*/

	/*
  	if( Search_Record.news_sys.repeat_flag ){
		if(num_repeat<2){
			num_repeat++;
	   		(void)act_do(action_addr_bak, action_flag_bak);
			state_search_to_play();
			return;
			}
		 Search_Record.news_sys.repeat_flag  = FALSE;
  		}
	if(Search_Record.news_sys.sx_play_flag ){
		Current_Index++;
		if(Current_Index > Index_Range.Normal_Index_Max ){
			Current_Index = Index_Range.Normal_Index_Min;
			}
			index_flag = TRUE;
#ifdef TX_DEBUG
		SendString("\r\nmin:max:cur=");
		send_hex_u16(Index_Range.Normal_Index_Min);
		SendString(":");
		send_hex_u16(Index_Range.Normal_Index_Max);
		SendString(":");
		send_hex_u16(Current_Index);
#endif			
			return;
			}
	*/
  if(bRemoteChangeAp3)
  	{
  	   
		SDKLoadCode(PLAYEND_DOWITH_01_CODE);
		if(change_ap4()){
			tishi_mp3(FLAG_NO_FINDAP4,false);
			}
		SDKLoadCode(OPEN_NEWAP4_CODE);
		(void)open_newAp4();
		return;
  	}

    if(1==first_go_ap4)
  	{
  		scan_oid_enable();
		Kbd_set_evt_enabled();
		//Search_Record.news_sys.flag_index = FALSE;
		index_flag = FALSE;
		first_go_ap4 = 0;
		//bRemoteChangeAp3 = FALSE;
  	}
 #if 1
    if(2==game_timer_stop)
  	{
  		game_timer_stop = 1;

	   if( Current_GameNews.Time )
	   {
	      GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
	   }
	      if( Current_GameNews.OverTime_Game && ( Current_GameNews.Fuction_Question != GAME_TYPE_REV ))
	      {
	         GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
	      }
  	}




//  SystemClock(FEQ_7P5M);	
  	if( g_b_autopoweroff_forbidden && (State_Search == Idle_Search))
  		{
#ifdef TX_DEBUG
		SendString("\r\nap4 autoclose!");
#endif  			
  			power_authorize_autopoweroff();
  		}

	if((timer_get_time() >= g_autopoweroff_timer )&& !g_b_autopoweroff_forbidden)
		{
			 
			 SDKLoadCode(PLAYEND_DOWITH_02_CODE);
			 auto_close_dowith();
			 return;
		}


#endif		
//    SystemClock(FEQ_7P5M);
#if 0
//   if( bRemoteChangeAp3 )
//   {
//      bRemoteChangeAp3 = FALSE;
//      mail_send_event( EVT_SEARCH_REMOTE_CHANGE_AP3, 0 );
//   }
//   else if( bRmoteExitAp4 )
//   {
      /*bRmoteExitAp4 = FALSE;
      bInCal = FALSE;*/
//      remote_bits.value &= (U16)(~0x0880);
//      mail_send_event( EVT_SEARCH_END, remote_to_mode );
//   }
   if( Search_Record.news_sys.reread_flag )
   {
      //init_array_v();
      scan_oid_disable();
      /*Search_Record.news_sys.reread_flag = 0;
      Search_Record.news_sys.flag_index = 0;
      Search_Record.news_sys.flag_anjian = 0;*/
      Search_Record.value &= (U16)(~0x0043);
//      mail_send_event( EVT_SEARCH_AP3RECORD, 0 );
   }
   else if( Search_Record.news_sys.play_record_flag )
   {
      Search_Record.news_sys.play_record_flag = 0;
//      mail_send_event( EVT_SEARCH_AP3RECORD_PLAY, 0 );
   }
   else if( Search_Record.news_sys.repeat_flag )
   {
      Point_Read = 0;
   }
   else if( Search_Record.news_sys.poweroff_flag == 2 )
   {
      Search_Record.news_sys.poweroff_flag = 0;
//      mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
   }
#endif   
}




Bool player_ap3_start( U32 start_addr, U8 flag )
{
	clkup_time(TRUE);
	SDKLoadCode(PLAY_AP3DEAL_CODE);
	if(!play_ap3_deal(start_addr, flag))return FALSE;
 	SDKLoadCode(DAC_COMMON_CODE);
	SDKLoadCode(MP3_COMMON_CODE);
   return player_music_start();
}


//the function is read need data from nf
void read_data_from_nf( U32 start_addr, U8 _MEM_TYPE_SLOW_ *store_addr, U16 leng_data )
{
   if( file_seek( start_addr, FS_SEEK_SET ))
   {
      //if( file_read_buf(( U8 _MEM_TYPE_SLOW_ *)store_addr, leng_data )== leng_data )
      (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)store_addr, leng_data );
         //return TRUE;
   }
   //return FALSE;
}


//the function is clear array array-v;
void init_array_v( void )
{
   Point_Read = 0;
   Point_Write = 0;
   //memset( Array_V, 0, Length_v );
}

/*
void repeat_save( void )
{
   repeat_point = Point_Write;
}

*/

void clear_bigflag(void)
{
	//g_b_reread = FALSE;
	//g_b_repeat = FALSE;
	//g_b_sx_play = FALSE;
}
void clear_smallflag(void)
{
  Search_Record.news_sys.repeat_flag = FALSE;
   Search_Record.news_sys.reread_flag = FALSE;
   Search_Record.news_sys.sx_play_flag = FALSE;
}
//#the function is deal with the act;
Bool act_do( U32 address, Bool game_act_do_flag )
{
   //U16 temp;
   if( address == 0 )
      return FALSE;
   //if( player_ap3_is_busy())
    //     player_ap3_stop();
    action_addr_bak = address;
    action_flag_bak = game_act_do_flag;
   SDKLoadCode(ACT_READ_CODE);
   act_read(address);
   SDKLoadCode(VOICE_PLAY_CODE);
   switch( Current_Act.Type )
   {
      case Type_Repaet_Speech:
      if( repeat_point )
      {
         Point_Read = repeat_point - 1;
         Point_Write = repeat_point;
      }
      //break;
      return TRUE;
      
      case Type_Stop:
      //if( player_ap3_is_busy())
      //   player_ap3_stop();
      //break;
      return TRUE;

      case Type_Vol_Up:
      voice_play_number( address );
      case Type_V_Big:
//      srvplayer_volume_change( SRVPLAYER_VOL_UP );
      //break;
      if(vol_idx < (VOL_NUM-1))
		{
		     vol_idx++;
	//		 AudioMP3SetVol(VolTbl[vol_idx]);
		}
      return TRUE;

      case Type_Vol_Down:
      voice_play_number( address );
      case Type_V_Small:
//      srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
      //break;
      if(vol_idx > 0)
		{
		     vol_idx--;
	//		 AudioMP3SetVol(VolTbl[vol_idx]);
		}
      return TRUE;
#if 1
      case Type_Repeat_Game_Question:
      //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
      if( bInGame &&( News_Control.Type_Control == GAME_TYPE ))
      {	
//      	 SendString("\r\nType_Repeat_Game_Question\r\n");
         init_current_game();
         bGameTimeOverPlay = TRUE;
         return TRUE;
      }
      //break;
      if( game_act_do_flag )
         return FALSE;
      else
         return TRUE;

      case Type_Next_Game:
      //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
      if( News_Control.Type_Control == GAME_TYPE )
      {
         bSkipNext = TRUE;
         return TRUE;
      }
      //break;
      if( game_act_do_flag )
         return FALSE;
      else
         return TRUE;

      case Type_Prev_Game:
      //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
      if( News_Control.Type_Control == GAME_TYPE )
      {
         bSkipPrevious = TRUE;
//		 SendString("Type_Prev_Game\r\n");
         return TRUE;
      }
      //break;
      if( game_act_do_flag )
         return FALSE;
      else
         return TRUE;

      case Type_Quit_Game:
      //if( bInGame &&( Search_Record.news_sys.mode_flag == GAME_TABLE ))
      if( bInGame &&( News_Control.Type_Control == GAME_TYPE ))
      {
//      	 SendString("\r\nType_Quit_Game\r\n");
         bTipPlay = TRUE;
         return TRUE;
      }
      //break;
      if( game_act_do_flag )
         return FALSE;
      else
         return TRUE;

//      case Type_Go_Mp3:
//      mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
//      return TRUE;
#endif
      case Type_Play:
      case Type_Play_Title:
      case Type_play_Caption:
      if( game_act_do_flag )
         return FALSE;
//	  if((News_Control.Type_Control==SINGLE_TABLE_TYPE)&&(News_Control.Flag&0x02))
	
	// if(flag_fanyi){
		  if(News_Control.Type_Control==SINGLE_TABLE_TYPE)
		  	{
		  		Addr_Fanyi = address + 5;
				Num_Fanyi = Current_Act.act_info.speech_info.Num_Voice;
				Num_Cur_Fanyi = 1;
		  	}
		 
	 	//}
	 if(address == addr_novoice){
	 	Addr_Fanyi = 0;
		 Num_Fanyi = 0;
	 	}
/*
	 if(flag_gendu > 0x7F){
	 	Search_Record.news_sys.reread_flag = 1;
	 	}
	 else{
	 	Search_Record.news_sys.reread_flag = 0;
	 	}//*/
      voice_play_number( address );
      //break;
      return TRUE;

	  case Type_Script:
//	  swap_code(GAME_Script_CODE_FLAG);
	  SDKLoadCode(GAME_Script_CODE);
	  ScriptCurrentAddress = address+2;
	  ScriptCurrentAddress_bak = ScriptCurrentAddress;
	  script_decode();
	  break;
	  
      case Type_play_Reread:
      if( game_act_do_flag )
         return FALSE;
	  
      Search_Record.news_sys.reread_flag = TRUE;
	  flag_wav_play= FALSE;
	  mp3_begin_clock = timer_get_time();
      voice_play_number( address );
      //break;
      return TRUE;

      case Type_play_Repeat:
     if( game_act_do_flag )
         return FALSE;
      //Search_Record.news_sys.repeat_flag = 1;
      voice_play_number( address );
      //break;
      return TRUE;
/*
      case Type_Play_Sequence:
//      if( game_act_do_flag )
//         return FALSE;
      //Search_Record.news_sys.sequence_flag = 1;
      read_data_from_nf( address + 2, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act_Sequence, ( U16 )sizeof( Action_Sequence ));
      address += sizeof( Action_Sequence );
      read_data_from_nf( address + 2, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act.act_info.speech_info.Num_Voice, 3);
      if( ap3_mode == SINGLE_PLAY_MODE )
      {
         voice_play_number( address );
      }
      else if( ap3_mode == SEQUENCE_PLAY_MODE )
      {
         sequence_speech_address = Current_Act_Sequence.Speech_Start_Address;
         sequence_speech_read_point = sequence_speech_address;
         sequence_text_address = Current_Act_Sequence.Text_Start_Address;
         State_Search = Sequence_Search;
      }
      //break;
      
      return TRUE;
      */
#if 1
	case Type_SelectBook:
	case Type_SelectPage:
	case Type_SelectControl:
		SDKLoadCode(ACTSELECT_CODE);
		actSelect(Current_Act.Type);
		return true;
#else
//***********************************************
      //add by xuanyu
      case Type_SelectBook:

     if( Current_Act.act_info.Num_Change_Mode >= All_Control.Num_Book )
        break;
      Search_Record.news_sys.ap3_aeb_flag = 1;
      Current_Record.Book_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Book_Search;
	  Current_Record.Page_num = 0;
	  Current_Record.Control_num = 0;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
      //break;
      return TRUE;
      
      case Type_SelectPage:
      if( Current_Act.act_info.Num_Change_Mode >= News_Book.Num_GoTable )
         break;
      Search_Record.news_sys.ap3_aeb_flag = 1;
      Current_Record.Page_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Page_Search;
	  Current_Record.Control_num = 0;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
      //break;
      return TRUE;
      
      case Type_SelectControl:
      if( Current_Act.act_info.Num_Change_Mode >= News_Go.Num_ControlTab )
         break;
      Search_Record.news_sys.ap3_aeb_flag = 1;
      Temp_Control_num = Current_Record.Control_num;
      Current_Record.Control_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Control_Mode_Search;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
      //break;
      return TRUE;
//***********************************************
#endif
	  default:
	  return FALSE;
   }
   if( game_act_do_flag )
      return FALSE;
   return TRUE;
}





// record the news of the media 
void write_media_addr( U32 address, U8 flag )
{
   if( Point_Write >=( Length_v - 1 ))
   {
      return;
   }
   Array_V[ Point_Write ].Addr_Speech = address;
   Array_V[ Point_Write ].Flag_Speech = flag;
   Point_Write++;
}

#if 0
Bool fix_index(void){

	if(Current_Index==INDEX_GO_AP4){
//		Search_Record.news_sys.reread_flag = TRUE;
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
		SDKLoadCode(AP3_OPEN_FILE_CODE);
		tishi_mp3(9);
		return TRUE;
		}
	if(Current_Index==INDEX_GO_DIC){
//		Search_Record.news_sys.reread_flag = TRUE;
		bRmoteExitAp4 = TRUE;
        remote_to_mode = DIC_APPLI;
		return TRUE;
		}	
	if(Current_Index==INDEX_GO_AP4){
		bak_to_news();
		file_close();
		CommonLoadCode(C_OID);
		return TRUE;
		}
	if(Current_Index==INDEX_START_RECORD){
//		Search_Record.news_sys.reread_flag = TRUE;
		bRmoteExitAp4 = TRUE;
        remote_to_mode = PASTER_APPLI;
		wav_action = WAV_RECORD_FLAG;
		return TRUE;
		}
	if(Current_Index==INDEX_STOP_RECORD){
//		Search_Record.news_sys.reread_flag = TRUE;
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
		SDKLoadCode(AP3_OPEN_FILE_CODE);
		tishi_mp3(13);
		return TRUE;
		}
	if(Current_Index==INDEX_PLAY_RECORD)
		{
//		flag_wav_play=TRUE;
		bRmoteExitAp4 = TRUE;
        remote_to_mode = PASTER_APPLI;
		wav_action = WAV_PLAY_FLAG;
		return TRUE;
		}
	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX))
		{
//		flag_wav_play=TRUE;
		bRmoteExitAp4 = TRUE;
        remote_to_mode = PASTER_APPLI;
		wav_action = Current_Index;
		return TRUE;
		}	
	if(Current_Index==INDEX_DEL_RECORD)
		{
//		 swap_code(IMA_CODEC_CODE_FLAG);
		 SDKLoadCode(IMA_CODEC_CODE);
	   	 if(WAV_Mode(WAV_DEL_MODE))
	   	 	{
//	   	 		swap_code(AP3_OPEN_FILE_CODE_FLAG);
				SDKLoadCode(AP3_OPEN_FILE_CODE);
				tishi_mp3(12);
	   	 	}
	   	 power_authorize_autopoweroff();
		 return TRUE;
		}
	
		if((Current_Index==INDEX_NEXT_MP3)||(Current_Index==INDEX_PREV_MP3)||(Current_Index==INDEX_PAUSE_PLAY)){
//			swap_code(AP3_OPEN_FILE_CODE_FLAG);
			SDKLoadCode(AP3_OPEN_FILE_CODE);
			tishi_mp3(13);
			return TRUE;
			}
		
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
		{
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 //AudioMP3SetVol(VolTbl[vol_idx]);
		 init_array_v();
		 (void)act_do(All_Control.Addr_Antion_key, 0);	
		 Search_Record.news_sys.reread_flag = 0;
		 return TRUE;
		}
	if((Current_Index==INDEX_INC_VOL))
		{
	   		if(vol_idx < (VOL_NUM-1))
				{
				     vol_idx++;
//					 AudioMP3SetVol(VolTbl[vol_idx]);
				}
		 (void)act_do(All_Control.Addr_Antion_key, 0);
		 Search_Record.news_sys.reread_flag = 0;
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
	 	   if(vol_idx > 0)
			{
			     vol_idx--;
//				 AudioMP3SetVol(VolTbl[vol_idx]);
			}
		 (void)act_do(All_Control.Addr_Antion_key, 0);  
		 Search_Record.news_sys.reread_flag = 0;
         return TRUE;
		}
	if((Current_Index==INDEX_GO_MP3))
		{
		 bRmoteExitAp4 = TRUE;
         remote_to_mode = PLAYER_APPLI;
         return TRUE;
		}
	if(((Current_Index>=INDEX_GO_GAME_MIN)&&(Current_Index<=INDEX_GO_GAME_MAX))||
		((Current_Index>=INDEX_LANG_MIN)&&(Current_Index<=INDEX_LANG_MAX))
		)
		{
		  SDKLoadCode(SEARCH_TWOBYTE_CODE);
		 if( search_twobytes_coordinate( News_Go.Point_PageCoorTable, News_Go.Num_ControlTab ))
         {
            Temp_Control_num = Current_Record.Control_num;
            Current_Record.Control_num = Current_Coor.X0;
            State_Search = Control_Mode_Search;
           return TRUE;
         }	 
		 return FALSE;
		}
/*	
	if((Current_Index>=INDEX_CHANGE_AP4_MIN)&&(Current_Index<=(INDEX_CHANGE_AP4_MIN+RANG_CHANGE_AP4)))
		{
		 
		 RemoteNumber = Current_Index-INDEX_CHANGE_AP4_MIN+1;
         bRemoteChangeAp3 = TRUE;
#ifdef TX_DEBUG
		 SendString("\r\nCurrent_Index:RemoteNumber=");
		 send_hex_u16(Current_Index);
		 SendString(":");
		 send_hex_u16(RemoteNumber);
#endif		 
		 return TRUE;
		}
//*/	
	return FALSE;
}
Bool nostop_index(void)
{
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_LAST_VOL)))
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
		//		 AudioMP3SetVol(VolTbl[vol_idx]);
			}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
		 if(vol_idx > 0)
			{
			     vol_idx--;
		//		 AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}
	  return FALSE;
}
#endif
U16 rand( void )
{
	Seed = Seed * 1103515245 + 12345;
	return ( U16 )( Seed / 65536 )% 32768;
}

void srand( U32 s )
{
	Seed = s;
}

void readyGoNewPorject(unsigned int subProject)
{
	prev_task = C_OID;
	CommonLoadCode(subProject);
	
}

#pragma CODE = PLAY_AP3DEAL_CODE
Bool play_ap3_deal(U32 addr, U8 flag)
{
   U8 codec;
   ENCRYPT_DATA ap4_audio_var;
   //if( player_ap3_is_busy())
  // {
   //   return FALSE;
   //}
   if( !file_seek( addr, FS_SEEK_SET ))
   {
      return FALSE;
   }
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&mp3_begin_add, sizeof( mp3_begin_add));

#if 0  
   SendString("start_addr:");
   send_hex_u32(start_addr);
   SendString("\r\n");
   SendString("file_size_count:");
   send_hex_u32(file_size_count);
   SendString("\r\n");
#endif   
   if( !mp3_begin_add )
      return FALSE;

 (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&play_file_size, sizeof( play_file_size));   

#if 0   
   SendString(( char *)"mp3_begin_addr=");
   send_hex_u32(file_size_count);
   SendString(( char *)"\r\n");
   SendString(( char *)"play_file_size=");
   send_hex_u32(play_file_size);
   SendString(( char *)"\r\n");
#endif

   if( play_file_size < FS_512B*4 )
      return FALSE;
#if 0
   codec = ( U8 )file_getc();
   /*if( codec )
      return FALSE;*/
   switch( codec )
   {
      case 0:
//      varAl.WAVE_Flg = 0;
      break;

      case 2:
//      varAl.WAVE_Flg = 1;
      break;

      default:
      return FALSE;
   }
#endif   

   ap4_audio_var.audio_var.begin_addr= mp3_begin_add;
   ap4_audio_var.audio_var.file_size = play_file_size;

   if( flag == 1 )
   {  
   	  if(( RemoteVersion >= 4 )
         &&( RemoteFlag & ENCRYPT_SPEECH_FLAG ))
      {
         if( RemoteEncryptKey > 0x07 )
            return FALSE;
         //codec = ap4_audio_var.encrypt[ Encrypt_Key ];
		 codec = ap4_audio_var.encrypt[ RemoteEncryptKey ];
		// MSB0( Key_encrypt_mp3 ) = codec;
   		// MSB1( Key_encrypt_mp3 ) = codec;
   		// MSB2( Key_encrypt_mp3 ) = codec;
   		// MSB3( Key_encrypt_mp3 ) = codec;
   		gOidXOR_Key = codec;
      }
      else
      {
         //Key_encrypt_mp3 = 0;
         gOidXOR_Key = 0;

      }
		mp3_begin_add += RemoteBaseAddress;
   }
  
   else
   {
      if(( All_Control.Version >= 6 )
         &&( All_Control.Mask_Gloablext & ENCRYPT_AUDIO_DATA_FLAG ))
      {
         if( Encrypt_Key > 0x07 )
            return FALSE;
		 
         codec = ap4_audio_var.encrypt[ Encrypt_Key ];
		// MSB0( Key_encrypt_mp3 ) = codec;
   		// MSB1( Key_encrypt_mp3 ) = codec;
   		// MSB2( Key_encrypt_mp3 ) = codec;
   		// MSB3( Key_encrypt_mp3 ) = codec;
   		gOidXOR_Key = codec;
//         //LROM_EXCHANGE = 0x00;
//         wav_encrypt = TRUE;
      }
      else
      {
      	   gOidXOR_Key = 0;
//         LROM_SCRAMBLE = 0x00;
//         //LROM_EXCHANGE = 0x00;
 //        wav_encrypt = FALSE;
      }
   }
   
 #if 0//def TX_DEBUG
   SendString("\r\nmp3_begin_add:play_file_size:key");
   send_hex_u32(mp3_begin_add); SendString(":");
   send_hex_u32(play_file_size); SendString(":");
   send_hex_u8(gOidXOR_Key);
#endif  

	if(mp3_begin_add>=size_current_ap4)return FALSE;
#if 0   
   SendString(( char *)"play_file_size_3:");
   send_hex_u32(play_file_size);
   SendString(( char *)"\r\n");
#endif
	OID_PLAY = true;
	g_b_play_afx = false;
	lastSpeech.Addr_Speech = addr;
	lastSpeech.Flag_Speech = flag;
	return TRUE;
}

#pragma CODE = ACT_READ_CODE
void act_read(U32 addr)
{
   unsigned char g_buf[128];
   unsigned char t=0;
   U8 i;
   clear_smallflag();
	Addr_Fanyi = 0;
	Num_Fanyi = 0;

   if(g_b_reread){
   	mp3_begin_clock = timer_get_time();
	Search_Record.news_sys.reread_flag = TRUE;
   	}
   if(g_b_repeat){
   	 Search_Record.news_sys.repeat_flag = TRUE;
   	}
   if(g_b_sx_play){
   	Search_Record.news_sys.sx_play_flag = TRUE;
   	}
//   read_data_from_nf( address, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act sizeof(U16));
   read_data_from_nf( addr, ( U8 _MEM_TYPE_SLOW_ *)&g_buf, sizeof(U8)*5);
//   BS(All_Control.Version,0)	= g_buf[buf_offset++];
   BS(Current_Act.Type,0) = g_buf[t++];
   BS(Current_Act.Type,1) = g_buf[t++];
   BS(Current_Act.act_info.speech_info.Num_Voice,0) = g_buf[t++];
   BS(Current_Act.act_info.speech_info.Mode_Play,0) = g_buf[t++];
   BS(Current_Act.Num_Play,0) = g_buf[t++];
	if(prev_index != Current_Index){
		prev_index = Current_Index;
		remember_num = Current_Act.act_info.speech_info.Num_Voice;
		}
   //if(!( Current_Act.act_info.speech_info.Mode_Play == Remember_Play ))
 //  Current_Act.Type = UIntLE2BE(Current_Act.Type);
//   Endian_Cast(Current_Act.act_info.Num_Change_Mode);
   //MSB( temp ) = Current_Act.Num_Voice;
   //LSB( temp ) = Current_Act.Mode_Play;
#ifdef TX_DEBUG
 SendString("\r\nADDR:Current_Act=");
 send_hex_u32(addr);
  SendString(":");

	for(i=0;i<6;i++){
		send_hex_u8(g_buf[i]);
		SendString(":");
		}
#endif	
}

#pragma CODE = ACTSELECT_CODE
void actSelect(U16 data){
	if(data == Type_SelectBook){
     if( Current_Act.act_info.Num_Change_Mode >= All_Control.Num_Book )return;
      //Search_Record.news_sys.ap3_aeb_flag = 1;
      Current_Record.Book_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Book_Search;
	  Current_Record.Page_num = 0;
	  Current_Record.Control_num = 0;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
      //break;
	}
      
	if(data == Type_SelectPage){  	
      if( Current_Act.act_info.Num_Change_Mode >= News_Book.Num_GoTable )return;
      //Search_Record.news_sys.ap3_aeb_flag = 1;
      Current_Record.Page_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Page_Search;
	  Current_Record.Control_num = 0;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
      //break;
	}
      
	if(data == Type_SelectControl){    	
      if( Current_Act.act_info.Num_Change_Mode >= News_Go.Num_ControlTab )return;
     // Search_Record.news_sys.ap3_aeb_flag = 1;
      Temp_Control_num = Current_Record.Control_num;
      Current_Record.Control_num = Current_Act.act_info.Num_Change_Mode;
      State_Search = Control_Mode_Search;
//	  ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
//	  ap4_mp3_flag = 0;
//      ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
      //break;
	}
}

#pragma CODE = VOICE_PLAY_CODE
//! This fonction initialize the random table
//!
void ap3_rand_init( void )
{
   // Init rand table
   memset( g_ap3_rand_tab , 0, AP3_MAX_RANGE_RAND );
}

Bool ap3_rand_table_is_full( U8 number )
{
   //U8 _MEM_TYPE_SLOW_ i;
   //U8 _MEM_TYPE_SLOW_ number;
   if( Current_Act.act_info.speech_info.Num_Voice >( AP3_MAX_RANGE_RAND * 8 ))
      number = AP3_MAX_RANGE_RAND * 8;
   //else
      //number = Current_Act.act_info.speech_info.Num_Voice;
   //for( i = 0; i < number; i++ )
   while( number )
   {
      number--;
      if( 0 == ( g_ap3_rand_tab[ number / 8 ]&( 1 << ( number % 8 ))))
      {
         return FALSE;
      }
   }
   return TRUE;
}

void ready_record(U32 addr,U8 num)
{
	U16 _MEM_TYPE_SLOW_ text_length;
	 U32 _MEM_TYPE_SLOW_ Iaddress;
	 
	 Iaddress = addr + ( U32 )5;
 	 switch( Current_Act.Type )
      {
         case Type_Play:
         case Type_play_Reread:
         case Type_play_Repeat:
         case Type_Play_Sequence:
         Iaddress += ( U32 )num * 9;
         break;

         case Type_Play_Title:
         case Type_play_Caption:
         read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&text_length, ( U16 )sizeof( U16 ));
         switch( All_Control.Version )
         {
            case 2:
            Iaddress += ( U32 )text_length + 2 +( U32 )num * 9;
            break;
         }
         break;
      }
      write_media_addr( Iaddress, 0 );
}

void random_ready(U32 address)
{
  U8 _MEM_TYPE_SLOW_ i, j, m;
   ap3_rand_init();
   srand( timer_get_time());
   for( i = 0; i < Current_Act.Num_Play; i++ )
   {
	 
     if( i >= Current_Act.act_info.speech_info.Num_Voice )
     	{
        	ap3_rand_init();
     	}
         //srand( timer_get_time());
     while( 1 )
     {
        if( ap3_rand_table_is_full( Current_Act.act_info.speech_info.Num_Voice ))
        	{
           		ap3_rand_init();
        	}
        m = rand() % Current_Act.act_info.speech_info.Num_Voice;
        j = m %( AP3_MAX_RANGE_RAND * 8 );
        if( 0 == ( g_ap3_rand_tab[ j / 8 ]&( 1 << ( j % 8 ))))
        {
           g_ap3_rand_tab[ j / 8 ] |= 1 << ( j % 8 ); // Update table
           break;   // A no used position find
        }
     }
	ready_record(address,m);
   
   }
}

void ordea_ready(U32 address)
{
  U8 _MEM_TYPE_SLOW_ i, j, m;
   for( i = 0; i < Current_Act.Num_Play; i++ )
   {
       m = i;
        if( m >= Current_Act.act_info.speech_info.Num_Voice ){
            	m %= Current_Act.act_info.speech_info.Num_Voice;
        	}
	ready_record(address,m);	
   }
}

void remember_ready(U32 address)
{
   remember_num++;
	if( remember_num >= Current_Act.act_info.speech_info.Num_Voice ){
	    	//remember_num %= Current_Act.act_info.speech_info.Num_Voice;
	    	remember_num = 0;
		}
	ready_record(address,remember_num);	

}

//the function is deal with the action 
void voice_play_number( U32 address )
{
   U8 _MEM_TYPE_SLOW_ i, j, m;
   U32 _MEM_TYPE_SLOW_ Iaddress;
   

   if( !Current_Act.act_info.speech_info.Num_Voice )
      return;

	 if( Current_Act.act_info.speech_info.Mode_Play == Random_Play )
      {
		random_ready(address);
      }
      else if( Current_Act.act_info.speech_info.Mode_Play == Order_Play )
      {
		ordea_ready(address);
      }
	  else if( Current_Act.act_info.speech_info.Mode_Play == Remember_Play)
      {
		remember_ready(address);
      }
	  else if( Current_Act.act_info.speech_info.Mode_Play == Linkwithmem_Play )
      {
		ordea_ready(address);
      } 
      else
         return;

}


#pragma CODE = SEARCH_TWOBYTE_CODE
// the function is quickly search.
Bool search_twobytes_coordinate( U32 addr, U16 number )
{
   U16 _MEM_TYPE_SLOW_ nStart, nMiddle,coor/* nEnd,, Step*/;
   U32 _MEM_TYPE_SLOW_ iAddress,temp_index;
#undef nEnd
#define nEnd   number
#ifdef TX_DEBUG
	SendString("\r\ntwobytes=");
	send_hex_u32( addr);
	SendString(":");
	send_hex_u16(number);
#endif	
   if( !addr || !number )
      return FALSE;
   nStart = 0;
   nEnd = number - 1;
   while( nStart <= nEnd )
   { 
      nMiddle = ( nStart + nEnd )/ 2;
      //iAddress = addr + sizeof( U16 ) * nMiddle;
      //read_data_from_nf( iAddress, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( U16 ));
  if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
      {
         iAddress = addr + sizeof( U16 ) * nMiddle;
         read_data_from_nf( iAddress, ( U8 _MEM_TYPE_SLOW_ *)&coor, sizeof( U16 ));
         temp_index = ( U32 )coor;
      }
      else
      {
         iAddress = addr + sizeof( U32 ) * nMiddle;
         read_data_from_nf( iAddress, ( U8 _MEM_TYPE_SLOW_ *)&temp_index, sizeof( U32 ));
      }
	  
      if( Current_Index == temp_index ) 
      {
         //i = iAddress - *addr;
         //Current_Coor.X0 = ( U16 )( i / sizeof( U16 ));
         Current_Coor.X0 = nMiddle;
         return TRUE;
      }
      else if( Current_Index < temp_index )
      {
         if( nMiddle == 0 )
         {
            return FALSE;
         }
         nEnd = nMiddle - 1;
      }
      else
      {
         nStart = nMiddle + 1;
      }
   }
   return FALSE;
#undef nEnd
}

#pragma CODE = READ_PAGE_OTHERCODE
Bool read_page_other_news(void)
{
#ifdef TX_DEBUG
	SendString("\r\nPage other News!");
#endif
   if((b_back_from_mp3))
 	{
 	   if((b_current_control_game))
 	   	{
		  Search_Record.news_sys.table_flag = DEFAULT_TABLE;
		  scan_oid_enable();
          Kbd_set_evt_enabled();
		  state_search_to_play();
 	   	}
	   else
	   	{
	   	 scan_oid_enable();
  		Kbd_set_evt_enabled();
		SDKLoadCode(READ_CONTR_NEWS_CODE);
	   	read_control_mode_news( Current_Record.Control_num );
	   	Search_Record.news_sys.table_flag = CONTROL_TABLE;
//            Ap3_MenuPath = CONTROL_MENU_PATH;
        State_Search = Control_Mode_Search;
	   	}
		return TRUE;
 	}
  if( News_Go.Point_ControlTab && News_Go.Num_ControlTab )
  //test auto goto first control mode
  {
  	 SDKLoadCode(READ_CONTR_NEWS_CODE);
     for( Current_Record.Control_num = 0; Current_Record.Control_num < News_Go.Num_ControlTab; Current_Record.Control_num++ )
     {
     	
        read_control_mode_news( Current_Record.Control_num );
        if(( News_Control.Type_Control == SINGLE_TABLE_TYPE )
           && News_Control.Point )
        {
           Search_Record.news_sys.table_flag = CONTROL_TABLE;
           State_Search = Control_Mode_Search;
		   b_current_control_game = FALSE;
           Temp_Control_num = Current_Record.Control_num;
		   goto goto_play;
        }
     }
     Current_Record.Control_num = 0;
  }
//***********************************************
	News_Control.Type_Control = NO_TYPE;
	b_current_control_game = TRUE;
 	Search_Record.news_sys.table_flag = DEFAULT_TABLE;
#ifdef TX_DEBUG
	SendString("\r\nall ctrl is game!");
#endif
  //if( !( News_Go.Point_ControlTab && News_Go.Num_ControlTab ))
    //ap4_mp3_flag |= CURRENT_CONTROL_GAME;
     if( News_Go.Point_DefaultTab )
     {
#ifdef TX_DEBUG
	SendString("\r\nDEFAULT_TABLE");
#endif           
           Search_Record.news_sys.table_flag = DEFAULT_TABLE;
           goto goto_play;
     }

     if( News_Go.Point_BackgroudTab )
     {

#ifdef TX_DEBUG
	SendString("\r\nBACKGROUND_TABLE");
#endif            
          Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
		  goto goto_play;

     }
	 (void)act_do( News_Go.Action_NoVoice, 0 );
      Search_Record.news_sys.table_flag = 0;
      state_search_to_play();
	 return TRUE;
goto_play:	 
	 state_search_to_play();
	 scan_oid_enable();
  	 Kbd_set_evt_enabled();
	return TRUE;
}

#pragma CODE = ENCRY_NEWS_CODE
Bool encry_news(void)
{
	U8	 all_news_key[4] = {'D','F','B','K'};
	U32  keyword;
#ifdef TX_DEBUG
	SendString("\r\nencry news!");
#endif
#if 1
   MSB0( keyword ) = all_news_key[ 0 ];
   MSB1( keyword ) = all_news_key[ 1 ];
   MSB2( keyword ) = all_news_key[ 2 ];
   MSB3( keyword ) = all_news_key[ 3 ];

   if( MSB0( All_Control.Point_Book_Table )!= all_news_key[ 0 ] )return FALSE;

     
   All_Control.Mask_Gloablext ^= ( U16 )keyword;
   
   All_Control.Point_Remote ^= keyword;
   All_Control.Flag_Xuanshuye ^= ( U8 )keyword;
   All_Control.Action_Xuanshu ^= keyword;
   All_Control.Point_Book_Table ^= keyword;
   All_Control.Num_Book ^= ( U16 )keyword;
   All_Control.Point_Gloablext_Coor ^= keyword;
   All_Control.Addr_Action_Open ^= keyword;
   All_Control.Addr_Action_close ^= keyword;
   All_Control.Addr_Antion_key ^= keyword;
   All_Control.Point_Special_Action ^= keyword;

   Encrypt_Key ^= ( U8 )keyword;

   //if( All_Control.Version < 15 )
   if(b_index_four_byte)
   {
      LSW( All_Control.Rang_X )^= ( U16 )keyword;
      LSW( All_Control.Rang_Y )^= ( U16 )keyword;
      LSW( All_Control.Excursion_Code )^= ( U16 )keyword;
      LSW( All_Control.Max_Code )^= ( U16 )keyword;
      
      LSW( Ap4_Record_Range.Record_Index_Min )^= ( U16 )keyword;
      LSW( Ap4_Record_Range.Record_Index_Max )^= ( U16 )keyword;

      LSW( Index_Range.Normal_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Normal_Index_Max )^= ( U16 )keyword;
      LSW( Index_Range.Book_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Book_Index_Max )^= ( U16 )keyword;
      LSW( Index_Range.Page_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Page_Index_Max )^= ( U16 )keyword;
      LSW( Index_Range.Control_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Control_Index_Max )^= ( U16 )keyword;
      LSW( Index_Range.Remote_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Remote_Index_Max )^= ( U16 )keyword;
      LSW( Index_Range.Resv_Index_Min )^= ( U16 )keyword;
      LSW( Index_Range.Resv_Index_Max )^= ( U16 )keyword;

      LSW( Ap4_Record_Range.Play_Index_Min )^= ( U16 )keyword;
      LSW( Ap4_Record_Range.Play_Index_Max )^= ( U16 )keyword;
      
      if( All_Control.Version >= 9 )
      {
         LSW( Ap4_Record_Range_2.Record_Index_Min )^= ( U16 )keyword;
         LSW( Ap4_Record_Range_2.Record_Index_Max )^= ( U16 )keyword;
         LSW( Ap4_Record_Range_2.Play_Index_Min )^= ( U16 )keyword;
         LSW( Ap4_Record_Range_2.Play_Index_Max )^= ( U16 )keyword;
      }

      //speical
      LSW( Current_Other_Range.Ap3_Min)^= ( U16 )keyword;
      LSW( Current_Other_Range.Ap3_Max)^= ( U16 )keyword;
      LSW( Current_Other_Range.Guding_Min)^= ( U16 )keyword;
      LSW( Current_Other_Range.Guding_Max)^= ( U16 )keyword;
   }
   else
   {
      All_Control.Rang_X ^= keyword;
      All_Control.Rang_Y ^= keyword;
      All_Control.Excursion_Code ^= keyword;
      All_Control.Max_Code ^= keyword;
      
      Ap4_Record_Range.Record_Index_Min ^= keyword;
      Ap4_Record_Range.Record_Index_Max ^= keyword;

      Index_Range.Normal_Index_Min ^= keyword;
      Index_Range.Normal_Index_Max ^= keyword;
      Index_Range.Book_Index_Min ^= keyword;
      Index_Range.Book_Index_Max ^= keyword;
      Index_Range.Page_Index_Min ^= keyword;
      Index_Range.Page_Index_Max ^= keyword;
      Index_Range.Control_Index_Min ^= keyword;
      Index_Range.Control_Index_Max ^= keyword;
      Index_Range.Remote_Index_Min ^= keyword;
      Index_Range.Remote_Index_Max ^= keyword;
      Index_Range.Resv_Index_Min ^= keyword;
      Index_Range.Resv_Index_Max ^= keyword;

      Ap4_Record_Range.Play_Index_Min ^= keyword;
      Ap4_Record_Range.Play_Index_Max ^= keyword;
      
      Ap4_Record_Range_2.Record_Index_Min ^= keyword;
      Ap4_Record_Range_2.Record_Index_Max ^= keyword;
      Ap4_Record_Range_2.Play_Index_Min ^= keyword;
      Ap4_Record_Range_2.Play_Index_Max ^= keyword;

      //speical
      Current_Other_Range.Ap3_Min ^= keyword;
      Current_Other_Range.Ap3_Max^= keyword;
      Current_Other_Range.Guding_Min ^= keyword;
      Current_Other_Range.Guding_Max ^= keyword;
   }
	Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
	RemoteBaseAddress = All_Control.Point_Remote;
	return TRUE;
	
#else
	return TRUE;
#endif	
}

#pragma CODE = READ_ALL_NEWS_CODE

#if 0//def 	ALL_NEWS_KEY

U8	U8_key(U8 num)
{
	//return (num^all_news_key[3]);
	return (num^(U8)all_news_key);
}

U16	U16_key(U16 num)
{
	U16 temp;
//	MSB(temp) = MSB(num)^all_news_key[2];
//	LSB(temp) = LSB(num)^all_news_key[3];
	temp = num^(U16)all_news_key;
	return temp;
}

U32	U32_key(U32 num)
{
	U32 temp;
//	MSB0(temp) = MSB0(num)^all_news_key[0];
//	MSB1(temp) = MSB1(num)^all_news_key[1];
//	MSB2(temp) = MSB2(num)^all_news_key[2];
//	MSB3(temp) = MSB3(num)^all_news_key[3];
	temp = num ^(U32)all_news_key;
	return temp;
}

#endif


#if 1

void read_all_news(void)
{
//	U8	 all_news_key[ 4 ] = {'H','Y','N','W'};
	U32  keyword;
	U32  offaddr = 0;
	//U16 *u32_point;
	U32 *temp;
    U16 m, i;
    U8 *p;
	Encrypt_Key = 0xFF;
#ifdef TX_DEBUG
		SendString("\r\nopenfile ok!");
#endif
#ifdef TX_DEBUG
	SendString("\r\nall_news!");
#endif
   b_index_four_byte = false;
   Search_Record.value = 0;
   News_Control.Type_Control = NO_TYPE;
 // (void)file_seek( 0, FS_SEEK_SET );
//  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control, sizeof(U16)*2);

  	//(void)file_seek( 0, FS_SEEK_SET );
 	//(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Buf, sizeof(U8)*512);
	memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Version,&Buf[0],4);
	
 // (void)file_seek( 0, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control, sizeof(U16)*2);

 	if(( All_Control.Version & MSK_EXT_INDEX )!= 0 ){
#ifdef TX_DEBUG
	SendString("\r\nindex leng=4");
#endif 
	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Rang_X,&Buf[4],21);
	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu,&Buf[25],30);

	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Current_Other_Range.Ap3_Min,&Buf[59],sizeof(Current_Other_Range));
	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Min,&Buf[75],8);
	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Normal_Index_Min,&Buf[83],48);
	   
	   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Min,&Buf[131],8);

	   Encrypt_Key = Buf[139];

       memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range_2, &Buf[ 146 ], sizeof( Record_Range ));
		
 		}
	else{
#ifdef TX_DEBUG
	SendString("\r\nindex leng=2");
#endif 		
		
	  temp = ( U32 *)&All_Control.Rang_X;
      p = ( U8 *)&Buf[ 4 ];
      for( i = 0; i < 4; i++ )
      {
         MSB( m ) = *p++;
         LSB( m ) = *p++;
         *temp++ = m;
      }
      /*memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Rang_X, &all_control_news[ 4 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Rang_Y, &all_control_news[ 6 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Excursion_Code, &all_control_news[ 8 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Max_Code, &all_control_news[ 10 ], sizeof( U16 ));
      */
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Point_Remote, &Buf[ 12 ], 5 );

      memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, &Buf[ 17 ], 30 );

      //speical
      temp = ( U32 *)&Current_Other_Range.Ap3_Min;
      p = ( U8 *)&Buf[ 51 ];
      for( i = 0; i < 4; i++ )
      {
         MSB( m ) = *p++;
         LSB( m ) = *p++;
         *temp++ = m;
      }
      /*memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap3_Mobile_Range.Ap3_Index_Min, &all_control_news[ 51 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap3_Mobile_Range.Ap3_Index_Max, &all_control_news[ 53 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap3_Mobile_Range.Mobile_Coor_Min, &all_control_news[ 55 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap3_Mobile_Range.Mobile_Coor_Max, &all_control_news[ 57 ], sizeof( U16 ));
      */
      temp = ( U32 *)&Ap4_Record_Range.Record_Index_Min;
      //p = ( U8 *)&all_control_news[ 59 ];
      for( i = 0; i < 2; i++ )
      {
         MSB( m ) = *p++;
         LSB( m ) = *p++;
         *temp++ = m;
      }
      /*memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Min, &all_control_news[ 59 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Max, &all_control_news[ 61 ], sizeof( U16 ));
      */
      temp = ( U32 *)&Index_Range.Normal_Index_Min;
      //p = ( U8 *)&all_control_news[ 63 ];
      for( i = 0; i < 12; i++ )
      {
         MSB( m ) = *p++;
         LSB( m ) = *p++;
         *temp++ = m;
      }
      /*memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Normal_Index_Min, &all_control_news[ 63 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Normal_Index_Max, &all_control_news[ 65 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Book_Index_Min, &all_control_news[ 67 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Book_Index_Max, &all_control_news[ 69 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Page_Index_Min, &all_control_news[ 71 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Page_Index_Max, &all_control_news[ 73 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Control_Index_Min, &all_control_news[ 75 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Control_Index_Max, &all_control_news[ 77 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Remote_Index_Min, &all_control_news[ 79 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range.Remote_Index_Max, &all_control_news[ 81 ], sizeof( U16 ));
      */
      temp = ( U32 *)&Ap4_Record_Range.Play_Index_Min;
      p = ( U8 *)&Buf[ 87 ];
      for( i = 0; i < 2; i++ )
      {
         MSB( m ) = *p++;
         LSB( m ) = *p++;
         *temp++ = m;
      }
      /*memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Min, &all_control_news[ 87 ], sizeof( U16 ));
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Max, &all_control_news[ 89 ], sizeof( U16 ));
      */
      if( All_Control.Version >= 6 )
      {
         Encrypt_Key = Buf[ 91 ];
      }

      if( All_Control.Version >= 9 )
      {
         temp = ( U32 *)&Ap4_Record_Range_2.Record_Index_Min;
         p = ( U8 *)&Buf[ 98 ];
         for( i = 0; i < 4; i++ )
         {
            MSB( m ) = *p++;
            LSB( m ) = *p++;
            *temp++ = m;
         }

      }
	
		
    }



	
 

 // (void)file_seek( 16, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Flag_Xuanshuye, sizeof(U8));
	 

 // (void)file_seek( 17, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, sizeof(U16)*15);
	  

 // (void)file_seek( 59, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&(Ap4_Record_Range.Record_Index_Min), sizeof(U16)*2);

//  (void)file_seek( 51, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_Other_Range,sizeof(U16)*4);

 // (void)file_seek( 63, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Index_Range, sizeof( News_Index_Range ));


 // (void)file_seek( 87, FS_SEEK_SET );
 // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&(Ap4_Record_Range.Play_Index_Min), sizeof(U16)*2);

  // (void)file_seek( 83, FS_SEEK_SET );
  //(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Current_Other_Range.Rev_Min,sizeof(U16)*2);

  

 // if( All_Control.Version >= 6 )
 // {
     //(void)file_seek( 91, FS_SEEK_SET );
     //(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Encrypt_Key, sizeof( U8 ));
//  }
  //else
 // {
  	
  //}
  /*
#ifdef TX_DEBUG
	SendString("\r\nrecord_min:max:play_min:max=");
	send_hex_u16(Ap4_Record_Range.Record_Index_Min);SendString(":");
	send_hex_u16(Ap4_Record_Range.Record_Index_Max);SendString(":");
	send_hex_u16(Ap4_Record_Range.Play_Index_Min);SendString(":");
	send_hex_u16(Ap4_Record_Range.Play_Index_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nap3_min:max:guding_min:max:rev_min:max=");
	send_hex_u16(Current_Other_Range.Ap3_Min);SendString(":");
	send_hex_u16(Current_Other_Range.Ap3_Max);SendString(":");
	send_hex_u16(Current_Other_Range.Guding_Min);SendString(":");
	send_hex_u16(Current_Other_Range.Guding_Max);SendString(":");
	send_hex_u16(Current_Other_Range.Rev_Min);SendString(":");
	send_hex_u16(Current_Other_Range.Rev_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nap4_key=");
	send_hex_u8(Encrypt_Key);
#endif
*/

/*
   MSB0( keyword ) = all_news_key[ 0 ];
   MSB1( keyword ) = all_news_key[ 1 ];
   MSB2( keyword ) = all_news_key[ 2 ];
   MSB3( keyword ) = all_news_key[ 3 ];

   All_Control.Mask_Gloablext ^= ( U16 )keyword;
   All_Control.Rang_X ^= ( U16 )keyword;
   All_Control.Rang_Y ^= ( U16 )keyword;
   All_Control.Excursion_Code ^= ( U16 )keyword;
   All_Control.Max_Code ^= ( U16 )keyword;
   All_Control.Point_Remote ^= keyword;
   All_Control.Flag_Xuanshuye ^= ( U8 )keyword;
   All_Control.Action_Xuanshu ^= keyword;
   All_Control.Point_Book_Table ^= keyword;
   All_Control.Num_Book ^= ( U16 )keyword;
   All_Control.Point_Gloablext_Coor ^= keyword;
   All_Control.Addr_Action_Open ^= keyword;
   All_Control.Addr_Action_close ^= keyword;
   All_Control.Addr_Antion_key ^= keyword;
   All_Control.Point_Special_Action ^= keyword;

   Ap4_Record_Range.Record_Index_Min ^= ( U16 )keyword;
   Ap4_Record_Range.Record_Index_Max ^= ( U16 )keyword;

   Index_Range.Normal_Index_Min ^= ( U16 )keyword;
   Index_Range.Normal_Index_Max ^= ( U16 )keyword;
   Index_Range.Book_Index_Min ^= ( U16 )keyword;
   Index_Range.Book_Index_Max ^= ( U16 )keyword;
   Index_Range.Page_Index_Min ^= ( U16 )keyword;
   Index_Range.Page_Index_Max ^= ( U16 )keyword;
   Index_Range.Control_Index_Min ^= ( U16 )keyword;
   Index_Range.Control_Index_Max ^= ( U16 )keyword;
   Index_Range.Remote_Index_Min ^= ( U16 )keyword;
   Index_Range.Remote_Index_Max ^= ( U16 )keyword;

   Ap4_Record_Range.Play_Index_Min ^= ( U16 )keyword;
   Ap4_Record_Range.Play_Index_Max ^= ( U16 )keyword;

   
   
   if( All_Control.Version >= 6 )
   {
      Encrypt_Key ^= ( U8 )keyword;
   }


   
  // ap4_AllControl_MaskGlobal = All_Control.Mask_Gloablext;
//*/
	Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
	RemoteBaseAddress = All_Control.Point_Remote;
}

#else
void read_all_news(void)
{
      file_seek( 0, FS_SEEK_SET );
	  file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control, sizeof(U16)*8);


	  file_seek( 16, FS_SEEK_SET );
	  file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Flag_Xuanshuye, sizeof(U8));
 

	  file_seek( 17, FS_SEEK_SET );
	  file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, sizeof(U16)*15);
  
	  RemoteBaseAddress = All_Control.Point_Remote;

      file_seek( 59, FS_SEEK_SET );
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&(Ap4_Record_Range.Record_Index_Min), sizeof(U16)*2);


      file_seek( 63, FS_SEEK_SET );
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Index_Range, sizeof( News_Index_Range ));


	  file_seek( 87, FS_SEEK_SET );
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&(Ap4_Record_Range.Play_Index_Min), sizeof(U16)*2);
  

	  if( All_Control.Version >= 6 )
      {
         file_seek( 91, FS_SEEK_SET );
         file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Encrypt_Key, sizeof( U8 ));
      }
	  else
	  {
	  	Encrypt_Key = 0xFF;
	  }
}
#endif
#pragma CODE = PRINT_NEWS_AP4_CODE
void print_news_ap4(void)
{
#ifdef TX_DEBUG
	SendString("\r\nversion=");
	send_hex_u16(All_Control.Version);
	SendString("\r\nremote_point=");
	send_hex_u32(All_Control.Point_Remote);//SendString(":");
	//send_hex_u32(Current_Other_Range.Ap3_Max);SendString(":");
	//send_hex_u32(Current_Other_Range.Guding_Min);SendString(":");
	//send_hex_u32(Current_Other_Range.Guding_Max);SendString(":");
	//send_hex_u16(Current_Other_Range.Rev_Min);SendString(":");
	//send_hex_u16(Current_Other_Range.Rev_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nrecord_min:max:play_min:max=");
	send_hex_u32(Ap4_Record_Range.Record_Index_Min);SendString(":");
	send_hex_u32(Ap4_Record_Range.Record_Index_Max);SendString(":");
	send_hex_u32(Ap4_Record_Range.Play_Index_Min);SendString(":");
	send_hex_u32(Ap4_Record_Range.Play_Index_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nap3_min:max:guding_min:max:rev_min:max=");
	send_hex_u32(Current_Other_Range.Ap3_Min);SendString(":");
	send_hex_u32(Current_Other_Range.Ap3_Max);SendString(":");
	send_hex_u32(Current_Other_Range.Guding_Min);SendString(":");
	send_hex_u32(Current_Other_Range.Guding_Max);SendString(":");
	//send_hex_u16(Current_Other_Range.Rev_Min);SendString(":");
	//send_hex_u16(Current_Other_Range.Rev_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nNormal_Index=");send_hex_u32(Index_Range.Normal_Index_Min);SendString(":");send_hex_u32(Index_Range.Normal_Index_Max);
	SendString("\r\nBook_Index=");send_hex_u32(Index_Range.Book_Index_Min);SendString(":");send_hex_u32(Index_Range.Book_Index_Max);
	SendString("\r\nPage_Index=");send_hex_u32(Index_Range.Page_Index_Min);SendString(":");send_hex_u32(Index_Range.Page_Index_Max);
	SendString("\r\nControl_Index=");send_hex_u32(Index_Range.Control_Index_Min);SendString(":");send_hex_u32(Index_Range.Control_Index_Max);
	SendString("\r\nRemote_Index=");send_hex_u32(Index_Range.Remote_Index_Min);SendString(":");send_hex_u32(Index_Range.Remote_Index_Max);
#endif
#ifdef TX_DEBUG
	SendString("\r\nap4_key=");
	send_hex_u8(Encrypt_Key);
#endif
}

#pragma CODE = READ_BOOK_NEWS_CODE
// the function is read current bood news;
void read_book_news( U16 num_book )
{
   U32 _MEM_TYPE_SLOW_ Address;
   U32 _MEM_TYPE_SLOW_ BookAddress;
   U8 _MEM_TYPE_SLOW_ buf_offset = 0;
   U8 _MEM_TYPE_SLOW_ t = 0;
   U16 _MEM_TYPE_SLOW_ temp;
   
#ifdef TX_DEBUG
	SendString("\r\nBook Search num=");
	send_hex_u16(Current_Record.Book_num);
#endif
    Search_Record.value = 0;
   Address = All_Control.Point_Book_Table + sizeof( U32 )* num_book;
 
   (void)read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&BookAddress, sizeof( U32 ));
#ifdef TX_DEBUG
	SendString(":");
	send_hex_u32(BookAddress);
#endif     
//   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&BookAddress, sizeof( U32 ));
//   read_data_from_nf( BookAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Book, sizeof( Book_Table ));
/*
	  (void)file_seek( BookAddress, FS_SEEK_SET );
	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Book, sizeof(U16)*11);	
	  (void)file_seek( (BookAddress+22), FS_SEEK_SET );
	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Book.Flag_CurrentBook, sizeof(U8));	
	  (void)file_seek( (BookAddress+23), FS_SEEK_SET );
	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, sizeof(U16)*13);
*/
if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
   {
      read_data_from_nf( BookAddress, ( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      News_Book.Max_Index_TheBook = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Book.Addr_Action_BookTable, sizeof( U32 ));
      read_data_from_nf( BookAddress + 22, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Flag_CurrentBook, sizeof( U8 ));
      //read_data_from_nf( BookAddress + 23, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, 26 );
   }
   else
   {
      read_data_from_nf( BookAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Book, 8 );
      read_data_from_nf( BookAddress + 24, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Flag_CurrentBook, sizeof( U8 ));
      //read_data_from_nf( BookAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, 26 );
   }
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, 26 );

	  (void)act_do( News_Book.Addr_Action_BookTable, 0 );
//		state_search_to_play();
//		break;
      if( News_Book.Point_GoTable && News_Book.Num_GoTable )
      {
         if( News_Book.Flag_CurrentBook & 0x01 )
         {
               if(b_back_from_mp3)
	         	{
	         	 scan_oid_enable();
	  			 Kbd_set_evt_enabled();
	         	}
			   else
			   	{
            		Current_Record.Page_num = 0;
			   	}
            State_Search = Page_Search;
            return;
         }
      }
      /*else
      {
         Search_Record.news_sys.disable_next_key_flag = 1;
      }*/
      scan_oid_enable();
	  Kbd_set_evt_enabled();
      state_search_to_play();
}

#pragma CODE = READ_PAGE_NEWS_CODE
// the function is read current bood news;
void read_page_news( U16 num_page )
{
   U32 _MEM_TYPE_SLOW_ Address;
   U32 _MEM_TYPE_SLOW_ PageAddress;
   U8 _MEM_TYPE_SLOW_ buf_offset = 0;
   U8 _MEM_TYPE_SLOW_ t = 0;
#ifdef TX_DEBUG
	SendString("\r\nPage Search num=");
	send_hex_u16(Current_Record.Page_num);
#endif
    Search_Record.value = 0;
   Address = News_Book.Point_GoTable + sizeof( U32 )* num_page;
   
   (void)read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&PageAddress, sizeof( U32 ));
//   read_data_from_nf( PageAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Go, sizeof( Go_Table ));
  // (void)file_seek( PageAddress, FS_SEEK_SET );
  // (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Go, sizeof(U16)*27);
#ifdef TX_DEBUG
	SendString(":");
	send_hex_u32(PageAddress);
#endif 
 if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
   {
      U16 _MEM_TYPE_SLOW_ temp;
      read_data_from_nf( PageAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Go, 8 );
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      //News_Go.page_info.Min_Index_ThePage = ( U32 )temp;
      News_Go.Min_Index_ThePage = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      News_Go.Max_Index_ThePage = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Go.Addr_Action, sizeof( U32 ));

      read_data_from_nf( PageAddress + 32, ( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_DefaultTab, sizeof( U32 ));
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      News_Go.Num_DefaultTab = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_BackgroudTab, sizeof( U32 ));
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      News_Go.Num_BackgroudTab = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_ControlTab, 10 );

      if( All_Control.Version >= 14 )
      {
         read_data_from_nf( PageAddress + 56, ( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_IndexGroup, sizeof( U32 ));
      }
   }
   else
   {
      read_data_from_nf( PageAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Go, 20 );
      read_data_from_nf( PageAddress + 36, ( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_DefaultTab, 26 );
      read_data_from_nf( PageAddress + 64, ( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_IndexGroup, sizeof( U32 ));
   }


     (void)act_do( News_Go.Addr_Action, 0 );
		 
	  //	}
	 // flag_change_ap4 = TRUE;
	  addr_novoice = News_Go.Action_NoVoice;
#ifdef TX_DEBUG
	SendString("\r\nPoint_IndexGroup = ");
	send_hex_u32(News_Go.Point_IndexGroup);
#endif
}
#pragma CODE = CONTROL_NEWS_CODE
void control_news_dowith(void)
{
      Search_Record.news_sys.table_flag = CONTROL_TABLE;
      switch( News_Control.Type_Control )
      {
         case SCRIPT_GAME_TYPE:
		    SDKLoadCode(GAME_Script_CODE);
			script_init();
			script_decode();
		 	break;
         case GAME_TYPE:
#if 1		 	
	  		 SDKLoadCode(GAME_INITIAL_CODE);
	         if( !game_initial())
	         {
	            News_Control.Type_Control = 0;
	            Search_Record.news_sys.table_flag = DEFAULT_TABLE;

	         }
			 else
			 	{
				SDKLoadCode(GAME_SEARCH_1_CODE);
				 game_initial_1();
			 	}
		
#else
		 if(player_ap3_is_busy())
			player_ap3_stop();
		 file_close();
		 bak_to_news();
		 CommonLoadCode(C_GAME);
#endif		 
         break;

         case DIC_TYPE:
         Search_Record.news_sys.table_flag = DEFAULT_TABLE;
         //Search_Record.news_sys.disable_next_key_flag = 1;
 //        Ap3_MenuPath = CONTROL_MENU_PATH;
         break;

         case QA_TYPE:
         Search_Record.news_sys.table_flag = DEFAULT_TABLE;
         break;
      }
	  scan_oid_enable();
	  Kbd_set_evt_enabled();
      state_search_to_play();
}

#pragma CODE = READ_CONTR_NEWS_CODE
// the function is read current control mode news;
void read_control_mode_news( U16 num_control )
{
   U32 _MEM_TYPE_SLOW_ Address;
   U32 _MEM_TYPE_SLOW_ ControlAddress;
   U8 _MEM_TYPE_SLOW_ buf_offset = 0;
   U8 _MEM_TYPE_SLOW_ t = 0;
 #ifdef TX_DEBUG
	SendString("\r\ncontrol search num=");
 	send_hex_u16(num_control);
#endif  
/*
   //mp3_addr_bak_flag_record = 0;
   Address = News_Go.Point_ControlTab + sizeof( U32 )* num_control;
   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&ControlAddress, sizeof( U32 ));

   (void)file_seek( ControlAddress, FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Control, sizeof(U16)*12);
   
   (void)file_seek( (ControlAddress+24), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Control.Type_Control, sizeof(U8));
   (void)file_seek( (ControlAddress+25), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Control.Num_Control, sizeof(U16));
   (void)file_seek( (ControlAddress+27), FS_SEEK_SET );
   (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Control.Flag, sizeof(U8));
*/
    Address = News_Go.Point_ControlTab + sizeof( U32 )* num_control;

   read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&ControlAddress, sizeof( U32 ));
#ifdef TX_DEBUG
	SendString(":");
	send_hex_u32(ControlAddress);
#endif    
   //read_data_from_nf( ControlAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Control, sizeof( Control_Table ));
   read_data_from_nf( ControlAddress + 16, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Point, 9 );
   //read_data_from_nf( ControlAddress + 24, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Type_Control, sizeof( U8 ));
   //if( All_Control.Version < 15 )
   if(( All_Control.Version & MSK_EXT_INDEX )== 0 )
   {
      U16 _MEM_TYPE_SLOW_ temp;

      read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U16 ));
      News_Control.Num_Control = ( U32 )temp;
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&News_Control.Flag, sizeof( U8 ));
   }
   else
      read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Num_Control, 5 );
   //read_data_from_nf( ControlAddress + 27, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Flag, sizeof( U8 ));

#ifdef TX_DEBUG
	SendString("\r\nctrl_poing:num=");
	send_hex_u32(News_Control.Point);
	SendString(":");
	send_hex_u32(News_Control.Num_Control);
#endif    
 
}


#pragma CODE = READ_SGDB_CODE

/*
void findActAddress(U32 addr)
{
	U32 Address,Offset;
	U32 SpeechAddress;
	U16 Count;
	U16 tempI = 0;
	if(News_Go.Point_IndexGroup ==0)return;
#ifdef TX_DEBUG
   	 SendString("\r\nstz_addr=");
	 send_hex_u32(addr);
#endif	
	
//	read_book_data(Address,&Count,sizeof(Count));
	read_data_from_nf( News_Go.Point_IndexGroup, ( U8 _MEM_TYPE_SLOW_*)&Count, sizeof( Count ));

	if(Count==0)
	{
		return;
	}
	Offset = News_Go.Point_IndexGroup + (U32)2;
	for(tempI = 0; tempI < Count; tempI ++){
		Address = Offset +  (U32)(tempI<<2);
		read_data_from_nf(Address ,( U8 _MEM_TYPE_SLOW_ *)&SpeechAddress, 4);
		if(addr == SpeechAddress){
			stzPlayNum = tempI;
#ifdef TX_DEBUG
   	 SendString("\r\ncurNum=");
	 send_hex_u16(stzPlayNum);
#endif				
			return;
			}
		}
	
}
*/
Bool findIndexAction(U32 addr,U16 num)
{

	//U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
	U16 _MEM_TYPE_SLOW_ nStart, nMiddle;
   	U32 _MEM_TYPE_SLOW_ iAddress;
	FOURBYTESNEWS tempFourBytesIndex;
#if 0//def TX_DEBUG
   	 SendString("\r\nfindIndexAction=");
	 send_hex_u32(addr);
#endif	
	all_control_addr = 0;
	if(( All_Control.Version & MSK_SORT_FLAG)== 0 ){
		iAddress = addr + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof(U32);
    	(void)read_data_from_nf( iAddress, ( _MEM_TYPE_SLOW_ U8 *)&all_control_addr, sizeof( U32 ));
		}
	else{

		#undef nEnd
		#define nEnd   num
#if 0//def TX_DEBUG
		SendString("\r\ntwobytes=");
		send_hex_u32( addr);
		SendString(":");
		send_hex_u16(num);
#endif	
	   if( !addr || !num )
	      return FALSE;
	   nStart = 0;
	   nEnd = num - 1;
	   while( nStart <= nEnd )
	   { 
	      nMiddle = ( nStart + nEnd )/ 2;

	      iAddress = addr + sizeof( FOURBYTESNEWS ) * nMiddle;
	      read_data_from_nf( iAddress, ( U8 _MEM_TYPE_SLOW_ *)&tempFourBytesIndex.Index, sizeof( FOURBYTESNEWS ));
#if 0//def TX_DEBUG
   	 SendString("\r\nI:A=");
	 send_hex_u32(tempFourBytesIndex.Index);
	 SendString(":");
	 send_hex_u32(tempFourBytesIndex.ActionIndex);
#endif			  
	      if( Current_Index == tempFourBytesIndex.Index) 
	      {
	         all_control_addr = tempFourBytesIndex.ActionIndex;
	         return TRUE;
	      }
	      else if( Current_Index < tempFourBytesIndex.Index )
	      {
	         if( nMiddle == 0 )
	         {
	            return FALSE;
	         }
	         nEnd = nMiddle - 1;
	      }
	      else
	      {
	         nStart = nMiddle + 1;
	      }
	   }
		return FALSE;
	 #undef nEnd
	}
	return true;
}
void read_sgdb( void )
{
   U8  _MEM_TYPE_SLOW_ search_status;
   U32 _MEM_TYPE_SLOW_ kuang_adress, action_address;
   
    SendString("\r\nNormal_Search");
	//off_width =  sizeof(U32);
	/*
   if(( All_Control.Version & MSK_EXT_INDEX )!= 0 ){
   		off_width +=  sizeof(U32);
   	}*/
   if( Current_Index > News_Book.Max_Index_TheBook )
   {
      //(void)act_do( News_Go.Action_NoVoice, 0 );
      bInvalidFlag = true;
      return;
   }
   if( Coor_Type == IMMOBILITY_COOR_MODE )
   {
      if(( Current_Index > News_Go.Max_Index_ThePage )
         ||( Current_Index < News_Go.Min_Index_ThePage ))
      {
        // (void)act_do( News_Go.Action_NoVoice, 0 );
        bInvalidFlag = true;
         return;
      }
   }
   /*Search_Record.news_sys.reread_flag 
      = Search_Record.news_sys.repeat_flag
         = Search_Record.news_sys.sequence_flag
               = Search_Record.news_sys.play_record_flag
                  = 0;*/
   Search_Record.value &= ~(U16)0x81C0;
   if( Search_Record.news_sys.table_flag == DEFAULT_TABLE )
   {
       if( Coor_Type == IMMOBILITY_COOR_MODE ){
         search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
       	}
         
   }
   else if( Search_Record.news_sys.table_flag == BACKGROUND_TABLE )
   {
      if( Coor_Type == IMMOBILITY_COOR_MODE ){
         search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
      }
   }
   else if( Search_Record.news_sys.table_flag == CONTROL_TABLE )
   {
      //if( Search_Record.news_sys.mode_flag == SINGLE_TABLE )
      if( News_Control.Type_Control == SINGLE_TABLE_TYPE )
      {
        if( Coor_Type == IMMOBILITY_COOR_MODE ){
            search_status = CONTROL_IMMOBILITY_COOR_STATUS;
         }
      }
      else if( News_Control.Type_Control == GAME_TYPE )
      {
//         swap_code_flag &= ~CUSTER_CODE_FLAG;
//		 swap_code(GAME_SEARCH_1_CODE_FLAG);
		  SDKLoadCode(GAME_SEARCH_1_CODE);
         in_game();
//		 flag_gendu = 0;
         return;
      }
	  else
      {
         return;
      }
   }
   else
   {
      //(void)act_do( News_Go.Action_NoVoice, 0 );
      bInvalidFlag = true;
      return;
   }
   while( 1 )
   {
      switch( search_status )
      {
         case CONTROL_IMMOBILITY_COOR_STATUS:
		  SendString("\r\nCONTROL");	
         if(( !News_Go.Point_ControlTab )||( !News_Go.Num_ControlTab )||( !News_Control.Point ))
         	{
            search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
            break;
         	}
        // kuang_adress = News_Control.Point + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof(U32);
         //(void)read_data_from_nf( kuang_adress, ( _MEM_TYPE_SLOW_ U8 *)&action_address, sizeof( U32 ));
         (void)findIndexAction( News_Control.Point, News_Control.Num_Control);
         //if( News_Control.Num_Control )
        // {
            if( act_do( all_control_addr , 0 ))
            {
               return;
            }
         //}
		
         search_status = DEFAULT_IMMOBILITY_COOR_STATUS;
         break;
		
         case DEFAULT_IMMOBILITY_COOR_STATUS:
		 SendString("\r\nDEFAULT");
         if( !News_Go.Point_DefaultTab )
         {
            search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
            break;
         }
         //kuang_adress = News_Go.Point_DefaultTab + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof(U32);
         //read_data_from_nf( kuang_adress, ( _MEM_TYPE_SLOW_ U8 *)&action_address, sizeof( U32 ));
          (void)findIndexAction( News_Go.Point_DefaultTab, News_Go.Num_DefaultTab);
         if( act_do( all_control_addr, 0 ))
         {
            return;
         }
         search_status = BACKGROUND_IMMOBILITY_COOR_STATUS;
         break;

         case BACKGROUND_IMMOBILITY_COOR_STATUS:
		  SendString("\r\nBACKGROUND");	
         if( !News_Go.Point_BackgroudTab )
         {
            //void)act_do( News_Go.Action_NoVoice, 0 );
            bInvalidFlag = true;
            return;
         }
         //if(( All_Control.Mask_Gloablext & MSK_COOR_MODE )== IMMOBILITY_COOR_MODE )
         {
            //kuang_adress = News_Go.Point_BackgroudTab + ( U32 )( Current_Index - News_Go.Min_Index_ThePage )* ( U32 )sizeof(U32);
            //(void)read_data_from_nf( kuang_adress, ( _MEM_TYPE_SLOW_ U8 *)&action_address, sizeof( U32 ));
             (void)findIndexAction( News_Go.Point_BackgroudTab, News_Go.Num_BackgroudTab);
            if( act_do( all_control_addr, 0 ))
            {
               return;
            }
            //(void)act_do( News_Go.Action_NoVoice, 0 );
            bInvalidFlag = true;
            return;
         }
      }
   }
}

#pragma CODE = JUDGE_COOR01_CODE
Bool judge_coor_01(void)
{
      Bool temp_flag;   
	  	//sysup_opstart();
#ifdef TX_DEBUG
	SendString("\r\njudge_coor_01");
#endif
	 g_b_autopoweroff_forbidden = TRUE;
     interrupt_play();
#ifdef RP_RECORD
	SDKLoadCode(RP_JUDGE_CODE);
	temp_flag = record_judge();
//	SDKLoadCode(READ_SGDB_CODE);
   if(temp_flag)return TRUE;
#endif   
   /*
   Search_Record.value &= (U16)(~0x81C2);
   if(( State_Search != Play_Search )
      &&( State_Search != Idle_Search )
         &&( State_Search != Sequence_Search )
            &&( State_Search != Null_state ))
      return TRUE;*/
 //  if(nostop_index())return;


//   repeat_save();
   init_array_v();
   state_search_to_play();
   return FALSE;
}
#pragma CODE = JUDGE_COOR02_CODE
//the fuction is to judge coordinate range
Bool judge_coor_02( void )
{
   /*Search_Record.news_sys.reread_flag 
      = Search_Record.news_sys.repeat_flag 
         = Search_Record.news_sys.sequence_flag
               = Search_Record.news_sys.play_record_flag
                  = 0;*/

//   if((Current_Index == 0xFFFF)&&(News_Control.Type_Control == GAME_TYPE)){
//   	 return;
//   	}
   //Search_Record.news_sys.flag_index = 0;
 	//state_search_to_play();
    //num_repeat = 0;
   if( Coor_Type != IMMOBILITY_COOR_MODE)return false;
    if(RemoteBaseAddress>=size_current_ap4)return FALSE;
	 if( !bHaveRemote )return false;
      if((( Current_Index >= Index_Range.Remote_Index_Min )
         &&( Current_Index <= Index_Range.Remote_Index_Max ))||
         (( Current_Index >= Current_Other_Range.Guding_Min)
         &&( Current_Index <= Current_Other_Range.Guding_Max)))
      {
            SendString("\r\nremote_task!");
            SDKLoadCode(REMOTE_INDEX_CODE);
		    if(search_remote_index()){
				SDKLoadCode(REMOTE_TASK_CODE);
	//			swap_code(REMOTE_TASK_CODE_FLAG);
           		 remote_task();
				if(cal_flag == 2)
			         {
			            //SDKLoadCode(AP3_OPEN_FILE_CODE);
			      		//SDKLoadCode(FILE_EXTERN_CODE);
			      	    
#ifdef TX_DEBUG
						 SendString("\r\ncal_flag =2");
#endif		      	    
			            Point_Read = 0;
			            Point_Write = 0;
			            cal_flag = 0;
						b_cal_over = TRUE;
						return TRUE;
			         }
			//if(News_Control.Type_Control == GAME_TYPE){
			//	SDKLoadCode(GAME_SEARCH_1_CODE);
			//	}
				 if(bRemoteChangeAp3){
				 	scan_oid_disable();
					Kbd_set_evt_disabled();
				 	}
#if 0//def TX_DEBUG
				 SendString("\r\nRemoteNumber=");
				// send_hex_u16(Current_Index);
				 //SendString(":");
				 send_hex_u16(RemoteNumber);
#endif	
		 //return FALSE;
         }
         //break;
         //return FALSE;
         return TRUE;
      }
	  if(g_b_cal){
		  	//return FALSE;
		  	return TRUE;
	}  
   return FALSE;
}

#pragma CODE = JUDGE_COOR03_CODE
//the fuction is to judge coordinate range
//the fuction is to judge coordinate range
Bool judge_coor_03( void )
{
  
   if( Coor_Type != IMMOBILITY_COOR_MODE)return false;
	  SendString("\r\nbegin search!");
	  SDKLoadCode(SEARCH_TWOBYTE_CODE);
      if((( Current_Index >= Index_Range.Normal_Index_Min )
         &&( Current_Index <= Index_Range.Normal_Index_Max ))||
         (( Current_Index >= Index_Range.Resv_Index_Min)
         &&( Current_Index <= Index_Range.Resv_Index_Max)))
      {
		 SendString("\r\nnormal search!");
         State_Search = Normal_Search;
		 return false;
      }
    if(( Current_Index >= Index_Range.Book_Index_Min )
               &&( Current_Index <= Index_Range.Book_Index_Max ))
      {
       SendString("\r\nbook search!");
         if( search_twobytes_coordinate( All_Control.Point_Gloablext_Coor, All_Control.Num_Book ))
         {
            Current_Record.Book_num = Current_Coor.X0;
            State_Search = Book_Search;
			Current_Record.Page_num = 0;
			Current_Record.Control_num = 0;
             
         }else{
           // (void)act_do( News_Go.Action_NoVoice, 0 );
 			goto findFail;
         	}
		return false;
      }
	
     if(( Current_Index >= Index_Range.Page_Index_Min )
               &&( Current_Index <= Index_Range.Page_Index_Max ))
      {
      SendString("\r\npage search!");
			//SDKLoadCode(SEARCH_TWOBYTE_CODE);
         if( search_twobytes_coordinate( News_Book.Point_GOTable_Coordinate, News_Book.Num_GoTable ))
         {
            Current_Record.Page_num = Current_Coor.X0;
            State_Search = Page_Search;
			Current_Record.Control_num = 0;
         }else{
			// (void)act_do( News_Go.Action_NoVoice, 0 );
 			goto findFail;
			}
			return false;
      }
      if((( Current_Index >= Index_Range.Control_Index_Min )
               &&( Current_Index <= Index_Range.Control_Index_Max ))||
               (( Current_Index >= Current_Other_Range.Ap3_Min)
               &&( Current_Index <= Current_Other_Range.Ap3_Max)))
      {
          SendString("\r\ncontrol search!=");
			//SDKLoadCode(SEARCH_TWOBYTE_CODE);
         if( search_twobytes_coordinate( News_Go.Point_PageCoorTable, News_Go.Num_ControlTab ))
         {
            Temp_Control_num = Current_Record.Control_num;
            Current_Record.Control_num = Current_Coor.X0;
//			ap4_mp3_flag &= ~(BANK_FORM_MP3|BANK_HAVE_INDEX|CURRENT_CONTROL_GAME);
            State_Search = Control_Mode_Search;
			SendString("\r\ntrue");
            //break;
         }else{
			// (void)act_do( News_Go.Action_NoVoice, 0 );
 			goto findFail;
            	}
			return false;
      }
findFail:
            //(void)act_do( News_Go.Action_NoVoice, 0 );
            bInvalidFlag = true;

   return FALSE;
}


#pragma CODE = RP_JUDGE_CODE
#ifdef RP_RECORD

void play_mp3_from_addr(U32 addr, U8 flag)
{
	 if(player_ap3_start( addr, flag ))
	 	{
	 	    while (g_b_player_on)
	    	{    		
	    	    player_task();
			    if(rec_led_is_timer_timeout())
				{
					BlueLed_FLASH();
					RedLed_OFF();
					rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
				}
	    	}
	 	}
}

Bool record_judge(void)
{
#if 0
	if (((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))||
		((Current_Index >= Ap4_Record_Range.Play_Index_Min)&&((Current_Index <= Ap4_Record_Range.Play_Index_Max))))
		{
next_rp:		
		 if( player_ap3_is_busy())
			 player_ap3_stop();
		 Search_Record.news_sys.flag_index = 0;
		 Search_Record.news_sys.reread_flag = 0;
		swap_code(AP3_OPEN_FILE_CODE_FLAG);
 		swap_code(FILE_EXTERN_CODE_FLAG);
	if((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))
		{
		 tishi_mp3(4);
		}
	else
		{
		 tishi_mp3(6);
		}
		 swap_code(RP_RECORD_CODE_FLAG);
		 rp_task();
		 swap_code_flag &= ~CUSTER_CODE_FLAG;
		 swap_code(AP3_OPEN_FILE_CODE_FLAG);
 		 swap_code(FILE_EXTERN_CODE_FLAG);
		 if(rp_flag&0x0001)
		 	{
		 	tishi_mp3(5);
		 	}
		 if(rp_flag&0x0002)
		 	{
		 	tishi_mp3(7);
		 	}
		 if(rp_flag&0x0004)
		 	{
		 	tishi_mp3(8);
		 	}
		 if(Search_Record.news_sys.flag_index){
		 		if (((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))||
					((Current_Index >= Ap4_Record_Range.Play_Index_Min)&&((Current_Index <= Ap4_Record_Range.Play_Index_Max)))){
						goto next_rp;
		 			}
		 	return FALSE;
		 	}else {
		 		return TRUE;
		 		}
//		 return TRUE;
		}
	   return FALSE;
#else	 
    if((Ap4_Record_Range.Record_Index_Min== 0)||(Ap4_Record_Range.Record_Index_Max==0))return FALSE;
	if (((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))||
		((Current_Index >= Ap4_Record_Range.Play_Index_Min)&&((Current_Index <= Ap4_Record_Range.Play_Index_Max))))
		{
next_rp:		
		 interrupt_play();
		 //Search_Record.news_sys.flag_index = 0;
		 index_flag = FALSE;
		 Search_Record.news_sys.reread_flag = 0;
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
// 		swap_code(FILE_EXTERN_CODE_FLAG);
	if((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))
		{
		 play_mp3_from_addr(RemoteRecordStartAddr,1);
		}
	else
		{
		 play_mp3_from_addr(RemotePlayStartAddr,1);
		}
//		 swap_code(RP_RECORD_CODE_FLAG);
		 SDKLoadCode(RP_RECORD_CODE);
		 rp_task();
		 //RedLed_OFF();
		 op_start_timer();
		 Kbd_set_evt_enabled();
//		 swap_code_flag &= ~(RP_RECORD_CODE_FLAG|FILE_EXTERN_CODE_FLAG);
//		 swap_code_flag &= ~SYSTEM_CODE_FLAG;
//		 swap_code(AP3_OPEN_FILE_CODE_FLAG);
// 		 swap_code(FILE_EXTERN_CODE_FLAG);
		 if(rp_flag&0x0001)
		 	{
		 	 play_mp3_from_addr(RemoteRecordEndAddr,1);
		 	}
		 if(rp_flag&0x0002)
		 	{
		 	 play_mp3_from_addr(RemotePlayEndAddr,1);
		 	}
/*		 
		 if(rp_flag&0x0004)
		 	{
		 	tishi_mp3(8);
		 	}
*/
		 if(index_flag){
		 		if (((Current_Index >= Ap4_Record_Range.Record_Index_Min)&&((Current_Index <= Ap4_Record_Range.Record_Index_Max)))||
					((Current_Index >= Ap4_Record_Range.Play_Index_Min)&&((Current_Index <= Ap4_Record_Range.Play_Index_Max)))){
						goto next_rp;
		 			}
		 	return FALSE;
		 	}else {
		 		power_authorize_autopoweroff();
		 		return TRUE;
		 		}
//		 return TRUE;
		}
	   return FALSE;
#endif	   
}
#endif

#pragma CODE = PLAYEND_DOWITH_01_CODE
Bool change_ap4(void)
{
	//	SendString("step `: ");
	//	SendString((char *)CurrentShortFilePath);
	
		//SDKLoadCode(AP3_OPEN_FILE_CODE);
		Bool temp = FALSE;
		bRemoteChangeAp3 = FALSE;
		SendString("\r\nRemoteNumber=");
		send_hex_u16(RemoteNumber);
	    //scan_oid_disable();
		//Kbd_set_evt_disabled();
		SDKLoadCode(DO_CHANGE_AP4_CODE);
      if(!do_change_ap3())
    	{
//    	  swap_code(REMOTE_TASK_CODE_FLAG);
//    	  play_RMT_speech( RemoteSpeech.SpeechNull);
//		  swap_code(AP3_OPEN_FILE_CODE_FLAG);
//		  swap_code(FILE_EXTERN_CODE_FLAG);
			//SDKLoadCode(AP3_OPEN_FILE_CODE);
		 // SDKLoadCode(FILE_EXTERN_CODE);
		   temp = TRUE;
		  //tishi_mp3(10);
//		  send_hex_u32(Current_Record.Ap3_num);

 //       SDKLoadCode(AP3_OPEN_FILE_CODE);
//		SDKLoadCode(FILE_EXTERN_CODE);
   	    //Current_Record.Ap3_num = Temp_Ap3_file_num;
//  	Get_File_Name( (unsigned char *)Ext, Current_Record.Ap3_num );
 //	     open_file();
		   //News_Control.Type_Control = NO_TYPE;
	     // Search_Record.news_sys.table_flag = DEFAULT_TABLE;
//		  act_do( All_Control.Addr_Action_Open, 0 );
//         State_Search = Play_Search;
		   SendString("\r\n no find ap4");
           //State_Search = Open_File_Search;
//		  flag_change_ap4 = FALSE;
		  //scan_oid_enable();
		  //Kbd_set_evt_enabled();
		  // first_go_ap4 = 0;
		  //ap4_mp3_flag = 0;
    	}
	  else
	  	{
		 // scan_oid_disable();
		 /// Kbd_set_evt_disabled();
		  //News_Control.Type_Control = NO_TYPE;
	      //Search_Record.news_sys.table_flag = DEFAULT_TABLE;
	  	  //first_go_ap4 = 0;
		 // ap4_mp3_flag = 0;
		  SendString("\r\nopen new file!");
		  Current_Record.Book_num = 0;
		  Current_Record.Page_num = 0;
		  Current_Record.Control_num = 0;
//		  file_init();
		  //State_Search = Open_File_Search;
		  
	  	}
	    g_b_open_file_error = TRUE;
	  	first_go_ap4 = 0;
  		b_back_from_mp3 = FALSE;
		//b_back_from_rp = FALSE;
		b_back_have_index = FALSE;
		b_current_control_game = FALSE;
  		
	    News_Control.Type_Control = NO_TYPE;
	    Search_Record.news_sys.table_flag = DEFAULT_TABLE;
	  	//first_go_ap4 = 0;
		//ap4_mp3_flag = 0;
		State_Search = Open_File_Search;
	  	return temp;
}

#pragma CODE = PLAYEND_DOWITH_02_CODE
void auto_close_dowith(void)
{
///*
			 if(!g_b_wait_time){
			 	
				tishi_mp3(FLAG_MIN_TISHI,false);
#ifdef TX_DEBUG
				SendString("\r\n3min tishi_1!");
#endif
				g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(TISHI_TIME));
				g_b_autopoweroff_forbidden = FALSE;
				g_b_wait_time = TRUE;
				g_b_tishi_2 = FALSE;
				return;
			 	}	
/*			 
			 if(!g_b_tishi_2){
			 	tishi_mp3(FLAG_MIN_TISHI);
				
#ifdef TX_DEBUG
				SendString("\r\n3min tishi2!");
#endif
				g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(TISHI_TIME));
				g_b_autopoweroff_forbidden = FALSE;
				g_b_wait_time = TRUE;
				g_b_tishi_2 = TRUE;
				return;
			 	}
//*/

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
	    return;
}

#pragma CODE = SEARCH_OPEN_AP4_CODE00
Bool search_open_ap4_00(void)
{
	 Bool testFlag = FALSE;
      first_go_ap4 = 1;
	  /*
      scan_oid_disable();
	  Kbd_set_evt_disabled();
      //SendString("Search\r\n");
     // ap3_rand_init();
      init_array_v();
	 // clear_bigflag();
	  clear_smallflag();
//	  flag_gendu = 0;
     News_Control.Type_Control = NO_TYPE;
*/
	 SDKLoadCode(CLEANSTRUCT_CODE);
	  cleanStruct();
next_open:
	SDKLoadCode(OPEN_FILE_CODE);
//      Get_File_Name( (unsigned char *)Ext, Current_Record.Ap3_num );
      if( !open_file())
      {
file_search_error1:
#ifdef TX_DEBUG
		SendString("\r\nopen_f!");
#endif
         Search_Record.value = 0;  
		 if( Current_Record.Ap3_num > 0)
               {
                  Current_Record.Ap3_num = ap3_file_number - 1;
				  if(testFlag){
#ifdef TX_DEBUG
						SendString("\r\nonfind_f");
#endif 
						system_reset();
				  	}
				  if(!testFlag)testFlag = TRUE;
               }
               else
                  Current_Record.Ap3_num--;
			  // State_Search = Open_File_Search;
			  goto next_open;
		       //return FALSE;   
      }

	  SDKLoadCode(FS_CREATE_BUF_CODE);
	  (void)FSCreateBuf(fat_g_cur_file);
	  //SDKLoadCode(FILE_EXTERN_CODE);
	  
	  //state_search_to_ap3();
     // break;
	  //nor_ap4_bak = Current_Record.Ap3_num;
     // case Ap3_Search:
	  //flag_gendu = 0;
     // ap3_rand_init();
 //     SendString("Ap3 Search\r\n");
     // scan_oid_disable();
	//  Kbd_set_evt_disabled();
	  

	  SDKLoadCode(JIAMI_CODE);
   	  if(judge_password()){
   	 	//SDKLoadCode(FIND_NEWS_PASS_CODE);
   		//read_new_ap4_password();
   		SDKLoadCode(FIND_PASS_03_CODE);
		new_read_all_control_news();
   		}
  	 else{
	 	//goto file_search_error2;
  	 	//SDKLoadCode( READ_ALLNEWS_CODE );
   		//read_all_control_news();
   		//(void)file_seek( 0, FS_SEEK_SET );
 		//(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Buf, sizeof(U8)*512);
   		}
	  
      //Search_Record.value = 0;
	  SDKLoadCode(READ_ALL_NEWS_CODE);
	  read_all_news();
	  
	  SDKLoadCode(PRINT_NEWS_AP4_CODE);
	  print_news_ap4();

	  SDKLoadCode(ENCRY_NEWS_CODE);
	  (void)encry_news();
      //scan_oid_enable();
      if( All_Control.Version < 2 )
      {
#ifdef TX_DEBUG
		SendString("\r\nv_wrong!");
#endif      
file_search_error2:
         file_close();
         State_Search = Open_File_Search;
         goto file_search_error1;
      }
#if( AP4_ENCRYPT == ENABLE )
      if( MSB0( All_Control.Point_Book_Table )== 'D' )
      {
         MSB0( All_Control.Point_Book_Table )^= 'D';
         MSB1( All_Control.Point_Book_Table )^= 'F';
         MSB2( All_Control.Point_Book_Table )^= 'B';
         MSB3( All_Control.Point_Book_Table )^= 'K';
      }
      else
      {
#if( DECODE_NORMAL_FILE == ENABLE )
         if( !MSB0( All_Control.Point_Book_Table ))
         {
            goto ap4_search_decode_file_ok;
         }
#endif   //#if( DECODE_NORMAL_FILE == ENABLE )
         goto file_search_error2;
      }
#else    //#if( AP4_ENCRYPT == DISABLE )
      if( MSB0( All_Control.Point_Book_Table ))
      {
         goto file_search_error2;
      }
#endif   //#if( AP4_ENCRYPT == ENABLE )
#if( AP4_ENCRYPT == ENABLE )
#if( DECODE_NORMAL_FILE == ENABLE )
ap4_search_decode_file_ok:
#endif   //#if( DECODE_NORMAL_FILE == ENABLE )
#endif   //#if( AP4_ENCRYPT == ENABLE )
//      ap4_change_file = MMI_AP3_NO_CHANGE_FILE;

		return TRUE;      
}
#pragma CODE = SEARCH_OPEN_AP4_CODE01
void search_open_ap4_01(void)
{
		if(!b_first_ap4_mp3_flag){			
           	
			b_first_ap4_mp3_flag = TRUE;
			if(!b_startup_play){

				(void) act_do( All_Control.Addr_Action_Open, 0 );
				}
		  }
		else{
			(void) act_do( All_Control.Addr_Action_Open, 0 );
			}
		
      if( All_Control.Point_Special_Action )
         read_data_from_nf( All_Control.Point_Special_Action, ( U8 _MEM_TYPE_SLOW_ *)&Current_Special_Action, sizeof( U32));
      {
//	  	   Current_Special_Action.Battery_Low_Speech = ULongLE2BE(Current_Special_Action.Battery_Low_Speech);
		 SDKLoadCode(REMOTE_TASK_INIT_CODE);
//		 swap_code(REMOTE_TASK_INIT_CODE_FLAG);
         remote_task_init();
         //RemoteBaseAddress = All_Control.Point_Remote;
         init_remote();
//         Ap3_MenuPath = BOOK_MENU_PATH;
			
//          state_search_to_play();
//		  break;

         //Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
         if( All_Control.Point_Book_Table && All_Control.Num_Book )
         {
            if( All_Control.Mask_Gloablext & AUTO_FIRST_BOOK )
            {
               if(b_back_from_mp3)
	         	{
	         	}
			   else
			   	{
	               Current_Record.Book_num = 0;
			   	}
               State_Search = Book_Search;
               return;
            }
         }
         //scan_oid_enable();
	  	 //Kbd_set_evt_enabled();
         state_search_to_play();
      }
}
#pragma CODE = 	FIX_INDEX_CODE01
void vol_tishi(void)
{
	//interrupt_play();
	/*
	if(vol_idx==0){
		tishi_mp3(FLAG_MINVOL);
		return;
		}
	if(vol_idx==(VOL_NUM-1)){
		tishi_mp3(FLAG_MAXVOL);
		return;
		}*/
	//tishi_mp3(FLAG_VOLUME);


			/*
			if(!g_b_player_on){
				if(vol_idx == (VOL_NUM-1)){
					//interrupt_play();
					tishi_mp3(FLAG_MAXVOL);
					return;
					}
				if(vol_idx == (0)){
					//(void)act_do(All_Control.Addr_Antion_key, 0);
					//state_search_to_play();
					tishi_mp3(FLAG_MINVOL);
					return;
					}
				
					tishi_mp3(FLAG_VOLUME);
				}*/
				if(!g_b_player_on){
					tishi_mp3(FLAG_VOLUME,false);
					}

}
Bool fix_index_01(void)
{
	U8 i = 0;
	//if(!bHaveRemote)return false;
#ifdef TX_DEBUG
		SendString("\r\nfix_index_01");
#endif  	
	if(remote_index_tab[REMOTE_VOL_UP] == Current_Index){
		 if(vol_idx < (VOL_NUM-1))
			{
			     vol_idx++;
			}
		 if(player_ap3_is_busy()){
		 	}else{
		 		(void)act_do(All_Control.Addr_Antion_key, 0);
				 state_search_to_play();
		 		}
			return TRUE;
		}
	
	if(remote_index_tab[REMOTE_VOL_DOWN] == Current_Index){
		 if(vol_idx > 0)
			{
			     vol_idx--;
			}
		 if(player_ap3_is_busy()){
		 	}else{
		 		(void)act_do(All_Control.Addr_Antion_key, 0);
				 state_search_to_play();
		 		}
			return TRUE;
		}
	return FALSE;
}

#pragma CODE = 	FIX_INDEX_CODE02
Bool fix_index_02(void)
{

	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE03
Bool fix_index_03(void)
{
	return FALSE;
}

#pragma CODE = 	READY_OPENNEWAP4_CODE
void change_area(void)
{
	current_sd = record_sd_id;
	Current_Record.Ap3_num = record_ap4_num;
}
void ready_open_newap4(void)
{
	file_close();
	Search_Record.value = 0;
	State_Search = Open_File_Search;
	News_Control.Type_Control = NO_TYPE;
	Search_Record.news_sys.table_flag = DEFAULT_TABLE;
	first_go_ap4 = 0;
}
#pragma CODE = 	BEGIN_FIND_AP4_CODE
/*
void ready_open_file(void)
{
	file_close();
	Search_Record.value = 0;
	State_Search = Open_File_Search;
    g_b_open_file_error = TRUE;
    init_array_v();
	News_Control.Type_Control = NO_TYPE;
	Search_Record.news_sys.table_flag = DEFAULT_TABLE;
	first_go_ap4 = 0;
}
*/
Bool begin_find_ap4(U16 temp_num)
{
	U16 temp_sd;
	SDKLoadCode(AP4_CHOOSE_PRE_NEXT);
    temp_sd = check_no_ff((temp_num<<1), (ALL_OFFSET + (temp_num<<1)));
	if(0!=temp_sd){
		//Current_lastbook[current_area].Book_num = Current_Record.Ap3_num;
		//Current_lastbook[current_area].Book_id = temp_num;
		//Current_lastbook[current_area].Sd_num = current_sd;
		current_sd = temp_sd;
		Current_Record.Ap3_num = record_ap4_num;
		file_close();
		Search_Record.value = 0;
		State_Search = Open_File_Search;
	   // g_b_open_file_error = TRUE;
	   // init_array_v();
		News_Control.Type_Control = NO_TYPE;
		Search_Record.news_sys.table_flag = DEFAULT_TABLE;
		first_go_ap4 = 0;
		return TRUE;
		}
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE04
Bool fix_index_04(void)
{
	return FALSE;
}

#pragma CODE = 	FIX_INDEX_CODE05

Bool fix_index_05(void)
{
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE06
Bool fix_index_06(void)
{
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE07
Bool fix_index_07(void)
{
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE08
Bool fix_index_08(void)
{
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE09
Bool fix_index_09(void)
{
	return FALSE;
}
#pragma CODE = 	FIX_INDEX_CODE10
Bool fix_index_10(void)
{
	return FALSE;
}

#pragma CODE = 	OPEN_NEWAP4_CODE
Bool open_newAp4(void)
{
	Current_Record.Book_num = 0;
	Current_Record.Page_num = 0;
	Current_Record.Control_num = 0;
	state_search_to_play();
	SDKLoadCode(SEARCH_OPEN_AP4_CODE00);
	(void)search_open_ap4_00();
	SDKLoadCode(SEARCH_OPEN_AP4_CODE01);
	search_open_ap4_01();

    init_array_v();
    (void)act_do(All_Control.Addr_Action_Open, 0);
	SDKLoadCode(READ_BOOK_NEWS_CODE);
    read_book_news(( U16 ) Current_Record.Book_num );
	SDKLoadCode(READ_PAGE_NEWS_CODE);
    read_page_news(( U16 ) Current_Record.Page_num );

	SDKLoadCode(READ_PAGE_OTHERCODE);
  	(void)read_page_other_news();
	if(b_current_control_game)return false;
	SDKLoadCode(READ_CONTR_NEWS_CODE);
     read_control_mode_news( Current_Record.Control_num );
     //if((1!=first_go_ap4)){
     // 	(void)act_do( News_Control.Addr_Action, 0 );
	  //}
	  SDKLoadCode(CONTROL_NEWS_CODE);
	  control_news_dowith();
	  return true;
}

#pragma CODE = 	BACK_BAK_DOWITH_CODE
Bool bak_dowith(void)
{
	scan_oid_enable();
    Kbd_set_evt_enabled();
	b_back_from_mp3 = FALSE;
		 
	 if(b_back_have_index)
	 	{
#ifdef TX_DEBUG
	SendString("\r\nbak have index! = ");
#endif	 	
	 	init_array_v();
	 	 b_back_have_index = FALSE;
		 Current_Index = bak_current_index;
		 index_flag = TRUE;
#ifdef TX_DEBUG
	send_hex_u32(Current_Index);
	SendString(":");send_hex_u16(oid_index_type);
#endif		 
		 return TRUE;
	 	}
	 if(b_back_have_key){
#ifdef TX_DEBUG
	SendString("\r\nbak have key!");
#endif	 	
	 	init_array_v();
	 	b_back_have_key = FALSE;
		first_go_ap4 = 0;
		kbd_begin_write(MSB(bak_current_index), LSB(bak_current_index));
		return TRUE;
	 	}
	return FALSE;
}

#pragma CODE = 	BANKTOAP4_CODE
void bak_to_ap4(void)
{
#ifdef TX_DEBUG
	SendString("\r\nBAK_to_ap4");
#endif
	  SDKLoadCode(READ_BOOK_NEWS_CODE);
      read_book_news(( U16 ) Current_Record.Book_num );
	  SDKLoadCode(READ_PAGE_NEWS_CODE);
      read_page_news(( U16 ) Current_Record.Page_num );

       
//***********************************************
	  SDKLoadCode(READ_PAGE_OTHERCODE);
  	  (void)read_page_other_news();
	  if(!b_current_control_game){
		  clear_bigflag();
		  clear_smallflag();
		  SDKLoadCode(READ_CONTR_NEWS_CODE);
	      read_control_mode_news( Current_Record.Control_num );
	      if((1!=first_go_ap4)){
	      	(void)act_do( News_Control.Addr_Action, 0 );
		  }
		  SDKLoadCode(CONTROL_NEWS_CODE);
		  control_news_dowith();
	  	}
	  state_search_to_play();
}

#pragma CODE = JIAMI_CODE
Bool judge_password(void)
{
   U16 new_ap4_flag;
   U32 temp;
   Search_Record.value = 0;
   all_control_addr = 0;
   all_control_pw = 0;
   ( void )file_seek( 0, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&new_ap4_flag, sizeof( U16 ));
   if( new_ap4_flag != 0xFFEE ){
   	  (void)file_seek( 0, FS_SEEK_SET );
 	  (void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Buf, sizeof(U8)*512);
      return FALSE;
	  }

   ( void )file_seek( 0x35+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&all_control_addr, sizeof( U32 ));
   ( void )file_seek( 0x99+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U32 ));
   all_control_addr ^= temp;
   
   SendString((char *)"\r\nall_control_addr=");
   send_hex_u32(all_control_addr);   
   
   return TRUE;
}



#pragma CODE = FIND_PASS_01_CODE

U32	read_new_ap4_password_01( void )
{
   U16 new_ap4_flag, rand_len;
   U32 ap4_pw, temp;
   U8 pw_temp;
  

   //SDKLoadCode( JIAMI_CODE );
   

   

   ( void )file_seek( 0x60+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&rand_len, sizeof( U16 ));
   ( void )file_seek( 0xA0+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&new_ap4_flag, sizeof( U16 ));
   rand_len ^= new_ap4_flag;
   rand_len += 256u;

   ( void )file_seek( 2+( rand_len >> 1 ), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U32 ));
   
   ( void )file_seek( 2+( MSB0( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB0( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB1( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB1( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB2( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB2( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB3( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB3( ap4_pw ) = pw_temp;
   
   return ap4_pw;
}


#pragma CODE = FIND_PASS_02_CODE 

#if 0
#define  PASSWORD_POS1        0x8F  //0x5C  //0x40//
#define  PASSWORD_POS2        0xF1  //0x7B  //0x75//
#define  PASSWORD_POS3        0x7F  //0x9D  //0x50//
#define  PASSWORD_POS4        0x10  //0x38  //0x85//
#else
#define  PASSWORD_POS1        0xEF  //0x5C  //0x40//
#define  PASSWORD_POS2        0xA1  //0x7B  //0x75//
#define  PASSWORD_POS3        0x91  //0x9D  //0x50//
#define  PASSWORD_POS4        0x10  //0x38  //0x85//
#endif


U32  read_new_ap4_password_02( void )
{
   //U16 new_ap4_flag, rand_len;
   U32 temp;
   U8 pw_temp;

	//Ap4_Record_Range_2.Record_Index_Min = 0;
   // Ap4_Record_Range_2.Record_Index_Max = 0;
   // Ap4_Record_Range_2.Play_Index_Min = 0;
    //Ap4_Record_Range_2.Play_Index_Max = 0;

   
   ( void )file_seek( 2+PASSWORD_POS1, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB0( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB1( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS3, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB2( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS4, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB3( temp ) = pw_temp;
   return temp;
}

/*
#pragma CODE = FIND_NEWS_PASS_CODE 

U32 pass_jiami( U32 shuju, U32 mima )
{
   U8 byte, i;
  
   i = 4 ;
   shuju ^= mima;
   do
   {
      byte = shuju >> 24;
      shuju <<= 8;
      shuju += byte;
      shuju ^= mima;
   }
   while( --i );
   return shuju;
}

Bool read_new_ap4_password( void )
{
   U16 new_ap4_flag, rand_len;
   U32 ap4_pw, user_pw, temp;
   U8 pw_temp;
  

   //SDKLoadCode( JIAMI_CODE );
   
#if 0

   return TRUE;
#else
   ( void )file_seek( 0x60+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&rand_len, sizeof( U16 ));
   ( void )file_seek( 0xA0+2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&new_ap4_flag, sizeof( U16 ));
   rand_len ^= new_ap4_flag;
   rand_len += 256u;

   ( void )file_seek( 2+( rand_len >> 1 ), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U32 ));
   ( void )file_seek( 2+( MSB0( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB0( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB1( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB1( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB2( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB2( ap4_pw ) = pw_temp;
   ( void )file_seek( 2+( MSB3( temp )), FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB3( ap4_pw ) = pw_temp;

   MSB0( temp ) = 'A';
   MSB1( temp ) = 'F';
   MSB2( temp ) = 'I';
   MSB3( temp ) = 'Y';
   user_pw = pass_jiami( ap4_pw, temp );

   ( void )file_seek( 2+PASSWORD_POS1, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB0( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS2, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB1( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS3, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB2( temp ) = pw_temp;
   ( void )file_seek( 2+PASSWORD_POS4, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&pw_temp, sizeof( U8 ));
   MSB3( temp ) = pw_temp;
   all_control_pw = pass_jiami( user_pw, temp );
#endif

   return TRUE;
   
}
*/
	
#pragma CODE = FIND_PASS_03_CODE

U32 jiami( U32 shuju, U32 mima )
{
   U8 byte, i;
  
   i = 4 ;
   shuju ^= mima;
   do
   {
      byte = shuju >> 24;
      shuju <<= 8;
      shuju += byte;
      shuju ^= mima;
   }
   while( --i );
   return shuju;
}

// the function is read all control news;
void new_read_all_control_news( void )
{
   //U8 all_control_news[ 108 ];
   U8 i;
   //U16 new_ap4_flag, rand_len;
   U32 ap4_pw, user_pw, temp_01,temp_02;
   U8 pw_temp;

   U32 *temp = ( U32 *)&Buf;
   
   SDKLoadCode(FIND_PASS_01_CODE);
   ap4_pw = read_new_ap4_password_01();
   SDKLoadCode(FIND_PASS_02_CODE);
   temp_02 = read_new_ap4_password_02();
   /* 
   MSB0( temp_01 ) = 'A';
   MSB1( temp_01 ) = 'F';
   MSB2( temp_01 ) = 'I';
   MSB3( temp_01 ) = 'Y';
 //*/  
   temp_01 = 0x41464959;

   //SDKLoadCode(FIND_PASS_03_CODE);
   user_pw = jiami( ap4_pw, temp_01 );
   all_control_pw = jiami( user_pw, temp_02 );
   
   
 	//SDKLoadCode( JIAMI_CODE );
   ( void )file_seek( all_control_addr, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Buf, 512);
  // SDKLoadCode(FIND_PASS_03_CODE);
   for( i = 0; i <( 512 / 4 ); i++ )
    //for( i = 0; i <27; i++ )
   {
      *temp = jiami( *temp, all_control_pw );
      temp++;
   }
   
/*
   memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control, all_control_news, 17 );
   memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, &all_control_news[ 17 ], 30 );
   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Min, &all_control_news[ 59 ], sizeof( U16 )* 2 );
   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range, &all_control_news[ 63 ], sizeof( News_Index_Range ));
   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Min, &all_control_news[ 87 ], sizeof( U16 )* 2 );

   if( All_Control.Version >= 6 )
   {
      Encrypt_Key = all_control_news[ 91 ];
   }

   if( All_Control.Version >= 9 )
   {
      memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range_2, &all_control_news[ 98 ], sizeof( Record_Range ));
   }*/
/*
   else
   {
      Ap4_Record_Range_2.Record_Index_Min = 0;
      Ap4_Record_Range_2.Record_Index_Max = 0;
      Ap4_Record_Range_2.Play_Index_Min = 0;
      Ap4_Record_Range_2.Play_Index_Max = 0;
   }
*/
  // Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
   //ap4_AllControl_MaskGlobal = All_Control.Mask_Gloablext;

   //speical
//   b_search_speical_control_remote = FALSE;
//   memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap3_Mobile_Range, &all_control_news[ 51 ], sizeof( News_Ap3_Mobile_Range ));

#if 0//( AP4_HEAD_ENCRYPT == DISABLE )
   /*if( !Ap3_Mobile_Range.Ap3_Index_Min
      &&( Ap3_Mobile_Range.Ap3_Index_Max == 9 ))*/
   if( Ap3_Mobile_Range.Ap3_Index_Min || Ap3_Mobile_Range.Ap3_Index_Max 
      || Ap3_Mobile_Range.Mobile_Coor_Min || Ap3_Mobile_Range.Mobile_Coor_Max
      || Index_Range.Resv_Index_Min || Index_Range.Resv_Index_Max )
      b_search_speical_control_remote = TRUE;
#endif   //#if( AP4_HEAD_ENCRYPT == DISABLE )

	//SDKLoadCode(PRINT_CODE);
    //print_news();

}



#pragma CODE = 	CLEANSTRUCT_CODE

void cleanStruct(void)
{
	scan_oid_disable();
	  Kbd_set_evt_disabled();
      //SendString("Search\r\n");
     // ap3_rand_init();
      init_array_v();
	 // clear_bigflag();
	  clear_smallflag();
//	  flag_gendu = 0;
     News_Control.Type_Control = NO_TYPE;
		
	  Ap4_Record_Range_2.Record_Index_Min = 0;
      Ap4_Record_Range_2.Record_Index_Max = 0;
      Ap4_Record_Range_2.Play_Index_Min = 0;
      Ap4_Record_Range_2.Play_Index_Max = 0;
}

#pragma CODE = 	REMOTE_CHANGEPRO_CODE
void remote_change_project(void)
{
#ifdef TX_DEBUG
	SendString("\r\nremote_change_project!");
#endif
		bRmoteExitAp4 = FALSE;
	    prev_task=C_OID;
//		bak_to_news();
		file_close();
		switch(remote_to_mode)
			{
#if 1			
				case PLAYER_APPLI :
				//CommonLoadCode(C_MP3);
				readyGoNewPorject(C_MP3);
				break;
#endif				
#if 1					
			case PASTER_APPLI:
				//CommonLoadCode(C_REC_PASTER);
				readyGoNewPorject(C_REC_PASTER);
				break;
#endif				
#if (REMOTE_DIC == ENABLE)						
			case DIC_APPLI:	
				//CommonLoadCode(C_DIC);
				readyGoNewPorject(C_DIC);
             break;
#endif					 
			}
}
#endif  //#if( AP3 == ENABLE )

#endif  //_search_c_
