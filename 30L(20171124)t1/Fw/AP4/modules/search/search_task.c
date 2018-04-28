//!
//! @file search_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the searc task.
//!
//! @version 1.53 snd3b-dvk-1_9_0 $Id: searc_task.c,v 1.53 2007/04/03 09:49:23 sguyon Exp $
//!
//! @todo
//! @bug


#pragma DATA = SEARCH_TASK_DATA
#pragma CODE = SEARCH_TASK_CODE

#ifndef _search_task_c_
#define _search_task_c_

//_____  I N C L U D E S ___________________________________________________

#include "compiler.h"
//#include "search.h"
#include "search_task.h"
#include "player_ap3.h"
#include "file.h"
#include "navigation.h"
#include "power.h"             // Power module definition
#include "com.h"
#include "conf_oid.h"

#include "player_task.h"
//#include "mmi_player.h"
#include "mmi_ap3.h"
#include "power_task.h"
#include "player.h"
//#include "mail.h"
//#include "mail_cmd.h"
//#include "mail_evt.h"
//#include "timer_soft.h"
#include "mmgr_kernel.h"
//#include "com_appli.h"

#include "search.h"

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________
Bool g_b_search_on = FALSE;
//_____ D E C L A R A T I O N ______________________________________________


//! Initialization of search task
//!
//void search_task_init( void )
//{
//}

//! Start search task
//!
void search_task_start( void )
{
   g_b_search_on = TRUE;
}

//! Stop search task
//!
//void search_task_stop( void )
//{
//   g_b_search_on = FALSE;
//}

//the function is realize the search
void search_task( void )
{
   if( g_b_search_on )
   {
//      if( Power_is_poweroff_activated())
//         return;
      search_evt();
   }   
}

#endif  //#if( AP3 == ENABLE )

#endif  //_search_task_c_
