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


#ifndef _search_c_
#define _search_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include <stdio.h>

#include "search.h"
#include "search_game.h"
#include "search_remote.h"
#include "modules\player_ap3\player_ap3.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
#include "lib_system\unicode\unicode.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"

#include "sio\com.h"
#include "conf\conf_oid.h"
#include "mmi\ap3\mmi_ap3.h"
#include "string.h"
#include "mmi\player\srv_player.h"
#include "modules\search\search_task.h"
#include "mmi\player\mmi_player.h"
//#include "stdlib.h" // Contains Funktion "rand()"
#include "conf\conf_sio.h"
#include "lib_mcu\timer\timer_drv.h"
#include "drivers\oid\oid.h"
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"

#include "modules\power\power.h"             // Power module definition
//#include "SSSMP3_Regs.h"
#include "mmi\player\mmi_player.h"
#include "modules\player\player_task.h"
#include "modules\power\power_task.h"
#include "lib_mcu\int\int_drv.h"
#include "mmi\startup\mmi_startup.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "lib_mcu\lcd\lcd_drv.h"
#include "mmi\images\tab_pict.h"
#include "mmi\status\mmi_status.h"
#include "mmi\recorder\srv_recorder.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "mmi\kbd\keypad.h"


#pragma DATA = SEARCH_DATA
#pragma CODE = SEARCH_CODE

ImageDeclareType( FIND_BOOKSYS_CODE );
ImageDeclareType( CHANGE_OID_CODE );
ImageDeclareType( SOME_INDEX_CODE );
ImageDeclareType( ACTION_TEMP_CODE08 );

ImageDeclareType(JUDGE_REREAD_CODE);

#if( AP3 == ENABLE )
//extern   xdata    AL_Variables        varAl;

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


//_____ D E F I N I T I O N ________________________________________________

_MEM_TYPE_SLOW_  Action          Current_Act;
_MEM_TYPE_SLOW_  U8              Point_Write /*= 0*/;
_MEM_TYPE_SLOW_  U8              Point_Read /*= 0*/;
//_MEM_TYPE_SLOW_  Speech          Array_V[ Length_v ];
__Y  Speech                     Array_V[ Length_v ];
_MEM_TYPE_SLOW_  U8              repeat_point /*= 0*/;

_MEM_TYPE_SLOW_  U16             search_last_oid_index;
_MEM_TYPE_SLOW_  U16             display_word_length;

_MEM_TYPE_SLOW_  U32             action_address;
Bool            act_do_flag;
_MEM_TYPE_SLOW_  U32             findword_address;
_MEM_TYPE_SLOW_  U32             firstword_address;

_MEM_TYPE_SLOW_  U16              leng_en;//原文英文长度
_MEM_TYPE_SLOW_  U16              leng_ch;//原文中文长度

_MEM_TYPE_SLOW_  U16             Display_Page_Num;
_MEM_TYPE_SLOW_  U16             current_display_num;
_MEM_TYPE_SLOW_  U16             current_display_num_bak;
_MEM_TYPE_SLOW_  U32             ch_begin_address;
_MEM_TYPE_SLOW_  U32             ch_display_address;
_MEM_TYPE_SLOW_  U32             ch_display_address_bak;
_MEM_TYPE_SLOW_  U32             word_mp3_address;
//                 Bool            b_play_repeat = FALSE;
//                 Bool            b_remote_play_repeat = FALSE;

//                 Bool            reread_enable_flag = FALSE;

//***********************************************
#define AP3_MAX_RANGE_RAND       4       // 32 files
_MEM_TYPE_SLOW_  U8              g_ap3_rand_tab[ AP3_MAX_RANGE_RAND ];
//***********************************************
extern   Bool                    g_b_search_on;
extern _MEM_TYPE_MEDSLOW_   U8    u8_date[4];

//_____ D E C L A R A T I O N ______________________________________________
void  judge_coor                    ( void );
void  judge_key                     ( void );
Bool  search_twobytes_coordinate    ( U32 addr, U16 number );
void  read_book_news                ( U16 num_book );
void  read_control_mode_news        ( U16 num_control );
void  read_page_news                ( U16 num_page );
void  read_sgdb                     ( void );
void  repeat_open_file              ( void );
void  speech_play_end               ( void );
Bool  ap3_rand_table_is_full        ( U8 number );
void  judge_immobility_coor         ( void );
void  judge_mobility_coor           ( void );
void  judge_mix_coor                ( void );
void  search_open_file_fail_deal    ( void );
void  ap4_search_deal               ( void );
void  page_search_deal              ( void );
void  reload_ap4_info_deal          ( void );
void  search_open_file_deal         ( void );
void  search_ap4_deal               ( void );
void  search_book_deal              ( void );
void  search_page_deal              ( void );
void  search_control_deal           ( void );
void  index_coor_deal               ( void );
void  normal_search_deal            ( void );
void  idle_search_deal              ( void );
void  play_search_deal              ( void );
void  sequence_search_deal          ( void );
Bool  search_read_ap4_info          ( U32 start_addr, U8 flag );
Bool  coor_judge_deal               ( void );
void  rp_confirm_display            ( void );
Bool  sequence_judge                ( void );
Bool  game_time_judge               ( void );
Bool  act_do_deal                   ( void );
void  action_play_repeat_display    ( void );
Bool  action_play_sequence_deal     ( void );
Bool  action_select_book            ( void );
Bool  action_select_page            ( void );
Bool  action_select_control         ( void );
void  ap4_text_display              ( /*U32 address*/void );
Bool  judge_index_deal              ( void );
void  search_normal_state_deal      ( void );
void  search_game_state_deal        ( void );
void  judge_index_coor_deal         ( void );
Bool  action_play_sequence          ( void );
Bool  action_play_repeat            ( void );
Bool  action_play_reread            ( void );
Bool  action_play                   ( void );
Bool  action_repeat_game_question   ( void );
Bool  action_next_game              ( void );
Bool  action_prev_game              ( void );
Bool  action_quit_game              ( void );

void brower_file_init(void);
void brower_file(void);
void nofind_display( void );
U16  read_bnl_file(U16 off_t);
Bool some_index_dowith(void);
void action_fanyi(void);
Bool rereadKeyDowith(void);
//***********************************************

extern   Bool     g_b_player_on;

extern   Bool     g_b_search_record;
extern   Bool     g_b_recorder_on;

//_MEM_TYPE_SLOW_   SEARCH_STATUS        Search_Record = { 0 };

_MEM_TYPE_SLOW_   Special_Action       Current_Special_Action;
_MEM_TYPE_SLOW_   Action_Sequence      Current_Act_Sequence;
_MEM_TYPE_SLOW_   U32                  sequence_speech_address;
_MEM_TYPE_SLOW_   U32                  sequence_text_address;
_MEM_TYPE_SLOW_   U32                  sequence_speech_read_point;

//_MEM_TYPE_SLOW_   Record               Current_Record;
_MEM_TYPE_SLOW_   News_All_Control     All_Control;
_MEM_TYPE_SLOW_   News_Index_Range     Index_Range;
_MEM_TYPE_SLOW_   Record_Range        Ap4_Record_Range;
_MEM_TYPE_SLOW_   Book_Table           News_Book;
_MEM_TYPE_SLOW_   Go_Table             News_Go;
_MEM_TYPE_SLOW_   Control_Table        News_Control;
_MEM_TYPE_SLOW_   Coor                 Current_Coor;
_MEM_TYPE_SLOW_   U8                   Coor_Type;
_MEM_TYPE_SLOW_   U8                   Encrypt_Key;
_MEM_TYPE_SLOW_   U8                   browserfile_cmd_id;
_MEM_TYPE_SLOW_   Record_Range         Ap4_Record_Range_2;

_MEM_TYPE_SLOW_   U16                  Temp_Control_num;
_MEM_TYPE_SLOW_   U16                  Temp_Ap3_file_num;
_MEM_TYPE_SLOW_   U16                  Temp_Search_record_value;
_MEM_TYPE_SLOW_   U8                   order_play_count;
_MEM_TYPE_SLOW_   U16                  rp_oid_index;

_MEM_TYPE_SLOW_   U8                   State_Search;

_MEM_TYPE_SLOW_   U32                  Seed;
_MEM_TYPE_SLOW_   U8                   en_text_bf[LEN_EN_TEXTBF];
_MEM_TYPE_SLOW_   U8                   en_word_bf[LEN_EN_WORDBF];
_MEM_TYPE_SLOW_   U8                   num_current_word;//当前要显示下划线的单词
_MEM_TYPE_SLOW_   U8                   allnum_word;//所有单词的数目
_MEM_TYPE_SLOW_   U8                    fenjie_num;//分界点的单词数
_MEM_TYPE_SLOW_   U32                   all_control_addr;
_MEM_TYPE_SLOW_   U32                    all_control_pw;
_MEM_TYPE_SLOW_   U16                    Len_Cap_Book;
_MEM_TYPE_SLOW_   U32                    Addr_Cap_Book;
//                  Bool                 b_reload_ap4 = FALSE;
//                  Bool                 g_b_ap4_caption = FALSE;

//Bool                 action_repeat_game = FALSE;
//                  Bool                 action_display_text = FALSE;
//                  Bool                 action_repeat_display = FALSE;
//                  Bool                 game_exit_to_control = FALSE;

_MEM_TYPE_SLOW_  U32                   Addr_Fanyi;
_MEM_TYPE_SLOW_ U8                     Num_Fanyi;
_MEM_TYPE_SLOW_ U8                     Num_Cur_Fanyi;
_MEM_TYPE_SLOW_ DisplayNews            displayNews;

_MEM_TYPE_MEDSLOW_   U16               virtualKeyState;

extern   _MEM_TYPE_SLOW_   U8    s_ts_ap3record_display_id;
extern __X   char     CurrentShortFilePath[ 256 ];
extern _MEM_TYPE_MEDSLOW_   U16               virtualKeyState;
//***********************************************
#if 1

//the function is realize the search
void search_evt( void )
{
    if( Search_Record.news_sys.flag_index )
        judge_coor();
    if( Search_Record.news_sys.flag_anjian ) {
        SDKLoadCode( JUDGE_KEY_CODE );
        judge_key();
    }
    if(virtualKeyState != IDLE_STATE){
#if (REREAD_MODE == ENABLE)
    SDKLoadCode(JUDGE_REREAD_CODE);
    if(rereadKeyDowith())return;
#endif
    }

    switch( State_Search ) {
        case Browser_Dir_Init:
            SDKLoadCode( BROWER_DIR_INIT_CODE );
            if(brower_dir_init()) {
                SDKLoadCode( BROWER_DIR_CODE );
                goto begin_browserdir;
            }
            SDKLoadCode( BROWER_FILE_CODE );
            goto ready_browserfile;
            break;

        case Browser_DirFile:
        begin_browserdir:
            brower_dir();
            break;

        case Browser_File_Init:
            SDKLoadCode( BROWER_FILE_INIT_CODE );
            brower_file_init();
        ready_browserfile:
            SDKLoadCode( BROWER_FILE_CODE );
            goto begin_browserfile;
            break;

        case Browser_File:
        begin_browserfile:
            brower_file();
            break;

        case READY_GOTO_SUB:
            SDKLoadCode(READY_GO_SUB_CODE);
            ready_go_sub(ap4current_mark);
            break;

        case BACK_TO_SUBROOT:
            SDKLoadCode(BROWER_BACK_SUBROOT_CODE);
            ready_goback_subdir();
            break;


        case Open_File_Search:
            SDKLoadCode( SEARCH_OPEN_FILE_TEMP_CODE );
            search_open_file_deal();
            break;

        case Ap3_Search:
            SDKLoadCode( SEARCH_AP4_TEMP_CODE );
            search_ap4_deal();
            break;

        case Book_Search:
            SDKLoadCode( SEARCH_BOOK_TEMP_CODE );
            search_book_deal();
            break;

        case Page_Search:
            SDKLoadCode( SEARCH_PAGE_TEMP_CODE );
            search_page_deal();
            break;

        case Control_Mode_Search:
            SDKLoadCode( SEARCH_CONTROL_TEMP_CODE );
            search_control_deal();
            break;

        case Normal_Search:
            SDKLoadCode( SEARCH_NORMAL_CODE );
            search_normal_state_deal();
            break;

        case Idle_Search:
            if( g_b_player_on )
                break;
            SDKLoadCode( SEARCH_IDLE_TEMP_CODE );
            idle_search_deal();
            break;

        case Play_Search:
            //if( g_b_recorder_on || g_b_search_record || g_b_player_on )
            //if( g_b_player_on )
            //{
            //init_array_v();
            //break;
            //}
            if(g_b_enable_flush) {
                SDKLoadCode( ACTION_TEXT_DISPLAY_CODE );
                ap4_text_display();
            }

            SDKLoadCode( SEARCH_PLAY_TEMP_CODE );
            play_search_deal();
            break;

        case Sequence_Search:
            //if( bSpeechEnd == FALSE )
            //if( g_b_player_on )
            //   break;
            SDKLoadCode( SEARCH_SEQUENCE_TEMP_CODE );
            sequence_search_deal();
            break;

            /*case Err_Search:
            break;

            case Null_state:
            default:
            break;*/
    }

    /*if( action_repeat_game )
    {
       action_repeat_game = FALSE;
       SDKLoadCode( SEARCH_GAME_CODE );
       init_current_game();
    }*/

//if( action_display_text )//文本capable显示
    /*
      if(g_b_enable_flush)
      {
         SDKLoadCode( ACTION_TEXT_DISPLAY_CODE );
         ap4_text_display();
      }
     */
    /*
       if( b_first_findword_display )//查词原文显示
       {
          SDKLoadCode( FINDWORD_DISPLAY_CODE );
          findword_display();
       }


       if( b_fanyi_back_display )//翻译或者查词显示后返回显示查词原文显示
       {
          SDKLoadCode( TEXT_BACKDISPLAY_CODE );
          text_backdisplay();
       }

      if( b_findword_ch_display )//翻译显示或者查词词义显示
       {
          SendString((U8 *)"\r\nch_display_address=");
          send_hex_u32(ch_display_address);
          b_findword_ch_display = FALSE;
          SDKLoadCode( TEXT_CH_DISPLAY_CODE );
          if(!text_chinese_display(ch_display_address)){
            SDKLoadCode(TEXT_NOFIND_DISPLAY_CODE);
            nofind_display();
            }
          if(b_play_word_mp3){
            b_play_word_mp3 = FALSE;
            write_media_addr(word_mp3_address, 0);
            state_search_to_play();
            }

       }
    */
    /*
    if( action_repeat_display )
    {
       //action_repeat_display = FALSE;
       SDKLoadCode( SEARCH_ACTION_REPEAT_CODE );
       action_play_repeat_display();
    }*/
    if( g_b_recorder_on )
        return;

    SDKLoadCode( SEARCH_GAME_STATUS_CODE );
    search_game_state_deal();
}


//the fuction is to judge coordinate range
void judge_coor( void )
{
    SDKLoadCode( COOR_JUDGE_DEAL_CODE );
    if( coor_judge_deal())
        return;

    SDKLoadCode( SEARCH_INDEX_COOR_CODE );
    judge_index_coor_deal();
}


#if( FAT_CLUSTER_BUF == ENABLE )

#pragma CODE = CLUSTERLIST_CODE
#if( FS_NB_CACHE_CLUSLIST == 1 )
#define  u8_i        0
#endif
void fat_clusterlist_restore( void )
{
#if (FS_NB_CACHE_CLUSLIST>1)
    U8 u8_i;
    for( u8_i=0; u8_i<FS_NB_CACHE_CLUSLIST; u8_i++ )
#endif
        cluster_buf[ u8_i ] = cluster_buf_bak[ u8_i ];
}


void fat_clusterlist_save( void )
{
#if (FS_NB_CACHE_CLUSLIST>1)
    U8 u8_i;
    for( u8_i=0; u8_i<FS_NB_CACHE_CLUSLIST; u8_i++ )
#endif
        cluster_buf_bak[ u8_i ] = cluster_buf[ u8_i ];
}
#pragma CODE = SEARCH_CODE
void cluster_dowith(Bool flag)
{
    SDKLoadCode(CLUSTERLIST_CODE);
    if(flag) {
        fat_clusterlist_restore();
    } else {
        fat_clusterlist_save();
    }
}
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

#pragma CODE = JUDGE_REREAD_CODE
Bool rereadKeyDowith(void){
    Bool flag = false;
    U16 temKey = virtualKeyState;
    virtualKeyState = IDLE_STATE;
    if(player_ap3_is_busy()){
         player_ap3_stop();
        }
     SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    if(temKey == GOTO_NOMAL) {
    
        SendString((U8 *)"\r\nnormal change");
        flag = true;
        bBackNormal = true;
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, NORMAL_MODE_DISPLAY);
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, READ_CTRL_DISPLAY);
        mail_send_event( EVT_SEARCH_HINT_PLAY, NORMAL_MODE_HINT);
        if(g_b_REREAD_Flag){
        g_b_REREAD_Flag = false;
        //if( News_Go.Num_ControlTab > 0){
        //Temp_Control_num = Current_Record.Control_num;
        // Current_Record.Control_num = 0;
        //State_Search = Control_Mode_Search;
        //ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
       // mail_send_event( EVT_SEARCH_FLUSH_CONTROL, READ_CTRL_DISPLAY);
       
        //}
        }
        //return true;
    }
    if(temKey == GOTO_REREAD) {
        SendString((U8 *)"\r\n reread change");
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, REPEAT_MODE_DISPLAY);
        //ap4_read_status = MMI_AP3_RERAED_STATUS;
        //write_media_addr(bakRemoteAddr, 1);
        if(!g_b_REREAD_Flag){
        g_b_REREAD_Flag = true;
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, REPEAT_CTRL_DISPLAY);
        //mail_send_event( EVT_SEARCH_HINT_PLAY, REREAD_MODE_HINT );
        // goto NORMAL_CONTROL;
        flag = true;
        }
        //return true;
    }
    
    return flag;
}
//#pragma CODE = SEARCH_TEMP_CODE06
#pragma CODE = JUDGE_KEY_CODE
//the fuction is to judge key
void judge_key( void )
{
    /*Search_Record.news_sys.reread_flag
       = Search_Record.news_sys.repeat_flag
          = Search_Record.news_sys.sequence_flag
             = Search_Record.news_sys.index_normal_flag
                = Search_Record.news_sys.play_record_flag
                   = 0;
    Search_Record.news_sys.flag_anjian = 0;*/
#ifdef TX_DEBUG
    SendString((U8 *)"\r\najK!");
#endif
    Search_Record.value &= ~0x83C1u;//0x81C1u;
    reread_enable_flag = FALSE;
    //if( player_ap3_is_busy())
    //   player_ap3_stop();
    repeat_save();
    init_array_v();

    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;
    leng_ch = 0;
#if( AP4_RP == ENABLE )
    /*rp_go_on_flag = FALSE;
    rp_record_start = FALSE;
    rp_play_start = FALSE;*/
    rp_flag_bits.value &= ~0xE000u;//~0x07;
#endif   //#if( AP4_RP == ENABLE )

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );

    if( s_ts_id_read != UNDEFINED_TIMER ) {
        Ts_free( s_ts_id_read );

//      lcd_goto_xy( 15, 12 );
//      Ih_set_curr_font( IDC_FONT_16 );
//      lcd_puts( g_string );
        mmi_ap3_display_icon(FALSE);
    }
    {
        switch( Search_Record.news_sys.poweroff_flag ) {
            case 1:
                Search_Record.news_sys.poweroff_flag = 2;
                if( internal_speech & POWEROFF_SPEECH ) {
                    Search_Record.news_sys.poweroff_flag = 0;
                    mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
                    break;
                }
                if( act_do( All_Control.Addr_Action_close, 0 )) {

                    state_search_to_play();
                } else {
                    Search_Record.news_sys.poweroff_flag = 0;
                    mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
                }
                break;

            case 3:
                Search_Record.news_sys.poweroff_flag = 2;
                if( internal_speech & BATTERYLOW_SPEECH ) {
                    Search_Record.news_sys.poweroff_flag = 0;
                    mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
                    break;
                }
                if( act_do( Current_Special_Action.Battery_Low_Speech, 0 )) {
                    b_play_bat_waring = TRUE;
                    state_search_to_play();
                } else {
                    Search_Record.news_sys.poweroff_flag = 0;
                    mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
                }
                break;

            case 5:
                Search_Record.news_sys.poweroff_flag = 0;
                if( act_do( All_Control.Addr_Antion_key, 0 )) {
                    state_search_to_play();
                }
                break;
        }
    }
}




#pragma CODE = JIAMI_CODE
Bool judge_password(void)
{
    U16 new_ap4_flag;
    U32 temp;
    reread_enable_flag = FALSE;
    Search_Record.value = 0;
    all_control_addr = 0;
    all_control_pw = 0;
    ( void )file_seek( 0, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&new_ap4_flag, sizeof( U16 ));
    if( new_ap4_flag != 0xFFEE )
        return FALSE;

    ( void )file_seek( 0x35+2, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&all_control_addr, sizeof( U32 ));
    ( void )file_seek( 0x99+2, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&temp, sizeof( U32 ));
    all_control_addr ^= temp;

    SendString((U8 *)"\r\nall_control_addr=");
    send_hex_u32(all_control_addr);

    return TRUE;
}

#pragma CODE = FIND_PASS_01_CODE

U32 read_new_ap4_password_01( void )
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

#if 1
#define  PASSWORD_POS1        0x38  //0x5C  //0x40//
#define  PASSWORD_POS2        0xE1  //0x7B  //0x75//
#define  PASSWORD_POS3        0x9A  //0x9D  //0x50//
#define  PASSWORD_POS4        0xE9  //0x38  //0x85//
#else
#define  PASSWORD_POS1        0x8F//0x38  //0x5C  //0x40//
#define  PASSWORD_POS2        0xCE//0xE1  //0x7B  //0x75//
#define  PASSWORD_POS3        0xA9//0x9A  //0x9D  //0x50//
#define  PASSWORD_POS4        0x9E//0xE9  //0x38  //0x85//
#endif
U32  read_new_ap4_password_02( void )
{
    //U16 new_ap4_flag, rand_len;
    U32 temp;
    U8 pw_temp;

    Ap4_Record_Range_2.Record_Index_Min = 0;
    Ap4_Record_Range_2.Record_Index_Max = 0;
    Ap4_Record_Range_2.Play_Index_Min = 0;
    Ap4_Record_Range_2.Play_Index_Max = 0;


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
    do {
        byte = shuju >> 24;
        shuju <<= 8;
        shuju += byte;
        shuju ^= mima;
    } while( --i );
    return shuju;
}
#pragma CODE = NEW_READ_ALLNEWS_CODE
// the function is read all control news;
void new_read_all_control_news( void )
{
    U8 all_control_news[ 108 ], i;
    //U16 new_ap4_flag, rand_len;
    U32 ap4_pw, user_pw, temp_01,temp_02;
    U8 pw_temp;

    U32 *temp = ( U32 *)&all_control_news;

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

    SDKLoadCode(FIND_PASS_03_CODE);
    user_pw = jiami( ap4_pw, temp_01 );
    all_control_pw = jiami( user_pw, temp_02 );


    //SDKLoadCode( JIAMI_CODE );
    ( void )file_seek( all_control_addr, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&all_control_news, sizeof( all_control_news ));
    SDKLoadCode(FIND_PASS_03_CODE);
    for( i = 0; i <( sizeof( all_control_news )/ 4 ); i++ )
        //for( i = 0; i <27; i++ )
    {
        *temp = jiami( *temp, all_control_pw );
        temp++;
    }

    memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control, all_control_news, 17 );
    memcpy(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, &all_control_news[ 17 ], 30 );
    memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Min, &all_control_news[ 59 ], sizeof( U16 )* 2 );
    memcpy(( U8 _MEM_TYPE_SLOW_ *)&Index_Range, &all_control_news[ 63 ], sizeof( News_Index_Range ));
    memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Min, &all_control_news[ 87 ], sizeof( U16 )* 2 );

    if( All_Control.Version >= 6 ) {
        Encrypt_Key = all_control_news[ 91 ];
    }

    if( All_Control.Version >= 9 ) {
        memcpy(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range_2, &all_control_news[ 98 ], sizeof( Record_Range ));
    }
    /*
       else
       {
          Ap4_Record_Range_2.Record_Index_Min = 0;
          Ap4_Record_Range_2.Record_Index_Max = 0;
          Ap4_Record_Range_2.Play_Index_Min = 0;
          Ap4_Record_Range_2.Play_Index_Max = 0;
       }
    */
    Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
    ap4_AllControl_MaskGlobal = All_Control.Mask_Gloablext;

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

    SDKLoadCode(PRINT_CODE);
    print_news();

}

