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
#if (PACKAGE_VER != 0x0904)
//# error "Package Version should be 0904"
#endif

//=============================================================================
// OPTION
//=============================================================================
#if USE_UGP
//=============================================================================
// PROGRAM
//=============================================================================
void ugp_dev_init( void );
int  ugp_dev_write( unsigned long sector, __X void* buf );
int  ugp_dev_read ( unsigned long sector, __X void* buf );
unsigned long ugp_dev_size(void);

static int example_ugp_is_write_protect( void )
{
    return 0;
}

static int example_ugp_status( void )
{
    return USB_STORAGE_READY;
}

static int example_ugp_flush(void)
{
    return 0;
}

void MassStorage_ugp_Register(int lun)
{
        USBStorageRegister( lun,
                        ugp_dev_read,
                        ugp_dev_write,
                        example_ugp_is_write_protect,
                        example_ugp_flush,
                        ugp_dev_size,
                        example_ugp_status,
                        (DIRECT_ACCESS|REMOVABLE),
                        "KB USB20",
                        "FC DEVICE Disk  ",
                        "1.00" );

}

void MassStorage_ugp_Loop( void )
{
}

void MassStorage_ugp_Init( void )
{
    ugp_dev_init();
}

const unsigned char ugp_root_sec[] = {
 'C', 'W', ' ', ' ', ' ', ' ', ' ', ' ',    //command write file name, 8 ascii
 'I', 'N', 'I',                             //command write file ext name, 3 ascii
 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00,
 
 'C', 'R', ' ', ' ', ' ', ' ', ' ', ' ',    //result read file name, 8 ascii    
 'I', 'N', 'I',                             //result read file ext name, 3 ascii
 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00,
 
 'D', 'W', ' ', ' ', ' ', ' ', ' ', ' ',    //data write file name, 8 ascii    
 'I', 'N', 'I',                             //data write file ext name, 3 ascii
 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x04, 0x00, 0x00, 0xf8, 0x0f, 0x00,
 
 'D', 'R', ' ', ' ', ' ', ' ', ' ', ' ',    //data read file name, 8 ascii    
 'I', 'N', 'I',                             //data read file ext name, 3 ascii
 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00,
 }; 

int user_command( __X void* buf )
{
    return 0;
}

int user_respond( __X void* buf )
{
    return 0;
}

void user_write( __X void* buf )
{
}

void user_read( __X void* buf )
{
}


#endif
//=================================================================================
// EOF
//=================================================================================
    