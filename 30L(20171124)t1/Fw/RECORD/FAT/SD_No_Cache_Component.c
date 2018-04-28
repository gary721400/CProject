/******************************************************************************
** Copyright (c) 2009
** King Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
//-------------------------------------------
//-------------------------------------------
#include "fsconfig_KBDM9_internal.h"

int SDClear(void);
int SD_read(unsigned long sector,unsigned char *buffer);
int SD_write(unsigned long sector,unsigned char *buffer);
static FS_ERR sdtl_mount_ini(void);
extern void SD_delay_callback(void);

//=============================================================================
//  FAT_SD_BUF
//=============================================================================
extern __X unsigned char fsbuffer_SD[SECTOR_SIZE];
extern __X FSINFO _SD_fsi;
//-------------------------------------------
#pragma CODE = FAT_TEMP_CODE17
//-------------------------------------------
const tFSDrvHandle SD_Drive = {{ SDClear,
                                 SD_read,
                                 SD_write,
                                 sdtl_get_size,
                                 sdtl_flush,
                                 fsbuffer_SD,
                                 0L,
                                 0,
                                 &_SD_fsi
                               },
//                               SD_DELAY_CALLBACK_TRAP,
//                               SD_delay_callback,
                               sdtl_mount_ini
                              };

void _SetFlashBusy(void);
void _ClearFlashBusy(void);

static FS_ERR sdtl_mount_ini(void)
{
    SetVector(SET_FLASH_BUSY_TRAP, _SetFlashBusy);
    SetVector(CLEAR_FLASH_BUSY_TRAP,_ClearFlashBusy);
    if( 0 != sdtl_mount() )
        return FS_DRIVE_ERROR;
	return FS_NO_ERROR;
}

//-------------------------------------------
#pragma CODE = FAT_COMMON_CODE
//-------------------------------------------
int SDClear(void)
{
    return 0;
}

int SD_read(unsigned long sector,unsigned char *buffer)
{
    return sdtl_read(sector, buffer);
}

int SD_write(unsigned long sector,unsigned char *buffer)
{
    return sdtl_write(sector, buffer);
}

//=============================================================================
// END OF FILE
//=============================================================================