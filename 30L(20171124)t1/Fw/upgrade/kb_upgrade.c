#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE


#include "KBDM9_DDK.h"
#include <string.h>
#include <ctype.h>
#include "SYSTEM.H"
//#include "SDImg.h"
#include "fs.h"
//#include "sio.h"
//#include "config.h"

#define SPI_BURN_FILE_NAME "0:\\up.afa"//ZOE_MODIFY
//#define SPIF_SECTOR_SIZE (4096L)
//#define PAGE_SIZE (256L)
//__X unsigned char buffer[SPIF_SECTOR_SIZE];
//__X unsigned char xor_buffer[SPIF_SECTOR_SIZE];
//__X unsigned int CalibrationData[ 256 ];
#define SPIF_SECTOR_SIZE   ( 4096L )
#define PAGE_SIZE          ( 256L )
#define BUF_SIZE           ( 512L*8 )

__X   unsigned char     SD_Buf[ BUF_SIZE ];
__X   unsigned char     SPI_Buf[ BUF_SIZE ];
//extern __X	   unsigned char	  sector_buffer[512];
///*
void Delay_ms(int count);
__asm{
_Delay_ms::
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
//*/
/*
void rewrite_cab(void)
{
#if ( T01OID_CAB == ENABLE )	
 	unsigned long SectorAddress, DataLength;
	 memset( CalibrationData, 0, sizeof( CalibrationData ));
      GetDataIdInfo( D_CAB, &SectorAddress, &DataLength );
      //Get_Calibration_Data( CalibrationData );
      CalibrationData[( DataLength >> 1 )- 1 ] = 0xA55A;
	  //CalibrationData[ 0 ] = ( unsigned int )t01_calib_data1;
      //CalibrationData[ 1 ] = ( unsigned int )( t01_calib_data1 >> 16 )& 0x0F;
	  //CalibrationData[ 2 ] = ( unsigned int )t01_calib_data2;
      //CalibrationData[ 3 ] = ( unsigned int )( t01_calib_data2 >> 16 )& 0x0F;
      CalibrationData[ 0 ] = MSW( t01_calib_data1 );
      CalibrationData[ 1 ] = LSW( t01_calib_data1 );
	  CalibrationData[ 2 ] = MSW( t01_calib_data2 );
      CalibrationData[ 3 ] = LSW( t01_calib_data2 );

	  
	  //CalibrationData[0] = t01_calib_data1;
	 // CalibrationData[1] = t01_calib_data2;
#ifdef SDBOOT_KBDM9
      sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#ifdef SPIBOOT_KBDM9
      spif_resv_write( SectorAddress, CalibrationData );
#endif
#endif

}
*/

void systemReset(void)
{
#ifdef DEBUG
	SendString("\r\nsystem reset!");
#endif
	BlueLed_ON();
	RedLed_OFF();
	PowerTurnOff();
	RTCWDT = 0xBFFFU;
	while(1)
	{
	}
}


void Leave_fail(void)
{
  unsigned int i=0;
#ifdef  DEBUG
	SendString(( char *)"\r\n upgrade fail!");
#endif	  	
  while(i++<20){
		RedLed_FLASH();
		BlueLed_OFF();
		Delay_ms(200);
	}
  //FSDelete(SPI_BURN_FILE_NAME);
 // rewrite_cab();
  systemReset();
	
}

void leaveOk(void)
{
  unsigned int i=0;
  while(i++<20){
		RedLed_OFF();
		BlueLed_FLASH();
		Delay_ms(200);
	}
	//FSDelete(SPI_BURN_FILE_NAME);
	 systemReset();
}

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
  	BlueLed_ON();
	RedLed_ON();
#ifdef DEBUG   
   initUart();
   SendString("\r\nKB Update");
#endif	
   Int_enable();

   //lcd_turn_on();
  // SDKLoadCode( LCD_SSD1303_TEMP_CODE02 );
 //  lcd_clear_screen();
  // update_purcent_bar_init();    // display the empty progression bar
  // lcd_goto_xy( 40, 8 );
  // Ih_set_curr_font( IDC_FONT_16 );
  // lcd_puts(( U8 *)"Updating" );

	FSInit( );
	if ( FS_mount( 0, &SD1_Drive ) == FS_DRIVE_ERROR )
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
	
	file_size = FSGetFileSize(( char *)SPI_BURN_FILE_NAME, &fserr );
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

	fshandle = FSOpen(( char *)SPI_BURN_FILE_NAME, FS_MODE_READ, &fserr );
   if( fserr != FS_NO_ERROR )
      goto updata_fail;

   if ( file_size <= SPIF_SECTOR_SIZE )
      goto updata_fw1;
#ifdef DEBUG  
   SendString("\r\nupdata fw2");
#endif
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
 #ifdef DEBUG    
      SendString("\r\nerase error");
 #endif
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
#ifdef DEBUG        
         SendString("\r\nsd read error\r\n");
#endif
         goto updata_fail1;
      }

      /* SendString((U8 *)"spi program=");
      send_hex_u32(spi_offset);
      SendString((U8 *)"\r\n");*/
      // write data from buffer to flash
      if(( SPIF_Page_Program( spi_offset, SD_Buf, BUF_SIZE / PAGE_SIZE )) != SPIF_No_Err )
      {
#ifdef DEBUG        
         SendString("\r\nspi program error\r\n");
#endif
         goto updata_fail1;
      }

      if( SPIF_Read_Byte( spi_offset, SPI_Buf, BUF_SIZE ) != SPIF_No_Err )
      {
#ifdef DEBUG        
         SendString("\r\nspi read error\r\n");.
 #endif        
         goto updata_fail1;
      }

      if( memcmp( SD_Buf, SPI_Buf, BUF_SIZE ))
      {
#ifdef DEBUG        
         SendString("\r\ncompare error\r\n");
#endif
         goto updata_fail1;
      }

      spi_offset += BUF_SIZE;
	  BlueLed_FLASH();
	  RedLed_FLASH();
   }

