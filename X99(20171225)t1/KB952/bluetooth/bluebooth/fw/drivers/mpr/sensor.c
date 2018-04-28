#include <sfr.h>
#include <stdlib.h>
#include <stdio.h>
#include "KBDM9_DDK.H"
#include "mrm_lite.h"

#pragma CODE = MRM_INIT_CODE
#pragma DATA = MRM_DATA

ImageDeclareType( MRM_INIT_CODE );

extern   mrmByte  _MrmI2cRead    ( mrmByte addr );
extern   mrmBool  _MrmI2cWrite   ( mrmByte addr, mrmByte data );

mrmBool  _AdjustExpose  ( void );
mrmBool  _WinInit       ( void );
mrmBool  _GetSensorPara ( mrmWord *pStartX, mrmWord *pStartY, mrmWord *pExpose );
mrmBool  _SetSensorPara ( mrmWord startX, mrmWord startY, mrmWord expose );

static SensorPara_t  g_SensorPara;
static const mrmByte SensorInitData[][ 2 ] = 
{
   {0x43, 0x00},
   {0x44, 0xa2},
   {0x40, 0x10},
   {0x41, 0x00},
   {0x42, 0x10},
   {0x47, 0x00},
   {0x48, 0xc3},

   {0x49, 0x00},
   {0x4a, 0x00},

   {0x4b, 0x00},  
   {0x4e, 0x23},  // 24MHz/(x+1)
   {0x4f, 0x01},  //0x10 Y first, 0x00 Y last
   {0x01, 0x6a},
   {0x02, 0x70},
   {0x1c, 0x00},
   {0x1d, 0x00},
   {0x10, 0x00},
   {0x11, 0x05},

   {0x05, 0x00},
   {0x06, 0x00},
   {0x07, 0x00},
   {0x08, 0x02},
   {0x09, 0x01},
   {0x0a, 0xe0},
   {0x0b, 0x02},
   {0x0c, 0x80},

   {0x0d, 0x22},
   {0x0e, 0x02},
   {0x0f, 0x82},

   {0x12, 0x70},
   {0x13, 0x00},
   {0x14, 0x00},
   {0x15, 0xba},
   {0x16, 0x13},
   {0x17, 0x52},
   {0x18, 0xc0},
   {0x1e, 0x0d},
   {0x1f, 0x32},
   {0x47, 0x00},
   {0x19, 0x06},
   {0x1a, 0x06},
   {0x31, 0x00},
   {0x3b, 0x00},
   {0x59, 0x0f},
   {0x58, 0x88},
   {0x57, 0x08},
   {0x56, 0x77},

   {0x35, 0xd8},
   {0x36, 0x40},

   {0x3c, 0x00},
   {0x3d, 0x00},
   {0x3e, 0x00},
   {0x3f, 0x00},

   {0xb5, 0x70},
   {0xb6, 0x40},
   {0xb7, 0x00},
   {0xb8, 0x38},
   {0xb9, 0xc3},
   {0xba, 0x0f},

   {0x7e, 0x35},
   {0x7f, 0x86},

   {0x5c, 0x68},
   {0x5d, 0x78},

   {0x61, 0x80},
   {0x63, 0x80},
   {0x65, 0x98},
   {0x67, 0x80},
   {0x68, 0x14},

   {0x69  ,0x58},
   {0x6A  ,0xf6},
   {0x6B  ,0xfb},
   {0x6C  ,0xf4},
   {0x6D  ,0x5a},
   {0x6E  ,0xe6},

   {0x6f  ,0x00},  

   {0x70  ,0x14},
   {0x71  ,0x1c},
   {0x72  ,0x20},

   {0x73  ,0x10},
   {0x74  ,0x3c},
   {0x75  ,0x52}, 

   {0x7d  ,0x2f},
   {0x80  ,0x0c},
   {0x81  ,0x0c},
   {0x82  ,0x44},

   {0x83  ,0x18}, 
   {0x84  ,0x18}, 
   {0x85  ,0x04}, 
   {0x87  ,0x34}, 

   {0x88  ,0x04}, 
   {0x89  ,0x01}, 
   {0x8a  ,0x50}, 
   {0x8b  ,0x50}, 
   {0x8c  ,0x07},

   {0x50  ,0x0c}, 
   {0x5f  ,0x3c}, 
   {0x8e  ,0x02}, 
   {0x86  ,0x02}, 
   {0x51  ,0x20}, 
   {0x52  ,0x08}, 
   {0x53  ,0x00}, 

   {0x77  ,0x80}, 
   {0x78  ,0x00}, 
   {0x79  ,0x00}, 
   {0x7a  ,0x20}, 
   {0x7b  ,0x40}, 
   {0x7c  ,0x00},

   {0xa0  ,0x40}, 
   {0xa1  ,0x60}, 
   {0xa2  ,0x3c}, 
   {0xa3  ,0x3c}, 

   {0xa4  ,0xc8}, 
   {0xa5  ,0x02}, 
   {0xa6  ,0x28}, 
   {0xa7  ,0x02}, 

   {0xa8  ,0xee}, 
   {0xa9  ,0x12}, 
   {0xaa  ,0x01}, 
   {0xab  ,0x20}, 
   {0xac  ,0xf0},
   {0xad  ,0x10},

   {0xae  ,0x18},
   {0xaf  ,0x74},
   {0xb0  ,0xe0},
   {0xb1  ,0x20},
   {0xb2  ,0x6c},
   {0xb3  ,0x40},
   {0xb4  ,0x04},

   {0xbb  ,0x42},
   {0xbc  ,0x60},
   {0xbd  ,0x50},
   {0xbe  ,0x50},
   {0xbf  ,0x0c},
   {0xc0  ,0x06},
   {0xc1  ,0x70},
   {0xc2  ,0xf1},
   {0xc3  ,0x40},
   {0xc4  ,0x20},
   {0xc5  ,0x33},     
   {0xc6  ,0x1d},  

   {0xca  ,0x70},  
   {0xcb  ,0x70},  
   {0xcc  ,0x78},  
   {0xcd  ,0x80},  
   {0xce  ,0x80},  
   {0xcf  ,0x80},  

   {0x20  ,0x02},  
   {0x21  ,0xc0},  
   {0x22  ,0x40},  
   {0x23  ,0x88},  
   {0x24  ,0x96},  
   {0x25  ,0x30},  
   {0x26  ,0xd0},  
   {0x27  ,0x00},  

   {0x28  ,0x02},  
   {0x29  ,0x58},  
   {0x2a  ,0x03},  
   {0x2b  ,0x84},  
   {0x2c  ,0x09},  
   {0x2d  ,0x60},  
   {0x2e  ,0x0a},  
   {0x2f  ,0x8c},  

   {0x30  ,0x20},  
   {0x31  ,0x00},  
   {0x32  ,0x1c},  
   {0x33  ,0x90},  
   {0x34  ,0x10},  
   {0xd0  ,0x34},  
   {0xd1  ,0x60},  
   {0xd2  ,0xf2},  
   {0xd4  ,0x96},  
   {0xd5  ,0x10},  
   {0xd6  ,0x96},  
   {0xd7  ,0x10},  
   {0xd8  ,0x02}, 

   {0xdd  ,0x32},   
   {0xe0  ,0x03},  
   {0xe1  ,0x02},  
   {0xe2  ,0x27},  
   {0xe3  ,0x1e},  
   {0xe8  ,0x3b},  
   {0xe9  ,0x6e},  
   {0xea  ,0x2c},  
   {0xeb  ,0x50},  
   {0xec  ,0x73},   

   {0xed  ,0x00},  
   {0xee  ,0x00},  
   {0xef  ,0x00}, 

   {0xf0  ,0x01},  
   //readSensor(0xf0},
   {0x00  ,0x20},  
   {0x01  ,0x20},  
   {0x02  ,0x20},  
   {0x03  ,0x22},  
   {0x04  ,0x99},  
   {0x05  ,0x78},  
   {0x06  ,0x78},  
   {0x07  ,0x78},

   {0x10  ,0x04},  
   {0x11  ,0x04},  
   {0x12  ,0x04},  
   {0x13  ,0x04},  
   {0x14  ,0x01},  
   {0x15  ,0x01},  
   {0x16  ,0x01},  
   {0x17  ,0x01}, 

   {0x20  ,0x00},  
   {0x21  ,0x00},  
   {0x22  ,0x00},  
   {0x23  ,0x00},  
   {0x24  ,0x00},  
   {0x25  ,0x00},  
   {0x26  ,0x00},  
   {0x27  ,0x00},
   {0x40  ,0x11}, 

   {0x45  ,0x06}, 
   {0x46  ,0x06}, 
   {0x47  ,0x05}, 
   {0x48  ,0x04}, 
   {0x49  ,0x03}, 
   {0x4a  ,0x03}, 

   {0x62  ,0xd8}, 
   {0x63  ,0x24}, 
   {0x64  ,0x24}, 
   {0x65  ,0x24}, 
   {0x66  ,0xd8}, 
   {0x67  ,0x24}, 

   {0x5a  ,0x00}, 
   {0x5b  ,0x00}, 
   {0x5c  ,0x00}, 
   {0x5d  ,0x00}, 
   {0x5e  ,0x00}, 
   {0x5f  ,0x00},

   {0x69  ,0x03}, 
   {0x70  ,0x5d}, 
   {0x71  ,0xed}, 
   {0x72  ,0xff}, 
   {0x73  ,0xe5}, 
   {0x74  ,0x5f}, 
   {0x75  ,0xe6}, 

   {0x76  ,0x41}, 
   {0x77  ,0xef}, 
   {0x78  ,0xff},   
   {0x79  ,0xff},  
   {0x7a  ,0x5f},  
   {0x7b  ,0xfa}, 

   {0x7e  ,0x00},  
   {0x7f  ,0x40},  
   {0x80  ,0x48},  
   {0x81  ,0x06}, 
   {0x82  ,0x08}, 
   {0x83  ,0x23}, 
   {0x84  ,0x38}, 
   {0x85  ,0x4F}, 
   {0x86  ,0x61}, 
   {0x87  ,0x72}, 
   {0x88  ,0x80}, 
   {0x89  ,0x8D}, 
   {0x8a  ,0xA2}, 
   {0x8b  ,0xB2}, 
   {0x8c  ,0xC0}, 
   {0x8d  ,0xCA}, 
   {0x8e  ,0xD3},  
   {0x8f  ,0xDB},  
   {0x90  ,0xE2},   
   {0x91  ,0xED},  
   {0x92  ,0xF6},  
   {0x93  ,0xFD}, 

   {0x94  ,0x04},  
   {0x95  ,0x0E},  
   {0x96  ,0x1B},  
   {0x97  ,0x28},  
   {0x98  ,0x35},  
   {0x99  ,0x41},  
   {0x9a  ,0x4E},  
   {0x9b  ,0x67},  
   {0x9c  ,0x7E},  
   {0x9d  ,0x94},  
   {0x9e  ,0xA7},  
   {0x9f  ,0xBA},  
   {0xa0  ,0xC8},  
   {0xa1  ,0xD4},  
   {0xa2  ,0xE7},  
   {0xa3  ,0xF4},  
   {0xa4  ,0xFA}, 

   {0xf0  ,0x00},
   {0x03  ,0x00},   // ??1abit11..bit8
   {0x04  ,0x99},   // ??1abit7..bit0
   {0x45  ,0x24},  
   {0x40  ,0x10},  
   {0x41  ,0x00},  
   {0x47  ,0x30},

   {0x43  ,0x40},  
   {0x44  ,0xE2},  
};


