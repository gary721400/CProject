#pragma CODE = USB_HID_CODE
#pragma DATA = USB_HID_XRAM

#include "KBDM9_DDK.h"

#include "string.h"
#include "sio\com.h"
#include "sdimg.h"
//#include "SYSTEM.H"
//#include "power.h"

#define CMD_READ  0x02
#define CMD_WRITE 0x01
#define CMD_UPDATA_FW 		0x03



#define FW_UPDATA_TRUE 0x01
#define FW_TO_BOOT	   0x02

#define SPI_BURN_FILE_NAME "0:\\updata.bin"//ZOE_MODIFY
#define SPIF_SECTOR_SIZE (4096L)

#define PAGE_SIZE (256L)
__X unsigned char buffer[SPIF_SECTOR_SIZE];
unsigned int HID_rx_len;

unsigned char HID_INDEX=0;
__X unsigned char CalibrationData[ 512 ];
unsigned char cmd_type;

//#define	m16Merge(arg)			(*((__X unsigned int*)&(arg)))
//#define	m32Merge(arg)			(*((__X unsigned long*)&(arg)))

Bool updata_fw(void);
extern 	Bool updata_flag;
unsigned char HID_get_idle( unsigned int id )
{
    return 0;
}

void HID_set_idle( unsigned int id, unsigned int value )
{
}

void HID_set_protocol( unsigned int value )
{
}

unsigned int HID_get_protocol( void )
{
    return 0;
}

typedef void (*report_rx_func)( unsigned int length );
extern __X unsigned char ep0_buf[];
__X unsigned char gbuf[64];
__X unsigned char gbuf1[64];
__X unsigned long Cnt1 = 0;
__X unsigned long Cnt2 = 0;
__X unsigned int Flag;
void HID_report_rx( unsigned int length )
{
    int	test;
    unsigned long RX_Header;
    unsigned char RX_Checksum, CalChecksum;
 //   unsigned int  Cnt;
	unsigned long SectorAddress,DataLength;  
	unsigned char i;
    Cnt1++;
//    SendString("\r11");
    memcpy(gbuf, ep0_buf, 64);
	/*
    for(i=0;i<64;i++)
	{
		if((i%8)==0)
			SendString("\r\n");
		send_hex_u8(gbuf[i]);
		SendString(" ");
	}
	*/
	cmd_type = ep0_buf[0];
	if((cmd_type == CMD_UPDATA_FW)&&!updata_flag)
	{
		//BlueLed_OFF();
		GetDataIdInfo( D_USB_HID, &SectorAddress, &DataLength );
		spif_resv_read(SectorAddress,CalibrationData);
//		(void)updata_fw();
		if(updata_fw())
		{
			
			updata_flag = FW_UPDATA_TRUE ;
		}
		else
		{
			updata_flag = 0 ;
		}
		//BlueLed_ON();
		FSDelete(SPI_BURN_FILE_NAME);
		
		GetDataIdInfo( D_USB_HID, &SectorAddress, &DataLength );
		spif_resv_write( SectorAddress, CalibrationData );
	}
	if(cmd_type != CMD_WRITE)
		return ;

	
	RX_Checksum = 0 ;
	for(i=0;i<9;i++)
	{
		RX_Checksum ^= gbuf[i];
	}
	if(gbuf[9] != RX_Checksum)
		return ;
#if 0

    
    //Check header
    
   RX_Header =  m32Merge(gbuf[0]);
   SendString("\r12");
   if (RX_Header != 0x31413242)  //"1A2B3C4D"
    	goto RX_err;
    	    	
 	//Check Checksum
    RX_Checksum =  m32Merge(gbuf[60]);	
 	CalChecksum = 0;  	
	for(Cnt=0;Cnt<60;Cnt++)
	{
		CalChecksum += 	gbuf[Cnt];
	}
	
	if (CalChecksum == RX_Checksum)
		goto RX_OK;
RX_err:
	//clear Buff    	
 	for(Cnt=0;Cnt<60;Cnt++)
	{
		gbuf[Cnt] = 0;
	}
 SendString("\r13");
#endif
RX_OK:   
//	test = 0;
	GetDataIdInfo( D_USB_HID, &SectorAddress, &DataLength );
	spif_resv_read(SectorAddress,CalibrationData);
	HID_INDEX   =  CalibrationData[0];
//	HID_INDEX ++ ;
	if( HID_INDEX < 5 )
	{
		i =  HID_INDEX  * 8 + 1 ;
		memcpy( &CalibrationData[i], &ep0_buf[1], 8 );
		HID_INDEX ++ ;
	}
	else
	{
		i = 33  ;     //( 5 - 1 ) * 8 + 1 ;
		HID_INDEX = 5 ;
		memcpy( &CalibrationData[33], &ep0_buf[1], 8 );
	}
	CalibrationData[0] = HID_INDEX ;
	
#if 0//def SDBOOT_KBDM9
	      sdtl_resv_write( SectorAddress, CalibrationData );
#else
//#if 1//def SPIBOOT_KBDM9
	spif_resv_write( SectorAddress, CalibrationData );
#endif
    
//	SendString("\r14");
}

