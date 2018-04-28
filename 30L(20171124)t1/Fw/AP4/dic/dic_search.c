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

#pragma DATA = DIC_SEARCH_DATA
#pragma CODE = DIC_SEARCH_CODE
#ifndef _search_c_
#define _search_c_

//_____  I N C L U D E S ___________________________________________________

//#include "config.h"
#include "stdio.h"
#include <string.h>

#include "dic_search.h"
//#include "search_game.h"
#include "dic_remote.h"
//#include "player_ap3.h"
//#include "file.h"
//#include "navigation.h"
//#include "unicode.h"

//#include "mail.h"
//#include "mail_evt.h"
#include "dic_kernel.h"
//#include "com_appli.h"
//#include "com_var.h"
//#include "com.h"
#include "conf_oid.h"
#include "dic_ap3.h"
//#include "string.h"
//#include "srv_player.h"
//#include "search_task.h"
//#include "mmi_player.h"
//#include "stdlib.h" // Contains Funktion "rand()"

#include "system.h"
#include "oid.h"
#include "power.h"             // Power module definition
#include "com.h"
//#include "mmi_player.h"
//#include "player_task.h"
//#include "power_task.h"
//#include "int_drv.h"
//#include "mmi_startup.h"
#include "MP3_API.h"    // MP3 ApplicationLayer header file
#include "keypad.h"
//#include "BinaryImage.h"
#include "dic_file.H"
#if 1//( AP3 == ENABLE )
//extern   xdata    AL_Variables     	varAl;

//_____ M A C R O S ________________________________________________________

//define coordinate range

//extern   __X  Bool       		 index_flag;

 //_MEM_TYPE_SLOW_  U16                  Current_Index;
//#define Current_Index                           u16_current_index


 _MEM_TYPE_SLOW_  U16                  Temp_Control_num;
 _MEM_TYPE_SLOW_  U16                  Temp_Ap3_file_num;
 _MEM_TYPE_SLOW_  U16                  Temp_Search_record_value;
 _MEM_TYPE_SLOW_  U8         	        Remote_Search_first_Ap3_file_num;

 _MEM_TYPE_SLOW_  U8                   State_Search;

 _MEM_TYPE_SLOW_  U32                  Seed;

 _MEM_TYPE_SLOW_ SEARCH_STATUS 			Search_Record;

 _MEM_TYPE_SLOW_  U8                   first_go_ap4;
 //_MEM_TYPE_SLOW_  U32 					Key_encrypt_mp3;

_MEM_TYPE_SLOW_   U8					flag_ap4_play_pause;
 _MEM_TYPE_SLOW_  U32                  size_current_ap4;
 //_MEM_TYPE_SLOW_ U32 file_size_count;
 //_MEM_TYPE_SLOW_ U32 play_file_size;


  _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;
  _MEM_TYPE_SLOW_  U32				   mp3_over_clock;

//_____ D E F I N I T I O N ________________________________________________

_MEM_TYPE_SLOW_  Action          Current_Act;
_MEM_TYPE_SLOW_  U8              Point_Write;
_MEM_TYPE_SLOW_  U8              Point_Read;
_MEM_TYPE_SLOW_  Speech          Array_V[ Length_v ];
_MEM_TYPE_SLOW_  U8              repeat_point;
_MEM_TYPE_SLOW_  U32             ap4_qa_addr=0;
_MEM_TYPE_SLOW_  U32             ap4_qa_addr_bak=0;
_MEM_TYPE_SLOW_  U32             actAddressBak;

_MEM_TYPE_SLOW_	  U32 					all_control_addr;
_MEM_TYPE_SLOW_	  U32 					 all_control_pw;
//***********************************************
#define AP3_MAX_RANGE_RAND       4       // 32 files
_MEM_TYPE_SLOW_  U8              g_ap3_rand_tab[ AP3_MAX_RANGE_RAND ];
//***********************************************
extern Bool g_b_search_on;
extern U32		swap_code_flag;
extern unsigned long free_space,wav_space;
//extern unsigned int systerm_flag;
extern unsigned int wav_action;
extern Bool OID_PLAY;
//#define AP4_ENCRYPT              ENABLE
#define AP4_ENCRYPT              DISABLE


//const U8 DIC_MODE[]="dictionary.mp3";
//const U8 BATTERY_LOW_FILE[] = "batterylow.mp3";
//const U8 POWER_OFF_FILE[] = "poweroff.mp3";

