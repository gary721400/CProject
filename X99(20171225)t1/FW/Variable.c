#include "variable.h"

ClusterListItem   cluster_buf_bak[ FS_NB_CACHE_CLUSLIST ]    @ ( 0x6F0C - FS_NB_CACHE_CLUSLIST * sizeof( ClusterListItem ));
U16               adc3_value                             @ 0x6F0C;
U16               adc7_value                             @ 0x6F10;
BIT_NEW    		  gSysRevFlag							 @ 0x6F12;
U16               reread_num                             @ 0x6F14;
U16				  system_dir[LEN_SYSTEM_DIR]			 @ 0x6F16;	
BIT_NEW    		  g_sys_flag							 @ 0x6F36;
U16               all_mp3_num                            @ 0x6F38;
U16             t01_calib_data1                        @ 0x6F3A;
U16             t01_calib_data2                        @ 0x6F3C;

//U32               t01_calib_data1                        @ 0x6F3A;
//U32               t01_calib_data2                        @ 0x6F3E;

AP4_BIT_NEW		  ap4_bit_news							 @ 0x6F42;
U16               dirnum_diandu                          @ 0x6F44;
U16               ap4num_diandu                          @ 0x6F46;
U32               point_A		                         @ 0x6F48;
U32               point_B                                @ 0x6F4C;
U16               count_AB	                             @ 0x6F50;
U16               size_psec_wav                          @ 0x6F52;
U32               current_play_time                      @ 0x6F54;
U16               adc2_value                             @ 0x6F58;
SETTING_NEW       g_setting_list                         @ 0x6F5A;//20//16


U16               remote_index_tab[ REMOTE_INDEX_MAX ]   @ 0x6F6E; //33*2

U8                s_battery_level                        @ 0x6FB0;
U16               ap4_AllControl_MaskGlobal              @ 0x6FB2;

U8                last_record_number                     @ 0x6FB4;
U8                count_type_num                         @ 0x6FB6;
U8                Wrong_Answered[ 4 ]                    @ 0x6FB8;

U32               batt_low_warning_timer                 @ 0x6FBC;
U32               usb_charge_time                        @ 0x6FC0;

U8                LastKeyState                           @ 0x6FC4;
U16               adc0value                              @ 0x6FC6;

GAME_STATUS       bGameStatus                            @ 0x6FC8;
GAME_BIT_NEW      game_bits                              @ 0x6FCA;
U8                HintAnswerIndex                        @ 0x6FCC;
U8                g_game_rand_tab[ 4 ]                   @ 0x6FCE;
U8                AnswerIndex                            @ 0x6FD2;
U8                AnswerCur                              @ 0x6FD4;
U8                CurrentAnswerCount                     @ 0x6FD6;
U8                ErrCur                                 @ 0x6FD8;
U8                Answered[ 4 ]                          @ 0x6FDA;
U8                GameCount                              @ 0x6FDE;
U8                RightNumber                            @ 0x6FE0;
U16               CurrentGame                            @ 0x6FE2;

Record            Current_Record                         @ 0x6FE4;
SEARCH_STATUS     Search_Record                          @ 0x6FEC;
U8                ap3_mode                               @ 0x6FEE;
U8                Ap3_MenuPath                           @ 0x6FF0;
U8                ap3_poweron_state                      @ 0x6FF2;

U8                g_power_off_mode                       @ 0x6FF4;
U8                mmi_file_flag                          @ 0x6FF6;
SYSTEM_BIT_NEW    g_system_flag                          @ 0x6FF8;
U8                recorder_mode                          @ 0x6FFA;
//U8                g_volume                               @ 0x6FFA;

U8                g_old_select_appli                     @ 0x6FFC;
U8                g_select_appli                         @ 0x6FFE;

