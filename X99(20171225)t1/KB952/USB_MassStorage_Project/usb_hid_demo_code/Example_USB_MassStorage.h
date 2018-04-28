/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/

#ifndef _EXAMPLE_USB_STORAGE_H_
#define _EXAMPLE_USB_STORAGE_H_
//#include "config.h"
//#include "conf_kb.h"

///*
//=============================================================================
// OPTION
//=============================================================================
#define USE_NF              0           //  1:use NAND-FLASH    0:no use NADN-FLASH
#define USE_RESV_NF     	0           //  1:use Resv Area     0:no use NADN-FLASH Resv Area
#define USE_SD              1           //  1:use SD            0:no use SD
#define USE_UGP             0           //  1:use UGP           0:no use SD
//*/

#if USE_NF
    #define     USE_NF_CACHE       1   //1:use cache       0: no use
    extern void nftl_read2_init(void);
    extern int  nftl_read2(unsigned long sector, __X void* bufs);
#endif

#if USE_RESV_NF
    #define     USE_CDROM             1       // 1:CDROM disk    0:normal disk
    #define     NF_DISK_IMAGE_ID      128     // ID of disk image in Resv Area.
#endif

#if USE_SD
    #define     USE_SD_4K_CACHE          1       // 1:use 4K byte cache     0: use 2K byte cache
  #if USE_SD_4K_CACHE
     extern SDTL_ERR sdtl_usb_read_4k(unsigned long sector, __X void *buf);
     extern void sdtl_usb_cache_init(void);
     extern SDTL_ERR sdtl_usb_write(unsigned long sector, __X void *buf);
     extern int sdtl_usb_flush(void);    
  #endif
#endif    

#define  USE_TIMER          3     //  Timer : 0~3, other is disable
#define  USB_COMMIT_TIME    8     //  N/2 Sec
//=============================================================================
// Function Declare
//=============================================================================

#if  USE_SD
void MassStorage_SD_Init( void );
void MassStorage_SD_Loop( void );
void MassStorage_SD_Register( int lun );
#endif

#if  USE_NF
void MassStorage_NF_Init( void );
void MassStorage_NF_Loop( void );
void MassStorage_NF_Register( int lun );
#endif

#if  USE_RESV_NF
void MassStorage_resvNF_Init( void );
void MassStorage_resvNF_Loop( void );
void MassStorage_resvNF_Register( int lun );
#endif

#if USE_UGP
void MassStorage_ugp_Init( void );
void MassStorage_ugp_Loop( void );
void MassStorage_ugp_Register( int lun );
#endif

typedef enum
{
    USB_CONNECT         = 0,        /**< USB線拔除，USB_HelperState的初始設定 */ 
    USB_DISCONN         = 1,        /**< 由PC端強迫移除USB裝置，非拔除USB線，通常是為了可以繼續以USB充電*/
}tUSBStatus;

void        MassStorage_Init(void);         /*1. Initial */ 
void        MassStorage_Registers(void);    /*2. Register */ 
tUSBStatus  MassStorage_Loop(void);         /*3. In Loop */ 
void        MassStorage_Exit(void);         /*4. Exit */ 
void        MassStorage_Write(void);        

#endif// _EXAMPLE_USB_STORAGE_H_

//=================================================================================
// EOF
//=================================================================================


