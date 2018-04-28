/******************************************************************************
** Copyright (c) 2009
** King Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#define FAT_VARIABLE
#include "fsconfig_KBDM9_internal.h"


//-------------------------------------------
#pragma DATA = FS_DATA
//-------------------------------------------
tStorageBusyCallBackFunc pStorageBusyCallBackFunc;
unsigned int STORAGEFLAGS;   
unsigned int PROTECTFLAGS;   
char active_drive;
FCFILE _fslots[FOPEN_MAX];
int drv_count;
int max_drv_count;
struct _date_n_time fs_date_time;
struct _drive _drives_xram[4];
//=============================================================================
// END OF FILE
//=============================================================================