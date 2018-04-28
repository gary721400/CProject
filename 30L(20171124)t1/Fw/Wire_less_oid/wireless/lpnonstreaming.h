//! @file lpnonstreaming.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: lpnonstreaming.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _LPNONSTREAMING_H_
#define _LPNONSTREAMING_H_

//_____ I N C L U D E S ____________________________________________________

#undef _GLOBEXT_
#if (defined _lpnonstreaming_c_)
#  define _GLOBEXT_
#elif (!defined _GLOBEXT_)
#  define _GLOBEXT_ extern
#endif


#include "lpradio.h"

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________

RADIO_STATE    RadioBlockingTransmit   ( U8 retryCount, RADIO_LENGTH length );
void           RadioGetTransmitStateIsr( void );
void           RadioStartReceive       ( void );
RADIO_STATE    RadioGetReceiveState    ( void );
void           RadioGetReceiveStateIsr ( void );
RADIO_LENGTH   RadioEndReceive         ( void );
void           RadioForceState         ( XACT_CONFIG endStateBitsOnly );
RADIO_RSSI     RadioGetRssi            ( void );
RADIO_LENGTH   RadioAbort              ( void );


#endif  // _LPNONSTREAMING_H_

