//--------------------------------------------------------------------
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE
//--------------------------------------------------------------------
#include <string.h>
#include "KBDM9_DDK.h"
#include "UpgBurnSPI.h"
//--------------------------------------------------------------------
__X BHCBW* pBhCbw;
__X unsigned char Upb4kBuf[BUF_B4KB][BUF_SIZB];
__X unsigned long UpgCommand, UpgAdr, UpgReset;
//--------------------------------------------------------------------
const unsigned char ugp_root_sec[] = {
	'S', 'P', 'I', 'C', 'W', ' ', ' ', ' ', 'I', 'N', 'I', 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00,
	'S', 'P', 'I', 'C', 'R', ' ', ' ', ' ', 'I', 'N', 'I', 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x02, 0x00, 0x00,
	'S', 'P', 'I', 'D', 'W', ' ', ' ', ' ', 'I', 'N', 'I', 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0xf8, 0x0f, 0x00,
	'S', 'P', 'I', 'D', 'R', ' ', ' ', ' ', 'I', 'N', 'I', 0x26, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0x00,
 };
//--------------------------------------------------------------------
int user_command( __X void* buf )
{
	UpgCommand = 0;
	pBhCbw = (__X BHCBW*)buf;
	if( pBhCbw->Signature1 != CMD_ID1 )
		return 1;
	if( pBhCbw->Signature2 != CMD_ID2 )
		return 1;
	if( pBhCbw->Command == CMD_BURN )
	{
		UpgCommand = pBhCbw->Command;
		return UpgAdr=0;
	}
	if( pBhCbw->Command == CMD_RESET )
	{
		UpgReset++;
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------------
int user_respond( __X void* buf )
{
	return 0;
}
//--------------------------------------------------------------------
void user_write( __X void* buf )
{
	unsigned long Mod;

	if( UpgCommand == CMD_BURN )
	{
		if( (Mod=UpgAdr%BUF_B4KB) == 0 )
			SPIF_Sector_Erase( UpgAdr*BUF_SIZB );
		memcpy( Upb4kBuf[Mod], buf, BUF_SIZB );
		if( Mod == BUF_B4KB-1 )
			SPIF_Page_Program( (UpgAdr-Mod)*BUF_SIZB, (__X unsigned char*)Upb4kBuf, BUF_B4KB*2 );
		UpgAdr++;
	}
}
//--------------------------------------------------------------------
void user_read( __X void* buf )
{
	if( UpgCommand == CMD_BURN )
	{
		*((__X unsigned long*)buf) = (spif_resv_size()+spif_get_size()+4)<<9;
	}
}
//--------------------------------------------------------------------
