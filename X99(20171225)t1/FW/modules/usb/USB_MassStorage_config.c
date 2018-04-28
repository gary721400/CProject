/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = USB_MASSSTORAGE_DATA
#pragma CODE = USB_STORAGE_CODE

#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
#include <sfr.h>
#include "sio\com.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail.h"
#include "mmi\mstorage\mmi_storage.h"
#include "lib_mcu\timer\timer_drv.h"

//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//# error "Package Version should be 0904"
//#endif
extern   Bool  g_b_usb_on;

extern Bool b_msstorage_flag;

extern   _MEM_TYPE_SLOW_   U32   usb_nosd_timer;

extern   _MEM_TYPE_SLOW_   U8    usb_count;
extern                     Bool  b_usb_detection;

void  USBCoreUninit  ( void );
//=============================================================================
// OPTION
//=============================================================================
#if USE_TIMER == 0
   #define  TMR_ICR  TM0ICR
   #define  TMR_CR   TMR0CR
   #define  TMR_PRD  TM0PRD
   #define  TMR_VAL  TMR0
   #define  TMR_TRAP 0x10
#endif
#if USE_TIMER == 1
   #define  TMR_ICR  TM1ICR
   #define  TMR_CR   TMR1CR
   #define  TMR_PRD  TM1PRD
   #define  TMR_VAL  TMR1
   #define  TMR_TRAP 0x12
#endif
#if USE_TIMER == 2
   #define  TMR_ICR  TM2ICR
   #define  TMR_CR   TMR2CR
   #define  TMR_PRD  TM2PRD
   #define  TMR_VAL  TMR2
   #define  TMR_TRAP 0x14
#endif
#if USE_TIMER == 3
   #define  TMR_ICR  TM3ICR
   #define  TMR_CR   TMR3CR
   #define  TMR_PRD  TM3PRD
   #define  TMR_VAL  TMR3
   #define  TMR_TRAP 0x16
#endif
#if USE_TIMER == 4
   #define  TMR_ICR  TM4ICR
   #define  TMR_CR   TMR4CR
   #define  TMR_PRD  TM4PRD
   #define  TMR_VAL  TMR4
   #define  TMR_TRAP 0x18
#endif
//=============================================================================
// PROGRAM
//=============================================================================
#define  USB_MAX_LUN    ( USE_NF + USE_SD + USE_RESV_NF + USE_UGP )

_STORAGE usb_stor[ USB_MAX_LUN ];
unsigned int usb_write_idle;


//#if USE_TIMER < 4
void __interrupt __usbtimer( void )
{
   ++usb_write_idle;
}
//#endif


static void USBWriteCommit( void )
{
   int i = USB_MAX_LUN;
   TMR_ICR = 0;
   TMR_CR = 0;
   while( i )
   {
      usb_stor[ --i ].stor_flush();
   }
   usb_write_idle = 0;
#if USE_NF | USE_RESV_NF
   nftl_unmount();
#endif
}


void MassStorage_Write( void )
{
//#if USE_TIMER < 4
   usb_write_idle = 0;
   SetVector( TMR_TRAP, __usbtimer );
   TMR_ICR  =  1;
   TMR_CR   =  0xD007; //12Mhz   //0xE007;
   TMR_PRD  =  0xB71B; //500ms   //0x7F;
   TMR_VAL  =  0x00;

   if( b_msstorage_flag )
   {
      b_msstorage_flag = FALSE;
      mail_send_event( EVT_USB_DEVICE_RECEIV, 0 );
      SendString((U8 *)"sd write\r\n");
   }
//#endif    
}

#pragma CODE = USB_STORAGE_TEMP_CODE00

void MassStorage_Registers( void )
{
   int lun = 0;

   USBCoreInit();
   USBStorageInit( USB_MAX_LUN, 6 );

   // Register USB Storage Access function Pointers
#if USE_SD
   MassStorage_SD_Register( lun++ );
#endif
#if USE_NF
   MassStorage_NF_Register( lun++ );
#endif
#if USE_RESV_NF
   MassStorage_resvNF_Register( lun++ );
#endif
#if USE_UGP
   MassStorage_ugp_Register( lun++ );
#endif
}


#pragma CODE = USB_STORAGE_CODE

tUSBStatus MassStorage_Loop( void )
{
   int temp_usb_core;

#if USE_SD
   MassStorage_SD_Loop();
#endif
#if USE_NF
   MassStorage_NF_Loop();
#endif
#if USE_RESV_NF
   MassStorage_resvNF_Loop();
#endif
#if USE_UGP
   MassStorage_ugp_Loop();
#endif

   if( usb_write_idle >= USB_COMMIT_TIME )
   {
      USBWriteCommit();
   }

   if( USBRemoveCheck())
   {
      return USB_DISCONN; //
   }

   temp_usb_core = USBCoreLoop();
   if( b_usb_detection )
   {
      if( usb_core_status != temp_usb_core )
      {
         usb_count++;
         usb_core_status = temp_usb_core;
      }
      if( usb_count < 2 )
      {
         if( Timer_is_timeout( usb_nosd_timer ))
         {
            SendString(( U8 *)"usb_count=");
            send_hex_u8(usb_count);
            SendString(( U8 *)"\r\n");
            b_usb_detection = FALSE;
            Usb_connect = FALSE;
            mail_send_event( EVT_USB_DEVICE_CHARGE, 0 );
            //g_b_usb_on = FALSE;
            //return USB_DISCONN;
         }
      }
      else
         b_usb_detection = FALSE;
   }

   return USB_CONNECT;
}


#pragma CODE = USB_STORAGE_TEMP_CODE01

void MassStorage_Exit( void )
{
   USBCoreUninit();
   USBStorageUninit();
   USBWriteCommit();
}


#pragma CODE = USB_STORAGE_TEMP_CODE00
//#pragma CODE = USB_STORAGE_CODE

void MassStorage_Init( void )
{
#if USE_SD
   MassStorage_SD_Init();
#endif 
#if USE_NF
   MassStorage_NF_Init();
#endif 
#if USE_IDE
   MassStorage_IDE_Init();
#endif 
#if USE_RESV_NF
   MassStorage_resvNF_Init();
#endif    
#if USE_UGP
   MassStorage_ugp_Init();
#endif
}
//=================================================================================
// EOF
//=================================================================================

