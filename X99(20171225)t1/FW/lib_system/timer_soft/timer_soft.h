//! @file timer_soft.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the timer driver definitions.
//!
//! @version 1.14 snd3-dvk-1_9_5 $Id: timer_soft.h,v 1.14 2006/12/08 16:22:10 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _TIMER_SOFT_H_
#define _TIMER_SOFT_H_

#undef _GLOBEXT_
#if (defined _timer_soft_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________
#include "lib_mcu\timer\timer_drv.h"


//_____ D E F I N I T I O N S ______________________________________________



//_____ M A C R O S ________________________________________________________



//_____ D E F I N I T I O N S _______________________________________________

// The next definitions will be obsolete. Please use the ones above
//
//! Possible values to set timers with a tick period set to 2ms
//! The tick period is set in "config.h" and should be not modified
#define TIMER_2_MS      1
#define TIMER_4_MS      2     //(TIMER_2_MS * 2)
#define TIMER_6_MS      3     //(TIMER_4_MS + TIMER_2_MS)
#define TIMER_8_MS      4     //(TIMER_6_MS + TIMER_2_MS)
#define TIMER_10_MS     5     //(TIMER_8_MS + TIMER_2_MS)
#define TIMER_20_MS     10    //(TIMER_10_MS * 2)
#define TIMER_30_MS     20    //(TIMER_10_MS * 3)
#define TIMER_40_MS     30    //(TIMER_10_MS * 4)
#define TIMER_50_MS     40    //(TIMER_10_MS * 5)
#define TIMER_100_MS    50    //(TIMER_50_MS * 2)
#define TIMER_200_MS    100   //(TIMER_100_MS * 2)
#define TIMER_600_MS    300   //(TIMER_100_MS * 5)
#define TIMER_1_S       500   //(TIMER_500_MS * 2)
#define TIMER_2_S       1000  //(TIMER_1_S * 2)
#define TIMER_4_S       2000  //(TIMER_1_S * 4)
#define TIMER_6_S       3000  //(TIMER_1_S * 5)
#define TIMER_10_S      5000  //(TIMER_1_S * 10)
#define TIMER_20_S      10000 //(TIMER_1_S * 20)
#define TIMER_40_S      20000 //(TIMER_1_S * 30)
#define TIMER_1_MIN     30000 //(TIMER_1_S * 60)


// For the following structure, the byte being the most used is defined first.
// The less used is defined last. The adressing of this structure trough a pointer
// will be then efficient.
typedef struct 
{
   U8  status;
   U32 reg;
}Timer_soft;

#define TIMER_MASK_RUN      0x01
#define TIMER_MASK_OVF      0x02
#define TIMER_MASK_NO_EVT   0x04
#define TIMER_MASK_USED     0x08

#define UNDEFINED_TIMER  0


//_____ M A C R O S ________________________________________________________
///*
#define Ts_free( id )      \
{                          \
   _ts_free(id);           \
   id = UNDEFINED_TIMER;   \
}
//*/

//_____ D E C L A R A T I O N S _______________________________________________

_GLOBEXT_ void   ts_init(     void);


_GLOBEXT_ U8     ts_alloc(    void              );
_GLOBEXT_ void   _ts_free(    U8 id             ); // Should not be used, prefer macro "Ts_free()"
_GLOBEXT_ void   ts_set_time( U8 id, U32 delay );
_GLOBEXT_ void   ts_stop(           U8 id       );
_GLOBEXT_ Bool   ts_is_overflowed(  U8 id       );
_GLOBEXT_ Bool   ts_is_running   (  U8 id       );
_GLOBEXT_ void   ts_process(        void        );
//_GLOBEXT_ void   Ts_free				( U8 id )    ;


#endif  // _TIMER_SOFT_H_



