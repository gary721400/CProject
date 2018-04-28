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

#include "mmi\shared\com_var.h"
#include "conf\conf_sio.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _search_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Current_Index;
//#define Current_Index                           u16_current_index



//#define OID2_DIY_PLAY_MIN			55001
//#define OID2_DIY_PLAY_MAX			60000

//#define OID2_DIY_OFFSET				(5000)


#if( AP3 == ENABLE )


//#define AP4_ENCRYPT              ENABLE
#define AP4_ENCRYPT              DISABLE

#if( AP4_ENCRYPT == ENABLE )
//#define DECODE_NORMAL_FILE       ENABLE
#define DECODE_NORMAL_FILE       DISABLE

Bool  ap4_book_encrypt           ( void );

#endif   //#if( AP4_ENCRYPT == ENABLE )


//define the caption length of all struct
#define Length_Caption              16
///define the length of the read data 
#define length_eight_coordiante     64
//define the number of the voice address 
#define Length_v                    256//64

#define DICTEXT_LINE_PAGE			2
#define AP4_LINE_PAGE				3
#define MAX_NUM_LINE				64//32

#define MAX_NUM_PAGE				(MAX_NUM_LINE*3)//(96)
/*
#define AP4_MENU_X	
#define AP4_MENU_Y	
#define AP4_BROW_X	
#define AP4_BROW_Y
*/
//#define all function
#define state_search_to_open_file() do{State_Search = Open_File_Search;}while(0)
#define state_search_to_ap3()       do{State_Search = Ap3_Search;}while(0)
#define state_search_to_book()      do{State_Search = Book_Search;}while(0)
#define state_search_to_page()      do{State_Search = Page_Search;}while(0)
#define state_search_to_ctr_mode()  do{State_Search = Control_Mode_Search;}while(0)
#define state_search_to_idle()      do{State_Search = Idle_Search;}while(0)
#define state_search_to_play()      do{State_Search = Play_Search;}while(0)
#define state_search_to_error()     do{State_Search = Err_Search;}while(0)


#define MSK_COOR_MODE               0x03u
#define IMMOBILITY_COOR_MODE        0x00u
#define MOBILE_COOR_MODE            0x01u
#define MIX_COOR_MODE               0x02u
#define AUTO_FIRST_BOOK             0x04u
#define AP3_TO_AEB_FLAG             0x08u
#define SIMPLE_FLAG                 0x10u
#define SEQUENCE_FLAG               0x20u
#define DEFAULT_SEQUENCE_FLAG       0x40u
#define ENCRYPT_AUDIO_DATA_FLAG     0x80u

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
#define Browser_File_Init	        ( Start_Search + 13 )
#define Browser_File				( Start_Search + 14 )
#define Browser_Dir_Init	        ( Start_Search + 15 )
#define Browser_DirFile				( Start_Search + 16 )
#define READY_GOTO_SUB				( Start_Search + 17 )
#define BACK_TO_SUB					( Start_Search + 18 )
#define BACK_TO_SUBROOT				( Start_Search + 19 )
#define Browser_Page_Init	        ( Start_Search + 20 )
#define Browser_Page				( Start_Search + 21 )
#define Browser_Unite_Init			( Start_Search + 22 )
#define Browser_Unite				( Start_Search + 23 )
#define Goto_Unite					( Start_Search + 24 )


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



#define Type_Script                 ( Type_Action + 20 )

#define Type_Quit_Game_Speech       ( Type_Action + 30 )


#define Type_Play_Findword          ( Type_Action + 36 )
//***********************************************
//add bu xuanyu
#define Type_SelectBook          0x101
#define Type_SelectPage          0x102
#define Type_SelectControl       0x103
//***********************************************


#define Order_Play               ( 0x01 )
#define Random_Play              ( Order_Play + 1 )
#define Order_Play_R             ( Order_Play + 2 )


#define IDLE_CTRL_DISPLAY             ( 0x00 )
#define READ_CTRL_DISPLAY             ( 0x01 )
#define REPEAT_CTRL_DISPLAY           ( 0x02 )

#define NORMAL_MODE_DISPLAY             ( 0x81 )
#define REPEAT_MODE_DISPLAY             ( 0x82 )
#define READ_ALOUD_MODE_DISPLAY         ( 0x83 )
#define AP4_SINGLE_MODE_DISPLAY         ( 0x84 )




#define REREAD_MODE_HINT                 ( 0x00 )
#define NORMAL_MODE_HINT                 ( 0x01 )

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

