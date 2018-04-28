//! @file conf_lcd.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the LCD interface
//! All the default configuration for the external LCD controller will be put
//! here.
//! This file will be given to any external customer
//!
//! @version 1.32 snd3-dvk-1_9_5 $Id: conf_lcd.h,v 1.32 2007/07/24 14:03:43 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_LCD_H_
#define _CONF_LCD_H_

//_____ G E N E R I C   C O N F I G U R A T I O N __________________________

//! --- Is-there a LCD on the board ---
//! Values: - ENABLED  : There is a LCD on the board.
//!         - DISABLED : There is no LCD on the board.
#define LCD_PRESENT               ( ENABLED )
//#define LCD_PRESENT               ( DISABLED )

//! --- Embedded code font configuration ---
//! Allow local font usage in code
//! Values: - ENABLE
//!         - DISABLE
#define EMBEDDED_CODE_FONT       DISABLE


//! --- Width of the LCD screen in pixels ---
#define LCD_WIDTH                 128//256
//#define LCD_WIDTH                 160

//! --- Height of the LCD screen in pixels ---
//#define LCD_HEIGHT                64
#define LCD_HEIGHT                64//104//

//_____ T E X T   S C R O L L I N G   C O N T R O L ________________________

//! Implement or not the code to have the text scrolling available
//! Values: - ENABLE:  screen saver code have to be implemented
//!         - DISABLE: screen saver code don't have to be implemented
#define TEXT_SCROLLING           ENABLE


//_____ B A C K L I G H T   C O N T R O L __________________________________

//! Implement or not the routines to command the backlight
//! Values: - ENABLED:  backlight macros have to be implemented
//!         - DISABLED: backlight macros don't have to be implemented
#define BACKLIGHT                 ( ENABLE )
//#define BACKLIGHT                 ( DISABLE )

//! Output pin controlling the switching on/off of the lcd backlight
//#define BACKLIGHT_CTRL_PIN       P1_7

//! Level on pin to drive the backlight on
//! Values: - HIGH_LEVEL: backlight is on with high level on the output pin
//!         - LOW_LEVEL : backlight is on with low  level on the output pin
#define BACKLIGHT_ON_PIN_LEVEL   (LOW_LEVEL)


//_____ S E L E C T I O N   O F   T H E   L C D   C O N T R O L L E R ______
//! Define here one of the lcd controllers used in your current application

//! Label definitions of the lcd controllers being supported in this project
//! (Define here the label of your new lcd controller to support with a different value)
#define LCD_CONTROLLER_ST7565S   (0)
#define LCD_CONTROLLER_LH155BA   (1)
#define LCD_CONTROLLER_LGDP4216  (2)
#define LCD_CONTROLLER_S6B0719   (3)
#define LCD_CONTROLLER_SSD1303   (4)

//! Select the lcd controller to implement in your project code
//! (For this, use one of the label defined just above)
//# define LCD_CONTROLLER_PART_NUMBER         LCD_CONTROLLER_ST7565S
//# define LCD_CONTROLLER_PART_NUMBER         LCD_CONTROLLER_S6B0719
# define LCD_CONTROLLER_PART_NUMBER         LCD_CONTROLLER_SSD1303

//! Lcd controller files to include according to the choice made just above
#if (LCD_CONTROLLER_PART_NUMBER	== LCD_CONTROLLER_ST7565S)
#  define LCD_CONTROLLER_INCLUDE_NAME_FILE     "drivers\lcd\lcd_ST7565S.h"
#elif (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LH155BA)
#  define LCD_CONTROLLER_INCLUDE_NAME_FILE     "drivers\lcd\lcd_LH155BA.h"
#elif (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LGDP4216)
#  define LCD_CONTROLLER_INCLUDE_NAME_FILE     "drivers\lcd\lcd_LGDP4216.h"
#elif( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_S6B0719 )
# define LCD_CONTROLLER_INCLUDE_NAME_FILE       "drivers\lcd\lcd_S6B0719.h"
#elif( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303)
# define LCD_CONTROLLER_INCLUDE_NAME_FILE       "drivers\lcd\lcd_SSD1303.h"
#else
#  error include name file not defined for the current lcd controller
#endif


//_____ D I S P L A Y   I N T E R F A C E   C O N F I G U R A T I O N ______

//! Hereafter macros and HW config to drive properly LCD controllers
//! Input data  : timing and interface features form the LCD controller datasheets for VDD=2.7 to 5.5V
//! Output data : values computed and loaded in the 3 display interface registers
//!               LCDCON0, LCDCON1, LCDBUM by the function lcd_drv_basic_init_interface()