#pragma CODE = READ_ALLNEWS_CODE
// the function is read all control news;
void read_all_control_news( void )
{
    ( void )file_seek( 0, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control, 17 );
    //file_seek( 16, FS_SEEK_SET );
    //file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Flag_Xuanshuye, sizeof( U8 ));
    ( void )file_seek( 17, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&All_Control.Action_Xuanshu, 30 );

    ( void )file_seek( 59, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Record_Index_Min, sizeof( U16 )* 2 );

    ( void )file_seek( 63, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Index_Range, sizeof( News_Index_Range ));

    ( void )file_seek( 87, FS_SEEK_SET );
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range.Play_Index_Min, sizeof( U16 )* 2 );

    if( All_Control.Version >= 6 ) {
        ( void )file_seek( 91, FS_SEEK_SET );
        file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Encrypt_Key, sizeof( U8 ));
    }

    if( All_Control.Version >= 9 ) {
        ( void )file_seek( 98, FS_SEEK_SET );
        file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&Ap4_Record_Range_2, sizeof( Record_Range ));
    } else {
        Ap4_Record_Range_2.Record_Index_Min = 0;
        Ap4_Record_Range_2.Record_Index_Max = 0;
        Ap4_Record_Range_2.Play_Index_Min = 0;
        Ap4_Record_Range_2.Play_Index_Max = 0;
    }

    Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
    ap4_AllControl_MaskGlobal = All_Control.Mask_Gloablext;
}
#pragma CODE =  PRINT_CODE
void print_news(void)
{

    SendString((U8 *)"\r\nbegin:key=");
    send_hex_u16(all_control_addr);
    SendString((U8 *)":");
    send_hex_u16(all_control_pw);

    SendString((U8 *)"\r\nall_news:flag:key=");
    send_hex_u16(All_Control.Version);
    SendString((U8 *)":");
    send_hex_u8(Encrypt_Key);

    
    SendString((U8 *)"\r\nremotePoint=");
    send_hex_u32(All_Control.Point_Remote);
    SendString((U8 *)"\r\n");


}
#pragma CODE =  LCE_ENCRYPT_CODE
void lce_sec(void)
{
    U8       all_news_key[] = {'H','Y','N','W'};
    U32  keyword;
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

    RemoteBaseAddress = All_Control.Point_Remote;

    if( All_Control.Version >= 6 ) {
        Encrypt_Key ^= ( U8 )keyword;
    }


    Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
    ap4_AllControl_MaskGlobal = All_Control.Mask_Gloablext;
}

//#pragma CODE = SEARCH_TEMP_CODE01
#pragma CODE = READ_BOOKNEWS_CODE
// the function is read current bood news;
void read_book_news( U16 num_book )
{
    U32 _MEM_TYPE_SLOW_ Address;
    U32 _MEM_TYPE_SLOW_ BookAddress;
    Len_Cap_Book = 0;
    Address = All_Control.Point_Book_Table + sizeof( U32 )* num_book;
    read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&BookAddress, sizeof( U32 ));
    //read_data_from_nf( BookAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Book, sizeof( Book_Table ));
    //SendString((U8 *)"\r\nb_d=");send_hex_u32(BookAddress);
    read_data_from_nf( BookAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Book, 6 );
    read_data_from_nf( BookAddress + 22, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Flag_CurrentBook, sizeof( U8 ));
    //read_data_from_nf( BookAddress + 23, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, 26 );
    read_data_from_nf( BookAddress + 23, ( U8 _MEM_TYPE_SLOW_ *)&News_Book.Point_GoTable, 34 );
    // read_data_from_nf( News_Book.Point_Book_Cap, ( U8 _MEM_TYPE_SLOW_ *)&Len_Cap_Book, 2 );

    //Addr_Cap_Book =  News_Book.Point_Book_Cap + 2;
}


//#pragma CODE = SEARCH_TEMP_CODE02
#pragma CODE = READ_PAGENEWS_CODE
// the function is read current bood news;
void read_page_news( U16 num_page )
{
    U32 _MEM_TYPE_SLOW_ Address;
    U32 _MEM_TYPE_SLOW_ PageAddress;
    SendString((U8 *)"\r\npage_n=");
    send_hex_u16(num_page);
    Address = News_Book.Point_GoTable + sizeof( U32 )* num_page;
    read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&PageAddress, sizeof( U32 ));
    //read_data_from_nf( PageAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Go, sizeof( Go_Table ));
    read_data_from_nf( PageAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Go, 16 );
    read_data_from_nf( PageAddress + 32, ( U8 _MEM_TYPE_SLOW_ *)&News_Go.Point_DefaultTab, 22 );
}


//#pragma CODE = SEARCH_TEMP_CODE03
#pragma CODE = READ_CONTROLNEWS_CODE
// the function is read current control mode news;
void read_control_mode_news( U16 num_control )
{
    U32 _MEM_TYPE_SLOW_ Address;
    U32 _MEM_TYPE_SLOW_ ControlAddress;
    Address = News_Go.Point_ControlTab + sizeof( U32 )* num_control;
    read_data_from_nf( Address, ( U8 _MEM_TYPE_SLOW_ *)&ControlAddress, sizeof( U32 ));
    read_data_from_nf( ControlAddress, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Caption_Control[ 0 ], Length_Caption );
    read_data_from_nf( ControlAddress + Length_Caption, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Point, 9 );
    //read_data_from_nf( ControlAddress + 24, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Type_Control, sizeof( U8 ));
    read_data_from_nf( ControlAddress + 25, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Num_Control, 3 );
    //read_data_from_nf( ControlAddress + 27, ( U8 _MEM_TYPE_SLOW_ *)&News_Control.Flag, sizeof( U8 ));
}


//#pragma CODE = SEARCH_TEMP_CODE11
#pragma CODE = SEARCH_TWOBYTES_COOR_CODE
// the function is quickly search.
Bool search_twobytes_coordinate( U32 addr, U16 number )
{
    U16 _MEM_TYPE_SLOW_ nStart, nMiddle, Temp_coor/* nEnd,, Step*/;
    U32 _MEM_TYPE_SLOW_ iAddress;
#undef nEnd
#define nEnd   number

    if( !addr || !number )
        return FALSE;
    nStart = 0;
    nEnd = number - 1;
    while( nStart <= nEnd ) {
        //nMiddle = ( nStart + nEnd )/ 2;
        nMiddle = ( nStart + nEnd )>> 1;
        iAddress = addr + sizeof( U16 ) * nMiddle;
        read_data_from_nf( iAddress, ( U8 _MEM_TYPE_SLOW_ *)&Temp_coor, sizeof( U16 ));
        if( Current_Index == Temp_coor ) {
            //i = iAddress - *addr;
            //Current_Coor.X0 = ( U16 )( i / sizeof( U16 ));
            Current_Coor.X0 = nMiddle;
            return TRUE;
        } else if( Current_Index < Temp_coor ) {
            if( nMiddle == 0 ) {
                return FALSE;
            }
            nEnd = nMiddle - 1;
        } else {
            nStart = nMiddle + 1;
        }
    }
    return FALSE;
#undef nEnd
}


//#pragma CODE = SEARCH_TEMP_CODE04
#pragma CODE = SEARCH_PLAY_END_CODE
void speech_play_end( void )
{
    //Search_Record.news_sys.index_normal_flag = 0;
    if( bRemoteChangeAp3 ) {
        bRemoteChangeAp3 = FALSE;
        mail_send_event( EVT_SEARCH_REMOTE_CHANGE_AP3, 0 );
    } else if( bRmoteExitAp4 ) {
        /*bRmoteExitAp4 = FALSE;
        bInCal = FALSE;*/
        remote_bits.value &= ~0x0110u;//0x0880u;
        mail_send_event( EVT_SEARCH_END, remote_to_mode );
    }
#if( AP4_RP == ENABLE )
    else if( rp_record_start ) {
        rp_record_start = FALSE;
        mail_send_event( EVT_SEARCH_RP_RECORD_START, 0 );
    } else if( rp_play_start ) {
        rp_play_start = FALSE;
        mail_send_event( EVT_SEARCH_RP_PLAY_START, 0 );
    }
#endif   //#if( AP4_RP == ENABLE )

    if( b_remote_play_repeat ) {
        Point_Read = 0;
        state_search_to_play();
        return;
    }

    //if( Search_Record.news_sys.reread_flag )
    if( reread_enable_flag ) {
        reread_enable_flag = FALSE;
        //init_array_v();
        scan_oid_disable();
        /*Search_Record.news_sys.reread_flag = 0;
        Search_Record.news_sys.flag_index = 0;
        Search_Record.news_sys.flag_anjian = 0;*/
        Search_Record.value &= ~0xC200u;//0x0043u;
        //mail_send_event( EVT_SEARCH_AP3RECORD, 0 );
        mail_send_event( EVT_SEARCH_PLAYREREAD, 0 );
        reread_end_time += timer_get_time()- reread_start_time;
        if( reread_end_time < 1000 )
            reread_end_time = 1000;
        reread_end_time =  reread_end_time * 3 / 1000;
        if( reread_end_time >= 24ul * 60 * 60 )
            reread_end_time = 23ul * 60 * 60 + 59ul * 60 + 59;
        MSB0( reread_time ) = 0;
        MSB1( reread_time ) = reread_end_time / 3600;   // hours
        reread_end_time %= 3600;
        MSB2( reread_time ) = reread_end_time / 60;     // minutes
        MSB3( reread_time ) = reread_end_time % 60;
    } else if( Search_Record.news_sys.play_record_flag ) {
        Search_Record.news_sys.play_record_flag = 0;
        mail_send_event( EVT_SEARCH_AP3RECORD_PLAY, 0 );
    } else if( Search_Record.news_sys.repeat_flag ) {
        Point_Read = 0;
        state_search_to_play();
    } else if( Search_Record.news_sys.poweroff_flag == 2 ) {
        Search_Record.news_sys.poweroff_flag = 0;
        mail_send_event( EVT_SEARCH_AP3POWEROFF, 0 );
    }
}


//#pragma CODE = SEARCH_TEMP_CODE12
#pragma CODE = CHECK_GAMETIEM_CODE
void check_game_time( void )
{
    if( bTimerEnable ) {
        if( GameTipTick && Timer_is_timeout( GameTipTick )) {
            bTipPlay = TRUE;
            GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
        }
    }
    if( bTimeOverEnable ) {
        if( GameTimeOverTick && Timer_is_timeout( GameTimeOverTick )) {
            bGameTimeOverPlay = TRUE;
            GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
        }
    }
}


//#pragma CODE = SEARCH_TEMP_CODE13
#pragma CODE = GAME_TIMEOUT_DEAL_CODE
void game_timeout_deal( void )
{
    if( bTipPlay ) {
        SDKLoadCode( SEARCH_GAME_CODE );
        game_exit();
        if( !bExitGamePRN ) {
            play_timer_tip();
        }
        /*bSearchFound = FALSE;
        bTipPlay = FALSE;*/
        bGameStatus.value &= ~0x0084u;//0x2100u;
    }
    if( bGameTimeOverPlay ) {
        bGameTimeOverPlay = FALSE;
        switch( Current_GameNews.Fuction_Question ) {
            case GAME_TYPE_USER:
                if( !bSubject )
                    break;
            // else, no break

            case GAME_TYPE_SEQU:
            case GAME_TYPE_RAND:
                if( bInGame ) {
                    init_array_v();
                    ( void )act_do( Current_Game.Act_Game_Concent, 0 );
                }
                break;

                //case GAME_TYPE_REV:
                // no break
                //default:
                //break;
        }
    }
    if( bSkipNext ) {
        bSkipNext = FALSE;
        SDKLoadCode( SEARCH_GAME_CODE );
        switch( Current_GameNews.Fuction_Question ) {
            case GAME_TYPE_RAND:
            case GAME_TYPE_SEQU:
            case GAME_TYPE_USER:
                if( bNotAutoNextGame ) {
                    bNotAutoNextGame = FALSE;
                    game_next();
                    bNotAutoNextGame = TRUE;
                } else
                    game_next();
                break;
        }
    }
    if( bSkipPrevious ) {
        bSkipPrevious = FALSE;
        SDKLoadCode( SEARCH_GAME_CODE );
        switch( Current_GameNews.Fuction_Question ) {
            case GAME_TYPE_RAND:
            case GAME_TYPE_SEQU:
            case GAME_TYPE_USER:
                if( bNotAutoNextGame ) {
                    bNotAutoNextGame = FALSE;
                    bGamePrev = TRUE;
                    game_next();
                    bGamePrev = FALSE;
                    bNotAutoNextGame = TRUE;
                } else {
                    bGamePrev = TRUE;
                    game_next();
                    bGamePrev = FALSE;
                }
                break;
        }
    }
}


//#pragma CODE = SEARCH_AP4INFO_TEMP_CODE05
#pragma CODE = SEARCH_AP4INFO_CODE
Bool search_read_ap4_info( U32 start_addr, U8 flag )
{
    U8 codec;
    /*if( player_ap3_is_busy())
    {
       return FALSE;
    }*/
    if( !file_seek( start_addr, FS_SEEK_SET )) {
        return FALSE;
    }
    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ap4_audio_var.audio_var.file_size_count, 8 );

    /*
    //SendString((U8 *)"count:");
    send_hex_u32(ap4_audio_var.audio_var.file_size_count);
     send_hex_u32(ap4_audio_var.audio_var.play_file_size);
    //SendString((U8 *)"\r\n");
    //*/
    if( !ap4_audio_var.audio_var.file_size_count )
        return FALSE;

    //file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ap4_audio_var.audio_var.play_file_size, sizeof( U32 ));
    /*SendString((U8 *)"size:");
    send_hex_u32(ap4_audio_var.audio_var.play_file_size);
    SendString((U8 *)"\r\n");*/

    if( ap4_audio_var.audio_var.play_file_size < 512 )
        return FALSE;

    file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&codec, sizeof( U8 ));
    /*if( codec )
       return FALSE;*/
    switch( codec ) {
        case 0:
            audio_is_wav = 0;
            //SendString((U8 *)"mp3\r\n");
            break;

        case 2:
            audio_is_wav = 1;
        //SendString((U8 *)"wav\r\n");
        //break;

        default:
            return FALSE;
    }

    if( flag == 1 ) {
        if(( RemoteVersion >= 4 )
           &&( RemoteFlag & ENCRYPT_SPEECH_FLAG )) {
            if( RemoteEncryptKey > 0x07 )
                return FALSE;
            codec = ap4_audio_var.encrypt[ RemoteEncryptKey ];
            //LROM_EXCHANGE = 0x00;
            MSB0( Key_encrypt_mp3 ) = codec;
            MSB1( Key_encrypt_mp3 ) = codec;
            MSB2( Key_encrypt_mp3 ) = codec;
            MSB3( Key_encrypt_mp3 ) = codec;
            //wav_encrypt = TRUE;
        } else {
            Key_encrypt_mp3 = 0x00;
            //LROM_EXCHANGE = 0x00;
            //wav_encrypt = FALSE;
        }
        ap4_audio_var.audio_var.file_size_count += RemoteBaseAddress;
    } else {
        if(( All_Control.Version >= 6 )
           &&( All_Control.Mask_Gloablext & ENCRYPT_AUDIO_DATA_FLAG )) {
            if( Encrypt_Key > 0x07 )
                return FALSE;
            codec = ap4_audio_var.encrypt[ Encrypt_Key ];
            //LROM_EXCHANGE = 0x00;
            MSB0( Key_encrypt_mp3 ) = codec;
            MSB1( Key_encrypt_mp3 ) = codec;
            MSB2( Key_encrypt_mp3 ) = codec;
            MSB3( Key_encrypt_mp3 ) = codec;
            //wav_encrypt = TRUE;
        } else {
            Key_encrypt_mp3 = 0x00;
            //LROM_EXCHANGE = 0x00;
            //wav_encrypt = FALSE;
        }
    }

    if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
        return FALSE;
    //ap4_audio_var.audio_var.play_file_size = ( ap4_audio_var.audio_var.play_file_size + FS_512B_MASK )&(~( U32 )FS_512B_MASK );
    //if( !file_seek( ap4_audio_var.audio_var.play_file_size, FS_SEEK_SET ))
    //return FALSE;
    //if( !varAl.WAVE_Flg )
    //   ap4_audio_var.audio_var.play_file_size += ap4_audio_var.audio_var.file_size_count;
    return TRUE;
}


#pragma CODE = SEARCH_ADBINFO_CODE
Bool search_read_adb_info( U32 start_addr )
{
    U8 codec;
    FS_ERR err;
    /*if( player_ap3_is_busy())
    {
       return FALSE;
    }*/
    if( FSSeek(adb_g_file,start_addr ) != FS_NO_ERROR ) {
        return FALSE;
    }
    // file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ap4_audio_var.audio_var.file_size_count, 8 );
    (void)FSRead(adb_g_file, (__X void *)&ap4_audio_var.audio_var.file_size_count, 8, &err);
    /*
    //SendString((U8 *)"count:");
    send_hex_u32(ap4_audio_var.audio_var.file_size_count);
     send_hex_u32(ap4_audio_var.audio_var.play_file_size);
    //SendString((U8 *)"\r\n");
    //*/
    if( !ap4_audio_var.audio_var.file_size_count )
        return FALSE;

    //file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&ap4_audio_var.audio_var.play_file_size, sizeof( U32 ));
    /*SendString((U8 *)"size:");
    send_hex_u32(ap4_audio_var.audio_var.play_file_size);
    SendString((U8 *)"\r\n");*/

    if( ap4_audio_var.audio_var.play_file_size < 512 )
        return FALSE;

    //file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&codec, sizeof( U8 ));


    audio_is_wav = 0;
    Key_encrypt_mp3 = 0x00;

    // if( !file_seek( ap4_audio_var.audio_var.file_size_count, FS_SEEK_SET ))
    //   return FALSE;

    if( FSSeek(adb_g_file,ap4_audio_var.audio_var.file_size_count ) != FS_NO_ERROR ) {
        return FALSE;
    }
    b_play_adb_mp3 = TRUE;
    return TRUE;
}

//#pragma CODE = SEARCH_TEMP_CODE05
#pragma CODE = PLAYER_AP3_START_CODE
Bool player_ap3_start( U32 start_addr, U8 flag )
{
#if 0
    if(flag ==  2) {
        ///*
        SDKLoadCode( SEARCH_ADBINFO_CODE );
        if( !search_read_adb_info( start_addr ))
            return FALSE;
        // */
    } else {
        if( !search_read_ap4_info( start_addr, flag ))
            return FALSE;
    }
#else
    b_play_adb_mp3 = FALSE;
    if( !search_read_ap4_info( start_addr, flag ))
        return FALSE;
#endif
    return player_music_start();
}


#pragma CODE = SEARCH_CODE

