//! @file file_aux.c,v
//!
//! Copyright (c) 2005 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the file aux.
//!
//! @version 1.2 snd3b-dvk-1_0_33 $Id: file_aux.c,v 1.2 2005/10/19 15:09:19 eaudreze Exp $
//!
//! @todo
//! @bug
//!
#ifndef _file_aux_c_
#define _file_aux_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "file_aux.h"

#include "modules\file_system\fs_com.h"
//#include "modules\file_system\fat.h"
#include "modules\file_system\navigation.h"
#include "modules\file_system\file.h"

#include "modules\control_access\ctrl_status.h"
#include "modules\control_access\ctrl_access.h"

#include "conf\conf_sio.h"
//#include "lib_system/utilities/string.h"
#include "modules\paster\paster_base.h"
//#include "modules\file_system\fat.h"

#pragma DATA = FILE_AUX_DATA
#pragma CODE = FILE_AUX_CODE

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________
Bool  g_b_disk_is_full /*= FALSE*/;

//_____ D E C L A R A T I O N ______________________________________________
#if 0
#if( AP3 == ENABLE )
#if 0
void file_read_u32( U32 *u32_data )
{
/*
   BYTE_SELECT_PTR( u32_data, 0 ) = ( U8 )file_getc();
   BYTE_SELECT_PTR( u32_data, 1 ) = ( U8 )file_getc();
   BYTE_SELECT_PTR( u32_data, 2 ) = ( U8 )file_getc();
   BYTE_SELECT_PTR( u32_data, 3 ) = ( U8 )file_getc();
*/
   MSB0( *u32_data ) = ( U8 )file_getc();
   MSB1( *u32_data ) = ( U8 )file_getc();
   MSB2( *u32_data ) = ( U8 )file_getc();
   MSB3( *u32_data ) = ( U8 )file_getc();
}
#endif
#endif  //#if( AP3 == ENABLE )


#if( PASTER == ENABLE )
//Fs_file_segment   _MEM_TYPE_SLOW_   segment_src  = { 0xFF, 0, 0 };
Fs_file_segment   _MEM_TYPE_SLOW_   segment_dest = { 0xFF, 0, 0 };

#if 0
void file_read_u32_le( U32 * u32_data )
{
/*
   BYTE_SELECT_PTR( u32_data, 3 ) = (U8)file_getc();
   BYTE_SELECT_PTR( u32_data, 2 ) = (U8)file_getc();
   BYTE_SELECT_PTR( u32_data, 1 ) = (U8)file_getc();
   BYTE_SELECT_PTR( u32_data, 0 ) = (U8)file_getc();
*/
   MSB3( *u32_data ) = ( U8 )file_getc();
   MSB2( *u32_data ) = ( U8 )file_getc();
   MSB1( *u32_data ) = ( U8 )file_getc();
   MSB0( *u32_data ) = ( U8 )file_getc();
}


void file_write_u32_le( U32 * u32_data )
{
/*
   file_putc( BYTE_SELECT_PTR( u32_data, 3 ));
   file_putc( BYTE_SELECT_PTR( u32_data, 2 ));
   file_putc( BYTE_SELECT_PTR( u32_data, 1 ));
   file_putc( BYTE_SELECT_PTR( u32_data, 0 ));
*/
   file_putc( LSB0( *u32_data ));
   file_putc( LSB1( *u32_data ));
   file_putc( LSB2( *u32_data ));
   file_putc( LSB3( *u32_data ));
}


#define COPY_BLOCK_SIZE          32


