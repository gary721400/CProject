#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "ht1382.h"
#undef _GLOBEXT_
#if (defined _i2c_drv_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

#define SCL			4
#define SDL			13


#define SCL_GPIO()    (_bitCLR( _PBEN_b0 + SCL))   // I2C_SCL pin GPIO 
#define SDL_GPIO()    (_bitCLR( _PBEN_b0 + SDL))   // I2C_SDA pin GPIO 

#define SCL_INTERRUPT()    (_bitSET( _PBEN_b0 + SCL))   // I2C_SCL pin INTERRUPT 
#define SDL_INTERRUPT()    (_bitSET( _PBEN_b0 + SDL))   // I2C_SDA pin INTERRUPT
   
#define SCL_OUTPUT()   (_bitCLR( _PBDIR_b0 + SCL))  // I2C_SCL set output
#define SCL_INPUT()    (_bitSET( _PBDIR_b0 + SCL))  // I2C_SCL set input
#define SDL_OUTPUT()   (_bitCLR( _PBDIR_b0 + SDL))  // I2C_SDA set output
#define SDL_INPUT()    (_bitSET( _PBDIR_b0 + SDL))  // I2C_SDA set input

#define SCL_LOW()      (_bitCLR( _PBOD_b0 + SCL))  // I2C_SCL set low
#define SCL_HEIGH()    (_bitSET( _PBOD_b0 + SCL))  // I2C_SDA set heigh
#define SDL_LOW()      (_bitCLR( _PBOD_b0 + SDL))  // I2C_SDA set input
#define SDL_HEIGH()    (_bitSET( _PBOD_b0 + SDL))  // I2C_SDA set input
#define GET_SDL()	   (_bit_get( _PBID_b0 + SDL )) // get sdl state


///----define ht1382 register
#define ADDR_READ_BEGIN	 		0XD1
#define ADDR_WRITE_BEGIN		0XD0



#define  I2C_SPEED                  ( 3 << 1 )

#define  I2C_INIT_TIME              TIMER_MS( 5 )
#define  I2C_DATA_MAX_LEN           64//32//


_GLOBEXT_   S16   i2c_rf_count;
_GLOBEXT_   U8    i2c_data[ I2C_DATA_MAX_LEN ];
_GLOBEXT_   U8    i2c_tx_buf[ I2C_DATA_MAX_LEN ];
_GLOBEXT_   U8    i2c_rf_state;
_GLOBEXT_   U8    *i2c_data_point;

_GLOBEXT_   U16   i2c_data_packet_count;


void  i2c_init ( void );
void  i2c_task ( void );

#endif   //_I2C_DRV_H_
