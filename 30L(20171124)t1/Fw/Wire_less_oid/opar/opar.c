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
//#include "com.h"
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

//#include "apManager.h"
#include "cis.h"
#include "668.h"
//#include "oparcode.h"
//#include "IRControl.h"

//_____ M A C R O S ________________________________________________________

 _MEM_TYPE_SLOW_ U32 u32_oid_read_timeout;
 
 Bool bPenLeave;
 extern U32  packet_counter;


//_____ D E F I N I T I O N ________________________________________________
//Bool rw_flag = 0; //read/write flag
//#if( OID_VERSION == OID1 )
Bool bPenOn;
//#endif


//#endif
Bool exit_flag; //exit main flag

//extern Bool g_b_search_on;


//#if( PASTER == ENABLE )
//extern Bool g_b_paster_on;
//#endif

//extern __X unsigned int Current_Index;
//extern Bool g_b_player_on;
#if 0
 __X  Bool         index_flag;
__X unsigned int PlayCode;
 Bool b_oid_init_flag;
  Bool   g_b_wireless_bind_flag ;
  Bool   g_b_oid_data_flag;
  unsigned long g_timer_tick;
  _MEM_TYPE_SLOW_  U16     Current_Index;
#else
extern __X  Bool         index_flag;
__X unsigned int PlayCode;
 Bool b_oid_init_flag;
 _MEM_TYPE_SLOW_  U16     Current_Index_bak;
extern  Bool   g_b_wireless_bind_flag ;
extern  Bool   g_b_oid_data_flag;
extern _MEM_TYPE_SLOW_  U16     Current_Index;
#endif

//extern char Decode(void);

//_____ D E C L A R A T I O N ______________________________________________

//! @brief scan_oid
//!
//! To scan oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!


void init_getid(void)
{
	bPenOn=FALSE;
	u32_oid_read_timeout=0;
	 bPenLeave = TRUE;
	 b_oid_init_flag=FALSE;
//	 Current_Index=0;
   Current_Index_bak=0;
	OPAR_Init();
}

void GetID ( void )
{
#if 1

  U16 __X data_from_pen=0;
  data_from_pen = Decode();
   if( data_from_pen != 0)
   {
   //      bPenOn = TRUE;
	//	 bPenLeave = FALSE;

		 
	      if( u32_oid_read_timeout && (!Timer_is_timeout( u32_oid_read_timeout ))) //50 * 10ms
	      {
	         if(Current_Index_bak == data_from_pen )return;
	      }
		 
           u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 1000 ));

		  if(64766 == data_from_pen)//if(64766 == data_from_pen)
		   {
		     	g_b_wireless_bind_flag = TRUE;
			    return;
		   }
		 
	       if(Current_Index_bak == data_from_pen )
	       {
	       
	       }
	       else
	       {
	              Current_Index = data_from_pen;
				  Current_Index_bak = data_from_pen;
				  index_flag = TRUE;
//		          bPenLeave = FALSE;
				  packet_counter++;
				  g_b_oid_data_flag=TRUE;
	       }
   }
//   if( bPenOn )
//   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
      	 u32_oid_read_timeout = 0;
		 Current_Index_bak = 0;
 //        bPenOn = FALSE;
 //        bPenLeave = TRUE;
      }
 //  }
#else
   if( Decode())
   {
         bPenOn = TRUE;
         u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 100 ));     		 
	       if( Current_Index == PlayCode)
	       {
	       }
	       else
	       {
	          bPenLeave = TRUE;
	       }
           Current_Index = PlayCode;
           if( bPenLeave )
           {
              if( scan_oid_enable())
              {
                    Search_Record.news_sys.flag_index = 1;
					index_flag=TRUE;
   
               }
               else
                    Search_Record.news_sys.flag_index = 0;
            }
              bPenLeave = FALSE;


   }
   if( bPenOn )
   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
         bPenOn = FALSE;
      }
   }
#endif   
}


void delay3us( void )
{
   /*_nop_();*/
   U8 i = 14;
   while( --i );
}

//! @brief delay10us
//!
//! To delay time 10us
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param delay time
//! @return none
//!
void delay10us( void )
{
   /*U8 i = 8;
   while( --i );*/
   U8 i = 49;
   while( --i );
}


void delay100us( void )
{
	int mscount,timecount=0;
	for(mscount=0;mscount<=640;mscount++)
	{
		 ;
	}
}

void delay1ms( int delay_time )
{
	int mscount,timecount=0;
	for(timecount=0;timecount<delay_time;timecount++)
		for(mscount=0;mscount<=6400;mscount++)
		{
			 ;
		}
}



//==============================================================================
#if 0
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
  	SystemClock(0x02);	//(0x02);


 // 	test_flag = 0;
#ifdef SPIBOOT_KBDM9
	SPIF_Set_Clock(SPI_20M);	//(SPI_30M);
#endif

 	SetVector(EXCEPTION_TRAP,Exception);

	g_timer_tick=0;
    // Keyboard I/O port initial
	System_Timer_Init(3,TICK_PERIOD);  


//	LoadCodeStatus = SDKLoadCodInit();
//	SDKLoadCode(INIT_CODE);
//	main_system_init();
	CIS_Init();

//	SDKLoadCode(TEST_IRAM);
//	main_loop();
	while(1){
		GetID();
	}

}

#endif

#endif  // _opar_c_
