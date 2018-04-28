//! @file unicode.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the functions to support unicode
//!
//! @version 1.16 snd3-dvk-1_9_5 $Id: unicode.c,v 1.16 2007/06/25 09:31:52 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _unicode_c_
#define _unicode_c_

//_____  I N C L U D E S ____________________________________________________
#include "config.h"
#include "drivers\mmi\img_handler.h"
#include "modules\control_access\ctrl_access.h"
#include "mmi\shared\com_var.h"
#include "unicode.h"
#include "sio\com.h"
#include "conf\conf_sio.h"                   // Sio configuration
#include "conf\conf_lcd.h"
#include "string.h"
//#include "modules\paster\paster_base.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "fs.h"

#pragma DATA = UNICODE_DATA
#pragma CODE = UNICODE_CODE

ImageDeclareType( UNICODE_TEMP_CODE00 );
ImageDeclareType( UNICODE_TEMP_CODE01 );
ImageDeclareType( UNICODE_TEMP_CODE02 );

ImageDeclareType( IMG_HANDLER_TEMP_CODE00 );


UnicodeBuffer  g_conv_buffer[ SIZE_CONV_UNICODE_BUFFER ];

//_____ P R I V A T E   D E C L A R A T I O N S _____________________________
static void utf16_to_unicode( UnicodeBuffer *buffer, U8 text_encoding );
static void utf8_to_unicode(  UnicodeBuffer *buffer );
static void ascii_to_unicode( UnicodeBuffer *buffer, U8 lang );


//!_____ I N F O R M A T I O N S ____________________________________________
// The feature unicode is supported with the help of the functions of this module
// and with a dedicated buffer type called UnicodeBuffer. This buffer is initialized 
// with "Unicode_init_buffer()" reserving the 6 first bytes for processing status.
// The marker identifying this buffer type is "\xfe\xff\xff" or "\xfe\xff\xfe".
// This kind of buffer is mandatory to do string operations with different encodings
// since it converts first all input strings in big endian unicode encoding.
// If no encoding marker is not detected in the input string by the function "unicode_converter()",
// the value of the variable language hmi encoding is then taken into account ("g_setting_list.language_ihm").
//
// Otherwise, it is possible to input a string without the use of a UnicodeBuffer-type buffer.
// Indeed, the input string can be also declared in code or in a common xram buffer.
// Meanwhile, it is recommended to start the string encoding with the encoding marker.
// Below, an example to display in Chinese the name of the city Shanghai.
// The Chinese string is encoding in big-endian unicode in code with the ideograph codes
// SHANG HAI = 4E 0A  6D 77 = &#19978 &#28023 :
//    char code txt_unicode_chinese_shanghai[]="\xfe\xff\x4e\x0a\x6d\x77\0\0";   
//    lcd_puts(txt_unicode_chinese_shanghai);


//!_____ D E F I N I T I O N S ______________________________________________

#pragma CODE = SRV_PLAYER_TEMP_CODE02

//! Init the unicode buffer and set the encoding type
//!
void unicode_init_buffer_enc( U8 _MEM_TYPE_SLOW_ *buffer, U8 buffer_size , U8 encoding )
{
   Unicode_init_buffer( buffer, buffer_size );
   unicode_set_input_encoding( Unicode_get_addr_string( buffer ), encoding );
}


#pragma CODE = UNICODE_TEMP_CODE03

