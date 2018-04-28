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
#ifndef _oid_c_
#define _oid_c_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "lib_mcu\compiler.h"         // Compiler definitions
//#include "dp8051.h"
#include "lib_mcu\mcu_drv.h"          // Component declaration
#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge

#include "lib_system\mailbox\mail.h"
#include "lib_system\mailbox\mail_evt.h"
#include "sio\com.h"
#include "drivers\oid\oid.h"
//#include <intrins.h>
#include "drivers\mmi\img_handler.h"


//#include "modules\search\search.h"
#include "modules\power\power.h"
//#include "modules\player_ap3\player_ap3.h"
//#include "modules\search\search_game.h"
//#include "modules\search\search_remote.h"
#include "mmi\idle\mmi_idle.h"
#include <stdio.h>
#include "lib_mcu\int\int_drv.h"                // Interrupt definitions
#include "mmi\shared\com_var.h"
//#include "SSSMP3_Regs.h"
//#include "header.h"
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = OID_DATA
#pragma CODE = OID_CODE
ImageDeclareType(SET_OID_COMMAND);
//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________
//Bool rw_flag = 0; //read/write flag
#if( OID_VERSION == OID1 )
Bool bPenOn = FALSE;
#endif
Bool exit_flag /*= FALSE*/; //exit main flag

#if 0//( AP3 == ENABLE )
extern Bool g_b_search_on;
#endif

#if 0//( PASTER == ENABLE )
extern Bool g_b_paster_on;
#endif

#if 0//( DIC == ENABLE )
extern Bool g_b_dic_on;
#endif

//extern Bool g_b_recorder_on;

//_MEM_TYPE_SLOW_   U8    oid_battery_low_counter = 0;

//Bool  b_oid_pen = FALSE;
//Bool  b_oid_bat_lower = FALSE;

Bool  g_b_oid_enable /*= FALSE*/;

//Bool  b_oid_init_flag = FALSE;//

_MEM_TYPE_MEDSLOW_   U32   u32_oid_read_timeout /*= 0*/;

Bool  bPenLeave /*= TRUE*/;

Bool  g_oid /*= FALSE*/;

_MEM_TYPE_MEDSLOW_   U16   Current_Index;

_MEM_TYPE_MEDSLOW_   U32   u32_oid_read_time;

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
void delay3us( void )
{
   U8 i = 2;
   while( --i );
}


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
}*/


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
}
void write_oid_ready(void)
{
   if( !read_sdio())
      read_oid_pen();
   Int_disable();
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
   Int_enable();
}

