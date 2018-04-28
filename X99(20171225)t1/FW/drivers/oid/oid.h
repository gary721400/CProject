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

#include "lib_mcu\compiler.h"
#include "conf\conf_oid.h"

#include "mmi\shared\com_var.h"
//#include <intrins.h>

//_____ D E F I N I T I O N ________________________________________________
#undef _GLOBEXT_
#if (defined _oid_c_)
# define _GLOBEXT_ extern
#else
# define _GLOBEXT_ extern
#endif

void  delay1ms    ( U16 );
void  delay10us   ( void );
void  delay100us  ( void );
//#define delay3us()      _nop_()//;_nop_();_nop_()
void  delay3us    ( void );

_GLOBEXT_ Bool g_b_oid_enable
#if 0//def _oid_c_
  = FALSE
#endif
;
#define scan_oid_enable()                 ( g_b_oid_enable = TRUE )
#define scan_oid_disable()                ( g_b_oid_enable = FALSE )
#define scan_oid_is_enable()              ( g_b_oid_enable == TRUE )
#define scan_oid_is_disable()             ( g_b_oid_enable != TRUE )
/*
_GLOBEXT_ Bool b_oid_init_flag
#if 0//def _oid_c_
   = FALSE
#endif
;//
*/
_GLOBEXT_ _MEM_TYPE_MEDSLOW_ U32 u32_oid_read_timeout
#if 0//def _oid_c_
   = 0
#endif
;

_GLOBEXT_ _MEM_TYPE_MEDSLOW_ U32 u32_oid_read_time;

_GLOBEXT_ Bool bPenLeave
#if 0//def _oid_c_
   = TRUE
#endif
;

//#define g_oid                             Search_Record.news_sys.flag_index
_GLOBEXT_   Bool     g_oid
#if 0//def _oid_c_
   = FALSE
#endif
;

//#define g_b_bind_flag                     b_oid_init_flag
//#define g_b_wireless_flag                 bPenLeave
#if( OID == ENABLE )
//_____ M A C R O S ________________________________________________________

#define SCK_DIR                           ( _PBDIR_b0 + 8 )
#define SCK                               ( _PBOD_b0 + 8 )

#define SDIO_DIR                          ( _PBDIR_b0 + 7 )
#define SDIO_PH                           ( _PBPH_b0 + 7 )
#define SDIO                              ( _PBOD_b0 + 7 )
#define SDIO_IN                           ( _PBID_b0 + 7 )

#define P_EN_DIR                          ( _PBDIR_b0 + 6 )
#define P_EN                              ( _PBOD_b0 + 6 )
//#define OID_DC_DC_CTL                     P3_0

//#define OID_CTRL                          P2_0
#define oid_power_on()                    //( OID_CTRL = 1 )
#define oid_power_off()                   //( OID_CTRL = 0 )
#define oid_dc_dc_on()
#define oid_dc_dc_off()

#define pen_enable()                      ( _bitSET( P_EN ))
#define pen_disable()                     ( _bitCLR( P_EN ))

#define sck_out()                         //( _bitCLR( SCK_DIR ))
#define sck_in()                          ( _bitSET( SCK_DIR ))
#define set_sck()                         ( _bitSET( SCK ))
#define clr_sck()                         ( _bitCLR( SCK ))
#define sdio_out()                        ( _bitCLR( SDIO_DIR ))
#define sdio_in()                         ( _bitSET( SDIO_DIR ))
#define set_sdio()                        ( _bitSET( SDIO ))
#define clr_sdio()                        ( _bitCLR( SDIO ))
#define read_sdio()                       ( _bit_get( SDIO_IN ))

#define oid_sdio_input()

//command from OID to DSP
#if( OID_VERSION == OID1 )
#define OIDCmd_PowerOn                    0xFFF8
#define OIDCmd_PowerDown                  0xFFF7

//index from OID
#define OID_KEY_PRESS                     0x3FFE
#define OID_KEY_RELEASE                   0x5FFE
#define OID_INVALID_INDEX                 0x1FFF  //mapping error
#define OID_PAGE_INDEX                    0x10C0  //4288
#define OID_IDLE_INDEX                    0x1000
#define OID_ISOLATION_INDEX               0x1001
#define OID_NULL_INDEX                    0x1FFE

#elif( OID_VERSION == OID2 )
#define OIDCmd_PowerOn                    0x60FFF8u
#define OIDCmd_PowerDown                  0x60FFF7u
#define OIDCmd_SystemReset                0x60FFF1u

#define OID_IndexType_Normal_DontCare     0x3FFFBu
#define OID_IndexType_Normal_Missing_Min  0x3FFFCu
#define OID_IndexType_Normal_Missing_Max  0x3FFFFu

#endif


//command from DSP to OID
#define UserCmd_PowerOnOID                0x53
#define UserCmd_PowerDownOID              0x56
#define UserCmd_AutoSleepFunDisable       0xA3

#define UserCmd_SetCal1                   0x0101
#define UserCmd_SetCal2                   0x0201

#define UserCmd_Calib_Trigger0            0x0501
#define UserCmd_Calib_Trigger1            0x00C8
#define UserCmd_Calib_Trigger2            0x0300

#define OIDCmd_CalibTrigger_Ack           0x700007u
#define OIDCmd_Calib_ReportStatus         0x700000u

