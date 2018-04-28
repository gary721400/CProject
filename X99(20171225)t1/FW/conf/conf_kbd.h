//! @file conf_kbd.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the customer's configuration of the keyboard
//!
//! @version 1.32 snd3-dvk-1_9_5 $Id: conf_kbd.h,v 1.32 2007/06/25 12:43:02 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_KBD_H_
#define _CONF_KBD_H_


// --- Keypad Timing Configuration ---
//! keypad repeat behavior
//!           --------------------------------------------------------------> t
//! key_press ^          ^    ^    ^    ^    ^    ^    ^    ^    ^    ^    ^
//!           <--- 1 --->< 2 >< 2 >< 2 >< 2 >< 2 >< 2 >< 2 >< 2 >< 2 >< 2 ><
//!                      <----------- 3 -------------->
//! delays: 0: KBD_DEBOUNCE_TIME      debounce time (not shown)
//!         1: KBD_REPEAT_START_TIME  auto-repeat start time
//!         2: KBD_REPEAT_CONT_TIME   auto-repeat time
//!         3: KBD_REPEAT_LONG_TIME   long repeat start time
//#define KBD_DEBOUNCE_TIME     50    //!< debounce time (ms) depends on key type
//#define KBD_REPEAT_START_TIME 600   //!< first repeat time (ms)
//#define KBD_REPEAT_CONT_TIME  200   //!< time between repeat event (ms)
//#define KBD_REPEAT_LONG_TIME  4000  //!< time of long press event (ms)


// --- Keypad Layout Configuration ---
//! Define the number max of keys
//! Values:
//! - KBD_16_KEYS: 4x4 matrix         P1.0  P1.1  P1.2  P1.3
//!                                     |     |     |     |
//!                            VSS ------------------------ R0
//!                                     |     |     |     |
//!                           P1.4 ------------------------ R1
//!                                     |     |     |     |
//!                           P1.5 ------------------------ R2
//!                                     |     |     |     |
//!                           P1.6 ------------------------ R3
//!                                    C0    C1    C2    C3
//!
//! - KBD_12_KEYS: 4x3 matrix         P1.0  P1.1  P1.2  P1.3
//!                                     |     |     |     |
//!                            VSS ------------------------ R0
//!                                     |     |     |     |
//!                           P1.4 ------------------------ R1
//!                                     |     |     |     |
//!                           P1.5 ------------------------ R2
//!                                    C0    C1    C2    C3
//!
//! - KBD_10_KEYS:  2x5 matrix         P1.0  P1.1
//!                                     |     |
//!                            VSS ------------ R0
//!                                     |     |
//!                           P1.2 ------------ R1
//!                                     |     |
//!                           P1.3 ------------ R2
//!                                     |     |
//!                           P1.4 ------------ R3
//!                                     |     |
//!                           P1.5 ------------ R4
//!                                    C0    C1
//!
//! - KBD_9_KEYS:  3x3 matrix         P1.0  P1.1  P1.2
//!                                     |     |     |
//!                            VSS ------------------ R0
//!                                     |     |     |
//!                           P1.3 ------------------ R1
//!                                     |     |     |
//!                           P1.4 ------------------ R2
//!                                    C0    C1    C2
//!
//! - KBD_6_KEYS:  3x2 matrix         P1.0  P1.1  P1.2
//!                                     |     |     |
//!                            VSS ------------------ R0
//!                                     |     |     |
//!                           P1.3 ------------------ R1
//!                                    C0    C1    C2
//!
//! - KBD_4_KEYS:  2x2 matrix         P1.0  P1.1
//!                                     |     |
//!                            VSS ------------ R0
//!                                     |     |
//!                           P1.2 ------------ R1
//!                                    C0    C1
//!
//! - KBD_2_KEYS:  2x1 matrix         P1.0  P1.1
//!                                     |     |
//!                            VSS ------------ R0
//!                                    C0    C1
//!
//! - KBD_1_KEYS:  1x1 matrix         P1.0
//!                                     |
//!                            VSS ------ R0
//!                                    C0
//!
//#define KBD_USE_NB_KEYS       KBD_10_KEYS

// --- Keypad Power-On Key Configuration ---
//! Define if the on-chip DC-DC power-on key is used as function key
//! Values: - TRUE:  power-on key is used
//!         - FALSE: power-on key is not used
//#define KBD_USE_POWER_KEY     FALSE



// --- Board Keypad Configuration ---
//! Define a pre-define configuration (see below)
//! Values: - BOARD_REFD1:    Atmel Reference Design (64K Color OLED)
//!         - BOARD_USER:     User Configuration
#define  BOARD_REFD1


#if (defined BOARD_REFD1)
   // --- Keypad Decoding Policy ---
#  define KBD_DECODE_POLICY      KBD_INT_DECODE

   // --- Keypad Timing Configuration ---
#  define KBD_DEBOUNCE_TIME      50    //!< debounce time (ms) depends on key type
#  define KBD_REPEAT_START_TIME  400   //!< first repeat time (ms)
#  define KBD_REPEAT_CONT_TIME   100   //!< time between repeat event (ms)
#  define KBD_REPEAT_LONG_TIME   2000  //!< time of long press event (ms)

   // --- Keypad Layout Configuration ---
#  define KBD_USE_NB_KEYS        KBD_10_KEYS//KBD_9_KEYS

   // --- Keypad Power-On Key Configuration ---
#  define KBD_USE_POWER_KEY      FALSE

   // --- Standart Key Renaming ---
#  define KEY_MENU               KEY_R4_C0//KEY_R0_C0
#  define KEY_RIGHT              KEY_R1_C1//KEY_R1_C2
#  define KEY_UP                 KEY_R0_C1//KEY_R0_C2
#  define KEY_CENTER             KEY_R2_C1//KEY_R1_C1
#  define KEY_DOWN               KEY_R4_C1//KEY_R2_C0
#  define KEY_LEFT               KEY_R3_C1//KEY_R1_C0
#  define KEY_SPARE1             KEY_R3_C0//KEY_R0_C1
#  define KEY_DEC                KEY_R1_C0//KEY_R2_C1   // does not exist
#  define KEY_INC                KEY_R2_C0//KEY_R2_C2   // does not exist


#elif (defined BOARD_USER)
   // --- Keypad Decoding Policy ---
#  define KBD_DECODE_POLICY      KBD_???_DECODE

   // --- Keypad Timing Configuration ---
#  define KBD_DEBOUNCE_TIME      ??    //!< debounce time (ms) depends on key type
#  define KBD_REPEAT_START_TIME  ???   //!< first repeat time (ms)
#  define KBD_REPEAT_CONT_TIME   ???   //!< time between repeat event (ms)
#  define KBD_REPEAT_LONG_TIME   ???   //!< time of long press event (ms)

   // --- Keypad Layout Configuration ---
#  define KBD_USE_NB_KEYS        KBD_??_KEYS

   // --- Keypad Power-On Key Configuration ---
#  define KBD_USE_POWER_KEY      ?????

   // --- Standart Key Renaming ---
#  define KEY_LOCK               KEY_R?_C?
#  define KEY_MENU               KEY_R?_C?
#  define KEY_RIGHT              KEY_R?_C?
#  define KEY_UP                 KEY_R?_C?
#  define KEY_CENTER             KEY_R?_C?
#  define KEY_DOWN               KEY_R?_C?
#  define KEY_LEFT               KEY_R?_C?
#  define KEY_INC                KEY_R?_C?
#  define KEY_DEC                KEY_R?_C?


#else
#  error Keypad configuration must be defined in conf_kbd.h
#endif

#endif  // _CONF_KBD_H_
