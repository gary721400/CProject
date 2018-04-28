//!
//! @file mmi_startup.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application startup
//!
//! Contains startup and animation management
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: mmi_startup.c,v 2.3 2007/09/24 09:46:24 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_startup_c_
#define _mmi_startup_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\timer_soft\timer_soft.h"

#include "mmi\shared\com_appli.h"
#include "mmi\shared\com_evt.h"
#include "mmi\shared\com_var.h"
#include "mmi\kbd\keypad.h"
#include "mmi_startup.h"

#include <stdio.h>

#include "modules\mmi_manager\mmgr_kernel.h"
//#include "lib_system\utilities\string.h"
//#include "mmi\ap3\mmi_ap3.h"
//#include "mmi\player\mmi_player.h"
//#include "modules\paster\paster_base.h"
//#include "modules/text/text_viewer.h"
#include "modules\file_system\file.h"           // for file management 
#include "modules\power\power.h"             // Power module definition
#include "mmi\player\srv_player.h"
#include "modules\player\player.h"
#include "conf\conf_sio.h"
//#include "SSSMP3_Regs.h"
#include "modules\player\player_task.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
#include "mmi\player\srv_player.h"

#include "sio\com.h"
#include "string.h"
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
#include "drivers\oid\oid.h"
#if 0//( DIC == ENABLE )
#include "mmi\dictionary\mmi_dictionary.h"
#endif
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fs.h"

#include "USB_MAssStorage.h"
#include "modules\file_system\navigation.h"
#include "mmi\player\mmi_player.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
#include "drivers\mmi\img_handler.h"
#include "mmi\status\mmi_status.h"

#pragma DATA = MMI_STARTUP_DATA
#pragma CODE = MMI_STARTUP_CODE

ImageDeclareType( MMI_PLAYINTRO_CODE00 );
ImageDeclareType( MMI_PLAYINTRO_CODE01 );

ImageDeclareType( NAVIGATION_CODE );
ImageDeclareType( NAVIGATION_TEMP_CODE00 );
ImageDeclareType( NAVIGATION_TEMP_CODE01 );
ImageDeclareType( NAVIGATION_TEMP_CODE02 );
ImageDeclareType( NAVIGATION_TEMP_CODE03 );
ImageDeclareType( NAVIGATION_TEMP_CODE04 );
ImageDeclareType( NAVIGATION_TEMP_CODE05 );

ImageDeclareType( LCD_SSD1303_TEMP_CODE00 );
ImageDeclareType( LCD_SSD1303_TEMP_CODE02 );

ImageDeclareType( MMI_STARTUP_TEMP_CODE00 );
ImageDeclareType( MMI_STARTUP_TEMP_CODE01 );

ImageDeclareType( POWER_TEMP_CODE00 );

ImageDeclareType( UPDATE_FIRMWARE_CODE );

ImageDeclareType( POWER_TASK_TEMP_CODE05 );

ImageDeclareType( HARDCHECK_CODE );

// Work package version
//#define WP_NAME                  "V4 V1.0.1.2A"

//_MEM_TYPE_MEDSLOW_   U8    internal_speech = 0;

//                     Bool  play_startup_flag = FALSE;

//extern   xdata    AL_Variables        varAl;
__X unsigned int CalData[ 128 ];
unsigned int cal_data_1;
unsigned int cal_data_2;
char    __X g_full_path[128];

//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

// ID of the timer soft that delays the display of the start-up splash screens
static _MEM_TYPE_SLOW_ U8  s_ts_id;

void  mmistartup_force_build_NF     ( void );
void  mmc_sd_all_mode_dir_make      ( void );
Bool  update_firmware_deal          ( void );
void  test_rtc_timer(void);
void mmi_startup_display_volume( void );
Bool display_systemname( void );
extern void  lcd_display_time         ( Bool flag);
void find_sys_dir( void );
//void recover_oid_cal(void);
//void read_oid_cal(void);
void read_cab(void);
Bool save_oid_cal(void);

Bool check_hard_news(void);
void check_hard_version(void);
void set_headphone_pin(void);
//_____ D E F I N I T I O N S ______________________________________________
#if 0
void __interrupt rtc_isr( void )
{
    b_flag_rtc = TRUE;
    RTCHDT = 0x0000;
    RTCMST = 0;
    RTCTBR = 0x0010;
    __asm{
        bset RTCMST.15
        bset RTCICR.0
        bset RTCTBR.15
    }
}

