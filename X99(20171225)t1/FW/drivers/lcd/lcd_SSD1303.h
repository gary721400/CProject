//!
//! @file lcd_SSD1303.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file gives definitions of the SSD1303 lcd controller driver functions used in MMI applications.
//!      Code only implemented if "LCD_CONTROLLER_PART_NUMBER" has been defined with the label "LCD_CONTROLLER_SSD1303".
//!        This definition is to do in the file "conf_lcd.h".
//!        Note that the SSD1303 is a monochrom lcd with horizontal pages
//!
//!
//! @version 1.16 snd3b-dvk-1_9_3 $Id: lcd_SSD1303.h,v 1.16 2007/07/26 15:01:04 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _LCD_SSD1303_H_
#define _LCD_SSD1303_H_


//_____ I N C L U D E S ____________________________________________________

#include "conf\conf_lcd.h"
#if ( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303 )

#include "lib_mcu\lcd\lcd_drv.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _lcd_SSD1303_c_)
#  define _GLOBEXT_ extern
#else                                        
#  define _GLOBEXT_ extern
#endif


//_____ D E F I N E S ______________________________________________________
#define  LCD_ON_TIME       TIMER_MS(30)
#define  LCD_OFF_TIME      TIMER_MS(500)

#define LCD_nON_DIR              ( _PBDIR_b0 + 12 )
#define LCD_nON                  ( _PBOD_b0 + 12 )
//! Power On
#define Lcd_power_switch_on()    ( _bitSET( LCD_nON ))

//! Power Off
#define Lcd_power_switch_off()   ( _bitCLR( LCD_nON ))

//_____ LCD FEATURES _______________________________________________________

//! For monochrom LCDs, the terms "lines" versus "colums" are not used hereafter in macros and functions.
//! Instead, terms "pages" versus "lines" are used to take into account the specific ram organization.
//! Here, with the S6B0719, "pages" are horizontal including 8 "lines" while "lines" corresponds to "columns"
#define NB_LCD_PAGES       ( LCD_HEIGHT / 8 )
#define NB_LCD_LINES       ( LCD_WIDTH )

//! Define active or not the lcd dummy read via the macro Lcd_dummy_read()
//! Values: - ENABLED:  Lcd_dummy_read() does a lcd dummy read 
//!         - DISABLED: Lcd_dummy_read() is an empty macro
#define LCD_DUMMY_READ     ( ENABLED )


//_____ DISPLAY BUFFERIZATION ______________________________________________

//! Implement or not a buffer to draw in it the screen before sending it to the LCD
//! Values: - ENABLED:  Implementation of the buffer and associated macros
//!         - DISABLED: No screen buffer
#define LCD_SCREEN_BUFFER   ( DISABLED )
//#define LCD_SCREEN_BUFFER   ( ENABLED )


//_____ COMMANDS ___________________________________________________________

#define LCD_DISPLAY_ON                       0xAF     //10101111
#define LCD_DISPLAY_OFF                      0xAE     //10101110
#define LCD_ENTIRE_DISPLAY_ON                0xA5     //10100101
#define LCD_ENTIRE_DISPLAY_OFF               0xA4     //10100100
#define LCD_DISPLAY_NORMAL                   0xA6     //10100110
#define LCD_DISPLAY_REVERSED                 0xA7     //10100111
#define LCD_ADC_NORMAL                       0xA0     //10100000
#define LCD_ADC_REVERSED                     0xA1     //10100001
#define LCD_COMMON_OUTPUT_NORMAL             0xC0     //11000000
#define LCD_COMMON_OUTPUT_REVERSED           0xC8     //11001000

#define LCD_CMD_PAGE_ADDR                    0xB0     //10110xxx
#define LCD_CMD_COL_HIGH_ADDR                0x10     //0001xxxx
#define LCD_CMD_COL_LOW_ADDR                 0x00     //0000xxxx
								   
#define LCD_CMD_DC_DC_SETUP                  0xAD     //10101101
#define LCD_VAL_DC_DC_ON                     0x8B     //1000101x
#define LCD_VAL_DC_DC_OFF                    0x8A     //1000101x

