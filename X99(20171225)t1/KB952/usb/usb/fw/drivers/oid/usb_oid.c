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

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

Bool  g_b_oid_enable = FALSE;
Bool  bPenLeave = TRUE;

//_MEM_TYPE_MEDSLOW_   U32   u32_i2c_timeout = 0;

#if( OID == ENABLE )

#if 1
//Bool rw_flag = 0; //read/write flag
#if( OID_VERSION == OID1 )
Bool bPenOn = FALSE;
#endif
Bool exit_flag = FALSE; //exit main flag

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

//Bool  b_oid_init_flag = FALSE;//

_MEM_TYPE_MEDSLOW_   U32   u32_oid_read_timeout/* = 0*/;

//Bool  bPenLeave = TRUE;

Bool  g_oid = FALSE;

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

#if( OID == ENABLE )
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
         timer = Timer_set_timer( TIMER_MS( 100 ));
         while( !Timer_is_timeout( timer ));
         oid_power_on();
         pen_enable();
         timer = Timer_set_timer( TIMER_MS( 100 ));
         while( !Timer_is_timeout( timer ));
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
   delay1ms( 5 );
   write_oid_pen( UserCmd_PowerOnOID );
   write_oid_pen( UserCmd_AutoSleepFunDisable );

   SendString((U8 *)"init ok\r\n");
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
   //≥ı ºªØOID pen
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
      sdio_in();
      delay3us();
      clr_sck();
      delay3us();
	   //SDIO = 1;
      sdio_in();
      if( read_sdio())
         from_pen_data |= 0x01;
   }
   delay100us();  //delay 100us
   if( from_pen_data == OIDCmd_PowerDown )
      exit_flag = TRUE;
   return from_pen_data;
}
#endif

#if 0
void write_t01_cmd( U16 val1, U16 val2, U16 val3 )
{
   if( !read_sdio())
      read_oid_pen();

   clr_sck();
   sdio_in();
   delay10us();
   set_sck();
   delay10us();
   clr_sck();
   delay10us();
   write_t01_pen( val1 );
   write_t01_pen( val2 );
   write_t01_pen( val3 );
   delay100us();
   oid_sdio_input();
   sdio_in();
}


void write_t01_cmd1( U16 val )
{
   if( !read_sdio())
      read_oid_pen();

   clr_sck();
   sdio_in();
   delay10us();
   set_sck();
   delay10us();
   clr_sck();
   delay10us();
   write_t01_pen( val );
   delay100us();
   oid_sdio_input();
   sdio_in();
}


//! @brief write_oid_pen
//!
//! To write data to oid pen device
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param data for write to oid pen device
//! @return none
//!
void write_t01_pen( U16 val )
{
   U16 count;

//   if( !read_sdio())
//      read_oid_pen();

   for( count = 0; count < 16; count++ )
   {
      set_sck();
      //delay10us(); //delay 10us
		
      if( val & 0x8000 )
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
		
      delay10us(); //delay 20us
      clr_sck();
      delay10us();
   }

   //rw_flag = 0;  //clear write flag
   oid_sdio_input();
   //sdio_in();
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
   U8 count;

   if( !read_sdio())
      read_oid_pen();

   clr_sck();
   sdio_in();
   delay10us();
   set_sck();
   delay10us();
   clr_sck();
   delay10us();
   for( count = 0; count < 8; count++ )
   {
      set_sck();
      //delay10us(); //delay 10us
		
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
		
      delay10us(); //delay 20us
      clr_sck();
	  delay10us();
   }
   delay100us();  //delay 100us

   //rw_flag = 0;  //clear write flag
   oid_sdio_input();
   sdio_in();
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
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
                  else if( g_b_remote_on )
                  {
                     if( scan_oid_is_enable())
                        Search_Record.news_sys.flag_index = 1;
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif   //#if( AP3 == ENABLE )
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

         if( !green_led_is_timer_stopped())
         {
            green_led_stop_timer();
            green_led_on();
         }
      }
   }
}

#elif( OID_VERSION == OID2 )
void scan_oid( void )
{
   //extern Bool g_oid;
   //extern Bool s_b_batt_evt_low_sent;
   //extern Bool s_b_vbus_present;
   U32 data_from_pen;

   if( u32_oid_read_time && !Timer_is_timeout( u32_oid_read_time ))
      return;
   u32_oid_read_time = 0;
   
   //sdio_in();
   if( !read_sdio())
   {
      //Int_disable();

      data_from_pen = read_oid_pen();
      //Int_enable();
      u32_oid_read_time = Timer_set_timer( TIMER_MS( 30 ));
      
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
               SendString((U8 *)"index=");
               send_hex_u16(u16_current_index);
               SendString((U8 *)"\r\n");
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
#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
                  else if( g_b_remote_on )
                  {
                     if( scan_oid_is_enable())
                        Search_Record.news_sys.flag_index = 1;
                     else
                        Search_Record.news_sys.flag_index = 0;
                  }
#endif   //#if(( REMOTE_MP3 == ENABLE )||( REMOTE_DIC == ENABLE )||( REMOTE_PASTER == ENABLE )||( REMOTE_REC == ENABLE ))
#endif   //#if( AP3 == ENABLE )

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
                  //if( green_led_is_timer_stopped())
                  //   green_led_start_timer( GREEN_LED_ON_READ_TIME );
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

         if( !green_led_is_timer_stopped())
         {
            green_led_stop_timer();
            green_led_on();
         }
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
void delay1ms( U16 delay_time )
{
   /*U8 i,j;
   do
   {
      j = 33;
      do
      {
         i = 38;
         while( --i );
      }
      while( --j );
   }
   while( --delay_time );*/
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
#endif

#if 0
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
   OPARPowerOff();
   b_oid_init_flag = FALSE;
}
#endif
#endif   //#if( OID == ENABLE )


#endif  // _oid_c_
