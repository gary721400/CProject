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
#include "conf_sio.h"
#include "conf_kb.h"
#include "system.h"
#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________


//define the caption length of all struct
#define Length_Caption              16
///define the length of the read data 
#define length_eight_coordiante     64
//define the number of the voice address 
#define Length_v                    256//64

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
#define  MSK_EXT_INDEX              0x8000u
#define  MSK_SORT_FLAG              0x4000u
//------------------------------------
#define Order_Play                  ( 0x01 )
#define Random_Play                 ( Order_Play + 1 )
#define Remember_Play               ( Order_Play + 2 )
#define Linkwithmem_Play            ( Order_Play + 3 )

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
/*
typedef struct
{
   U16   Ap3_num;
   U16   Book_num;
   U16   Page_num;
   U16   Control_num;
}Record;
*/
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
      unsigned sx_play_flag          :1;
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
   U32   Rang_X;
   U32   Rang_Y;
   U32   Excursion_Code;
   U32   Max_Code;
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
   U32   Ap3_Min;
   U32   Ap3_Max;
   U32   Guding_Min;
   U32   Guding_Max;
   //U32	 Rev_Min;
   //U32   Rev_Max;
}Other_Range;


typedef struct
{
   U32   Normal_Index_Min;
   U32   Normal_Index_Max;
   U32   Book_Index_Min;
   U32   Book_Index_Max;
   U32   Page_Index_Min;
   U32   Page_Index_Max;
   U32   Control_Index_Min;
   U32   Control_Index_Max;
   U32   Remote_Index_Min;
   U32   Remote_Index_Max;

   U32	 Resv_Index_Min;
   U32   Resv_Index_Max;
}News_Index_Range;

typedef struct
{
	U32  Record_Index_Min;
	U32  Record_Index_Max;
	U32  Play_Index_Min;
	U32  Play_Index_Max;	
}Record_Range;

typedef struct
{
   U32   Battery_Low_Speech;
}Special_Action;


typedef struct
{
   U32   Max_Index_TheBook;
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
   U32   Min_Index_ThePage;
//   }page_info;
   U32   Max_Index_ThePage;
   U32   Addr_Action;
   U16    Caption_Go[ Length_Caption/2 ];
   U32   Point_DefaultTab;
   U32   Num_DefaultTab;
   U32   Point_BackgroudTab;
   U32   Num_BackgroudTab;
   U32   Point_ControlTab;
   U16   Num_ControlTab; 
   U32   Action_NoVoice;
   U32   Point_IndexGroup;
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

#define CUSTER_CODE_FLAG						(REMOTE_TASK_INIT_CODE_FLAG|REMOTE_TASK_CODE_FLAG\
												|AP3_OPEN_FILE_CODE_FLAG|GAME_INITIAL_CODE_FLAG\
												|GAME_SEARCH_1_CODE_FLAG|GAME_Script_CODE_FLAG\
												|IMA_CODEC_CODE_FLAG|RP_RECORD_CODE_FLAG)

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
   U32   Num_Control;
   U8    Flag;
}Control_Table;


typedef struct
{
   U32   Addr_Speech;
   U8    Flag_Speech;
}Speech;


typedef struct
{
   U16   min;
   U16   max;
}RANGE_AREA;
/*
enum
{
	Fat_Empty_Cluster
	,Fat_Valid_Cluster
	,Fat_End_Cluster
	,Fat_Invalid_Cluster
};

typedef	struct	_ClusterListItem
{
	U16	disk_id;
	U32	start_cluster;
	U32	offset_by_clus;
	U32	cluster;
	U32	seg_len;
}ClusterListItem;
*/
typedef enum
{
   IDLE_APPLI = 0
,  PLAYER_APPLI  
,  PASTER_APPLI
,  AP3_APPLI
,  RECORDER_APPLI
,  DIC_APPLI
,  SETTING_APPLI
,  MSTORAGE_APPLI
,  POWER_OFF_APPLI
}select_appli;

/*
//DEFINE  ALL BITS OF AP4_MP4_FLAG 
#define BANK_FORM_MP3       	0X8000
#define BANK_FORM_RP            0X4000
#define BANK_HAVE_INDEX     	0X2000
#define CURRENT_CONTROL_GAME 	0X1000
*/

extern _MEM_TYPE_SLOW_  U32					 Current_Special_Action;
extern _MEM_TYPE_SLOW_  Action_Sequence      Current_Act_Sequence;
extern _MEM_TYPE_SLOW_  U32                  sequence_speech_address;
extern _MEM_TYPE_SLOW_  U32                  sequence_text_address;
extern _MEM_TYPE_SLOW_  U32                  sequence_speech_read_point;

//extern _MEM_TYPE_SLOW_  U16                  Current_Index;
//#define Current_Index                           u16_current_index