report_rx_func HID_set_report( unsigned int rx_len )
{
    HID_rx_len = rx_len;
//	SendString("\r15");
    return HID_report_rx;
}

void HID_get_report( __X unsigned char* buf, unsigned int length )
{
	unsigned char CalChecksum;
	unsigned int Cnt;
	unsigned long SectorAddress,DataLength;  
	
#if 1
	if(cmd_type == CMD_UPDATA_FW)
	{
		if(updata_flag & FW_UPDATA_TRUE)
			updata_flag = FW_TO_BOOT ;
//		else
//			return ;
	}
#endif

	
		Cnt2++;
	if(updata_flag == FW_TO_BOOT)
	{
		memset(CalibrationData,0,sizeof(CalibrationData));
		CalibrationData[0] = 0xFF;
		CalibrationData[1] = 0x55;
		CalibrationData[2] = 0xAA;
	}
	else
	{
		//	SendString("\r16");
	GetDataIdInfo(D_USB_HID,&SectorAddress,&DataLength);
#if 0  
           	sdtl_resv_read(SectorAddress,CalibrationData);	  
#else
    spif_resv_read(SectorAddress,CalibrationData);
#endif 
		
	}



	/*
	for( Cnt = 0 ; Cnt < 64; Cnt ++ )
	{
		if((Cnt%8)==0)
			SendString("\r\n");
		send_hex_u8( CalibrationData[ Cnt ] );
		SendString(" ");
		
	}
	//Test
	
*/
	memcpy(gbuf1, CalibrationData, 64);
	CalChecksum = 0 ;
	for(Cnt=0;Cnt<41;Cnt++)
	{
		CalChecksum ^= 	gbuf1[Cnt];
	}
	gbuf1[ 41 ]	= CalChecksum ;
	/*
	SendString("\rstart to send data");
	for( Cnt = 0 ; Cnt < 64; Cnt ++ )
	{
		if((Cnt%8)==0)
			SendString("\r\n");
		send_hex_u8( gbuf1[ Cnt ] );
		SendString(" ");
	}
	*/
	memcpy(&ep0_buf[1], gbuf1, 64);    
	length += 0;
	Flag = 0;
	SendString((U8 *)"\r\n18");
}

