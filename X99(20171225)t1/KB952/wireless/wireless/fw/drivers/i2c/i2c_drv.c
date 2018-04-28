
#ifndef _i2c_drv_c_
#define _i2c_drv_c_

#include "lib_mcu\compiler.h" 
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "sio\com.h"
#include "i2c_drv.h"
#include "mmi\kbd\keypad.h"
#include "timer_drv.h"
#include "string.h"


#pragma DATA = I2C_DRV_DATA
#pragma CODE = I2C_DRV_CODE


void                 i2c_init          ( void );
void                 i2c_slave_init    ( void );

void  __interrupt    i2c_isr           ( void );

U32   i2c_slave_timer;
S16   i2c_num;
U8    i2c_data[ I2C_DATA_MAX_LEN ];
U8    i2c_tx_buf[ I2C_DATA_MAX_LEN ];
U8    i2c_count;
Bool  rx_flag, tx_flag;
S16   i2c_rf_count;

U8    *i2c_data_point;
U8    i2c_rf_state;

U16   i2c_data_packet_count;

void i2c_init( void )
{
   U8 i;

   i2c_data_packet_count = 0;
   
   i2c_rf_count = -1;
   
   SetVector( I2C_INT_TRAP, i2c_isr );
  
   _bitSET( _PBEN_b0 + 8 );   // I2C_SCL pin GPIO disable
   _bitSET( _PBEN_b0 + 9 );   // I2C_SDA pin GPIO disable
   _bitCLR( _PBODN_b0 + 8 );  // I2C_SCL pin open drain enable
   _bitCLR( _PBODN_b0 + 9 );  // I2C_SDA pin open drain enable
   PBPH = 0x30;
   //_bitCLR( _PBOD_b0 + 8 );
   //_bitCLR( _PBDIR_b0 + 8 );
   
   __asm{ bset i2cicr.5 bset	i2cicr.4 bset i2cicr.0 }
   I2CSAR = 0xE0;

   SendString((U8 *)"I2C slave\r\n");

   /*for( i = 0; i < I2C_DATA_MAX_LEN; i++ )
   {
      i2c_tx_buf[ i ] = i+1;
      i2c_data[ i ] = 0;
   }*/

   i2c_slave_init();

   //i2c_data[ 1 ] = 0x10;
}


void i2c_slave_init( void )
{
   unsigned int i;
   U32 bus_free_timer;

   //_bitSET( _PBDIR_b0 + 8 );
   //I2CCR = 0xFC18;
   //bus_free_timer = Timer_set_timer( TIMER_MS( 1 ));
   //while( !Timer_is_timeout( bus_free_timer ));
   
   I2CSAR = 0xE0;
   I2CCR = 0xFC00 | I2C_SPEED;

   i2c_num = -1;
   i2c_count = 0;

   rx_flag = 0;
   tx_flag = 0;

   //i2c_slave_timer = Timer_set_timer( I2C_INIT_TIME );
   i = 20;
   while( 1 )
   {
      //_bitSET( _PBDIR_b0 + 8 );
      
      if(( PBID & 0x0300 )!= 0x0300 )
      {
         I2CCR = 0xFC18 | I2C_SPEED;
         //SendString((U8 *)"slave bus free\r\n");
         bus_free_timer = Timer_set_timer( TIMER_MS( 4 ));
         while( !Timer_is_timeout( bus_free_timer ));
         I2CCR = 0xFC00 | I2C_SPEED;
         //bus_free_timer = Timer_set_timer( TIMER_MS( 5 ));
         //while( !Timer_is_timeout( bus_free_timer ));
      }
      else
         return;

      i--;
      if( !i )
      {
         //SendString((U8 *)"bus error\r\n");
         return;
      }
   }
}


