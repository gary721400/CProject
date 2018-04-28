/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/

#ifndef _USB_STORAGE_H_
#define _USB_STORAGE_H_

//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//# error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
#define USE_NF                0           //  1:use NAND-FLASH    0:no use NADN-FLASH
#define USE_RESV_NF           0           //  1:use Resv Area     0:no use NADN-FLASH Resv Area
#define USE_SD                1           //  1:use SD            0:no use SD
#define USE_UGP               0           //  1:use UGP           0:no use SD

#if USE_NF
   #define  USE_NF_CACHE      1           //1:use cache       0: no use
   extern   void  nftl_read2_init   ( void );
   extern   int   nftl_read2        ( unsigned long sector, __X void *bufs );
#endif

#if USE_RESV_NF
   #define  USE_CDROM         1           // 1:CDROM disk    0:normal disk
   #define  NF_DISK_IMAGE_ID  128         // ID of disk image in Resv Area.
#endif

#if USE_SD
   #define  USE_SD_8K_CACHE   1           // 1:use 8K byte cache     0: use 4K byte cache
   #define  USE_SD_4K_CACHE   0           // 1:use 4K byte cache     0: use 2K byte cache
   #define  USE_SD_CACHE      1           // 1:use cache     0:no use
#endif    

#define  USE_TIMER            0           //  Timer : 0~3, other is disable
#define  USB_COMMIT_TIME      8           //  N/2 Sec
//=============================================================================
// Function Declare
//=============================================================================

#if USE_SD
   void  MassStorage_SD_Init           ( void );
   void  MassStorage_SD_Loop           ( void );
   void  MassStorage_SD_Register       ( int lun );
#endif

#if USE_NF
   void  MassStorage_NF_Init           ( void );
   void  MassStorage_NF_Loop           ( void );
   void  MassStorage_NF_Register       ( int lun );
#endif

#if  USE_RESV_NF
   void  MassStorage_resvNF_Init       ( void );
   void  MassStorage_resvNF_Loop       ( void );
   void  MassStorage_resvNF_Register   ( int lun );
#endif

#if USE_UGP
   void  MassStorage_ugp_Init          ( void );
   void  MassStorage_ugp_Loop          ( void );
   void  MassStorage_ugp_Register      ( int lun );
#endif

typedef enum
{
   USB_CONNECT = 0,        /**< USB线拔除，USB_HelperState的初始设定 */ 
   USB_DISCONN = 1,        /**< 由PC端强迫移除USB装置，非拔除USB线，通常是为了可以继续以USB充电*/
}tUSBStatus;

void        MassStorage_Init        ( void );   /*1. Initial */ 
void        MassStorage_Registers   ( void );   /*2. Register */ 
tUSBStatus  MassStorage_Loop        ( void );   /*3. In Loop */ 
void        MassStorage_Exit        ( void );   /*4. Exit */ 
void        MassStorage_Write       ( void );        

#endif// _EXAMPLE_USB_STORAGE_H_

//=================================================================================
// EOF
//=================================================================================


