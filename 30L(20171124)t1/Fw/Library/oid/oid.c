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


#pragma DATA = OID_DATA
#pragma CODE = OID_CODE

#ifndef _oid_c_
#define _oid_c_

//_____ I N C L U D E S ____________________________________________________
#include "oid.h"
#include "config.h"
#include "system.h"
//#include "compiler.h"         // Compiler definitions

//#include "mcu_drv.h"          // Component declaration
//#include "timer_drv.h"  // Include the timer to precharge

//#include "mail.h"
//#include "mail_evt.h"
#include "com.h"

//#include "intrins.h"

//#include "search.h"
//#include "power.h"
//#include "player_ap3.h"
//#include "search_game.h"
//#include "search_remote.h"
//#include "mmi_idle.h"
//#include <stdio.h>
//#include "MP3_api.h" 
//#include "Int_drv.h"
//#include "com_var.h"

//_____ M A C R O S ________________________________________________________
 Bool g_b_oid_enable;
#if( T01OID_CAB == ENABLE)
void set_calib_data( void );
#endif

 Bool b_oid_init_flag;

 _MEM_TYPE_SLOW_ U32 u32_oid_read_timeout = 0;
 
 Bool bPenLeave;
 Bool g_oid;
// _MEM_TYPE_SLOW_  U32                  Current_Index; 

//_MEM_TYPE_SLOW_   U16   oid_index_type;
//Bool t01_rst_flag = FALSE;
extern Bool nostop_index(void);

//_____ D E F I N I T I O N ________________________________________________
//Bool rw_flag = 0; //read/write flag
//#if( OID_VERSION == OID1 )
Bool bPenOn;
//#endif
Bool exit_flag = FALSE; //exit main flag

//#if( PASTER == ENABLE )
//extern Bool g_b_paster_on;
//#endif



extern Bool g_b_player_on;

__X  Bool         index_flag;


//_____ D E C L A R A T I O N ______________________________________________
void  oid_init       ( void );
#if( OID_VERSION == OID1 )
U16   read_oid_pen   ( void );
#elif( OID_VERSION == OID2 )
U32   read_oid_pen   ( void );
#endif
void  write_oid_pen  ( U8 );
void  wakeup_oid_mcu ( void );

#if 1//( OID == ENABLE )
/////////WSTS =0
/*



void delay10us( void )
{
   U8 i = 28;
   while( --i );
}


void delay100us( void )
{
   U8 i= 10;
   for( ; i > 0; i-- )
      delay10us();
}


//! @brief delay3us
//!
//! To delay time 3us
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
//WSTS =3
*/
//void delay3us( void )
//{
//   _nop_();
//}
/*
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
   U8 i = 8;
   while( --i );
}

*/
void delay100us( void )
{
   U8 i= 10;
   for( ; i > 0; i-- )
      delay10us();
}


//! @brief oid_init
//!
//! To configurate the oid interface
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
void oid_init( void )
{
   sck_out();
   //SCK = 0;
   clr_sck();
   sdio_in();
   
   g_b_oid_enable = FALSE;
   b_oid_init_flag = FALSE;
   u32_oid_read_timeout = 0;
   bPenLeave = TRUE;
   exit_flag = 0; //exit main flag
   //t01_rst_flag = FALSE;
#if( OID_VERSION == OID1 )
   bPenOn = FALSE;
#endif
  
   
}
void write_oid_ready(void)
{
   if( !read_sdio())
      read_oid_pen();


   OID_INT_DISABLE();
   clr_sck();
   sdio_in();
   delay10us();
   set_sck();
   delay10us();
   clr_sck();
   delay10us();
}

void write_oid_over(void)
{
   delay100us();  //delay 100us
   oid_sdio_input();
   sdio_in();
   OID_INT_ENABLE();
}

