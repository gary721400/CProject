//! @file oid.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the oid driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: dacc213.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _OID_H_
#define _OID_H_

//_____ I N C L U D E S ____________________________________________________

#include "conf_kb.h"
#include "conf_oid.h"
#include "power.h"


//#include "GPIO_bitref.H"
//#include "SFR.H"

//_____ D E F I N I T I O N ________________________________________________


//extern void  delay1ms    ( int count );
void  delay10us   ( void );
void  delay100us  ( void );
//#define delay3us()      _nop_()//;_nop_();_nop_()
void  delay3us	  ( void );

extern Bool g_b_oid_enable;
extern Bool bPenOn;

#define scan_oid_enable()                 ( g_b_oid_enable = TRUE )
#define scan_oid_disable()                ( g_b_oid_enable = FALSE )
#define scan_oid_is_enable()              ( g_b_oid_enable == TRUE )
#define scan_oid_is_disable()             ( g_b_oid_enable != TRUE )

extern Bool b_oid_init_flag;
extern _MEM_TYPE_SLOW_ U32 u32_oid_read_timeout;

extern Bool bPenLeave;

#define g_b_bind_flag                     b_oid_init_flag
//#define g_b_wireless_flag                 bPenLeave
#if 1//( OID == ENABLE )
//_____ M A C R O S ________________________________________________________


//------------------------------------------------------------------------------
// Option
//------------------------------------------------------------------------------
//#define PORT            1 // 0: Port A, 1: Port B, 2: Port C, ...
//#define SCK_PIN         8 // 0: bit 0, 1: bit 1, 2: bit 2, ...
//#define SDIO_PIN        7 // 0: bit 0, 1: bit 1, 2: bit 2, ...

//------------------------------------------------------------------------------
//  Define
//------------------------------------------------------------------------------
#if PORT < 0 || PORT > 5
    #error "THE DEFINE PORT NUMBER IS OVER." 
#endif
#if SCK_PIN < 0 || SCK_PIN > 15
    #error "THE DEFINE PIN NUMBER IS OVER." 
#endif
#if SDIO_PIN < 0 || SDIO_PIN > 15
    #error "THE DEFINE PIN NUMBER IS OVER." 
#endif

