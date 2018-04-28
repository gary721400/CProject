//--------------------------------------------------------------------
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE
//--------------------------------------------------------------------
#include <sfr.h>
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//--------------------------------------------------------------------
#if USE_TIMER == 0
	#define	TMR_ICR  TM0ICR
	#define	TMR_CR   TMR0CR
	#define	TMR_PRD  TM0PRD
	#define	TMR_VAL  TMR0
	#define	TMR_TRAP 0x12
#endif
#if USE_TIMER == 1
	#define	TMR_ICR  TM1ICR
	#define	TMR_CR   TMR1CR
	#define	TMR_PRD  TM1PRD
	#define	TMR_VAL  TMR1
	#define	TMR_TRAP 0x14
#endif
#if USE_TIMER == 2
	#define	TMR_ICR  TM2ICR
	#define	TMR_CR   TMR2CR
	#define	TMR_PRD  TM2PRD
	#define	TMR_VAL  TMR2
	#define	TMR_TRAP 0x16
#endif
#if USE_TIMER == 3
	#define	TMR_ICR  TM3ICR
	#define	TMR_CR   TMR3CR
	#define	TMR_PRD  TM3PRD
	#define	TMR_VAL  TMR3
	#define	TMR_TRAP 0x18
#endif
//--------------------------------------------------------------------
	#define	USB_MAX_LUN	(USE_SD+USB_UGP)
//--------------------------------------------------------------------
	_STORAGE usb_stor[USB_MAX_LUN];
	unsigned int usb_write_idle;
//--------------------------------------------------------------------
#if USE_TIMER < 4
//--------------------------------------------------------------------
void __interrupt __usbtimer( void )
{
	++usb_write_idle;
}
//--------------------------------------------------------------------
#endif
//--------------------------------------------------------------------
static void USBWriteCommit( void )
{
	int Cnt;

	for( TMR_ICR=0,Cnt=USB_MAX_LUN-1; Cnt; usb_stor[--Cnt].stor_flush() );
	usb_write_idle = 0;
}
//--------------------------------------------------------------------
void MassStorage_Write( void )
{
#if USE_TIMER < 4
	usb_write_idle = 0;
	SetVector( TMR_TRAP, __usbtimer );
	TMR_ICR = 1;
	TMR_CR  = 0xE007;
	TMR_PRD = 0x7F;
	TMR_VAL = 0x0;
#endif
}
//--------------------------------------------------------------------
void MassStorage_Registers( void )
{
	int lun;

	lun = 0;
	USBCoreInit();
   	USBStorageInit(USB_MAX_LUN, 6);
#if USE_SD
	MassStorage_SD_Register( lun++ );
#endif
#if USB_UGP
	MassStorage_ugp_Register( lun++ );
#endif
}
//--------------------------------------------------------------------
tUSBStatus MassStorage_Loop( void )
{
#if USE_SD
	MassStorage_SD_Loop();
#endif
#if USB_UGP
	MassStorage_ugp_Loop();
#endif
	if( usb_write_idle >= USB_COMMIT_TIME )
		USBWriteCommit();
	if( USBRemoveCheck() )
		return USB_DISCONN;
	USBCoreLoop();
	return USB_CONNECT;
}
//--------------------------------------------------------------------
void MassStorage_Exit(void)
{
	USBStorageUninit();
	USBWriteCommit();
}
//--------------------------------------------------------------------
void MassStorage_Init(void)
{
#if USE_SD
	MassStorage_SD_Init();
#endif
#if USB_UGP
	MassStorage_ugp_Init();
#endif
}
//--------------------------------------------------------------------