void write_oid_U8( U8 val )
{
   U8 count;
   for( count = 0; count < 8; count++ )
   {
      set_sck();
	  OID_INT_ENABLE();
    //  delay10us(); //delay 10us
		
      if( val & 0x80 )
      {
         set_sdio();
         sdio_out();;
      }
      else
      {
         clr_sdio();
         sdio_out();;
      }
      val <<= 1;
		
      delay10us( ); //delay 20us
      OID_INT_DISABLE();
      clr_sck();
	  delay10us();
   }
}
//! @brief oid_pen_init
//!
//! To configurate the oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
#if( OID_VERSION == OID1 )
void oid_pen_init( void )
{
   U32 timer;

   //Bool oid_init_flag = 0; //oid initial success flag, success set 1, fail set 0
   U32 u32_timeout = Timer_set_timer( TIMER_S( 6 ));
   u16_current_index = 0;	
   clr_sck();
   sdio_in(); //set SDIO as input
   //SCK = 0;
   
   bPenLeave = TRUE;
   bPenOn = FALSE;
   
   //if( read_oid_pen() != OIDCmd_PowerOn )
      //oid_init_flag = 1;  //initial oid success flag
   //if( read_oid_pen() != OIDCmd_PowerOn )
      //goto oid_pen_init_end;  //initial oid success flag
   
   //while( !oid_init_flag ) //wakeup by mcu
   while( 1 )
   {
      if( Timer_is_timeout( u32_timeout ))
      {

         //Reset player now
//         SendString(( char *)"oid init fail!\r\n");
	//  	 while(1){
	//		RESET_SYS();
	//		}
			Exception();
         
      }

      wakeup_oid_mcu(); //toggle OID

      timer = Timer_set_timer( TIMER_MS( 60 ));
      while( !Timer_is_timeout( timer ))  //60ms timeout ?
      {
         sdio_in();
         if( !read_sdio())
         {
            oid_init();
            if( read_oid_pen() == OIDCmd_PowerOn )
            {
               //oid_init_flag = 1;
               //break;
               goto oid_pen_init_end;
            }
            else
            {
               oid_init();
               break;
            }
         }
      }
   }
oid_pen_init_end:   
   Delay_ms( 5 );
   write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );
}

#elif( OID_VERSION == OID2 )

#if (T01OID == ENABLE)

//! @brief write_oid_pen
//!
//! To write data to oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param data for write to oid pen device
//! @return none
//!

void write_t01_cmd( U16 val1, U16 val2, U16 val3 )
{
	
 	write_oid_ready();
	write_oid_U8(MSB(val1));
	write_oid_U8(LSB(val1));
	write_oid_U8(MSB(val2));
	write_oid_U8(LSB(val2));
	write_oid_U8(MSB(val3));
	write_oid_U8(LSB(val3));
	write_oid_over();
}



void set_t01_mode( U16 mode_pam1,U16 mode_pam2,U16 mode_pam3,U32 mode_ack)
{
   U32 timer, data_from_pen;
   U16 cnt = 3;
	//timer = Timer_set_timer( TIMER_MS( 300 ));
set_mode_again:
  // write_t01_cmd( UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2 );
  write_t01_cmd( mode_pam1, mode_pam2, mode_pam3 );
   timer = Timer_set_timer( TIMER_MS( 300 ));
   while( read_sdio())
   {
      if( Timer_is_timeout( timer ))
      {
#if 0//def TX_DEBUG      
        SendString((char *)"\r\nset mode err");
#endif         
         goto set_mode_next;
      }
   }
   data_from_pen = read_oid_pen();
   Delay_ms(2);
#if 0//def TX_DEBUG   
   SendString((char *)"\r\ncal1=");
   send_hex_u32(data_from_pen);
#endif
   //if( data_from_pen == OIDCmd_SetMode_Ack )
   if( data_from_pen == mode_ack )
   {
#if 0//def TX_DEBUG   
      SendString((char *)"\r\nset mode ok");
#endif
      return;
   }
set_mode_next:
	//timer = Timer_set_timer( TIMER_MS( 300 ));
   cnt--;
   if( cnt )
      goto set_mode_again;
}