void SensorInit(void)
{
   mrmWord ii, count;

   count = sizeof( SensorInitData )/ 2;
   for( ii = 0; ii < count; ii++ )
   {
      ( void )_MrmI2cWrite( SensorInitData[ ii ][ 0 ], SensorInitData[ ii ][ 1 ]);
   }
    
   /* 设置曝光值 */
#if( SENSOR_FREQ == SENSOR_6MHZ )
   g_SensorPara.expose = 0x80;
#elif( SENSOR_FREQ == SENSOR_12MHZ )
   g_SensorPara.expose = 0x200;
#elif( SENSOR_FREQ == SENSOR_24MHZ )
   g_SensorPara.expose = 0x280;
#endif
   ( void )_MrmI2cWrite( 0x03, g_SensorPara.expose / 0x100 );
   ( void )_MrmI2cWrite( 0x04, g_SensorPara.expose % 0x100 );

   /* 设置亮度 */
#if( BRIGHTNESS == BRIGHTNESS_0 )
   ( void )_MrmI2cWrite( 0x7A, 0xE0 );
   ( void )_MrmI2cWrite( 0xD1, 0x38 );
#elif( BRIGHTNESS == BRIGHTNESS_1 )
   ( void )_MrmI2cWrite( 0x7A, 0xF0 );
   ( void )_MrmI2cWrite( 0xD1, 0x40 );
#elif( BRIGHTNESS == BRIGHTNESS_2)
   ( void )_MrmI2cWrite( 0x7A, 0x00 );
   ( void )_MrmI2cWrite( 0xD1, 0x50 );
#elif( BRIGHTNESS == BRIGHTNESS_3 )
   ( void )_MrmI2cWrite( 0x7A, 0x20 );
   ( void )_MrmI2cWrite( 0xD1, 0x60 );
#elif( BRIGHTNESS == BRIGHTNESS_4 )
   ( void )_MrmI2cWrite( 0x7A, 0x30 );
   ( void )_MrmI2cWrite( 0xD1, 0x68 );
#elif( BRIGHTNESS == BRIGHTNESS_5 )
   ( void )_MrmI2cWrite( 0x7A, 0x40 );
   ( void )_MrmI2cWrite( 0xD1, 0x70 );
#elif( BRIGHTNESS == BRIGHTNESS_6)
   ( void )_MrmI2cWrite( 0x7A, 0x50 );
   ( void )_MrmI2cWrite( 0xD1, 0x80 );
#endif
}


