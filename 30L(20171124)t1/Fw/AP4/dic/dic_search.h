//!
//! @file search.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: search.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _SEARCH_H_
#define _SEARCH_H_

//#include "com_var.h"
#include "compiler.h"
#include "conf_sio.h"
#include "conf_kb.h"


#if( 1 )
//_____ M A C R O S ________________________________________________________


//define the caption length of all struct
#define Length_Caption              16
///define the length of the read data 
#define length_eight_coordiante     64
//define the number of the voice address 
#define Length_v                    64

//#define all function
#define state_search_to_open_file() do{State_Search = Open_File_Search;}while(0)
#define state_search_to_ap3()       (State_Search = Ap3_Search)
#define state_search_to_book()      do{State_Search = Book_Search;}while(0)
#define state_search_to_page()      do{State_Search = Page_Search;}while(0)
#define state_search_to_ctr_mode()  do{State_Search = Control_Mode_Search;}while(0)
#define state_search_to_idle()      do{State_Search = Idle_Search;}while(0)
#define state_search_to_play()      do{State_Search = Play_Search;}while(0)
#define state_search_to_error()     do{State_Search = Err_Search;}while(0)
#define state_search_to_goonplay()  do{State_Search = Wait_Goon_Play;}while(0)


#define MSK_COOR_MODE               0x03
#define IMMOBILITY_COOR_MODE        0x00
#define MOBILE_COOR_MODE            0x01
#define MIX_COOR_MODE               0x02
#define AUTO_FIRST_BOOK             0x04
#define AP3_TO_AEB_FLAG             0x08
#define SIMPLE_FLAG                 0x10
#define SEQUENCE_FLAG               0x20
#define DEFAULT_SEQUENCE_FLAG       0x40
#define ENCRYPT_AUDIO_DATA_FLAG     0x80

// define the state of the seacher
#define Start_Search                ( 0x10 )
#define Ap3_Search                  ( Start_Search + 1 )
#define Book_Search                 ( Start_Search + 2 )
#define Page_Search                 ( Start_Search + 3 )
#define Control_Mode_Search         ( Start_Search + 4 )
#define Idle_Search                 ( Start_Search + 5 )
#define Play_Search                 ( Start_Search + 6 )
#define Err_Search                  ( Start_Search + 7 )
#define Normal_Search               ( Start_Search + 8 )
#define Open_File_Search            ( Start_Search + 9 )
#define Null_state                  ( Start_Search + 10 )
#define Sequence_Search             ( Start_Search + 11 )
#define Wait_Remote_Do_Game         ( Start_Search + 12 )
#define Wait_Goon_Play				( Start_Search + 13 )

// define the type of the action
#define Type_Action                 ( 0x00 ) 
#define Type_Play                   ( Type_Action + 1 )
#define Type_Stop                   ( Type_Action + 2 )
#define Type_V_Big                  ( Type_Action + 3 )
#define Type_V_Small                ( Type_Action + 4 )
#define Type_Repeat_Game_Question   ( Type_Action + 5 )
#define Type_Prev_Game              ( Type_Action + 6 )
#define Type_Next_Game              ( Type_Action + 7 )
#define Type_Quit_Game              ( Type_Action + 8 )
#define Type_Go_Mp3                 ( Type_Action + 9 )
#define Type_Play_Title             ( Type_Action + 10 )
#define Type_play_Caption           ( Type_Action + 11 )
#define Type_play_Reread            ( Type_Action + 12 )
#define Type_play_Repeat            ( Type_Action + 13 )
#define Type_Play_Sequence          ( Type_Action + 14 )
#define Type_Repaet_Speech          ( Type_Action + 15 )
#define Type_Vol_Up                 ( Type_Action + 16 )
#define Type_Vol_Down               ( Type_Action + 17 )

#define Type_Script               	( Type_Action + 20 )

//***********************************************
//add bu xuanyu
#define Type_SelectBook          0x101
#define Type_SelectPage          0x102
#define Type_SelectControl       0x103
//***********************************************


#define Order_Play                  ( 0x01 )
#define Random_Play                 ( Order_Play + 1 )

//#define   offsetof(struct_t,member)   ((unsigned int)(char   *)&((struct_t   *)0)->member) 

U16	UIntLE2BE(U16 v);
U32	ULongLE2BE(U32 v);
#if 0
void	endian_cast(U8 * v,U8 size);
#define	Endian_Cast(v)	endian_cast((U8 *)&(v),sizeof(v))
void	endian_assign16(U16 * dst,U16 * src);
void	endian_assign32(U32 * dst,U32 * src);
#endif
typedef struct
{
   U16   X0;
   U16   Y0;
   U16   X1;
   U16   Y1;
}Coor;