// NB : 105 common x 160 segment => 1/105 duty (set by hardware)
#define LCD_CMD_DUTY_RATIO                   0xA8     //10101000		2 bytes
#define LCD_VAL_DUTY_RATIO                   0x1F//

#define LCD_CMD_DISPLAY_OFFSET               0xD3     //11010011
#define LCD_VAL_DISPLAY_OFFSET               0x00//0x1F//    //00xxxxxx

#define LCD_CMD_FRAME_RATE                   0xD5     //11010101
#define LCD_VAL_FRAME_RATE                   0x51     //xxxxxxxx

#define LCD_CMD_SET_MODE                     0xD8     //11011000
#define LCD_VAL_MONO_MODE                    0x00     //00xx0x0x

//  => booster circuit on, voltage regulator on, voltage follower on
#define LCD_CMD_ELECTRONIC_VOLUME            0x81     //10000001
#define LCD_VAL_DEFAULT_ELEC_VOLUME          0x35     //xxxxxxxx

#define LCD_CMD_BIGHTNESS                    0x82     //10000010
#define LCD_VAL_BIGHTNESS                    0x80     //xxxxxxxx

#define LCD_CMD_PIN_CONFIG                   0xDA     //11011010
#define LCD_VAL_PIN_CONFIG                   0x12     //000x0010

#define LCD_CMD_SET_VCOM                     0xDB     //11011011
#define LCD_VAL_SET_VCOM                     0x35     //0xxxxxxx

#define LCD_CMD_SET_PRECHARGE                0xD9     //11011001
#define LCD_VAL_SET_PRECHARGE                0x22     //xxxxxxxx

#define LCD_CMD_SET_DIR                      0xC8     //1100x000

#define LCD_POSX0  LCD_CMD_COL_LOW_ADDR   //0x00
#define LCD_POSX1  LCD_CMD_COL_HIGH_ADDR  //0x10
#define LCD_POSY0  LCD_CMD_PAGE_ADDR      //0xB0



//_____ D E C L A R A T I O N S ____________________________________________

//! Global x and y variables for display
_GLOBEXT_ U8  _MEM_TYPE_SLOW_   lcd_g_lcd_x
#if 0//def _lcd_SSD1303_c_
   = 0
#endif
;
_GLOBEXT_ U8  _MEM_TYPE_SLOW_   lcd_g_lcd_y
#if 0//def _lcd_SSD1303_c_
   = 0
#endif
;

//! Global one-bit variable for the merging or not of superimposed displays
_GLOBEXT_ Bool b_merging
#if 0//def _lcd_SSD1303_c_
   = FALSE
#endif
;

//! Set in "lcd_puts()" if effective text scrolling done
//! and only read by MMI via the macro "Lcd_have_scrolling_been_effective()"
_GLOBEXT_ Bool b_scrolling_mode_effective;

//! Variable used in "lcd_puts()" to define the viewport
typedef struct
{
   U8  right_x;
}lcd_viewport;

_GLOBEXT_ _MEM_TYPE_SLOW_ lcd_viewport lcd_g_viewport
#if 0//def _lcd_SSD1303_c_
   = {LCD_WIDTH - 1}
#endif
;

//! Structure used by MMI to param the text scrolling
typedef struct
{
   U8  idx_start_char;
   S16 offset_x;
}st_txt_scrolling;

//! Pointer used in "lcd_puts()" for the text scrolling
_GLOBEXT_ st_txt_scrolling _MEM_TYPE_SLOW_* ptr_txt_scrolling_prm
#if 0//def _lcd_SSD1303_c_
   = NULL
#endif
;

//! Bit that indicates that the screen has been changed
//! Only declared if screen buffer authorized
#if( LCD_SCREEN_BUFFER == ENABLED )
   _GLOBEXT_ Bool b_new_screen_to_display
#  ifdef _lcd_SSD1303_c_
      = FALSE
#  endif
   ;
#endif

