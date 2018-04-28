/******************************************************************************
** Copyright (c) 2010
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

unsigned long sdtl_get_size_x2(void)
{
	return (sdtl_get_size() * 2);
}

#if USE_SD_CACHE

static int example_sd_write_cache1(unsigned long sector, __X void *buf )
{
    MassStorage_Write();
    return(sdtl_cache_write(sector, buf));          // cache for speed up writing

}

#else

static int example_sd_write_cache1(unsigned long sector, __X void *buf )
{
    MassStorage_Write();
    return(sdtl_write(sector, buf));
}

#endif

//PA13
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

#if USE_SD_CACHE

	USBStorageRegister( lun,
          	          	(reg_type)sdtl_usb_read,
          	          	example_sd_write_cache1,
          	          	example_sd_is_write_protect,
          	          	sdtl_cache_flush,
              	      	//sdtl_get_size_x2,
              	      	sdtl_get_size,
              	      	example_sd_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	"KB USB20",
                     	"SD Disk         ",
                     	"1.00" );
#else

	USBStorageRegister( lun,
          	          	(reg_type)sdtl_read,
          	          	example_sd_write_cache1,
          	          	example_sd_is_write_protect,
          	          	sdtl_flush,
              	      	//sdtl_get_size_x2,
              	      	sdtl_get_size,
              	      	example_sd_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	"KB USB20",
                     	"SD Disk         ",
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
#if USE_SD_CACHE
	sdtl_cache_init();
#endif
}

#endif
//=================================================================================
// EOF
//=================================================================================

