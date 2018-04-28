#ifndef _NAVEXT_H_
#define _NAVEXT_H_

#include "conf\conf_sio.h"

#if( DIC == ENABLE )

U8    *navext_file_set_ext    ( U8 _MEM_TYPE_SLOW_ *ext );
void  navext_file_use_dir     ( Bool v );
Bool  navext_file_prev        ( void );
Bool  navext_file_next        ( void );
Bool  navext_file_first       ( void );
Bool  navext_file_last        ( void );
Bool  navext_file_dir         ( Bool type );
U8    *navext_file_get_name   ( void );

#endif   //#if( DIC == ENABLE )
#endif   //_NAVEXT_H_