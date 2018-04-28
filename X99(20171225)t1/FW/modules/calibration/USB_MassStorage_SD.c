/******************************************************************************
** Copyright (c) 2010
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = USB_MASSSTORAGE_DATA
#pragma CODE = USB_STORAGE_CODE

#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"


#define  USB_DEVICE_MS_VENDOR          "A30L SPI"           //  8 Bytes mandatory
#define  USB_DEVICE_MS_PRODUCT         "OID4 V1.0.2.5TLH"   // 16 Bytes mandatory
#define  USB_DEVICE_MS_REVISION        "1.02"               //  4 Bytes mandatory


//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x1005)
//# error "Package Version should be 1005"
//#endif

//=============================================================================
// OPTION
//=============================================================================
#if USE_SD
//=============================================================================
// PROGRAM
//=============================================================================
int sd_num;
int have_sd_card;
extern unsigned int _is_sd_mounted;


static int example_sd_is_write_protect( void )
{
   return 0;
}


unsigned long sdtl_get_size_x2( void )
{
   return ( sdtl_get_size()* 2 );
}


#  if USE_SD_CACHE
static int example_sd_write_cache1( unsigned long sector, __X void *buf )
{
   MassStorage_Write();
   return( sdtl_cache_write (sector, buf ));    // cache for speed up writing    
}

#  else 
static int example_sd_write_cache1( unsigned long sector, __X void *buf )
{
   MassStorage_Write();
   return( sdtl_write( sector, buf ));
}

#endif


//PA13
static int example_sd_status( void )
{
	//do your check code
	have_sd_card = ( int )_is_sd_mounted;
   if( have_sd_card )
   {
      return USB_STORAGE_READY;
   }
   else	
   {
      return USB_STORAGE_NOT_READY;
   }
}


typedef int ( *reg_type )( unsigned long, __X void * );


#pragma CODE = USB_STORAGE_TEMP_CODE00

void MassStorage_SD_Register( int lun )
{
   sd_num = lun;

#  if USE_SD_CACHE
   USBStorageRegister( lun,
                        ( reg_type )sdtl_usb_read, 
                        example_sd_write_cache1,
                        example_sd_is_write_protect,
                        sdtl_cache_flush,
                        sdtl_get_size,
                        example_sd_status,
                        ( DIRECT_ACCESS | REMOVABLE ),
                        USB_DEVICE_MS_VENDOR,
                        USB_DEVICE_MS_PRODUCT,
                        USB_DEVICE_MS_REVISION );
#  else
   USBStorageRegister( lun,
                        ( reg_type )sdtl_read,
                        example_sd_write_cache1,
                        example_sd_is_write_protect,
                        sdtl_flush,
                        sdtl_get_size,
                        example_sd_status,
                        ( DIRECT_ACCESS | REMOVABLE ),
                        USB_DEVICE_MS_VENDOR,
                        USB_DEVICE_MS_PRODUCT,
                        USB_DEVICE_MS_REVISION );
#endif
}


#pragma CODE = USB_STORAGE_CODE

void MassStorage_SD_Loop( void )
{
   // Check SD card exist or not ??
}


#pragma CODE = USB_STORAGE_TEMP_CODE00

void MassStorage_SD_Init( void )
{
   sdtl_mount();
#  if USE_SD_CACHE	
   sdtl_cache_init();
#endif
}
#endif  	
//=================================================================================
// EOF
//=================================================================================