void set_pam_after_reset(void)
{
   //timer = Timer_set_timer( TIMER_MS( 100 ));
   //while( !Timer_is_timeout( timer ));
   //Delay_ms( 200 );
   //read_set_calib(); 
  // SDKLoadCode(SET_CALIB_CODE);
   //set_calib_data();
   set_t01_mode(UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2,OIDCmd_SetMode_Ack);

   set_t01_mode(UserCmd_SetCal1,  MSW( t01_calib_data1 ),  LSW( t01_calib_data1 ),OIDCmd_Setcab1_Ack);
   set_t01_mode(UserCmd_SetCal2,  MSW( t01_calib_data2 ),  LSW( t01_calib_data2 ),OIDCmd_Setcab2_Ack);
#if( DPR3 == ENABLE )
   set_t01_mode( UserCmd_SetCal3, MSB( t01_calib_data3 ), LSB( t01_calib_data3 ), OIDCmd_SetCal3_Ack );
#endif   //#if( DPR3 == ENABLE )


   set_t01_mode(UserCmd_SetGroup, UserCmd_SetGroupParam1, UserCmd_SetGroupParam2,OIDCmd_SetGroup_Ack);


   //write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );
   Delay_ms( 100 );
   write_oid_pen( 0x24 );
   Delay_ms( 100 );
#if( T01OID == ENABLE)
   //SDKLoadCode(T01_COMMAND_CODE);
   
  // set_t01_mode(UserCmd_SetOid1, UserCmd_SetOid1Param1, UserCmd_SetOid1Param2,OIDCmd_SetOid1_Ack);
#endif //#if( T01OID == ENABLE) 

   exit_flag = FALSE;
   //t01_rst_flag = FALSE;
   //set_t01_mode( 0x0901, 0x0000, 0x0000, 0x70001B );
}
#endif

void oid_pen_init( void )
{
   U32 timer;
   U32 u32_timeout = Timer_set_timer( TIMER_S( 6 ));
   u16_current_index = 0;
oid_pen_init:
   if( Timer_is_timeout( u32_timeout ))
   {
      //Reset player now
//      SendString(( char *)"oid init fail!\r\n");
//	  while(1){
//			RESET_SYS();
//			}
		Exception();
   }
   oid_init();
   delay100us();
  
   //SCK = 0;
   clr_sck();
   sdio_in(); //set SDIO as input

   bPenLeave = TRUE;
   bPenOn = FALSE;

   wakeup_oid_mcu(); //toggle OID
   sdio_in();
   timer = Timer_set_timer( TIMER_MS( 2000 ));
   while( read_sdio())
   {
      if( Timer_is_timeout( u32_timeout ))
      {
         //Reset player now
        //	  while(1){
//			RESET_SYS();
//			}
		Exception();
         
      }
      if( Timer_is_timeout( timer ))
      {
         turn_off_oid();
         pen_disable();
         oid_power_off();
         //timer = Timer_set_timer( TIMER_MS( 100 ));
         //while( !Timer_is_timeout( timer ));
         Delay_ms( 100 );
         oid_power_on();
         pen_enable();
         //timer = Timer_set_timer( TIMER_MS( 100 ));
         //while( !Timer_is_timeout( timer ));
         Delay_ms( 100 );
         goto oid_pen_init;
      }
   }
   if( read_oid_pen()!= OID2Cmd_PowerOn )
   {
      turn_off_oid();
      //timer = Timer_set_timer( TIMER_MS( 500 ));
      //while( !Timer_is_timeout( timer ));
      goto oid_pen_init;
   }
	set_pam_after_reset();
}
#endif


#pragma CODE = INIT_OID_CODE
/*
void reinit_oid_pen( void )
{
   U32 timer;
   
   pen_disable();
   oid_power_off();
   //oid_dc_dc_off();
//   timer = Timer_set_timer( TIMER_MS( 1000 ));
//   while( !Timer_is_timeout( timer ));
   timer = Timer_set_timer( TIMER_MS( 10 ));
   while( !Timer_is_timeout( timer ));
   
   oid_power_on();
   pen_enable();
//   timer = Timer_set_timer( TIMER_MS( 500 ));
//   while( !Timer_is_timeout( timer ));
   timer = Timer_set_timer( TIMER_MS( 5 ));
   while( !Timer_is_timeout( timer ));
   //初始化OID pen
   oid_pen_init();
#if 0//( OID_VERSION == OID2 )
   u32_oid_read_timeout = Timer_set_timer( TIMER_S( 15 ));
#endif   //if( OID_VERSION == OID2 )
   b_oid_init_flag = TRUE;
	bPenOn = FALSE;
   
}
*/


