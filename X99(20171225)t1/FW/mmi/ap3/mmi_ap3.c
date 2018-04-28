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
#ifndef _mmi_ap3_c_
#define _mmi_ap3_c_

//_____  I N C L U D E S ___________________________________________________
#include "lib_mcu\compiler.h"
#include <stdio.h>
#include <string.h>
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "mmi\kbd\keypad.h"

#include "mmi_ap3.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
#include "lib_system\unicode\unicode.h"
#include "modules\search\search.h"
#include "modules\search\search_task.h"
#include "mmi\status\mmi_status.h"
#include "mmi\player\srv_player.h"
#include "mmi\shared\com_var.h"

#include "sio\com.h"
#include "conf\conf_oid.h"
#include "modules\player_ap3\player_ap3.h"
//#include "mmi\player\mmi_player.h"
#include "modules\recorder\recorder.h"
#include "mmi\recorder\srv_recorder.h"
#include "drivers\oid\oid.h"
#include "modules\recorder\recorder_task.h"
#include "modules\search\search_game.h"
#include "modules\search\search_remote.h"
#include "modules\player\player.h"
//#include "lib_system\utilities\string.h"
#include "mmi\startup\mmi_startup.h"
#include "modules\player\player_task.h"
#include "modules\power\power_task.h"
#include "modules\power\power.h"             // Power module definition

#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "mmi\shared\com_var.h"
#include "conf\conf_sio.h"
#include <sfr.h>

#include "KBDM952.H"
#include "fc_internal.h"

#include "point_g.h"


#pragma DATA = MMI_AP4_DATA
#pragma CODE = MMI_AP4_CODE


#if( AP3 == ENABLE )
//_____ P R I V A T E   D E F I N I T I O N S _____________________________

// Coordinate of the setting window title "My settings"
//#define POSX_WND_TITLE                            60
//#define POSY_WND_TITLE                            8


//** To manage display **

// Position of graphical elements
#define MMIAP3_POSX_TITLE                         40
#define MMIAP3_POSY_TITLE                         4

#define MMIAP3LIST_POSX_NAME_CURRENT_FILE         10
#define MMIAP3LIST_POSX_ICON_CURSOR               2
#define MMIAP3LIST_POSY_NAME_CURRENT_FOLDER       3





//***********************************************
// ID of the timer soft used by this MMI appli
_MEM_TYPE_MEDSLOW_ U8  s_ts_ap3record_display_id;
_MEM_TYPE_MEDSLOW_ U8  remote_search_file_counter /*= 0*/;

#define AP3RECORDER_TIME   TIMER_MS( 500 )

extern   Bool  g_b_recorder_on;
extern  _MEM_TYPE_SLOW_     U8                   num_current_word;
extern  _MEM_TYPE_SLOW_     U8                   en_word_bf[LEN_EN_WORDBF];
extern  _MEM_TYPE_SLOW_     U8                   allnum_word;
extern  _MEM_TYPE_SLOW_     U8                   fenjie_num;
extern _MEM_TYPE_MEDSLOW_   U8                   u8_date[4];


extern _MEM_TYPE_SLOW_  U32             action_address;
extern                  Bool            act_do_flag;
extern _MEM_TYPE_SLOW_  U32             findword_address;
extern _MEM_TYPE_SLOW_  U16              Display_Page_Num;
extern _MEM_TYPE_SLOW_  U16              findword_num;
extern _MEM_TYPE_SLOW_  U32             ch_display_address;
extern _MEM_TYPE_SLOW_  U32             word_mp3_address;
extern _MEM_TYPE_SLOW_  U32             firstword_address;
extern _MEM_TYPE_SLOW_  U16              leng_en;
extern _MEM_TYPE_SLOW_  U16              leng_ch;
extern _MEM_TYPE_SLOW_  U16             current_display_num;
extern _MEM_TYPE_SLOW_  U32             ch_begin_address;
extern _MEM_TYPE_SLOW_  U16             current_display_num_bak;
extern _MEM_TYPE_MEDSLOW_   U16               virtualKeyState;

Bool  g_b_search_record /*= FALSE*/;

// Icon for each player state
_CONST_TYPE_ U32 mmiap3_state_icon[] = {
    IDP_PLAYER_STOP
    ,  IDP_PLAYER_PLAY
    ,  IDP_PLAYER_PAUSE
    ,  IDP_PLAYER_FFW
    ,  IDP_PLAYER_FRW
    ,  IDP_PLAYER_MARK_A
    ,  IDP_PLAYER_MARK_AB
    ,  IDP_PLAYER_MARK_AB
};

// ID of the timer soft used by this MMI appli
_MEM_TYPE_SLOW_ U8 s_ts_id;
//static _MEM_TYPE_SLOW_ U8 s_ts_id_read;

Bool  b_kbd_process_disable;
Bool  g_b_open_file_error;

_MEM_TYPE_MEDSLOW_   U8                ap4_play_state /*= AP4_PLAY_ERROR*/;

_MEM_TYPE_MEDSLOW_   U16               diandu_entrynum;
_MEM_TYPE_MEDSLOW_   U16               ap3_file_number;
_MEM_TYPE_MEDSLOW_   U16               display_page_num;
_MEM_TYPE_MEDSLOW_   U16               ap4current_displaypage_num;
_MEM_TYPE_MEDSLOW_   U16               ap4current_mark;

_MEM_TYPE_MEDSLOW_   U16               open_dir_num;

_MEM_TYPE_MEDSLOW_   U16               unit_display_num;
_MEM_TYPE_MEDSLOW_   U16               unitcurrent_display_num;
_MEM_TYPE_MEDSLOW_   U16               unitcurrent_mark;



//const    U8    Ext[] = "AP4";

_MEM_TYPE_MEDSLOW_   AP3_CHANGE_FILE   ap4_change_file /*= MMI_AP3_NO_CHANGE_FILE*/;

_MEM_TYPE_MEDSLOW_   U32               reread_start_time;
_MEM_TYPE_MEDSLOW_   U32               reread_end_time;
_MEM_TYPE_MEDSLOW_   U32               reread_time;
_MEM_TYPE_MEDSLOW_   U8                s_ts_id_read;

_MEM_TYPE_SLOW_      U8                time_buf[ 10 ];
_MEM_TYPE_SLOW_      U8                text_line_buf[ LEN_DISPLAY_BUF ];
_MEM_TYPE_SLOW_      U16               text_off_addr[ MAX_PAGE_NUMBER ];
#if 1//( AP4_RP == ENABLE )
_MEM_TYPE_SLOW_      U8                ap4_short_name[ SHORT_NAME_SIZE + 1 ];
_MEM_TYPE_SLOW_      U8                ap4_long_name[ G_STRING_SIZE ];
//_MEM_TYPE_SLOW_      RP_BIT_NEW        rp_flag_bits = { 0x00 };
#endif   //#if( AP4_RP == ENABLE )
_MEM_TYPE_SLOW_      U8                current_ap4_short_name[ 32 ];
_MEM_TYPE_SLOW_      U8                 current_ap4_short_path[ 16 ];

_MEM_TYPE_SLOW_      AP3_READ_STATUS   ap4_read_status /*= MMI_AP3_SIMPLE_STATUS*/;

_MEM_TYPE_SLOW_  char hintName[64];
_MEM_TYPE_SLOW_     U16 eofParam;
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________
U16   Get_File_Number                  ( U8 *sz_filterext );
#if( LCD_PRESENT == ENABLE )
void  back_to_ap3_prev_menu      ( void );
void  mmiap3_display_backgroung  ( void );
void  mmiap3list_display_list    ( void );
U16   ap3_nav_getnbfile          ( void );
void  mmiap3list_display_cursor  ( void );
#else
#define back_to_ap3_prev_menu()
#define mmiap3_display_backgroung()
#define mmiap3list_display_list()
#define ap3_nav_getnbfile()
#define mmiap3list_display_cursor()
#endif  //#if( LCD_PRESENT == ENABLE )

Bool  ap3srvrec_ready                  ( void );
Bool  ap3srvrec_reread_on              ( void );
Bool  mmi_ap3_no_ap4_playintro         ( void );
Bool  mmi_ap3_sd_remove_playintro      ( void );
Bool  mmi_ap3_only_simple_playintro    ( void );
Bool  mmi_ap3_only_sequence_playintro  ( void );
Bool  mmi_ap3_simple_playintro         ( void );
Bool  mmi_ap3_sequence_playintro       ( void );
Bool  mmi_ap3_not_find_playintro       ( void );
Bool  mmi_ap3_startup_playintro        ( void );
Bool  mmi_ap3_reread_status_playintro  ( void );
Bool  mmi_ap3_reread_playintro         ( void );
Bool  mmi_ap3_play_hint_mp3            ( U16 param );
void  reread_mode_enter_exit_deal      ( U8 mode );

U16   get_name_lead                    ( void );
void  mmi_ap4_power_off                ( U8 event );
void  key_long_press_deal              ( U8 keyID );
void  mmi_ap4_player_eof_deal          ( void );
void  key_press_deal                   ( U8 keyID );
void  change_file_deal                 ( U8 keyID );

Bool  rp_record_find                   ( U16 index );
Bool  rp_play_find                     ( U16 index );
Bool  Ap4_rp_play_start                ( void );
Bool  paster_record_start              ( void );
void  rp_stop_deal                     ( void );

void  ap4_start_appli_deal             ( void );
void  ap4_start_init_deal              ( void );
void  ap4_record_prepare_display       ( void );
void  ap4_record_fail_display          ( void );
void  ap4_record_ok_display            ( void );
void  ap4_record_playback_display      ( void );
void  ap4_record_playback_fail_deal    ( void );
Bool  ap4_record_time_deal             ( void );
void  ap4_original_display             ( void );
void  ap4_record_diskfull_deal         ( void );
void  ap4_search_nofind_file_display   ( void );
void  no_ap4_file_display              ( void );
void  rp_nofile_display                ( void );
void  rp_play_fail_deal                ( void );
void  mmiap4_play_time                 ( void );
void  ap4_reload_deal                  ( void );
void  mmi_ap4_usb_deal                 ( void );
void  ap4_search_end_deal              ( U8 param );
void  mmi_ap4_poweroff_deal            ( void );
void  ap4_play_stop                    ( void );
void  bat_low_waring_deal              ( U16 param );
Bool  mmi_ap4_key_deal                 ( U16 param );
Bool  power_key_press                  ( void );
void  menu_key_press                   ( U8 appli_id );
Bool  mode_key_press                   ( void );
Bool  volume_key_press                 ( void );
void  mmi_ap4_exit_to_idle             ( void );
void  power_key_long_press             ( void );
void  rec_key_long_press               ( void );
void  mode_key_long_press              ( U8 appli_id);
void  vol_key_long_press               ( U8 keyID );
void  mmi_ap4_reread_record_deal       ( void );
void  mmi_ap4_reread_play_deal         ( void );
void  remote_change_ap4_file_deal      ( void );
void  remote_not_find_ap4_deal         ( void );
void  rp_record_prepare_deal           ( void );
void  rp_record_start_deal             ( void );
void  rp_play_prepare_deal             ( void );
void  rp_play_start_deal               ( void );

Bool  browser_key_release              ( U16 param );
void  ap4_search_end_to_browser        ( U8 param );
Bool  mmi_ap4_globekey_deal( U16 param );
void  fanye_display_text(U8 key_data );
void  ready_change_book(void);
Bool  bak_key_press(void);
//Bool  bak_key_press_two(void);
Bool  ctrl_display( U16 param );
Bool  ctrl_display_ready( U16 param );
void mmi_ap4_no_player_deal( U16 param);
/*********************** Menus **********************/

extern   Bool                 g_b_player_on;
extern   Bool                 g_b_play_ap3;


extern  _MEM_TYPE_SLOW_  U32                   Addr_Fanyi;
extern  _MEM_TYPE_SLOW_ U8                     Num_Fanyi;
extern  _MEM_TYPE_SLOW_ U8                     Num_Cur_Fanyi;

//extern                  Bool  Usb_connect;

//extern _MEM_TYPE_SLOW_  U8    g_paster_play_file_type;

extern  void  player_stop_stream    ( void );
extern  void  player_codec_stop     ( void );
extern  void   gendu_tiaozhen(void);
extern  void set_oid_type(Bool set_type);
//_MEM_TYPE_MEDSLOW_   U8       ap4_play_state;
_MEM_TYPE_MEDSLOW_   U8       internal_speech;

//Bool  b_kbd_process_disable;

extern __X   FCFILE   *ADPCM_File;


//===========================================
ImageDeclareType(READY_CHANGE_BOOK_CODE);
ImageDeclareType(OID_INIT_CODE);
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
void ap3_mmi_appli( U8 event, U16 param )
{
    switch( event ) {
        case EVT_START_APPLI:
            SDKLoadCode( MMI_AP4_START_TEMP_CODE00 );
            ap4_start_appli_deal();
            break;

        case EVT_START_AP3_APPLI:
            SDKLoadCode( MMI_AP4_START_TEMP_CODE01 );
            ap4_start_init_deal();
            break;

        /*case EVT_APPLI_KILLED:
        break;

        case EVT_BACK_TO_TOP:
        break;*/


        case EVT_ENTER_IN_POWEROFF:
        //g_power_off_mode = POWER_OFF;
        //goto mmi_ap4_power_off;

        case EVT_ALERT_BATTERY_LOW:
            //g_power_off_mode = BATTERY_LOW;
//mmi_ap4_power_off:
            /*Search_Record.news_sys.index_normal_flag = 0;
            b_play_repeat = FALSE;
            b_remote_play_repeat = FALSE;*/

            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_REC_TEMP_CODE02 );
            mmi_ap4_power_off( event );
            break;

        case EVT_CHANGE_OID_TYPE:
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            /*
            if(g_bluetooth){
                g_bluetooth = FALSE;
                }
            else{
                g_bluetooth = TRUE;
                }*/
            g_bluetooth = ~g_bluetooth;
            //SendString(( U8 *)"\n\ro");
            SDKLoadCode(OID_INIT_CODE);
            SDKLoadCode(T01_COMMAND_CODE);
            set_oid_type(g_bluetooth);
            lcd_goto_xy( OID_TYPE_FLAG_X, OID_TYPE_FLAG_Y);
            Ih_set_curr_font( IDC_FONT_08 );
            if(g_bluetooth) {
                lcd_puts(( U8 *)"OID2");
            } else {
                lcd_puts(( U8 *)"DPR3");
            }
            break;


        case EVT_CHANGE_AP4_VOL:
            srvplayer_volume_send();
            mmi_ap3_display_volume();
            break;
        case EVT_SEARCH_AP3EXIT:
            SDKLoadCode( MMI_AP4_TEMP_CODE00 );
            mmi_ap3_no_ap4_exit();
            break;

        case EVT_SEARCH_END:
            SDKLoadCode( AP4_SEARCH_END_TEMP_CODE00 );
            ap4_search_end_deal( LSB( param ));
            break;

        case EVT_SEARCH_TO_BROWSER:
            SDKLoadCode( AP4_SEARCH_TOBROWSER_CODE );
            ap4_search_end_to_browser( LSB( param ));
            break;
        /*
              case EVT_SEARCH_FLUSH_ALLTIME:
              SDKLoadCode( MMI_AP4_DISPALLTIME_CODE );
              mmi_ap4_display_all_time();
              break;
        */
        case EVT_SEARCH_AP3POWEROFF:
            SDKLoadCode( MMI_AP4_POWEROFF_TEMP_CODE00 );
            mmi_ap4_poweroff_deal();
            break;

        case EVT_SEARCH_FLUSH_TEXT:
            // SendString((U8 *)"\r\nflushtext");
            // SendString((U8 *)"\r\nflush=");
            // send_hex_u32(timer_get_time());
            SDKLoadCode( TEXT_CH_DISPLAY_CODE );
            (void)text_chinese_display(ch_display_address);
            //SendString((U8 *)"\r\naddr:page_num:off=");
            // send_hex_u32(ch_display_address);SendString((U8 *)":");
            //send_hex_u16(Display_Page_Num);SendString((U8 *)":");
            //send_hex_u16(text_off_addr[Display_Page_Num]);
            // SendString((U8 *)":");
            //send_hex_u32(timer_get_time());
            break;

        case EVT_SEARCH_FLUSH_CONTROL:
            // SendString((U8 *)"\r\nflushtext");
            // SendString((U8 *)"\r\nflush=");
            // send_hex_u32(timer_get_time());
            SDKLoadCode( CTRL_DISPLAY_READY_CODE );
            (void)ctrl_display_ready(param);
            SDKLoadCode( CTRL_DISPLAY_CODE );
            (void)ctrl_display(param);
            //SendString((U8 *)"\r\naddr:page_num:off=");
            // send_hex_u32(ch_display_address);SendString((U8 *)":");
            //send_hex_u16(Display_Page_Num);SendString((U8 *)":");
            //send_hex_u16(text_off_addr[Display_Page_Num]);
            // SendString((U8 *)":");
            //send_hex_u32(timer_get_time());
            break;

        case EVT_KEY_PRESSED:

            SDKLoadCode( MMI_AP4_KEY_CODE );
            if( mmi_ap4_key_deal( param ))
                break;
            if( KBD_KEY_LONGPRESSED == KEY_STATUS( param )) {
                //SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
                //ap4_play_stop();
                SDKLoadCode( MMI_AP4_REC_TEMP_CODE03 );
                key_long_press_deal( KEY_ID( param ));
            } else if( KBD_KEY_RELEASED == KEY_STATUS( param )) {

                SDKLoadCode( MMI_AP4_REC_TEMP_CODE05 );
                key_press_deal( KEY_ID( param ));
            }
            break;

//***********************************************
        case EVT_USB_POWERED:
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_USB_TEMP_CODE00 );
            mmi_ap4_usb_deal();
            break;
//***********************************************
        case EVT_SEARCH_REREADMODE:
            SDKLoadCode( MMI_AP4_TEMP_CODE08 );
            reread_mode_enter_exit_deal( LSB( param ));
            break;

        case EVT_SEARCH_PLAYREREAD:   //跟读开始，播放提示音
            SDKLoadCode( MMI_AP4_TEMP_CODE07 );
            ( void )mmi_ap3_reread_playintro();
            break;

        case EVT_SEARCH_AP3RECORD://跟读开始
            SDKLoadCode( MMI_AP4_REREAD_REC_CODE );
            mmi_ap4_reread_record_deal();
            break;

        case EVT_SEARCH_AP3RECORD_PLAY://跟读播放
            SDKLoadCode( MMI_AP4_REREAD_PLAY_CODE );
            mmi_ap4_reread_play_deal();
            break;

        case EVT_TIMER:
#if 1
            if( LSB(param) == s_ts_id ) {

                SDKLoadCode( MMI_AP4_DISPTIME_CODE );
                mmi_ap4_display_dynami_time(PLAY_CURRENT_TIME);

//        mmi_ap4_exit_to_idle();
                /*Ts_free( s_ts_id );
                g_select_appli = IDLE_APPLI;
                Mmgr_kill_this_mmi_appli();*/
                break;
            }

            if( LSB(param) == s_ts_id_read ) {
                //Ts_free( s_ts_id_read );
                SDKLoadCode( MMI_AP4_RELOAD_TEMP_CODE00 );
                ap4_reload_deal();
                break;
            }
#endif
            if( LSB( param ) == s_ts_ap3record_display_id ) {
                //ts_set_time( s_ts_ap3record_display_id, AP3RECORDER_TIME );
                SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE06 );
                if( ap4_record_time_deal()) {
                    SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE05 );
                    ap4_original_display();
                }
                break;
            }
#if 0
            if( LSB(param) == mmipaster_play_id_timerdisplay ) {
                /*
                //ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
                SDKLoadCode( SRV_PLAYER_TEMP_CODE07 );
                srvplayer_gettime();
                SDKLoadCode( MMI_AP4_PLAYTIME_TEMP_CODE );
                mmiap4_play_time();
                //*/
            }
            /*else
            {
               Mmgr_krn_forward_current_event();
            }*/
#endif
            break;

        case EVT_BAT_WARING:
            SDKLoadCode( MMI_AP4_BAT_LOW_CODE );
            bat_low_waring_deal(param);
            break;

        case EVT_RECORDER:
            //if( param == RECORD_EOF )
            /*{
               Ts_free( s_ts_ap3record_display_id );
               scan_oid_enable();
               mmi_ap3_kbd_enable();
            }*/
            SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE07 );
            ap4_record_diskfull_deal();
            break;

        case EVT_SEARCH_HINT_PLAY:
            SDKLoadCode( MMI_AP4_TEMP_CODE09 );
            (void)mmi_ap3_play_hint_mp3(param);
            break;

        case EVT_PLAYER:
            SDKLoadCode( MMI_AP4_PLAYER_EOF_CODE );
            mmi_ap4_player_eof_deal();
            break;

        case EVT_PLAYER_NO_FILE:
            SDKLoadCode( MMI_AP4_NO_PLAYER_CODE );
            mmi_ap4_no_player_deal(param);
            break;
        case EVT_GOTO_OID_MODE:
            SDKLoadCode(READY_CHANGE_BOOK_CODE);
            ready_change_book();
            break;

        case EVT_SEARCH_CHANGE_FILE:
            SDKLoadCode( MMI_AP4_REC_TEMP_CODE04 );
            change_file_deal( LSB( param ));
            break;

        case EVT_SEARCH_REMOTE_CHANGE_AP3:
            SDKLoadCode( MMI_AP4_REMOTE_AP4_CODE );
            remote_change_ap4_file_deal();
            break;

        case EVT_SEARCH_NO_FIND_FILE:
            SDKLoadCode( MMI_AP4_NOTFIND_AP4_CODE );
            remote_not_find_ap4_deal();
            break;

            /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
            power_system_off( !power_is_vbus_present());
            break;*/

#if( AP4_RP == ENABLE )
        case EVT_SEARCH_RP_RECORD:
            SDKLoadCode( MMI_AP4_RP_REC_TEMP_CODE00 );
            rp_record_prepare_deal();
#if 0
            SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE00 );
            ap4_record_prepare_display();
            //SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
            SDKLoadCode( MMI_AP4_RP_TEMP_CODE01 );
            if( !rp_record_find( rp_oid_index )) {
                SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE01 );
                ap4_record_fail_display();
                break;
            }
#endif
            break;

        case EVT_SEARCH_RP_RECORD_START:
            SDKLoadCode( MMI_AP4_RP_REC_TEMP_CODE01 );
            rp_record_start_deal();
#if 0
            //scan_oid_disable();
            //mmi_ap3_kbd_disable();
            SDKLoadCode( MMI_AP4_RP_TEMP_CODE02 );
            if( !paster_record_start()) {
                //scan_oid_enable();
                //mmi_ap3_kbd_enable();
                SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE01 );
                ap4_record_fail_display();
            }
#endif
            break;

        case EVT_SEARCH_RP_PLAY:
            SDKLoadCode( MMI_AP4_RP_PLAY_TEMP_CODE00 );
            rp_play_prepare_deal();
#if 0
            //SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
            SDKLoadCode( MMI_AP4_RP_TEMP_CODE03 );
            if( !rp_play_find( rp_oid_index )) {
                SDKLoadCode( MMI_AP4_RP_TEMP_CODE06 );
                rp_nofile_display();
                break;
            }
#endif
            break;

        case EVT_SEARCH_RP_PLAY_START:
            SDKLoadCode( MMI_AP4_RP_PLAY_TEMP_CODE01 );
            rp_play_start_deal();
#if 0
            SDKLoadCode( MMI_AP4_RP_TEMP_CODE04 );
            if( !Ap4_rp_play_start()) {
                SDKLoadCode( MMI_AP4_RP_TEMP_CODE07 );
                rp_play_fail_deal();
                break;
            }
#endif
            break;

        case EVT_SEARCH_RP_STOP:
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_RP_TEMP_CODE05 );
            rp_stop_deal();
            break;
#endif   //#if( AP4_RP == ENABLE )

        default:
            Mmgr_krn_forward_current_event();
            break;
    }
}



#pragma CODE = MMI_AP4_COV8_CODE

char *convert_U8( char *p, U8 i )
{
    if( i / 10 )
        p = convert_U8( p, i / 10 );
    //else if( i < 0 )
    //   *p++ = '-';
    return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}


#pragma CODE = MMI_AP4_PLAYTIME_TEMP_CODE

void mmiap4_play_time( void )
{
    /*
       _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;

       Ih_set_curr_font( IDC_FONT_08 );
       lcd_goto_xy( 4, 56 );
       //sprintf( string , "%02bu:%02bu:%02bu", srvplayer_g_arg.start_time.hours, srvplayer_g_arg.start_time.minutes, srvplayer_g_arg.start_time.seconds );
       memset( string, 0, 9+4 );
       SDKLoadCode( MMI_AP4_COV8_CODE );
       convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.hours );
       i = strlen(( char *)&string[ 9 ] );
       for( ; i < 2; i++ )
          strcat(( char *)string, "0" );
       strcat(( char *)string, ( char *)&string[ 9 ] );
       strcat(( char *)string, ":" );

       memset( &string[ 9 ], 0, 4 );
       convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.minutes );
       i = strlen(( char *)&string[ 9 ] );
       for( ; i < 2; i++ )
          strcat(( char *)string, "0" );
       strcat(( char *)string, ( char *)&string[ 9 ] );
       strcat(( char *)string, ":" );

       memset( &string[ 9 ], 0, 4 );
       convert_U8(( char *)&string[ 9 ], srvplayer_g_arg.start_time.seconds );
       i = strlen(( char *)&string[ 9 ] );
       for( ; i < 2; i++ )
          strcat(( char *)string, "0" );
       strcat(( char *)string, ( char *)&string[ 9 ] );
       lcd_puts( string );
    //*/
}


#if 1
///*
//#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE06
#pragma CODE = GET_DIR_NAME_CODE
//the function is get the file name;
Bool Get_Dir_Name( U16 Number  )
{
    U16 _MEM_TYPE_SLOW_ Number_Temp = 0;
    U8 _MEM_TYPE_SLOW_ dot_index;
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( nav_file_isdir()) {
            if( fat_g_cur_nav->name[ 0 ] != '.' ) {
                Number_Temp++;
                if( ( Number + 1 ) == Number_Temp ) {
                    for( dot_index = 0; dot_index < SHORT_NAME_SIZE; ++dot_index ) {
                        ap4_short_name[ dot_index ] = fat_g_cur_nav->name[ dot_index ];
                    }
                    ap4_short_name[ dot_index ] = 0;
                    return nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
                }
            }
        }
    }
    return FALSE;
}
//*/
//#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE06
#pragma CODE = GET_FILE_NAME_CODE
//the function is get the file name;
Bool Get_File_Name( U8 *sz_filterext, U16 Number )
{
    U16 _MEM_TYPE_SLOW_ Number_Temp = 0;
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( !nav_file_isdir()) {
            if( nav_file_checkext( sz_filterext )
                && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string )/*unicode_get_required_size( Unicode_get_addr_string( g_string ))*/, FS_NAME_GET, FALSE )) {
                Number_Temp++;
            }
            if(( Number + 1 )== Number_Temp ) {
#if( AP4_RP == ENABLE )
                U8 _MEM_TYPE_SLOW_ dot_index;
                U16 *p1, *p2;

                for( dot_index = 0; dot_index < SHORT_NAME_SIZE; ++dot_index ) {
                    if( fat_g_cur_nav->name[ dot_index ] == '.' )
                        break;
                    if( fat_g_cur_nav->name[ dot_index ] == 0 )
                        break;
                    ap4_short_name[ dot_index ] = fat_g_cur_nav->name[ dot_index ];
                }
                ap4_short_name[ dot_index ] = 0;

                p1 = ( U16 *)Unicode_get_addr_string( g_string );
                p2 = ( U16 *)ap4_long_name;
                for( dot_index = 0; dot_index <( G_STRING_SIZE / 2 - 1 ); ++dot_index ) {
                    if( *p1 == '.' )
                        break;
                    if( *p1 == 0 )
                        break;
                    *p2++ = *p1++;
                }
                *p2 = 0;
#endif   //#if( AP4_RP == ENABLE )
                return TRUE;
            }
        }
    }
    return FALSE;
}


#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE04

//the function is find the number of ap3 in the root;
U16 Get_File_Number( U8 *sz_filterext )
{
    U16 _MEM_TYPE_SLOW_ Number = 0;
    //SendString((U8 *)"file num\r\n");
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        /*
          SendString((U8 *)"\r\ntemp_name=");
          SendString((U8 *)fat_g_cur_nav->name);
         */
        if( !nav_file_isdir()) {
            if( nav_file_checkext( sz_filterext )) {
                Number++;
                //SendString((U8 *)"file+\r\n");
            }
        }
    }
    /*
    SendString((U8 *)"Number=");
    send_hex_u16(Number);
    SendString((U8 *)"\r\n");
    //*/
    return Number;
}


#if( LCD_PRESENT == ENABLED )
//--------------   mmi ap3 DISPLAY routines ------------------------------------

#pragma CODE = MMI_AP4_TEMP_CODE05

//! Draw the background
//!
void mmiap3_display_backgroung( void )
{
    //SDKLoadCode( LCD_SSD1303_TEMP_CODE03 );
    lcd_turn_on();
    Lcd_start_backlight_timer();

    //lcd_draw_picture( 0, 0, IDP_BACKGROUND );
    //lcd_draw_picture( 0, 0, IDP_RADIO_WND_ICON );
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//  lcd_clear_screen();
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
    lcd_clear_part_of_screen(20, 0,64, 8);
    lcd_goto_xy( 20, 0 );
    Ih_set_curr_font( IDC_FONT_08);
    lcd_puts(( U8 *)"READ" );
    if( !bRemoteEnterCal ) {
        Ih_set_curr_font( IDC_FONT_16 );
#if( afaya_pen == ENABLE )
        lcd_goto_xy( 28, 12 );
        if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP) {
            lcd_puts(( U8 *)"点读笔" );
        } else {
            lcd_puts(( U8 *)"Afaya Pen" );
        }

#endif   //#if( afaya_pen == ENABLE )
#if( i_speak == ENABLE )
        lcd_goto_xy( 36, 12 );
        lcd_puts(( U8 *)"I speak" );
#endif   //#if( i_speak == ENABLE )
        /*
              lcd_goto_xy( 52, 0 );
              Ih_set_curr_font( IDC_FONT_08 );
              lcd_puts(( U8 *)"READ" );
        */
    } else {
        lcd_goto_xy( 24, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts(( U8 *)"Calculator" );
    }
    SDKLoadCode( MMI_STATUS_TEMP_CODE01 );
    status_display();

    lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
    mmi_ap3_display_volume();
}


#pragma CODE = MMI_AP4_CODE

//! @brief Display volume
//!
void mmi_ap3_display_volume( void )
{
    _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

    Ih_set_curr_font( IDC_FONT_08 );
    lcd_goto_xy( 6, 0 );
    //sprintf( string , "%02bu", srvplayer_g_arg.volume );
    memset( string, 0, 8 );
    SDKLoadCode( MMI_AP4_COV8_CODE );
    convert_U8(( char *)&string[ 4 ], g_volume );
    i = strlen(( char *)&string[ 4 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 4 ] );
    lcd_puts( string );
}


//! @brief Display state icon
//!
void mmi_ap3_display_icon( Bool flag_icon )
{
    //if(!mmi_player_display_is_autorize())  return;
#if 0
    if( News_Control.Type_Control == GAME_TYPE )
        return;
#endif
//  if( !flag_icon )return;
//   if( g_b_ap4_caption )return;
//   lcd_draw_picture(0,12, mmiap3_state_icon[ srvplayer_g_player_state ] );

}

#endif  //#if( LCD_PRESENT == ENABLE )


#pragma CODE = MMI_AP4_REC_TEMP_CODE00

//#define AP4_REC_FILE    "0:\\ap4\\record.wav"
//#define AP4_REC_FILE    "0:\\ap4\\ap3record.wav"
#define AP4_REC_FILE    "ap3record.wav"


//! Ready the ap3 record
//!
Bool ap3srvrec_ready( void )
{
#if 0

#else
    __X FS_ERR err;
    __X U32 file_size;
    U16 mode;
    Bool fat_resize_flag = TRUE;

    //SendString((U8 *)"AP4RECORD\r\n");
    //init_array_v();
    scan_oid_disable();
    mmi_ap3_kbd_disable();
    record_result = RECORD_ERROR;
    gbRereadingFlag = false;
    mode = FS_MODE_READ_WRITE;
    //strcpy( tisiFileName, AP4_REC_FILE );
    //strcpy( tisiFileName,AP4_PATH);
    strcpy( tisiFileName,(char *)&current_ap4_short_path);
    strcat( tisiFileName,AP4_REC_FILE);
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath )) {
        mode = FS_MODE_CREATE;
        //file_size = MAX_LENGTH;

        //strcpy( tisiFileName,AP4_PATH);
        strcpy( tisiFileName,(char *)&current_ap4_short_path);
        ( void )find_path_a(( U8 *)tisiFileName, ( U8 *)tisiFileShortpath );
    } else {
        file_size = FSGetFileSize( tisiFileShortpath, &err );
        if( file_size >= MAX_LENGTH )
            fat_resize_flag = FALSE;
    }

    if( mode == FS_MODE_READ_WRITE ) {
        ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
    } else {
        //ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, mode, &err );
        ADPCM_File = ( __X FCFILE *)FSOpenForUnicode2(( char *)tisiFileShortpath, ( char *)"record~5.wav", ( U8 *)atou(( U8 *)"ap3record.wav"), FS_MODE_CREATE, &err );

        //SDKLoadCode( KB_RESIZE_CODE );
        //kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );

        strcat( tisiFileShortpath, "\\" );
        strcat( tisiFileShortpath, "record~5.wav" );
        ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
    }
    if( err ) {
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }

    SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE02 );
    ap4_record_ok_display();


    if( fat_resize_flag ) {
        file_size = MAX_LENGTH;
        SDKLoadCode( KB_RESIZE_CODE );
        err = kb_fat_resize(( __X FCFILE *)ADPCM_File, file_size );
        if( err != FS_NO_ERROR ) {
            kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
            FSClose(( __X FCFILE *)ADPCM_File );
            if( err == FS_WRITE_OUT_RANGE )
                record_result = RECORD_MEMORY_FULL;
            return FALSE;
        }
    }

    WavStepSize = file_size;

    SDKLoadCode( RECORDER_TEMP_CODE00 );
    if( !record_on()) {
        //FSResize(( __X FCFILE *)ADPCM_File, 0 );
        SDKLoadCode( KB_RESIZE_CODE );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }
    SDKLoadCode( RECORDER_TEMP_CODE01 );
    if( !record_start()) {
        //FSResize(( __X FCFILE *)ADPCM_File, 0 );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }


    record_result = RECORD_OK;
    lcd_draw_picture( 4, 26, IDP_PLAYER_RECORD );

    power_forbid_autopoweroff();
    //g_b_search_record = TRUE;
    s_ts_ap3record_display_id = ts_alloc();
    ts_set_time( s_ts_ap3record_display_id, AP3RECORDER_TIME );
    green_led_stop_timer();
    red_led_stop_timer();
    red_led_on();
    green_led_on();
    //reread_end_time = Timer_set_timer( reread_end_time );

    reread_record_flag = TRUE;
    gbRereadingFlag = true;
    return TRUE;
#endif
}


#pragma CODE = MMI_AP4_REC_TEMP_CODE01

//! Start Reread the ap3 record
//!
Bool ap3srvrec_reread_on( void )
{
    __X FS_ERR fs_err;

    g_b_search_record = TRUE;

    //strcpy( tisiFileShortpath, AP4_REC_FILE );
    //strcpy( tisiFileName, AP4_REC_FILE );
    //strcpy( tisiFileName, AP4_REC_FILE );
    //strcpy( tisiFileName,AP4_PATH);
    strcpy( tisiFileName,(char *)&current_ap4_short_path);
    strcat( tisiFileName,AP4_REC_FILE);
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath ))
        return FALSE;

    if( FSGetFileSize( tisiFileShortpath, &fs_err )== 0 )
        return FALSE;

    audio_is_wav = 1;
    g_b_play_ap3 = FALSE;
    ///*
    //SendString((U8 *)tisiFileShortpath);
    if( !player_play_file( ( char *)tisiFileShortpath )) {
        //SendString((U8 *)"play error\r\n");
        return FALSE;
    }
    //*/
    /*power_forbid_autopoweroff();

    srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
    player_task_start();       // Enable event of player task
    SendString((U8 *)"play star\r\n");*/
    return TRUE;
    //return FALSE;
}


#pragma CODE = MMI_AP4_REC_TEMP_CODE02

void mmi_ap4_power_off( U8 event )
{
    Search_Record.news_sys.index_normal_flag = 0;
    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;

#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    scan_oid_disable();
    mmi_ap3_kbd_disable();
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    //if( ap4_play_state != AP4_PLAY_ERROR )
    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }

    if( s_ts_id_read != UNDEFINED_TIMER ) {
        Ts_free( s_ts_id_read );
    }
    /*
          SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
          lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
          lcd_goto_xy( 15, 24 );
          Ih_set_curr_font( IDC_FONT_16 );
          lcd_puts( g_string );
    */
    mmi_ap3_display_icon(FALSE);