mrmBool _AdjustExpose( void )
{
   mrmWord sensorExpose, minExpose, maxExpose;
   mrmDword count = 0;
   mrmSByte sensorflag = 0x00;
   mrmSWord ImageDataLens = 0;
   mrmSWord i = 0;
   mrmByte temp;
   mrmBool ret = mrmFalse;
   mrmWord timeout;

   minExpose = MIN_EXPOSE;
   maxExpose = MAX_EXPOSE;
   while( 1 )
   {
      sensorExpose = ( maxExpose + minExpose )/ 2;
      ( void )_MrmI2cWrite( 0x03, sensorExpose / 0x100 );
      ( void )_MrmI2cWrite( 0x04, sensorExpose % 0x100 );
      DelayMS( 100 );

      ( void )MrmWriteReg( 0x30, 0x40 );
      ( void )MrmWriteReg( 0x19, 0x01 );
      ( void )MrmWriteReg( 0x07, 0x02 );
      ( void )MrmWriteReg( 0x19, 0x03 );

      timeout = 1000;
      do
      {
         if( !MrmReadReg( 0x16, &temp )||( --timeout == 0 ))
         {
            return mrmFalse;
         }
      }
      while( !( temp & 0x02 ));

      ( void )MrmWriteReg( 0x16, temp );
      ( void )MrmReadReg( 0x32, &temp );
      ImageDataLens = temp;
      ( void )MrmReadReg( 0x33, &temp );
      ImageDataLens += ( unsigned int )temp * 0x100;
      for( i = 0, count = 0; i < ImageDataLens; i++ )
      {
         ( void )MrmReadReg( 0x31, &temp );
         count += temp;
      }
      count = count / 9604;
      if( count >= BRIGHTNESS_MIN && count <= BRIGHTNESS_MAX )
      {         
         sensorflag = 0;
         g_SensorPara.expose = sensorExpose;
         ret = mrmTrue;
         break;
      }
      else if( count < BRIGHTNESS_MIN )
      {
         minExpose = sensorExpose + 1;
         if( minExpose > MAX_EXPOSE )
         {
            sensorflag = 1;
            break;
         }
      }
      else if( count > BRIGHTNESS_MAX )
      {
         maxExpose = sensorExpose - 1;
         if( maxExpose < MIN_EXPOSE )
         {
            sensorflag = 2;
            break;
         }
      }
      if( minExpose > maxExpose )
      {
         sensorflag = 3;
         break;
      }
      ( void )MrmWriteReg( 0x07, 0x01 );  //reset sisp
      ( void )MrmWriteReg( 0x08, 0xC3 );           
   }

   ( void )MrmWriteReg( 0x07, 0x01 );    //reset sisp
   ( void )MrmWriteReg( 0x08, 0xC3 );
   DelayMS( 10 );
   ( void )_MrmI2cWrite( 0x03, g_SensorPara.expose / 0x100 );
   ( void )_MrmI2cWrite( 0x04, g_SensorPara.expose % 0x100 );
   return ret;
}


