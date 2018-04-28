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
#if USE_RESV_NF
//=============================================================================
// PROGRAM
//=============================================================================
__X unsigned long DiskStartSec;
__X unsigned long DiskSize;

static int example_resvNF_is_write_protect( void )
{
    return 0;
}

static unsigned long example_resvNF_size( void )
{
	return DiskSize/512; //in sector unit
}

static int example_resvNF_read(unsigned long sector, __X void *buf )
{
	return nftl_resv_read( sector+DiskStartSec, buf );
}

static int example_resvNF_status( void )
{
    return USB_STORAGE_READY;
}

static int example_resvNF_write(unsigned long sector, __X void *buf )
{
#if USE_CDROM
    return 0;
#else    
    MassStorage_Write();
	return nftl_resv_write( sector+DiskStartSec, buf );
#endif	 
}

void MassStorage_resvNF_Register(int lun)
{
#if USE_CDROM
	USBStorageRegister(	lun,
    	               	example_resvNF_read,
        	          	example_resvNF_write,
            	        example_resvNF_is_write_protect,
                	    nftl_flush,
                  	  	example_resvNF_size,
                  	  	example_resvNF_status,
                    	(CDROM|REMOVABLE),
                    	"KB USB20",
                    	"NandFlash Disk  ",
                    	"1.00" );
#else	
	USBStorageRegister(	lun,
    	               	example_resvNF_read,
        	          	example_resvNF_write,
            	        example_resvNF_is_write_protect,
                	    nftl_flush,
                  	  	example_resvNF_size,
                  	  	example_resvNF_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	"KB USB20",
                    	"NandFlash Disk  ",
                    	"1.00" );
#endif
	// Set storage device to ready.
}

void MassStorage_resvNF_Loop( void )
{
}

void MassStorage_resvNF_Init( void )
{
    GetDataIdInfo(NF_DISK_IMAGE_ID, &DiskStartSec, &DiskSize);
}
#endif	
//=================================================================================
// EOF
//=================================================================================