U8 file_aux_paste( Bool b_stop )
{
   b_stop = b_stop;
/*
   extern U8 _MEM_TYPE_SLOW_ g_id_trans_memtomem;
   Ctrl_status status_stream;
   U8 status_copy;
   U8 nav_id_save;

   nav_id_save = nav_get();  // Get current navigator

   // Check, if copy is running
   if( ID_STREAM_ERR == g_id_trans_memtomem )
   {
      return COPY_FAIL;
   }

   if( b_stop )
   {
      status_copy = COPY_FAIL;
   }
   else
   {
      if(( ID_STREAM_ERR - 1 ) != g_id_trans_memtomem )
      {
         // it isn't the beginning of copy action, then check current stream
         status_stream = stream_state( g_id_trans_memtomem );
         switch( status_stream )  
         {
            case CTRL_BUSY:
            status_copy = COPY_BUSY;
            break;
            
            case CTRL_GOOD:
            status_copy = COPY_FINISH;
            break;
            
            case CTRL_FAIL:
            default:
            status_copy = COPY_FAIL;
            break;
         }
      }
      else
      {
         status_copy = COPY_FINISH;
      }
   
      // Compute a new stream
      if( COPY_FINISH == status_copy )
      {
         stream_stop( g_id_trans_memtomem );
  
         // check eof source file
         nav_select( FS_NAV_ID_USER );
         if( segment_src.u16_size == 0 )
         {
            if( 0 == file_eof())
            {
               status_copy = COPY_BUSY;
               segment_src.u16_size = 0xFFFF; // Get the maximum segment supported by navigation (U16)
               if( !file_read( &segment_src ))
               {
                  status_copy = COPY_FAIL;
               }
            }
            else
            {
               status_copy = COPY_FINISH;
            }
         }
         else
         {
            status_copy = COPY_BUSY;
         }
         nav_select( nav_id_save );
     
         // check destination file
         if( COPY_BUSY == status_copy )
         {
            //if( segment_src.u32_size_or_pos >= fs_g_nav.u8_BPB_SecPerClus )
            if( segment_src.u16_size >= COPY_BLOCK_SIZE )
            {
               //segment_dest.u32_size_or_pos = fs_g_nav.u8_BPB_SecPerClus;
               segment_dest.u16_size = COPY_BLOCK_SIZE;
            }
            else
            {
               segment_dest.u16_size = segment_src.u16_size;
            }
            if( !file_write( &segment_dest ))
            {
               status_copy = COPY_FAIL;
            }
         }
  
         if( COPY_BUSY == status_copy )
         {
            g_id_trans_memtomem = stream_mem_to_mem( segment_src.u8_lun, segment_src.u32_addr, segment_dest.u8_lun, segment_dest.u32_addr,segment_dest.u16_size );
                                  
            segment_src.u16_size -= segment_dest.u16_size;
            segment_src.u32_addr += segment_dest.u16_size;
  
            if( ID_STREAM_ERR == g_id_trans_memtomem )
            {
               status_copy = COPY_FAIL;
            }
         }
      }
   }

   // Check end of copy
   if( COPY_BUSY != status_copy )
   {
      U32 u32_size_exact;
      // Stop copy
      stream_stop( g_id_trans_memtomem );
      g_id_trans_memtomem = ID_STREAM_ERR;

      // Get exact size and close the source file
      nav_select( FS_NAV_ID_USER );
      u32_size_exact = nav_file_lgt();
      file_close();
      nav_select( nav_id_save );

      // If no error then set the exact size on the destination file
      if( COPY_FINISH == status_copy )
      {
         if( !file_seek( u32_size_exact, FS_SEEK_SET ))
         {
            status_copy = COPY_FAIL;
         }
         else
         {
            if( !file_set_eof())
            {
               status_copy = COPY_FAIL;
            }
         }
      }
      file_close();
      // If error then delete the destination file
      if( COPY_FAIL == status_copy )
      {
         //nav_file_del();
//***********************************************
         //2007-04-11 change by xuanyu
         nav_file_del( TRUE );
//***********************************************
      }
   }
   if( status_copy == COPY_FINISH )
   {
      segment_src.u16_size = 0;
   }
   return status_copy;
*/
   return COPY_FAIL;
}


//Bool file_aux_paster_start()
Bool  file_aux_paste_copy( void )
{
/*
   _MEM_TYPE_SLOW_ Fs_index index;
   U8 nav_id_save;
   Bool  status;

   if( nav_file_isdir())
   {
      fs_g_status = FS_ERR_COPY_DIR; // Impossible to copy a directory
      return FALSE;
   }
   // Get reference of copy file 
   index = nav_getindex();

   // In "copy file" navigator select the copy file
   nav_id_save = nav_get();      
   nav_select( FS_NAV_ID_USER );
   status = nav_gotoindex( &index );
   nav_select( nav_id_save );

   return status;
*/
   return FALSE;
}


