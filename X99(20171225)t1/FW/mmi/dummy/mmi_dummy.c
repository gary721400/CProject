//!
//! @file mmi_dummy.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI application PLAYER
//!
//! @version 1.2 snd3-dvk-1_9_5 $Id: mmi_dummy.c,v 1.2 2005/10/06 12:51:04 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mmi_dummy_c_
#define _mmi_dummy_c_


//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "mmi_dummy.h"


//_____ D E F I N I T I O N S ______________________________________________
//!
//! dummy_mmi_appli
//!
//! Dummy MMI application must be put in bank 0 but must NOT be customized or deleted.
//! The bank 0 is reserved to distinguish the common code and the banked mmi applications.
//! Read "lib_system\banking\Banking_CRP.txt".
//! 
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void dummy_mmi_appli(U8 event, U16 param)
{
   // DO NOT USE THIS FUNCTION. DO NOT DELETE THIS FUNCTION.
   event = event;  // To avoid warnings
   param = param;  // To avoid warnings
   return;
}


#endif

