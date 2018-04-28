#include "compiler.h"

#define CacheNum 6

#define NUM_AREA 		5

typedef struct
{
   U16   Sd_num;
   U16   Book_num;
   U16   Book_id;
}LASTBOOK;



typedef struct
{
   U16   Cur_num;
   U16   Max_num;
}BOOKNEWS;


typedef struct
{
   U16   Ap3_num;
   U16   Book_num;
   U16   Page_num;
   U16   Control_num;
}Record;
extern  Record        Current_Record;

#define NUM_CLUSTR 		16
typedef	struct	_ClusterListItem
{
	U16	disk_id;
	U32	start_cluster;
	U32	offset_by_clus;
	U32	cluster;
	U32	seg_len;
}ClusterListItem;

extern ClusterListItem cluster_buf[NUM_CLUSTR];
typedef struct
{
   U32   Index;
   U32   ActionIndex;
}FOURBYTESNEWS;
/*
typedef union
{
   struct
   {
      unsigned    bGflagRev0          :1;//
      unsigned    bGflagRev1          :1;//
      unsigned    bGflagRev2          :1;//
      unsigned    bGflagRev3          :1;//
      
      unsigned    bGflagRev4          :1;
      unsigned    bGflagRev5          :1;
      unsigned    bGflagRev6          :1;
      unsigned    bGflagRev7          :1;
      
      unsigned    bGflagRev8          :1;
      unsigned    bGflagRev9          :1;
      unsigned    bGflagRev10         :1;
      unsigned    bGflagRev11         :1;

      unsigned    bGflagRev12         :1;
      unsigned    bGflagRev13         :1;
      unsigned    bGflagRev14         :1;
      unsigned    bGflagRev15         :1;

   }bits;
   U16      value;
}GFAG_BIT_NEW;
*/
typedef union
{
   struct
   {
      unsigned    bRev0          :1;//
      unsigned    bRev1          :1;//
      unsigned    bRev2          :1;//
      unsigned    bRev3          :1;//
      
      unsigned    bRev4          :1;
      unsigned    bRev5          :1;
      unsigned    bRev6          :1;
      unsigned    bRev7          :1;
      
      unsigned    bRev8          :1;
      unsigned    bRev9          :1;
      unsigned    bRev10         :1;
      unsigned    bRev11         :1;

      unsigned    bRev12         :1;
      unsigned    bRev13         :1;
      unsigned    bRev14         :1;
      unsigned    bRev15         :1;

   }bits;
   U16      value;
}GREV_BIT_NEW;

extern GREV_BIT_NEW gflag;


#define b_usb			      			gflag.bits.bRev0
#define b_startup_play            		gflag.bits.bRev1
#define bDiskFullFlag	              	gflag.bits.bRev2
#define b_play_pause          			gflag.bits.bRev3



#define b_back_from_mp3      			gflag.bits.bRev4
#define g_b_system_clk_low          	gflag.bits.bRev5
#define b_back_have_index              	gflag.bits.bRev6
#define b_current_control_game			gflag.bits.bRev7

#define b_oid_ok						gflag.bits.bRev8
#define b_headphone_flag				gflag.bits.bRev9
#define b_find_usb_flag					gflag.bits.bRev10
#define b_first_ap4_mp3_flag			gflag.bits.bRev11

#define b_card_state				    gflag.bits.bRev12
#define b_low_battary					gflag.bits.bRev13
#define b_vol_play						gflag.bits.bRev14
#define b_back_have_key					gflag.bits.bRev15


extern GREV_BIT_NEW systerm_flag;
#define b_clusterlist		      			systerm_flag.bits.bRev0
#define g_b_play_afx            			systerm_flag.bits.bRev1
#define g_b_reread							systerm_flag.bits.bRev2
#define g_b_record		              		systerm_flag.bits.bRev3

#define b_sd1_ok	            			systerm_flag.bits.bRev4
#define b_sd2_ok	            			systerm_flag.bits.bRev5
#define g_b_wait_time			            systerm_flag.bits.bRev6
#define g_b_repeat		              		systerm_flag.bits.bRev7

#define g_b_noap4		              		systerm_flag.bits.bRev8
#define g_b_nomp3		              		systerm_flag.bits.bRev9
#define g_b_secondgo_mp3       				systerm_flag.bits.bRev10
#define g_b_tishi_2          				systerm_flag.bits.bRev11

#define g_b_sx_play		              		systerm_flag.bits.bRev12
#define bInvalidFlag	              		systerm_flag.bits.bRev13
#define bBnl_FastSeek	              		systerm_flag.bits.bRev14
#define g_b_cal								systerm_flag.bits.bRev15




extern GREV_BIT_NEW sys_flag;
#define b_lock_key		      				sys_flag.bits.bRev0
#define b_sleep_mode		      			sys_flag.bits.bRev1
#define b_cal_over			      			sys_flag.bits.bRev2
#define b_cal_init_fail		      			sys_flag.bits.bRev3

#define b_index_four_byte	      			sys_flag.bits.bRev4
#define b_flash_led	   	      			    sys_flag.bits.bRev5
//#define bInvalidFlag						sys_flag.bits.bRev13

