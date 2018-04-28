
#include "lib_mcu\compiler.h"       // Compiler definitions


#define  NORMAL_MODE_INDEX			62508//(DPR3_GO_AP4)   //62508
#define  REREAD_MODE_INDEX			(NORMAL_MODE_INDEX + 1)//62509
//#define  DPR3_REREAD_MODE_INDEX			(DPR3_GO_AP4 + 1)//62509
//#define  DPR3_NORMAL_MODE_INDEX			(DPR3_GO_AP4)   //62508




#define IDLE_STATE      0
#define GOTO_REREAD     (IDLE_STATE + 1)
#define GOTO_NOMAL      (IDLE_STATE + 2)

#define  REMOTE_INDEX_MAX                 33//17//16//

#define  LEN_SYSTEM_DIR					  16

//! Number of cache used to store a cluster list of files (interresting in case of many "open file")
//#define  FS_NB_CACHE_CLUSLIST 1  // In player mode, 1 is OK (shall be > 0)
#define  FS_NB_CACHE_CLUSLIST             16//16  // In player mode, 1 is OK (shall be > 0)

typedef struct
{
	U16	disk_id;
	U32	start_cluster;
	U32	offset_by_clus;
	U32	cluster;
	U32	seg_len;
}ClusterListItem;

typedef union
{
   struct
   {  
   // GLOBAL
      U8          language_ihm;
      U8          poweroff_value;
   // DISPLAY
      U8          backlight_mode;
      U8          backlight_timeout;
   // PLAYER
      U8          player_repeat;
      U8          player_volume;
   // DEVICE
   	  U8		  bluetooth;
   	  U8          alarm;
   // DATE
   	  U8          year;
   	  U8          month;
	  U8		  day;
   // TIMER
   	  U8          hour;
      U8          min;
   // alarm timer
   	  //U8		  alarm_hour;
   	  //U8          alarm_min;
   	  U16		 ap4_num;
   	  U16        studyAp4;
   	  U16        studyUnite;
   }struct_setting;

    U8     buffer[20];
   //U8    buffer[ 16 ];
}SETTING_NEW;



typedef struct
{
   U16   Ap3_num;
   U16   Book_num;
   U16   Page_num;
   U16   Control_num;
}Record;

typedef union
{
   struct
   {
      // 0 :no anjian;1 : anjian
      unsigned flag_anjian            :1;
      //0 :no new index;1 new index;
      unsigned flag_index             :1;
      //
      unsigned flag_anjian_to_index   :1;
      //00:no contrl;01:single table; 10 :game table;
      //unsigned mode_flag              :2;
      // 0: enable next key; 1: disable next key
      unsigned disable_next_key_flag  :1;
      
      // 01: control table; 10: default table; 11: background table;
      unsigned table_flag             :2;
      //0: no reread; 1: reread;
      unsigned reread_flag            :1;
      //0: no repeat; 1: repeat;
      unsigned repeat_flag            :1;
      
      //0: no sequence; 1: sequence;
      unsigned sequence_flag          :1;
      //0: not oid index normal action; 1: oid index normal action;
      unsigned index_normal_flag      :1;
      //000: no exit flag; 001: poweroff sound start; 010: poweroff sound end;
      //011: search end sound start; 100: search end sound end;
      unsigned poweroff_flag          :4;
      //0: no change mode; 1:change mode
      unsigned ap3_aeb_flag           :1;
      //
      unsigned play_record_flag       :1;

   }news_sys;
   U16     value;
}SEARCH_STATUS;


typedef union
{
  struct
  {
    unsigned   bInGame             :1;	/* check new key flag,bit0 */
    unsigned   bWrong              :1;	/* search result flag,bit1 */
    unsigned   bSubject            :1;	/* current control mode,bit3 */
    unsigned   bGameSpeech         :1; /* speech end flag */
    
    unsigned   bGameFirst          :1;	/*  */
    unsigned   bAnswerWrong        :1;	
    unsigned   bTimerEnable        :1;
    unsigned   bTimeOverEnable     :1;
    
    unsigned   bTipPlay            :1;
    unsigned   bGameTimeOverPlay   :1;
    unsigned   bGameClicked        :1;
    unsigned   bWaitSubject        :1;
    
    unsigned   bNewLink            :1; 
    /* bNewLink=FALSE,means leapfrog type,
       bNewLink=TRUE,means paka type.*/
    unsigned   bSearchFound        :1;
    unsigned   bGameRev1           :1;
    unsigned   bGameRev2           :1;
  }bits;
  U16     value;
}GAME_STATUS;

