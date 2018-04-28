
#include "stdio.h"
#include "lib_mcu\timer\timer_drv.h"
#include "lib_mcu\int\int_drv.h"
#include "compiler.h"
#include "sio\com.h"
#include "modules\player\player_task.h"
#include "mmi\kbd\keypad.h"
//#include "modules\search\search_task.h"
#include "drivers\oid\oid.h"
#include "modules\power\power_task.h"
#include "modules\scheduler\scheduler.h"
#include "mmi\shared\com_var.h"

//#include "modules\usb\usb_task_sch.h"
#include "modules\power\power.h"             // Power module definition
//#include "mmi_ap3.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "string.h"

#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE
//#include "drivers\mmi\img_handler.h"
#include "adc.h"

#pragma DATA = MAIN_DATA
#pragma CODE = MAIN_CODE

ImageDeclareType( TASK_INIT_CODE );


const unsigned int otpdata128_test[ 8 ] = { 0x0001u, 0x5555u, 0x4321u, 0x0800u, 0x9527u, 0x0097u, 0xFFFFu, 0xB458u };
//const unsigned int otpdata128_test_2[ 8 ] = { 0xFFFEu, 0xFFF0u, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu };

extern   int   OtpVerify128Bit   ( __X unsigned int *ptr );

void  InitAfayaPort     ( void );
/*
#ifdef  TX_DEBUG
void  InitUart          ( void );
#else
#define InitUart()
#endif
*/



void __interrupt Exception( void )
{
   //RESET_SYS();
   //while( 1 );
   RTCWDT = 0xBFFFu;
}


Bool otpverify( void )
{
   //err = OtpVerify128Bit( otpdata128 );
   __X unsigned int temp_sec[ 8 ];
   temp_sec[ 0 ] = otpdata128_test[ 0 ] + 0xDFFFu;
   temp_sec[ 1 ] = ~otpdata128_test[ 1 ];
   temp_sec[ 2 ] = otpdata128_test[ 2 ] + 0x6569u;
   //temp_sec[ 3 ] = otpdata128_test[ 3 ] & 0x2012;
   temp_sec[ 3 ] = otpdata128_test[ 3 ];
   temp_sec[ 4 ] = otpdata128_test[ 4 ] - 0x79D1u;
   temp_sec[ 5 ] = otpdata128_test[ 5 ] + 0x8919u;
   temp_sec[ 6 ] = otpdata128_test[ 6 ] % 3;
   temp_sec[ 7 ] = otpdata128_test[ 7 ] - 0x29E0u;
   if( !OtpVerify128Bit( temp_sec ))
      return TRUE;
	return FALSE;
	//Return 0 , Verify 128Bits OK
	//Return -1 , Verify 128Bits Error
	//Return -2 , FUSCON bit 12 Error
	//Return -3 , OTPVHH Pin >= 6.5v
}

void check_floating(void)
{
	  g_b_V1Hard_Flag = TRUE;

	  
	  //_bitCLR( CHECK_DET_DIR );//input
	 // _bitSET( CHECK_DET_PH );//set ph
      //_bitSET( RED_LED );
	  _bitSET( CHECK_DET_PH );//set ph
	  _bitSET( CHECK_DET_DIR );//input
	  delay1ms(30);
	  if(CHECK_READ_FLAG()){
	  	SendString((U8 *)"\r\nPB13 heigh!");
		//temp_1_flag = FALSE;
		//MSB(TempBuf[0]) = HARD_V2_VERSION

			_bitCLR( CHECK_DET_PH );//clr ph
	  		_bitSET( CHECK_DET_DIR );//input
	  		delay1ms(30);
			if(!CHECK_READ_FLAG()){
			 	SendString((U8 *)"\r\nPB13 low!");
				g_b_V1Hard_Flag = FALSE;
			  }
	  	}
	  
	if(g_b_V1Hard_Flag){
			SendString((U8 *)"\r\nv1 headphone!");
			_bitSET( HP_V1_DET_DIR );
			_bitSET( HP_V1_DET_PH );
			}
		else{
			SendString((U8 *)"\r\nv2 headphone!");
			_bitSET( HP_V2_DET_DIR );
			_bitSET( HP_V2_DET_PH );
			}

}