void init_rtc_timer(void)
{

    SendString((U8 *)"\r\ninit_rtc_timer");
//  __asm{bset MODCON.13}
//  MODCON |= 0x20;
    //__asm{ BSET MODCON.13 }
//  g_setting_list.struct_setting.min = 0;
    b_flag_rtc = FALSE;

    if(!b_rtc_set_flag) {
        b_rtc_set_flag = TRUE;
        RTCHDT = 0x0000;
        // RTCMST = g_setting_list.struct_setting.min<<4;
        // RTCMST = 0;
        RTCMST = 0;
        RTCTBR = 0x0010;
    }

    SetVector( RTC_INT_TRAP, rtc_isr );
//  g_setting_list.struct_setting.min ++;
//  RTCTBR = g_setting_list.struct_setting.min<<4;

    __asm{
        bset MODCON.13
        bset RTCMST.15
        bset RTCICR.0
        bset RTCTBR.15
    }
    /*
      SendString((U8 *)"\r\nset time=");
      send_hex_u8((0x1F&RTCHDT));
      SendString((U8 *)"/");
      send_hex_u8((0x1F&(RTCMST>>6)));
    */
}
#endif
//! Manage the application startup
//!
//! @param U8  event
//!        U16 param
//!
void startup_mmi_appli(U8 event, U16 param)
{
    //U8 _MEM_TYPE_SLOW_ sz_label[] = "A10F(V4)";
    U8 i,j;
    U8 temp_char[26];
    char temp_lab[12];
    char sys_lab[12];
    char digi[6];
    switch( event ) {
        case EVT_START_APPLI:
            SendString((U8 *)"\r\nstartup\r\n");

#if 0//( OID == ENABLE )
            //b_oid_init_flag = FALSE;
            if( !b_oid_init_flag ) {
                reinit_oid_pen();
            }
#endif   //#if( OID == ENABLE )
            /*
              SDKLoadCode(HARDCHECK_CODE);
              if(!check_hard_news()){
                check_hard_version();
                }

              set_headphone_pin();
            */
            SDKLoadCode( MMI_STARTUP_TEMP_CODE00 );
            mmistartup_force_build_NF();
            
            SendString((U8 *)"\r\nstudy_AP4:Unite = ");
            send_hex_u16(gStudyAp4);
            SendString((U8 *)":");
            send_hex_u16(gStudyUnite);
    
            SDKLoadCode( MMI_STARTUP_TEMP_CODE01 );
            mmc_sd_all_mode_dir_make();

            find_sys_dir();
#if( LCD_PRESENT==ENABLED )
            SDKLoadCode( LCD_SSD1303_TEMP_CODE00 );
//    SendString((U8 *)"init lcd begin\r\n");
            lcd_init();
            SendString((U8 *)"\r\ninit lcd over\r\n");
            Lcd_start_backlight_timer();
            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_screen();
            lcd_turn_on();
            //test_rtc_timer();
//    init_rtc_timer();

//    g_setting_list.struct_setting.hour = 23;
//    g_setting_list.struct_setting.min = 55;
            SDKLoadCode(POWER_TASK_TEMP_CODE05);
            lcd_display_time(FALSE);
#endif
            /*
                  Lcd_set_merging();
            //      lcd_goto_xy( 2, 0 );
            //      Ih_set_curr_font( IDC_FONT_08 );
            //    lcd_puts(( U8 *)"01250125");

             //     lcd_puts(( U8 *)WP_NAME );
                  Ih_set_curr_font( IDC_FONT_08);

                  for(j=0;j<4;j++){
                      lcd_goto_xy( 0, (j+1)*8 );
                      //lcd_goto_xy( 16, 8 );

                      for(i=0;i<24;i++){
                        temp_char[i] = i +32+ j*24;
                        }
                      temp_char[24] = '\0';
                      temp_char[25] = '\0';
                      lcd_puts(( U8 *)temp_char);
                        }

            //    lcd_goto_xy( 0, 24 );
            //      lcd_puts(( U8 *)"abcdefghigklmnopqrstuvwxyz" );

                  lcd_goto_xy( 0, 40 );
            //    Ih_set_curr_font( IDC_FONT_16 );
            //      lcd_puts(( U8 *)"我是 中国$!@#%^&人 MNOP" );
            //      lcd_draw_picture(2, 0, 0x75B0);
                  lcd_draw_picture(120, 0, 0x75B0);
            //    lcd_draw_picture(100,40,0x1CA7);
                  //lcd_puts(( U8 *)"0");
                  while(1)
                    {
                       if(b_flag_rtc){
                        b_flag_rtc = FALSE;
            //          SDKLoadCode(POWER_TASK_TEMP_CODE05);
                        lcd_display_time(TRUE);

                        }
                    };
            //*/

            //while(1){}
            if(display_systemname()) {
                lcd_goto_xy( 8, 16 );
                Ih_set_curr_font( IDC_FONT_16 );
                j = g_setting_list.struct_setting.language_ihm;
                g_setting_list.struct_setting.language_ihm = LANG_CHINESE_SIMP;
                lcd_puts(( U8 *)g_string);
                g_setting_list.struct_setting.language_ihm = j;
            } else {
//        lcd_goto_xy( 16, 8 );
//        Ih_set_curr_font( IDC_FONT_24 );
//        lcd_puts(( U8 *)"Welcome!" );
//        lcd_draw_picture( 2, 14, IDP_STATUS_DINGYA_LOG);
//          lcd_draw_picture( 2, 14, IDP_STATUS_WAIYAN_LOG);
                lcd_draw_picture( 0, 0, IDP_STATUS_NEWEAST_LOG);
            }

            read_cab();
            SDKLoadCode( UPDATE_FIRMWARE_CODE );
            if(update_firmware_deal()) {
                (void)save_oid_cal();
                SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
                lcd_clear_screen();
                lcd_goto_xy( 16, 8 );
                Ih_set_curr_font( IDC_FONT_16 );
                lcd_puts(( U8 *)"Updating" );
                CommonLoadCode( C_UPDATE );
            }
            //srvplayer_volume_send();

            //digi[0] = HARD_VERSION;
            //digi[1] = MAJOR_FW_VERSION;
            //digi[2] = MINOR_FW_VERSION;
            //digi[3] = 0;
            memset(sys_lab,0,12);
            strcpy(sys_lab,SYS_LAB);
            strcat(sys_lab,(char *)HARD_VERSION);
            strcat(sys_lab,(char *)MAJOR_FW_VERSION);
            strcat(sys_lab,(char *)MINOR_FW_VERSION);

            FSVolume(0,temp_lab,0);
            if(strcmp(temp_lab,sys_lab)) {
                FSVolume(0,sys_lab,1);
            }

            // FSVolume( 0, "X99(V4)", 1 );
            /*if( nav_drive_set( LUN_ID_MMC_SD ))
            {
               nav_partition_label( FS_LABEL_WRITE, sz_label );
            }*/
            // lcd_draw_picture( 64, 0, IDP_STATUS_BLUETOOTH );

            //lcd_goto_xy( 94, 0 );
            // Ih_set_curr_font( IDC_FONT_08);
            // lcd_puts(( U8 *)"OID2" );

            //    lcd_goto_xy( 60, 0 );
            //Ih_set_curr_font( IDC_FONT_08);
            //lcd_puts(( U8 *)"DPR3" );

            //lcd_draw_picture( 94, 0, IDP_OID2_FLAG);
            //lcd_draw_picture( 60, 0, IDP_DPR3_FLAG);

            //while(1){};
            lcd_draw_picture( 0, 0, IDP_VOLUME );
            mmi_startup_display_volume();

            Kbd_set_evt_enabled();
            if( Kbd_is_event_triggered())
                Kbd_set_evt_disabled();

//***********************************************
            if( power_is_vbus_present()) {
                s_ts_id = ts_alloc();
                ts_set_time( s_ts_id, SPLASH_TIME );
                break;
            }
            play_startup_flag = TRUE;
            // EXEMPLE PLAY ONE FILE

            if( !mmi_startup_playintro() ) {
                s_ts_id = ts_alloc();
                ts_set_time( s_ts_id, SPLASH_TIME );
                // Play fail then startup finish
            }
            break;
#if 0
        case EVT_APPLI_KILLED:
        case EVT_BACK_TO_TOP:
            break;
#endif
        case EVT_TIMER:
            if( LSB(param) != s_ts_id )
                break;
            // Startup finish
            //s_startup_step = DEVICE_STATUS;
            Ts_free( s_ts_id );
            Mmgr_kill_this_mmi_appli();
            break;

        case EVT_PLAYER:
            if( param == PLAYER_EOF ) {
                //srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
                //srvplayer_switch_off();  // Power off player
                //power_authorize_autopoweroff();
                Mmgr_kill_this_mmi_appli();
            }
            break;

#if 0
        case EVT_ANIMATION:
            break;
#endif
        case EVT_KEY_PRESSED:
            if( KBD_KEY_RELEASED == KEY_STATUS( param )) {
                switch( KEY_ID( param )) {
                    case KEY_VOL_UP:
                        srvplayer_volume_change( SRVPLAYER_VOL_UP );
                        //srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                        break;

                    case KEY_VOL_DOWN:
                        srvplayer_volume_change( SRVPLAYER_VOL_DOWN );
                        //srvplayer_volume_change( SRVPLAYER_VOL_UP );
                        break;

                }
            }
            break;

        /*case EVT_EXP_MEMORY_MMCSD_CHANGE:
        power_system_off( !power_is_vbus_present());
        break;*/

        case EVT_ALERT_BATTERY_LOW:
            SendString((U8 *)"\r\nEVT_ALERT_BATTERY_LOW");
            power_system_off( TRUE );
            break;

        case EVT_USB_CTRL_UNPOWERED:
            SendString((U8 *)"\r\nEVT_USB_CTRL_UNPOWERED");
            //mail_send_command(CMD_POWER_OFF, 0);
            Power_set_poweroff_activated();

            SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
            lcd_clear_screen();
            //Display off
            lcd_turn_off();
            // Reset the OLED controller
            Lcd_reset_on();

            SDKLoadCode( POWER_TEMP_CODE00 );
            power_system_off( TRUE );
            break;
    }
}


//  Start a  play
//  Note: you can't use FileSytem and player service before "mmistartup_force_build_NF()"
Bool mmi_startup_playintro( void )
{
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    // Search file to play
    if( !search_subdir_mp3_file( "startup.mp3" ))
        return FALSE;

    SDKLoadCode( MMI_PLAYINTRO_CODE01 );
    if( !play_sbdir_mp3_file())
        return FALSE;
    return TRUE;
}

char *convert_U8( char *p, U8 i )
{
    if( i / 10 )
        p = convert_U8( p, i / 10 );
    //else if( i < 0 )
    //   *p++ = '-';
    return *( char *)p = (/* i % 10 < 0 ? -1 *( i % 10 ): */i % 10 )+ '0', p + 1;
}

void mmi_startup_display_volume( void )
{
    _MEM_TYPE_SLOW_ U8 string[ 8 ], i;


    Ih_set_curr_font( IDC_FONT_08 );
    lcd_goto_xy( 6, 0 );
    //sprintf( string , "%02bu", srvplayer_g_arg.volume );
    memset( string, 0, 8 );
    convert_U8(( char *)&string[ 4 ], g_volume );
    i = strlen(( char *)&string[ 4 ] );
    for( ; i < 2; i++ )
        strcat(( char *)string, "0" );
    strcat(( char *)string, ( char *)&string[ 4 ]);
    lcd_puts( string );
}

