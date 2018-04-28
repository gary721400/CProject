//!
//! @file wireless.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the wireless.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: wireless.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _WIRELESS_H_
#define _WIRELESS_H_

//_____ M A C R O S ________________________________________________________
// Enumeration of mouse states
typedef enum
{
   MOUSE_STATE_DISCONNECTED = 0,
   MOUSE_STATE_ACTIVE,
   MOUSE_STATE_IDLE,
   MOUSE_STATE_OFF,
   MOUSE_STATE_COUNT

}MOUSE_STATES;
//extern MOUSE_STATES mouse_state;
//===============================================================
extern  U32 	g_kbd_key;
//==================================================================
void  wireless_evt   ( void );


#endif  //_WIRELESS_H_