#define bInGame               bGameStatus.bits.bInGame
#define bWrong                bGameStatus.bits.bWrong
#define bSubject              bGameStatus.bits.bSubject
#define bGameSpeech           bGameStatus.bits.bGameSpeech
#define bGameFirst            bGameStatus.bits.bGameFirst
#define bAnswerWrong          bGameStatus.bits.bAnswerWrong
#define bTimerEnable          bGameStatus.bits.bTimerEnable
#define bTimeOverEnable       bGameStatus.bits.bTimeOverEnable
#define bTipPlay              bGameStatus.bits.bTipPlay
#define bGameTimeOverPlay     bGameStatus.bits.bGameTimeOverPlay
#define bGameClicked          bGameStatus.bits.bGameClicked
#define bWaitSubject          bGameStatus.bits.bWaitSubject
#define bNewLink              bGameStatus.bits.bNewLink
#define bSearchFound          bGameStatus.bits.bSearchFound

#define game_remote_search    bGameStatus.bits.bGameRev1
#define gl_key_press          bGameStatus.bits.bGameRev2

typedef union
{
  struct
  {
    unsigned    bNotAutoNextGame    :1;//
    unsigned    bGamePrev           :1;//
    unsigned    bExitGamePRN        :1;//
    unsigned    bQuestion           :1;//
    
    unsigned    bSpeechEnd          :1;
    unsigned    bSkipNext           :1;
    unsigned    bSkipPrevious       :1;
    unsigned    bGameRev3           :1;

    unsigned    bGameRev4           :1;
    unsigned    bGameRev5           :1;
    unsigned    bGameRev6           :1;
    unsigned    bGameRev7           :1;

    unsigned    bGameRev8           :1;
    unsigned    bGameRev9           :1;
    unsigned    bGameRev10          :1;
    unsigned    bGameRev11          :1;
  }bits;
  U16      value;
}GAME_BIT_NEW;

#define bNotAutoNextGame      game_bits.bits.bNotAutoNextGame
#define bGamePrev             game_bits.bits.bGamePrev
#define bExitGamePRN          game_bits.bits.bExitGamePRN
#define bQuestion             game_bits.bits.bQuestion
#define bSpeechEnd            game_bits.bits.bSpeechEnd
#define bSkipNext             game_bits.bits.bSkipNext
#define bSkipPrevious         game_bits.bits.bSkipPrevious

#define reread_enable_flag    game_bits.bits.bGameRev3
#define game_exit_to_control  game_bits.bits.bGameRev4
#define g_b_ap4_caption       game_bits.bits.bGameRev5
#define b_reload_ap4          game_bits.bits.bGameRev6
#define b_remote_play_repeat  game_bits.bits.bGameRev7
#define b_play_repeat         game_bits.bits.bGameRev8
#define b_rev09				  game_bits.bits.bGameRev9
#define action_repeat_display game_bits.bits.bGameRev10

#define bExitGameAction       game_bits.bits.bGameRev11

typedef union
{
   struct
   {
      unsigned    bAp4Rev0         	:1;//
      unsigned    bAp4Rev1          :1;//
      unsigned    bAp4Rev2          :1;//
      unsigned    bAp4Rev3          :1;//
      
      unsigned    bAp4Rev4          :1;
      unsigned    bAp4Rev5          :1;
      unsigned    bAp4Rev6          :1;
      unsigned    bAp4Rev7          :1;
      
      unsigned    bAp4Rev8          :1;
      unsigned    bAp4Rev9          :1;
      unsigned    bAp4Rev10         :1;
      unsigned    bAp4Rev11         :1;

      unsigned    bAp4Rev12         :1;
      unsigned    bAp4Rev13         :1;
      unsigned    bAp4Rev14         :1;
      unsigned    bAp4Rev15         :1;

   }bits;
   U16      value;
}AP4_BIT_NEW;


