//--------------------------------------------------------------------
#include "sfr.h"
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//--------------------------------------------------------------------
#pragma DATA = USB_MS_XRAM
#pragma CODE = USB_MS_CODE
//--------------------------------------------------------------------
#if USE_SD2
//--------------------------------------------------------------------
int usd2_protect( void )
{
	return 0;
}
//--------------------------------------------------------------------
int usd2_write( unsigned long Sector, __X void *Buf )
{
	MassStorage_Write();
	return sd2_cache_write(Sector,Buf);
}
//--------------------------------------------------------------------
int usd2_status( void )
{
	if( SD2_Mounted )
		return USB_STORAGE_READY;
	else
		return USB_STORAGE_NOT_READY;
}
//--------------------------------------------------------------------
void MassStorage_SD2_Register( int lun )
{
	USBStorageRegister( lun, sd2_cache_read, usd2_write, usd2_protect, sd2_cache_flush, sd2_fat_size, usd2_status, DIRECT_ACCESS|REMOVABLE, "KB USB20", "SD-2 Disk", "1.00" );
}
//--------------------------------------------------------------------
void MassStorage_SD2_Loop( void )
{

}
//--------------------------------------------------------------------
void MassStorage_SD2_Init( void )
{
	sd2_mount();
	SDCKCR = 3;
#if   SD2_CACHE == SD_CACHE_2K
	sd2_cache2k_init();
#elif SD2_CACHE == SD_CACHE_4K
	sd2_cache4k_init();
#elif SD2_CACHE == SD_CACHE_8K
	sd2_cache8k_init();
#else
	while(1);
#endif
}
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
