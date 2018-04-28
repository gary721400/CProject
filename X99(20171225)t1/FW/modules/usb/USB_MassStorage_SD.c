/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = USB_MASSSTORAGE_DATA
#pragma CODE = USB_STORAGE_CODE

#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
#include "config.h"
#include "string.h"

//#define  USB_DEVICE_MS_VENDOR        "X99  SPI"           //  8 Bytes mandatory
#define  USB_DEVICE_MS_VENDOR          "X99  XDF"           //  8 Bytes mandatory
#define  USB_DEVICE_MS_PRODUCT         "OID4 V2.0.4.2B  "   // 16 Bytes mandatory
#define  USB_DEVICE_MS_REVISION        "2.04"               //  4 Bytes mandatory


//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//# error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
#if USE_SD

#if USE_SD_8K_CACHE 
   extern   SDTL_ERR    sdtl_usb_write_8k       ( unsigned long sector, __X void *buf );
   extern   SDTL_ERR    sdtl_usb_read_8k        ( unsigned long sector, __X void *buf );
   extern   void        sdtl_usb_cache8k_init   ( void );
   extern   int         sdtl_usb_flush8k        ( void );
#endif

#if USE_SD_4K_CACHE
   extern   SDTL_ERR    sdtl_usb_read_4k     ( unsigned long sector, __X void *buf );
   extern   void        sdtl_usb_cache_init  ( void );
   extern   SDTL_ERR    sdtl_usb_write       ( unsigned long sector, __X void *buf );
   extern   int         sdtl_usb_flush       ( void );    
#endif

//=============================================================================
// PROGRAM
//=============================================================================
//char product[16];
//char revision[4];
int sd_num;
int have_sd_card;
extern unsigned int _is_sd_mounted;
//const unsigned char product[16]={'O','I','D','4','-',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE_VERSION,' ',' ',' ',' ',' ',' ',' '};
//const unsigned char product[16]={' ',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE_VERSION,'-',H_YEAR,L_YEAR,H_MONTH,L_MONTH,H_DAY,L_DAY,' ',' ',' ',' '};
unsigned char product[16];
unsigned char revision[4];
//const unsigned char revision[4]={HARD_VERSION,'.',MAJOR_FW_VERSION,MINOR_FW_VERSION};
/*
void ready_version(void)
{
   //product[0] = HARD_VERSION;USB_DEVICE_MS_PRODUCT         "OID4 V2.0.4.2B  "
   strcpy(product,"OID");
   strcat(product,(char *)OID_TYPE_VERSION);
   strcat(product,(char *)' ');
   strcat(product,(char *)'V');
   strcat(product,(char *)HARD_VERSION);
   strcat(product,(char *)'.');
   strcat(product,(char *)MAJOR_FW_VERSION);
   strcat(product,(char *)'.');
   strcat(product,(char *)MINOR_FW_VERSION);
   strcat(product,(char *)'.');
   strcat(product,(char *)OID_TYPE_VERSION);
   strcat(product,(char *)"   ");
   
   strcat(revision,(char *)HARD_VERSION);
   strcat(revision,(char *)'.');
   strcat(revision,(char *)MAJOR_FW_VERSION);
   strcat(revision,(char *)MINOR_FW_VERSION);
}
*/
static int example_sd_is_write_protect( void )
{
   return 0;
}


#if USE_SD_8K_CACHE
static int example_sd_write_cache1( unsigned long sector, __X void *buf )
{
   MassStorage_Write();
   return( sdtl_usb_write_8k( sector, buf ));   // 8k cache for speed up writing
}

#elif USE_SD_4K_CACHE 
static int example_sd_write_cache1( unsigned long sector, __X void *buf )
{
   MassStorage_Write();
   return( sdtl_usb_write( sector, buf ));   // 4k cache for speed up writing 
}

#else
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
#  endif
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
//   ready_version();
/*
    char temp_buf[16];
    memset(temp_buf,0,sizeof(temp_buf));
	strcpy(temp_buf,(char *)HARD_VERSION);
	strcat(temp_buf,(char *)MAJOR_FW_VERSION);
	strcat(temp_buf,(char *)MINOR_FW_VERSION);
	strcat(temp_buf,(char *)'-');
	*/

   sd_num = lun;
	

#if USE_SD_8K_CACHE
	USBStorageRegister( lun,
          	          	( reg_type )sdtl_usb_read_8k,
                        example_sd_write_cache1,
                        example_sd_is_write_protect,
                        sdtl_usb_flush8k,
                        sdtl_get_size,
                        example_sd_status,
                        ( DIRECT_ACCESS | REMOVABLE ),
                        USB_DEVICE_MS_VENDOR,
                        (char *)&product,
                        (char *)&revision);
#elif USE_SD_4K_CACHE
   USBStorageRegister( lun,
                        ( reg_type )sdtl_usb_read_4k, 
                        example_sd_write_cache1,
                        example_sd_is_write_protect,
                        sdtl_usb_flush,
                        sdtl_get_size,
                        example_sd_status,
                        ( DIRECT_ACCESS | REMOVABLE ),
                        USB_DEVICE_MS_VENDOR,
                        (char *)&product,
                        (char *)&revision );
#else

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
                        (char *)&product,
                        (char *)&revision );
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
                        (char *)&product,
                        (char *)&revision );
#  endif
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
#if USE_SD_8K_CACHE
   sdtl_usb_cache8k_init();      // 8K cache
#elif USE_SD_4K_CACHE
   sdtl_usb_cache_init();        // 4K cache
#else
#  if USE_SD_CACHE	
   sdtl_cache_init();
#  endif
#endif
}
#endif  	
//=================================================================================
// EOF
//=================================================================================

