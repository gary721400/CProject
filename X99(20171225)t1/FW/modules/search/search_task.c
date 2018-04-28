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


#ifndef _search_task_c_
#define _search_task_c_

//_____  I N C L U D E S ___________________________________________________

#include "lib_mcu\compiler.h"
//#include "search.h"
#include "search_task.h"
#include "modules\player_ap3\player_ap3.h"
#include "modules\file_system\file.h"
#include "modules\file_system\navigation.h"
#include "modules\power\power.h"             // Power module definition
#include "sio\com.h"
#include "conf\conf_oid.h"
//#include "SSSMP3_Regs.h"
#include "modules\player\player_task.h"
#include "mmi\player\mmi_player.h"
#include "mmi\ap3\mmi_ap3.h"
#include "modules\power\power_task.h"
#include "modules\player\player.h"
#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_cmd.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"
#include "mmi\shared\com_appli.h"

#include "modules\search\search.h"
#include "modules\search\search_game.h"

#pragma DATA = SEARCH_TASK_DATA
#pragma CODE = SEARCH_TASK_CODE

#if( AP3 == ENABLE )
//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________
Bool g_b_search_on /*= FALSE*/;

//_MEM_TYPE_MEDSLOW_   U32   search_clock_time;

extern   Bool  g_b_recorder_on;

//_____ D E C L A R A T I O N ______________________________________________


void __interrupt timer1_isr( void )
{
   ScriptVariable[ 99 ]++;
}


#pragma CODE = TASK_INIT_CODE

//! Initialization of search task
//!
void search_task_init( void )
{
   //search_clock_time = Timer_set_timer( SEARCH_SYSTEM_CLOCK );

   g_b_search_on = FALSE;
}


#pragma CODE = MMI_AP4_START_TEMP_CODE01

//! Start search task
//!
void search_task_start( void )
{
   g_b_search_on = TRUE;
   
   //search_clock_time = Timer_set_timer( SEARCH_SYSTEM_CLOCK );

   SetVector( 0x12, timer1_isr );
   TM1ICR = 0x31;
   TMR1CR = 0xD005;
   TM1PRD = 12000 / 32 * 10 - 1;
   TMR1 = 0x00;

   // And initialise our tick counter: 1 IT is 10MS
   ScriptVariable[ 99 ] =  0;
}


#pragma CODE = SEARCH_TASK_CODE

//! Stop search task
//!
void search_task_stop( void )
{
   g_b_search_on = FALSE;

   TM1ICR = 0x00;
   TMR1CR = 0x00;
}


//the function is realize the search
void search_task( void )
{
   extern Bool g_b_player_on;
   if( g_b_search_on )
   {
      if( Power_is_poweroff_activated())
         return;

      /*if( Timer_is_timeout( search_clock_time ))
      {
         search_clock_time = Timer_set_timer( SEARCH_SYSTEM_CLOCK );
         ScriptVariable[ 99 ]++;
      }*/
      if( !( Search_Record.value & 0xC000u ))
      {
         if( g_b_player_on || g_b_recorder_on )
            return;
      }
      search_evt();
   }
}

#endif  //#if( AP3 == ENABLE )

#endif  //_search_task_c_