Bool file_aux_paste_start( void )
{
/*
   extern U8 _MEM_TYPE_SLOW_ g_id_trans_memtomem;
   U8 _MEM_TYPE_SLOW_ nav_id_save;
   Bool status;

   if(ID_STREAM_ERR!= g_id_trans_memtomem)
   {
      fs_g_status = FS_ERR_COPY_RUNNING;  // A copy action is always running
      return FALSE;
   }

   // Open file in write mode with size 0
   //if( !file_open( FOPEN_MODE_W_PLUS ))
   if( !file_open( FOPEN_WRITE_ACCESS | FOPEN_CLEAR_PTR ))
      return FALSE;

   // Open file to copy
   nav_id_save = nav_get();      
   nav_select(  FS_NAV_ID_USER );
   status = file_open(FOPEN_MODE_R);
   nav_select( nav_id_save );

   // If error then close "paste file"
   if( !status )
   {
      file_close();
      return FALSE;
   }
   else
   {
      // Signal start copy
      segment_src.u8_lun = 0xFF;
      segment_src.u32_addr = 0;
      segment_src.u16_size = 0;
      segment_dest = segment_src;
      g_id_trans_memtomem = ID_STREAM_ERR - 1;
      return TRUE;
   }
*/
   return FALSE;
}


U16 file_aux_copy_proccess( void )
{
/*
   U8 nav_id_saved;
   U32 current_pos;
   nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_USER );
   current_pos = ( U16 )( fs_g_nav_entry.u32_pos_in_file >> FS_512B_SHIFT_BIT )- segment_src.u16_size;
   current_pos = current_pos * 100 /(( U16 )( fs_g_nav_entry.u32_size >> FS_512B_SHIFT_BIT ));
   nav_select( nav_id_saved );
   return ( U16 )current_pos;
*/
   return 0;
}
#endif

#if 0
U32 _MEM_TYPE_SLOW_ temp;
U8  _MEM_TYPE_SLOW_ g_temp_file_name[] = "temp.dat";

void file_aux_modify_wav_data( void )
{
   extern U32  _MEM_TYPE_SLOW_ g_u32_play_ap3_start;
   extern U8   _MEM_TYPE_SLOW_ g_id_trans_memtomem;
   U8 nav_id_saved;
   file_open( FOPEN_MODE_R );
   segment_src.u16_size = 1;
   file_read( &segment_src );
   file_close();

   nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_USER );
   if( !nav_drive_set( FILE_AUX_DRIVE ))
   {
      nav_select( nav_id_saved );
      return;
   }
   //nav_drive_set( LUN_ID_NF_DISKMASS );
   if( !nav_dir_root())
   {
      nav_select( nav_id_saved );
      return;
   }
   paster_back_to_top_dir();
   if( !nav_filelist_findname( string_unicode_ptr( g_temp_file_name ), FALSE ))
      nav_file_create( string_unicode_ptr( g_temp_file_name ));
   file_open( FOPEN_MODE_W_PLUS );
   segment_dest.u16_size = 1;
   file_write( &segment_dest );

   g_id_trans_memtomem = stream_mem_to_mem( segment_src.u8_lun, segment_src.u32_addr,
                                             segment_dest.u8_lun, segment_dest.u32_addr,
                                                segment_dest.u16_size );
   stream_stop( g_id_trans_memtomem );
   g_id_trans_memtomem = ID_STREAM_ERR;

   file_seek( segment_dest.u16_size * FS_512B, FS_SEEK_SET );
   file_set_eof();
	
   file_seek( 4, FS_SEEK_SET );
   //file_read_u32_le( &temp );
   MSB3( temp ) = ( U8 )file_getc();
   MSB2( temp ) = ( U8 )file_getc();
   MSB1( temp ) = ( U8 )file_getc();
   MSB0( temp ) = ( U8 )file_getc();
	
   //temp-=(g_u32_play_ap3_start-8);
   temp += FS_512B;
   temp -= g_u32_play_ap3_start;
   file_seek( 4, FS_SEEK_SET );
   //file_write_u32_le( &temp );
   file_putc( LSB0( temp ));
   file_putc( LSB1( temp ));
   file_putc( LSB2( temp ));
   file_putc( LSB3( temp ));

   file_seek( 40, FS_SEEK_SET );
   //file_read_u32_le( &temp );
   MSB3( temp ) = ( U8 )file_getc();
   MSB2( temp ) = ( U8 )file_getc();
   MSB1( temp ) = ( U8 )file_getc();
   MSB0( temp ) = ( U8 )file_getc();
	
   //temp -= ( g_u32_play_ap3_start -( 44 ));
   temp += FS_512B;
   temp -= g_u32_play_ap3_start;
   file_seek( 40, FS_SEEK_SET );
   //file_write_u32_le( &temp );
   file_putc( LSB0( temp ));
   file_putc( LSB1( temp ));
   file_putc( LSB2( temp ));
   file_putc( LSB3( temp ));

   file_close();
   nav_select( nav_id_saved );
}
#endif