#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;

    if( rp_ok_flag )
    {
       rp_ok_flag = FALSE;
    }
    rp_flag_bits.value = 0;*/

    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
    if( rp_ok_flag ) {
        rp_ok_flag = FALSE;
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );
              lcd_goto_xy( 15, 24 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
    }
#endif   //#if( AP4_RP == ENABLE )

    reread_enable_flag = FALSE;
    if( event == EVT_ENTER_IN_POWEROFF )
        g_power_off_mode = POWER_OFF;
    else
        g_power_off_mode = BATTERY_LOW;

    SendString((U8 *)"\r\n1279\r\n");

    if( Ap3_MenuPath != AP3_MENU_PATH ) {
        Search_Record.news_sys.flag_anjian = 1;
        if( event == EVT_ENTER_IN_POWEROFF )
            Search_Record.news_sys.poweroff_flag = 1;
        else
            Search_Record.news_sys.poweroff_flag = 3;
        return;
    }
    SendString((U8 *)"\r\n1290\r\n");
    mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
}


#pragma CODE = MMI_AP4_REC_TEMP_CODE03

void key_long_press_deal( U8 keyID )
{
    switch( keyID ) {
        /*
              case KEY_POWER:
              SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
              ap4_play_stop();
              SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE00 );
              power_key_long_press();

              break;

        #if ( PASTER_MODE == ENABLE )
              case KEY_MEN:
              SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
              ap4_play_stop();
              SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE01 );
              rec_key_long_press();
        #if 0

        #endif
              break;
        #endif   //#if( PASTER_MODE == ENABLE )

              case KEY_MODE:
              SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
              ap4_play_stop();
              SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
              mode_key_long_press(IDLE_APPLI);
              break;

              case KEY_FANYI:
              SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
              ap4_play_stop();
              SDKLoadCode( MMI_AP4_KEY_PRESS_CODE01 );
              menu_key_press( RECORDER_APPLI );
              break;
        */
        case KEY_VOL_UP:
        case KEY_VOL_DOWN:
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE03 );
            vol_key_long_press( keyID );
            break;
    }
}


#pragma CODE = MMI_AP4_REC_TEMP_CODE04

void change_file_deal( U8 keyID )
{
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();
    ap3_poweron_state = MMI_AP3_CHANGE_FILE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    bRemoteEnterCal = FALSE;
    bInCal = FALSE;

    switch( keyID ) {
        case Browser_File_Init:
            ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
            g_b_open_file_error = TRUE;
            g_b_search_record = FALSE;
            State_Search = Browser_File_Init;
            break;
        case KEY_VOL_UP:
            //SendString((U8 *)"next\r\n");
            Current_Record.Ap3_num++;
            if( Current_Record.Ap3_num >= ap3_file_number ) {
                //change_disk();
                Current_Record.Ap3_num = 0;
            }
            ap4_change_file = MMI_AP3_KEY_NEXT_CHANGE_FILE;
            /*State_Search = Open_File_Search;
            g_b_open_file_error = TRUE;
            init_array_v();
            g_b_recorder_on = FALSE;
            g_b_search_record = FALSE;
            mmi_ap3_kbd_enable();
            scan_oid_enable();*/
            goto key_change_file;
            break;

        case KEY_VOL_DOWN:
            //SendString((U8 *)"prev\r\n");
            if( !Current_Record.Ap3_num ) {
                //change_disk();
                Current_Record.Ap3_num = ap3_file_number - 1;
            } else
                Current_Record.Ap3_num--;
            ap4_change_file = MMI_AP3_KEY_PREV_CHANGE_FILE;
        key_change_file:
            State_Search = Open_File_Search;
            g_b_open_file_error = TRUE;
            init_array_v();
            g_b_search_record = FALSE;
            mmi_ap3_kbd_enable();
            scan_oid_enable();
            break;
    }
}

#pragma CODE = GENDU_CONTROL_TEMPCODE00
/*
void gendu_control_02(void)
{
//SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
//( void )srvplayer_pause();

op_disable();
exrcen_disable();
AudioMP3SetPlayTime(point_A);
//AudioMP3Play();
//SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
//( void )srvplayer_restart_play();

op_enable();
exrcen_enable();
}
*/
void gendu_control_00(void)
{
    if(count_AB == 3) {
        SendString((U8*)"\r\n3");
        count_AB = 0;
        if( audio_is_wav == 0 ) {
            //AudioMP3SetPlayTime(point_A);
            //AudioMP3Play();
            //point_A = point_B;
            //gendu_control_02();
            gendu_tiaozhen();
        }
        SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen(44, 56, 16, 8);
        return;
    }
    if(count_AB == 2) {
        SendString((U8*)"\r\n2");
        count_AB ++;
        if( audio_is_wav == 0 ) {
            if((point_B - point_A)<2000) {
                count_AB = 0;
                return;
            } else {
                gendu_tiaozhen();
            }
        }
        lcd_draw_picture(44, 56, IDP_PLAYER_MARK_AB);
    }
}
#pragma CODE = GENDU_CONTROL_TEMPCODE01
void gendu_control_01(void)
{
    if(count_AB == 1) {
        SendString((U8*)"\r\n1");
        if( audio_is_wav == 0 ) {
            point_B = current_play_time;
        }
        count_AB ++;
        lcd_draw_picture(44, 56, IDP_PLAYER_MARK_B);
    }
    if(count_AB == 0) {
        SendString((U8*)"\r\n0");
        if( audio_is_wav == 0 ) {
            point_A = current_play_time;
        }
        count_AB ++;
        lcd_draw_picture(44, 56, IDP_PLAYER_MARK_A);
    }
}
#pragma CODE = GENDU_CONTROL_CODE
void gendu_control(void)
{
    if(srvplayer_g_player_state != MMI_PLAYER_STATE_PLAY)return;
    if(count_AB > 0x7F)return;
    if(count_AB < 2) {
        SDKLoadCode(GENDU_CONTROL_TEMPCODE01);
        gendu_control_01();
    } else {
        SDKLoadCode(GENDU_CONTROL_TEMPCODE00);
        gendu_control_00();
    }
}

#pragma CODE = LCD_LINE_CLEAR_CODE
void lcd_clear_line(U8 line_num )
{
    U8 column_number;
    lcd_write_command(0x40);
    lcd_write_command(0xB0);
    lcd_write_command(line_num);
    for(column_number=0; column_number<LCD_WIDTH; column_number++) {
        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(0);
    }
}
#pragma CODE = LCD_LINE_WRITE_CODE
void lcd_write_line(U8 line_num,U8 begin_point,U8 word_width)
{
    U8 all_width,column_number;
//     lcd_write_command(0x40);
    all_width = (begin_point+word_width);
    //if(all_width > LCD_WIDTH)all_width=LCD_WIDTH-1;
    lcd_write_command(0xB0);
    lcd_write_command(line_num);
    for(column_number=begin_point; column_number<all_width; column_number++) {
        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(0xFF);
    }
}

#pragma CODE = FIND_LINE_PARAM_CODE
void find_line_param(void)
{
    U8 i,j,temp_line,w_len,m;
    Bool temp_flag = TRUE;
    temp_line = FIRST_LINE_ADDR;
    j = en_word_bf[(num_current_word -1)];
    i = en_word_bf[num_current_word];
    //SendString((U8 *)"\r\nnum_current_word:j:i=");
// send_hex_u8(num_current_word);send_hex_u8(j);send_hex_u8(i);
    //m = 0;
    //while(en_word_bf[m]<= 32){
    //    m++;
    //    }
#if 1
    if(num_current_word == 1)temp_flag = FALSE;
    if( i > 32) {
        u8_date[0] = SECOND_LINE_ADDR;
        if( j <= 32) {
            u8_date[1]= 0;
            u8_date[2] = (i - j)<<2;
            temp_flag = FALSE;
        } else {

            i -= j;
            j -= fenjie_num;
            if(num_current_word == allnum_word) {
                i = (U8)leng_en - en_word_bf[(num_current_word -1)];
                temp_flag = FALSE;
                //send_hex_u8(i);
            }


            u8_date[1]= j<<2;
            u8_date[2] = i<<2;
        }
    } else {
        u8_date[0]= temp_line;
        u8_date[1]= j<<2;
        u8_date[2] = (i - j)<<2;
    }
    if(temp_flag) {
        u8_date[1] += 2;
    }
    /*
      if(num_current_word != 1){
         u8_date[1] += 2;
         if( u8_date[2] > 2)u8_date[2] -= 2;
        }
      */
#else
    if( i > 32) {
        if( j <= 32) {
            u8_date[0]= temp_line;
            u8_date[1]= j<<2;
            u8_date[2] = (32 - j)<<2;
        } else {
            temp_line = SECOND_LINE_ADDR;
            //m = en_word_bf[(num_current_word -2)];

            i -= j;
            j -= 32;
            u8_date[0]= temp_line;
            u8_date[1]= j<<2;
            u8_date[2] = i<<2;
        }
    } else {
        u8_date[0]= temp_line;
        u8_date[1]= j<<2;
        u8_date[2] = (i - j)<<2;
    }

    if(num_current_word != 1) {
        u8_date[1] += 2;
        if( u8_date[2] > 2)u8_date[2] -= 2;
    }
#endif
    i = u8_date[1];
    j = u8_date[2];
    if((i+j)>LCD_WIDTH) {
        j = LCD_WIDTH - i - 1;
    }
    u8_date[2] = j;
    SendString((U8 *)"\r\n1=");
    send_hex_u8(u8_date[0]);
    send_hex_u8(u8_date[1]);
    send_hex_u8(u8_date[2]);
}
#pragma CODE = DISPLAY_CONTEXT_CODE
void fanye_display_text(U8 key_data )
{
    if(!g_b_flush_display)return;
    if(key_data == KEY_NEXT) {
        if(b_LastPage_Flag)return;
        current_display_num = text_off_addr[Display_Page_Num];
        ch_display_address = ch_begin_address + (U32)current_display_num;
    } else {
        if(Display_Page_Num <= 1)return;
        Display_Page_Num -= 2;
        current_display_num = text_off_addr[Display_Page_Num];
        ch_display_address = ch_begin_address + (U32)current_display_num;
    }
    mail_send_event( EVT_SEARCH_FLUSH_TEXT, 0);
    g_b_first_display_flag = TRUE;
}
#pragma CODE = CHOICE_WORD_CTRL_CODE
void choice_word(U8 change_id)
{
    U8 i,j,temp_line,w_len,begin_p;

    if(!b_ready_findword)return;
    if(b_fanyi)return;
    if( KEY_NEXT == change_id ) {
        if( num_current_word >= allnum_word)return;
        num_current_word ++;
    } else {
        if( num_current_word <= 1)return;
        num_current_word --;
    }

// w_len = w_len<<2;
// SendString((U8 *)"\r\nb:l=");
// send_hex_u8(begin_p);send_hex_u8(w_len);
    SDKLoadCode(FIND_LINE_PARAM_CODE);
    find_line_param();
    SDKLoadCode(LCD_LINE_CLEAR_CODE);
    lcd_clear_line(FIRST_LINE_ADDR);
    lcd_clear_line(SECOND_LINE_ADDR);

    SDKLoadCode(LCD_LINE_WRITE_CODE);
    lcd_write_line(u8_date[0],u8_date[1],u8_date[2]);
}

#pragma CODE = OID_NEXT_PRESS_CODE
Bool oid_next_key_press( void )
{
  //  if(g_setting_list.struct_setting.bluetooth) {
  //    SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
  //    ap4_play_stop();
 #ifdef TX_DEBUG
        SendString((U8 *)"\r\n2 n");
 #endif
        if(Addr_Fanyi) {
            if(Num_Cur_Fanyi>=Num_Fanyi) {
                if(Num_Fanyi==1) {
                    Num_Cur_Fanyi=1;
                } else {
                    Num_Cur_Fanyi = 2;
                }
            } else {
                Num_Cur_Fanyi++;
            }
            write_media_addr( (Addr_Fanyi+(Num_Cur_Fanyi-1)*9), 0 );
            state_search_to_play();
        }
        return true;
   // }
   // return false;
}

#pragma CODE = MMI_AP4_REC_TEMP_CODE05
void key_press_deal( U8 keyID )
{
    //Bool display_flag = FALSE;
    Bool temp_flag;
    //SendString((U8 *)"\r\nkpd");
    switch( keyID ) {
        case KEY_POWER:
            SDKLoadCode( MMI_AP4_KEY_PRESS_CODE00 );
            if( power_key_press()) {
                SDKLoadCode(MMI_AP4_KEY_PRESS_CODE06);
                display_key_text();
            }
            break;
///*
        case KEY_FANYI:
            //SDKLoadCode( MMI_AP4_KEY_PRESS_CODE01 );
            // menu_key_press( RECORDER_APPLI );
            SDKLoadCode(MMI_AP4_FANYI_PRESS_CODE);
            fanyi_key_press();
            break;
//*/


        /*    case KEY_GENDU:
              recorder_mode = 1;
              goto begin_reread;
              break;

              case KEY_DUIBI:
              recorder_mode = 0;
        begin_reread:
              SDKLoadCode( MMI_AP4_KEY_PRESS_CODE01 );
              menu_key_press( REREAD_APPLI );
              break;
        */
        case KEY_BAK:
            //if(!g_setting_list.struct_setting.bluetooth)break;
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_BAKKEY_PRESS_CODE );
            (void)bak_key_press();
            //SDKLoadCode( MMI_AP4_BAKKEY_PRESS_CODE02 );
            //(void)bak_key_press_two();
            break;
        case KEY_MODE:
            SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
            ap4_play_stop();
            SDKLoadCode( MMI_AP4_KEY_PRESS_CODE02 );
            if( mode_key_press()) {

            }
            //display_flag = TRUE;
            //goto key_press_display;
            break;

        case KEY_VOL_UP:
//     srvplayer_volume_change( SRVPLAYER_VOL_UP );
        //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        //if( bSpeechEnd && !g_b_recorder_on && !g_b_search_record && !bInCal
        /*if( !g_b_player_on && !g_b_recorder_on && !g_b_search_record && !bInCal
           &&( ap4_play_state == AP4_PLAY_ERROR ))
        {
           Search_Record.news_sys.flag_anjian = 1;
           Search_Record.news_sys.poweroff_flag = 5;
        }*/
//      goto key_volume_change;
        //break;

        case KEY_VOL_DOWN:
//     srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
//key_volume_change:
            SDKLoadCode( MMI_AP4_KEY_PRESS_CODE03 );
            temp_flag = volume_key_press();
            //if( volume_key_press())
            //display_flag = TRUE;
            // goto key_press_display;
            break;
//    case KEY_BAK:
        /*
        SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
        ap4_play_stop();
        if(g_b_reread_play){
            g_b_reread_play = FALSE;
            mail_send_event( EVT_BAT_WARING, 2 );
            }
        else {
                g_b_reread_play = TRUE;
                mail_send_event( EVT_BAT_WARING, 1 );
            }
        */
//      break;
        /*
              case KEY_FUDU:
            //  case KEY_PREV:
        #if (MP3_AB == ENABLE)
                SDKLoadCode( GENDU_CONTROL_CODE );
                gendu_control();
        #endif
                break;
        */
        case KEY_NEXT:
         if(g_setting_list.struct_setting.bluetooth) {
             SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
             ap4_play_stop();
             SDKLoadCode(OID_NEXT_PRESS_CODE);
             (void)oid_next_key_press();
             break;
            }
            //if(oid_next_key_press()){
            //  break;
            //}
        case KEY_PREV:
            if(b_ready_findword) {
                //SDKLoadCode(CHOICE_WORD_CTRL_CODE);
                //choice_word(keyID);
            } else {
                SDKLoadCode(DISPLAY_CONTEXT_CODE);
                fanye_display_text(keyID);
            }
            break;
    }
    return;
}


#pragma CODE = MMI_AP4_PLAYER_EOF_CODE

void mmi_ap4_player_eof_deal( void )
{
    /*
       if(count_AB == 3){
        Point_Read--;
        state_search_to_play();
        return;
        }
        //*/
    //SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    //lcd_clear_part_of_screen(44, 56, 16, 8);
    if( !g_b_search_record )return;
    SendString((U8 *)"\r\nap4_play_state=");
    send_hex_u8(ap4_play_state);
    if( ap4_play_state != AP4_PLAY_ERROR ) {
        //power_authorize_autopoweroff();
        switch( ap4_play_state ) {
            case AP4_PLAY_REREAD:
                
                mail_send_event( EVT_SEARCH_AP3RECORD, 0 );

            case AP4_PLAY_SEQUENCE:
            case AP4_PLAY_REREAD_STATUS:
                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                //mail_send_event( EVT_SEARCH_SEQUENCE, 0 );
                scan_oid_enable();
                mmi_ap3_kbd_enable();
                break;

            case AP4_PLAY_NOT_FIND_FILE:
                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE05 );
                //SDKLoadCode( NAVIGATION_CODE );
                ( void )open_file();

#if( FAT_CLUSTER_BUF == ENABLE )
                //SDKLoadCode( FS_CREATE_BUF_CODE );
                //( void )FSCreateBuf();
                //fat_clusterlist_restore();
                cluster_dowith(TRUE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

                SDKLoadCode( MMI_AP4_RELOAD_TEMP_CODE00 );
                ap4_reload_deal();
                break;

            case AP4_PLAY_SIMPLE:
                if( ap4_read_status == MMI_AP3_RERAED_STATUS ) {
                    ap4_read_status = MMI_AP3_SIMPLE_STATUS;
                    scan_oid_enable();
                    mmi_ap3_kbd_enable();
                    ap4_play_state = AP4_PLAY_ERROR;
                    g_b_search_record = FALSE;
                    break;
                }

            case AP4_PLAY_STARTUP:
            case AP4_PLAY_ONLY_SEQUENCE:
                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                mail_send_event( EVT_START_AP3_APPLI, 0 );
                break;

            case AP4_PLAY_NORMAL:
                if( News_Go.Num_ControlTab > 0) {
                    Temp_Control_num = Current_Record.Control_num;
                    Current_Record.Control_num = 0;
                    State_Search = Control_Mode_Search;
                }
            case AP4_PLAY_ONLY_SIMPLE:
                scan_oid_enable();
                mmi_ap3_kbd_enable();
                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                break;


            /*
             case AP4_PLAY_ONLY_SIMPLE:
             ap4_play_state = AP4_PLAY_ERROR;
             g_b_search_record = FALSE;
             //Search_Record.value = 0;
             Search_Record.value &= ~0xF3FFu;//0xFFCFu;
             mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
             scan_oid_enable();
             mmi_ap3_kbd_enable();
             break;
             //*/

            case AP4_PLAY_NO_AP4:
//        Ts_free( s_ts_id );

                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                search_task_stop();
#if 0

#else
                g_select_appli = IDLE_APPLI;
#endif
                Mmgr_kill_this_mmi_appli();
                break;

            default:
                ap4_play_state = AP4_PLAY_ERROR;
                g_b_search_record = FALSE;
                break;
        }
    } else {
        //if( g_b_search_record )
        {
            Ts_free( mmipaster_play_id_timerdisplay );
            power_authorize_autopoweroff();
            g_b_search_record = FALSE;
            //scan_oid_enable();
            mmi_ap3_kbd_enable();
#if( AP4_RP == ENABLE )
            rp_go_on_flag = FALSE;
#endif   //#if( AP4_RP == ENABLE )
            /*
                     SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
                     lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
                     lcd_goto_xy( 15, 24 );
                     Ih_set_curr_font( IDC_FONT_16 );
                     lcd_puts( g_string );
            */
            mmi_ap3_display_icon(FALSE);
        }
    }
}