#if 0
typedef struct
{
   U16   Ap3_num;
   U16   Book_num;
   U16   Page_num;
   U16   Control_num;
   /*struct
   {
      U8    Ap3_flag       :1;
      U8    Book_flag      :1;
      U8    Page_flag      :1;
      U8    Control_flag   :1;
   }flag;*/
}Record;

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
      //unsigned mode_flag              :2;
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
_GLOBEXT_ _MEM_TYPE_SLOW_ SEARCH_STATUS Search_Record;
#endif

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
//   U8    Caption_Book[ Length_Caption ];      //no use, not read
   U8    Flag_CurrentBook;
   U32   Point_GoTable;
   U32   Point_GOTable_Coordinate;
   U16   Num_GoTable;
   U32   Point_GameRightAnswer;
   U32   Point_SequenceSpeechTable;  //firmeware no used
   U32   Point_SequenceTextTable;    //firmeware no used
   U32   Point_NumSpeechTable;
   U32	 Point_UniteTable;
   U32   Point_Book_Cap;
}Book_Table;


typedef struct
{
	U16		Cur_Unite_Num;
	U16		Cap_Num;
	U32		Point_UniteNews;
}Unite_News;

/*
typedef struct
{
	U16		Min_Code;
	U16		Max_Code;
	//U32		Point_Default;
}Jian_News;
*/

typedef struct
{
	U16 Unite_Num;
	U32 Cap_Point;
}All_Unite_News;

/*
typedef struct
{
	U16		Page_ID;
	U16		Index_ID;
}Run_Unite_News;
*/
typedef struct
{
   U32   Point_PageCoorTable;
   U32   Point_BackgroudCoor;
   union
   {
      U16   Num_Sub;
      U16   Min_Index_ThePage;
   }page_info;
   U16   Max_Index_ThePage;
   U32   Addr_Action;
//   U8    Caption_Go[ Length_Caption ];     //no use, not read
   U32   Point_DefaultTab;
   U16   Num_DefaultTab;
   U32   Point_BackgroudTab;
   U16   Num_BackgroudTab;
   U32   Point_ControlTab;
   U16   Num_ControlTab; 
   U32   Action_NoVoice;
}Go_Table;


//define control mode
//#define SINGLE_TABLE              ( 0x01 )
//#define GAME_TABLE                ( SINGLE_TABLE + 1 )

// control type
#define SINGLE_TABLE_TYPE           ( 10 )
#define GAME_TYPE                   ( 11 )
#define DIC_TYPE                    ( 12 )
#define QA_TYPE                     ( 13 )
#define SCRIPT_TYPE			         ( 14 )


//define table
#define CONTROL_TABLE               ( 0x01 )
#define DEFAULT_TABLE               ( CONTROL_TABLE + 1 )
#define BACKGROUND_TABLE            ( DEFAULT_TABLE + 1 )

//record and play index mode flag
#define RP_1_INDEX_FLAG             0x55
#define RP_2_INDEX_FLAG             0xAA

#define CMP_DIR_NUM_OFFSET			0x07FF
#define DIR_NUM_OFFSET				0x0800


#define LEN_EN_TEXTBF				32
#define LEN_EN_WORDBF				32

#define MAX_PAGE_NUMBER				32

#define LEN_DISPLAY_BUF				68

#define FIRST_LINE_ADDR			    (38)		
#define SECOND_LINE_ADDR		    (54)
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


enum
{
   BROWSER_Cmd_None,
   BROWSER_Cmd_CurFile,
   BROWSER_Cmd_NextFile,
   BROWSER_Cmd_PrevFile,
   BROWSER_Cmd_Enter,
   BROWSER_Cmd_Back,
   BROWSER_Cmd_Exit,
   BROWSER_Cmd_End
};


enum
{
  NEXT_FILE,
  PREV_FILE
};

typedef struct
{
   U8    Caption_Control[ Length_Caption ];
   U32   Point;
   U32   Addr_Action;
   U8    Type_Control;
   U16   Num_Control;
   U8    Flag;
}Control_Table;

#define  AUTO_IN_GAME_FLAG          0x01
#define  TRANS_FLAG                 0x02

typedef struct
{
   U32   Addr_Speech;
   U8    Flag_Speech;
}Speech;

typedef struct
{
   U8   x;
   U8   y;
   U8   heigh;
   U8   lang;
}DisplayNews;

