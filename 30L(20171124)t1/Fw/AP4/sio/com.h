#ifndef _COM_H_
#define _COM_H_
//***********************************************
#include "config.h"
#include "compiler.h"

#ifdef TX_DEBUG
void  UART_Init   (void);
void  SendByte    ( U8 value );
void  SendChar    ( U8 value );
void  SendInt     ( U16 value );
void  SendLong    ( U32 value );
void  SendBuffer  ( U8 _MEM_TYPE_SLOW_ *p, U16 length );
void  SendString  ( char *p );
void  SendReturn  ( void );
void  clear_DMA   ( U8 _MEM_TYPE_SLOW_ *p, U16 length );
void  print_hex   ( void * pdat, Uint8 len );
void  send_hex_u8 ( U8 value );
void  send_hex_u32( U32 value );
void  send_hex_u16( U16 value );
void  sio_task    ( void );

#else
#define InitUart( void )
#define SendByte( value )
#define SendChar( value )
#define SendInt( value )
#define SendLong( value )
#define SendBuffer( p, length )
#define SendString( p )
#define SendReturn()
#define clear_DMA( p, length )
#define print_hex( pdat,len)
#define send_hex_u8( value )
#define send_hex_u16( value )
#define send_hex_u32( value )
#define sio_task()
#endif   //#ifdef TX_DEBUG

#define _USB_LOG_U8_( value )
#define _USB_LOG_U16_( value )
#define _USB_LOG_U32_( value )
#define _USB_LOG_STR_( string )
//***********************************************
#endif