void main( void )
{
   U8 key_power_press_time = 30;
   U16 bat_val,temp_val;
   U16 adc7_temp;
   U8 i = 0;

   // Since the SD, NAND flash and 8088 interface use the same I/O pins, 
   // if both SD card and NAND Flash are used, or both SD card and 8088 interface are used in the system
   // this function must be executed to avoid the competition of I/O pins.   
   SetVector( SD_DELAY_CALLBACK_TRAP, SD_Busy_Check );

   IP_INIT();
   __asm{ BCLR USBCON.14 };
   
#if 1
   if( !otpverify())
   {
      RESET_SYS();
   }
#endif
   Int_disable();
   
   SystemClock( SYSTEM_CLOCK );
   SDKLoadCodInit();
   //SetVector( EXCEPTION_TRAP, Exception );
   InitAfayaPort();
   //ADC_init();
   InitUart();

   //PWR_OFF();
   EXT_DC_PWR_OFF();

  // if( power_is_vbus_present())
   if(0)
   {
      do
      {
         delay1ms( 20 );
         if( !power_is_vbus_present())
            goto battery_power_next;
      }
      while( --key_power_press_time );
      if( !power_is_vbus_present())
         goto battery_power_next;
	  
      SendString((U8 *)"\r\nusb power");
      //PWR_OFF();   	     
      //EXT_DC_PWR_OFF();
   }
   else
   {
battery_power_next:
      key_power_press_time = 25;
      SendString((U8 *)"\r\nbat power");
      SDKLoadCode( TASK_INIT_CODE );
      ADC_init();
      do
      {
         //adc2_value = Get_ADCValue( 0 );
         delay1ms( 15 );
         adc7_value =  Get_ADCValue( 7 );
		 delay1ms( 15 );
         adc3_value = Get_ADCValue( 3 );
         delay1ms( 15 );
         temp_val = Get_ADCValue( 0 );
  		 delay1ms( 15 );
         bat_val = Get_ADCValue( 2 );
		 adc2_value = bat_val;

		// if()
		 
		 if(key_power_press_time%5 == 0){
			 SendString((U8 *)"\r\nadc0:2:3:7=");
			 send_hex_u16(temp_val);SendString((U8 *)":");
			 send_hex_u16(adc2_value);SendString((U8 *)":");
			  send_hex_u16(adc3_value);SendString((U8 *)":");
			 send_hex_u16(adc7_value);
		 	}
		/*
		 if((adc3_value >= SW3_MIN)&&(adc3_value <= SW3_MAX)){
		 	g_b_V1Hard_Flag = TRUE;
		 	}

		  if((adc3_value >= (SW3_MIN-Power_Offset))&&(adc3_value <= (SW3_MAX-Power_Offset))){
		 	g_b_V1Hard_Flag = FALSE;
		 	}
		// */
		 battery_value = (U16)((temp_val*100UL)/adc7_value);
//         SendString((U8 *)"bat=");
//         send_hex_u16(bat_val);
//         SendString((U8 *)"\r\n");
         if( battery_value < BVT_VALUE )
         {
            i++;
            if( i >= 5 )
            {
               //__asm( "reset" );
               i = 0;
               RESET_SYS();
            }
         }
         else
         {
            i = 0;
         }
//         delay1ms( 15 );
//         bat_val = Get_ADCValue( 2 );
//		 adc2_value = bat_val;
         if( bat_val > 100 )
         //if( !key_power_is_pressed())
         {
            /*P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
            //Reset player now
            (*(void(*)())0)();*/
            //power_system_off( FALSE );
            //RESET_SYS();

			
            b_battery_charging = TRUE;
			if(key_power_press_time%5 == 0){
				SendString((U8 *)"\r\nb_battery_charging = TRUE");
				}
			
         }
		 else{
		 	b_battery_charging = FALSE;
			if(key_power_press_time%5 == 0){
				SendString((U8 *)"\r\nb_battery_charging = FALSE");
				}
		 	}
      }
      while( --key_power_press_time );
      //PWR_ON();
      //EXT_DC_PWR_ON();
   }
   PWR_ON();
   EXT_DC_PWR_ON();

   EXT_PWR_CTL_ON();
   green_led_on();
   check_floating();
   SendString((U8 *)"\r\nKB startup");

   Int_enable();

   //nav_reset();
   // Endless scheduler execution
   scheduler();
}

