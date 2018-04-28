#ifndef _OPAR_DRV_H_
#define _OPAR_DRV_H_

#undef _GLOBEXT_
#if (defined _opar_drv_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif


void  opar_init ( void );
void  opar_task ( void );

#endif   //_I2C_DRV_H_