mrmBool _WinInit( void )
{
   mrmDword u32RowBrightness[ 15 ], u32ColBrightness[ 15 ];
   mrmDword u32Count, u32Row;
   mrmWord ii, timeout;
   mrmByte u8Temp;

   //memset(u32RowBrightness, 0, sizeof(u32RowBrightness));
   //memset(u32ColBrightness, 0, sizeof(u32ColBrightness));
   for( ii = 0; ii < 15; ii++ )
   {
      u32RowBrightness[ ii ] = 0;
      u32ColBrightness[ ii ] = 0;
   }

   /* 窗口位置恢复默认值 */
   ( void )MrmWriteReg( 0x12, 0x00 );  // 行
   ( void )MrmWriteReg( 0x13, 0x00 );  // 列低字节
   ( void )MrmWriteReg( 0x14, 0x00 );  // 列高字节

   /* 读640x15数据 */
   ( void )MrmWriteReg( 0x30, 0x20 );  // 设置D640模式
   ( void )MrmWriteReg( 0x19, 0x01 );  // 设置手动
   ( void )MrmWriteReg( 0x07, 0x02 );
   ( void )MrmWriteReg( 0x19, 0x03 );  // 启动

   timeout = 1000;
   do
   {
      if( !MrmReadReg( 0x16, &u8Temp )||( --timeout == 0 ))
         return mrmFalse;
   }
   while( !( u8Temp & 0x02 ));
   ( void )MrmWriteReg( 0x16, 0x02 );

   /* 读数据大小 */
   ( void )MrmReadReg( 0x32, &u8Temp );
   u32Count = u8Temp;
   ( void )MrmReadReg( 0x33, &u8Temp );    
   u32Count += (( unsigned long )u8Temp << 8 );
   if( u32Count > 640*15 )
      u32Count = 640*15;

   /* 读取数据 */
   for( ii = 0, u32Row = 0; ii < u32Count; ii++ )
   {
      ( void )MrmReadReg( 0x31, &u8Temp );
      u32RowBrightness[ u32Row / 32] += u8Temp;
      if(( ii % 640 )== 639)
         u32Row += 32;
   }
   ( void )MrmWriteReg( 0x30, 0x00 );
   ( void )MrmWriteReg( 0x07, 0x01 );  // Chip software reset
   ( void )MrmWriteReg( 0x08, 0xC3 );  // LED on
   DelayMS( 10 );

   /* 读15x480数据 */
   ( void )MrmWriteReg( 0x30, 0xA0 );  // 设置D640模式
   ( void )MrmWriteReg( 0x19, 0x01 );  // 设置手动
   ( void )MrmWriteReg( 0x07, 0x02 );
   ( void )MrmWriteReg( 0x19, 0x03 );  // 启动

   timeout = 500;
   do
   {
      if( !MrmReadReg( 0x16, &u8Temp )||( --timeout == 0 ))
         return mrmFalse;
   }
   while( !( u8Temp & 0x02 ));
   ( void )MrmWriteReg( 0x16, 0x02 );

   /* 读数据大小 */
   ( void )MrmReadReg( 0x32, &u8Temp );
   u32Count = u8Temp;
   ( void )MrmReadReg( 0x33, &u8Temp );
   u32Count += (( unsigned long )u8Temp << 8 );
   if( u32Count > 480*16 )
      u32Count = 480*16;

   /* 读取数据 */
   for( ii = 0, u32Row = 0; ii < u32Count; ii++ )
   {
      ( void )MrmReadReg( 0x31, &u8Temp );
      if(( ii % 16 )== 15 )
         u32Row++;
      else
         u32ColBrightness[ ii % 16 ] += u8Temp;    // 放弃每一行的第16个点
   }
   ( void )MrmWriteReg( 0x07, 0x01 );  // Chip software reset
   ( void )MrmWriteReg( 0x08, 0xC3 );  // LED on
   DelayMS( 10 );

   /* 校正窗口位置 */
   g_SensorPara.x = 0;
   g_SensorPara.y = 0;
   for( ii = 1; ii < 15; ii++ )  // 查找最亮行
   {
      if( u32RowBrightness[ ii ] > u32RowBrightness[ g_SensorPara.y ])
         g_SensorPara.y = ii;
   }

   for( ii = 1; ii < 15; ii++ )  // 查找最亮列
   {
      if( u32ColBrightness[ ii ] > u32ColBrightness[ g_SensorPara.x ])
         g_SensorPara.x = ii;
   }

   /* 计算窗口位置 */
   g_SensorPara.x *= 42;
   g_SensorPara.y *= 32;
   if(g_SensorPara.x < 360/2 )
      g_SensorPara.x = 0;
   else
      g_SensorPara.x -= 360/2;
   if(g_SensorPara.y < 360/2 )
      g_SensorPara.y = 0;
   else
      g_SensorPara.y -= 360/2;
   ( void )MrmWriteReg( 0x12, g_SensorPara.y );          // 行
   ( void )MrmWriteReg( 0x13, g_SensorPara.x % 0x100 );  // 列低字节
   ( void )MrmWriteReg( 0x14, g_SensorPara.x / 0x100 );  // 列高字节
   return mrmTrue;
}


