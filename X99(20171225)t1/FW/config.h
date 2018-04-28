//! @file config.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the system configuration definition
//!
//! @version 2.6 snd3-dvk-1_9_5 $Id: config.h,v 2.6 2007/11/22 17:09:09 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define KEIL
//#define SND3B

//_____ I N C L U D E S ____________________________________________________

#include "lib_mcu\compiler.h"                // Compiler definitions
//#include "lib_mcu\mcu_drv.h"                 // Component declaration
#include "SDImg.h"
#include "variable.h"

//_____ M A C R O S ________________________________________________________
#define UNICODE_TYPE  ENABLE
//#define UNICODE_TYPE  DISABLE


//#define ASCII_TYPE  	ENABLE
//#define ASCII_TYPE  	DISABLE

//#define ATMEL_WARNING    // Define it to display ATMEL warnings

// This flag autorize the quick startup
//#define START_QUICK_MODE

//#define  AP4_DIR        "Ap4"
//#define  MP3_DIR        "Mp3"
//#define  AP4_EXT        "AP4"

#define  AP4_EXT        "XDF"
#define  MP3_EXT        "MP3"
#define  RECORD_EXT     "WAV"
#define  DIC_EXT        "ADB"

#define  BOOK_SYS_FILE 	"0:\\BOOK.SYS"

#if (UNICODE_TYPE == ENABLE )
#define  AP4_DIR        "点读文件"
#define  MP3_DIR        "音乐文件"
#define  RECORD_DIR     "录音文件"
#define  PASTER_DIR     "Rec"
#define  DIC_DIR        "词库文件"


#define  SUBDIR_PATH	"0:\\系统文件\\"
#define  AP4_PATH		"0:\\点读文件\\"
#define  MP3_PATH		"0:\\MP3文件\\"
#define  RECORD_PATH	"0:\\录音文件\\"
#define  PASTER_PATH	"0:\\Rec\\"
#define  DIC_PATH		"0:\\词典文件\\"

#define  DEFAULT_DIC_FILE 	"0:\\词典文件\\default.adb"
#endif
#if (UNICODE_TYPE == DISABLED )
#define  AP4_DIR        "Books"
#define  MP3_DIR        "Music"
#define  RECORD_DIR     "Recorder"
#define  PASTER_DIR     "Rec"
#define  DIC_DIR        "Dic"



#define  SUBDIR_PATH	"0:\\system\\"
#define  AP4_PATH		"0:\\Books\\"
#define  MP3_PATH		"0:\\Music\\"
#define  RECORD_PATH	"0:\\Recorder\\"
#define  PASTER_PATH	"0:\\Rec\\"
#define  DIC_PATH		"0:\\DIC\\"

#define  DEFAULT_DIC_FILE 	"0:\\DIC\\default.adb"
#endif

#define  DIY_REC_DIR     "0:\\DIYREC\\"


#define  VOL_LEVEL        8//5//




#define  LOW_BAT_WARNING	"batwarn.mp3"
#define  REPEAT_MP3			"repeat.mp3"
#define  DIANDU_MP3			"simple.mp3"

#define SYS_LAB			"X99_"

#define HARD_VERSION              "1"
#define MAJOR_FW_VERSION          "0"
#define MINOR_FW_VERSION          "5"
//#define OID_TYPE_VERSION          '3'
#define OID_TYPE_VERSION          "5"

#define H_YEAR					  '1'
#define L_YEAR					  '5'
#define H_MONTH					  '0'
#define L_MONTH					  '8'
#define H_DAY					  '1'
#define L_DAY					  '0'
// Work package version
//#define  WP_NAME                       "V4 V1.0.1.2"
//#define  VERSION_NAME                  "1.0.1.2"
#define VERSION_NAME			  "1.0.5.5"
// USB
#define USB_NO_INTERRUPT (DISABLE)

#define _MEM_TYPE_MALLOC_MAIL_   _MEM_TYPE_SLOW_


#define ISP_VERSION              0x05


#define SAVE_OID_CAL_ADDR		(4084*1024UL)//(4086*1024UL)//(4184064UL)//
//#define SAVE_OID_CAL_ADDR		(1040384UL)
//#define SAVE_OID_CAL_ADDR		(2024UL)

#define HARD_V1_VERSION		(0X01)
#define HARD_V2_VERSION		(0X02)


// To check if update file is compliant with this design (during update task)
// !!! Warning !! device type 0x00000000 is reserved. Please do not use this device type number
#define DEVICE_TYPE_MSW       0x1234
#define DEVICE_TYPE_LSW       0x5678
#define DEVICE_TYPE           ((((U32)DEVICE_TYPE_MSW)<<16) + (U32)DEVICE_TYPE_LSW)  


//! Periode of the tick in ms
//! Here the tick is set to 2ms
#define TICK_PERIOD              2


//! Number of timers software available
#define NB_TIMER_SOFT            10

//! Number of graphical objects available in auto display
//! Warning : each object requires a timer soft for running.
#define NB_AD_OBJ                5


//! Number of MMI applications being able to be processed at the same time
#define MMGR_KERNEL_STACK_SIZE   8


// keypad configuration
// --------------------
//! Define if keypad is used for power-down mode exit
//! Values: - TRUE:  power-down mode exit by keypad enabled
//!         - FALSE: power-down mode exit by keypad disabled
#define KBD_POWER_DOWN_EXIT      FALSE


//***********************************************
#define	SIZE_OF_ITEM( a )             ( sizeof( a )/ sizeof(( a )[ 0 ]))
//***********************************************
//_____ I N C L U D E S ____________________________________________________

//! INCLUDE PATH and compile options (don't touch)
#define LIB_MEM         "lib_system\memory\memory.h"
#define LIB_CTRLACCESS  "modules\control_access\ctrl_access.h"

#include "option.h"



#endif  //! _CONFIG_H_