#endif  //#if( PASTER == ENABLE )

#if( PASTER == ENABLE )
Bool                          g_b_free_cluster_info_is_init = FALSE;	/*标志，看free cluster有无初始化*/
QueueType   _MEM_TYPE_SLOW_   g_free_cluster_queue_head = 0;	/*堆栈顶数据，真正的栈顶数据索引在g_free_cluster_queue_head-1*/
U8          _MEM_TYPE_SLOW_   g_str_free_cluster_temp_file[] = "sys.tmp";	/*放堆栈的文件*/
U32         _MEM_TYPE_SLOW_   g_u32_free_cluster_buffer_addr = 0;		/*堆栈数据的扇区所在地址*/


U8          _MEM_TYPE_SLOW_   g_cluster_buffer[ FS_CACHE_SIZE ];

#endif  //#if( PASTER == ENABLE )


#if ( USE_FILE_AUX == 1 )

/*空闲簇堆栈处理模块，由于RAM没有足够的空间
存放堆栈数据，所以就在磁盘上建立一个文件sys.tmp
此文件一个扇区大小，用作堆栈数据的存放，需要
处理堆栈的时候就把文件加载到缓存fs_g_sector中来，
虽然这样效率很低，但是应该也能满足要求*/

#define U32_PTR( p )          (( U32 * )( p ))
#if 0
//! 此函数返回堆栈是否为空
Bool file_aux_free_cluster_is_empty( void )
{
   return g_free_cluster_queue_head == 0;
}
#endif

#if( PASTER == ENABLE )
//! 此函数返回堆栈是否为空
Bool file_aux_free_cluster_is_full( void )
{
   return ( g_free_cluster_queue_head )>= MAX_ITEM_NUM;
}


Bool file_aux_load_cluster_buffer( void )
{
   U16 iter;
   if( g_u32_free_cluster_buffer_addr == 0 )
      return FALSE;
   fs_gu32_addrsector = g_u32_free_cluster_buffer_addr;
   if( !fat_cache_read_sector( TRUE ))
      return FALSE;
   for( iter = 0; iter < FS_CACHE_SIZE; ++iter )
      g_cluster_buffer[ iter ] = fs_g_sector[ iter ];
   return TRUE;
}


Bool file_aux_save_cluster_buffer( void )
{
   U16 iter;
   if( g_u32_free_cluster_buffer_addr == 0 )
      return FALSE;
   if( !fat_cache_flush())
      return FALSE;
   for( iter = 0; iter < FS_CACHE_SIZE; ++iter )
      fs_g_sector[ iter ] = g_cluster_buffer[ iter ];
   fs_g_sectorcache.u32_addr = g_u32_free_cluster_buffer_addr;
   fat_cache_sector_is_modify();
   if( !fat_cache_flush())
      return FALSE;
   return TRUE;
}


