//!
//! @file mmi_idle.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application IDLE
//!
//! Is the base application of the MMI.
//! Must be lauched first by the kernel.
//!
//! @version 2.4 snd3-dvk-1_9_5 $Id: mmi_idle.c,v 2.4 2007/07/26 14:59:08 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_idle_c_
#define _mmi_idle_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\status\mmi_status.h"
#include "mmi\player\mmi_player.h"
#include "mmi\player\srv_player.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "mmi\kbd\keypad.h"

#include "mmi_idle.h"
#include "point_g.h"

#include "conf\conf_oid.h"
#include "conf\conf_sio.h"
#include "modules\player\player.h"
#include "sio\com.h"
#include "mmi\shared\com_var.h"
#include <stdio.h>
#include "modules\search\search_remote.h"
#include "drivers\oid\oid.h"
#include "modules\power\power.h"             // Power module definition

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include <string.h>
#include "mp3_include.h"

#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
//#include "mmi\status\mmi_status.h"


#pragma DATA = MMI_IDLE_DATA
#pragma CODE = MMI_IDLE_CODE

ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );
ImageDeclareType( POWER_TASK_TEMP_CODE05 );

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

ImageDeclareType( READ_CAB_CODE );
ImageDeclareType( INDEX_DISPLAY_CODE );
ImageDeclareType( FIND_DIR_FILE_01_CODE );
ImageDeclareType( FIND_DIR_FILE_02_CODE );
ImageDeclareType( CREAT_BOOK_CODE );
ImageDeclareType( INIT_IDLE_CODE );

ImageDeclareType( READ_BNL_CODE );
ImageDeclareType( SOME_INDEX_CODE );
ImageDeclareType( FIND_INDEX_BOOK_CODE );
ImageDeclareType( CHANGE_SHU_CODE );
ImageDeclareType( NOFINDBOOK_CODE );
ImageDeclareType( EXIT_IDLE_CODE );
ImageDeclareType( CHECK_UPDATE_CODE );
ImageDeclareType( CHANGE_OIDTYPE_CODE );

_MEM_TYPE_SLOW_ U8 oid_display_enable;
_MEM_TYPE_SLOW_   U8                   idle_current_mark;
_MEM_TYPE_SLOW_   U8                   idle_page_num;
_MEM_TYPE_SLOW_   U8                   idle_current_page_num;
_MEM_TYPE_SLOW_   U8                   idlenum;

Bool  b_kbd_process_disable;


//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

#define IDLE_LINE_PAGE          2
//------------------------------------------------------------------------
#if( LCD_PRESENT == ENABLE )
//***********************************************
//2007-03-28 add by xuanyu
#if( MAIN_MENU_PICTURE == DISABLED )
#if 1
#define HEIGH_MENU   18
#define WIDTH_MENU   42

U8 point_en_array[12]= {0,26,42,26,84,26,0,45,42,45,84,45};
//U8 point_ch_array[12]= {0,26,44,26,84,26,0,45,44,45,84,45};
U8 point_ch_array[12]= {0,26,44,26,84,26,0,45,44,45,84,45};
#define NB_MAIN_MENU_CHINESE_SIMP           2

#else
#define NB_MAIN_MENU_CHINESE_SIMP           5
const char *s_main_menu_english[ ] = {
//  "U disk"

#if( AP3 == ENABLE )
#if( afaya_pen == ENABLE )
    "Afaya Pen"
#endif   //#if( afaya_pen == ENABLE )
#if( i_speak == ENABLE )
    "I speak"
#endif   //#if( i_speak == ENABLE )
#endif   //#if( AP3 == ENABLE )
#if( PASTER == ENABLE )
    , "Paster"
#endif   //#if( PASTER == ENABLE )


    , "Study Text"

    , "Record/Mp3"
#if( DIC == ENABLE )
    , "Dictionary"
#endif   //#if( DIC == ENABLE )

    , "Setting"
    , "Wireless"
};


const char *s_main_menu_chinese_simp[ ] = {
//  "U disk"
#if( AP3 == ENABLE )
#if( afaya_pen == ENABLE )
    "点 读 机"
#endif   //#if( afaya_pen == ENABLE )
#if( i_speak == ENABLE )
    "I speak"
#endif   //#if( i_speak == ENABLE )
#endif   //#if( AP3 == ENABLE )

#if( PASTER == ENABLE )
    , "Paster"
#endif   //#if( PASTER == ENABLE )


    , "课本学习"

    , "录音/MP3"

#if( DIC == ENABLE )
    , "电子词典"
#endif   //#if( DIC == ENABLE )
    , "系统设置"
    , "无线模组"
};
#endif
#endif

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

void     idle_display            ( U8 );
void     idle_display_oid        ( U16 index );
char     *convert_U8             ( char *p, U8 i );
char     *convert_U16            ( char *p, U16 i );
U8       conv_hex                ( U8 v );
void     mmi_idle_display_volume ( void );
void init_idle(void);
void idle_mark_display(U8 mark);
void idle_menu_display(U16 display_num_mark);
extern void  lcd_display_time         ( Bool flag);
extern void set_oid_type(Bool set_type);
#else
#define idle_display( entire_draw )
#define idle_display_oid( index )
#endif  //#if( LCD_PRESENT == ENABLE )
Bool find_ap4_dir( void );
void look_ap4_file( void );
void lcd_map_line(U8 app,U8 write_data);
void update_check(void);
void read_set_calib( void );
Bool global_key_deal(U16 param);
void wireless_index_display(U16 temp_data);
void create_readbnl_file(void);
void clear_bnl(void);
void write_readbnl_file(U32 offset,U16 data);
U16 get_name_lead( void );
U16 read_bnl_file(U16 off_t);
Bool some_index_dowith( void );
Bool judge_shu(void);
U16  chang_shu_num(U16 yuan_index);

void no_find_xdf    (void);
void exit_idle(void);
Bool no_find_xdf_playintro( void );

//extern   Bool                    g_b_storage_bulid_on;
static _MEM_TYPE_SLOW_ U8  s_ts_id;