Bool display_systemname( void )
{
    FS_ERR fs_err;
    __X U32 file_size;
    __X FCFILE *Temp_File;
    unsigned long temp_len;
    char temp_file[32];

//  char temp_file[]="0:\\subdir\\name.sys";
// strcpy( temp_file,SUBDIR_PATH );
    strcpy( temp_file, (char *)&system_dir);
    strcat( temp_file, "name.sys" );
    temp_len = FSGetFileSize((char *)temp_file, &fs_err);
    if(temp_len>0) {
        Temp_File = FSOpen((char *)temp_file,FS_MODE_READ,&fs_err);
        FSSeek(Temp_File,0);
        memset(g_string,0,G_STRING_SIZE);
        FSRead(Temp_File, (void *)&g_string,temp_len,&fs_err);
        FSClose(Temp_File);
        return TRUE;
    } else {
        return FALSE;
    }
}

void find_sys_dir( void )
{
#if (UNICODE_TYPE == ENABLED)
    U16 *p;
    U16 SYS_DIR_NAME[] = {0x7CFB,0x7EDF,0x6587,0x4EF6,0x0000};
    //nav_string_unicode();
    memset( system_dir, 0, sizeof( system_dir ));
    p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"0:\\"), 3+1 );
    p = str_copy_u_n( p, ( U16 *)SYS_DIR_NAME, 4+1 );
    p = str_copy_u_n( p, atou(( U8 *)"\\"), 1+1 );
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    (void)find_path_u(( U16 *)tisiFileName, (U8 *)system_dir );
    strcat((char *)system_dir,(char *)"\\");
#endif
#if (UNICODE_TYPE == DISABLED)
    strcpy( (char *)system_dir, SUBDIR_PATH );
#endif
    SendString((U8 *)"\r\nsystem_dir=");
    SendString((U8 *)system_dir);
}

#if 0//( AP3 == ENABLE )
//
void search_startup( void )
{
    internal_speech &= ~STARTUP_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "startup.mp3" ))
        internal_speech |= STARTUP_SPEECH;
}


//
void search_poweroff( void )
{
    internal_speech &= ~POWEROFF_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "poweroff.mp3" ))
        internal_speech |= POWEROFF_SPEECH;
}


//
void search_batterylow( void )
{
    internal_speech &= ~BATTERYLOW_SPEECH;
    // Search file to play
    if( search_subdir_mp3_file( "batterylow.mp3" ))
        internal_speech |= BATTERYLOW_SPEECH;
}
#endif   //#if( AP3 == ENABLE )

#if 0
Bool play_sbdir_mp3_file( void )
{
    extern Bool g_b_play_ap3;
    if( fs_g_nav_entry.u32_size < FS_512B*4 ) {
        //file_close();
        return FALSE;
    }
    varAl.WAVE_Flg = 0;
    wav_encrypt = FALSE;
    srvplayer_volume_send();
    //LROM_SCRAMBLE = 0x00;
    //LROM_EXCHANGE = 0x00;
    g_b_play_ap3 = FALSE;
    if( !player_play_file( 0 )) {
        return FALSE;
    }
    power_forbid_autopoweroff();

    srvplayer_g_player_state   = MMI_PLAYER_STATE_PLAY;
    player_task_start();       // Enable event of player task
    return TRUE;
}
#endif

#if( UNICODE_TYPE == ENABLE )
Bool  nav_unicode_dir_make( FS_STRING short_name,FS_STRING sz_name  )
{
    char __X temp[ 32 ];
    FS_ERR error;

    //strcpy(( char *)temp, ( char *)CurrentShortFilePath );
    //strcpy(( char *)temp, ( char *)"1~5" );
    //strcat(( char *)temp, ( char *)sz_name );
    //error = FSDirectory(( char *)temp );
    //error = FSDirForUnicode( ( char *)temp, ( char *)sz_name, ( U8 *)atou( sz_name ));
    error = FSDirForUnicode( ( char *)"0:", ( char *)short_name, ( U8 *)sz_name);
    SendString((U8 *)"\r\nerror=");
    send_hex_u8(error);
    if( error == FS_DIRECTORY_CONFLICT )
        fs_g_status = FS_ERR_FILE_EXIST;
    return   ( error == FS_NO_ERROR );
}
#endif



Bool save_oid_cal(void)
{
    //__X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 temp_i;
    SPIF_ERR cur_err;
    __X U16 CalData[ 128 ];
#if 1//( T01OID_CAB == ENABLE )     

    //GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 1//def SPIBOOT_KBDM9
    //spif_resv_read( DataStartSector, TempBuf );
#endif
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString((U8 *)"\r\nsave cab data\r\n");
    //if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ){
    //   }
    //else{
    // MSW( t01_calib_data1 ) = TempBuf[ 0 ];
    // LSW( t01_calib_data1 ) = TempBuf[ 1 ];
    // MSW( t01_calib_data2 ) = TempBuf[ 2 ];
    //LSW( t01_calib_data2 ) = TempBuf[ 3 ];
    // SendString("\r\ncal=");
    //send_hex_u32(t01_calib_data1);
    // SendString(":");
    //send_hex_u32(t01_calib_data2);
    ///*

    if( spif_mount() != SPIF_No_Err ) {
        SendString((U8 *)"\r\nspi mount fail!");
        return FALSE;
    }

    // Configure the flash to read/write mode.
    SPIF_Read_Write();

    // memset( CalData, 0, sizeof( CalData ));
    for(temp_i=0; temp_i<128; temp_i++) {
        CalData[temp_i] = 0;
    }
    CalData[0] = 0x1111;
    CalData[1] = 0x2222;


    CalData[2] = t01_calib_data1 ;
    CalData[3] = t01_calib_data2 ;
    CalData[4] = 0;
    CalData[5] = 0;

    CalData[6] = 0;
    CalData[7] = 0;

    CalData[8] = 0x3333;
    CalData[9] = 0x4444;

    SendString((U8 *)"\r\nread save data = ");
    for(temp_i=0; temp_i<10; temp_i++) {
        send_hex_u16(CalData[temp_i]);
        SendString((U8 *)":");
    }
    //DataStartSector = (U32)(4086UL<<10);
    //DataStartSector = (U32)(0x003FD800);
    DataStartSector = SAVE_OID_CAL_ADDR;

    SendString((U8 *)"\r\nwrite_addr=");
    send_hex_u32(DataStartSector);
    if(( SPIF_Page_Program( DataStartSector, (__X unsigned char *)&CalData, 1 )) != SPIF_No_Err ) {
        SendString((U8 *)"\r\nspi program error");
        return FALSE;
    }

    DataStartSector = SAVE_OID_CAL_ADDR;
    SendString((U8 *)"\r\nread_addr=");
    send_hex_u32(DataStartSector);

    if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&CalData, sizeof(CalData) ) != SPIF_No_Err ) {
        SendString((U8 *)"\r\nspi read error");
        return FALSE;
    }

    SendString((U8 *)"\r\nsave data = ");
    for(temp_i=0; temp_i<10; temp_i++) {
        send_hex_u16(CalData[temp_i]);
        SendString((U8 *)":");
    }
    return TRUE;
    //*/
    //}
#endif //

}


