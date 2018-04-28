//!
//! @file srv_recorder.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the services for the application RECORDER
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: srv_recorder.h,v 2.0 2007/03/30 12:26:50 sguyon Exp $
//!
//! @todo
//! @bug
//!

#ifndef _srv_recorder_h_
#define _srv_recorder_h_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"


//_____ M A C R O S ________________________________________________________



//_____ D E F I N I T I O N S ______________________________________________


//_____ M A C R O S ________________________________________________________


//_____ D E C L A R A T I O N S ____________________________________________

void  srvrec_init    ( Bool source_mic );
Bool  srvrec_start   ( void );
void  srvrec_stop    ( void );

#endif  //! _srv_recorder_h_
