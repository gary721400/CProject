//! @file ctrl_access.c,v
//!
//! Copyright (c) 2004-Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the interface of logical unit (LU Number)
//!
//! This interface may be controled by a "Access Memory Control" for :
//! - include a management of write protect global or specific
//! - include a management of transfer (ex: DFC on Qundau product)
//! - include a management of access password
//! - ...
//!
//! @version 1.81 snd3-dvk-1_9_5 $Id: ctrl_access.c,v 1.81 2007/09/24 09:37:24 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _ctrl_access_c_
#define _ctrl_access_c_

//_____ I N C L U D E S ____________________________________________________

#include "ctrl_access.h"
#include "lib_mcu\int\int_drv.h"                // Interrupt definitions
#include "lib_mcu\compiler.h"
#include "conf\conf_access.h"
#include "lib_system\memory\memory.h"
#include "modules\file_system\fat.h"
#include "stdio.h"
#include "sdhost.h"      // Global constant for SD_CIF 
#include "mmi\shared\com_var.h"
#include "mmi\rw_nor\rw_nor.h"
#include "modules\paster\paster_base.h"
#include "drivers\mmi\img_handler.h"

//_____ R E F E R E N C E S ________________________________________________


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N S ______________________________________________
#if (MEM_NF == ENABLED)
Bool g_public_drive_write_enable = FALSE;
#endif


//------- Test the configuration in conf_access.h

// Active the Logical Unit
#ifndef  MEM_NF
#  error MEM_NF must be defined with ENABLED or DISABLED in conf_access.h
#endif
#if (MEM_NF == ENABLED)
#  include "\lib_mem\nand_flash\nf_mngt.h"
#endif

#ifndef  MEM_MMC
#  error MEM_MMC must be defined with ENABLED or DISABLED in conf_access.h
#endif
#if (MEM_MMC == ENABLED)
#include "lib_mem\mmc\mmc_sd.h"
#endif

#ifndef  MEM_USB
#  error MEM_USB must be defined with ENABLED or DISABLED in conf_access.h
#endif
#if (MEM_USB == ENABLED)
#include "\modules\usb\host_drivers\usb_host_ms.h"
#endif

//! Specific option of control access
#ifndef  GLOBAL_WR_PROTECT
#  error GLOBAL_WR_PROTECT must be defined with ENABLED or DISABLED in conf_access.h
#endif

// Check configuration
#  if (MAX_LUN == 0)
#     error No memory is active in conf_access.h
#  endif

// Write protect variable
#if (GLOBAL_WR_PROTECT == ENABLED)
   static U8 s_u8_wr_protect;
#endif


//_____ D E F I N I T I O N S __ F O N C T I O N S _________________________


//! This fonction return the number of logical unit for embedded system
//!
//! @return U8   number of logical unit in the system
//!
U8 get_nb_lun( void )
{
   return MAX_LUN;
}


//! This fonction test the state of memory, and start the initialisation of the memory
//!
//! MORE (see SPC-3 ?.2.4) : The TEST UNIT READY command allows an application client
//! to poll a logical unit until it is ready without the need to allocate space for returned data.
//! The TEST UNIT READY command may be used to check the media status of logical units with removable media.
//!
//! @param lun        Logical unit number
//!
//! @return                          Ctrl_status
//!   It is ready              ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
Ctrl_status mem_test_unit_ready( U8 lun )
{
   switch( lun )
   {
#if (MEM_NF == ENABLED)
      case LUN_ID_NF_RESERVED:   // do not break
      case LUN_ID_NF_SETTING:    // do not break
#  if (LUN_ID_NF_SECURITY != LUN_ID_NF_SETTING)
      case LUN_ID_NF_SECURITY:   // do not break
#  endif
      case LUN_ID_NF_DISKMASS:
      return nf_test_unit_ready();
      break;
#endif

#if (MEM_MMC == ENABLED)
      case LUN_ID_MMC_SD:
      return mmc_test_unit_ready();
      break;
#endif
   }
   return CTRL_FAIL;
}