void write_oid_U8( U8 val )
{
   U8 count;
   for( count = 0; count < 8; count++ )
   {
      set_sck();
	  Int_enable();
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
      Int_disable();
      clr_sck();
	  delay10us();
   }
  // Int_enable();
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
	
   clr_sck();
   sdio_in(); //set SDIO as input
   //SCK = 0;
   
   bPenLeave = TRUE;
   bPenOn = FALSE;
   green_led_on();
   
   //if( read_oid_pen() != OIDCmd_PowerOn )
      //oid_init_flag = 1;  //initial oid success flag
   //if( read_oid_pen() != OIDCmd_PowerOn )
      //goto oid_pen_init_end;  //initial oid success flag
   
   //while( !oid_init_flag ) //wakeup by mcu
   while( 1 )
   {
      if( Timer_is_timeout( u32_timeout ))
      {
         //CLASS_D_POP_CTL = 0;
         //CLASS_D_PWM_CTL = 0;
         //Reset player now
         /*P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
         (*(void(*)())0)();*/
         //power_system_off( FALSE );
         //RESET_SYS();
         RTCWDT = 0xBFFFu;
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
   delay1ms( 5 );
   write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );
}

#elif( OID_VERSION == OID2 )

#if 1//( T01OID == ENABLE)

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








Bool set_t01_one_mode( U8 mode_pam1,U32 mode_ack)
{
   U32 timer, data_from_pen;
   U16 cnt = 3;
	timer = Timer_set_timer( TIMER_MS( 300 ));
set_mode_again:
  // write_t01_cmd( UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2 );
  //write_t01_cmd( mode_pam1, mode_pam2, mode_pam3 );
    write_oid_ready();
	write_oid_U8(mode_pam1);
	write_oid_over();
//   timer = Timer_set_timer( TIMER_MS( 50 ));
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
   delay1ms(5);
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
      return TRUE;
   }
set_mode_next:
	timer = Timer_set_timer( TIMER_MS( 300 ));
   cnt--;
   if( cnt )
      goto set_mode_again;

   return FALSE;
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
//   timer = Timer_set_timer( TIMER_MS( 50 ));
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
   delay1ms(5);
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

void set_calib_data( void )
{
   U32 timer;
   //write_t01_cmd( UserCmd_SetCal1, MSW( t01_calib_data1 ), LSW( t01_calib_data1 ));
      set_t01_mode(UserCmd_SetCal1,   t01_calib_data1 ,  0 ,OIDCmd_Setcab1_Ack);
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
    set_t01_mode(UserCmd_SetCal2,  t01_calib_data2 ,  0 ,OIDCmd_Setcab2_Ack);
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

#pragma CODE = SET_OID_COMMAND
void set_oid_type(Bool set_type)
{
	 if(set_type){
		 (void)set_t01_one_mode(OID_SONIX_2_COMMAND,OID_SONIX_2_COMMAND_ACK);
		 //set_t01_mode(UserCmd_SetMode_Waiyan, UserCmd_SetModeParam1_Waiyan, UserCmd_SetModeParam2_Waiyan,OIDCmd_SetMode_Ack_Waiyan);
		 (void)set_t01_one_mode(UserCmd_Waiyan_Command_01_Param1,UserCmd_Waiyan_Ack_01);
		 set_t01_mode(UserCmd_Waiyan_Command_02_Param1, UserCmd_Waiyan_Command_02_Param2, UserCmd_Waiyan_Command_02_Param3,UserCmd_Waiyan_Ack_02);
		 (void)set_t01_one_mode(UserCmd_Waiyan_Command_03_Param1,UserCmd_Waiyan_Ack_03);
		 (void)set_t01_one_mode(UserCmd_Waiyan_Command_04_Param1,UserCmd_Waiyan_Ack_04);
		 set_t01_mode(UserCmd_Waiyan_Command_05_Param1, UserCmd_Waiyan_Command_05_Param2, UserCmd_Waiyan_Command_05_Param3,UserCmd_Waiyan_Ack_05);
		 set_t01_mode(UserCmd_Waiyan_Command_06_Param1, UserCmd_Waiyan_Command_06_Param2, UserCmd_Waiyan_Command_06_Param3,UserCmd_Waiyan_Ack_06);
		 set_t01_mode(UserCmd_Waiyan_Command_07_Param1, UserCmd_Waiyan_Command_07_Param2, UserCmd_Waiyan_Command_07_Param3,UserCmd_Waiyan_Ack_07);
		 (void)set_t01_one_mode(UserCmd_Waiyan_Command_08_Param1,UserCmd_Waiyan_Ack_08);
		 }
	 else{
		 (void)set_t01_one_mode(OID_DPR3_COMMAND,OID_DPR3_COMMAND_ACK);
		 }
}

void set_pam_after_reset(void)
{
	//SDKLoadCode(OID_INIT_CODE);
#if( T01OID == ENABLE)
   //SDKLoadCode(T01_COMMAND_CODE);
   set_t01_mode(UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2,OIDCmd_SetMode_Ack);
   set_t01_mode(UserCmd_SetGroup, UserCmd_SetGroupParam1, UserCmd_SetGroupParam2,OIDCmd_SetGroup_Ack);
#endif //#if( T01OID == ENABLE) 
  
   //Delay_ms( 5 );
   write_oid_pen( 0x24 );
	delay1ms( 100 );
   //write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );
   delay1ms( 100 );


#if( T01OID_CAB == ENABLE)
   //timer = Timer_set_timer( TIMER_MS( 100 ));
   //while( !Timer_is_timeout( timer ));
   //Delay_ms( 200 );
   //read_set_calib(); 
  // SDKLoadCode(SET_CALIB_CODE);
   set_calib_data();
#if 1
	set_oid_type(g_bluetooth);
	set_t01_mode(OID_THRID_COMMAND_PARAM_1,OID_THRID_COMMAND_PARAM_2, OID_THRID_COMMAND_PARAM_3, OID_THRID_COMMAND_ACK);
#else
	if(g_setting_list.struct_setting.bluetooth){
		(void)set_t01_one_mode(OID_SONIX_2_COMMAND,OID_SONIX_2_COMMAND_ACK);
		//set_t01_mode(UserCmd_SetMode_Waiyan, UserCmd_SetModeParam1_Waiyan, UserCmd_SetModeParam2_Waiyan,OIDCmd_SetMode_Ack_Waiyan);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_01_Param1,UserCmd_Waiyan_Ack_01);
		set_t01_mode(UserCmd_Waiyan_Command_02_Param1, UserCmd_Waiyan_Command_02_Param2, UserCmd_Waiyan_Command_02_Param3,UserCmd_Waiyan_Ack_02);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_03_Param1,UserCmd_Waiyan_Ack_03);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_04_Param1,UserCmd_Waiyan_Ack_04);
		set_t01_mode(UserCmd_Waiyan_Command_05_Param1, UserCmd_Waiyan_Command_05_Param2, UserCmd_Waiyan_Command_05_Param3,UserCmd_Waiyan_Ack_05);
		set_t01_mode(UserCmd_Waiyan_Command_06_Param1, UserCmd_Waiyan_Command_06_Param2, UserCmd_Waiyan_Command_06_Param3,UserCmd_Waiyan_Ack_06);
		set_t01_mode(UserCmd_Waiyan_Command_07_Param1, UserCmd_Waiyan_Command_07_Param2, UserCmd_Waiyan_Command_07_Param3,UserCmd_Waiyan_Ack_07);
		(void)set_t01_one_mode(UserCmd_Waiyan_Command_08_Param1,UserCmd_Waiyan_Ack_08);
   		}
	else{
		(void)set_t01_one_mode(OID_DPR3_COMMAND,OID_DPR3_COMMAND_ACK);
		}
   set_t01_mode(OID_THRID_COMMAND_PARAM_1,OID_THRID_COMMAND_PARAM_2, OID_THRID_COMMAND_PARAM_3, OID_THRID_COMMAND_ACK);
#endif   
#endif   //#if( T01OID_CAB == ENABLE)
   exit_flag = FALSE;
}