//extern _MEM_TYPE_SLOW_  Record               Current_Record;
extern _MEM_TYPE_SLOW_  News_All_Control     All_Control;
extern _MEM_TYPE_SLOW_  News_Index_Range     Index_Range;
extern _MEM_TYPE_SLOW_  Record_Range		 Ap4_Record_Range;
extern _MEM_TYPE_SLOW_  Record_Range		 Ap4_Record_Range_2;
extern _MEM_TYPE_SLOW_  Book_Table           News_Book;
extern _MEM_TYPE_SLOW_  Go_Table             News_Go;
extern _MEM_TYPE_SLOW_  Control_Table        News_Control;
extern _MEM_TYPE_SLOW_  Coor                 Current_Coor;
extern _MEM_TYPE_SLOW_  U8                   Coor_Type;
extern _MEM_TYPE_SLOW_  U8                   Encrypt_Key;

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

extern   _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;
extern   _MEM_TYPE_SLOW_  U32				   mp3_over_clock;

extern _MEM_TYPE_SLOW_  U32 					Addr_Fanyi;  
extern  _MEM_TYPE_SLOW_ U8					Num_Fanyi;
extern  _MEM_TYPE_SLOW_ U8					Num_Cur_Fanyi;
extern _MEM_TYPE_SLOW_ Bool									flag_fanyi;

extern  _MEM_TYPE_SLOW_  U16 					flag_record_ap4_num;

extern _MEM_TYPE_SLOW_ unsigned int cal_flag;

//extern _MEM_TYPE_SLOW_  U8              Point_Write;
//extern _MEM_TYPE_SLOW_  U8              Point_Read;
//extern _MEM_TYPE_SLOW_  Speech          Array_V[ Length_v ];
extern _MEM_TYPE_SLOW_  U16              Point_Write;
extern _MEM_TYPE_SLOW_  U16              Point_Read;
extern __Y  Speech          Array_V[ Length_v ];
//extern unsigned int ap4_mp3_flag;

extern _MEM_TYPE_SLOW_  U16		 	 mp3_addr_bak_flag_record;
extern _MEM_TYPE_SLOW_  U16		 	 mp3_addr_bak_num_record;
extern _MEM_TYPE_SLOW_  Speech		 	 mp3_addr_bak[ Length_v ];
extern _MEM_TYPE_SLOW_  char             ap4_dirname[  ];

extern unsigned int begin_R;
extern unsigned int over_R;
extern unsigned int begin_P;
extern unsigned int over_P;

// define need's variable
//_MEM_TYPE_SLOW_ U16 Current_Index;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 X_Coor;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 Y_Coor;
#define X_Coor                                  Current_Coor.X1
#define Y_Coor                                  Current_Coor.Y1

extern _MEM_TYPE_SLOW_  U8     State_Search;


extern _MEM_TYPE_SLOW_  U32    Seed;
extern _MEM_TYPE_SLOW_  U16    code_flag;
extern _MEM_TYPE_SLOW_  Speech         lastSpeech;

extern _MEM_TYPE_SLOW_	  U32 					all_control_addr;
extern _MEM_TYPE_SLOW_	  U32 					 all_control_pw;

extern const  RANGE_AREA CUR_RANGE_AREA[NUM_AREA];
extern const U16 OFFSET_AREA[NUM_AREA];


extern  _MEM_TYPE_SLOW_ LASTBOOK    Current_lastbook[];
extern  _MEM_TYPE_SLOW_ BOOKNEWS    Current_booknews[];

#ifdef UNCOIDE_FORMAT
extern const U16 NO_AP4_FILE[];
extern const U16 SYS_OPEN_FILE[];
extern const U16 BATTERY_LOW_FILE[];
extern const U16 POWER_OFF_FILE[];
extern const U16 RECORDSTART[];
extern const U16 RECORDEND[];
extern const U16 PLAYSTART[];
extern const U16 PLAYEND[];
extern const U16 DISKFULL[];
extern const U16 SIMPLE[];
extern const U16 NO_FINDAP4[];
extern const U16 STOPREC[];
extern const U16 DEL[];
extern const U16 ERROR[];
extern const U16 FILEFULL[];
extern const U16 REREAD[];
extern const U16 MIN_TISHI[];
#endif

/*
#ifdef ASCII_FORMAT
extern const U8 NO_AP4_FILE[];
extern const U8 SYS_OPEN_FILE[];
extern const U8 BATTERY_LOW_FILE[];
extern const U8 POWER_OFF_FILE[];
extern const U8 BATTERY_WARN_FILE[];
extern const U8 RECORDSTART[];
extern const U8 RECORDEND[];
extern const U8 PLAYSTART[];
extern const U8 PLAYEND[];
extern const U8 DISKFULL[];
extern const U8 SIMPLE[];
extern const U8 NO_FINDAP4[];
extern const U8 MIN_TISHI[];
extern const U8 STOP_MP3[];
extern const U8 REPEAT_MP3[];

#endif
*/