//! This fonction return the capacity of the memory
//!
//! @param lun        Logical unit number
//!
//! @return the last LBA of disk (unit 512B)
//!
//! @return                          Ctrl_status
//!   It is ready              ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
#if 0
Ctrl_status mem_read_capacity( U8 lun , U32 _MEM_TYPE_SLOW_ *u32_nb_sector )
{
   switch( lun )
   {
#if (MEM_NF == ENABLED)
      case LUN_ID_NF_RESERVED:
      *u32_nb_sector = OFFSET_NF_SETTING -OFFSET_NF_RESERVED -1;
		return CTRL_GOOD;
      case LUN_ID_NF_SETTING:
      *u32_nb_sector = OFFSET_NF_SECURITY -OFFSET_NF_SETTING -1;
		return CTRL_GOOD;
#  if (LUN_ID_NF_SECURITY != LUN_ID_NF_SETTING)
      case LUN_ID_NF_SECURITY:
      *u32_nb_sector = OFFSET_NF_DISKMASS -OFFSET_NF_SECURITY -1;
		return CTRL_GOOD;
#  endif
      case LUN_ID_NF_DISKMASS:
      status = nf_read_capacity( u32_nb_sector );
		*u32_nb_sector -= OFFSET_NF_DISKMASS;
		return status;
      break;
#endif

#if (MEM_MMC == ENABLED)
      case LUN_ID_MMC_SD:
      return mmc_read_capacity( u32_nb_sector );
      break;
#endif

   }
   return CTRL_FAIL;
}
#endif

//! This fonction return the physical sector of the memory
//!
//! @param lun        Logical unit number
//!
//! @return size of physical sector of disk (unit 512B)
//!
//! @return                          Ctrl_status
//!   It is ready              ->    CTRL_GOOD
//!   Memory unplug            ->    CTRL_NO_PRESENT
//!   Not initialize or change ->    CTRL_BUSY
//!   A error occur            ->    CTRL_FAIL
//!
U8    mem_sector_size( U8 lun )
{
   if( lun < LUN_ID_USB )
      return 1;   // For NandFlash and MMC/SD, it is a sector of 512B

   return 0;
}


//! This fonction return is the write protected mode
//!
//! @param lun        Logical unit number
//!
//! Only used by memory removal with a HARDWARE SPECIFIC write protected detection
//! !!! The customer must be unplug the card for change this write protected mode.
//!
//! @return TRUE  -> the memory is protected
//!
Bool  mem_wr_protect( U8 lun )
{
   lun = lun;
   return FALSE;
#if 0
   switch( lun )
   {
#if (MEM_NF == ENABLED)
      case LUN_ID_NF_RESERVED:   // do not break
      case LUN_ID_NF_SETTING:    // do not break
      case LUN_ID_NF_DISKMASS:
      return nf_wr_protect();
      break;
#  if (LUN_ID_NF_SECURITY != LUN_ID_NF_SETTING)
      case LUN_ID_NF_SECURITY:   // do not break
      return !g_public_drive_write_enable;
      break;
#  endif
#endif

#if (MEM_MMC == ENABLED)
      case LUN_ID_MMC_SD:
      return mmc_wr_protect();
      break;
#endif

   }
   return TRUE;
#endif
}


//************************************************************************************
//!----------- Listing of READ/WRITE interface with MODE ACCESS DFC ------------------
//************************************************************************************

_MEM_TYPE_SLOW_   U8  g_src_lun;
_MEM_TYPE_SLOW_   U32 g_src_addr;
_MEM_TYPE_SLOW_   U8  g_dest_lun;
_MEM_TYPE_SLOW_   U32 g_dest_addr;
_MEM_TYPE_SLOW_   U16 g_nb_sector;


//! This fonction initialise the informations about memory source 
//!
void ctrl_set_lun_src( U8 lun )
{
   g_src_lun   = lun;
}


//! This fonction initialise the informations about memory destination
//!
void ctrl_set_lun_dest( U8 lun )
{
   g_dest_lun   = lun;
}