#if( LCD_PRESENT == ENABLED )
//! @brief unicode_converter
//!
//! Convert a string coming from the sytem into a unicode string if needed.
//! The system must update the "g_text_encoding" to say which encodings
//! could be used for the fisrt string and the rest of the text.
//! This is required as the BOM (Byte Order Marker at the beginning of strings)
//! is not always present.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 *p_buffer     : String to convert in unicode.
//                             Points a buffer declared in rom or in ram
//! @return UnicodeBuffer *s : Unicode buffer storing the converted string
UnicodeBuffer *unicode_converter( U8 *p_buffer )
{
   _MEM_TYPE_SLOW_ U8   lang;
   _MEM_TYPE_SLOW_ U8   text_encoding;

   // "p_buffer" doesn't point a unicode buffer
   // String will be so copied in the temporary unicode buffer "g_conv_buffer"
   if( !Unicode_is_string_from_unicode_buffer(p_buffer) )
   {  
      lang = g_setting_list.struct_setting.language_ihm;
      text_encoding = ENCODING_ASCII;     // by default ascii, changing later only if BOM detected

      Unicode_init_buffer(g_conv_buffer, SIZE_CONV_UNICODE_BUFFER);
      SDKLoadCode( UNICODE_TEMP_CODE02 );
      unicode_append_unicode_strings(p_buffer, g_conv_buffer);
      p_buffer = g_conv_buffer;
   }

   // String of the pointed unicode buffer has been already converted. So return right now.
   else if( Unicode_is_unicode_buffer_converted(p_buffer) )       
      { return(( UnicodeBuffer *)p_buffer); }

   // "p_buffer" points a unicode buffer.
   // The string of the pointed unicode buffer will be converted once.
   else
   {  
      text_encoding = Unicode_get_input_encoding(p_buffer);
      // File language used here to display properly the file content
      // being able to be written in extended ascii
      //lang = g_setting_list.language_file;
      lang = g_setting_list.struct_setting.language_ihm;
   }

   // Detect the encoding type with the help of the BOM if exists 
   // whatever the value returned in "g_text_encoding"
#if( UTF8_DECODING == ENABLED )
   if ( Is_UTF8_string(Unicode_get_addr_string(p_buffer)) )
      { text_encoding = ENCODING_UTF8; }
   else
#endif
   if ( Is_UTF16BE_string(Unicode_get_addr_string(p_buffer)) )
      { text_encoding = ENCODING_UTF16BE; }
   else if ( Is_UTF16LE_string(Unicode_get_addr_string(p_buffer)) )
      { text_encoding = ENCODING_UTF16LE; }

   switch( text_encoding )
   {
      case ENCODING_UTF16   :
         text_encoding = ENCODING_UTF16BE;    // unicode endianess unknown: by default big-endian
      case ENCODING_UTF16LE :
      case ENCODING_UTF16BE :
         SDKLoadCode( UNICODE_TEMP_CODE00 );
         utf16_to_unicode(( UnicodeBuffer *)p_buffer, text_encoding);
         break;
#if( UTF8_DECODING == ENABLED )
      case ENCODING_UTF8    :
         utf8_to_unicode(( UnicodeBuffer *)p_buffer);
         break;
#endif
      case ENCODING_ASCII   :
      default:
         SDKLoadCode( UNICODE_TEMP_CODE01 );
         ascii_to_unicode(( UnicodeBuffer *)p_buffer, lang);
         break;
   }
   Unicode_set_unicode_buffer_converted(p_buffer);
   Unicode_set_input_encoding(p_buffer, text_encoding);  // Default setting for the next string
   return(( UnicodeBuffer *)p_buffer);
}


#pragma CODE = UNICODE_TEMP_CODE00

//! @brief utf16_to_unicode
//!
//! Decode a UTF-16 string in a unicode buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  UnicodeBuffer *s : the input utf-16 string to decode 
//!                            the ouput string, result of the decoding in big-endian unicode
//! @return none

/* Here we accept FFFE/FEFF marks as endianness indicators (or Byte Order Marker) everywhere
   in the stream, not just at the beginning. (This is contrary to what
   RFC 2781 section 3.2 specifies, but it allows concatenation of byte
   sequences to work flawlessly, while disagreeing with the RFC behaviour
   only for strings containing U+FEFF characters, which is quite rare.)
   The default is big-endian. */
/* The state is 0 if big-endian, 1 if little-endian. */

/* UTF-16 represents a character above hexadecimal FFFF as a surrogate pair of code values from the range D800-DFFF */
/* For example, the character at code point hexadecimal 10000 becomes the code value sequence D800 DC00, 
   and the character at hexadecimal 10FFFD, the upper limit of Unicode, becomes the code value sequence DBFF DFFD */
/* Unicode code point coded beyond 65535 is not supported here */