//_____ D E F I N I T I O N S ______________________________________________
//!
//! idle_mmi_appli
//!
//! Manage the application idle
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void idle_mmi_appli(U8 event, U16 param)
{
    switch( event ) {
        case EVT_START_APPLI:
            SendString((U8 *)"\r\nidle");

            mmi_idle_kbd_disable();
            mmgr_set_appli_full_screen();
            //Lcd_backlight_enable();
            //SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            //lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
            //lcd_clear_part_of_screen( 64, 0,54, 8);
            //lcd_clear_part_of_screen(20, 0,64, 8);
            Lcd_rearm_backlight_on();
            power_authorize_autopoweroff();
            //srvplayer_volume_send();
            SDKLoadCode(CHECK_UPDATE_CODE);
            update_check();
            s_ts_id = UNDEFINED_TIMER;

            if(!g_b_booksys_flag) {
                g_b_booksys_flag = TRUE;
                //SDKLoadCode(CREAT_BOOK_CODE);
                //create_readbnl_file();
                //clear_bnl();
#if( T01OID_CAB == ENABLE )
                SDKLoadCode(READ_CAB_CODE);
                read_set_calib();
#endif //     
                //g_b_storage_bulid_on = FALSE;

                SDKLoadCode(FIND_DIR_FILE_01_CODE);
                if(find_ap4_dir()) {
                    ap4num_diandu = 0;
                    dirnum_diandu = 0;
                    SDKLoadCode(FIND_DIR_FILE_02_CODE);
                    look_ap4_file();
                }
                dirnum_diandu = 0;
            }

#if( OID == ENABLE )
            // b_oid_init_flag = FALSE;
            if( !b_oid_init_flag ) {
                reinit_oid_pen();
                scan_oid_disable();
            }
#endif   //#if( OID == ENABLE )
#if( OID == ENABLE )
#if( OID_VERSION == OID2 )
            bPenLeave = TRUE;
#endif   //#if( OID_VERSION == OID2 )
#endif   //#if( OID == ENABLE )

            Kbd_set_evt_enabled();
            if( Kbd_is_event_triggered())
                Kbd_set_evt_disabled();

            scan_oid_enable();
            mmi_idle_kbd_enable();
            oid_display_enable = FALSE;

            if(g_b_xuanshu_flag) {
                mail_send_event( EVT_IDLE_CHOICE_BOOK_MODE, 0 );
            } else {
                mail_send_event( EVT_IDLE_INIT_OVER_MODE, 0 );
            }
            break;
        case EVT_IDLE_CHOICE_BOOK_MODE:
            //SendString((U8 *)"\r\nc_b!");
            scan_oid_disable();
            mmi_idle_kbd_disable();
            SDKLoadCode(FIND_INDEX_BOOK_CODE);
            if(!judge_shu()) {
                SDKLoadCode(NOFINDBOOK_CODE);
                no_find_xdf();

                if( !no_find_xdf_playintro() ) {
                    s_ts_id = ts_alloc();
                    ts_set_time( s_ts_id, MODE_DELAY);
                    // Play fail then startup finish
                }

//          mail_send_event( EVT_IDLE_INIT_OVER_MODE, 0 );
            }
            break;
        case EVT_IDLE_INIT_OVER_MODE:
            // SendString((U8 *)"\r\ni_O!");
            scan_oid_enable();
            mmi_idle_kbd_enable();
            b_Idle_ReadOid = FALSE;
            SDKLoadCode(INIT_IDLE_CODE);
            init_idle();
            //
            //scan_oid_enable();
            b_g_index_flag = FALSE;
//    Kbd_set_evt_enabled();
#if 0

#else
//     goto mmi_idle_next;
#endif
            break;

        // case EVT_APPLI_KILLED:
        // break;

        case EVT_TIMER:
            if( LSB(param) != s_ts_id )
                break;
            // Startup finish
            //s_startup_step = DEVICE_STATUS;
            Ts_free( s_ts_id );
            //Mmgr_kill_this_mmi_appli();
            //goto next_idle;
            mail_send_event( EVT_IDLE_INIT_OVER_MODE, 0 );
            break;

        case EVT_PLAYER:
            if( param == PLAYER_EOF ) {
                //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
                //srvplayer_switch_off();  // Power off player
                //power_authorize_autopoweroff();
                //Mmgr_kill_this_mmi_appli();
//next_idle:
                mail_send_event( EVT_IDLE_INIT_OVER_MODE, 0 );
                //scan_oid_enable();
                //Kbd_set_evt_enabled();
            }
            break;

        case EVT_BACK_TO_TOP:
//mmi_idle_next:
            // We just need to redisplay the idle screen entirely
//      if( g_select_appli == IDLE_APPLI )
//         idle_display( TRUE );

            //SendString((U8 *)"start appli\r\n");
#if 0
            switch( g_select_appli ) {
                case IDLE_APPLI:
                    bInEbookMode = FALSE;
                    oid_display_enable = TRUE;
                    break;

#if( PASTER == ENABLE )
                case PASTER_APPLI:
#if( PASTER_MODE == ENABLE )
                    bInEbookMode = FALSE;
                    g_old_select_appli = PASTER_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_PASTER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_PASTER );
#endif   //#if( PASTER_MODE == ENABLE )
                    break;
#endif   //#if( PASTER == ENABLE )

                case PLAYER_APPLI:
                    bInEbookMode = FALSE;
                    g_old_select_appli = PLAYER_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_PLAYER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_PLAYER );
                    break;

#if( AP3 == ENABLE )
                case AP3_APPLI:
                    bInEbookMode = FALSE;
                    g_old_select_appli = AP3_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_AP3, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_AP4 );
                    break;
#endif   //#if( AP3 == ENABLE )

                case STUDY_APPLI:
                    bInEbookMode = FALSE;
                    g_old_select_appli = STUDY_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_AP3, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_STUDY);
                    break;

                case RECORDER_APPLI:
                    /*
                    bInEbookMode = FALSE;
                    g_old_select_appli = RECORDER_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_RECORD );
                    */
                    break;

#if( DIC == ENABLE )
                case DIC_APPLI:
                    g_old_select_appli = DIC_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_DIC, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_DIC );
                    break;
#endif   //#if( DIC == ENABLE )

                case SETTING_APPLI:
                    bInEbookMode = FALSE;
                    g_old_select_appli = SETTING_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_SETTING, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_SETTING );
                    break;

                case POWER_OFF_APPLI:
                    bInEbookMode = FALSE;
                    g_old_select_appli = POWER_OFF_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_POWEROFF );
                    break;
            }