//! Timer to delay the backlight switching off
//! Only declared if backlight authorized
#if (BACKLIGHT == ENABLE)
   _GLOBEXT_ U32 _MEM_TYPE_SLOW_ g_timer_backlight
#  if 0//def _lcd_SSD1303_c_
   = 0x00
#  endif
   ;

//! Allow dynamic disable of backlight
   _GLOBEXT_ U8 _MEM_TYPE_SLOW_ g_backlight_en
#  if 0//def _lcd_SSD1303_c_
   = TRUE
#  endif
   ;
#endif

_GLOBEXT_ U8 timer_display_array[ 14 ];

//_____ M A C R O S ________________________________________________________


//_____ READ/WRITE MACROS __________________________________________________
#if 0
//! Write a command to the LCD controller
#define Lcd_write_command(cmd)                                    \
{                                                                 \
   /* Waiting display interface available to lauch a writing */   \
   Lcd_drv_test_lcbusy();                                         \
                                                                  \
   /* Select instruction register */                              \
   Lcd_drv_select_instr_reg();                                    \
                                                                  \ 
   /* Transmit command value */                                   \
   Lcd_drv_write_cmd(cmd);                                        \
                                                                  \
   /* Data register access by default */                          \
   /* ! MUST BE ALWAYS IMPLEMENTED HERE AS LAST INSTRUCTION ! */  \
   Lcd_drv_select_data_reg();                                     \
}
#endif
//! Write a display data in the LCD controller or in the screen buffer
#if( LCD_SCREEN_BUFFER == DISABLED )
   //! Write a data in the LCD controller display ram
//extern   Bool  lcd_g_inverse;
#if 0
#  define Lcd_write_display_data(val)                                \
   {                                                                 \
      SetStorageBusy();                                              \
      Lcd_drv_enable_interface();                                    \
      /* Waiting display interface available to lauch a writing */   \
      Lcd_drv_test_lcbusy();                                         \
                                                                     \
      /* Transmit display data */                                    \
      /*Lcd_drv_write_data(val);*/                                   \
      Lcd_drv_write_data(( lcd_g_inverse )?( ~( val )):( val ));     \
      Lcd_drv_test_lcbusy();                                         \
      Lcd_drv_disable_interface();                                   \
      ClearStorageBusy();                                            \
   }
#endif
void  Lcd_write_display_data  ( U8 val );
#else
   //! Write a data in the screen buffer
#  define Lcd_write_display_data(val)                                \
   {                                                                 \
      *(s_ptr_screen_buffer++) = (val);                              \
   }
#endif

//! Read a display data from the LCD controller or the screen buffer
#if( LCD_SCREEN_BUFFER == DISABLED )
   //! The read from the LCD controller display ram is done in two steps :
   //!   1) A read request is to sent to the controller via the macro
   //!      "Lcd_read_display_data()"
   //!   2) The read value is got with the help of a second macro
   //!      "display_data_read"
   //! These two macros avoids the call of a unique function that would be slower in execution.
#if 0
#  define Lcd_read_display_data()                                       \
{                                                                       \
   /* Waiting display interface available to lauch a writing */         \
   Lcd_drv_test_lcbusy();                                               \
                                                                        \
   /* Set the read command */                                           \
   Lcd_drv_set_read_command();                                          \
                                                                        \
   /* Waiting display interface available to get a correct reading  */  \
   Lcd_drv_test_lcbusy();                                               \
                                                                        \
   /* Get the display data by adressing Lcd_drv_display_data_read() */  \
   /* just after the implementation of this macro                   */  \
}
#endif
#  define Lcd_read_display_data()
#  define display_data_read             0//Lcd_drv_read_data()
#else
   //! The read from the screen buffer is also done in the two steps 
   //! defined above in order to keep a code compatibility
   //!   1) "Lcd_read_display_data()" is an empty macro
   //!   2) The read value is got with the help of a second macro
   //!      "display_data_read"
   //! These two macros avoids the call of a unique function that would be slower in execution.
#  define Lcd_read_display_data()
#  define display_data_read             *(s_ptr_screen_buffer)
#endif
//#endif
#define Lcd_dummy_read()      //Lcd_read_display_data()


