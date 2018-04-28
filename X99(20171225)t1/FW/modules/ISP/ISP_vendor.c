//=======================================================================
// Copyright: 2008 Solid State System Co., Ltd.
// Project  : 3S1120 USB ISP demo 
// File     : vendor.c
// Author   :  
// Date     : 2008/8/22
// Version  : 
// Abstract : 
//=======================================================================
#include "header.h"
#include "ISP.h"
#include "ISP_Vendor.h"
#include "intrins.h"

extern   xdata AL_Variables   varAl;
extern   xdata USB_Variables  USB;
extern   xdata U8             Isp_flag;

#define  MAX_VENDOR_COMMANDS  27
code CDB_FUN VendorCmdFun[ MAX_VENDOR_COMMANDS ]=
{
   {  OP_null_01,          OP_Vendor_Null          },
   {  OP_GET_DEVICE_INFO,  OP_Vendor_GetDeviceInfo },
   {  OP_null_03,          OP_Vendor_Null          },
   {  OP_null_04,          OP_Vendor_Null          },
   {  OP_null_05,          OP_Vendor_Null          },
   {  OP_null_06,          OP_Vendor_Null          },
   {  OP_NOR_SectorErase,  OP_Vendor_SectorERASE   },
   {  OP_NOR_WRITE512,     OP_Vendor_NorWrite512   },
   {  OP_NOR_READ512,      OP_Vendor_NorRead512    },
   {  OP_NOR_ID,           OP_Vendor_ReadNorID     },
   {  OP_null_0c,          OP_Vendor_Null          },
   {  OP_null_0d,          OP_Vendor_Null          },
   {  OP_null_0e,          OP_Vendor_Null          },
   {  OP_null_0f,          OP_Vendor_Null          },
   {  OP_SRAM_READ,        OP_Vendor_SramAccess    },
   {  OP_SRAM_WRITE,       OP_Vendor_SramAccess    },
   {  OP_null_13,          OP_Vendor_Null          },
   {  OP_null_14,          OP_Vendor_Null          },
   {  OP_null_15,          OP_Vendor_Null          },
   {  OP_null_18,          OP_Vendor_Null          },
   {  OP_null_19,          OP_Vendor_Null          },
   {  OP_null_1a,          OP_Vendor_Null          },
   {  OP_null_1f,          OP_Vendor_Null          },
   {  OP_null_1b,          OP_Vendor_Null          },
   {  OP_null_1c,          OP_Vendor_Null          },
   {  OP_null_1d,          OP_Vendor_Null          },
   {  OP_null_21,          OP_Vendor_Null          }
};

#define  Sector_ERASE_COMMAND             0x66  //erase one flash block
#define  P4_Chip_Erase_CMD                0x67  //erase one flash block
#define  P4_READ_NOR_ID_CMD               0x68  //read flash ID

#define  P4_READ_NOR_CMD                  0x74  //read 108H word without ECC
#define  P4_WRITE_NOR_CMD                 0x75  // read one block data out without ECC

extern   void  exeinram          ( void );      // Function to copy into RAM
extern   void  copy_ISPcode_SRAM ( void );

extern   idata unsigned int   RAMBuf_adr;
extern   idata unsigned short PMC_ID;
extern   idata unsigned int   ui_j;
extern   idata unsigned char  uc_ISP_sel;


void OP_Vendor_GetDeviceInfo( void )
{
   // Fill in meaningful bytes to sector buffer
   Sector_Buffer[ 0 ] = 0xF0;  // this byte for PC software check device
   Sector_Buffer[ 1 ] = 0;
   Sector_Buffer[ 2 ] = 1;
   Sector_Buffer[ 3 ] = 2;
   Sector_Buffer[ 4 ] = 3;
   Sector_Buffer[ 5 ] = 4;
   Sector_Buffer[ 6 ] = 5;
   Sector_Buffer[ 7 ] = 6;

   Sector_Buffer[ 8 ] = 7;
   Sector_Buffer[ 9 ] = 8;
   Sector_Buffer[ 10 ] = 9;
   Sector_Buffer[ 11 ] = 10;
   Sector_Buffer[ 12 ] = 11;
   Sector_Buffer[ 13 ] = 12; 
   Sector_Buffer[ 14 ] = 13; 
   Sector_Buffer[ 15 ] = 14; 
   frontend_SendData( CBW_DataLen.BYTE.B0 );
}


