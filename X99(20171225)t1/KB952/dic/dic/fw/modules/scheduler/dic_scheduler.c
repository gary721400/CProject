//! @file scheduler.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//! @brief This file contains the scheduler routines
//!
//!Configuration:
//!  - SCHEDULER_TYPE in scheduler.h header file
//!  - Task & init for at least task number 1 must be defined
//!
//! @version 1.4 snd3-dvk-1_9_5 $Id: scheduler.c,v 1.4 2006/12/08 16:22:10 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _startup_scheduler_c_
#define _startup_scheduler_c_

//!_____ I N C L U D E S ____________________________________________________
#include "config.h"                       // system definition 
#include "dic_conf_scheduler.h"       // Configuration for the scheduler
#include "dic_scheduler.h"            // scheduler definition 
#include "sio\com.h"
#include <KBDM9_DDK.H>

#pragma DATA = SCHEDULER_DATA
#pragma CODE = SCHEDULER_CODE


ImageDeclareType( TASK_INIT_CODE );

//!_____ M A C R O S ________________________________________________________
//!_____ D E F I N I T I O N ________________________________________________
#if SCHEDULER_TYPE != SCHEDULER_FREE
//! When SCHEDULER_TYPE != SCHEDULER_FREE, this flag control task calls.
bit   scheduler_tick_flag;
#endif

#ifdef TOKEN_MODE
//! Can be used to avoid that some tasks executes at same time. 
//! The tasks check if the token is free before executing. 
//! If the token is free, the tasks reserve it at the begin of the execution 
//! and release it at the end of the execution to enable next waiting tasks to 
//! run.	
Uchar token;
#endif

//!_____ D E C L A R A T I O N ______________________________________________
//! Scheduler initialization
//!
//! Task_x_init() and Task_x_fct() are defined in config.h
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  :none
//! @return :none
void scheduler_init (void)
{
   SDKLoadCode( TASK_INIT_CODE );
   
   #ifdef Scheduler_time_init
      Scheduler_time_init();
   #endif
   #ifdef Scheduler_task_1_init
      Scheduler_task_1_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_2_init
      Scheduler_task_2_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_3_init
      Scheduler_task_3_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_4_init
      Scheduler_task_4_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_5_init
      Scheduler_task_5_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_6_init
      Scheduler_task_6_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_7_init
      Scheduler_task_7_init();  
      Scheduler_call_next_init();
   #endif
   #ifdef Scheduler_task_8_init
      Scheduler_task_8_init();  
      Scheduler_call_next_init();
   #endif

//***********************************************
   //2007-04-09 add by xuanyu
   #ifdef Scheduler_task_9_init
      Scheduler_task_9_init();  
      Scheduler_call_next_init();
   #endif

   #ifdef Scheduler_task_10_init
      Scheduler_task_10_init();  
      Scheduler_call_next_init();
   #endif
//***********************************************
   Scheduler_reset_tick_flag();
}

//! Task execution scheduler
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  :none
//! @return :none
void scheduler_tasks (void)
{
   // To avoid uncalled segment warning if the empty function is not used
   scheduler_empty_fct();

   for(;;)
   {
      Scheduler_new_schedule();
      #ifdef Scheduler_task_1
         Scheduler_task_1();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_2
         Scheduler_task_2();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_3
         Scheduler_task_3();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_4
         Scheduler_task_4();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_5
         Scheduler_task_5();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_6
         Scheduler_task_6();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_7
         Scheduler_task_7();
         Scheduler_call_next_task();
      #endif
      #ifdef Scheduler_task_8
         Scheduler_task_8();
         Scheduler_call_next_task();
      #endif
//***********************************************
      //2007-04-09 add by xuanyu
      #ifdef Scheduler_task_9
         Scheduler_task_9();
         Scheduler_call_next_task();
      #endif

      #ifdef Scheduler_task_10
         Scheduler_task_10();
         Scheduler_call_next_task();
      #endif
//***********************************************
   }
}

//! Init & run the scheduler
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  :none
//! @return :none
void scheduler (void)
{
   scheduler_init();
   scheduler_tasks();
}


//! Do nothing
//! Avoid uncalled segment warning if the empty function is not used
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  :none
//! @return :none
void scheduler_empty_fct (void)
{
}

#endif	// _scheduler_c_
