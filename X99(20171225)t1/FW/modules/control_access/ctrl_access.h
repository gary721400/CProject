//! @file ctrl_access.h,v
//!
//! Copyright (c) 2004-Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the interface :
//! - between USB <-> MEMORY
//! OR
//! - between USB <- Access Memory Ctrl -> Memory
//!
//! This interface may be controled by a "Access Memory Control" for :
//! - include a management of write protect global or specific
//! - include a management of transfer (ex: DFC on Qundau product)
//! - include a management of access password
//! - ...
//!
//! @version 1.32 snd3-dvk-1_9_5 $Id: ctrl_access.h,v 1.32 2007/09/24 09:37:24 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef _MEM_CTRL_H_
#define _MEM_CTRL_H_

#include "conf\conf_access.h"
#include "conf\conf_explorer.h"                 // Needs setting configuration
#include "ctrl_status.h"
#include "lib_mcu\compiler.h"

//_____ M A C R O S ________________________________________________________

#undef _GLOBEXT_
#if (defined _ctrl_access_c_)
#  define _GLOBEXT_
#else
#  define _GLOBEXT_ extern
#endif

//------- Test the configuration in conf_access.h

//------- END OF Test the configuration in conf_access.h


// FYC: LUN = Logical Unit Number


//! *** Management multi-area SIZE on Nand Flash
// Remark :
// - The sector 0 is always reserved by update and security module
// - The sector 1 to SETTING_AREA_SIZE is always reserved to store the setting
// - The size of security disk (used to store app. PWD) is always of 1.44MB (=B86h sectors)
#define OFFSET_NF_RESERVED    0
#define OFFSET_NF_SETTING     1
#define OFFSET_NF_SECURITY   (1+SETTING_AREA_SIZE)


//! *** Management multi-area LUN on Nand Flash
#define   LUN_ID_NF_RESERVED   (0)
#define   LUN_ID_NF_SETTING    (1)
#define   LUN_ID_NF_DISKMASS   (LUN_ID_NF_SECURITY+1)


//! Computed the other lun (don't add the lun of Mass Storage in mode USB Host)
// FYC: Memory = Logical Unit
#if (MEM_NF == ENABLED)
#  define   LUN_NF      (LUN_ID_NF_DISKMASS+1)
#else
#  define   LUN_NF      0
#endif
#if (MEM_MMC == ENABLED)
#  define   LUN_MMC_SD  1
#else
#  define   LUN_MMC_SD  0
#endif
#if (MEM_USB == ENABLED)
#  define   LUN_USB 1
#else
#  define   LUN_USB 0
#endif

#define  LUN_ID_MMC_SD   (LUN_NF)
#define  LUN_ID_USB      (LUN_NF+LUN_MMC_SD)
#define  MAX_LUN         (LUN_NF+LUN_MMC_SD)


//_____ D E F I N I T I O N S ______________________________________________


//!**** Listing of commun interface ****************************************

U8             get_nb_lun           ( void );
Ctrl_status    mem_test_unit_ready  ( U8 lun );
Ctrl_status    mem_read_capacity    ( U8 lun , U32 _MEM_TYPE_SLOW_ *u32_nb_sector );
U8             mem_sector_size      ( U8 lun );
Bool           mem_wr_protect       ( U8 lun );
Bool           mem_removal          ( U8 lun );


//!**** Listing of READ/WRITE interface ************************************


//!---- Interfaces for memory and RAM --------------------------------------
Ctrl_status    memory_2_ram         ( U8 lun , U32 addr , U8 _MEM_TYPE_SLOW_* Ram );
Ctrl_status    ram_2_memory         ( U8 lun , U32 addr , U8 _MEM_TYPE_SLOW_* Ram );

//!---- Interface for streaming interface ----------------------------------
#define  ID_STREAM_ERR        0xFF
#define  ID_STREAM_NODFC      0x10  // Stream with in the same memory, then don't use DFC interface
/*

#define  TYPE_STREAM_USB      DFC_ID_USB
#define  TYPE_STREAM_AUDIO    DFC_ID_AUDIOPROC1
#define  TYPE_STREAM_AUDIO2   DFC_ID_AUDIOPROC2
#define  TYPE_STREAM_SIO      DFC_ID_SIO
#define  TYPE_STREAM_SPI      DFC_ID_SPI
#define  TYPE_STREAM_DEVNULL  DFC_ID_NULL_DEV
*/

U8          stream_mem_to_mem       ( U8 src_lun , U32 src_addr , U8 dest_lun , U32 dest_addr , U16 nb_sector );
U8          stream_read_10_start    ( U8 TypeStream , U8 lun , U32 addr , U16 nb_sector );
U8          stream_write_10_start   ( U8 TypeStream , U8 lun , U32 addr , U16 nb_sector );
U8          stream_to_stream_start  ( U8 TypeStream_src , U8 TypeStream_dest  , U16 nb_sector );
Ctrl_status stream_state            ( U8 Id );
U16         stream_stop             ( U8 Id );


//!---- Global buffer management -------------------------------------------
//_GLOBEXT_ _MEM_TYPE_SLOW_ U8 g_trans_buffer[512];
Bool        trans_buffer_alloc      ( void );
void        trans_buffer_free       ( void );



   void disp_sys_init_load( U8 id_module_display, U8 display_zone );
#endif   // _MEM_CTRL_H_