void __interrupt i2c_isr( void )
{
   _bitCLR( _I2CCR_b0 + 0 );

   if( _bit_get( _I2CCR_b0 + 9 ))
   {
      if( !_bit_get( _I2CCR_b0 + 7 ))
      {
         //I2CTDR = i2c_data[ i2c_count++ ];
         I2CTDR = i2c_tx_buf[ i2c_count++ ];
         if( i2c_count >= I2C_DATA_MAX_LEN )
            i2c_count = 0;
      }
      else
      {
         I2CCR = 0xFC18 | I2C_SPEED;
         tx_flag = 1;
      }
   }
   else
   {
      i2c_slave_timer = Timer_set_timer( I2C_INIT_TIME );
      _bitCLR( _I2CCR_b0 + 8 );

      if( i2c_num == -1 )
      {
         i2c_num = I2CRDR;
         I2CCR = 0xFC00 | I2C_SPEED;
         if( i2c_num == 1 )
         {
            //_bitSET( _I2CCR_b0 + 5 );
            I2CCR = 0xFC18 | I2C_SPEED;
         }
         i2c_data[ i2c_count++ ] = i2c_num;
         return;
      }

      i2c_num--;
      if( i2c_num == 0 )
      {
         _bitCLR( _I2CCR_b0 + 5 );
         I2CCR = 0xFC18 | I2C_SPEED;
         
         i2c_data[ i2c_count++ ] = I2CRDR;

         rx_flag = 1;
         
         //__asm{ bclr	i2cicr.0 }
      }
      else if( i2c_num == 1 )
      {
         i2c_data[ i2c_count++ ] = I2CRDR;
         _bitSET( _I2CCR_b0 + 5 );
      }
      else
      {
         i2c_data[ i2c_count++ ] = I2CRDR;
      }
      if( i2c_count >= I2C_DATA_MAX_LEN )
         i2c_count = 0;
   }
}