//! This fonction Iinitialise memory
//!
//! @return Ctrl status
//!
Ctrl_status ctrl_init_memory( void )
{
   _MEM_TYPE_SLOW_ Ctrl_status status = CTRL_GOOD;
   
   switch( g_src_lun )
   {
#  if (MEM_NF == ENABLED)
      case LUN_ID_NF_RESERVED:
      status = nf_10(g_src_addr + OFFSET_NF_RESERVED, g_nb_sector, NF_READ);
      break;
      case LUN_ID_NF_SETTING:
      status = nf_10(g_src_addr + OFFSET_NF_SETTING, g_nb_sector, NF_READ);
      break;
#     if (LUN_ID_NF_SECURITY != LUN_ID_NF_SETTING)
      case LUN_ID_NF_SECURITY:
      status = nf_10(g_src_addr + OFFSET_NF_SECURITY, g_nb_sector, NF_READ);
      break;
#     endif
      case LUN_ID_NF_DISKMASS:
      status = nf_10(g_src_addr + OFFSET_NF_DISKMASS, g_nb_sector, NF_READ);
      break;
#  endif

#  if (MEM_MMC == ENABLED)
      case LUN_ID_MMC_SD:
      status = mmc_10(g_src_addr , g_nb_sector, MMC_READ);
      break;
#  endif

   }
   
   if( CTRL_GOOD != status)
      return status;

   switch( g_dest_lun )
   {
#  if (MEM_NF == ENABLED)
      case LUN_ID_NF_RESERVED:
      status = nf_10(g_dest_addr + OFFSET_NF_RESERVED, g_nb_sector, NF_WRITE);
      break;
      case LUN_ID_NF_SETTING:
      status = nf_10(g_dest_addr + OFFSET_NF_SETTING,  g_nb_sector, NF_WRITE);
      break;
#     if (LUN_ID_NF_SECURITY != LUN_ID_NF_SETTING)
      case LUN_ID_NF_SECURITY:
      status = nf_10(g_dest_addr + OFFSET_NF_SECURITY, g_nb_sector, NF_WRITE);
      break;
#     endif
      case LUN_ID_NF_DISKMASS:
      status = nf_10(g_dest_addr + OFFSET_NF_DISKMASS, g_nb_sector, NF_WRITE);
      break;
#  endif

#  if (MEM_MMC == ENABLED)
      case LUN_ID_MMC_SD:
      status = mmc_10(g_dest_addr , g_nb_sector, MMC_WRITE);
      break;
#  endif

   }
   return status;
}


//!---- Interfaces for memory and RAM --------------------------------------

//! This fonction tranfer one sector data from memory to ram
//!
//! @param lun          Logical unit number
//! @param addr         Sector address to start read (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//! @param ram          Adresse of ram buffer (only xdata)
//!
Ctrl_status memory_2_ram( U8 lun, U32 addr, U8 _MEM_TYPE_SLOW_ *ram )
{
   _MEM_TYPE_SLOW_ Ctrl_status status;
   Bool flag = 0;
   
   // Initialise transfer informations
   g_dest_lun  = 0xFF;
   g_nb_sector = 1;
   g_src_addr  = addr;
   ctrl_set_lun_src( lun );
   switch(( U16 )ram )
   {
      case 0xA800:
      SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800
      break;

      case 0xB800:
      SDH_DMA_CS = 0x08;
      break;

      case 0xBA00:
      SDH_DMA_CS = 0x18;
      break;

      case 0xBC00:
      SDH_DMA_CS = 0x28;
      break;

      case 0xBE00:
      SDH_DMA_CS = 0x38;
      break;

      case 0xC000:
      SDH_DMA_CS = 0x09;
      break;

      case 0xC200:
      SDH_DMA_CS = 0x19;
      break;

      case 0xC400:
      SDH_DMA_CS = 0x29;
      break;

      case 0xC600:
      SDH_DMA_CS = 0x39;
      break;
      
      case 0xC800:
      SDH_DMA_CS = 0x0A;   // select buffer 0 at 0xC800
      break;

      case 0xCA00:
      SDH_DMA_CS = 0x1A;   // select buffer 0 at 0xCA00
      break;

      case 0xCC00:
      SDH_DMA_CS = 0x2A;   // select buffer 0 at 0xCC00
      break;
      
      case 0xCE00:
      SDH_DMA_CS = 0x3A;   // select buffer 0 at 0xCE00
      break;

      case 0xD000:
      SDH_DMA_CS = 0x0B;
      break;

      case 0xD200:
      SDH_DMA_CS = 0x1B;
      break;

      case 0xD400:
      SDH_DMA_CS = 0x2B;
      break;

      case 0xD600:
      SDH_DMA_CS = 0x3B;
      break;
      
      default:
      SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800
      flag = 1;
      break;
   }
   
   // Init memory, start, wait end of transfer
   status = ctrl_init_memory();

   //dfc_free( &status, g_id_curr_trans_dfc );
   status = mmc_dfc_read_stop();
   if( flag )
      memcpy_ram2ram( ram, ( U8 _MEM_TYPE_SLOW_ *)0xA800, FS_CACHE_SIZE );
   return status;
}


