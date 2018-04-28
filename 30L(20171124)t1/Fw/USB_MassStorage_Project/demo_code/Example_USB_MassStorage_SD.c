/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE

#include "KBDM9_DDK.h"
#include "Example_USB_MAssStorage.h"



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
//=============================================================================
const unsigned char MASTER_VER[8]={MASTER_0,MASTER_1,MASTER_2,MASTER_3,MASTER_4,MASTER_5,MASTER_6,MASTER_7};
const unsigned char FW_VER[16]={'O','I','D',OID_TYPE,'-',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE,' ',' ',' ',' ',' ',' ',' '};
// PROGRAM
//=============================================================================
int sd_num;
int have_sd_card;
extern unsigned int _is_sd_mounted;
static int example_sd_is_write_protect( void )
{
    return 0;
}

#if USE_SD_4K_CACHE 

static int example_sd_write_cache1(unsigned long sector, __X void *buf )
{
    MassStorage_Write();
    return(sdtl_usb_write(sector, buf));          // 4k cache for speed up writing 

}

#else 

static int example_sd_write_cache1(unsigned long sector, __X void *buf )
{
    MassStorage_Write();
    return(sdtl_cache_write(sector, buf));          // 2k cache for speed up writing    
}

#endif

//PA13
extern unsigned int _is_sd_mounted;
static int example_sd_status(void)
{
	//do your check code
	have_sd_card = (int)_is_sd_mounted;
	if( have_sd_card )
    {
		return USB_STORAGE_READY;
  	}
    else	
    {
	    return USB_STORAGE_NOT_READY;
	}
}

typedef int (*reg_type)(unsigned long,__X void*);

void MassStorage_SD_Register(int lun)
{
	sd_num = lun;

#if USE_SD_4K_CACHE
	USBStorageRegister( lun,
          	          	(reg_type)sdtl_usb_read_4k, 
          	          	example_sd_write_cache1,
          	          	example_sd_is_write_protect,
          	          	sdtl_usb_flush,
              	      	sdtl_get_size,
              	      	example_sd_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	(char *)&MASTER_VER,
                     	(char *)&FW_VER,
                     	"1.00" );

#else

	USBStorageRegister( lun,
          	          	(reg_type)sdtl_usb_read, 
          	          	example_sd_write_cache1,
          	          	example_sd_is_write_protect,
          	          	sdtl_cache_flush,
              	      	sdtl_get_size,
              	      	example_sd_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	(char *)&MASTER_VER,
                     	(char *)&FW_VER,
                     	"1.00" );

    
#endif
}

void MassStorage_SD_Loop( void )
{
    // Check SD card exist or not ??
}


void MassStorage_SD_Init( void )
{
	sdtl_mount();
	

#if USE_SD_4K_CACHE

  sdtl_usb_cache_init();        // 4K cache
  
#else

	sdtl_cache_init();            // 2k cache
  
#endif

}
	
#endif  	
//=================================================================================
// EOF
//=================================================================================

