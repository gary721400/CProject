//! @file e2radio.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: e2radio.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _e2radio_c_
#define _e2radio_c_

//_____ I N C L U D E S ____________________________________________________
#include <KBDM9_DDK.H>
#include "config.h"
#include "compiler.h"
#include "e2radio.h"
#include "lpregs.h"
#include "protocol.h"
//#include "int_drv.h"
#include "system.h"
#include "power.h"
//#include "timer_drv.h"
#include "com.h"

//#define RF_SPI_MODE  DISABLE
#define RF_SPI_MODE  ENABLE

#pragma DATA = E2RADIO_DATA
#pragma CODE = E2RADIO_CODE



//===========================================================================
extern void delay1ms( int delay_time );
//===========================================================================
//_____ M A C R O S ________________________________________________________

//========================================================================
 RADIO_BUFFER_PTR  RadioWipPtr;
 RADIO_LENGTH      RadioWipLen;
 RADIO_BUFFER_PTR  RadioPtr;
 RADIO_LENGTH      RadioLen;

//=============================================================================

_MEM_TYPE_SLOW_ U16   rf_SP1CR;
_MEM_TYPE_SLOW_ U16   nor_SP1CR;


extern U8 rf_recive_data[LEN_RF_RECIVE_DATA];
extern U8 rf_send_data[LEN_RF_RECIVE_DATA];

//_____ D E F I N I T I O N ________________________________________________

void  rf_write_byte     ( U8 b );
U8    rf_read_byte      ( void );
void  RadioBurstWrite   ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioFileWriteWip ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioBurstReadWip ( RADIO_REG_ADDR addr, RADIO_LENGTH length );
void  RadioBurstRead    ( RADIO_REG_ADDR addr, RADIO_LENGTH length );

//_____ D E C L A R A T I O N ______________________________________________

#if( RF_SPI_MODE == ENABLE )
void Clr_rf_csb( void )
{
   _bitSET( _PCOD_b0 + 2 );
   _bitCLR( _PCDIR_b0 + 2 );
   SP1CR = rf_SP1CR;
   __asm
   {
      REP #30
      NOP
   }
   __asm{
		//BCLR PBDIR.0  //set output  
		//BCLR PBOD.0   //clr pb_0

		//BSET PCDIR.10	//set output
		BCLR PCOD.2    
	}
}


void Set_rf_csb( void )
{
//   _bitSET( RF_SS_OD );
   _bitSET( _PCOD_b0 + 2 );
   _bitCLR( _PCDIR_b0 + 2 );
   SP1CR = nor_SP1CR;
   __asm
   {
      REP #30
      NOP
   }
}
U8 rf_spi_master_byte_txrx( U8 val )
{
   //while( SPI2ICR &( 1 << 0 ));
   SP1DR = val;
   while( !( SP1SR&( 1 << 7 )));
   _bitCLR( _SP1SR_b0 + 7 );
   return ( U8 )SP1DR;
}
#endif   //#if( RF_SPI_MODE == ENABLE )


//! RadioReset
//!
//! Hard reset of the FPGA
//!
//! @return
void RadioReset( void )
{
  Set_rf_csb();
  RadioWrite( MODE_OVERRIDE_ADR, MSK_RST );    //RST
}


#if( RF_SPI_MODE == DISABLE )
//! rf_write_byte
//!
//! Write a byte on spi bus
//!
//! @return none
void rf_write_byte( U8 b )
{
  register U8 c;

  for( c = 8; c; c-- )
  {
    if( b & 0x80 )
    {
      Set_rf_din();
      Out_rf_din();
    }
    else
    {
      Clr_rf_din();
      Out_rf_din();
    }
    Set_rf_clk();
    Clr_rf_clk();                         // data bit sampled on rising edge
    b <<= 1;
  }
}


//! rf_read_byte
//!
//! Read an a byte on spi bus
//!
//! @return data at U8 format
U8 rf_read_byte( void )
{
  U8 c;
  U8 dat = 0;

  for( c = 8; c; c-- )
  {
    dat <<= 1;
    Set_rf_clk();
    if( Read_rf_dout())
      dat++;
    Clr_rf_clk();                        // data bit sampled on rising edge
  }
  return dat;
}
#endif  //#if( RF_SPI_MODE == DISABLE )

//! RadioWrite
//!
//! Write a single byte to an Radio register
//!
//! @return none
void RadioWrite( RADIO_REG_ADDR addr, U8 value )
{
#if( RF_SPI_MODE == DISABLE )
  Clr_rf_clk();
  In_rf_din();
  Set_rf_din();                        //FF check if necessary
  addr |= RF_WRITE_MODE;
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  rf_write_byte( addr );                 // Begin Addr transfer
  rf_write_byte( value );                    // Begin Data/Cmd transfer

  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
  addr |= RF_WRITE_MODE;
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  /*SPDR = addr;                 // Begin Addr transfer
  while(!( SPSR &( 1 << 7 )));
  SPDR = value;                    // Begin Data/Cmd transfer
  while(!( SPSR &( 1 << 7 )));
  */
  ( void )rf_spi_master_byte_txrx( addr );   // Begin Addr transfer
  ( void )rf_spi_master_byte_txrx( value );  // Begin Data/Cmd transfer

  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == ENABLE )
}


