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
//#include <KBDM9_DDK.H>
#include "system.H"

#define RF_SPI_MODE           ENABLE

//_____ M A C R O S ________________________________________________________
#define RF_WRITE_MODE       0x80
#define RF_AUTO_INC         0x40
#define RF_READ_MODE        0x00

//=====rf spi2 interface
#define RF_MISO_DIR   (_PBDIR_b0+2)
#define RF_MISO_PH    (_PBPH_b0+2)
#define RF_MISO_ID    (_PBID_b0+2)

#define RF_MOSI_DIR   (_PBDIR_b0+1)
#define RF_MOSI_PH    (_PBPH_b0+1)
#define RF_MOSI_OD    (_PBOD_b0+1)

#define RF_SCK_DIR    (_PBDIR_b0+3)
#define RF_SCK_PH     (_PBPH_b0+3)
#define RF_SCK_OD     (_PBOD_b0+3)

#define RF_SS_DIR     (_PBDIR_b0+0)
#define RF_SS_PH      (_PBPH_b0+0)
#define RF_SS_OD      (_PBOD_b0+0)



//========rf control interface
#define RF_IRQ_DIR    (_PBDIR_b0+15)
#define RF_IRQ_PH     (_PBPH_b0+15)
#define RF_IRQ_ID     (_PBID_b0+15)
#define RF_IRQ_OD     (_PBOD_b0+15)
#define RF_RST_DIR    (_PCDIR_b0+3)
#define RF_RST_PH     (_PCPH_b0+3)
#define RF_RST_OD     (_PCOD_b0+3)


#define set_rf_rst()          ( _bitSET(RF_RST_OD))
#define clr_rf_rst()          ( _bitCLR(RF_RST_OD))

#if 1
//#define Set_rf_csb()          ( _bitSET(RF_SS_OD))
//#define Clr_rf_csb()          ( _bitCLR(RF_SS_OD))
//#define  RF_CSB               RF_SS_OD

#define Set_rf_clk()          ( _bitSET(RF_SCK_OD))
#define Clr_rf_clk()          ( _bitCLR(RF_SCK_OD))

/*
#define Set_rf_din()          ( _bitSET(RF_MISO_ID))
#define Clr_rf_din()          ( _bitCLR(RF_MISO_ID))

#define Out_rf_din()          ( _bitCLR(RF_MISO_DIR))
#define In_rf_din()           ( _bitSET(RF_MISO_DIR))

#define Read_rf_dout()        ( _bit_get(RF_MOSI_OD))
*/
#define Set_rf_din()          ( _bitSET(RF_MOSI_OD))
#define Clr_rf_din()          ( _bitCLR(RF_MOSI_OD))

#define Out_rf_din()          ( _bitSET(RF_MOSI_DIR))//set --dir is input
#define In_rf_din()           ( _bitCLR(RF_MOSI_DIR))//clr---dir is output

#define Read_rf_dout()        ( _bit_get(RF_MISO_ID))

#else
#define Set_rf_csb()          ( PORTB |= 1 << RF_CSB )
#define Clr_rf_csb()          ( PORTB &= ~( 1 << RF_CSB ))
#define Set_rf_clk()          ( PORTB |= 1 << RF_CLK )
#define Clr_rf_clk()          ( PORTB &= ~( 1 << RF_CLK ))
#define Set_rf_din()          ( PORTB |= 1 << RF_DIN )
#define Clr_rf_din()          ( PORTB &= ~( 1 << RF_DIN ))
#define Out_rf_din()          ( DDRB |= 1 << RF_DIN )
#define In_rf_din()           ( DDRB &= ~( 1 << RF_DIN ))
#define Read_rf_dout()        ( PINB &( 1 << RF_DOUT ))
#endif




//_____ D E F I N I T I O N ________________________________________________


//#define IRQ                 ( PIND &( 1 << RF_IRQ ))
#define IRQ                 ( !_bit_get(RF_IRQ_ID))

//_____ D E C L A R A T I O N ______________________________________________

extern RADIO_BUFFER_PTR  RadioWipPtr;
extern RADIO_LENGTH      RadioWipLen;
extern RADIO_BUFFER_PTR  RadioPtr;
extern RADIO_LENGTH      RadioLen;

extern _MEM_TYPE_SLOW_ U16   rf_SP1CR;
extern _MEM_TYPE_SLOW_ U16   nor_SP1CR;

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

