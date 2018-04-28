// Decleare bit referece macros for SFR, to be used with
// intrinsic bit operations like _bit_get, _bitSET, etc.

#ifndef _INC_SFR
#define _INC_SFR 

// include definition of _bitref
#include <intrinsic.h>


#define _R0_b0   ((_bitref)(0xf00U))
#define _R1_b0   ((_bitref)(0xf10U))
#define _R2_b0   ((_bitref)(0xf20U))
#define _R3_b0   ((_bitref)(0xf30U))
#define _R4_b0   ((_bitref)(0xf40U))
#define _R5_b0   ((_bitref)(0xf50U))
#define _R6_b0   ((_bitref)(0xf60U))
#define _R7_b0   ((_bitref)(0xf70U))
#define _R8_b0   ((_bitref)(0xf80U))
#define _R9_b0   ((_bitref)(0xf90U))
#define _R10_b0  ((_bitref)(0xfa0U))
#define _R11_b0  ((_bitref)(0xfb0U))
#define _R12_b0  ((_bitref)(0xfc0U))
#define _R13_b0  ((_bitref)(0xfd0U))
#define _R14_b0  ((_bitref)(0xfe0U))
#define _R15_b0  ((_bitref)(0xff0U))

// Not all bits of GPR are defined, here are some examples you can use.
//#define _R0_b1	    ((_bitref)(0xf01U))
//#define _R7_b1	    ((_bitref)(0xf71U))
//#define _R7_b11	    ((_bitref)(0xf7bU))
//#define _R15_b3	    ((_bitref)(0xfF3U))

// SFRs in [0xF100..0xF16F] can be address with bit address [0..6Fh]
#define _SOCR_b0     ((_bitref)0x00U)
#define _PSR_b0	   ((_bitref)0x10U)
#define _SB_b0	      ((_bitref)0x20U)
#define _STOVUN_b0   ((_bitref)0x30U)
#define _CS_b0	      ((_bitref)0x080U)
#define _RC_b0	      ((_bitref)0x090U)
#define _SRC_b0	   ((_bitref)0x0a0U)
#define _RS_b0	      ((_bitref)0x0b0U)
#define _RE_b0	      ((_bitref)0x0c0U)
#define _PC_b0	      ((_bitref)0x0d0U)
#define _DS0_b0	   ((_bitref)0x100U)
#define _DS1_b0	   ((_bitref)0x110U)
// TODO: define other SFR and bits

#define _SOCR_STKCHK ((_bitref)0x00U)
#define _SOCR_RAM    ((_bitref)0x01U)
#define _SOCR_FR     ((_bitref)0x03U)
#define _SOCR_MAS    ((_bitref)0x04U)
#define _SOCR_NSEG   ((_bitref)0x05U)
#define _SOCR_WS0    ((_bitref)0x06U)
#define _SOCR_WS1    ((_bitref)0x07U)
#define _SOCR_UP     ((_bitref)0x0eU)
#define _SOCR_IE     ((_bitref)0x0fU)

#define _PSR_Z       ((_bitref)0x10U)
#define _PSR_V       ((_bitref)0x11U)
#define _PSR_C       ((_bitref)0x12U)
#define _PSR_N       ((_bitref)0x13U)
#define _PSR_MV      ((_bitref)0x16U)
#define _PSR_MS      ((_bitref)0x17U)
#define _PSR_MX      ((_bitref)0x18U)
#define _PSR_CPRI0   ((_bitref)0x1bU)
#define _PSR_CPRI1   ((_bitref)0x1cU)
#define _PSR_CPRI2   ((_bitref)0x1dU)
#define _PSR_CIRQ    ((_bitref)0x1eU)

//KERNEL
extern __X unsigned int SOCR;
extern __X unsigned int PSR;
extern __X unsigned int SB;
extern __X unsigned int STOVUN;

