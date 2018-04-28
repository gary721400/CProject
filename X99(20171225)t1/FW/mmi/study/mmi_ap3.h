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

#ifndef	_MMI_AP3_H_
#define _MMI_AP3_H_

#include "conf\conf_sio.h"
#include "modules\file_system\fs_com.h"
#include "conf\conf_lcd.h"
#include "KBDM9_DDK.H"
#include "include\ap4news.h"
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if( defined _mmi_ap3_c_ )
  #define _GLOBEXT_ extern
#else
  #define _GLOBEXT_ extern
#endif


//_____ D E C L A R A T I O N S ____________________________________________

//_____ M A C R O S ________________________________________________________

//_____ D E C L A R A T I O N S ____________________________________________
_GLOBEXT_                     Bool       b_kbd_process_disable;
#define mmi_ap3_kbd_disable()    ( b_kbd_process_disable = TRUE )
#define mmi_ap3_kbd_enable()     ( b_kbd_process_disable = FALSE )
#define mmi_ap3_kbd_is_disable() ( b_kbd_process_disable == TRUE )


_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U8       ap4_play_state
#if 0//def _mmi_ap3_c_
= AP4_PLAY_ERROR
#endif
;

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

//_GLOBEXT_   _MEM_TYPE_SLOW_   MMI_AP3_State  ap3_poweron_state;

#if( AP3 == ENABLE )
_GLOBEXT_                     Bool           g_b_open_file_error;

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

//_GLOBEXT_   _MEM_TYPE_MEDSLOW_   AP4_Mode ap3_mode;
//_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U8       Ap3_MenuPath;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U16      ap3_file_number;

//_GLOBEXT_   const                U8       Ext[]
//#if 0//def _mmi_ap3_c_
//= "Ap4"
//#endif
//;

#if 1//( AP4_RP == ENABLE )
typedef union
{
   struct
   {
      unsigned    rp_go_on_flag        :1;//
      unsigned    rp_record_start      :1;//
      unsigned    rp_play_start        :1;//
      unsigned    rp_ok_flag           :1;//

      unsigned    reread_record_flag   :1;
      unsigned    rp_rev               :11;//
   }bits;
   U16      value;
}RP_BIT_NEW;
_GLOBEXT_ _MEM_TYPE_SLOW_ RP_BIT_NEW rp_flag_bits
#if 0//def _mmi_ap3_c_
  = { 0x00 }
#endif
;
#define  rp_go_on_flag        rp_flag_bits.bits.rp_go_on_flag
#define  rp_record_start      rp_flag_bits.bits.rp_record_start
#define  rp_play_start        rp_flag_bits.bits.rp_play_start
#define  rp_ok_flag           rp_flag_bits.bits.rp_ok_flag

#define  reread_record_flag   rp_flag_bits.bits.reread_record_flag
#endif   //#if( AP4_RP == ENABLE )

/*
_GLOBEXT_                     Bool           g_b_mmc_sd_write
#ifdef _mmi_ap3_c_
   = FALSE
#endif
;
*/

typedef enum
{
   MMI_AP3_NO_CHANGE_FILE
,  MMI_AP3_KEY_NEXT_CHANGE_FILE
,  MMI_AP3_KEY_PREV_CHANGE_FILE
,  MMI_AP3_REMOTE_CHANGE_FILE
}AP3_CHANGE_FILE;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   AP3_CHANGE_FILE   ap4_change_file
#if 0//def _mmi_ap3_c_
   = MMI_AP3_NO_CHANGE_FILE
#endif
;

_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32            reread_start_time;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32            reread_end_time;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32            reread_time;
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U8             s_ts_id_read;

_GLOBEXT_   _MEM_TYPE_SLOW_      U8             mmipaster_play_id_timerdisplay;

typedef enum
{
   MMI_AP3_SIMPLE_STATUS
,  MMI_AP3_RERAED_STATUS
}AP3_READ_STATUS;
_GLOBEXT_   _MEM_TYPE_SLOW_   AP3_READ_STATUS   ap4_read_status
#if 0//def _mmi_ap3_c_
   = MMI_AP3_SIMPLE_STATUS
#endif
;





ImageDeclareType( MMI_AP4_START_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_START_TEMP_CODE01 );

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( SRV_PLAYER_TEMP_CODE00 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE01 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE02 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE03 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE04 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE05 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE06 );
ImageDeclareType( SRV_PLAYER_TEMP_CODE07 );

ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE02 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE04 );
ImageDeclareType( MMI_AP4_SEARCH_TEMP_CODE05 );
ImageDeclareType( GET_FILE_NAME_CODE );

ImageDeclareType( MMI_AP4_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_TEMP_CODE02 );
ImageDeclareType( MMI_AP4_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_TEMP_CODE04 );
ImageDeclareType( MMI_AP4_TEMP_CODE05 );
ImageDeclareType( MMI_AP4_TEMP_CODE06 );
ImageDeclareType( MMI_AP4_TEMP_CODE07 );
ImageDeclareType( MMI_AP4_TEMP_CODE08 );

ImageDeclareType( READ_ALLNEWS_CODE );

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( FILE_TEMP_CODE00 );
ImageDeclareType( FILE_TEMP_CODE01 );
ImageDeclareType( FILE_TEMP_CODE02 );

ImageDeclareType( MMI_AP4_REC_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_REC_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_REC_TEMP_CODE02 );
ImageDeclareType( MMI_AP4_REC_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_REC_TEMP_CODE04 );
ImageDeclareType( MMI_AP4_REC_TEMP_CODE05 );