//the function is read need data from nf
void read_data_from_nf( U32 start_addr, U8 _MEM_TYPE_SLOW_ *store_addr, U16 leng_data )
{
    //if( file_seek( start_addr, FS_SEEK_SET ))
    ( void )file_seek( start_addr, FS_SEEK_SET );
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


void repeat_save( void )
{
    repeat_point = Point_Write;
}


//#the function is deal with the act;
Bool act_do( U32 address, Bool game_act_do_flag )
{
    //U16 temp;
    b_play_repeat = FALSE;

    if( address == 0 )
        return FALSE;
    count_AB = 0;
    action_address = address;
    act_do_flag = game_act_do_flag;
    SendString((U8 *)"\r\nd=");
    send_hex_u32(action_address);
    SDKLoadCode( SEARCH_ACTION_TEMP_CODE00 );
    return act_do_deal();
}
/*
void setRereadMode(void){
   Search_Record.news_sys.reread_flag = 1;
   reread_start_time = timer_get_time();
   reread_end_time = 0;
   reread_time = 0;
}
*/


#pragma CODE = ACTION_TEMP_CODE07

Bool action_quit_game( void )
{
    if( bInGame &&( News_Control.Type_Control == GAME_TYPE )) {
        bTipPlay = TRUE;
        bExitGameAction = FALSE;
        return TRUE;
    }
    return FALSE;
}


#pragma CODE = ACTION_TEMP_CODE06

Bool action_prev_game( void )
{
    if( News_Control.Type_Control == GAME_TYPE ) {
        bSkipPrevious = TRUE;
        return TRUE;
    }
    return FALSE;
}


#pragma CODE = ACTION_TEMP_CODE05

Bool action_next_game( void )
{
    if( News_Control.Type_Control == GAME_TYPE ) {
        bSkipNext = TRUE;
        return TRUE;
    }
    return FALSE;
}


#pragma CODE = ACTION_TEMP_CODE04

Bool action_repeat_game_question( void )
{
    if( bInGame &&( News_Control.Type_Control == GAME_TYPE )) {
        SDKLoadCode( SEARCH_GAME_CODE );
        init_current_game();
        bGameTimeOverPlay = TRUE;
        return TRUE;
    }
    return FALSE;
}

#pragma CODE = ACTION_TEMP_CODE10
void actionVolueChange( void )
{
    if(Current_Act.Type == Type_Vol_Up) {
        voice_play_number();
        srvplayer_volume_change( SRVPLAYER_VOL_UP );
        // mmi_ap3_display_volume();
    }

    if(Current_Act.Type == Type_V_Big) {
        srvplayer_volume_change( SRVPLAYER_VOL_UP );
        //mmi_ap3_display_volume();
    }

    if(Current_Act.Type == Type_Vol_Down) {
        voice_play_number();
        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        // mmi_ap3_display_volume();
    }

    if(Current_Act.Type == Type_V_Small) {
        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        //
    }
    mmi_ap3_display_volume();
}

#pragma CODE = ACTION_TEMP_CODE09

void actionModeChange( void )
{
    if(g_b_REREAD_Flag) {

        Search_Record.news_sys.reread_flag = 1;
        reread_start_time = timer_get_time();
        reread_end_time = 0;
        reread_time = 0;
    }
}

#pragma CODE = ACTION_TEMP_CODE08
void action_fanyi(void)
{
    Addr_Fanyi = 0;
    Num_Fanyi = 0;
    if(News_Control.Type_Control==SINGLE_TABLE_TYPE) {
        Addr_Fanyi = action_address + 5;
        Num_Fanyi = Current_Act.act_info.speech_info.Num_Voice;
        Num_Cur_Fanyi = 1;
    }

    if(action_address == News_Go.Action_NoVoice) {
        Addr_Fanyi = 0;
        Num_Fanyi = 0;
    }

    if(g_b_reread_play) {
        Search_Record.news_sys.reread_flag = 1;
        reread_start_time = timer_get_time();
        reread_end_time = 0;
        reread_time = 0;
    }
}

#pragma CODE = ACTION_TEMP_CODE03

Bool action_play( void )
{
    if( act_do_flag )
        return FALSE;

    SDKLoadCode( ACTION_TEMP_CODE08 );
    action_fanyi();
    b_findword_ch_display = FALSE;
    action_display_text = FALSE;
    if( Current_Act.Type == Type_Play ) {
        b_play_repeat = TRUE;
    } else if( Current_Act.Type == Type_Play_Findword) {
        b_first_findword_display  = TRUE;
        //action_display_text = FALSE;
    } else {
        action_display_text = TRUE;
    }
    voice_play_number();
    return TRUE;
}


#pragma CODE = ACTION_TEMP_CODE02

Bool action_play_reread( void )
{
    if( act_do_flag )
        return FALSE;
    voice_play_number();
    /*
       Search_Record.news_sys.reread_flag = 1;
       reread_start_time = timer_get_time();
       reread_end_time = 0;
       reread_time = 0;
    //*/
    return TRUE;
}


#pragma CODE = ACTION_TEMP_CODE01

Bool action_play_repeat( void )
{
    if( act_do_flag )
        return FALSE;

    voice_play_number();
//  action_repeat_display = TRUE;
    SDKLoadCode( SEARCH_ACTION_REPEAT_CODE );
    action_play_repeat_display();
    return TRUE;
}

#pragma CODE = SEARCH_ACTION_TEMP_CODE00

Bool act_do_deal( void )
{
    read_data_from_nf( action_address, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act, ( U16 )5 );

    //MSB( temp ) = Current_Act.Num_Voice;
    //LSB( temp ) = Current_Act.Mode_Play;

#if (REREAD_MODE == ENABLE)
    SDKLoadCode( ACTION_TEMP_CODE09 );
    actionModeChange();
#endif
    SDKLoadCode( SEARCH_ACTION_TEMP_CODE01 );

    switch( Current_Act.Type ) {
        case Type_Repaet_Speech:
            if( repeat_point ) {
                Point_Read = repeat_point - 1;
                Point_Write = repeat_point;
            }
            //break;
            return TRUE;

        case Type_Stop:
            if( player_ap3_is_busy())
                player_ap3_stop();
            //break;
            return TRUE;

        case Type_Vol_Up:
        // voice_play_number();
        case Type_V_Big:
        // srvplayer_volume_change( SRVPLAYER_VOL_UP );
        // mmi_ap3_display_volume();
        //break;
        //return TRUE;

        case Type_Vol_Down:
        // voice_play_number();
        case Type_V_Small:
            // srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
            // mmi_ap3_display_volume();
            //break;
            SDKLoadCode(ACTION_TEMP_CODE10);
            actionVolueChange();
            return TRUE;

        case Type_Repeat_Game_Question:
#if 0
            //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
            if( bInGame &&( News_Control.Type_Control == GAME_TYPE )) {
                //action_repeat_game = TRUE;
                SDKLoadCode( SEARCH_GAME_CODE );
                init_current_game();
                bGameTimeOverPlay = TRUE;
                return TRUE;
            }
            break;
#else
            SDKLoadCode( ACTION_TEMP_CODE04 );
            return action_repeat_game_question();
#endif

        case Type_Next_Game:
#if 0
            //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
            if( News_Control.Type_Control == GAME_TYPE ) {
                bSkipNext = TRUE;
                return TRUE;
            }
            break;
#else
            SDKLoadCode( ACTION_TEMP_CODE05 );
            return action_next_game();
#endif

        case Type_Prev_Game:
#if 0
            //if( Search_Record.news_sys.mode_flag == GAME_TABLE )
            if( News_Control.Type_Control == GAME_TYPE ) {
                bSkipPrevious = TRUE;
                return TRUE;
            }
            break;
#else
            SDKLoadCode( ACTION_TEMP_CODE06 );
            return action_prev_game();
#endif

        case Type_Quit_Game_Speech:
            voice_play_number();
        case Type_Quit_Game:
#if 0
            //if( bInGame &&( Search_Record.news_sys.mode_flag == GAME_TABLE ))
            if( bInGame &&( News_Control.Type_Control == GAME_TYPE )) {
                bTipPlay = TRUE;
                bExitGameAction = FALSE;
                return TRUE;
            }
            break;
#else
            SDKLoadCode( ACTION_TEMP_CODE07 );
            return action_quit_game();
#endif

        case Type_Go_Mp3:
            mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
            //break;
            return TRUE;

        case Type_Play:
        case Type_Play_Title:
        case Type_play_Caption:
        case Type_Play_Findword:
#if 0
            if( act_do_flag )
                break;
            b_play_repeat = TRUE;
            voice_play_number();
            //break;
            return TRUE;

        case Type_Play_Title:
        case Type_play_Caption:
            if( act_do_flag )
                break;
            voice_play_number();
            action_display_text = TRUE;
            //SDKLoadCode( ACTION_TEXT_DISPLAY_CODE );
            //ap4_text_display();
            //break;
            return TRUE;
#else



            SDKLoadCode( ACTION_TEMP_CODE03 );
            return action_play();
#endif

        case Type_play_Reread:
#if 0
            if( act_do_flag )
                break;
            voice_play_number();
            Search_Record.news_sys.reread_flag = 1;
            reread_start_time = timer_get_time();
            reread_end_time = 0;
            reread_time = 0;
            //break;
            return TRUE;
#else
            SDKLoadCode( ACTION_TEMP_CODE02 );
            return action_play_reread();
#endif

        case Type_play_Repeat:
#if 0
#else
            SDKLoadCode( ACTION_TEMP_CODE01 );
            return action_play_repeat();
#endif

        case Type_Play_Sequence:
#if 0
#else
            SDKLoadCode( ACTION_PLAY_SEQUENCE_CODE );
            return action_play_sequence();
#endif

//***********************************************
        //add by xuanyu
        case Type_SelectBook:
            SDKLoadCode( ACTION_SELECT_BOOK_CODE );
            /*if( !action_select_book())
               break;
            #if 0
            if( Coor_Type == MOBILE_COOR_MODE )
            {
               if( !Current_Act.act_info.Num_Change_Mode )
                  break;
               if( Current_Act.act_info.Num_Change_Mode > All_Control.Num_Book )
                  break;
               Current_Act.act_info.Num_Change_Mode--;
            }
            else
            {
               if( Current_Act.act_info.Num_Change_Mode >= All_Control.Num_Book )
                  break;
            }
            Search_Record.news_sys.ap3_aeb_flag = 1;
            Current_Record.Book_num = Current_Act.act_info.Num_Change_Mode;
            State_Search = Book_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
            #endif
            //break;
            return TRUE;*/
            return action_select_book();

        case Type_SelectPage:
            SDKLoadCode( ACTION_SELECT_PAGE_CODE );
            /*if( !action_select_page())
               break;
            #if 0
            if( Current_Act.act_info.Num_Change_Mode >= News_Book.Num_GoTable )
               break;
            Search_Record.news_sys.ap3_aeb_flag = 1;
            Current_Record.Page_num = Current_Act.act_info.Num_Change_Mode;
            State_Search = Page_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
            #endif
            //break;
            return TRUE;*/
            return action_select_page();

        case Type_SelectControl:
            SDKLoadCode( ACTION_SELECT_CONTROL_CODE );
            /*if( !action_select_control())
               break;
            #if 0
            if( Current_Act.act_info.Num_Change_Mode >= News_Go.Num_ControlTab )
               break;
            Search_Record.news_sys.ap3_aeb_flag = 1;
            Temp_Control_num = Current_Record.Control_num;
            Current_Record.Control_num = Current_Act.act_info.Num_Change_Mode;
            State_Search = Control_Mode_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
            #endif
            //break;
            return TRUE;*/
            return action_select_control();
//***********************************************

        case Type_Script:
            SDKLoadCode( SEARCH_SCRIPT_CODE );
            script_init( action_address + ( U32 )sizeof( U16 ));
            script_decode();
            break;

            /*default:
            return FALSE;*/
    }
    //if( act_do_flag )
    //   return FALSE;
    return FALSE;
}


//#pragma CODE = ACTION_TEMP_CODE00
#pragma CODE = ACTION_PLAY_SEQUENCE_CODE
Bool action_play_sequence( void )
{
    if( act_do_flag )
        return FALSE;
    SDKLoadCode( ACTION_SEQUENCE_CODE );
    if( action_play_sequence_deal()) {
        SDKLoadCode( SEARCH_ACTION_TEMP_CODE01 );
        voice_play_number();
    }
    return TRUE;
}


#pragma CODE = ACTION_SELECT_CONTROL_CODE

Bool action_select_control( void )
{
    if( Current_Act.act_info.Num_Change_Mode >= News_Go.Num_ControlTab )
        return FALSE;
    Search_Record.news_sys.ap3_aeb_flag = 1;
    Temp_Control_num = Current_Record.Control_num;
    Current_Record.Control_num = Current_Act.act_info.Num_Change_Mode;
    State_Search = Control_Mode_Search;
    ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
    return TRUE;
}


#pragma CODE = ACTION_SELECT_PAGE_CODE

Bool action_select_page( void )
{
    if( Current_Act.act_info.Num_Change_Mode >= News_Book.Num_GoTable )
        return FALSE;
    Search_Record.news_sys.ap3_aeb_flag = 1;
    Current_Record.Page_num = Current_Act.act_info.Num_Change_Mode;
    State_Search = Page_Search;
    ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
    return TRUE;
}


#pragma CODE = ACTION_SELECT_BOOK_CODE

Bool action_select_book( void )
{
    if( Coor_Type == MOBILE_COOR_MODE ) {
        if( !Current_Act.act_info.Num_Change_Mode )
            return FALSE;
        if( Current_Act.act_info.Num_Change_Mode > All_Control.Num_Book )
            return FALSE;
        Current_Act.act_info.Num_Change_Mode--;
    } else {
        if( Current_Act.act_info.Num_Change_Mode >= All_Control.Num_Book )
            return FALSE;
    }
    Search_Record.news_sys.ap3_aeb_flag = 1;
    Current_Record.Book_num = Current_Act.act_info.Num_Change_Mode;
    State_Search = Book_Search;
    ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
    return TRUE;
}


#pragma CODE = ACTION_SEQUENCE_CODE

Bool action_play_sequence_deal( void )
{
    U32 _MEM_TYPE_SLOW_ Iaddress = action_address + 2;

    //Search_Record.news_sys.sequence_flag = 1;
    read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act_Sequence, ( U16 )sizeof( Action_Sequence ));
    Iaddress += sizeof( Action_Sequence ) + 2;
    read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&Current_Act.act_info.speech_info.Num_Voice, 3 );

    if( ap3_mode == SINGLE_PLAY_MODE ) {
        /*SDKLoadCode( SEARCH_ACTION_TEMP_CODE01 );
        voice_play_number();*/
        return TRUE;
    } else if( ap3_mode == SEQUENCE_PLAY_MODE ) {
        sequence_speech_address = Current_Act_Sequence.Speech_Start_Address;
        sequence_speech_read_point = sequence_speech_address;
        sequence_text_address = Current_Act_Sequence.Text_Start_Address;
        State_Search = Sequence_Search;
    }
    return FALSE;
}


#pragma CODE = SEARCH_ACTION_REPEAT_CODE

void action_play_repeat_display( void )
{
    action_repeat_display = FALSE;

    Search_Record.news_sys.repeat_flag = 1;
///*
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, 32 );
    lcd_goto_xy( 15, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_puts(( U8 *)"Repeating" );
#elif( LANGUAGE == Simplified )
    lcd_puts(( U8 *)"复读中" );
#elif( LANGUAGE == Traditional )
    lcd_puts(( U8 *)"確弄い" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_puts(( U8 *)"Repeating" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_puts(( U8 *)"复读中" );
            break;

        case LANG_CHINESE_TRAD:
            lcd_puts(( U8 *)"確弄い" );
            break;
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
//*/
}


#pragma CODE = SEARCH_CODE

//! This fonction initialize the random table
//!
void ap3_rand_init( void )
{
    // Init rand table
    memset( g_ap3_rand_tab, 0, AP3_MAX_RANGE_RAND );
}


#pragma CODE = SEARCH_ACTION_TEMP_CODE01

Bool ap3_rand_table_is_full( U8 number )
{
    //U8 _MEM_TYPE_SLOW_ i;
    //U8 _MEM_TYPE_SLOW_ number;
    if( Current_Act.act_info.speech_info.Num_Voice >( AP3_MAX_RANGE_RAND * 8 ))
        number = AP3_MAX_RANGE_RAND * 8;
    //else
    //number = Current_Act.act_info.speech_info.Num_Voice;
    //for( i = 0; i < number; i++ )
    while( number ) {
        number--;
        //if( 0 == ( g_ap3_rand_tab[ number / 8 ]&( 1 << ( number % 8 ))))
        if( 0 == ( g_ap3_rand_tab[ number >> 3 ]&( 1 << ( number & 0x07 )))) {
            return FALSE;
        }
    }
    return TRUE;
}


#pragma CODE = SEARCH_RAND_CODE

U16 rand( void )
{
    Seed = Seed * 1103515245 + 12345;
    //return ( U16 )( Seed / 65536 )% 32768;
    return ( U16 )(( Seed >> 16 )& 0x7FFF );
}


void srand( U32 s )
{
    Seed = s;
}

#pragma CODE = FINDWORD_TEMP01_CODE
void ready_find_word(void)
{
    U32 _MEM_TYPE_SLOW_ Iaddress;
    Ih_set_curr_font( IDC_FONT_16 );
    findword_address = action_address + (U32)(Current_Act.act_info.speech_info.Num_Voice * 9 + 5);
    read_data_from_nf( findword_address, ( U8 _MEM_TYPE_SLOW_ *)&leng_en, ( U16 )sizeof( U16 ));
    Iaddress = findword_address + (U32)(leng_en+2);
    ch_begin_address = Iaddress;
    read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&leng_ch, ( U16 )sizeof( U16 ));
    firstword_address = Iaddress + (U32)(leng_ch+2);
    read_data_from_nf( firstword_address, ( U8 _MEM_TYPE_SLOW_ *)&Display_Page_Num, ( U16 )sizeof( U16));
    firstword_address += (U32)2;
    //SendString((U8 *)"\r\nadd:len_en:len_ch:first_addr:wordnum=");
    //send_hex_u32(findword_address);
    //send_hex_u16(leng_en);send_hex_u16(leng_ch);send_hex_u32(firstword_address);send_hex_u16(findword_num);
}

#pragma CODE = ACTION_TEXT_WORDNUM_CODE

Bool determine_nochar(U8 temp_char)
{
    if((temp_char == 32)||(temp_char == 44)||(temp_char == 46))return TRUE;
    return FALSE;
}

void ap4_word_num( void )
{
#define TEXT_BUF_LEN    64
    U8 _MEM_TYPE_SLOW_ word_addr,i;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    //U16 _MEM_TYPE_SLOW_ text_length;
    U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];


    Iaddress = findword_address + ( U32 )2;
    if(display_word_length > TEXT_BUF_LEN) {
        display_word_length = TEXT_BUF_LEN;
    }
    allnum_word = 0;
    num_current_word = 1;
    en_word_bf[0] = 0;
    read_data_from_nf( Iaddress, text_buf, display_word_length );
    word_addr = 1;
    for(i=0; i<display_word_length; i++) {
        if(determine_nochar(text_buf[i])) {
            en_word_bf[word_addr] = i;
            word_addr ++;
            if(word_addr >= LEN_EN_WORDBF)return;
            allnum_word += 1;
        }
    }
    en_word_bf[word_addr] = display_word_length;
    allnum_word += 1;
#undef TEXT_BUF_LEN
}

#pragma CODE = ACTION_TEXT_ONELINE_CODE
void text_display_oneline(U8 line_num,U8 oneline_begin)
{
#define TEXT_BUF_LEN    32
    U8 _MEM_TYPE_SLOW_ i,word_len;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;
    U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];


    Iaddress = findword_address + ( U32 )2;
    memset( text_buf, 0, TEXT_BUF_LEN);
    lcd_goto_xy( 0, 22 );
    read_data_from_nf( Iaddress, text_buf, oneline_begin);
    SendString((U8 *)"\r\n1_add:len:buf=");
    send_hex_u32(Iaddress);
    SendString((U8 *)":");
    send_hex_u16(oneline_begin);

    lcd_puts( text_buf );

#undef TEXT_BUF_LEN
}

#pragma CODE = ACTION_TEXT_TWOLINE_CODE
void text_display_twoline(U8 line_num,U8 twoline_begin)
{
#define TEXT_BUF_LEN    32
    U8 _MEM_TYPE_SLOW_ i,word_len;
    U32 _MEM_TYPE_SLOW_ Iaddress,temp_add;
    U16 _MEM_TYPE_SLOW_ text_length;
    U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];

    fenjie_num = twoline_begin+1;
    temp_add = (U32)(twoline_begin + 3);
//  Iaddress = findword_address + temp_add;
    Iaddress = findword_address + temp_add;
    memset( text_buf, 0, TEXT_BUF_LEN);

//        b_disptime = 1;
    lcd_goto_xy( 0, 38 );
    word_len = allnum_word;

    text_length = (U16)(en_word_bf[allnum_word] - twoline_begin);
    SendString((U8 *)"\r\n2_add:len=");
    send_hex_u32(Iaddress);
    SendString((U8 *)":");
    send_hex_u16(text_length);
    SendString((U8 *)":");

    read_data_from_nf( Iaddress, text_buf,text_length );
    /*
    for(i = 0;i<32;i++){
            send_hex_u8(text_buf[i]);
            }
       */
    lcd_puts( text_buf );

#undef TEXT_BUF_LEN
}

#pragma CODE = ACTION_TEXT_DISPLAY_CODE
#if 1
void ap4_text_display( void )
{
//#define TEXT_BUF_LEN    64
    //U8 _MEM_TYPE_SLOW_ old_lang;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;
//  U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];
    SendString((U8*)"\r\na_d_t");
    //action_display_text = FALSE;
    //if(!g_b_enable_flush)return;
    g_b_enable_flush = FALSE;
    Ih_set_curr_font( IDC_FONT_16 );
///*
    // if(Current_Act.Type == Type_Play){
    if(All_Control.Version < 15) {
        lcd_goto_xy( 12, 32 );
        if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP) {
            lcd_puts((U8 *)"本书无屏显内容");
        } else {
            //lcd_puts((U8 *)"No Unit");
            lcd_puts((U8 *)"No display content");
        }
        return;
    }

    if(!action_display_text) {
        //   if(State_Search >= Browser_File_Init )return;
        //if()
        leng_ch = Len_Cap_Book;
        ch_begin_address = News_Book.Point_Book_Cap + ( U32 )4;;
    } else {

        Iaddress = action_address + ( U32 )5;

        read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&text_length, ( U16 )sizeof( U16 ));
        if( !text_length )
            return;
        leng_ch = text_length;
        ch_begin_address = action_address + ( U32 )9;
    }
//  g_b_ap4_caption = TRUE;

    action_display_text = FALSE;
    //SDKLoadCode( FINDWORD_TEMP01_CODE );
    //ready_find_word();
    current_display_num = 0;
    Display_Page_Num = 0;
    b_LastPage_Flag = FALSE;
    if(0 == leng_ch)return;
    //findword_address = action_address + (U32)(Current_Act.act_info.speech_info.Num_Voice * 9 + 5);
    //ch_begin_address += (U32)4;
    //ch_begin_address = action_address + ( U32 )9;
    leng_ch -= (U16)2;
//   ch_begin_address   += (U32)leng_en;
    memset(text_off_addr,0,MAX_PAGE_NUMBER);
    ch_display_address = ch_begin_address;
    //b_findword_ch_display = TRUE;
    mail_send_event( EVT_SEARCH_FLUSH_TEXT, 0);
    SendString((U8*)"\r\nt_d:l=");
    send_hex_u32(ch_begin_address);
    SendString((U8*)":");
    send_hex_u16(leng_ch);
    text_line_buf[0] = 0xFF;
    text_line_buf[1] = 0xFE;
    text_line_buf[(LEN_DISPLAY_BUF-1)] = 0;
    text_line_buf[(LEN_DISPLAY_BUF-2)] = 0;
    g_b_flush_display = TRUE;
    g_b_first_display_flag = TRUE;
    /*
      memset( text_buf, 0, TEXT_BUF_LEN );
      SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
      lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );

      old_lang = g_setting_list.struct_setting.language_ihm;
      g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

      Ih_set_curr_font( IDC_FONT_16 );
      lcd_goto_xy( 0, 24 );
      if(text_length > 32){
       temp_len = 32;
       }
      else{
       temp_len = text_length;
       }
      read_data_from_nf( Iaddress + 2, text_buf, temp_len );
      text_buf[temp_len] = 0;
      lcd_puts( text_buf );

      if( text_length > 32 )
      {
         lcd_goto_xy( 0, 40 );
      memset( text_buf, 0, TEXT_BUF_LEN );
       read_data_from_nf( Iaddress + 34, text_buf, TEXT_BUF_LEN - 2 );
       lcd_puts( text_buf );
      }
      g_setting_list.struct_setting.language_ihm = old_lang;

    #undef TEXT_BUF_LEN
    */
}
#else
void ap4_text_display( /*U32 address*/void )
{

    U8 _MEM_TYPE_SLOW_ old_lang,i,word_len,temp_line,temp_begin;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;


    action_display_text = FALSE;

    //Iaddress = address + ( U32 )sizeof( Action );
    //Iaddress = action_address + ( U32 )sizeof( Action );
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );

    Iaddress = action_address + ( U32 )5;

    read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&display_word_length, ( U16 )sizeof( U16 ));
    if(0 == display_word_length )
        return;

    SDKLoadCode(ACTION_TEXT_WORDNUM_CODE);
    ap4_word_num();
    g_b_ap4_caption = TRUE;

    old_lang = g_setting_list.struct_setting.language_ihm;
    g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

    Ih_set_curr_font( IDC_FONT_16 );
    /*
    //  send_hex_u8(en_word_bf[0]);
        for(i=0;i<16;i++){
            send_hex_u8(en_word_bf[i]);
            }
    */
    temp_line = 1;
    word_len = allnum_word;

    if(en_word_bf[word_len] > 32) {
        temp_line = 2;
        i = 0;
        do {
            i++;
        } while(en_word_bf[i]<=32);

        temp_begin = en_word_bf[i-1];
//  SendString((U8 *)"\r\n");
//  send_hex_u8(en_word_bf[i]);
    } else {
        temp_begin = en_word_bf[word_len];
    }

    SDKLoadCode(ACTION_TEXT_ONELINE_CODE);
    text_display_oneline(temp_line,temp_begin);
    if(temp_line > 1) {
        SDKLoadCode(ACTION_TEXT_TWOLINE_CODE);
        text_display_twoline(temp_line,temp_begin);
    }
    g_setting_list.struct_setting.language_ihm = old_lang;
}
#endif