//! RadioRead
//!
//! Read a single byte from an Radio register
//!
//! @return data at U8 format
U8 RadioRead( RADIO_REG_ADDR addr )
{
#if( RF_SPI_MODE == DISABLE )
  U8 b;

  Clr_rf_clk();
  In_rf_din();
  Set_rf_din();
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  rf_write_byte( addr );                 // Begin Addr transfer
//  Delay_ms(10);
  b = rf_read_byte();                    // Begin Data/Cmd transfer

  Set_rf_csb();                        // cal RF
  
  Int_enable();
  return b;
#endif  //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
  U8 b;

  Int_disable();
  
  Clr_rf_csb();                        // select RF

  /*SPDR = addr;                 // Begin Addr transfer
  while(!( SPSR &( 1 << SPIF )));
  SPDR = 0;
  while(!( SPSR &( 1 << SPIF )));
  b = SPDR;                    // Begin Data/Cmd transfer
  */
  ( void )rf_spi_master_byte_txrx( addr );   // Begin Addr transfer
  b = rf_spi_master_byte_txrx( 0 );  // Begin Data/Cmd transfer
  
  Set_rf_csb();                        // de-select DAC
  
  Int_enable();
  return b;
#endif  //#if( RF_SPI_MODE == ENABLE )
}


//! RadioReadStatusDebounced
//!
//! Read a single byte from the RX_IRQ_STATUS_ADR register and
//! debounce the update of the RXC and RXE bits
//!
//! @return
//!
U8 RadioReadStatusDebounced( RADIO_REG_ADDR addr )
{
#if( RF_SPI_MODE == DISABLE )
  U8 status;
  U8 RadioSpiTemp;
   
  Clr_rf_clk();
  In_rf_din();
  Set_rf_din();
  Int_disable();

  Clr_rf_csb();                        // select RF

  rf_write_byte( addr );                 // Begin Addr transfer
  status = rf_read_byte();               // Begin Data/Cmd transfer
  RadioSpiTemp = status;

  status &= MSK_RXC_IRQ | MSK_RXE_IRQ;
  if( status &&( status !=( MSK_RXC_IRQ | MSK_RXE_IRQ ) ))
  {
    RadioSpiTemp |= rf_read_byte();
  }

  Set_rf_csb();                        // de-select DAC
  
  Int_enable();
  return RadioSpiTemp;
#endif  //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
  U8 status;
  U8 RadioSpiTemp;
   
  Int_disable();

  Clr_rf_csb();                        // select RF

  /*SPDR = addr;                 // Begin Addr transfer
  while(!( SPSR &( 1 << SPIF )));
  SPDR = 0;
  while(!( SPSR &( 1 << SPIF )));
  status = SPDR;               // Begin Data/Cmd transfer
  */
  ( void )rf_spi_master_byte_txrx( addr );         // Begin Addr transfer
  status = rf_spi_master_byte_txrx( 0 );   // Begin Data/Cmd transfer
  RadioSpiTemp = status;

  status &= MSK_RXC_IRQ | MSK_RXE_IRQ;
  if( status &&( status !=( MSK_RXC_IRQ | MSK_RXE_IRQ ) ))
  {
    /*SPDR = 0;
    while(!( SPSR &( 1 << SPIF )));
    RadioSpiTemp |= SPDR;
    */
    RadioSpiTemp |= rf_spi_master_byte_txrx( 0 );
  }

  Set_rf_csb();                        // de-select DAC
  
  Int_enable();
  return RadioSpiTemp;
#endif  //#if( RF_SPI_MODE == ENABLE )
}


//! RadioSetPtr
//!
//! Set the buffer pointer address for RadioBurstRead
//!
//! @return data at U8 format
void RadioSetPtr( RADIO_BUFFER_PTR ramPtr )
{
  RadioPtr = ramPtr;
}


//! RadioSetLength
//!
//! Set the buffer length for RadioBurstRead & RadioFileRead
//!
//! @return data at U8 format
void RadioSetLength( RADIO_LENGTH length )
{
  RadioLen = length;
}


#if 0
//! RadioBurstWriteWip
//!
//! Write a sequence of bytes to a sequence of Radio registers
//!
//! @return none
void RadioBurstWriteWip( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
  addr |= RF_AUTO_INC;
  RadioFileWriteWip( addr, length );
}


//! RadioBurstWrite
//!
//! Write a sequence of bytes to a sequence of Radio registers
//!
//! @return none
void RadioBurstWrite( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
  addr |= RF_AUTO_INC;
  RadioFileWrite( addr, length );
}
#endif