void reinit_oid_pen( void )
{
   U32 timer;
#ifdef TX_DEBUG
   SendString((char *)"\r\nbegin init!");
#endif   
   pen_disable();
   oid_power_off();
#if(T01OID_CAB == ENABLE)
if((t01_calib_data1 == 0)&&(t01_calib_data2 == 0)){
	RTCWDT = 0xBFFFu;
	}
#endif  
   //oid_dc_dc_off();
#if( OID_VERSION == OID1 )
   timer = Timer_set_timer( TIMER_MS( 100 ));
#elif( OID_VERSION == OID2 )
   timer = Timer_set_timer( TIMER_MS( 100 ));
#endif   //#if( OID_VERSION == OID1 )
   
   while( !Timer_is_timeout( timer ));
   
   oid_power_on();
   pen_enable();
#if( OID_VERSION == OID2 )
   timer = Timer_set_timer( TIMER_MS( 100 ));
   while( !Timer_is_timeout( timer ));
#endif   //#if( OID_VERSION == OID2 )
   //初始化OID pen
   oid_pen_init();
#ifdef TX_DEBUG
   SendString((char *)"\r\ninit ok");
#endif
   /*timer = Timer_set_timer( TIMER_MS( 100 ));
   while( !Timer_is_timeout( timer ));
   read_set_calib();*/
   b_oid_init_flag = TRUE;
   exit_flag = FALSE;
   bPenOn = FALSE;
   //t01_rst_flag = FALSE;
}



#pragma CODE = OID_CODE
#if 0//( T01OID_CAB == ENABLE)
void set_calib_data( void )
{
   U32 timer;
   //write_t01_cmd( UserCmd_SetCal1, MSW( t01_calib_data1 ), LSW( t01_calib_data1 ));
      set_t01_mode(UserCmd_SetCal1,  MSW( t01_calib_data1 ),  LSW( t01_calib_data1 ),OIDCmd_Setcab1_Ack);
 /*  
   timer = Timer_set_timer( TIMER_MS( 300 ));
   while( read_sdio())
   {
      if( Timer_is_timeout( timer ))
      {
#if 0//def TX_DEBUG
         SendString("\r\nset cal1 err");
#endif		 
         return;
      }
   }
   timer = read_oid_pen();
  
   Delay_ms(10);
 */
#if 0//def TX_DEBUG   
   SendString("\r\ncal1=");
   send_hex_u32(timer);
   //SendString((U8 *)"");
#endif	

   //write_t01_cmd( UserCmd_SetCal2, MSW( t01_calib_data2 ), LSW( t01_calib_data2 ));
    set_t01_mode(UserCmd_SetCal2,  MSW( t01_calib_data2 ),  LSW( t01_calib_data2 ),OIDCmd_Setcab2_Ack);
/*
   timer = Timer_set_timer( TIMER_MS( 300 ));
   while( read_sdio())
   {
      if( Timer_is_timeout( timer ))
      {
#if 0//def TX_DEBUG       
         SendString("\r\nset cal2 err!");
#endif         
         return;
      }
   }
   timer = read_oid_pen();
     Delay_ms(10);
*/	 
#if 0//def TX_DEBUG   
   SendString("\r\ncal2=");
   send_hex_u32(timer);
   //SendString((U8 *)"");
#endif	   
}
#endif //
//! @brief turn_off_oid
//!
//! To turn off the oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
void turn_off_oid( void )
{
   U32 timer;
   write_oid_pen( UserCmd_PowerDownOID );

   clr_sdio();
   sdio_out();
#if( OID_VERSION == OID1 )
   timer = Timer_set_timer( TIMER_MS( 80 ));
#elif( OID_VERSION == OID2 )
   timer = Timer_set_timer( TIMER_MS( 300 ));
#endif
   while( !Timer_is_timeout( timer ))
   {
      sdio_in();
      if( !read_sdio())
      {
         read_oid_pen();
         break;
      }
      clr_sdio();
      sdio_out();
   }
   clr_sdio();
   sdio_out();
   clr_sck();
}