//! This fonction transfer a data from ram to memory
//!
//! @param lun          Logical unit number
//! @param addr         Sector address to start write (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//! @param ram          Adresse of ram buffer (only xdata)
//!
//! @return TRUE  -> The memory is removal
//!
Ctrl_status    ram_2_memory( U8 lun , U32 addr , U8 _MEM_TYPE_SLOW_ * ram )
{
   _MEM_TYPE_SLOW_ Ctrl_status status;
   Bool flag = 0;
   switch(( U16 )ram )
   {
      case 0xA800:
      SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800
      break;

      case 0xAA00:
      SDH_DMA_CS = 0x16;   // select buffer 0 at 0xAA00
      break;

      case 0xAC00:
      SDH_DMA_CS = 0x26;   // select buffer 0 at 0xAC00
      break;

      case 0xB800:
      SDH_DMA_CS = 0x08;
      break;

      case 0xBA00:
      SDH_DMA_CS = 0x18;
      break;

      case 0xBC00:
      SDH_DMA_CS = 0x28;
      break;

      case 0xBE00:
      SDH_DMA_CS = 0x38;
      break;

      case 0xC000:
      SDH_DMA_CS = 0x09;
      break;

      case 0xC200:
      SDH_DMA_CS = 0x19;
      break;

      case 0xC400:
      SDH_DMA_CS = 0x29;
      break;

      case 0xC600:
      SDH_DMA_CS = 0x39;
      break;
/*
      case 0xC800:
      SDH_DMA_CS = 0x0A;   // select buffer 0 at 0xC800
      break;

      case 0xCA00:
      SDH_DMA_CS = 0x1A;   // select buffer 0 at 0xCA00
      break;

      case 0xCC00:
      SDH_DMA_CS = 0x2A;   // select buffer 0 at 0xCC00
      break;

      case 0xCE00:
      SDH_DMA_CS = 0x3A;   // select buffer 0 at 0xCE00
      break;

      case 0xD000:
      SDH_DMA_CS = 0x0B;
      break;

      case 0xD200:
      SDH_DMA_CS = 0x1B;
      break;

      case 0xD400:
      SDH_DMA_CS = 0x2B;
      break;

      case 0xD600:
      SDH_DMA_CS = 0x3B;
      break;*/
      
      default:
      SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800
      flag = 1;
      break;
   }
   if( flag )
      memcpy_ram2ram(( U8 _MEM_TYPE_SLOW_ *)0xA800, ram, FS_CACHE_SIZE );
   // Initialise transfer informations
   g_src_lun  = 0xFF;
   g_nb_sector = 1;
   g_dest_addr  = addr;
   ctrl_set_lun_dest( lun );
   
   // Init memory, start, wait end of transfer
   status = ctrl_init_memory();
   if( CTRL_GOOD != status)
      return status;

   //dfc_free( &status, g_id_curr_trans_dfc );
   status = mmc_dfc_write_stop();
   return status;
}

#if 0
//!---- Interface for streaming interface ----------------------------------

