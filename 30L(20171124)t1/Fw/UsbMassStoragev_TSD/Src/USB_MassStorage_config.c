//------------------------------------------------------------------------------
#include <sfr.h>
#include "KBDM9_DDK.h"
#include "USB_MAssStorage.h"
//------------------------------------------------------------------------------
#pragma DATA = USB_MS_XRAM
#pragma CODE = USB_MS_CODE
//------------------------------------------------------------------------------
#if   USE_TIMER == 0
	#define	TMR_ICR  TM0ICR
	#define	TMR_CR   TMR0CR
	#define	TMR_PRD  TM0PRD
	#define	TMR_VAL  TMR0
	#define	TMR_TRAP TIMER0_INT_TRAP
#elif USE_TIMER == 1
	#define	TMR_ICR  TM1ICR
	#define	TMR_CR   TMR1CR
	#define	TMR_PRD  TM1PRD
	#define	TMR_VAL  TMR1
	#define	TMR_TRAP TIMER1_INT_TRAP
#elif USE_TIMER == 2
	#define	TMR_ICR  TM2ICR
	#define	TMR_CR   TMR2CR
	#define	TMR_PRD  TM2PRD
	#define	TMR_VAL  TMR2
	#define	TMR_TRAP TIMER2_INT_TRAP
#elif USE_TIMER == 3
	#define	TMR_ICR  TM3ICR
	#define	TMR_CR   TMR3CR
	#define	TMR_PRD  TM3PRD
	#define	TMR_VAL  TMR3
	#define	TMR_TRAP TIMER3_INT_TRAP
#endif
//------------------------------------------------------------------------------
	#define USB_MAX_LUN (USE_SD1+USE_SD2+USE_UGP+USE_SPIF)
//------------------------------------------------------------------------------
	_STORAGE usb_stor[USB_MAX_LUN];

extern Bool adpate_flag;
extern _MEM_TYPE_SLOW_   U32   usb_nosd_timer;

extern _MEM_TYPE_SLOW_   U8    usb_count;
extern Bool  b_usb_detection;	
extern int usb_core_status;
//------------------------------------------------------------------------------
#if USE_TIMER < 4
void __interrupt __usbtimer( void )
{
	__asm{ SUB TMR_CR, #0x4000 }
//	__asm{ BCLR PBOD.6 }
}
#endif
//------------------------------------------------------------------------------
void USBWriteCommit( void )
{
	int Cnt;

	for( Cnt=USB_MAX_LUN; Cnt; )
		usb_stor[--Cnt].stor_flush();
	__asm{ BCLR TMR_CR.14 }
}
//------------------------------------------------------------------------------
void MassStorage_Write( void )
{
	__asm{ PUSH R0 MOVX R0, TMR_CR MOV  TMR_CR, #0 BSET R0.15 MOVX TMR_CR, R0 POP R0 }
//	__asm{ BSET PBOD.6 }
}
//------------------------------------------------------------------------------
void MassStorage_Registers( void )
{
	int lun;

	lun = 0;
	USBCoreInit();
	USBStorageInit( USB_MAX_LUN, 6 );
#if USE_SD1
	MassStorage_SD1_Register( lun++ );
#endif
#if USE_SD2
	MassStorage_SD2_Register( lun++ );
#endif
#if USE_UGP
	MassStorage_ugp_Register( lun++ );
#endif
#if USE_SPIF
	MassStorage_SPIF_Register( lun++ );
#endif
}
//------------------------------------------------------------------------------
int MassStorage_Loop( void )
{
int temp_usb_core;
#if USE_SD1
	MassStorage_SD1_Loop();
#endif
#if USE_SD2
	MassStorage_SD2_Loop();
#endif
#if USE_UGP
	MassStorage_ugp_Loop();
#endif
#if USE_SPIF
	MassStorage_SPIF_Loop();
#endif
	if( TMR_CR & 0x4000 )
		USBWriteCommit();
	if( USBRemoveCheck() )
		return USB_DISCONN;
	temp_usb_core = USBCoreLoop();
   if( b_usb_detection )
   {
      if( usb_core_status != temp_usb_core )
      {
         usb_count++;
         usb_core_status = temp_usb_core;
      }
	   if( Timer_is_timeout( usb_nosd_timer ))
         {
         	usb_nosd_timer = 0;
            SendString(( char *)"\r\nusb_count=");
            send_hex_u8(usb_count);
			 if( usb_count < 2 )
		      {
		       
           	   adpate_flag = TRUE;
		      }
            b_usb_detection = FALSE;
         }
   }
	return USB_CONNECT;
}
//------------------------------------------------------------------------------
void USBCoreUninit( void );
void MassStorage_Exit( void )
{
	USBCoreUninit();
	USBStorageUninit();
	USBWriteCommit();
}
//------------------------------------------------------------------------------
void MassStorage_Init( void )
{
	int Ck;

#if USE_TIMER < 4
	SetVector( TMR_TRAP, __usbtimer );
	TMR_PRD  = 93750/10;	// 10Hz
	TMR_CR   = 0x1007;	// 12,000,000/128 = 93,750
	TMR_ICR  = 1;
#endif
#if USE_SD1
	MassStorage_SD1_Init();
#endif
#if USE_SD2
	Ck = SDCCR;
	MassStorage_SD2_Init();
//	#if USE_SD1
//		if( (Ck&0x40) != 0 )
//			if( (SDCCR&0x40) == 0 )
//				MassStorage_SD1_Init();
//	#endif
#endif
#if USE_UGP
	MassStorage_ugp_Init();
#endif
#if USE_SPIF
	MassStorage_SPIF_Init();
#endif

//	MassStorage_Write();
//	while(1)
//	{
//		if( TMR_CR & 0x4000 )
//		{
//			__asm{ BCLR TMR_CR.14 }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			MassStorage_Write();
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
//			__asm{ REP #-1 NOP }
////	TMR_PRD  = 0;	// 10Hz
////			MassStorage_Write();
//			MassStorage_Write();
////	TMR_PRD  = 93750/10;	// 10Hz
//		}
//	}
}
//------------------------------------------------------------------------------
