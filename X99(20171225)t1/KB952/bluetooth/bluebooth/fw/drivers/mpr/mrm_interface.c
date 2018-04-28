#include <sfr.h>
#include <stdlib.h>
#include <stdio.h>
#include "KBDM9_DDK.H"
#include "mrm_lite.h"
#include "spi.h"

#pragma CODE = MRM_COMMON_CODE
#pragma DATA = MRM_DATA

//mrmByte g_MprPath[ 32 ] = "0:\\MPR\\";

void DelayUS( mrmWord us )
{
   while( --us )
   {
      __asm
      {
         REP #80
         NOP
      }
   }
}


void DelayMS( mrmWord ms )
{
   while( ms-- )
      DelayUS( 1000 );
}


#pragma CODE = MRM_INIT_CODE

void InitMrmInterface( void )
{
   InitSPI();
#if 0//DEMO_BOARD
   PBEN  &= 0xFF9FU;   // PB5, PB6
   PBDIR &= 0xFF9FU;
#else
   //PBEN  &= 0xFCFFU;    // PB8, PB9
   //PBDIR |= 0x0300;    // PB8, PB9 IN

   /*PCEN  &= 0xFFD7U;   // PC3, PC5
   PCDIR &= 0xFFF7U;   // PC3 OUT
   PCDIR |= 0x0020;    // PC5 IN

   PDEN  &= 0xFF5FU;   // PD5, PD7
   PDDIR &= 0xFF5FU;   // PD5, PD7 OUT*/

   //MAIN_POWER_ON();
#endif
   /* …œµÁ */
   MRM_PWRDN_DIS();

   /* ∏¥Œª */
   MRM_RESET_LOW();
   DelayMS( 100 );
   MRM_RESET_HIGH();
   DelayMS( 1 );
}


#pragma CODE = MRM_COMMON_CODE

void InitSPI( void )
{
   /*mrmWord temp;

   SP1CR = 0x8044;
   temp = ( PBEN & 0xFFF0U )| 0x000E;
   PBEN  = temp;
   temp = ( PBDIR & 0xFFF0U )| 0X0004;
   PBDIR = temp;
   SPI_CS_HIGH();*/

   PBEN |= ( BIT( 13 )| BIT( 12 )| BIT( 11 ));
   SP2CR = SPI_SPE( 1 )|
               SPI_SPFIE( 0 )|
               SPI_SPIS( 0 )|
               SPI_SPIE( 0 )|
               SPI_MFEN( 0 )|
               SPI_MSTR( 1 )|
               SPI_MODE( SPI_Mode0 )|
               SPI_SPR( SPI_10MHz );
   SPI_CS_HIGH();
}


mrmBool SPI_Transmit( mrmByte *pData, mrmByte size )
{
   mrmWord cnt;

   while( size-- )
   {
      SP2DR = *pData;
      cnt = 5000;
      while(( !( SP2SR & 0x80 ))&&( cnt-- ));
      if( !cnt )
         return 0;
      pData++;
      SP2SR = 0;
   }
   return 1;
}


/*
mrmByte *GetMprPath( void )
{
   return g_MprPath;
}
*/