#if 0
SROM_PS( EXEINRAM_ISP )    // Define SROM variables
void copy_ISPcode_SRAM( void )
{
#if 0
   unsigned char xdata *pChar;
   unsigned char volatile code *pCode;

   //== copy ISP Code to RAM 0xB400
   pCode =( unsigned char volatile code *)SROM_PS_SRC( EXEINRAM_ISP );
   pChar = ( unsigned char xdata *)SROM_PS_TRG( EXEINRAM_ISP ); 
#endif
   memcpy(( unsigned char xdata *)SROM_PS_TRG( EXEINRAM_ISP ), ( unsigned char volatile code *)SROM_PS_SRC( EXEINRAM_ISP ), SROM_PS_LEN( EXEINRAM_ISP ));
}
#endif

void InitAfayaPort( void )
{
#if 1
   PBEN &= 0x0Fu;
   PCEN = 0x00;

   //_bitSET( HP_DET_DIR );
   //_bitSET( HP_DET_PH );

 //  _bitCLR( EX_RC_EN_DIR);
   //_bitSET( RED_LED );
//   red_led_off();
//   _bitCLR( RED_LED_DIR );
/*
   if(g_b_V1Hard_Flag){
		   SendString((U8 *)"\r\nv1 headphone!");
		   _bitSET( HP_V1_DET_DIR );
		   _bitSET( HP_V1_DET_PH );
		   }
	   else{
		   SendString((U8 *)"\r\nv2 headphone!");
		   _bitSET( HP_V2_DET_DIR );
		   _bitSET( HP_V2_DET_PH );
		   }
*/
   //_bitCLR( P_EN );
   pen_disable();
   _bitCLR( P_EN_DIR );

   _bitSET( SDIO_DIR );
   _bitSET( SDIO_PH );

   //_bitCLR( SCK );
   clr_sck();
   _bitCLR( SCK_DIR );

   _bitSET( BAT_nEOC_DIR );
   _bitSET( BAT_nEOC_PH );

   //_bitCLR( PWRON );
   PWR_OFF();
   _bitCLR( PWRON_DIR );

   sd_det_disable();
   _bitCLR( SD_DET_PIF );
   PBICR = 0x00;
   _bitSET( SD_DET_DIR );
  // _bitSET( SD_DET_PH );
  	_bitCLR( SD_DET_PH );

   //_bitCLR( LCD_nON );
   Lcd_power_switch_off();
   _bitCLR( LCD_nON_DIR );

   //_bitSET( GREEN_LED );
   green_led_off();
   _bitCLR( GREEN_LED_DIR );

   //_bitSET( KBD_PWR_DIR );
   //_bitSET( KBD_PWR_PH );
#else
   PBOD = 0x3030;
   PBDIR = 0xCAAF;
   PBPH = 0xFFFF;
#endif

#if 1
   //_bitCLR( OP_CTL );
   op_disable();
   _bitCLR( OP_CTL_DIR );

   //_bitCLR( LCD_nRST );
   Lcd_reset_on();
   _bitCLR( LCD_nRST_DIR );

 //  _bitSET( USB_DET_DIR );
//   _bitCLR( USB_DET_PH );
#else
   PCOD = 0x0000;
   PCDIR = 0x05FD;
   PCPH = 0x07FF;
#endif

   /*PDOD = 0x0000;
   PDDIR = 0x00FF;
   PDPH = 0x00FF;*/

}