static void utf16_to_unicode( UnicodeBuffer *buffer, U8 text_encoding )
{
   Bool b_utf16be;
   U16 wc; // wide char (unicode char)
   U8 *s;
   U8 *d;
   U8 sz;

#define PTR16(p)  ((U16 _MEM_TYPE_SLOW_ *)(p))

   d = s = ( U8 _MEM_TYPE_SLOW_ *)Unicode_get_addr_string(buffer);
   sz = Unicode_get_size_buffer(buffer);

    // The buffer's size must be an even value
   if (1 == sz % 2)
   { sz--; }

   // Unicode NULL-termination set at the end of the buffer
   // to be sure to detect the end of the input string
   *PTR16(s+sz-2) = 0x0000;
   
   if( ENCODING_UTF16BE==text_encoding )      { b_utf16be = TRUE;  }
   else                                       { b_utf16be = FALSE; }

   // Discard unicode marker if exists
   if (Is_UTF16BE_string(s) || Is_UTF16LE_string(s))
      { s+=2; }

   do
   {       
      // Get the wide-char by taking into account the current endianness
      if (b_utf16be)    
      {
         MSB(wc) = *s;
         LSB(wc) = *(s+1);
      }
      else
      {
         LSB(wc) = *s;
         MSB(wc) = *(s+1);
      }

      if( MSB(wc)<0xd8 )
      {
         *PTR16(d) = wc;
      }
      else if( MSB(wc)<0xf9 )
      { 
         // Surrogate pair of code values from the range D800-DFFF for the first wide char.
         // The second one must have a value in the range : DC00 - E000.
         // -> code points in 21bits with range: from 10000 to 10FFFD
         // As, these code points are used for minority languages, they are not supported.
         // Moreover, the range E000-F8FF are unused zone
         // So, all these code points are substituted by the "undefined" one.
         *PTR16(s) = UNDEFINED_UNICODE_CHAR;
      }
      else if( 0xfeff==wc )   { b_utf16be = TRUE;  }
      else if( 0xfffe==wc )   { b_utf16be = FALSE; }
      else                    { *PTR16(d) = wc;    }

      //PTR16(s)++;
      //PTR16(d)++;
      s++; s++;
      d++; d++;
   }
   while( 0x0000 != wc );

#undef PTR16
}
#endif  //#if( LCD_PRESENT == ENABLED )

#if( UTF8_DECODING == ENABLED )
//! @brief utf8_to_unicode
//!
//! Decode a UTF-8 string in a unicode buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  UnicodeBuffer *s : the input utf-16 string to decode 
//!                            the ouput string, result of the decoding in big-endian unicode
//! @return none

// Binary format of bytes in 3 first sequences:
//    1st byte    2nd byte    3rd byte    Max unicode value
//    0xxxxxxx                            0x00007F     (127)
//    110xxxxx    10xxxxxx                0x0007FF    (2047)
//    1110xxxx    10xxxxxx    10xxxxxx    0x00FFFF   (65535) 