/*
#if PORT == 0
#define SCK_PORT_PH     _PAPH_b0
#define SCK_PORT_OD     _PAOD_b0
#define SCK_PORT_DIR    _PADIR_b0
#define SDIO_PORT_PH    _PAPH_b0
#define SDIO_PORT_OD    _PAOD_b0
#define SDIO_PORT_ID    _PAID_b0
#define SDIO_PORT_DIR   _PADIR_b0
#define SDIO_PORT_PIE   _PAPIE_b0
#define SDIO_PORT_PIP   _PAPIP_b0
#define SDIO_PORT_PIF   _PAPIF_b0
#define GPIO_TRAP_NO    0x20
#define GPIO_ICR_INIT() PAICR = 0x0001
#elif PORT == 1
#define SCK_PORT_PH     _PBPH_b0
#define SCK_PORT_OD     _PBOD_b0
#define SCK_PORT_DIR    _PBDIR_b0
#define SDIO_PORT_PH    _PBPH_b0
#define SDIO_PORT_OD    _PBOD_b0
#define SDIO_PORT_ID    _PBID_b0
#define SDIO_PORT_DIR   _PBDIR_b0
#define SDIO_PORT_PIE   _PBPIE_b0
#define SDIO_PORT_PIP   _PBPIP_b0
#define SDIO_PORT_PIF   _PBPIF_b0
#define GPIO_TRAP_NO    0x22
#define GPIO_ICR_INIT() PBICR = 0x0001
#elif PORT == 2
#define SCK_PORT_PH     _PCPH_b0
#define SCK_PORT_OD     _PCOD_b0
#define SCK_PORT_DIR    _PCDIR_b0
#define SDIO_PORT_PH    _PCPH_b0
#define SDIO_PORT_OD    _PCOD_b0
#define SDIO_PORT_ID    _PCID_b0
#define SDIO_PORT_DIR   _PCDIR_b0
#define SDIO_PORT_PIE   _PCPIE_b0
#define SDIO_PORT_PIP   _PCPIP_b0
#define SDIO_PORT_PIF   _PCPIF_b0
#define GPIO_TRAP_NO    0x24
#define GPIO_ICR_INIT() PCICR = 0x0001
#elif PORT == 3
#define SCK_PORT_PH     _PDPH_b0
#define SCK_PORT_OD     _PDOD_b0
#define SCK_PORT_DIR    _PDDIR_b0
#define SDIO_PORT_PH    _PDPH_b0
#define SDIO_PORT_OD    _PDOD_b0
#define SDIO_PORT_ID    _PDID_b0
#define SDIO_PORT_DIR   _PDDIR_b0
#define SDIO_PORT_PIE   _PDPIE_b0
#define SDIO_PORT_PIP   _PDPIP_b0
#define SDIO_PORT_PIF   _PDPIF_b0
#define GPIO_TRAP_NO    0x26
#define GPIO_ICR_INIT() PDICR = 0x0001
#elif PORT == 4
#define SCK_PORT_PH     _PEPH_b0
#define SCK_PORT_OD     _PEOD_b0
#define SCK_PORT_DIR    _PEDIR_b0
#define SDIO_PORT_PH    _PEPH_b0
#define SDIO_PORT_OD    _PEOD_b0
#define SDIO_PORT_ID    _PEID_b0
#define SDIO_PORT_DIR   _PEDIR_b0
#define SDIO_PORT_PIE   _PEPIE_b0
#define SDIO_PORT_PIP   _PEPIP_b0
#define SDIO_PORT_PIF   _PEPIF_b0
#define GPIO_TRAP_NO    0x28
#define GPIO_ICR_INIT() PEICR = 0x0001
#elif PORT == 5
#define SCK_PORT_PH     _PFPH_b0
#define SCK_PORT_OD     _PFOD_b0
#define SCK_PORT_DIR    _PFDIR_b0
#define SDIO_PORT_PH    _PFPH_b0
#define SDIO_PORT_OD    _PFCOD_b0
#define SDIO_PORT_ID    _PFID_b0
#define SDIO_PORT_DIR   _PFDIR_b0
#define SDIO_PORT_PIE   _PFPIE_b0
#define SDIO_PORT_PIP   _PFPIP_b0
#define SDIO_PORT_PIF   _PFPIF_b0
#define GPIO_TRAP_NO    0x2A
#define GPIO_ICR_INIT() PFICR = 0x0001
#endif
*/
#if 1
#define SCK_PIN         7 // 0: bit 0, 1: bit 1, 2: bit 2, ...
#define SDIO_PIN        6 // 0: bit 0, 1: bit 1, 2: bit 2, ...

#define SCK_PORT_PH     _PCPH_b0
#define SCK_PORT_OD     _PCOD_b0
#define SCK_PORT_DIR    _PCDIR_b0

#define SDIO_PORT_PH    _PCPH_b0
#define SDIO_PORT_OD    _PCOD_b0
#define SDIO_PORT_ID    _PCID_b0
#define SDIO_PORT_DIR   _PCDIR_b0

//#define SDIO_PORT_PIE   _PBPIE_b0
//#define SDIO_PORT_PIP   _PBPIP_b0
//#define SDIO_PORT_PIF   _PBPIF_b0
#else
#define SCK_PIN         6 // 0: bit 0, 1: bit 1, 2: bit 2, ...
#define SDIO_PIN        5 // 0: bit 0, 1: bit 1, 2: bit 2, ...
#define SCK_PORT_PH     _PBPH_b0
#define SCK_PORT_OD     _PBOD_b0
#define SCK_PORT_DIR    _PBDIR_b0

