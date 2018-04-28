#ifndef _TEXT_TASK_H_
#define _TEXT_TASK_H_

#include "conf\conf_sio.h"

#if( DIC == ENABLE )
void  dic_task_init        ( void );
void  dictext_task_start   ( void );
void  dictext_task_stop    ( void );
void  dic_task             ( void );
#endif   //#if( DIC == ENABLE )

#endif   //_TEXT_TASK_H_