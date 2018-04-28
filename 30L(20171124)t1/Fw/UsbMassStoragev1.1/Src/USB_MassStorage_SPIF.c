//------------------------------------------------------------------------------
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//------------------------------------------------------------------------------
#pragma DATA = USB_MS_XRAM
#pragma CODE = USB_MS_CODE
//------------------------------------------------------------------------------
#if USE_SPIF
//------------------------------------------------------------------------------
int spif_protect( void )
{
	return 0;
}
//------------------------------------------------------------------------------
int spif_write( unsigned long sector, __X void *buf )
{
	MassStorage_Write();
	return spif_cache_write( sector, buf );
}
//------------------------------------------------------------------------------
int spif_read( unsigned long sector, __X void *buf )
{
	return spif_cache_read( sector, buf );
}
//------------------------------------------------------------------------------
int spif_status( void )
{
	return USB_STORAGE_READY;
}
//------------------------------------------------------------------------------
void MassStorage_SPIF_Register(int lun)
{
	USBStorageRegister( lun, spif_read, spif_write, spif_protect, spif_cache_flush, spif_get_size, spif_status, DIRECT_ACCESS|REMOVABLE, "KB USB20", "SPIF Disk", "1.00" );
}
//------------------------------------------------------------------------------
void MassStorage_SPIF_Loop( void )
{
}
//------------------------------------------------------------------------------
void MassStorage_SPIF_Init( void )
{
	spif_mount();
	SPIF_Read_Write();
	spif_cache_init();
}
//------------------------------------------------------------------------------
#endif
//------------------------------------------------------------------------------
