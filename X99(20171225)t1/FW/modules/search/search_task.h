//!
//! @file search_task.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the search task.
//!
//! @version 1.1 snd3b-dvk-1_0_33 $Id: search_task.h,v 1.1 2006/12/19 13:13:34 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _SEARCH_TASK_H_
#define _SEARCH_TASK_H_

#include "conf\conf_sio.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _search_task_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ D E F I N I T I O N ________________________________________________

//#define  SEARCH_SYSTEM_CLOCK        TIMER_MS( 10 )


//_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U32   search_clock_time;


//_____ D E C L A R A T I O N ______________________________________________

void  search_task_init  ( void );
void  search_task_start ( void );
void  search_task_stop  ( void );
void  search_task       ( void );

#endif  //#if( AP3 == ENABLE )

#endif  // _SEARCH_TASK_H_ 
