#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_

#undef _GLOBEXT_
#if (defined _i2c_drv_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

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