//!初始化堆栈模块
Bool file_aux_free_cluster_init( void )
{
   U8 _MEM_TYPE_SLOW_ nav_id_saved;
   Bool is_free_cluster_first_create = FALSE;
   g_b_disk_is_full = FALSE;
   g_b_free_cluster_info_is_init = FALSE;
   //初始化文件sys.tmp，如果文件不存在就建立文件
   //如果文件存在就打开
   nav_id_saved = nav_get();
   nav_select( FS_NAV_ID_USER );
   if( !nav_drive_set( FILE_AUX_DRIVE ))
   {
      goto __error;
   }
   //nav_drive_set( LUN_ID_NF_DISKMASS );
   if( !nav_dir_root())
   {
      goto __error;
   }
   paster_back_to_top_dir();
   if( !nav_filelist_findname( string_unicode_ptr( g_str_free_cluster_temp_file ), FALSE ))
   {
      if( !nav_file_create( string_unicode_ptr( g_str_free_cluster_temp_file )))
         goto __error;
      is_free_cluster_first_create = TRUE;
   }
   if( !file_open( FOPEN_MODE_W_PLUS ))
      goto __error;
   segment_dest.u16_size = 1;
   //获取文件第一个扇区的扇区地址
   if( !file_write( &segment_dest ))
   {
      file_close();
      goto __error;
   }
   file_set_eof();
   file_close();
   nav_select( nav_id_saved );

   //初始化变量
   g_u32_free_cluster_buffer_addr = segment_dest.u32_addr;
   g_free_cluster_queue_head = 0;

   //如果sys.tmp文件已经存在，说明上一次的
   //堆栈数据可以继续使用
   if( !file_aux_load_cluster_buffer())
      return FALSE;
   if( !is_free_cluster_first_create )
   {
      for( g_free_cluster_queue_head = 0;
            g_free_cluster_queue_head < MAX_ITEM_NUM;
               ++g_free_cluster_queue_head )
      {
         if( U32_PTR( g_cluster_buffer )[ g_free_cluster_queue_head ]== 0 )
            break;
      }
      g_free_cluster_queue_head = file_aux_free_cluster_adj();
   }
   else
   {
      U32_PTR( g_cluster_buffer )[0] = 0;
   }
   g_b_free_cluster_info_is_init = TRUE;
   //如果g_free_cluster_queue_head为0，说明需要重新开始
   //搜索空闲簇，分两个步骤，先搜索前500个簇
   //如果失败就以500为间隔跳跃搜索一个空闲簇链
   //将搜索到的空闲簇链的首簇纪录到堆栈中
   if( g_free_cluster_queue_head == 0 )
   {
      fs_g_cluster.u32_pos = 2;
      if( !fat_cluster_val( FS_CLUST_VAL_READ ))
         return FALSE;
      do
      {
         if( fs_g_cluster.u32_val == 0 )
         {
            file_aux_free_cluster_put( fs_g_cluster.u32_pos );
            break;
         }
         ++fs_g_cluster.u32_pos;
         if( !fat_cluster_readnext())
            return FALSE;
      }
      while( fs_g_cluster.u32_pos < 502 );
   }
   if( g_free_cluster_queue_head == 0 )
   {
      while( fs_g_cluster.u32_pos < fs_g_nav.u32_CountofCluster )
      {
         if( !fat_cluster_val( FS_CLUST_VAL_READ ))
            return FALSE;
         if( fs_g_cluster.u32_val == 0 )
         {
            for( ; fs_g_cluster.u32_pos >= 2; --fs_g_cluster.u32_pos )
            {
               if( !fat_cluster_val( FS_CLUST_VAL_READ ))
                  return FALSE;
               if( fs_g_cluster.u32_val != 0 )
               {
                  fs_g_cluster.u32_pos++;
                  file_aux_free_cluster_put( fs_g_cluster.u32_pos );
                  break;
               }
            }
            break;
         }
         fs_g_cluster.u32_pos += 500;
      }
   }
   if( file_aux_save_cluster_buffer())
      return TRUE;
   return FALSE;
__error:
   nav_select( nav_id_saved );
   return FALSE;
}

#if 0
Bool file_aux_free_cluster_end( void )
{
   g_b_free_cluster_info_is_init = FALSE;
   return file_aux_save_cluster_buffer();
}
#endif


Fs_cluster _MEM_TYPE_SLOW_ g_cluster_temp;/*临时变量，保存fs_g_cluster的值*/
#endif  //#if( PASTER == ENABLE )


#if 0
//从堆栈获取一个空闲簇的值
U32 file_aux_free_cluster_get( void )
{
   if( !g_b_free_cluster_info_is_init || file_aux_free_cluster_is_empty())
      return 0;
   g_u32_param = 0;
   g_cluster_temp = fs_g_cluster;
   do
   {
      //堆栈弹出数据，检测此数据是否为空闲簇
      g_free_cluster_queue_head--;
      fs_g_cluster.u32_pos = U32_PTR( g_cluster_buffer )[ g_free_cluster_queue_head ];
      if( !fat_cluster_val( FS_CLUST_VAL_READ ))
         return 0;
      //虽然堆栈里所有数据刚放进来的时候的确是空闲簇
      //但是FAT随时变化，所以还必须检测是否为空闲簇
      //不断弹出数据，直到找到一个有效数据为止
      //找到空闲簇以后继续弹出数据，直到栈顶顶数据为空闲簇
      if( fs_g_cluster.u32_val == 0 )
      {
         if( g_u32_param == 0 )
            g_u32_param = fs_g_cluster.u32_pos;
         else
         {
            g_free_cluster_queue_head++;
            break;
         }
      }
   }
   while( !file_aux_free_cluster_is_empty());

   //将g_free_cluster_queue_head所在地方标志为0，就可以记录
   //下栈顶位置
   U32_PTR( g_cluster_buffer )[ g_free_cluster_queue_head ] = 0;
   fs_g_cluster = g_cluster_temp;
   return g_u32_param;
}
#endif


