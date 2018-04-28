/******************************************************************************
** Copyright (c) 2009
** King Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/

#include <string.h>
#include "fsconfig_KBDM9_internal.h"

//=============================================================================
// VERSION
//=============================================================================


//=============================================================================
//  FS_DATA
//=============================================================================
extern __X tStorageBusyCallBackFunc pStorageBusyCallBackFunc;
extern __X unsigned int STORAGEFLAGS; 
extern __X unsigned int PROTECTFLAGS  ;
extern __X char active_drive;
extern __X FCFILE _fslots[];
extern __X int drv_count;
extern __X int max_drv_count;

/******************************
*   Decalrations
******************************/
extern __X struct _drive * __X gdrv;
extern __X FCFILE* __X gfslot;
extern __X struct _date_n_time fs_date_time;
extern int FileSystemSuspend( unsigned char drive );
extern int ReadFileSystemInfo( unsigned char drive );
extern int IsWriteProtect(int drive);
extern void SetWriteProtect(int drive);
extern void ClearWriteProtect(int drive);
//================================================================
//      FS_TEMP_CODE
//================================================================
#pragma CODE = FAT_TEMP_CODE03

int FSCloseAll(void)
{
    int i, err=0;
    for( i=0; i<FOPEN_MAX; ++i)
    {
        if( -1 != _fslots[i].drive && 0 != _fslots[i].used )
        {
            err |= FSClose( &(_fslots[i]) );
        }
    }
    return err;
}

/******************************
*   Platform Dependent Functions
******************************/

//================================================================
//      FS_COMMON_CODE
//================================================================
#pragma CODE = FAT_COMMON_CODE

int IsWriteProtect(int drive)
{
    return PROTECTFLAGS&(1<<drive);
}

void _IsWriteProtect(void);
__asm{
__IsWriteProtect::    
    SJMP _IsWriteProtect
}

int ChangeDrive(unsigned char drive)
{
    if( drive >= max_drv_count ) {
        return -1;
    }

    active_drive = (char)drive;
    return 0;
}

void _ChangeDrive(void);
__asm{
    __ChangeDrive::
        SJMP _ChangeDrive
}

void GetDateNTimeFunc(struct _date_n_time *p)
{
    nearcpy( p, &fs_date_time, sizeof(fs_date_time) );
//    p->year = (2009-1980);   // >1980
//    p->month = 6;
//    p->day = 2;
//    p->hour = 14;
//    p->minute = 40;
//    p->second = 15;
}

void _GetDateNTime(void);
__asm{
    __GetDateNTime::
        SJMP _GetDateNTimeFunc
}

int IsFileSystemBusy(int drive)
{
    if(STORAGEFLAGS & (FS_BUSY_FLAG|FLASH_BUSY_FLAG|BUS_BUSY_FLAG))
        return 1;
    else
        return 0;
}

void _SetFlashBusy(void)
{
    STORAGEFLAGS |= FLASH_BUSY_FLAG;
}

void _ClearFlashBusy(void)
{
    STORAGEFLAGS &= (~FLASH_BUSY_FLAG);
}

void _SetFileSystemBusy(void)
{
    STORAGEFLAGS |= FS_BUSY_FLAG;
}

int IsFlashBusy(void)
{
    if(STORAGEFLAGS & FS_BUSY_FLAG)
        return 1;
    else
        return 0;
}

void _ClearFileSystemBusy(void)
{
    STORAGEFLAGS &= (~FS_BUSY_FLAG);
}


int IsStorageBusy(void)
{
    if(STORAGEFLAGS & BUS_BUSY_FLAG)
        return 1;
    else
        return 0;
}

void SetStorageBusy(void)
{
    STORAGEFLAGS |= BUS_BUSY_FLAG;
}

void Null_Func(void);
void ClearStorageBusy(void)
{
    STORAGEFLAGS &= (~BUS_BUSY_FLAG);
    if(pStorageBusyCallBackFunc != Null_Func)
        pStorageBusyCallBackFunc();
    pStorageBusyCallBackFunc = Null_Func;
}

void SetStorageBusyCallBackFunc(tStorageBusyCallBackFunc pFuncPoint)
{
    if(STORAGEFLAGS & BUS_BUSY_FLAG)
        pStorageBusyCallBackFunc = pFuncPoint;
}

__asm{
_Null_Func::
    RETS
}

//================================================================
//  FS_INIT_CODE
//================================================================
#pragma CODE = FAT_TEMP_CODE17
void FSInit_(int MaxDrvCount)
//void FSInit(__X struct _drive *pDrvSpace, int MaxDrvCount)
{
    int i;
    __X struct _fcb *next;

    active_drive = -1;
    SetVector(Set_File_System_Busy,_SetFileSystemBusy ); 
    SetVector(Clr_File_System_Busy,_ClearFileSystemBusy ); 
    SetVector(Is_CWrite_Protect,_IsWriteProtect ); 
    SetVector(GET_DATE_NTIME,_GetDateNTime );
    SetVector(I_Change_Drive,_ChangeDrive ); 
    
    pStorageBusyCallBackFunc = Null_Func;
    SetVector(STORAGE_SET_BUSY,SetStorageBusy);
    SetVector(STORAGE_CLR_BUSY,ClearStorageBusy);

    gfslot = &_fslots[0];
    
    for(i = FOPEN_MAX - 1,next = (__X struct _fcb *)NULL; i >= 0; i--) {
        _fslots[i].next = next;
        _fslots[i].used = 0;
        _fslots[i].drive = -1;
        next = &_fslots[i];
    }
    gdrv = &_drives_xram[0];
    drv_count = 0;
    STORAGEFLAGS = PROTECTFLAGS = 0;
    max_drv_count = MaxDrvCount;
    fs_date_time.year = (2009-1980);   // >1980
    fs_date_time.month = 9;
    fs_date_time.day = 1;
    fs_date_time.hour = 12;
    fs_date_time.minute = 00;
    fs_date_time.second = 00;    
}

FS_ERR FS_mount_(const tFSDrvHandle *pdrive)
{
    FS_ERR fserr;
    
    
    if(ChangeDrive(drv_count))
        return FS_DRIVE_ERROR;
    gdrv[drv_count] = pdrive->Drv;
    //SetVector((*pdrive).TrapNo,pdrive->pFunc);
    gdrv[drv_count].buffer_status = EMPTY;
    memset( gdrv[drv_count].fsinfo, 0, sizeof( FSINFO ) );
    fserr = (pdrive->pMountFunc)();
    if(fserr != FS_NO_ERROR)
        return fserr;
    return ReadFileSystemInfo(drv_count++);
}

#pragma CODE = FAT_COMMON_CODE

ImageDeclareType(FAT_TEMP_CODE17);
void FSInit(int MaxDrvCount)
//void FSInit(__X struct _drive *pDrvSpace, int MaxDrvCount)
{
    SDKLoadCode(FAT_TEMP_CODE17);
    FSInit_( MaxDrvCount );
}

FS_ERR FS_mount(const tFSDrvHandle *pdrive)
{
    SDKLoadCode(FAT_TEMP_CODE17);
    return FS_mount_(pdrive);
}

//=============================================================================
// END OF FILE
//=============================================================================