//======================================================



#define MIN_LENGTH  (1UL*1024*1024)
#define PCM_BUF_SIZE 505    // It can only be 505     
#define ADPCM_BUF_SIZE      ((PCM_BUF_SIZE-1)/4+2)          // word
#define ADPCM_BUF_CNT       6

//int PCM_CACHE_Buf[PCM_BUF_SIZE];
//__X int ADPCM_Buf[ADPCM_BUF_SIZE];
//__X int ADVACE_ADPCM_Buf[ADPCM_BUF_CNT][ADPCM_BUF_SIZE];
//__X int PCM_Buf1[PCM_BUF_SIZE];
//__X int PCM_Buf2[PCM_BUF_SIZE];
//__X int NotifyFlag, NotifyError;
//int *NotifyBuf;

char dir_path[128];


//=================================================

//#if( AP4_ENCRYPT == ENABLE )
//#define DECODE_NORMAL_FILE       ENABLE
//#define DECODE_NORMAL_FILE       DISABLE
//#endif   //#if( AP4_ENCRYPT == ENABLE )
//extern unsigned int vol_idx;
//extern const int VolTbl[7];
//extern unsigned int volume_inc_flag;
//extern unsigned int prev_task;
//extern _MEM_TYPE_SLOW_ U32 ScriptCurrentAddress;
//extern _MEM_TYPE_SLOW_ U32 ScriptCurrentAddress_bak;
extern    _MEM_TYPE_SLOW_   U8                    ADBVersion;   
//extern unsigned long resv_size;
//_____ D E C L A R A T I O N ______________________________________________
void  judge_coor                    ( void );
Bool  search_twobytes_coordinate    ( U32 addr, U16 number );
void read_news(void);
void  read_book_news                ( U16 num_book );
void  read_control_mode_news        ( U16 num_control );
void  read_page_news                ( U16 num_page );
void  read_sgdb                     ( void );
void  repeat_open_file              ( void );
void  speech_play_end               ( void );
Bool  ap3_rand_table_is_full        ( U8 number );
void  play_mp3_from_addr			(U32 addr,U8 flag,U8 led_flag);
//***********************************************

//#define SIZE_G_BUF 128
//unsigned char g_buf[SIZE_G_BUF];

Bool                  g_b_player_on;
//extern __X U32  g_timer_tick;

Bool g_b_search_on = FALSE;
Bool   from_control_mode = FALSE;
Bool   oid_control_flag = FALSE;
//Bool flag_wav_play;
//extern __X  U16         rp_flag;
unsigned int play_wav_num;
//extern Bool dic_play;
_MEM_TYPE_SLOW_  U16    code_flag;

extern   Bool     g_b_search_record;
extern unsigned int current_wav_num;
//extern unsigned int bak_current_index;
extern FCFILE __X *	fat_g_cur_file;
//extern   _MEM_TYPE_SLOW_   U16            remote_index_tab[ REMOTE_INDEX_MAX ];

