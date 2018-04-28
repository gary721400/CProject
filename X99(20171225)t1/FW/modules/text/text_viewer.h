#ifndef	_TEXT_VIEWER_H_
#define	_TEXT_VIEWER_H_

#include "modules/search/search_game.h"
//-------------LCD显示-------------
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "conf\conf_sio.h"
#include "mmi/shared/com_var.h"

#if( DIC == ENABLE )

#undef _GLOBEXT_
#if (defined _text_viewer_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

_GLOBEXT_   _MEM_TYPE_SLOW_   select_appli   exit_dictext_to_appli;
_GLOBEXT_   _MEM_TYPE_SLOW_   U8             dic_file_open_flag;


_GLOBEXT_   _MEM_TYPE_SLOW_  U16              leng_en;//原文英文长度
_GLOBEXT_   _MEM_TYPE_SLOW_  U16              leng_ch;//原文中文长度

_GLOBEXT_   _MEM_TYPE_SLOW_  U16             Display_Page_Num;	
_GLOBEXT_   _MEM_TYPE_SLOW_  U16             current_display_num;
_GLOBEXT_   _MEM_TYPE_SLOW_  U16             current_display_num_bak;
_GLOBEXT_   _MEM_TYPE_SLOW_  U32             ch_begin_address;
_GLOBEXT_   _MEM_TYPE_SLOW_  U32             ch_display_address;
_GLOBEXT_   _MEM_TYPE_SLOW_  U32             ch_display_address_bak;


_GLOBEXT_   const   U8             dic_g_dir[]
#if 0//def _text_viewer_c_
= "Dic"
#endif
;

#define  DICTEXT_Screen_Width       LCD_WIDTH
#define  DICTEXT_Screen_Height      LCD_HEIGHT

Bool  textviewer_init            ( U32 _MEM_TYPE_SLOW_ *start_offset, U32 _MEM_TYPE_SLOW_ *size );
/*Bool*/void  textviewer_scroll_up       ( void );
/*Bool*/void  textviewer_scroll_down     ( void );
/*Bool*/void  textviewer_scroll_to       ( U16 line_num );
U8    *textviewer_get_line_str   ( U8 index );

enum
{
   DICTEXT_Cmd_None, //0
   DICTEXT_Cmd_Start,//1
   DICTEXT_Cmd_Exit, //2
   DICTEXT_Cmd_KeyInput,//3

   FILEBROWSER_Cmd_CurFile,//4
   FILEBROWSER_Cmd_NextFile,//5
   FILEBROWSER_Cmd_PrevFile,//6
   FILEBROWSER_Cmd_Enter,//7
   FILEBROWSER_Cmd_Back,//8
   FILEBROWSER_Cmd_Exit,//9

   DIC_Cmd_KeyInput,  //10
   DIC_Cmd_KeyboardLeft, //11
   DIC_Cmd_KeyboardRight,//12
   DIC_Cmd_KeyboardScroll,//13
   DIC_Cmd_KeyboardSelect,//14
   DIC_Cmd_Enter,//15
   DIC_Cmd_CancelInput,//16
   DIC_Cmd_Exit,//17
   DIC_Cmd_VolUp,//18
   DIC_Cmd_VolDown,//19
   DIC_Cmd_PowerOff,//20
   DIC_Cmd_Backspace,

   TEXTVIEWER_Cmd_ScrollDown,
   TEXTVIEWER_Cmd_ScrollUp,
   TEXTVIEWER_Cmd_ScrollTo,
   TEXTVIEWER_Cmd_Back,

   TEXTVIEWER_Cmd_Voice,

   TEXTVIEWER_Cmd_Debug,

   DICTEXT_Cmd_Timer
};

enum
{
   DICTEXT_State_Idle,
   DICTEXT_State_Init,

   FILEBROWSER_State_Init,
   FILEBROWSER_State_Refresh,
   FILEBROWSER_State_Interface,
   FILEBROWSER_State_End,
   FILEBROWSER_State_NextInit,

