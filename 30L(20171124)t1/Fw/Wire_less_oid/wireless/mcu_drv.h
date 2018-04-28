//! @file mcu_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the AT8xC51SND3 driver definition.
//!
//! @version 1.37 snd3-dvk-1_9_5
//!
//! @todo
//! @bug

#ifndef _MCU_DRV_H_
#define _MCU_DRV_H_

// ____ I N C L U D E S ____________________________________________________


// ____ M A C R O S ________________________________________________________


// ____ D E F I N I T I O N ________________________________________________

// INTERRUPT NUMBER
//! @defgroup irq_macros Macro interrupt requests level.
//! List of the interrupt levels for each macro
//! @{
#define IRQ_INT0    0
#define IRQ_T0      1
#define IRQ_INT1    2
#define IRQ_T1      3
#define IRQ_SIO     4
#define IRQ_DFC     5
#define IRQ_AUP     6
#define IRQ_USB     7
#define IRQ_KB      8
#define IRQ_PSI     9
#define IRQ_SPI    10
#define IRQ_NFC    11
#define IRQ_MMC    12
//! @}


// C51 CORE
#define MSK_PPS               0x0F  //PPCON
#define MSK_PAGE0             0x01
#define MSK_PAGE1             0x02
#define MSK_PAGE2             0x03
#define MSK_PAGE3             0x04


// POWER & SYSTEM MANAGEMENT
#define MSK_UVDET             0x80  // PSTA
#define MSK_HVDET             0x40
#define MSK_WDTRST            0x04
#define MSK_EXTRST            0x02
#define MSK_PFDRST            0x01

#define MSK_VBCEN             0x80  // PCON
#define MSK_VBPEN             0x40
#define MSK_DCPBST            0x20  // PCON
#define MSK_GF0               0x10
#define MSK_DCEN              0x08
#define MSK_PMLCK             0x04
#define MSK_PD                0x02
#define MSK_IDL               0x01

#define MSK_GF3               0x08  // AUXR1
#define MSK_DPS               0x01

#define MSK_VBEN              0x80  // VBAT
#define MSK_VBERR             0x40
#define MSK_VB                0x1F


// CLOCK MANAGEMENT
#define MSK_WDX2              0x40  // CKCON
#define MSK_OSCF              0x18
#define MSK_T1X2              0x04
#define MSK_T0X2              0x02
#define MSK_X2                0x01

#define MSK_CKGENE            0x80  // CKEN
#define MSK_PLLEN             0x40
#define MSK_PLOCK             0x10
#define MSK_MMCKEN            0x08
#ifdef SND3
#  define MSK_NFCKEN            0x04
#  define MSK_DFCKEN            0x01
#else // Second silicon and higher
#  define MSK_SIOCKEN           0x02
#  define MSK_DFCKEN            0x01
#endif

#ifdef SND3
#  define MSK_DFCCKS            0xE0  // DFCCLK
#  define MSK_DFCDIV            0x1F
#  define MSK_NFCCKS            0xE0  // NFCCLK
#  define MSK_NFCDIV            0x1F
#else // Second silicon and higher
#  define MSK_DNFCKS            0xE0  // CKSEL
#  define MSK_PLLCKS            0x18
#  define MSK_SIOCKS            0x04
#  define MSK_SYSCKS            0x03

#  define MSK_PLLR              0xF0  // PLLCLK
#  define MSK_PLLN              0x0F
#endif

#define MSK_MMCCKS            0xE0  // MMCCLK
#define MSK_MMCDIV            0x1F


// INTERRUPT
#define MSK_EAUP              0x40  // IEN0
#define MSK_EDFC              0x20
#define MSK_ES                0x10
#define MSK_ET1               0x08
#define MSK_EX1               0x04
#define MSK_ET0               0x02
#define MSK_EX0               0x01

#define MSK_EMMC              0x20  // IEN1
#define MSK_ENFC              0x10
#define MSK_ESPI              0x08
#define MSK_EPSI              0x04
#define MSK_EKB               0x02
#define MSK_EUSB              0x01


