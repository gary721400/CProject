//!
//! @file mmi_idle.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file declares the MMI application IDLE
//!
//! Is the base application of the MMI.
//!
//! @version 1.22 snd3-dvk-1_9_5 $Id: mmi_idle.h,v 1.22 2007/06/25 13:12:28 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MMI_WIRELESS_H_
#define _MMI_WIREELSS_H_



//_____ I N C L U D E S ____________________________________________________

#include "config.h"

//_____ M A C R O S ________________________________________________________

//! --- Add the Update application in the idle menu ---
//! Values: - ENABLE
//!         - DISABLE

#undef _GLOBEXT_
#if (defined _mmi_wireless_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


//_____ M A C R O S ________________________________________________________


//_____ D E C L A R A T I O N S ____________________________________________


#endif  //! _MMI_WIRELESS_H_