_GLOBEXT_ _MEM_TYPE_SLOW_  Special_Action       Current_Special_Action;
_GLOBEXT_ _MEM_TYPE_SLOW_  Action_Sequence      Current_Act_Sequence;
_GLOBEXT_ _MEM_TYPE_SLOW_  U32                  sequence_speech_address;
_GLOBEXT_ _MEM_TYPE_SLOW_  U32                  sequence_text_address;
_GLOBEXT_ _MEM_TYPE_SLOW_  U32                  sequence_speech_read_point;

//_GLOBEXT_ _MEM_TYPE_SLOW_  Record               Current_Record;
_GLOBEXT_ _MEM_TYPE_SLOW_  News_All_Control     All_Control;
_GLOBEXT_ _MEM_TYPE_SLOW_  News_Index_Range     Index_Range;
_GLOBEXT_ _MEM_TYPE_SLOW_  Record_Range         Ap4_Record_Range;
_GLOBEXT_ _MEM_TYPE_SLOW_  Book_Table           News_Book;
_GLOBEXT_ _MEM_TYPE_SLOW_  Go_Table             News_Go;
_GLOBEXT_ _MEM_TYPE_SLOW_  Control_Table        News_Control;
_GLOBEXT_ _MEM_TYPE_SLOW_  Coor                 Current_Coor;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   Coor_Type;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   Encrypt_Key;

_GLOBEXT_ _MEM_TYPE_SLOW_  Record_Range         Ap4_Record_Range_2;

_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Temp_Control_num;
_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Temp_Ap3_file_num;
_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Temp_Search_record_value;
//_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Remote_Search_first_Ap3_file_num;
//_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  Remote_Change_Ap4_file_count;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   order_play_count;
_GLOBEXT_ _MEM_TYPE_SLOW_  U16                  rp_oid_index;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               diandu_entrynum;
_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               display_page_num;

//_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               ap4current_displayfile_num;
_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               ap4current_displaypage_num;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               ap4current_mark;
//_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               ap4CurrentMarkBak;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               open_dir_num;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               unit_display_num;
_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               unitcurrent_display_num;
_GLOBEXT_ _MEM_TYPE_MEDSLOW_   U16               unitcurrent_mark;

_GLOBEXT_ _MEM_TYPE_SLOW_   U8                   browserfile_cmd_id;

_GLOBEXT_  _MEM_TYPE_SLOW_  U8 					 s_ts_id;
_GLOBEXT_	_MEM_TYPE_SLOW_      U8                text_line_buf[ LEN_DISPLAY_BUF ];
_GLOBEXT_	_MEM_TYPE_SLOW_      U16             text_off_addr[ MAX_PAGE_NUMBER ];
//_GLOBEXT_                  Bool                 b_play_repeat;
//_GLOBEXT_                  Bool                 b_remote_play_repeat;
 _GLOBEXT_ _MEM_TYPE_SLOW_ DisplayNews            displayNews;

// define need's variable
//_MEM_TYPE_SLOW_ U16 Current_Index;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 X_Coor;
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16                 Y_Coor;
#define X_Coor                                  Current_Coor.X1
#define Y_Coor                                  Current_Coor.Y1

_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   State_Search;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
//_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   Save_State_Search;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

_GLOBEXT_ _MEM_TYPE_SLOW_  U32                  Seed;
/*
_GLOBEXT_                  Bool                 b_reload_ap4
#if 0//def _search_c_
   = FALSE
#endif
;
_GLOBEXT_                  Bool                 g_b_ap4_caption
#if 0//def _search_c_
   = FALSE
#endif
;

_GLOBEXT_                  Bool                 game_exit_to_control;

_GLOBEXT_                  Bool                 reread_enable_flag
#if 0//def _search_c_
   = FALSE
#endif
;*/

_GLOBEXT_ _MEM_TYPE_SLOW_  Action               Current_Act;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   Point_Write;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   Point_Read;
_GLOBEXT_ _MEM_TYPE_SLOW_  U8                   repeat_point;


#if 1//( AP4_RP == ENABLE )
_GLOBEXT_  _MEM_TYPE_SLOW_      U8                ap4_short_name[ SHORT_NAME_SIZE + 1 ];
_GLOBEXT_  _MEM_TYPE_SLOW_      U8                ap4_long_name[ G_STRING_SIZE ];
//_MEM_TYPE_SLOW_      RP_BIT_NEW        rp_flag_bits = { 0x00 };
#endif   //#if( AP4_RP == ENABLE )