// TIMERS
#define MSK_GATE1             0x80  // TMOD
#define MSK_C_T1              0x40
#define MSK_MO1               0x30
#define MSK_GATE0             0x08
#define MSK_C_T0              0x04
#define MSK_MO0               0x03


// WATCHDOG
#define MSK_WTO               0x07  // WDTPRG


// RAM INTERFACE
#define MSK_RA16              0x01  // RDFCH


// SCHEDULER CLOCK MANAGEMENT
#define MSK_T0ETB             0x70  // SCHCLK
#define SHIFT_T0ETB           4


// DATA FLOW CONTROLLER
#define MSK_DFRES             0x40  // DFCON
#define MSK_DFCRCEN           0x10
#define MSK_DFPRIO1           0x08
#define MSK_DFPRIO0           0x04
#define MSK_DFABTM            0x02
#if (! defined SND3) // Second silicon and higher
#  define MSK_DFEN            0x01
#else
#  define MSK_DFEN            0x81
#endif

#if (! defined SND3) // Second silicon and higher
#  define MSK_DFABT1          0x80 // DFCCON
#  define MSK_EOFE1           0x40
#  define MSK_EOFIA1          0x20
#  define MSK_DFABT0          0x08
#  define MSK_EOFE0           0x04
#  define MSK_EOFIA0          0x02
#endif

// NAND FLASH CONTROLLER
#if (! defined SND3)
#  define MSK_NFCON_TRS       0x10  // NFCON
#endif
#define MSK_NFCON_WP          0x08  // NFCON
#define MSK_NFCON_SPZEN       0x04
#define MSK_NFCON_ECCEN       0x02
#define MSK_NFCON_EN          0x01

#define MSK_NFACT_EXT         0x18  // NFACT
#define MSK_NFACT_ACT         0x07

#define MSK_NFINT_SMCTI       0x10  // NFINT
#define MSK_NFINT_ILGLI       0x08
#define MSK_NFINT_ECCRDYI     0x04
#define MSK_NFINT_ECCERRI     0x02
#define MSK_NFINT_STOPI       0x01

#define MSK_NFIEN_SMCTE       0x10  // NFIEN
#define MSK_NFIEN_ILGLE       0x08
#define MSK_NFIEN_ECCRDYE     0x04
#define MSK_NFIEN_ECCERRE     0x02
#define MSK_NFIEN_STOPE       0x01

#if (! defined SND3)
#  define MSK_NFSTA_NECC      0x0E  // NFSTA
#else
#  define MSK_NFSTA_POS       0x1E  // NFSTA
#endif

#define MSK_NFPGCFG_NDB       0xF0  // NFCFG Offset 0
#define MSK_SMPGCFG_NDB       0xF0  //       Offset 1
#if (! defined SND3)
#  define MSK_NFPGCFG_NDBH    0x08
#  define MSK_SMPGCFG_NDBH    0x08
#else
#  define MSK_NFPGCFG_NZONE   0x0F
#  define MSK_SMPGCFG_NZONE   0x0F
#endif
#define MSK_SCFG1_NUMDEV      0x60  //       Offset 2
#define MSK_SCFG1_PDEV        0x1E
#define MSK_SCFG1_SMCEN       0x01
#define MSK_SCFG2_BSIZE       0x03  //       Offset 3


// USB CONTROLLER
// USB General
#define MSK_USBE              0x80  // USBCON
#define MSK_HOST              0x40
#define MSK_FRZCLK            0x20
#define MSK_OTGPADE           0x10
#define MSK_IDTE              0x02
#define MSK_VBUSTE            0x01

#define MSK_SPEED             0x04  // USBSTA
#define MSK_ID                0x02
#define MSK_VBUS              0x01

#define MSK_IDTI              0x02  // USBINT
#define MSK_VBUSTI            0x01

#define MSK_DPACC             0x80  // UDPADDH
#define MSK_DPADDH            0x07

#define MSK_DPADDL            0xFF  // UDPADDL

