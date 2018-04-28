//!
//! @file player_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player task.
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: player_task.c,v 2.1 2007/06/25 09:20:11 sguyon Exp $
//!
//! @todo
//! @bug

#pragma DATA = PLAYER_TASK_DATA
#pragma CODE = PLAYER_TASK_CODE


#ifndef _player_task_c_
#define _player_task_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
//#include "conf_mmi.h"
//#include "conf_explorer.h"
#include "system.h"             // Power module definition
#include "player_task.h"

// Include for mail box
//#include "mail.h"
//#include "mail_cmd.h"
//#include "mail_evt.h"
// Include player module
//#include "player.h"
//#include "ctrl_access.h"
#include "string.h"
#include "stdio.h"
//#include "explorer.h"
//#include "mmi_player.h"

#include "file.h"           // for file management 
#include "com.h"

#include "keypad_drv.h"
#include "keypad.h"
#include "conf_oid.h"
#include "search.h"
#include "search_remote.h"
#include "search_task.h"
//#include "power_task.h"
#include "search_game.h"
#include "system.h"  // Include the timer to precharge
#include "mmi_ap3.h"
//#include "MP3_appl.h"    // MP3 ApplicationLayer header file
//#include "srv_player.h"

//#include "com_appli.h"
//#include "mp3_include.h"

//_____ M A C R O S ________________________________________________________

// _MEM_TYPE_SLOW_ _FileData FileData;

//extern   xdata    AL_Variables     	varAl;
extern   xdata 		U8 				current_api;

extern   _MEM_TYPE_SLOW_  U32				   mp3_begin_clock;
extern   _MEM_TYPE_SLOW_  U32				   mp3_over_clock;
//extern   Bool     op_open_flag;
//extern void SpeakDetect(void);
extern void judge_op(void);
extern tAudioErr AudioMP3Uninit(void);
extern tAudioErr AudioMP3Stop(void);
extern void player_ap3_stop( void );
//extern void swap_code(U16 flag);
//_____ D E F I N I T I O N ________________________________________________
//Bool  CheckWAVFrameHeader  ( void );

extern void player_volume_off( void );

Bool _MEM_TYPE_SLOW_ g_b_player_on;

extern unsigned int vol_idx;
extern const int VolTbl[VOL_NUM];

extern void vol_dec(void);

//Bool play_flash_flag	   = 0;
//! Initialization of player task
//!
void player_task_init(void)
{
//   mail_send_event( EVT_UPDATE_START, UPDATE_STOPPED );
  g_b_player_on = FALSE;
}

//! Start player task
//!
void player_task_start(void)
{
   g_b_player_on = TRUE;
}


//! Stop player task
//!
void player_task_stop(void)
{
   g_b_player_on = FALSE;
}

//! This fonction manage the player commands
//!
void player_task(void)
{
   if( g_b_player_on )
   {
      if( ! CheckPlaying(FALSE))
      {
//		   MP3_PrevInit();
//         player_volume_off();
//         player_off();
           //g_b_player_on = FALSE;
//         srvplayer_g_player_state = MMI_PLAYER_STATE_STOP;
//         mail_send_event( EVT_PLAYER, PLAYER_EOF )
		 //vol_dec();
 //		 DAC_FadeOut();
//		 OP_CLOSE();
         //AudioMP3Stop();
         //AudioMP3Uninit();
		 //power_authorize_autopoweroff();
		 player_ap3_stop();
		
		 //op_start_timer();
		 
		//SendString(( char *)"\r\nplayer music end!");
      }
   }
//***********************************************
//***********************************************
}
/*
const int PlayStatusTable[] = {PLAYING_STATUS,
                         PAUSE_STATUS,
                         AB_REPEAT_STATUS,
                         A_STAGE_STATUS,
                         FWRWD_PROLOG_STATUS,
                         FORWARD_STATUS,
                         REWIND_STATUS,
                         FWRWD_EPILOG_STATUS,
                         (-1)}; 
*/
/*
Bool CheckPlaying(Bool flag)
{
    tMusicStatus status;
 //   int i = 0;
    
//    SpeakDetect();
	if(DAC_CCMC_Status != 0)
	{
    	DAC_Headphone_Reset();
   // 	DAC_Enable_HeadPhone();
   // 	DAC_HeadPhone_Volume(VolTbl[vol_idx]); // 此處的Vol為UI之前設定的音量值
   		vol_idx_bak = VOL_NUM;
    	DAC_Go();
	}
	adc_get();
	SpeakDetect(FALSE);
	
    status = AudioMP3Status();
  if(PLAYING_STATUS == status)
	 return TRUE;
   else
     return FALSE;
}
*/




int CheckPlaying(Bool flag)
{
/*
    tMusicStatus status;
    int i = 0;

	if(DAC_CCMC_Status != 0)
	{
#ifdef  TX_DEBUG	
		SendString("dac reset\r\n");
#endif
    	DAC_Headphone_Reset();
    //	DAC_Enable_HeadPhone();
    //	DAC_HeadPhone_Volume(VolTbl[vol_idx]); // 此處的Vol為UI之前設定的音量值
    //	DAC_Go();
	}
*/
    SpeakDetect(FALSE);
/*
    status = AudioMP3Status();
    while(1)
    {
        if(PlayStatusTable[i] == (-1))
            break;
        else if(PlayStatusTable[i] == status)
            return (-1);
        else
            i++;
    }
    return 0;
*/	
	return AudioMP3ChkPlaying(); 
}

/*
Bool play_file_start( void )
{
   power_forbid_autopoweroff();
   return TRUE;
}
*/
#endif   //_player_task_c_