//!_____ DEFINITIONS TO CONFIGURE DISPLAY INTERFACE FOR THE LH155BA _______
#if (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LH155BA)


//! ****** Access cycles ******
//!
//! --- Interface type ---
//! Either following definition is to set active
#define LCD_INTERFACE_TYPE_8080
//#define LCD_INTERFACE_TYPE_6800

//! --- Cycle type ---
//! Either following definition is to set active
#define LCD_INTERFACE_CYCLE_NORMALIZED
//#define LCD_INTERFACE_CYCLE_NOT_NORMALIZED


//! ****** Cycle timings ******
//!
//! --- Address set-up/hold time ---
//!  Times to define in unit ns
#define LCD_INTERFACE_ADDR_SETUP_TIME   40
#define LCD_INTERFACE_ADDR_HOLD_TIME    60

//! --- Access width time ---
//!  Read pulse width (in unit ns)
#define LCD_INTERFACE_RD_PULSE_WIDTH    270
//!  Write pulse width (in unit ns)
#define LCD_INTERFACE_WR_PULSE_WIDTH    100

//! --- Full access cycle time ---
//! Define the sleep time (inaction time) in the access cycle time
//! Values: - TRUE:  Access cycle time fully used (sleep time automatically computed)
//!         - FALSE: Sleep time takes the same value than the one of access width
#define LCD_INTERFACE_FULL_ACCESS_CYCLE_TIME    TRUE

//! --- Cycle time ---
//!  Full access cycle time (in unit ns)
#define LCD_INTERFACE_CYCLE_TIME        450


//! ****** Automatic busy process ******
//!
//! --- Bits of the LCD controller status to check busy states ---
//! Values: 0x00        busy check disabled since no bit selected
//!         ...
//!         0xFF        all bits of the lcd controller status are to check
#define LCD_INTERFACE_BUSY_MASK         0

//! --- Busy active level ---
//! Define the active level of the busy bits
//! Values: - HIGH_LEVEL: Active level for the busy bits is high
//!         - LOW_LEVEL : Active level for the busy bits is low
#define LCD_INTERFACE_BUSY_ACTIVE_LEVEL    HIGH_LEVEL


//! ****** Read / write operations ******
//!
//! --- Level on A0/RS pin to select the data and instruction registers ---
//! Values: - HIGH_LEVEL: high level on pin A0/RS to select the instruction register
//!         - LOW_LEVEL : low  level on pin A0/RS to select the instruction register
#define LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL   HIGH_LEVEL

//! Implement or not the functions to display JPEG pictures
//! Values: - ENABLED
//!         - DISABLED
#define IMAGE_SUPPORT            (DISABLED)


#endif   // #ifdef (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LH155BA)



//!_____ DEFINITIONS TO CONFIGURE DISPLAY INTERFACE FOR THE SSD1303 _______
#if( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303 )

//! ****** Access cycles ******
//!
//! --- Interface type ---
//! Either following definition is to set active
#define LCD_INTERFACE_TYPE_8080
//#define LCD_INTERFACE_TYPE_6800

//! --- Cycle type ---
//! Either following definition is to set active
#define LCD_INTERFACE_CYCLE_NORMALIZED
//#define LCD_INTERFACE_CYCLE_NOT_NORMALIZED


//! ****** Cycle timings ******
//!
//! --- Address set-up/hold time ---
//!  Times to define in unit ns
#define LCD_INTERFACE_ADDR_SETUP_TIME   0
#define LCD_INTERFACE_ADDR_HOLD_TIME    0

//! --- Access width time ---
//!  Write pulse width (in unit ns)
#define LCD_INTERFACE_WR_PULSE_WIDTH                  60
//!  Read pulse width (in unit ns)
#define LCD_INTERFACE_RD_PULSE_WIDTH                  120

//! --- Full access cycle time ---
//! Define the sleep time (inaction time) in the access cycle time
//! Values: - TRUE:  Access cycle time fully used (sleep time automatically computed)
//!         - FALSE: Sleep time takes the same value than the one of access width
#define LCD_INTERFACE_FULL_ACCESS_CYCLE_TIME    TRUE


//! --- Cycle time ---
//!  Full access cycle time (in unit ns)
#define LCD_INTERFACE_CYCLE_TIME                      300


//! ****** Automatic busy process ******
//!
//! --- Bits of the LCD controller status to check busy states ---
//! Values: 0x00        busy check disabled since no bit selected
//!         ...
//!         0xFF        all bits of the lcd controller status are to check
#define LCD_INTERFACE_BUSY_MASK         0

