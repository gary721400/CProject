#include "BinaryImage.h"

#include "config.h"
#include "conf_kb.h"

#include <KBDM9_DDK.h>
#include <SFR.h>
#include <string.h>
#include "cis.h"
#include "Calibration.h"
#include "com.h"
#include "com.h"
//#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
#include "oid.h"
#include "power.h"             // Power module definition
#include "Example_USB_MAssStorage.h"
#include "system.h"
//#include "SDImg.h"
//#include "globalvariable.h"
//#include "KBDM952.H"

//#include "conf\conf_lcd.h"
//#include LCD_CONTROLLER_INCLUDE_NAME_FILE
//#include "drivers\mmi\img_handler.h"
//#include "lib_mcu\lcd\lcd_drv.h"
//#include "mmi\images\tab_pict.h"


extern unsigned long t01_calib_data1;
extern unsigned long t01_calib_data2;

#pragma CODE = Test_Code
#pragma DATA = Test_Data

extern void write_t01_cmd1( U16 val );
extern void write_t01_cmd( U16 val1, U16 val2, U16 val3 );
extern U32   read_oid_pen   ( void );
extern void set_t01_mode( U16 mode_pam1,U16 mode_pam2,U16 mode_pam3,U32 mode_ack);
//__X unsigned int ReserveCalibrationData[ 256 ];
#define  ReserveCalibrationData     CalibrationData

 _MEM_TYPE_SLOW_   U32   usb_nosd_timer;

#if 0
unsigned int Calib_IR( void )
{
   __X unsigned long OPARCode = 0;
   unsigned long i;

   g_select_appli = MSTORAGE_HID_APPLI;
   usb_nosd_timer = 0;
   green_led_stop_timer();
   red_led_stop_timer();
   green_led_on();red_led_on();
   
   for( i = 0; i < 256; i++ )
      ReserveCalibrationData[ i ] = 0x00;

   OPARI2C_Ini();
   OPARCalibration();
   SendString((U8 *)"calibration\r\n");

   //i = 1000000000;
   //while( i-- )
   i = Timer_set_timer(TIMER_S( 50 ));
   while( 1 )
   {
      if( Decode())
      {
         OPARCode = GetOPARCode();
         SendString((U8 *)"opar=");
         send_hex_u32(OPARCode);
         SendString((U8 *)"\r\n");
         //if(( OPARCode & 0xF60000 )== 0xF60000 )
         switch( MSB1( OPARCode ))
         {
            case 0xF6:
            ReserveCalibrationData[ 0 ] =( unsigned int )(( OPARCode & 0x00FF00 )>> 8 );
            ReserveCalibrationData[ 1 ] = 0x01;
            ReserveCalibrationData[ 2 ] = 0x01;
            ReserveCalibrationData[ 3 ] = ( unsigned int )( OPARCode & 0xFF );

            ReserveCalibrationData[ 4 ] = ReserveCalibrationData[ 0 ]+ 0x0A;
            ReserveCalibrationData[ 5 ] = 0x01;
            //ReserveCalibrationData[ 4 ] = ReserveCalibrationData[ 0 ]- 0x0A;
            //ReserveCalibrationData[ 5 ] = ReserveCalibrationData[ 3 ]- 0x0A;

            ReserveCalibrationData[ 6 ] = 0x01;
            ReserveCalibrationData[ 7 ] = ReserveCalibrationData[3]+0x0A;
            //ReserveCalibrationData[ 6 ]=ReserveCalibrationData[0]-0x06;
            //ReserveCalibrationData[ 7 ]=ReserveCalibrationData[3]-0x06;

            ReserveCalibrationData[ 8 ] = ReserveCalibrationData[ 4 ]+ 0x12;
            ReserveCalibrationData[ 9 ] = 0x01;
            ReserveCalibrationData[ 10 ] = 0x01;
            ReserveCalibrationData[ 11 ] = ReserveCalibrationData[ 7 ]+ 0x12;

            ReserveCalibrationData[ 12 ] = ReserveCalibrationData[ 0 ]- 0x06;
            ReserveCalibrationData[ 13 ] = ReserveCalibrationData[ 3 ]- 0x06;

            red_led_off();
            return 1;

            case 0xF1:
            case 0xF2:
            case 0xF3:
            case 0xF4:
            case 0xF5:
            green_led_off();
            return 0;
         }
      }
      if( u32_i2c_timeout && Timer_is_timeout( u32_i2c_timeout ))
      {
         u32_i2c_timeout = 0;
         FirstBit = 1;
         RXFlag = 0;
      }
      if( Timer_is_timeout( i ))
         break;

      /*if( Usb_connect )
      {
         MassStorage_Loop();
      }*/
   }

   green_led_off();
   return 0;
}
#else

unsigned int calib_state = CALIB_IDLE;
unsigned long calib_timer;

