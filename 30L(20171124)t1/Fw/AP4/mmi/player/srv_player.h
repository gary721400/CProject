//!
//! @file srv_player.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the services for the MMI application PLAYER
//!
//! @version 2.5 snd3-dvk-1_9_5 $Id: srv_player.h,v 2.5 2007/10/08 13:44:44 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _SRV_PLAYER_H_
#define _SRV_PLAYER_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "com_var.h"


//_____ M A C R O S ________________________________________________________




//_____ D E F I N I T I O N S ______________________________________________

// Values return by srvplayer_play()
#define  SRVPLAYER_PLAY_OK          0
#define  SRVPLAYER_PLAY_NOCODEC     1
#define  SRVPLAYER_PLAY_NOSUPPORTED 2
#define  SRVPLAYER_PLAY_FAIL        3

// Values used by srvplayer_volume_change()
#define  SRVPLAYER_VOL_UP           TRUE
#define  SRVPLAYER_VOL_DOWN         FALSE
#define  SRVPLAYER_VOL_LEVEL        5//16

// Definition of the player functionnality states 
typedef enum
{
   MMI_PLAYER_STATE_STOP = 0
,  MMI_PLAYER_STATE_PLAY
,  MMI_PLAYER_STATE_PAUSE
,  MMI_PLAYER_STATE_PLAY_FFW
,  MMI_PLAYER_STATE_PLAY_FRW
,  MMI_PLAYER_STATE_PLAY_A
,  MMI_PLAYER_STATE_SEARCH_A
,  MMI_PLAYER_STATE_PLAY_AB
}Srv_player_states;

// Configuration of player service
typedef struct
{
   // Configue explorer
   /*Fs_index       file;
   U8             exp_use;
   U16            exp_pos;
   Exp_mode       exp_mode;
   Exp_rand       exp_rand;*/
   Repeat         repeat;
   // Configue player
   //Elapsed_time   start_time;
   U8             volume;
   /*U8             eq_ambiance;
   Eq_bands       eq_user;
   U8             speed;
   U8             bassboost;
   U8             vsurround;*/
} St_player_arg;


// Gives the state of the player
//extern   _MEM_TYPE_SLOW_   Srv_player_states    srvplayer_g_player_state;
//extern   _MEM_TYPE_SLOW_   St_player_arg        srvplayer_g_arg;


//_____ D E C L A R A T I O N S ____________________________________________

//Bool  srvplayer_switch_on        ( void );
//void  srvplayer_switch_off       ( void );
void  srvplayer_file_getinfos    ( void );
U8    srvplayer_play             ( Bool b_resume_time );
void  srvplayer_stop             ( void );
void  srvplayer_volume_send      ( void );
void  srvplayer_volume_change    ( Bool change );
Bool  srvplayer_pause            ( void );
Bool  srvplayer_restart_play     ( void );


#endif  //! _SRV_PLAYER_H_