//! @brief read_oid_pen
//!
//! To read data from oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return data from oid pen
//!
#if( OID_VERSION == OID1 )
U16 read_oid_pen( void )
{
   U16 __X from_pen_data;
   U8 __X  count;

   OID_INT_DISABLE();
   clr_sck();  //SCK = 0
   sdio_in(); //SDIO as input
   //SCK = 0;  //SCK = 0
   delay3us();
   set_sck();
   clr_sdio();
   sdio_out();; //SDIO as output, SDIO = 0
   delay3us();
   clr_sck();
   delay3us();

   for( count = 0; count < 16; count++ )
   {
      from_pen_data <<= 1;
      set_sck();
	  OID_INT_ENABLE();
      sdio_in();
      delay3us();
	  OID_INT_DISABLE();
      clr_sck();
      delay3us();
      //SDIO = 1;
      if( read_sdio())
         from_pen_data |= 0x01;
   }
   OID_INT_ENABLE();
   delay100us();  //delay 100us

   /*if( !( from_pen_data & 0x4000 ))
   { // bit14 == 0 
      if( from_pen_data & 0x2000 )
      { // bit13 == 1
         rw_flag = 1;
         write_pen_data = UserCmd_PowerDownOID;
      }
   }*/
   oid_sdio_input();
   if( from_pen_data == OIDCmd_PowerDown )
      exit_flag = TRUE;
   return from_pen_data;
}

#elif( OID_VERSION == OID2 )

#if 1
U32 read_oid_pen( void )
{
   U32 from_pen_data = 0;
   U16 count, head_data = 0, cnt;

   oid_index_type = 0;
   //Int_disable();
    OID_INT_DISABLE();
   
   clr_sck();
   sdio_in();
   delay3us();
   set_sck();
   clr_sdio();
   sdio_out();
   delay3us();
   clr_sck();
   delay3us();

   for( count = 0; count < 7; count++ )
   {
      head_data <<= 1;
      set_sck();

     // Int_enable();
      OID_INT_ENABLE();
      
      sdio_in();
      delay3us();

      //Int_disable();
       OID_INT_DISABLE();
      
      clr_sck();
      delay3us();
	   //SDIO = 1;
      sdio_in();
      if( read_sdio())
         head_data |= 0x01;
   }
   set_sck();
   if( head_data == 0x61 )//oid3 
   {
      oid_index_type = 0x8000;
//      head_data = 0;
      for( count = 0; count < 6; count++ )
      {
         head_data <<= 1;
         set_sck();

        // Int_enable();
         OID_INT_ENABLE();
         
         sdio_in();
         delay3us();

        // Int_disable();
         OID_INT_DISABLE();
         
         clr_sck();
         delay3us();
   	   //SDIO = 1;
         sdio_in();
         if( read_sdio())
            head_data |= 0x01;
      }
      for( count = 0; count < 32; count++ )
      {
         from_pen_data <<= 1;
         set_sck();

       //  Int_enable();
        OID_INT_ENABLE();
         
         sdio_in();
         delay3us();

         //Int_disable();
          OID_INT_DISABLE();
         
         clr_sck();
         delay3us();
   	   //SDIO = 1;
         sdio_in();
         if( read_sdio())
            from_pen_data |= 0x01;
      }
   }
   else //oid2
   {
      /*if(( head_data == 0x50 )
         ||( head_data == 0x40 )
         ||( head_data == 0x60 )
         ||( head_data == 0x53 )
         ||( head_data == 0x43 )
         ||( head_data == 0x63 )
         ||( head_data == 0x70 ))*/
         
      {
         cnt = 16;
         from_pen_data = head_data;
      }
      /*else
      {
         cnt = 23;
         //oid_index_type = 0x8000;
      }*/
      for( count = 0; count < cnt; count++ )
      {
         from_pen_data <<= 1;
         set_sck();

        // Int_enable();
         OID_INT_ENABLE();
         
         sdio_in();
         delay3us();

         //Int_disable();
          OID_INT_DISABLE();
         
         clr_sck();
         delay3us();
   	   //SDIO = 1;
         sdio_in();
         if( read_sdio())
            from_pen_data |= 0x01;
      }
      //if( oid_index_type )
      //   from_pen_data &= 0xFFFF;
   }
   //Int_enable();
    OID_INT_ENABLE();
   
   delay100us();  //delay 100us
   if(( oid_index_type == 0 )&& from_pen_data == OID2Cmd_PowerDown )
      exit_flag = TRUE;

   return from_pen_data;
}