void i2c_task( void )
{
   U8 i;
   
   if( rx_flag == 1 )
   {
      //_bitCLR( _PBOD_b0 + 8 );
      //_bitCLR( _PBDIR_b0 + 8 );
      //I2CCR = 0xFC10;
      i2c_slave_timer = Timer_set_timer( I2C_INIT_TIME );
      I2CCR = 0xFC18 | I2C_SPEED;
      
      rx_flag = 0;
      __asm{ bclr	i2cicr.0 }

      /*SendString((U8 *)"i2c_data=");
      for( i = 0; i <= i2c_data[ 0 ]; i++ )
      {
         send_hex_u8( i2c_data[ i ]);
         SendString((U8 *)" ");
      }
      SendString((U8 *)"\r\n");*/
      
      /*MSB0( g_i2c_rx_data ) = i2c_data[ 1 ];
      MSB1( g_i2c_rx_data ) = i2c_data[ 2 ];
      MSB2( g_i2c_rx_data ) = i2c_data[ 3 ];
      MSB3( g_i2c_rx_data ) = i2c_data[ 4 ];

      MSB( g_i2c_rx_data_ex0 ) = i2c_data[ 5 ];
      LSB( g_i2c_rx_data_ex0 ) = i2c_data[ 6 ];*/
      
      //i2c_data_point = &i2c_data[ 1 ];
      /*i2c_rf_count = 0;

      //MSB0( g_i2c_rx_data ) = i2c_data[ 0 ];
      MSB0( g_i2c_rx_data ) = *i2c_data_point++;
      MSB1( g_i2c_rx_data ) = *i2c_data_point++;
      MSB2( g_i2c_rx_data ) = *i2c_data_point++;
      MSB3( g_i2c_rx_data ) = *i2c_data_point++;

      MSB( g_i2c_rx_data_ex0 ) = *i2c_data_point++;
      LSB( g_i2c_rx_data_ex0 ) = *i2c_data_point++;

      //i2c_rf_count += 6;
      i2c_rf_count += 5;*/
      
      //g_b_i2c_flag = TRUE;
      //packet_counter++;
      /*SendString((U8 *)"g_i2c_rx_data=");
      send_hex_u32(g_i2c_rx_data);
      SendString((U8 *)"\r\n");
      SendString((U8 *)"g_i2c_rx_data_ex0=");
      send_hex_u16(g_i2c_rx_data_ex0);
      SendString((U8 *)"\r\n");*/

      i2c_rf_count = -1;
      
      if(( i2c_data[ 0 ]>= 3 )
         /*&&( i2c_data[ 0 ] ==( i2c_data[ 1 ] + 1 ))*/)
      {
         memcpy( i2c_tx_buf, i2c_data, I2C_DATA_MAX_LEN );
         i2c_data_point = &i2c_tx_buf[ 1 ];
         i2c_rf_state = 0;
         g_b_i2c_flag = TRUE;

         i2c_rf_count = 13;

         i2c_data_packet_count++;
      }

//i2c_next:
      i2c_slave_init();
      //__asm{ bset	i2cicr.0 }

      //_bitCLR( _PBOD_b0 + 8 );
      //_bitCLR( _PBDIR_b0 + 8 );
      //I2CCR = 0x00;
   }
   else if( tx_flag == 1 )
   {
      __asm{ bclr	i2cicr.0 }
      
      I2CCR = 0xFC18 | I2C_SPEED;
      tx_flag = 0;
      /*SendString((U8 *)"slave tx\r\n");
      SendString((U8 *)"i2c_count=");
      send_hex_u8(i2c_count);
      SendString((U8 *)"\r\n");*/
      i2c_slave_init();

      __asm{ bset	i2cicr.0 }
   }

//i2c_next:
   if( i2c_slave_timer && Timer_is_timeout( i2c_slave_timer ))
   {
      //g_b_kbd_data_flag = TRUE;
      
      i2c_slave_timer = Timer_set_timer( I2C_INIT_TIME );
      //if( i2c_rf_count == -1 )
         i2c_slave_init();
   }
   if( g_b_i2c_flag == FALSE )
   {
      //i2c_slave_init();
      //__asm{ bset	i2cicr.0 }

      if( i2c_rf_count != -1 )
      {
         if( i2c_rf_count < i2c_tx_buf[ 1 ])
         {
            //p = &i2c_data[ i2c_rf_count + 1 ];

            /*MSB0( g_i2c_rx_data ) = i2c_data[ 1 ]- i2c_rf_count;
            //MSB0( g_i2c_rx_data ) = *p++;
            MSB1( g_i2c_rx_data ) = *i2c_data_point++;
            MSB2( g_i2c_rx_data ) = *i2c_data_point++;
            MSB3( g_i2c_rx_data ) = *i2c_data_point++;

            MSB( g_i2c_rx_data_ex0 ) = *i2c_data_point++;
            LSB( g_i2c_rx_data_ex0 ) = *i2c_data_point++;

            //i2c_rf_count += 6;
            i2c_rf_count += 5;*/
            //i2c_rf_state++;
            i2c_rf_count += 13;
            if( i2c_rf_count < i2c_tx_buf[ 1 ])
               i2c_rf_state = 1;
            else
               i2c_rf_state = 2;
            
            //g_b_i2c_flag = TRUE;
            //packet_counter++;
            /*SendString((U8 *)"g_i2c_rx_data=");
            send_hex_u32(g_i2c_rx_data);
            SendString((U8 *)"\r\n");
            SendString((U8 *)"g_i2c_rx_data_ex0=");
            send_hex_u16(g_i2c_rx_data_ex0);
            SendString((U8 *)"\r\n");*/

            /*if(( i2c_data[ 0 ]>= 2 )
               &&( i2c_data[ 0 ] ==( i2c_data[ 1 ]+ 1 )))*/
               g_b_i2c_flag = TRUE;
         }
         else
         {
            /*for( i = 0; i < I2C_DATA_MAX_LEN; i++ )
            {
               i2c_data[ i ] = 0;
            }*/
            i2c_rf_count = -1;

            //_bitSET( _PBOD_b0 + 8 );
            //_bitSET( _PBDIR_b0 + 8 );
            //I2CCR = 0x00;

            //i2c_slave_init();
            __asm{ bset	i2cicr.0 }
         }
      }
   }

   //g_b_i2c_flag = TRUE;
   //packet_counter++;
   //if( i2c_rf_count == -1 )
   //   rx_flag = 1;
}


#endif   //_i2c_drv_c_
