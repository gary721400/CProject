//!
//! @file com_evt.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of all events that could be find
//! within the embedded SW.
//! This file can be easily updated by anybody external.
//! 
//! If an event needs to be removed, the user needs to take care that 
//! the event is no more used in the embedded SW
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: com_evt.h,v 2.1 2007/04/19 14:06:27 coger Exp $
//!
//! @todo
//! @bug
//!
#ifndef _COM_EVT_H_
#define _COM_EVT_H_


// Definition of Events used in the MMI

// MMI appli scroller
#define EVT_SCROLLER                (EVT_START_EVENT_LIST_CUSTOMER + 0)
#define EVT_KILL_SCROLLER           (EVT_START_EVENT_LIST_CUSTOMER + 1)

// Relay the VBUS state information from MMI_STATUS to the others
#define EVT_VBUS_STATE              (EVT_START_EVENT_LIST_CUSTOMER + 3)

// Relay the battery low-level information from MMI_STATUS to the others
#define EVT_ALERT_BATTERY_LOW       (EVT_START_EVENT_LIST_CUSTOMER + 4)

// Relay the information "do enter in power-off mode" to the others MMI
#define EVT_ENTER_IN_POWEROFF       (EVT_START_EVENT_LIST_CUSTOMER + 5)

// Relay the information "end of the power-off" to the others MMI
#define EVT_EXIT_POWEROFF           (EVT_START_EVENT_LIST_CUSTOMER + 6)

//***********************************************
#define EVT_USB_POWERED             ( EVT_START_EVENT_LIST_CUSTOMER + 7 )
#define EVT_BAT_WARING              ( EVT_START_EVENT_LIST_CUSTOMER + 8 )
//***********************************************

#endif  //! _COM_EVT_H_ 