#define SDIO_PORT_PH    _PBPH_b0
#define SDIO_PORT_OD    _PBOD_b0
#define SDIO_PORT_ID    _PBID_b0
#define SDIO_PORT_DIR   _PBDIR_b0
#define SDIO_PORT_PIE   _PBPIE_b0
#define SDIO_PORT_PIP   _PBPIP_b0
#define SDIO_PORT_PIF   _PBPIF_b0
#endif

/*
#define SCK_INIT()      _bitSET(SCK_PORT_PH+SCK_PIN);\
                        _bitSET(SCK_PORT_OD+SCK_PIN);\
                        _bitCLR(SCK_PORT_DIR+SCK_PIN)

#define SDIO_INIT()     _bitSET(SDIO_PORT_PH+SDIO_PIN);\
                        _bitSET(SDIO_PORT_OD+SDIO_PIN);\
                        _bitCLR(SDIO_PORT_DIR+SDIO_PIN);\
                        _bitSET(SDIO_PORT_PIE+SDIO_PIN);\
                        _bitCLR(SDIO_PORT_PIP+SDIO_PIN);\
                        _bitCLR(SDIO_PORT_PIF+SDIO_PIN)

*/

#define SCK_OUT()       _bitCLR(SCK_PORT_DIR+SCK_PIN)
#define SCK_IN()		_bitSET(SCK_PORT_DIR+SCK_PIN)
#define SCK_HIGH()      _bitSET(SCK_PORT_OD+SCK_PIN)
#define SCK_LOW()       _bitCLR(SCK_PORT_OD+SCK_PIN)
#define SDIO_OUT()      _bitCLR(SDIO_PORT_DIR+SDIO_PIN)
#define SDIO_IN()       _bitSET(SDIO_PORT_DIR+SDIO_PIN)
#define SDIO_HIGH()     _bitSET(SDIO_PORT_OD+SDIO_PIN)
#define SDIO_LOW()      _bitCLR(SDIO_PORT_OD+SDIO_PIN)
//#define CLR_SDIO_PIF()  _bitCLR(SDIO_PORT_PIF+SDIO_PIN)
#define GET_SDIO()      _bit_get(SDIO_PORT_ID+SDIO_PIN)
//------------------------------------------------------------------------------

#define SCK                               //P4_1
#define SDIO                              //P4_2
                        

#define oid_power_on()                   // OID_PWR_ON()//( P2_0 = 0 )
#define oid_power_off()                 //  OID_PWR_OFF()//( P2_0 = 1 )
#define oid_dc_dc_on()
#define oid_dc_dc_off()

#define pen_enable()                      //( P4DIR |= BIT3, P4 |= BIT3 )
#define pen_disable()                     //( P4DIR |= BIT3, P4 &= ~BIT3 )


#define sck_out()                         SCK_OUT()
#define sck_in()                          SCK_IN()
#define set_sck()                         SCK_HIGH()
#define clr_sck()                         SCK_LOW()
#define sdio_out()                        SDIO_OUT()
#define sdio_in()                         SDIO_IN()
#define set_sdio()                        SDIO_HIGH()
#define clr_sdio()                        SDIO_LOW()
#define read_sdio()                       GET_SDIO()

#define oid_sdio_input()


typedef enum{
    NO_WAIT = 0,
    WAIT_CODE = 1
}tOID_Mode;

/*
typedef struct{
    unsigned Not_used : 9;          // bit 31 ~ 23
    unsigned Oid_bit : 1;           // bit 22
    unsigned Commnd_or_Index : 1;   // bit 21
    unsigned Battery_status : 1;    // bit 20
    unsigned Reserved : 2;          // bit 19 ~ 18
    unsigned Extended : 2;          // bit 17 ~ 16
    unsigned Index_number : 16;     // bit 15 ~ 0
}tOID_Packet;
*/




//command from OID to DSP
//#if( OID_VERSION == OID1 )
#define OID1Cmd_PowerOn                    0xFFF8
#define OID1Cmd_PowerDown                  0xFFF7

