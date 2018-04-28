//--------------------------------------------------------------------
#include "KBDM9_DDK.h"
//#include "BinaryImage.h"
#include "USB_MAssStorage.h"
//--------------------------------------------------------------------
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE
//--------------------------------------------------------------------
extern __X unsigned long UpgReset;
//--------------------------------------------------------------------
void main(void)
{
	IP_INIT();
	USBStoragePoweroff();
	MassStorage_Init();
	USBSetDebounceCount(0x18000);
	while( !USBAttachCheck() );
	MassStorage_Registers();
	for( UpgReset=0; UpgReset<0x30000; UpgReset+=(UpgReset!=0) )
		if( MassStorage_Loop()==USB_DISCONN )
			break;
	MassStorage_Exit();
//	CommonLoadCode( C_UPG_BURN );
	__asm{ CRIT #4 NOP BCLR SOCR.15 CRIT #4 NOP MOV USB1ICR, #0 CRIT #4 NOP MOV USB2ICR, #0 CRIT #4 NOP MOV USB3ICR, #0 CRIT #4 NOP CRIT #4 RESET RESET RESET RESET }
}
//--------------------------------------------------------------------