//! This fonction copy a data from memory to other memory
//!
//! @param addr         Sector address to start write
//! @param nb_sector    Number of sectors to transfer
//!
U8    stream_mem_to_mem( U8 src_lun , U32 src_addr , U8 dest_lun , U32 dest_addr , U16 nb_sector )
{
   _MEM_TYPE_SLOW_ Ctrl_status status;
   
   // Initialise transfer informations
   g_nb_sector = nb_sector;
   g_src_addr  = src_addr;
   g_dest_addr  = dest_addr;
   ctrl_set_lun_src( src_lun );
   ctrl_set_lun_dest( dest_lun );
   
   if( g_dest_dfc != g_src_dfc )
   {
      // ID dfc different, then high speed transfer possible

      // Alloc transfer
      if( !ctrl_dfc_alloc())
         return ID_STREAM_ERR;
         
      // Init memory
      status = ctrl_init_memory();
      if(CTRL_GOOD == status)
      {
         // start transfer
         dfc_start( g_id_curr_trans_dfc , g_nb_sector );
         dfc_state( g_id_curr_trans_dfc );
         return g_id_curr_trans_dfc;
      }else{
         // Error then Free channel
         dfc_free( &status, g_id_curr_trans_dfc );
         return ID_STREAM_ERR;
      }
   }else{
      // ID dfc is the same, then transfert data via buffer
      if( !trans_buffer_alloc() )
         return ID_STREAM_ERR;

      while( nb_sector != 0 )
      {
	      status = memory_2_ram( src_lun , src_addr , g_trans_buffer );
         if(CTRL_GOOD != status)
            break;
	      status = ram_2_memory( dest_lun , dest_addr , g_trans_buffer);
         if(CTRL_GOOD != status)
            break;
         src_addr++;
         dest_addr++;
         nb_sector--;
      }
      trans_buffer_free();
      if(CTRL_GOOD != status)
         return ID_STREAM_ERR;
      return ID_STREAM_NODFC;
   }
}


//! Create a transfer from memory to stream
//!
//! @param TypeStream   id of stream pipe
//! @param lun          Logical unit number
//! @param addr         Sector address to start write (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//!
//! @return transfer id or ID_STREAM_ERR
//!
U8 stream_read_10_start( U8 TypeStream , U8 lun , U32 addr , U16 nb_sector )
{
   _MEM_TYPE_SLOW_ Ctrl_status status;
    
   // Initialise transfer informations
   g_dest_lun  = 0xFF;
   g_dest_dfc  = TypeStream;
   g_nb_sector = nb_sector;
   g_src_addr  = addr;
   ctrl_set_lun_src( lun );

   // Alloc transfer
   if( !ctrl_dfc_alloc())
      return ID_STREAM_ERR;
      
   // Init memory
   status = ctrl_init_memory();

   if(CTRL_GOOD == status)
   {
#  if (MEM_MMC == ENABLED)
      if((g_src_lun  == LUN_ID_MMC_SD )
      &&  pll_is_cpu_boost())
      {
         // Sync DFC frequency on MMC freq
         Mcu_set_sfr_page_clock();
         Clk_dfc_nfc_set_clock(clk_mmc_get_dfc_freq());
         Mcu_set_sfr_page_default();
      }
#  endif
      // start transfer
      dfc_start( g_id_curr_trans_dfc , g_nb_sector );
      dfc_state( g_id_curr_trans_dfc );
      return g_id_curr_trans_dfc;
   }else{
      // Error then Free channel
      dfc_free( &status, g_id_curr_trans_dfc );
      return ID_STREAM_ERR;
   }
}


//! Create a transfer from stream to memory
//!
//! @param TypeStream   id of stream pipe
//! @param lun          Logical unit number
//! @param addr         Sector address to start write (sector = 512B)
//! @param nb_sector    Number of sectors to transfer
//!
//! @return transfer id or ID_STREAM_ERR
//!
U8 stream_write_10_start( U8 TypeStream , U8 lun , U32 addr , U16 nb_sector )
{
   _MEM_TYPE_SLOW_ Ctrl_status status;
   
   // Initialise transfer informations
   g_src_lun   = 0xFF;
   g_src_dfc   = TypeStream;
   g_nb_sector = nb_sector;
   g_dest_addr = addr;
   ctrl_set_lun_dest( lun );

   // Alloc transfer
   if( !ctrl_dfc_alloc())
      return ID_STREAM_ERR;
      
   // Init memory
   status = ctrl_init_memory();
   if(CTRL_GOOD == status)
   {
#  if (MEM_MMC == ENABLED)
      if((g_dest_lun == LUN_ID_MMC_SD )
      &&  pll_is_cpu_boost())
      {
         // Sync DFC frequency on MMC freq
         Mcu_set_sfr_page_clock();
         Clk_dfc_nfc_set_clock(clk_mmc_get_dfc_freq());
         Mcu_set_sfr_page_default();
      }
#  endif
      // start transfer
      dfc_start( g_id_curr_trans_dfc , g_nb_sector );
      dfc_state( g_id_curr_trans_dfc );
      return g_id_curr_trans_dfc;
   }else{
      // Error then Free channel
      dfc_free( &status, g_id_curr_trans_dfc );
      return ID_STREAM_ERR;
   }
}