extern __X unsigned int RectifyStatus;
extern __X unsigned int CalibrationData[ 256 ];






void save_oid_cal(void)
{
	//__X U16 TempBuf[ 256 ];
	__X U32 DataStartSector;
	__X U32 DataLength;
	__X U16 temp_i;
	SPIF_ERR cur_err;
	__X U16 CalData[ 256 ];
#if 1//( T01OID_CAB == ENABLE )     
				 
	//GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
			   nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
			   sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 1//def SPIBOOT_KBDM9
	//spif_resv_read( DataStartSector, TempBuf );
#endif
/*
			   if( play_startup_flag )
			   {
				  if( !ChangeToUSBMode())
					 break;
			   }
*/
		SendString("\r\nsave cab data\r\n");
	   //if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ){
	   //	}
	   //else{
		  // MSW( t01_calib_data1 ) = TempBuf[ 0 ];
		  // LSW( t01_calib_data1 ) = TempBuf[ 1 ];
		  // MSW( t01_calib_data2 ) = TempBuf[ 2 ];
		   //LSW( t01_calib_data2 ) = TempBuf[ 3 ];
		   SendString("\r\ncal=");
		   send_hex_u32(t01_calib_data1);
		   SendString(":");
		   send_hex_u32(t01_calib_data2);
		///*
		   
		   if( spif_mount() != SPIF_No_Err )
			   {
			       SendString("\r\nspi mount fail!");
				   return;
			   }
				   
			   // Configure the flash to read/write mode.
		  SPIF_Read_Write();
			   
		  // memset( CalData, 0, sizeof( CalData ));
		  for(temp_i=0;temp_i<128;temp_i++){
		  	CalData[temp_i] = 0;
		  	}
		   CalData[0] = 0x1111;
		   CalData[1] = 0x2222;


		   //CalData[2] = MSW( t01_calib_data1 );
		   //CalData[3] = LSW( t01_calib_data1 );
		   //CalData[4] = MSW( t01_calib_data2 );
		   //CalData[5] = LSW( t01_calib_data2 );

		   CalData[2] = CalibrationData[0];
		   CalData[3] = CalibrationData[1];
		   CalData[4] = CalibrationData[2];
		   CalData[5] = CalibrationData[3];
		   CalData[6] = CalibrationData[4];
		   CalData[7] = CalibrationData[5];
		   
		   
		   CalData[8] = 0x3333;
		   CalData[9] = 0x4444;

			 SendString("\r\nread save data = ");
			for(temp_i=0;temp_i<10;temp_i++){
				send_hex_u16(CalData[temp_i]);
				SendString(":");
				}
		  //DataStartSector = (U32)(4086UL<<10);
		  //DataStartSector = (U32)(0x003FD800);
		  DataStartSector = SAVE_OID_CAL_ADDR;

		 if(SPIF_Sector_Erase(DataStartSector)>0) {
			//Leave_fail();//ZOE
			SendString("\r\nspi erase error");
			return ;
			}
		  SendString("\r\nwrite_addr=");send_hex_u32(DataStartSector);
		  if(( SPIF_Page_Program( DataStartSector, (__X unsigned char *)&CalData, 1 )) != SPIF_No_Err )
		  //cur_err = spif_write(DataStartSector,(__X void *)CalData);
		  //if(cur_err != SPIF_No_Err)
	      {
	         SendString("\r\nspi program error");
	         return;
	      }
		  
		  DataStartSector = SAVE_OID_CAL_ADDR;
		  SendString("\r\nread_addr=");send_hex_u32(DataStartSector);
	      if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&CalData, sizeof(CalData) ) != SPIF_No_Err )
		  // cur_err = spif_write(DataStartSector,(__X void *)CalData);
	      //if(cur_err != SPIF_No_Err)
	      {
	         SendString("\r\nspi read error");
	         return;
	      }

		    SendString("\r\nsave data = ");
			for(temp_i=0;temp_i<10;temp_i++){
				send_hex_u16(CalData[temp_i]);
				SendString(":");
				}
			
			//*/
		//}
#endif //

}



