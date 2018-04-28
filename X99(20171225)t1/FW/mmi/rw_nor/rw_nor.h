#ifndef _RW_NOR_H_
#define _RW_NOR_H_

#include "lib_mcu\compiler.h"
#include "conf\conf_lcd.h"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _rw_nor_c_)
#  define _GLOBEXT_
#else                                        
#  define _GLOBEXT_ extern
#endif

#define FONT_8                      1
#define FONT_16                     2
#define FONT_24                     3

#define GB2312_ADDRESS              0x00080000   
#define BG5_ADDRESS                 0x00090000
#define PICTURE_BEGIN_ADDRESS       0x000A0000

#define FONT_8_ADDRESS_UNICODE      0x000B0000
#define FONT_16_ADDRESS_UNICODE     0X000C0000
#define FONT_24_ADDRESS_UNICODE     0X001C0000


#define HEIGH_OFFSET_ADDRESS        0x00000300
//#define LOW_BEGIN_ADDRESS        	(BEGIN_ADDRESS_UNICODE+HEIGH_OFFSET_ADDRESS+5)	


/*
typedef union
{
   struct
   {
      Byte B3;
      Byte B2;
      Byte B1;
      Byte B0;
   } BYTE;
   unsigned long LONG;
} LongByte, *PLONGBYTE, LONGBYTE;
*/
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   nor_flash_address;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   nor_flash_bass_address;

_GLOBEXT_                     Bool  g_b_animation
#ifdef _rw_nor_c_
   = FALSE
#endif
;


#if( LCD_PRESENT == ENABLED )
void  Disp_sys_set_position                  ( U32 IDP );
U8    Disp_sys_get_data_and_fetch_next_one   ( void );

void  Read_data_from_nor   ( U32 nor_addr, U16 Sram_add, U16 len );
U8    Read_byte_from_nor   ( U32	nor_addr );
U8    Get_font_heigh       ( void );
U8    Get_font_width       ( U16 unicode_data );
U32   Get_addr_lcddata     ( U16 unicode_data );
void  Set_font_address     ( U8 type );
#endif   //#if( LCD_PRESENT == ENABLED )

void  eraseSector          ( U32 );  
void  byteProgram          ( U32, Byte );

#endif   //_RW_NOR_H_