/*
#define INDEX_RECORD_MIN		29001
#define INDEX_RECORD_MAX		30000

#define INDEX_INC_VOL			65321
#define INDEX_DEC_VOL			65524

#define INDEX_GO_MP3			65384
#define INDEX_NEXT_MP3			65385
#define INDEX_PREV_MP3			65386

#define INDEX_START_RECORD		65344
#define INDEX_STOP_RECORD		65345
#define INDEX_PLAY_RECORD		65346
#define INDEX_DEL_RECORD		65347

#define INDEX_PAUSE_PLAY		65387
#define INDEX_GO_AP4			65388

#define INDEX_FIRST_VOL			65389
#define INDEX_LAST_VOL			65395

#define INDEX_GO_GAME_MIN		60001
#define INDEX_GO_GAME_MAX		61438

#define INDEX_EXIT_GAME			61439

#define INDEX_LANG_MIN			65001
#define INDEX_LANG_MAX			65020

#define INDEX_CHANGE_AP4_MIN	61440
#define RANG_CHANGE_AP4			2047

#define INDEX_GO_DIC			65396

//*/

#define  REMOTE_INDEX_MAX                 32
#define  REMOTE_VOL_UP                    0
#define  REMOTE_VOL_DOWN                  ( REMOTE_VOL_UP + 1 )
#define  REMOTE_POWER_OFF                 ( REMOTE_VOL_UP + 2 )
#define  REMOTE_EXIT_MP3                  ( REMOTE_VOL_UP + 3 )
#define  REMOTE_MP3_PLAY_PAUSE            ( REMOTE_VOL_UP + 4 )
#define  REMOTE_MP3_PREV                  ( REMOTE_VOL_UP + 5 )
#define  REMOTE_MP3_NEXT                  ( REMOTE_VOL_UP + 6 )
#define  REMOTE_AP4_MODE                  ( REMOTE_VOL_UP + 7 )
#define  REMOTE_MP3_MODE                  ( REMOTE_VOL_UP + 8 )
#define  REMOTE_REC_MODE                  ( REMOTE_VOL_UP + 9 )
#define  REMOTE_PASTER_MODE               ( REMOTE_VOL_UP + 10 )
#define  REMOTE_DIC_MODE                  ( REMOTE_VOL_UP + 11 )
#define  REMOTE_CAL_MODE                  ( REMOTE_VOL_UP + 12 )
#define  REMOTE_REC_START                 ( REMOTE_VOL_UP + 13 )
#define  REMOTE_REC_PLAY                  ( REMOTE_VOL_UP + 14 )
#define  REMOTE_REC_STOP                  ( REMOTE_VOL_UP + 15 )
#define  REMOTE_OK                  	  ( REMOTE_VOL_UP + 16 )
#define  REMOTE_V_0                  	  ( REMOTE_VOL_UP + 17 )
#define  REMOTE_V_1                  	  ( REMOTE_VOL_UP + 18 )
#define  REMOTE_V_2                  	  ( REMOTE_VOL_UP + 19 )
#define  REMOTE_V_3                  	  ( REMOTE_VOL_UP + 20 )
#define  REMOTE_V_4                  	  ( REMOTE_VOL_UP + 21 )
#define  REMOTE_V_5                  	  ( REMOTE_VOL_UP + 22 )
#define  REMOTE_V_6                  	  ( REMOTE_VOL_UP + 23 )
#define  REMOTE_V_7                  	  ( REMOTE_VOL_UP + 24 )
#define  REMOTE_V_8                  	  ( REMOTE_VOL_UP + 25 )
#define  REMOTE_V_9                  	  ( REMOTE_VOL_UP + 26 )


extern GREV_BIT_NEW rev1Flag;
extern GREV_BIT_NEW rev2Flag;

extern unsigned int vol_idx;
extern unsigned int ap3_file_number;
extern unsigned int ap4_mp3_flag;
extern unsigned int current_sd;
extern unsigned int sd2_ap3_number;
extern unsigned int current_mp3_sd;
//extern unsigned int bak_ap4_num;
//extern unsigned int bak_book_num;
//extern unsigned int bak_page_num;
//extern unsigned int bak_control_mode_num;
extern unsigned int volume_inc_flag;
extern unsigned long bak_current_index;
//extern unsigned int systerm_flag;
//extern unsigned long free_space;
extern unsigned int wav_action;
extern unsigned int current_wav_num;
extern unsigned int current_playmp3_number;
extern unsigned int current_area;
//extern unsigned int bookarer_record;
extern unsigned int 	all_mp3_num;

extern unsigned int prev_task; //0x689C
extern unsigned int adc0_value; //0x689C
extern unsigned int adc2_value; //0x689C
extern unsigned int adc3_value; //0x689C
extern unsigned int adc7_value; //0x689C
extern unsigned long t01_calib_data1;
extern unsigned long t01_calib_data2;
extern unsigned int  t01_calib_data3;

extern unsigned int  rev_int_1;
extern unsigned int  rev_int_2;
extern unsigned int  oid_index_type;

extern unsigned int g_string2[30];  //0x681c--6858
extern unsigned int g_tishi_dir[30];
extern U32 remote_index_tab[REMOTE_INDEX_MAX]; //0x685A---0x689A
extern unsigned int mp3_file_number; //0x689C