//_____ SCREEN BUFFER MACROS _______________________________________________

#if( LCD_SCREEN_BUFFER == DISABLED )
   // Empty macros to keep the code compatibility
#  define Lcd_new_screen_to_display()
#  define Lcd_is_new_screen_to_display()
#  define Lcd_set_ptr_screen_buffer()
#  define Lcd_transfert_screen_to_lcd()
#else
#  define Lcd_new_screen_to_display()     (b_new_screen_to_display  = TRUE)
#  define Lcd_is_new_screen_to_display()  (b_new_screen_to_display == TRUE)
#  define Lcd_set_ptr_screen_buffer()                                           \
   {                                                                            \
     s_ptr_screen_buffer = &s_screen_buffer[lcd_g_page*LCD_WIDTH +lcd_g_line];  \
   }
#  define Lcd_transfert_screen_to_lcd()               \
   {                                                  \
      /* The screen display has been changed?    */   \
      if( Lcd_is_new_screen_to_display() )            \
      { /* Yes, so new screen is to write in LCD */   \
         lcd_display_screen();                        \
      }                                               \
   }
#endif


//_____ POSITIONNING MACROS ________________________________________________

#if 0
#define Lcd_set_cursor()                                                  \
{                                                                          \
   lcd_write_command( LCD_POSY0 | lcd_g_page );                            \
                                                                           \
   /* Write the 4 lower bits of the line value */                          \
   lcd_write_command( LCD_POSX0 | ( 0x0F &( lcd_g_line + 2 )) );           \
                                                                           \
	/* Write the 4 upper bits of the line value */                          \
   lcd_write_command( LCD_POSX1 | (( lcd_g_line + 2 )>> 4) );              \
																		   \
   Lcd_set_ptr_screen_buffer();                                            \
}

#define Lcd_goto_x( x0 )                                                   \
{                                                                          \
   /* Store the new X-coordinate in the dedicated global variable */       \
   lcd_g_lcd_x = (x0);                                                     \
                                                                           \
   /* Write the new LCD current line to the controller */                  \
   Lcd_set_line( lcd_g_lcd_x );                                            \
}

#define Lcd_goto_y( y0 )                                                   \
{                                                                          \
   /* Store the new Y-coordinate in the dedicated global variable */       \
   lcd_g_lcd_y = (y0);                                                     \
                                                                           \
   /* Write the new LCD current page to the controller */                  \
   Lcd_set_page( lcd_g_lcd_y /8 );                                         \
                                                                           \
   /* Get the shifting to apply to the display data */                     \
   lcd_g_shift_in_page = PosBin[ lcd_g_lcd_y %8 ];                         \
}
#else
void  Lcd_set_cursor ( void );
void  Lcd_goto_x     ( U8 x0 );
void  Lcd_goto_y     ( U8 y0 );
#endif

//! Get the current X-coordinate
#define Lcd_get_x()                (lcd_g_lcd_x)

//! Get the current Y-coordinate
#define Lcd_get_y()                (lcd_g_lcd_y)

//! Set the new current X-coordinate
#define Lcd_set_x( val )           (lcd_g_lcd_x = val)

//! Set the new current Y-coordinate
#define Lcd_set_y( val )           (lcd_g_lcd_y = val)

//! Get the current display page
#define Lcd_get_page()             (lcd_g_page)

//! Set the current display page
#define Lcd_set_page( page )       (lcd_g_page  = page)

//! Get the current shift in page
#define Lcd_get_shift_in_page()    (lcd_g_shift_in_page)

//! Get the current display line 
#define Lcd_get_line()             (lcd_g_line)

//! Set the current display line 
#define Lcd_set_line( line )       (lcd_g_line  = line)


//_____ GRAPHICAL MACROS ___________________________________________________
#if( LCD_PRESENT == ENABLED )
// OR operation on pixels when draw graphics at screen
#define Lcd_set_merging()                 ( b_merging = 1  )   
#define Lcd_unset_merging()               ( b_merging = 0  )
#define Lcd_is_merging()                  ( 1 == b_merging )
#define Lcd_save_status_merging()         ( b_merging_old = b_merging )
#define Lcd_restore_status_merging()      ( b_merging = b_merging_old )