// The value of each individual byte indicates its UTF-8 functions, as follows:
//    0x00 to 0x7F   : first and only byte of a sequence
//    0x80 to 0xBF   : continuing byte in a multi-byte sequence
//    0xC0 to 0xDF   : first byte of a two-byte sequence
//    0xE0 to 0xEF   : first byte of a three-byte sequence
//    0xF0 to 0xF7   : first byte of a four-byte sequence
//    0xF8 to 0xFB   : first byte of a five-byte sequence
//    0xFC to 0xFD   : first byte of a six-byte sequence
//   -> 0xFE and FF not coded in the input string
// The function supports UTF8 sequences up to the three-byte.
// Unicode code point coded beyond 65535 is be not supported
// Specification: RFC 2279
static void utf8_to_unicode( UnicodeBuffer *buffer )
{
   Bool b_BOM_present;
   U8  i,j;
   U8  c0, c1, c2;
   U16 wc; // wide char (unicode char)
   U8 *s;
   U8 sz;

   // Number of bytes available to store a string in this buffer
   sz = Unicode_get_size_buffer(buffer);

   s  = Unicode_get_addr_string(buffer);
   // Add ascii NULL-termination in case there is none
   s[sz-1] = 0x00;
   
   // Determinate the number of ASCII characters (result in "i")
   // and the index of the last byte of the input string (result in "j")
   i=0;
   for( j=0;; j++)
   {
      if( 0x00 == s[j] ) { break; }
      if( s[j]<0x80 )    { i++;   }
   }

   // Test if the buffer is big enough to store the output string
   //    Input code        Input code size   Output code size
   //    ASCII (0->127)    1 byte            2 bytes
   //    (128 ->2047)      2 bytes           2 bytes
   //    (2048->65535)     3 bytes           2 bytes
   // 
   // So, the buffer can be not big enough because of ASCII characters.
   // To have an efficient processing with few ressources,
   // the idea is to shift the string towards the end of the buffer.
   // The shifting value is the number of ASCII characters.
   // Advantage : after shifting, it's easy to replace the ASCII character with its code point.
   // Drawbacks : - the buffer is not fully used when 1-byte and 3-byte characters are present simultaneously.
   //             - the input string can be truncated to make it possible the conversion.

   // Input string begins with the utf-8 BOM?
   if( Is_UTF8_string(s) )
   {
      b_BOM_present = TRUE;
      j -= 3;  // Length of the input string (nb of bytes) without the BOM
   }
   else
      { b_BOM_present = FALSE; }


   if( 0 != i )
   {
      // Shifting of the input string to the right direction is required to make the decoding possible 
      //  Test is the input string is to truncate
      if( (j+i) >= sz )
      {
         // Find the start byte of the codechar
         while( (s[sz-i]>=0x80)&&(s[sz-i]<=0xbf) )    { sz--; }
         // The truncation is done with a ascii null terminaison
         s[sz-i] = 0x00;     
      }

      // Shifting of the input string
      i += j;
      while( 1 )
      {
         if( b_BOM_present )  { s[i] = s[j+3]; }
         else                 { s[i] = s[j];   }
         if( 0 == j )         { break; }
         i--; j--;
      }
   }
   
   // Conversion UTF-8 to unicode
   j = 0;
   do
   {       
      c0 = s[i];
      c1 = s[i+1];
      c2 = s[i+2];
      if( 0x00 == (c0 & 0x80) )
      {
         wc = c0;
         i++;
      }
      else if( 0xC0 == (c0 & 0xE0) )
      {
         wc = ((U16)(c0 & 0x1F)<<6) | ((U16)(c1 & 0x3F));
         i += 2;
      }
      else if( 0xE0 == (c0 & 0xF0) )
      {
         wc = ((U16)(c0 & 0x0F)<<(6+6)) | ((U16)(c1 & 0x3F)<<(6)) | ((U16)(c2 & 0x3F));
         i += 3;
      }
      s[j+0] = MSB( wc );   // Output string stored in big-endian
      s[j+1] = LSB( wc );
      j += 2;
   }
   while( 0x0000 != wc );
}
#endif

#if( LCD_PRESENT == ENABLED )

#pragma CODE = UNICODE_TEMP_CODE01