#define UserCmd_RestartSys                0x6300

#define OIDCmd_RestartSys_Ack             0x70000Au

#if 1
#define UserCmd_SetMode                   0x02AA
#define UserCmd_SetModeParam1             0x1800
#define UserCmd_SetModeParam2             0x0945
#define OIDCmd_SetMode_Ack                0x700001u

#define UserCmd_SetGroup                  0x0801
#define UserCmd_SetGroupParam1            0x0002
#define UserCmd_SetGroupParam2            0x0000
#define OIDCmd_SetGroup_Ack               0x70000Fu
//#else


#if 1
#define UserCmd_Waiyan_Command_01_Param1    0x64
#define UserCmd_Waiyan_Ack_01    			0x70000C

#define UserCmd_Waiyan_Command_02_Param1    0x02AA
#define UserCmd_Waiyan_Command_02_Param2    0x0F88
#define UserCmd_Waiyan_Command_02_Param3    0x0432
#define UserCmd_Waiyan_Ack_02    			0x700001

#define UserCmd_Waiyan_Command_03_Param1    0x65
#define UserCmd_Waiyan_Ack_03    			0x70000D

#define UserCmd_Waiyan_Command_04_Param1    0x69
#define UserCmd_Waiyan_Ack_04    			0x70001F


#define UserCmd_Waiyan_Command_05_Param1    0x0D01
#define UserCmd_Waiyan_Command_05_Param2    0x000A
#define UserCmd_Waiyan_Command_05_Param3    0x0000
#define UserCmd_Waiyan_Ack_05    			0x700021

#define UserCmd_Waiyan_Command_06_Param1    0x02AA
#define UserCmd_Waiyan_Command_06_Param2    0x0C7A
#define UserCmd_Waiyan_Command_06_Param3    0x01FC
#define UserCmd_Waiyan_Ack_06    			0x700001

#define UserCmd_Waiyan_Command_07_Param1    0x02AA
#define UserCmd_Waiyan_Command_07_Param2    0x0A23
#define UserCmd_Waiyan_Command_07_Param3    0x09B4
#define UserCmd_Waiyan_Ack_07    			0x700001


#define UserCmd_Waiyan_Command_08_Param1    0x70
#define UserCmd_Waiyan_Ack_08    			0x700020


#else
#define UserCmd_SetMode_Waiyan              0x4281
#define UserCmd_SetModeParam1_Waiyan        0xa325
#define UserCmd_SetModeParam2_Waiyan        0x9743
#define OIDCmd_SetMode_Ack_Waiyan           0x700014u

#define UserCmd_SetMode_Waiyan              0xB679
#define UserCmd_SetModeParam1_Waiyan        0x9866
#define UserCmd_SetModeParam2_Waiyan        0x6785
#define OIDCmd_SetMode_Ack_Waiyan           0x700013u

#endif


#define UserCmd_SetMode_2                   0xc601
#define UserCmd_SetModeParam1_2             0xa8cd
#define UserCmd_SetModeParam2_2             0x2493
#define OIDCmd_SetMode_Ack_2                0x700016u

#define UserCmd_SetMode_3                  	0x9c05
#define UserCmd_SetModeParam1_3            	0x8520
#define UserCmd_SetModeParam2_3            	0x1177
#define OIDCmd_SetMode_Ack_3                0x700018u
#endif

#define OID_NORMAL_GROUP                  0x00
#define OID_WAIYAN_GROUP                  0x01
#define OID_FUZHOU_GROUP                  0x02
#define OID_GROUP3                        0x03

#define OIDCmd_Setcab1_Ack                0x700003u
#define OIDCmd_Setcab2_Ack                0x700004u


#define OID_SONIX_2_COMMAND                    0x73
#define OID_SONIX_2_COMMAND_ACK		           0x700026u

#define OID_DPR3_COMMAND                       0x72
#define OID_DPR3_COMMAND_ACK		           0x700025u

#define OID_THRID_COMMAND_PARAM_1              0x02AA
#define OID_THRID_COMMAND_PARAM_2              0x0E08
#define OID_THRID_COMMAND_PARAM_3              0x0004
#define OID_THRID_COMMAND_ACK		           0x700001u

//control oid pen power
#define OID_POWER_ON
#define OID_POWER_OFF
//_____ D E C L A R A T I O N ______________________________________________
_GLOBEXT_   _MEM_TYPE_MEDSLOW_   U16   Current_Index;

_GLOBEXT_                        Bool  exit_flag; //exit main flag

#define u16_current_index                 Current_Index

void  oid_pen_init   ( void );
void  reinit_oid_pen ( void );
#if( OID_VERSION == OID1)
U16 read_oid_pen( void );
#endif
#if( OID_VERSION == OID2)
U32 read_oid_pen( void );
#endif

void  turn_off_oid   ( void );
void  scan_oid       ( void );
#if( T01OID == ENABLE )
void write_t01_pen( U16 val );
void write_t01_cmd( U16 val1, U16 val2, U16 val3 );
void set_t01_mode( U16 mode_pam1,U16 mode_pam2,U16 mode_pam3,U32 mode_ack);
Bool set_t01_one_mode( U8 mode_pam1,U32 mode_ack);
#endif

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

#endif  // _OID_H_

