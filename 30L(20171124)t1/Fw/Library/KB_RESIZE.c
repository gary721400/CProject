#include "fc_internal.h"
#include <fs.h>
#include "com.h"

#pragma DATA = KB_RESIZE_DATA
#pragma CODE = KB_RESIZE_CODE


FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size )
{
   unsigned int drive = file->drive;
   unsigned long cluster, cluster2, n, pos;
   PxFSINFO fsinfo = GdrvGet_fsinfo( drive );
   int err, cut = 0, res;

   if( 0 == file )
      return FS_OPENFILE_FULL;

   if( isCWriteProtect( drive ))
      return FS_ACCESS_DENIED;

   if( iChangeDrive( drive ))
      return FS_DRIVE_ERROR;

   if( 0 == fsinfo->total_sectors )
      return FS_FORMAT_ERROR;

   SetFileSystemBusy( drive );

   if( new_size == file->size )
      return FS_NO_ERROR;
   //travel file & cut file if needed
   pos = 0;
   n = fsinfo->sectors_per_cluster * fsinfo->bytes_per_sector;
    
   cluster = file->first_cluster;
   if( 0 == cluster )
   {
      cluster = AddNewCluster( drive, cluster, 2 );
      file->first_cluster = cluster;
      file->cluster = cluster;
      pos = n;
      goto _Expand_file;
   }
   pos = n;
   do
   {
      cluster2 = GetFATEntry(drive, cluster);
      if( 0 == cut && pos >= new_size )
      {
         SetFATEntry( drive, cluster, fsinfo->last_cluster_mark );
         cut = 1;
      }
      else if( cut )
      {
         SetFATEntry( drive, cluster, 0 );
      }
      if( INVALID_CLUSTER == cluster2 )
      {
         err = FS_DRIVE_ERROR;
         goto GoodBye_FCResize;
      }
      if( cluster2 >= fsinfo->last_cluster_mark )
      {
         break; 
      }
      pos += n;
      cluster = cluster2;
   }
   while( 1 );

   if( cut )
   {
      goto _Lresize_end;
   }
   //expand file
_Expand_file:    
   if( file->cluster >= fsinfo->last_cluster_mark )
   {
      cut = 1;
   }
   /*SendString(( U8 *)"new_size == ");
   send_hex_u32(new_size);
   SendReturn();
   SendString(( U8 *)"fsinfo->max_cluster_num == ");
   send_hex_u32(fsinfo->max_cluster_num);
   SendReturn();*/
   for( cluster2 = cluster + 1; cluster2 <= fsinfo->max_cluster_num; ++cluster2 )
   {
      unsigned long x = GetFATEntry( drive, cluster2 );

      if( EMPTY_CLUSTER == x )
      {
         /*SendString(( U8 *)"cluster2 == ");
         send_hex_u32(cluster2);
         SendString(( U8 *)" pos == ");
         send_hex_u32(pos);
         SendReturn();*/
         SetFATEntry( drive, cluster, cluster2 );
         cluster = cluster2;
         pos += n;
         if( pos >= new_size )
         {
            //SendString(( U8 *)"pos>=new_size");
            SetFATEntry( drive, cluster2, fsinfo->last_cluster_mark );
            goto _Lresize_end;
         }
      }
      /*else if( INVALID_CLUSTER == x )
      {
         //SendString(( U8 *)"INVALID_CLUSTER == x");
         goto _Lend;
      }*/
      if( cut )
      {
         file->cluster = cluster2;
         cut = 0;
      }
   }
   
_Lend:
   SetFATEntry( drive, cluster, fsinfo->last_cluster_mark );
   if( pos < new_size )
   {
      /*SendString(( U8 *)"end pos == ");
      send_hex_u32(pos);
      SendReturn();*/
      file->mode |= WRITTEN;
      //file->last_cluster = cluster;
      file->size = pos;

      FSFlush(( __X FCFILE *)file );
      
      ClearFileSystemBusy( drive );        
      return FS_WRITE_OUT_RANGE;
   }
   
_Lresize_end:
   //SendString(( U8 *)"good end\r\n");
   file->mode |= WRITTEN;
   //file->last_cluster = cluster;
   file->size = new_size;
   if( file->size == 0 )
      file->first_cluster = 0;
   
   FSFlush(( __X FCFILE *)file );

   ClearFileSystemBusy( drive );        
   return FS_NO_ERROR;

GoodBye_FCResize:
   
   FSFlush(( __X FCFILE *)file );
   
   ClearFileSystemBusy( drive );
   return err;    
}


