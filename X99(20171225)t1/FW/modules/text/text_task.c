#ifndef _text_task_c_
#define _text_task_c_

#include "config.h"

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\mailbox\mail_cmd.h"

#include "lib_mcu\timer\timer_drv.h"

#include "text_task.h"
#include "text_viewer.h"

#pragma DATA = DIC_TASK_DATA
#pragma CODE = DIC_TASK_CODE


ImageDeclareType( TEXT_VIEWER_KEY_CODE );


#if( DIC == ENABLE )

Bool g_b_dic_on /*= FALSE*/;

extern   U8    _MEM_TYPE_SLOW_   dictext_g_cmd_id;
extern   U16   _MEM_TYPE_SLOW_   dictext_g_param;


#pragma CODE = TASK_INIT_CODE

void dic_task_init( void )
{
   g_b_dic_on = FALSE;
   //dictext_evt_init();
}


#pragma CODE = MMI_DIC_TEMP_CODE04

void dictext_task_start( void )
{
   g_b_dic_on = TRUE;
}


#pragma CODE = DIC_TASK_CODE

void dictext_task_stop( void )
{
   g_b_dic_on = FALSE;
}


void dic_task( void )
{
   Msg _MEM_TYPE_MALLOC_MAIL_ cmd;
   if( g_b_dic_on )
   {
      if( dictext_g_cmd_id == DICTEXT_Cmd_None )
      {
         if( MAILBOX_NO_MSG != mail_get_command( TASK_DIC, &cmd ))
         {
            SDKLoadCode( TEXT_VIEWER_KEY_CODE );
            dictext_trans_cmd( LSB( cmd.id ), cmd.param );
         }
      }
      dictext_evt();
   }
}

#endif   //#if( DIC == ENABLE )
#endif   //_text_task_c_