#define b_BackSubdir	      			ap4_bit_news.bits.bAp4Rev0
#define b_disptime            			ap4_bit_news.bits.bAp4Rev1
#define b_fanyi               			ap4_bit_news.bits.bAp4Rev2
#define b_check_word          			ap4_bit_news.bits.bAp4Rev3

#define b_first_findword_display    	ap4_bit_news.bits.bAp4Rev4
#define b_ready_findword      			ap4_bit_news.bits.bAp4Rev5
#define b_findword_ch_display 			ap4_bit_news.bits.bAp4Rev6
#define b_fanyi_back_display       		ap4_bit_news.bits.bAp4Rev7


#define b_play_word_mp3		       		ap4_bit_news.bits.bAp4Rev8
#define b_adb_file			       		ap4_bit_news.bits.bAp4Rev9
#define b_play_adb_mp3			       	ap4_bit_news.bits.bAp4Rev10
#define b_g_list				       	ap4_bit_news.bits.bAp4Rev11

#define b_g_index_flag			       	ap4_bit_news.bits.bAp4Rev12
#define b_LastPage_Flag			       	ap4_bit_news.bits.bAp4Rev13

extern   U16               all_mp3_num;
extern   U16             t01_calib_data1;
extern	 U16             t01_calib_data2;

extern   AP4_BIT_NEW       ap4_bit_news;

extern   U8                count_type_num;
extern   U8                Wrong_Answered[ 4 ];

extern   U32               batt_low_warning_timer;
extern   U32               usb_charge_time;

extern   U8                LastKeyState;
extern   U8                last_record_number;
// Declaration of the setting structure
extern   SETTING_NEW       g_setting_list;

#define	 battery_value	   adc0value

extern   U16               dirnum_diandu;
extern   U16               ap4num_diandu;
extern   U32               point_A;
extern   U32               point_B;
extern   U16			   reread_num;
extern   U16               count_AB;
extern   U16               size_psec_wav;
extern   U32      		   current_play_time;
extern   U16               adc0value;
extern   U16               adc2_value;
extern   U16               adc3_value;
extern   U16               adc7_value;
extern   GAME_STATUS       bGameStatus;
extern   GAME_BIT_NEW      game_bits;
extern   U8                HintAnswerIndex;
extern   U8                g_game_rand_tab[ 4 ];
extern   U8                AnswerIndex;
extern   U8                AnswerCur;
extern   U8                CurrentAnswerCount;
extern   U8                ErrCur;
extern   U8                Answered[ 4 ];
extern   U8                GameCount;
extern   U8                RightNumber;
extern   U16               CurrentGame;
extern   U16			   system_dir[LEN_SYSTEM_DIR];
extern   Record            Current_Record;
extern   SEARCH_STATUS     Search_Record;

extern   U8                ap3_mode;
extern   U8                Ap3_MenuPath;
extern   U16               ap4_AllControl_MaskGlobal;

extern   U16               remote_index_tab[ REMOTE_INDEX_MAX ];
extern   U8                ap3_poweron_state;
extern   U8                g_power_off_mode;
extern   U8                mmi_file_flag;

extern   U8                recorder_mode;
//extern   U8                g_volume;
#define  g_volume          g_setting_list.struct_setting.player_volume

#define  g_bluetooth	   g_setting_list.struct_setting.bluetooth
//#define  g_alarm		   g_setting_list.struct_setting.alarm
#define  g_xuanshu_num	   g_setting_list.struct_setting.ap4_num

#define gStudyAp4          g_setting_list.struct_setting.studyAp4
#define gStudyUnite        g_setting_list.struct_setting.studyUnite


extern   U8                g_old_select_appli;
extern   U8                g_select_appli;

extern   U8                s_battery_level;