#else
U32 read_oid_pen( void )
{
   U32 __X from_pen_data = 0;
   unsigned char __X count = 0;

   OID_INT_DISABLE();
   clr_sck();
   sdio_in();
   delay3us();
   set_sck();
   clr_sdio();
   sdio_out();
   delay3us();
   clr_sck();
   delay3us();

   for( count = 0; count < 23; count++ )
   {
      from_pen_data <<= 1;
      set_sck();
	  OID_INT_ENABLE();
      sdio_in();
      delay3us();
	  OID_INT_DISABLE();
      clr_sck();
      delay3us();
	   //SDIO = 1;
      sdio_in();
      if( read_sdio())
         from_pen_data |= 0x01;
   }
   OID_INT_ENABLE();
   delay100us();  //delay 100us
   if( from_pen_data == OID2Cmd_PowerDown )
      exit_flag = TRUE;
   return from_pen_data;
}

#endif
#endif

//! @brief write_oid_pen
//!
//! To write data to oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param data for write to oid pen device
//! @return none
//!
void write_oid_pen( U8 val )
{
	write_oid_ready();
	write_oid_U8(val);
	write_oid_over();
}

//! @brief wakeup_oid_mcu
//!
//! To wake up oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
void wakeup_oid_mcu( void )
{
   U32 __X timer;
   set_sck();  //start to toggle OID
#if( OID_VERSION == OID1 )
   timer = Timer_set_timer( TIMER_MS( 17 ));
#elif( OID_VERSION == OID2 )
   timer = Timer_set_timer( TIMER_MS( 50 ));
#endif
   while( !Timer_is_timeout( timer )); //delay 17ms
   clr_sck();  //end of toggle OID
   delay100us();
   delay10us();   //delay 110us
   oid_sdio_input();
}


//#if 1//(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))
//! @brief scan_oid
//!
//! To scan oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param none
//! @return none
//!
#if( OID_VERSION == OID1 )
void scan_oid ( void )
{
   
   U16 __X data_from_pen;
   sdio_in();
   if( !read_sdio())
   {
      //if( !bPenOn )
      {
         bPenOn = TRUE;
         data_from_pen = read_oid_pen();
         u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 100 ));

         /*if( bOidEnable )
         {
            if( data_from_pen == OID_KEY_PRESS )
            {
               bOidEnable = FALSE;
               bOidKey = TRUE;
               return;
            }
         }
         if( data_from_pen == OID_KEY_RELEASE )
         {
            bOidEnable = TRUE;
            return;
         }*/      		 
         if( !( data_from_pen & 0x8000 ))
         {
            data_from_pen &= OID1_INVALID_INDEX;
            if( data_from_pen == OID1_INVALID_INDEX )
               return; //mapping error

            if( data_from_pen > 4095 )
               data_from_pen -= 2;
            if( data_from_pen < OID1_PAGE_INDEX )
            {
//#if (POWER_OFF_AUTO == ENABLE)
//               power_rearm_autopoweroff_delay();
//#endif   //#if (POWER_OFF_AUTO == ENABLE)
               if( u16_current_index == data_from_pen )
               {
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     if( !bSpeechEnd )
                        return;
                  }
#endif
//#if( PASTER == ENABLE )
//                  if( g_b_paster_on )
//                     return;
//#endif
               }
               else
               {
                  bPenLeave = TRUE;
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     if( scan_oid_is_enable())
                     {
                        ap3_rand_init();
                     }
                  }
#endif
               }
               u16_current_index = data_from_pen;
               if( bPenLeave )
               {
#if 1//( AP3 == ENABLE )
                 // if( g_b_search_on )
                  {
//                     GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
//                     GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
                     if( scan_oid_is_enable())
                     {
#if 1                     
 //                       if( !bRemoteChangeAp3 )

                           //Search_Record.news_sys.flag_index = 1;
						   index_flag=TRUE;



   					  }
#endif						
                     else
                        //Search_Record.news_sys.flag_index = 0;
                        index_flag = FALSE;
                     }

                  }

#endif   //#if( AP3 == ENABLE )

                  bPenLeave = FALSE;
               }
            }
         }
   
         //if( rw_flag )
            //write_oid_pen( write_pen_data );

         if( exit_flag )
         {
            oid_pen_init();
            exit_flag = FALSE;
         }