Bool read_spi_data(void)
{
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 256 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;
    cal_data_1 = 0;
    cal_data_2 = 0;
    if( spif_mount() != SPIF_No_Err ) {
        SendString((U8 *)"\r\nspi mount fail!");
        return FALSE;
    }

    // Configure the flash to read/write mode.
    SPIF_Read_Write();

    //DataStartSector = (U32)(4086UL<<10);
    //DataStartSector = 1040384UL;
    DataStartSector = SAVE_OID_CAL_ADDR;
    //if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
    // cur_err = spif_write(DataStartSector,(__X void *)CalData);
    if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&CalData, sizeof(CalData) ) != SPIF_No_Err )
        /// if(cur_err != SPIF_No_Err)
    {
        SendString((U8 *)"\r\nspi read error");
        return FALSE;
    }

    SendString((U8 *)"\r\nrecover data = ");
    for(temp_i=0; temp_i<10; temp_i++) {
        send_hex_u16(CalData[temp_i]);
        SendString((U8 *)":");
    }
    if((CalData[0] == 0x1111)&&(CalData[1] == 0x2222)
       &&(CalData[8] == 0x3333)&&(CalData[9] == 0x4444)) {
        //memset( CalData, 0, sizeof( CalData ));
        //CalData[ 0 ] = TempBuf[ 2 ];
        //CalData[ 1 ] = TempBuf[ 3 ];
        //CalData[ 2 ] = TempBuf[ 4 ];
        //CalData[ 3 ] = TempBuf[ 5 ];
        //CalData[ 4 ] = TempBuf[ 6 ];
        //CalData[ 5 ] = TempBuf[ 7 ];

        cal_data_1  = CalData[ 2 ];
        cal_data_2  = CalData[ 3 ];


        //LSW( cal_data_1 ) = CalData[ 2 ];
        //MSW( cal_data_1 ) = CalData[ 3 ];
        //LSW( cal_data_2 ) = CalData[ 4 ];
        //MSW( cal_data_2 ) = CalData[ 5 ];
        if((cal_data_1 == 0)||(cal_data_2 == 0)) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}


void recover_oid_cal(void)
{
#if 1
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 256 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;

    //GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SPIBOOT_KBDM9
    spif_resv_read( DataStartSector, TempBuf );
#endif


    memset( CalData, 0, sizeof( CalData ));
    //CalData[ 0 ] = TempBuf[ 2 ];
    //CalData[ 1 ] = TempBuf[ 3 ];
    //CalData[ 2 ] = TempBuf[ 4 ];
    //CalData[ 3 ] = TempBuf[ 5 ];
    //CalData[ 4 ] = TempBuf[ 6 ];
    //CalData[ 5 ] = TempBuf[ 7 ];

    CalData[ 0 ] =  cal_data_1 ;
    CalData[ 1 ] =  cal_data_2 ;

    //CalData[ 0 ] = MSW( cal_data_1 );
    //CalData[ 1 ] = LSW( cal_data_1 );
    //CalData[ 2 ] = MSW( cal_data_2 );
    //CalData[ 3 ] = LSW( cal_data_2 );



    GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
    //Get_Calibration_Data( CalibrationData );
    CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
    sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
    spif_resv_write( DataStartSector, CalData );
#endif
    SendString((U8 *)"\r\nrecover ok!");



#else
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 256 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;
#if( T01OID_CAB == ENABLE )

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
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString("\r\nrecover read cab data\r\n");
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {

        if( spif_mount() != SPIF_No_Err ) {
            SendString("\r\nspi mount fail!");
            return;
        }

        // Configure the flash to read/write mode.
        SPIF_Read_Write();

        //DataStartSector = (U32)(4086UL<<10);
        //DataStartSector = 1040384UL;
        DataStartSector = SAVE_OID_CAL_ADDR;
        //if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
        // cur_err = spif_write(DataStartSector,(__X void *)CalData);
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&CalData, sizeof(CalData) ) != SPIF_No_Err )
            /// if(cur_err != SPIF_No_Err)
        {
            SendString("\r\nspi read error");
            return;
        }

        SendString("\r\nrecover data = ");
        for(temp_i=0; temp_i<10; temp_i++) {
            send_hex_u16(TempBuf[temp_i]);
            SendString(":");
        }

        if((TempBuf[0] == 0x1111)&&(TempBuf[1] == 0x2222)
           &&(TempBuf[8] == 0x3333)&&(TempBuf[9] == 0x4444)) {
            memset( CalData, 0, sizeof( CalData ));
            CalData[ 0 ] = TempBuf[ 2 ];
            CalData[ 1 ] = TempBuf[ 3 ];
            CalData[ 2 ] = TempBuf[ 4 ];
            CalData[ 3 ] = TempBuf[ 5 ];
            CalData[ 4 ] = TempBuf[ 6 ];
            CalData[ 5 ] = TempBuf[ 7 ];
            //t01_calib_data3 = TempBuf[ 4 ];


            GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
            //Get_Calibration_Data( CalibrationData );
            CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
            sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
            spif_resv_write( DataStartSector, CalData );
#endif
        }
        SendString("\r\nrecover ok!");
        /*
        for(temp_i=0;temp_i<10;temp_i++){
            //send_hex_u16(TempBuf[temp_i]);
            //SendString(":");
            TempBuf[temp_i] = 0;
            }

        DataStartSector = 1040384UL;
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
        {
        SendString("\r\nspi read error");
        return;
        }
        SendString("\r\nrecover erase ok!");
        */
    } else {
        SendString("\r\nno need recoverdata\r\n");
    }
#endif //
#endif
}



void read_cab(void)
{
    __X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 temp_i;
    t01_calib_data1 = 0;
    t01_calib_data2 = 0;
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
#ifdef TX_DEBUG
    SendString((U8 *)"\r\nread cab data\r\n");
#endif
    for(temp_i=0; temp_i<10; temp_i++) {
#ifdef TX_DEBUG
        send_hex_u16(TempBuf[temp_i]);
        SendString((U8 *)":");
#endif
    }
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {
        //if(USB_DET()){
        //  CommonLoadCode(C_CAB);
        //  }
#ifdef TX_DEBUG
        SendString((U8 *)"\r\nno cal data! ");
#endif
        //cal_oid_data = FALSE;
        if(read_spi_data()) {
            t01_calib_data1  = cal_data_1;
            t01_calib_data2  = cal_data_1;
            recover_oid_cal();
        }
    } else {

#if 1
        t01_calib_data1  = TempBuf[ 0 ];
        t01_calib_data2  = TempBuf[ 1 ];
        // MSW( t01_calib_data2 ) = TempBuf[ 2 ];
        //LSW( t01_calib_data2 ) = TempBuf[ 3 ];
#else
        MSW( t01_calib_data1 ) = TempBuf[ 1 ];
        LSW( t01_calib_data1 ) = TempBuf[ 0 ];
        MSW( t01_calib_data2 ) = TempBuf[ 3 ];
        LSW( t01_calib_data2 ) = TempBuf[ 2 ];
#endif
#ifdef TX_DEBUG
        SendString((U8 *)"\r\nfind cal data! ");
#endif
        //cal_oid_data = TRUE;
    }
}

#if 0
void read_oid_cal(void)
{
    __X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 temp_i;
    SPIF_ERR cur_err;

#if 1//( T01OID_CAB == ENABLE )     

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
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString((U8 *)"\r\nread cab data\r\n");
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {
    } else {
        MSW( t01_calib_data1 ) = TempBuf[ 0 ];
        LSW( t01_calib_data1 ) = TempBuf[ 1 ];
        MSW( t01_calib_data2 ) = TempBuf[ 2 ];
        LSW( t01_calib_data2 ) = TempBuf[ 3 ];
        SendString((U8 *)"\r\ncal=");
        send_hex_u32(t01_calib_data1);
        SendString((U8 *)":");
        send_hex_u32(t01_calib_data2);
        /*
           if( spif_mount() != SPIF_No_Err )
               {
                   return;
               }

               // Configure the flash to read/write mode.
               SPIF_Read_Write();


           memset( CalData, 0, sizeof( CalData ));
           CalData[0] = 0x1111;
           CalData[1] = 0x2222;


           CalData[2] = MSW( t01_calib_data1 );
           CalData[3] = LSW( t01_calib_data1 );
           CalData[4] = MSW( t01_calib_data2 );
           CalData[5] = LSW( t01_calib_data2 );

           CalData[6] = 0x3333;
           CalData[7] = 0x4444;

          //DataStartSector = (U32)(4086UL<<10);
          DataStartSector = (U32)(0x003FD800);
          SendString((U8 *)"\r\ndaddr=");send_hex_u32(DataStartSector);
          if(( SPIF_Page_Program( DataStartSector, (__X unsigned char *)CalData, sizeof(CalData) )) != SPIF_No_Err )
          {
             SendString((U8 *)"\r\nspi program error");
             return;
          }

          if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)CalData, sizeof(CalData) ) != SPIF_No_Err )
          {
             SendString((U8 *)"\r\nspi read error");
             return;
          }

           SendString((U8 *)"\r\nsave data = ");
            for(temp_i=0;temp_i<10;temp_i++){
                send_hex_u16(CalData[temp_i]);
                SendString((U8 *)":");
                }
            //*/
    }
#endif //

}