#endif
            break;

        /*case EVT_EXIT_POWEROFF:
        break;*/

        case EVT_KEY_PRESSED:
            if( mmi_idle_kbd_is_disable())break;
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
            if( g_mmi_lcd_backlight_off ) {
                if( KEY_STATUS( param )&( KBD_KEY_RELEASED | KBD_KEY_REPEAT_RELEASED ))
                    g_mmi_lcd_backlight_off = FALSE;
                break;
            }
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
            // Only if feature keyboard is activated
            SendString((U8 *)"\r\nidle_key=");
            send_hex_u16(param);
            if(global_key_deal(param))break;
            if( KBD_KEY_LONGPRESSED == KEY_STATUS( param )) {
                switch( KEY_ID( param )) {
                    case KEY_POWER:
                        //oid_display_enable = FALSE;
                        if( power_is_vbus_present()) {
                            //g_select_appli = MSTORAGE_APPLI;
                            //mmgr_activate_mmi_appli( MMI_APPLI_MSTORAGE, 0 );
                            CommonLoadCode( C_USB );
                        } else {
                            //mail_send_command( CMD_POWER_OFF, 0 );
                            g_select_appli = POWER_OFF_APPLI;
                            g_power_off_mode = POWER_OFF;
                            //mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
                            CommonLoadCode( C_POWEROFF );
                        }
                        //Mmgr_kill_this_mmi_appli();
                        break;

                    case KEY_BAK:
                        g_select_appli = g_old_select_appli = RECORDER_APPLI;
                        //mmgr_activate_mmi_appli( MMI_APPLI_PASTER, 0 );
                        //Mmgr_kill_this_mmi_appli();
                        recorder_mode = 1;
                        CommonLoadCode( C_RECORD);
                        break;
#if ( PASTER_MODE == ENABLE )
                    case KEY_MEN:
                        g_select_appli = g_old_select_appli = PASTER_APPLI;
                        //mmgr_activate_mmi_appli( MMI_APPLI_PASTER, 0 );
                        //Mmgr_kill_this_mmi_appli();
                        CommonLoadCode( C_PASTER );
                        break;
#endif   //#if( PASTER_MODE == ENABLE )
                    case KEY_MODE:
                        if(!b_Idle_ReadOid) {
                            scan_oid_enable();
                            b_Idle_ReadOid = TRUE;
                        }
                        break;
                }
            } else if( KBD_KEY_RELEASED == KEY_STATUS( param )) {
                switch( KEY_ID( param )) {
                    case KEY_VOL_DOWN:
                        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                        mmi_idle_display_volume();
                        break;
                    case KEY_VOL_UP:
                        srvplayer_volume_change( SRVPLAYER_VOL_UP);
                        mmi_idle_display_volume();
                        break;
#if 1

                    case KEY_PREV:
                        if((idle_current_mark)==0)break;
                        lcd_map_line(idle_current_mark, 0x00);
                        idle_current_mark --;
                        lcd_map_line(idle_current_mark, 0xFF);

//          SendString((U8 *)"\r\nidle_current_mark:idle_current_page_num=");
//          send_hex_u8(idle_current_mark);
//          SendString((U8 *)":");
//          send_hex_u8(idle_current_page_num);
                        g_old_select_appli = idle_current_mark;
                        break;

                    case KEY_NEXT:
//            srvplayer_volume_change( SRVPLAYER_VOL_UP );
//            mmi_idle_display_volume();
                        if((idle_current_mark+1)==idlenum)break;
                        lcd_map_line(idle_current_mark, 0x00);
                        idle_current_mark ++;
                        lcd_map_line(idle_current_mark, 0xFF);

//          SendString((U8 *)"\r\nidle_current_mark:idle_current_page_num=");
//          send_hex_u8(idle_current_mark);
//          SendString((U8 *)":");
//          send_hex_u8(idle_current_page_num);
                        g_old_select_appli = idle_current_mark;
                        break;
#else
                    case KEY_PREV:
                        if((idle_current_mark)==0)break;
                        if((idle_current_mark%IDLE_LINE_PAGE)!=0) {
                            idle_current_mark --;
                            idle_mark_display(idle_current_mark);
                        } else {
                            if(idle_current_page_num > 1) {
                                idle_current_page_num--;
                                idle_current_mark --;
                                idle_menu_display(idle_current_page_num);
                                idle_mark_display(idle_current_mark);
                            }
                        }
//          SendString((U8 *)"\r\nidle_current_mark:idle_current_page_num=");
//          send_hex_u8(idle_current_mark);
//          SendString((U8 *)":");
//          send_hex_u8(idle_current_page_num);
                        g_old_select_appli = idle_current_mark;
                        break;

                    case KEY_NEXT:
//            srvplayer_volume_change( SRVPLAYER_VOL_UP );
//            mmi_idle_display_volume();
                        if((idle_current_mark+1)==idlenum)break;
                        if((idle_current_mark%IDLE_LINE_PAGE)==0) {
                            idle_current_mark ++;
                            idle_mark_display(idle_current_mark);
                        } else {
                            if(idle_current_page_num<idle_page_num) {
                                idle_current_page_num++;
                                idle_current_mark ++;

                                idle_menu_display(idle_current_page_num);
                                idle_mark_display(idle_current_mark);
                            }
                        }
//          SendString((U8 *)"\r\nidle_current_mark:idle_current_page_num=");
//          send_hex_u8(idle_current_mark);
//          SendString((U8 *)":");
//          send_hex_u8(idle_current_page_num);
                        g_old_select_appli = idle_current_mark;
                        break;
#endif
                    case KEY_MODE:
                        if(b_Idle_ReadOid) mail_send_event( EVT_IDLE_INIT_OVER_MODE, 0 );
                        break;
                    case KEY_POWER:
                        //case KEY_VOL_UP:
//          g_old_select_appli = 4;
                        SDKLoadCode(EXIT_IDLE_CODE);
                        exit_idle();
                        break;

                    //case KEY_MEN:
                    case KEY_DUIBI:
                        //oid_display_enable = FALSE;
                        g_select_appli = g_old_select_appli = RECORDER_APPLI;
                        recorder_mode = 0;
                        //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
                        //Mmgr_kill_this_mmi_appli();
                        CommonLoadCode( C_REREAD);
                        break;
                    case KEY_GENDU:
                        //case KEY_FUDU:
                        //oid_display_enable = FALSE;
                        g_select_appli = g_old_select_appli = RECORDER_APPLI;
                        recorder_mode = 1;
                        //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 1 );
                        //Mmgr_kill_this_mmi_appli();
                        CommonLoadCode( C_REREAD );
                        break;
                }
            }
            break;

        /*case EVT_USB_DEVICE_MS_STARTED:
        // This event confirms that the command CMD_MSTORAGE_START,
        // posted in the mmi mass storage application, has succeeded.
        // This banked mmi application idle is killed to have only one banked mmi application launched.
        // This prevents the bank switching that make the USB tranfert bandwith collapse
        // when events not catched are forwarded
        Mmgr_kill_this_mmi_appli();
        // This event is forwarded to the mmi application status
        Mmgr_krn_forward_current_event();
        break;*/

        /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
        power_system_off( !power_is_vbus_present());
        break;*/

        case EVT_ENTER_IN_POWEROFF:
            g_power_off_mode = POWER_OFF;
            goto mmi_idle_power_off;

        case EVT_ALERT_BATTERY_LOW:
            g_power_off_mode = BATTERY_LOW;
        mmi_idle_power_off:
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
            //oid_display_enable = FALSE;
            g_select_appli = POWER_OFF_APPLI;
            //mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
            //Mmgr_kill_this_mmi_appli();
            CommonLoadCode( C_POWEROFF );
            break;

        case EVT_IDLE_DOWITH_OID:
            SendString((U8 *)"\r\nindex=");
            send_hex_u16(param);
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
            if( g_b_lcd_backlight_off ) {
                Lcd_rearm_backlight_on();
                g_mmi_lcd_backlight_off = FALSE;
                break;
            }
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )     
            SDKLoadCode(READ_BNL_CODE);
            if(0xFFFF != read_bnl_file(param)) {
                mail_send_event( EVT_IDLE_CHOICE_BOOK_MODE, 0 );
                /*
                g_b_xuanshu_flag = TRUE;
                bInEbookMode = FALSE;
                g_old_select_appli = AP3_APPLI;
                Mmgr_kill_this_mmi_appli();
                CommonLoadCode( C_AP4 );
                */
                return;
            }
            SDKLoadCode(SOME_INDEX_CODE);
            (void)some_index_dowith();
            break;
        case EVT_CHANGE_OID_TYPE:
            //SDKLoadCode(CHANGE_OIDTYPE_CODE);
            /*
            if(g_bluetooth){
                g_bluetooth = FALSE;
                }
            else{
                g_bluetooth = TRUE;
                }*/
#ifdef TX_DEBUG
            SendString(( U8 *)"\r\nCHANGE_OID");
#endif
            g_bluetooth = ~g_bluetooth;
            set_oid_type(g_bluetooth);
            lcd_goto_xy( OID_TYPE_FLAG_X, OID_TYPE_FLAG_Y);
            Ih_set_curr_font( IDC_FONT_08 );
            if(g_bluetooth) {
                lcd_puts(( U8 *)"OID2");
            } else {
                lcd_puts(( U8 *)"DPR3");
            }
            break;
        case EVT_IDLE_DISPLAY_OID:
            /*
            #if( LCD_PRESENT == ENABLE )
            #if(BACKLIGHT == ENABLE)
                  if( g_b_lcd_backlight_off )
                  {
                     Lcd_rearm_backlight_on();
                     g_mmi_lcd_backlight_off = FALSE;
                     break;
                  }
            #endif //#if(BACKLIGHT == ENABLE)
            #endif   //#if( LCD_PRESENT == ENABLE )
            */
            idle_display_oid( param );
            break;