#if( PASTER == ENABLE )
//!将一个空闲簇的值放入堆栈
//参数g_u32_param，存放空闲簇的值
Bool file_aux_free_cluster_put( U32 cluster )
{
   QueueType iter;
   if( !g_b_free_cluster_info_is_init || file_aux_free_cluster_is_full())
      return FALSE;
   //监测此空闲簇的值是否已经存在
   for( iter = g_free_cluster_queue_head; iter > 0; --iter )
   {
      if( U32_PTR( g_cluster_buffer )[ iter - 1 ] == cluster )
      {
         goto __end;
      }
   }

   //将此空闲簇的值放入堆栈
   U32_PTR( g_cluster_buffer )[ g_free_cluster_queue_head ] = cluster;
   g_free_cluster_queue_head++;
   if( !file_aux_free_cluster_is_full())
      U32_PTR( g_cluster_buffer )[ g_free_cluster_queue_head ] = 0;
   //如果堆栈大小刚好为FREE_CLUSTER_RANGE整数倍的
   //时候整理堆栈，将堆栈中无效的数据清除
   //if( g_free_cluster_queue_head % FREE_CLUSTER_RANGE == 0 )
   if( g_free_cluster_queue_head >= MAX_ITEM_NUM )
   {
      g_free_cluster_queue_head = file_aux_free_cluster_adj();
   }
   g_b_disk_is_full = FALSE;
__end:
   return TRUE;
}


//此函数调整堆栈，将堆栈中无效的数据清除
//函数返回堆栈整理后的大小
QueueType file_aux_free_cluster_adj( void )
{
   QueueType _MEM_TYPE_SLOW_ left;
   QueueType _MEM_TYPE_SLOW_ right;
   g_cluster_temp = fs_g_cluster;
   //将堆栈中所有无效的数据全部清除为0
   for( left = 0; left < g_free_cluster_queue_head; ++left )
   {
      fs_g_cluster.u32_pos = U32_PTR( g_cluster_buffer )[ left ];
      if( !fat_cluster_val( FS_CLUST_VAL_READ ))
         goto __bad_end;
      if( fs_g_cluster.u32_val != 0 )
      {
         U32_PTR( g_cluster_buffer )[ left ] = 0;
      }
   }

   //将堆栈中所有留下来的有效数据压缩
   //在一起，也就把之前标志为0的地方全部清除
   for( left = 0; left < g_free_cluster_queue_head; ++left )
   {
      if( U32_PTR( g_cluster_buffer )[ left ] == 0 )
         break;
   }
   if( left >= g_free_cluster_queue_head )
      goto __good_end;
   for( right = left + 1; right < g_free_cluster_queue_head; ++right )
   {
      if( U32_PTR( g_cluster_buffer )[ right ] != 0 )
         break;
   }
   if( right >= g_free_cluster_queue_head )
      goto __good_end;
   for( ; ; )
   {
      while(( U32_PTR( g_cluster_buffer )[ right ] == 0 )
            &&( right < g_free_cluster_queue_head ))
         right++;
      if( right >= g_free_cluster_queue_head )
         break;
      U32_PTR( g_cluster_buffer )[ left ] = U32_PTR( g_cluster_buffer )[ right ];
      U32_PTR( g_cluster_buffer )[ right ] = 0;
      left++;
      right++;
   }
   //fat_cache_flush();
__good_end:
   fs_g_cluster = g_cluster_temp;
   return left;
__bad_end:
   fs_g_cluster = g_cluster_temp;
   return g_free_cluster_queue_head;
}
#endif   //#if( PASTER == ENABLE )

#endif
#endif
#endif  //_file_aux_c_
