//!
//! @file img_handler.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the graphical library
//!
//! @version 1.37 snd3-dvk-1_9_5 $Id: img_handler.h,v 1.37 2007/06/25 12:53:07 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _IMG_HANDLER_H_
#define _IMG_HANDLER_H_



//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_mmi.h"
#include "conf\conf_lcd.h"

#include "lib_mcu\mcu_drv.h"

#include "mmi\images\tab_pict.h"
#if (EMBEDDED_CODE_FONT == ENABLE)
#  include "mmi\images\tab_char.h"
#endif
#include "modules\control_access\ctrl_access.h"


//_____ M A C R O S __________________________________________________________

#undef _GLOBEXT_
#if (defined _img_handler_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


//_____ IDs OF AVAILABLE FONTS _______________________________________________

#define IDC_FONT_16     0
#define IDC_FONT_24     1
//#define IDC_FONT_10     2
//#define IDC_FONT_14     3
#define IDC_FONT_08     4
#define IDC_FONT_DIC_16 0


//    Module IDs of the picture zone
#define  PICTURE_DEFAULT   (0)

//    Module IDs of the font zone
#define  FONT_NORMAL       (0)
#define  FONT_TBL_GB2312   (20)
#define  FONT_TBL_B5       (21)


//_____ M A C R O S   A N D   D A T A ________________________________________

//_____ Z O N E   D E F I N I T I O N S ____________________________________

#define  NB_ZONES     6 
// ZONE_XX must be just befor ZONE_XX_COPY. 
// Keep ZONE_FIRMWARE & ZONE_FIRMWARE_COPY first!!!
typedef enum { ZONE_FIRMWARE, ZONE_FIRMWARE_COPY, ZONE_CODEC, ZONE_CODEC_COPY, ZONE_PICTURE, ZONE_FONT, ZONE_NO_ZONE} Zone;

//_____ HANDLERS OF PICTURES _________________________________________________

#define Ih_open_picture_access()          disp_sys_init_load(PICTURE_DEFAULT, ZONE_PICTURE)
#define Ih_select_picture(IDP)            nfloader_compute_physic_addr(IDP)//Disp_sys_set_position(IDP)
#define Ih_end_picture_access()
#define Ih_get_picture_data()             Disp_sys_get_data_and_fetch_next_one()


//_____ HANDLERS OF FONTS ____________________________________________________

_GLOBEXT_ _MEM_TYPE_SLOW_ U8              ih_g_curr_font;

_GLOBEXT_   _MEM_TYPE_SLOW_   U32   nor_flash_address;
_GLOBEXT_   _MEM_TYPE_SLOW_   U32   nor_flash_bass_address;

_GLOBEXT_                     Bool  g_b_animation
#if 0//def _rw_nor_c_
   = FALSE
#endif
;

_GLOBEXT_   __X   U32   Start_Sector, Length;
_GLOBEXT_   __X   U8    Buf[ 512 ];

//***********************************************
//2007-05-14 delete by xuanyu, because unused
//_GLOBEXT_ _MEM_TYPE_SLOW_ U32             ih_g_curr_font_addr;
//***********************************************
#define Ih_open_font_access()             disp_sys_init_load(ih_g_curr_font, ZONE_FONT)
#define Ih_get_font_data()                Disp_sys_get_data_and_fetch_next_one()
//#define Ih_add_font_offset(offset)        ih_add_font_offset( (offset) )
#define Ih_end_font_access()
#define Ih_set_curr_font(IDC)             ( ih_g_curr_font = IDC )

#define nfloader_get_data_and_fetch_next_one()  Disp_sys_get_data_and_fetch_next_one()

//_____ HANDLERS OF FONTS IN CODE ____________________________________________
#if( EMBEDDED_CODE_FONT == ENABLE )
#define HFONT  U8 code*
_GLOBEXT_ HFONT                           ih_g_ptr_code_font;
#define Ih_open_code_font_access()        ih_g_ptr_code_font = &TabChar[0]
#define Ih_get_code_font_data()           ( *(ih_g_ptr_code_font++)        )
#define Ih_add_code_font_offset(offset)   ( ih_g_ptr_code_font += (offset) )
#define Ih_end_code_font_access()
#endif   //#if (EMBEDDED_CODE_FONT == ENABLE)


//_____ D E C L A R A T I O N S ______________________________________________
_GLOBEXT_ void nfloader_compute_physic_addr  ( U32  ); 

_GLOBEXT_ U8   ih_get_font_height(          void );
_GLOBEXT_ void ih_ptr_char(                 U16 wc );
#if (EMBEDDED_CODE_FONT == ENABLE)
_GLOBEXT_ void ih_ptr_code_char(            U16 wc );
#endif
_GLOBEXT_ void ih_add_font_offset(          U8 offset );
#if 0
_GLOBEXT_ U8   ih_get_string_length_in_dot( U8* string );
#endif

U8    Disp_sys_get_data_and_fetch_next_one   ( void );

#endif   // _IMG_HANDLER_H_