unsigned int Calib_IR( void )
{
   __X unsigned long OPARCode ;
   unsigned long i;
   unsigned long SectorAddress, DataLength;
   SPIF_ERR cur_err;

   switch( calib_state )
   {
      case CALIB_START:
	  SendString("\r\nCALIB_START");
      RectifyStatus = 0;
      
      //g_select_appli = MSTORAGE_HID_APPLI;
      usb_nosd_timer = 0;
     // green_led_stop_timer();
      //red_led_stop_timer();
      //green_led_on();
	  //red_led_on();
      
      //for( i = 0; i < 256; i++ )
      //   ReserveCalibrationData[ i ] = 0x00;
      memset( CalibrationData, 0, sizeof( CalibrationData ));

      //OPARI2C_Ini();
      //OPARCalibration();
      SendString("\r\ncalibration");

      //oid_power_on();
      //pen_enable();
      //if( b_oid_init_flag )
      ///*
      {
         //turn_off_oid();
         pen_disable();
         oid_power_off();
         oid_dc_dc_off();
      }
	  //*/
     // b_oid_init_flag = FALSE;
      //if( b_oid_init_flag == FALSE )
      b_cal_init_fail = FALSE;
         reinit_oid_pen();
		 if(b_cal_init_fail){
		 	calib_state = CALIB_KO;
			break;
		 	}
      
      calib_timer = Timer_set_timer( TIMER_MS( 150 ));
      while( !Timer_is_timeout( calib_timer ));
      calib_timer = Timer_set_timer( TIMER_S( 10 ));
      calib_state = CALIB_WAIT;
		
	 // set_t01_mode(UserCmd_SetMode, UserCmd_SetModeParam1, UserCmd_SetModeParam2,OIDCmd_SetMode_Ack);
      //write_t01_cmd( UserCmd_Calib_Trigger0, UserCmd_Calib_Trigger1, UserCmd_Calib_Trigger2 );
#if( DPR3 == DISABLE )
#ifdef TX_DEBUG
			 SendString("\r\nno dpr3 command!");
#endif
			write_t01_cmd( UserCmd_Calib_Trigger0, UserCmd_Calib_Trigger1, UserCmd_Calib_Trigger2 );
#endif   //#if( DPR3 == DISABLE )
#if( DPR3 == ENABLE )
#ifdef TX_DEBUG
			SendString("\r\ndpr3 command!");
#endif

			write_t01_cmd( UserCmd_Calib_Trigger3, UserCmd_Calib_Trigger1, UserCmd_Calib_Trigger2 );
#endif   //#if( DPR3 == ENABLE )	  
      break;

      case CALIB_WAIT:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
         SendString("\r\nack=");
         send_hex_u32(OPARCode);
#if( DPR3 == DISABLE )
		  if( OPARCode != OIDCmd_CalibTrigger_Ack )
#endif   //#if( DPR3 == DISABLE )
#if( DPR3 == ENABLE )
		  if( OPARCode != OIDCmd_CalibTrigger_Ack1 )
#endif   //#if( DPR3 == ENABLE )
         {
            //green_led_off();red_led_on();
            //SendString((U8 *)"ack err\r\n");
            calib_state = CALIB_KO;
            return 0;
         }
         calib_state = CALIB_RD_START;
         calib_timer = Timer_set_timer( TIMER_S( 20 ));
         return 0;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         SendString("\r\nack timer out");
         calib_state = CALIB_KO;
         return 0;
      }
      break;

      case CALIB_RD_START:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
         SendString("\r\nstatus=");
         send_hex_u32(OPARCode);

         if( OPARCode != OIDCmd_Calib_ReportStatus )
         {
            //green_led_off();red_led_on();
            //SendString((U8 *)"report err\r\n");
            calib_state = CALIB_KO;
            return 0;
         }
         calib_state = CALIB_RD;
         calib_timer = Timer_set_timer( TIMER_S( 5 ));
         return 0;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         SendString("\r\nreport timer out");
         //green_led_off();
         calib_state = CALIB_KO;
      }
      break;

      case CALIB_RD:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
         SendString("\r\ncal1=");
         send_hex_u32(OPARCode);

         CalibrationData[ 0 ] = ( unsigned int )OPARCode;
         CalibrationData[ 1 ] = ( unsigned int )( OPARCode >> 16 )& 0x0F;
         calib_state = CALIB_RD1;
         calib_timer = Timer_set_timer( TIMER_S( 5 ));
         break;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         SendString("\r\nread calib1 timer out");
         calib_state = CALIB_KO;
      }
      break;

      case CALIB_RD1:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
         SendString("\r\ncal2=");
         send_hex_u32(OPARCode);
    
         CalibrationData[ 2 ] = ( unsigned int )OPARCode;
         CalibrationData[ 3 ] = ( unsigned int )( OPARCode >> 16 )& 0x0F;
#if( DPR3 == DISABLE )
         calib_state = CALIB_OK;
#endif   //#if( DPR3 == DISABLE )
#if( DPR3 == ENABLE )
         calib_state = CALIB_RD2;
         calib_timer = Timer_set_timer( TIMER_S( 5 ));
#endif   //#if( DPR3 == ENABLE )
         break;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         SendString("\r\nread calib2 timer out");
         calib_state = CALIB_KO;
      }
      break;
#if( DPR3 == ENABLE )
      case CALIB_RD2:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
//         SendString((U8 *)"cal3=");
         send_hex_u32(OPARCode);
