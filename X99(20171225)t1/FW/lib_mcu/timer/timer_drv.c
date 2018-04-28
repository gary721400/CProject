//! @file timer_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the timer driver definitions.
//!
//! @version 1.6 snd3-dvk-1_9_5 $Id: timer_drv.c,v 1.6 2007/06/25 09:35:19 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _timer_drv_c_
#define _timer_drv_c_


//_____  I N C L U D E S ____________________________________________________

#include "timer_drv.h"
#include "lib_mcu\compiler.h" 
#include "lib_mcu\int\int_drv.h"             // C51 interruption definition
#include "lib_mcu\mcu_drv.h"                 // Component declaration
#include "stdio.h"
#include "sio\com.h"
#include "modules\power\power.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = TIMER_DRV_DATA
#pragma CODE = TIMER_DRV_CODE

//!_____ D E F I N I T I O N S ______________________________________________

void  __interrupt    timer0_isr  ( void );

#if 0
void  System_Timer_Init ( int TimerSource, unsigned int time );

#pragma CODE = TASK_INIT_CODE

static const unsigned int TimerSourceTBL[ TMR_SRC_NUM ][ TMR_RGS_NUM ] =
{
   {( unsigned int )TIMER0, ( unsigned int )&TMR0CR, ( unsigned int )&TM0PRD, TIMER0_INT_TRAP },
   {( unsigned int )TIMER1, ( unsigned int )&TMR1CR, ( unsigned int )&TM1PRD, TIMER1_INT_TRAP },
   {( unsigned int )TIMER2, ( unsigned int )&TMR2CR, ( unsigned int )&TM2PRD, TIMER2_INT_TRAP },
   {( unsigned int )TIMER3, ( unsigned int )&TMR3CR, ( unsigned int )&TM3PRD, TIMER3_INT_TRAP },
   {( unsigned int )TIMER4, ( unsigned int )&TMR4CR, ( unsigned int )&TM4PRD, TIMER4_INT_TRAP }
};
#endif

//! Number of ms since Reset
U32   _MEM_TYPE_MEDSLOW_   g_timer_tick;

//extern Bool gfSysUSBMP3Mode;

#pragma CODE = TASK_INIT_CODE

//! @brief InitTimer
//!
//! This function initialise the soft timer library.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//! @return none
void timer_init_timer(void)
{
   // Init the timer interuption in the correct page
/*#if( SYS_TIMER == 0 )
   Int_t0_disable();
#elif( SYS_TIMER == 1 )
   Int_t1_disable();
#elif( SYS_TIMER == 2 )
   Int_t2_disable();
#elif( SYS_TIMER == 3 )
   Int_t3_disable();
#elif( SYS_TIMER == 4 )
   Int_t4_disable();
#endif*/

   //System_Timer_Init( SYS_TIMER, TICK_PERIOD );
   SetVector( TIMER_TRAP, timer0_isr );
   TIMER_ICR = 0x01;
   TIMER_CR = 0xD005;
   TIMER_PRD = TMRxPRD( TICK_PERIOD );
   TIMER_VAL = 0x00;

   // And initialise our tick counter: 1 IT is 2MS
   g_timer_tick   =  0;

/*#if( SYS_TIMER == 0 )
   Int_t0_enable(); // This shall be the last thing to do
#elif( SYS_TIMER == 1 )
   Int_t1_enable();
#elif( SYS_TIMER == 2 )
   Int_t2_enable();
#elif( SYS_TIMER == 3 )
   Int_t3_enable();
#elif( SYS_TIMER == 4 )
   Int_t4_enable();
#endif*/
}


#pragma CODE = TIMER_DRV_CODE

//! @brief timer_check_timeout
//!
//! This function test if a timeout occured on the timer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U32 timer: variable used for timer to compare
//!
//! @return TRUE if timeout occured, else FALSE.
Bool timer_check_timeout(U32 timer)
{
   if( timer_get_time()>=timer ) { return TRUE;  }
   else                          { return FALSE; }
}


#if 0
//! @brief timer_wait
//!
//! This function just wait the timeout of the timer
//!
//! @warning Code:XX bytes (function code length)
//!
//! Ex: timer__03Wait(TIMER_1_S);  // Wait 1 sec.
//!
//! @param  U32 duration : the duration to wait
//! @return none
void timer_wait( U32 duration )
{

#  ifndef KBD_SIMU
   // Set our local timer
   register U32 timer   =  Timer_set_timer( duration );

#     ifdef TEST_TIMER
   // We are testing our timer like that. After setting the duration, we pass
   // the Port 2:0 to 0 and wait for the timeout. When occured, we repass to
   // 1 the line and make our calculation via a scope connected on the line
   P2_0  =  0;
#     endif

   // And wait the time out
   while (!Timer_is_timeout(timer));

#     ifdef TEST_TIMER
   // We are testing our timer like that. After setting the duration, we pass
   // the Port 2:0 to 0 and wait for the timeout. When occured, we repass to
   // 1 the line and make our calculation via a scope connected on the line
   P2_0  =  1;
#     endif

#  endif
}
#endif

//! @brief timer_get_time.
//!
//! This function just give the current tick value.
//!
//! @param  none
//! @return the time
U32 timer_get_time( void )
{
   U32 time;
   
/*#if( SYS_TIMER == 0 )
   Int_t0_disable();
#elif( SYS_TIMER == 1 )
   Int_t1_disable();
#elif( SYS_TIMER == 2 )
   Int_t2_disable();
#elif( SYS_TIMER == 3 )
   Int_t3_disable();
#elif( SYS_TIMER == 4 )
   Int_t4_disable();
#endif*/

   time = g_timer_tick;

/*#if( SYS_TIMER == 0 )
   Int_t0_enable();
#elif( SYS_TIMER == 1 )
   Int_t1_enable();
#elif( SYS_TIMER == 2 )
   Int_t2_enable();
#elif( SYS_TIMER == 3 )
   Int_t3_enable();
#elif( SYS_TIMER == 4 )
   Int_t4_enable();
#endif*/

   return time;
}


void __interrupt timer0_isr( void )
{
   g_timer_tick++;
}



#if 0
#pragma CODE = TASK_INIT_CODE

void System_Timer_Init( int TimerSource, unsigned int time )
{
   // clock source is 12Mhz, pre-scale is divided by 32.
   /**(( __X unsigned int *)TimerSourceTBL[ TimerSource ][ 1 ]) = 0xD005u;
   *(( __X unsigned int *)TimerSourceTBL[ TimerSource ][ 2 ]) = TMRxPRD( time );
   SetVector( TimerSourceTBL [TimerSource ][ 3 ], timer0_isr );
   *(( __X unsigned int *)TimerSourceTBL[ TimerSource ][ 0 ]) = 0x0001;
   */

   SetVector( TIMER_TRAP, timer0_isr );
   TIMER_ICR = 0x01;
   TIMER_CR = 0xD005;
   TIMER_PRD = TMRxPRD( time );
   TIMER_VAL = 0x00;
}
#endif

#endif   // _timer_drv_c_