#pragma CODE = FINDWORD_DISPLAY_CODE


void findword_display( void )
{

    U8 _MEM_TYPE_SLOW_ old_lang,i,word_len,temp_line,temp_begin;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;


    b_first_findword_display = FALSE;
    b_findword_ch_display = FALSE;
    SDKLoadCode( FINDWORD_TEMP01_CODE );
    ready_find_word();
    current_display_num = 0;
    Display_Page_Num = 0;
    b_LastPage_Flag = FALSE;
    if(0 == leng_ch)return;
    //findword_address = action_address + (U32)(Current_Act.act_info.speech_info.Num_Voice * 9 + 5);
    ch_begin_address += (U32)4;
    leng_ch -= (U16)2;
//   ch_begin_address   += (U32)leng_en;
    memset(text_off_addr,0,MAX_PAGE_NUMBER);
    ch_display_address = ch_begin_address;
    b_findword_ch_display = TRUE;
    mail_send_event( EVT_SEARCH_FLUSH_TEXT, 0);
    SendString((U8*)"\r\naddr:leng=");
    send_hex_u32(ch_begin_address);
    SendString((U8*)":");
    send_hex_u16(leng_ch);
    text_line_buf[0] = 0xFF;
    text_line_buf[1] = 0xFE;
    text_line_buf[(LEN_DISPLAY_BUF-1)] = 0;
    text_line_buf[(LEN_DISPLAY_BUF-2)] = 0;
    g_b_first_display_flag = TRUE;
    /*
       SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
       lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );

     //  Iaddress = action_address + ( U32 )5;

    //   read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&display_word_length, ( U16 )sizeof( U16 ));
       display_word_length = leng_en;
       if(0 == display_word_length )
          return;

       SDKLoadCode(ACTION_TEXT_WORDNUM_CODE);
       ap4_word_num();



       old_lang = g_setting_list.struct_setting.language_ihm;
       g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

       temp_line = 1;
       word_len = allnum_word;

       if(en_word_bf[word_len] > 32){
        temp_line = 2;
        i = 0;
        do{
            i++;
            }while(en_word_bf[i]<=32);

        temp_begin = en_word_bf[i-1];
        //SendString((U8 *)"\r\n");
        send_hex_u8(en_word_bf[i]);
        }
       else{
        temp_begin = en_word_bf[word_len];
        }

       SDKLoadCode(ACTION_TEXT_ONELINE_CODE);
       text_display_oneline(temp_line,temp_begin);
       if(temp_line > 1){
         SDKLoadCode(ACTION_TEXT_TWOLINE_CODE);
         text_display_twoline(temp_line,temp_begin);
        }
       g_setting_list.struct_setting.language_ihm = old_lang;

       SDKLoadCode( DISPLAY_FIRSTWORD_CODE );
       display_firstword();
    */
}

/*
void findword_display( void )
{

   U8 _MEM_TYPE_SLOW_ old_lang,i,word_len,temp_line,temp_begin;
   U32 _MEM_TYPE_SLOW_ Iaddress;
   U16 _MEM_TYPE_SLOW_ text_length;


   b_first_findword_display = FALSE;

   SDKLoadCode( FINDWORD_TEMP01_CODE );
   ready_find_word();

   SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );

 //  Iaddress = action_address + ( U32 )5;

//   read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&display_word_length, ( U16 )sizeof( U16 ));
   display_word_length = leng_en;
   if(0 == display_word_length )
      return;

   SDKLoadCode(ACTION_TEXT_WORDNUM_CODE);
   ap4_word_num();



   old_lang = g_setting_list.struct_setting.language_ihm;
   g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

   temp_line = 1;
   word_len = allnum_word;

   if(en_word_bf[word_len] > 32){
    temp_line = 2;
    i = 0;
    do{
        i++;
        }while(en_word_bf[i]<=32);

    temp_begin = en_word_bf[i-1];
    //SendString((U8 *)"\r\n");
    send_hex_u8(en_word_bf[i]);
    }
   else{
    temp_begin = en_word_bf[word_len];
    }

   SDKLoadCode(ACTION_TEXT_ONELINE_CODE);
   text_display_oneline(temp_line,temp_begin);
   if(temp_line > 1){
     SDKLoadCode(ACTION_TEXT_TWOLINE_CODE);
     text_display_twoline(temp_line,temp_begin);
    }
   g_setting_list.struct_setting.language_ihm = old_lang;

   SDKLoadCode( DISPLAY_FIRSTWORD_CODE );
   display_firstword();
}
*/
#pragma CODE = TEXT_BACKDISPLAY_CODE
void text_backdisplay( void )
{

    U8 _MEM_TYPE_SLOW_ old_lang,i,word_len,temp_line,temp_begin;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;

    b_check_word = TRUE;
    b_ready_findword = TRUE;
    b_fanyi      = FALSE;
    b_fanyi_back_display = FALSE;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );


    old_lang = g_setting_list.struct_setting.language_ihm;
    g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

    Ih_set_curr_font( IDC_FONT_16 );
    /*
    //  send_hex_u8(en_word_bf[0]);
        for(i=0;i<16;i++){
            send_hex_u8(en_word_bf[i]);
            }
    */
    temp_line = 1;
    word_len = allnum_word;

    if(en_word_bf[word_len] > 32) {
        temp_line = 2;
        i = 0;
        do {
            i++;
        } while(en_word_bf[i]<=32);

        temp_begin = en_word_bf[i-1];
//  SendString((U8 *)"\r\n");
//  send_hex_u8(en_word_bf[i]);
    } else {
        temp_begin = en_word_bf[word_len];
    }

    SDKLoadCode(ACTION_TEXT_ONELINE_CODE);
    text_display_oneline(temp_line,temp_begin);
    if(temp_line > 1) {
        SDKLoadCode(ACTION_TEXT_TWOLINE_CODE);
        text_display_twoline(temp_line,temp_begin);
    }
    g_setting_list.struct_setting.language_ihm = old_lang;

    SDKLoadCode(LCD_LINE_WRITE_CODE);
    lcd_write_line(u8_date[0], u8_date[1], u8_date[2]);
}

#pragma CODE = TEXT_CH_DISPLAY_CODE
Bool text_chinese_display( U32 fan_add )
{
//#define TEXT_BUF_LEN    42
    U8 _MEM_TYPE_SLOW_ old_lang;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length,temp_len,all_len = 0;
    U16 i,j,m,temp_file;
    Bool temp_flag;
    __X FS_ERR fs_err;
    __X FCFILE *Temp_File;

    //U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];

//   b_findword_ch_display = FALSE;
    Iaddress = fan_add;
    g_b_first_display_flag = FALSE;
//  memset( text_buf, 0, TEXT_BUF_LEN );
    //text_buf[0] = 0xFF;
    //text_buf[1] = 0xFE;

#if 1//( UNICODE_TYPE == DISABLE )
    //strcpy( tisiFileShortpath,AP4_PATH);
    strcpy( tisiFileShortpath,(char *)&current_ap4_short_path);
    strcat( tisiFileShortpath,(char *)&current_ap4_short_name);
#endif
    //strcpy( tisiFileShortpath,(char *)&current_ap4_short_name);


    //SendString((U8 *)tisiFileShortpath);
    Temp_File = ( __X FCFILE *)FSOpen( tisiFileShortpath, FS_MODE_READ, &fs_err );
    if(fs_err != FS_NO_ERROR)return FALSE;

    old_lang = g_setting_list.struct_setting.language_ihm;
    g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
    Ih_set_curr_font( IDC_FONT_16 );
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
    b_LastPage_Flag = FALSE;
    for(i=0; i<AP4_LINE_PAGE; i++ ) {
        if((leng_ch <= current_display_num))break;

        temp_len = leng_ch - current_display_num;
        if(temp_len >= MAX_NUM_LINE) {
            temp_len = MAX_NUM_LINE;
        }
        //current_display_num += temp_len;
        (void)FSSeek( Temp_File, Iaddress );
        FSRead(Temp_File, (U8 _MEM_TYPE_SLOW_ *)&text_line_buf[2], temp_len, &fs_err);
//   read_data_from_nf( Iaddress, (U8 _MEM_TYPE_SLOW_ *)&text_line_buf[2], temp_len );
        text_line_buf[temp_len+2] = 0;
        text_line_buf[(temp_len+3)] = 0;
        //SendString((U8 *)"\r\naddress:leng=");
        //send_hex_u32(Iaddress);SendString((U8 *)":");send_hex_u16(temp_len);
        lcd_goto_xy( 0, 8+i*20 );
        lcd_puts( text_line_buf );
///*
        // SendString((U8 *)"\r\n1:2:3=");
        SendString((U8 *)"\r\nlen_1:leng_2=");
        send_hex_u16(temp_len);
        //SendString((U8 *)":");
        send_hex_u16(lcd_display_num);
//*/
        temp_len = (lcd_display_num)<<1;
        all_len += temp_len;
        current_display_num += temp_len;
        //send_hex_u16(current_display_num);
        Iaddress += (U32)temp_len;
    }
    g_setting_list.struct_setting.language_ihm = old_lang;
    FSClose(Temp_File);
    if(current_display_num >= leng_ch) {
        b_LastPage_Flag = TRUE;
    }
    Display_Page_Num ++;
    if(Display_Page_Num >= MAX_PAGE_NUMBER) {
        b_LastPage_Flag = TRUE;
    } else {
        text_off_addr[Display_Page_Num] = text_off_addr[(Display_Page_Num-1)] + all_len;
    }
    //SendString((U8 *)"\r\npage_num:off=");
    //send_hex_u16(Display_Page_Num);SendString((U8 *)":");send_hex_u16(all_len);
//#undef TEXT_BUF_LEN
    return TRUE;
}

#pragma CODE = CTRL_DISPLAY_READY_CODE
Bool ctrl_display_ready( U16 param )
{
//#define TEXT_BUF_LEN    42
    U8 _MEM_TYPE_SLOW_ old_lang,curHeigh,tempOff;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length,temp_len,all_len = 0;
    U16 i,j,m;
    Bool temp_flag;

    //Iaddress = param;
    //g_b_first_display_flag = FALSE;
//  memset( text_buf, 0, TEXT_BUF_LEN );
    //text_buf[0] = 0xFF;
    //text_buf[1] = 0xFE;
    SendString((U8 *)"\r\nctrFlush = ");
    send_hex_u16(param);
    //old_lang = g_setting_list.struct_setting.language_ihm;
    // g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
    memset( text_line_buf, 0, LEN_DISPLAY_BUF );
    text_line_buf[0] = 0xFF;
    text_line_buf[1] = 0xFE;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    if(param < NORMAL_MODE_DISPLAY) {
        //lcd_clear_part_of_screen(20, 0,64, 8);
        //lcd_goto_xy( 20, 0 );
        //Ih_set_curr_font( IDC_FONT_08);
        //lcd_puts(( U8 *)"READ" );
        Ih_set_curr_font( IDC_FONT_08 );
        lcd_clear_part_of_screen(20, 0,64, 8);
        lcd_goto_xy( 20, 0);
    } else {
        Ih_set_curr_font( IDC_FONT_16);
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
        lcd_goto_xy( 15, 24 );
    }
    return true;
    /*
    // text_line_buf[0] = 0xFF;
    //text_line_buf[1] = 0xFE;
    //text_line_buf[(LEN_DISPLAY_BUF-1)] = 0;
    //text_line_buf[(LEN_DISPLAY_BUF-2)] = 0;
    //Ih_set_curr_font( displayNews.heigh );
    if(param == READ_CTRL_DISPLAY){
    //READ
    lcd_puts(( U8 *)"READ" );
    }else if(param == REPEAT_CTRL_DISPLAY){
    //READ
    lcd_puts(( U8 *)"REPEAT" );
    }else if(param == REPEAT_MODE_DISPLAY){
    //录音跟读
    tempOff = 4;
    text_line_buf[tempOff++] = 0xC2; text_line_buf[tempOff++] = 0xBC;
    text_line_buf[tempOff++] = 0xD2; text_line_buf[tempOff++] = 0xF4;
    text_line_buf[tempOff++] = 0xB8; text_line_buf[tempOff++] = 0xFA;
    text_line_buf[tempOff++] = 0xB6; text_line_buf[tempOff++] = 0xC1;
    text_line_buf[2] = 0; text_line_buf[3] = 4;
    lcd_puts(text_line_buf);

    }else if(param == READ_ALOUD_MODE_DISPLAY){

    //单词朗读
    text_line_buf[2] = 0xB5; text_line_buf[3] = 0xA5;
    text_line_buf[4] = 0xB4; text_line_buf[5] = 0xCA;
    text_line_buf[6] = 0xC0; text_line_buf[7] = 0xCA;
    text_line_buf[8] = 0xB6; text_line_buf[9] = 0xC1;
    lcd_puts(text_line_buf);

    }else if(param == NORMAL_MODE_DISPLAY){
    //普通点读模式

    text_line_buf[2] = 0xC6; text_line_buf[3] = 0xD5;
    text_line_buf[4] = 0xCD; text_line_buf[5] = 0xA8;
    text_line_buf[6] = 0xB5; text_line_buf[7] = 0xE3;
    text_line_buf[8] = 0xB6; text_line_buf[9] = 0xC1;
    text_line_buf[10] = 0xC4; text_line_buf[11] = 0xA3;
    text_line_buf[12] = 0xCA; text_line_buf[13] = 0xBD;
    lcd_puts(text_line_buf);

    }else if(param == AP4_SINGLE_MODE_DISPLAY){
    //AP4 控制方式读取
    curHeigh = 16;
    i = 0;
    while(curHeigh > 0){
        m = News_Control.Caption_Control[i];
        if(m == 0 )break;
        text_line_buf[4+i] = m;
        i ++;
        curHeigh --;
    }
    i = i>>1;
    text_line_buf[2] = MSB(i); text_line_buf[3] = LSB(i);
    lcd_puts(text_line_buf);
    }

    g_setting_list.struct_setting.language_ihm = old_lang;
    return TRUE;
    //*/
}
#pragma CODE = CTRL_DISPLAY_CODE
Bool ctrl_display( U16 param )
{
//#define TEXT_BUF_LEN    42
    U8 _MEM_TYPE_SLOW_ old_lang,curHeigh,tempOff;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length,temp_len,all_len = 0;
    U16 i,j,m;
    Bool temp_flag;

    //SendString((U8 *)"\r\nctrFlush = ");send_hex_u16(param);
    old_lang = g_setting_list.struct_setting.language_ihm;
    g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
    /*
    memset( text_line_buf, 0, LEN_DISPLAY_BUF );
    text_line_buf[0] = 0xFF;
    text_line_buf[1] = 0xFE;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    if(param < NORMAL_MODE_DISPLAY){
     //lcd_clear_part_of_screen(20, 0,64, 8);
     //lcd_goto_xy( 20, 0 );
     //Ih_set_curr_font( IDC_FONT_08);
     //lcd_puts(( U8 *)"READ" );
     Ih_set_curr_font( IDC_FONT_08 );
     lcd_clear_part_of_screen(20, 0,64, 8);
     lcd_goto_xy( 20, 0);
    }else{
     Ih_set_curr_font( IDC_FONT_16);
     lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8));
     lcd_goto_xy( 32, 12 );
    }
    */

    // text_line_buf[0] = 0xFF;
    //text_line_buf[1] = 0xFE;
    //text_line_buf[(LEN_DISPLAY_BUF-1)] = 0;
    //text_line_buf[(LEN_DISPLAY_BUF-2)] = 0;
    //Ih_set_curr_font( displayNews.heigh );
    if(param == READ_CTRL_DISPLAY) {
        //READ
        lcd_puts(( U8 *)"READ" );
    } else if(param == REPEAT_CTRL_DISPLAY) {
        //READ
        //lcd_puts(( U8 *)"REPEAT" );
        lcd_puts(( U8 *)"RECORD" );
    } else if(param == REPEAT_MODE_DISPLAY) {
        //录音跟读
        //tempOff = 2;
        //text_line_buf[tempOff++] = 0xC2;text_line_buf[tempOff++] = 0xBC;
        //text_line_buf[tempOff++] = 0xD2;text_line_buf[tempOff++] = 0xF4;
        //text_line_buf[tempOff++] = 0xB8;text_line_buf[tempOff++] = 0xFA;
        //text_line_buf[tempOff++] = 0xB6;text_line_buf[tempOff++] = 0xC1;

        // text_line_buf[2] = 0;           text_line_buf[3] = 4;
        lcd_puts((U8 *)"跟读对比");//lcd_puts((U8 *)"录音跟读");

    } else if(param == READ_ALOUD_MODE_DISPLAY) {
        /*
        //单词朗读
        text_line_buf[2] = 0xB5; text_line_buf[3] = 0xA5;
        text_line_buf[4] = 0xB4; text_line_buf[5] = 0xCA;
        text_line_buf[6] = 0xC0; text_line_buf[7] = 0xCA;
        text_line_buf[8] = 0xB6; text_line_buf[9] = 0xC1;
        lcd_puts(text_line_buf);
        */
    } else if(param == NORMAL_MODE_DISPLAY) {
        //普通点读模式
        lcd_puts((U8 *)"普通点读模式");
        /*
        text_line_buf[2] = 0xC6; text_line_buf[3] = 0xD5;
        text_line_buf[4] = 0xCD; text_line_buf[5] = 0xA8;
        text_line_buf[6] = 0xB5; text_line_buf[7] = 0xE3;
        text_line_buf[8] = 0xB6; text_line_buf[9] = 0xC1;
        text_line_buf[10] = 0xC4; text_line_buf[11] = 0xA3;
        text_line_buf[12] = 0xCA; text_line_buf[13] = 0xBD;
        lcd_puts(text_line_buf);
        //*/
    } else if(param == AP4_SINGLE_MODE_DISPLAY) {
        //AP4 控制方式读取
        // curHeigh = 0;
        i = 0;
        for(curHeigh = 0; curHeigh < Length_Caption; curHeigh += 2) {

            text_line_buf[i] = News_Control.Caption_Control[curHeigh];
            text_line_buf[i+1] = News_Control.Caption_Control[curHeigh+1];
            if((News_Control.Caption_Control[curHeigh] == 0) && (News_Control.Caption_Control[curHeigh] == 0)) {
                break;
            }
            i += 2;
        }
        //text_line_buf[2] = LSB(curHeigh); text_line_buf[3] = MSB(curHeigh);
        lcd_puts(text_line_buf);
    }

    g_setting_list.struct_setting.language_ihm = old_lang;
    return TRUE;
}

#pragma CODE = TEXT_NOFIND_DISPLAY_CODE
void nofind_display( void )
{
#define TEXT_BUF_LEN    64
    U8 _MEM_TYPE_SLOW_ old_lang;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length,temp_len;
    U8 _MEM_TYPE_SLOW_ text_buf[ TEXT_BUF_LEN ];


    SendString((U8 *)"\r\nno find dic");
    memset( text_buf, 0, TEXT_BUF_LEN );
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT - 24) );

    old_lang = g_setting_list.struct_setting.language_ihm;
    g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;

    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 0, 24 );
    lcd_puts((U8 *)"词典中未找到");
    g_setting_list.struct_setting.language_ihm = old_lang;

#undef TEXT_BUF_LEN
}


#pragma CODE = DISPLAY_FIRSTWORD_CODE
void display_firstword( void )
{
    U8 i,j;
    j = allnum_word;
    SendString((U8 *)"\r\nn:en_bf=");
    send_hex_u8(allnum_word);
    for(i=0; i<j; i++) {
        send_hex_u8(en_word_bf[i]);
    }
    b_ready_findword = TRUE;
    b_fanyi    = FALSE;
    b_check_word = TRUE;
    b_fanyi_back_display = FALSE;
    u8_date[0] = FIRST_LINE_ADDR;
    u8_date[1] = 0;
    u8_date[2] = (en_word_bf[1]<<2);
    SDKLoadCode(LCD_LINE_WRITE_CODE);
    lcd_write_line(u8_date[0],u8_date[1],u8_date[2]);
}

#pragma CODE = SEARCH_ACTION_TEMP_CODE01