typedef struct
{
   U16   Type;
   union
   {
      U16   Num_Change_Mode;
      struct
      {
         U8    Num_Voice;
         U8    Mode_Play;
      }speech_info;
   }act_info;
   U8    Num_Play;
}Action;


typedef struct
{
   U32   Speech_Start_Address;
   U32   Speech_End_Address;
   U32   Text_Start_Address;
   U32   Text_End_Address;
}Action_Sequence;


#if 1
typedef union
{
   struct
   {
      // 0 :no anjian;1 : anjian
      unsigned flag_anjian            :1;
      //0 :no new index;1 new index;
      unsigned flag_index             :1;
      //
      unsigned flag_anjian_to_index   :1;
      //00:no contrl;01:single table; 10 :game table;
      //U16 mode_flag              :2;
      // 0: enable next key; 1: disable next key
      unsigned disable_next_key_flag  :1;
      
      // 01: control table; 10: default table; 11: background table;
      unsigned table_flag             :2;
      //0: no reread; 1: reread;
      unsigned reread_flag            :1;
      //0: no repeat; 1: repeat;
      unsigned repeat_flag            :1;
      
      //0: no sequence; 1: sequence;
      unsigned sequence_flag          :1;
      //0: sequence play is stop; 1: sequence play is start;
      unsigned sequence_is_on_flag    :1;
      //000: no exit flag; 001: poweroff sound start; 010: poweroff sound end;
      //011: search end sound start; 100: search end sound end;
      unsigned poweroff_flag          :4;
      //0: no change mode; 1:change mode
      unsigned ap3_aeb_flag           :1;
      //
      unsigned play_record_flag       :1;
   }news_sys;
   U16     value;
}SEARCH_STATUS;
#else
typedef union
{
   struct
   {

      //
      unsigned play_record_flag       :1;
      //0: no change mode; 1:change mode
      unsigned ap3_aeb_flag           :1;
        //000: no exit flag; 001: poweroff sound start; 010: poweroff sound end;
      //011: search end sound start; 100: search end sound end;
      unsigned poweroff_flag          :4; 
      //0: sequence play is stop; 1: sequence play is start;
      unsigned sequence_is_on_flag    :1;
      //0: no sequence; 1: sequence;
      unsigned sequence_flag          :1;   


      //0: no repeat; 1: repeat;
      unsigned repeat_flag            :1;
      //0: no reread; 1: reread;
      unsigned reread_flag            :1;
      // 01: control table; 10: default table; 11: background table;
      unsigned table_flag             :2;


      // 0: enable next key; 1: disable next key
      unsigned disable_next_key_flag  :1;
      //
      unsigned flag_anjian_to_index   :1;
       //0 :no new index;1 new index;
      unsigned flag_index             :1;  
      // 0 :no anjian;1 : anjian
      unsigned flag_anjian            :1;
   }news_sys;
   U16     value;
}SEARCH_STATUS;
#endif

extern _MEM_TYPE_SLOW_ SEARCH_STATUS Search_Record;


typedef struct
{
   U16   Version;
   U16   Mask_Gloablext;
   U16   Rang_X;
   U16   Rang_Y;
   U16   Excursion_Code;
   U16   Max_Code;
   U32   Point_Remote;
   U8    Flag_Xuanshuye;
   U32   Action_Xuanshu;
   U32   Point_Book_Table;
   U16   Num_Book;
   //U32   Point_First_Book;
   U32   Point_Gloablext_Coor;
   U32   Addr_Action_Open;
   U32   Addr_Action_close;
   U32   Addr_Antion_key;
   U32   Point_Special_Action;
}News_All_Control;


typedef struct
{
   U16   Normal_Index_Min;
   U16   Normal_Index_Max;
   U16   Book_Index_Min;
   U16   Book_Index_Max;
   U16   Page_Index_Min;
   U16   Page_Index_Max;
   U16   Control_Index_Min;
   U16   Control_Index_Max;
   U16   Remote_Index_Min;
   U16   Remote_Index_Max;
}News_Index_Range;

typedef struct
{
	U16  Record_Index_Min;
	U16  Record_Index_Max;
	U16  Play_Index_Min;
	U16  Play_Index_Max;	
}Record_Range;

typedef struct
{
   U32   Battery_Low_Speech;
}Special_Action;


typedef struct
{
   U16   Max_Index_TheBook;
   U32   Addr_Action_BookTable;
   U16    Caption_Book[ Length_Caption/2 ];
   U8    Flag_CurrentBook;
   U32   Point_GoTable;
   U32   Point_GOTable_Coordinate;
   U16   Num_GoTable;
   U32   Point_GameRightAnswer;
   U32   Point_SequenceSpeechTable;  //firmeware no used
   U32   Point_SequenceTextTable;    //firmeware no used
   U32   Point_ScritNumber;
}Book_Table;