/* 读取SENSOR校正参数 */
mrmBool _GetSensorPara( mrmWord *pStartX, mrmWord *pStartY, mrmWord *pExpose )
{
   *pStartX = g_SensorPara.x;
   *pStartY = g_SensorPara.y;
   *pExpose = g_SensorPara.expose;
   return mrmTrue;
}


/* 设置SENSOR校正参数 */
mrmBool _SetSensorPara( mrmWord startX, mrmWord startY, mrmWord expose )
{
   g_SensorPara.x = startX;
   g_SensorPara.y = startY;
   g_SensorPara.expose = expose;

   ( void )_MrmI2cWrite( 0x03, g_SensorPara.expose / 0x100 );
   ( void )_MrmI2cWrite( 0x04, g_SensorPara.expose % 0x100 );
   ( void )MrmWriteReg( 0x12, g_SensorPara.y );          // 行
   ( void )MrmWriteReg( 0x13, g_SensorPara.x % 0x100 );  // 列低字节
   ( void )MrmWriteReg( 0x14, g_SensorPara.x / 0x100 );  // 列高字节
   return mrmTrue;
}


#pragma CODE = MRM_COMMON_CODE

mrmBool AdjustExpose( void )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _AdjustExpose();
}


mrmBool WinInit( void )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _WinInit();
}


mrmBool GetSensorPara( mrmWord *pStartX, mrmWord *pStartY, mrmWord *pExpose )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _GetSensorPara( pStartX, pStartY, pExpose );
}

mrmBool SetSensorPara( mrmWord startX, mrmWord startY, mrmWord expose )
{
   SDKLoadCode( MRM_INIT_CODE );
   return _SetSensorPara( startX, startY, expose );
}


