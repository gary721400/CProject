/******************************************************************************
** Copyright (c) 2009
** King Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
//-------------------------------------------
//-------------------------------------------
#include "fsconfig_KBDM9_internal.h"

int NandClear(void);
int NF_read(unsigned long sector,unsigned char *buffer);
int NF_write(unsigned long sector,unsigned char *buffer);
static FS_ERR nftl_mount_ini(void);
extern void NFC_delay_callback(void);

//=============================================================================
//  FAT_NF_BUF
//=============================================================================
extern __X unsigned char fsbuffer_NF[SECTOR_SIZE];
extern __X FSINFO _NF_fsi;

//-------------------------------------------
#pragma CODE = FAT_TEMP_CODE17
//-------------------------------------------
const tFSDrvHandle NF_Drive = {{ NandClear,
                                 NF_read,
                                 NF_write,
                                 nftl_get_size,
                                 nftl_flush,
                                 fsbuffer_NF,
                                 0L,
                                 0, 
                                 &_NF_fsi
                               },
//                               NFC_DELAY_CALLBACK_TRAP,
//                               NFC_delay_callback,
                               nftl_mount_ini
                              };

void _SetFlashBusy(void);
void _ClearFlashBusy(void);

static FS_ERR nftl_mount_ini(void)
{
    SetVector(SET_FLASH_BUSY_TRAP, _SetFlashBusy);
    SetVector(CLEAR_FLASH_BUSY_TRAP,_ClearFlashBusy);
    return FS_NO_ERROR;
}

//-------------------------------------------
#pragma CODE = FAT_COMMON_CODE
//-------------------------------------------
int NandClear(void)
{
    return 0;
}

int NF_read(unsigned long sector,unsigned char *buffer)
{
    return nftl_read(sector,buffer);
}

int NF_write(unsigned long sector,unsigned char *buffer)
{
    return nftl_write(sector,buffer);
}
//=============================================================================
// END OF FILE
//=============================================================================