#pragma CODE = MMI_AP4_NO_PLAYER_CODE
void mmi_ap4_no_player_deal( U16 param)
{
    SendString((U8 *)"\r\nno_player_deal=");
    send_hex_u8(param);
    if(param == AP4_PLAY_NORMAL) {
        if( News_Go.Num_ControlTab > 0) {
            Temp_Control_num = Current_Record.Control_num;
            Current_Record.Control_num = 0;
            State_Search = Control_Mode_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
        }else{
             bBackNormal = false;
             if(bRemoteNormal){
                 bRemoteNormal = false;
                 play_RMT_speech( normalAddress );
                 state_search_to_play();
                }
        }
    }
}


/*
//! Stop Reread the ap3 record
//!
void ap3srvrec_reread_off( void )
{
   U8 _MEM_TYPE_SLOW_ nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_AP3 );
   file_close();
   nav_select( nav_id_saved );
}*/


#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE04

//
Bool scank_disk( void )
{
    _MEM_TYPE_SLOW_ U16 ap3_g_u16_list_size;
    U16 AP4_DIR_NAME[] = {0x70B9,0x8BFB,0x6587,0x4EF6,0x0000};//点读文件

    SDKLoadCode( NAVIGATION_TEMP_CODE00 );
    SDKLoadCode( FILE_TEMP_CODE02 );
    if( !nav_dir_root()) {
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        return FALSE;
    }
//#if 0
//#else
    //unicode_init_buffer_enc( g_string, G_STRING_SIZE, ENCODING_UTF16BE );
    //string_ascii_to_unicode( g_string, Ext );
    //if( nav_filelist_findname( Unicode_get_addr_string( g_string ), FALSE ))
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
#if( UNICODE_TYPE == DISABLE )
    nav_string_ascii();  // ASCII mode for name of copy file
#endif
#if( UNICODE_TYPE == ENABLE )
    nav_string_unicode();
#endif
#endif
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
#if( UNICODE_TYPE == DISABLE )
    if( !nav_filelist_findname(( U8 *)AP4_DIR, FALSE ))
#endif
#if( UNICODE_TYPE == ENABLE )
        if( !nav_filelist_findname(( U8 *)AP4_DIR_NAME, FALSE ))
#endif
        {
            SendString((U8 *)"nofind");
            g_b_remote_on = FALSE;
            return FALSE;
            // goto scan_disk_enter_dir;
        }
    /*
       SDKLoadCode( NAVIGATION_TEMP_CODE02 );
       if( !nav_dir_make(( U8 *)AP4_DIR ))
    #endif
       {
          if( FS_ERR_FILE_EXIST != fs_g_status )
          {
             SDKLoadCode( NAVIGATION_TEMP_CODE04 );
             ( void )nav_file_del( TRUE );
    #if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
             nav_string_unicode();
    #endif
    #if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
             g_b_remote_on = FALSE;
    #endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
             return FALSE;
          }
       }
    scan_disk_enter_dir:
    */
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_unicode();
#endif
    //SendString((U8 *)"cd\r\n");
    SDKLoadCode( NAVIGATION_TEMP_CODE01 );
    if( !nav_dir_cd()) {
#if 0
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
        nav_string_unicode();
#endif
#endif
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        return FALSE;
    }
#if 0
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_unicode();
#endif
#endif
    diandu_entrynum = dirnum_diandu + ap4num_diandu;
    if(diandu_entrynum == 0)return FALSE;
    //SendString((U8 *)"get num\r\n");
    /*
    ap3_g_u16_list_size = Get_File_Number(( U8 *)Ext );
    if( !ap3_g_u16_list_size )
    {
    #if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
       g_b_remote_on = FALSE;
    #endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
       return FALSE;
    }
    ap3_file_number = ap3_g_u16_list_size;
    */
    /*
    ap4file_display_num = (display_page_num+1)/2;
    ap4current_display_num = 1;
    ap4current_mark = 0;
    */
    return TRUE;
}


#pragma CODE = MMI_AP4_TEMP_CODE00

void mmi_ap3_no_ap4_exit( void )
{
    SDKLoadCode( MMI_AP4_NOFILE_TEMP_CODE );
    no_ap4_file_display();
    ap4_play_state = AP4_PLAY_ERROR;
    if( !mmi_ap3_no_ap4_playintro()) {
        search_task_stop();
#if 1
//      s_ts_id = ts_alloc();
//      ts_set_time( s_ts_id, SPLASH_TIME );
        g_select_appli = IDLE_APPLI;
        Mmgr_kill_this_mmi_appli();
#else

#endif
    }
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_no_ap4_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    // Search file to play
    if( !search_subdir_mp3_file( "noap4.mp3" )) {
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return FALSE;
    }

    ap4_play_state = AP4_PLAY_NO_AP4;
    g_b_search_record = TRUE;
    //play_subdir_file = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_NOFILE_TEMP_CODE

void no_ap4_file_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_goto_xy( 20, 12 );
    lcd_puts(( U8 *)"No AP4 file" );
#elif( LANGUAGE == Simplified )
    lcd_goto_xy( 24, 12 );
    lcd_puts(( U8 *)"无书本资料" );
#elif( LANGUAGE == Traditional )
    lcd_goto_xy( 24, 12 );
    lcd_puts(( U8 *)"礚セ戈" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_goto_xy( 20, 12 );
            lcd_puts(( U8 *)"No AP4 file" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_goto_xy( 24, 12 );
            lcd_puts(( U8 *)"无书本资料" );
            break;

        case LANG_CHINESE_TRAD:
            lcd_goto_xy( 24, 12 );
            lcd_puts(( U8 *)"礚セ戈" );
            break;
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}

#if 0
//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_only_simple_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE );

    // Search file to play
    if( !search_subdir_mp3_file( "onlysimple.mp3" )
        || !play_sbdir_mp3_file()) {
        return FALSE;
    }
    return TRUE;
}
#endif


#pragma CODE = MMI_AP4_TEMP_CODE01

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_only_sequence_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    // Search file to play
    if( !search_subdir_mp3_file( "onlysequence.mp3" )) {
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return FALSE;
    }

    ap4_play_state = AP4_PLAY_ONLY_SEQUENCE;
    g_b_search_record = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_TEMP_CODE02

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_simple_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    // Search file to play
    if( !search_subdir_mp3_file( "simple.mp3" )) {
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return FALSE;
    }

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    ap4_play_state = AP4_PLAY_SIMPLE;
    g_b_search_record = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_TEMP_CODE03

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_startup_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    // Search file to play
    if( !search_subdir_mp3_file( "startup.mp3" )) {
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return FALSE;
    }

    ap4_play_state = AP4_PLAY_STARTUP;
    g_b_search_record = TRUE;
    return TRUE;
}


#if 0
//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_sequence_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE );

    // Search file to play
    if( !search_subdir_mp3_file( "sequence.mp3" )
        || !play_sbdir_mp3_file()) {
        return FALSE;
    }
    return TRUE;
}
#endif


#pragma CODE = MMI_AP4_TEMP_CODE04

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_not_find_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    ap4_play_state = AP4_PLAY_ERROR;
    // Search file to play
    /*if( !search_subdir_mp3_file( "notfindap4.mp3" ))
    {
       return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file())
    {
       return FALSE;
    }

    ap4_play_state = AP4_PLAY_NOT_FIND_FILE;
    g_b_search_record = TRUE;
    return TRUE;*/
    if( search_subdir_mp3_file( "notfindap4.mp3" )) {
        SDKLoadCode( MMI_PLAYINTRO_CODE01 );
        if( play_sbdir_mp3_file()) {
            ap4_play_state = AP4_PLAY_NOT_FIND_FILE;
            g_b_search_record = TRUE;
            return TRUE;
        }
    }

    s_ts_id_read = ts_alloc();
    ts_set_time( s_ts_id_read, SPLASH_TIME );
    return FALSE;
}


#pragma CODE = MMI_AP4_TEMP_CODE06

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_reread_status_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    ap4_play_state = AP4_PLAY_ERROR;
    // Search file to play
    if( !search_subdir_mp3_file( "rereadmode.mp3" )) {
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return FALSE;
    }

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    ap4_play_state = AP4_PLAY_REREAD_STATUS;
    g_b_search_record = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_TEMP_CODE07

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_ap3_reread_playintro( void )
{
#if (REREAD_MODE == ENABLE)
    if(!g_b_REREAD_Flag)return false;
#endif   
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    ap4_play_state = AP4_PLAY_ERROR;
    // Search file to play
    if( !search_subdir_mp3_file( "reread.mp3" )) {
        mail_send_event( EVT_SEARCH_AP3RECORD, 0 );
        return FALSE;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        mail_send_event( EVT_SEARCH_AP3RECORD, 0 );
        return FALSE;
    }

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    ap4_play_state = AP4_PLAY_REREAD;
    g_b_search_record = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_TEMP_CODE08

void reread_mode_enter_exit_deal( U8 mode )
{
    if( mode == 0 ) {
        SDKLoadCode( MMI_AP4_TEMP_CODE06 );
        ( void )mmi_ap3_reread_status_playintro();
    } else {
        SDKLoadCode( MMI_AP4_TEMP_CODE02 );
        if( !mmi_ap3_simple_playintro()) {
            ap4_read_status = MMI_AP3_SIMPLE_STATUS;
        }
    }
}

#pragma CODE = HINT_NAME_COPY_CODE
void copyHintName(U16 param)
{
    if(param == REREAD_MODE_HINT) {
        strcpy(hintName,"rereadmode1234.mp3");
        eofParam = AP4_PLAY_ONLY_SIMPLE;
    } else if(param == NORMAL_MODE_HINT) {
        strcpy(hintName,"simple12345.mp3");
        eofParam = AP4_PLAY_NORMAL;
    }
    SendString((U8 *)"\r\nhintName = ");
    SendString((U8 *)hintName);

    SendString((U8 *)"\r\neofParam = ");
    send_hex_u16(eofParam);
     scan_oid_enable();
    mmi_ap3_kbd_enable();
}

#pragma CODE = MMI_AP4_TEMP_CODE09

//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool  mmi_ap3_play_hint_mp3( U16 param )
{
    //char hintName[64];
    //U16 eofParam = 0;
    //ap4_play_state = AP4_PLAY_ERROR;
///*    
    SDKLoadCode( HINT_NAME_COPY_CODE );
    copyHintName(param);
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    ap4_play_state = AP4_PLAY_ERROR;
    // Search file to play
    if( !search_subdir_mp3_file( hintName)) {
        // return FALSE;
        goto fail;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        //return FALSE;
        goto fail;
    }

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    // ap4_play_state = AP4_PLAY_REREAD_STATUS;
    g_b_search_record = TRUE;
    ap4_play_state = eofParam;
    return true;


fail:
//*/
   
    if(param == NORMAL_MODE_HINT){
      mail_send_event( EVT_PLAYER_NO_FILE, AP4_PLAY_NORMAL );
    }
    return false;
}

#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE05

//the funcion is open the first file of changed file;
Bool open_file( void )
{
    U16 _MEM_TYPE_SLOW_ Number = 0;

    SDKLoadCode( NAVIGATION_CODE );
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( !nav_file_isdir()) {
            if( nav_file_checkext(( U8 *)AP4_EXT)) {
                if( Number == Current_Record.Ap3_num ) {
                    SDKLoadCode( FILE_TEMP_CODE00 );
                    if( file_open( FOPEN_MODE_R )) {
                        if( file_getsize() < 512*4 ) {
                            SDKLoadCode( FILE_TEMP_CODE01 );
                            file_close();
                            return FALSE;
                        }
                        return TRUE;
                    }
                    return FALSE;
                }
                Number++;
            }
        }
    }
    return FALSE;
}


#pragma CODE = MMI_AP4_CONTROL_TEMP_CODE00

void display_control_caption( void )
{
    U8 _MEM_TYPE_SLOW_ *p = News_Control.Caption_Control;
    U8 _MEM_TYPE_SLOW_ buffer[] = " ";
    U8 _MEM_TYPE_SLOW_ i = 0, j;

    return;
    while( *p &&( *p < 0x80 )) {
        p++;
        i++;
        if( i >= 14 )
            break;
    }

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 22, 0, 87, 8 );
    Ih_set_curr_font( IDC_FONT_08 );

    if( !i ) {
        lcd_goto_xy( 52, 0 );
        //Ih_set_curr_font( IDC_FONT_08 );
        lcd_puts(( U8 *)"READ" );
        switch( ap3_mode ) {
            case SINGLE_PLAY_MODE:
                lcd_draw_picture( 98-6, 0, IDP_AP4_SINGLE );
                break;

            case SEQUENCE_PLAY_MODE:
                lcd_draw_picture( 98-6, 0, IDP_AP4_SEQUENCE );
                break;
        }
        return;
    }

    j = 27 + 39 - i * 3;
    lcd_goto_xy( j, 0 );
    p = News_Control.Caption_Control;
    do {
        buffer[ 0 ] = *p;
        buffer[ 1 ] = '\0';
        lcd_puts( buffer );
        p++;
    } while( --i );
}


#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE03

Bool remote_search_file( /*U16 _MEM_TYPE_SLOW_ file_num*/void )
{
#if 0

#else
    Bool find_flag = FALSE;
    //nav_filelist_reset();
    //unicode_init_buffer_enc( g_string2, G_STRING_SIZE2, ENCODING_UTF16BE );
    //Unicode_prepare_buffer_for_mailing( g_string2, Unicode_get_char_space( g_string2 ));
    Current_Record.Ap3_num += 1;

find_next_file:
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( !nav_file_isdir()) {
            if( nav_file_checkext(( U8 *)AP4_EXT)
                && nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE )) {
                Current_Record.Ap3_num++;
                if( remote_search_file_counter == 1 ) {
                    if( Current_Record.Ap3_num > Temp_Ap3_file_num + 1 )
                        return FALSE;
                } else if( remote_search_file_counter > 1 )
                    return FALSE;
                if( RemoteNumber == get_name_lead()) {
                    if(( Temp_Ap3_file_num + 1 )== Current_Record.Ap3_num ) {
                        /*open_file();
                        State_Search = Ap3_Search;
                        return TRUE;*/
                    }
                    Current_Record.Ap3_num -= 1;
                    State_Search = Open_File_Search;
                    ap4_change_file = MMI_AP3_REMOTE_CHANGE_FILE;
                    //SendString((U8 *)"remote file\r\n");
                    //g_b_open_file_error = FALSE;
                    return TRUE;
                }
            }
        }
    }
    ( void )nav_filelist_reset();
    if( !find_flag ) {
        find_flag = TRUE;
        Current_Record.Ap3_num = 0;
        remote_search_file_counter++;
        goto find_next_file;
    }
    return FALSE;
#endif
}


//change ap3 file
Bool do_change_ap3( void )
{
    //_MEM_TYPE_SLOW_ U16 i;

    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();
    //Remote_Search_first_Ap3_file_num = Current_Record.Ap3_num;
    Temp_Ap3_file_num = Current_Record.Ap3_num;
    Temp_Search_record_value = Search_Record.value;
    remote_search_file_counter = 0;
#if 0

#endif
    //Remote_Change_Ap4_file_count = Remote_Search_first_Ap3_file_num + 1;
    if( remote_search_file( /*Remote_Change_Ap4_file_count*/ )) {
        ap3_poweron_state = MMI_AP3_CHANGE_FILE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        bRemoteEnterCal = FALSE;
        return TRUE;
    }
    Current_Record.Ap3_num = Temp_Ap3_file_num;
    //Get_File_Name(( U8 *)Ext, Current_Record.Ap3_num );

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    mail_send_event( EVT_SEARCH_NO_FIND_FILE, 0 );
    return FALSE;
}


//get ap3 file name's header
U16 get_name_lead( void )
{
    _MEM_TYPE_SLOW_ U16 *p = ( U16 _MEM_TYPE_SLOW_ *)Unicode_get_addr_string( g_string );
    _MEM_TYPE_SLOW_ U16 iNameLead;

    //bRemoteChoiceAp3 = FALSE;
    iNameLead = 0;
    if( !((*p >= '0' )&&( *p <= '9'))) {
        //bRemoteChoiceAp3 = TRUE;
        return ( RemoteNumber + 1 );
    }
    while((*p >= '0' )&&( *p <= '9')) {
        iNameLead = iNameLead * 10 + *p - '0';
        p++;
    }

    return iNameLead;
}

#if 0
#if( FS_NB_CACHE_CLUSLIST == 1 )
#define  u8_i        0
#endif
void fat_clusterlist_restore( void )
{
#if (FS_NB_CACHE_CLUSLIST>1)
    U8 u8_i;
    for( u8_i=0; u8_i<FS_NB_CACHE_CLUSLIST; u8_i++ )
#endif
        fs_g_cache_clusterlist[ u8_i ] = fs_g_cache_clusterlist_bak[ u8_i ];
}


void fat_clusterlist_save( void )
{
#if (FS_NB_CACHE_CLUSLIST>1)
    U8 u8_i;
    for( u8_i=0; u8_i<FS_NB_CACHE_CLUSLIST; u8_i++ )
#endif
        fs_g_cache_clusterlist_bak[ u8_i ] = fs_g_cache_clusterlist[ u8_i ];
}



#if( FS_NB_CACHE_CLUSLIST == 1 )
#  undef u8_i
#endif
#endif

#endif

