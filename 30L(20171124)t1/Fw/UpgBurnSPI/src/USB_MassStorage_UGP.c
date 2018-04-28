//--------------------------------------------------------------------
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE
//--------------------------------------------------------------------
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//--------------------------------------------------------------------
#if USB_UGP
//--------------------------------------------------------------------
void ugp_dev_init( void );
int  ugp_dev_write( unsigned long sector, __X void* buf );
int  ugp_dev_read ( unsigned long sector, __X void* buf );
//--------------------------------------------------------------------
unsigned long ugp_dev_size( void );
//--------------------------------------------------------------------
static int ugp_write_protect( void )
{
	return 0;
}
//--------------------------------------------------------------------
static int ugp_status( void )
{
	return USB_STORAGE_READY;
}
//--------------------------------------------------------------------
static int ugp_flush( void )
{
	return 0;
}
//--------------------------------------------------------------------
void MassStorage_ugp_Register( int lun )
{
	USBStorageRegister( lun, ugp_dev_read, ugp_dev_write, ugp_write_protect, ugp_flush, ugp_dev_size, ugp_status, (DIRECT_ACCESS|REMOVABLE), "Imagosemi", "UpgBurnSPI", "1.00" );
}
//--------------------------------------------------------------------
void MassStorage_ugp_Loop( void )
{
}
//--------------------------------------------------------------------
void MassStorage_ugp_Init( void )
{
	ugp_dev_init();
}
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------