#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        case EVT_REMOTE:
            switch( LSB( param )) {
                case REMOTE_VOL_UP:
                    srvplayer_volume_change( SRVPLAYER_VOL_UP );
                    mmi_idle_display_volume();
                    break;

                case REMOTE_VOL_DOWN:
                    srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                    mmi_idle_display_volume();
                    break;

                case REMOTE_VOL0:
                case REMOTE_VOL1:
                case REMOTE_VOL2:
                case REMOTE_VOL3:
                case REMOTE_VOL4:
                case REMOTE_VOL5:
                case REMOTE_VOL6:
                case REMOTE_VOL7:
                case REMOTE_VOL8:
                case REMOTE_VOL9:
                case REMOTE_VOL10:
                case REMOTE_VOL11:
                case REMOTE_VOL12:
                case REMOTE_VOL13:
                case REMOTE_VOL14:
                    if(( LSB( param )- REMOTE_VOL0 )>( SRVPLAYER_VOL_LEVEL - 1 ))
                        break;
                    g_volume = LSB( param )- REMOTE_VOL0;
                    srvplayer_volume_send();
                    mmi_idle_display_volume();
                    break;

                case REMOTE_POWER_OFF:
                    if( power_is_vbus_present())
                        break;
                    //g_b_mp3_remote_on = FALSE;
                    g_b_remote_on = FALSE;
                    //g_select_appli = IDLE_APPLI;
                    g_select_appli = POWER_OFF_APPLI;
                    g_power_off_mode = POWER_OFF;
                    //mmgr_activate_mmi_appli( MMI_APPLI_POWEROFF, g_power_off_mode );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_POWEROFF );
                    break;

                /*case REMOTE_EXIT_MP3:
                case REMOTE_MP3_PLAY_PAUSE:
                case REMOTE_MP3_PREV:
                case REMOTE_MP3_NEXT:
                case REMOTE_REC_START:
                case REMOTE_REC_PLAY:
                case REMOTE_REC_STOP:
                break;*/

                case REMOTE_CAL_MODE:
                    bRemoteEnterCal = TRUE;
                    g_select_appli = g_old_select_appli = AP3_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_AP3, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_AP4 );
                    break;

                case REMOTE_AP4_MODE:
                    //g_b_remote_on = FALSE;
                    bRemoteEnterCal = FALSE;
                    g_select_appli = g_old_select_appli = AP3_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_AP3, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_AP4 );
                    break;

#if( REMOTE_REC == ENABLE )
                case REMOTE_REC_MODE:
                    g_select_appli = g_old_select_appli = RECORDER_APPLI;
                    recorder_mode = 0;
                    //mmgr_activate_mmi_appli( MMI_APPLI_RECORDER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_RECORD );
                    break;
#endif   //#if( REMOTE_REC == ENABLE )

#if( REMOTE_MP3 == ENABLE )
                case REMOTE_MP3_MODE:
                    g_select_appli = g_old_select_appli = PLAYER_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_PLAYER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_PLAYER );
                    break;
#endif   //#if( REMOTE_MP3 == ENABLE )

#if( REMOTE_PASTER == ENABLE )
                case REMOTE_PASTER_MODE:
                    g_select_appli = g_old_select_appli = PASTER_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_PASTER, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_PASTER );
                    break;
#endif   //#if( REMOTE_PASTER == ENABLE )

#if( REMOTE_DIC == ENABLE )
                case REMOTE_DIC_MODE:
                    g_select_appli = g_old_select_appli = DIC_APPLI;
                    //mmgr_activate_mmi_appli( MMI_APPLI_DIC, 0 );
                    //Mmgr_kill_this_mmi_appli();
                    CommonLoadCode( C_DIC );
                    break;
#endif   //#if( REMOTE_DIC == ENABLE )
            }
            break;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

        default:
            Mmgr_krn_forward_current_event();
            break;
    }
}


#if( LCD_PRESENT == ENABLE )
//! idle_display
//!
//! Display the screen of the application IDLE
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param   U8 entire_draw: Draw the screen entirely or not
//!
//! @return none
/*
void idle_display( U8 entire_draw )
{
   if( TRUE == entire_draw )
   {
      lcd_turn_on();
      Lcd_start_backlight_timer();

      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
     // lcd_clear_screen();
      lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
   }
   //lcd_goto_xy( 28, 12 );
   switch( g_old_select_appli )
   {
      case IDLE_APPLI:
      case PLAYER_APPLI:
      case PASTER_APPLI:
      lcd_goto_xy( 40, 12 );
      break;

      case RECORDER_APPLI:
      lcd_goto_xy( 32, 12 );
      break;

#if( afaya_pen == ENABLE )
      case AP3_APPLI:
      lcd_goto_xy( 28, 12 );
      break;
#endif   //#if( afaya_pen == ENABLE )

      case DIC_APPLI:
      lcd_goto_xy( 24, 12 );
      break;

#if( i_speak == ENABLE )
      case AP3_APPLI:
#endif   //#if( i_speak == ENABLE )
      case SETTING_APPLI:
      lcd_goto_xy( 36, 12 );
      break;
   }
   Ih_set_curr_font( IDC_FONT_16 );
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      lcd_puts(( U8 *)s_main_menu_english[ g_old_select_appli ]);
      break;

      case LANG_CHINESE_SIMP:
      lcd_puts(( U8 *)s_main_menu_chinese_simp[ g_old_select_appli ]);
      break;

      case LANG_CHINESE_TRAD:
   //   lcd_puts(( U8 *)s_setting_name_chinese_trad[ MenuIndex - 1 ]);
      break;
   }

   lcd_goto_xy( 52, 0 );
   Ih_set_curr_font( IDC_FONT_08 );
   lcd_puts(( U8 *)"IDLE" );
   status_display();

   lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
   mmi_idle_display_volume();
}
*/

//! @brief Display volume
//!
void mmi_idle_display_volume( void )
{
#if 1
    _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

    Ih_set_curr_font( IDC_FONT_08 );
    lcd_goto_xy( 6, 0 );
    //sprintf( string , "%02bu", srvplayer_g_arg.volume );
    memset( string, 0, 8 );

    convert_U8(( char *)&string[ 4 ], g_volume );
    i = strlen(( char *)&string[ 4 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 4 ] );
    lcd_puts( string );
#else
    _MEM_TYPE_SLOW_ U8 string[ 8 ], i;

    Ih_set_curr_font( IDC_FONT_08 );
    lcd_goto_xy( 10, 0 );
    //sprintf( string , "%02bu", srvplayer_g_arg.volume );
    memset( string, 0, 8 );
    convert_U8(( char *)&string[ 4 ], g_volume );
    i = strlen(( char *)&string[ 4 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 4 ] );
    lcd_puts( string );
#endif
}

//! @brief Display oid index
//!
void idle_display_oid( U16 index )
{
#if 1
    SDKLoadCode(INDEX_DISPLAY_CODE);
    wireless_index_display(index);
#else
    _MEM_TYPE_SLOW_ U8 string[ 13 + 6 ], i;

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT - 8));
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 0, 12 );
    lcd_puts(( U8 *)"oid:" );

    //sprintf( string , "%5u 0x%04X", index, index );
    memset( string, 0, 19 );
    convert_U16(( char *)&string[ 13 ], index );
    i = strlen(( char *)&string[ 13 ] );
    for( ; i < 5; i++ )
        strcat(( char *)string, " " );
    strcat(( char *)string, ( char *)&string[ 13 ] );

    strcat(( char *)string, " 0x" );
    memset( &string[ 13 ], 0, 6 );
    string[ 13 ] = conv_hex(( index & 0xF000 )>> 12 );
    string[ 14 ] = conv_hex(( index & 0x0F00 )>> 8 );
    string[ 15 ] = conv_hex(( index & 0x00F0 )>> 4 );
    string[ 16 ] = conv_hex(( index & 0x000F ));
    strcat(( char *)string, ( char *)&string[ 13 ] );
    lcd_puts( string );
#endif
}


char *convert_U8( char *p, U8 i )
{
    if( i / 10 )
        p = convert_U8( p, i / 10 );
    //else if( i < 0 )
    //   *p++ = '-';
    return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}

#if 1


