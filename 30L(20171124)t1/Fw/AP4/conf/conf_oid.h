//! @file conf_oid.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the OID interface
//! All the default configuration for the OID controller will be put
//! here.
//! This file will be given to any external customer
//!
//! @version 1.28 snd3b-dvk-1_0_33 $Id: conf_oid.h,v 1.28 2006/12/08 16:21:57 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_OID_H_
#define _CONF_OID_H_

#include "conf_kb.h"

//_____ G E N E R I C   C O N F I G U R A T I O N __________________________

//! --- oid configuration ---
//! Values: - ENABLED  :enable oid
//!         - DISABLED : disable oid
//! Note: the OID and WIRELESS can't be enable in same time

//#define OID                         ( DISABLE )
#define OID                         ( ENABLE )

//! Values: - ENABLED  :enable WIRELESS
//!         - DISABLED : disable WIRELESS
//! Note: the OID and WIRELESS can't be enable in same time
#define WIRELESS                    DISABLED
//#define WIRELESS                    ENABLED
 
#define OID1                        0
#define OID2                        1

//#define OID_AUTO_DETECT             TRUE
#define OID_AUTO_DETECT             FALSE



#endif  // _CONF_OID_H_

