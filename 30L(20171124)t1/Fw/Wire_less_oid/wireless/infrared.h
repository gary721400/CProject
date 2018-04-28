//! @file e2radio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: e2radio.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _INFRARED_H_
#define _INFRARED_H_

//_____ I N C L U D E S ____________________________________________________


#include "config.h"
#include <ISD12_DDK.H>
#include "GPIO_Bitref.H"

//_____ M A C R O S ________________________________________________________



#define RED_CLK_DIR   (_PCDIR_b0+5)
#define RED_CLK_PH    (_PCPH_b0+5)
#define RED_CLK_OD    (_PCOD_b0+5)

#define RED_DATA_DIR   (_PCDIR_b0+6)
#define RED_DATA_PH    (_PCPH_b0+6)
#define RED_DATA_OD    (_PCOD_b0+6)




#define Set_red_clk()          ( _bitSET(RED_CLK_OD))
#define Clr_red_clk()          ( _bitCLR(RED_CLK_OD))
#define Flash_red_clk()		   (_bitXOR(RED_CLK_OD,1))

#define Set_red_data()          ( _bitSET(RED_DATA_OD))
#define Clr_red_data()          ( _bitCLR(RED_DATA_OD))






//_____ D E F I N I T I O N ________________________________________________



//_____ D E C L A R A T I O N ______________________________________________




void init_infrared(void);
//void IRed_Send_data(U8 address,U8 command);
void Infrared_Send_data(U8 address, U8 command);
void test_38K(void);

#endif  // _INFRARED_H_