#if 1
//the function is deal with the action
void voice_play_number( /*U32 address*/void )
{
#if 1
    U8 _MEM_TYPE_SLOW_ i, j, m;
    U32 _MEM_TYPE_SLOW_ Iaddress;
    U16 _MEM_TYPE_SLOW_ text_length;

    if( !Current_Act.act_info.speech_info.Num_Voice )
        return;
    //*address += ( U32 )sizeof( Action );
    //ap3_rand_init();


    SDKLoadCode( SEARCH_RAND_CODE );
    srand( timer_get_time());
    for( i = 0; i < Current_Act.Num_Play; i++ ) {
        //Iaddress = address + ( U32 )5;
        Iaddress = action_address + ( U32 )5;

        if( Current_Act.act_info.speech_info.Mode_Play == Random_Play ) {
            if( i >= Current_Act.act_info.speech_info.Num_Voice )
                ap3_rand_init();
            //srand( timer_get_time());
            while( 1 ) {
                if( ap3_rand_table_is_full( Current_Act.act_info.speech_info.Num_Voice ))
                    ap3_rand_init();
                m = rand() % Current_Act.act_info.speech_info.Num_Voice;
                //j = m %( AP3_MAX_RANGE_RAND * 8 );
                //if( 0 == ( g_ap3_rand_tab[ j / 8 ]&( 1 << ( j % 8 ))))
                j = m &( AP3_MAX_RANGE_RAND * 8 - 1 );
                if( 0 == ( g_ap3_rand_tab[ j >> 3 ]&( 1 << ( j & 0x07 )))) {
                    //g_ap3_rand_tab[ j / 8 ] |= 1 << ( j % 8 ); // Update table
                    g_ap3_rand_tab[ j >> 3 ] |= 1 << ( j & 0x07 ); // Update table
                    break;   // A no used position find
                }
            }
        } else if( Current_Act.act_info.speech_info.Mode_Play == Order_Play ) {
            m = i;
            if( m >= Current_Act.act_info.speech_info.Num_Voice )
                m %= Current_Act.act_info.speech_info.Num_Voice;
        } else if( Current_Act.act_info.speech_info.Mode_Play == Order_Play_R ) {
            if( order_play_count >= Current_Act.act_info.speech_info.Num_Voice )
                order_play_count = 0;
            m = order_play_count;
            order_play_count++;
        } else
            return;

        switch( Current_Act.Type ) {
            case Type_Play:
            case Type_play_Reread:
            case Type_play_Repeat:
            case Type_Play_Sequence:
            case Type_Play_Findword:
                Iaddress += ( U32 )m * 9;
                break;

            case Type_Play_Title:
            case Type_play_Caption:
                read_data_from_nf( Iaddress, ( U8 _MEM_TYPE_SLOW_ *)&text_length, ( U16 )sizeof( U16 ));
                if( All_Control.Version >= 2 )
                    Iaddress += ( U32 )text_length + 2 +( U32 )m * 9;
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
#endif
}
#endif


#pragma CODE = SEARCH_CODE

// record the news of the media
void write_media_addr( U32 address, U8 flag )
{
    if( Point_Write >=( Length_v - 1 )) {
        return;
    }
    Array_V[ Point_Write ].Addr_Speech = address;
    Array_V[ Point_Write ].Flag_Speech = flag;
    Point_Write++;
}


//#pragma CODE = SEARCH_TEMP_CODE07
#pragma CODE = JUDGE_IMMOBILITY_COOR_CODE
void judge_immobility_coor( void )
{
    if( bInCal )
        return;
    SDKLoadCode( SEARCH_TWOBYTES_COOR_CODE );
    if(( Current_Index >= Index_Range.Normal_Index_Min )
       &&( Current_Index <= Index_Range.Normal_Index_Max )) {
        if( Ap3_MenuPath < CONTROL_MENU_PATH )
            return;
        State_Search = Normal_Search;
    } else if(( Current_Index >= Index_Range.Book_Index_Min )
              &&( Current_Index <= Index_Range.Book_Index_Max )) {
        if( Ap3_MenuPath < BOOK_MENU_PATH )
            return;
        if( search_twobytes_coordinate( All_Control.Point_Gloablext_Coor, All_Control.Num_Book )) {
            Current_Record.Book_num = Current_Coor.X0;
            State_Search = Book_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
            return;
        }
        if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
    } else if(( Current_Index >= Index_Range.Page_Index_Min )
              &&( Current_Index <= Index_Range.Page_Index_Max )) {
        if( Ap3_MenuPath < PAGE_MENU_PATH )
            return;
        if( search_twobytes_coordinate( News_Book.Point_GOTable_Coordinate, News_Book.Num_GoTable )) {
            Current_Record.Page_num = Current_Coor.X0;
            State_Search = Page_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
            return;
        }
        if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
    } else if(( Current_Index >= Index_Range.Control_Index_Min )
              &&( Current_Index <= Index_Range.Control_Index_Max )) {
        if( Ap3_MenuPath < CONTROL_MENU_PATH )
            return;
        if( search_twobytes_coordinate( News_Go.Point_PageCoorTable, News_Go.Num_ControlTab )) {
            Temp_Control_num = Current_Record.Control_num;
            Current_Record.Control_num = Current_Coor.X0;
            State_Search = Control_Mode_Search;
            ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
            return;
        }
        if( Ap3_MenuPath >= CONTROL_MENU_PATH )
            ( void )act_do( News_Go.Action_NoVoice, 0 );
    } else {
        if( Ap3_MenuPath >= CONTROL_MENU_PATH ) {
            g_b_enable_flush = TRUE;
            ( void )act_do( News_Go.Action_NoVoice, 0 );
        }
    }
}


//#pragma CODE = SEARCH_TEMP_CODE08
#pragma CODE = JUDGE_MOBILITY_COOR_CODE
void judge_mobility_coor( void )
{
    if( bInCal )
        return;
    Current_Index = ( U16 )((( U32 )Current_Index +( U32 )All_Control.Max_Code -( U32 )All_Control.Excursion_Code )%( U32 )All_Control.Max_Code );
    X_Coor = Current_Index % All_Control.Rang_X;
    Y_Coor = Current_Index / All_Control.Rang_X;
    /*if(( Ap3_MenuPath >= BOOK_MENU_PATH )
       && search_eightbytes_coordinate( All_Control.Point_Gloablext_Coor, All_Control.Num_Book + 1 ))
    {
       if( Current_Coor.X0 )
       {
          Current_Record.Book_num = Current_Coor.X0 - 1;
          State_Search = Book_Search;
          ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
          return;
       }
       act_do( All_Control.Action_Xuanshu, 0 );
       return;
    }
    else if(( Ap3_MenuPath >= PAGE_MENU_PATH )
       && search_eightbytes_coordinate( News_Book.Point_GOTable_Coordinate, News_Book.Num_GoTable ))
    {
       Current_Record.Page_num = Current_Coor.X0;
       State_Search = Page_Search;
       ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
       return;
    }
    else*/ if( Ap3_MenuPath >= CONTROL_MENU_PATH ) {
        /*if( search_eightbytes_coordinate( News_Go.Point_PageCoorTable, News_Go.Num_ControlTab ))
        {
           Current_Record.Control_num = Current_Coor.X0;
           State_Search = Control_Mode_Search;
           ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
           return;
        }*/
        State_Search = Normal_Search;
    }
}


//#pragma CODE = SEARCH_TEMP_CODE09
#pragma CODE = JUDGE_MIX_COOR_CODE
void judge_mix_coor( void )
{
    if( bInCal )
        return;
    SDKLoadCode( SEARCH_TWOBYTES_COOR_CODE );
    if(( Ap3_MenuPath >= BOOK_MENU_PATH )
       && search_twobytes_coordinate( All_Control.Point_Gloablext_Coor, All_Control.Num_Book )) {
        Current_Record.Book_num = Current_Coor.X0;
        State_Search = Book_Search;
        ap3_poweron_state = MMI_AP3_CHANGE_BOOK;
        return;
    } else if(( Ap3_MenuPath >= PAGE_MENU_PATH )
              && search_twobytes_coordinate( News_Book.Point_GOTable_Coordinate, News_Book.Num_GoTable )) {
        Current_Record.Page_num = Current_Coor.X0;
        State_Search = Page_Search;
        ap3_poweron_state = MMI_AP3_CHANGE_PAGE;
        return;
    } else {
        Current_Index = ( U16 )((( U32 )Current_Index +( U32 )All_Control.Max_Code -( U32 )All_Control.Excursion_Code )%( U32 )All_Control.Max_Code );
        X_Coor = Current_Index % All_Control.Rang_X;
        Y_Coor = Current_Index / All_Control.Rang_X;
        if( Ap3_MenuPath >= CONTROL_MENU_PATH ) {
            SDKLoadCode( SEARCH_GAME_CODE );
            if( search_eightbytes_coordinate( News_Go.Point_PageCoorTable, News_Go.Num_ControlTab )) {
                Temp_Control_num = Current_Record.Control_num;
                Current_Record.Control_num = Current_Coor.X0;
                State_Search = Control_Mode_Search;
                ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
                return;
            }
            State_Search = Normal_Search;
        }
    }
}


#pragma CODE = SEARCH_OPEN_FILE_FAIL_CODE

void search_open_file_fail_deal( void )
{
    if( !g_b_open_file_error ) {
        g_b_open_file_error = TRUE;
        //if( scank_disk())
        if( ap3_file_number )
            Current_Record.Ap3_num = 0;
        else {
            //mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
            mail_send_event( EVT_SEARCH_AP3EXIT, 0 );
            state_search_to_error();
        }
    } else {
        if( ap4_change_file == MMI_AP3_KEY_PREV_CHANGE_FILE ) {
            if( !Current_Record.Ap3_num ) {
                Current_Record.Ap3_num = ap3_file_number - 1;
            } else
                Current_Record.Ap3_num--;
            return;
        }
        if( ++Current_Record.Ap3_num >= ap3_file_number ) {
            if( ap4_change_file == MMI_AP3_KEY_NEXT_CHANGE_FILE ) {
                ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
                Current_Record.Ap3_num = 0;
                return;
            }
            //mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
            mail_send_event( EVT_SEARCH_AP3EXIT, 0 );
            state_search_to_error();
        }
    }
}


#pragma CODE = AP4_SEARCH_CODE

void ap4_search_deal( void )
{
    Ap3_MenuPath = BOOK_MENU_PATH;
#if( DEFAULT_APPLI == 3 )
    if( !( internal_speech & STARTUP_SPEECH )
        ||(( ap3_poweron_state != MMI_AP3_POWERON )&&( ap3_poweron_state != MMI_AP3_NORMAL )))
#endif   //#if( DEFAULT_APPLI == 3 )
    {
        ( void )act_do( All_Control.Addr_Action_Open, 0 );
    }

    //Coor_Type = All_Control.Mask_Gloablext & MSK_COOR_MODE;
    switch( All_Control.Mask_Gloablext &( SIMPLE_FLAG | SEQUENCE_FLAG )) {
        case SIMPLE_FLAG:
            ap3_mode = SINGLE_PLAY_MODE;
            break;

        case SEQUENCE_FLAG:
            ap3_mode = SEQUENCE_PLAY_MODE;
            break;

        case ( SIMPLE_FLAG | SEQUENCE_FLAG ):
            if(( ap3_poweron_state == MMI_AP3_NORMAL )
               ||( ap3_poweron_state == MMI_AP3_CHANGE_FILE )) {
                if( All_Control.Mask_Gloablext & DEFAULT_SEQUENCE_FLAG )
                    ap3_mode = SEQUENCE_PLAY_MODE;
                else
                    ap3_mode = SINGLE_PLAY_MODE;
            }
            break;
    }
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
    if( All_Control.Point_Book_Table && All_Control.Num_Book ) {
        if( All_Control.Mask_Gloablext & AUTO_FIRST_BOOK ) {
            Current_Record.Book_num = 0;
            State_Search = Book_Search;
            return;
        }
    }
    /*else
    {
       Search_Record.news_sys.disable_next_key_flag = 1;
    }*/
    state_search_to_play();
}


#pragma CODE = PAGE_SEARCH_CODE

void page_search_deal( void )
{
//***********************************************
    if( News_Go.Point_ControlTab && News_Go.Num_ControlTab )
        //test auto goto first control mode
    {
        SDKLoadCode( READ_CONTROLNEWS_CODE );
        for( Current_Record.Control_num = 0; Current_Record.Control_num < News_Go.Num_ControlTab; Current_Record.Control_num++ ) {
            read_control_mode_news( Current_Record.Control_num );
            if((( News_Control.Type_Control == SINGLE_TABLE_TYPE )&& News_Control.Point )
               ||( News_Control.Type_Control == SCRIPT_TYPE )
               ||(( News_Control.Type_Control == GAME_TYPE )&&( News_Control.Flag & BIT0 ))) {
                Search_Record.news_sys.table_flag = CONTROL_TABLE;
                Ap3_MenuPath = CONTROL_MENU_PATH;
                State_Search = Control_Mode_Search;
                Temp_Control_num = Current_Record.Control_num;
                return;
            }
        }
    }
    News_Control.Type_Control = 0;
    Current_Record.Control_num = 0;
//***********************************************
    //if( !( News_Go.Point_ControlTab && News_Go.Num_ControlTab ))
    {
        if( News_Go.Point_DefaultTab ) {
            if( Coor_Type == IMMOBILITY_COOR_MODE ) {
                Search_Record.news_sys.table_flag = DEFAULT_TABLE;
                //Search_Record.news_sys.disable_next_key_flag = 1;
                Ap3_MenuPath = CONTROL_MENU_PATH;
                state_search_to_play();
                return;
            } else {
                if( News_Go.Num_DefaultTab ) {
                    Search_Record.news_sys.table_flag = DEFAULT_TABLE;
                    //Search_Record.news_sys.disable_next_key_flag = 1;
                    Ap3_MenuPath = CONTROL_MENU_PATH;
                    state_search_to_play();
                    return;
                }
            }
        }

        if( News_Go.Point_BackgroudTab ) {
            if( Coor_Type == IMMOBILITY_COOR_MODE ) {
                Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
                //Search_Record.news_sys.disable_next_key_flag = 1;
                Ap3_MenuPath = CONTROL_MENU_PATH;
                state_search_to_play();
                return;
            } else {
                if( News_Go.Num_BackgroudTab ) {
                    Search_Record.news_sys.table_flag = BACKGROUND_TABLE;
                    //Search_Record.news_sys.disable_next_key_flag = 1;
                    Ap3_MenuPath = CONTROL_MENU_PATH;
                    state_search_to_play();
                    return;
                }
            }
        }
        ( void )act_do( News_Go.Action_NoVoice, 0 );
        Search_Record.news_sys.table_flag = 0;
        //Search_Record.news_sys.disable_next_key_flag = 1;
        Ap3_MenuPath = CONTROL_MENU_PATH;
        state_search_to_play();
        return;
    }
}


#pragma CODE = SEARCH_RELOAD_AP4_INFO_CODE

void reload_ap4_info_deal( void )
{
    SendString((U8 *)"\r\nreload_ap4_info_deal!");
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
#if 0//( AP4_ENCRYPT == ENABLE )
    SDKLoadCode( LCE_ENCRYPT_CODE );
    ( void )lce_sec();
#endif   //#if( AP4_ENCRYPT == ENABLE )
    if( All_Control.Point_Special_Action )
        read_data_from_nf( All_Control.Point_Special_Action, ( U8 _MEM_TYPE_SLOW_ *)&Current_Special_Action, sizeof( Special_Action ));
    SDKLoadCode( SEARCH_REMOTE_TEMP_CODE00 );
    remote_task_init();
    init_remote();
    SDKLoadCode( READ_BOOKNEWS_CODE );
    read_book_news(( U16 ) Current_Record.Book_num );
    SDKLoadCode( READ_PAGENEWS_CODE );
    read_page_news(( U16 ) Current_Record.Page_num );
    News_Control.Type_Control = 0;
    if( Search_Record.news_sys.table_flag == CONTROL_TABLE ) {
        SDKLoadCode( READ_CONTROLNEWS_CODE );
        Temp_Control_num = Current_Record.Control_num;
        read_control_mode_news( Current_Record.Control_num );
        if( News_Control.Type_Control == GAME_TYPE ) {
            read_data_from_nf( News_Control.Point, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews, 23 );
            read_data_from_nf( News_Control.Point + 23, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Time, 7 );
            read_data_from_nf( News_Control.Point + 30, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameNews.Point_Extern_Game_Project, sizeof( U32 ));

            if( All_Control.Version >= 7 && Current_GameNews.Point_Extern_Game_Project )
                read_data_from_nf( Current_GameNews.Point_Extern_Game_Project, ( U8 _MEM_TYPE_SLOW_ *)&GameProjectExternNum, sizeof( U8 ));
            GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
            GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
            SDKLoadCode( SEARCH_GAME_CODE );
            read_game_project_table( Current_GameNews.Point_Game_Project + CurrentGame * 24, &Current_Game );

            if( All_Control.Version >= 7 && Current_GameNews.Point_Extern_Game_Project ) {
                read_data_from_nf( Current_GameNews.Point_Extern_Game_Project + 1
                                   + ( U32 )CurrentGame * ( U32 )GameProjectExternNum,
                                   ( U8 _MEM_TYPE_SLOW_ *)&Addr_Count_Game, sizeof( U32 ));

                if( All_Control.Version >= 12 ) {
                    read_data_from_nf( Current_GameNews.Point_Extern_Game_Project + 1 + sizeof( U32 )
                                       + ( U32 )CurrentGame * ( U32 )GameProjectExternNum,
                                       ( U8 _MEM_TYPE_SLOW_ *)&Addr_Game_Wrong_Answer, sizeof( U32 ));
                }
            }
            AnswerCount = Current_Game.Num_Key;
            read_data_from_nf( Current_Game.Addr_Key_Index, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyIndex, sizeof( U8 )* AnswerCount );
            read_data_from_nf( Current_Game.Addr_key_Concent, ( U8 _MEM_TYPE_SLOW_ *)&Current_GameKeyConcent, sizeof( Game_concent_Table )* AnswerCount );

        }
    }

    if( bRemoteEnterCal ) {
        bRemoteEnterCal = FALSE;
        Search_Record.news_sys.flag_index = TRUE;
        Current_Index = remote_index_tab[ REMOTE_CAL_MODE ];
        return;
    }

    ( void )act_do( All_Control.Addr_Action_Open, 0 );
    if( Ap3_MenuPath >= PAGE_MENU_PATH )
        ( void )act_do( News_Book.Addr_Action_BookTable, 0 );
    if( Ap3_MenuPath >= CONTROL_MENU_PATH )
        ( void )act_do( News_Go.Addr_Action, 0 );
}


//#pragma CODE = SEARCH_TEMP_CODE10
#pragma CODE = RECORD_JUDGE_CODE
#if( AP4_RP == ENABLE )
Bool record_judge( void )
{
    if( !Current_Index )
        return FALSE;
    if( bInCal )
        return FALSE;
    rp_play_start = FALSE;
    //U16 dic_temp,mp3_temp,ap4_temp,v_adc,v_dec,v_min,v_max;
    if(g_setting_list.struct_setting.bluetooth) {
        if(( Current_Index >=  OID2_DIY_RECORD_MIN)&&( Current_Index <= OID2_DIY_RECORD_MAX)) {
            if(Current_Index%2 == 0) {
                rp_oid_index = Current_Index;
                goto record_rp;
            } else {
                rp_oid_index = Current_Index - 1;
                goto play_rp;
            }
        }
    } else {
        if(( Current_Index >=  DPR3_DIY_RECORD_MIN)&&( Current_Index <= DPR3_DIY_RECORD_MAX)) {
            rp_oid_index = Current_Index;
            goto record_rp;
        }
        if(( Current_Index >= DPR3_DIY_PLAY_MIN)&&( Current_Index <= DPR3_DIY_PLAY_MAX )) {
            rp_oid_index = Current_Index - (U16)(DPR3_DIY_OFFSET);
            goto play_rp;
        }
    }

    return FALSE;

record_rp:
    SendString((U8 *)"\r\nrp_r");
    rp_flag_bits.value = 0;
    //rp_oid_index = Current_Index;
    //rp_flag_bits.value &= ~0xE000u;//~0x07;
    // rp_ok_flag = TRUE;
    mail_send_event( EVT_SEARCH_RP_RECORD, 0 );
    return TRUE;
play_rp:
    SendString((U8 *)"\r\nrp_p");
    rp_flag_bits.value = 0;
    // rp_oid_index = Current_Index - (U16)(DIY_OFFSET);
    mail_send_event( EVT_SEARCH_RP_PLAY, 0 );
    return TRUE;

#if 0

    if(( Current_Index >=  DIY_RECORD_MIN)&&( Current_Index <= DIY_RECORD_MAX)) {
        /*rp_go_on_flag = FALSE;
        rp_record_start = FALSE;
        rp_play_start = FALSE;
        rp_ok_flag = FALSE;*/
        //rp_flag_bits.value &= ~0x0F;
        rp_flag_bits.value = 0;
        rp_oid_index = Current_Index;
        mail_send_event( EVT_SEARCH_RP_RECORD, 0 );
        return TRUE;
    }
    if(( Current_Index >= DIY_PLAY_MIN)&&( Current_Index <= DIY_PLAY_MAX )) {
        /*if( !Ap4_Record_Range.Record_Index_Min
           || !Ap4_Record_Range.Record_Index_Max
              || !Ap4_Record_Range.Play_Index_Min
                 || !Ap4_Record_Range.Play_Index_Max )
           return FALSE;*/
        /*rp_go_on_flag = FALSE;
        rp_record_start = FALSE;
        rp_play_start = FALSE;
        rp_ok_flag = FALSE;*/
        //rp_flag_bits.value &= ~0x0F;
        rp_flag_bits.value = 0;
        rp_oid_index = Current_Index - (U16)(DIY_OFFSET);
        mail_send_event( EVT_SEARCH_RP_PLAY, 0 );
        return TRUE;
    }


    if(( Current_Index >= Ap4_Record_Range_2.Record_Index_Min )&&( Current_Index <= Ap4_Record_Range_2.Record_Index_Max )) {
        //mail_send_event( EVT_SEARCH_RP_RECORD, 1 );
        /*rp_go_on_flag = FALSE;
        rp_record_start = FALSE;
        rp_play_start = FALSE;*/
        rp_flag_bits.value &= ~0xE000u;//~0x07;
        rp_ok_flag = TRUE;
        rp_oid_index = Current_Index;
        play_RMT_speech( Ap4_Record_2_Address );

        game_remote_search = TRUE;
#if 0
        SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
        lcd_goto_xy( 8, 12 );
        lcd_puts(( U8 *)"Please confirm" );
#elif( LANGUAGE == Simplified )
        lcd_goto_xy( 40, 12 );
        lcd_puts(( U8 *)"请确认" );
#elif( LANGUAGE == Traditional )
        lcd_goto_xy( 40, 12 );
        lcd_puts(( U8 *)"叫絋粄" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
        switch( g_setting_list.struct_setting.language_ihm ) {
            case LANG_ENGLISH:
                lcd_goto_xy( 8, 12 );
                lcd_puts(( U8 *)"Please confirm" );
                break;

            case LANG_CHINESE_SIMP:
                lcd_goto_xy( 40, 12 );
                lcd_puts(( U8 *)"请确认" );
                break;

            case LANG_CHINESE_TRAD:
                lcd_goto_xy( 40, 12 );
                lcd_puts(( U8 *)"叫絋粄" );
                break;
        }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
#endif
        return TRUE;
    }
    if(( Current_Index >= Ap4_Record_Range_2.Play_Index_Min )&&( Current_Index <= Ap4_Record_Range_2.Play_Index_Max )) {
        /*if( !Ap4_Record_Range_2.Record_Index_Min
           || !Ap4_Record_Range_2.Record_Index_Max
              || !Ap4_Record_Range_2.Play_Index_Min
                 || !Ap4_Record_Range_2.Play_Index_Max )
           return FALSE;*/
        /*rp_go_on_flag = FALSE;
        rp_record_start = FALSE;
        rp_play_start = FALSE;
        rp_ok_flag = FALSE;*/
        //rp_flag_bits.value &= ~0x0F;
        rp_flag_bits.value = 0;
        rp_oid_index = Current_Index - Ap4_Record_Range_2.Play_Index_Min + Ap4_Record_Range_2.Record_Index_Min;
        mail_send_event( EVT_SEARCH_RP_PLAY, 0 );
        return TRUE;
    }

    return FALSE;
#endif
}
#endif   //#if( AP4_RP == ENABLE )


#pragma CODE = SEARCH_OPEN_FILE_TEMP_CODE

void search_open_file_deal( void )
{
#if 1
    SDKLoadCode( SEARCH_SCRIPT_CODE );
    clear_script_variable();
#endif
    /*SendString((U8 *)"Search=");
    send_hex_u16(Current_Record.Ap3_num);
    SendString((U8 *)"\r\n");*/
    reread_enable_flag = FALSE;
    g_b_enable_flush = FALSE;
    g_b_flush_display = FALSE;
   
    scan_oid_disable();
    ap3_rand_init();
    init_array_v();
    SDKLoadCode( GET_FILE_NAME_CODE );
    SDKLoadCode( NAVIGATION_CODE );
    if( Get_File_Name(( U8 *)AP4_EXT, Current_Record.Ap3_num )) {
        SendString((U8 *)"\r\nfind file!");
        if( !bRemoteEnterCal ) {
            //if( !bGameClicked || !g_b_remote_on )
            {
                //bGameClicked = FALSE;
                SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
                lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
                /*lcd_goto_xy( 52, 0 );
                Ih_set_curr_font( IDC_FONT_08 );
                lcd_puts( "READ" );*/
                Ih_set_curr_font( IDC_FONT_16 );
                lcd_goto_xy( 6, 8 );
//          lcd_puts(( U8 *)"点读文件:" );
//            lcd_goto_xy( 12, 28 );
//            Ih_set_curr_font( IDC_FONT_16 );
                lcd_puts( g_string );
//           mmi_ap3_display_icon();
            }
        }
    }
    ///*
    strcpy((char *)current_ap4_short_name,(char *)ap4_short_name);
    strcat((char *)current_ap4_short_name,(char *)".");
    strcat((char *)current_ap4_short_name,(char *)AP4_EXT);

    SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE05 );
    //SDKLoadCode( NAVIGATION_CODE );
    if( !open_file()) {
//file_search_error1:
#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        Search_Record.value = 0;
        SendString((U8 *)"\r\nopenfail");
        if( ap4_change_file == MMI_AP3_REMOTE_CHANGE_FILE ) {
            SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE03 );
            SDKLoadCode( NAVIGATION_CODE );
            if( remote_search_file( /*Remote_Change_Ap4_file_count*/ ))
                return;
            //ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
            Current_Record.Ap3_num = Temp_Ap3_file_num;
            SDKLoadCode( GET_FILE_NAME_CODE );
            ( void )Get_File_Name(( U8 *)AP4_EXT, Current_Record.Ap3_num );
            scan_oid_disable();
            mmi_ap3_kbd_disable();
            mail_send_event( EVT_SEARCH_NO_FIND_FILE, 0 );
            state_search_to_error();
            return;
        }
        SDKLoadCode( SEARCH_OPEN_FILE_FAIL_CODE );
        search_open_file_fail_deal();
        return;
    }

    strcpy((char *)current_ap4_short_path,(char *)CurrentShortFilePath);
    strcat((char *)current_ap4_short_path,(char *)"\\");

    SendString((U8 *)"\r\npath=");
    SendString((U8 *)current_ap4_short_path);
    // SendString((U8 *)":");
    // SendString((U8 *)current_ap4_short_name);



    /*
       SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
       lcd_clear_part_of_screen( 22, 0, 87, 8 );
       lcd_goto_xy( 52, 0 );
       Ih_set_curr_font( IDC_FONT_08 );
       lcd_puts(( U8 *)"READ" );
    */
    /*
    #if( FAT_CLUSTER_BUF == ENABLE )
       SDKLoadCode( FS_CREATE_BUF_CODE );
       ( void )FSCreateBuf();
    #endif   //#if( FAT_CLUSTER_BUF == ENABLE )
    */
    {
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        /*if( g_b_remote_on
           ||( ap3_poweron_state == MMI_CHANGE_TO_AP3 )
              ||( ap3_poweron_state == MMI_AP3_ENTER_USB ))*/
        if(( ap3_poweron_state == MMI_CHANGE_TO_AP3 )
           ||( ap3_poweron_state == MMI_AP3_ENTER_USB )) {
            //g_b_remote_on = FALSE;
            //State_Search = Save_State_Search;
            Search_Record.value &= ~0xF3FFu;//0xFFCFu;
            if( b_reload_ap4 ) {
                b_reload_ap4 = FALSE;
                goto reload_ap4_info;
            }

#if( FAT_CLUSTER_BUF == ENABLE )
            //SDKLoadCode( FS_CREATE_BUF_CODE );
            //( void )FSCreateBuf();
            //fat_clusterlist_restore();
            cluster_dowith(TRUE);
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

            scan_oid_enable();
            mmi_ap3_kbd_enable();
            state_search_to_play();

            //reload ap4 info
            SDKLoadCode( SEARCH_RELOAD_AP4_INFO_CODE );
            reload_ap4_info_deal();
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
            /*if( News_Control.Type_Control == GAME_TYPE )
            {
               print_dic_screen();
               print_lettle_screen( lettle_count );
               break;
            }*/
            return;
        }
    reload_ap4_info:
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))

#if( FAT_CLUSTER_BUF == ENABLE )
        SDKLoadCode( FS_CREATE_BUF_CODE );
        ( void )FSCreateBuf();
#endif   //#if( FAT_CLUSTER_BUF == ENABLE )

        Search_Record.value = 0;
        State_Search = Ap3_Search;
    }
}