//         SendString((U8 *)"\r\n");
         CalibrationData[ 4 ] = ( unsigned int )OPARCode;
         //CalibrationData[ 5 ] = ( unsigned int )( OPARCode >> 16 )& 0x0F;
         calib_state = CALIB_OK;
         break;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         //SendString((U8 *)"read calib2 timer out\r\n");
         calib_state = CALIB_KO;
      }
      break;
#endif   //#if( DPR3 == ENABLE )
      case CALIB_OK:
      RedLed_OFF();
	  BlueLed_ON();
      GetDataIdInfo( D_CAB, &SectorAddress, &DataLength );
      //Get_Calibration_Data( CalibrationData );
      CalibrationData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
      sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
      cur_err = spif_resv_write( SectorAddress, CalibrationData );
#endif
#ifdef TX_DEBUG
			if(cur_err != SPIF_No_Err){
				SendString("\r\ncal save fail!");
				}
			else{
				SendString("\r\ncal save ok!");
				}
#endif		
      RectifyStatus = 0x55;
      //calib_state = CALIB_IDLE;
      calib_state = CALIB_RESTART;
	  // usb_nosd_timer = Timer_set_timer( TIMER_MS( 1000 ));
	   //t01_calib_data1 = CalibrationData[0];
	   //t01_calib_data2 = CalibrationData[2];
	   save_oid_cal();
	  Delay_ms(6000);
      break;

      case CALIB_KO:
      RectifyStatus = 0xFF;
      //green_led_off();
	  //red_led_on();
	  BlueLed_OFF();
	  RedLed_ON();
      //calib_state = CALIB_IDLE;
      calib_state = CALIB_RESTART;
	  
	  //usb_nosd_timer = Timer_set_timer( TIMER_MS( 1000 ));
	  Delay_ms(6000);
      break;

      case CALIB_RESTART:
      SendString("\r\nRestart sys");
      calib_timer = Timer_set_timer( TIMER_MS( 5 ));
      while( !Timer_is_timeout( calib_timer ));
      write_t01_cmd1( UserCmd_RestartSys );
      
      calib_timer = Timer_set_timer( TIMER_S( 5 ));
      calib_state = CALIB_RESTART_ACK;
      break;

      case CALIB_RESTART_ACK:
      if( !read_sdio())
      {
         OPARCode = read_oid_pen();
         SendString("\r\nsysack=");
         send_hex_u32(OPARCode);

         if( OPARCode != OIDCmd_RestartSys_Ack )
         {
            //green_led_off();red_led_on();
            SendString("\r\nsys ack err");
            calib_state = CALIB_IDLE;
            return 0;
         }
         SendString("\r\nsys ok");
         calib_state = CALIB_IDLE;
		 //save_oid_cal();
		 calib_timer = Timer_set_timer( TIMER_S( 3 ));
      	 while( !Timer_is_timeout( calib_timer ));
		 SendString("\r\ncal reset!");
		 RTCWDT = 0xBFFFU;

		 //lcd_goto_xy( 30, 30 );
        // Ih_set_curr_font( IDC_FONT_16 );
        // lcd_puts(( U8 *)"OK" );
#if 0
         calib_timer = Timer_set_timer( TIMER_MS( 100 ));
         while( !Timer_is_timeout( calib_timer ));
         write_t01_cmd( UserCmd_SetCal1, CalibrationData[ 0 ], CalibrationData[ 1 ]);
         calib_timer = Timer_set_timer( TIMER_MS( 300 ));
         while( read_sdio())
         {
            if( Timer_is_timeout( calib_timer ))
            {
               SendString("set cal1 err\r\n");
               return 0;
            }
         }
         OPARCode = read_oid_pen();
         SendString("cal1=");
         send_hex_u32(OPARCode);
         SendString((U8 *)"\r\n");
         //calib_timer = Timer_set_timer( TIMER_MS( 5 ));
         //while( !Timer_is_timeout( calib_timer ));
         write_t01_cmd( UserCmd_SetCal2, CalibrationData[ 2 ], CalibrationData[ 3 ]);
         calib_timer = Timer_set_timer( TIMER_MS( 300 ));
         while( read_sdio())
         {
            if( Timer_is_timeout( calib_timer ))
            {
               SendString((U8 *)"set cal2 err\r\n");
               return 0;
            }
         }
         OPARCode = read_oid_pen();
         SendString((U8 *)"cal2=");
         send_hex_u32(OPARCode);
         SendString((U8 *)"\r\n");
#endif
         return 0;
      }
      if( Timer_is_timeout( calib_timer ))
      {
         //SendString((U8 *)"sys timer out\r\n");
         //green_led_off();
         calib_state = CALIB_IDLE;
      }
      break;
   }
   return 0;
}
#endif

/*
void Get_Calibration_Data( __X unsigned int* ptr )
{
   memcpy( ptr, ReserveCalibrationData, 512 );
}
*/