#endif


#if 0

void recover_oid_cal(void)
{
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 128 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;
#if( T01OID_CAB == ENABLE )

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
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString((U8 *)"\r\nrecover read cab data\r\n");
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {

        if( spif_mount() != SPIF_No_Err ) {
            SendString((U8 *)"\r\nspi mount fail!");
            return;
        }

        // Configure the flash to read/write mode.
        SPIF_Read_Write();

        //DataStartSector = (U32)(4086UL<<10);
        DataStartSector = 4184064UL;
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err ) {
            SendString((U8 *)"\r\nspi read error");
            return;
        }

        SendString((U8 *)"\r\nrecover data = ");
        for(temp_i=0; temp_i<10; temp_i++) {
            send_hex_u16(TempBuf[temp_i]);
            SendString((U8 *)":");
        }

        if((TempBuf[0] == 0x1111)&&(TempBuf[1] == 0x2222)
           &&(TempBuf[8] == 0x3333)&&(TempBuf[9] == 0x4444)) {
            memset( CalData, 0, sizeof( CalData ));
            CalData[ 0 ] = TempBuf[ 2 ];
            CalData[ 1 ] = TempBuf[ 3 ];
            CalData[ 2 ] = TempBuf[ 4 ];
            CalData[ 3 ] = TempBuf[ 5 ];
            CalData[ 4 ] = TempBuf[ 6 ];
            CalData[ 5 ] = TempBuf[ 7 ];
            //t01_calib_data3 = TempBuf[ 4 ];


            GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
            //Get_Calibration_Data( CalibrationData );
            CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
            sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
            spif_resv_write( DataStartSector, CalData );
#endif
        }
        SendString((U8 *)"\r\nrecover ok!");
        for(temp_i=0; temp_i<10; temp_i++) {
            //send_hex_u16(TempBuf[temp_i]);
            //SendString(":");
            TempBuf[temp_i] = 0;
        }

        DataStartSector = 1040384UL;
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err ) {
            SendString((U8 *)"\r\nspi read error");
            return;
        }
        SendString((U8 *)"\r\nrecover erase ok!");
    } else {
        SendString((U8 *)"\r\nno need recoverdata\r\n");
    }
#endif //

}

//#else
void recover_oid_cal(void)
{
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;

    __X U16 temp_i;
    SPIF_ERR cur_err;
#if( T01OID_CAB == ENABLE )

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
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString((U8 *)"\r\nrecover read cab data\r\n");
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {

        if( spif_mount() != SPIF_No_Err ) {
            return;
        }

        // Configure the flash to read/write mode.
        SPIF_Read_Write();

        //DataStartSector = (U32)(4086UL<<10);
        DataStartSector = (U32)(0x003FD800);
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err ) {
            SendString((U8 *)"\r\nspi read error");
            return;
        }

        SendString((U8 *)"\r\nrecover data = ");
        for(temp_i=0; temp_i<10; temp_i++) {
            send_hex_u16(TempBuf[temp_i]);
            SendString((U8 *)":");
        }

        if((TempBuf[0] == 0x1111)&&(TempBuf[1] == 0x2222)
           &&(TempBuf[6] == 0x3333)&&(TempBuf[7] == 0x4444)) {
            memset( CalData, 0, sizeof( CalData ));
            CalData[ 0 ] = TempBuf[ 2 ];
            CalData[ 1 ] = TempBuf[ 3 ];
            CalData[ 2 ] = TempBuf[ 4 ];
            CalData[ 3 ] = TempBuf[ 5 ];


            GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
            //Get_Calibration_Data( CalibrationData );
            CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
            sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
            spif_resv_write( DataStartSector, CalData );
#endif
        }

    } else {
        SendString((U8 *)"\r\nno need recoverdata\r\n");
    }
#endif //

}
#endif

#if 1
#pragma CODE = MMI_STARTUP_TEMP_CODE01
/*
const    U8    playerdir[] = "MP3";

#if( AP3 == ENABLE )
   const    U8    ap4dir[] = "AP4";
#endif   //#if( AP3 == ENABLE )

#if( RECORDER == ENABLE )
   const    U8    recorderdir[] = "RECORDER";
#endif   //#if( RECORDER == ENABLE )

#if( PASTER == ENABLE )
   const    U8    pasterdir[] = "REC";
#endif   //#if( PASTER == ENABLE )

#if( DIC == ENABLE )
   const    U8    dicdir[] = "DIC";
#endif   //#if( DIC == ENABLE )
*/
void mmc_sd_all_mode_dir_make( void )
{
    U16 *p;
#if( UNICODE_TYPE == ENABLE )
    U16 AP4_DIR_NAME[] = {0x70B9,0x8BFB,0x6587,0x4EF6,0x0000};//点读文件
    //U8 MP3_DIR_NAME[] = {0x70,0xB9,0x8B,0xFB,0x65,0x87,0x4E,0xF6,0x00};//mp3文件
    U16 MP3_DIR_NAME[] = {0x6587,0x4EF6,0x0000};//mp3文件
    U16 RECORD_DIR_NAME[] = {0x5F55,0x97F3,0x6587,0x4EF6,0x0000};//录音文件
    U16 DIC_DIR_NAME[] = {0x8BCD,0x5E93,0x6587,0x4EF6,0x0000};//词库文件
    U16 SYS_DIR_NAME[] = {0x7CFB,0x7EDF,0x6587,0x4EF6,0x0000};//系统文件
#endif
    SDKLoadCode( NAVIGATION_CODE );
    SDKLoadCode( NAVIGATION_TEMP_CODE05 );
    file_init();
    SDKLoadCode( NAVIGATION_TEMP_CODE00 );
    SendString((U8 *)"\r\nroot\r\n");
    if( !nav_dir_root())
        return;

#if( UNICODE_TYPE == DISABLE )

#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_ascii();
#endif
    SendString((U8 *)"\r\ncreat\r\n");
#if( AP3 == ENABLE )
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );

    if( !nav_filelist_findname(( U8 *)AP4_DIR, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );

        if( !nav_dir_make(( U8 *)AP4_DIR )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( AP3 == ENABLE )
    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)MP3_DIR, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_dir_make(( U8 *)MP3_DIR )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#if( RECORDER == ENABLE )
    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)RECORD_DIR, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_dir_make(( U8 *)RECORD_DIR )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( RECORDER == ENABLE )
//#if( REMOTE_PASTER == ENABLE )
    /*
       ( void )nav_filelist_reset();
       SDKLoadCode( NAVIGATION_TEMP_CODE03 );
       if( !nav_filelist_findname(( U8 *)PASTER_DIR, FALSE ))
       {
          SDKLoadCode( NAVIGATION_TEMP_CODE02 );
          if( !nav_dir_make(( U8 *)PASTER_DIR ))
          {
             if( fs_g_status != FS_ERR_FILE_EXIST )
             {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
             }
          }
       }
       */
//#endif   //#if( REMOTE_PASTER == ENABLE )
#if( DIC == ENABLE )
    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)DIC_DIR, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_dir_make(( U8 *)DIC_DIR )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( DIC == ENABLE )

#endif //#if( UNICODE_TYPE == DISABLE )

#if( UNICODE_TYPE == ENABLE )

#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_unicode();
#endif
    SendString((U8 *)"\r\ncreat\r\n");
#if( AP3 == ENABLE )
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );

    if( !nav_filelist_findname(( U8 *)AP4_DIR_NAME, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );

        if( !nav_unicode_dir_make(( U8 *)"1~1",( U8 *)AP4_DIR_NAME )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( AP3 == ENABLE )


    p = str_copy_u_n(( U16 *)tisiFileName, atou(( U8 *)"mp3"), 4 );
    p = str_copy_u_n( p, ( U16 *)MP3_DIR_NAME, 2+1 );

    ( void )nav_filelist_reset();

    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)tisiFileName, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_unicode_dir_make(( U8 *)"1~2",( U8 *)tisiFileName )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#if( RECORDER == ENABLE )
    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)RECORD_DIR_NAME, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_unicode_dir_make(( U8 *)"1~3",( U8 *)RECORD_DIR_NAME )) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( RECORDER == ENABLE )
