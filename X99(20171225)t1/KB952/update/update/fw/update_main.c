
#include "stdio.h"
//#include "lib_mcu\timer\timer_drv.h"
#include "lib_mcu\int\int_drv.h"
#include "compiler.h"
#include "sio\com.h"
//#include "modules\player\player_task.h"
//#include "mmi\kbd\keypad.h"
//#include "modules\search\search_task.h"
//#include "drivers\oid\oid.h"
//#include "modules\power\power_task.h"
//#include "modules\scheduler\scheduler.h"
//#include "mmi\shared\com_var.h"

//#include "modules\usb\usb_task_sch.h"
#include "modules\power\power.h"             // Power module definition
//#include "mmi_ap3.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "string.h"
//#include "lib_mcu\timer\timer_drv.h"  // Include the timer to precharge
#include "config.h"
//#include "drivers\mmi\img_handler.h"
//#include "mmi\status\mmi_status.h"
#include "conf\conf_lcd.h"
#include LCD_CONTROLLER_INCLUDE_NAME_FILE

#pragma DATA = MAIN_DATA
#pragma CODE = MAIN_CODE

//#define SPI_BURN_FILE_NAME "0:\\up20D.bin"
#define SPIF_SECTOR_SIZE   ( 4096L )
#define PAGE_SIZE          ( 256L )
#define BUF_SIZE           ( 512L*8 )

__X   unsigned char     SD_Buf[ BUF_SIZE ];
__X   unsigned char     SPI_Buf[ BUF_SIZE ];

//const unsigned int otpdata128_test[ 8 ] = { 0x0001u, 0x5555u, 0x4321u, 0x0800u, 0x9527u, 0x0097u, 0xFFFFu, 0xB458u };
//const unsigned int otpdata128_test_2[ 8 ] = { 0xFFFEu, 0xFFF0u, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu };

//extern   int   OtpVerify128Bit   ( __X unsigned int *ptr );


//void  update_hbar             ( U8 x0, U8 y0 );
//void  update_percent_bar      ( U8 u8_percent );
//void  update_purcent_bar_init ( void );

/*
#ifdef  TX_DEBUG
void  InitUart          ( void );
#else
#define InitUart()
#endif
*/




void recover_oid_cal(void)
{
#if 1
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 256 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;

    //GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SPIBOOT_KBDM9
    spif_resv_read( DataStartSector, TempBuf );
#endif


    memset( CalData, 0, sizeof( CalData ));
    //CalData[ 0 ] = TempBuf[ 2 ];
    //CalData[ 1 ] = TempBuf[ 3 ];
    //CalData[ 2 ] = TempBuf[ 4 ];
    //CalData[ 3 ] = TempBuf[ 5 ];
    //CalData[ 4 ] = TempBuf[ 6 ];
    //CalData[ 5 ] = TempBuf[ 7 ];

    CalData[ 0 ] =  t01_calib_data1 ;
    CalData[ 1 ] =  t01_calib_data2 ;

    //CalData[ 0 ] = MSW( cal_data_1 );
    //CalData[ 1 ] = LSW( cal_data_1 );
    //CalData[ 2 ] = MSW( cal_data_2 );
    //CalData[ 3 ] = LSW( cal_data_2 );



    GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
    //Get_Calibration_Data( CalibrationData );
    CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
    sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
    spif_resv_write( DataStartSector, CalData );
#endif
    SendString((U8 *)"\r\nrecover ok!");



#else
    __X U16 TempBuf[ 256 ];

    __X U32 DataStartSector;
    __X U32 DataLength;
    __X U16 CalData[ 256 ];

    __X U16 temp_i;
    SPIF_ERR cur_err;
#if( T01OID_CAB == ENABLE )

    GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
    nftl_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def SDBOOT_KBDM9
    sdtl_resv_read( DataStartSector, TempBuf );
#endif
#if 1//def SPIBOOT_KBDM9
    spif_resv_read( DataStartSector, TempBuf );
#endif
    /*
                   if( play_startup_flag )
                   {
                      if( !ChangeToUSBMode())
                         break;
                   }
    */
    SendString("\r\nrecover read cab data\r\n");
    if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ) {

        if( spif_mount() != SPIF_No_Err ) {
            SendString("\r\nspi mount fail!");
            return;
        }

        // Configure the flash to read/write mode.
        SPIF_Read_Write();

        //DataStartSector = (U32)(4086UL<<10);
        //DataStartSector = 1040384UL;
        DataStartSector = SAVE_OID_CAL_ADDR;
        //if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
        // cur_err = spif_write(DataStartSector,(__X void *)CalData);
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&CalData, sizeof(CalData) ) != SPIF_No_Err )
            /// if(cur_err != SPIF_No_Err)
        {
            SendString("\r\nspi read error");
            return;
        }

        SendString("\r\nrecover data = ");
        for(temp_i=0; temp_i<10; temp_i++) {
            send_hex_u16(TempBuf[temp_i]);
            SendString(":");
        }

        if((TempBuf[0] == 0x1111)&&(TempBuf[1] == 0x2222)
           &&(TempBuf[8] == 0x3333)&&(TempBuf[9] == 0x4444)) {
            memset( CalData, 0, sizeof( CalData ));
            CalData[ 0 ] = TempBuf[ 2 ];
            CalData[ 1 ] = TempBuf[ 3 ];
            CalData[ 2 ] = TempBuf[ 4 ];
            CalData[ 3 ] = TempBuf[ 5 ];
            CalData[ 4 ] = TempBuf[ 6 ];
            CalData[ 5 ] = TempBuf[ 7 ];
            //t01_calib_data3 = TempBuf[ 4 ];


            GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
            //Get_Calibration_Data( CalibrationData );
            CalData[( DataLength >> 1 )- 1 ] = 0xA55A;
#if 0//def SDBOOT_KBDM9
            sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
            spif_resv_write( DataStartSector, CalData );
#endif
        }
        SendString("\r\nrecover ok!");
        /*
        for(temp_i=0;temp_i<10;temp_i++){
            //send_hex_u16(TempBuf[temp_i]);
            //SendString(":");
            TempBuf[temp_i] = 0;
            }

        DataStartSector = 1040384UL;
        if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
        {
        SendString("\r\nspi read error");
        return;
        }
        SendString("\r\nrecover erase ok!");
        */
    } else {
        SendString("\r\nno need recoverdata\r\n");
    }
