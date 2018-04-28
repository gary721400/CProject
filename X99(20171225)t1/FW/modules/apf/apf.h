#ifndef	_APF_H_
#define	_APF_H_

#undef _GLOBEXT_
#if (defined _apf_c_)
#  define _GLOBEXT_
#else                                        
#  define _GLOBEXT_ extern
#endif


_GLOBEXT_   const   U8    g_apf_default_name[]
#if 0//def _apf_c_
= "apf"
#endif
;
#define	APF_DEFAULT_FILE_NAME           g_apf_default_name

#define	APF_OFFSET_CUE_INDEX_NUM        0
#define	APF_OFFSET_OID_CMD_NUM          2
#define	APF_OFFSET_INDEX_START          4
#define	APF_OFFSET_INDEX_DIVISION       6
#define	APF_SIZE_HEADER                 512

#define	APF_OFFSET_CUE_INDEX_TABLE      (( U32 )512)
#define	APF_SIZE_CUE_INDEX_TABLE        ( 512 )

#define	APF_OFFSET_OID_CMD_TABLE        (( U32 )1024 )
#define	APF_SIZE_CUE_INDEX_TABLE        ( 512 )

struct APF_CUE_INFO
{
  U32   offset;
  U32   size;
};
#define	APF_SIZE_FS_INDEX_ITEM          8

#endif