//#if( REMOTE_PASTER == ENABLE )
    /*
       ( void )nav_filelist_reset();
       SDKLoadCode( NAVIGATION_TEMP_CODE03 );
       if( !nav_filelist_findname(( U8 *)"DIYREC", FALSE ))
       {
          SDKLoadCode( NAVIGATION_TEMP_CODE02 );
          if( !nav_dir_make(( U8 *)"DIYREC" ))
          {
             if( fs_g_status != FS_ERR_FILE_EXIST )
             {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
             }
          }
       }
       //*/
//#endif   //#if( REMOTE_PASTER == ENABLE )
#if( DIC == ENABLE )
    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)DIC_DIR_NAME, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_unicode_dir_make(( U8 *)"1~4",( U8 *)DIC_DIR_NAME)) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }
#endif   //#if( DIC == ENABLE )

    ( void )nav_filelist_reset();
    SDKLoadCode( NAVIGATION_TEMP_CODE03 );
    if( !nav_filelist_findname(( U8 *)SYS_DIR_NAME, FALSE )) {
        SDKLoadCode( NAVIGATION_TEMP_CODE02 );
        if( !nav_unicode_dir_make(( U8 *)"1~5",( U8 *)SYS_DIR_NAME)) {
            if( fs_g_status != FS_ERR_FILE_EXIST ) {
                SDKLoadCode( NAVIGATION_TEMP_CODE04 );
                ( void )nav_file_del( TRUE );
            }
        }
    }

#endif //#if( UNICODE_TYPE == DISABLE )

#if(( FS_ASCII == ENABLED )&&( FS_UNICODE == ENABLED ))
    nav_string_unicode();
#endif
}
#endif


#pragma CODE = MMI_STARTUP_TEMP_CODE00

//#define  SETTING_VERSION         "SETTING V2"

// Force LUT build on NF, rather than do it
// during USB enumration. If the init is long (lot of
// NF devices, lot of blocks, ...) USB enumeration may
// lead to time-out.
void mmistartup_force_build_NF( void )
{
    SPIF_ERR SPI_STATE;
    U8 temp;

    GetDataIdInfo( D_SETTINGS, &Start_Sector, &Length );
    SPI_STATE = spif_resv_read( Start_Sector, ( __X void *)Buf );
    if( SPI_STATE != SDTL_No_Err )
        goto reset_setting_error;

    if( Buf[ 0 ] !=( 1 + SETTING_VERSION_LEN + sizeof( SETTING_NEW )))
        goto reset_setting_error;

    memcpy(( U8 *)g_setting_list.buffer, &Buf[ 1 + SETTING_VERSION_LEN ], sizeof( SETTING_NEW ));
    /*g_setting_list.language_ihm = Buf[ 0+ 1 + SETTING_VERSION_LEN ];
    g_setting_list.poweroff_value = Buf[ 1+ 1 + SETTING_VERSION_LEN ];
    g_setting_list.backlight_mode = Buf[ 2+ 1 + SETTING_VERSION_LEN ];
    g_setting_list.backlight_timeout = Buf[ 3+ 1 + SETTING_VERSION_LEN ];
    g_setting_list.player_repeat = Buf[ 4+ 1 + SETTING_VERSION_LEN ];
    g_setting_list.player_volume = Buf[ 5+ 1 + SETTING_VERSION_LEN ];*/

    // Init structure size before load
    if( com_check_setting()) {
        SendString((U8 *)"\r\ncom_check_setting!");
#if 0//( aigo_pen == ENABLE )
        g_setting_list.backlight_mode    = BACKLIGHT_MODE_FULL_OFF;
        g_setting_list.backlight_timeout = 60; // N seconds
        g_setting_list.player_volume     = 8;
#endif   //#if( aigo_pen == ENABLE )
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
        g_setting_list.struct_setting.language_ihm      = LANG_ENGLISH;
        //g_setting_list.language_file     = LANG_ENGLISH;
#elif( LANGUAGE == Simplified )
        g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_SIMP;
        //g_setting_list.language_file     = LANG_CHINESE_SIMP;
#elif( LANGUAGE == Traditional )
        g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_TRAD;
        //g_setting_list.language_file     = LANG_CHINESE_TRAD;
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )

#if( SELECT_LANGUAGE == ENABLE )
        switch( g_setting_list.struct_setting.language_ihm ) {
            case LANG_ENGLISH:
            case LANG_CHINESE_SIMP:
//           case LANG_CHINESE_TRAD:
                break;

            default:
#if( LANGUAGE_DEFAULT == ENGLISH )
                g_setting_list.struct_setting.language_ihm      = LANG_ENGLISH;
                //g_setting_list.language_file     = LANG_ENGLISH;
#elif( LANGUAGE_DEFAULT == Simplified )
                g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_SIMP;
                //g_setting_list.language_file     = LANG_CHINESE_SIMP;
#elif( LANGUAGE_DEFAULT == Traditional )
                g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_TRAD;
                //g_setting_list.language_file     = LANG_CHINESE_TRAD;
#endif
                break;
        }
#endif   //#if( SELECT_LANGUAGE == ENABLE )


        if( g_setting_list.struct_setting.bluetooth != 0xFF ) {
            g_setting_list.struct_setting.bluetooth = 0x00;
        }

        if( g_setting_list.struct_setting.alarm > 1 ) {
            g_setting_list.struct_setting.alarm = 0;
        }

        temp = g_setting_list.struct_setting.poweroff_value;
        if((temp == 0)||(temp == 5)||(temp == 10)||(temp == 30)) {
        } else {
            g_setting_list.struct_setting.poweroff_value = 10;
        }
        /*
                 if(( g_setting_list.struct_setting.poweroff_value > 10 )
                    &&( g_setting_list.struct_setting.poweroff_value != 60 ))
                    g_setting_list.struct_setting.poweroff_value = 3;
        */
        // DISPLAY
        if( g_setting_list.struct_setting.backlight_timeout > 3 ) {
            g_setting_list.struct_setting.backlight_mode    = BACKLIGHT_MODE_FULL_OFF;
            g_setting_list.struct_setting.backlight_timeout = 3;//10;  // N seconds
        }

        if( g_setting_list.struct_setting.player_volume >= VOL_LEVEL ) {
            g_setting_list.struct_setting.player_volume = DEFAULT_VOLUME;
        }

        if( !g_setting_list.struct_setting.player_repeat
            ||( g_setting_list.struct_setting.player_repeat >= NB_REPEAT ))
            g_setting_list.struct_setting.player_repeat = NORMAL;
///*
        if(g_setting_list.struct_setting.year < 14)goto reset_date;
        if(g_setting_list.struct_setting.month > 12)goto reset_date;
        if(g_setting_list.struct_setting.day > 31)goto reset_date;
        if(g_setting_list.struct_setting.hour > 12)goto reset_date;
        if(g_setting_list.struct_setting.min > 60)goto reset_date;

        return;
    reset_date:
//*/
        g_setting_list.struct_setting.year = 14;
        g_setting_list.struct_setting.month = 3;
        g_setting_list.struct_setting.day  = 17;
        g_setting_list.struct_setting.hour = 12;
        g_setting_list.struct_setting.min  = 00;
        return;  // Setting load OK


    }
reset_setting_error:
    com_reset_setting();
}