//! --- Busy active level ---
//! Define the active level of the busy bits
//! Values: - HIGH_LEVEL: Active level for the busy bits is high
//!         - LOW_LEVEL : Active level for the busy bits is low
#define LCD_INTERFACE_BUSY_ACTIVE_LEVEL    HIGH_LEVEL


//! ****** Read / write operations ******
//!
//! --- Level on A0/RS pin to select the data and instruction registers ---
//! Values: - HIGH_LEVEL: high level on pin A0/RS to select the instruction register
//!         - LOW_LEVEL : low  level on pin A0/RS to select the instruction register
#define LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL  LOW_LEVEL   

//! Implement or not the functions to display JPEG pictures
//! Values: - ENABLED
//!         - DISABLED
#define IMAGE_SUPPORT            (DISABLED)

#endif   // #ifdef (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_SSD1303)


//!_____ DEFINITIONS TO CONFIGURE DISPLAY INTERFACE FOR THE S6B0719 _______
#if( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_S6B0719 )

//! ****** Access cycles ******
//!
//! --- Interface type ---
//! Either following definition is to set active
#define LCD_INTERFACE_TYPE_8080
//#define LCD_INTERFACE_TYPE_6800

//! --- Cycle type ---
//! Either following definition is to set active
#define LCD_INTERFACE_CYCLE_NORMALIZED
//#define LCD_INTERFACE_CYCLE_NOT_NORMALIZED


//! ****** Cycle timings ******
//!
//! --- Address set-up/hold time ---
//!  Times to define in unit ns
#define LCD_INTERFACE_ADDR_SETUP_TIME   0
#define LCD_INTERFACE_ADDR_HOLD_TIME    0

//! --- Access width time ---
//!  Write pulse width (in unit ns)
#define LCD_INTERFACE_WR_PULSE_WIDTH                  60
//!  Read pulse width (in unit ns)
#define LCD_INTERFACE_RD_PULSE_WIDTH                  120

//! --- Full access cycle time ---
//! Define the sleep time (inaction time) in the access cycle time
//! Values: - TRUE:  Access cycle time fully used (sleep time automatically computed)
//!         - FALSE: Sleep time takes the same value than the one of access width
#define LCD_INTERFACE_FULL_ACCESS_CYCLE_TIME    TRUE


//! --- Cycle time ---
//!  Full access cycle time (in unit ns)
#define LCD_INTERFACE_CYCLE_TIME                      300


//! ****** Automatic busy process ******
//!
//! --- Bits of the LCD controller status to check busy states ---
//! Values: 0x00        busy check disabled since no bit selected
//!         ...
//!         0xFF        all bits of the lcd controller status are to check
#define LCD_INTERFACE_BUSY_MASK         0

//! --- Busy active level ---
//! Define the active level of the busy bits
//! Values: - HIGH_LEVEL: Active level for the busy bits is high
//!         - LOW_LEVEL : Active level for the busy bits is low
#define LCD_INTERFACE_BUSY_ACTIVE_LEVEL    HIGH_LEVEL


//! ****** Read / write operations ******
//!
//! --- Level on A0/RS pin to select the data and instruction registers ---
//! Values: - HIGH_LEVEL: high level on pin A0/RS to select the instruction register
//!         - LOW_LEVEL : low  level on pin A0/RS to select the instruction register
#define LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL  LOW_LEVEL   

//! Implement or not the functions to display JPEG pictures
//! Values: - ENABLED
//!         - DISABLED
#define IMAGE_SUPPORT            (DISABLED)

#endif   // #ifdef (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_S6B0719)



//!_____ DEFINITIONS TO CONFIGURE DISPLAY INTERFACE FOR THE ST7565S _______
#if( LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_ST7565S )

//! ****** Access cycles ******
//!
//! --- Interface type ---
//! Either following definition is to set active
#define LCD_INTERFACE_TYPE_8080
//#define LCD_INTERFACE_TYPE_6800

//! --- Cycle type ---
//! Either following definition is to set active
#define LCD_INTERFACE_CYCLE_NORMALIZED
//#define LCD_INTERFACE_CYCLE_NOT_NORMALIZED


//! ****** Cycle timings ******
//!
//! --- Address set-up/hold time ---
//!  Times to define in unit ns
#define LCD_INTERFACE_ADDR_SETUP_TIME   0
#define LCD_INTERFACE_ADDR_HOLD_TIME    0

//! --- Access width time ---
//!  Write pulse width (in unit ns)
#define LCD_INTERFACE_WR_PULSE_WIDTH    80
//!  Read pulse width (in unit ns)
#define LCD_INTERFACE_RD_PULSE_WIDTH    125  // Higher value makes some text display wrong @ 24MHz(tested with one DVK only)

