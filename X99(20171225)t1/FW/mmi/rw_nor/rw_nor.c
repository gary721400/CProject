#ifndef _rw_nor_c_
#define _rw_nor_c_

//*************************************************************
#include "lib_mcu\compiler.h"
#include "conf\conf_lcd.h"
#include "sio\com.h"
#include "SSSMP3_Regs.h"
#include "DP8051.H"
#include "define.h"
#include "rw_nor.h"
#include "mmi\images\ascii.h"


_MEM_TYPE_SLOW_   U32   address_unicode = 0;
_MEM_TYPE_SLOW_   U8    display_data   _at_( 0xB7FE );


code U8 animation_picture[ ] =
{
   0x02, 0x04, 0x0D, 0x07, 0x1C, 0x7F, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x7F,
                           0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F,
	                        0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x5D, 0x5D, 0x41, 0x7F,
	                        0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x7F

};


#if( LCD_PRESENT == ENABLED )
/*void Disp_sys_set_position( U32 IDP )
{
   nor_flash_address = nor_flash_bass_address + IDP;
   
   //== Read Flash data using XDATA[0x700] Liner ROM ..
   //Start address
   XBYTE[ 0x705 ] = ( U8 )( nor_flash_address >> 16 );
   XBYTE[ 0x706 ] = ( U8 )( nor_flash_address >> 8 );
   XBYTE[ 0x707 ] = ( U8 )nor_flash_address;
}
*/

#pragma disable
U8 Disp_sys_get_data_and_fetch_next_one( void )
{
   if( g_b_animation )
   {
      return animation_picture[ nor_flash_address++ ];
   }

   nor_flash_address++;
   XBYTE[ 0x709 ] = ( U8 )( nor_flash_address >> 16 );
   XBYTE[ 0x70A ] = ( U8 )( nor_flash_address >> 8 );
   XBYTE[ 0x70B ] = ( U8 )nor_flash_address;

   XBYTE[ 0x70C ] = 0xB7;
   XBYTE[ 0x70D ] = 0xFE;

   //XBYTE[ 0x70E ] = 0xB7;
   //XBYTE[ 0x70F ] = 0xFF;

   XBYTE[ 0x702 ] = 0x80;  // enable LROM SRAM buffer access
   do
   {
      XBYTE[ 0x701 ] = XBYTE[ 0x700 ];
   }
   //while(( XBYTE[ 0x702 ]& 0x01 )== 0 );
   while( XBYTE[ 0x702 ] & 0x80 ); // wait bit7 == 0 indicate SRAM buffer access end

   return XBYTE[ 0xB7FE ];
}

#if 0
U8 Read_byte_from_nor( U32 nor_addr )
{
   if( !nor_addr )
      return 0;
   
   //== Read Flash data using XDATA[0x700] Liner ROM ..
   //Start address
   XBYTE[ 0x705 ] = ( U8 )( nor_addr >> 16 );
   XBYTE[ 0x706 ] = ( U8 )( nor_addr >> 8 );
   XBYTE[ 0x707 ] = ( U8 )nor_addr;

   nor_addr++;

   XBYTE[ 0x709 ] = ( U8 )( nor_addr >> 16 );
   XBYTE[ 0x70A ] = ( U8 )( nor_addr >> 8 );
   XBYTE[ 0x70B ] = ( U8 )nor_addr;

   XBYTE[ 0x70C ] = 0xB0;
   XBYTE[ 0x70D ] = 0x00;  

   XBYTE[ 0x70E ] = 0xB0;
   XBYTE[ 0x70F ] = 0x01;   

   XBYTE[ 0x702 ] = 0x80;  // enable LROM SRAM buffer access
   do
   {
      XBYTE[ 0x701 ] = XBYTE[ 0x700 ];
   }
   //while(( XBYTE[ 0x702 ]& 0x01 )== 0 );
   while( XBYTE[ 0x702 ] & 0x80 ); // wait bit7 == 0 indicate SRAM buffer access end

   return XBYTE[ 0xB000 ];
}


void Set_font_address( U8 type )
{
   switch( type )
   {
      case FONT_8:
      address_unicode = FONT_8_ADDRESS_UNICODE;
      break;
      
      case FONT_16:
      address_unicode = FONT_16_ADDRESS_UNICODE;
      break;
      
      case FONT_24:
      address_unicode = FONT_24_ADDRESS_UNICODE;
      break;
   }
}


U8	Get_font_heigh( void )
{
	return Read_byte_from_nor( address_unicode + HEIGH_OFFSET_ADDRESS );
}


U8	Get_font_width( U16 unicode_data )
{
   U32 temp_add, add;
   U16 low_add;
   temp_add = 0;
   add = address_unicode;

   low_add = LSB( unicode_data )* 2;
	
   add += MSB( unicode_data )* 3;
	

	temp_add+=Read_byte_from_nor(add++);
	temp_add = temp_add<<8;

   temp_add+=Read_byte_from_nor(add++);
	temp_add = temp_add<<8;

	temp_add+=Read_byte_from_nor(add++);
   add = temp_add;
   
	temp_add += address_unicode + (U32)low_add;
	low_add = 0;
	low_add+=Read_byte_from_nor(temp_add++);
	low_add = low_add<<8;	
	low_add+=Read_byte_from_nor(temp_add);

	return Read_byte_from_nor(address_unicode+add+(U32)low_add);
}


U32	Get_addr_lcddata(U16 unicode_data)
{
	U32	temp_add,add;
	U16 low_add;
	
	add=address_unicode;
	low_add=LSB(unicode_data)*2;
	
	add += MSB(unicode_data)*3;
	temp_add=0;
	
	temp_add+=Read_byte_from_nor(add++);
	temp_add = temp_add<<8;

	temp_add+=Read_byte_from_nor(add++);
	temp_add = temp_add<<8;

	temp_add+=Read_byte_from_nor(add);

    add = temp_add;
	temp_add += address_unicode + (U32)low_add;
	low_add = 0;
	low_add+=Read_byte_from_nor(temp_add++);
	low_add = low_add<<8;	
	low_add+=Read_byte_from_nor(temp_add);
//	temp_add += BEGIN_ADDRESS_UNICODE + (U32)low_add + 1;
	return (address_unicode+add+(U32)low_add + 1);
}
#endif

#endif//#if( LCD_PRESENT==ENABLED )



#endif   //_rw_nor_c_