/*
U8 conv_hex( U8 v )
{
   if( v < 10 )
   {
      return v + '0';
   }
   else
   {
      return v - 10 + 'A';
   }
}
*/
#endif
/*
void idle_menu_display(U16 display_num_mark)
{
    U16 i,j,temp_file;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   temp_file = (display_num_mark-1)*IDLE_LINE_PAGE;
   if((idlenum - temp_file) >= IDLE_LINE_PAGE ){
    j = IDLE_LINE_PAGE;
    }
   else{
    j = IDLE_LINE_PAGE-1;
    }
  for(i=0;i<j;i++ ){
      lcd_goto_xy( 32, 28+i*20 );
//    lcd_puts( ( U8 *)s_main_menu_chinese_simp[temp_file ] );
     switch( g_setting_list.struct_setting.language_ihm )
       {
          case LANG_ENGLISH:
          lcd_puts(( U8 *)s_main_menu_english[ temp_file ]);
          break;

          case LANG_CHINESE_SIMP:
          lcd_puts(( U8 *)s_main_menu_chinese_simp[ temp_file ]);
          break;
       }
      temp_file ++;
    }
}

void idle_mark_display(U8 mark)
{
    U16 i,temp_file;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 28, 30, (LCD_HEIGHT-28));
    lcd_draw_picture(12,32+20*(mark%IDLE_LINE_PAGE), IDP_PLAYER_PLAY);
}
*/

Bool global_key_deal(U16 param)
{
    if(param == ((KEY_REC<<8)|KBD_KEY_PRESSED)) {
        g_select_appli = g_old_select_appli = PLAYER_APPLI;//DIC_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_PLAYER);
        return TRUE;
    }
    return FALSE;
}

void lcd_map_line(U8 app,U8 write_data)
{
    U8 i,j,x0,y0,row_number,column_number;
    i = app<<1;
    if( g_setting_list.struct_setting.language_ihm == LANG_ENGLISH ) {
        x0 = point_en_array[i];
        y0 = point_en_array[i+1];
    } else {
        x0 = point_ch_array[i];
        y0 = point_ch_array[i+1];
    }


    lcd_write_command(0x40);

    lcd_write_command(0xB0);
    lcd_write_command(y0);
    for(column_number=x0; column_number<(x0+WIDTH_MENU); column_number++) {

        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(write_data);
    }

    lcd_write_command(0xB0);
    lcd_write_command(y0+HEIGH_MENU);
    for(column_number=x0; column_number<(x0+WIDTH_MENU); column_number++) {

        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(write_data);
    }


    for(row_number=y0; row_number<(y0+HEIGH_MENU); row_number++) {
        lcd_write_command(0xB0);
        lcd_write_command(row_number);
        column_number = x0;
        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(write_data);
    }

    for(row_number=y0; row_number<(y0+HEIGH_MENU); row_number++) {
        lcd_write_command(0xB0);
        lcd_write_command(row_number);
        column_number = x0+WIDTH_MENU;
        lcd_write_command(0x10|((column_number/16)));
        lcd_write_command(0x00|((column_number%16)));
        Lcd_write_display_data(write_data);
    }
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
    if( !((*p >= '0' )&&( *p <= '9'))) {
        //bRemoteChoiceAp3 = TRUE;
        return 0xFFFF;
    }
    while((*p >= '0' )&&( *p <= '9')) {
        iNameLead = iNameLead * 10 + *p - '0';
        p++;
    }
    //SendString((U8 *)"\r\nnamelead=");
    //send_hex_u16(iNameLead);
    return iNameLead;
}

#pragma CODE = CHECK_UPDATE_CODE

void update_check(void)
{
    U8 i;
    if(point_B != 0x55AA)return ;
    if(point_A == 0x5555) {
#ifdef TX_DEBUG
        SendString((U8 *)"\r\nupgrade after!");
#endif
        lcd_goto_xy( 58, 24 );
        lcd_puts(( U8 *)"OK" );
        for( i = 0; i < 40; i++ ) {
            //red_led_flash();
            delay1ms( 200 );
        }
        CommonLoadCode( C_BOOT );
    }
    if(point_A == 0xAAAA) {

        lcd_goto_xy( 58, 24 );
        lcd_puts(( U8 *)"KO" );
        for( i = 0; i < 40; i++ ) {
            //red_led_flash();
            delay1ms( 200 );
        }
        CommonLoadCode( C_BOOT );
    }
    return ;
}

#pragma CODE = READ_BNL_CODE

U16 read_bnl_file(U16 off_t)
{
    U16 offset,bid = 0;
    U32 temp_off;
    //char tmpbuf[32];
    char rbuf[4];
    __X FCFILE *REC_File;
    __X FS_ERR err;

    U16 temp_min,temp_max,book_off;
#if 0//(REREAD_MODE == ENABLE)   
    if(Current_Index == NORMAL_MODE_INDEX){
        return 0XFFFF;
        }
    if(Current_Index == REREAD_MODE_INDEX){
        return 0xFFFF;
        }
#endif    //(REREAD_MODE == ENABLE)    
#if 0
    if(g_setting_list.struct_setting.bluetooth) {
        temp_min = DPR3_XUANSHU_MIN;
        temp_max = DPR3_XUANSHU_MAX;
    } else {

        temp_min = OID2_XUANSHU_MIN;
        temp_max = OID2_XUANSHU_MAX;
    }
#else
    if(g_setting_list.struct_setting.bluetooth) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\r\noid2!");
#endif
        temp_min = OID2_XUANSHU_MIN;
        temp_max = OID2_XUANSHU_MAX;
        //book_off = OID2_XUANSHU_OFFSET;
    } else {
#ifdef TX_DEBUG
        SendString(( U8 *)"\r\ndpr3!");
#endif
        temp_min = DPR3_XUANSHU_MIN;
        temp_max = DPR3_XUANSHU_MAX;
        //book_off = DPR3_XUANSHU_OFFSET;
    }
#endif
    //temp_min = OID2_XUANSHU_MIN;
    //temp_max = OID2_XUANSHU_MAX;
    if((off_t < temp_min)||(off_t > temp_max)) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\n\rout range!");
#endif
        return 0XFFFF;
    }

    g_xuanshu_num = off_t;
    return off_t;
    /*
        off_t -= book_off;
        //off_t += 1;
        //off_t = temp_min;
        REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_READ_WRITE,&err);
        if(err != FS_NO_ERROR){
    #ifdef TX_DEBUG
        SendString(( U8 *)"\n\ropen fail!");
    #endif
            return 0XFFFF;
            }



        temp_off = (U32)(off_t<<1);
        FSSeek(REC_File, temp_off);
        FSRead(REC_File, rbuf, sizeof(rbuf), &err);
        //FSClose(Temp_File);
        MSB(bid) = rbuf[0x00];
        LSB(bid) = rbuf[0x01];
        FSClose(REC_File);
    #ifdef TX_DEBUG
        SendString(( U8 *)"\r\noff:rid=");
        send_hex_u32(temp_off);SendString(( U8 *)":");
        send_hex_u16(bid);
    #endif
        g_xuanshu_num = bid;
        return bid;
    */
}


#pragma CODE = SOME_INDEX_CODE

