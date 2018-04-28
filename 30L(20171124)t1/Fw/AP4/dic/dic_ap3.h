//!
//! @file mmi_ap3.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI ap3
//!
//! @version 1.1 snd3b-dvk-1_0_33 $Id: mmi_ap3.h,v 1.1 2005/10/03 11:13:20 coger Exp $
//!
//! @todo
//! @bug
//!

#ifndef	_DIC_AP3_H_
#define _DIC_AP3_H_

#include "compiler.h"

//_____ M A C R O S ________________________________________________________




//_____ D E C L A R A T I O N S ____________________________________________

//_____ M A C R O S ________________________________________________________
// Strings to store a text (Global to optimize the data space)
#define  G_STRING_SIZE     (30 *2)
#define  G_STRING_SIZE2    (30 *2)

//=================================================================
#define DIC_MP3_ENCRY		(0x0001u)


//extern UnicodeBuffer g_string   [G_STRING_SIZE ];
//extern UnicodeBuffer g_string2  [G_STRING_SIZE2];
//_____ D E C L A R A T I O N S ____________________________________________
 extern                    Bool       b_kbd_process_disable;
//#define mmi_ap3_kbd_disable()    ( b_kbd_process_disable = TRUE )
//#define mmi_ap3_kbd_enable()     ( b_kbd_process_disable = FALSE )
//#define mmi_ap3_kbd_is_disable() ( b_kbd_process_disable == TRUE )

#define AP3_MENU_PATH                 ( 0x00 )
#define BOOK_MENU_PATH                ( AP3_MENU_PATH + 1 )
#define PAGE_MENU_PATH                ( AP3_MENU_PATH + 2 )
#define CONTROL_MENU_PATH             ( AP3_MENU_PATH + 3 )
#define CONTROL_MODE_MENU_PATH        ( AP3_MENU_PATH + 4 )

typedef enum
{
   SINGLE_PLAY_MODE,
   SEQUENCE_PLAY_MODE
}AP4_Mode;

extern   _MEM_TYPE_SLOW_   AP4_Mode ap3_mode;
extern   _MEM_TYPE_SLOW_   U8       Ap3_MenuPath;
extern _MEM_TYPE_SLOW_   U16      curAdbFile;
extern _MEM_TYPE_SLOW_   U16      adb_file_number;

extern   const  U8       Ext[];
extern   const  U8       AP4[];
extern   const  U8       DICT[];
extern   const  U8       ADB[];


extern    _MEM_TYPE_SLOW_   Bool      poweroff_falg;
typedef enum
{
   AP4_PLAY_SEQUENCE
,  AP4_PLAY_NOT_FIND_FILE
,  AP4_PLAY_SIMPLE
,  AP4_PLAY_ONLY_SIMPLE
,  AP4_PLAY_ONLY_SEQUENCE
,  AP4_PLAY_SD_REMOVE
,  AP4_PLAY_NO_AP4
,  AP4_PLAY_ERROR
}Ap4_Play;
extern  _MEM_TYPE_SLOW_   U8       ap4_play_state ;

typedef enum
{
   MMI_AP3_POWERON
,  MMI_AP3_NORMAL
,  MMI_AP3_CHANGE_CONTROL
,  MMI_CHANGE_TO_AP3
,  MMI_AP3_SD_REMOVE
,  MMI_AP3_ENTER_USB
,  MMI_AP3_CHANGE_FILE
,  MMI_AP3_CHANGE_BOOK
,  MMI_AP3_CHANGE_PAGE
,  MMI_AP3_USB_WRITE
}MMI_AP3_State;

//extern   _MEM_TYPE_SLOW_   MMI_AP3_State  ap3_poweron_state;
extern                    Bool           g_b_open_file_error;

extern                    Bool           g_b_mmc_sd_write;

extern char tisiFileName[128];

typedef enum
{
   MMI_AP3_NO_CHANGE_FILE
,  MMI_AP3_KEY_NEXT_CHANGE_FILE
,  MMI_AP3_KEY_PREV_CHANGE_FILE
,  MMI_AP3_REMOTE_CHANGE_FILE
}AP3_CHANGE_FILE;

extern   _MEM_TYPE_SLOW_   AP3_CHANGE_FILE   ap4_change_file;
void  ap3_mmi_appli			  ( void );
Bool  Get_File_Name           (  U8  *sz_filterext,  U16 Number );
//void  ap3srvrec_reread_off    ( void );
//void  mmi_ap3_no_ap4_exit     ( void );
Bool  scank_disk              ( void );
//void  fat_clusterlist_restore ( void );
//void  fat_clusterlist_save    ( void );
U8	  open_file               ( U16 fileNum );
Bool  remote_search_file      ( void );
void  mmc_sd_all_mode_dir_make( void );
void  Ap4_Key_Process         ( U16 Key_Code);
void  dowith_USB 				  ( void );
void change_control_mode(void);
void bak_to_news(void);
void bak_from_news(void);
//void clear_RP(void);
//void set_RP(void);
void tishi_mp3(U8 flag);
Bool find_path_u(U16 * path_str,U8 * short_path);
void read_all_news(void);
//void read_system_file(void);


#endif  //! _DIC_AP3_H_

