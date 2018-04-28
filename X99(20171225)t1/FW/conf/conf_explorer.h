//! @file conf_explorer.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the explorer
//! This file will be given to any external customer
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: conf_explorer.h,v 2.0 2007/03/30 12:26:45 sguyon Exp $
//!
//! @todo
//! @bug

#include "config.h"

#ifndef _CONF_EXPLORER_H_
#define _CONF_EXPLORER_H_

//! FAT supported (ENABLED or DISABLE)
#define  FS_FAT_12   ENABLED
#define  FS_FAT_16   ENABLED
#define  FS_FAT_32   ENABLED

//! The explorer may be support the string in format ASCII or UNICODE, or the both
#define  FS_ASCII    ENABLED
#define  FS_UNICODE  ENABLED

//! The navigator may be support only the first partition (DISABLED), or the multiple partion (ENABLED)
#define  FS_MULTI_PARTITION    DISABLED

//! Level of feature
//! Choose between:
//! FSFEATURE_READ                  All read fonctions
//! FSFEATURE_WRITE                 nav_file_copy(), nav_file_paste(), nav_file_del(), file_create(), file_open(MODE_WRITE), file_write(), file_putc()
//! FSFEATURE_WRITE_COMPLET         FSFEATURE_WRITE + nav_drive_format(), nav_dir_make(), nav_file_rename(), nav_file_dateset(), nav_file_attributset()
//! FSFEATURE_ALL
#define  FS_LEVEL_FEATURES             (FSFEATURE_READ | FSFEATURE_WRITE_COMPLET)

//! Number of cache used to store a cluster list of files (interresting in case of many "open file")
//#define  FS_NB_CACHE_CLUSLIST 1  // In player mode, 1 is OK (shall be > 0)
//#define  FS_NB_CACHE_CLUSLIST 16  // In player mode, 1 is OK (shall be > 0)

//! 2009.11.09 wangwei 加入FS_USE_CLUSTER_LIST_OPTIMIZATION这个编译选项，设为TRUE可以
//! 支持同时操作多个簇链，优化大文件多碎片情况下的性能，
//! 设为FALSE就取消优化，使用老的代码。这个优化是有限的，
//! 如果文件碎片过多，还是有严重的性能问题；
#define  FS_USE_CLUSTER_LIST_OPTIMIZATION       TRUE
//#define  FS_USE_CLUSTER_LIST_OPTIMIZATION       FALSE

//! Maximum of navigator
//#define FS_NB_NAVIGATOR           3
#define FS_NB_NAVIGATOR           1//4

//! *** Define the affiliation of navigator (Rq: the explorer has always the navigator ID 0)
// The play list use the navigator ID 1
#define  FS_NAV_ID_PLAYLIST   1
// The explorer use the navigator ID 2 to open the "copy file" and ID 0 to open the "paste file"
// Note: Play list use the copy file to restore the modifications of list, so FS_NAV_ID_PLAYLIST != FS_NAV_ID_COPYFILE
#define  FS_NAV_ID_COPYFILE   2

//***********************************************
//add by xuanyu, used in paster and ap3
#define FS_NAV_ID_USER            3
#define FS_NAV_ID_AP3             3
//***********************************************

// The karaoke and ID3 use a different navigator (2) than player explorer or playlist, because it may be enabled in same time
#define  FS_NAV_ID_ID3        2
#define  FS_NAV_ID_KARAOKE    2
// The update and status file use a different navigator (2) because they are enabled in same time 
#define  FS_NAV_ID_UPDATEFILE 1  // Define the default navigator used to open the default update file
#define  FS_NAV_ID_STATUSFILE 2


//! *** PLAY LIST configuration ****

// Size of cache to store the path of a file (here, is mapped on a global buffer)
#include "modules\control_access\ctrl_access.h"
//#define  PL_CACHE_PATH_MAX_SIZE     sizeof(g_trans_buffer)
//#define  pl_cache_path              g_trans_buffer

//! Play list in format ASCII or UNICODE
#define  PL_UNICODE  DISABLED  // ENABLED to UNICODE,  DISABLE to ASCII
//#define  PL_UNICODE  ENABLED  // ENABLED to UNICODE,  DISABLE to ASCII

//****** END OF play list configuration ****



//! *** EXPLORER API configuration ****

// Number maximum of file include in each random (Unit 8 files)
#define  EXP_MAX_RANGE_RAND      2       // 16 files
#define  EXP_GET_RAND( value )   {value=TL0%(EXP_MAX_RANGE_RAND*8); }

//****** END OF explorer API configuration ****



//! *** SETTING ( = customer area) configuration ****

// Setting area management : FAT (ENABLED) or CUSTOMER structure (DISABLED)
#define SETTING_AREA_FAT           DISABLED
//#define SETTING_AREA_FAT           ENABLED
// Setting area autorize user access via explorer MMI
#define SETTING_AREA_ACCESS_NAV    DISABLED
// Setting area autorize user access via USB mass-storage
#define SETTING_AREA_ACCESS_USB    DISABLED
// Setting area size (unit Number of sector - 512B)
#if (SETTING_AREA_FAT == ENABLED)
#  define SETTING_AREA_SIZE     (2*1024L*1024L/512)   // disk of 2MB
#else
#  define SETTING_AREA_SIZE     1
#endif

#define  SETTING_CASE_SENSITIVE     FALSE     // For setting path search (TRUE = match case)
#define  SETTING_STRING_UNICODE     FALSE     // String format used

#if (SETTING_AREA_FAT == ENABLED)
#  if (SETTING_STRING_UNICODE == FALSE)
#     define   FILE_SETTING         "settings.dat"
#  else
#     define   FILE_SETTING         {'s','e','t','t','i','n','g','s','.','d','a','t','\0'}
#  endif
#endif

#if (SETTING_STRING_UNICODE == FALSE)
#  define      UPDATE_SETTING_PATH  "C:\\update\\settings.dat"
#else
#  define      UPDATE_SETTING_PATH  {'C',':','\\','u','p','d','a','t','e','\\','s','e','t','t','i','n','g','s','.','d','a','t','\0'}
#endif

//****** END OF setting configuration ****


#endif  //! _CONF_EXPLORER_H_