#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE00
//
void search_startup( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    //internal_speech &= ~STARTUP_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "startup.mp3" ))
        internal_speech |= STARTUP_SPEECH;
}


#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE01
//
void search_poweroff( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    //internal_speech &= ~POWEROFF_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "poweroff.mp3" ))
        internal_speech |= POWEROFF_SPEECH;
}


#pragma CODE = MMI_AP4_SEARCH_TEMP_CODE02
//
void search_batterylow( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    //internal_speech &= ~BATTERYLOW_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "batterylow.mp3" ))
        internal_speech |= BATTERYLOW_SPEECH;
}


#if( AP4_RP == ENABLE )
// Definitions of zone module IDs
//    Module IDs of the codec zone
#define  CODEC_PLAY_FIRST  (0)
#define  CODEC_MP3         (0)
#define  CODEC_WMA         (1)
#define  CODEC_WAV         (2)
#define  CODEC_MTV         (3)
#define  CODEC_MP2         (4)
#define  CODEC_WMA_2       (5)
#define  CODEC_PLAY_MAX    (6)

#define  PASTER_INDEX_MASK          (( U16 )0xF000 )

#define  RP_DIR               "0:\\RP"

#define  RP_PATH_LENGTH       64
char  rp_path[ RP_PATH_LENGTH ];
char  rp_file[ 13 ];
U16 *rp_point;

U8    _MEM_TYPE_SLOW_   rp_play_file_type;


#pragma CODE = MMI_AP4_RP_TEMP_CODE00

Bool paster_index_file_find( U16 index, Bool create_path )
{
    U16 _MEM_TYPE_SLOW_ mask;
    U16 _MEM_TYPE_SLOW_ current_dir_mask;
    U8 file_name[ 9+6 ], i;
    FS_ERR error;
    U16 *p;

    SDKLoadCode( SEARCH_REMOTE_COV_CODE );
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    current_dir_mask = PASTER_INDEX_MASK;
    mask = PASTER_INDEX_MASK >> 4;
    while( mask ) {
        //file_name = paster_index_to_unicode( index & current_dir_mask, PASTER_INDEX_TO_DIR, 0 );
        //strcat( rp_path, "REC");
        memset( file_name, 0, 9+6 );
        strcpy(( char *)file_name, "REC" );
        convert_U16(( char *)&file_name[ 9 ], index & current_dir_mask );
        i = strlen(( char *)&file_name[ 9 ]);
        for( ; i < 5; i++ )
            strcat(( char *)file_name, "0" );
        strcat(( char *)file_name, ( char *)&file_name[ 9 ]);

        p = str_copy_u_n( rp_point, atou( file_name ), 8+1 );
        rp_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
        if( !find_path_u(( U16 *)tisiFileName, ( U8 *)tisiFileShortpath )) {
            if( create_path ) {
                error = FSDirForUnicode( rp_path, ( char *)file_name, ( U8 *)atou( file_name ));
                if( error == FS_DIRECTORY_FULL ) {
                    //g_b_disk_is_full = TRUE;
                    return FALSE;
                }
                if(( error != FS_DIRECTORY_CONFLICT )
                   &&( error != FS_NO_ERROR )) {
                    return FALSE;
                }
                //rp_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
                ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
            } else
                return FALSE;
        }
        strcpy( rp_path, tisiFileShortpath );
        current_dir_mask |= mask;
        mask >>= 4;
    }

    //file_name = paster_index_to_unicode( index, PASTER_INDEX_TO_FILE, CODEC_WAV );
    strcpy( rp_file, "Rec");
    memset( file_name, 0, 6 );
    convert_U16(( char *)file_name, index & current_dir_mask );
    i = strlen(( char *)file_name );
    for( ; i < 5; i++ )
        strcat(( char *)rp_file, "0" );
    strcat(( char *)rp_file, ( char *)file_name );
    strcat( rp_file, ".wav" );

    return TRUE;
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE01

Bool rp_record_find( U16 index )
{
    __X FS_ERR err;
    U16 *p;

    record_result = RECORD_ERROR;
    strcpy( tisiFileName, "0:\\DIYREC\\" );
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath )) {
        err = FSDirForUnicode( "0:", ( char *)"DIYREC", ( U8 *)atou(( U8 *)"DIYREC" ));
        if( err == FS_DIRECTORY_FULL ) {
            //g_b_disk_is_full = TRUE;
            return FALSE;
        }
        if(( err != FS_DIRECTORY_CONFLICT )
           &&( err != FS_NO_ERROR )) {
            return FALSE;
        }
        ( void )find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath );
    }
    strcpy( rp_path, tisiFileShortpath );

    p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\DIYREC\\"), 10+1 );
    p = str_copy_u_n( p, ( U16 *)ap4_long_name, G_STRING_SIZE/2 );
    rp_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
    if( !find_path_u(( U16 *)tisiFileName,  (U8 *)tisiFileShortpath )) {
        err = FSDirForUnicode( rp_path, ( char *)ap4_short_name, ap4_long_name );
        if( err == FS_DIRECTORY_FULL ) {
            //g_b_disk_is_full = TRUE;
            return FALSE;
        }
        if(( err != FS_DIRECTORY_CONFLICT )
           &&( err != FS_NO_ERROR )) {
            return FALSE;
        }
        //rp_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
        ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
    }
    strcpy( rp_path, tisiFileShortpath );

    //strcat( rp_path, "\\" );
    SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
    if( paster_index_file_find( index, TRUE )) {
        srvplayer_g_arg.start_time.hours   = 0;
        srvplayer_g_arg.start_time.minutes = 0;
        srvplayer_g_arg.start_time.seconds = 0;

        SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_goto_xy( 15, 24 );
        lcd_puts(( U8 *)rp_file );

        mmi_ap3_display_icon(FALSE);
        SDKLoadCode( MMI_AP4_PLAYTIME_TEMP_CODE );
        mmiap4_play_time();

        rp_record_start = TRUE;
        play_RMT_speech( Ap4_Record_Address );
        return TRUE;
    }
    return FALSE;
}

/*
Bool AP4_rp_record_start( void )
{
   if( !paster_record_start())
   {
      return FALSE;
   }
   return TRUE;
}*/


#pragma CODE = MMI_AP4_RP_TEMP_CODE02

Bool paster_record_start( void )
{
    __X FS_ERR err;
    __X U32 file_size;
    U16 mode;
    U8 i;

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    record_result = RECORD_ERROR;

#if 0
    free_space = FSFreeSpace( 0, &err );
    free_space *= 512ul;
    /*SendString((U8 *)"free_space=");
    send_hex_u32(free_space);
    SendString((U8 *)"\r\n");*/
    if( err )
        return FALSE;
#endif

    mode = FS_MODE_READ_WRITE;
    str_copy_u_n( rp_point, atou(( U8 *)rp_file ), 12+1 );
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath )) {
        mode = FS_MODE_CREATE;
        file_size = 0;

        *rp_point = 0;
        ( void )find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath );
        strcpy( rp_path, tisiFileShortpath );
    }
#if 0

#else
    file_size = MAX_LENGTH;
#endif
    if( mode == FS_MODE_READ_WRITE ) {
        ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
    } else {
        ADPCM_File = ( __X FCFILE *)FSOpenForUnicode2(( char *)rp_path, ( char *)rp_file, ( U8 *)atou(( U8 *)rp_file), FS_MODE_CREATE, &err );

        SDKLoadCode( KB_RESIZE_CODE );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );

        strcpy( tisiFileShortpath, rp_path );
        strcat( tisiFileShortpath, "\\" );
        strcat( tisiFileShortpath, rp_file );
        ADPCM_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ_WRITE, &err );
    }
    if( err ) {
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }

#if 0
    FSResize(( __X FCFILE *)ADPCM_File, file_size );
#else
    SDKLoadCode( KB_RESIZE_CODE );
    err = kb_fat_resize(( __X FCFILE *)ADPCM_File, file_size );
    if( err != FS_NO_ERROR ) {
        SDKLoadCode( KB_RESIZE_CODE );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );
        if( err == FS_WRITE_OUT_RANGE )
            record_result = RECORD_MEMORY_FULL;
        return FALSE;
    }
#endif
    WavStepSize = file_size;
    //WavWriteSize = 0;

    SDKLoadCode( RECORDER_TEMP_CODE00 );
    if( !record_on()) {
        //FSResize(( __X FCFILE *)ADPCM_File, file_size );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }
    SDKLoadCode( RECORDER_TEMP_CODE01 );
    if( !record_start()) {
        //FSResize(( __X FCFILE *)ADPCM_File, file_size );
        kb_fat_resize(( __X FCFILE *)ADPCM_File, 0 );
        FSClose(( __X FCFILE *)ADPCM_File );
        return FALSE;
    }
    //recorder_task_start();  // Enable event of recorder task
    power_forbid_autopoweroff();

    record_result = RECORD_OK;
    lcd_draw_picture( 4, 26, IDP_PLAYER_RECORD );

    rp_go_on_flag = TRUE;
    s_ts_ap3record_display_id = ts_alloc();
    ts_set_time( s_ts_ap3record_display_id, AP3RECORDER_TIME );

    green_led_stop_timer();
    red_led_stop_timer();
    red_led_on();
    green_led_on();
    return TRUE;
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE03

Bool rp_play_find( U16 index )
{
    __X FS_ERR err;
    __X U32 file_size;
    U8 i;
    U16 *p;

    p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\DIYREC\\"), 10+1 );
    p = str_copy_u_n( p, ( U16 *)ap4_long_name, G_STRING_SIZE/2 );
    rp_point = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    if( !find_path_u(( U16 *)tisiFileName,  (U8 *)tisiFileShortpath )) {
        return FALSE;
    }

    SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
    if( !paster_index_file_find( index, FALSE )) {
        return FALSE;
        //g_paster_play_file_type = CODEC_MP3;
    }

    rp_play_file_type = CODEC_WAV;
    str_copy_u_n( rp_point, atou(( U8 *)rp_file ), 12+1 );
    if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath )) {
        i = strlen(( char *)rp_file );
        rp_file[ i - 3 ] = 0;
        strcat( rp_file, "MP3" );
        str_copy_u_n( rp_point, atou(( U8 *)rp_file ), 12+1 );
        if( !find_path_u(( U16 *)tisiFileName, (U8 *)tisiFileShortpath ))
            return FALSE;
        rp_play_file_type = CODEC_MP3;
    }

    file_size = FSGetFileSize( tisiFileShortpath, &err );
    /*SendString((U8 *)"file_size=");
    send_hex_u32(file_size);
    SendString((U8 *)"\r\n");*/
    if( !file_size ) {
        return FALSE;
    }

    srvplayer_g_arg.start_time.hours   = 0;
    srvplayer_g_arg.start_time.minutes = 0;
    srvplayer_g_arg.start_time.seconds = 0;

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 15, 24 );
    lcd_puts(( U8 *)rp_file );

    mmi_ap3_display_icon(FALSE);
    SDKLoadCode( MMI_AP4_PLAYTIME_TEMP_CODE );
    mmiap4_play_time();

    rp_play_start = TRUE;
    play_RMT_speech( Ap4_Play_Address );
    return TRUE;
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE04

Bool Ap4_rp_play_start( void )
{
    //wav_encrypt = FALSE;
    g_b_play_ap3 = FALSE;

    switch( rp_play_file_type ) {
        case CODEC_MP3:
            audio_is_wav = 0;
            break;

        case CODEC_WAV:
            audio_is_wav = 1;
            break;

        default:
            return FALSE;
    }
    if( !player_play_file( tisiFileShortpath )) {
        return FALSE;
    }
    /*power_forbid_autopoweroff();

    srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
    player_task_start();       // Enable event of player task
    */
    srvplayer_g_arg.start_time.hours   = 0;
    srvplayer_g_arg.start_time.minutes = 0;
    srvplayer_g_arg.start_time.seconds = 0;
    mmipaster_play_id_timerdisplay = ts_alloc();
    ts_set_time( mmipaster_play_id_timerdisplay, PLAYER_TIME );
    mmi_ap3_display_icon(FALSE);
    //SDKLoadCode( MMI_AP4_PLAYTIME_TEMP_CODE );
    //mmiap4_play_time();

    rp_go_on_flag = TRUE;

    g_b_search_record = TRUE;
    return TRUE;
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE05

void rp_stop_deal( void )
{
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;
    rp_ok_flag = FALSE;*/
    //rp_flag_bits.value &= ~0x0F;
    rp_flag_bits.value &= ~0xF000u;
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
              lcd_goto_xy( 15, 24 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
        /*if( News_Control.Type_Control == GAME_TYPE )
        {
           print_dic_screen();
           print_lettle_screen( lettle_count );
        }*/

        play_RMT_speech( Ap4_Record_Stop_Address );
        return;
    }
    if( g_b_search_record ) {
        Ts_free( mmipaster_play_id_timerdisplay );
        //power_authorize_autopoweroff();
        //if( player_ap3_is_busy())
        //   player_ap3_stop();
        //ap3srvrec_reread_off();
        g_b_search_record = FALSE;
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
              lcd_goto_xy( 15, 24 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
        /*if( News_Control.Type_Control == GAME_TYPE )
        {
           print_dic_screen();
           print_lettle_screen( lettle_count );
        }*/

        play_RMT_speech( Ap4_Play_Stop_Address );
    }
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE06

void rp_nofile_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_goto_xy( 36, 12 );
    lcd_puts(( U8 *)"No file" );
#elif( LANGUAGE == Simplified )
    lcd_goto_xy( 40, 12 );
    lcd_puts(( U8 *)"无文件" );
#elif( LANGUAGE == Traditional )
    lcd_goto_xy( 40, 12 );
    lcd_puts(( U8 *)"礚ゅン" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    lcd_goto_xy(15, 24);
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
//     lcd_goto_xy( 36, 12 );
            lcd_puts(( U8 *)"No file" );
            break;

        case LANG_CHINESE_SIMP:
//     lcd_goto_xy( 40, 12 );
            lcd_puts(( U8 *)"无文件" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_goto_xy( 40, 12 );
                  lcd_puts(( U8 *)"礚ゅン" );
                  break;
                  */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )

    s_ts_id_read = ts_alloc();
    ts_set_time( s_ts_id_read, SPLASH_TIME );
}


#pragma CODE = MMI_AP4_RP_TEMP_CODE07

void rp_play_fail_deal( void )
{
    srvplayer_g_arg.start_time.hours   = 0;
    srvplayer_g_arg.start_time.minutes = 0;
    srvplayer_g_arg.start_time.seconds = 0;
    /*
       SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
       lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
       lcd_goto_xy( 15, 24 );
       Ih_set_curr_font( IDC_FONT_16 );
       lcd_puts( g_string );
    */
    mmi_ap3_display_icon(FALSE);
}

#endif   //#if( AP4_RP == ENABLE )


#pragma CODE = MMI_AP4_START_TEMP_CODE00

void ap4_start_appli_deal( void )
{
    SendString((U8 *)"\r\nap3");
    mmgr_set_appli_full_screen();
    SDKLoadCode( MMI_AP4_TEMP_CODE05 );
    mmiap3_display_backgroung();

    SDKLoadCode( NAVIGATION_TEMP_CODE05 );
    //SDKLoadCode( NAVIGATION_CODE );
    file_init();
    //SDKLoadCode( NAVIGATION_TEMP_CODE00 );
    //nav_dir_root();

    s_ts_ap3record_display_id  = UNDEFINED_TIMER;
    s_ts_id_read               = UNDEFINED_TIMER;
    s_ts_id                    = UNDEFINED_TIMER;
    mmipaster_play_id_timerdisplay = UNDEFINED_TIMER;
    ap4_bit_news.value = 0;
#if( OID == ENABLE )
    // b_oid_init_flag = FALSE;
    if( !b_oid_init_flag ) {
        SDKLoadCode( OID_TEMP_CODE00 );
        reinit_oid_pen();
//    SendString((U8 *)"\r\n OID init ok!");
    }
#endif   //#if( OID == ENABLE )

    Kbd_set_evt_enabled();
    if( Kbd_is_event_triggered())
        Kbd_set_evt_disabled();

    internal_speech = 0;
    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE00 );
    search_startup();
    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE01 );
    search_poweroff();
    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE02 );
    search_batterylow();
    order_play_count = 0;

#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;
    rp_ok_flag = FALSE;*/
    rp_flag_bits.value = 0;
#endif   //#if( AP4_RP == ENABLE )

    //srvplayer_volume_send();
    //mmi_file_flag &= ~AP4_NO_FILE_FLAG;
#if( PASTER_MODE == DISABLE )
    //mmi_file_flag |= APF_NO_FILE_FLAG;
#endif   //#if( PASTER_MODE == DISABLE )
#if( DIC == DISABLE )
    //mmi_file_flag |= ADB_NO_FILE_FLAG;
#endif   //#if( DIC == DISABLE )
//***********************************************
    g_b_ap4_caption = FALSE;

    reread_enable_flag = FALSE;
    ap4_read_status = MMI_AP3_SIMPLE_STATUS;
    g_b_search_record = FALSE;

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    ap4_play_state = AP4_PLAY_ERROR;
    if(( ap3_poweron_state == MMI_CHANGE_TO_AP3 )
       ||( ap3_poweron_state == MMI_AP3_ENTER_USB )) {

        if( bRemoteEnterCal ) {
            mail_send_event( EVT_START_AP3_APPLI, 0 );
            return;
        }
        All_Control.Mask_Gloablext = ap4_AllControl_MaskGlobal;
        if(( All_Control.Mask_Gloablext &( SIMPLE_FLAG | SEQUENCE_FLAG ))== SEQUENCE_FLAG ) {
            SDKLoadCode( MMI_AP4_TEMP_CODE01 );
            if( mmi_ap3_only_sequence_playintro()) {
                return;
                //mail_send_event( EVT_START_AP3_APPLI, 0 );
            }
            /*else
            {
               ap4_play_state = AP4_PLAY_ONLY_SEQUENCE;
               g_b_search_record = TRUE;
            }*/
        } else {
            ap3_mode = SINGLE_PLAY_MODE;
            goto mmi_ap3_simple_play;
            /*SDKLoadCode( MMI_AP4_TEMP_CODE02 );
            if( mmi_ap3_simple_playintro())
            {
               break;
               //mail_send_event( EVT_START_AP3_APPLI, 0 );
            }
            else
            {
               ap4_play_state = AP4_PLAY_SIMPLE;
               g_b_search_record = TRUE;
            }*/
        }
    } else if( power_is_vbus_present()
               && !play_startup_flag ) {
        play_startup_flag = TRUE;
        SDKLoadCode( MMI_AP4_TEMP_CODE03 );
        if( mmi_ap3_startup_playintro()) {
            return;
            //mail_send_event( EVT_START_AP3_APPLI, 0 );
        }
        /*else
        {
           ap4_play_state = AP4_PLAY_STARTUP;
           g_b_search_record = TRUE;
        }*/
    }

    else if(( ap3_poweron_state == MMI_AP3_USB_WRITE )
#if( DEFAULT_APPLI != 3 )
            ||( ap3_poweron_state == MMI_AP3_POWERON )
            ||( ap3_poweron_state == MMI_AP3_NORMAL )
#endif   //#if( DEFAULT_APPLI != AP3_APPLI )
           ) {
    mmi_ap3_simple_play:
        SendString((U8 *)"\r\n17");
#if 0
        SDKLoadCode( MMI_AP4_TEMP_CODE02 );
        if( mmi_ap3_simple_playintro()) {
            return;
            //SendString((U8 *)"intro\r\n");
            //mail_send_event( EVT_START_AP3_APPLI, 0 );
        }
#endif
        /*else
        {
           SendString((U8 *)"play intro\r\n");
           ap4_play_state = AP4_PLAY_SIMPLE;
           g_b_search_record = TRUE;
        }*/
    }
    //else

    mail_send_event( EVT_START_AP3_APPLI, 0 );
}