#define MSK_VBUSRQC           0x01
#define MSK_VBUSREQ           0x02  // OTGCON
#define MSK_HNPREQ            0x20
#define MSK_SRPREQ            0x10
#define MSK_SRPSEL            0x08
#define MSK_VBUSHWC           0x04

#define MSK_STOE              0x20  // OTGIEN
#define MSK_HNPERRE           0x10
#define MSK_ROLEEXE           0x08
#define MSK_BCERRE            0x04
#define MSK_VBERRE            0x02
#define MSK_SRPE              0x01

#define MSK_STOI              0x20  // OTGINT
#define MSK_HNPERRI           0x10
#define MSK_ROLEEXI           0x08
#define MSK_BCERRI            0x04
#define MSK_VBERRI            0x02
#define MSK_SRPI              0x01

// USB Device
#define MSK_RMWKUP            0x02  // UDCON
#define MSK_DETACH            0x01

#define MSK_UPRSMI            0x40  // UDINT
#define MSK_EORSMI            0x20
#define MSK_WAKEUPI           0x10
#define MSK_EORSTI            0x08
#define MSK_SOFI              0x04
#define MSK_MSOFI             0x02
#define MSK_SUSPI             0x01

#define MSK_UPRSME            0x40  // UDIEN
#define MSK_EORSME            0x20
#define MSK_WAKEUPE           0x10
#define MSK_EORSTE            0x08
#define MSK_SOFE              0x04
#define MSK_MSOFE             0x02
#define MSK_SUSPE             0x01

#define MSK_ADDEN             0x80  // UDADDR
#define MSK_UADD              0x7F

#define MSK_FNUMH             0x07  // UDFNUMH

#define MSK_FNUML             0xFF  // UDFNUML

#define MSK_FNCERR            0x10  // UDMFN
#define MSK_MFNUM             0x07

#define MSK_TSTPCKT           0x10  // UDTST
#define MSK_TSTK              0x08
#define MSK_TSTJ              0x04
#define MSK_SPDCONF           0x03

// USB Endpoint
#define MSK_EPNUM             0x07  // UENUM

#define MSK_EPRST             0x7F  // UERST

#define MSK_EPEN              0x01  // UECONX
#define MSK_STALLRQ           0x20
#define MSK_STALLRQC          0x10
#define MSK_RSTDT             0x08
#define MSK_EPNUMS            0x04
#define MSK_DFCRDY            0x02

#define MSK_EPTYPE            0xC0  // UECFG0X
#define MSK_NYETDIS           0x02
#define MSK_EPDIR             0x01
#define MSK_ISOSW             0x08
#define MSK_AUTOSW            0x04

#define MSK_EPSIZE            0x70  // UECFG1X
#define MSK_EPBK              0x0C
#define MSK_ALLOC             0x02

#define MSK_CFGOK             0x80  // UESTA0X, UPSTAX
#define MSK_OVERFI            0x40
#define MSK_UNDERFI           0x20
#define MSK_ZLPSEEN           0x10
#define MSK_DTSEQ             0x0C
#define MSK_NBUSYBK           0x03

#define MSK_CTRLDIR           0x04  // UESTA1X
#define MSK_CURRBK            0x03

#define MSK_FLERRE            0x80  // UEIENX
#define MSK_NAKINE            0x40
#define MSK_NAKOUTE           0x10
#define MSK_RXSTPE            0x08
#define MSK_RXOUTE            0x04
#define MSK_STALLEDE          0x02
#define MSK_TXINE             0x01

//#define MSK_FIFOCON           0x80// UPINTX
#define MSK_NAKINI            0x40
//#define MSK_RWAL              0x20
#define MSK_NAKOUTI           0x10
#define MSK_RXSTPI            0x08
#define MSK_RXOUTI            0x04
#define MSK_STALLI            0x02
#define MSK_TXINI             0x01

#define MSK_FIFOCON           0x80  // UPINTX
#define MSK_NAKEDI            0x40
#define MSK_RWAL              0x20
#define MSK_PERRI             0x10
#define MSK_TXSTPI            0x08
#define MSK_TXOUTI            0x04
#define MSK_RXSTALLI          0x02
#define MSK_RXINI             0x01

