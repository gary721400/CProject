//------------------------------------------------------------------------------
#ifndef _EXAMPLE_USB_STORAGE_H_
#define _EXAMPLE_USB_STORAGE_H_
#include "conf_kb.h"
#include "compiler.h"
#include "com.h"
#include "SYSTEM.H"
//------------------------------------------------------------------------------
	//#define USE_SD1		1	//  1:use SD1	0:no use SD2
	//#define USE_SD2		1	//  1:use SD1	0:no use SD2
	#define USE_UGP		0	//  1:use UGP	0:no use UGP
	#define USE_SPIF	0	//  1:use SPI	0:no use SPI
//------
	#define USE_TIMER	2	//  Timer : 0~3, other is disable
//------
	#define SD_CACHE_2K	0
	#define SD_CACHE_4K	1
	#define SD_CACHE_8K	2
//------
	#define SD1_CACHE	SD_CACHE_8K
	#define SD2_CACHE	SD_CACHE_4K//SD1_CACHE
//------------------------------------------------------------------------------
#if USE_SD1
	extern void MassStorage_SD1_Init( void );
	extern void MassStorage_SD1_Loop( void );
	extern void MassStorage_SD1_Register( int lun );
#endif
//------
#if  USE_SD2
	extern void MassStorage_SD2_Init( void );
	extern void MassStorage_SD2_Loop( void );
	extern void MassStorage_SD2_Register( int lun );
#endif
//------
#if USE_UGP
	extern void MassStorage_ugp_Init( void );
	extern void MassStorage_ugp_Loop( void );
	extern void MassStorage_ugp_Register( int lun );
#endif
//------
#if  USE_SPIF
	extern void spif_cache_init(void);
	extern SPIF_ERR spif_cache_read(unsigned long sector, __X void *buf);
	extern SPIF_ERR spif_cache_write(unsigned long sector, __X void *buf);
	extern int spif_cache_flush(void) ;

	extern void MassStorage_SPIF_Init( void );
	extern void MassStorage_SPIF_Loop( void );
	extern void MassStorage_SPIF_Register( int lun );
#endif
//------------------------------------------------------------------------------
	#define USB_CONNECT 0
	#define USB_DISCONN 1
//------
	extern void MassStorage_Init(void);         /*1. Initial */
	extern void MassStorage_Registers(void);    /*2. Register */
	extern int  MassStorage_Loop(void);         /*3. In Loop */
	extern void MassStorage_Exit(void);         /*4. Exit */
	extern void MassStorage_Write(void);
#endif
//------------------------------------------------------------------------------