//index from OID
#define OID1_KEY_PRESS                     0x3FFE
#define OID1_KEY_RELEASE                   0x5FFE
#define OID1_INVALID_INDEX                 0x1FFF  //mapping error
#define OID1_PAGE_INDEX                    0x10C0  //4288
#define OID1_IDLE_INDEX                    0x1000
#define OID1_ISOLATION_INDEX               0x1001
#define OID1_NULL_INDEX                    0x1FFE

//#elif( OID_VERSION == OID2 )
#define OID2Cmd_PowerOn                    0x60FFF8u
#define OID2Cmd_PowerDown                  0x60FFF7u
#define OID2Cmd_SystemReset                0x60FFF1u

#define OID2_IndexType_Normal_DontCare     0x3FFFBu
#define OID2_IndexType_Normal_Missing_Min  0x3FFFCu
#define OID2_IndexType_Normal_Missing_Max  0x3FFFFu

//#endif


//command from DSP to OID
#define UserCmd_PowerOnOID                0x53
#define UserCmd_PowerDownOID              0x56
#define UserCmd_AutoSleepFunDisable       0xA3

#define UserCmd_SetCal1                   0x0101
#define UserCmd_SetCal2                   0x0201
#define UserCmd_SetCal3                   0x0e04

#define OIDCmd_Setcab1_Ack                0x700003u
#define OIDCmd_Setcab2_Ack                0x700004u
#define OIDCmd_SetCal3_Ack                0x70002Du

#define UserCmd_Calib_Trigger0            0x0501
#define UserCmd_Calib_Trigger1            0x00C8
#define UserCmd_Calib_Trigger2            0x0300

#define UserCmd_Calib_Trigger3            0x0502
#define OIDCmd_CalibTrigger_Ack1          0x700024u


#define OIDCmd_CalibTrigger_Ack           0x700007u
#define OIDCmd_Calib_ReportStatus         0x700000u

#define UserCmd_RestartSys                0x6300

#define OIDCmd_RestartSys_Ack             0x70000Au

#define UserCmd_SetMode                   0x02AA
#define UserCmd_SetModeParam1             0x1800
#define UserCmd_SetModeParam2             0x0945
#define OIDCmd_SetMode_Ack                0x700001u

#define UserCmd_SetGroup                  0x0801
#define UserCmd_SetGroupParam1            0x0002
#define UserCmd_SetGroupParam2            0x0000
#define OIDCmd_SetGroup_Ack               0x70000Fu

#define UserCmd_SetOid1                   0x0B01
#define UserCmd_SetOid1Param1             0x0008
#define UserCmd_SetOid1Param2             0x0000
#define OIDCmd_SetOid1_Ack                0x70001Du

#define OID__NORMAL_GROUP                 0x00
#define OID_WAIYAN_GROUP                  0x01
#define OID_FUZHOU_GROUP                  0x02
#define OID_GROUP3                        0x03


//control oid pen power
#define OID_POWER_ON
#define OID_POWER_OFF
#if 0
#define OID_INT_ENABLE()				
#define OID_INT_DISABLE()				
#else
#define OID_INT_ENABLE()				Int_enable()
#define OID_INT_DISABLE()				Int_disable()
#endif
//_____ D E C L A R A T I O N ______________________________________________
//_GLOBEXT_ _MEM_TYPE_SLOW_ U16 u16_current_index;

extern __X  Bool         index_flag;
extern  _MEM_TYPE_SLOW_  U32                  Current_Index; 

#define u16_current_index                 Current_Index

void  oid_pen_init   ( void );
void  reinit_oid_pen ( void );

void  turn_off_oid   ( void );
void  scan_oid       ( void );


#else
#define oid_power_on()
#define oid_power_off()
#define oid_dc_dc_on()
#define oid_dc_dc_off()
#define pen_enable()
#define pen_disable()
#define oid_sdio_input()

//#define scan_oid_enable()
//#define scan_oid_disable()

#define oid_pen_init()
#define reinit_oid_pen()
#define turn_off_oid()
#define scan_oid()
#endif	//  #if( OID == ENABLE )

void  delay10us(void);
//void  Delay_ms(int count);

#endif  // _OID_H_

