//!
//! @file auto_disp.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the kernel
//!
//! The mmi_manager task is running on messages posted in the mailbox by tasks.
//! These messages can be distributed to the different launched applications
//! in the processing order defined by the LIFO architecture of the stack.
//! An event can be passed from the top application towards the bottom application
//! to the destination application.
//! Each mmi application can catch an event that is destinated to it
//! or forward it to the next lowest-priority application.
//! If any application catch the event, the mmi_manager destroyes it.
//!
//! @version 1.6 snd3-dvk-1_9_5 $Id: auto_disp.h,v 1.6 2006/12/08 16:21:55 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _AUTO_DISP_H_
#define _AUTO_DISP_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "lib_system\mailbox\mail.h"


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _auto_disp_c_)
#  define _GLOBEXT_ extern
#else                                        
#  define _GLOBEXT_ extern
#endif


//_____ D E F I N E S ______________________________________________________

// Binary values of the object variable status (internally use)
#define AD_MASK_RUN      (0x01)
#define AD_MASK_REFRESH  (0x02)
#define AD_MASK_UPDATE   (0x04)
#define AD_MASK_USED     (0x08)

// Binary values of the object variable state_machine
#define AD_MASK_END_ANIM (0x80)

// Undefined object id
#define UNDEF_AD_OBJ     0


//_____ M A C R O S ________________________________________________________
#if 0
#define Ad_free( id )      \
{                          \
   _ad_free(id);           \
   id = UNDEF_AD_OBJ;      \
}
#endif

//_____ D E C L A R A T I O N S ____________________________________________

//! Variable to monitor or get information outside this module
_GLOBEXT_ U8  _MEM_TYPE_SLOW_   ad_g_extra_param;


// Parameter structures to set the graphical objects
typedef struct
{
   U8  x;
   U8  y;
   U32 IDP;  
   U8  time_10ms;    // Time granularity: 10ms
   U8  mode;
}Ad_prm_animation;

// Universal pointer on the parameter structure declared just above
typedef union
{
   Ad_prm_animation* anim;
//   Ad_prm_scrolling* scroll;
}Ad_p_prm;


//_____ D E F I N E S ______________________________________________________

// ********* ANIMATIONS **********
// ID of object types and the options
#define AD_ID_ANIMATION    0


// Information sent with the event EVT_ANIMATION
#define ANIMATION_END      0


// Parameters of the structure Ad_prm_animation
//  Repeat mode
#define AD_SINGLE             0
#define AD_FOREVER            1
#define AD_REPEAT_MODE_MASK   1
//  Display the animation even if its MMI is not at the top 
#define AD_DISP_OVER          2


//_____ D E C L A R A T I O N S ____________________________________________

_GLOBEXT_   void     ad_init(     void          );
_GLOBEXT_   Bool     ad_scan_evt( const Msg evt );
_GLOBEXT_   void     ad_process(  void          );
_GLOBEXT_   U8       ad_allocate( const U8 obj_type, Ad_p_prm _MEM_TYPE_SLOW_ *p_param_struct );
_GLOBEXT_   void     ad_start(    U8 id );
_GLOBEXT_   void     ad_pause(    U8 id );
_GLOBEXT_   void     ad_stop(     U8 id );
_GLOBEXT_   void     ad_refresh(  U8 id );
_GLOBEXT_   void     _ad_free(    U8 id ); // Should not be used directly, prefer macro "Ad_free()"

#endif  //! _AUTO_DISP_H_