//      }
   }
   if( bPenOn )
   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
         bPenOn = FALSE;
         bPenLeave = TRUE;
      }
   }
}
#endif

#if( OID_VERSION == OID2 )

#if( WAIYAN_OID2 == ENABLE )
U16 chang_oid(U16 dpr_index)
{
  SPIF_ERR SPI_STATE;
  U32 temp_sector;
  U16 temp_off,all_off = dpr_index;

  U16 *p = (U16 *)&Buf;
  temp_off = all_off%256;
  GetDataIdInfo( D_OID, &Start_Sector, &Length );
  temp_sector = Start_Sector +(U32)(all_off >> 8);
  SPI_STATE = spif_resv_read( temp_sector, ( __X void *)Buf );
  if( SPI_STATE != SDTL_No_Err ) return 0xFFFF;
  return p[temp_off];  
}
#endif

void no_index_state(void)
{
	bPenOn = FALSE;
    bPenLeave = TRUE;
}
void scan_oid( void )
{
   U32 __X data_from_pen;
   U16 oid_group;
    sdio_in();
   if( !read_sdio())
   {

//      Int_disable();      
      data_from_pen = read_oid_pen();
//	  Int_enable();

#if 0//def TX_DEBUG
   SendString("\r\noid_index_type:data_from_pen=");
	send_hex_u16(oid_index_type);
	SendString(":");
	send_hex_u32(data_from_pen);
#endif
	   
#if( T01OID == ENABLE)	  
   if(( data_from_pen == OID2Cmd_SystemReset )&&( oid_index_type == 0 ))
   {
 #ifdef TX_DEBUG
   SendString("\r\nOIDCmd_SystemReset!");
#endif   
   	 //SDKLoadCode(T01_COMMAND_CODE);
   	 //SysClkUpgrade();
   	 clkup_time(TRUE);
     set_pam_after_reset();
   }     
#endif //#if( T01OID == ENABLE)      

    if( exit_flag )
      {
#ifdef TX_DEBUG
   SendString("\r\nexit_flag=TRUE!");
#endif       
		 //SysClkUpgrade();
		 clkup_time(TRUE);
         oid_pen_init();
		// SysClkDowngrade();
         exit_flag = FALSE;
		 scan_oid_enable();
		 return;
      }

	if( oid_index_type != 0)goto scan_oid_next;


	 if( !(data_from_pen & 0x400000)){
	 	no_index_state();
        return;
	 	}

	 if(( data_from_pen & 0x200000 )){
	 	no_index_state();
        return;
	 	}

#if( T01OID == ENABLE)
		oid_group = ( U16 )(( data_from_pen >> 18 )& 0x03 );
#endif //#if( T01OID == ENABLE)                
        data_from_pen &= 0x3FFFF;
        if( data_from_pen == OID2_IndexType_Normal_DontCare )
           return;
        if(( data_from_pen >= OID2_IndexType_Normal_Missing_Min )
           &&( data_from_pen <= OID2_IndexType_Normal_Missing_Max ))
        {
           no_index_state();
           return;
        }
#if 0//( T01OID == ENABLE)
        if( oid_group > OID_WAIYAN_GROUP )
           return;
#endif //#if( T01OID == ENABLE)

#if 0
		if( data_from_pen & 0x020000 ){
			data_from_pen &= OID1_INVALID_INDEX;
           if(( data_from_pen == OID1_INVALID_INDEX )
              ||( data_from_pen == OID1_NULL_INDEX )
                 ||( data_from_pen == OID1_ISOLATION_INDEX )
                    ||( data_from_pen == OID1_IDLE_INDEX ))
              return; //mapping error
           if( data_from_pen >( 4095 + 2 ))
              data_from_pen -= 2;
           if( data_from_pen >= OID1_PAGE_INDEX )
              return;
		    SendString((U8 *)"oid1=");
            send_hex_u32(data_from_pen);
            SendString((U8 *)"\r\n");
      
			}else{
					return;
				}

#endif

#if( WAIYAN_OID2 == ENABLE )	
#if 0//def TX_DEBUG
		SendString("\r\nbefore_Index =");
		send_hex_u16(data_from_pen);
#endif 
		data_from_pen = chang_oid((U16)data_from_pen);
#if 0//def TX_DEBUG
		SendString("\r\nafater_Index =");
		send_hex_u16(data_from_pen);
#endif
#endif
        //if( data_from_pen <= 0xFFFF )
scan_oid_next:        
        {
           bPenOn = TRUE;
           u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 1000 ));
           u16_current_index = data_from_pen;
#ifdef TX_DEBUG
SendString("\r\noid_index_type:Current_Index =");
send_hex_u16(oid_index_type);
SendString(":");
send_hex_u32(Current_Index);
#endif   
             if( scan_oid_is_enable())
             {
				index_flag=TRUE;
             }
        }