#pragma CODE = SEARCH_AP4_TEMP_CODE

void search_ap4_deal( void )
{
    //SendString((U8 *)"\r\nAp4");
    //ap3_rand_init();
    //red_led_on();
    //scan_oid_disable();
    //reread_enable_flag = FALSE;
    //Search_Record.value = 0;
   
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
#if 0//( AP4_ENCRYPT == ENABLE )
    SDKLoadCode( LCE_ENCRYPT_CODE );
    ( void )lce_sec();
#endif   //#if( AP4_ENCRYPT == ENABLE )   
    scan_oid_enable();

    // SDKLoadCode(PRINT_CODE);
    // print_news();

    if( All_Control.Version < 2 ) {
    file_search_error2:
        SDKLoadCode( FILE_TEMP_CODE01 );
        file_close();
        State_Search = Open_File_Search;
        //goto file_search_error1;

#if 1//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        g_b_remote_on = FALSE;
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
        // Search_Record.value = 0;

        if( ap4_change_file == MMI_AP3_REMOTE_CHANGE_FILE ) {
            SDKLoadCode( MMI_AP4_SEARCH_TEMP_CODE03 );
            SDKLoadCode( NAVIGATION_CODE );
            if( remote_search_file( /*Remote_Change_Ap4_file_count*/ ))
                return;
            //ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
            Current_Record.Ap3_num = Temp_Ap3_file_num;
            SDKLoadCode( GET_FILE_NAME_CODE );
            ( void )Get_File_Name(( U8 *)AP4_EXT, Current_Record.Ap3_num );
            // scan_oid_disable();
            // mmi_ap3_kbd_disable();
            mail_send_event( EVT_SEARCH_NO_FIND_FILE, 0 );
            state_search_to_error();
            return;
        }
        SDKLoadCode( SEARCH_OPEN_FILE_FAIL_CODE );
        search_open_file_fail_deal();
        return;
    }
#if( AP4_ENCRYPT == ENABLE )
    /*if( MSB0( All_Control.Point_Book_Table )== 'F' )
    {
       MSB0( All_Control.Point_Book_Table )^= 'L';
       MSB1( All_Control.Point_Book_Table )^= 'X';
       MSB2( All_Control.Point_Book_Table )^= 'K';
       MSB3( All_Control.Point_Book_Table )^= 'J';
    }
    else*/
    SDKLoadCode( SEARCH_BOOK_ENCRYPT_CODE );
    if( !ap4_book_encrypt()) {
#if( DECODE_NORMAL_FILE == ENABLE )
        if( !MSB0( All_Control.Point_Book_Table )) {
            goto ap4_search_decode_file_ok;
        }
#endif   //#if( DECODE_NORMAL_FILE == ENABLE )
        goto file_search_error2;
    }
#else    //#if( AP4_ENCRYPT == DISABLE )
    if( MSB0( All_Control.Point_Book_Table )) {
        goto file_search_error2;
    }
#endif   //#if( AP4_ENCRYPT == ENABLE )
#if( AP4_ENCRYPT == ENABLE )
#if( DECODE_NORMAL_FILE == ENABLE )
ap4_search_decode_file_ok:
#endif   //#if( DECODE_NORMAL_FILE == ENABLE )
#endif   //#if( AP4_ENCRYPT == ENABLE )
    ap4_change_file = MMI_AP3_NO_CHANGE_FILE;
    if( All_Control.Point_Special_Action )
        read_data_from_nf( All_Control.Point_Special_Action, ( U8 _MEM_TYPE_SLOW_ *)&Current_Special_Action, sizeof( Special_Action ));
    {
        SDKLoadCode( SEARCH_REMOTE_TEMP_CODE00 );
        remote_task_init();
        //RemoteBaseAddress = All_Control.Point_Remote;
        init_remote();
        SDKLoadCode( AP4_SEARCH_CODE );
        ap4_search_deal();
    }
}


#if( AP4_ENCRYPT == ENABLE )
#pragma CODE =  SEARCH_BOOK_ENCRYPT_CODE
Bool ap4_book_encrypt( void )
{
    if( MSB0( All_Control.Point_Book_Table )== 'F' ) {
        MSB0( All_Control.Point_Book_Table )^= 'F';
        MSB1( All_Control.Point_Book_Table )^= 'Z';
        MSB2( All_Control.Point_Book_Table )^= 'Z';
        MSB3( All_Control.Point_Book_Table )^= 'T';
        return TRUE;
    } else if( MSB0( All_Control.Point_Book_Table )== 'M' ) {
        MSB0( All_Control.Point_Book_Table )^= 'M';
        MSB1( All_Control.Point_Book_Table )^= 'J';
        MSB2( All_Control.Point_Book_Table )^= 'F';
        MSB3( All_Control.Point_Book_Table )^= 'Z';
        return TRUE;
    } else if( MSB0( All_Control.Point_Book_Table )== 'Z' ) {
        MSB0( All_Control.Point_Book_Table )^= 'Z';
        MSB1( All_Control.Point_Book_Table )^= 'Q';
        MSB2( All_Control.Point_Book_Table )^= 'F';
        MSB3( All_Control.Point_Book_Table )^= 'Z';
        return TRUE;
    }
    return FALSE;
}
#endif   //#if( AP4_ENCRYPT == ENABLE )


#pragma CODE = SEARCH_BOOK_TEMP_CODE

void search_book_deal( void )
{
    //SendString((U8 *)"Book\r\n");
    bBackNormal = false;
    reread_enable_flag = FALSE;
    g_b_REREAD_Flag = false;
    ap3_rand_init();
    if( bGameClicked ) {
        bGameClicked = FALSE;
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 22, 0, 87, 8 );
              lcd_goto_xy( 52, 0 );
              Ih_set_curr_font( IDC_FONT_08 );
              lcd_puts(( U8 *)"READ" );

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
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
              lcd_goto_xy( 15, 12 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
    }

    Search_Record.value = 0;
    SDKLoadCode( READ_BOOKNEWS_CODE );
    read_book_news(( U16 ) Current_Record.Book_num );
    SendString((U8 *)"\r\ncap=");
    send_hex_u32(News_Book.Point_Book_Cap);
    if( 0 != News_Book.Point_Book_Cap ) {
        read_data_from_nf( News_Book.Point_Book_Cap, ( U8 _MEM_TYPE_SLOW_ *)&Len_Cap_Book, 2 );
    }

    if(All_Control.Version < 15) {
        News_Book.Point_Book_Cap = 0;
        Len_Cap_Book = 0;
    }

    Ap3_MenuPath = PAGE_MENU_PATH;
    ( void )act_do( News_Book.Addr_Action_BookTable, 0 );
    if( News_Book.Point_GoTable && News_Book.Num_GoTable ) {
        if( News_Book.Flag_CurrentBook & 0x01 ) {
            Current_Record.Page_num = 0;
            State_Search = Page_Search;
            return;
        }
    }
    /*else
    {
       Search_Record.news_sys.disable_next_key_flag = 1;
    }*/
    state_search_to_play();
}


#pragma CODE = SEARCH_PAGE_TEMP_CODE

void search_page_deal( void )
{
#if 1
    SDKLoadCode( SEARCH_SCRIPT_CODE );
    clear_script_variable();
#endif

    SendString((U8 *)"\r\nPage");
    reread_enable_flag = FALSE;
    g_b_REREAD_Flag = false;
    g_b_reread_play = FALSE;
    ap3_rand_init();
    if( bGameClicked ) {
        bGameClicked = FALSE;
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 22, 0, 87, 8 );
              lcd_goto_xy( 52, 0 );
              Ih_set_curr_font( IDC_FONT_08 );
              lcd_puts(( U8 *)"READ" );

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
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
              lcd_goto_xy( 15, 12 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
    }

    Search_Record.value = 0;
    SDKLoadCode( READ_PAGENEWS_CODE );
    read_page_news(( U16 ) Current_Record.Page_num );
    ( void )act_do( News_Go.Addr_Action, 0 );
    SDKLoadCode( PAGE_SEARCH_CODE );
    page_search_deal();
}


#pragma CODE = SEARCH_CONTROL_TEMP_CODE

void search_control_deal( void )
{
    SendString((U8 *)"\r\nControl");
    reread_enable_flag = FALSE;
    g_b_REREAD_Flag = false;
    ap3_rand_init();
    if( bGameClicked ) {
        bGameClicked = FALSE;
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 22, 0, 87, 8 );
              lcd_goto_xy( 52, 0 );
              Ih_set_curr_font( IDC_FONT_08 );
              lcd_puts(( U8 *)"READ" );

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
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
              lcd_goto_xy( 15, 12 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
    }
    scan_oid_enable();
    SDKLoadCode( READ_CONTROLNEWS_CODE );
    read_control_mode_news( Current_Record.Control_num );
    if(!bBackNormal){
        SendString((U8 *)"\r\nbBackNormal");    
        
        SDKLoadCode( MMI_AP4_CONTROL_TEMP_CODE00 );
        display_control_caption();
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, READ_CTRL_DISPLAY);
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, AP4_SINGLE_MODE_DISPLAY);
    }
    bBackNormal = false;
    if(bRemoteNormal){
        bRemoteNormal = false;
        play_RMT_speech( normalAddress );
    }
        
    {
        if( ap3_poweron_state == MMI_AP3_CHANGE_CONTROL )
            ( void )act_do( News_Control.Addr_Action, 0 );
    }
    //Search_Record.news_sys.table_flag = CONTROL_TABLE;
    

    //mail_send_event( EVT_SEARCH_FLUSH_CONTROL, AP4_SINGLE_MODE_DISPLAY);

    switch( News_Control.Type_Control ) {
        case SCRIPT_TYPE:
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
            SDKLoadCode( SEARCH_SCRIPT_CODE );
            script_init( News_Control.Point );
            script_decode();
            break;

        case GAME_TYPE:
            SDKLoadCode( SEARCH_GAME_TEMP_CODE00 );
            SDKLoadCode( SEARCH_GAME_CODE );
            if( !game_initial()) {
                News_Control.Type_Control = 0;
                Search_Record.news_sys.table_flag = DEFAULT_TABLE;
                //Search_Record.news_sys.disable_next_key_flag = 1;
                Ap3_MenuPath = CONTROL_MENU_PATH;
            } else
                Search_Record.news_sys.table_flag = CONTROL_TABLE;
            break;

        case DIC_TYPE:
#if( DIC == DISABLE )
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
#endif   //#if( DIC == DISABLE )
#if( DIC == ENABLE )
#if( REMOTE_DIC == ENABLE )
            /*bRmoteExitAp4 = TRUE;
            bInEbookMode = TRUE;*/
            remote_bits.value |= 0x1010u;//0x0808u;
            remote_to_mode = DIC_APPLI;
            if( bKeyChangeControl ) {
                Search_Record.news_sys.table_flag = DEFAULT_TABLE;
                //Search_Record.news_sys.disable_next_key_flag = 1;
                Ap3_MenuPath = CONTROL_MENU_PATH;
            } else {
                if( Search_Record.news_sys.table_flag == CONTROL_TABLE ) {
                    Current_Record.Control_num = Temp_Control_num;
                    SDKLoadCode( READ_CONTROLNEWS_CODE );
                    read_control_mode_news( Current_Record.Control_num );
                }
            }
#endif   //#if( REMOTE_DIC == ENABLE )
#if( REMOTE_DIC == DISABLE )
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
#endif   //#if( REMOTE_DIC == DISABLE )
#endif   //#if( DIC == ENABLE )
            break;

        case QA_TYPE:
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            //Search_Record.news_sys.disable_next_key_flag = 1;
            Ap3_MenuPath = CONTROL_MENU_PATH;
            break;

        case SINGLE_TABLE_TYPE:
            Search_Record.news_sys.table_flag = CONTROL_TABLE;
            break;

        default:
            Search_Record.news_sys.table_flag = DEFAULT_TABLE;
            Ap3_MenuPath = CONTROL_MENU_PATH;
            break;
    }
    bKeyChangeControl = FALSE;
    Ap3_MenuPath = CONTROL_MODE_MENU_PATH;
    state_search_to_play();
}


#pragma CODE = SEARCH_COOR_TEMP_CODE

void index_coor_deal( void )
{
#if 0
    Bool display_flag = FALSE;

    if( g_b_ap4_caption ) {
        g_b_ap4_caption = FALSE;
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }

    if( Current_Index != search_last_oid_index ) {
        search_last_oid_index = Current_Index;
        order_play_count = 0;
    }

#if( AP4_RP == ENABLE )
    if( Current_Index == remote_index_tab[ REMOTE_REC_RECORD_OK ]) {
        //SendString(( U8 *)"Rec ok\r\n");
        if( rp_ok_flag ) {
            rp_ok_flag = FALSE;
            mail_send_event( EVT_SEARCH_RP_RECORD, 0 );
        }
        return;
    } else if( Current_Index == remote_index_tab[ REMOTE_REC_STOP ]) {
        //SendString(( U8 *)"Rec stop\r\n");
        if( rp_ok_flag ) {
            rp_ok_flag = FALSE;
        }
        if( rp_go_on_flag ) {
            mail_send_event( EVT_SEARCH_RP_STOP, 0 );
        }
        if( rp_flag_bits.value & 0x6000u ) {
            /*rp_record_start = FALSE;
            rp_play_start = FALSE;*/
            rp_flag_bits.value &= ~0x6000u;//~0x06;

            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();
        }
        return;
    }

    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
        mmi_ap3_kbd_enable();
        scan_oid_enable();

        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();*/
            display_flag = TRUE;
        }
    }
#endif   //#if( AP4_RP == ENABLE )

    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();
            //mmi_ap3_display_volume();*/
            display_flag = TRUE;
        }
        ap4_play_state = AP4_PLAY_ERROR;
    }

    if( s_ts_id_read != UNDEFINED_TIMER ) {
        Ts_free( s_ts_id_read );
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }
    if( !bInCal && !( rp_flag_bits.value & 0x7000u ))
        mmi_ap3_display_icon();

    repeat_save();
    init_array_v();
    state_search_to_play();
    bRmoteExitAp4 = FALSE;

#if( AP4_RP == ENABLE )
    SDKLoadCode( SEARCH_TEMP_CODE10 );
    if( record_judge()) {
        if( game_remote_search ) {
            SDKLoadCode( SEARCH_RP_CONFIRM_CODE );
            rp_confirm_display();
        }
        game_remote_search = FALSE;
        return;
    }

#if 1
    rp_go_on_flag = FALSE;
    //if( rp_ok_flag || rp_record_start || rp_play_start )
    if( rp_flag_bits.value & 0x7000u ) {
        /*rp_record_start = FALSE;
        rp_play_start = FALSE;
        rp_ok_flag = FALSE;*/
        rp_flag_bits.value &= ~0x7000u;//~0x0E;

        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }
#endif
    rp_flag_bits.value = 0;
#endif   //#if( AP4_RP == ENABLE )

    if( display_flag ) {
        SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();
    }
    display_flag = FALSE;
#endif

    game_remote_search = FALSE;


#if (REREAD_MODE == ENABLE)
    // if(!g_setting_list.struct_setting.bluetooth){

    if(Current_Index == NORMAL_MODE_INDEX) {
        SendString((U8 *)"\r\nnormal mode");
        //Search_Record.news_sys.flag_index = true;
        virtualKeyState = GOTO_NOMAL;
        bRemoteNormal = true;
        // play_RMT_speech( normalAddress );
        /*

        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, NORMAL_MODE_DISPLAY);
        if(!g_b_REREAD_Flag)return;
        g_b_REREAD_Flag = false;
        NORMAL_CONTROL:
        //if( News_Go.Num_ControlTab > 0){
        //Temp_Control_num = Current_Record.Control_num;
        // Current_Record.Control_num = 0;
        //State_Search = Control_Mode_Search;
        //ap3_poweron_state = MMI_AP3_CHANGE_CONTROL;
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, READ_CTRL_DISPLAY);

        mail_send_event( EVT_SEARCH_HINT_PLAY, NORMAL_MODE_HINT);
        //}
        */
        return;
    }
    if(Current_Index == REREAD_MODE_INDEX) {
        SendString((U8 *)"\r\n reread mode");
       // Search_Record.news_sys.flag_anjian = true;
        virtualKeyState = GOTO_REREAD;
        play_RMT_speech( rereadAddress );
        /*
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, REPEAT_MODE_DISPLAY);
        if(g_b_REREAD_Flag)return;
        g_b_REREAD_Flag = true;
        mail_send_event( EVT_SEARCH_FLUSH_CONTROL, REPEAT_CTRL_DISPLAY);

        mail_send_event( EVT_SEARCH_HINT_PLAY, REREAD_MODE_HINT );
        // goto NORMAL_CONTROL;
        */
        return;
    }

    //}
#endif

    /*
       SDKLoadCode(FIND_BOOKSYS_CODE);
        if(0xFFFF != read_bnl_file(Current_Index)){
             mail_send_event(EVT_GOTO_OID_MODE, 0);
             return;
            }
        */
    switch( Coor_Type ) {
        case IMMOBILITY_COOR_MODE:
            if(( Current_Index >= Index_Range.Remote_Index_Min )
               &&( Current_Index <= Index_Range.Remote_Index_Max )) {
                if( bHaveRemote ) {
                    game_remote_search = TRUE;
                    /*SDKLoadCode( SEARCH_REMOTE_TEMP_CODE01 );
                    if( search_remote_index())
                    {
                       SDKLoadCode( SEARCH_REMOTE_TEMP_CODE02 );
                       remote_task();
                    }*/
                }
                break;
            }
            SDKLoadCode( JUDGE_IMMOBILITY_COOR_CODE );
            judge_immobility_coor();
            break;

        case MOBILE_COOR_MODE:
            if(( Current_Index >= Index_Range.Remote_Index_Min )
               &&( Current_Index <= Index_Range.Remote_Index_Max )) {
                if( bHaveRemote ) {
                    game_remote_search = TRUE;
                    /*SDKLoadCode( SEARCH_REMOTE_TEMP_CODE01 );
                    if( search_remote_index())
                    {
                       SDKLoadCode( SEARCH_REMOTE_TEMP_CODE02 );
                       remote_task();
                    }*/
                }
                break;
            }
            SDKLoadCode( JUDGE_MOBILITY_COOR_CODE );
            judge_mobility_coor();
            break;

        case MIX_COOR_MODE:
            if(( Current_Index >= Index_Range.Remote_Index_Min )
               &&( Current_Index <= Index_Range.Remote_Index_Max )) {
                if( bHaveRemote ) {
                    game_remote_search = TRUE;
                    /*SDKLoadCode( SEARCH_REMOTE_TEMP_CODE01 );
                    if( search_remote_index())
                    {
                       SDKLoadCode( SEARCH_REMOTE_TEMP_CODE02 );
                       remote_task();
                    }*/
                }
                break;
            }
            SDKLoadCode( JUDGE_MIX_COOR_CODE );
            judge_mix_coor();
            break;
    }
}


#pragma CODE = SEARCH_NORMAL_TEMP_CODE