#endif //#if( T01OID == ENABLE)

#pragma CODE = OID_CODE

void oid_pen_init( void )
{
   U32 timer;
   U32 u32_timeout = Timer_set_timer( TIMER_S( 6 ));

   green_led_on();
oid_pen_init_restart:
   if( Timer_is_timeout( u32_timeout ))
   {
      //CLASS_D_POP_CTL = 0;
      //CLASS_D_PWM_CTL = 0;
      //Reset player now
      /*P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
      (*(void(*)())0)();*/
      //power_system_off( FALSE );
      //RESET_SYS();
      RTCWDT = 0xBFFFu;
   }
   oid_init();
   delay100us();
  
   //SCK = 0;
   clr_sck();
   sdio_in(); //set SDIO as input

   bPenLeave = TRUE;

   wakeup_oid_mcu(); //toggle OID
   sdio_in();
   timer = Timer_set_timer( TIMER_MS( 2000 ));
   while( read_sdio())
   {
      if( Timer_is_timeout( u32_timeout ))
      {
         //CLASS_D_POP_CTL = 0;
         //CLASS_D_PWM_CTL = 0;
         //Reset player now
         /*P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
         (*(void(*)())0)();*/
         //power_system_off( FALSE );
         //RESET_SYS();
         RTCWDT = 0xBFFFu;
      }
      if( Timer_is_timeout( timer ))
      {
         turn_off_oid();
         pen_disable();
         oid_power_off();
         //timer = Timer_set_timer( TIMER_MS( 100 ));
         //while( !Timer_is_timeout( timer ));
         delay1ms(100);
         oid_power_on();
         pen_enable();
         //timer = Timer_set_timer( TIMER_MS( 100 ));
         //while( !Timer_is_timeout( timer ));
         delay1ms(100);
         goto oid_pen_init_restart;
      }
   }
   if( read_oid_pen()!= OIDCmd_PowerOn )
   {
      turn_off_oid();
      //timer = Timer_set_timer( TIMER_MS( 500 ));
      //while( !Timer_is_timeout( timer ));
      goto oid_pen_init_restart;
   }
   SDKLoadCode(SET_OID_COMMAND);
   set_pam_after_reset();
   /*
#if( T01OID == ENABLE)
   set_t01_mode(UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2,OIDCmd_SetMode_Ack);
   set_t01_mode(UserCmd_SetGroup, UserCmd_SetGroupParam1, UserCmd_SetGroupParam2,OIDCmd_SetGroup_Ack);
#endif //#if( T01OID == ENABLE)   

   delay1ms( 5 );
   write_oid_pen( 0x24 );
   write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );
#if( T01OID_CAB == ENABLE)
   //timer = Timer_set_timer( TIMER_MS( 100 ));
   //while( !Timer_is_timeout( timer ));
   delay1ms( 100 );
   //read_set_calib(); 
   set_calib_data();
#endif   //#if( T01OID_CAB == ENABLE)
*/
}
#endif


