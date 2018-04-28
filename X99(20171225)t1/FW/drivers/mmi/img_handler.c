//!
//! @file img_handler.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the graphical library
//!
//! Routines to get mmi graphical object data from different storage sources
//! The file "conf\conf_mmi.h" defines where graphical objects are to fetch
//!
//! @version 1.29 snd3-dvk-1_9_5 $Id: img_handler.c,v 1.29 2007/06/25 12:53:07 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _img_handler_c_
#define _img_handler_c_


//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_lcd.h"
#include "lib_system\unicode\unicode.h"
#include "modules\control_access\ctrl_access.h"
#include "img_handler.h"
#include "sio\com.h"
#include "conf\conf_access.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fs.h"


#pragma DATA = IMG_HANDLER_DATA
#pragma CODE = IMG_HANDLER_CODE

__X   U32   Start_Sector, Length;
__X   U8    Buf[ 512 ];

_MEM_TYPE_SLOW_   U8    ih_g_curr_font;

_MEM_TYPE_SLOW_   U32   nor_flash_address;
_MEM_TYPE_SLOW_   U32   nor_flash_bass_address;

                  Bool  g_b_animation /*= FALSE*/;
/*
const U8 animation_picture[ ] =
{

   0x02, 0x04, 0x0D, 0x07, 0x1C, 0x7F, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x7F,
                           0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x7F,
                           0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x5D, 0x5D, 0x41, 0x7F,
                           0x1C, 0x7F, 0x41, 0x41, 0x41, 0x41, 0x5D, 0x5D, 0x41, 0x5D, 0x5D, 0x41, 0x7F

};
*/
//_____ D E F I N I T I O N S ______________________________________________

#pragma CODE = LCD_SSD1303_TEMP_CODE01


#if( LCD_PRESENT == ENABLED )
//! ih_get_font_height
//!
//! Get the height of the current font
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//! @return U8    height of the font
//!
U8 ih_get_font_height( void )
{
   nfloader_compute_physic_addr( 768 );
   //nfloader_open_page_for_reading_via_register();
   return( nfloader_get_data_and_fetch_next_one() );
}


//! ih_ptr_char
//!
//! To set the pointer to the character of the current font
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  char ch : character to display
//! @return none
//!
void ih_ptr_char( U16 wc )
{
   //static _MEM_TYPE_SLOW_ U8  s_prev_table_idx = 0xFF;
   static _MEM_TYPE_SLOW_ U32 s_table_addr;   
   U16  offset_glyph;


   //if( s_prev_table_idx != MSB(wc) )
   {
      // This character and the previous one don't belong to the same glyph table.
      // So, the address of the glyph table is to compute back
      while( 1 )
      {
         nfloader_compute_physic_addr( MSB(wc)*3 );
         //nfloader_open_page_for_reading_via_register();
         MSB0(s_table_addr) = 0;
         MSB1(s_table_addr) = nfloader_get_data_and_fetch_next_one();
         MSB2(s_table_addr) = nfloader_get_data_and_fetch_next_one();
         MSB3(s_table_addr) = nfloader_get_data_and_fetch_next_one();
         if( 0xFFFFFF != s_table_addr )
         {
            // glyph table is well-defined
            break;
         }
         // The glyph table is not defined, the undefined character is applied.
         // So, the address of its glyph table is to compute again.
get_undef_char:
         wc = 0x007F; 
      }
   }
   //s_prev_table_idx = MSB(wc);
   nfloader_compute_physic_addr( s_table_addr + LSB(wc) *2);
   //nfloader_open_page_for_reading_via_register();
   MSB(offset_glyph) = nfloader_get_data_and_fetch_next_one();
   LSB(offset_glyph) = nfloader_get_data_and_fetch_next_one();
   if( 0xFFFF == offset_glyph )
      { goto get_undef_char; }
   nfloader_compute_physic_addr( s_table_addr + offset_glyph );
   //nfloader_open_page_for_reading_via_register(); 
}

//! @brief ih_add_font_offset
//! Add offset to the current read position in the font
//! @param  U8 offset   offset to add to the current position
//! @return none
void ih_add_font_offset( U8 offset )
{
   while( 0 != offset-- )
   {
      ( void )Disp_sys_get_data_and_fetch_next_one();
   }
}
#endif  //#if( LCD_PRESENT == ENABLED )

#if 0
//! @brief ih_get_string_length_in_dot
//! Return the string length in dot unit
//! @param  U8* string              input string
//! @return U8                      length in dots
U8 ih_get_string_length_in_dot( U8* string )
{
   U8 dot_nb = 0;
   U16 s_wc;

   // Skip the the unicode buffer markers
   if( Unicode_is_string_from_unicode_buffer(string) )
      { string = Unicode_get_addr_string(string); }

   while(1)
   {
      s_wc = *((U16*)string)++;

      if( 0 == s_wc )
         { break; }

      ih_ptr_char( s_wc );  
      dot_nb += Ih_get_font_data();
   }
   return(dot_nb);
}
#endif