//! Create a transfer from stream to stream
//!
//! @param TypeStream   id of stream pipe source
//! @param TypeStream   id of stream pipe destination
//! @param nb_sector    Number of sectors to transfer (0 == no limit)
//!
//! @return transfer id or ID_STREAM_ERR
//!
U8 stream_to_stream_start( U8 TypeStream_src , U8 TypeStream_dest  , U16 nb_sector )
{
   // Initialise transfer informations
   g_src_dfc   = TypeStream_src;
   g_dest_dfc  = TypeStream_dest;
   g_nb_sector = nb_sector;

   // Alloc transfer
   if( !ctrl_dfc_alloc())
      return ID_STREAM_ERR;
      
   // start transfer
   dfc_start( g_id_curr_trans_dfc , g_nb_sector );
   dfc_state( g_id_curr_trans_dfc );
   return g_id_curr_trans_dfc;
}


//! Returns the state on a data transfer
//!
//! @param id           transfer id
//!
//! @return the state of the transfer
//!          CTRL_GOOD              It is finish
//!          CTRL_BUSY              It is running
//!          CTRL_FAIL              It is fail
//!
Ctrl_status stream_state( U8 Id )
{
   if( Id == ID_STREAM_NODFC )
      return CTRL_GOOD;   // finish in start routine
   return dfc_state( Id );
}


//! Stop the data transfer
//!
//! @param id  Transfer id
//!
//! @return the number of sector remainder
//!
U16 stream_stop( U8 Id )
{
   if( Id == ID_STREAM_NODFC )
      return 0;   // finish in start routine

#  if (MEM_MMC == ENABLED)
   if( pll_is_cpu_boost() )
   {
      // Restore DFC freq (default is sync with NFC)
      Mcu_set_sfr_page_clock();
      Clk_dfc_nfc_set_clock(G_CLK_DFC_NFC);
      Mcu_set_sfr_page_default();
   }
#  endif
   return dfc_free( &s_status, Id );
}


//!---- Global buffer management -------------------------------------------

//! Reserve the "trans buffer"
//!
//! @return status   TRUE = alloc OK,  FALSE = error alloc
//!
Bool  trans_buffer_alloc   ( void )
{
   if( b_alloc ) return FALSE;
   b_alloc = TRUE;
   return TRUE;
}


//! Interface for loading display objects through the register only
//!
void  trans_buffer_free    ( void )
{
   b_alloc = FALSE;
}
#endif   // ACCESS_DISPLAY == ENABLED

//! Interface for loading display objects through the register only
#if (ACCESS_DISPLAY == ENABLED)
//! Init the loading of a display object
//!
//! @param  id_module_display id of the display object to load
//!         display_zone      zone in which this object is stored
//!
//! @return status      PASS or FAIL about the loading
//!
void disp_sys_init_load( U8 id_module_display, U8 display_zone )
{
   if( g_b_animation )
      return;

   switch( display_zone )
   {
      case ZONE_PICTURE:
      nor_flash_bass_address = PICTURE_BEGIN_ADDRESS;
      break;

      case ZONE_FONT:
      switch( id_module_display )
      {
         case FONT_TBL_GB2312:
         nor_flash_bass_address = GB2312_ADDRESS;
         break;

         case FONT_TBL_B5:
         nor_flash_bass_address = BG5_ADDRESS;
         break;

         case IDC_FONT_16:
         nor_flash_bass_address = FONT_16_ADDRESS_UNICODE;
         break;

         /*case IDC_FONT_08:
         nor_flash_bass_address = FONT_8_ADDRESS_UNICODE;
         break;*/

         case IDC_FONT_24:
         nor_flash_bass_address = FONT_24_ADDRESS_UNICODE;
         break;
      }
      break;
   }
/*
   //== Read Flash data using XDATA[0x700] Liner ROM ..
   //Start address
   XBYTE[ 0x705 ] = ( U8 )( nor_flash_address >> 16 );
   XBYTE[ 0x706 ] = ( U8 )( nor_flash_address >> 8 );
   XBYTE[ 0x707 ] = ( U8 )nor_flash_address;
*/
   XBYTE[ 0x70E ] = 0xB7;
   XBYTE[ 0x70F ] = 0xFF;
}
#endif

#endif // _ctrl_access_c_