#pragma CODE = MMI_AP4_START_TEMP_CODE01

void ap4_start_init_deal( void )
{
    SendString((U8 *)"\r\nstar ap3");
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
    bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )
    power_authorize_autopoweroff();
    ap4_play_state = AP4_PLAY_ERROR;
    SDKLoadCode( SRV_PLAYER_TEMP_CODE02 );
    unicode_init_buffer_enc(( U8 _MEM_TYPE_SLOW_ *)g_string, G_STRING_SIZE, ENCODING_UTF16BE );
    Unicode_prepare_buffer_for_mailing( g_string, Unicode_get_char_space( g_string ));


    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE04 );
    SDKLoadCode( NAVIGATION_CODE );
    if( !scank_disk()) {
        //SendString((U8 *)"error\r\n");
        //mmi_ap3_no_ap4_exit();
        mail_send_event( EVT_SEARCH_AP3EXIT, 0 );
        return;
    }

    //SendString("disk ok\r\n");
    search_task_start();
    // TO DO
    ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
//  State_Search = Open_File_Search;
    State_Search = Browser_Dir_Init;
    g_b_open_file_error = FALSE;
    //bSpeechEnd = TRUE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    /*if( g_b_remote_on
       ||( ap3_poweron_state == MMI_CHANGE_TO_AP3 )
          ||( ap3_poweron_state == MMI_AP3_ENTER_USB ))*/
    if(( ap3_poweron_state == MMI_CHANGE_TO_AP3 )
       ||( ap3_poweron_state == MMI_AP3_ENTER_USB )) {
        Search_Record.value &= ~0xF3FFu;//0xFFCFu;
    } else
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    {
        Ap3_MenuPath = AP3_MENU_PATH;
        ap3_mode = SINGLE_PLAY_MODE;
        switch( ap3_poweron_state ) {
            case MMI_CHANGE_TO_AP3:
            //case MMI_AP3_SD_REMOVE:
            case MMI_AP3_ENTER_USB:
                break;

            default:
                Current_Record.Ap3_num = 0;
                break;
        }

    }
    Search_Record.value = 0;
    b_clusterlist = FALSE;
    init_array_v();
    repeat_point = 0;
    g_b_recorder_on = FALSE;
    g_b_search_record = FALSE;
    mmi_ap3_kbd_enable();
    scan_oid_enable();

    remote_bits.value = 0x00;
    if(g_b_xuanshu_flag) {
        bInEbookMode = FALSE;
        ap3_poweron_state = MMI_AP3_NORMAL;
        State_Search = Open_File_Search;

        Current_Record.Ap3_num = g_xuanshu_num -1;
        Current_Record.Book_num = 0;
        Current_Record.Page_num = 0;
        Current_Record.Control_num = 0;
        g_b_xuanshu_flag = FALSE;
#if 1
        SendString((U8 *)"\r\nxuanshu_flag");
#endif
    }
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE00