void normal_search_deal( void )
{
    game_remote_search = FALSE;

    if( Search_Record.news_sys.ap3_aeb_flag ) {
        /*Search_Record.news_sys.index_normal_flag = 0;
        Search_Record.news_sys.ap3_aeb_flag = 0;*/
        Search_Record.value &= ~0x0042u;//~0x4200;
        reread_enable_flag = FALSE;
    } else if( ap3_mode == SINGLE_PLAY_MODE ) {
        state_search_to_play();
    } else if( ap3_mode == SEQUENCE_PLAY_MODE ) {
        if( State_Search != Sequence_Search ) {
            Search_Record.news_sys.index_normal_flag = 0;
            reread_enable_flag = FALSE;
            init_array_v();
            state_search_to_play();
        }
    } else {
        Search_Record.news_sys.index_normal_flag = 0;
        reread_enable_flag = FALSE;
        init_array_v();
        state_search_to_play();
    }
}


#pragma CODE = SEARCH_IDLE_TEMP_CODE

void idle_search_deal( void )
{
    state_search_to_play();
    if( Point_Read == Point_Write ) {
        if( !bInCal
#if( AP4_RP == ENABLE )
            && !( rp_flag_bits.value & 0x7000u )
#endif   //#if( AP4_RP == ENABLE )
          )
            mmi_ap3_display_icon(FALSE);

        SDKLoadCode( SEARCH_PLAY_END_CODE );
        speech_play_end();
    }
}


#pragma CODE = SEARCH_PLAY_TEMP_CODE

void play_search_deal( void )
{
    if(g_b_first_display_flag)return;

    if( g_b_search_record || g_b_player_on  )
        return;

    if( bRemoteEnterCal ) {
        bRemoteEnterCal = FALSE;
        Search_Record.news_sys.flag_index = TRUE;
        Current_Index = remote_index_tab[ REMOTE_CAL_MODE ];
        return;
    }

    if( Point_Write != Point_Read ) {
        if( bTipPlay || bGameTimeOverPlay
            || bSkipNext || bSkipPrevious ) {
            return;
        }
        SendString((U8 *)"\r\nplayer");
        /*
        SendString((U8 *)"V[");
        send_hex_u8(Point_Read);
        SendString((U8 *)"].D:");
        send_hex_u32(Array_V[ Point_Read ].Addr_Speech);
        SendString((U8 *)"\r\n");
        //*/

        SDKLoadCode( PLAYER_AP3_START_CODE );
        SDKLoadCode( SEARCH_AP4INFO_CODE );
        if( player_ap3_start( Array_V[ Point_Read ].Addr_Speech, Array_V[ Point_Read ].Flag_Speech )) {
            //SendString((U8 *)"\r\nbegin play");
            if( !bInCal
#if( AP4_RP == ENABLE )
                && !( rp_flag_bits.value & 0x07000u )
#endif   //#if( AP4_RP == ENABLE )
              ) {
                mmi_ap3_display_icon(FALSE);

//          SDKLoadCode( MMI_AP4_DISPTIME_CODE );
//          mmi_ap4_display_dynami_time(PLAY_BEGIN_TIME);
//          mmi_ap4_display_dynami_time(PLAY_ALL_TIME);
                mail_send_event( EVT_SEARCH_FLUSH_ALLTIME, 0 );
            }

            //bSpeechEnd = FALSE;
            state_search_to_idle();
        }
        Point_Read++;
    } else {
        SDKLoadCode( SEARCH_PLAY_END_CODE );
        speech_play_end();
    }
}


#pragma CODE = SEARCH_SEQUENCE_TEMP_CODE

void sequence_search_deal( void )
{
    if( g_b_player_on )
        return;
#if 0
    if( sequence_speech_read_point > Current_Act_Sequence.Speech_End_Address ) {
        Search_Record.news_sys.index_normal_flag = 0;
        State_Search = Null_state;
        if( !bInCal && !( rp_flag_bits.value & 0x07000u ))
            mmi_ap3_display_icon();
        return;
    }
#endif
    SDKLoadCode( SEARCH_SEQUENCE_JUDGE_CODE );
    if( sequence_judge())
        return;
    if( bTipPlay || bGameTimeOverPlay
        || bSkipNext || bSkipPrevious ) {
        return;
    }
    SDKLoadCode( PLAYER_AP3_START_CODE );
    if( player_ap3_start( sequence_speech_read_point, 0 )) {
        if( !bInCal
#if( AP4_RP == ENABLE )
            && !( rp_flag_bits.value & 0x07000u )
#endif   //#if( AP4_RP == ENABLE )
          )
            mmi_ap3_display_icon(FALSE);
        //bSpeechEnd = FALSE;
    }
    //if( Search_Record.news_sys.repeat_flag )
    //   return;
    sequence_speech_read_point += 9;
}


#pragma CODE = SEARCH_SEQUENCE_JUDGE_CODE

Bool sequence_judge( void )
{
    if( sequence_speech_read_point > Current_Act_Sequence.Speech_End_Address ) {
        Search_Record.news_sys.index_normal_flag = 0;
        State_Search = Null_state;
        if( !bInCal
#if( AP4_RP == ENABLE )
            && !( rp_flag_bits.value & 0x07000u )
#endif   //#if( AP4_RP == ENABLE )
          )
            mmi_ap3_display_icon(FALSE);
        return TRUE;
    }
    return FALSE;
}

#pragma CODE = CHANGE_OID_CODE

U16 chang_oid(U16 dpr_index)
{
    SPIF_ERR SPI_STATE;
    U32 temp_sector;
    U16 temp_off,all_off = dpr_index;


    U16 *p = (U16 *)&Buf;
#if 0//(REREAD_MODE == ENABLE)
    if(Current_Index == NORMAL_MODE_INDEX) {
        return NORMAL_MODE_INDEX;
    }
    if(Current_Index == REREAD_MODE_INDEX) {
        return REREAD_MODE_INDEX;
    }
#endif
    temp_off = all_off%256;
    GetDataIdInfo( D_OID, &Start_Sector, &Length );
    temp_sector = Start_Sector +(U32)(all_off >> 8);
    SPI_STATE = spif_resv_read( temp_sector, ( __X void *)Buf );
    if( SPI_STATE != SDTL_No_Err ) return 0xFFFF;
    return p[temp_off];
}

//#pragma CODE = SEARCH_COOR_CODE00
#pragma CODE = COOR_JUDGE_DEAL_CODE
Bool coor_judge_deal( void )
{
    if(g_setting_list.struct_setting.bluetooth) {
        SDKLoadCode(CHANGE_OID_CODE);
        Current_Index = chang_oid( Current_Index );
    }
    SendString((U8 *)"\nX=");
    send_hex_u16(Current_Index);
    Search_Record.news_sys.flag_index = 0;
    //if(g_b_lcd_backlight_off)return TRUE;
#if( LCD_PRESENT == ENABLE )
#if(BACKLIGHT == ENABLE)
    if( g_b_lcd_backlight_off ) {
        Lcd_rearm_backlight_on();
        g_mmi_lcd_backlight_off = FALSE;
        return TRUE;
    }
     if(gbRereadingFlag)return true;
#endif //#if(BACKLIGHT == ENABLE)
#endif   //#if( LCD_PRESENT == ENABLE )
    if(!g_b_recorder_on) {
        SDKLoadCode(SOME_INDEX_CODE);
        if(some_index_dowith())return TRUE;
#if 1
        SDKLoadCode(FIND_BOOKSYS_CODE);
        if(0xFFFF != read_bnl_file(Current_Index))return TRUE;
#else
        if(State_Search == Browser_File ) {
            SDKLoadCode(FIND_BOOKSYS_CODE);
            if(0xFFFF != read_bnl_file(Current_Index)) {
                //mail_send_event(EVT_GOTO_OID_MODE, 0);
                ap3_poweron_state = MMI_AP3_NORMAL;
                State_Search = Open_File_Search;
                Current_Record.Ap3_num = g_xuanshu_num -1;
            }
            return TRUE;
        }
#endif
    }
    if(State_Search >= Browser_File_Init )return TRUE;
    g_b_flush_display = FALSE;

    //if( Search_Record.news_sys.repeat_flag || Search_Record.news_sys.play_record_flag )
    if( Search_Record.value & 0x0101u/*0x8080*/ ) {
        /*Search_Record.news_sys.repeat_flag
           = Search_Record.news_sys.play_record_flag
              = 0;*/
        g_b_ap4_caption = TRUE;
#if 0
        if( player_ap3_is_busy())
            player_ap3_stop();
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();
#endif
    }
    //SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    //lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    // lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
    /*Search_Record.news_sys.reread_flag
       = Search_Record.news_sys.repeat_flag
          = Search_Record.news_sys.sequence_flag
             //= Search_Record.news_sys.index_normal_flag
                = Search_Record.news_sys.play_record_flag
                   = 0;*/

    //Search_Record.value &= ~0x4381u;//0x81C2u;
    reread_enable_flag = FALSE;
    if(( State_Search != Play_Search )
       &&( State_Search != Idle_Search )
       &&( State_Search != Sequence_Search )
       &&( State_Search != Null_state ))
        return TRUE;

    if( player_ap3_is_busy()) {
        if( Current_Index ) {
            _MEM_TYPE_SLOW_ U8 i;
            for( i = 0; i < REMOTE_INDEX_MAX; i++ ) {
                if( Current_Index == remote_index_tab[ i ]) {
#if 1
                    if(i == REMOTE_VOL_UP) {
                        srvplayer_volume_change( SRVPLAYER_VOL_UP );
                        mmi_ap3_display_volume();
                        return TRUE;
                    }
                    if(i == REMOTE_VOL_DOWN) {
                        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                        mmi_ap3_display_volume();
                        return TRUE;
                    }
                    if(( i >= REMOTE_VOL0 )&&( i <= REMOTE_VOL7)) {
                        if(( i - REMOTE_VOL0 )>( VOL_LEVEL - 1 ))
                            return TRUE;
                        g_volume = i - REMOTE_VOL0;
                        srvplayer_volume_send();
                        mmi_ap3_display_volume();
                        return TRUE;
                    }
#else
                    switch( i ) {
                        case REMOTE_VOL_UP:
                            srvplayer_volume_change( SRVPLAYER_VOL_UP );
                            mmi_ap3_display_volume();
                            return TRUE;
                        //break;

                        case REMOTE_VOL_DOWN:
                            srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                            mmi_ap3_display_volume();
                            return TRUE;
                        //break;

                        case REMOTE_VOL0:
                        case REMOTE_VOL1:
                        case REMOTE_VOL2:
                        case REMOTE_VOL3:
                        case REMOTE_VOL4:
                        case REMOTE_VOL5:
                        case REMOTE_VOL6:
                        case REMOTE_VOL7:
                            //case REMOTE_VOL8:
                            //case REMOTE_VOL9:
                            //case REMOTE_VOL10:
                            //case REMOTE_VOL11:
                            //case REMOTE_VOL12:
                            //case REMOTE_VOL13:
                            //case REMOTE_VOL14:
                            if(( i - REMOTE_VOL0 )>( SRVPLAYER_VOL_LEVEL - 1 ))
                                return TRUE;
                            g_volume = i - REMOTE_VOL0;
                            srvplayer_volume_send();
                            mmi_ap3_display_volume();
                            return TRUE;
                            //break;
                    }
                    break;
#endif
                }
            }
        }
        player_ap3_stop();
    }
    Search_Record.value &= ~0x4381u;
    game_remote_search = FALSE;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    //lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24) );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );

    return FALSE;
}


#pragma CODE = SEARCH_RP_CONFIRM_CODE

void rp_confirm_display( void )
{
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
    Ih_set_curr_font( IDC_FONT_16 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    lcd_goto_xy( 8, 12 );
    lcd_puts(( U8 *)"Please confirm" );
#elif( LANGUAGE == Simplified )
    lcd_goto_xy( 40, 12 );
    lcd_puts(( U8 *)"请确认" );
#elif( LANGUAGE == Traditional )
    lcd_goto_xy( 40, 12 );
    lcd_puts(( U8 *)"叫絋粄" );
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
    switch( g_setting_list.struct_setting.language_ihm ) {
        case LANG_ENGLISH:
            lcd_goto_xy( 8, 12 );
            lcd_puts(( U8 *)"Please confirm" );
            break;

        case LANG_CHINESE_SIMP:
            lcd_goto_xy( 40, 12 );
            lcd_puts(( U8 *)"请确认" );
            break;
            /*
                  case LANG_CHINESE_TRAD:
                  lcd_goto_xy( 40, 12 );
                  lcd_puts(( U8 *)"叫絋粄" );
                  break;
            */
    }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
}


#pragma CODE = SEARCH_GAME_TIME_CODE

Bool game_time_judge( void )
{
    if(( State_Search != Play_Search )
       &&( State_Search != Idle_Search )
       &&( State_Search != Sequence_Search )
       &&( State_Search != Null_state ))
        return TRUE ;
    if( g_b_player_on )
        return TRUE;

    if( bInCal ) {
        gl_key_press = FALSE;
        return TRUE;
    }
    return FALSE;
}


//#pragma CODE = SEARCH_COOR_CODE01
#pragma CODE = JUDGE_INDEX_DEAL_CODE
Bool judge_index_deal( void )
{
    Bool display_flag = FALSE;

    if( g_b_ap4_caption ) {
        g_b_ap4_caption = FALSE;
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }

    if( Current_Index != search_last_oid_index ) {
        search_last_oid_index = Current_Index;
        order_play_count = 0;
    }

    if( bRmoteExitAp4 ) {
        init_array_v();
        state_search_to_play();
        Search_Record.news_sys.index_normal_flag = 0;
        return TRUE;
    }

#if( AP4_RP == ENABLE )
    if( Current_Index ) {
        if( Current_Index == remote_index_tab[ REMOTE_REC_RECORD_OK ]) {
            SendString(( U8 *)"\r\nRec ok");
            if( rp_ok_flag ) {
                rp_ok_flag = FALSE;
                mail_send_event( EVT_SEARCH_RP_RECORD, 0 );
            }
            Search_Record.news_sys.index_normal_flag = 0;
            return TRUE;
        } else if( Current_Index == remote_index_tab[ REMOTE_REC_STOP ]) {
            SendString(( U8 *)"\r\nRec stop");
            if( rp_ok_flag ) {
                rp_ok_flag = FALSE;
            }
            if( rp_go_on_flag ) {
                mail_send_event( EVT_SEARCH_RP_STOP, 0 );
            }
            if( rp_flag_bits.value & 0x6000u ) {
                /*rp_record_start = FALSE;
                rp_play_start = FALSE;*/
                rp_flag_bits.value &= ~0x6000u;//~0x06;
                /*
                            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
                            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
                            lcd_goto_xy( 15, 12 );
                            Ih_set_curr_font( IDC_FONT_16 );
                            lcd_puts( g_string );
                */
                mmi_ap3_display_icon(FALSE);
            }
            Search_Record.news_sys.index_normal_flag = 0;
            return TRUE;
        }
    }
//#endif
    if( g_b_recorder_on ) {
        srvrec_stop();
        Ts_free( s_ts_ap3record_display_id );
        mmi_ap3_kbd_enable();
        scan_oid_enable();

        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();*/
            display_flag = TRUE;
        }
    }
#endif   //#if( AP4_RP == ENABLE )

    if( g_b_search_record ) {
        g_b_search_record = FALSE;
        if( ap4_play_state == AP4_PLAY_ERROR ) {
            /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
            lcd_goto_xy( 15, 12 );
            Ih_set_curr_font( IDC_FONT_16 );
            lcd_puts( g_string );
            mmi_ap3_display_icon();
            //mmi_ap3_display_volume();*/
            display_flag = TRUE;
        }
        ap4_play_state = AP4_PLAY_ERROR;
        Ts_free( mmipaster_play_id_timerdisplay );
    }

    if( s_ts_id_read != UNDEFINED_TIMER ) {
        Ts_free( s_ts_id_read );
        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }
    if( !bInCal
#if( AP4_RP == ENABLE )
        && !( rp_flag_bits.value & 0x7000u )
#endif   //#if( AP4_RP == ENABLE )
      )
        mmi_ap3_display_icon(FALSE);


    if( Current_Index ) {
        if( Current_Index == remote_index_tab[ REMOTE_SPEECH_REPEAT ]) {
            if( Search_Record.news_sys.index_normal_flag ) {
                if( b_play_repeat ) {
                    Point_Read = 0;
                    b_remote_play_repeat = TRUE;
                    state_search_to_play();
                    return TRUE;
                }
            }
        }
    }
    Search_Record.news_sys.index_normal_flag = 0;
    b_play_repeat = FALSE;
    b_remote_play_repeat = FALSE;

    repeat_save();
    init_array_v();
    state_search_to_play();
    bRmoteExitAp4 = FALSE;

#if( AP4_RP == ENABLE )
    SDKLoadCode( RECORD_JUDGE_CODE );
    if( record_judge()) {
        if( game_remote_search ) {
            SDKLoadCode( SEARCH_RP_CONFIRM_CODE );
            rp_confirm_display();
        }
        game_remote_search = FALSE;
        return TRUE;
    }

#if 1
    rp_go_on_flag = FALSE;
    //if( rp_ok_flag || rp_record_start || rp_play_start )
    if( rp_flag_bits.value & 0x7000u ) {
        /*rp_record_start = FALSE;
        rp_play_start = FALSE;
        rp_ok_flag = FALSE;*/
        rp_flag_bits.value &= ~0x7000u;//~0x0E;

        /*SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
        lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
        lcd_goto_xy( 15, 12 );
        Ih_set_curr_font( IDC_FONT_16 );
        lcd_puts( g_string );
        mmi_ap3_display_icon();*/
        display_flag = TRUE;
    }
#endif
    rp_flag_bits.value = 0;
#endif   //#if( AP4_RP == ENABLE )

    if( display_flag ) {
        /*
              SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
              lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, 24 );
              lcd_goto_xy( 15, 12 );
              Ih_set_curr_font( IDC_FONT_16 );
              lcd_puts( g_string );
        */
        mmi_ap3_display_icon(FALSE);
    }
    display_flag = FALSE;

    return FALSE;
}


#pragma CODE = SEARCH_NORMAL_CODE

void search_normal_state_deal( void )
{
    //game_remote_search = FALSE;
    g_b_enable_flush = TRUE;
    SDKLoadCode( SEARCH_GAME_TEMP_CODE01 );
    //SDKLoadCode( SEARCH_GAME_CODE );
    read_sgdb();
    if( game_remote_search ) {
        SDKLoadCode( SEARCH_IN_GAME_CODE );
        in_game();
    }
    SDKLoadCode( SEARCH_NORMAL_TEMP_CODE );
    normal_search_deal();
}


#pragma CODE = SEARCH_GAME_STATUS_CODE

void search_game_state_deal( void )
{
    if( News_Control.Type_Control == GAME_TYPE ) {
        //if( g_b_recorder_on )
        //   return;
        SDKLoadCode( SEARCH_GAME_TIME_CODE );
        if( game_time_judge())
            return;
        SDKLoadCode( CHECK_GAMETIEM_CODE );
        check_game_time();
        SDKLoadCode( GAME_TIMEOUT_DEAL_CODE );
        game_timeout_deal();

        if( gl_key_press ) {
            SDKLoadCode( SEARCH_GAME_CONSOLE_CODE );
            game_console();
        }
    }

    if( game_exit_to_control ) {
        SDKLoadCode( GAME_EXIT_TO_CONTROL_CODE );
        exit_game_to_control();
    }
}


#pragma CODE = SEARCH_INDEX_COOR_CODE

void judge_index_coor_deal( void )
{
    SDKLoadCode( JUDGE_INDEX_DEAL_CODE );
    if( judge_index_deal())
        return;

    SDKLoadCode( SEARCH_COOR_TEMP_CODE );
    index_coor_deal();
    if( game_remote_search ) {
        game_remote_search = FALSE;
        SDKLoadCode( SEARCH_REMOTE_TEMP_CODE01 );
        if( search_remote_index()) {
            SDKLoadCode( SEARCH_REMOTE_TEMP_CODE02 );
            remote_task();
        }
    }
}

#pragma CODE = BROWER_DIR_INIT_CODE
Bool brower_dir_init(void)
{
    display_page_num = (diandu_entrynum+1)/2;
    ap4current_displaypage_num = 1;
    ap4current_mark = 0;

    SendString((U8 *)"display_page_num=");
    send_hex_u16(display_page_num);
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
    /*lcd_goto_xy( 52, 0 );
    Ih_set_curr_font( IDC_FONT_08 );
    lcd_puts( "READ" );*/
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 0, 8 );
    if(g_setting_list.struct_setting.language_ihm == LANG_CHINESE_SIMP) {
        lcd_puts(( U8 *)"点读文件" );
    } else {
        lcd_puts(( U8 *)"Xdf file" );
    }
//    lcd_goto_xy( 12, 28 );
//    Ih_set_curr_font( IDC_FONT_16 );
//    lcd_puts( g_string );
//  SDKLoadCode( NAVIGATION_CODE );
    browserfile_cmd_id = BROWSER_Cmd_CurFile;
    if(dirnum_diandu != 0) {
        State_Search = Browser_DirFile;
        return TRUE;
    }
    ap3_file_number = ap4num_diandu;
    State_Search = Browser_File_Init;
    return FALSE;
//  Kbd_set_evt_enabled();
//    mmi_ap3_display_icon();
}



#pragma CODE = BROWER_DIR_CODE
void dirfile_filename_display(U16 display_num_mark)
{
    U16 i,j,m,temp_file;
    Bool temp_flag;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//     lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
//     lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
    temp_file = (display_num_mark-1)*DICTEXT_LINE_PAGE;
    if((diandu_entrynum - temp_file) >= DICTEXT_LINE_PAGE ) {
        j = DICTEXT_LINE_PAGE;
    } else {
        j = DICTEXT_LINE_PAGE-1;
    }
    Ih_set_curr_font( IDC_FONT_16 );
    for(i=0; i<j; i++ ) {
        SDKLoadCode( NAVIGATION_CODE );
        if(temp_file < dirnum_diandu) {
            SDKLoadCode( GET_DIR_NAME_CODE );
            temp_flag = Get_Dir_Name(temp_file);
        } else {
            SDKLoadCode( GET_FILE_NAME_CODE );
            temp_flag = Get_File_Name(( U8 *)AP4_EXT, (temp_file - dirnum_diandu) );
        }
        if(temp_flag) {
            /*
              SendString((U8 *)"\r\ng_string=");
              for(m=0;m<127;m++){
                send_hex_u8(g_string[m]);
                SendString((U8 *)":");
                }
              //*/
            lcd_goto_xy( 12, 28+i*20 );
            lcd_puts( g_string );
        }
        temp_file ++;
    }
}

void dirfile_mark_display(void)
{
    U16 i,temp_file;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, 10, (LCD_HEIGHT-24));
    lcd_draw_picture(5,32+20*(ap4current_mark%DICTEXT_LINE_PAGE), IDP_PLAYER_PLAY);
}

