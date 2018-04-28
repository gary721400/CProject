//! @file conf_mmi.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the mmi
//! This file will be given to any external customer
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: conf_mmi.h,v 2.3 2007/07/26 15:02:05 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_MMI_H_
#define _CONF_MMI_H_


//! Auto start USB device at startup if USB power detected (enable/disable)
#define  USB_DEVICE_AUTO_LAUNCH        ENABLE

//! Auto stop USB device if USB activie stop during USB_ACTIVITE_TIMEOUT (enable/disable)
#define  USB_DEVICE_AUTO_STOP          ENABLE


/********* Time definitions for software timers ***********/
//
// Please use TIMER_MS(x)  to wait 'x' milli-seconds (minimum 2ms),
//         or TIMER_S(x)   to wait 'x' seconds,
//         or TIMER_MIN(x) to wait 'x' minutes.
// Timer grnularity is 2 ms.
//
#define SPLASH_TIME           (TIMER_S(1))    //! Delay of the splash screens
#define PLAYER_TIME           (TIMER_MS(1000)) //! Period of the screen refreshing in mmi application Player
#define TS_FLASH_TIME         (TIMER_MS(500)) //! Period of the screen refreshing in mmi application Player
#define STORAGE_ARROW_TIMER   (TIMER_MS(1000))//! Refresh delay of the USB R/W animation in mmi application Storage 
#define USB_ACTIVITE_TIMEOUT  (TIMER_S(2))    //! Exit USB device mode when you receiv a suspend and no activite during a time
#define VOLUME_DELAY          (TIMER_S(2))    //! Display duration of the volume window in mmi application Volume 
#define EQUALIZER_DELAY       (TIMER_S(4))    //! Display duration of the equalizer window in mmi application Equalizer 
#define MODE_DELAY            (TIMER_S(4))    //! Display duration of the mode window in mmi application Mode 
#define INFO_DELAY            (TIMER_S(4))    //! Delay to switch off the Info mmi
#define STORAGE_IDLE_TIMER    (TIMER_MS(1500))//! Delay to detect U-disk idle mode
#define GLIST_TIMER    		  (TIMER_S(10))		//! Delay to detect U-disk idle mode

/********* Other time definitions ***********/
//
#define BATTERY_ANIM_TIME     (50)            //! Time for the charging battery animation (multiple of 10ms)


// Key Mnemonic Renaming
#define KBD_LOCK_SWITCH       KEY_LOCK
#define KBD_MMI_F             KEY_MENU
#define KBD_MMI_SELECT        KEY_CENTER
#define KBD_MMI_NEXT          KEY_RIGHT
#define KBD_MMI_PREV          KEY_LEFT
#define KBD_MMI_UP            KEY_UP
#define KBD_MMI_DOWN          KEY_DOWN
#define KBD_MMI_VOL_HIGH      KEY_DOWN
#define KBD_MMI_VOL_LOW       KEY_UP
#define KBD_MMI_F1            KEY_SPARE1
#define KBD_MMI_F2            KEY_SPARE2
#define KBD_MMI_REC_MIC       KEY_SPARE1
#define KBD_MMI_REC_LINE      KEY_SPARE2
#define KBD_MMI_REC_STOP      KEY_SPARE1
#define KBD_MMI_REC_STOP2     KEY_SPARE2

// PLAYER MMI
#define KBD_PLAYER_MENU       KEY_MENU
#define KBD_PLAYER_NEXT       KEY_RIGHT
#define KBD_PLAYER_PREV       KEY_LEFT
#define KBD_PLAYER_FFW        KEY_RIGHT      //! Fast Forward key (repeat press on the Next key)
#define KBD_PLAYER_FRW        KEY_LEFT       //! Fast Rewind key (repeat press on the Prev key)
#define KBD_PLAYER_PLAY_PAUSE KEY_CENTER
#define KBD_PLAYER_SELECT     KEY_CENTER
#define KBD_PLAYER_RPT_AB     KEY_SPARE1
#define KBD_PLAYER_VOL_INC    KEY_DOWN
#define KBD_PLAYER_VOL_DEC    KEY_UP
#define KBD_PLAYER_EXIT       KEY_CENTER     //! Exit key (repeat press on the Select key)
#define KBD_PLAYER_OFF        KEY_CENTER     //! Power off key (long press on the Select key)

// RADIO MMI
#define RAD_SCAN_TIME         (TIMER_MS(200)) // scan frequency refresh period

#define KBD_RAD_MENU          KEY_MENU
#define KBD_RAD_MODE          KEY_CENTER
#define KBD_RAD_NEXT_STATION  KEY_RIGHT
#define KBD_RAD_PREV_STATION  KEY_LEFT
#define KBD_RAD_VOL_HIGH      KEY_DOWN
#define KBD_RAD_VOL_LOW       KEY_UP
#define KBD_RAD_REC           KEY_SPARE1


#endif  // _CONF_MMI_H_
