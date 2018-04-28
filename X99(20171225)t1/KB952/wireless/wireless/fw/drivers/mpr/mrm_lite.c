#include <sfr.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fs.h"
#include "KBDM9_DDK.H"
#include "mrm_lite.h"

#pragma DATA = MRM_DATA

extern   void  SensorInit  ( void );

mrmByte  _MrmI2cRead    ( mrmByte addr);
mrmBool  _MrmI2cWrite   ( mrmByte addr, mrmByte data );
void     _MrmInit       ( void );
mrmBool  _MrmGetCode    ( mrmByte *pBuff, mrmBool bRestart );

ImageDeclareType( MRM_INIT_CODE );
ImageDeclareType( MRM_RUNNING_CODE );

/******************
* MRM_COMMON_CODE *
******************/
#pragma CODE = MRM_COMMON_CODE;

mrmBool MrmReadReg( mrmByte reg, mrmByte *pData )
{
   mrmByte cmd[ 3 ];
   mrmByte rtn;

   cmd[ 0 ] = 0x20;
   cmd[ 1 ] = reg;
   cmd[ 2 ] = 0;
   SPI_CS_LOW();
   DelayUS( 1 );
   rtn = SPI_Transmit( cmd, 3 );
   *pData = SP2DR;
   DelayUS( 20 );
   SPI_CS_HIGH();
   DelayUS( 22*4 );
   //*pData = SP2DR;
   return rtn;
}


mrmBool MrmWriteReg( mrmByte reg, mrmByte data )
{
   mrmByte cmd[ 3 ];
   mrmByte rtn;

   cmd[ 0 ] = 0x30;
   cmd[ 1 ] = reg;
   cmd[ 2 ] = data;
   SPI_CS_LOW();
   DelayUS( 1 );
   rtn = SPI_Transmit( cmd, 3 );
   DelayUS( 20 );
   SPI_CS_HIGH();
   DelayUS( 22*4 );
   return rtn;
}


mrmByte MrmI2cRead( mrmByte addr )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _MrmI2cRead( addr );
}


mrmBool MrmI2cWrite( mrmByte addr, mrmByte data )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _MrmI2cWrite( addr, data );
}


void MrmInit(void)
{
   SDKLoadCode( MRM_INIT_CODE );
   _MrmInit();
}


mrmWord MrmGetChipID( void )
{
   mrmByte temp = 0;
   mrmWord id;

   ( void )MrmReadReg( 0x00, &temp );  // 0x01
   id = temp;
   ( void )MrmReadReg( 0x01, &temp );  // 0x76
   id += (( unsigned short )temp << 8 );
   return id;
}


void MrmStart(void)
{
   ( void )MrmWriteReg( 0x08, 0x03 );
   ( void )MrmWriteReg( 0x06, 0x01 );
   ( void )MrmWriteReg( 0x17, 0x05 );
   ( void )MrmWriteReg( 0x30, 0x00 );

   ( void )MrmWriteReg( 0x19, 0x01 );
   ( void )MrmWriteReg( 0x07, 0x02 );
   ( void )MrmWriteReg( 0x19, 0x00 );
}


#pragma CODE = MRM_COMMON_CODE;

mrmBool MrmGetCode( mrmByte *pBuff, mrmBool bRestart )
{
   SDKLoadCode( MRM_RUNNING_CODE );
   return _MrmGetCode( pBuff, bRestart );
}


/******************
*  MRM_INIT_CODE  *
******************/
#pragma CODE = MRM_INIT_CODE

