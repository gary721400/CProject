#ifndef	_FILE_AUX_H_
#define	_FILE_AUX_H_

#include "modules\file_system\file.h"
#include "conf\conf_sio.h"

#undef _GLOBEXT_
#if (defined _file_aux_c_)
#  define _GLOBEXT_ extern
#else
#  define _GLOBEXT_ extern
#endif

void  file_read_u32             ( U32 *u32_data );

U8    file_aux_paste            ( Bool b_stop );
Bool  file_aux_paste_start      ( void );
Bool  file_aux_paste_copy       ( void );
U16   file_aux_copy_proccess    ( void );
void  file_aux_modify_wav_data  ( void );


//_GLOBEXT_   U32   _MEM_TYPE_SLOW_   g_u32_last_pos;
//_GLOBEXT_   U32   _MEM_TYPE_SLOW_   g_u32_param;

#if 1//( PASTER == ENABLE )
_GLOBEXT_   Bool                    g_b_disk_is_full
#if 0//def _file_aux_c_
   = FALSE
#endif
;
#endif  //#if( PASTER == ENABLE )

#define MAX_ITEM_NUM             ( FS_512B / 4 )
#define FREE_CLUSTER_RANGE       16

#if( MAX_ITEM_NUM <= 255 )
   typedef U8                    QueueType;
#else
   typedef U16                   QueueType;
#endif


#define USE_FILE_AUX             1

//#define FILE_AUX_DRIVE	         LUN_ID_MMC_SD

#if( USE_FILE_AUX == 1 )

//#define file_aux_free_cluster_put( v )     ( g_u32_param = ( v ), _file_aux_free_cluster_put())

Bool        file_aux_load_cluster_buffer     ( void );
Bool        file_aux_save_cluster_buffer     ( void );
Bool        file_aux_free_cluster_init       ( void );
Bool        file_aux_free_cluster_end        ( void );
U32         file_aux_free_cluster_get        ( void );
Bool        file_aux_free_cluster_put        ( U32 cluster );
Bool        file_aux_free_cluster_is_empty   ( void );
Bool        file_aux_free_cluster_is_full    ( void );
QueueType   file_aux_free_cluster_adj        ( void );
#else
#define file_aux_load_cluster_buffer()    TRUE
#define file_aux_save_cluster_buffer()    TRUE
#define file_aux_free_cluster_init()      TRUE
#define file_aux_free_cluster_end()       TRUE
#define file_aux_free_cluster_get()       ( 0 )
#define file_aux_free_cluster_put( v )
#define file_aux_free_cluster_is_empty()  FALSE
#define file_aux_free_cluster_is_full()   FALSE
#define file_aux_free_cluster_adj()       ( 0 )
#endif

#endif
