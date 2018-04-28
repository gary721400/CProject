//!
//! @file mmi_ap3.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI ap3
//!
//! @version 1.1 snd3b-dvk-1_0_33 $Id: mmi_ap3.h,v 1.1 2005/10/03 11:13:20 coger Exp $
//!
//! @todo
//! @bug
//!

#ifndef	_AP4NEWS_H_
#define _AP4NEWS_H_

#include "KBDM9_DDK.H"
//_____ M A C R O S ________________________________________________________



//_____ D E C L A R A T I O N S ____________________________________________

//_____ M A C R O S ________________________________________________________

//_____ D E C L A R A T I O N S ____________________________________________


typedef enum
{
   AP4_PLAY_SEQUENCE            //0
,  AP4_PLAY_NOT_FIND_FILE       //1
,  AP4_PLAY_SIMPLE              //2
,  AP4_PLAY_ONLY_SIMPLE         //3
,  AP4_PLAY_NORMAL              //4
,  AP4_PLAY_ONLY_SEQUENCE       //5
,  AP4_PLAY_SD_REMOVE           //6
,  AP4_PLAY_NO_AP4              //7
,  AP4_PLAY_STARTUP             //8
,  AP4_PLAY_REREAD_STATUS       //9
,  AP4_PLAY_REREAD              //10
,  AP4_PLAY_ERROR               //11
,  AP4_PLAY_BROWSER_PAGE        //12
}Ap4_Play;



#endif  //! _AP4NEWS_H_