void com_reset_setting( void )
{
    // fill structure with 0
    //memset( &g_setting_list , 0 , sizeof(g_setting_list) );
    //g_setting_list.size_of_struct    = sizeof(g_setting_list);
    // GLOBAL
    //memcpy( g_setting_list.version, SETTING_VERSION , sizeof(SETTING_VERSION) );
    SendString((U8 *)"\r\ncom_reset_setting!");
/*    
#if( SELECT_LANGUAGE == DISABLE )
#if( LANGUAGE == ENGLISH )
    g_setting_list.struct_setting.language_ihm      = LANG_ENGLISH;
    //g_setting_list.language_file     = LANG_ENGLISH;
#elif( LANGUAGE == Simplified )
    g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_SIMP;
    //g_setting_list.language_file     = LANG_CHINESE_SIMP;
#elif( LANGUAGE == Traditional )
    g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_TRAD;
    //g_setting_list.language_file     = LANG_CHINESE_TRAD;
#endif
#endif   //#if( SELECT_LANGUAGE == DISABLE )

#if( SELECT_LANGUAGE == ENABLE )
#if( LANGUAGE_DEFAULT == ENGLISH )
    g_setting_list.struct_setting.language_ihm      = LANG_ENGLISH;
    //g_setting_list.language_file     = LANG_ENGLISH;
#elif( LANGUAGE_DEFAULT == Simplified )
    g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_SIMP;
    //g_setting_list.language_file     = LANG_CHINESE_SIMP;
#elif( LANGUAGE_DEFAULT == Traditional )
    g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_TRAD;
    //g_setting_list.language_file     = LANG_CHINESE_TRAD;
#endif
#endif   //#if( SELECT_LANGUAGE == ENABLE )

*/
    g_setting_list.struct_setting.language_ihm      = LANG_CHINESE_SIMP;
    g_setting_list.struct_setting.bluetooth            = 0x00;
    g_setting_list.struct_setting.alarm = 0;
    g_setting_list.struct_setting.poweroff_value    = 10;              //0-"Never", 1-"2 min", 2-"10 min" (define in power.c and see mmi_setting.c)

// DISPLAY
    g_setting_list.struct_setting.backlight_mode    = BACKLIGHT_MODE_FULL_OFF;
    g_setting_list.struct_setting.backlight_timeout = 3;//10;  // N seconds
#if 1
    g_setting_list.struct_setting.player_repeat        = NORMAL;
#else
    g_setting_list.struct_setting.player_repeat        = REPEAT_ALL;
#endif
    g_setting_list.struct_setting.player_volume        = DEFAULT_VOLUME;

    

    g_setting_list.struct_setting.year   = 14;
    g_setting_list.struct_setting.month  = 5;
    g_setting_list.struct_setting.day    = 01;

    g_setting_list.struct_setting.hour   = 00;
    g_setting_list.struct_setting.min    = 00;

    g_setting_list.struct_setting.studyAp4   = 00;
    g_setting_list.struct_setting.studyUnite    = 00;
}


Bool com_check_setting( void )
{
    return (0 == memcmp( &Buf[ 1 ], SETTING_VERSION, sizeof( SETTING_VERSION )));
}




#pragma CODE = HARDCHECK_CODE

Bool check_hard_news( void )
{
    __X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 temp_i;
    GetDataIdInfo( D_CHECK, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 1//def SPIBOOT_KBDM9
    spif_resv_read( DataStartSector, TempBuf );
#endif
#ifdef TX_DEBUG
    SendString((U8 *)"\r\nread check data\r\n");
#endif
    for(temp_i=0; temp_i<22; temp_i++) {
#ifdef TX_DEBUG
        send_hex_u16(TempBuf[temp_i]);
        SendString((U8 *)":");
#endif
    }

    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A )return FALSE;
    if( MSB(TempBuf[0]) == HARD_V1_VERSION) {
        g_b_V1Hard_Flag = TRUE;
        return TRUE;
    }

    if( MSB(TempBuf[0]) == HARD_V2_VERSION) {
        g_b_V1Hard_Flag = FALSE;
        return TRUE;
    }
    return FALSE;
}

void check_hard_version( void )
{
#if 0

    while(1) {
        _bitCLR( CHECK_DET_PH );//clr ph
        _bitSET( CHECK_DET_DIR );//input
        delay1ms(255);


        //_bitSET( RED_LED );
        SendString((U8 *)"\r\nversion check!");

        if(CHECK_READ_FLAG()) {
            SendString((U8 *)"\r\nPB13 heigh!");
        } else {
            SendString((U8 *)"\r\nPB13 low!");
        }
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);

        _bitSET( CHECK_DET_DIR );//input
        _bitSET( CHECK_DET_PH );//set ph
        delay1ms(255);
        if(CHECK_READ_FLAG()) {
            SendString((U8 *)"\r\nPB13 heigh!");
        } else {
            SendString((U8 *)"\r\nPB13 low!");
        }

        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);

        _bitCLR( HP_V2_DET_PH);//clr ph
        _bitSET( HP_V2_DET_DIR );//input
        delay1ms(255);


        //_bitSET( RED_LED );
        SendString((U8 *)"\r\nversion check!");

        if(HP_V2_FLAG()) {
            SendString((U8 *)"\r\nPB5 low!");
        } else {
            SendString((U8 *)"\r\nPB5 heigh!");
        }
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);


        _bitSET( HP_V2_DET_PH );//set ph
        _bitSET( HP_V2_DET_DIR );//input
        delay1ms(255);
        if(HP_V2_FLAG()) {
            SendString((U8 *)"\r\nPB5 low!");
        } else {
            SendString((U8 *)"\r\nPB5 heigh!");
        }
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);
        delay1ms(255);


    }
    /*
     g_b_V1Hard_Flag = TRUE;
     if(CHECK_READ_FLAG()){
       SendString((U8 *)"\r\nPB13 heigh!");
       _bitCLR( CHECK_DET_PH );//clr ph
        if(!CHECK_READ_FLAG()){
            SendString((U8 *)"\r\nPB13 low!");
            g_b_V1Hard_Flag = FALSE;
            }

       }
        */
#else
    __X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 temp_i;
    Bool temp_1_flag = FALSE;
    Bool temp_2_flag = FALSE;

    //temp_flag = g_b_V1Hard_Flag;
    g_b_V1Hard_Flag = TRUE;
    memset( TempBuf, 0, sizeof( TempBuf ));

    //_bitCLR( CHECK_DET_DIR );//input
    // _bitSET( CHECK_DET_PH );//set ph
    //_bitSET( RED_LED );
    _bitSET( CHECK_DET_PH );//set ph
    _bitSET( CHECK_DET_DIR );//input
    delay1ms(30);
    if(CHECK_READ_FLAG()) {
        SendString((U8 *)"\r\nPB13 heigh!");
        //temp_1_flag = FALSE;
        //MSB(TempBuf[0]) = HARD_V2_VERSION

        _bitCLR( CHECK_DET_PH );//clr ph
        _bitSET( CHECK_DET_DIR );//input
        delay1ms(30);
        if(!CHECK_READ_FLAG()) {
            SendString((U8 *)"\r\nPB13 low!");
            g_b_V1Hard_Flag = FALSE;
        }
    }


    if(g_b_V1Hard_Flag) {
        SendString((U8 *)"\r\nv1 headphone!");
        MSB(TempBuf[0]) = HARD_V1_VERSION;
    } else {
        SendString((U8 *)"\r\nv2 headphone!");
        MSB(TempBuf[0]) = HARD_V2_VERSION;
    }
    GetDataIdInfo( D_CHECK, &DataStartSector, &DataLength );
    TempBuf[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
    sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9



    //

    spif_resv_write( DataStartSector, TempBuf );

#endif

#endif
}

void set_headphone_pin(void)
{
    if(g_b_V1Hard_Flag) {
        SendString((U8 *)"\r\nv1 headphone!");
        _bitSET( HP_V1_DET_DIR );
        _bitSET( HP_V1_DET_PH );
    } else {
        SendString((U8 *)"\r\nv2 headphone!");
        _bitSET( HP_V2_DET_DIR );
        _bitSET( HP_V2_DET_PH );
    }
}

