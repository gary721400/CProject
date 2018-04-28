//=======================================================================
// Copyright: 2008 Solid State System Co., Ltd.
// Project  : 
// File     : vendor.h
// Author   :  
// Date     : 2008/8/22
// Version  : 
// Abstract : 
//=======================================================================

//This is a vendor command(cdb[0])
#define  OP_3S_VENDOR_CDB        0xFF
//The followings are sub vendor command(cdb[1])
#define  OP_null_01              0x01

#define  OP_GET_DEVICE_INFO      0x02
#define  OP_null_03              0x03
#define  OP_null_04              0x04
#define  OP_null_05              0x05
#define  OP_null_06              0x06

#define  OP_NOR_SectorErase      0x07
#define  OP_null_08              0x08

#define  OP_NOR_WRITE512         0x09
#define  OP_NOR_READ512          0x0A
#define  OP_NOR_ID               0x0B
#define  OP_null_0c              0x0C
#define  OP_null_0d              0x0D
#define  OP_null_0e              0x0E
#define  OP_null_0f              0x0F
#define  OP_null_10              0x10
//these two commands are for CBI
#define  OP_SRAM_READ            0x11
#define  OP_SRAM_WRITE           0x12
#define  OP_null_13              0x13
#define  OP_null_14              0x14
#define  OP_null_15              0x15
#define  OP_null_16              0x16
#define  OP_null_17              0x17
#define  OP_null_18              0x18
#define  OP_null_19              0x19
#define  OP_null_1a              0x1A
#define  OP_null_1b              0x1B
#define  OP_null_1c              0x1C
#define  OP_null_1d              0x1D
#define  OP_null_1f              0x1F
#define  OP_null_20              0x20
#define  OP_null_21              0x21
#define  OP_WRITE10              0x2A


void  OP_Vendor_GetDeviceInfo ( void );
void  OP_Vendor_Null          ( void );
void  OP_Vendor_SramAccess    ( void );
void  OP_Vendor_SectorERASE   ( void );
void  OP_Vendor_NorWrite512   ( void );
void  OP_Vendor_NorRead512    ( void );
void  OP_Vendor_ReadNorID     ( void );