//extern void rp_task(void);
void search_task( void );
Bool adb_index(void);
Bool player_music_start( void );
void openNewAdbFile(void);
extern void readyGoNewPorject(unsigned int subProject);
//extern void judge_op(void);
//extern void SpeakDetect(void);
/////////////////////////////////////////////////////////
Bool fix_index(void);
Bool nostop_index(void);
void interrupt_play(void);
//***********************************************
//Bool WavPlayStart(void);
//void Wav_Key_Process(unsigned int Key_Code);
///Bool WavDel(void);
//void wav_process(void);
//Bool find_wav(Bool flag);
//extern void save_wav_num(void);
extern void DAC_Stereo_to_Mono(unsigned Option);
extern void search_evt( void );
void AudioMP3FileOpenFirst(char *file);
MP3API_STATUS AudioMP3FileCloseAfter( void );
Bool judge_password(void);
void new_read_all_control_news( void );
//extern Bool FSCreateBuf( FCFILE *pfile );
//_____ D E C L A R A T I O N ______________________________________________
//ImageDeclareType(POWER_SYSTEM_OFF_CODE);
ImageDeclareType(FS_CREATE_BUF_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(MP3_COMMON_CODE);
ImageDeclareType(JIAMI_CODE);
ImageDeclareType(FIND_PASS_03_CODE);
ImageDeclareType(FIND_PASS_01_CODE);
ImageDeclareType(FIND_PASS_02_CODE);
//===========================================================================
//! Stop search task
//!
//void search_task_stop( void )
//{
//   g_b_search_on = FALSE;
//}
void AudioMP3FileOpenFirst(char *file)
{
	FS_ERR fs_err;
	if(OID_PLAY){
			AudioMP3Handle.FSHandle = fat_g_cur_file;
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
//the function is realize the search

//the function is realize the search
void search_evt( void )
{
	//unsigned int	buf_offset = 0;
	//unsigned char	t;
	//Bool flag = false;
	//U16 temp = 0;
   if( index_flag )
   	{
   		index_flag = FALSE;
		//sysup_opstart();
	  clkup_time(TRUE);
	  g_b_autopoweroff_forbidden = TRUE;
	  //SendString("\r\njudge_coor\r\n");
	  judge_coor();
   	}
   switch( State_Search )
   {
      case Open_File_Search:
//	  SystemClock(FEQ_60M);

    //  first_go_ap4 = 1;
      scan_oid_disable();
	  Kbd_set_evt_disabled();
     SendString("\r\nSearch\r\n");
//      ap3_rand_init();
      init_array_v();
	  g_b_player_on=FALSE;
//	  swap_code(FILE_EXTERN_CODE_FLAG);
    //  Get_File_Name( (unsigned char *)AP4, Current_Record.Ap3_num );
//      Get_File_Name( (unsigned char *)ADB,0 );
//	  if(0xFF==open_file(curAdbFile))
//	  	{
//		 SendString("\r\nno open adb!\r\n");
//		  CommonLoadCode(C_OID);

//		 return;
//	  	}

	  	openNewAdbFile();
 		SendString("\r\nopen adb ok!\r\n");
	 // if(!adb_file_init())
	//  	return;
	 // read_news();
//	  state_search_to_ap3();
//      break;

//      case Ap3_Search:
	
      ap3_rand_init();
      Search_Record.value = 0;
 //     SDKLoadCode(SEARCH_READ_CODE);
	 //  read_all_news();

   	  SDKLoadCode(JIAMI_CODE);
   	  if(judge_password()){
   	 	//SDKLoadCode(FIND_NEWS_PASS_CODE);
   		//read_new_ap4_password();
   		SDKLoadCode(FIND_PASS_03_CODE);
		new_read_all_control_news();
   		}
  	 else{
	 	
	 	  //file_close();
		  //goto newOpen;
   		}
	   read_news();

//	  	   Current_Special_Action.Battery_Low_Speech = ULongLE2BE(Current_Special_Action.Battery_Low_Speech);
//		 SDKLoadCode(REMOTE_TASK_INIT_CODE);

//		 swap_code(REMOTE_TASK_INIT_CODE_FLAG);
         remote_task_init();
         //RemoteBaseAddress = All_Control.Point_Remote;
         init_remote();
		 if(!bHaveRemote){
		 	file_close();
#ifdef TX_DEBUG
		SendString("\r\nremote err");
#endif			
			readyGoNewPorject(C_OID);
		 	}
#ifdef TX_DEBUG
		SendString("\r\nADBVersion=");
		send_hex_u8(ADBVersion);
#endif
		 find_noact_mp3();
		 dic_start_init();
		 (void)dic_search_init();
         scan_oid_enable();
		 Kbd_set_evt_enabled();
         state_search_to_play();
 		 g_b_autopoweroff_forbidden = TRUE;
      break;

      case Idle_Search:
	  if( !g_b_player_on )
      {
         if( Point_Read == Point_Write )
         {
            speech_play_end();
         }
         state_search_to_play();
      }

      break;

	  case Wait_Goon_Play:
	  break;
	  
      case Play_Search:
	  state_search_to_idle();
      if( Point_Write != Point_Read )
      {
      	 //clkup_time(TRUE);
		  
         if( player_ap3_start( Array_V[ Point_Read ].Addr_Speech, Array_V[ Point_Read ].Flag_Speech ))
         {
            state_search_to_idle();
         }
         Point_Read++;
      }
      else
      {
         speech_play_end();
      }
      break;
	  
   }
 
}


//the fuction is to judge coordinate range
void judge_coor( void )
{

  // Search_Record.news_sys.flag_index = 0;
  // if(first_go_ap4)first_go_ap4=0;
  // if(nostop_index())return;

     if(adb_index())return;
 /*  
   if( g_b_search_record )
   {
//      ap3srvrec_reread_off();
//      fat_clusterlist_restore();
      g_b_search_record = FALSE;
      ap4_play_state = AP4_PLAY_ERROR;
   }
*/   
//   repeat_save();
	interrupt_play();
   init_array_v();
   state_search_to_play();
//   SDKLoadCode(REMOTE_TASK_CODE);
  // if(fix_index())return;
   remote_task();
//   bRmoteExitAp4 = FALSE;     
}



void speech_play_end( void )
{

/*  if(poweroff_falg)
  	{
  	    OP_CLOSE();
		file_close();
		
  		power_system_off();
  	}*//* 
  if( Search_Record.news_sys.repeat_flag )
    {
       Point_Read = 0;
	   return;
    }

  if(dic_play && !player_ap3_is_busy())
  	{
  		dic_voice_to_play();
  	}
*/

  if(1==first_go_ap4)
  	{
  		scan_oid_enable();
		Kbd_set_evt_enabled();
		//Search_Record.news_sys.flag_index = FALSE;
		first_go_ap4 = 0;
	//	bRemoteChangeAp3 = FALSE;
  	}
  	if( g_b_autopoweroff_forbidden && (State_Search == Idle_Search))
  		{
  			power_authorize_autopoweroff();
			g_autopoweroff_timer = Timer_set_timer(TIMER_MIN(AUTO_CLOSE_TIME));
  		}

	if((timer_get_time() >= g_autopoweroff_timer )&& !g_b_autopoweroff_forbidden)
	{
		 scan_oid_disable();
		 Kbd_set_evt_disabled();

 	    init_array_v();
 	//    poweroff_falg = TRUE;
//		swap_code(AP3_OPEN_FILE_CODE_FLAG);
//		swap_code(FILE_EXTERN_CODE_FLAG);
		tishi_mp3(FLAG_POWER_OFF_FILE);
		OP_CLOSE();
		file_close();
		//save_wav_num();
		//SDKLoadCode(POWER_SYSTEM_OFF_CODE);
		//power_system_off();
		CommonLoadCode(C_POWEROFF);
 	//	act_do(All_Control.Addr_Action_close,0);
	//	state_search_to_play();
	}

	 if(bRmoteExitAp4)
	 	{
	 		prev_task=C_DIC;
	 		switch(remote_to_mode)
	 			{
	 				case PLAYER_APPLI :
						//bak_to_news();
						file_close();
						CommonLoadCode(C_MP3);
						break;
					case PASTER_APPLI:
                        break;
               case AP3_APPLI :
			   			file_close();
						//ap4_mp3_flag |=BANK_FORM_MP3;
						b_back_from_mp3 = TRUE;
						CommonLoadCode(C_OID);
						break;
	 			}
	 	}

}




Bool player_ap3_start( U32 start_addr, U8 flag )
{
   U8 codec;
   ENCRYPT_DATA ap4_audio_var;
   //if( player_ap3_is_busy())
  // {
  //    return FALSE;
 //  }
   if( !file_seek( start_addr, FS_SEEK_SET ))
   {
      return FALSE;
   }
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&mp3_begin_add, sizeof( mp3_begin_add));
//   MSB0( file_size_count ) = ( U8 )file_getc();
//   MSB1( file_size_count ) = ( U8 )file_getc();
//   MSB2( file_size_count ) = ( U8 )file_getc();
//   MSB3( file_size_count ) = ( U8 )file_getc();
  
   if( !mp3_begin_add )
      return FALSE;
//   if( flag == 1 )
//      file_size_count += RemoteBaseAddress;
   if( !file_seek( (start_addr+4), FS_SEEK_SET ))
   {
      return FALSE;
   }
file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&play_file_size, sizeof( play_file_size));   
//   MSB0( play_file_size ) = ( U8 )file_getc();
//   MSB1( play_file_size ) = ( U8 )file_getc();
//   MSB2( play_file_size ) = ( U8 )file_getc();
//   MSB3( play_file_size ) = ( U8 )file_getc();


   if( play_file_size < 512*4 )
      return FALSE;

   ap4_audio_var.audio_var.begin_addr= mp3_begin_add;
   ap4_audio_var.audio_var.file_size = play_file_size;

   if( flag == 1 )
   {  
 ///*  
   	  if(( RemoteVersion >= 4 )
         &&( RemoteFlag & ENCRYPT_SPEECH_FLAG ))
      {
         if( RemoteEncryptKey > 0x07 )
            return FALSE;
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
  //*/    
  		//gOidXOR_Key = 0;
		mp3_begin_add += RemoteBaseAddress;
   }
   else{
   	 gOidXOR_Key = 0;

	if(ADBVersion >= 3){
		if(dic_g_main_info.flag && DIC_MP3_ENCRY == 1){
	 			gOidXOR_Key = ap4_audio_var.encrypt[ (U8)dic_g_main_info.mp3Key ];
			}
		}

   	}
//   if(file_size_count>=size_current_ap4)return;
	// gOidXOR_Key = 0;
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
      file_read_buf(( U8 _MEM_TYPE_SLOW_ *)store_addr, leng_data );
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


//void repeat_save( void )
//{
//   repeat_point = Point_Write;
//}

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


U16 rand( void )
{
	Seed = Seed * 1103515245 + 12345;
	return ( U16 )( Seed / 65536 )% 32768;
}


void srand( U32 s )
{
	Seed = s;
}


//the function is deal with the action 
void voice_play_number( U32 address )
{
   U8 _MEM_TYPE_SLOW_ i, j, m;
   U32 _MEM_TYPE_SLOW_ Iaddress;
   U16 _MEM_TYPE_SLOW_ text_length;

   if( !Current_Act.act_info.speech_info.Num_Voice )
      return;
   //*address += ( U32 )sizeof( Action );
   //ap3_rand_init();
   srand( timer_get_time());
   for( i = 0; i < Current_Act.Num_Play; i++ )
   {
  //    Iaddress = address + ( U32 )sizeof( Action );
        Iaddress = address + ( U32 )5;
	 
      if( Current_Act.act_info.speech_info.Mode_Play == Random_Play )
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
      }
      else// if( Current_Act.act_info.speech_info.Mode_Play == Order_Play )
      {
         m = i;
         if( m >= Current_Act.act_info.speech_info.Num_Voice )
            m %= Current_Act.act_info.speech_info.Num_Voice;
      }


      switch( Current_Act.Type )
      {
         case Type_Play:
         case Type_play_Reread:
         case Type_play_Repeat:
         case Type_Play_Sequence:
         Iaddress += ( U32 )m * 9;
         break;

         case Type_Play_Title:
         case Type_play_Caption:
         read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&text_length, ( U16 )sizeof( U16 ));
/*		 
         switch( All_Control.Version )
         {
            case 2:
            Iaddress += ( U32 )text_length + 2 +( U32 )m * 9;
            break;
         }
*/		 
         break;
      }
      write_media_addr( Iaddress, 0 );
   }

   /*if(( Current_Act.Type == Type_Play_Title)
      ||( Current_Act.Type == Type_play_Caption ))
   {
      Iaddress = *address + ( U32 )sizeof( Action );
      //read_data_from_nf( Iaddress, Act_Caption[ Length_Caption - 1 ], ( U16 )sizeof( Act_Caption[ Length_Caption - 1 ]));
   }*/
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





void read_news(void)
{
	//(void)file_seek( 0, FS_SEEK_SET );
	//(void)file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&dic_g_main_info , sizeof( dic_g_main_info ));
	//if(all_control_addr == 0){
	memcpy(( U8 _MEM_TYPE_SLOW_ *)&dic_g_main_info.word_table_addr,&Buf[0],sizeof( dic_g_main_info ));
	RemoteBaseAddress = dic_g_main_info.remote_addr;
		//}
	//else{
	//	}
#ifdef TX_DEBUG
	SendString("\r\nword_table_addr=");send_hex_u32(dic_g_main_info.word_table_addr);
	SendString("\r\nchar_num=");send_hex_u16(dic_g_main_info.char_num);
	SendString("\r\ntotal_words_count=");send_hex_u32(dic_g_main_info.total_words_count);
	SendString("\r\nword_info_addr=");send_hex_u32(dic_g_main_info.word_info_addr);
	SendString("\r\nremote_addr=");send_hex_u32(dic_g_main_info.remote_addr);
	SendString("\r\nflag=");send_hex_u16(dic_g_main_info.flag);
	SendString("\r\nmp3Key=");send_hex_u16(dic_g_main_info.mp3Key);


#endif
}




//======================================================================
Bool adb_index(void)
{
/*
	if((Current_Index==INDEX_NEXT_MP3)||(Current_Index==INDEX_PREV_MP3)){
	return TRUE;
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
				 //AudioMP3SetVol(VolTbl[vol_idx]);
			}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
		 if(vol_idx > 0)
			{
			     vol_idx--;
				 //AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}
	 if((Current_Index>=INDEX_CHANGE_AP4_MIN)&&(Current_Index<=(INDEX_CHANGE_AP4_MAX))){
	 	interrupt_play();
		//b_back_from_mp3 = TRUE;
		b_back_have_index = TRUE;
		bak_current_index = Current_Index;
		bRmoteExitAp4 = TRUE;
		remote_to_mode = AP3_APPLI;
		state_search_to_play();
		return TRUE;
	 	}

		if((Current_Index==INDEX_GO_MP3)||(Current_Index==INDEX_PLAY_MP3)){
		interrupt_play();
		file_close();
		CommonLoadCode(C_MP3);
		return TRUE;
		}
	
	/*
	if(Current_Index==INDEX_STOP_MP3){
		return TRUE;
		}
	if(Current_Index==INDEX_NEXT_MP3){
		return TRUE;
		}
	if(Current_Index==INDEX_PREV_MP3){
		return TRUE;
		}*/
	if(Current_Index==INDEX_START_RECORD){
        //g_b_record = TRUE;
        //interrupt_play();
		//wav_action = WAV_RECORD_FLAG;
		kbd_begin_write(KBD_KEY_RELEASED, KEY_REC);
		return TRUE;
		}
	if(Current_Index==INDEX_PLAY_RECORD){
	    //g_b_record = TRUE;
		//wav_action = WAV_PLAY_FLAG;
		kbd_begin_write(KBD_KEY_LONGPRESSED, KEY_REC);
		return TRUE;
		}
	return FALSE;
}


//======================================================================
Bool nostop_index(void)
{
/*
	if((Current_Index==INDEX_NEXT_MP3)||(Current_Index==INDEX_PREV_MP3)){
	return TRUE;
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
				 //AudioMP3SetVol(VolTbl[vol_idx]);
			}
		 return TRUE;
		}
	if((Current_Index==INDEX_DEC_VOL))
		{
		 if(vol_idx > 0)
			{
			     vol_idx--;
				 //AudioMP3SetVol(VolTbl[vol_idx]);
			}
         return TRUE;
		}
	return FALSE;
}

Bool fix_index(void)
{
	if(Current_Index==INDEX_GO_AP4){
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_OID);
		return TRUE;
		}	
	if(Current_Index==INDEX_GO_MP3){
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		CommonLoadCode(C_MP3);
		return TRUE;
		}	
	if(Current_Index==INDEX_START_RECORD){
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		wav_action = 0x1111;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}

	if(Current_Index==INDEX_STOP){
		if( player_ap3_is_busy())
			 player_ap3_stop();
		//tishi_mp3(6);
		return TRUE;
		}
	
	if(Current_Index==INDEX_PLAY_RECORD)
		{
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		wav_action = 0x2222;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}
	/*
	if(Current_Index==INDEX_DEL_RECORD)
		{	
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		if(WavDel()){
			tishi_mp3(7);
			}
		else{
			tishi_mp3(6);
			}
		 return TRUE;
		}
*/
/*
	if((Current_Index>=INDEX_RECORD_MIN)&&(Current_Index<=INDEX_RECORD_MAX))
		{
		if( player_ap3_is_busy())
			 player_ap3_stop();
		file_close();
//		OP_CLOSE();
		//ap4_mp3_flag |= BANK_FORM_MP3;
		b_back_from_mp3 = TRUE;
		wav_action = Current_Index;
		CommonLoadCode(C_REC_PASTER);
		return TRUE;
		}	
*/
	if((Current_Index>=INDEX_FIRST_VOL)&&(Current_Index<=(INDEX_FIRST_VOL+6)))
		{
		 vol_idx=Current_Index-(U16)INDEX_FIRST_VOL;
		 //AudioMP3SetVol(VolTbl[vol_idx]);
//		 init_array_v();
//		 act_do(All_Control.Addr_Antion_key, 0);		 
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


//==============================================================
void dic_player_task(void)
{
   if( g_b_player_on )
   {
      if( ! CheckPlaying(FALSE))
      {
//		   MP3_PrevInit();
//         player_volume_off();
//         player_off();
           g_b_player_on = FALSE;
//         srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
//         mail_send_event( EVT_PLAYER, PLAYER_EOF )
		 //vol_dec();
// 		 DAC_FadeOut();
//		 OP_CLOSE();
         AudioMP3Stop();
         AudioMP3Uninit();
		 op_start_timer();

      }
   }
//***********************************************
//***********************************************
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


Bool player_music_start( void )
{
   tAudioErr   Audio_err; 
   OID_PLAY = TRUE;
//***********************************************
	AudioMP3FileOpenFirst(0);
   Audio_err = AudioMP3Init1k0(0);
    if(Audio_err != AUDIO_ERROR_OK){
 		SendString("\r\n314");		
		goto play_fail;
    	}

//   g_b_player_on = TRUE;
   
#if 0

#endif
	vol_idx_bak = VOL_NUM;
	flag_wav_play = FALSE;
	SpeakDetect(TRUE);
	//DAC_Stereo_to_Mono(1);
    Audio_err = AudioMP3Open((char *)"0");
      //Audio_err =  AudioMP3FileOpen( (char *)0 );
   if( Audio_err == AUDIO_ERROR_OK)
   {    
		
		//DAC_FadeIn();
         Audio_err = AudioMP3Play();
       if(Audio_err != AUDIO_ERROR_OK)
       	{
 //      		g_b_player_on = FALSE;
 		 	SendString("\r\n358");
    		goto play_fail;
       	}
	   
//	  	player_volume_on();
      	power_forbid_autopoweroff();
		g_b_player_on = TRUE;
		
//	    SendString("\r\nplay ok!");
      	op_stop_timer();
		clkup_time(FALSE);
      return TRUE;
   }
play_fail:   
 	SendString("\r\nplay fail!\r\n");
	op_start_timer();
   return FALSE;
//***********************************************
}

/*
Bool player_music_start( void )
{
   tAudioErr   Audio_err;
   OID_PLAY = TRUE;
//***********************************************
#if 1  
   SendString("\r\nstart_addr:file_size_count=");
   send_hex_u32(mp3_begin_add);
   SendString(":");
   send_hex_u32(play_file_size);
#endif 
     //SDKLoadCode(DAC_COMMON_CODE);
	//SDKLoadCode(MP3_COMMON_CODE);
	AudioMP3FileOpenFirst(0);
   Audio_err = AudioMP3Init(8);
    if(Audio_err != AUDIO_ERROR_OK){
		SendString("\r\n988");	
    	return FALSE;
    	}

		//DAC_HeadPhone_Volume(0);
		//judge_op();
		//vol_inc();
		
   //Audio_err = AudioMP3OpenOIDFile(mp3_begin_add,play_file_size,Key_encrypt_mp3);
   Audio_err = AudioMP3FileOpen((char *)"0");
   if( Audio_err == AUDIO_ERROR_OK)
   {    

		vol_idx_bak = VOL_NUM;
		flag_wav_play = FALSE;
		SpeakDetect(TRUE);
	    DAC_Stereo_to_Mono(1);
	//	DAC_FadeIn();
         Audio_err = AudioMP3Play();
       if(Audio_err != AUDIO_ERROR_OK)
       	{
		     SendString("\r\n1009=");
			 send_hex_u8(Audio_err);
    		return FALSE;
       	}
		g_b_player_on = TRUE;
      	power_forbid_autopoweroff();
	  op_stop_timer();
	  clkup_time(FALSE);
	  	SendString("\r\n1015");	
      return TRUE;
   }
   SendString("\r\n1018");	
   return FALSE;
//***********************************************
}

*/
void player_ap3_stop( void )
{
  tAudioErr   Audio_err;
//   player_volume_off();
//   player_stop_stream();
 //  player_codec_stop();
   g_b_player_on = FALSE;

//   srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;

   
//   GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
//   GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
//   bSpeechEnd = TRUE;
//   player_task_stop();
   //speech_status = SPEECH_IDLE;
//   	vol_dec();
//   	 DAC_FadeOut();
//     OP_CLOSE();
  	 Audio_err = AudioMP3Stop();
	Audio_err = AudioMP3Close();
	Audio_err = AudioMP3Uninit();
	   AudioMP3FileCloseAfter();
	   power_authorize_autopoweroff();


   op_start_timer();
   clkup_time(TRUE);
//   swap_code(FILE_CODE_FLAG);
//    if(Audio_err != AUDIO_ERROR_OK)
//        SendString(( char *)"The ap4 play uninitial fail.");
}

void interrupt_play(void)
{
	if(player_ap3_is_busy()||(b_play_pause)){
		SDKLoadCode(DAC_COMMON_CODE);
		SDKLoadCode(MP3_COMMON_CODE);
		player_ap3_stop();
		init_array_v();
		}
}

Bool player_ap3_is_busy( void ) 
{
   //return g_b_play_ap3 && g_b_player_on;
   return ( g_b_player_on );
}
#pragma CODE = JIAMI_CODE
Bool judge_password(void)
{
   U16 new_ap4_flag;
   U32 temp;
  // Search_Record.value = 0;
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

#ifdef TX_DEBUG
   SendString((char *)"\r\nall_control_addr=");
   send_hex_u32(all_control_addr);   
#endif   
   return TRUE;
}

#pragma CODE = FIND_PASS_01_CODE

U32	read_new_ap4_password_01( void )
{
   U16 new_ap4_flag, rand_len;
   U32 ap4_pw, temp;
   U8 pw_temp;

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
#define  PASSWORD_POS1        0x9E  //0x5C  //0x40//
#define  PASSWORD_POS2        0x4F  //0x7B  //0x75//
#define  PASSWORD_POS3        0xA1  //0x9D  //0x50//
#define  PASSWORD_POS4        0x19  //0x38  //0x85//
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
#if 0//def TX_DEBUG
   SendString((char *)"\r\nshuju:mima=");
   send_hex_u32(shuju);   
   SendString((char *)":");
   send_hex_u32(mima); 
#endif     
   shuju ^= mima;
#if 0//def TX_DEBUG
	  SendString((char *)"\r\nshuju_1=");
	  send_hex_u32(shuju);	 
#endif  

   do
   {
      byte = shuju >> 24;
#if 0//def TX_DEBUG
	  SendString((char *)"\r\nbyte=");
	  send_hex_u8(byte);	 
#endif  	  
      shuju <<= 8;
#if 0//def TX_DEBUG
	  SendString((char *)"\r\nshuju_2=");
	  send_hex_u32(shuju);	   
#endif  

      shuju += byte;
#if 0//def TX_DEBUG
			SendString((char *)"\r\nshuju_3=");
			send_hex_u32(shuju);	 
#endif

      shuju ^= mima;
#if 0//def TX_DEBUG
		 SendString((char *)"\r\nshuju_4=");
		 send_hex_u32(shuju);	  
#endif

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
#ifdef TX_DEBUG
   SendString((char *)"\r\nap4_pw:temp_02=");
   send_hex_u32(ap4_pw);   
   SendString((char *)":");
   send_hex_u32(temp_02); 
#endif    
   /* 
   MSB0( temp_01 ) = 'A';
   MSB1( temp_01 ) = 'F';
   MSB2( temp_01 ) = 'I';
   MSB3( temp_01 ) = 'Y';
 //*/  
   temp_01 = 0x41464959;

   //SDKLoadCode(FIND_PASS_03_CODE);
   user_pw = jiami( ap4_pw, temp_01 );
#ifdef TX_DEBUG
   SendString((char *)"\r\nuser_pw=");
   send_hex_u32(user_pw);    
#endif    
   all_control_pw = jiami( user_pw, temp_02 );
#ifdef TX_DEBUG
	SendString("\r\nall_control_pw = ");send_hex_u32(all_control_pw);
#endif
   
 	//SDKLoadCode( JIAMI_CODE );
   ( void )file_seek( all_control_addr, FS_SEEK_SET );
   file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Buf, 512);
  ///* 
  // SDKLoadCode(FIND_PASS_03_CODE);
   for( i = 0; i <( 512 / 4 ); i++ )
    //for( i = 0; i <27; i++ )
   {
      *temp = jiami( *temp, all_control_pw );
      temp++;
   }
  // */
}

#endif  //#if( AP3 == ENABLE )

#endif  //_search_c_
