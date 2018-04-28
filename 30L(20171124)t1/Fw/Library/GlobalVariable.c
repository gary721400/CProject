#include "GlobalVariable.h"

//unsigned int test_flag  @ 0x6750;  //for Test
/*
unsigned int gflag @ 0x5000;
unsigned int vol_idx @ 0x5002;
unsigned int ap3_file_number @ 0x5004;
unsigned int ap4_mp3_flag @ 0x5006;
unsigned int bak_ap4_num @ 0x5008;
unsigned int bak_book_num @ 0x500a;
unsigned int bak_page_num @ 0x500c;
unsigned int bak_control_mode_num @ 0x500e;
unsigned int volume_inc_flag @ 0x5010;
unsigned int bak_current_index @ 0x5012;
unsigned int systerm_flag @ 0x5014;
unsigned long free_space @ 0x5016;

unsigned int g_string2[30] @ 0x501c;  //0x681c--6858
unsigned int remote_index_tab[32] @ 0x505A; //0x685A---0x689A
unsigned int mp3_file_number @ 0x509C; //0x689C
*/

//ClusterListItem cluster_buf[NUM_CLUSTR] @ 0x6D10;
//ClusterListItem	afx_cluster_buf[ NUM_CLUSTR ] @ 0x6E30;
//unsigned int bookarer_record @ 0x6E1C;

unsigned int g_tishi_dir[30] @ 0x6DA6;  //
ClusterListItem cluster_buf[NUM_CLUSTR] @ 0x6DE2;
//BOOKNEWS    Current_booknews[NUM_AREA]@ 0x6DEE;

//LASTBOOK Current_lastbook[NUM_AREA] @ 0x6EDA;

unsigned int 	all_mp3_num @ 0x6F02;
GREV_BIT_NEW sys_flag @ 0x6F04;
unsigned int 	current_mp3_sd @ 0x6F06;
unsigned int current_area @ 0x6F08;
unsigned int current_playmp3_number @ 0x6F0A;
unsigned int sd2_ap3_number @ 0x6F0C;
unsigned int 	current_sd @ 0x6F0E;
unsigned long t01_calib_data1 @ 0x6F10;
unsigned long t01_calib_data2 @ 0x6F14;
GREV_BIT_NEW gflag @ 0x6F18;
unsigned int vol_idx @ 0x6F1A;
unsigned int ap3_file_number @ 0x6F1C;
unsigned int ap4_mp3_flag @ 0x6F1E;
Record       Current_Record @ 0x6F20;
unsigned int volume_inc_flag @ 0x6F28;
unsigned long bak_current_index @ 0x6F2A;
GREV_BIT_NEW systerm_flag @ 0x6F2E;
//unsigned int systerm_flag @ 0x687C;
//unsigned long free_space @ 0x6F6E;
//GREV_BIT_NEW  rev1Flag @ 0x6F2E;
unsigned int  t01_calib_data3 @ 0x6F30;

unsigned int prev_task @ 0x6F32;

unsigned int g_string2[30] @ 0x6F34;  //0x681c--6858
U32 remote_index_tab[REMOTE_INDEX_MAX] @ 0x6F70; //0x685A---0x689C



unsigned int wav_action @ 0x6FF0;
unsigned int current_wav_num @ 0x6FF2;
unsigned int adc0_value @ 0x6FF4;
unsigned int adc2_value @ 0x6FF6;
unsigned int adc3_value @ 0x6FF8;
unsigned int adc7_value @ 0x6FFA;

//unsigned int  rev_int_1 @ 0x6FFC;
unsigned int  bn1FileNumber @ 0x6FFC;
//unsigned int  rev_int_2 @ 0x6FFE;
unsigned int  oid_index_type @ 0x6FFE;


/*
unsigned int vol_idx @ 0x6800;
GFAG_BIT_NEW gflag @ 0x6802;

unsigned int ap3_file_number @ 0x6804;
unsigned int ap4_mp3_flag @ 0x6806;
unsigned int bak_ap4_num @ 0x6808;
unsigned int bak_book_num @ 0x680a;
unsigned int bak_page_num @ 0x680c;
unsigned int bak_control_mode_num @ 0x680e;
unsigned int volume_inc_flag @ 0x6810;
unsigned int bak_current_index @ 0x6812;
unsigned int systerm_flag @ 0x6814;
unsigned long free_space @ 0x6816;
unsigned int mp3_file_number @ 0x681A; //0x689C
unsigned int prev_task @ 0x681C; //0x689C
unsigned int adc1_value@ 0x681E; //0x689C
unsigned int adc2_value@ 0x6820; //0x689C
unsigned int adc7_value@ 0x6822; //0x689C

unsigned int g_string2[30] @ 0x6830;  //0x681c--6858
unsigned int remote_index_tab[REMOTE_INDEX_MAX] @ 0x686C; //0x685A---0x689A
*/

//*/