void brower_dir(void)
{
    switch( browserfile_cmd_id ) {
        case BROWSER_Cmd_CurFile:
            dirfile_filename_display(ap4current_displaypage_num);
            dirfile_mark_display();
            break;
        case BROWSER_Cmd_PrevFile:
            if((ap4current_mark%DICTEXT_LINE_PAGE)!=0) {
                ap4current_mark --;
                dirfile_mark_display();
            } else {
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
                if(ap4current_displaypage_num!=1) {
                    ap4current_displaypage_num--;
                    ap4current_mark --;
//          SendString((U8 *)"\r\nnewpage");
//          send_hex_u16(adb_file_display_num);
                    dirfile_filename_display(ap4current_displaypage_num);
                    dirfile_mark_display();
                }
            }
            break;

        case BROWSER_Cmd_NextFile:
//      SendString((U8 *)"\r\nFILEBROWSER_Cmd_NextFile");
//      SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
            if((ap4current_mark%DICTEXT_LINE_PAGE)==0) {
                if((ap4current_mark+1)==diandu_entrynum)break;
                ap4current_mark ++;
                dirfile_mark_display();
            } else {
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
                if(ap4current_displaypage_num<display_page_num) {
                    ap4current_displaypage_num++;
                    dirfile_filename_display(ap4current_displaypage_num);
                    ap4current_mark ++;
                    dirfile_mark_display();
                }
            }

            break;

        case BROWSER_Cmd_Enter:
//    Current_Record.Ap3_num = ap4current_mark;
            State_Search =Open_File_Search;//FILEBROWSER_State_End;
            if(ap4current_mark < dirnum_diandu) {
                SendString((U8 *)"\r\nready goto sub!");
                open_dir_num = ap4current_mark + DIR_NUM_OFFSET;
                send_hex_u16(open_dir_num);
                State_Search =READY_GOTO_SUB;//FILEBROWSER_State_End;
            } else {
                Current_Record.Ap3_num = ap4current_mark - dirnum_diandu;
                open_dir_num = 0;
                State_Search =Open_File_Search;//FILEBROWSER_State_End;
            }
            break;

        case BROWSER_Cmd_Back:
            //mail_send_event( EVT_SEARCH_TO_BROWSER, Browser_File_Init );
            //State_Search =Browser_File_Init;//FILEBROWSER_State_End;
            //SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
            //mode_key_long_press(IDLE_APPLI);
            SendString((U8 *)"dir mode back!");
            //search_task_stop();
            //g_select_appli = IDLE_APPLI;
            //Mmgr_kill_this_mmi_appli();
            mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_MODE<<8)|KBD_KEY_LONGPRESSED) );
            break;

    }
    browserfile_cmd_id = BROWSER_Cmd_None;
}

#pragma CODE = BROWER_FILE_INIT_CODE
void brower_file_init(void)
{
    display_page_num = (ap3_file_number+1)/2;
    ap4current_displaypage_num = 1;
    ap4current_mark = 0;
    SendString((U8 *)"\r\ndisplay_subpage_num=");
    send_hex_u16(display_page_num);

//    lcd_goto_xy( 12, 28 );
//    Ih_set_curr_font( IDC_FONT_16 );
//    lcd_puts( g_string );
//  SDKLoadCode( NAVIGATION_CODE );


//  browserfile_cmd_id = BROWSER_Cmd_CurFile;
    State_Search = Browser_File;


//  Kbd_set_evt_enabled();
//    mmi_ap3_display_icon();
}


#pragma CODE = BROWER_FILE_CODE
void AP4_filename_display(U16 display_num_mark)
{
    U16 i,j,m,temp_file;
    Bool temp_flag;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
//     lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
//     lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
    lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
    temp_file = (display_num_mark-1)*DICTEXT_LINE_PAGE;
    if((ap3_file_number - temp_file) >= DICTEXT_LINE_PAGE ) {
        j = DICTEXT_LINE_PAGE;
    } else {
        j = DICTEXT_LINE_PAGE-1;
    }
    Ih_set_curr_font( IDC_FONT_16 );
    for(i=0; i<j; i++ ) {
        SDKLoadCode( NAVIGATION_CODE );
        SDKLoadCode( GET_FILE_NAME_CODE );
        temp_flag = Get_File_Name(( U8 *)AP4_EXT, temp_file );
        if(temp_flag) {
            /*
              SendString((U8 *)"\r\ng_string=");
              for(m=0;m<127;m++){
                send_hex_u8(g_string[m]);
                SendString((U8 *)":");
                }
              //*/
            lcd_goto_xy( 12, 28+i*20 );
            lcd_puts( g_string );
        }
        temp_file ++;
    }
}

void ap4_mark_display(void)
{
    U16 i,temp_file;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, 10, (LCD_HEIGHT-24));
    lcd_draw_picture(5,32+20*(ap4current_mark%DICTEXT_LINE_PAGE), IDP_PLAYER_PLAY);
}

void brower_file(void)
{

    switch( browserfile_cmd_id ) {
        case BROWSER_Cmd_CurFile:
            AP4_filename_display(ap4current_displaypage_num);
            ap4_mark_display();
            break;
        case BROWSER_Cmd_PrevFile:
            if((ap4current_mark%DICTEXT_LINE_PAGE)!=0) {
                ap4current_mark --;
                ap4_mark_display();
            } else {
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
                if(ap4current_displaypage_num!=1) {
                    ap4current_displaypage_num--;
                    ap4current_mark --;
//          SendString((U8 *)"\r\nnewpage");
//          send_hex_u16(adb_file_display_num);
                    AP4_filename_display(ap4current_displaypage_num);
                    ap4_mark_display();
                }
            }
            break;

        case BROWSER_Cmd_NextFile:
//      SendString((U8 *)"\r\nFILEBROWSER_Cmd_NextFile");
//      SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
            if((ap4current_mark%DICTEXT_LINE_PAGE)==0) {
                if((ap4current_mark+1)==ap3_file_number)break;
                ap4current_mark ++;
                ap4_mark_display();
            } else {
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
                if(ap4current_displaypage_num<display_page_num) {
                    ap4current_displaypage_num++;
                    AP4_filename_display(ap4current_displaypage_num);
                    ap4current_mark ++;
                    ap4_mark_display();
                }
            }

            break;

        case BROWSER_Cmd_Enter:
            ap3_poweron_state = MMI_AP3_NORMAL;
            Current_Record.Ap3_num = ap4current_mark;
            State_Search =Open_File_Search;//FILEBROWSER_State_End;
            break;

        case BROWSER_Cmd_Back:
            if(dirnum_diandu != 0) {
                //mail_send_event( EVT_SEARCH_TO_BROWSER, Browser_Dir_Init );
                State_Search = BACK_TO_SUBROOT;
            } else {
                //SDKLoadCode( MMI_AP4_KEY_LONGPRESS_CODE02 );
                //mode_key_long_press(IDLE_APPLI);
                SendString((U8 *)"file mode back!");
                //search_task_stop();
                //g_select_appli = IDLE_APPLI;
                // Mmgr_kill_this_mmi_appli();
                mail_send_event(( U16 )EVT_KEY_PRESSED, (U16)((KEY_MODE<<8)|KBD_KEY_LONGPRESSED));
            }

            //State_Search =Browser_File_Init;//FILEBROWSER_State_End;
            break;

    }
    browserfile_cmd_id = BROWSER_Cmd_None;
}

#pragma CODE = BROWER_BACK_SUBROOT_CODE

void ready_goback_subdir( void )
{
    U16 AP4_DIR_NAME[] = {0x70B9,0x8BFB,0x6587,0x4EF6,0x0000};//点读文件
    g_b_reread_play = FALSE;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 64, 0, 12, 8 );
    // SendString((U8 *)"\r\n reset!");
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( NAVIGATION_TEMP_CODE05 );
    //SDKLoadCode( NAVIGATION_CODE );
    file_init();
    SDKLoadCode( NAVIGATION_TEMP_CODE00 );
    if( !nav_dir_root()) {
        //     SendString((U8 *)"\r\nroot!");
        goto enter_dir;
    }
    //unicode_init_buffer_enc( g_string, G_STRING_SIZE, ENCODING_UTF16BE );
    //string_ascii_to_unicode( g_string, Ext );
    //if( nav_filelist_findname( Unicode_get_addr_string( g_string ), FALSE ))
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    SDKLoadCode( FILE_TEMP_CODE02 );
#if( UNICODE_TYPE == DISABLE )
    nav_string_ascii();  // ASCII mode for name of copy file
#endif
#if( UNICODE_TYPE == ENABLE )
    nav_string_unicode();
#endif
#endif
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
#if( UNICODE_TYPE == DISABLE )
    if( !nav_filelist_findname(( U8 *)AP4_DIR, FALSE ))
#endif
#if( UNICODE_TYPE == ENABLE )
        if( !nav_filelist_findname(( U8 *)AP4_DIR_NAME, FALSE ))
#endif
        {
//     SendString((U8 *)"\r\nno ap4");
            goto enter_dir;
        }
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    SDKLoadCode( FILE_TEMP_CODE02 );
    nav_string_unicode();
#endif
    //SendString((U8 *)"cd\r\n");
    SDKLoadCode( NAVIGATION_TEMP_CODE01 );
    if( !nav_dir_cd()) {
        //     SendString((U8 *)"\r\nno goto");
        goto enter_dir;
    }
    SendString((U8 *)"\r\nreset ok!");
    if(b_BackSubdir) {
        SendString((U8 *)"\r\nback_subdir");
        b_BackSubdir = 0;
        if(dirnum_diandu != 0) {
            State_Search = READY_GOTO_SUB;
            ap4current_mark = open_dir_num;
        } else {
            SendString((U8 *)"\r\nback root");
            State_Search = Browser_Dir_Init;
        }

    } else {
        SendString((U8 *)"\r\nback root");
        State_Search = Browser_Dir_Init;
    }
    return;
enter_dir:
//  SendString((U8 *)"\r\nreset fail!");
    g_select_appli = IDLE_APPLI;
    mail_send_event(( U16 )EVT_KEY_PRESSED, (U16)((KEY_MODE<<8)|KBD_KEY_LONGPRESSED));
    return;
}




#pragma CODE = FIND_BOOKSYS_CODE
U16 read_bnl_file(U16 off_t)
{
    U16 offset,bid = 0;
    U32 temp_off;
    //char tmpbuf[32];
    char rbuf[4];
    __X FCFILE *REC_File;
    __X FS_ERR err;

    U16 temp_min,temp_max,book_off;
#if (REREAD_MODE == ENABLE)
    if(Current_Index == NORMAL_MODE_INDEX) {
        return 0XFFFF;
    }
    if(Current_Index == REREAD_MODE_INDEX) {
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
        temp_min = OID2_XUANSHU_MIN;
        temp_max = OID2_XUANSHU_MAX;
        book_off = OID2_XUANSHU_OFFSET;
    } else {
        temp_min = DPR3_XUANSHU_MIN;
        temp_max = DPR3_XUANSHU_MAX;
        book_off = DPR3_XUANSHU_OFFSET;
    }
#endif
    //temp_min = 59001;
    //temp_max = 61250;
    if((off_t < temp_min)||(off_t > temp_max)) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\n\rout range!");
#endif
        return 0XFFFF;
    }
    /*
        off_t -= book_off;
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
        SendString(( U8 *)"\n\rid=");
        send_hex_u16(bid);
    #endif
        if(bid == 0xFFFF)return 0XFFFF;
        g_xuanshu_num = bid;

        Current_Record.Book_num = 0;
        Current_Record.Page_num = 0;
        Current_Record.Control_num = 0;

        if(State_Search == Browser_File ){
            ap3_poweron_state = MMI_AP3_NORMAL;
            State_Search = Open_File_Search;
            Current_Record.Ap3_num = bid -1;
            }
        else{
            mail_send_event(EVT_GOTO_OID_MODE, 0);
            }
    */
    g_xuanshu_num = off_t;
    g_b_xuanshu_flag = TRUE;
    mail_send_event(EVT_GOTO_OID_MODE, 0);
    return off_t;
}


#pragma CODE = SOME_INDEX_CODE
Bool some_index_dowith( void )
{
    U16 dic_temp,mp3_temp,ap4_temp,v_adc,v_dec,v_min,v_max,diy_min,diy_max;
    if(g_setting_list.struct_setting.bluetooth) {
        dic_temp = OID2_GO_DIC;
        mp3_temp = OID2_GO_MP3;
        //ap4_temp = OID2_GO_AP4;
        v_adc    = OID2_V_ADC_INDEX;
        v_dec    = OID2_V_DEC_INDEX;

        v_min    = OID2_V_1_INDEX;
        v_max    = OID2_V_7_INDEX;

        diy_min  = OID2_DIY_RECORD_MIN;
        diy_max  = OID2_DIY_RECORD_MAX;
    } else {
        dic_temp = DPR3_GO_DIC;
        mp3_temp = DPR3_GO_MP3;
        //ap4_temp = DPR3_GO_AP4;
        v_adc    = DPR3_V_ADC_INDEX;
        v_dec    = DPR3_V_DEC_INDEX;

        v_min    = DPR3_V_1_INDEX;
        v_max    = DPR3_V_7_INDEX;

        diy_min  = DPR3_DIY_RECORD_MIN;
        diy_max  = DPR3_DIY_PLAY_MAX;
    }


    if(Current_Index == dic_temp) {
        g_select_appli = g_old_select_appli = DIC_APPLI;//DIC_APPLI;
    some_index:
        /*
        if(State_Search == Browser_File){
            //search_task_stop();
            //g_select_appli = param;
            Mmgr_kill_this_mmi_appli();
            }
        else{
            remote_bits.value &= ~0x0110u;//0x0880u;
            mail_send_event( EVT_SEARCH_END, g_select_appli );
            }
        */
        remote_bits.value &= ~0x0110u;//0x0880u;
        mail_send_event( EVT_SEARCH_END, g_select_appli );
        return TRUE;
    }
    if(Current_Index == mp3_temp) {
        g_select_appli = g_old_select_appli = PLAYER_APPLI;//MP3_APPLI;
        //remote_bits.value &= ~0x0110u;//0x0880u;
        //mail_send_event( EVT_SEARCH_END, PLAYER_APPLI );
        goto some_index;
    }
    /*
     if(Current_Index == ap4_temp){
    g_select_appli = g_old_select_appli = AP3_APPLI;//AP3_APPLI;
    Mmgr_kill_this_mmi_appli();
    CommonLoadCode(C_AP4);
    return TRUE;
    }
    */
#if (DIY_REC == ENABLE)
    if((Current_Index >= diy_min)&&(Current_Index <= diy_max)) {
        reread_num = Current_Index;
        g_select_appli = g_old_select_appli = REREAD_APPLI;//DIC_APPLI;
        goto some_index;
    }
#endif

    if(Current_Index == v_adc) {
        //mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_UP<<8)|KBD_KEY_RELEASED));
        // mail_send_event( EVT_REMOTE,(Current_Index - v_min + REMOTE_VOL0) );
        //srvplayer_volume_change( SRVPLAYER_VOL_UP );
        //mmi_ap3_display_volume();
        if( g_volume < (VOL_LEVEL-1)) {
            g_volume++;
            mail_send_event( EVT_CHANGE_AP4_VOL, 0 );
        }
        return TRUE;
    }
    if(Current_Index == v_dec) {
        //mail_send_event(EVT_KEY_PRESSED, (U16)((KEY_VOL_DOWN<<8)|KBD_KEY_RELEASED));
        //mail_send_event( EVT_REMOTE, REMOTE_VOL_DOWN);
        //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
        //mmi_ap3_display_volume();

        if( g_volume ) {
            g_volume--;
            mail_send_event( EVT_CHANGE_AP4_VOL, 0 );
        }
        return TRUE;
    }
    if((Current_Index >= v_min)&&(Current_Index <= v_max)) {
        g_volume = Current_Index- v_min;
        mail_send_event( EVT_CHANGE_AP4_VOL, 0 );
        // srvplayer_volume_send();
        //mmi_ap3_display_volume();
        //mail_send_event( EVT_REMOTE,(Current_Index - v_min + REMOTE_VOL0) );
        return TRUE;
    }
    if(Current_Index == CHANGE_OID_TYPE) {
        SendString(( U8 *)"\n\roid_change!");
        mail_send_event( EVT_CHANGE_OID_TYPE, 0 );
        return TRUE;
    }
    return FALSE;
}


#pragma CODE = READY_GO_SUB_CODE
void ready_go_sub(U16 subdir_num)
{
    Bool temp_flag;
    //U16  temp_num;
#define  Wait_10ms()    timer = Timer_set_timer(TIMER_MS(3000)); \
                           while (!Timer_is_timeout(timer));
    U32 timer;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 64, 0, 12, 8 );
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( GET_DIR_NAME_CODE );
    temp_flag = Get_Dir_Name(subdir_num);

    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 0, 8 );
    lcd_puts( g_string );
    /*
        SDKLoadCode( NAVIGATION_TEMP_CODE03 );
       if( nav_filelist_findname(( U8 *)ap4_short_name, FALSE ))
       {
          //SendString((U8 *)"find\r\n");
          goto fail_find;
       }
    //*/
    SDKLoadCode( FILE_TEMP_CODE02 );
#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_unicode();
#endif
    //SendString((U8 *)"cd\r\n");
    SDKLoadCode( NAVIGATION_TEMP_CODE01 );
    if( !nav_dir_cd()) {
//      SendString((U8 *)"\r\nsub fail!");
        goto fail_find;
    }
//    SendString((U8 *)"\r\nsub ok!");
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode(MMI_AP4_SEARCH_TEMP_CODE04);
    ap3_file_number = Get_File_Number(( U8 *)AP4_EXT );
//  ap3_file_number = 3;
    SendString((U8 *)"\r\nsub ap4=");
    send_hex_u16(ap3_file_number);
    if( ap3_file_number == 0 ) {
        goto fail_find;
    }
    State_Search = Browser_File_Init;
    return;
fail_find:
    lcd_goto_xy(15, 26 );
    if(g_setting_list.struct_setting.language_ihm == LANG_ENGLISH) {
        lcd_puts(( U8 *)"No file" );
    } else {
        lcd_puts(( U8 *)"无文件" );
    }
    Wait_10ms();
    State_Search = BACK_TO_SUBROOT;
    return;
}
/*
#pragma CODE = BROWER_UNIT_INIT_CODE
void brower_PAGE_init(void)
{
    unit_display_num = (News_Book.Num_GoTable+1)/2;
    unitcurrent_display_num = 1;
    unitcurrent_mark = 0;
   SendString((U8 *)"unit_display_num=");
   send_hex_u16(unit_display_num);
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 8, LCD_WIDTH, (LCD_HEIGHT-8) );
    Ih_set_curr_font( IDC_FONT_16 );
    lcd_goto_xy( 0, 8 );
    lcd_puts(g_string);
//    lcd_goto_xy( 12, 28 );
//    Ih_set_curr_font( IDC_FONT_16 );
//    lcd_puts( g_string );
//  SDKLoadCode( NAVIGATION_CODE );
    browserfile_cmd_id = BROWSER_Cmd_CurFile;
    State_Search = Browser_File;
//  Kbd_set_evt_enabled();
//    mmi_ap3_display_icon();
}
#pragma CODE = BROWER_UNITE_CODE

void unit_filename_display(U16 display_num_mark)
{
    U16 i,j,m,temp_file;
    U8 temp_num;
    char temp_char[3];
    char unit_name[ 13 ];
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //     lcd_clear_part_of_screen( 0, 8, FILEBROWSER_Width_FileItem, 24 );
 //     lcd_goto_xy( FILEBROWSER_PosFile_StartX, FILEBROWSER_PosFile_StartY );
   lcd_clear_part_of_screen( 0, 24, LCD_WIDTH, (LCD_HEIGHT-24));
   temp_file = (display_num_mark-1)*DICTEXT_LINE_PAGE;
   if((News_Book.Num_GoTable - temp_file) >= DICTEXT_LINE_PAGE ){
    j = DICTEXT_LINE_PAGE;
    }
   else{
    j = DICTEXT_LINE_PAGE-1;
    }
   Ih_set_curr_font( IDC_FONT_16 );
  for(i=0;i<j;i++ ){
     lcd_goto_xy( 12, 28+i*20 );
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
   strcpy(unit_name,"Unit");
#elif( LANGUAGE == Simplified )
   strcpy(unit_name,"单元");
   //lcd_puts(( U8 *)"跟读录音" );
//#elif( LANGUAGE == Traditional )

#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )
#if( SELECT_LANGUAGE == ENABLE )
   switch( g_setting_list.struct_setting.language_ihm )
   {
      case LANG_ENGLISH:
      strcpy(unit_name,"Unit");
      break;

      case LANG_CHINESE_SIMP:
      strcpy(unit_name,"单元");
      break;
   }
#endif   //#if( SELECT_LANGUAGE == ENABLE )
     temp_num = display_num_mark;
     temp_char[0] = temp_num/10 + '0';
     temp_char[1] = temp_num%10 + '0';
     temp_char[2] = '\0';
     strcat(( char *)unit_name,( char *)&temp_char);
     lcd_puts( (U8 *)unit_name );
     temp_file ++;
    }
}

void unit_mark_display(void)
{
    U16 i,temp_file;
    SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
    lcd_clear_part_of_screen( 0, 24, 10, (LCD_HEIGHT-24));
    lcd_draw_picture(5,32+20*(unitcurrent_mark%DICTEXT_LINE_PAGE), IDP_PLAYER_PLAY);
}

void brower_unit(void)
{

   switch( browserfile_cmd_id )
   {
      case BROWSER_Cmd_CurFile:
        unit_filename_display(unitcurrent_display_num);
        unit_mark_display();
        break;
      case BROWSER_Cmd_PrevFile:
      if((unitcurrent_mark%DICTEXT_LINE_PAGE)!=0){
        unitcurrent_mark --;
        unit_mark_display();
        }
      else{
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
        if(unitcurrent_display_num!=1){
            unitcurrent_display_num--;
            unitcurrent_mark --;
//          SendString((U8 *)"\r\nnewpage");
//          send_hex_u16(adb_file_display_num);
            unit_filename_display(unitcurrent_display_num);
            unit_mark_display();
            }
        }
      break;

      case BROWSER_Cmd_NextFile:
//      SendString((U8 *)"\r\nFILEBROWSER_Cmd_NextFile");
//      SDKLoadCode( TEXT_VIEWER_TEMP_CODE17 );
      if((unitcurrent_mark%DICTEXT_LINE_PAGE)==0){
        if((unitcurrent_mark+1)==News_Book.Num_GoTable)break;
        unitcurrent_mark ++;
        unit_mark_display();
        }
      else{
//      SendString((U8 *)"\r\nnowpage");
//      send_hex_u16(adb_file_display_num);
        if(unitcurrent_display_num<ap4file_display_num){
            unitcurrent_display_num++;
            unit_filename_display(unitcurrent_display_num);
            unitcurrent_mark ++;
            unit_mark_display();
            }
        }

      break;

      case BROWSER_Cmd_Enter:
      Current_Record.Ap3_num = ap4current_mark;
      State_Search =Open_File_Search;//FILEBROWSER_State_End;
      break;

   }
   browserfile_cmd_id = BROWSER_Cmd_None;
}
//*/
#endif
#endif  //#if( AP3 == ENABLE )

#endif  //_search_c_
