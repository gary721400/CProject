//!
//! @file mmi_player.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: mmi_player.h,v 2.1 2007/08/22 14:07:01 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_PLAYER_H_
#define _MMI_PLAYER_H_


//_____ I N C L U D E S ____________________________________________________

#include "compiler.h"

#include "mmi_ap3.h"

//_____ M A C R O S ________________________________________________________




//_____ D E F I N I T I O N S ______________________________________________
extern _MEM_TYPE_SLOW_ U32 file_size_count;
extern  _MEM_TYPE_SLOW_ U32 play_file_size;


//_____ M A C R O S ________________________________________________________
#define mmi_player_kbd_disable()    ( b_kbd_process_disable = TRUE )
#define mmi_player_kbd_enable()     ( b_kbd_process_disable = FALSE )
#define mmi_player_kbd_is_disable() ( b_kbd_process_disable == TRUE )

#define mp3_player_state            ap4_play_state

extern  const U8	 player_dir[];
#endif  //! _MMI_PLAYER_H_
