//=======================================================================
// Copyright: 2008 Solid State System Co., Ltd.
// Project  : 3S1120 USB ISP demo 
// File     : ISP.h
// Author   :  
// Date     : 2008/8/22
// Version  : 
// Abstract : 
//=======================================================================
// use the NOR Flash Pm39LV512

#define SST39VF1681					1


#define NOR_FLASH					SST39VF1681

enum ISP_function_sel
{
   L_ReadID = 0,
   L_EraseSector,    
   L_ProgramByte
};


//Reset CMD
#define NF_ResetCMD()                                       \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xF0;                                   \
}


#define SD_ReadSector()                                     \
{                                                           \
   SDVar.ErrVal = 0;	                                    \
   BLKLEN11_08 = 0x02;                                      \
   BLKLEN07_00 = 0x00;                                      \
   DATACON = 0x00;                                          \
   ARG31_24 = SDVar.FlashAddr[ 0 ];                         \
   ARG23_16 = SDVar.FlashAddr[ 1 ];                         \
   ARG15_08 = SDVar.FlashAddr[ 2 ];                         \
   ARG07_00 = SDVar.FlashAddr[ 3 ];                         \
   CMDCON = 0x06;                                           \
   CMDINDEX = READ_SINGLE_BLOCK;                            \
                                                            \
   while( !( STATUS07_00 & BIT0 ));                         \
   while( !( STATUS07_00 & BIT1 ))                          \
   {                                                        \
      if( STATUS07_00 & BIT2 )                              \
      {                                                     \
         SDVar.ErrVal = 1;                                  \
         break;              	                            \
      } 	                                                \
                                                            \
      if( STATUS07_00 & BIT3 )                              \
      {                                                     \
         SDVar.ErrVal = 1;                                  \
         break;              	                            \
      }                                                     \
   }                                                        \
                                                            \
   while( !( STATUS07_00 & BIT7 ))                          \
   {                                                        \
      if( STATUS07_00 & BIT6 )                              \
      {                                                     \
         SDVar.ErrVal = 1;                                  \
         break;              	                            \
      } 	                                                \
                                                            \
      if( STATUS07_00 & BIT4 )                              \
      {                                                     \
         SDVar.ErrVal = 1;                                  \
         break;              	                            \
      }                                                     \
   }                                                        \
                                                            \
   STATUS07_00 = 0xFF;                                      \
                                                            \
   if( SDVar.ErrVal )                                       \
   {                                                        \
      MP3_SWReset();                                        \
      /*MP3_PrevInit();*/SSSCode_PrevInit();                \
      SYSDelay();                                           \
      EIE = 0x01;                                           \
   }                                                        \
}


// NOR Flash chip erase
//== R/W Flash data using XDATA[0x700]
#define NF_ChipErase()                                      \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x55;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x2A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x55;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x80;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x55;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x2A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x55;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x10;                                   \
}


//Erase sector PMC
/*
#define NF_EraseSector()                                    \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x2A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x80;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x02;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = lba.BYTE.B1;                            \
   XBYTE[ 0x707 ] = lba.BYTE.B0;                            \
   XBYTE[ 0x700 ] = 0x30;                                   \
}
*/

//      XBYTE[ 0x700 ] = 0x00;  
// Read Toggle bit for Erase Chip.. but it like no help
#define NF_ReadToggleBit()                                  \
{                                                           \
   do                                                       \
   {                                                        \
      XBYTE[ 0x705 ] = lba.BYTE.B2/*0x00*/;                 \
      XBYTE[ 0x706 ] = lba.BYTE.B1;                         \
      XBYTE[ 0x707 ] = lba.BYTE.B0;                         \
      TByte1 = XBYTE[ 0x700 ];                              \
                                                            \
      XBYTE[ 0x705 ] = lba.BYTE.B2/*0x00*/;                 \
      XBYTE[ 0x706 ] = lba.BYTE.B1;                         \
      XBYTE[ 0x707 ] = lba.BYTE.B0;                         \
      TByte2 = XBYTE[ 0x700 ];                              \
	                                                        \
      if(( TByte1 & 0x40 ) == ( TByte2 & 0x40 ))            \
      {                                                     \
         break;                                             \
      }	                                                    \
   }                                                        \
   while( 1 );                                              \
}


// byte program .. 512 byte PMC
/*
#define NF_ByteProgram()                                    \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x02;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0xA0;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = lba.BYTE.B1;                            \
   XBYTE[ 0x707 ] = lba.BYTE.B0;                            \
   XBYTE[ 0x700 ] = XBYTE[ 0xB000 + ui_j ];                 \
}
*/



//Erase sector SST
#define NF_EraseSector()                                    \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x0A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x0A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0x80;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x0A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = lba.BYTE.B2/*0x00*/;                    \
   XBYTE[ 0x706 ] = lba.BYTE.B1;                            \
   XBYTE[ 0x707 ] = lba.BYTE.B0;                            \
   XBYTE[ 0x700 ] = 0x50;                                   \
}


// byte program .. 512 byte SST
#define NF_ByteProgram()                                    \
{                                                           \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x0A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0xAA;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x05;                                   \
   XBYTE[ 0x707 ] = 0x55;                                   \
   XBYTE[ 0x700 ] = 0x55;                                   \
                                                            \
   XBYTE[ 0x705 ] = 0x00;                                   \
   XBYTE[ 0x706 ] = 0x0A;                                   \
   XBYTE[ 0x707 ] = 0xAA;                                   \
   XBYTE[ 0x700 ] = 0xA0;                                   \
                                                            \
   XBYTE[ 0x705 ] = lba.BYTE.B2/*0x00*/;                    \
   XBYTE[ 0x706 ] = lba.BYTE.B1;                            \
   XBYTE[ 0x707 ] = lba.BYTE.B0;                            \
   XBYTE[ 0x700 ] = XBYTE[ 0xB800 + ui_j ];                 \
}