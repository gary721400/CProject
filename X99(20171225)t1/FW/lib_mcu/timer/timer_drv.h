//! @file timer_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the timers 0, 1 and watchdog driver definition
//!
//! @version 1.8 snd3-dvk-1_9_5 $Id: timer_drv.h,v 1.8 2007/01/26 15:58:51 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _TIMER_DRV_H_
#define _TIMER_DRV_H_

#undef _GLOBEXT_
#if (defined _timer_drv_c_)
#  define _GLOBEXT_
#else
#  define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________

#include "lib_mcu\compiler.h"


//_____ O P T I O N S ______________________________________________________

//! Periode of the tick in ms
//! Here the tick is set to 2ms
#define TICK_PERIOD              2

#define SYS_TIMER          3

#define CLOCK              ( 12000 / 32 )       // Kilo Hz
#define TMRxPRD( T )       ( CLOCK *( T )- 1 )  // millisecond

#define TMR_SRC_NUM        5
#define TMR_RGS_NUM        4

#if SYS_TIMER == 0
   #define  TIMER_ICR   TM0ICR
   #define  TIMER_CR    TMR0CR
   #define  TIMER_PR    TM0PRD
   #define  TIMER_VAL   TMR0
   #define  TIMER_TRAP  0x10
#endif
#if SYS_TIMER == 1
   #define  TIMER_ICR   TM1ICR
   #define  TIMER_CR    TMR1CR
   #define  TIMER_PRD   TM1PRD
   #define  TIMER_VAL   TMR1
   #define  TIMER_TRAP  0x12
#endif
#if SYS_TIMER == 2
   #define  TIMER_ICR   TM2ICR
   #define  TIMER_CR    TMR2CR
   #define  TIMER_PRD   TM2PRD
   #define  TIMER_VAL   TMR2
   #define  TIMER_TRAP  0x14
#endif
#if SYS_TIMER == 3
   #define  TIMER_ICR   TM3ICR
   #define  TIMER_CR    TMR3CR
   #define  TIMER_PRD   TM3PRD
   #define  TIMER_VAL   TMR3
   #define  TIMER_TRAP  0x16
#endif
#if SYS_TIMER == 4
   #define  TIMER_ICR   TM4ICR
   #define  TIMER_CR    TMR4CR
   #define  TIMER_PRD   TM4PRD
   #define  TIMER_VAL   TMR4
   #define  TIMER_TRAP  0x18
#endif

//_____ M A C R O S ________________________________________________________


//_____ M A C R O S ________________________________________________________

//! Timer_set_timer
//! Set a timer variable.
//! Ex: my_timer = Timer_set_timer(TIMER_4_S);  
//! @param  delay: delay in ms before timeout
//! @return value to store in the timer variable (x bits)
#define Timer_set_timer(delay)  (timer_get_time() + (U32)(delay) +1)

//! Timer_is_timeout
//! Test if a timeout occurs on a timer.
//! Ex:  if (Timer_is_timeout(my_timer,TIMER_1_S))    // action 
//!      else // wait
//! @param  timer: variable used for timer
//!         delay: delay in ms to compare
//! @return TRUE if timeout occured, else FALSE.
#define Timer_is_timeout(timer)         ( timer_check_timeout(timer) )

//! Timer_reset_timer
//! Force a timeout on timer.
//! Ex:  my_timer = Timer_set_timer(TIMER_1_S);
//!      Timer_reset_timer(my_timer);
//!      if (Timer_is_timeout(my_timer))
//!         ... // Always true due to Timer_reset_timer
//! @param  timer: variable used for timer
//! @return the new value of timer
#define Timer_reset_timer(timer)   ((timer) = timer_get_time())

//! To ease the setting of timers
#define TIMER_MS(x)    ((U32)(x)/(TICK_PERIOD))         //! Wait for 'x' ms
#define TIMER_S(x)     ((U32)(x)*1000/(TICK_PERIOD))    //! Wait for 'x' sec
#define TIMER_MIN(x)   ((U32)(x)*60000/(TICK_PERIOD))   //! Wait for 'x' minutes

#define TIMER_25MS(x)  ((U32)(x)/(25))                  //! Wait for 'x' ms

//_____ D E F I N I T I O N ________________________________________________


_GLOBEXT_ void    timer_init_timer(    void);
_GLOBEXT_ Bool    timer_check_timeout( U32 );
_GLOBEXT_ void    timer_update_timer(  void);
_GLOBEXT_ void    timer_wait(          U32 );
_GLOBEXT_ U32     timer_get_time(      void);


#endif // _TIMER_DRV_H_
