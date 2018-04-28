//! @file unicode.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the timer driver definitions.
//!
//! @version 1.10 snd3-dvk-1_9_5 $Id: unicode.h,v 1.10 2007/06/25 09:31:52 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _UNICODE_H_
#define _UNICODE_H_



//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _unicode_c_)       
   #define _GLOBEXT_ extern
#else
   #define _GLOBEXT_ extern
#endif


//_____ D E F I N I T I O N S _______________________________________________

#define UTF8_DECODING      DISABLE//ENABLED
#define UnicodeBuffer      U8 //_MEM_TYPE_SLOW_
#define _UnicodeBuffer     U8  // a variant to have success in compilation when used in structure/union definition

#define UNDEFINED_UNICODE_CHAR  0x007F

// Text encoding information returned by the system
#define ENCODING_UNKNOWN   0x00
#define ENCODING_ASCII     0x01
#define ENCODING_UTF8      0x02
#define ENCODING_UTF16BE   0x04           // unicode endianess known: big-endian
#define ENCODING_UTF16LE   0x08           // unicode endianess known: little-endian
#define ENCODING_UTF16     (ENCODING_UTF16BE|ENCODING_UTF16LE) // unicode endianess unknown
#define ENCODING_USER1     0x10           // for other encoding
#define ENCODING_USER2     0x20
#define ENCODING_USER3     0x40
#define ENCODING_USER4     0x80


// Size of the unicode buffer dedicated in string conversion
#define SIZE_CONV_UNICODE_BUFFER              (40 *2)


//_____ D E C L A R A T I O N S ____________________________________________

// Temp strings stored in unicode
_GLOBEXT_ UnicodeBuffer g_conv_buffer[SIZE_CONV_UNICODE_BUFFER];


//_____ M A C R O S ________________________________________________________

//! The following macros are used to manipulate the format "UnicodeBuffer".
//! The specificity of this fomat is to introduce the big-endian Byte Order Marker
//! in each unicode string. This enables the distinguishing of the unicode string
//! and the ascii string by the lcd functions

//! Number of bytes used to define the header of unicode buffers
#define UNICODE_BUFFER_HEADER_SZ   6

//! Init the unicode buffer
#define Unicode_init_buffer(buffer,sz)                   \
{                                                        \
   /* Unicode buffer marker */                           \
   (buffer)[0] = 0xfe;                                   \
   (buffer)[1] = 0xff;                                   \
   (buffer)[2] = 0xff;   /* (string not converted) */    \
   /* Available room to place the unicode string */      \
   (buffer)[3] = sz-UNICODE_BUFFER_HEADER_SZ;            \
   /* Input string encoding */                           \
   (buffer)[4] = ENCODING_UNKNOWN;                       \
   /* Required size for mailing */                       \
   (buffer)[5] = 0x00;                                   \
   /* NULL terminaison */                                \
   (buffer)[6] = 0x00;                                   \
   (buffer)[7] = 0x00;                                   \
}

//! Prepare the unicode buffer for mailing
//! Set the max number of characters that the system can return through mailing
#define Unicode_prepare_buffer_for_mailing(buffer,nb_char)     \
{                                                              \
   if( (nb_char) > ((buffer)[3] /2) )                          \
      { (buffer)[5] = (buffer)[3] /2; }                        \
   else                                                        \
      { (buffer)[5] = nb_char; }                               \
  (buffer)[2] = 0xff;   /* (string not converted) */           \
}

//! Set the unicode-string buffer empty with the help of an unicode null termination
#define Unicode_set_buffer_empty(buffer)              {(buffer)[6]=0; (buffer)[7]=0;}

//! Test if the string is one from a unicode buffer
#define Unicode_is_string_from_unicode_buffer(buffer) ((0xfe==(buffer)[0])&&(0xff==(buffer)[1])&&(0xff==((buffer)[2]|0x01)))

//! Test if the string of a unicode buffer has been converted
#define Unicode_is_unicode_buffer_converted(buffer)   (0xfe == (buffer)[2])

//! Set the marker : string of a unicode buffer has been converted
#define Unicode_set_unicode_buffer_converted(buffer)  ((buffer)[2] = 0xfe)

//! Get the address of the unicode string in the buffer
#define Unicode_get_addr_string(buffer)               (&(buffer)[6])

//! Get the buffer space available to store characters (unit: nb of bytes)
#define Unicode_get_size_buffer(buffer)               ((buffer)[3])

//! Get the buffer space available to store characters (unit: nb of unicode character)
#define Unicode_get_char_space(buffer)                ((buffer)[3]/2)

//! Get the input string encoding
#define Unicode_get_input_encoding(buffer)            ((buffer)[4])

//! Set the input string encoding
#define Unicode_set_input_encoding(buffer,encoding)   {buffer[4]=encoding;}

//! Get char
#define Unicode_char(buffer,idx_char)                 ( ((U16 _MEM_TYPE_SLOW_*)(buffer))[idx_char*2+UNICODE_BUFFER_HEADER_SZ] )


//_____ P R I V A T E   M A C R O S ___________________________________________
//! Test if the string is encoded in UTF8
#define Is_UTF8_string(s)         ( (0xef==*(s)) && (0xbb==*(s+1)) && (0xbf==*(s+2)) )

//! Test if the string is encoded in UTF16-BE (Big Endian)
#define Is_UTF16BE_string(s)      ( (0xfe==*(s)) && (0xff==*(s+1)) )

//! Test if the string is encoded in UTF16-LE (Little Endian)
#define Is_UTF16LE_string(s)      ( (0xff==*(s)) && (0xfe==*(s+1)) )


//_____ D E C L A R A T I O N S _______________________________________________


//_____ D E F I N I T I O N S _________________________________________________

void           unicode_init_buffer_enc                ( U8 _MEM_TYPE_SLOW_ *buffer, U8 buffer_size , U8 encoding );
UnicodeBuffer* unicode_converter                      ( U8 *buffer );
U8             unicode_get_string_length              ( UnicodeBuffer* buffer );
U8             unicode_get_required_size              ( U8 _MEM_TYPE_SLOW_ *buffer);
void           unicode_set_input_encoding             ( U8 _MEM_TYPE_SLOW_ *buffer, U8 encoding);
void           unicode_append_ascii_string_to_buffer  ( U8 *buffer, U8 *ascii_string);
void           unicode_append_unicode_strings         ( U8 *src_buffer, UnicodeBuffer *dst_buffer);
void           unicode_copy_unicode_strings           ( U8 _MEM_TYPE_SLOW_ *src_buffer, UnicodeBuffer *dst_buffer);

#endif  // _UNICODE_H_