extern __X unsigned int EMCR1;
extern __X unsigned int EMCR2;
extern __X unsigned int EMCR3;

extern __X unsigned int CS;
extern __X unsigned int RC;
extern __X unsigned int SRC;
extern __X unsigned int RS;
extern __X unsigned int RE;
extern __X unsigned int PC;
extern __X unsigned int DS0;
extern __X unsigned int DS1;
extern __X unsigned int EXR;
extern __X unsigned int IRAMA;
extern __X unsigned int IRAMDH;
extern __X unsigned int IRAMDL;
extern __X unsigned int ES0;
extern __X unsigned int ES1;
extern __X unsigned int XC0;
extern __X unsigned int XM0;
extern __X unsigned int XC1;
extern __X unsigned int XM1;
extern __X unsigned int XC2;
extern __X unsigned int XM2;
extern __X unsigned int YC0;
extern __X unsigned int YM0;
extern __X unsigned int YC1;
extern __X unsigned int YM1;
extern __X unsigned int YC2;
extern __X unsigned int YM2;
extern __X unsigned int MODCON;
extern __X unsigned int USBCON;
extern __X unsigned int RTCTBR;
extern __X unsigned int RTCMST;
extern __X unsigned int RTCHDT;
extern __X unsigned int RTCWDT;
extern __X unsigned int TMR0CR;
extern __X unsigned int TMR0;
extern __X unsigned int TM0PRD;
extern __X unsigned int TMR1CR;
extern __X unsigned int TMR1;
extern __X unsigned int TM1PRD;
extern __X unsigned int TMR2CR;
extern __X unsigned int TMR2;
extern __X unsigned int TM2PRD;
extern __X unsigned int TMR3CR;
extern __X unsigned int TMR3;
extern __X unsigned int TM3PRD;
extern __X unsigned int TMR4CR;
extern __X unsigned int TMR4;
extern __X unsigned int TM4PRD;
extern __X unsigned int PWM0;
extern __X unsigned int PWM1;
extern __X unsigned int PWM2;
extern __X unsigned int PWM3;
extern __X unsigned int PWM4;
extern __X unsigned int PWM5;
extern __X unsigned int PWM6;
extern __X unsigned int PWM7;


extern __X unsigned int TBICR;
extern __X unsigned int TM0ICR;
extern __X unsigned int TM1ICR;
extern __X unsigned int TM2ICR;
extern __X unsigned int TM3ICR;
extern __X unsigned int TM4ICR;
extern __X unsigned int USB2ICR;
extern __X unsigned int USB3ICR;
extern __X unsigned int I2CICR;
extern __X unsigned int ADCICR;
extern __X unsigned int PBICR;
extern __X unsigned int PCICR;
extern __X unsigned int CSICR;
extern __X unsigned int SPI2ICR;
extern __X unsigned int SPI1ICR;
extern __X unsigned int USB1ICR;
extern __X unsigned int MICICR;
extern __X unsigned int CODECICR;
extern __X unsigned int SDICR;
extern __X unsigned int NFICR;
extern __X unsigned int DACRICR;
extern __X unsigned int P80ICR;			
extern __X unsigned int DACLICR;
extern __X unsigned int UART_RX_ICR;
extern __X unsigned int RTCICR;	
extern __X unsigned int UART_TX_ICR;
extern __X unsigned int UARTICR;			

extern __X unsigned int DTUC0;
extern __X unsigned int DTUC1;
extern __X unsigned int DTUC2;
extern __X unsigned int DTUC3;
extern __X unsigned int SRCP0;
extern __X unsigned int DSTP0;
extern __X unsigned int SRCP1;
extern __X unsigned int DSTP1;
extern __X unsigned int SRCP2;
extern __X unsigned int DSTP2;
extern __X unsigned int SRCP3;
extern __X unsigned int DSTP3;	

