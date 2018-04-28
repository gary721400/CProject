#ifndef __com_c__
#define __com_c__
//***********************************************
#include "config.h"


//#include "com.h"
#include "compiler.h"
//#include "timer_drv.h"  // Include the timer to precharge
//#include "mail.h"
//#include "mail_evt.h"



#include <KBDM9_DDK.H>
#include <sfr.h>
//***********************************************
#ifdef TX_DEBUG
U8 conv( U8 v );
//_MEM_TYPE_SLOW_ U8 sio_buffer[ 8 ];
//_MEM_TYPE_SLOW_ U8 sio_counter = 0;
//_MEM_TYPE_SLOW_ U32 sio_timer = 0;

//void  send_hex_byte  ( U8 v );
//U8    conv           ( U8 v );


//#pragma DATA = UART_DATA



//char gUartRxBuf[UART_MSG_NUM];
//long gUartRxIndex;
//long gUartTxIndex;
//int gFlag;


//extern	int	glen;
//extern	int	glenindex;
//extern	unsigned char gbuf[100];

//unsigned char gbuf[UART_MSG_NUM];

#pragma CODE = UART_CODE
#if 0
void UART_Tx(const unsigned char *buf, int len)
{
	int i;
	
	for (i = 0; i<len; i++) {
		UARTDR = *(buf + i);
		__asm{
			NOP
			NOP
		}
		while(!(UARSR & 0x0001));
	}
}
#endif

void UART_Init(void)
{
	UARBR = 0x0002u;  //115200 bps
	UARCR = 0x8001; // disable parity, one stop bit, 8 bits per character
	PBEN |= 0x0010;
}

#if 0
void sio_task( void )
{
   if( RI )
   {
      RI = 0;
      sio_timer = Timer_set_timer( TIMER_MS( 100 ));
      if( sio_counter <= 6 )
      {
         sio_buffer[ sio_counter ] = SBUF;
         switch( sio_counter )
         {
            case 0x00:
            if( sio_buffer[ 0 ]== 0xFF )
               sio_counter++;
            break;

            case 0x01:
            if( sio_buffer[ 1 ]== 0x55 )
               sio_counter++;
            break;

            case 0x02:
            if( sio_buffer[ 2 ]== 0x03 )
               sio_counter++;
            break;

            case 0x03:
            if( sio_buffer[ 3 ]== 0x02 )
               sio_counter++;
            break;

            case 0x04:
            if( sio_buffer[ 4 ]== 0x00 )
               sio_counter++;
            break;

            case 0x05:
            sio_counter++;
            break;
            
            case 0x06:
            switch( sio_buffer[ 5 ])
            {
               case 0x01:
               sio_counter = 7;
               mail_send_event( EVT_EXO_SIO , 1 );
               //SendString("play/pause\r\n");
               break;

               case 0x02:
               sio_counter = 7;
               mail_send_event( EVT_EXO_SIO , 2 );
               //SendString("vol+\r\n");
               break;

               case 0x04:
               sio_counter = 7;
               mail_send_event( EVT_EXO_SIO , 3 );
               //SendString("vol-\r\n");
               break;

               default:
               sio_counter = 0;
               break;
            }
            break;
         }
      }
      else
         sio_buffer[ 7 ] = SBUF;
   }
   if( sio_timer && Timer_is_timeout( sio_timer ))
   {
      sio_timer = 0;
      sio_counter = 0;
   }
}
#endif
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
   CKCON |= 0x10;//divied by 4,set SMOD=1,we can get 2400*6=14400 baud rate

   TMOD |= T1_M1_;//set TMOD mode3:00100001
   //TH1 = 0xFC;//38400 baud rate @ 30MHz,SMOD=1;    
   //TH1 = 0xE6;//4800 baud rate @ 24MHz,SMOD=1;  
   TH1 = 0xF3;//2400 baud rate @ 12MHz,SMOD = 0;
   TL1 = 0xF3;
   SCON = 0x52;
   PCON = 0x80;//set SMOD = 1;
   //PCON = 0x00;//set SMOD = 0;
   TR1 = 1;//start TIMER1 counter
   TI = 1;

   //ES = 1;
}
#endif
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
#if 0
   while( !TI );
   TI = 0;
   SBUF = value;
   while( !TI );
#else
		UARTDR = value;
		__asm{
			NOP
			NOP
		}
		while(!(UARSR & 0x0001));
#endif
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
//#if( LCD_PRESENT==ENABLED )
#if 0
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
#if 1
void SendString( char *p )
{
//	UART_Tx(p,strlen((const char *)p))
   while( *p )
  {
     SendByte( *p++ );
  }
}
#endif


void send_hex_byte( U8 v )
{
  SendByte( conv(( v & 0xF0 )>> 4 ));
  SendByte( conv( v & 0x0F ));
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
  SendByte( (unsigned char)'0' );SendByte( (unsigned char)'x' );
  send_hex_byte( (unsigned char)value );
}
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
  SendByte( (unsigned char)'0' );SendByte( (unsigned char)'x' );
  send_hex_byte( value >> 8 );
  send_hex_byte( value );
}
#endif
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
  SendByte( (unsigned char)'0' );SendByte( (unsigned char)'x' );
  send_hex_byte( value >> 24 );
  send_hex_byte( value >> 16 );
  send_hex_byte( value >> 8 );
  send_hex_byte( value );
}



U8 conv( U8 v )
{
  if( v < 10 )
  {
    return v + '0';
  }
  else
  {
    return v - 10 + 'a';
  }
}
/*void print_hex(void *pdat,Uint8 len)
{
	Uint8 h,l,i;
	SendByte('0');SendByte('x');
	for(i=0;i<len;i++)
	{
		h = (*((Uint8*)pdat+i)&0xF0);
		h>>=4;
		if(h<10)
			h=h+'0';
		else
			h=h-10+'A';
		l = (*((Uint8*)pdat+i)&0x0F);
		if(l<10)
			l=l+'0';
		else
			l=l-10+'A';	
    SendByte(h);SendByte(l);SendByte(' ');		
	}
}*/
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
   SendString("1\r\n");
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