//! RadioFileWrite
//!
//! Write a sequence of bytes to a single Radio register
//!
//! @return none
void RadioFileWrite( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
#if 1
  RadioWipPtr = &rf_send_data;
  RadioFileWriteWip( addr, length );
#else
  RadioWipPtr = RadioPtr;
  RadioFileWriteWip( addr, length );
#endif  
}


//! RadioFileWriteWip
//!
//! Write a sequence of bytes to a single Radio register
//!
//! @return none
void RadioFileWriteWip( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
#if( RF_SPI_MODE == DISABLE )
  U8 *p = RadioWipPtr;

  if( !length )
    return;

  Clr_rf_clk();
  In_rf_din();
  Set_rf_din();                        //FF check if necessary
  addr |= RF_WRITE_MODE;
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  rf_write_byte( addr );                 // Begin Addr transfer
  do
    rf_write_byte( *p++ );    // Begin Data/Cmd transfer
  while( --length );

  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
  U8 *p = RadioWipPtr;

  if( !length )
    return;

  addr |= RF_WRITE_MODE;
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  /*SPDR = addr;                 // Begin Addr transfer
  while(!( SPSR &( 1 << SPIF )));
  */
  ( void )rf_spi_master_byte_txrx( addr );      // Begin Addr transfer
  do
  {
    /*SPDR = *p++;    // Begin Data/Cmd transfer
    while(!( SPSR &( 1 << SPIF )));
    */
    ( void )rf_spi_master_byte_txrx( *p );   // Begin Data/Cmd transfer
    p++;
  }
  while( --length );

  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == ENABLE )
}


#if 0
//! RadioBurstReadWip
//!
//! Read a sequence of bytes from a sequence of Radio registers
//! using RadioWipPtr as the buffer pointer
//! @return none
void RadioBurstReadWip( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
  addr |= RF_AUTO_INC;
  RadioFileReadWip( addr, length );
}


//! RadioBurstRead
//!
//! Read a sequence of bytes from a sequence of Radio registers
//! using RadioWipPtr as the buffer pointer
//! @return none
void RadioBurstRead( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
  addr |= RF_AUTO_INC;
  RadioFileRead( addr, length );
}
#endif

//! RadioFileRead
//!
//! Read a sequence of bytes from a sequence of Radio registers
//! using RadioWipPtr as the buffer pointer
//! @return none
void RadioFileRead( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
  RadioWipPtr = &rf_recive_data;
  RadioWipLen = RadioLen;
  RadioFileReadWip( addr, length );
}
//#endif


//! RadioFileReadWip
//!
//! Read a sequence of bytes from a single Radio registers
//!
//! @return none
void RadioFileReadWip( RADIO_REG_ADDR addr, RADIO_LENGTH length )
{
#if( RF_SPI_MODE == DISABLE )
  U8 *p = RadioWipPtr;
  U8 RadioSpiTemp;

  if( !length )
    return;

  if( length >= RadioWipLen )
  {
    RadioSpiTemp = length - RadioWipLen;
    length = RadioWipLen;
    RadioWipLen = 0;
  }
  else
  {
    RadioWipLen -= length;
    RadioSpiTemp = 0;
  }
  Clr_rf_clk();
  In_rf_din();
  Set_rf_din();                       //FF check if necessary
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  rf_write_byte( addr );                 // Begin Addr transfer
  while( length )
  {
    length--;
    *p++ = rf_read_byte();    // Begin Data/Cmd transfer
  }

  while( RadioSpiTemp-- )
    rf_read_byte();
  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == DISABLE )
#if( RF_SPI_MODE == ENABLE )
  U8 *p = RadioWipPtr;
  U8 RadioSpiTemp;

  if( !length )
    return;

  if( length >= RadioWipLen )
  {
    RadioSpiTemp = length - RadioWipLen;
    length = RadioWipLen;
    RadioWipLen = 0;
  }
  else
  {
    RadioWipLen -= length;
    RadioSpiTemp = 0;
  }
  Int_disable();
  
  Clr_rf_csb();                        // select RF

  /*SPDR = addr;                 // Begin Addr transfer
  while(!( SPSR &( 1 << SPIF )));
  */
  ( void )rf_spi_master_byte_txrx( addr );         // Begin Addr transfer
  while( length )
  {
    length--;
    /*SPDR = 0;
    while(!( SPSR &( 1 << SPIF )));
    *p++ = SPDR;    // Begin Data/Cmd transfer
    */
    *p++ = rf_spi_master_byte_txrx( 0 );  // Begin Data/Cmd transfer
  }

  while( RadioSpiTemp-- )
  {
    /*SPDR = 0;
    while(!( SPSR &( 1 << SPIF )));
    */
    ( void )rf_spi_master_byte_txrx( 0 );
  }
  Set_rf_csb();                        // de-select RF
  
  Int_enable();
#endif  //#if( RF_SPI_MODE == ENABLE )
}


#endif  // _e2radio_c_
