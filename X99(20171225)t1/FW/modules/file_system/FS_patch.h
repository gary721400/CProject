/*
 * FS_patch.h
 *
 * Description: 
 *
 * Author: Nick Lee <linin@kingb.com.tw>
 * Copyright(c)2010 King Billion electronics Co., Ltd.<http://www.kingb.com.tw/>
 * All rights reserved.
 *
 * Modifications:
 *
 */

/* Includes ------------------------------------------------------------------*/
#include <KBDM9_DDK.H>
#include <string.h>

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
Bool     FSPatchInit             ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry );
//FS_ERR   FSGotoEntry_u           ( unsigned short *spec, unsigned int index, __X FILE_INFO *finfo, __X FS_ENTRY_INFO *entry_info );
//FS_ERR   FSGotoEntry             ( char *spec, unsigned int index, __X FILE_INFO *finfo, __X FS_ENTRY_INFO *entry_info );
FS_ERR   FSNextEntry             ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry );
FS_ERR   FSPrevEntry             ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry );
FS_ERR   FSParentEntry           ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry );
FS_ERR   FSSubdirEntry           ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry );
FS_ERR   FSSeekEntry             ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int index );
FCFILE   *FSOpenEntry            ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int mode, __X FS_ERR *err );
FCFILE   *FSOpenEntryForFastSeek ( __X FILE_INFO *finfo, __X FS_ENTRY_INFO *fentry, unsigned int mode, unsigned int *cache_buf, unsigned int buf_size, __X FS_ERR *err );
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* End of file ---------------------------------------------------------------*/