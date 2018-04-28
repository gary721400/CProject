//!
//! @file paster_task.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster task.
//!
//! @version 1.3 snd3b-dvk-1_0_33 $Id: paster_task.h,v 1.3 2006/03/29 14:25:04 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef	_PASTER_TASK_H_
#define	_PASTER_TASK_H_

//_____  I N C L U D E S ___________________________________________________
#include "conf\conf_sio.h"

#if( PASTER == ENABLE )
//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________

void  paster_task_init  ( void );
void  paster_task_start ( void );
void  paster_task_stop  ( void );

#endif  //#if( PASTER == ENABLE )

#endif  // _PASTER_H_ 