//   update_percent_bar(( U8 )(( spif_total_size * SPIF_SECTOR_SIZE * 100 )/ file_size ));
   
   spif_total_size++;
   goto updata_fw_next;

updata_fw1:
#ifdef DEBUG  	
   SendString("updata fw1\r\n");
#endif
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
#ifdef DEBUG        
         SendString("sd read0 error\r\n");
#endif
         goto updata_fail1;
      }

      /*SendString((U8 *)"spi program=");
      send_hex_u32(spi_offset);
      SendString((U8 *)"\r\n");*/
      // write data from buffer to flash
      if(( SPIF_Page_Program( spi_offset, SD_Buf, BUF_SIZE / PAGE_SIZE )) != SPIF_No_Err )
      {
#ifdef DEBUG        
         SendString("spi program0 error\r\n");
#endif
         goto updata_fail1;
      }

      if( SPIF_Read_Byte( spi_offset, SPI_Buf, BUF_SIZE ) != SPIF_No_Err )
      {
#ifdef DEBUG        
         SendString("spi read0 error\r\n");
#endif
         goto updata_fail1;
      }

      if( memcmp( SD_Buf, SPI_Buf, BUF_SIZE ))
      {
#ifdef DEBUG        
         SendString("compare0 error\r\n");
#endif
         goto updata_fail1;
      }
      spi_offset += BUF_SIZE;
	  BlueLed_FLASH();
	  RedLed_FLASH();
   }

updata_ok:
#ifdef DEBUG  	
   SendString("\r\nupdate end");
#endif
   leaveOk();
   
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
#ifdef DEBUG     
   SendString("\r\nupdate ok");
#endif
   //update_ok_flag = TRUE;

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
#ifdef DEBUG  
   SendString("\r\nready reset");
#endif
   PWR_OFF();
   while(1){
   	 RTCWDT = 0xBFFFu;
   	}

      
   //CommonLoadCode( C_IDLE );

updata_fail1:
	Leave_fail();
   //FSClose( fshandle );
updata_fail:
#ifdef DEBUG  
	SendString("update fail\r\n");
#endif
	//update_ok_flag = FALSE;
	//point_A = 0xAAAA;
  // lcd_goto_xy( 58, 24 );
 //  lcd_puts(( U8 *)"KO" );
	goto updata_end;   
}