mrmByte _MrmI2cRead( mrmByte addr )
{
   mrmByte temp, result;
   mrmWord cnt;

   // START + 0307 addr(W)
   ( void )MrmWriteReg( 0xA2, 0xA0 );
   ( void )MrmWriteReg( 0xA3, 0x42 );
   ( void )MrmWriteReg( 0xA2, 0xA2 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   // reg addr
   ( void )MrmWriteReg( 0xA3, addr );
   ( void )MrmWriteReg( 0xA5, 0x02 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   // STOP
   ( void )MrmWriteReg( 0xA2, 0xA1 );
   ( void )MrmWriteReg( 0xA5, 0x02 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   // START + 0307 addr(R)
   ( void )MrmWriteReg( 0xA5, 0x04 );
   ( void )MrmWriteReg( 0xA2, 0x80 );
   ( void )MrmWriteReg( 0xA3, 0x43 );
   ( void )MrmWriteReg( 0xA2, 0x82 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;
   ( void )MrmWriteReg( 0xA5, 0x02 );

   // wait RX ready
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   // read reg content + stop
   ( void )MrmReadReg( 0xA3, &result );
   ( void )MrmWriteReg( 0xA2, 0x81 );
   ( void )MrmWriteReg( 0xA5, 0x01 );

   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   ( void )MrmWriteReg( 0xA5, 0x04 );
   return result;
}


mrmBool _MrmI2cWrite( mrmByte addr, mrmByte data )
{
   mrmByte temp;
   mrmWord cnt;

   ( void )MrmWriteReg( 0xA2, 0xA0 );
   ( void )MrmWriteReg( 0xA3, 0x42 );
   ( void )MrmWriteReg( 0xA2, 0xA2 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   ( void )MrmWriteReg( 0xA3, addr );
   ( void )MrmWriteReg( 0xA5, 0x02 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   ( void )MrmWriteReg( 0xA3, data );
   ( void )MrmWriteReg( 0xA5, 0x02 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;

   ( void )MrmWriteReg( 0xA2, 0xA1 );
   ( void )MrmWriteReg( 0xA5, 0x02 );
   cnt = 1000;
   do
   {
      cnt--;
      if( !MrmReadReg( 0xA5, &temp ))
      {
         return 0;
      }
   }
   while(( !temp )&& cnt );
   if( !cnt )
      return 0;
   ( void )MrmWriteReg( 0xA5, 0x04 );

   return 1;
}


void _MrmInit( void )
{
   mrmByte temp;

   InitMrmInterface();
    
   /* 设置sensor工作频率 */
   ( void )MrmWriteReg( 0x11, SENSOR_FREQ );
   ( void )MrmReadReg( 0x11, &temp );
   DelayMS( 10 );
  
   /* 设置PLL */
#if( MPR7601_PLL == MPR7601_PLL_80MHz )
   ( void )MrmWriteReg( 0x03, 0x03 );
   ( void )MrmWriteReg( 0x04, 0x0A );
#elif( MPR7601_PLL == MPR7601_PLL_60MHz )
   ( void )MrmWriteReg( 0x03, 0x02 );
   ( void )MrmWriteReg( 0x04, 0x05 );
#elif( MPR7601_PLL == MPR7601_PLL_40MHz )
   ( void )MrmWriteReg( 0x03, 0x03);
   ( void )MrmWriteReg( 0x04, 0x05);
#elif( MPR7601_PLL == MPR7601_PLL_20MHz )
   ( void )MrmWriteReg( 0x03, 0x06 );
   ( void )MrmWriteReg( 0x04, 0x05 );
#endif
   ( void )MrmReadReg( 0x03, &temp );
   ( void )MrmReadReg( 0x04, &temp );
   ( void )MrmWriteReg( 0x02, 0x01 );
   DelayMS( 50 );

   /* 设置LED灯 */
   ( void )MrmWriteReg( 0x08, 0xC3 );
   ( void )MrmReadReg( 0x08, &temp );
   DelayMS( 10 );

   /* 初始化sensor */
   SensorInit();
}


/******************
*   MRM_RUN_CODE  *
******************/
#pragma CODE = MRM_RUNNING_CODE

mrmBool _MrmGetCode( mrmByte *pBuff, mrmBool bRestart )
{
   mrmByte temp = 0;

   if( bRestart )
   {
      ( void )MrmWriteReg( 0x19, 0x01 );
      ( void )MrmWriteReg( 0x07, 0x02 );
      ( void )MrmWriteReg( 0x19, 0x00 );
   }
   ( void )MrmReadReg( 0x16, &temp );
   if( temp )
   {
      ( void )MrmWriteReg( 0x16, temp );
      if( temp & 0x01 )
      {
         ( void )MrmReadReg( 0x20, &pBuff[ 0 ]);
         ( void )MrmReadReg( 0x21, &pBuff[ 1 ]);
         ( void )MrmReadReg( 0x22, &pBuff[ 2 ]);
         ( void )MrmReadReg( 0x23, &pBuff[ 3 ]);
         ( void )MrmReadReg( 0x24, &pBuff[ 4 ]);
         ( void )MrmReadReg( 0x25, &pBuff[ 5 ]);
         ( void )MrmReadReg( 0x26, &pBuff[ 6 ]);
         return mrmTrue;
      }
   }
   return mrmFalse;
}


void _MDiv( mrmByte *mprcode, mrmDword *res_code, mrmByte *book_pre, mrmDword *book_post )
{
   mrmByte   a0 = 0;
   mrmDword  b0 = 0, b1 = 0, b2 = 0, b3 = 0, b5 = 0; 

   b0 = ( mrmDword )mprcode[ 0 ]*0x8637BD;   //*0x20  256^6 /(10^6)/0x20
   //256^5/(10^6)              256^4/(10^6)    
   b1 = ( mrmDword )mprcode[ 1 ]*0x10C6F7 +( mrmDword )mprcode[ 2 ]* 0x10C6 +( mrmDword )( mprcode[ 3 ]<< 4 );
   //余数部分      (256^6)%(10^6)
   b2 = ( mrmDword )mprcode[ 0 ]* 0xAD800 +( mrmDword )mprcode[ 1 ]* 0x99440 +( mrmDword )mprcode[ 2 ]* 0xEC280; 
   //                                 256^2            256                     
   b3 = ( mrmDword )mprcode[ 3 ]* 0xBDC00 +( mrmDword )mprcode[ 4 ]* 0x10000 +( mrmDword )mprcode[ 5 ]* 0x100 + mprcode[ 6 ];
   b2 += b3;
   b3 = b2 / 1000000;
   b1 += b3;      //b0*0x20+b1是整数   

   b3 = b2 % 1000000;   //b3为余数      
   a0 = b0 >> 27;   
   b0 = b0 << 5;

   if( b0 > ~b1 )
      a0++;
   b0 += b1;

   *res_code = b3;
   *book_pre = a0;
   *book_post = b0;
}


