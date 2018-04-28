//! @file conf_update.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the update
//! This file will be given to any external customer
//!
//! @version 1.5 snd3-dvk-1_9_5 $Id: conf_update.h,v 1.5 2006/12/01 14:55:45 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_UPDATE_H_
#define _CONF_UPDATE_H_

//_____ F E A T U R E   C O N F I G U R A T I O N __________________________

//! --- Update feature control ---
//! Values: - ENABLE:  global update feature enable
//!         - DISABLE: global update feature disable
//#define FUNC_UPDATE              ENABLE
#define FUNC_UPDATE              DISABLE

#if (FUNC_UPDATE == ENABLE)

//! --- Auto update feature control ---
//! Start update when find an update file on the disk
//! Values: - ENABLE:  automatic start update at power-on
//!         - DISABLE: no auto-start
#define UPDATE_AUTO_UPDATE       ENABLE
                                                            
//! --- Erase update file after update ---
//! Values: - ENABLE:  update file erased after update
//!         - DISABLE: update file unchanged after update
//! Note: In order to avoid update at each power-up UPDATE_ERASE_UP_FILE
//!       is over-defined to ENABLE in case of auto-update
#define UPDATE_ERASE_UP_FILE     ENABLE

//! --- Create update directories if any ---
//! Values: - ENABLE:  Directories will be created if not present
//!         - DISABLE: Directories will not be created
//! Note: This option is available in auto update mode only
#define UPDATE_CREATE_DIRECTORY  DISABLE

//_____ U P D A T E   F I L E   C O N F I G U R A T I O N __________________

// Define the default LUN for the update file
//! Values: - LUN_ID_NF_DISKMASS: Nand Flash select
//!         - LUN_ID_MMC: MMC/SD Select
#define UPFILE_LUN               LUN_ID_NF_DISKMASS

//! --- Update file path ---
//! Value: - char table
//! Example: path = \ATMEL\Update\<Update_file>:
//! {{'A','T','M','E','L','\0'},{'U','p','d','a','t','e','\0'}}
#define UNICODENAME_UPDATE_FILE_DIRECTORY {'U','p','d','a','t','e','\0'}

//! --- Update file path size ---
//! Value: - number of directory in the path
//! Example: path = \ATMEL\Update\<Update_file>: 2
#define UPFILE_NB_DIRECTORY      1

//! --- Update file name ---
//! Value: - char table
//! Example: name = Player_snd3:
//! {'P','l','a','y','e','r','_','s','n','d','3'}
#define UNICODENAME_UPDATE_FILE_NAME      {'P','l','a','y','e','r','_','d','v','k','.','a','t','m','\0'}

//! --- Update file extention for explorer filtering ---
//! Value: - 3 ascii char string
#define UPDATE_FILE_EXT                   {"atm"}

# else
#  define UPDATE_AUTO_UPDATE              DISABLE
#  define UPDATE_ERASE_UP_FILE            DISABLE
# endif //#if (FUNC_UPDATE == ENABLE)

#endif   // _CONF_UPDATE_H_

