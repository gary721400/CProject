//! @file timer_soft.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the software timer functions.
//!
//! @version 1.23 snd3-dvk-1_9_5 $Id: timer_soft.c,v 1.23 2007/06/25 09:32:27 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _timer_soft_c_
#define _timer_soft_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"

#include "timer_soft.h"

#pragma DATA = TIMER_SOFT_DATA
#pragma CODE = TIMER_SOFT_CODE

//_____ P R I V A T E   D E C L A R A T I O N S ______________________________
/*volatile*/ Timer_soft _MEM_TYPE_MALLOC_MAIL_ s_timer_soft[NB_TIMER_SOFT];
         Bool                       s_timer_on;


//_____ P R I V A T E   D E C L A R A T I O N S ______________________________



//!_____ D E F I N I T I O N S ______________________________________________

#pragma CODE = TASK_INIT_CODE

//! @brief ts_init
//!
//! This function initialise the software timers.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//! @return none
void ts_init(void)
{
   Timer_soft _MEM_TYPE_MALLOC_MAIL_* ptr_timer;

   // And initialise the status of each timer
   ptr_timer = &s_timer_soft[NB_TIMER_SOFT-1];
   
   s_timer_on=FALSE;

   do
   { 
      ptr_timer->status = 0;
   }
   while( ptr_timer-- != &s_timer_soft[0] );
}


#pragma CODE = TIMER_SOFT_CODE

//! @brief ts_alloc
//!
//! Get a timer software
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return id of the timer allocated       if success
//!         returned value UNDEFINED_TIMER  if failure
//!
U8 ts_alloc( void )
{
   U8 i;
   Timer_soft _MEM_TYPE_MALLOC_MAIL_* ptr_timer;

   ptr_timer = &s_timer_soft[NB_TIMER_SOFT-1];
   i = NB_TIMER_SOFT-1;
   do
   {
      if( 0 == (ptr_timer->status & TIMER_MASK_USED) )
      {
         ptr_timer->status |= TIMER_MASK_USED; // This timer slot is then used
         s_timer_on=TRUE;
         return (i+1);   // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection
      }
      ptr_timer--;
   }
   while( i-- != 0 );

   //no timer slot free or duration egal to zero gives UNDEFINED_TIMER as returned value 
   mail_send_event(EVT_TIMER_ALLOC_FAILURE, 0);
   return UNDEFINED_TIMER;
}


//! @brief _ts_free
//!
//! Set free a timer software
//! DO NOT CALL THIS FUNCTION DIRECTLY : use the macro Ts_free() instead of it.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the timer software to get free
//!
//! @return none
//!
void _ts_free( U8 id )
{
   U8 i;
   Timer_soft _MEM_TYPE_MALLOC_MAIL_* ptr_timer;
   
   id -= 1;  // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection

   if( id >= NB_TIMER_SOFT )   return;

   s_timer_soft[id].status = 0;

   s_timer_on=FALSE;
   ptr_timer = &s_timer_soft[NB_TIMER_SOFT-1];
   i = NB_TIMER_SOFT-1;
   do
   {
      if( 0 == (ptr_timer->status & TIMER_MASK_USED) )
      {
         s_timer_on=TRUE;
      }
      ptr_timer--;
   }
   while( i-- != 0 );
}

/*
void Ts_free( U8 id )      
{   
   if( id != UNDEFINED_TIMER ){
  	 _ts_free(id);           
   	 id = UNDEFINED_TIMER;   
   	}
}
*/

//! @brief ts_set_time
//!
//! Set the time value to one of the software timers allocated
//! and enable the overflow test with the flag "run" set
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id    : id of the timer software to set time
//! @param  U8 delay : delay before getting an overflow
//!
//! @return none
//!
void ts_set_time( U8 id, const U32 delay )
{
   id -= 1;   // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection

   if( id >= NB_TIMER_SOFT )   return;

   s_timer_soft[id].reg     = Timer_set_timer( delay );
   s_timer_soft[id].status |= TIMER_MASK_RUN;  // Required to make timer run after its allocation
   s_timer_soft[id].status &= ~TIMER_MASK_OVF; // Required to test a next overflow of the timer
}

#if 0
//! @brief ts_stop
//!
//! Disable the overflow test with the flag "run" cleared
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the timer software to stop
//!
//! @return none
//!
void ts_stop( U8 id )
{
   id -= 1;   // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection

   if( id >= NB_TIMER_SOFT )   return;

   s_timer_soft[id].status &= ~TIMER_MASK_RUN;
}
#endif

//! @brief ts_is_overflowed
//!
//! Get the information on overflow of a timer software
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the timer software to test
//!
//! @return TRUE or FALSE
//!
#if 0
Bool ts_is_overflowed( U8 id )
{
   id -= 1;   // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection

   if( id >= NB_TIMER_SOFT )   { return FALSE; }

   if( TIMER_MASK_OVF == (s_timer_soft[id].status & TIMER_MASK_OVF) )   { return TRUE;  }
   else                                                                 { return FALSE; }
}



//! @brief ts_is_running
//!
//! Get the information on running or not of a timer software
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the timer software to test
//!
//! @return TRUE or FALSE
//!
Bool ts_is_running( U8 id )
{
   id -= 1;   // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection

   if( id >= NB_TIMER_SOFT )   { return FALSE; }

   if( TIMER_MASK_RUN == (s_timer_soft[id].status & TIMER_MASK_RUN) )   { return TRUE;  }
   else                                                                 { return FALSE; }
}
#endif


#pragma CODE = TIMER_SOFT_PROCESS_CODE

//! @brief timer_process
//!
//! Test if timers software have overflowed.
//! If true, a message EVT_TIMER with the timer id as parameter is posted to the event mailbox
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
//!
void ts_process( void )
{
   U8 i;
   Timer_soft _MEM_TYPE_MALLOC_MAIL_* ptr_timer;

   // Fast test for a quick exit
   //
   if( !s_timer_on ) return;

   ptr_timer = &s_timer_soft[NB_TIMER_SOFT-1];
   i = NB_TIMER_SOFT-1;

   do
   {
      // Decrementation of the timer register value 
      if( TIMER_MASK_RUN == (ptr_timer->status & TIMER_MASK_RUN) )
      { // This timer is running...
         if( 0 == (ptr_timer->status & TIMER_MASK_OVF) )
         { // Timer has not overflowed yet
            if( timer_get_time() >= ptr_timer->reg )
            { // Timer has overflowed
               ptr_timer->status |= TIMER_MASK_OVF;
               mail_send_event(EVT_TIMER, i+1); // Timer index : from 1 to NB_TIMER_SOFT; 0 is kept for no timer selection
            }
         }
      }
      ptr_timer--;
   }
   while( i-- != 0 );
}



#endif   // _timer_soft_c_