//! @brief ascii_to_unicode
//!
//! Convert ascii into unicode the string of the unicode buffer 
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  UnicodeBuffer *s : the input ascii string to convert 
//!                            the ouput string, result of the decoding in big-endian unicode
//! @param  U8 lang          : the language in which the ascii text has been written
//! @return none
static void ascii_to_unicode(UnicodeBuffer *buffer, U8 lang)
{
   Bool b_doublebyte_input;
   _MEM_TYPE_SLOW_ U16 charset_start;
   _MEM_TYPE_SLOW_ U8 sz;
   U8 *s;
   U8 *d;

   union
   {
      struct
      {
         U8 i;
         U8 j;
      }
      idx;
      U16 addr;
   }
   _MEM_TYPE_MEDFAST_ v0;

#define I         v0.idx.i
#define J         v0.idx.j
#define ADDR      v0.addr
#define PTR16(p)  ((U16 _MEM_TYPE_SLOW_ *)(p))


   // sz : number of bytes available to store a string in this buffer minus the Unicode NULL-termination
   sz = Unicode_get_size_buffer(buffer)-2;
   s  = Unicode_get_addr_string(buffer);

   switch( lang ) 
   {
      case LANG_CHINESE_SIMP:
         SDKLoadCode( IMG_HANDLER_TEMP_CODE00 );
         disp_sys_init_load(FONT_TBL_GB2312, ZONE_FONT);
         charset_start      = 0xa1a1;
         b_doublebyte_input = TRUE;
         break;
      case LANG_CHINESE_TRAD:
         SDKLoadCode( IMG_HANDLER_TEMP_CODE00 );
         //disp_sys_init_load(FONT_TBL_B5, ZONE_PICTURE);
         disp_sys_init_load( FONT_TBL_B5, ZONE_FONT );
         charset_start      = 0xa140;
         b_doublebyte_input = TRUE;
         break;
      default:
         b_doublebyte_input = FALSE;
   }

   // Truncate the input string for the unicode conversion
   // "i" is the number of bytes required for the output string in unicode
   for(I=0,J=0;I<sz;J++)
   {
      if( 0x00 == s[J] )   { break; }
      I+=2;
      if( (b_doublebyte_input) && (s[J]>=MSB(charset_start)) )  { J++; }
   }
   s[J] = 0x00;   // Ascii NULL-termination to truncate the input string


   if( I != J )
   {
      // Shifting of the input string to the right direction is required to make the decoding possible 
      while( 1 )
      {
         s[I] = s[J];
         if( 0 == J )   { break; }
         J--; I--;
      }
   }  
   else 
   { I=0; }    

   // "i" is the index on the input ascii string characters (keep its value computed just above)
   // "j" is the index on the output unicode string characters
   d = s;
   s = s+I;
   do
   {    
      if(b_doublebyte_input)
      {
         if( *s >= MSB(charset_start) )
         {
            // ASCII double byte into unicode
            //ADDR = (*PTR16(s)++) - charset_start;
            //ADDR = (*PTR16(s)) - charset_start;
            I = s[ 0 ]; J = s[ 1 ];
            ADDR -= charset_start;
            s++; s++;
            nfloader_compute_physic_addr((U32)ADDR*2);
            //nfloader_open_page_for_reading_via_register();
            *d++ = nfloader_get_data_and_fetch_next_one();
            *d++ = nfloader_get_data_and_fetch_next_one();
            continue;
         }
      }

      // ASCII single byte into unicode
      //*PTR16(d)++ = (U16)*s++;
      *PTR16(d) = (U16)*s++;
      d++; d++;
   }
   while( *s!=0x00 );

   // Unicode NULL-termination
   *PTR16(d) = 0;

   //if( b_doublebyte_input )   { disp_end_load(); }

#undef PTR16
}
#if 0
//! @brief unicode_get_string_length
//!
//! Return the number of unicode characters of the unicode buffer string.
//! Only supported for non-encoded unicode.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  UnicodeBuffer  Unicode buffer to determinate the length of its string
//! @return U8             Number of characters (excepted the NULL terminaison)
U8 unicode_get_string_length( UnicodeBuffer *buffer )
{
   U8  nb_wc = 0; // number of unicode character

#define PTR16(p)  ((U16 _MEM_TYPE_SLOW_ *)(p))

   // Skip the the unicode buffer marker
   buffer += UNICODE_BUFFER_HEADER_SZ;

   // Detect the unicode NULL terminaison to end the count of characters
   while( 0x00 != *PTR16(buffer) )
   {
      PTR16(buffer)++;
      nb_wc++;
   }
   return(nb_wc);

#undef PTR16
}
#endif
#endif  //#if( LCD_PRESENT == ENABLED )

#if 0
//! @brief unicode_get_required_size
//!
//! Return the number of allowed unicode characters for mailing.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8*  Address of the string that can belong to a unicode buffer or not
//! @return U8   Number of allowed unicode characters (NULL terminaison included)
U8 unicode_get_required_size(U8 _MEM_TYPE_SLOW_ *buffer)
{
   if( Unicode_is_string_from_unicode_buffer((buffer-6)) ) 
      return(*(buffer-1));
   else
      return(buffer[0]);
}
#endif

#pragma CODE = SRV_PLAYER_TEMP_CODE02

//! @brief unicode_set_input_encoding
//!
//! Set the input string encoding if the type of the pointed buffer is a "unicode buffer"
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8* buffer   : Address of the string that can belong to a unicode buffer or not
//!         U8  encoding : Input string encoding. (If unknown, use UNKNOWN_ENCODING)
//! @return none
void unicode_set_input_encoding(U8 _MEM_TYPE_SLOW_ *buffer, U8 encoding)
{
   if( Unicode_is_string_from_unicode_buffer((buffer-6)) ) 
      { *(buffer-2)=encoding; }
}

#if 0
//! @brief unicode_append_ascii_string_to_buffer
//!
//! Append a non-extended ascii string in the unicode buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  UnicodeBuffer *buffer  : the unicode buffer to modify
//          U8 *ascii_string       : the string to add to the queue of the unicode string
//! @return none
void unicode_append_ascii_string_to_buffer(UnicodeBuffer* buffer, U8* ascii_string)
{
   U8 nb_char;

#define PTR16(p)  ((U16 _MEM_TYPE_SLOW_ *)(p))

   // Number of unicode characters that can be stored the destination buffer
   // minus the NULL termination
   nb_char = Unicode_get_char_space(buffer) -1;

   // Skip the unicode buffer marker
   buffer = Unicode_get_addr_string(buffer);

   // Unicode NULL-termination set at the end of the buffer
   // to be sure to detect the end of the string
   PTR16(buffer)[nb_char] = 0x0000;

   // Seek the NULL terminaison to determinate the number of characters that can be appended
   while( *PTR16(buffer)!=0 )
   {
      //PTR16(buffer)++;
      buffer++; buffer++;
      nb_char--;
   }

   // Copy the string
   while( 0 != nb_char-- )
   {
      //*PTR16(buffer)++ = (U16)*ascii_string++;
      *PTR16(buffer) = (U16)*ascii_string++;
      buffer++; buffer++;
   }
   // Add default NULL termination
   *PTR16(buffer) = 0x0000;

#undef PTR16   
}
#endif

