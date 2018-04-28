//! @file oid.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the oid management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: oid.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug


#pragma DATA = GETID_DATA
#pragma CODE = GETID_CODE

#ifndef _opar_c_
#define _opar_c_

//_____ I N C L U D E S ____________________________________________________

//#include "config.h"
#include "compiler.h"         // Compiler definitions

//#include "mcu_drv.h"          // Component declaration
#include "timer_drv.h"  // Include the timer to precharge

//#include "mail.h"
//#include "mail_evt.h"
#include "com.h"
//#include "oid.h"
//#include "intrins.h"

//#include "search.h"
#include "power.h"
//#include "player_ap3.h"
//#include "search_game.h"
//#include "search_remote.h"
//#include "mmi_ap3.h"
#include <stdio.h>
//#include "MP3_api.h" 
//#include "Int_drv.h"
//#include "com_var.h"

#include <sfr.h>
#include "668.h"
#include "cis.h"

//_____ M A C R O S ________________________________________________________

extern  _MEM_TYPE_SLOW_ U32 u32_oid_read_timeout;
 
extern  Bool bPenLeave;
 U32  packet_counter;


//_____ D E F I N I T I O N ________________________________________________
//Bool rw_flag = 0; //read/write flag
//#if( OID_VERSION == OID1 )
extern Bool bPenOn;
//#endif


//#endif
extern Bool exit_flag; //exit main flag

//extern Bool g_b_search_on;


//#if( PASTER == ENABLE )
//extern Bool g_b_paster_on;
//#endif

//extern __X unsigned int Current_Index;
//extern Bool g_b_player_on;
#if 1
 __X  Bool         index_flag;
extern __X unsigned int PlayCode;
extern Bool b_oid_init_flag;
  Bool   g_b_wireless_bind_flag ;
 Bool   g_b_oid_data_flag;
  unsigned long g_timer_tick;
  _MEM_TYPE_SLOW_  U16     Current_Index;
#else
extern __X  Bool         index_flag;
__X unsigned int PlayCode;
 Bool b_oid_init_flag;
extern  Bool   g_b_wireless_bind_flag ;
extern  Bool   g_b_oid_data_flag;
extern _MEM_TYPE_SLOW_  U16     Current_Index;
#endif



extern char GetID(void);
extern void init_getid(void);


//==============================================================================
#if 1
void __interrupt Timer_infrared(void)
{


}



void __interrupt Exception(void)
{

	while(1);	
}

void main(void)
{
	tSDKLoadCodeStatus LoadCodeStatus;
	unsigned int PlayingOIDCode;
// Since the SD, NAND flash and 8088 interface use the same I/O pins,
// if both SD card and NAND Flash are used, or both SD card and 8088 interface are used in the system
// this function must be executed to avoid the competition of I/O pins.

	IP_INIT();
  	SystemClock(0x03);	//(0x02);

/*
 // 	test_flag = 0;
#ifdef SPIBOOT_KBDM9
	SPIF_Set_Clock(SPI_20M);	//(SPI_30M);
#endif
*/
 	SetVector(EXCEPTION_TRAP,Exception);

#ifdef  TX_DEBUG
    UART_Init();
	SendString(( char *)"opar begin!\r\n");
#endif

	g_timer_tick=0;
    // Keyboard I/O port initial
	System_Timer_Init(3,2);  


//	LoadCodeStatus = SDKLoadCodInit();
//	SDKLoadCode(INIT_CODE);
//	main_system_init();

//	init_raw_data();
	//CIS_Init();
	//	OPAR_Init();
	init_getid();

//	SDKLoadCode(TEST_IRAM);
//	main_loop();
	while(1){
		GetID();
		if(index_flag){
			index_flag=FALSE;
#ifdef  TX_DEBUG
	SendString(( char *)"\r\ncurrent_index=");
	send_hex_u16(Current_Index);
#endif			
			}
	}

}

#endif

#endif  // _opar_c_
