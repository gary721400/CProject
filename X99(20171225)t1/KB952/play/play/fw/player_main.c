
#include "stdio.h"
//#include "lib_mcu\timer\timer_drv.h"
#include "lib_mcu\int\int_drv.h"
#include "compiler.h"
#include "sio\com.h"
//#include "modules\player\player_task.h"
//#include "mmi\kbd\keypad.h"
//#include "modules\search\search_task.h"
#include "drivers\oid\oid.h"
//#include "modules\power\power_task.h"
#include "modules\scheduler\scheduler.h"
//#include "mmi\shared\com_var.h"

//#include "modules\usb\usb_task_sch.h"
#include "modules\power\power.h"             // Power module definition
//#include "mmi_ap3.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "string.h"

#pragma DATA = MAIN_DATA
#pragma CODE = MAIN_CODE

//const unsigned int otpdata128_test[ 8 ] = { 0x0001u, 0x5555u, 0x4321u, 0x0800u, 0x9527u, 0x0097u, 0xFFFFu, 0xB458u };
//const unsigned int otpdata128_test_2[ 8 ] = { 0xFFFEu, 0xFFF0u, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu };

//extern   int   OtpVerify128Bit   ( __X unsigned int *ptr );

void  InitAfayaPort     ( void );
/*
#ifdef  TX_DEBUG
void  InitUart          ( void );
#else
#define InitUart()
#endif
*/
#if 0//def  TX_DEBUG
void InitUart( void )
{
   UARBR = 0x0002u;  //115200 bps
   UARCR = 0x8001;   // disable parity, one stop bit, 8 bits per character
   PBEN |= 0x0010;
}
#endif


void __interrupt Exception( void )
{
   //RESET_SYS();
   //while( 1 );
   RTCWDT = 0xBFFFu;
}


/*
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
*/

void main( void )
{
   // Since the SD, NAND flash and 8088 interface use the same I/O pins, 
   // if both SD card and NAND Flash are used, or both SD card and 8088 interface are used in the system
   // this function must be executed to avoid the competition of I/O pins.   
   SetVector( SD_DELAY_CALLBACK_TRAP, SD_Busy_Check );

   IP_INIT();
#if 0
   if( !otpverify())
   {
      RESET_SYS();
   }
#endif
   Int_disable();
   
   SystemClock( SYSTEM_CLOCK );
   SDKLoadCodInit();
   //SetVector( EXCEPTION_TRAP, Exception );
   //InitAfayaPort();
   //ADC_init();
   
#if 0
   //PWR_OFF();
   EXT_DC_PWR_OFF();

   if( power_is_vbus_present())
   {
      SendString((U8 *)"usb power\r\n");
      PWR_OFF();   	     
      EXT_DC_PWR_OFF();
   }
   else
   {
#if 0
      U8 key_power_press_time = 30;
      do
      {
         //delay1ms( 15 );
         if( !key_power_is_pressed())
         {
            /*P0DIR = P1DIR = P2DIR = P3DIR = P4DIR = 0;
            //Reset player now
            (*(void(*)())0)();*/
            //power_system_off( FALSE );
            RESET_SYS();
         }
      }
      while( --key_power_press_time );
#endif
      SendString((U8 *)"bat power\r\n");
      PWR_ON();
      EXT_DC_PWR_ON();
   }

   EXT_PWR_CTL_ON();
#endif
   //green_led_on();
  

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

#if 0
void InitAfayaPort( void )
{
#if 1
   _bitSET( HP_DET_DIR );
   _bitSET( HP_DET_PH );

   _bitSET( RED_LED );
   _bitCLR( RED_LED_DIR );

   _bitCLR( P_EN );
   _bitCLR( P_EN_DIR );

   _bitSET( SDIO_DIR );
   _bitSET( SDIO_PH );

   _bitCLR( SCK );
   _bitCLR( SCK_DIR );

   _bitSET( BAT_nEOC_DIR );
   _bitSET( BAT_nEOC_PH );

   //_bitCLR( PWRON );
   _bitCLR( PWRON_DIR );

   _bitSET( SD_DET_DIR );
   _bitSET( SD_DET_PH );

   //_bitSET( LCD_nON );
   //_bitCLR( LCD_nON_DIR );

   _bitSET( GREEN_LED );
   _bitCLR( GREEN_LED_DIR );

   //_bitSET( KBD_PWR_DIR );
   //_bitSET( KBD_PWR_PH );
#else
   PBOD = 0x3030;
   PBDIR = 0xCAAF;
   PBPH = 0xFFFF;
#endif

#if 1
   _bitCLR( OP_CTL );
   _bitCLR( OP_CTL_DIR );

   //_bitCLR( LCD_nRST );
   //_bitCLR( LCD_nRST_DIR );

   _bitSET( USB_DET_DIR );
   _bitCLR( USB_DET_PH );
#else
   PCOD = 0x0000;
   PCDIR = 0x05FD;
   PCPH = 0x07FF;
#endif

   /*PDOD = 0x0000;
   PDDIR = 0x00FF;
   PDPH = 0x00FF;*/
}
#endif

