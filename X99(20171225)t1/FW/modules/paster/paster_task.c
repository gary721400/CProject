//!
//! @file paster_task.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the paster task.
//!
//! @version 1.60 snd3b-dvk-1_0_33 $Id: paster_task.c,v 1.60 2006/12/19 12:59:28 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _paster_task_c_
#define _paster_task_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "paster_task.h"
#include "paster.h"

#include "paster_base.h"
#include "modules\cue\cue.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "mmi\kbd\keypad.h"

#include "modules\file_aux\file_aux.h"
//#include "lib_system\utilities\string.h"

#include "modules\player\player.h"
#include "modules\player_ap3\player_ap3.h"

#include "conf\conf_sio.h"

#pragma DATA = PASTER_TASK_DATA
#pragma CODE = PASTER_TASK_CODE

#if( PASTER == ENABLE )

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________
Bool g_b_paster_on /*= FALSE*/;


#pragma CODE = TASK_INIT_CODE

//! Initialization of paster task
//!
void paster_task_init( void )
{
   g_b_paster_on = FALSE;
}


#pragma CODE = PASTER_TASK_CODE

//! Start paster task
//!
void paster_task_start(void)
{
   g_b_paster_on = TRUE;
}


//! Stop paster task
//!
void paster_task_stop(void)
{
   g_b_paster_on = FALSE;
}


//! This fonction manage the paster commands
//!
void paster_task( void )
{
   _MEM_TYPE_MALLOC_MAIL_  Msg   cmd;
 	extern   U8    _MEM_TYPE_SLOW_   g_paster_cmd_id;
	extern   U16	_MEM_TYPE_SLOW_	g_paster_cmd_param;

   if( g_b_paster_on )
   {
      if( MAILBOX_NO_MSG != mail_get_command( PASTER_TASK_ID, &cmd ))
      {
         g_paster_cmd_id = LSB( cmd.id );
         g_paster_cmd_param = cmd.param;
      }
      paster_evt();
   }
}

#endif  //#if( PASTER == ENABLE )

#endif  //_paster_task_c
