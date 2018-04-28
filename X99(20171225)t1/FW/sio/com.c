#ifndef __com_c__
#define __com_c__
//***********************************************
#include "absacc.h"
#include "com.h"
#include "lib_mcu\compiler.h"
//#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
//#include "lib_system\mailbox\mail.h"
//#include "lib_system\mailbox\mail_evt.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = USART_DATA
#pragma CODE = USART_CODE

//***********************************************
#ifdef TX_DEBUG

void  send_hex_byte  ( U8 v );
U8    conv           ( U8 v );
/************************************************
函 数 名: InitUart()
功    能：初始化串口
说    明：
调    用：
全局变量：
入口参数：无
出口参数：无
返 回 值：无
************************************************/
#if 0
void InitUart( void )
{
   UARBR = 0x0002u;  //115200 bps
   UARCR = 0x8001;   // disable parity, one stop bit, 8 bits per character
   PBEN |= 0x0010;
}
#endif

void InitUart( void )
{
   UARBR = 0x0002u;  //115200 bps
   UARCR = 0x8001;   // disable parity, one stop bit, 8 bits per character
   PBEN |= 0x0010;
}

/************************************************
函 数 名: SendByte()
功    能：发送一个字节
说    明：
调    用：
全局变量：
入口参数：value: 发送的数据
出口参数：无
返 回 值：无
************************************************/
void SendByte( U8 value )
{
   UARTDR = value;
   __asm
   {
      NOP
      NOP
   }
   while( !_bit_get( _UARSR_b0 + 0 ));
}
/************************************************
函 数 名: SendChar()
功    能：发送一个字节
说    明：
调    用：
全局变量：
入口参数：value: 发送的数据
出口参数：无
返 回 值：无
************************************************/
#if 0
void SendChar( U8 value )
{
   U8 _MEM_TYPE_SLOW_ temp;
   temp = value >> 4;
   if( temp < 10 )
      temp += '0';
   else
      temp += 'a' - 10;
   SendByte( temp );
   temp = value & 0x0F;
   if( temp < 10 )
      temp += '0';
   else
      temp += 'a' - 10; 
   SendByte( temp );
}
/************************************************
函 数 名: SendInt()
功    能：发送一个整型数据
说    明：
调    用：
全局变量：
入口参数：value: 发送的数据
出口参数：无
返 回 值：无
************************************************/

void SendInt( U16 value )
{
   SendChar( MSB( value ));
   SendChar( LSB( value ));
}
/************************************************
函 数 名: SendLong()
功    能：发送一个长整型数据
说    明：
调    用：
全局变量：
入口参数：value: 发送的数据
出口参数：无
返 回 值：无
************************************************/
void SendLong( U32 value )
{
   SendChar( MSB0( value ));
   SendChar( MSB1( value ));
   SendChar( MSB2( value ));
   SendChar( MSB3( value ));
}
#endif
/************************************************
函 数 名: SendBuffer()
功    能：
说    明：
调    用：
全局变量：
入口参数：无
出口参数：无
返 回 值：无
************************************************/
#if 0
void SendBuffer( U8 _MEM_TYPE_SLOW_ *p, U16 length )
{
   while( length )
   {
      send_hex_u8( *p++ );
      SendByte( ' ' );
      length--;
   }
}
#endif
/************************************************
函 数 名: SendString()
功    能：
说    明：
调    用：
全局变量：
入口参数：无
出口参数：无
返 回 值：无
************************************************/
void SendString( U8 *p )
{
   while( *p )
   {
      SendByte( *p++ );
   }
}
#if 1
/************************************************
函 数 名: send_hex_u8()
功    能：串口发送数据
说    明：
调    用：
全局变量：
入口参数：
出口参数：无
返 回 值：无
************************************************/
void send_hex_u8( U8 value )
{
   //SendByte( '0' ); SendByte( 'x' );
   SendString((U8 *)"0x");
   send_hex_byte( value );
}
#if 1
/************************************************
函 数 名: send_hex_u16()
功    能：串口发送数据
说    明：
调    用：
全局变量：
入口参数：
出口参数：无
返 回 值：无
************************************************/
void send_hex_u16( U16 value )
{
   //SendByte( '0' ); SendByte( 'x' );
   //send_hex_byte( value >> 8 );
   //send_hex_byte( value );
   SendString((U8 *)"0x");
   send_hex_byte( MSB( value ));
   send_hex_byte( LSB( value ));
}
#endif
#if 1
/************************************************
函 数 名: send_hex_u32()
功    能：串口发送数据
说    明：
调    用：
全局变量：
入口参数：
出口参数：无
返 回 值：无
************************************************/
void send_hex_u32( U32 value )
{
   //SendByte( '0' ); SendByte( 'x' );
   //send_hex_byte( value >> 24 );
   //send_hex_byte( value >> 16 );
   //send_hex_byte( value >> 8 );
   //send_hex_byte( value );
   SendString((U8 *)"0x");
   send_hex_byte( MSB0( value ));
   send_hex_byte( MSB1( value ));
   send_hex_byte( MSB2( value ));
   send_hex_byte( MSB3( value ));
}
#endif


void send_hex_byte( U8 v )
{
   SendByte( conv(( v & 0xF0 )>> 4 ));
   SendByte( conv( v & 0x0F ));
}


U8 conv( U8 v )
{
   if( v < 10 )
   {
      return v + '0';
   }
   //else
   {
      return v - 10 + 'a';
   }
}
#endif
/************************************************
函 数 名: SendReturn()
功    能：
说    明：
调    用：
全局变量：
入口参数：无
出口参数：无
返 回 值：无
************************************************/
#if 0
void SendReturn( void )
{
/*   SendByte( '\r' );
   SendByte( '\n' );
   */
   SendString((U8 *)"\r\n");
}
#endif
/************************************************
函 数 名: clear_DMA()
功    能：
说    明：
调    用：
全局变量：
入口参数：无
出口参数：无
返 回 值：无
************************************************/
#if 0
void clear_DMA( U8 _MEM_TYPE_SLOW_ *p, U16 length )
{
   while( length )
   {
      *p++ = 0;
      length--;
   }
}
#endif
//***********************************************
#endif   //#ifdef TX_DEBUG
//***********************************************
#endif