void reinit_oid_pen( void )
{
   U32 timer;
   
   pen_disable();
   oid_power_off();
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
   //³õÊ¼»¯OID pen
   oid_pen_init();
   b_oid_init_flag = TRUE;
   exit_flag = FALSE;
}


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
   U16 from_pen_data;
   U8 count;

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
      sdio_in();
      delay3us();
      clr_sck();
      delay3us();
      //SDIO = 1;
      if( read_sdio())
         from_pen_data |= 0x01;
   }
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
U32 read_oid_pen( void )
{
   U32 from_pen_data = 0;
   U8 count;
   Int_disable();
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
	  Int_enable();
      sdio_in();
      delay3us();
	  Int_disable();
      clr_sck();
      delay3us();
	   //SDIO = 1;
      sdio_in();
      if( read_sdio())
         from_pen_data |= 0x01;
   }
   Int_enable();
   delay100us();  //delay 100us
   if( from_pen_data == OIDCmd_PowerDown )
      exit_flag = TRUE;
   return from_pen_data;
}
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
   U32 timer;
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


#if 1//(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))
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
   //extern Bool g_oid;
   U16 data_from_pen;
   //sdio_in();

   if( u32_oid_read_time && !Timer_is_timeout( u32_oid_read_time ))
      return;
   u32_oid_read_time = 0;
   
   if( !read_sdio())
   {
      //if( !bPenOn )
      {
         bPenOn = TRUE;
         //Int_disable();
         data_from_pen = read_oid_pen();
         //Int_enable();
         u32_oid_read_time = Timer_set_timer( TIMER_MS( 50 ));
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
            data_from_pen &= OID_INVALID_INDEX;
            if( data_from_pen == OID_INVALID_INDEX )
               return; //mapping error

            if(( data_from_pen == OID_NULL_INDEX )
               ||( data_from_pen == OID_ISOLATION_INDEX )
                  ||( data_from_pen == OID_IDLE_INDEX ))
               return; //mapping error

            if( data_from_pen >( 4095 + 2 ))
               data_from_pen -= 2;
            if( data_from_pen < OID_PAGE_INDEX )
            {
               /*if( bPenLeave && !g_b_recorder_on )
               {
                  green_led_on();
                  green_led_start_timer( GREEN_LED_ON_READ_TIME );
               }*/
#if (POWER_OFF_AUTO == ENABLE)
               power_rearm_autopoweroff_delay();
#endif   //#if (POWER_OFF_AUTO == ENABLE)
               if( u16_current_index == data_from_pen )
               {
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     if( !bSpeechEnd )
                        return;
                  }
#endif
#if 0//( PASTER == ENABLE )
                  if( g_b_paster_on )
                     return;
#endif
#if 0//( DIC == ENABLE )
                  if( g_b_dic_on )
                     return;
#endif
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
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
                     GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
                     if( scan_oid_is_enable())
                     {
                        if( !bRemoteChangeAp3 )
                        {
                           Search_Record.news_sys.flag_index = 1;
                           //Current_Index = u16_current_index;
                           //if( player_ap3_is_busy())
                              //player_ap3_stop();
                        }
                     }
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
                  else if( g_b_remote_on )
                  {
                     if( scan_oid_is_enable())
                        Search_Record.news_sys.flag_index = 1;
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif   //#if( AP3 == ENABLE )
                  //if( g_b_remote_on )
                  {
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                  }
#if 0//( PASTER == ENABLE )
                  if( g_b_paster_on )
                  {
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                     //paster_send_cmd( CMD_PASTER_OID, u16_current_index );
                  }
#endif
#if 0//( DIC == ENABLE )
                  if( g_b_dic_on )
                  {
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                  }
#endif
                  bPenLeave = FALSE;
                  if( green_led_is_timer_stopped())
                     green_led_start_timer( GREEN_LED_ON_READ_TIME );
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
      }
   }
   if( bPenOn )
   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
#if 0//( PASTER == ENABLE )
         if( g_b_paster_on )
            u16_current_index = 0;
#endif
#if 0//( DIC == ENABLE )
         if( g_b_dic_on )
            u16_current_index = 0;
#endif
         bPenOn = FALSE;
         bPenLeave = TRUE;
      }
   }
}