void ap4_record_prepare_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
    Ih_set_curr_font( IDC_FONT_16 );
    if( g_b_battery_low ) {
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
        lcd_goto_xy(( U8 *)20, 12 );
        lcd_puts( "Low voltage" );
#elif( LANGUAGE == Simplified )
        lcd_goto_xy( 28, 12 );
        lcd_puts(( U8 *)"电量不足!" );
#elif( LANGUAGE == Traditional )
        lcd_goto_xy( 28, 12 );
        lcd_puts(( U8 *)"筿秖ぃì!" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
        lcd_goto_xy( 15, 24 );
        switch( g_setting_list.struct_setting.language_ihm ) {
            case LANG_ENGLISH:
//        lcd_goto_xy( 20, 12 );
                lcd_puts(( U8 *)"Low voltage" );
                break;

            case LANG_CHINESE_SIMP:
//         lcd_goto_xy( 28, 12 );
                lcd_puts(( U8 *)"电量不足!" );
                break;
                /*
                         case LANG_CHINESE_TRAD:
                         lcd_goto_xy( 28, 12 );
                         lcd_puts(( U8 *)"筿秖ぃì!" );
                         break;
                */
        }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
        return;
    }

#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_goto_xy( 28, 12 );
    lcd_puts(( U8 *)"Preparing" );
#elif( LANGUAGE == Simplified )
    lcd_goto_xy( 32, 12 );
    lcd_puts(( U8 *)"准备录音" );
#elif( LANGUAGE == Traditional )
    lcd_goto_xy( 32, 12 );
    lcd_puts(( U8 *)"称魁" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    lcd_goto_xy( 15, 24 );
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
//      lcd_goto_xy( 28, 12 );
            lcd_puts(( U8 *)"Preparing" );
            break;

        case LANG_CHINESE_SIMP:
//      lcd_goto_xy( 32, 12 );
            lcd_puts(( U8 *)"准备录音" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_goto_xy( 32, 12 );
                  lcd_puts(( U8 *)"称魁" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE01

void ap4_record_fail_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );

    Ih_set_curr_font( IDC_FONT_16 );

    switch( record_result ) {
        case RECORD_ERROR:
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 16, 12 );
            lcd_puts(( U8 *)"Record error" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts(( U8 *)"录音错误" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts(( U8 *)"魁岿粇" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            lcd_goto_xy( 15, 24 );
            switch( g_setting_list.struct_setting.language_ihm ) {
                case LANG_ENGLISH:
//         lcd_goto_xy( 16, 12 );
                    lcd_puts(( U8 *)"Record error" );
                    break;

                case LANG_CHINESE_SIMP:
//         lcd_goto_xy( 32, 12 );
                    lcd_puts(( U8 *)"录音错误" );
                    break;
                    /*
                             case LANG_CHINESE_TRAD:
                             lcd_goto_xy( 32, 12 );
                             lcd_puts(( U8 *)"魁岿粇" );
                             break;
                    */
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            break;

        case RECORD_FILE_FULL:
        case RECORD_FILE_ERROR:
        case RECORD_MEMORY_FULL:
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
            lcd_goto_xy( 20, 12 );
            lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
            lcd_goto_xy( 32, 12 );
            lcd_puts(( U8 *)"存储空间不足" );
#elif( LANGUAGE == Traditional )
            lcd_goto_xy( 32, 12 );
            lcd_puts(( U8 *)"丁ぃì" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
            lcd_goto_xy( 15, 24 );
            switch( g_setting_list.struct_setting.language_ihm ) {
                case LANG_ENGLISH:
//         lcd_goto_xy( 20, 12 );
                    lcd_puts(( U8 *)"Memory full" );
                    break;

                case LANG_CHINESE_SIMP:
//         lcd_goto_xy( 32, 12 );
                    lcd_puts(( U8 *)"空间不足" );
                    break;
                    /*
                             case LANG_CHINESE_TRAD:
                             lcd_goto_xy( 32, 12 );
                             lcd_puts(( U8 *)"丁ぃì" );
                             break;
                    */
            }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
            break;
    }

    s_ts_id_read = ts_alloc();
    ts_set_time( s_ts_id_read, SPLASH_TIME );

    scan_oid_enable();
    mmi_ap3_kbd_enable();
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE02

void ap4_record_ok_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    lcd_goto_xy( 15, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_puts(( U8 *)"Record" );
#elif( LANGUAGE == Simplified )
    lcd_puts(( U8 *)"跟读录音" );
#elif( LANGUAGE == Traditional )
    lcd_puts(( U8 *)"蛤弄魁" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Record" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"跟读录音" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_puts(( U8 *)"蛤弄魁" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE03

void ap4_record_playback_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    // lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );

    lcd_goto_xy( 15, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_puts(( U8 *)"Playback" );
#elif( LANGUAGE == Simplified )
    lcd_puts(( U8 *)"录音回放" );
#elif( LANGUAGE == Traditional )
    lcd_puts(( U8 *)"魁" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Playback" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"录音回放" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_puts(( U8 *)"魁" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE04

void ap4_record_playback_fail_deal( void )
{
    g_b_search_record = FALSE;
    /*
       SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
       lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
       lcd_goto_xy( 15, 24 );
       Ih_set_curr_font( IDC_FONT_16 );
       lcd_puts( g_string );
    */
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE06

Bool ap4_record_time_deal( void )
{
    _MEM_TYPE_SLOW_ U8 string[ 9+4 ], i;
    U32 time;

    ts_set_time( s_ts_ap3record_display_id, AP3RECORDER_TIME );
    SDKLoadCode( RECORDER_TEMP_CODE04 );
    time = record_gettime();
    //sprintf( string , "%02bu:%02bu:%02bu", MSB1( time ), MSB2( time ), MSB3( time ));
    memset( string, 0, 9+4 );
    SDKLoadCode( MMI_AP4_COV8_CODE );
    convert_U8(( char *)&string[ 9 ], MSB1( time ));
    i = strlen(( char *)&string[ 9 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 9 ] );
    strcat(( char *)string, ":" );

    memset( &string[ 9 ], 0, 4 );
    convert_U8(( char *)&string[ 9 ], MSB2( time ));
    i = strlen(( char *)&string[ 9 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 9 ] );
    strcat(( char *)string, ":" );

    memset( &string[ 9 ], 0, 4 );
    convert_U8(( char *)&string[ 9 ], MSB3( time ));
    i = strlen(( char *)&string[ 9 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 9 ] );

#if 0//( AP4_RP == ENABLE )
    if( rp_go_on_flag )
        lcd_goto_xy( 32, 24 );
    else
#endif   //#if( AP4_RP == ENABLE )
        lcd_goto_xy( 4, 56 );

    Ih_set_curr_font( IDC_FONT_08 );
    lcd_puts( string );

    if( time >= 1 ) {
        mmi_ap3_kbd_enable();
        scan_oid_enable();
    }
#if( AP4_RP == ENABLE )
    if( rp_go_on_flag )
        return FALSE;
#endif   //#if( AP4_RP == ENABLE )
    if( time > reread_time ) {
        srvrec_stop();
        mmi_ap3_kbd_enable();
        scan_oid_enable();
        Ts_free( s_ts_ap3record_display_id );
        gbRereadingFlag = false;
//***********************************************
        if( ap3_mode == SINGLE_PLAY_MODE ) {
            //SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE05 );
            //ap4_original_display();
            Point_Read = 0;
            Search_Record.news_sys.play_record_flag = 1;
            //state_search_to_play();
            return TRUE;
        } else if( ap3_mode == SEQUENCE_PLAY_MODE ) {
            mail_send_event( EVT_SEARCH_AP3RECORD_PLAY, 0 );
        }
//***********************************************
    }
    return FALSE;
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE05

void ap4_original_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    // lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
    lcd_goto_xy( 15, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_puts(( U8 *)"Original" );
#elif( LANGUAGE == Simplified )
    lcd_puts(( U8 *)"原音播放" );
#elif( LANGUAGE == Traditional )
    lcd_puts(( U8 *)"冀" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Original" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"原音播放" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_puts(( U8 *)"冀" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE07

void ap4_record_diskfull_deal( void )
{
    record_result = RECORD_MEMORY_FULL;
    Ts_free( s_ts_ap3record_display_id );
    //status_authorize_display();

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
    lcd_goto_xy( 15, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_puts(( U8 *)"Memory full" );
#elif( LANGUAGE == Simplified )
    lcd_puts(( U8 *)"空间不足" );
#elif( LANGUAGE == Traditional )
    lcd_puts(( U8 *)"丁ぃì" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Memory full" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"空间不足" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_puts(( U8 *)"丁ぃì" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
    s_ts_id_read = ts_alloc();
    ts_set_time( s_ts_id_read, SPLASH_TIME );
    scan_oid_enable();
    mmi_ap3_kbd_enable();
}


#pragma CODE = MMI_AP4_RECORD_TEMP_CODE08

void ap4_search_nofind_file_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_goto_xy( 8, 12 );
    lcd_puts(( U8 *)"File not found" );
#elif( LANGUAGE == Simplified )
    lcd_goto_xy( 24, 12 );
//   lcd_puts(( U8 *)"无点读资料" );
    lcd_puts(( U8 *)"无此书点读数据文件" );
#elif( LANGUAGE == Traditional )
    lcd_goto_xy( 24, 12 );
    lcd_puts(( U8 *)"礚翴弄戈" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    lcd_goto_xy( 15, 24 );
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
//     lcd_goto_xy( 8, 12 );
            lcd_puts(( U8 *)"File not found" );
            break;

        case LANG_CHINESE_SIMP:
//      lcd_goto_xy( 24, 12 );
            lcd_puts(( U8 *)"无此书点读数据文件" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
            //      lcd_goto_xy( 24, 12 );
                  lcd_puts(( U8 *)"礚翴弄戈" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = MMI_AP4_RELOAD_TEMP_CODE00

void ap4_reload_deal( void )
{
    Ts_free( s_ts_id_read );
    if( ap4_change_file == MMI_AP3_REMOTE_CHANGE_FILE ) {
        ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
        SDKLoadCode(JIAMI_CODE);
        if(judge_password()) {
            //SDKLoadCode(FIND_NEWS_PASS_CODE);
            //read_new_ap4_password();
            SDKLoadCode(NEW_READ_ALLNEWS_CODE);
            new_read_all_control_news();
        } else {
            SDKLoadCode( READ_ALLNEWS_CODE );
            read_all_control_news();
        }
#if( AP4_ENCRYPT == ENABLE )
        SDKLoadCode( SEARCH_BOOK_ENCRYPT_CODE );
        ( void )ap4_book_encrypt();
#endif   //#if( AP4_ENCRYPT == ENABLE )
        Search_Record.value = Temp_Search_record_value;
        //State_Search = Open_File_Search;
        state_search_to_play();
        /*
              switch( ap3_mode )
              {
                 case SINGLE_PLAY_MODE:
                 lcd_draw_picture( 98-6, 0, IDP_AP4_SINGLE );
                 break;

                 case SEQUENCE_PLAY_MODE:
                 lcd_draw_picture( 98-6, 0, IDP_AP4_SEQUENCE );
                 break;
              }
        */
        if( Ap3_MenuPath >= CONTROL_MENU_PATH ) {
            SDKLoadCode( MMI_AP4_CONTROL_TEMP_CODE00 );
            display_control_caption();
        }
    }
    scan_oid_enable();
    mmi_ap3_kbd_enable();
    /*
       SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
       lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
       lcd_goto_xy( 15, 24 );
       Ih_set_curr_font( IDC_FONT_16 );
       lcd_puts( g_string );
    */
    mmi_ap3_display_icon(FALSE);
    /*if( News_Control.Type_Control == GAME_TYPE )
    {
       print_dic_screen();
       print_lettle_screen( lettle_count );
    }*/
}


#pragma CODE = MMI_AP4_USB_TEMP_CODE00

void mmi_ap4_usb_deal( void )
{
    b_reload_ap4 = FALSE;
    if(( State_Search != Play_Search )
       &&( State_Search != Idle_Search )
       &&( State_Search != Sequence_Search )
       &&( State_Search != Null_state ))
        b_reload_ap4 = TRUE;

#if( FAT_CLUSTER_BUF == ENABLE )
    //fat_clusterlist_save();
    cluster_dowith(FALSE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

//   Ts_free( s_ts_id );
    Ts_free( s_ts_id_read );
    bInCal = FALSE;
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    scan_oid_disable();
    mmi_ap3_kbd_disable();
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }
    init_array_v();
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();
    search_task_stop();
    ap3_poweron_state = MMI_AP3_ENTER_USB;
    //g_select_appli = MSTORAGE_APPLI;
    //Mmgr_kill_this_mmi_appli();
    //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
    SDKLoadCode( AUADC_COMMON_CODE );
    AUADC_Power_Down();
    CommonLoadCode( C_USB );
}


#pragma CODE = AP4_SEARCH_END_TEMP_CODE00

void ap4_search_end_deal( U8 param )
{
#if( FAT_CLUSTER_BUF == ENABLE )
    //fat_clusterlist_save();
    cluster_dowith(FALSE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

    //SendString("SEARCH_END\r\n");
    bInCal = FALSE;
    ap3_poweron_state = MMI_CHANGE_TO_AP3;
    Ts_free( s_ts_id_read );
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //if( g_b_remote_on )
    //Save_State_Search = State_Search;
    //else
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //scan_oid_disable();
    mmi_ap3_kbd_disable();
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //if( g_b_remote_on )
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();

    search_task_stop();
    g_select_appli = param;
    Mmgr_kill_this_mmi_appli();
}

#pragma CODE = AP4_SEARCH_TOBROWSER_CODE

void ap4_search_end_to_browser( U8 param )
{

    bRmoteExitAp4 = FALSE;

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    //scan_oid_disable();
    //mmi_ap3_kbd_disable();
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    reread_enable_flag = FALSE;
#if (REREAD_MODE == ENABLE)
    g_b_REREAD_Flag = false;
    Search_Record.news_sys.reread_flag = false;   
#endif
    init_array_v();
#if( FAT_CLUSTER_BUF == ENABLE )
    // fat_clusterlist_save();
    cluster_dowith(FALSE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

    //SendString("SEARCH_END\r\n");
    bInCal = FALSE;
    ap3_poweron_state = MMI_CHANGE_TO_AP3;
    Ts_free( s_ts_id_read );
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //if( g_b_remote_on )
    //Save_State_Search = State_Search;
    //else
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //scan_oid_disable();
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //if( g_b_remote_on )
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();

    State_Search = param;
}

#pragma CODE = MMI_AP4_POWEROFF_TEMP_CODE00

void mmi_ap4_poweroff_deal( void )
{
    bInCal = FALSE;
//  Ts_free( s_ts_id );
    if(UNDEFINED_TIMER != s_ts_id_read)Ts_free( s_ts_id_read );
    SDKLoadCode( FILE_TEMP_CODE01 );
    file_close();
    search_task_stop();
    Mmgr_kill_this_mmi_appli();
    //power_authorize_autopoweroff();
    // Send command to power OFF ap3
    //mail_send_command( CMD_POWER_OFF, 0 );
    g_select_appli = POWER_OFF_APPLI;
//   CommonLoadCode( C_POWEROFF );
}


#pragma CODE = MMI_AP4_PLAY_STOP_CODE

void ap4_play_stop( void )
{
    if( player_ap3_is_busy()) {
        count_AB = 0;
        player_ap3_stop();
    }
}


#pragma CODE = MMI_AP4_BAT_LOW_CODE

void bat_low_waring_deal( U16 param )
{
#if 1
    U8 temp_buf[32];

    if(g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }

    SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
    ap4_play_stop();

    SDKLoadCode( MMI_PLAYINTRO_CODE00 );

    // Search file to play

    if( 0 == param ) {
        SendString((U8 *)"\r\nW!");
        strcpy((char *)temp_buf,(char *)LOW_BAT_WARNING);
    } else if( 1 == param) {
        SendString((U8 *)"\r\nr!");
        strcpy((char *)temp_buf,(char *)REPEAT_MP3);
    }

    else if( 2 == param) {
        SendString((U8 *)"\r\nd!");
        strcpy((char *)temp_buf,(char *)DIANDU_MP3);
    } else
        return;

    if( !search_subdir_mp3_file( (char *)temp_buf )) {
        return;
    }
    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file()) {
        return ;
    }
    ap4_play_state = AP4_PLAY_REREAD_STATUS;
    g_b_search_record = TRUE;
    mmi_ap3_kbd_disable();
    scan_oid_disable();
#else
    if( !g_b_recorder_on )
        return;
    srvrec_stop();

    Ts_free( s_ts_ap3record_display_id );
    mmi_ap3_kbd_enable();
    scan_oid_enable();
#endif
}

#pragma CODE = MMI_AP4_GLOBEKEY_CODE

Bool mmi_ap4_globekey_deal( U16 param )
{
#if 0
    switch( param ) {
        case ((KEY_POWER<<8)|KBD_KEY_LONGPRESSED):
            g_select_appli = POWER_OFF_APPLI;
            goto new_panduan;
            break;

        case ((KEY_MODE<<8)|KBD_KEY_LONGPRESSED):
            g_select_appli = IDLE_APPLI;
            goto new_panduan;
            break;

        case ((KEY_FANYI<<8)|KBD_KEY_LONGPRESSED):
            g_select_appli = RECORDER_APPLI;
            goto new_panduan;
            break;

        case ((KEY_VOL_UP<<8)|KBD_KEY_RELEASED):
            srvplayer_volume_change( SRVPLAYER_VOL_UP);
            goto vol_change;
            break;

        case ((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED):
            srvplayer_volume_change( SRVPLAYER_VOL_DOWN);
        vol_change:
            mmi_ap3_display_volume();
            if(g_b_player_on)return TRUE;
            return FALSE;
            break;

        case ((KEY_GENDU<<8)|KBD_KEY_LONGPRESSED):
        case ((KEY_GENDU<<8)|KBD_KEY_RELEASED):
            recorder_mode = 1;
            g_select_appli = REREAD_APPLI;
            goto new_panduan;
            break;

        case ((KEY_DUIBI<<8)|KBD_KEY_LONGPRESSED):
        case ((KEY_DUIBI<<8)|KBD_KEY_RELEASED):
            recorder_mode = 0;
            g_select_appli = REREAD_APPLI;
            goto new_panduan;
            break;

        default:
            break;
    }
    return FALSE;
#else
    if(param == ((KEY_POWER<<8)|KBD_KEY_LONGPRESSED)) {
        g_power_off_mode = POWER_OFF;
        g_select_appli = POWER_OFF_APPLI;
        goto new_panduan;
    } else if(param == ((KEY_MODE<<8)|KBD_KEY_LONGPRESSED)) {
        g_select_appli = IDLE_APPLI;
        goto new_panduan;
    } else if(param == ((KEY_BAK<<8)|KBD_KEY_LONGPRESSED)) {
        recorder_mode = 1;
        g_select_appli = RECORDER_APPLI;
        goto new_panduan;
    } else if(param == ((KEY_REC<<8)|KBD_KEY_PRESSED)) {
        g_select_appli = PLAYER_APPLI;
        goto new_panduan;
    }
    /*
      else if(KEY_ID(param)== KEY_GENDU){
        recorder_mode = 1;
        g_select_appli = REREAD_APPLI;
        goto new_panduan;
        }
      else if(KEY_ID(param)== KEY_DUIBI){
        recorder_mode = 0;
        g_select_appli = REREAD_APPLI;
        goto new_panduan;
        }*/
    else if(param == ((KEY_VOL_UP<<8)|KBD_KEY_RELEASED)) {
        srvplayer_volume_change( SRVPLAYER_VOL_UP);
        goto vol_change;
    } else if(param == ((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED)) {
        srvplayer_volume_change( SRVPLAYER_VOL_DOWN);
        goto vol_change;
    } else
        return FALSE;


vol_change:
    mmi_ap3_display_volume();
    if(g_b_player_on)return TRUE;
    return FALSE;
#endif

new_panduan:
    SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
    ap4_play_stop();
    SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
    mode_key_long_press(g_select_appli);
    /*
       if((State_Search == Browser_File)||(State_Search == Browser_DirFile)){
        Mmgr_kill_this_mmi_appli();
        }
       else{
        SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
        mode_key_long_press(g_select_appli);
        }
    */
    return TRUE;
}

#pragma CODE = MMI_AP4_KEY_CODE

Bool mmi_ap4_key_deal( U16 param )
{
    SendString((U8 *)"\r\nap4_key = ");
    send_hex_u16(param);
    SendString((U8 *)"\r\noidTYpe = ");
    send_hex_u8(g_setting_list.struct_setting.bluetooth);
#if (BACKLIGHT == ENABLE)
    //if( Lcd_backlight_is_enable() )
    {
        //SDKLoadCode( LCD_SSD1303_TEMP_CODE03 );
        Lcd_rearm_backlight_on();
    }
#endif
#if (POWER_OFF_AUTO == ENABLE)
    power_rearm_autopoweroff_delay();
#endif
    GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
    GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));


#if( LCD_PRESENT == ENABLE )
#  if(BACKLIGHT == ENABLE)
    if( g_mmi_lcd_backlight_off ) {
        if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
            g_mmi_lcd_backlight_off = FALSE;
        return TRUE;
    }
#  endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
    if(gbRereadingFlag)return true;
    SDKLoadCode( MMI_AP4_GLOBEKEY_CODE );
    if(mmi_ap4_globekey_deal(param))return  TRUE;

    if((State_Search == Browser_File)||(State_Search == Browser_DirFile)) {
        SDKLoadCode( BROWSER_KEY_RELEASE_CODE );
        if(browser_key_release(param))return  TRUE;
    }
    if( ap4_play_state == AP4_PLAY_ERROR ) {

        if( mmi_ap3_kbd_is_disable())
            return TRUE;
        if(( State_Search != Play_Search )
           &&( State_Search != Idle_Search )
           &&( State_Search != Sequence_Search )
           &&( State_Search != Null_state ))
            return TRUE;
    } else {
        if(( KBD_KEY_PRESSED | KBD_KEY_LONGPRESSED | KBD_KEY_REPEAT_RELEASED | KBD_KEY_REPEAT )
           & KEY_STATUS( param )) {
            return TRUE;
        }
        if( KBD_KEY_RELEASED & KEY_STATUS( param )) {
            if(( KEY_ID( param )!= KEY_VOL_UP )
               &&( KEY_ID( param )!= KEY_VOL_DOWN ))
                return TRUE;
        }
    }

#if( PASTER_MODE == DISABLE )
    if( KBD_KEY_LONGPRESSED == KEY_STATUS( param )) {
        if( KEY_ID( param )== KEY_GENDU)
            return TRUE;
    }
#endif   //#if( PASTER_MODE == DISABLE )
    return FALSE;
}


#pragma CODE = MMI_AP4_KEY_PRESS_CODE00

Bool power_key_press( void )
{
    if( bInCal ) {
        //bRmoteExitAp4 = FALSE;
        return FALSE;
    }

#if( AP4_RP == ENABLE )
    //if( rp_go_on_flag || rp_record_start || rp_play_start || rp_ok_flag )
    if( rp_flag_bits.value & 0xF000u )
        return FALSE;
#endif   //#if( AP4_RP == ENABLE )

    //if( Search_Record.news_sys.repeat_flag || Search_Record.news_sys.play_record_flag )
    if( Search_Record.value & 0x0101u/*0x8080*/ ) {
        /*Search_Record.news_sys.repeat_flag
           = Search_Record.news_sys.play_record_flag
              = 0;*/
        Search_Record.value &= ~0x0101u;//~0x8080;
        if( player_ap3_is_busy())
            player_ap3_stop();
        if( ap3_mode == SINGLE_PLAY_MODE )
            init_array_v();
        else if( ap3_mode == SEQUENCE_PLAY_MODE ) {
            State_Search = Null_state;
        }
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/

        bRmoteExitAp4 = FALSE;
        return FALSE;
    }

    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
        mmi_ap3_kbd_enable();
        scan_oid_enable();
        bRmoteExitAp4 = FALSE;
        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();*/
        }
        return FALSE;
    }
    if( g_b_search_record ) {
        if( ap4_play_state == AP4_PLAY_ERROR ) {
            if( player_ap3_is_busy())
                player_ap3_stop();

            g_b_search_record = FALSE;
            Ts_free( mmipaster_play_id_timerdisplay );

            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();*/

            bRmoteExitAp4 = FALSE;
            return FALSE;
        }
    }

    if( b_remote_play_repeat ) {
        Search_Record.news_sys.index_normal_flag = 0;
        b_play_repeat = FALSE;
        b_remote_play_repeat = FALSE;
        if( player_ap3_is_busy())
            player_ap3_stop();
        repeat_save();
        init_array_v();
        return FALSE;
    }

    /*
       //if( bSpeechEnd )
       if( !g_b_player_on )
       {
          if( g_b_ap4_caption )
          {
             g_b_ap4_caption = FALSE;
              return FALSE;
          }
          return FALSE;
       }
    */
    if(b_check_word&&b_ready_findword) {
        if( !b_fanyi ) {
            if( player_ap3_is_busy()) {
                player_ap3_stop();
                init_array_v();
            }

            return TRUE;
        }
        return FALSE;
    }
    switch( srvplayer_g_player_state ) {
        case MMI_PLAYER_STATE_PLAY :
            if( Search_Record.news_sys.reread_flag )
                reread_end_time += timer_get_time()- reread_start_time;
            SDKLoadCode( SRV_PLAYER_TEMP_CODE05 );
            ( void )srvplayer_pause();
            mmi_ap3_display_icon(FALSE);
            power_authorize_autopoweroff();
            break;

        case MMI_PLAYER_STATE_PAUSE :
            if( Search_Record.news_sys.reread_flag )
                reread_start_time = timer_get_time();
            SDKLoadCode( SRV_PLAYER_TEMP_CODE06 );
            ( void )srvplayer_restart_play();
            mmi_ap3_display_icon(FALSE);
            power_forbid_autopoweroff();
            break;
    }
    return FALSE;
}


#pragma CODE = MMI_AP4_KEY_PRESS_CODE01

void menu_key_press( U8 appli_id )
{
//   SendString((U8 *)"\r\nmenu_key_press!");
//  Ts_free( s_ts_id );
    Ts_free( s_ts_id_read );
    bRmoteExitAp4 = FALSE;
    /*if( g_b_recorder_on )
    {
       srvrec_stop();
       Ts_free( s_ts_ap3record_display_id );
       //break;
    }*/
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    scan_oid_disable();
    mmi_ap3_kbd_disable();
    if( player_ap3_is_busy())
        player_ap3_stop();
    /*if( g_b_search_record )
    {
       g_b_search_record = FALSE;
       ap4_play_state = AP4_PLAY_ERROR;
    }*/
    init_array_v();
    //file_close();
    //search_task_stop();
    Search_Record.value &= ~0xF3FFu;//0xFFCF;
    //ap3_poweron_state = MMI_CHANGE_TO_AP3;
    //g_select_appli = g_old_select_appli = RECORDER_APPLI;
    //Mmgr_kill_this_mmi_appli();
    //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
    //bInCal = FALSE;
    mail_send_event( EVT_SEARCH_END, appli_id );
//  recorder_mode = 1;

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    reread_enable_flag = FALSE;

#if( AP4_RP == ENABLE )
    rp_flag_bits.value &= ~0xF000u;
#endif   //#if( AP4_RP == ENABLE )
}

#pragma CODE = MMI_AP4_FANYI_PRESS_CODE

void fanyi_key_press( void )
{
    SendString((U8 *)"\r\nfanyi_key_press!");
    if(b_fanyi)return;
    if(b_ready_findword) {
        b_findword_ch_display = TRUE;
        b_ready_findword = FALSE;
        b_check_word = FALSE;
        send_hex_u16(leng_en);
        ch_display_address = findword_address + (U32)(leng_en+2);
//  ch_display_address += 2;
    }

}

#pragma CODE = MMI_AP4_BAKKEY_PRESS_CODE

Bool bak_key_press( void )
{
    init_array_v();
   // if(!g_setting_list.struct_setting.bluetooth) {
        if(g_b_REREAD_Flag) {
            //Current_Index = NORMAL_MODE_INDEX;
            virtualKeyState = GOTO_NOMAL;
        } else {
            //Current_Index = REREAD_MODE_INDEX;
            virtualKeyState = GOTO_REREAD;
        }
        //Search_Record.news_sys.flag_anjian = true;
    //}
    return TRUE;
}



#pragma CODE = MMI_AP4_KEY_PRESS_CODE02
#if 1
Bool mode_key_press( void )
{
    if(b_fanyi) {
        b_fanyi_back_display = TRUE;
        return TRUE;
    }
    
    mail_send_event( EVT_SEARCH_TO_BROWSER,BACK_TO_SUBROOT );
    if(open_dir_num <= CMP_DIR_NUM_OFFSET) {
        b_BackSubdir = 0;
    } else {
        open_dir_num -= DIR_NUM_OFFSET;
        b_BackSubdir = 1;
    }

    return FALSE;
}

#else
Bool mode_key_press( void )
{
    if( ap4_play_state != AP4_PLAY_ERROR )
        return FALSE;
    if( bInCal )
        return FALSE;
    if( g_b_recorder_on ) {
        /*srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
        mmi_ap3_kbd_enable();
        scan_oid_enable();*/
        return FALSE;
    }
    //bRmoteExitAp4 = FALSE;
    //Ts_free( s_ts_id_read );
    if( player_ap3_is_busy())
        player_ap3_stop();
    if( s_ts_id_read != UNDEFINED_TIMER ) {
        Ts_free( s_ts_id_read );
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );*/
    }

    if( g_b_search_record ) {
        g_b_search_record = FALSE;
#if 0
        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            //mmi_ap3_display_icon();*/
        }
#endif
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }
    //mmi_ap3_display_icon();

#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    /*rp_flag_bits.value &= ~0x07;
    if( rp_ok_flag )
    {
       rp_ok_flag = FALSE;
    }
    rp_flag_bits.value = 0;*/
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
    if( rp_ok_flag ) {
        rp_ok_flag = FALSE;
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
    }
#endif   //#if( AP4_RP == ENABLE )

    repeat_save();
    init_array_v();
    scan_oid_enable();
    //if( Search_Record.news_sys.repeat_flag || Search_Record.news_sys.play_record_flag )
    if( Search_Record.value & 0x0101u/*0x8080*/ ) {
        //Search_Record.news_sys.repeat_flag = 0;
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
    }
    if( g_b_ap4_caption ) {
        g_b_ap4_caption = FALSE;
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
    }

    /*Search_Record.news_sys.reread_flag
       = Search_Record.news_sys.repeat_flag
          = Search_Record.news_sys.sequence_flag
             = Search_Record.news_sys.index_normal_flag
                = Search_Record.news_sys.play_record_flag
                   = 0;*/
    Search_Record.value &= ~0x03C1u;//0x81C0u;

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;

    order_play_count = 0;
    if( bRmoteExitAp4 )
        return TRUE;

    if(( Ap3_MenuPath >= CONTROL_MENU_PATH )
       && News_Go.Num_ControlTab ) {
        Temp_Control_num = Current_Record.Control_num;
        if( ++Current_Record.Control_num >= News_Go.Num_ControlTab )
            Current_Record.Control_num = 0;
        State_Search = Control_Mode_Search;
        ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
        bKeyChangeControl = TRUE;
    }
    order_play_count = 0;
    reread_enable_flag = FALSE;

    return TRUE;
}
#endif

#pragma CODE = MMI_AP4_KEY_PRESS_CODE03

Bool volume_key_press( void )
{
//  mmi_ap3_display_volume();
    //srvplayer_volume_change( SRVPLAYER_VOL_UP );
    //if( bSpeechEnd && !g_b_recorder_on && !g_b_search_record && !bInCal
    if( !g_b_player_on && !g_b_recorder_on && !g_b_search_record && !bInCal
        &&( ap4_play_state == AP4_PLAY_ERROR )) {
        reread_enable_flag = FALSE;
        //g_b_enable_flush = TRUE;
        if( All_Control.Addr_Antion_key ) {
            Search_Record.news_sys.flag_anjian = 1;
            Search_Record.news_sys.poweroff_flag = 5;
        }
        if( g_b_ap4_caption ) {
            g_b_ap4_caption = FALSE;
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();*/
            return TRUE;
        }
    }
    return FALSE;
}

#pragma CODE = MMI_AP4_KEY_PRESS_CODE06
void display_key_text(void)
{
    U32 Iaddr;
    U16 temp_length;
    if(!b_ready_findword)return;
    if(b_fanyi)return;
    if(!b_check_word)return;
    b_findword_ch_display = TRUE;

    Iaddr = firstword_address + (U32)(4*(num_current_word-1));

    read_data_from_nf(Iaddr, (U8 _MEM_TYPE_SLOW_ *)&ch_display_address, (U16)sizeof(U32));
    SendString((U8 *)"\r\ndisplay key text!=");
    send_hex_u32(ch_display_address);
    if(ch_display_address == 0xFFFFFFFF)return;
    word_mp3_address = ch_display_address + (U32)(16);
    ch_display_address = ch_display_address + (U32)(25);
    //word_mp3_address = 280;
    //read_data_from_nf(ch_display_address, (U8 _MEM_TYPE_SLOW_ *)&temp_length, (U16)sizeof(U16));
    //Iaddr = ch_display_address + (U32)(temp_length+2);
    //send_hex_u32(word_mp3_address);
    //Iaddr = 0x00000118;
    //write_media_addr(word_mp3_address, 0 );
    //state_search_to_play();
    b_play_word_mp3 = TRUE;

    //ch_display_address += (U32)(9);



    /*
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
      lcd_goto_xy( 15, 24 );
      Ih_set_curr_font( IDC_FONT_16 );
      lcd_puts( g_string );
      mmi_ap3_display_icon(FALSE);
    */
}

//#pragma CODE = MMI_AP4_EXIT_CODE
#pragma CODE = MMI_AP4_DISPTIME_CODE01
/*
void mmi_ap4_exit_to_idle( void )
{
   Ts_free( s_ts_id );
   g_select_appli = IDLE_APPLI;
   Mmgr_kill_this_mmi_appli();
}
*/
//! @brief Display time and informations about play
//!

void dowith_timer(U8 time_flag)
{
    U32 temp_data;

    /*
      if(time_flag == PLAY_BEGIN_TIME){
       s_ts_id = ts_alloc();
       ts_set_time( s_ts_id, PLAYER_TIME );
       temp_data = 0;
       }
     else if(time_flag == PLAY_ALL_TIME){
       if(audio_is_wav == 0){
         temp_data = media_paly_all_time/1000;
           }
       else{
           temp_data = media_paly_all_time/(U32)size_psec_wav;
       //temp_data = media_paly_all_time;
           }
       if(temp_data<=2){
           count_AB = 0x80;
           }
       else{
           count_AB = 0;
           }

       }
     else{
          ts_set_time( s_ts_id, PLAYER_TIME );
       if(audio_is_wav == 0){
         temp_data = current_play_time/1000;
           }
       else{
    //      temp_data = Frame_Count;
       temp_data = current_play_time/(U32)size_psec_wav;
           //temp_data = (media_paly_all_time - temp_data<<8)/size_psec_wav;
       // temp_data = media_paly_all_time - temp_data;
           }
       }
    */

    ts_set_time( s_ts_id, PLAYER_TIME );
    if(audio_is_wav == 0) {
        temp_data = current_play_time/1000;
    } else {
        temp_data = current_play_time/(U32)size_psec_wav;
    }
//  SendString((U8 *)"\r\nCurrent_play_time = ");
//   send_hex_u32(temp_data);
    srvplayer_all_time.seconds = temp_data%60;
//  temp_data = temp_data/60;
    srvplayer_all_time.minutes = temp_data/60;

    Lcd_unset_merging();
    Ih_set_curr_font( IDC_FONT_08 );
    memset( time_buf, 0, 6+4 );
    lcd_goto_xy( 4, 56 );
}
#pragma CODE = MMI_AP4_DISPTIME_CODE
void mmi_ap4_display_dynami_time(U8 time_flag)
{
    _MEM_TYPE_SLOW_ U8 string[6+4], i;
    // Draw time
//      U32 temp_data;

    SDKLoadCode(MMI_AP4_DISPTIME_CODE01);
    dowith_timer(time_flag);
    /*
       U32 temp_data;

       if(time_flag == PLAY_BEGIN_TIME){
        temp_data = 0;
        }
      else if(time_flag == PLAY_ALL_TIME){
        if(audio_is_wav == 0){
             temp_data = media_paly_all_time/1000;
            }
           else{
            temp_data = media_paly_all_time/(U32)size_psec_wav;
            //temp_data = media_paly_all_time;
            }
           if(temp_data<=2){
            count_AB = 0x80;
            }
           else{
            count_AB = 0;
            }

        }
      else{
           if(audio_is_wav == 0){
             temp_data = current_play_time/1000;
            }
           else{
    //      temp_data = Frame_Count;
            temp_data = current_play_time/(U32)size_psec_wav;
            //temp_data = (media_paly_all_time - temp_data<<8)/size_psec_wav;
           // temp_data = media_paly_all_time - temp_data;
            }
        }

       SendString((U8 *)"\r\nCurrent_play_time = ");
       send_hex_u32(temp_data);
       srvplayer_all_time.seconds = temp_data%60;
     //  temp_data = temp_data/60;
       srvplayer_all_time.minutes = temp_data/60;

       Lcd_unset_merging();
       Ih_set_curr_font( IDC_FONT_08 );
       memset( string, 0, 6+4 );
       if(time_flag == PLAY_ALL_TIME){
        lcd_goto_xy( 20, 56 );
        strcat(( char *)string, "/" );
        }
       else{
          lcd_goto_xy( 4, 56 );
        }
     */
    SDKLoadCode( MMI_AP4_COV8_CODE );
    convert_U8(( char *)&time_buf[ 6 ], srvplayer_all_time.minutes );
    i = strlen(( char *)&time_buf[ 6 ] );
    for( ; i < 2; i++ )
        strcat(( char *)time_buf, "0" );
    strcat(( char *)time_buf, ( char *)&time_buf[ 6 ] );
    strcat(( char *)time_buf, ":" );

    memset( &time_buf[ 6 ], 0, 4 );
    convert_U8(( char *)&time_buf[ 6 ], srvplayer_all_time.seconds );
    i = strlen(( char *)&time_buf[ 6 ] );
    for( ; i < 2; i++ )
        strcat(( char *)time_buf, "0" );
    strcat(( char *)time_buf, ( char *)&time_buf[ 6 ] );
    lcd_puts( time_buf );
//   SendString((U8 *)"\r\nflush!");
}


#pragma CODE = MMI_AP4_DISPALLTIME_CODE
void mmi_ap4_display_all_time(void)
{
    /*
       _MEM_TYPE_SLOW_ U8 string[6+4], i;

       U32 temp_data;


       if(b_disptime){
        b_disptime = 0;
        return;
        }
       s_ts_id = ts_alloc();
       ts_set_time( s_ts_id, PLAYER_TIME );
        if(audio_is_wav == 0){
             temp_data = media_paly_all_time/1000;
            }
           else{
            temp_data = media_paly_all_time/(U32)size_psec_wav;
            }
           if(temp_data<=2){
            count_AB = 0x80;
            }

       srvplayer_all_time.seconds = temp_data%60;
       srvplayer_all_time.minutes = temp_data/60;

       Lcd_unset_merging();
       Ih_set_curr_font( IDC_FONT_08 );
       lcd_goto_xy( 4, 56 );
       lcd_puts((U8 *)"00:00/");
       memset( string, 0, 6+4 );
       lcd_goto_xy( 24, 56 );

       SDKLoadCode( MMI_AP4_COV8_CODE );
       convert_U8(( char *)&string[ 6 ], srvplayer_all_time.minutes );
       i = strlen(( char *)&string[ 6 ] );
       for( ; i < 2; i++ )
          strcat(( char *)string, "0" );
       strcat(( char *)string, ( char *)&string[ 6 ] );
       strcat(( char *)string, ":" );

       memset( &string[ 6 ], 0, 4 );
       SDKLoadCode( MMI_AP4_COV8_CODE );
       convert_U8(( char *)&string[ 6 ], srvplayer_all_time.seconds );
       i = strlen(( char *)&string[ 6 ] );
       for( ; i < 2; i++ )
          strcat(( char *)string, "0" );
       strcat(( char *)string, ( char *)&string[ 6 ] );
       lcd_puts( string );

    //*/
}

#pragma CODE = MMI_AP4_KEY_LONGPRESS_CODE00

void power_key_long_press( void )
{
    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;

    //bInCal = FALSE;
    bRmoteExitAp4 = FALSE;
    if( power_is_vbus_present()) {
        //if( Usb_connect )
        mail_send_event( EVT_USB_POWERED, 0 );
        return;
    }

#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
#endif   //#if( AP4_RP == ENABLE )

    reread_enable_flag = FALSE;
    Search_Record.value = 0;
    mail_send_event( EVT_ENTER_IN_POWEROFF, 0 );
}


#if ( PASTER_MODE == ENABLE )
#pragma CODE = MMI_AP4_KEY_LONGPRESS_CODE01

void rec_key_long_press( void )
{
    bRmoteExitAp4 = FALSE;
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
    //scan_oid_disable();
    //mmi_ap3_kbd_disable();
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    /*if( g_b_search_record )
    {
       g_b_search_record = FALSE;
       ap4_play_state = AP4_PLAY_ERROR;
    }*/
    init_array_v();
    //ap3_poweron_state = MMI_CHANGE_TO_AP3;
    //bInCal = FALSE;
    //Search_Record.value = 0;
    Search_Record.value &= ~0xF3FFu;//0xFFCF;
    mail_send_event( EVT_SEARCH_END, PASTER_APPLI );

#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
#endif   //#if( AP4_RP == ENABLE )

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    reread_enable_flag = FALSE;
}
#endif   //#if ( PASTER_MODE == ENABLE )


#pragma CODE = MMI_AP4_KEY_LONGPRESS_CODE02

void mode_key_long_press( U8 appli_id)
{
    bRmoteExitAp4 = FALSE;

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    //scan_oid_disable();
    //mmi_ap3_kbd_disable();
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    reread_enable_flag = FALSE;
    /*ap3_poweron_state = MMI_CHANGE_TO_AP3;
    if( g_b_search_record )
    {
       g_b_search_record = FALSE;
       ap4_play_state = AP4_PLAY_ERROR;
    }*/
    /*
       if(State_Search != Browser_File){
        mail_send_event( EVT_SEARCH_CHANGE_FILE, Browser_File_Init);
        return;
        }
    */
    switch( ap3_mode ) {
        case SINGLE_PLAY_MODE:
            //tx_send_string("sequence mode\r\n");
            init_array_v();

        case SEQUENCE_PLAY_MODE:
            bInCal = FALSE;
            //Search_Record.value = 0;
            Search_Record.value &= ~0xF3FFu;//0xFFCFu;
//      mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
            mail_send_event( EVT_SEARCH_END, appli_id );
            break;
    }

#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
#endif   //#if( AP4_RP == ENABLE )
}


#pragma CODE = MMI_AP4_KEY_LONGPRESS_CODE03

void vol_key_long_press( U8 keyID )
{
    //bRmoteExitAp4 = FALSE;

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    reread_enable_flag = FALSE;

    scan_oid_disable();
    mmi_ap3_kbd_disable();
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
    }
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }
    Search_Record.value = 0;
    init_array_v();
    state_search_to_play();
    //mail_send_event( EVT_SEARCH_CHANGE_FILE, keyID );
    order_play_count = 0;

    Ts_free( s_ts_id_read );
    g_b_ap4_caption = FALSE;

#if( AP4_RP == ENABLE )
    /*rp_ok_flag = FALSE;
    rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    //rp_flag_bits.value &= ~0x0F;
    rp_flag_bits.value &= ~0xF000u;
#endif   //#if( AP4_RP == ENABLE )

    if( !bRmoteExitAp4 )
        mail_send_event( EVT_SEARCH_CHANGE_FILE, keyID );
    bRmoteExitAp4 = FALSE;
}




#pragma CODE = MMI_AP4_REREAD_REC_CODE

void mmi_ap4_reread_record_deal( void )
{
#if (REREAD_MODE == ENABLE)
    if(!g_b_REREAD_Flag)return;
#endif    
    SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE00 );
    ap4_record_prepare_display();
    SDKLoadCode( MMI_AP4_REC_TEMP_CODE00 );
    if( !ap3srvrec_ready()) {
        SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE01 );
        ap4_record_fail_display();
    }
}


#pragma CODE = MMI_AP4_REREAD_PLAY_CODE

void mmi_ap4_reread_play_deal( void )
{

    //SendString((U8 *)"playRECORD\r\n");
#if (REREAD_MODE == ENABLE)
    if(!g_b_REREAD_Flag)return;
#endif     
    SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE03 );
    ap4_record_playback_display();
    SDKLoadCode( MMI_AP4_REC_TEMP_CODE01 );
    if( !ap3srvrec_reread_on()) {
        //g_b_search_record = FALSE;
        //scan_oid_enable();
        //mmi_ap3_kbd_enable();
        SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE04 );
        ap4_record_playback_fail_deal();
    }
    //scan_oid_enable();
    //SendString((U8 *)"\r\nwav star");
}


#pragma CODE = MMI_AP4_REMOTE_AP4_CODE

void remote_change_ap4_file_deal( void )
{
#if( FAT_CLUSTER_BUF == ENABLE )
    // fat_clusterlist_save();
    cluster_dowith(FALSE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( GET_FILE_NAME_CODE );
    ( void )Get_File_Name(( U8 *)AP4_EXT, Current_Record.Ap3_num );

    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE03 );
    //SDKLoadCode( NAVIGATION_CODE );
    if( !do_change_ap3()) {
        SDKLoadCode( GET_FILE_NAME_CODE );
        ( void )Get_File_Name(( U8 *)AP4_EXT, Current_Record.Ap3_num );
    }
}


#pragma CODE = MMI_AP4_NOTFIND_AP4_CODE

void remote_not_find_ap4_deal( void )
{
    //SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE08 );
    //ap4_search_nofind_file_display();
    SDKLoadCode( MMI_AP4_TEMP_CODE04 );
    if( !mmi_ap3_not_find_playintro()) {
        SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE05 );
        ( void )open_file();

#if( FAT_CLUSTER_BUF == ENABLE )
        //SDKLoadCode( FS_CREATE_BUF_CODE );
        //( void )FSCreateBuf();
        //fat_clusterlist_restore();
        cluster_dowith(TRUE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

    }
}

#pragma CODE = BROWSER_KEY_RELEASE_CODE
Bool browser_key_release(U16 param)
{
//  send_hex_u16(param);
    switch( param ) {

        case ((KEY_POWER<<8)|KBD_KEY_RELEASED):
            //    case ((KEY_VOL_UP<<8)|KBD_KEY_RELEASED):
            if((State_Search == Browser_File)||(State_Search == Browser_DirFile)) {
                browserfile_cmd_id = BROWSER_Cmd_Enter;
                return TRUE;
            }
            break;

        case ((KEY_NEXT<<8)|KBD_KEY_RELEASED):
            browserfile_cmd_id = BROWSER_Cmd_NextFile;
            return TRUE;
            break;

        case ((KEY_PREV<<8)|KBD_KEY_RELEASED):
            browserfile_cmd_id = BROWSER_Cmd_PrevFile;
            return TRUE;
            break;

        case ((KEY_MODE<<8)|KBD_KEY_RELEASED):
            browserfile_cmd_id = BROWSER_Cmd_Back;
            return TRUE;
            break;
            /*
                  case ((KEY_MODE<<8)|KBD_KEY_LONGPRESSED):
                    search_task_stop();
                    g_select_appli = IDLE_APPLI;
                    Mmgr_kill_this_mmi_appli();
                    return TRUE;
                    break;

                  case ((KEY_POWER<<8)|KBD_KEY_LONGPRESSED):
                    search_task_stop();
                    g_select_appli = POWER_OFF_APPLI;
                    Mmgr_kill_this_mmi_appli();
                    return TRUE;
            */
            break;


        default:
            browserfile_cmd_id = BROWSER_Cmd_End;
            break;

    }
    return FALSE;
}


#if( AP4_RP == ENABLE )

#pragma CODE = MMI_AP4_RP_REC_TEMP_CODE00

void rp_record_prepare_deal( void )
{
    SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE00 );
    ap4_record_prepare_display();
    //SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
    SDKLoadCode( MMI_AP4_RP_TEMP_CODE01 );
    if( !rp_record_find( rp_oid_index )) {
        SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE01 );
        ap4_record_fail_display();
    }
}


#pragma CODE = MMI_AP4_RP_REC_TEMP_CODE01

void rp_record_start_deal( void )
{
    SDKLoadCode( MMI_AP4_RP_TEMP_CODE02 );
    if( !paster_record_start()) {
        SDKLoadCode( MMI_AP4_RECORD_TEMP_CODE01 );
        ap4_record_fail_display();
    }
}


#pragma CODE = MMI_AP4_RP_PLAY_TEMP_CODE00

void rp_play_prepare_deal( void )
{
    //SDKLoadCode( MMI_AP4_RP_TEMP_CODE00 );
    SDKLoadCode( MMI_AP4_RP_TEMP_CODE03 );
    if( !rp_play_find( rp_oid_index )) {
        SDKLoadCode( MMI_AP4_RP_TEMP_CODE06 );
        rp_nofile_display();
    }
}


#pragma CODE = MMI_AP4_RP_PLAY_TEMP_CODE01

void rp_play_start_deal( void )
{
    SDKLoadCode( MMI_AP4_RP_TEMP_CODE04 );
    if( !Ap4_rp_play_start()) {
        SDKLoadCode( MMI_AP4_RP_TEMP_CODE07 );
        rp_play_fail_deal();
    }
}
#pragma CODE = READY_CHANGE_BOOK_CODE
void ready_change_book(void)
{
    SDKLoadCode( MMI_AP4_PLAY_STOP_CODE );
    ap4_play_stop();
    //SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
    //  mode_key_long_press(g_select_appli);
    mail_send_event( EVT_SEARCH_END, IDLE_APPLI);
    /*
      Current_Record.Ap3_num = g_xuanshu_num -1;
      SDKLoadCode( AP4_SEARCH_TOBROWSER_CODE );
      ap4_search_end_to_browser( Open_File_Search);
      ap3_poweron_state = MMI_AP3_NORMAL;
    */
}

/*
#pragma CODE = ADB_INIT_CODE

void init_adb_file( void )
{

    __X FS_ERR err;
    char temp_dir[256];
    int  temp_data;
    unsigned long temp_len;
    b_adb_file = FALSE;
    strcpy(temp_dir,DEFAULT_DIC_FILE);
    temp_len = FSGetFileSize(temp_dir,&err);
    if(temp_len==0)
    {
        return;
    }

    adb_g_file = ( FCFILE __X *)FSOpen(temp_dir,FS_MODE_READ,&err);
//  *fat_g_cur_file = FSOpenForFastSeekCustom(temp_dir,FS_MODE_READ,&cluster_buf,CLUSTER_BUF_SIZE,SKIPS_CLUSTER,&err);
    if(err == FS_NO_ERROR){
        b_adb_file = TRUE;
        }
    return;
}

void find_word_from_adb( void )
{

    return;
}
//*/

#endif   //#if( AP4_RP == ENABLE )


#endif  //#if( AP3 == ENABLE )

#endif // _mmi_ap3_c_