//! --- Full access cycle time ---
//! Define the sleep time (inaction time) in the access cycle time
//! Values: - TRUE:  Access cycle time fully used (sleep time automatically computed)
//!         - FALSE: Sleep time takes the same value than the one of access width
#define LCD_INTERFACE_FULL_ACCESS_CYCLE_TIME    TRUE


//! --- Cycle time ---
//!  Full access cycle time (in unit ns)
#define LCD_INTERFACE_CYCLE_TIME        240


//! ****** Automatic busy process ******
//!
//! --- Bits of the LCD controller status to check busy states ---
//! Values: 0x00        busy check disabled since no bit selected
//!         ...
//!         0xFF        all bits of the lcd controller status are to check
#define LCD_INTERFACE_BUSY_MASK         0

//! --- Busy active level ---
//! Define the active level of the busy bits
//! Values: - HIGH_LEVEL: Active level for the busy bits is high
//!         - LOW_LEVEL : Active level for the busy bits is low
#define LCD_INTERFACE_BUSY_ACTIVE_LEVEL    HIGH_LEVEL


//! ****** Read / write operations ******
//!
//! --- Level on A0/RS pin to select the data and instruction registers ---
//! Values: - HIGH_LEVEL: high level on pin A0/RS to select the instruction register
//!         - LOW_LEVEL : low  level on pin A0/RS to select the instruction register
#define LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL  LOW_LEVEL   

//! Implement or not the functions to display JPEG pictures
//! Values: - ENABLED
//!         - DISABLED
#define IMAGE_SUPPORT            (DISABLED)


#endif   // #ifdef (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_ST7565S)




//_____ L C D   C O N T R O L L E R   R E F  :  LGDP4216 ____________________
#if (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LGDP4216)


//!_____ DEFINITIONS TO CONFIGURE DISPLAY INTERFACE _________________________
//! Input data  : timing and interface features form the LCD controller datasheets for VDD=2.7 to 5.5V
//! Output data : values computed and loaded in the 3 display interface registers
//!               LCDCON0, LCDCON1, LCDBUM by the function lcd_drv_basic_init_interface()
//!
//! ****** Access cycles ******
//!
//! --- Interface type ---
//! Either following definition is to set active
#define LCD_INTERFACE_TYPE_8080

//! --- Cycle type ---
//! Either following definition is to set active
#define LCD_INTERFACE_CYCLE_NORMALIZED


//! ****** Cycle timings ******
//!
//! --- Address set-up/hold time ---
//!  Times to define in unit ns
#define LCD_INTERFACE_ADDR_SETUP_TIME   0
#define LCD_INTERFACE_ADDR_HOLD_TIME    0

//! --- Access width time ---
//!  Read pulse width (in unit ns)
#define LCD_INTERFACE_RD_PULSE_WIDTH    40
//!  Write pulse width (in unit ns)
#define LCD_INTERFACE_WR_PULSE_WIDTH    40

//! --- Full access cycle time ---
//! Define the sleep time (inaction time) in the access cycle time
//! Values: - TRUE:  Access cycle time fully used (sleep time automatically computed)
//!         - FALSE: Sleep time takes the same value than the one of access width
#define LCD_INTERFACE_FULL_ACCESS_CYCLE_TIME    TRUE

//! --- Cycle time ---
//!  Full access cycle time (in unit ns)
#define LCD_INTERFACE_CYCLE_TIME        100


//! ****** Automatic busy process ******
//!
//! --- Bits of the LCD controller status to check busy states ---
//! Values: 0x00        busy check disabled since no bit selected
//!         ...
//!         0xFF        all bits of the lcd controller status are to check
#define LCD_INTERFACE_BUSY_MASK         0

//! --- Busy active level ---
//! Define the active level of the busy bits
//! Values: - HIGH_LEVEL: Active level for the busy bits is high
//!         - LOW_LEVEL : Active level for the busy bits is low
#define LCD_INTERFACE_BUSY_ACTIVE_LEVEL    HIGH_LEVEL


//! ****** Read / write operations ******
//!
//! --- Level on A0/RS pin to select the data and instruction registers ---
//! Values: - HIGH_LEVEL: high level on pin A0/RS to select the instruction register
//!         - LOW_LEVEL : low  level on pin A0/RS to select the instruction register
#define LCD_INTERFACE_INSTRUCTION_REG_PIN_LEVEL  LOW_LEVEL   

//! Implement or not the functions to display user pictures JPEG/BMP
//! Values: - ENABLED
//!         - DISABLED
#define IMAGE_SUPPORT            (ENABLED)

#endif   // #ifdef (LCD_CONTROLLER_PART_NUMBER == LCD_CONTROLLER_LGDP4216)





#endif  //! _CONF_LCD_H_

