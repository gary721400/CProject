//! @file player_ap3.h,v
//!
//! Copyright (c) 2005 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the player ap3.
//!
//! @version 1.2 snd3b-dvk-1_0_33 $Id: player_ap3.h,v 1.2 2005/10/19 15:09:19 eaudreze Exp $
//!
//! @todo
//! @bug
//!
#ifndef	_PLAYER_AP3_H_
#define	_PLAYER_AP3_H_

//_____  I N C L U D E S ___________________________________________________

//#include "player.h"
#include "conf_sio.h"

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

//_____ D E C L A R A T I O N ______________________________________________
#if( AP3 == ENABLE )
  Bool   player_ap3_start     ( U32 start_addr, U8 flag );
#else
  #define player_ap3_start( start_addr, flag )
#endif  //#if( AP3 == ENABLE )
void  player_ap3_stop   ( void );
Bool  player_ap3_is_busy      ( void );
Bool  player_file_start       ( U32 start_addr, U8 codec );
void interrupt_play(void);
//extern   xdata  U8    g_b_player_on;
//Bool  player_apf_start        ( struct APF_CUE_INFO _MEM_TYPE_SLOW_ * cue_info, U8 codec );
Bool  player_music_start      ( void );

#endif  //_PLAYER_AP3_H_