void OP_Vendor_Null( void )
{
   //SET_SenseData( SenseInvalidCdbField );
}

#if 1
void OP_Vendor_ReadNorID( void ) // none call in the SW .. ng
{
   copy_ISPcode_SRAM();
   IE &= 0x7F; // disable All interrupt
   uc_ISP_sel = L_ReadID;
   SFR_IPGSIZE = 0x06;     // On_Chip RAM 0x8000-0xFFFF, Off-Chip 0x0000-0x7FFF
   //VBUS_ISP_PGM |= 0x01;   //Enable ISP
   exeinram();
   //VBUS_ISP_PGM &= 0xFE;   //Disable ISP
   SFR_IPGSIZE = 0x00;     // Off-Chip 0x0000-0xFFFF
   IE |= 0x80;
	
   XBYTE[ 0xB800 ] = ( unsigned char )( PMC_ID >> 8 );
   XBYTE[ 0xB801 ] = ( unsigned char )( PMC_ID & 0x00FF );

   memcpy(( UCHAR xdata * )BUFFER0_ADDR, ( UCHAR xdata * )BUFFER8_ADDR, 0x200 );
   frontend_SendData( 0x8100 );

   return;
}
#endif

void OP_Vendor_SectorERASE( void )  // none call in the SW .. ng
{
   if( !Isp_flag )
      return;
   lba.BYTE.B0 = cdb[ 4 ];
   lba.BYTE.B1 = cdb[ 5 ];
   lba.BYTE.B2 = cdb[ 6 ];
   lba.BYTE.B3 = cdb[ 7 ];
   if( !(/*( lba.BYTE.B2 == 0x11 )&&*/( lba.BYTE.B3 == 0x55 )))
      return;

   copy_ISPcode_SRAM();
   IE &= 0x7F; // disable All interrupt
   uc_ISP_sel = L_EraseSector;
   //== ready to Run ISP function
   SFR_IPGSIZE = 0x06;     // On_Chip RAM 0x8000-0xFFFF, Off-Chip 0x0000-0x7FFF
   //VBUS_ISP_PGM |= 0x01;   //Enable ISP
   exeinram();
   //VBUS_ISP_PGM &= 0xFE;   //Disable ISP
   SFR_IPGSIZE = 0x00;     // Off-Chip 0x0000-0xFFFF
   IE |= 0x80;
}


//OK
void OP_Vendor_NorRead512( void ) 
{
   lba.BYTE.B0 = cdb[ 4 ];
   lba.BYTE.B1 = cdb[ 5 ];
   lba.BYTE.B2 = cdb[ 6 ];
   lba.BYTE.B3 = 0;

   //== Read Flash data using XDATA[0x700] Liner ROM ..
   //Start address
   XBYTE[ 0x705 ] = lba.BYTE.B2;
   XBYTE[ 0x706 ] = lba.BYTE.B1;
   XBYTE[ 0x707 ] = lba.BYTE.B0;
   //End address
   lba.LONG += 0x200;

   XBYTE[ 0x709 ] = lba.BYTE.B2;
   XBYTE[ 0x70A ] = lba.BYTE.B1;
   XBYTE[ 0x70B ] = lba.BYTE.B0;

   XBYTE[ 0x70C ] = 0x80;
   XBYTE[ 0x70D ] = 0x00;                    

   XBYTE[ 0x70E ] = 0x82;
   XBYTE[ 0x70F ] = 0x00;                    

   XBYTE[0x702] = 0x80;             // enable LROM SRAM buffer access
   do
   {
      XBYTE[ 0x701 ] = XBYTE[ 0x700 ];
   }
   //while(( XBYTE[ 0x702 ]& 0x01 )== 0 );
   while( XBYTE[ 0x702 ] & 0x80 ); // wait bit7 == 0 indicate SRAM buffer access end

   //memcpy(( UCHAR xdata * )BUFFER0_ADDR, ( UCHAR xdata * )BUFFER8_ADDR, 0x200 );

   frontend_SendData( 0x8100 );
   buffer_ready_macro();
}


