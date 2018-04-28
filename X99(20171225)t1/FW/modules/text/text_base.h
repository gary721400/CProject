#ifndef _TEXT_BASE_H_
#define _TEXT_BASE_H_

#include "conf\conf_sio.h"

#if( DIC == ENABLE )

#undef _GLOBEXT_
#if (defined _text_base_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

enum
{
   TEXTBASE_Type_None,
   TEXTBASE_Type_TextFile,
   TEXTBASE_Type_SmallText
};

_GLOBEXT_   U8    _MEM_TYPE_SLOW_   textbase_g_type;

#define  textbase_get_type()        textbase_g_type

Bool  textbase_init     ( U8 type, void *param );
Bool  textbase_is_init  ( void );
/*Bool*/void  textbase_end      ( void );
Bool  textbase_seek     ( U32 pos );
U16   textbase_getc     ( void );
U32   textbase_get_pos  ( void );
//U8    textbase_get_type ( void );

#endif   //#if( DIC == ENABLE )
#endif   //_TEXT_BASE_H_
