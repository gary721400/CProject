//! @file e2radio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: e2radio.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _E2RADIO_H_
#define _E2RADIO_H_

//_____ I N C L U D E S ____________________________________________________

#include "lpradio.h"
#include "config.h"
#include "conf_sio.h"
//_____ M A C R O S ________________________________________________________
#define RF_WRITE_MODE       0x80
#define RF_AUTO_INC         0x40
#define RF_READ_MODE        0x00

//_____ D E F I N I T I O N ________________________________________________
#undef _GLOBEXT_
#if (defined _e2radio_c_)
# define _GLOBEXT_ extern
#else
# define _GLOBEXT_ extern
#endif
/*
#define RF_MISO_DIR   (_PBDIR_b0+12)
#define RF_MISO_PH    (_PBPH_b0+12)
#define RF_MISO_ID    (_PBID_b0+12)

#define RF_MOSI_DIR   (_PBDIR_b0+11)
#define RF_MOSI_PH    (_PBPH_b0+11)
#define RF_MOSI_OD    (_PBOD_b0+11)

#define RF_SCK_DIR    (_PBDIR_b0+13)
#define RF_SCK_PH     (_PBPH_b0+13)
#define RF_SCK_OD     (_PBOD_b0+13)

#define RF_SS_DIR     (_PBDIR_b0+10)
#define RF_SS_PH      (_PBPH_b0+10)
#define RF_SS_OD      (_PBOD_b0+10)

#define RF_IRQ_DIR    (_PCDIR_b0+8)
#define RF_IRQ_PH     (_PCPH_b0+8)
#define RF_IRQ_ID     (_PCID_b0+8)

#define RF_RST_DIR    (_PBDIR_b0+7)
#define RF_RST_PH     (_PBPH_b0+7)
#define RF_RST_OD     (_PBOD_b0+7)
*/


#define RF_RST_DIR               ( _PCDIR_b0 + 3 )
#define RF_RST                   ( _PCOD_b0 + 3 )

#define clr_rf_rst()             ( _bitCLR( RF_RST ))
#define set_rf_rst()             ( _bitSET( RF_RST ))

#define RF_CSB_DIR               ( _PCDIR_b0 + 10 )
#define RF_CSB                   ( _PCOD_b0 + 10 )

#define RF_CLK_DIR               ( _PBDIR_b0 + 3 )
#define RF_CLK                   ( _PBOD_b0 + 3 )

#define RF_DIN_DIR               ( _PBDIR_b0 + 1 )
#define RF_DIN                   ( _PBOD_b0 + 1 )

#define RF_DOUT_DIR              ( _PBDIR_b0 + 2 )
#define RF_DOUT                  ( _PBID_b0 + 2 )

#define RF_IRQ_DIR               ( _PBDIR_b0 + 15 )
#define RF_IRQ_PH                ( _PBPH_b0 + 15 )

#define RF_IRQ                   ( _PBID_b0 + 15 )
#define IRQ                      ( !_bit_get( RF_IRQ ))


#if( RF_SPI_MODE == DISABLE )
//#define Set_rf_csb()          ( _bitSET( RF_CSB ))
//#define Clr_rf_csb()          ( _bitCLR( RF_CSB ))
#endif
#define Set_rf_clk()          ( _bitSET( RF_CLK ))
#define Clr_rf_clk()          ( _bitCLR( RF_CLK ))
#define Set_rf_din()          ( _bitSET( RF_DIN ))
#define Clr_rf_din()          ( _bitCLR( RF_DIN ))
#define Out_rf_din()          ( _bitCLR( RF_DIN_DIR ))
#define In_rf_din()           ( _bitSET( RF_DIN_DIR ))
#define Read_rf_dout()        ( _bit_get( RF_DOUT ))


//_____ D E C L A R A T I O N ______________________________________________

_GLOBEXT_ RADIO_BUFFER_PTR  RadioWipPtr;
_GLOBEXT_ RADIO_LENGTH      RadioWipLen;
_GLOBEXT_ RADIO_BUFFER_PTR  RadioPtr;
_GLOBEXT_ RADIO_LENGTH      RadioLen;

_GLOBEXT_ _MEM_TYPE_SLOW_ U16   rf_SP1CR;
_GLOBEXT_ _MEM_TYPE_SLOW_ U16   nor_SP1CR;

void  RadioReset                ( void );
void  RadioWrite                ( RADIO_REG_ADDR addr, U8 value );
U8    RadioRead                 ( RADIO_REG_ADDR addr );
U8    RadioReadStatusDebounced  ( RADIO_REG_ADDR addr );
void  RadioSetPtr               ( RADIO_BUFFER_PTR ramPtr );
void  RadioSetLength            ( RADIO_LENGTH length );
void  RadioBurstWriteWip        ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioFileWrite            ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioFileRead             ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioFileReadWip          ( RADIO_REG_ADDR addr, RADIO_LENGTH length );


#endif  // _E2RADIO_H_