typedef struct
{
   U32   Point_PageCoorTable;
   U32   Point_BackgroudCoor;
//   union
//   {
//      U16   Num_Sub;
   U16   Min_Index_ThePage;
//   }page_info;
   U16   Max_Index_ThePage;
   U32   Addr_Action;
   U16    Caption_Go[ Length_Caption/2 ];
   U32   Point_DefaultTab;
   U16   Num_DefaultTab;
   U32   Point_BackgroudTab;
   U16   Num_BackgroudTab;
   U32   Point_ControlTab;
   U16   Num_ControlTab; 
   U32   Action_NoVoice;
}Go_Table;

typedef struct
{
   U32   begin_addr;
   U32   file_size;
}AUDIO_VAR;

typedef union
{
   AUDIO_VAR   audio_var;
   U8          encrypt[ 8 ];
}ENCRYPT_DATA;

//define control mode
//#define SINGLE_TABLE              ( 0x01 )
//#define GAME_TABLE                ( SINGLE_TABLE + 1 )

// control type
#define SINGLE_TABLE_TYPE           ( 10 )
#define GAME_TYPE                   ( 11 )
#define DIC_TYPE                    ( 12 )
#define QA_TYPE                     ( 13 )
#define SCRIPT_GAME_TYPE			( 14 )
#define NO_TYPE					    ( 15 )

//define table
#define CONTROL_TABLE               ( 0x01 )
#define DEFAULT_TABLE               ( CONTROL_TABLE + 1 )
#define BACKGROUND_TABLE            ( DEFAULT_TABLE + 1 )

//the function is search single table or game table 
// or default table or background table
#define CONTROL_IMMOBILITY_COOR_STATUS       ( 1 )
#define CONTROL_MOBILE_COOR_STATUS           ( CONTROL_IMMOBILITY_COOR_STATUS + 1 )
#define CONTROL_MIX_COOR_STATUS              ( CONTROL_IMMOBILITY_COOR_STATUS + 2 )
#define DEFAULT_IMMOBILITY_COOR_STATUS       ( CONTROL_IMMOBILITY_COOR_STATUS + 3 )
#define DEFAULT_MOBILE_COOR_STATUS           ( CONTROL_IMMOBILITY_COOR_STATUS + 4 )
#define DEFAULT_MIX_COOR_STATUS              ( CONTROL_IMMOBILITY_COOR_STATUS + 5 )
#define BACKGROUND_IMMOBILITY_COOR_STATUS    ( CONTROL_IMMOBILITY_COOR_STATUS + 6 )
#define BACKGROUND_MOBILE_COOR_STATUS        ( CONTROL_IMMOBILITY_COOR_STATUS + 7 )
#define BACKGROUND_MIX_COOR_STATUS           ( CONTROL_IMMOBILITY_COOR_STATUS + 8 )
//***********************************************
//#define SEARCH_NEXT_PAGE_STATUS              ( CONTROL_IMMOBILITY_COOR_STATUS + 9 )
//***********************************************

#define REMOTE_TASK_INIT_CODE_FLAG				(0X0001)	
#define REMOTE_TASK_CODE_FLAG					(0X0002)
#define AP3_OPEN_FILE_CODE_FLAG					(0X0004)
#define GAME_INITIAL_CODE_FLAG					(0X0008)
#define GAME_SEARCH_1_CODE_FLAG					(0X0010)
#define GAME_Script_CODE_FLAG					(0X0020)
#define IMA_CODEC_CODE_FLAG						(0X0040)

#define DAC_CODE_FLAG							(0X0080)
#define AUADC_CODE_FLAG							(0X0100)
#define FILE_EXTERN_CODE_FLAG					(0X0200)
#define RP_RECORD_CODE_FLAG						(0X0400)
#define ADPCM_RECORD_FLAG						(0X0800)

#define CUSTER_CODE_FLAG						(REMOTE_TASK_INIT_CODE_FLAG|REMOTE_TASK_CODE_FLAG\
												|AP3_OPEN_FILE_CODE_FLAG|GAME_INITIAL_CODE_FLAG\
												|GAME_SEARCH_1_CODE_FLAG|GAME_Script_CODE_FLAG\
												|IMA_CODEC_CODE_FLAG|RP_RECORD_CODE_FLAG\
												|ADPCM_RECORD_FLAG)

#define SYSTEM_CODE_FLAG						(DAC_CODE_FLAG|AUADC_CODE_FLAG\
												|FILE_EXTERN_CODE_FLAG)

//-------------------------------------------------------------------------------------
enum
{
  NEXT_FILE,
  PREV_FILE
};

