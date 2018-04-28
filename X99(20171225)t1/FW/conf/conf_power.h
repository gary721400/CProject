//! @file conf_power.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the power management configuration
//! This file will be given to any external customer
//!
//! @version 1.14 snd3-dvk-1_9_5 $Id: conf_power.h,v 1.14 2007/04/19 14:13:09 coger Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_POWER_H_
#define _CONF_POWER_H_

//#include "conf\conf_kbd.h"
#include "modules\power\power.h"             // Power module definition

//! --- Power Source Configuration ---
//! Values: - INTERNAL_VLV_DC_DC             1.8V internal DC-DC system power
//!         - EXTERNAL_VLV_REGULATOR         1.8V external source system power
//!         - EXTERNAL_LV_DC_DC              3V external DC-DC system power
//!         - EXTERNAL_LV_REGULATOR          3V external source system power
#define POWER_SOURCE       EXTERNAL_LV_REGULATOR
//#define POWER_SOURCE       EXTERNAL_LV_DC_DC

//! --- External DC-DC ON I/O signal ---
//! Values: - I/O pin asserted low
//#define POWER_EXT_DC_ON    P3_4              // exemple

//! --- Automatic Power Type Configuration ---
#if (POWER_SOURCE == INTERNAL_VLV_DC_DC) || (POWER_SOURCE == EXTERNAL_VLV_REGULATOR)
#  define   POWER_TYPE     POWER_VERY_LOW_VOLTAGE
#else
#  define   POWER_TYPE     POWER_LOW_VOLTAGE
#endif

//! --- Enable the auto (delayed) power off ---
//! Values: - ENABLE
//!         - DISABLE
#define POWER_OFF_AUTO              ENABLE

//! --- Battery scan period ---
//! Value in seconds
#define POWER_BATTERY_SCAN_PERIOD   5

//! --- Battery alert level---
//! Value : 0..16
#define POWER_BATTERY_LEVEL_ALERT   0

//_____ S E L E C T I O N   O F   B A T T E R Y   C H A R G E R ______
//! Define here one of the battery charger used in your current application

//! Label definitions of the battery chargers being supported in this project
//! (Define here the label of your new battery charger to support with a different value)
#define BAT_CHARGER_NONE            (0)
#define BAT_CHARGER_ILS6294         (1)

//! Select the battery charger to implement in your project code
//! (For this, use one of the label defined just above)
#define BAT_CHARGER_PART_NUMBER   BAT_CHARGER_NONE
//#define BAT_CHARGER_PART_NUMBER		BAT_CHARGER_ILS6294

//! Battery charger files to include according to the choice made just above
#if   (BAT_CHARGER_PART_NUMBER == BAT_CHARGER_NONE)
//#  define BAT_CHARGER_INCLUDE_FILE_NAME       (include definition ignored in this case)
#elif( BAT_CHARGER_PART_NUMBER == BAT_CHARGER_ILS6294 )
#  define BAT_CHARGER_INCLUDE_FILE_NAME      "drivers\bat\bat_isl6294.h"
#else
#  error Battery charger part number must be defined in conf_power.h
#endif

//! Define the level value from which the battery is fully charge
//! (Only for application having no charger IC: diode+resistor solution)
#if (BAT_CHARGER_PART_NUMBER == BAT_CHARGER_NONE)
#  define BATTERY_FULL_LEVEL   10
#endif

#endif  // _CONF_POWER_H_