extern __X unsigned int PBEN;
extern __X unsigned int PBDIR;
extern __X unsigned int PBOD;
extern __X unsigned int PBID;
extern __X unsigned int PBODN;
extern __X unsigned int PBPH;
extern __X unsigned int PBPIE;
extern __X unsigned int PBPIP;
extern __X unsigned int PBPIF;			
extern __X unsigned int PCEN;
extern __X unsigned int PCDIR;
extern __X unsigned int PCOD;
extern __X unsigned int PCDS;
extern __X unsigned int PCID;
extern __X unsigned int PCPH;
extern __X unsigned int PCPIE;
extern __X unsigned int PCPIF;			
extern __X unsigned int PDEN;
extern __X unsigned int PDDIR;
extern __X unsigned int PDOD;
extern __X unsigned int PDDS;
extern __X unsigned int PDID;
extern __X unsigned int PDPH;

extern __X unsigned int I2CCR;
extern __X unsigned int I2CSAR;
extern __X unsigned int I2CCKR;
extern __X unsigned int I2CTDR;
extern __X unsigned int I2CRDR;
extern __X unsigned int SP1CR;
extern __X unsigned int SP1SR;
extern __X unsigned int SP1DR;
extern __X unsigned int SP2CR;
extern __X unsigned int SP2SR;
extern __X unsigned int SP2DR;

extern __X unsigned int SDCCR;
extern __X unsigned int SDCIR;
extern __X unsigned int SDARG1;
extern __X unsigned int SDARG0;
extern __X unsigned int SDRSP0;
extern __X unsigned int SDRSP1;
extern __X unsigned int SDRSP2;
extern __X unsigned int SDRSP3;
extern __X unsigned int SDRSP4;
extern __X unsigned int SDRSP5;
extern __X unsigned int SDRSP6;
extern __X unsigned int SDRSP7;
extern __X unsigned int SDRSP8;
extern __X unsigned int SDCKCR;
extern __X unsigned int SDTMS;
extern __X unsigned int SDND;
extern __X unsigned int SDDBA;
extern __X unsigned int P80CON;
extern __X unsigned int P80CO;
extern __X unsigned int P80CI;
extern __X unsigned int P80DO;
extern __X unsigned int P80DI;
extern __X unsigned int P80NB;
extern __X unsigned int P80TMS;
extern __X unsigned int P80DBA;
extern __X unsigned int CSCON;
extern __X unsigned int CSNB;
extern __X unsigned int CSRDBA;
extern __X unsigned int CSWDBA;


extern __X unsigned int UARCR;
extern __X unsigned int UARSR;
extern __X unsigned int UARTDR;
extern __X unsigned int UARRDR;
extern __X unsigned int UARBR;

extern __X unsigned int SSW0;
extern __X unsigned int SSW1;
extern __X unsigned int SSW2;
extern __X unsigned int SSW3;
extern __X unsigned int SSW4;

extern __X unsigned int CODECCON;
extern __X unsigned int DACLD;
extern __X unsigned int DACRD;
extern __X unsigned int MICD;

extern __X unsigned int ADCCON;
extern __X unsigned int ADCD;

extern __X unsigned int FUSCON;
extern __X unsigned int FUSFIFO;
extern __X unsigned int FUSDAT;

#define SRCP0 (*((__X volatile int*)((unsigned int)0xF0E8)))
#define DSTP0 (*((__X volatile int*)((unsigned int)0xF0EA)))
#define SRCP1 (*((__X volatile int*)((unsigned int)0xF0EC)))
#define DSTP1 (*((__X volatile int*)((unsigned int)0xF0EE)))
#define SRCP2 (*((__X volatile int*)((unsigned int)0xF0F0)))
#define DSTP2 (*((__X volatile int*)((unsigned int)0xF0F2)))
#define SRCP3 (*((__X volatile int*)((unsigned int)0xF0F4)))
#define DSTP3 (*((__X volatile int*)((unsigned int)0xF0F6))) 
#endif /* _INC_SFR */