#define MSK_DAT               0xFF  // UEDATX

#define MSK_BYCTH             0x07  // UEBCHX

#define MSK_BYCTL             0xFF  // UEBCLX

#define MSK_EPINT             0x7F  // UEINT

// USB Host
#define MSK_RESUME            0x04  // UHCON
#define MSK_RESET             0x02
#define MSK_SOFEN             0x01

#define MSK_HWUPI             0x40  // UHINT
#define MSK_HSOFI             0x20
#define MSK_RXRSMI            0x10
#define MSK_RSMEDI            0x08
#define MSK_RSTI              0x04
#define MSK_DDISCI            0x02
#define MSK_DCONNI            0x01


#define MSK_HWUPE             0x40  // UHIEN
#define MSK_HSOFE             0x20
#define MSK_RXRSME            0x10
#define MSK_RSMEDE            0x08
#define MSK_RSTE              0x04
#define MSK_DDISCE            0x02
#define MSK_DCONNE            0x01

#define MSK_HADDR             0xFF  // UHADDR

//#define MSK_FNUMH             0x07  // UHFNUMH

//#define MSK_FNUML             0xFF  // UHFNUML

#define MSK_FLEN              0xFF  // UHFLEN

// USB Pipe
#define MSK_PNUM              0x07  // UPNUM

#define MSK_PRST              0x7F  // UPRST

#define MSK_PEN               0x01  // UPCONX
#define MSK_PFREEZE           0x40
#define MSK_INMODE            0x20
#define MSK_HAUTOSW           0x10
//#define MSK_RSTDT             0x08
#define MSK_PNUMS             0x04
//#define MSK_DFCRDY            0x02

#define MSK_PTYPE             0xC0  // UPCFG0X
#define MSK_PTOKEN            0x30
#define MSK_TOKEN_SETUP       0x30
#define MSK_TOKEN_IN          0x10
#define MSK_TOKEN_OUT         0x20
#define MSK_PEPNUM            0x0F
#define MSK_EP_DIR            0x80

#define MSK_PSIZE             0x70  // UPCFG1X
#define MSK_PBK               0x0C
//#define MSK_ALLOC             0x02

#define MSK_INTFRQ            0xFF  // UPCFG2X

#define MSK_INRQ              0xFF  // UPINRQX

#define MSK_COUNTER           0x60  // UPERRX
#define MSK_CRC16             0x10
#define MSK_TIMEOUT           0x08
#define MSK_PID               0x04
#define MSK_DATAPID           0x02
#define MSK_DATATGL           0x01
#define MSK_ERROR             0x1F

//#define MSK_FLERRE            0x80  // UPIENX
#define MSK_NAKED             0x40
#define MSK_PERRE             0x10
#define MSK_TXSTPE            0x08
#define MSK_TXOUTE            0x04
#define MSK_RXSTALLE          0x02
#define MSK_RXINE             0x01

#define MSK_PDAT              0xFF  // UPDATX

#define MSK_PBYCTH            0x07  // UPBCHX

#define MSK_PBYCTL            0xFF  // UPBCLX

#define MSK_PINT              0xFF  // UPINT


// MMC CONTROLLER
#define MSK_DPTRR             0x40
#define MSK_CRPTR             0x20
#define MSK_CTPTR             0x10
#define MSK_MBLOCK            0x08
#define MSK_DFMT              0x04
#define MSK_RFMT              0x02
#define MSK_CRCDIS            0x01

#define MSK_BLEN              0xF0  // MMCON1
#define MSK_DATDIR            0x08
#define MSK_DATEN             0x04
#define MSK_RXCEN             0x02
#define MSK_TXCEN             0x01

#define MSK_FCK               0x80  // MMCON2
#define MSK_DCR               0x40
#define MSK_CCR               0x20
#define MSK_DBSIZE            0x18
#define MSK_DATD              0x06
#define MSK_MMCEN             0x01

