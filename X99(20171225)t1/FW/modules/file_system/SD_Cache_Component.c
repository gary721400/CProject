/******************************************************************************
** Copyright (c) 2009
** King Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
//-------------------------------------------
//-------------------------------------------
#include "fsconfig_KBDM9_internal.h"

int SD_Cache_Clear(void);
int SD_Cache_read(unsigned long sector,unsigned char *buffer);
int SD_Cache_write(unsigned long sector,unsigned char *buffer);
static FS_ERR sdtl_cache_ini(void);

//=============================================================================
//  FAT_SD_BUF
//=============================================================================
extern __X unsigned char fsbuffer_SD[SECTOR_SIZE];
extern __X FSINFO _SD_fsi;
//-------------------------------------------
#pragma CODE = FSSD_INIT_CODE
//-------------------------------------------
const tFSDrvHandle SD_Cache_Drive = {{ SD_Cache_Clear,
                                 SD_Cache_read,
                                 SD_Cache_write,
                                 sdtl_get_size,
                                 sdtl_cache_flush,
                                 fsbuffer_SD,
                                 0L,
                                 0,
                                 &_SD_fsi
                               },
                               sdtl_cache_ini
                              };

void _SetFlashBusy(void);
void _ClearFlashBusy(void);

static FS_ERR sdtl_cache_ini(void)
{

    SetVector(SET_FLASH_BUSY_TRAP, _SetFlashBusy);
    SetVector(CLEAR_FLASH_BUSY_TRAP,_ClearFlashBusy);

  if( 0 != sdtl_mount() )
     return FS_DRIVE_ERROR;

  sdtl_cache_init();
	return FS_NO_ERROR;
}

//-------------------------------------------
#pragma CODE = FSSD_DRIVER_CODE
//-------------------------------------------
int SD_Cache_Clear(void)
{
    return 0;
}

int SD_Cache_read(unsigned long sector,unsigned char *buffer)
{
    return sdtl_cache_read(sector, buffer);
}

int SD_Cache_write(unsigned long sector,unsigned char *buffer)
{
    return sdtl_cache_write(sector, buffer);
}

//=============================================================================
// END OF FILE
//=============================================================================