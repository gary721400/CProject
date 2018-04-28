//! @file int_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the interrupt controller driver definitions.
//!
//! @version 1.6 snd3-refd1-1_9_5 $Id: int_drv.h,v 1.6 2006/07/10 08:17:30 ppatron Exp $
//!
//! @todo
//! @bug

#ifndef _INT_DRV_H_
#define _INT_DRV_H_

//_____ I N C L U D E S ____________________________________________________


//_____ M A C R O S ________________________________________________________


#define Int_enable()         _bitSET( _SOCR_b0 + 15 ) // Enable SOCR.15
#define Int_disable()        _bitCLR( _SOCR_b0 + 15 ) // Disable SOCR.15

//#define Int_ex0_enable()     (EX0 = 1 ) //!< ! must be executed in Page 0 ! External Interrupt 0 enable interrupt.
//#define Int_ex0_disable()    (EX0 = 0 ) //!< ! must be executed in Page 0 ! External Interrupt 0 disable interrupt.
#define Int_t0_enable()       _bitSET( _TM0ICR_b0 + 0 )
#define Int_t0_disable()      _bitCLR( _TM0ICR_b0 + 0 )
//#define Int_ex1_enable()     (EX1 = 1 ) //!< ! must be executed in Page 0 ! External Interrupt 1 enable interrupt.
//#define Int_ex1_disable()    (EX1 = 0 ) //!< ! must be executed in Page 0 ! External Interrupt 1 disable interrupt.
#define Int_t1_enable()      _bitSET( _TM1ICR_b0 + 0 )
#define Int_t1_disable()     _bitCLR( _TM1ICR_b0 + 0 )
#define Int_t2_enable()      _bitSET( _TM2ICR_b0 + 0 )
#define Int_t2_disable()     _bitCLR( _TM2ICR_b0 + 0 )
#define Int_t3_enable()      _bitSET( _TM3ICR_b0 + 0 )
#define Int_t3_disable()     _bitCLR( _TM3ICR_b0 + 0 )
#define Int_t4_enable()      _bitSET( _TM4ICR_b0 + 0 )
#define Int_t4_disable()     _bitCLR( _TM4ICR_b0 + 0 )

//#define Int_sio_enable()     (ES = 1  ) //!< ! must be executed in Page 0 ! SIO enable interrupt.
//#define Int_sio_disable()    (ES = 0  ) //!< ! must be executed in Page 0 ! SIO disable interrupt.


//_____ D E C L A R A T I O N S ____________________________________________


#endif  // _INT_DRV_H_