#define MSK_CSDWP             0x80  // MMSTA
#define MSK_CDET              0x40
#define MSK_CBUSY             0x20
#define MSK_CRC16S            0x10
#define MSK_DATFS             0x08
#define MSK_CRC7S             0x04
#define MSK_WFRS              0x02
#define MSK_HFRS              0x01

#define MSK_CDETI             0x80  // MMINT
#define MSK_EORI              0x40
#define MSK_EOCI              0x20
#define MSK_EOFI              0x10
#define MSK_WFRI              0x08
#define MSK_HFRI              0x04
#define MSK_EOBI              0x02

#define MSK_CDETM             0x80  // MMMSK
#define MSK_EORM              0x40
#define MSK_EOCM              0x20
#define MSK_EOFM              0x10
#define MSK_WFRM              0x08
#define MSK_HFRM              0x04
#define MSK_EOBM              0x02


// AUDIO CONTROLLER
#define MSK_BPEN              0x80  // AUCON
#define MSK_VSURND            0x40
#define MSK_BBOOST            0x20
#define MSK_ACCKEN            0x01

#define MSK_AOSC              0xE0  // ASCON
#define MSK_I2CRUN            0x04
#define MSK_BGEN              0x02
#define MSK_BGSE              0x01

#define MSK_APCMD             0x7F  // APCON0

#define MSK_ABACC             0x20  // APCON1
#define MSK_ABWPR             0x10
#define MSK_ABRPR             0x08
#define MSK_ABSPLIT           0x04
#define MSK_APLOAD            0x02
#define MSK_DAPEN             0x01

#define MSK_APGPI3            0x80  // APINT
#define MSK_APGPI2            0x40
#define MSK_APGPI1            0x20
#define MSK_APGPI0            0x10
#define MSK_APEVTI            0x08
#define MSK_ACLIPI            0x04
#define MSK_APRDYI            0x02
#define MSK_APREQI            0x01

#define MSK_APGPE3            0x80  // APIEN
#define MSK_APGPE2            0x40
#define MSK_APGPE1            0x20
#define MSK_APGPE0            0x10
#define MSK_APEVTE            0x08
#define MSK_ACLIPE            0x04
#define MSK_APRDYE            0x02
#define MSK_APREQE            0x01

#define MSK_DVOL              0x1F  // APxDVOL

#define MSK_AMBSEL            0x40  // ACCON
#define MSK_AMBEN             0x20
#define MSK_AISSEL            0x10
#define MSK_AIEN              0x08
#define MSK_AODRV             0x04
#define MSK_AOSSEL            0x02
#define MSK_AOEN              0x01

#define MSK_AODIS             0x02  // ACAUX
#define MSK_AOPRE             0x01

#define MSK_AOXG              0x1F  // ACOxG

#define MSK_AILPG             0x08  // ACIPG
#define MSK_AIPG              0x07

#define MSK_CSPOL             0x10  // ADICON0
#define MSK_DSIZE             0x08
#define MSK_OVERS             0x06
#define MSK_ADIEN             0x01

#define MSK_JUST              0x1F  // ADICON1


// SPI CONTROLLER
#define MSK_SPR               0x83  // SPCON
#define MSK_SPEN              0x40
#define MSK_SSDIS             0x20
#define MSK_MSTR              0x10
#define MSK_MODE              0x0C
#define MSK_CPOL              0x08
#define MSK_CPHA              0x04

#define MSK_SPIF              0x80  // SPSCR
#define MSK_OVR               0x20
#define MSK_MODF              0x10
#define MSK_SPTE              0x08
#define MSK_UARTM             0x04
#define MSK_SPTEIE            0x02
#define MSK_MODFIE            0x01


// SIO
#define MSK_SIOEN             0x80  // SCON
#define MSK_PMOD              0x60
#define MSK_PBEN              0x10
#define MSK_STOP              0x08
#define MSK_DLEN              0x04
#define MSK_GBIT              0x03