#if (EMBEDDED_CODE_FONT == ENABLE)
//! ih_ptr_code_char
//!
//! To set the pointer to the character of the current font
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  char ch : character to display
//! @return none
//!
void ih_ptr_code_char( U16 wc )
{
   union
   {
      char first_char;
      U8   dummy;
      U16  offset_char;
   }v0;

   // Set ptr_font to point the current font
   Ih_open_code_font_access();

   // Only for incrementing the pointer (Current data : font height)
   v0.dummy = Ih_get_code_font_data();

   // Get the first character defined in this font
   v0.first_char = (char) Ih_get_code_font_data();

   // Test if the character is not beyond the last character defined in this font
   if( LSB(wc) > (char) Ih_get_code_font_data() )
   {
     // Character is not defined in the table
     // (its index is up to the last one defined)
      LSB(wc) = 0;
   }
   else
   {
      LSB(wc) -= v0.first_char;
      if( LSB(wc) >= 0 )
      { // Character is defined in the table
         LSB(wc)++;
      }
      else
      { // Character is not defined in the table 
        // (its index is inferior to the first one defined)
         LSB(wc) = 0;
      }
   }

   Ih_add_code_font_offset( (U16)LSB(wc) * sizeof(U16) );
   LSB(v0.offset_char) = Ih_get_code_font_data();
   MSB(v0.offset_char) = Ih_get_code_font_data();
   Ih_add_code_font_offset( v0.offset_char );
}
#endif //#if ((FUNC_UPDATE == ENABLE) & (UPDATE_DISPLAY == ENABLE))


#pragma CODE = IMG_HANDLER_CODE

void nfloader_compute_physic_addr( U32 log_addr )
{
   U32 sector;
   
   if( g_b_animation )
   {
      nor_flash_address = log_addr -( IDP_BATT_CHARG );
      return;
   }

   nor_flash_address = nor_flash_bass_address + log_addr;
   //sector = nor_flash_address / 512ul;
   sector = nor_flash_address >> 9;
   if( sector != Start_Sector )
   {
      Start_Sector = sector;
      spif_resv_read( Start_Sector, ( __X void *)Buf );
   }
}


U8 Disp_sys_get_data_and_fetch_next_one( void )
{
   U32 sector;
   
   if( g_b_animation )
   {
 //     return animation_picture[ nor_flash_address++ ];
 		return 0;
   }

   //sector = nor_flash_address / 512ul;
   sector = nor_flash_address >> 9;
   if( sector != Start_Sector )
   {
      Start_Sector = sector;
      spif_resv_read( Start_Sector, ( __X void *)Buf );
   }
   //sector = nor_flash_address % 512ul;
   sector = nor_flash_address & 0x1FF;

   nor_flash_address++;

   return Buf[ sector ];
}


#pragma CODE = IMG_HANDLER_TEMP_CODE00

//! Interface for loading display objects through the register only
#if (ACCESS_DISPLAY == ENABLED)
//! Init the loading of a display object
//!
//! @param  id_module_display id of the display object to load
//!         display_zone      zone in which this object is stored
//!
//! @return status      PASS or FAIL about the loading
//!
void disp_sys_init_load( U8 id_module_display, U8 display_zone )
{
   U16 dataID;

   //if( g_b_animation )
   //   return;
#if 1

	if(display_zone == ZONE_PICTURE) dataID = D_PIC;
	if(display_zone == ZONE_FONT ){
		if(id_module_display == FONT_TBL_GB2312){
			dataID = D_GB2312;
		 }else if(id_module_display == FONT_TBL_B5){
		 	dataID = D_BIG5;
		 }else if(id_module_display == IDC_FONT_16){
		 	dataID = D_SIM16;
		 }else if(id_module_display == IDC_FONT_24){
		 	dataID = D_SIM24;
		 }else{
		 	return;
		 	}
		}
#else	
   switch( display_zone )
   {
      case ZONE_PICTURE:
      dataID = D_PIC;
      break;

      case ZONE_FONT:
      switch( id_module_display )
      {
         case FONT_TBL_GB2312:
         dataID = D_GB2312;
         break;

         case FONT_TBL_B5:
         dataID = D_BIG5;
         break;

         case IDC_FONT_16:
         dataID = D_SIM16;
         break;

         /*case IDC_FONT_08:
         dataID = D_SIM8;
         break;*/

         case IDC_FONT_24:
         dataID = D_SIM24;
         break;
      }
      break;
   }
#endif
   GetDataIdInfo( dataID, &Start_Sector, &Length );
   spif_resv_read( Start_Sector, ( __X void *)Buf );
   //nor_flash_bass_address = Start_Sector * 512ul;
   nor_flash_bass_address = Start_Sector << 9;
}
#endif

#endif   // _img_handler_c_

