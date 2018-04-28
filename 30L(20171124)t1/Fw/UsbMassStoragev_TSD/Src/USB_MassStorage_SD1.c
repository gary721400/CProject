//--------------------------------------------------------------------
#include "sfr.h"
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//--------------------------------------------------------------------
#pragma DATA = USB_MS_XRAM
#pragma CODE = USB_MS_CODE
//--------------------------------------------------------------------
#if USE_SD1
//--------------------------------------------------------------------
int usd1_protect( void )
{
	return 0;
}
//--------------------------------------------------------------------
int usd1_write( unsigned long Sector, __X void *Buf )
{
	MassStorage_Write();
	return sd1_cache_write(Sector,Buf);
}
//--------------------------------------------------------------------
int usd1_status( void )
{
	if( SD1_Mounted )
		return USB_STORAGE_READY;
	else
		return USB_STORAGE_NOT_READY;
}
//--------------------------------------------------------------------
void MassStorage_SD1_Register( int lun )
{
	USBStorageRegister( lun, sd1_cache_read, usd1_write, usd1_protect, sd1_cache_flush, sd1_fat_size, usd1_status, DIRECT_ACCESS|REMOVABLE, "KB USB20", "SD-1 Disk", "1.00" );
}
//--------------------------------------------------------------------
void MassStorage_SD1_Loop( void )
{

}
//--------------------------------------------------------------------
void MassStorage_SD1_Init( void )
{
	sd1_mount();
	SDCKCR = 3;
#if   SD1_CACHE == SD_CACHE_2K
	sd1_cache2k_init();
#elif SD1_CACHE == SD_CACHE_4K
	sd1_cache4k_init();
#elif SD1_CACHE == SD_CACHE_8K
	sd1_cache8k_init();
#else
	while(1);
#endif
}
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