//#pragma CODE = UPDATEFILE_CODE
#pragma CODE = UPDATE_FIRMWARE_CODE
char * get_ext(char* str)
{
    int index;
    int last;
    last = index = 0;
    while(str[last]) {
        if(str[last] == '.')
            index = last;
        ++last;
    }
    if(index == 0)
        index = last;
    else
        ++index;
    return  &str[index];
}
//============================================================
int file_get_ext_count(char * dir_path,char * ext)
{
    FILE_INFO   __X finfo;
    FS_ENTRY_INFO   __X fentry;
    FS_ERR    err;
    int count = 0;
    unsigned int i;
#if 0//def TX_DEBUG
    SendString("\r\nfile_get_ext_count");
#endif
    memset((char *)&finfo,0,sizeof(finfo));
#if 0
    err = FSGetEntry(dir_path,0,&finfo,&fentry);
#else
    for( i = 0; ; ++i ) {
        err = FSGetEntry(dir_path,i,&finfo,&fentry);
        if( err == FS_NO_ERROR )
            break;
        if( err == FS_FILE_NOT_FOUND )
            continue;
        return 0;
    }
#endif
#if 0//def TX_DEBUG
    SendString((U8 *)"\r\n42_err=");
    send_hex_u8(err);
    SendString((U8 *)"\r\n");
    SendString((char *)dir_path);
    SendString((U8 *)"\r\n");
    SendString((char *)ext);
#endif
    //if(err!=FS_NO_ERROR)
    //  return  0;
    do {
        if(( finfo.attribute & FS_ATTR_DIRECTORY )!= FS_ATTR_DIRECTORY ) {
#ifdef TX_DEBUG
            SendString((U8 *)"\r\nfinfo.namer=");
            SendString((U8 *)finfo.name);
#endif
            if(strcmp(ext,get_ext((char *)finfo.name))==0) {
#if 0//def TX_DEBUG
                SendString("\r\nfinfo.namer=");
                SendString((char *)finfo.name);
#endif
                if((finfo.size != 0)||(finfo.size != 0xFFFFFFFF)) {
                    ++count;
                }

            }
        }
    } while(FSGetNextEntry(&finfo,&fentry)==FS_NO_ERROR);
    return  count;
}

//--------------------------------------------------------------------------------

Bool file_get_path(char * dir_path,char * ext,int index)
{
    FILE_INFO   __X finfo;
    FS_ENTRY_INFO   __X fentry;
    FS_ERR    err;
    int temp_index = index + 1;
    unsigned int i;
    //++index;
    memset((char *)&finfo,0,sizeof(finfo));
    g_full_path[0] = 0;
#if 0
    err = FSGetEntry(dir_path,0,&finfo,&fentry);
    if(err!=FS_NO_ERROR)
        return  FALSE;
#else
    for( i = 0; ; ++i ) {
        err = FSGetEntry(dir_path,i,&finfo,&fentry);
        if( err == FS_NO_ERROR )
            break;
        if( err == FS_FILE_NOT_FOUND )
            continue;
        return false;
    }
#endif
#if 0//def TX_DEBUG
    SendString("\r\n70_err:index=");
    send_hex_u8(err);
    SendString(":");
    send_hex_u16(temp_index);
#endif

    while(temp_index) {
#if 0//def TX_DEBUG
        SendString("\r\nfinfo=");
        SendString((char *)finfo.name);
        SendString(":");
        send_hex_u8(finfo.attribute);
        SendString(":");
        send_hex_u32(finfo.size);
        SendString(":");
        send_hex_u8(finfo.index);
        SendString(":");
        SendString((char *)finfo.name);
#endif
        if(( finfo.attribute & FS_ATTR_DIRECTORY )!= FS_ATTR_DIRECTORY ) {

            if(strcmp(ext,get_ext((char *)finfo.name))==0) {
                --temp_index;
#if 0//def TX_DEBUG
                SendString("\r\nfinfo.namer=");
                SendString((char *)finfo.name);
#endif
                //if((finfo.size != 0)||(finfo.size != 0xFFFFFFFF)){
                //  --index;
                //  }
            }
        }
        if(temp_index == 0) {
            strcpy(g_full_path,dir_path);
            strcat(g_full_path,"\\");
            strcat(g_full_path,(char *)finfo.name);

            //memcpy((char *)fat_g_info.name,(char *)finfo.name,sizeof(FILE_INFO));
            //strcpy((char *)fat_g_info.name,(char *)finfo.name);
            //strcpy((char *)fat_g_info.attribute,(char *)finfo.name);
            return  TRUE;
        }
        memset((char *)&finfo,0,sizeof(finfo));
        if(FSGetNextEntry(&finfo,&fentry)!=FS_NO_ERROR)
            break;
    }
#ifdef TX_DEBUG
    SendString((U8 *)"\r\nnofind!");
#endif
    return  FALSE;
}


//#pragma CODE = UPDATE_FIRMWARE_CODE
Bool update_firmware_deal( void )
{
    FS_ERR fs_err;
    __X U32 file_size;
    __X FCFILE *REC_File;
    __X U8 TempBuf[ 4 ];
    U16 tempData = 0;
    char temp_filename[128];
    point_A = 0;
    point_B = 0;
    SDKLoadCode( MMI_PLAYINTRO_CODE00 );
    strcpy( tisiFileName, "0:\\up.afa" );
    if( find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath )) {
        FSDelete(( char *)tisiFileShortpath );
    }

    strcpy( tisiFileName, "0:\\x99update.bin" );
    if( find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath )) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\r\nfind update file");
#endif
        goto findBIN;
    }
    // return FALSE;


    tempData = file_get_ext_count((char *)"0:",(char *)"BIN");
#ifdef TX_DEBUG
    SendString(( U8 *)"\r\nbin_num = ");
    send_hex_u16(tempData);
#endif
    if(tempData == 0)return false;

    if(!file_get_path((char *)"0:",(char *)"BIN", 0)) {
#ifdef TX_DEBUG
        SendString(( U8 *)"\r\nfind update file fail!");
#endif
        return false;
    }
#ifdef TX_DEBUG
    SendString(( U8 *)"\r\nfull_path = ");
    SendString(( U8 *)g_full_path);
#endif
    strcpy( temp_filename, g_full_path );
    if( !find_path_a(( U8 *)temp_filename, (U8 *)tisiFileShortpath ))
        return FALSE;

findBIN:
    file_size = FSGetFileSize(( char *)tisiFileShortpath, &fs_err );
    if( fs_err != FS_NO_ERROR )
        return FALSE;
    if( file_size <( 1024ul*1024ul ))
        return FALSE;
    if( file_size >( 4084ul*1024ul ))
        return FALSE;

    REC_File = FSOpen((char *)tisiFileShortpath,FS_MODE_READ_WRITE,&fs_err);

    (void)FSSeek(REC_File,0);
    (void)FSRead(REC_File, (void *)&TempBuf,sizeof(TempBuf), &fs_err);
    (void)FSSeek(REC_File,0x00000020);
    (void)FSRead(REC_File, (void *)&Buf,sizeof(Buf), &fs_err);
    FSClose(REC_File);
    if(TempBuf[0] != 0x42)return false;
    if(TempBuf[1] != 0x4F)return false;
    if(TempBuf[2] != 0x4F)return false;
    if(TempBuf[3] != 0x54)return false;

    if(Buf[0] != 0x53)return false;
    if(Buf[1] != 0x50)return false;
    if(Buf[2] != 0x49)return false;
    if(Buf[3] != 0x46)return false;
    if(Buf[4] != 0x32)return false;
    if(Buf[5] != 0x30)return false;
    if(Buf[6] != 0x30)return false;
    if(Buf[7] != 0x39)return false;
    if(Buf[8] != 0x31)return false;
    if(Buf[9] != 0x32)return false;
    if(Buf[10] != 0x31)return false;
    if(Buf[11] != 0x35)return false;

    FSRename(( char *)tisiFileShortpath, "up.afa" );
    strcpy( tisiFileName, "0:\\up.afa" );
    if( !find_path_a(( U8 *)tisiFileName, (U8 *)tisiFileShortpath ))
        return FALSE;

    strcpy(( char *)&remote_index_tab[ 0 ], ( char *)tisiFileShortpath );

    return TRUE;
}

//#pragma CODE = DISPLAY_SYSTEMNAME_CODE



#endif // _mmi_startup_c_