// EXTERNAL macros used by MMI applications for text display
#define Lcd_set_txt_viewport_right( val )    ( lcd_g_viewport.right_x = (val) )
#define Lcd_have_scrolling_been_effective()  ( TRUE == b_scrolling_mode_effective )

// Macros to interface the structure "st_mmi_txt_scrolling" mainly in the MMI applications
// "var" to declare in MMI applications must be a _MEM_TYPE_SLOW_ type !!!
#define Lcd_init_txt_scrolling( var )     \
{                                         \
   var.idx_start_char = 0;                \
   var.offset_x = 0;                      \
}
#define Lcd_load_txt_scrolling( var )     \
{                                         \
   ptr_txt_scrolling_prm = &var;          \
}

#else
// OR operation on pixels when draw graphics at screen
#define Lcd_set_merging()
#define Lcd_unset_merging()
#define Lcd_is_merging()
#define Lcd_save_status_merging()
#define Lcd_restore_status_merging()

// EXTERNAL macros used by MMI applications for text display
#define Lcd_set_txt_viewport_right( val )
#define Lcd_have_scrolling_been_effective()

// Macros to interface the structure "st_mmi_txt_scrolling" mainly in the MMI applications
// "var" to declare in MMI applications must be a _MEM_TYPE_SLOW_ type !!!
#define Lcd_init_txt_scrolling( var )
#define Lcd_load_txt_scrolling( var )
#endif

//! Init display options to their default values 
//! just before executing another MMI application
#define Lcd_init_display_options()  \
{                                   \
   Lcd_unset_merging();             \
}

//! Display off
#define Lcd_display_off()                             \
{                                                     \
   Lcd_write_command( LCD_DISPLAY_OFF );              \
/* Lcd_write_command( LCD_DISPLAY_ALL_DOTS_ON);*/     \
}

//! Display on
#define Lcd_display_on()                              \
{                                                     \
   Lcd_write_command( LCD_DISPLAY_ON );               \
}

#if( LCD_PRESENT == ENABLED )
#define LCD_nRST_DIR       ( _PCDIR_b0 + 9 )
#define LCD_nRST           ( _PCOD_b0 + 9 )
// Reset On
#define Lcd_reset_off()     ( _bitCLR( LCD_nRST ))//( _bitSET( LCD_nRST ))//
// Reset Off
#define Lcd_reset_on()    ( _bitSET( LCD_nRST ))//( _bitCLR( LCD_nRST ))//
#endif   //#if( LCD_PRESENT == ENABLED )

//_____ BACKLIGHT MACROS ___________________________________________________

//! Backlight switch on/off definitions
#if( BACKLIGHT_ON_PIN_LEVEL == LOW_LEVEL )
#  define Lcd_set_backlight_on()      //(BACKLIGHT_CTRL_PIN = 0)
#  define Lcd_set_backlight_off()     //(BACKLIGHT_CTRL_PIN = 1)
#else
#  define Lcd_set_backlight_on()      //(BACKLIGHT_CTRL_PIN = 1)
#  define Lcd_set_backlight_off()     //(BACKLIGHT_CTRL_PIN = 0)
#endif
   
#define Lcd_start_backlight_timer()          (g_timer_backlight = Timer_set_timer(TIMER_MIN(g_setting_list.struct_setting.backlight_timeout)))
#define Lcd_stop_backlight_timer()           (g_timer_backlight = 0)
#define Lcd_is_backlight_timout()            (g_timer_backlight && Timer_is_timeout(g_timer_backlight))
//#define Lcd_is_backlight_timer()             (g_timer_backlight != 0)