#endif //
#endif
}


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


#if 0//def  TX_DEBUG
void InitUart( void )
{
   UARBR = 0x0002u;  //115200 bps
   UARCR = 0x8001;   // disable parity, one stop bit, 8 bits per character
   PBEN |= 0x0010;
}
#endif


void main( void )
{
	unsigned int i;
	unsigned long read_size = 0;
	unsigned long spif_total_size;
	unsigned long spi_offset;
	unsigned long file_size;
	FS_ERR fserr;
	__X FCFILE *fshandle;
	Bool update_ok_flag;

   IP_INIT();

   Int_disable();
   
   SystemClock( FEQ_60M );
   SDKLoadCodInit();
   //SetVector( EXCEPTION_TRAP, Exception );
   //InitAfayaPort();
   //ADC_init();
   green_led_off();
   red_led_on();
   InitUart();
   SendString((U8 *)"\r\nKB Update");

   Int_enable();

   //lcd_turn_on();
  // SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //  lcd_clear_screen();
  // update_purcent_bar_init();    // display the empty progression bar
  // lcd_goto_xy( 40, 8 );
  // Ih_set_curr_font( IDC_FONT_16 );
  // lcd_puts(( U8 *)"Updating" );

	FSInit( 1 );
	if ( FS_mount( &SD_Drive ) == FS_DRIVE_ERROR )
   {
      goto updata_fail;
   }
	
	// Initial the serial flash.
	if( spif_mount() != SPIF_No_Err )
	{
		goto updata_fail;
	}
		
	// Configure the flash to read/write mode.
	SPIF_Read_Write();
	
	// Get the flash sector size of total usable space.
	spif_total_size = spif_get_size() + spif_resv_size() + 4;
	
	file_size = FSGetFileSize(( char *)&remote_index_tab[ 0 ], &fserr );
   if( fserr != FS_NO_ERROR )
      goto updata_fail;

	if( spif_total_size <( file_size >> 9 ))
	{
		goto updata_fail;
	}

	// Ereas first sector(4K Bytes)
	if( SPIF_Sector_Erase( 0 )> 0 )
	{
		goto updata_fail;
	}

	fshandle = FSOpen(( char *)&remote_index_tab[ 0 ], FS_MODE_READ, &fserr );
   if( fserr != FS_NO_ERROR )
      goto updata_fail;

   if ( file_size <= SPIF_SECTOR_SIZE )
      goto updata_fw1;

   SendString((U8 *)"\r\nupdata fw2");
   spif_total_size = 1;
updata_fw_next:
   spi_offset = spif_total_size * SPIF_SECTOR_SIZE;
   if( spi_offset >= file_size )
       goto updata_fw1;

	/*SendString((U8 *)"spi erase=");
   send_hex_u32(spi_offset);
   SendString((U8 *)"\r\n");*/
   if( SPIF_Sector_Erase( spi_offset ))
   {
      SendString((U8 *)"\r\nerase error");
      goto updata_fail1;
   }

	read_size = spi_offset;
	FSSeek( fshandle, read_size );

	for( i = 0; i <( SPIF_SECTOR_SIZE / BUF_SIZE ); i++ )
   {
      memset( &SD_Buf, 0xFF, sizeof( SD_Buf ));
      memset( &SPI_Buf, 0xFF, sizeof( SPI_Buf ));

      if( read_size >= file_size )
         goto updata_fw1;

      // if( sdtl_resv_read( sd_sector, ( __X void *)SD_Buf )!= SDTL_No_Err )
      read_size += FSRead( fshandle, SD_Buf, BUF_SIZE, &fserr );
      if ( fserr != FS_NO_ERROR )
      {
         SendString((U8 *)"\r\nsd read error\r\n");
         goto updata_fail1;
      }

      /* SendString((U8 *)"spi program=");
      send_hex_u32(spi_offset);
      SendString((U8 *)"\r\n");*/
      // write data from buffer to flash
      if(( SPIF_Page_Program( spi_offset, SD_Buf, BUF_SIZE / PAGE_SIZE )) != SPIF_No_Err )
      {
         SendString((U8 *)"\r\nspi program error\r\n");
         goto updata_fail1;
      }

      if( SPIF_Read_Byte( spi_offset, SPI_Buf, BUF_SIZE ) != SPIF_No_Err )
      {
         SendString((U8 *)"\r\nspi read error\r\n");
         goto updata_fail1;
      }

      if( memcmp( SD_Buf, SPI_Buf, BUF_SIZE ))
      {
         SendString((U8 *)"\r\ncompare error\r\n");
         goto updata_fail1;
      }

      spi_offset += BUF_SIZE;
   }

//   update_percent_bar(( U8 )(( spif_total_size * SPIF_SECTOR_SIZE * 100 )/ file_size ));
   
   spif_total_size++;
   goto updata_fw_next;

updata_fw1:
   SendString((U8 *)"updata fw1\r\n");
   spi_offset = 0;

   read_size = spi_offset;
   FSSeek( fshandle, read_size );
   
   for( i = 0; i <( SPIF_SECTOR_SIZE / BUF_SIZE ); i++ )
   {
      memset( &SD_Buf, 0xFF, sizeof( SD_Buf ));
      memset( &SPI_Buf, 0xFF, sizeof( SPI_Buf ));
      
      if( read_size >= file_size )
         goto updata_ok;

      //if( sdtl_resv_read( sd_sector, ( __X void *)SD_Buf )!= SDTL_No_Err )
      read_size += FSRead( fshandle, SD_Buf, BUF_SIZE, &fserr );
      if( fserr != FS_NO_ERROR )
      {
         SendString((U8 *)"sd read0 error\r\n");
         goto updata_fail1;
      }

      /*SendString((U8 *)"spi program=");
      send_hex_u32(spi_offset);
      SendString((U8 *)"\r\n");*/
      // write data from buffer to flash
      if(( SPIF_Page_Program( spi_offset, SD_Buf, BUF_SIZE / PAGE_SIZE )) != SPIF_No_Err )
      {
         SendString((U8 *)"spi program0 error\r\n");
         goto updata_fail1;
      }

      if( SPIF_Read_Byte( spi_offset, SPI_Buf, BUF_SIZE ) != SPIF_No_Err )
      {
         SendString((U8 *)"spi read0 error\r\n");
         goto updata_fail1;
      }

      if( memcmp( SD_Buf, SPI_Buf, BUF_SIZE ))
      {
         SendString((U8 *)"compare0 error\r\n");
         goto updata_fail1;
      }
      spi_offset += BUF_SIZE;
   }

updata_ok:
   SendString((U8 *)"\r\nupdate end");
   //FSClose( fshandle );
   /*fserr = FSDelete(( char *)&remote_index_tab[ 0 ]);
   SendString((U8 *)"fserr=");
   send_hex_u8(fserr);
   SendString((U8 *)"\r\n");
   SendString((U8 *)&remote_index_tab[ 0 ] );
   if( fserr != FS_NO_ERROR )
   {
      fserr = FSDelete(( char *)&remote_index_tab[ 0 ]);
      SendString((U8 *)"delete again\r\n");
   }*/

  // lcd_goto_xy( 58, 24 );
  // lcd_puts(( U8 *)"OK" );
 //  update_percent_bar( 100 );
    recover_oid_cal();
   
   SendString((U8 *)"\r\nupdate ok");
   update_ok_flag = TRUE;
   point_A = 0x5555;
updata_end:
 //  for( i = 0; i < 10; i++ )
 //  {
      //red_led_flash();
 //     delay1ms( 200 );
 //  }  

//updata_end:
   /*red_led_off();
   green_led_on();
   while( 1 )
   {
	   delay1ms( 200 );
	   if( !update_ok_flag )
	   {
         green_led_flash();
      }
   }*/
   point_B = 0x55AA;
   point_A = 0;
   point_B = 0;
   SendString((U8 *)"\r\nready reset");
   PWR_OFF();
   while(1){
   	 RTCWDT = 0xBFFFu;
   	}

      
   //CommonLoadCode( C_IDLE );

updata_fail1:
   //FSClose( fshandle );
updata_fail:
	SendString((U8 *)"update fail\r\n");
	update_ok_flag = FALSE;
	point_A = 0xAAAA;
  // lcd_goto_xy( 58, 24 );
 //  lcd_puts(( U8 *)"KO" );
	goto updata_end;   
}



