/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE

#include "KBDM9_DDK.h"
#include "Example_USB_MAssStorage.h"
#include <sfr.h>
#include "conf_kb.h"
//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//# error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
#if USE_TIMER == 0
    #define    TMR_ICR  TM0ICR
    #define    TMR_CR   TMR0CR
    #define    TMR_PRD  TM0PRD
    #define    TMR_VAL  TMR0
    #define    TMR_TRAP 0x10
#endif
#if USE_TIMER == 1
    #define    TMR_ICR  TM1ICR
    #define    TMR_CR   TMR1CR
    #define    TMR_PRD  TM1PRD
    #define    TMR_VAL  TMR1
    #define    TMR_TRAP 0x12
#endif
#if USE_TIMER == 2
    #define    TMR_ICR  TM2ICR
    #define    TMR_CR   TMR2CR
    #define    TMR_PRD  TM2PRD
    #define    TMR_VAL  TMR2
    #define    TMR_TRAP 0x14
#endif
#if USE_TIMER == 3
    #define    TMR_ICR  TM3ICR
    #define    TMR_CR   TMR3CR
    #define    TMR_PRD  TM3PRD
    #define    TMR_VAL  TMR3
    #define    TMR_TRAP 0x16
#endif

//=============================================================================
//=============================================================================
// PROGRAM
//=============================================================================
#define  USB_MAX_LUN        (USE_NF+USE_SD+USE_RESV_NF+USE_UGP)

_STORAGE usb_stor[USB_MAX_LUN];
unsigned int usb_write_idle;

#if USE_TIMER < 4
void __interrupt __usbtimer(void)
{
    ++usb_write_idle;
}
#endif

static void USBWriteCommit(void)
{
    int i = USB_MAX_LUN-1;
    TMR_ICR = 0;
    while( i )
    {
        usb_stor[--i].stor_flush();
    }
    usb_write_idle = 0;
    #if USE_NF | USE_RESV_NF
        nftl_unmount();
    #endif
}
/*
	TMR3CR = 0xD005;	    // Clock = 12000, Div = 32
 	//TM3PRD = 9675; 		// 50ms*24M/128
 	TM3PRD = 750; 		// 2ms*24M/128
*/

void MassStorage_Write(void)
{
#if USE_TIMER < 4
    usb_write_idle = 0;
    SetVector( TMR_TRAP, __usbtimer );
    TMR_ICR  =  1;
    TMR_CR   =  0xD007;
    TMR_PRD  =  0xB71B;//500ms
    TMR_VAL  =  0x0;
#endif    
}

void MassStorage_Registers(void)
{
    int lun = 0;
    
    USBCoreInit();
    USBStorageInit(USB_MAX_LUN, 6);

    // Register USB Storage Access function Pointers
    #if USE_SD
        MassStorage_SD_Register( lun++ );
    #endif
    #if USE_NF
        MassStorage_NF_Register( lun++ );
    #endif
    #if USE_RESV_NF
        MassStorage_resvNF_Register( lun++ );
    #endif
        #if USE_UGP
                MassStorage_ugp_Register( lun++ );
        #endif
    
}


tUSBStatus MassStorage_Loop( void )
{
    #if USE_SD
        MassStorage_SD_Loop();
    #endif
    #if USE_NF
        MassStorage_NF_Loop();
    #endif
    #if USE_RESV_NF
        MassStorage_resvNF_Loop();
    #endif
        #if USE_UGP
                MassStorage_ugp_Loop();
        #endif
        
    if( usb_write_idle >= USB_COMMIT_TIME )
    {
        USBWriteCommit();
    }
    
    if( USBRemoveCheck() )
    {
        return USB_DISCONN; //
    }
    
    USBCoreLoop();
    
    return USB_CONNECT;
    
}

void USBCoreUninit(void);
void MassStorage_Exit(void)
{
    USBCoreUninit();
    USBStorageUninit();
    USBWriteCommit();
}

void MassStorage_Init(void)
{
    #if USE_SD
        MassStorage_SD_Init();
    #endif 
    #if USE_NF
        MassStorage_NF_Init();
    #endif 
    #if USE_IDE
        MassStorage_IDE_Init();
    #endif 
    #if USE_RESV_NF
        MassStorage_resvNF_Init();
    #endif    
    #if USE_UGP
            MassStorage_ugp_Init();
    #endif
}
//=================================================================================
// EOF
//=================================================================================