_GLOBEXT_ _MEM_TYPE_SLOW_  U8   current_ap4_short_name[ 32 ];
_GLOBEXT_ _MEM_TYPE_SLOW_  U8   current_ap4_short_path[ 16 ];




ImageDeclareType( SEARCH_REMOTE_TEMP_CODE00 );
ImageDeclareType( SEARCH_REMOTE_TEMP_CODE01 );
ImageDeclareType( SEARCH_REMOTE_TEMP_CODE02 );

ImageDeclareType( SEARCH_GAME_TEMP_CODE00 );
ImageDeclareType( SEARCH_GAME_TEMP_CODE01 );

ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE05 );
ImageDeclareType( GET_FILE_NAME_CODE );

ImageDeclareType( READ_ALLNEWS_CODE );
ImageDeclareType( READ_BOOKNEWS_CODE );
ImageDeclareType( READ_PAGENEWS_CODE );
ImageDeclareType( READ_CONTROLNEWS_CODE );
ImageDeclareType( SEARCH_PLAY_END_CODE );
ImageDeclareType( PLAYER_AP3_START_CODE );
ImageDeclareType( JUDGE_KEY_CODE );
ImageDeclareType( JUDGE_IMMOBILITY_COOR_CODE );
ImageDeclareType( JUDGE_MOBILITY_COOR_CODE );
ImageDeclareType( JUDGE_MIX_COOR_CODE );
ImageDeclareType( RECORD_JUDGE_CODE );
ImageDeclareType( SEARCH_TWOBYTES_COOR_CODE );
ImageDeclareType( CHECK_GAMETIEM_CODE );
ImageDeclareType( GAME_TIMEOUT_DEAL_CODE );

ImageDeclareType( SEARCH_GAME_CODE );

ImageDeclareType( NAVIGATION_CODE );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );

ImageDeclareType( SEARCH_OPEN_FILE_FAIL_CODE );
ImageDeclareType( AP4_SEARCH_CODE );
ImageDeclareType( PAGE_SEARCH_CODE );
ImageDeclareType( SEARCH_RELOAD_AP4_INFO_CODE );

ImageDeclareType( SEARCH_IN_GAME_CODE );

ImageDeclareType( SEARCH_SCRIPT_CODE );

ImageDeclareType( SEARCH_OPEN_FILE_TEMP_CODE );
ImageDeclareType( MMI_AP4_CONTROL_TEMP_CODE00 );
ImageDeclareType( SEARCH_AP4_TEMP_CODE );
ImageDeclareType( SEARCH_BOOK_TEMP_CODE );
ImageDeclareType( SEARCH_PAGE_TEMP_CODE );
ImageDeclareType( SEARCH_CONTROL_TEMP_CODE );
ImageDeclareType( SEARCH_COOR_TEMP_CODE );
ImageDeclareType( SEARCH_NORMAL_TEMP_CODE );
ImageDeclareType( SEARCH_IDLE_TEMP_CODE );
ImageDeclareType( SEARCH_PLAY_TEMP_CODE );
ImageDeclareType( SEARCH_SEQUENCE_TEMP_CODE );

ImageDeclareType( COOR_JUDGE_DEAL_CODE );
ImageDeclareType( JUDGE_INDEX_DEAL_CODE );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( SEARCH_AP4INFO_CODE );

ImageDeclareType( SEARCH_RP_CONFIRM_CODE );
ImageDeclareType( SEARCH_SEQUENCE_JUDGE_CODE );
ImageDeclareType( SEARCH_GAME_TIME_CODE );
ImageDeclareType( GAME_EXIT_TO_CONTROL_CODE );

ImageDeclareType( SEARCH_ACTION_TEMP_CODE00 );
ImageDeclareType( SEARCH_ACTION_TEMP_CODE01 );
ImageDeclareType( SEARCH_ACTION_REPEAT_CODE );
ImageDeclareType( ACTION_SEQUENCE_CODE );
ImageDeclareType( ACTION_SELECT_BOOK_CODE );
ImageDeclareType( ACTION_SELECT_PAGE_CODE );
ImageDeclareType( ACTION_SELECT_CONTROL_CODE );
ImageDeclareType( ACTION_TEXT_DISPLAY_CODE );

ImageDeclareType( SEARCH_GAME_CONSOLE_CODE );