#if 0

      if( data_from_pen & 0x400000 )
      {
         if( !( data_from_pen & 0x200000 ))
         {
            /*if( data_from_pen & 0x100000 )
               SendString("high\r\n");
            else
               SendString("low\r\n");*/
#if( T01OID == ENABLE)
			oid_group = ( U16 )(( data_from_pen >> 18 )& 0x03 );
#endif //#if( T01OID == ENABLE)                
            data_from_pen &= 0x3FFFF;
            if( data_from_pen == OID_IndexType_Normal_DontCare )
               return;
            if(( data_from_pen >= OID_IndexType_Normal_Missing_Min )
               &&( data_from_pen <= OID_IndexType_Normal_Missing_Max ))
            {
               no_index_state();
               return;
            }
#if( T01OID == ENABLE)
			    //if( oid_group != OID__NORMAL_GROUP )
            if( oid_group > OID_WAIYAN_GROUP )
               return;
#endif //#if( T01OID == ENABLE)
            if( data_from_pen <= 0xFFFF )
            {
               bPenOn = TRUE;
               u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 1000 ));
			   

               u16_current_index = (U16)data_from_pen;
#ifdef TX_DEBUG
   SendString("\r\nCurrent_Index =");
   send_hex_u16(Current_Index);
#endif   
                     if( scan_oid_is_enable())
                     {
                     
//                        if( !bRemoteChangeAp3 )
// 						if(nostop_index())return;
						index_flag=TRUE;
                     }

            }
			else
			{
			    bPenOn = FALSE;
        		bPenLeave = TRUE;
			}
         }
         else
         {
          	bPenOn = FALSE;
            bPenLeave = TRUE;
         }
      }
      else
      {
         bPenOn = FALSE;
         bPenLeave = TRUE;
      }
      
#endif

   }
   
   //if( bPenOn )
   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
         u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 100 ));
//		 SendString("859\r\n");
         bPenOn = FALSE;
         bPenLeave = TRUE;
      }
   }
}


#endif
//#endif  //#if(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))




//------------------------------------------------------------------------------

__asm{
_delay3us:
	PUSH R0
	MOV R0,#27 ; // for fsys = 48Mhz 36
$Loop:
	CMPD1 R0,#0
	AJMP IF_NE,$Loop
	POP R0
	RETS
}

__asm{
_delay10us:
	PUSH R0
	MOV R0,#93 ; // for fsys = 48Mhz //120
$Loop:
	CMPD1 R0,#0
	AJMP IF_NE,$Loop
	POP R0
	RETS
}
/*
__asm{
_delay1ms:
	LINK #0
	PUSH R0
	PUSH R1

	MOV R0,[FP+#(6)]
	MOV R1,#9300 ;// for fsys = 48Mhz
$Loop:
	CMPD1 R1,#0
	AJMP IF_NE,$Loop
	CMPD1 R0,#0
	AJMP IF_NE,$Loop
	POP R1
	POP R0
	UNLINK_MACRO
	RETS
}
*/
#endif  // #if( OID == ENABLE )

#endif  // _oid_c_
