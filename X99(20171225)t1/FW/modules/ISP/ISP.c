//=======================================================================
// Copyright: 2008 Solid State System Co., Ltd.
// Project  : 3S1120 USB ISP demo 
// File     : ISP.c
// Author   :  
// Date     : 2008/8/22
// Version  : 
// Abstract : 
//=======================================================================

// use the NOR Flash Pm39LV512 

#include "DP8051.H"
#include "header.h"
#include "ISP.h"

//extern         bit            bSysUSBLedTog;
extern   xdata AL_Variables   varAl;

extern   idata unsigned int   ui_j;
extern   idata unsigned char  uc_ISP_sel;

//extern   idata unsigned       char FlashData[4]; 
extern   idata unsigned short PMC_ID;

#define ISP_LBATable          (( ULONG xdata * )0x2800 )

#define LED_ON       0
#define LED_OFF      1


void exeinram( void )
{

   idata unsigned char TByte1, TByte2;
    
   /*switch( uc_ISP_sel )
   {
      case L_ReadID:
      goto ReadID;

      case L_EraseSector:
      goto exeinram_EraseSector;

      case L_ProgramByte:
      goto exeinram_ProgramByte;

      default:
      return;
   }*/
   if( uc_ISP_sel == 0 )
      goto ReadID;
   if( uc_ISP_sel == 1 )
      goto exeinram_EraseSector;
   if( uc_ISP_sel == 2 )
      goto exeinram_ProgramByte;
   return;
//-------------------------------------    
ReadID:
#if (NOR_FLASH==SST39VF1681)
   //=== Read ID OK   
   NF_ResetCMD();
   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0xAA;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x05;
   XBYTE[ 0x707 ] = 0x55;
   XBYTE[ 0x700 ] = 0x55;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0x90;

   //for( i = 0; i < 0x50; i++ );  //8ms
   for( varAl.i = 0; varAl.i < 0x1fff; varAl.i++ );//8ms

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x00;
   XBYTE[ 0x707 ] = 0x00;
   PMC_ID = XBYTE[ 0x700 ];

   PMC_ID <<= 8;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x00;
   XBYTE[ 0x707 ] = 0x01;

   PMC_ID |= XBYTE[ 0x700 ];
#elif
//=== Read ID OK   
   NF_ResetCMD();
   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0xAA;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x05;
   XBYTE[ 0x707 ] = 0x55;
   XBYTE[ 0x700 ] = 0x55;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0x90;

   //for( i = 0; i < 0x50; i++ );  //8ms
   for( varAl.i = 0; varAl.i < 0x1fff; varAl.i++ );//8ms

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x00;
   XBYTE[ 0x707 ] = 0x00;
   PMC_ID = XBYTE[ 0x700 ];

   PMC_ID <<= 8;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0xAA;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x05;
   XBYTE[ 0x707 ] = 0x55;
   XBYTE[ 0x700 ] = 0x55;

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x0A;
   XBYTE[ 0x707 ] = 0xAA;
   XBYTE[ 0x700 ] = 0x90;

   //for( i = 0; i < 0x50; i++ );  //8ms
   for( varAl.i = 0; varAl.i < 0x1fff; varAl.i++ );//8ms

   XBYTE[ 0x705 ] = 0x00;
   XBYTE[ 0x706 ] = 0x00;
   XBYTE[ 0x707 ] = 0x02;

   PMC_ID |= XBYTE[ 0x700 ];
#endif   

   NF_ResetCMD();
   return;
	
exeinram_EraseSector:
   NF_ResetCMD();

   // Erase Sector
   NF_EraseSector();
     
   NF_ReadToggleBit();

   //Reset CMD
   NF_ResetCMD();
   return;

exeinram_ProgramByte:
   NF_ByteProgram();
   NF_ReadToggleBit();
}
