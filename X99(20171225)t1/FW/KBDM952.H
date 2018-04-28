#ifndef _KBDM952_H_
#define _KBDM952_H_

#include "SFR.H"

/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* Defined configuration peripherals */

/* *********************************************************************** */
/* *********************************************************************** */

typedef enum
{
   TIMER0   =  ( int )0xF2A0, /**<IRQ source为Timer 0 , 所设定的内容值为KBMP3所定的T0ICR之register的位置*/
   TIMER1   =  ( int )0xF2A2,	/**<IRQ source为Timer 1 , 所设定的内容值为KBMP3所定的T1ICR之register的位置*/
   TIMER2   =  ( int )0xF2A4,	/**<IRQ source为Timer 2 , 所设定的内容值为KBMP3所定的T2ICR之register的位置*/
   TIMER3   =  ( int )0xF2A6,	/**<IRQ source为Timer 3 , 所设定的内容值为KBMP3所定的T3ICR之register的位置*/
   TIMER4   =  ( int )0xF2A8,	/**<IRQ source为Timer 4 , 所设定的内容值为KBMP3所定的T4ICR之register的位置*/
}tKBMP3_IRQSource;

#define _TM0ICR_b0      (( _bitref )0xD00u )
#define _TM1ICR_b0      (( _bitref )0xD10u )
#define _TM2ICR_b0      (( _bitref )0xD20u )
#define _TM3ICR_b0      (( _bitref )0xD30u )
#define _TM4ICR_b0      (( _bitref )0xD40u )

//KB REG
//----------------------------------------------
#define _PBEN_b0        (( _bitref )0x420u )
#define _PBDIR_b0       (( _bitref )0x430u )
#define _PBOD_b0        (( _bitref )0x440u )
//#define _PBDS_b0        (( _bitref )0x450u )
#define _PBID_b0        (( _bitref )0x460u )
#define _PBODN_b0       (( _bitref )0x470u )
#define _PBPH_b0        (( _bitref )0x480u )
#define _PBPIE_b0       (( _bitref )0x490u )
#define _PBPIP_b0       (( _bitref )0x4A0u )
#define _PBPIF_b0       (( _bitref )0x4B0u )
#define _PBICR_b0       (( _bitref )0xD90u )

#define _PCEN_b0        (( _bitref )0x4C0u )
#define _PCDIR_b0       (( _bitref )0x4D0u )
#define _PCOD_b0        (( _bitref )0x4E0u )
#define _PCDS_b0        (( _bitref )0x4F0u )
#define _PCID_b0        (( _bitref )0x500u )
#define _PCPH_b0        (( _bitref )0x510u )
#define _PCPIE_b0       (( _bitref )0x520u )
//#define _PCPIP_b0       (( _bitref )0x530u )
#define _PCPIF_b0       (( _bitref )0x540u )
#define _PCICR_b0       (( _bitref )0xDA0u )

#define _PDEN_b0        (( _bitref )0x550u )
#define _PDDIR_b0       (( _bitref )0x560u )
#define _PDOD_b0        (( _bitref )0x570u )
#define _PDDS_b0        (( _bitref )0x580u )
#define _PDID_b0        (( _bitref )0x590u )
#define _PDPH_b0        (( _bitref )0x5A0u )

#define _P80CON_b0      (( _bitref )0x710u )

#define _UARSR_b0       (( _bitref )0x860u )

#define _MODCON_b0      (( _bitref )0x400u )

#define _ADCCON_b0      (( _bitref )0xB80u )

#define _SP1CR_b0       (( _bitref )0x8A0u )
#define _SP1SR_b0       (( _bitref )0x8B0u )

#define _SP2CR_b0       (( _bitref )0x8D0u )
#define _SP2SR_b0       (( _bitref )0x8E0u )

#define _I2CCR_b0       (( _bitref )0x7C0u )

#endif /*_KBDM952_H_*/