   DIC_State_Init,
   DIC_State_Refresh,
   DIC_State_Interface,
   DIC_State_Search,
   DIC_State_SearchFailed,
   DIC_State_End,

   TEXTVIEWER_State_TextView_Init,
   TEXTVIEWER_State_TextView,
   TEXTVIEWER_State_TextView_End,

   DICTEXT_State_VoiceStart,
   DICTEXT_State_Voice,
   DICTEXT_State_VoiceEnd,

   DICTEXT_State_Error,
   DICTEXT_State_End
};

#define  DICTEXT_LINE_PAGE			   2

#define  DICTEXT_SmallFont             IDC_FONT_08
#define  DICTEXT_BigFont               IDC_FONT_DIC_16
#define  DICTEXT_Title_PosX            0
#define  DICTEXT_Title_PosY            0
#define  DICTEXT_Title_Height          16//8
#define  DICTEXT_Content_PosX          0
#define  DICTEXT_Content_PosY          8
#define  DICTEXT_Content_Height        16

//#define  DICTEXT_Title_CharWidth       6
#define  DICTEXT_Title_CharWidth       4


#define DIC_LINE_PAGE					2
#define DIC_MAX_NUM_LINE				64//32
#define DIC_MAX_PAGE_NUMBER					32
#define DIC_MAX_NUM_PAGE					(DIC_MAX_NUM_LINE*DIC_LINE_PAGE)//(96)

#define LEN_DISPLAY_BUF				68

#define FIRST_LINE_ADDR			    (38)		
#define SECOND_LINE_ADDR		    (54)


#define  FILEBROWSER_Num_FileList      1
#define  FILEBROWSER_PosItem_StartX    0
#define  FILEBROWSER_PosItem_StartY    DICTEXT_Content_PosY
#define  FILEBROWSER_PosFile_StartX    ( FILEBROWSER_PosItem_StartX/* + 18 */)//0
#define  FILEBROWSER_PosFile_StartY    ( FILEBROWSER_PosItem_StartY + 4 )//0+4
#define  FILEBROWSER_PosIcon_StartX    ( FILEBROWSER_PosItem_StartX + 9 )
#define  FILEBROWSER_PosIcon_StartY    ( FILEBROWSER_PosItem_StartY )
#define  FILEBROWSER_PosCursor_StartX  ( FILEBROWSER_PosItem_StartX + 0 )
#define  FILEBROWSER_PosCursor_StartY  ( FILEBROWSER_PosItem_StartY )
#define  FILEBROWSER_Height_FileItem   DICTEXT_Content_Height
#define  FILEBROWSER_Width_FileItem    DICTEXT_Screen_Width


//#define  TEXTVIEWER_Size_ExtBuffer        (( U16 )( 32 * sizeof( Game_concent_Table )))
#define  TEXTVIEWER_Size_ExtBuffer        256//128
#define  TEXTVIEWER_Num_CharLength        ( DICTEXT_Screen_Width / 8 )//32//16
#define  TEXTVIEWER_Num_LineLength        ( TEXTVIEWER_Num_CharLength * 2 + 6 + 4 )//74//42
#define  TEXTVIEWER_Num_MaxContentLines   ( TEXTVIEWER_Size_ExtBuffer / TEXTVIEWER_Num_LineLength )//3
#define  TEXTVIEWER_Num_ContentLines      textviewer_g_lines
#define  TEXTVIEWER_PosText_StartX        textviewer_g_start_x
#define  TEXTVIEWER_PosText_StartY        textviewer_g_start_y
#define  TEXTVIEWER_Height_TextLine       DICTEXT_Content_Height//16
#define  TEXTVIEWER_Width_TextLine        DICTEXT_Screen_Width//128

/*Bool*/void  dictext_evt_init  ( void );
void  dictext_evt       ( void );
void  dictext_send_cmd  ( U8 id, U16 param );
void  dictext_trans_cmd ( U8 evt_id, U16 evt_param );
//U8    dic_to_lower      ( U8 c );
void  init_array_v      ( void );
void dic_text_display( void );

#endif   //#if( DIC == ENABLE )
#endif   //_TEXT_VIEWER_H_