typedef struct
{
   U16    Caption_Control[ Length_Caption/2 ];
   U32   Point;
   U32   Addr_Action;
   U8    Type_Control;
   U16   Num_Control;
   U8    Flag;
}Control_Table;


typedef struct
{
   U32   Addr_Speech;
   U8    Flag_Speech;
}Speech;



//===================================================
#define AP3_APPLI   	1
#define PLAYER_APPLI   	2
#define PASTER_APPLI   	3
/*
//DEFINE  ALL BITS OF AP4_MP4_FLAG 
#define BANK_FORM_MP3       	0X8000
#define BANK_FORM_RP            0X4000
#define BANK_HAVE_INDEX     	0X2000
#define CURRENT_CONTROL_GAME 	0X1000

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

extern _MEM_TYPE_SLOW_  U32					 Current_Special_Action;
extern _MEM_TYPE_SLOW_  Action_Sequence      Current_Act_Sequence;
extern _MEM_TYPE_SLOW_  U32                  sequence_speech_address;
extern _MEM_TYPE_SLOW_  U32                  sequence_text_address;
extern _MEM_TYPE_SLOW_  U32                  sequence_speech_read_point;

extern _MEM_TYPE_SLOW_  U32                  Current_Index;
//#define Current_Index                           u16_current_index













extern _MEM_TYPE_SLOW_   U8					flag_ap4_play_pause;

extern _MEM_TYPE_SLOW_  U16                  Temp_Control_num;
extern _MEM_TYPE_SLOW_  U16                  Temp_Ap3_file_num;
extern _MEM_TYPE_SLOW_  U16                  Temp_Search_record_value;
extern _MEM_TYPE_SLOW_  U8         	        Remote_Search_first_Ap3_file_num;

extern  _MEM_TYPE_SLOW_  U8                   first_go_ap4;
extern  _MEM_TYPE_SLOW_  U32 					Key_encrypt_mp3;

//extern _MEM_TYPE_SLOW_ U32 file_size_count;
//extern _MEM_TYPE_SLOW_ U32 play_file_size;
extern _MEM_TYPE_SLOW_  U32                  size_current_ap4;
extern   _MEM_TYPE_SLOW_  U32				   mp3_over_clock;

extern   _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;

extern    _MEM_TYPE_SLOW_  U16                  Current_Mode;
extern  _MEM_TYPE_SLOW_  U32 				   Addr_Fanyi;  
extern  _MEM_TYPE_SLOW_  U8					   Num_Fanyi;
extern  _MEM_TYPE_SLOW_  U8						Num_Cur_Fanyi;

extern _MEM_TYPE_SLOW_  Action          Current_Act;
extern _MEM_TYPE_SLOW_  U8              Point_Write;
extern _MEM_TYPE_SLOW_  U8              Point_Read;

extern unsigned int ap4_mp3_flag;


// define need's variable
//_MEM_TYPE_SLOW_ U16 Current_Index;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 X_Coor;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 Y_Coor;
#define X_Coor                                  Current_Coor.X1
#define Y_Coor                                  Current_Coor.Y1

extern _MEM_TYPE_SLOW_  U8     State_Search;


extern _MEM_TYPE_SLOW_  U32    Seed;
extern _MEM_TYPE_SLOW_  U16    code_flag;



/*
extern const U8 NO_AP4_FILE[];
extern const U8 SYS_OPEN_FILE[];
extern const U8 BATTERY_LOW_FILE[];
extern const U8 POWER_OFF_FILE[];
extern const U8 RECORDSTART[];
extern const U8 RECORDEND[];
extern const U8 PLAYSTART[];
extern const U8 PLAYEND[];
extern const U8 DISKFULL[];
extern const U8 CALERR[];
extern const U8 DIC_MODE[];

*/




/// define the function
void  search_evt                    ( void );
void  init_array_v                  ( void );
Bool  act_do                        ( U32 address, Bool game_act_do_flag );
void  read_data_from_nf             ( U32 start_addr, U8 _MEM_TYPE_SLOW_ *store_addr, U16 leng_data );
//Bool  search_eightbytes_coordinate  ( U32 addr, U16 number );
void  remote_mp3_task               ( void );
void  write_media_addr              ( U32 address, U8 flag );
void  ap3_rand_init                 ( void );
void  voice_play_number             ( U32 address );
void  repeat_save                   ( void );
void  init_check_sd					(void);
Bool  player_ap3_is_busy(void);
Bool  player_ap3_start(U32 start_addr, U8 flag);
void  player_ap3_stop(void);
int CheckPlaying(Bool flag);
void  vol_dec(void);
void  vol_inc(void);

void  dic_player_task(void);
void  search_task_start(void);


#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