ImageDeclareType(REMOTE_TASK_INIT_CODE);
ImageDeclareType(REMOTE_TASK_CODE);
ImageDeclareType(AP3_OPEN_FILE_CODE);
ImageDeclareType(GAME_INITIAL_CODE);
ImageDeclareType(GAME_SEARCH_1_CODE);
ImageDeclareType(GAME_Script_CODE);
ImageDeclareType(IMA_CODEC_CODE);

ImageDeclareType(AUADC_COMMON_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(WAV_REC_CODE);
ImageDeclareType(WAV_PLAY_CODE);

ImageDeclareType(FS_CREATE_BUF_CODE);
ImageDeclareType(FILE_EXTERN_CODE);
ImageDeclareType(POWER_SYSTEM_OFF_CODE);


ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(AUADC_COMMON_CODE);

ImageDeclareType(KB_RESIZE_CODE);
ImageDeclareType(RP_RECORD_CODE);
ImageDeclareType(READ_ALL_NEWS_CODE);
ImageDeclareType(SEARCH_TWOBYTE_CODE);
ImageDeclareType(VOICE_PLAY_CODE);
ImageDeclareType(ACT_READ_CODE);
ImageDeclareType(PLAY_AP3DEAL_CODE);
ImageDeclareType(READ_PAGE_OTHERCODE);
ImageDeclareType(READ_SGDB_CODE);
ImageDeclareType(RP_JUDGE_CODE);
ImageDeclareType(MP3_COMMON_CODE);
ImageDeclareType(PLAYEND_DOWITH_01_CODE);
ImageDeclareType(PLAYEND_DOWITH_02_CODE);
ImageDeclareType(GENDU_CODEC_CODE);
ImageDeclareType(DO_CHANGE_AP4_CODE);
ImageDeclareType(OPEN_FILE_CODE);
ImageDeclareType(JUDGE_COOR01_CODE);
ImageDeclareType(JUDGE_COOR02_CODE);
ImageDeclareType(JUDGE_COOR03_CODE);
ImageDeclareType(SEARCH_OPEN_AP4_CODE00);
ImageDeclareType(SEARCH_OPEN_AP4_CODE01);
ImageDeclareType(READ_BOOK_NEWS_CODE);
ImageDeclareType(READ_PAGE_NEWS_CODE);
ImageDeclareType(REMOTE_CHANGEPRO_CODE);
ImageDeclareType(READ_CONTR_NEWS_CODE);
ImageDeclareType(CONTROL_NEWS_CODE);
ImageDeclareType(MAKE_SD_DIR_CODE);
ImageDeclareType(KEYLONG_VOL_CODE);
ImageDeclareType(FIX_INDEX_CODE01);
ImageDeclareType(FIX_INDEX_CODE02);
ImageDeclareType(FIX_INDEX_CODE03);
ImageDeclareType(FIX_INDEX_CODE04);
ImageDeclareType(FIX_INDEX_CODE05);
ImageDeclareType(FIX_INDEX_CODE06);
ImageDeclareType(FIX_INDEX_CODE07);
ImageDeclareType(FIX_INDEX_CODE08);
ImageDeclareType(FIX_INDEX_CODE09);
ImageDeclareType(FIX_INDEX_CODE10);
ImageDeclareType(FILESEARCH_CODE);
ImageDeclareType(CHANGE_CON_MODE_CODE);
ImageDeclareType(AP4_CHOOSE_PRE_NEXT);
ImageDeclareType(BEGIN_FIND_AP4_CODE);
ImageDeclareType(READY_OPENNEWAP4_CODE);
ImageDeclareType(BACK_BAK_DOWITH_CODE);
ImageDeclareType(ENCRY_NEWS_CODE);
ImageDeclareType(PRINT_NEWS_AP4_CODE);
ImageDeclareType(OPEN_NEWAP4_CODE);
ImageDeclareType(BANKTOAP4_CODE);
ImageDeclareType(REMOTE_INDEX_CODE);
ImageDeclareType(TRANSLATE_CODE);
ImageDeclareType(CLEANSTRUCT_CODE);
//mageDeclareType(NEW_READ_ALLNEWS_CODE);
ImageDeclareType(JIAMI_CODE);
ImageDeclareType(FIND_PASS_01_CODE);
ImageDeclareType(FIND_PASS_02_CODE);
ImageDeclareType(FIND_PASS_03_CODE);
ImageDeclareType(ACTSELECT_CODE);
/// define the function
void  search_evt                    ( void );
void  init_array_v                  ( void );
Bool  act_do                        ( U32 address, Bool game_act_do_flag );
void  read_data_from_nf             ( U32 start_addr, U8 _MEM_TYPE_SLOW_ *store_addr, U16 leng_data );
//Bool  search_eightbytes_coordinate  ( U32 addr, U16 number );
void  remote_mp3_task               ( void );
void  write_media_addr              ( U32 address, U8 flag );
void  ap3_rand_init                 ( void );
void  repeat_save                   ( void );
void clear_bigflag(void);
void clear_smallflag(void);
void readyGoNewPorject(unsigned int subProject);
#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
