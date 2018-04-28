//! @file oid_cmd.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the oid cmd definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: oid_cmd.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef	_OID_CMD_H_
#define	_OID_CMD_H_

#include "conf\conf_sio.h"

#if( PASTER == ENABLE )
#include "modules\apf\apf.h"

#undef _GLOBEXT_
#if (defined _oid_cmd_c_)
#  define _GLOBEXT_ extern
#else                                        
#  define _GLOBEXT_ extern
#endif

//#define	OID_CMD_DEFAULT_FILE_NAME	g_apf_default_name

enum
{
	OID_CMD_RECORD,
	OID_CMD_PLAY,
	OID_CMD_PAUSE,
	OID_CMD_STOP,
	OID_CMD_COPY,
	OID_CMD_BACK,

	OID_CMD_NUM,
	OID_CMD_NONE = OID_CMD_NUM
};

struct	OID_CMD_ITEM
{
	U16	cmd_id;
	U16	oid_index;
};

_GLOBEXT_   _MEM_TYPE_SLOW_   U16   Current_apf_num
#if 0//def _oid_cmd_c_
   = 0
#endif
;
_GLOBEXT_   _MEM_TYPE_SLOW_   U16   Total_apf_num
#if 0//def _oid_cmd_c_
   = 0
#endif
;
_GLOBEXT_                     Bool  b_apf_file_change
#if 0//def _oid_cmd_c_
   = TRUE
#endif
;

typedef enum
{
   MMI_PASTER_NO_CHANGE_FILE
,  MMI_PASTER_KEY_NEXT_CHANGE_FILE
,  MMI_PASTER_KEY_PREV_CHANGE_FILE
}APF_CHANGE_FILE;
_GLOBEXT_   _MEM_TYPE_SLOW_   APF_CHANGE_FILE   apf_change_file
#if 0//def _oid_cmd_c_
   = MMI_PASTER_NO_CHANGE_FILE
#endif
;


Bool  oid_cmd_init   ( void );
U8    oid_cmd_trans  ( U16 oid_value );

#endif  //#if( PASTER == ENABLE )

#endif  //_OID_CMD_H_