//OK
void OP_Vendor_NorWrite512( void )
{
   if( !Isp_flag )
      return;
   lba.BYTE.B0 = cdb[ 4 ];
   lba.BYTE.B1 = cdb[ 5 ];
   lba.BYTE.B2 = cdb[ 6 ];
   lba.BYTE.B3 = cdb[ 7 ];

   if( !(/*( lba.BYTE.B2 == 0x22 )&&*/( lba.BYTE.B3 == 0x66 )))
      return;

   frontend_RcvData( 0x8100 );

   memcpy(( UCHAR xdata * )BUFFER8_ADDR, ( UCHAR xdata * )BUFFER0_ADDR, 0x200 );

   copy_ISPcode_SRAM();

   for( ui_j = 0; ui_j < 512; ui_j++ )
   {
      IE &= 0x7F; // disable All interrupt
      uc_ISP_sel = L_ProgramByte;
      //== ready to Run ISP function
      //VBUS_ISP_PGM |= 0x01;   //Enable ISP
      SFR_IPGSIZE = 0x06;     // On_Chip RAM 0x8000-0xFFFF, Off-Chip 0x0000-0x7FFF
      exeinram();
      //VBUS_ISP_PGM &= 0xFE;   //Disable ISP
      SFR_IPGSIZE = 0x00;     // Off-Chip 0x0000-0xFFFF
      IE |= 0x80;
      lba.LONG++;
   }
}


void OP_Vendor_SramAccess( void )
{
#define address   sectorCnt
   
   address.BYTE.B0 = cdb[ 3 ];
   address.BYTE.B1 = cdb[ 4 ];
   if( cdb[ 2 ] == 1 )
   {
      //read xdata
#define bufSel    cdb[ 5 ]

      if( cdb[ 1 ] == OP_SRAM_READ )
      {
         memcpy(( UCHAR xdata * )BUFFER0_ADDR, ( UCHAR xdata * )address.INTEGER, 0x200 );	

         frontend_SendData( 0x8100 );
      }
      else
      {
         frontend_RcvData( 0x8100 );
         memcpy(( UCHAR xdata * )address.INTEGER, ( UCHAR xdata * )BUFFER0_ADDR, 0x200 );
      }

#undef bufSel
   }
   else
   {
      if( cdb[ 1 ] == OP_SRAM_READ )
      {
         memcpy(( UCHAR xdata * )BUFFER0_ADDR, ( UCHAR xdata * )address.INTEGER, 0x100 );	    

         frontend_SendData(0x8100);
      }
      else
      {
         frontend_RcvData( 0x8100 );
         memcpy(( UCHAR data * )address.INTEGER, ( UCHAR xdata * )BUFFER0_ADDR, 0x100 );
      }
   }
#undef address
}


/*
void frontend_RcvData( USHORT XferLen )
{
   if( XferLen == 0 )
      return;
	
   XferLen--;

   while(( USB_DMA_CTRL & 0x88 )!= 0 );

   BULK_EPE_DMA_EN_MACRO();

   USB_DMA_LEN0 = ( UCHAR )( XferLen );
   USB_DMA_LEN1 = ( UCHAR )( XferLen >> 8 ); 
   USB_DMA_CTRL = 0x09;
   while( USB_DMA_CTRL & 0x08 );
}


void frontend_SendData( USHORT XferLen )
{
   if( XferLen == 0 )
   {
      return;
   }

   XferLen--;

   while(( USB_DMA_CTRL & 0x88 )!= 0 );

   BULK_EPD_DMA_EN_MACRO();

   USB_DMA_LEN0 = ( UCHAR )( XferLen );
   USB_DMA_LEN1 = ( UCHAR )( XferLen >> 8 ); 
   USB_DMA_CTRL = 0x08;
   while( USB_DMA_CTRL & 0x08 );

   if( fUsbIsHighSpeed )
   {
      if( XferLen != 0x1FF )
      {
         FUSBPort[ REG_FIFO0_INSTRUCTION ] |= 0x08;   //mark short packet
      }
   }
   else
   {
      if( XferLen != 0x3F )
      {
         FUSBPort[ REG_FIFO0_INSTRUCTION ] |= 0x08;   //mark short packet
      }
   }
}
*/





