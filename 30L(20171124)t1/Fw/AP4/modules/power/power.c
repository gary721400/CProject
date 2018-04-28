//! @file power.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the player management routines
//!
//! @version 1.22 snd3-dvk-1_9_5 $Id: power.c,v 1.22 2007/05/25 14:26:37 sguyon Exp $
//!
//! @configuration
//!  This module must be configured in conf_power.h header file
//!
//! @todo
//!  Microphone bias management when microphone input is selected
//!  Codec information BR, FS, Channel reporting
//!
//! @bug

#pragma DATA = POWER_DATA
#pragma CODE = POWER_CODE

#ifndef _power_c_
#define _power_c_


//_____  I N C L U D E S ___________________________________________________


#include "compiler.h"                          // System definition
//#include "keypad_drv.h"          // Keypad driver definition
#include "power.h"             // Power module definition
#include "system.h"
#include "conf_oid.h"
//#include "Port.h"
//#include "BinaryImage.h"
//#include "USB_Detect.h"
#if( OID == ENABLE )
#include "oid.h"
#endif
#include "com.h"


//_____ M A C R O S ________________________________________________________

//_____ P R I V A T E    D E C L A R A T I O N _____________________________

#ifdef NIEQING_DIANCHI
 _MEM_TYPE_SLOW_ U32 nieqing_time;
 _MEM_TYPE_SLOW_ U8  nieqing_count;
#endif




 //U8 _MEM_TYPE_SLOW_ mmi_file_flag;
 //extern unsigned int gflag;



extern unsigned int current_wav_num;

//extern void Delay_ms(int count);
//extern void GPIO_init(void);

//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________


//_____ F U N C T I O N S __________________________________________________
#if 0
//! Power supply switch on
//!
void power_system_on(void)
{
}
#endif

//! power_forbid_autopoweroff
//!
//! Forbid the auto power off by the system part
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return U8 : voltage level 0x00 -> 0x10
//!


#if (POWER_OFF_AUTO == ENABLE)

#endif

#pragma CODE = POWER_SYSTEM_OFF_CODE

#endif   // _power_c_