#elif( OID_VERSION == OID2 )
U16 chang_oid(U16 dpr_index)
{
  SPIF_ERR SPI_STATE;
  U32 temp_sector;
  U16 temp_off,all_off = dpr_index;

  U16 *p = (U16 *)&Buf;
#if 0//(REREAD_MODE == ENABLE)
    if(Current_Index == NORMAL_MODE_INDEX){
        return NORMAL_MODE_INDEX;
        }
    if(Current_Index == REREAD_MODE_INDEX){
        return REREAD_MODE_INDEX;
        }
#endif  
  temp_off = all_off%256;
  GetDataIdInfo( D_OID, &Start_Sector, &Length );
  temp_sector = Start_Sector +(U32)(all_off >> 8);
  SPI_STATE = spif_resv_read( temp_sector, ( __X void *)Buf );
  if( SPI_STATE != SDTL_No_Err ) return 0xFFFF;
  return p[temp_off];  
}

void scan_oid( void )
{
   //extern Bool g_oid;
   //extern Bool s_b_batt_evt_low_sent;
   //extern Bool s_b_vbus_present;
   U32 data_from_pen;
   U16 oid_group;
   if( u32_oid_read_time && !Timer_is_timeout( u32_oid_read_time ))
      return;
   u32_oid_read_time = 0;
   
   //sdio_in();
   if( !read_sdio())
   {
      //Int_disable();

      data_from_pen = read_oid_pen();
     // Int_enable();
     //SendString((U8 *)"\r\ndata_from_pen=");
     //send_hex_u32(data_from_pen);
      u32_oid_read_time = Timer_set_timer( TIMER_MS( 30 ));
#if( T01OID == ENABLE)	  
   if( data_from_pen == OIDCmd_SystemReset )
   {
    clkup_time(true);
    SDKLoadCode(SET_OID_COMMAND);
   	set_pam_after_reset();
   /*
     set_t01_mode(UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2,OIDCmd_SetMode_Ack);
   	 set_t01_mode(UserCmd_SetGroup, UserCmd_SetGroupParam1, UserCmd_SetGroupParam2,OIDCmd_SetGroup_Ack);
      write_oid_pen( 0x24 );
      write_oid_pen( UserCmd_PowerOnOID );
      write_oid_pen( UserCmd_AutoSleepFunDisable );
#if( T01OID_CAB == ENABLE )	  
      set_calib_data();
#endif //	  
*/
   }     
#endif //#if( T01OID == ENABLE) 
      if( data_from_pen & 0x400000 )
      {
         if( !( data_from_pen & 0x200000 ))
         {
#if 0
            //b_oid_pen = TRUE;
            if( data_from_pen & 0x100000 )
            {
               SendString("high\r\n");
               //s_b_batt_evt_low_sent = FALSE;
               //oid_battery_low_counter = 0;
               //b_oid_bat_lower = FALSE;
            }
            else
            {
               SendString("low\r\n");
               //b_oid_bat_lower = TRUE;
               /*if( !s_b_vbus_present && !s_b_batt_evt_low_sent )
               {
                  //if( !g_b_battery_low )
                  {
                     oid_battery_low_counter++;
                     if( oid_battery_low_counter >= 3 )
                     {
                        oid_battery_low_counter = 0;
                        s_b_batt_evt_low_sent = TRUE;
                        // Event "EVT_BATTERY_ALERT_LEVEL"
                        mail_send_event( EVT_BATTERY_LEVEL_ALERT, 0 );
#if 0
                        g_b_battery_low = TRUE;
                        red_led_on();
                        green_led_off();
                        red_led_start_timer( RED_LED_ON_BATTERY_LEVEL_LOW_TIME );
                        batt_low_warning_timer = Timer_set_timer( TIMER_MIN( 3 ));
#endif
                     }
                  }
               }*/
            }
#endif
#if( T01OID == ENABLE)	
			oid_group = ( U16 )(( data_from_pen >> 18 )& 0x03 );
#endif //#if( T01OID == ENABLE) 

            data_from_pen &= 0x3FFFF;
            if( data_from_pen == OID_IndexType_Normal_DontCare )
               return;
            if(( data_from_pen >= OID_IndexType_Normal_Missing_Min )
               &&( data_from_pen <= OID_IndexType_Normal_Missing_Max ))
            {
#if 0//( PASTER == ENABLE )
               if( g_b_paster_on )
                  u16_current_index = 0;
#endif
#if 0//( DIC == ENABLE )
               if( g_b_dic_on )
                  u16_current_index = 0;
#endif
               //bPenOn = FALSE;
               bPenLeave = TRUE;
               return;
            }
#if 0//( T01OID == ENABLE)
			    //if( oid_group != OID__NORMAL_GROUP )
            if( oid_group > OID_WAIYAN_GROUP )
               return;
#endif //#if( T01OID == ENABLE) 
			
            if( data_from_pen <= 0xFFFF )
            {
               /*if( bPenLeave )
               {
                  green_led_on();
                  green_led_start_timer( GREEN_LED_ON_READ_TIME );
               }
               bPenLeave = FALSE;*/
               u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 100 ));
#if (POWER_OFF_AUTO == ENABLE)
               power_rearm_autopoweroff_delay();
#endif   //#if (POWER_OFF_AUTO == ENABLE)
               if( u16_current_index == data_from_pen )
               {
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     //if( g_b_play_ap3 ||( Point_Write != Point_Read ))
                     /*if( !bSpeechEnd )
                        return;*/
                  }
#endif
#if 0//( PASTER == ENABLE )
                  if( g_b_paster_on )
                     return;
#endif
#if 0//( DIC == ENABLE )
                  if( g_b_dic_on )
                     return;
#endif
               }
               else
               {
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
               u16_current_index = ( U16 )data_from_pen;
			    if(g_setting_list.struct_setting.bluetooth){
				//(void)set_t01_one_mode(OID_SONIX_2_COMMAND,OID_SONIX_2_COMMAND_ACK);
				u16_current_index = chang_oid( u16_current_index );
		   		}
               ///*
               //SendString((U8 *)"\r\nindex=");
               //send_hex_u16(u16_current_index);
			   //*/
               //if( bPenLeave )
               {
#if 0//( AP3 == ENABLE )
                  if( g_b_search_on )
                  {
                     GameTimeOverTick = Timer_set_timer( TIMER_S( Current_GameNews.OverTime_Game ));
                     GameTipTick = Timer_set_timer( TIMER_S( Current_GameNews.Time ));
                     if( scan_oid_is_enable())
                     {
                        if( !bRemoteChangeAp3 )
                        {
                           Search_Record.news_sys.flag_index = 1;
                           //Current_Index = u16_current_index;
                           //if( player_ap3_is_busy())
                              //player_ap3_stop();
                        }
                     }
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#if 0//(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
                  else if( g_b_remote_on )
                  {
                     if( scan_oid_is_enable())
                        Search_Record.news_sys.flag_index = 1;
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif   //#if( AP3 == ENABLE )
                  //if( g_b_remote_on )
                  {
                    b_readySysUP = true;
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                  }

#if 0//( PASTER == ENABLE )
                  if( g_b_paster_on )
                  {
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                     //paster_send_cmd( CMD_PASTER_OID, u16_current_index );
                  }
#endif
#if 0//( DIC == ENABLE )
                  if( g_b_dic_on )
                  {
                     if( scan_oid_is_enable())
                        g_oid = TRUE;
                     else
                        g_oid = FALSE;
                  }
#endif
                  bPenLeave = FALSE;
                  if( green_led_is_timer_stopped())
                     green_led_start_timer( GREEN_LED_ON_READ_TIME );
               }
            }
         }
         else
         {
#if 0//( PASTER == ENABLE )
            if( g_b_paster_on )
               u16_current_index = 0;
#endif
#if 0//( DIC == ENABLE )
            if( g_b_dic_on )
               u16_current_index = 0;
#endif
            bPenLeave = TRUE;
         }
      }
      else
      {
#if 0//( PASTER == ENABLE )
         if( g_b_paster_on )
            u16_current_index = 0;
#endif
#if 0//( DIC == ENABLE )
         if( g_b_dic_on )
            u16_current_index = 0;
#endif
         bPenLeave = TRUE;
      }
      //Int_enable();

      if( exit_flag )
      {
         oid_pen_init();
         exit_flag = FALSE;
      }
   }

scan_oid_exit:
#if 1
   //if( bPenOn )
   {
      if( u32_oid_read_timeout && Timer_is_timeout( u32_oid_read_timeout )) //50 * 10ms
      {
#if 0//( PASTER == ENABLE )
         if( g_b_paster_on )
            u16_current_index = 0;
#endif
#if 0//( DIC == ENABLE )
         if( g_b_dic_on )
            u16_current_index = 0;
#endif

         u32_oid_read_timeout = Timer_set_timer( TIMER_MS( 100 ));
         //bPenOn = FALSE;
         //bPenLeave = TRUE;
      }
   }
#endif
}
#endif
#endif  //#if(( PASTER == ENABLE )||( AP3 == ENABLE )||( DIC == ENABLE ))

#endif  // #if( OID == ENABLE )
//! @brief delay1ms
//!
//! To delay time 1ms
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param delay time
//! @return none
//!
/*
void delay1ms( U8 delay_time )
{

   U8 i,j;
   do
   {
      j = 36;
      do
      {
         i = 140;
         while( --i );
      }
      while( --j );
   }
   while( --delay_time );
}
*/
__asm{
_delay1ms::
    LINK #0
    PUSH R0
    PUSH R1

    MOV R0,[FP+#(6)]
$Loop2:
    MOV R1,#12000 ;// for fsys = 48Mhz
$Loop:
    CMPD1 R1,#0
    AJMP IF_NE,$Loop
    CMPD1 R0,#0
    AJMP IF_NE,$Loop2
    POP R1
    POP R0
    UNLINK_MACRO
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

#endif  // _oid_c_
