//!
//! @file mmi_storage.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief Application MMI mass storage
//!
//! @version 1.16 snd3-dvk-1_9_5 $Id: 
//!
//! @todo
//! @bug
#ifndef _MMI_STORAGE_H_
#define _MMI_STORAGE_H_


//_____ I N C L U D E S ____________________________________________________

#include "config.h"


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _mmi_storage_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


//_____ D E F I N I T I O N S ______________________________________________
// Definition of the applis in the scroller
typedef enum 
{
   HOME
,  EXPLORER
}Storage_applis_in_scroller;


//Display data of the point going along the link between the PC and the folder
typedef struct 
{
   U8 x;
   U8 y;
   U32 IDP;
}Point_data;

//***********************************************
//_GLOBEXT_ _MEM_TYPE_SLOW_ U8 b_usb_powered;
//_GLOBEXT_ Bool g_b_enter_usb_once;
_GLOBEXT_   Bool  usb_key;

_GLOBEXT_   int   usb_core_status;

//***********************************************
//_____ M A C R O S ________________________________________________________
#define  usb_key_enable()     ( usb_key = TRUE )
#define  usb_key_disable()    ( usb_key = FALSE )
#define  usb_key_is_enable()  ( usb_key == TRUE )
//_____ D E C L A R A T I O N S ______________________________________________


#endif  //! _MMI_STORAGE_H_ 