Bool some_index_dowith( void )
{
    U16 dic_temp,mp3_temp,ap4_temp,v_adc,v_dec,v_min,v_max,diy_min,diy_max;
    if(g_setting_list.struct_setting.bluetooth) {
        dic_temp = OID2_GO_DIC;
        mp3_temp = OID2_GO_MP3;
        ap4_temp = OID2_GO_AP4;
        v_adc    = OID2_V_ADC_INDEX;
        v_dec    = OID2_V_DEC_INDEX;
        v_min    = OID2_V_1_INDEX;
        v_max    = OID2_V_7_INDEX;
        diy_min  = OID2_DIY_RECORD_MIN;
        diy_max  = OID2_DIY_RECORD_MAX;
    } else {
        dic_temp = DPR3_GO_DIC;
        mp3_temp = DPR3_GO_MP3;
        ap4_temp = DPR3_GO_AP4;
        v_adc    = DPR3_V_ADC_INDEX;
        v_dec    = DPR3_V_DEC_INDEX;
        v_min    = DPR3_V_1_INDEX;
        v_max    = DPR3_V_7_INDEX;
        diy_min  = DPR3_DIY_RECORD_MIN;
        diy_max  = DPR3_DIY_PLAY_MAX;
    }
#if (DIY_REC == ENABLE)
    if((Current_Index >= diy_min)&&(Current_Index <= diy_max)) {
        reread_num = Current_Index;
        g_select_appli = g_old_select_appli = REREAD_APPLI;//DIC_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_REREAD);
        return TRUE;
    }
#endif
    if(Current_Index == dic_temp) {
        g_select_appli = g_old_select_appli = DIC_APPLI;//DIC_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_DIC);
        return TRUE;
    }
    if(Current_Index == mp3_temp) {
        g_select_appli = g_old_select_appli = PLAYER_APPLI;//MP3_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_PLAYER);
        return TRUE;
    }
    if(Current_Index == ap4_temp) {
        g_select_appli = g_old_select_appli = AP3_APPLI;//AP3_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_AP4);
        return TRUE;
    }
    if(Current_Index == v_adc) {
        //mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_UP<<8)|KBD_KEY_RELEASED));
        srvplayer_volume_change( SRVPLAYER_VOL_UP);
        mmi_idle_display_volume();
    }
    if(Current_Index == v_dec) {
        //mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED));
        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        mmi_idle_display_volume();
    }
    if((Current_Index >= v_min)&&(Current_Index <= v_max)) {
        g_volume = Current_Index- v_min;
        srvplayer_volume_send();
        mmi_idle_display_volume();
        return TRUE;
    }
    if(Current_Index == CHANGE_OID_TYPE) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\r\nEVT_CHANGE_OID_TYPE");
#endif
        mail_send_event( EVT_CHANGE_OID_TYPE, 0 );
        return TRUE;
    }
    return FALSE;
}



#pragma CODE = INIT_IDLE_CODE
const char *s_menu_nowifi_english[ ] = {
    " AfayaPen  StudyText Mp3Player"
    ,"Dictionary  Setting   "
};

const char *s_menu_nowifi_chinese_simp[ ] = {
    " 点 读 笔   学 习 机  MP3播放"
    ," 点读词典   系统设置 "
};
const char *s_menu_wifi_english[ ] = {
    " AfayaPen  StudyText  Recorder"
    ,"Mp3Player     Dict    Setting  "
};

#if( WIRELESS == ENABLE)
const char *s_menu_wifi_chinese_simp[ ] = {
    " 点 读 笔   学 习 机  录 音 笔 "
    ," MP3播 放   点读词典  系统设置"
};
#endif
#if( BLUETOOTH == ENABLE)
const char *s_menu_wifi_chinese_simp[ ] = {
    " 点 读 机   课本学习  录音/MP3"
    ," 电子词典   系统设置  蓝牙模组"
};
#endif

void init_idle(void)
{
    U8 i,j;
    lcd_turn_on();
    Lcd_start_backlight_timer();

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//      lcd_clear_screen();
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
    lcd_clear_part_of_screen(20, 0,98, 8);

    lcd_draw_picture( 0, 0, IDP_PLAYER_VOLUME );
    mmi_idle_display_volume();
    lcd_goto_xy( OID_TYPE_FLAG_X, OID_TYPE_FLAG_X );
    if(g_bluetooth) {
        lcd_puts(( U8 *)"OID2");
    } else {
        lcd_puts(( U8 *)"DPR3");
    }
    b_flag_rtc = FALSE;
    SDKLoadCode(POWER_TASK_TEMP_CODE05);
    lcd_display_time(FALSE);
    lcd_goto_xy( 0, 8 );
    Ih_set_curr_font( IDC_FONT_16 );
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Menu" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"主菜单" );
            break;
    }

//      g_setting_list.struct_setting.bluetooth = 1;
    for(i=0; i<2; i++) {
        lcd_goto_xy( 2, 28+i*19 );
        switch( g_setting_list.struct_setting.language_ihm ) {
            case LANG_ENGLISH:
#if 1
                lcd_puts(( U8 *)s_menu_wifi_english[ i ]);
#else
                if(g_setting_list.struct_setting.bluetooth) {
                    lcd_puts(( U8 *)s_menu_wifi_english[ i ]);
                } else {
                    lcd_puts(( U8 *)s_menu_nowifi_english[ i ]);
                }
#endif
                break;

            case LANG_CHINESE_SIMP:
#if 1
                lcd_puts(( U8 *)s_menu_wifi_chinese_simp[ i ]);
#else
                if(g_setting_list.struct_setting.bluetooth) {
                    lcd_puts(( U8 *)s_menu_wifi_chinese_simp[ i ]);
                } else {
                    lcd_puts(( U8 *)s_menu_nowifi_chinese_simp[ i ]);
                }
#endif
                break;
        }
    }
#if 1
    idlenum = 6;
#else
    if(g_setting_list.struct_setting.bluetooth) {
        idlenum = 6;
    } else {
        idlenum = 5;
    }
#endif
    idle_current_mark = 0;
    lcd_map_line(idle_current_mark, 0xFF);
    /*
          idlenum = NB_MAIN_MENU_CHINESE_SIMP;
          if(g_setting_list.struct_setting.bluetooth)idlenum += 1;
          idle_current_mark = 0;
          idle_current_page_num = 1;
          idle_page_num = (idlenum+1)/2;

          SendString((U8 *)"\r\nidle_page_num=");
          send_hex_u8(idle_page_num);

    //    display_menu();
    //     disp_Menu( g_setting_list.struct_setting.poweroff_value );

          idle_menu_display(idle_current_page_num);
          idle_mark_display(idle_current_mark);
    */

    status_display();
    g_select_appli = g_old_select_appli = idle_current_mark;
}


#pragma CODE = FIND_DIR_FILE_01_CODE
//the function is find the number of ap3 in the root;
Bool find_ap4_dir( void )
{
    // U8 AP4_DIR_NAME[] = {0x70B9,0x8B,0xFB,0x65,0x87,0x4E,0xF6,0x00};
    U16 AP4_DIR_NAME[] = {0x70B9,0x8BFB,0x6587,0x4EF6,0x0000};//点读文件
    SDKLoadCode( NAVIGATION_TEMP_CODE05 );
    file_init();
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( NAVIGATION_TEMP_CODE00 );
    if( !nav_dir_root())
        return FALSE;

#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
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
#if 1
            goto find_ap4_dir_error;
#else
            SDKLoadCode( NAVIGATION_TEMP_CODE02 );
            if( !nav_dir_make(( U8 *)RECORD_DIR ) ) {
                if( FS_ERR_FILE_EXIST != fs_g_status ) {
                    SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                    ( void )nav_file_del( TRUE );
                    goto find_rec_file_error;
                }
            }
#endif
        }
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
    nav_string_unicode();
#endif

    SDKLoadCode( NAVIGATION_TEMP_CODE01 );
    if( !nav_dir_cd() )
        return FALSE;
    return TRUE;
find_ap4_dir_error:
#if( (FS_ASCII == ENABLED) && (FS_UNICODE == ENABLED) )
    nav_string_unicode();
#endif
    return FALSE;
}