#if( LCD_PRESENT == ENABLED )

#pragma CODE = UNICODE_TEMP_CODE02

//! @brief unicode_append_unicode_strings
//!
//! Append the source unicode string in the destination unicode buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 *src_buffer             : Unicode string to append in the destination buffer
//                                       Points a buffer declared in rom or in ram
//!         UnicodeBuffer *dst_buffer  : Unicode buffer string to store the appending of the two strings
//! @return none
void unicode_append_unicode_strings(U8 *src_buffer, UnicodeBuffer *dst_buffer)
{
   _MEM_TYPE_SLOW_ U8 nb_char;

#define PTR16S(p)    ((U16 _MEM_TYPE_SLOW_ *)(p))
#define PTR16(p)     ((U16 *)(p))

   // Number of unicode characters that can be stored the destination buffer
   // minus the NULL termination
   nb_char = Unicode_get_char_space(dst_buffer) -1;

   // Skip the the unicode buffer marker
   dst_buffer = Unicode_get_addr_string(dst_buffer);

   // Unicode NULL-termination set at the end of the buffer
   // to be sure to detect the end of the string
   PTR16S(dst_buffer)[nb_char] = 0x0000;

   // Seek the NULL terminaison to determinate the number of characters that can be appended
   while( *PTR16S(dst_buffer)!=0 )
   {
      //PTR16S(dst_buffer)++;
      dst_buffer++; dst_buffer++;
      nb_char--;
   }

   // Skip the the unicode buffer markers
   if( Unicode_is_string_from_unicode_buffer(src_buffer) )
      { src_buffer = Unicode_get_addr_string(src_buffer); }

   // Copy the string
   while( 0 != nb_char-- )
   {
      //*PTR16S(dst_buffer)++ = *PTR16(src_buffer)++;
      *PTR16S(dst_buffer) = *PTR16(src_buffer);
      dst_buffer++; dst_buffer++;
      src_buffer++; src_buffer++;
   }
   // Add default NULL termination
   *PTR16S(dst_buffer) = 0x0000;

#undef PTR16S
#undef PTR16
}
#endif  //#if( LCD_PRESENT == ENABLED )

#if 0
//! @brief unicode_copy_unicode_strings
//!
//! Copy the source unicode string in the destination unicode buffer
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 *src_buffer             : Unicode string to append in the destination buffer
//                                       Input buffer type can be Unicode buffer or not
//!         UnicodeBuffer *dst_buffer  : Unicode buffer string to store the appending of the two strings
//! @return none
void unicode_copy_unicode_strings(U8 _MEM_TYPE_SLOW_ *src_buffer, UnicodeBuffer *dst_buffer)
{
   _MEM_TYPE_SLOW_ U8 nb_char;

#define PTR16(p)  ((U16 _MEM_TYPE_SLOW_ *)(p))

   // Number of unicode characters that can be stored the destination buffer
   // minus the NULL termination
   nb_char = Unicode_get_char_space(dst_buffer) -1;

   // Skip the the unicode buffer markers
   if( Unicode_is_string_from_unicode_buffer(src_buffer) )
   {
      Unicode_set_input_encoding(dst_buffer, Unicode_get_input_encoding(src_buffer));
      src_buffer = Unicode_get_addr_string(src_buffer);
   }

   // Skip the the unicode buffer marker
   dst_buffer = Unicode_get_addr_string(dst_buffer);

   // Copy the string
   while( 0 != nb_char-- )
   {
      *PTR16(dst_buffer)++ = *PTR16(src_buffer)++;
   }
   // Add default NULL termination
   *PTR16(dst_buffer) = 0x0000;

#undef PTR16
}
#endif

#endif   // _unicode_c_