extern   ClusterListItem   cluster_buf_bak[ FS_NB_CACHE_CLUSLIST ];
typedef union
{
   struct
   {
      unsigned    bPlayStartup         :1;//
      unsigned    bOidInit             :1;//
      unsigned    bEbookmode           :1;//
      unsigned    bEnterCal            :1;//
      
      unsigned    bRemote              :1;
      unsigned    bBatteryLow          :1;
      unsigned    bUsbConnect          :1;
      unsigned    bBatChargeOn         :1;
      
      unsigned    bHeadPhone           :1;
      unsigned    bBatCharging         :1;
      unsigned    bUsbFirstCheck       :1;
      unsigned    bChangeVolume        :1;

      unsigned    bPlayBatWaring       :1;
      unsigned    bSysRev13	           :1;
      unsigned    bSysRev14            :1;
      unsigned    bClusterList         :1;

   }bits;
   U16      value;
}SYSTEM_BIT_NEW;

extern   SYSTEM_BIT_NEW    g_system_flag;


#define  play_startup_flag          g_system_flag.bits.bPlayStartup
#define  b_oid_init_flag            g_system_flag.bits.bOidInit
#define  bInEbookMode               g_system_flag.bits.bEbookmode
#define  bRemoteEnterCal            g_system_flag.bits.bEnterCal
#define  g_b_remote_on              g_system_flag.bits.bRemote
#define  g_b_battery_low            g_system_flag.bits.bBatteryLow
#define  Usb_connect                g_system_flag.bits.bUsbConnect
#define  b_bat_charge_on            g_system_flag.bits.bBatChargeOn
#define  b_HeadPhone_on             g_system_flag.bits.bHeadPhone
#define  b_battery_charging         g_system_flag.bits.bBatCharging
#define  b_usb_first_check          g_system_flag.bits.bUsbFirstCheck
#define  b_change_volume            g_system_flag.bits.bChangeVolume
#define  b_play_bat_waring          g_system_flag.bits.bPlayBatWaring
#define  b_clusterlist              g_system_flag.bits.bClusterList
#define  b_Backap4		            g_system_flag.bits.bSysRev13
#define  g_b_system_clk_low			g_system_flag.bits.bSysRev14


typedef union
{
   struct
   {
      unsigned    bRev0         :1;//
      unsigned    bRev1         :1;//
      unsigned    bRev2         :1;//
      unsigned    bRev3         :1;//
      
      unsigned    bRev4         :1;
      unsigned    bRev5         :1;
      unsigned    bRev6         :1;
      unsigned    bRev7         :1;
      
      unsigned    bRev8         :1;
      unsigned    bRev9         :1;
      unsigned    bRev10        :1;
      unsigned    bRev11        :1;

      unsigned    bRev12        :1;
      unsigned    bRev13        :1;
      unsigned    bRev14        :1;
      unsigned    bRev15        :1;

   }bits;
   U16      value;
}BIT_NEW;

extern   BIT_NEW    g_sys_flag;

#define  b_NoRightAp4			g_sys_flag.bits.bRev0
#define  b_HaveKeyPress			g_sys_flag.bits.bRev1
#define  action_display_text	g_sys_flag.bits.bRev2
#define  content_back_unit		g_sys_flag.bits.bRev3

#define  b_Idle_ReadOid			g_sys_flag.bits.bRev4
#define  g_b_flush_display		g_sys_flag.bits.bRev5
#define  g_b_reread_play		g_sys_flag.bits.bRev6
#define  g_b_warning_tishi		g_sys_flag.bits.bRev7
#define  g_b_enable_flush		g_sys_flag.bits.bRev8

#define  g_b_booksys_flag		g_sys_flag.bits.bRev9
#define  g_b_xuanshu_flag		g_sys_flag.bits.bRev10

#define  g_b_first_n_p_key_flag g_sys_flag.bits.bRev11
#define  g_b_first_display_flag g_sys_flag.bits.bRev12


#define  g_b_V1Hard_Flag 		g_sys_flag.bits.bRev13
#define  g_b_REREAD_Flag 		g_sys_flag.bits.bRev14

#define  gbRereadingFlag 		g_sys_flag.bits.bRev15
extern BIT_NEW    		  gSysRevFlag;

#define  b_readySysUP			gSysRevFlag.bits.bRev0
#define  bRemoteNormal  		gSysRevFlag.bits.bRev1
#define  bBackNormal      		gSysRevFlag.bits.bRev2