#pragma CODE = FIND_DIR_FILE_02_CODE
//the function is find the number of ap3 in the root;
void look_ap4_file( void )
{
    U16 temp_data;

    SDKLoadCode( NAVIGATION_CODE );
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( !nav_file_isdir()) {
            if( nav_file_checkext( (U8 *)AP4_EXT)) {

                ap4num_diandu ++;
                //strcpy(( char *)( g_string ),(char *)fat_g_cur_nav->name);
                //temp_data = get_name_lead();
                //write_readbnl_file((U32)(temp_data<<1),ap4num_diandu);
            }
        } else {
            if( fat_g_cur_nav->name[ 0 ] != '.' ) {

                dirnum_diandu ++;
            }
        }
    }
    if(gStudyAp4 >= ap4num_diandu){
        gStudyAp4 = 0;
        gStudyUnite = 0;
    }
#if 1
    SendString((U8 *)"\r\ndir:file:studyFile=");
    send_hex_u16(dirnum_diandu);
    SendString((U8 *)":");
    send_hex_u16(ap4num_diandu);
     SendString((U8 *)":");
    send_hex_u16(gStudyAp4);
    SendString((U8 *)":");
    send_hex_u16(gStudyUnite);
#endif
}
/*
Bool Get_Dir_Name( U16 Number )
{
   U16 _MEM_TYPE_SLOW_ Number_Temp = 0;
   ( void )nav_filelist_reset();
   while( nav_filelist_set( 0, FS_FIND_NEXT ))
   {
      if( nav_file_isdir())
      {
          if( fat_g_cur_nav->name[ 0 ] != '.' ){
             Number_Temp++;
             if( Number == Number_Temp )
             {
                return nav_file_name( Unicode_get_addr_string( g_string ), Unicode_get_char_space( g_string ), FS_NAME_GET, FALSE );
             }
            }
      }
   }
   return FALSE;
}
*/
#endif  //#if( LCD_PRESENT == ENABLE )


///*
void write_readbnl_file(U32 offset,U16 data)
{

    __X FS_ERR err;
    __X U16 temp_data;
    __X FCFILE *REC_File;
#if 0//def TX_DEBUG
    SendString(( U8 *)"\n\rwrite_off:data=");
    send_hex_u32(offset);
    SendString(( U8 *)":");
    send_hex_u16(data);
#endif

    temp_data = data;
    REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_READ_WRITE,&err);
    (void)FSSeek(REC_File,offset);
    (void)FSWrite(REC_File, (void *)&temp_data,2, &err);
    FSClose(REC_File);
}

#pragma CODE = FIND_INDEX_BOOK_CODE

Bool find_index_book(U16 shu_index)
{
    U16 temp_data;
    U16 temp_book = 0;
    SDKLoadCode(FIND_DIR_FILE_01_CODE);
    (void)find_ap4_dir();
    SDKLoadCode( NAVIGATION_CODE );
    ( void )nav_filelist_reset();
    while( nav_filelist_set( 0, FS_FIND_NEXT )) {
        if( !nav_file_isdir()) {
            if( nav_file_checkext( (U8 *)AP4_EXT)) {

                //ap4num_diandu ++;
                temp_book ++;
                strcpy(( char *)( g_string ),(char *)fat_g_cur_nav->name);
                temp_data = get_name_lead();
                if( temp_data == shu_index) {
                    g_xuanshu_num = temp_book;
                    SendString(( U8 *)"\n\rd=");
                    send_hex_u16(temp_book);
                    return TRUE;
                }
                //write_readbnl_file((U32)(temp_data<<1),ap4num_diandu);
            }
        }
    }
    return FALSE;
#if 0
    SendString((U8 *)"\r\ndir:file=");
    send_hex_u16(dirnum_diandu);
    SendString((U8 *)":");
    send_hex_u16(ap4num_diandu);
#endif

}
Bool judge_shu(void)
{
    U16 temp_index,temp_data;

    temp_data = g_xuanshu_num;
    if(g_setting_list.struct_setting.bluetooth) {
        SDKLoadCode(CHANGE_SHU_CODE);
        temp_index = chang_shu_num((temp_data - OID2_XUANSHU_OFFSET));
        if(0xFFFF == temp_index)return FALSE;
        //book_off = OID2_XUANSHU_OFFSET;
    } else {
        temp_index = temp_data - DPR3_XUANSHU_OFFSET;
    }
    SendString(( U8 *)"\r\nn=");
    send_hex_u16(temp_index);
    if(find_index_book(temp_index)) {
        //bInEbookMode = FALSE;
        g_b_xuanshu_flag = TRUE;
        g_old_select_appli = AP3_APPLI;
        CommonLoadCode( C_AP4 );
        return TRUE;
    }
    return FALSE;
}

#pragma CODE = NOFINDBOOK_CODE

void no_find_xdf(void)
{
    g_b_xuanshu_flag = FALSE;
    lcd_turn_on();
    Lcd_start_backlight_timer();
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    //      lcd_clear_screen();
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
    lcd_clear_part_of_screen(20, 0,98, 8);
    lcd_goto_xy( 12, 32 );
    Ih_set_curr_font( IDC_FONT_16 );
    if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP) {
        lcd_puts((U8 *)"笔中无本书内容");
    } else {
        //lcd_puts((U8 *)"No Unit");
        lcd_puts((U8 *)"No display content");
    }
    scan_oid_disable();
    Kbd_set_evt_disabled();
    //delay1ms(5000);

}
#pragma CODE = CHANGE_SHU_CODE
U16 chang_shu_num(U16 yuan_index)
{
    SPIF_ERR SPI_STATE;
    U32 temp_sector;
    U16 temp_off,all_off = yuan_index;


    U16 *p = (U16 *)&Buf;

    temp_off = all_off%256;
    GetDataIdInfo( D_WYSRMT, &Start_Sector, &Length );
    temp_sector = Start_Sector +(U32)(all_off >> 8);
    SPI_STATE = spif_resv_read( temp_sector, ( __X void *)Buf );
    /*
    SendString((U8 *)"\r\nsec:off=");
    send_hex_u32(temp_sector);
    SendString((U8 *)":");
    send_hex_u32(temp_off);*/
    if( SPI_STATE != SDTL_No_Err ) return 0xFFFF;
    return p[temp_off];
}


#pragma CODE = CREAT_BOOK_CODE
void create_readbnl_file(void)
{

    __X FS_ERR err;
    U32 temp_len,need_len=128UL*1024;
    __X FCFILE *REC_File;

    temp_len = FSGetFileSize((char *)BOOK_SYS_FILE, &err);
    if(temp_len >= need_len) {
        REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_READ_WRITE,&err);
    } else {
        REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_CREATE,&err);
    }
    FSResize(REC_File,128UL*1024);
//      SDKLoadCode(KB_RESIZE_CODE);
//      err = kb_fat_resize(REC_File, 40UL*1024);
    FSClose(REC_File);
    FSSetAttribute((char *)BOOK_SYS_FILE,FS_ATTR_HIDDEN);
}

void clear_bnl(void)
{
    __X FS_ERR err;
    __X U16 temp_data;
    //U16 i;
    U32 i;
    __X FCFILE *REC_File;
    // U32 off_set;
    char temp_buf[512];
    memset(temp_buf,0xFF,sizeof(temp_buf));
    REC_File = FSOpen((char *)BOOK_SYS_FILE,FS_MODE_READ_WRITE,&err);
    for(i=0; i<256; i++) {
        //off_set = (unsigned long)(i<<9);
#if 0//def TX_DEBUG
        SendString(( U8 *)"\n\roff_set=");
        send_hex_u32(off_set);
#endif
        (void)FSSeek(REC_File,(unsigned long)(i<<9));
        (void)FSWrite(REC_File, (void *)&temp_buf,0x200, &err);
    }
    FSClose(REC_File);
}





#pragma CODE = INDEX_DISPLAY_CODE

char *convert_U16( char *p, U16 i )
{
    if( i / 10 )
        p = convert_U16( p, i / 10 );
    //else if( i < 0 )
    //   *p++ = '-';
    return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}