#define MSK_OVSF              0xF0  // SFCON
#define MSK_CTSEN             0x08
#define MSK_RTSEN             0x04
#define MSK_RTSTH             0x03

#define MSK_EOTI              0x20  // SINT
#define MSK_OEI               0x10
#define MSK_PEI               0x08
#define MSK_FEI               0x04
#define MSK_TI                0x02
#define MSK_RI                0x01

#define MSK_EOTIE             0x20  // SIEN
#define MSK_OEIE              0x10
#define MSK_PEIE              0x08
#define MSK_FEIE              0x04
#define MSK_TIE               0x02
#define MSK_RIE               0x01


// PSI
#define MSK_PSEN              0x80  // PSICON
#define MSK_PSBSYE            0x40
#define MSK_PSRUNE            0x20
#define MSK_PSWS              0x1C

#define MSK_PSEMPTY           0x80  // PSISTA
#define MSK_PSBSY             0x40
#define MSK_PSRUN             0x20
#define MSK_PSRDY             0x10

#define MSK_PSSTH             0x7F  // PSISTH


// LCD INTERFACE
#define MSK_BUINV             0x80  // LCDCON0
#define MSK_LCIFS             0x40
#define MSK_ADSUH             0x30
#define MSK_ACCW              0x0F

#define MSK_SLW               0xC0  // LCDCON1
#define MSK_RSCMD             0x20
#define MSK_LCYCW             0x10
#define MSK_LCYCT             0x08
#define MSK_LCEN              0x04
#define MSK_LCRD              0x02
#define MSK_LCRS              0x01

#define MSK_LCBUSY            0x01  // LCDSTA

#define SFT_BUINV             7     // LCDCON0
#define SFT_LCIFS             6
#define SFT_ADSUH             4
#define SFT_ACCW              0

#define SFT_SLW               6     // LCDCON1
#define SFT_RSCMD             5
#define SFT_LCYCW             4
#define SFT_LCYCT             3
#define SFT_LCEN              2
#define SFT_LCRD              1
#define SFT_LCRS              0



// KEYBOARD INTERFACE
#define MSK_KINL              0xF0  // KBCON
#define MSK_KINM              0x0F

#define MSK_KPDE              0x80  // KBSTA
#define MSK_KDCPE             0x40
#define MSK_KDCPL             0x20
#define MSK_KINF              0x0F


// OCD
#define MSK_OCDTX             0x01  // OCDPIN
#define MSK_OCDRX             0x02
#define MSK_OCDRI             0x01  // OSCON
#define MSK_OCDTI             0x02
#define MSK_OCDTBSY           0x80


// ____ D E C L A R A T I O N ______________________________________________

//! The AT8xC51SND3 derivatives implement a SFR pagination mechanism
//! which allows mapping of high number of peripherals in the SFR space.
//! Four pages are acessibles through the PPCON register.
//!
#define Mcu_get_sfr_page()             PPCON
#define Mcu_set_sfr_page(val)          (PPCON = val)
#define Mcu_set_sfr_page0()            (PPS0 = 1)
#define Mcu_set_sfr_page1()            (PPS1 = 1)
#define Mcu_set_sfr_page2()            (PPS2 = 1)
#define Mcu_set_sfr_page3()            (PPS3 = 1)

#define Mcu_set_sfr_page_default()     Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_p4()          Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_p5()          Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_power()       Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_clock()       Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_int()         Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_mem()         Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_t0()          Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_t1()          Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_wdt()         Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_kbd()         Mcu_set_sfr_page0()
#define Mcu_set_sfr_page_sio_init()    Mcu_set_sfr_page0()

#define Mcu_set_sfr_page_dfc()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_usb()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_ram()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_nfc()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_mmc()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_aup1()        Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_spi()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_psi()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_sio()         Mcu_set_sfr_page1()
#define Mcu_set_sfr_page_lcd()         Mcu_set_sfr_page1()

#define Mcu_set_sfr_page_aup2()        Mcu_set_sfr_page2()

#define Mcu_set_sfr_page_ocd()         Mcu_set_sfr_page3()


#endif   // _MCU_DRV_H_