Bool updata_fw(void)
{
#if 1
	int set_vol_;
	unsigned int i,idx;
	unsigned int CheckSum1,CheckSum2;
	unsigned long read_size;
	unsigned long SPIF_total_size;
	unsigned long file_size;
	unsigned long offset;
	FS_ERR fserr;
	SPIF_ERR spif_err;

	__X FCFILE *fshandle;

//	IP_INIT();
//	SystemClock(FEQ_60M);
//	SDKLoadCodInit();
	/*
	__asm{
		BCLR PCEN.6
		BCLR PCDIR.6
		BSET PCOD.6
	}
*/
 // Initial the file system base on SD card.
	FSInit(1);
	fserr = FS_mount(&SD_Drive);
	if( fserr == FS_DRIVE_ERROR )
	{
//	Leave_fail();//ZOE
		SendString((U8 *)"\r\nu1");
		return FALSE ;
	}
	
	// Initial the serial flash.
	if((spif_err = spif_mount()) != SPIF_No_Err)
	{
		SendString((U8 *)"\r\nu2");
	//		Leave_fail();//ZOE
		return FALSE ;
	}
		
	// Configure the flash to read/write mode.
	SPIF_Read_Write();
	
	// Get the flash sector size of total usable space.
	SPIF_total_size = spif_get_size()+spif_resv_size()+4;
	file_size = FSGetFileSize(SPI_BURN_FILE_NAME,&fserr);
	if(file_size < 1024U * 10) //filesize < 100k
	{
		SendString((U8 *)"\r\nno file exist");
		return FALSE ;
	}
	//if(SPIF_total_size < file_size>>9 ){//ZOE_MODIFY
	if(SPIF_total_size < (file_size>>9) )
	{//ZOE_MODIFY
//	Leave_fail();//ZOE
		SendString((U8 *)"\r\nu3");
		return FALSE ;
	}

	// Open Burn File and seek position to 4096.
	fshandle = FSOpen(SPI_BURN_FILE_NAME,FS_MODE_READ,&fserr);
	fserr = FSSeek(fshandle,SPIF_SECTOR_SIZE);

	// Ereas first sector(4K Bytes)
	if(SPIF_Sector_Erase(0)>0)
	{
		
		SendString((U8 *)"\r\nu4");
//		Leave_fail();//ZOE
		return FALSE ;
	}

	
	// set index = 1
	idx = 1;

    // Burn code loop.
	while((read_size = FSRead(fshandle,buffer,SPIF_SECTOR_SIZE,&fserr)) > 0) // read 4K bytes data from burn file to buffer.
	{
		// get check sum 1.
		for(i=0,CheckSum1=0;i<read_size;i++)
			CheckSum1 += buffer[i];
		offset = SPIF_SECTOR_SIZE*idx;
		
		// erese sector
		if(SPIF_Sector_Erase(offset)>0)
		{
//			Leave_fail();//ZOE
			SendString((U8 *)"\r\nu5");
			return FALSE ;
		}

		// write data from buffer to flash
		if((spif_err = SPIF_Page_Program(offset,buffer,SPIF_SECTOR_SIZE/PAGE_SIZE)) != SPIF_No_Err)
		{
		//	Leave_fail();//ZOE
			SendString((U8 *)"\r\nu6");
			return FALSE ;
		}


		// read data from flash to buffer
		if((spif_err = SPIF_Read_Byte(offset,buffer,SPIF_SECTOR_SIZE)) != SPIF_No_Err)
		{
		//	Leave_fail();//ZOE
			SendString((U8 *)"\r\nu7");
			return FALSE ;
		}

		// get check sum 2
		for(i=0,CheckSum2=0;i<read_size;i++)
			CheckSum2 += buffer[i];

		// compare check sum
		if(CheckSum1 != CheckSum2)
		{
			SendString((U8 *)"\r\nu8");
			return FALSE ;
		}


		// clear buffer.
		for(i=0;i<SPIF_SECTOR_SIZE;i+=2)
			*((unsigned int*)&buffer[i]) = 0xFFFFu;
		SendString((U8 *)"\r\nuu");
		// add index
		idx++;
		/*
		__asm{
			PUSH R0
			BSET R0.0
			BXOR PCOD.6,R0.0
			POP R0
		}
		*/
	}
	
	// seek burn file position to 0 
	fserr = FSSeek(fshandle,0);
	
	// read first 4K bytes from flash to buffer
	read_size = FSRead(fshandle,buffer,SPIF_SECTOR_SIZE,&fserr);
	for(i=0,CheckSum1=0;i<read_size;i++)
		CheckSum1 += buffer[i];
	if((spif_err = SPIF_Page_Program(0,buffer,SPIF_SECTOR_SIZE/PAGE_SIZE)) != SPIF_No_Err)
	{
		SendString((U8 *)"\r\nu9");
		return FALSE ;
	}
	if((spif_err = SPIF_Read_Byte(0,buffer,SPIF_SECTOR_SIZE)) != SPIF_No_Err)
	{
		SendString((U8 *)"\r\nu10");
		return FALSE ;
	}
	for(i=0,CheckSum2=0;i<read_size;i++)
		CheckSum2 += buffer[i];
	if(CheckSum1 != CheckSum2)
	{
	//	  Leave_fail();//ZOE
		return FALSE ;
	}
	FSClose(fshandle);
	SendString((U8 *)"\r\nutrue");
	return TRUE ;
//	__asm{BCLR PCOD.6}
	// burn code is finish, play finish music
#if 1
/*
  i=0;
  while(i++<20){
		RedLed_FLASH();
		BlueLed_OFF();
		Delay_ms(200);
	}
  */
//	FSDelete(SPI_BURN_FILE_NAME);
//   while(1){
//   	  __asm("reset");
//   	}
#endif


#else
	__X FCFILE 	*dump_file = NULL;
	FS_ERR	err;
	SPIF_ERR spif_err;
	int	index;

	// Configure the flash to read/write mode.
	SPIF_Read_Write();
	dump_file=FSOpen(dump_file_name,FS_MODE_CREATE,&err);
	if(dump_file == NULL||err!=FS_NO_ERROR)
		return	FALSE;
	FSResize(dump_file,1024*1024UL);
	for(index = 0;index<2048;++index)
	{
		if(spif_read(index,sector_buffer)!=SPIF_No_Err)
			goto	__error;
		FSSeek(dump_file,(index*512UL));
		FSWrite(dump_file,sector_buffer,sizeof(sector_buffer),&err);
		if(err!=FS_NO_ERROR)
			goto	__error;
	}
	FSClose(dump_file);
	return	TRUE;
__error:
	FSClose(dump_file);
	return	FALSE;
#endif	
}