//! @brief Draw the background
//!
void wireless_index_display(U16 temp_data)
{
    _MEM_TYPE_SLOW_ U8 string[ 7 ], temp_string[6],i;
    char p;
    U16   temp_bak = temp_data;

    if(Current_Index == CHANGE_OID_TYPE) {
        mail_send_event( EVT_CHANGE_OID_TYPE, 0 );
    }
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//   lcd_clear_screen();
    lcd_clear_part_of_screen(0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
    lcd_goto_xy( 0, 8 );
    Ih_set_curr_font( IDC_FONT_16 );
    if(g_setting_list.struct_setting.bluetooth) {
        lcd_puts(( U8 *)"OID2:" );
    } else {
        lcd_puts(( U8 *)"DPR3:" );
    }

    memset( string, 0, 7 );
    strcat(( char *)string, "0X" );

    memset( temp_string, 0, 6 );
    temp_string[1] = '\0';
    /*
    convert_U16(( char *)&temp_string[ 0 ], temp_data);
    i = strlen(( char *)&temp_string[ 0 ] );
    for( ; i < 5; i++ )
       strcat(( char *)string, "0" );
     strcat(( char *)string, ( char *)&temp_string[ 0 ] );
    string[6] = '\0';
    */
    i = temp_data>>12;
//   SendString((U8 *)"\r\ni=");
    if((i%16)>9) {
        temp_string[0] = (i%16) - 10 + 'A';
    } else {
        temp_string[0] = (i%16) + '0';
    }
//   SendString((U8 *)&temp_string);
//   SendString((U8 *)"\r\n");
    strcat(( char *)string, (char *)temp_string );

    i = (U8)(temp_data>>8);
    i = i&0x0F;
    if((i%16)>9) {
        temp_string[0] = (i%16) - 10 + 'A';
    } else {
        temp_string[0] = (i%16) + '0';
    }
//  SendString((U8 *)&temp_string);
//  SendString((U8 *)"\r\n");
    strcat(( char *)string, (char *)&temp_string  );
    i = (U8)(temp_data>>4);
    i = i&0x0F;
    if((i%16)>9) {
        temp_string[0] = (i%16) - 10 + 'A';
    } else {
        temp_string[0] = (i%16) + '0';
    }
//   SendString((U8 *)&temp_string);
//  SendString((U8 *)"\r\n");
    strcat(( char *)string, (char *)&temp_string  );
    i = (U8)temp_data;
    i = i&0x0F;
    if((i%16)>9) {
        temp_string[0] = (i%16) - 10 + 'A';
    } else {
        temp_string[0] = (i%16) + '0';
    }
//   SendString((U8 *)&temp_string);
//   SendString((U8 *)"\r\n");
    strcat(( char *)string, (char *)&temp_string  );
//   strcat(( char *)string, (char *)&temp_data);
    string[6] = '\0';
//  strcat(( char *)string, (char)temp_data );
//   strcat(( char *)string, NULL );

    lcd_goto_xy( 10, 26 );
    lcd_puts( (U8*)"Hex:" );
    lcd_goto_xy( 26, 26 );
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_puts( string );

    //temp_data = temp_bak;
    // SendString((U8 *)"\r\ntemp_data = ");
    //send_hex_u16(temp_data);
    memset( string, 0, 7 );
    memset( temp_string, 0, 6 );
    convert_U16(( char *)&temp_string[ 0 ], temp_bak);
    i = strlen(( char *)&temp_string[ 0 ] );
    // SendString((U8 *)"\r\ntemp_string = ");
    // SendString(temp_string);
    for( ; i < 5; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&temp_string[ 0 ] );
    string[6] = '\0';

    //SendString((U8 *)"\r\nstring = ");
    //SendString(string);
    lcd_goto_xy( 10, 44 );
    lcd_puts( (U8*)"Dec:" );
    lcd_goto_xy( 26, 44 );
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_puts( string );

    // SendString((U8 *)"\r\ndisplay index ok!");

}

#pragma CODE = EXIT_IDLE_CODE

void exit_idle(void)
{
    SendString((U8 *)"\r\ng_old_select_appli=");
    send_hex_u8(g_old_select_appli);
    if(g_old_select_appli == 0) {
        g_select_appli = g_old_select_appli = AP3_APPLI;//PLAYER_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_AP4);
        return;
    }
    if(g_old_select_appli == 1) {
        g_select_appli = g_old_select_appli = STUDY_APPLI;//AP3_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_STUDY);
        return;
    }
    if(g_old_select_appli == 2) {
        g_select_appli = g_old_select_appli = RECORDER_APPLI;//;PLAYER_APPLI
        Mmgr_kill_this_mmi_appli();
        recorder_mode = 0;
        CommonLoadCode(C_RECORD);
        return;
    }
    if(g_old_select_appli == 3) {
        g_select_appli = g_old_select_appli = PLAYER_APPLI;//DIC_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_PLAYER);
        return;
    }
    if(g_old_select_appli == 4) {
        g_select_appli = g_old_select_appli = DIC_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_DIC);
        return;
    }
    if(g_old_select_appli == 5) {
        g_select_appli = g_old_select_appli = SETTING_APPLI;
        Mmgr_kill_this_mmi_appli();
        CommonLoadCode(C_SETTING);
        return;
    }
}

#pragma CODE = MMI_IDLE_CODE
Bool no_find_xdf_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    // Search file to play
    if( !search_subdir_mp3_file( "notfindxdf.mp3" ))
        return FALSE;

    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file())
        return FALSE;
    return TRUE;
}

#pragma CODE = READ_CAB_CODE
void read_set_calib( void )
{
    __X U16 TempBuf[ 256 ], i;
    __X U32 DataStartSector;
    __X U32 DataLength;

    GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 1//def SPIBOOT_KBDM9
    spif_resv_read( DataStartSector, TempBuf );
#endif

    if( TempBuf[( DataLength >> 1 )- 1 ] != 0xA55A ) {
        //RESET_SYS();
        RTCWDT = 0xBFFFu;
    }

    t01_calib_data1  = TempBuf[ 0 ];
    t01_calib_data2  = TempBuf[ 1 ];


    //MSW( t01_calib_data1 ) = TempBuf[ 0 ];
    //LSW( t01_calib_data1 ) = TempBuf[ 1 ];
    //MSW( t01_calib_data2 ) = TempBuf[ 2 ];
    //LSW( t01_calib_data2 ) = TempBuf[ 3 ];
    SendString((U8 *)"\r\ncal=");
    send_hex_u16(t01_calib_data1);
    SendString((U8 *)":");
    send_hex_u16(t01_calib_data2);
#if 0
    write_t01_cmd( UserCmd_SetCal1, TempBuf[ 0 ], TempBuf[ 1 ]);
    DataStartSector = Timer_set_timer( TIMER_MS( 300 ));
    while( read_sdio()) {
        if( Timer_is_timeout( DataStartSector )) {
            SendString((U8 *)"set cal1 err\r\n");
            return;
        }
    }
    DataStartSector = read_oid_pen();
    SendString((U8 *)"cal1=");
    send_hex_u32(DataStartSector);
    SendString((U8 *)"\r\n");

    write_t01_cmd( UserCmd_SetCal2, TempBuf[ 2 ], TempBuf[ 3 ]);
    DataStartSector = Timer_set_timer( TIMER_MS( 300 ));
    while( read_sdio()) {
        if( Timer_is_timeout( DataStartSector )) {
            SendString((U8 *)"set cal2 err\r\n");
            return;
        }
    }
    DataStartSector = read_oid_pen();
    SendString((U8 *)"cal2=");
    send_hex_u32(DataStartSector);
    SendString((U8 *)"\r\n");
#endif
}

#endif // _mmi_idle_c_