ImageDeclareType( RECORDER_TEMP_CODE00 );
ImageDeclareType( RECORDER_TEMP_CODE01 );
ImageDeclareType( RECORDER_TEMP_CODE02 );
ImageDeclareType( RECORDER_TEMP_CODE03 );
ImageDeclareType( RECORDER_TEMP_CODE04 );
ImageDeclareType( AUADC_COMMON_CODE );

ImageDeclareType( MMI_AP4_PLAYER_EOF_CODE );

ImageDeclareType( MMI_AP4_RP_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE02 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE04 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE05 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE06 );
ImageDeclareType( MMI_AP4_RP_TEMP_CODE07 );

ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE02 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE03 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE04 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE05 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE06 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE07 );
ImageDeclareType( MMI_AP4_RECORD_TEMP_CODE08 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );
//ImageDeclareType( LCD_SSD1303_TEMP_CODE03 );

ImageDeclareType( MMI_AP4_NOFILE_TEMP_CODE );

ImageDeclareType( MMI_AP4_CONTROL_TEMP_CODE00 );

ImageDeclareType( MMI_AP4_RELOAD_TEMP_CODE00 );

ImageDeclareType( MMI_AP4_PLAYTIME_TEMP_CODE );

ImageDeclareType( MMI_AP4_USB_TEMP_CODE00 );

ImageDeclareType( AP4_SEARCH_END_TEMP_CODE00 );

ImageDeclareType( MMI_AP4_POWEROFF_TEMP_CODE00 );

ImageDeclareType( OID_TEMP_CODE00 );

ImageDeclareType( MMI_AP4_PLAY_STOP_CODE );
ImageDeclareType( MMI_AP4_BAT_LOW_CODE );
ImageDeclareType( MMI_AP4_KEY_CODE );

ImageDeclareType( MMI_AP4_KEY_PRESS_CODE00 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE01 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE02 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE03 );

ImageDeclareType( MMI_STATUS_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_DISPTIME_CODE );
ImageDeclareType( SEARCH_REMOTE_COV_CODE );

ImageDeclareType( MMI_AP4_COV8_CODE );

ImageDeclareType( KB_RESIZE_CODE );
ImageDeclareType( FS_CREATE_BUF_CODE );

ImageDeclareType( SEARCH_BOOK_ENCRYPT_CODE );


ImageDeclareType( MMI_AP4_KEY_LONGPRESS_CODE00 );
ImageDeclareType( MMI_AP4_KEY_LONGPRESS_CODE01 );
ImageDeclareType( MMI_AP4_KEY_LONGPRESS_CODE02 );
ImageDeclareType( MMI_AP4_KEY_LONGPRESS_CODE03 );

ImageDeclareType( MMI_AP4_REREAD_REC_CODE );
ImageDeclareType( MMI_AP4_REREAD_PLAY_CODE );
ImageDeclareType( MMI_AP4_REMOTE_AP4_CODE );
ImageDeclareType( MMI_AP4_NOTFIND_AP4_CODE );
ImageDeclareType( MMI_AP4_RP_REC_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_RP_REC_TEMP_CODE01 );
ImageDeclareType( MMI_AP4_RP_PLAY_TEMP_CODE00 );
ImageDeclareType( MMI_AP4_RP_PLAY_TEMP_CODE01 );

ImageDeclareType( BROWSER_KEY_RELEASE_CODE );
ImageDeclareType( AP4_SEARCH_TOBROWSER_CODE );
ImageDeclareType( MMI_AP4_DISPTIME_CODE01 );
ImageDeclareType( MMI_AP4_DISPALLTIME_CODE );
ImageDeclareType( GENDU_CONTROL_CODE );

ImageDeclareType( MMI_AP4_KEY_PRESS_CODE04 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE05 );
ImageDeclareType( GENDU_CONTROL_TEMPCODE01 );
ImageDeclareType( GENDU_CONTROL_TEMPCODE00 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE06 );

ImageDeclareType( CHOICE_WORD_CTRL_CODE );
ImageDeclareType( LCD_LINE_CLEAR_CODE );
ImageDeclareType( LCD_LINE_WRITE_CODE );
ImageDeclareType( FIND_LINE_PARAM_CODE );

ImageDeclareType( MMI_AP4_FANYI_PRESS_CODE );
ImageDeclareType( MMI_AP4_GLOBEKEY_CODE );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE05 );
ImageDeclareType( MMI_AP4_KEY_PRESS_CODE07 );
ImageDeclareType( DISPLAY_CONTEXT_CODE );



Bool  Get_File_Name           ( U8 *sz_filterext, U16 Number );
Bool  Get_Dir_Name          ( U16 Number );
void  ap3srvrec_reread_off    ( void );
void  mmi_ap3_no_ap4_exit     ( void );
Bool  scank_disk              ( void );
#if( LCD_PRESENT == ENABLE )
void  mmi_ap3_display_volume  ( void );
void  mmi_ap3_display_icon    ( Bool flag_icon );
#else
#define mmi_ap3_display_volume()
#define mmi_ap3_display_icon()
#endif   //#if( LCD_PRESENT == ENABLE )
void  fat_clusterlist_restore ( void );
void  fat_clusterlist_save    ( void );
void  cluster_dowith(Bool flag);
void  display_control_caption ( void );
Bool  open_file               ( void );
Bool  remote_search_file      ( /*U16 _MEM_TYPE_SLOW_ file_num*/void );
void  mmi_ap4_display_dynami_time(U8 time_flag);
void mmi_ap4_display_all_time(void);
//void next_key_press(void);
//void prev_key_press(void);
void display_key_text(void);
void lcd_write_line(U8 line_num,U8 begin_point,U8 word_width);
void fanyi_key_press( void );
void changepage_key_press(U8 change_direct);
#endif  //#if( AP3 == ENABLE )

#endif  //! _MMI_AP3_H_