ImageDeclareType( ACTION_PLAY_SEQUENCE_CODE );
ImageDeclareType( ACTION_TEMP_CODE01 );
ImageDeclareType( ACTION_TEMP_CODE02 );
ImageDeclareType( ACTION_TEMP_CODE03 );
ImageDeclareType( ACTION_TEMP_CODE04 );
ImageDeclareType( ACTION_TEMP_CODE05 );
ImageDeclareType( ACTION_TEMP_CODE06 );
ImageDeclareType( ACTION_TEMP_CODE07 );
ImageDeclareType( ACTION_TEMP_CODE09 );
ImageDeclareType( ACTION_TEMP_CODE10 );

ImageDeclareType( SEARCH_RAND_CODE );
ImageDeclareType( FS_CREATE_BUF_CODE );

ImageDeclareType( SEARCH_BOOK_ENCRYPT_CODE );

ImageDeclareType( SEARCH_NORMAL_CODE );
ImageDeclareType( SEARCH_GAME_STATUS_CODE );
ImageDeclareType( SEARCH_INDEX_COOR_CODE );

ImageDeclareType( SEARCH_REMOTE_CALEXIT_CODE );

ImageDeclareType( BROWER_FILE_INIT_CODE );
ImageDeclareType( BROWER_FILE_CODE );
ImageDeclareType( GET_DIR_NAME_CODE );
ImageDeclareType( BROWER_DIR_INIT_CODE );
ImageDeclareType( BROWER_DIR_CODE );
ImageDeclareType( BROWER_DIR_CODE );
ImageDeclareType( READY_GO_SUB_CODE );
ImageDeclareType( BROWER_BACK_SUBROOT_CODE );
ImageDeclareType( ACTION_TEXT_WORDNUM_CODE );
ImageDeclareType( ACTION_TEXT_ONELINE_CODE );
ImageDeclareType( ACTION_TEXT_TWOLINE_CODE );

ImageDeclareType( DISPLAY_FIRSTWORD_CODE );

ImageDeclareType( TEXT_BACKDISPLAY_CODE );
ImageDeclareType( TEXT_CH_DISPLAY_CODE );

ImageDeclareType( FINDWORD_TEMP01_CODE );
ImageDeclareType( FINDWORD_DISPLAY_CODE );
ImageDeclareType( FINDWORD_DISPLAY_CODE );

ImageDeclareType( CLUSTERLIST_CODE );
ImageDeclareType( DAC_COMMON_CODE );
ImageDeclareType( TEXT_NOFIND_DISPLAY_CODE );
ImageDeclareType( CTRL_DISPLAY_CODE );
ImageDeclareType( CTRL_DISPLAY_READY_CODE );

ImageDeclareType( SEARCH_ADBINFO_CODE );
ImageDeclareType( LCE_ENCRYPT_CODE );
ImageDeclareType( BROWER_GO_PAGE_CODE );
ImageDeclareType( PRINT_CODE );
ImageDeclareType( NEW_READ_ALLNEWS_CODE );
ImageDeclareType( JIAMI_CODE );
ImageDeclareType( FIND_PASS_01_CODE );
ImageDeclareType( FIND_PASS_02_CODE );
ImageDeclareType( FIND_PASS_03_CODE );

ImageDeclareType( T01_COMMAND_CODE );

/// define the function
void  search_evt                    ( void );
void  init_array_v                  ( void );
Bool  act_do                        ( U32 address, Bool game_act_do_flag );
void  read_data_from_nf             ( U32 start_addr, U8 _MEM_TYPE_SLOW_ *store_addr, U16 leng_data );
Bool  search_eightbytes_coordinate  ( U32 addr, U16 number );
void  remote_mp3_task               ( void );
void  write_media_addr              ( U32 address, U8 flag );
void  ap3_rand_init                 ( void );
void  repeat_save                   ( void );
void  voice_play_number             ( /*U32 address*/void );
void  read_all_control_news         ( void );
void  new_read_all_control_news		( void );
Bool  read_new_ap4_password			( void );
Bool  judge_password				( void );
U32	  read_new_ap4_password_01		( void );
U32	  read_new_ap4_password_02		( void );
void  lce_sec						( void );
U16   Get_File_Number				( U8 *sz_filterext );
void  print_news					( void );
Bool  record_judge                  ( void );
Bool  brower_dir_init 				( void );
void  brower_dir	 				( void );
void mode_key_long_press( U8 appli_id );
void  ready_go_sub					(U16 subdir_num);
void ready_goback_subdir( void );
void display_firstword( void );
void text_backdisplay( void );
Bool text_chinese_display( U32 fan_add );
void findword_display( void );
#endif  //#if( AP3 == ENABLE )

#endif  //_SEARCH_H_
