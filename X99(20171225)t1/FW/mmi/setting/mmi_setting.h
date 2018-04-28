//!
//! @file mmi_setting.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application EQUALIZER
//!
//! @version 1.7 snd3-dvk-1_9_5 $Id: mmi_setting.h,v 1.7 2006/03/29 08:02:51 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_SETTING_H_
#define _MMI_SETTING_H_



//_____ I N C L U D E S ____________________________________________________

#include "config.h"


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmi_setting_c_)
#  define _GLOBEXT_
#else
#  define _GLOBEXT_ extern
#endif


//_____ D E C L A R A T I O N S ____________________________________________

// Declaration of the applis being able to launched from the scroller
typedef enum 
{
   HOME
}Setting_applis_in_scroller;


//_____ M A C R O S ________________________________________________________

// Coordinate of the setting window title "My settings"
#define POSX_WND_TITLE          30
#define POSY_WND_TITLE           0

//_____ D E F I N I T I O N S ______________________________________________

// Definition of the ID of applis available via the scroller
#define  BAR_X0      13
#define  BAR_Y0      24
#define  BAR_WIDTH   102
#define  BAR_HEIGHT  7


//_____ D E C L A R A T I O N S ____________________________________________

#endif  //! _MMI_SETTING_H_
