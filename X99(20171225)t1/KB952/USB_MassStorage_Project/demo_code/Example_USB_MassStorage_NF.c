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
#if USE_NF
//=============================================================================
const unsigned char FW_VER[16]={'K','9','-',HARD_VERSION,MAJOR_FW_VERSION,MINOR_FW_VERSION,OID_TYPE,' ',' ',' ',' ',' ',' ',' ',' ',' '};
//=====================================================================================
// PROGRAM
//=============================================================================
static int example_NF_is_write_protect( void )
{
    return 0;
}

static int  example_NF_write( unsigned long sector, __X void *buf )
{
    MassStorage_Write();
	return nftl_write( sector, buf );
}

static int example_NF_status( void )
{
    return USB_STORAGE_READY;
}

void MassStorage_NF_Register(int lun)
{
	USBStorageRegister(	lun,
                    #if USE_NF_CACHE	
                        nftl_read2,
                    #else
    	               	nftl_read,
                    #endif    	               	
        	          	example_NF_write,
            	        example_NF_is_write_protect,
                	    nftl_flush,
                  	  	nftl_get_size,
                  	  	example_NF_status,
                    	(DIRECT_ACCESS|REMOVABLE),
                    	"A10C    ",
                    	(char *)&FW_VER,
                    	"1.00" );

}

void MassStorage_NF_Loop( void )
{
}

void MassStorage_NF_Init( void )
{
#if USE_NF_CACHE
    nftl_read2_init();
#endif
}

#endif
//=================================================================================
// EOF
//=================================================================================
	