#if( BACKLIGHT==DISABLE )
#  define Lcd_rearm_backlight_on()
#  define Lcd_shut_backlight_off()
#  define Lcd_backlight_enable()
#  define Lcd_backlight_disable()
#else
#  define Lcd_backlight_is_enable()    ((g_backlight_en)&&(g_setting_list.struct_setting.backlight_mode!=BACKLIGHT_MODE_OFF))
#if 0
#define Lcd_rearm_backlight_on()                \
{                                               \
   if( Lcd_backlight_is_enable() )              \
   {                                            \
   /* Set the backlight on */                   \
   Lcd_set_backlight_on();                      \
                                                \
   /* Armed the timer of the backlight */       \
   Lcd_start_backlight_timer();                 \
   }                                            \
}

#  define Lcd_shut_backlight_off()              \
{                                               \
   if( Lcd_backlight_is_enable() )              \
   {                                            \
   if( Lcd_is_backlight_timout() )              \
   {                                            \
      Lcd_stop_backlight_timer();               \
      Lcd_set_backlight_off();                  \
   }                                            \
   }                                            \
}
#else
#  define Lcd_rearm_backlight_on()     lcd_rearm_backlight_on()
#  define Lcd_shut_backlight_off()     lcd_shut_backlight_off()
#endif

#  define Lcd_backlight_enable()                (g_backlight_en=TRUE)
#  define Lcd_backlight_disable()               (g_backlight_en=FALSE)
#endif

#if (BACKLIGHT == ENABLE)
_GLOBEXT_   _MEM_TYPE_SLOW_ U8 g_mmi_lcd_backlight_off
#if 0//def _lcd_SSD1303_c_
   = FALSE
#endif
;

_GLOBEXT_   Bool  g_b_lcd_backlight_off /*= FALSE*/;
#endif

_GLOBEXT_   Bool b_flush_lcd;
_GLOBEXT_   Bool  lcd_g_inverse;
_GLOBEXT_	_MEM_TYPE_SLOW_ U16  lcd_display_num;

void  lcd_set_inverse   ( Bool v );
void  lcd_puts_inverse  ( U8 *str, Bool inverse );

//_____ D E C L A R A T I O N S ______________________________________________

#if( LCD_PRESENT == ENABLED )

_GLOBEXT_   void  lcd_write_command           (U8);
_GLOBEXT_   void  lcd_clear_screen            (void);
_GLOBEXT_   void  lcd_clear_part_of_screen    (U8, U8, U8, U8);
_GLOBEXT_   void  lcd_set_dot                 (U8, U8);
_GLOBEXT_   void  lcd_draw_picture            (U8, U8, U32);
_GLOBEXT_   void  lcd_puts                    (U8*);
//_GLOBEXT_   void  lcd_puts_ascii8             (U8*);

_GLOBEXT_   void  lcd_puts_code               (U8*);
_GLOBEXT_   void  lcd_clear_text_line_area    (U8);
_GLOBEXT_   void  lcd_init                    (void);
_GLOBEXT_   void  lcd_goto_x                  (U8);
_GLOBEXT_   void  lcd_goto_y                  (U8);
_GLOBEXT_   void  lcd_goto_xy                 (U8, U8);
_GLOBEXT_   void  lcd_display_screen          (void);
_GLOBEXT_	void  lcd_toggle_dot              (U8, U8);
_GLOBEXT_   void  lcd_turn_on                 (void);
_GLOBEXT_   void  lcd_turn_off                (void);

#if (BACKLIGHT == ENABLE)
_GLOBEXT_   void  lcd_rearm_backlight_on   (void);
_GLOBEXT_   void  lcd_shut_backlight_off   (void);
#endif


#else
#define Lcd_reset_on()
#define Lcd_reset_off()
#define lcd_draw_picture( x0, y0, IDP )
#define lcd_clear_part_of_screen( x0, y0, width, height )
#define lcd_goto_xy( x0, y0 )
#define lcd_clear_text_line_area( x_end )
#define lcd_puts( string )
//#define lcd_puts_ascii8( string )
#define lcd_clear_screen()
#define lcd_set_dot( x0, y0 )
#define lcd_turn_off()
#define lcd_turn_on()

#endif  //#if( LCD_PRESENT == ENABLED )

#endif   // #if ( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303 )

#endif   // _LCD_SSD1303_H_
