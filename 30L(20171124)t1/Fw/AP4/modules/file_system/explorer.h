//! @file explorer.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the explorer interface.
//!
//! @version 1.3 snd3-dvk-1_9_5 $Id: explorer.h,v 1.3 2007/08/30 09:24:35 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _EXPLORER_H_
#define _EXPLORER_H_

#include "fs_com.h"
#include "fat.h"
#include "navigation.h"

//! Structure to store the loop mode option
typedef  U8    Exp_mode;

//! \name Definition of the loop modes
//! @{
#define  EXP_MODE_DISKS          0
#define  EXP_MODE_DISK           1
#define  EXP_MODE_DIRONLY        2
#define  EXP_MODE_DIRSUB         3
#define  EXP_MODE_PLAYLIST       4
#define  EXP_MODE_NB             5
//! @}

//! Structure to store the random mode option
typedef  U8    Exp_rand;
//! \name Definition of the random modes
//! @{
#define  EXP_RAND_OFF            0
#define  EXP_RAND_ON             1
#define  EXP_RAND_NB             2
//! @}

//! Type definition of status from fonction exp_mov()
typedef  U8    Exp_mov_state;
//! \name Status list of fonction exp_mov()
//! @{
#define  EXP_MOV_OK              0
#define  EXP_MOV_OK_LOOP         1
#define  EXP_MOV_EMPTY           2
#define  EXP_MOV_DISKNOPRESENT   3
#define  EXP_MOV_DISKERROR       4
//! @}


Exp_mode    exp_getmode        ( void );
Bool        exp_setmode        ( Exp_mode exp_mode );
Exp_rand    exp_getrand        ( void );
void        exp_setrand        ( Exp_rand rand );
void        exp_init           ( const FS_STRING sz_filterext );
//Bool        exp_open           ( Exp_mode mode , U16 pos );
U16         exp_close          ( void );
U16         exp_getnb          ( void );
U16         exp_getpos         ( void );
//Exp_mov_state exp_mov          ( Bool b_direction );

#endif  // _EXPLORER_H_
