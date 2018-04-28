#pragma DATA = OPEN_MAIN_DATA
#pragma CODE = OPEN_MAIN_CODE
#include <sfr.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <KBDM9_DDK.H>
#include <ctype.h>
#include "com.h"
#include "power.h"
#include "FileSearch.h"
#include "adc.h"
#include "conf_kb.h"
#include "system.h"
//nclude "globalvariable.h"
///#include "BinaryImage.h"
/*
#define _USBCON_b0 ((_bitref)0x410)
//void Delay_ms(int count);

#define OP_DET_DIR (_PCDIR_b0+2)
#define OP_DET_PH (_PCPH_b0+2)
#define OP_DET_OD (_PCOD_b0+2)
*/
extern int OtpVerify128Bit(__X unsigned int *ptr);
/*
extern unsigned int gflag;
extern unsigned int ap4_mp3_flag;
extern unsigned int vol_idx;
extern unsigned int volume_inc_flag;
extern unsigned int systerm_flag;
extern unsigned int prev_task;
extern unsigned int current_wav_num;
extern unsigned int adc0_value;
extern unsigned int adc2_value;
extern unsigned int adc3_value;
extern unsigned int adc7_value;
*/
#ifdef NIEQING_DIANCHI
unsigned long nieqing_time_count;
_MEM_TYPE_SLOW_ U8  nieqing_count;
const unsigned long TIMER_COUNT = 300;
#endif
//extern unsigned int bak_ap4_num;
//extern unsigned int bak_book_num;
//extern unsigned int bak_page_num;
//extern unsigned int bak_control_mode_num;

//__X unsigned int	otpdata128[8] = {0x0007u,0x5555u,0x5115u,0x0010u,0x6AD8u,0x0D91u,0x0000u,0x1E51u};
//__X unsigned int	otpdata128_test[8] = {0x0001u,0xAAAAu,0x4321u,0x1999u,0x9527u,0x0097u,0xFFFFu,0x9797u};

//__X unsigned int	otpdata128[8] = {0xE000u,0xAAAAu,0xA88A,0x0800u,0x1B56u,0x89B0u,0x0000u,0x8A78u};
const unsigned int	otpdata128_test[8] = {0x0001u,0x5555u,0x4321u,0x0800u,0x9527u,0x0097u,0xFFFFu,0xB458u};
const unsigned int	otpdata128_test_2[8] = {0xFFFEu,0xFFF0u,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu,0xFFFFu};
/*****************************************************************************/

unsigned long cal_data_1;
unsigned long cal_data_2;
__X U16 CalData[ 256 ];

#define USB_READY_COUNT    30
unsigned long usb_count;

#define	find_path_a(a,b)	find_path_u(atou(a),b) 
Bool	find_path_u(U16 * path_str,U8 * short_path);
U16 *	atou(U8 * astr);
void Delay_ms(int count);

void mmc_sd_all_mode_dir_make( void );


__X unsigned int Cache[256*CacheNum];
//------------------------------------------------------------------------------
void GPIO_init(void)
{
	PBEN = 0;
	PCEN = 0;
	PDEN = 0;
#if (LI_DIANCHI_DIANYA == ENABLE)
	_bitSET(Lidian_EN_DIR);//锂电充电检查pin
	_bitSET(Lidian_EN_PH); 
#endif	
	_bitCLR(PWR_CTRL_DIR); // Set PWR_CTRL with Output.

	//_bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	//_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.

    _bitCLR(BLUE_LED_DIR);
	
	_bitCLR(RED_LED_DIR);

    //_bitCLR(RED_LED_DIR);
	//_bitCLR(RED_LED_DIR);

//	_bitCLR(BLUE2_LED_DIR);
//	_bitCLR(RED2_LED_DIR);	
	
	_bitCLR(CTL_OID_DIR);
	_bitSET(CTL_OID_OD);
	
	_bitSET(HP_DET_DIR);
	_bitSET(HP_DET_PH);
/*
	_bitSET(USB_DET_DIR);
	_bitCLR(USB_DET_PH);
*/

	_bitCLR(OP_DET_DIR);
	_bitSET(OP_DET_PH);

	_bitCLR(EX_DET_DIR);
	_bitSET(EX_DET_PH);
/*
	 sd_det_disable();
   _bitCLR( SD_DET_PIF );
    PBICR = 0x00;
   _bitSET( SD_DET_DIR );
  // _bitSET( SD_DET_PH );
  	_bitCLR( SD_DET_PH );
	*/
}




#ifdef NIEQING_DIANCHI

void Charge_Init(void)
{
	if(USB_DET()){
		Charge_EN_OUT();  //set output
	//	Charge_STA_IN();  //input
		//Charge_Disable();
		//Charge_NO_PH();//state input no_pullup 
		Charge_Enable();//使能充电
		Charge_STA_IN();
		heigh_num = 0;
		low_num = 0;
		qieqing_stop_timer();
		qieqing_start_timer(1000);
		}
	else{
		Charge_EN_OUT();  //set output
		Charge_Disable();//使能充电
		charge_state_output();
		charge_state_islow();
		qieqing_stop_timer();
		}
}



void Charge_Check(void)
{

if(qieqing_is_timer_timeout()){
	qieqing_start_timer(QIEQING_HALF_TIME);
	RedLed_FLASH();
	BlueLed_FLASH();
		if(Charge_State()){
			heigh_num++;
			}
		else{
			low_num++;
			}
		if((heigh_num>0)&&(low_num>0)){
			heigh_num = 0;
			low_num = 0;
			}
#ifdef TX_DEBUG		
	 SendString("\r\nheight:low=");
	 send_hex_u16(heigh_num);
	  SendString(":");
	 send_hex_u16(low_num);
#endif			
		if(((heigh_num>30)&&(low_num==0))||((heigh_num==0)&&(low_num>=30))){

			BlueLed_ON();
			RedLed_ON();
			Charge_Disable();
			charge_state_output();
			charge_state_islow();
			qieqing_stop_timer();
			}
		
		}

	
}

#endif




Bool otpverify(void)
{

	int	err;
#if 1
// 	err = OtpVerify128Bit(otpdata128);
    __X unsigned int	temp_sec[8];
   temp_sec[0] = otpdata128_test[0]+0xDFFFu;
   temp_sec[1] = ~otpdata128_test[1];
   temp_sec[2] = otpdata128_test[2]+0x6569u;
 //  temp_sec[3] = otpdata128_test[3]&0x2012;
   temp_sec[3] = otpdata128_test[3];
   temp_sec[4] = otpdata128_test[4]-0x79D1u;
   temp_sec[5] = otpdata128_test[5]+0x8919u;
   temp_sec[6] = otpdata128_test[6]%3;
   temp_sec[7] = otpdata128_test[7]-0x29E0u;
 	err = OtpVerify128Bit(temp_sec);
    if(err ==0)return TRUE;
#if 0
   temp_sec[0] = otpdata128_test_2[0]+0x0001u;
   temp_sec[1] = otpdata128_test_2[1]+0x000F;
   temp_sec[2] = otpdata128_test_2[2];
   temp_sec[3] = otpdata128_test_2[3];
   temp_sec[4] = otpdata128_test_2[4];
   temp_sec[5] = otpdata128_test_2[5];
   temp_sec[6] = otpdata128_test_2[6];
   temp_sec[7] = otpdata128_test_2[7];
   err = OtpVerify128Bit(temp_sec);
    if(err ==0)return TRUE;
#endif
	
	return FALSE;
	//Return 0 , Verify 128Bits OK
	//Return -1 , Verify 128Bits Error
	//Return -2 , FUSCON bit 12 Error
	//Return -3 , OTPVHH Pin >= 6.5v

//	while(1);
#endif
#if 0
// 	err = OtpVerify128Bit(otpdata128);
 	err = OtpVerify128Bit(otpdata128);
    if(err == 0)return TRUE;
	else return FALSE;
	//Return 0 , Verify 128Bits OK
	//Return -1 , Verify 128Bits Error
	//Return -2 , FUSCON bit 12 Error
	//Return -3 , OTPVHH Pin >= 6.5v

//	while(1);
#endif
}


#if 0
__asm{
_Delay_ms:
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


void ChangeToUSBMode(unsigned int CodeID)
{
	USBStoragePoweroff();
	Delay_ms(50);
	if(_bit_get(_USBCON_b0+13))
	{
		CommonLoadCode(CodeID);				
	}
}
#endif

#if 1


__asm{
_Delay_ms:
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

#if 1
void begin_CheckUSB(void)
{
	if(!b_usb){
		    if(USB_DET()) // Check USB VBUS
		    {	
		    		usb_count ++;
		            //Delay_ms(500); // VBUS(5V) Debounce
		            //if(USB_DET()) // Check USB VBUS
		            if( usb_count >= USB_READY_COUNT)
		            {
		                //if(ChangeToUSBMode())
		                    CommonLoadCode(C_USB);
		                    //CommonLoadCode(C_USB_HID);
		                // Is not connect with USB Host.
		                b_usb = FALSE; 
		            }

				   if( adc3_value < 0x300){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nready UPGBURNSPI!");
#endif					   	
				   	 CommonLoadCode(C_UPGBURNSPI);
				   	}
		    }else{
		    	}
		}
	else{
		usb_count = 0;
		}
}
#else
void begin_CheckUSB(void)
{
    if(USB_DET()) // Check USB VBUS
    {
#ifdef LI_DIANCHI	    
        BlueLed_ON();
		RedLed_ON();
#endif		
        if(!b_usb)
        {
            Delay_ms(500); // VBUS(5V) Debounce
            if(USB_DET()) // Check USB VBUS
            {
                if(ChangeToUSBMode())
                    CommonLoadCode(C_USB);
                    //CommonLoadCode(C_USB_HID);
                // Is not connect with USB Host.
                b_usb = FALSE; 
            }
        }
    }
    else
    {
        b_usb = FALSE; 
    }
}
#endif
#endif



Bool read_spi_data(void)
{
	__X U16 TempBuf[ 256 ];
	
	__X U32 DataStartSector;
	__X U32 DataLength;
	//__X U16 CalData[ 256 ];

	__X U16 temp_i;
	SPIF_ERR cur_err;
	//cal_data_1 = 0;
	//cal_data_2 = 0;
	
		 memset( CalData, 0, sizeof( CalData ));
	
		if( spif_mount() != SPIF_No_Err )
			   {
#ifdef TX_DEBUG
			       SendString("\r\nspi mount fail!");
#endif				   
				   return FALSE;
			   }
				   
			   // Configure the flash to read/write mode.
		  //SPIF_Read_Only();
		  SPIF_Read_Write();

		  //DataStartSector = (U32)(4086UL<<10);
		  //DataStartSector = 1040384UL;
		  DataStartSector = SAVE_OID_CAL_ADDR;
	      //if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
	      // cur_err = spif_write(DataStartSector,(__X void *)CalData);
	      if( SPIF_Read_Byte( DataStartSector, (__X unsigned char *)&TempBuf, sizeof(TempBuf) ) != SPIF_No_Err )
	     /// if(cur_err != SPIF_No_Err)
	      {
	         SendString("\r\nspi read error");
	         return FALSE;
	      }
			  
			SendString("\r\nrecover data = ");
			for(temp_i=0;temp_i<10;temp_i++){
				send_hex_u16(TempBuf[temp_i]);
				SendString(":");
				}
			
			if((TempBuf[0] == 0x1111)&&(TempBuf[1] == 0x2222)
				&&(TempBuf[8] == 0x3333)&&(TempBuf[9] == 0x4444)){
				
				CalData[ 0 ] = TempBuf[ 2 ];
		  		CalData[ 1 ] = TempBuf[ 3 ];
		   		CalData[ 2 ] = TempBuf[ 4 ];
		   		CalData[ 3 ] = TempBuf[ 5 ];
				CalData[ 4 ] = TempBuf[ 6 ];
				CalData[ 5 ] = TempBuf[ 7 ];
				
				MSW( t01_calib_data1 ) = TempBuf[ 2 ];
	   			LSW( t01_calib_data1 ) = TempBuf[ 3 ];
	   			MSW( t01_calib_data2 ) = TempBuf[ 4 ];
	   			LSW( t01_calib_data2 ) = TempBuf[ 5 ];
				t01_calib_data3  = TempBuf[ 6 ];
				
				if((t01_calib_data1 == 0)||(t01_calib_data1 ==0)){
					return FALSE;
					}
				return TRUE;
				}
			return FALSE;
}


void recover_oid_cal(void)
{

	__X U16 TempBuf[ 256 ];
	
	__X U32 DataStartSector;
	__X U32 DataLength;
	

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


				//memset( CalData, 0, sizeof( CalData ));
				//CalData[ 0 ] = TempBuf[ 2 ];
		  		//CalData[ 1 ] = TempBuf[ 3 ];
		   		//CalData[ 2 ] = TempBuf[ 4 ];
		   		//CalData[ 3 ] = TempBuf[ 5 ];
				//CalData[ 4 ] = TempBuf[ 6 ];
				//CalData[ 5 ] = TempBuf[ 7 ];
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
			     cur_err = spif_resv_write( DataStartSector, CalData );
#endif

#ifdef TX_DEBUG
			if(cur_err != SPIF_No_Err){
				SendString("\r\nrecover fail!");
				}
			else{
				SendString("\r\nrecover ok!");
				}
#endif			

}



//--------------------------------------------------------------------
void test_2sd(void)
{
	U16 i, temp_data;
	SendString("\r\ntest sd!");
	temp_data = file_get_ext_count((char *)"0:\\MP3",(char *)"MP3");
	for(i=0;i<temp_data;i++){
		if(file_get_path((char *)"0:\\MP3",(char *)"MP3", i)){
			SendString("\r\ni:current_paht=");
			send_hex_u16(i);
			SendString(":");
			SendString(g_full_path);
			}
		}
	temp_data = file_get_ext_count((char *)"1:\\MP3",(char *)"MP3");
	for(i=0;i<temp_data;i++){
		if(file_get_path((char *)"1:\\MP3",(char *)"MP3", i)){
			SendString("\r\ni:current_paht=");
			send_hex_u16(i);
			SendString(":");
			SendString(g_full_path);
			}
		}	
	SendString("\r\ntest sd ok!");
}

void test_adc(void)
{
	unsigned int i,j;
	PWR_ON();
	 while(1){
	 	 PWR_ON();
	     adc0_value = Get_ADCValue((unsigned int)0 );
		 Delay_ms(40);
		 adc2_value = Get_ADCValue((unsigned int)2 );
		 Delay_ms(40);
		 adc3_value = Get_ADCValue((unsigned int)3 );
	 	 Delay_ms(40);
		 adc7_value = Get_ADCValue((unsigned int)7 );
		 Delay_ms(40);
	     SendString("\r\nadc0:adc2:adc3:adc7:key:bat=");
	     send_hex_u16(adc0_value);
		 SendString(":");
	     send_hex_u16(adc2_value);
		 SendString(":");
		 send_hex_u16(adc3_value);
		 SendString(":");
	     send_hex_u16(adc7_value);
		 SendString(":");
		 i = (unsigned int)((adc3_value*100UL)/adc7_value);
	     send_hex_u16(i);
		  SendString(":");
		  j = (unsigned int)((adc0_value*100UL)/adc7_value);
	     send_hex_u16(j);
	 	}
	
}

void ClearRemote(void)
{
	U8 i;
	for(i = 0; i < REMOTE_INDEX_MAX; i ++){
		remote_index_tab[i] = 0;
		}
}

void read_bak_news(void)
{
   	__X U8 TempBuf[ 512 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
	__X U8  bakVol;
	__X U8 temp_leng;
	

     GetDataIdInfo( D_DATA, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
               nftl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SDBOOT_KBDM9
               sdtl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SPIBOOT_KBDM9
               spif_resv_read( DataStartSector, TempBuf );
#endif
#ifdef TX_DEBUG
		//SendString("\r\nread bak data sec len =");send_hex_u32(DataStartSector);
		//SendString("\r\n:");send_hex_u32(DataLength);
		//SendString("\r\nread bak data=");
		//for(temp_leng = 0;temp_leng < DataLength;temp_leng++){
		//	send_hex_u8(TempBuf[temp_leng]);SendString(":");
		//	}
#endif	 

		MSB(current_wav_num) = TempBuf[0];
		LSB(current_wav_num) = TempBuf[1];
		bakVol = TempBuf[2];
		if(bakVol >= 5)bakVol = 2;

		vol_idx = bakVol;
		if(current_wav_num == 0)current_wav_num = 0;
		if(current_wav_num >= 100)current_wav_num = 0;
#ifdef TX_DEBUG
	   SendString("\r\ncurrent_wav_num:bakVol=");
	   send_hex_u16(current_wav_num);
	   SendString(":");
	   send_hex_u8(vol_idx);
#endif	
}



#if( T01OID_CAB == ENABLE )  
void read_cab(void)
{
   	__X U16 TempBuf[ 256 ];
    __X U32 DataStartSector;
    __X U32 DataLength;
	__X U16 temp_i;
	SPIF_ERR cur_err;
	t01_calib_data1 = 0;
	t01_calib_data2 = 0;
     GetDataIdInfo( D_CAB, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
               nftl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SDBOOT_KBDM9
                cur_err = sdtl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SPIBOOT_KBDM9
               cur_err = spif_resv_read( DataStartSector, TempBuf );
#endif
#if 0//def TX_DEBUG
		SendString("\r\nerr:read cab data=\r\n");
		send_hex_u16(cur_err);
		SendString("\r\n");
		for(temp_i=0;temp_i<(U16)DataLength;temp_i++){
			send_hex_u16(TempBuf[temp_i]);
			SendString(":");
			}
#endif	 
	if( TempBuf[( DataLength >> 1)- 1 ] != 0xA55A ){
			
			if(read_spi_data()){
				recover_oid_cal();
				//t01_calib_data1 = cal_data_1;
				//t01_calib_data2 = cal_data_2;
				}
			else{
				PowerKeepOn();
				CommonLoadCode(C_CAB);
				}
		}
	else{

#if 1
	   MSW( t01_calib_data1 ) = TempBuf[ 0 ];
	   LSW( t01_calib_data1 ) = TempBuf[ 1 ];
	   MSW( t01_calib_data2 ) = TempBuf[ 2 ];
	   LSW( t01_calib_data2 ) = TempBuf[ 3 ];
	   t01_calib_data3 = TempBuf[ 4 ];
#else
	   MSW( t01_calib_data1 ) = TempBuf[ 1 ];
	   LSW( t01_calib_data1 ) = TempBuf[ 0 ];
	   MSW( t01_calib_data2 ) = TempBuf[ 3 ];
	   LSW( t01_calib_data2 ) = TempBuf[ 2 ];
#endif	   
		}
#ifdef TX_DEBUG
	   SendString("\r\ncal=");
	   send_hex_u32(t01_calib_data1);
	   SendString(":");
	   send_hex_u32(t01_calib_data2);
	    SendString(":");
	   send_hex_u16(t01_calib_data3);
#endif	   

}
#endif //


Bool update_firmware_deal( void )
{
   FS_ERR fs_err;
   __X U32 file_size;
	char temp_filename[128];
	char tisiFileShortpath[64];
/*
    strcpy( temp_filename, "1:\\UP.AFA" );
   if(FSGetFileSize((char *)temp_filename,&fs_err)>0){
		FSDelete(( char *)temp_filename );
	}
*/

   strcpy( temp_filename, "0:\\up.afa" );
   if( find_path_a(( U8 *)temp_filename, (U8 *)tisiFileShortpath )){
   		FSDelete(( char *)tisiFileShortpath );
   	}

   

    strcpy( temp_filename, "0:\\update.bin" );
   if( !find_path_a(( U8 *)temp_filename, (U8 *)tisiFileShortpath ))
      return FALSE;

   file_size = FSGetFileSize(( char *)tisiFileShortpath, &fs_err );
   
/*
   strcpy( temp_filename, "1:\\up.afa" );
   if(FSGetFileSize((char *)temp_filename,&fs_err)>0){
		FSDelete(( char *)temp_filename );
	}

   strcpy( temp_filename, "1:\\update.bin" );


   file_size = FSGetFileSize(( char *)temp_filename, &fs_err );
//*/ 
   if( fs_err != FS_NO_ERROR )
       return FALSE;
   if( file_size <( 1ul*1024ul ))
       return FALSE;
   if( file_size > SAVE_OID_CAL_ADDR )
       return FALSE;
	
   FSRename(( char *)tisiFileShortpath, "up.afa" );
   return TRUE;
  
}



Bool updateFireware( void )
{
   FS_ERR fs_err;
   __X U32 file_size;
	char temp_filename[128];
	char tisiFileShortpath[64];
	U16 tempData = 0;
    __X FCFILE *REC_File;
	__X U8 TempBuf[ 4 ];
	__X U8 Buf[ 12 ];
/*
    strcpy( temp_filename, "1:\\UP.AFA" );
   if(FSGetFileSize((char *)temp_filename,&fs_err)>0){
		FSDelete(( char *)temp_filename );
	}
*/

   strcpy( temp_filename, "0:\\up.afa" );
   if( find_path_a(( U8 *)temp_filename, (U8 *)tisiFileShortpath )){
   		FSDelete(( char *)tisiFileShortpath );
   	}

    tempData = file_get_ext_count((char *)"0:",(char *)"BIN");
#ifdef TX_DEBUG
	SendString(( char *)"\r\nbin_num = ");
	send_hex_u16(tempData);
#endif		
	if(tempData == 0)return false;
	
		if(!file_get_path((char *)"0:",(char *)"BIN", 0)){
#ifdef TX_DEBUG
			SendString(( char *)"\r\nfind update file fail!");
#endif	
			return false;
			}
#ifdef TX_DEBUG
	SendString(( char *)"\r\nfull_path = ");
	SendString(g_full_path);
#endif	
    strcpy( temp_filename, g_full_path );
   if( !find_path_a(( U8 *)temp_filename, (U8 *)tisiFileShortpath ))
      return FALSE;

   file_size = FSGetFileSize(( char *)tisiFileShortpath, &fs_err );
   
/*
   strcpy( temp_filename, "1:\\up.afa" );
   if(FSGetFileSize((char *)temp_filename,&fs_err)>0){
		FSDelete(( char *)temp_filename );
	}

   strcpy( temp_filename, "1:\\update.bin" );


   file_size = FSGetFileSize(( char *)temp_filename, &fs_err );
//*/ 
   if( fs_err != FS_NO_ERROR )
       return FALSE;
   if( file_size <( 1ul*1024ul ))
       return FALSE;
   if( file_size > SAVE_OID_CAL_ADDR )
       return FALSE;


	REC_File = FSOpen((char *)tisiFileShortpath,FS_MODE_READ_WRITE,&fs_err); 

	(void)FSSeek(REC_File,0);
	(void)FSRead(REC_File, (void *)&TempBuf,sizeof(TempBuf), &fs_err);
	(void)FSSeek(REC_File,0x00000020);
	(void)FSRead(REC_File, (void *)&Buf,sizeof(Buf), &fs_err);
	FSClose(REC_File);
	if(TempBuf[0] != 0x42)return false;
	if(TempBuf[1] != 0x4F)return false;
    if(TempBuf[2] != 0x4F)return false;
	if(TempBuf[3] != 0x54)return false;

	if(Buf[0] != 0x53)return false;
	if(Buf[1] != 0x50)return false;
	if(Buf[2] != 0x49)return false;
	if(Buf[3] != 0x46)return false;
	if(Buf[4] != 0x32)return false;
	if(Buf[5] != 0x30)return false;
	if(Buf[6] != 0x30)return false;
	if(Buf[7] != 0x39)return false;	
	if(Buf[8] != 0x31)return false;
	if(Buf[9] != 0x32)return false;
	if(Buf[10] != 0x31)return false;
	if(Buf[11] != 0x35)return false;

   FSRename(( char *)tisiFileShortpath, "up.afa" );
   return TRUE;
  
}


void ready_mount_sd(void)
{
	FS_ERR fserr;
	U8 i;
	//char temp_lab[12];
	//char sys_lab[12];
	//char digi[6];
	b_sd1_ok = FALSE;
	b_sd2_ok = FALSE;
	FSInit();
#if(USE_SD1 == 1)
	fserr = FS_mount( 0, &SD1_Drive_Cache );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 fail!");
#endif	
		}
	else{
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd1 ok!");
#endif		
		SDCKCR = 0x03;
		SD1_Cache_Open( Cache, CacheNum );
		b_sd1_ok = TRUE;
		}
#endif	

#if(USE_SD2 == 1)
	fserr = FS_mount( 1, &SD2_Drive );
	if( fserr != FS_NO_ERROR ){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd2 fail!");
#endif	
		}
	else{
#ifdef TX_DEBUG
		SendString(( char *)"\r\nmount sd2 ok!");
#endif			
		b_sd2_ok = TRUE;
		SDCKCR = 0x03;
		SD2_Cache_Open( Cache, CacheNum );
		}
#endif
}

void set_sd_volume(void)
{
	char temp_lab[12];
	char sys_lab[12];
	char digi[6];
	if(b_sd1_ok){
	   // digi[0] = HARD_VERSION;
		//digi[1] = MAJOR_FW_VERSION;
		//digi[2] = MINOR_FW_VERSION;
		//digi[3] = 0;
		memset(sys_lab,0,12);
		strcpy(sys_lab,SYS_LAB_1);
		//strcat(sys_lab,digi);


    	FSVolume(0,temp_lab,0);
		if(strcmp(temp_lab,sys_lab))
			{
	     	FSVolume(0,sys_lab,1);
			}
	}
	if(b_sd2_ok){
	   // digi[0] = HARD_VERSION;
		//digi[1] = MAJOR_FW_VERSION;
		//digi[2] = MINOR_FW_VERSION;
		//digi[3] = 0;
		memset(sys_lab,0,12);
		strcpy(sys_lab,SYS_LAB_2);
		//strcat(sys_lab,digi);


    	FSVolume(1,temp_lab,0);
		if(strcmp(temp_lab,sys_lab))
			{
	     	FSVolume(1,sys_lab,1);
			}
	}
}

void test_sd(Bool flag)
{
	FS_ERR fserr;
	U8 i;
	char temp_lab[12];
	char sys_lab[12];
	char digi[6];

	if((!b_sd1_ok)&&(!b_sd2_ok)){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nno sd!");
#endif		
		BlueLed_OFF();
		RedLed_ON();
		i=0;	
		while(i<20){
			if(rec_led_is_timer_timeout()){
						BlueLed_OFF();
						RedLed_FLASH();
						rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
						i++;
				}
			}
		  Exception();
		}
#ifdef TX_DEBUG
		SendString(( char *)"\r\nsd 0K!");
#endif	

	
}

void main(void)
{
/*
// float a,b,c;
 a=123456789;
 b=765432.15;
 c=a/b;
// d=a%b;
*/	
    FS_ERR fserr;
    unsigned int j,lidian_num,key_temp_count = 0;
	unsigned int keyCount = 0;
	Bool lidian_flag = FALSE;
	IP_INIT();
	Int_enable();

	

	SystemClock(SYS_CLOCK);
	GPIO_init();
	PWR_OFF();
	OP_CLOSE();
	BlueLed_OFF();
	RedLed_OFF();
	
	ADC_init();
	//gflag = 0;
	gflag.value = 0;
	systerm_flag.value = 0;
	sys_flag.value = 0;
 	vol_idx = 2;
	//ap4_mp3_flag =0;
	volume_inc_flag = 1;
	lidian_num = 0;
	usb_count = 0;
//	prev_task=0;
	clr_ex();
	current_sd = FLAG_SD1_CON;
	
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nKB OTP begin!");
#endif
	System_Timer3_Init();
#if 0   
    if(!otpverify())
    	{
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nOTPverify fail!");
#endif    	
//		RTCWDT = 0xBFFFU;
    	}
	else{
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nOTPverify OK!");
#endif  
		}
#endif

	 

	   //BlueLed_ON();
	   //RedLed_OFF();

	   BlueLed2_OFF();
	   RedLed2_OFF();

	  
	   
#ifdef NIEQING_DIANCHI	   
		Charge_Init();;
#endif


	 //adc2_value = Get_ADCValue((unsigned int)2 );
	 //Delay_ms(20);	

	 read_bak_news();
	// current_wav_num = 1;
	 vol_idx = 2;

#if ( T01OID_CAB == ENABLE )	
		read_cab();
#endif

	 ClearRemote();

	 ready_mount_sd();
	 set_sd_volume();
	// mmc_sd_all_mode_dir_make();
	 //test_2sd();


	//if(update_firmware_deal()){
	if(updateFireware()){
		PWR_ON();
#ifdef  TX_DEBUG
		SendString(( char *)"\r\nfirmware updagrade!");
#endif		
			CommonLoadCode(C_UPGRADE);
		}
	 
	// test_adc();	
	//b_usb = TRUE;
	//CommonLoadCode(C_MP3);
	 RedLed_ON();
	 mmc_sd_all_mode_dir_make();
	adc0_value = Get_ADCValue((unsigned int)0 );
	 Delay_ms(10);
	 adc2_value = Get_ADCValue((unsigned int)2 );
	 Delay_ms(10);
	 adc3_value = Get_ADCValue((unsigned int)3 );
	 Delay_ms(10);
	 adc7_value = Get_ADCValue((unsigned int)7 );
	 Delay_ms(10);
	 j = (unsigned int)((adc0_value*100UL)/adc7_value);
  while(1){
     
	  adc_get();
	  j =
(unsigned int)((adc0_value*100UL)/adc7_value);
	  keyCount ++;
	  if(keyCount%5 == 0){
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nad0:adc2:adc3=");
	send_hex_u16(adc0_value);
	SendString(( char *)":");
	send_hex_u16(adc2_value);
	SendString(( char *)":");
	send_hex_u16(adc3_value);
#endif	
	  	}
 

	 if(!USB_DET()){
	 	usb_count = 0;
	 // if(PWD_Key_Press()){
#ifdef  TX_DEBUG
	SendString(( char *)"\r\npower key!");
#endif		 	
	 	if(j<LVT_VALUE_BEGIN){
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nbegin low battery!:adc=");
	send_hex_u16(j);
#endif			
			key_temp_count = 0;
			BlueLed_OFF();
	   		RedLed_ON();
			RTCWDT = 0xBFFFu;
	 		}
	 	key_temp_count ++;
//		 Delay_ms(1000); // VBUS(5V) Debounce
		  if(key_temp_count >= 2){
		  	    test_sd(TRUE);
				BlueLed_ON();
				RedLed_OFF();
		  		PWR_ON();
		 		CommonLoadCode(C_READBNL);
// 		 		CommonLoadCode(C_WR_TX);
		  	}
	 	}
	 else{
#if 0//def  TX_DEBUG
			SendString(( char *)"\r\nno key!");
#endif
				
		    	//BlueLed_ON();
				//RedLed_OFF();
				BlueLed_OFF();
				RedLed_ON();
				begin_CheckUSB();
/*
			   if(PWD_Key_Press()){
#ifdef  TX_DEBUG
				SendString(( char *)"\r\npower key!");
#endif		 	
			 	key_temp_count ++;
				  if(key_temp_count >= 3){
				  		b_usb = TRUE;
				 		CommonLoadCode(C_OID);
				  	}
			   	}
			   else{
			   	key_temp_count = 0;
			   	}
			   */

#ifdef NIEQING_DIANCHI		
		Charge_Check();
#endif		
/*
#ifdef LI_DIANCHI	 
	 if(sys_led_is_timer_timeout()){
		sys_led_start_timer(SYS_LED_ON_TIME);
		 if(USB_DET()){
			 if(Lidian_ok()){
				sys_led_stop_timer();
				BlueLed_OFF();
				RedLed_ON();
				}
			else{
				BlueLed_OFF();
				RedLed_FLASH();
				}
	 		}
	 	}
#endif	 
*/
		  	}
  	}
}


#pragma CODE = FIND_PATH_CODE
//本地字符串(ascii字符串)转化为unicode字符串
void	local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size)
{
	while(*local_str)
	{
		*unicode_str++=*local_str++;
		buffer_size -= 2;
		if(buffer_size<=3)
		{
			*unicode_str = 0;
			return;
		}
	}
	*unicode_str = 0;
}
int	str_comp_u(U16  * sl,U16  * sr,U8 use_case)
{
	if(use_case)
	{
		while(*sl&&*sl==*sr)
		{
			++sl;
			++sr;
		}
		return	*sl - *sr;
	}
	else
	{
		while(*sl&&tolower(*sl)==tolower(*sr))
		{
			++sl;
			++sr;
		}
		return	tolower(*sl)-tolower(*sr);
	}
}

U16 *	atou(U8 * astr)
{
	U16 * ustr;
	ustr = (U16 *)g_string2;
	while(*astr)
	{
		*ustr++ = *astr++;
	}
	*ustr = 0;
	return	(U16 *)g_string2;
}
#define  FS_ATTR_LFN_ENTRY    ((Byte)0x0F)
//#define	find_path_a(a,b)	find_path_u(atou(a),b) 
Bool find_entry( U8 * spec, U16 *name, Bool is_dir, FILE_INFO __X *finfo, FS_ENTRY_INFO __X *entry )
{
   U16 buffer[ 25 ];
   
   //if( FSGetEntry(( char *)spec, 0, finfo, entry ) != FS_NO_ERROR )
   //   return FALSE;

   //change 2013-05-22 zoe
   unsigned int i;
   FS_ERR fserr;
   for( i = 0; ; ++i )
   {
      fserr = FSGetEntry(( char *)spec, i, finfo, entry );
      if( fserr == FS_NO_ERROR )
         break;
      if( fserr == FS_FILE_NOT_FOUND )
         continue;
      return FALSE;
   }

   do
   {
      if(( finfo->attribute & FS_ATTR_LFN_ENTRY )!= FS_ATTR_LFN_ENTRY )
      {
         local_to_unicode( finfo->name, buffer, sizeof( buffer ));
         if( str_comp_u(( U16 *)finfo->lfn, name, FALSE )== 0
            || str_comp_u( buffer, name, FALSE )== 0 )
         {
            if( is_dir )
            {
               if( finfo->attribute & FS_ATTR_DIRECTORY )
                  return TRUE;
            }
            else
            {
               if( !( finfo->attribute & FS_ATTR_DIRECTORY ))
                  return TRUE;
            }
         }
      }

      //change 2013-05-22 zoe
      fserr = FSGetNextEntry( finfo, entry );
      if(( fserr != FS_NO_ERROR )&&( fserr != FS_FILE_NOT_FOUND ))
         break;
   }
   //while( FSGetNextEntry( finfo, entry )== FS_NO_ERROR );
   while( 1 );
   
   return FALSE;
}


Bool	find_path_u(U16 * path_str,U8 * short_path)
{
	U16	left ,right;
	U16	iter;
	U16	old_char;
	FILE_INFO	__X	info;
	FS_ENTRY_INFO __X entry;
	left = 3,right = 3;

	//strcpy((char *)short_path,"0:");
	strcpy((char *)short_path,(char* )ROOT_DENTRY);
	memset(&info,0,sizeof(info));
	entry.drive = 0;
	 //FSGetEntry(( char *)short_path, 0, &info,&entry );
   {
   unsigned int i;
   FS_ERR fserr;
   for( i = 0; ; ++i )
   {
      fserr = FSGetEntry(( char *)short_path, i, &info,&entry );
      if( fserr == FS_NO_ERROR )
         break;
      if( fserr == FS_FILE_NOT_FOUND )
         continue;
      return FALSE;
   }
   }
   
   if( path_str[ left ] == '\0' )
      return TRUE;
   while( path_str[ left ] != '\0' )
   {
      while( path_str[ right ] != '\\'
         && path_str[ right ] != '/'
            && path_str[ right ] != '\0' )
         ++right;
      if( path_str[ right ] == '\\'
         || path_str[ right ] == '/' )
      {
         old_char = path_str[ right ];
         path_str[ right ] = 0;
         if( find_entry( short_path, ( U16 *)&path_str[ left ], TRUE, &info, &entry ))
         {
            //if( nav_filelist_findname(( FS_STRING )( &path_str[ left ]), FALSE ) && nav_file_isdir())
            strcat(( char *)short_path, ( char *)"\\" );
            strcat(( char *)short_path, ( char *)info.name );
            if( FSGetEntry(( char *)short_path, 0, &info, &entry )!= FS_NO_ERROR )
            {
               path_str[ right ] = old_char;
               goto __bad_end;
            }
         }
         else
         {
            path_str[ right ] = old_char;
            goto __bad_end;
         }
         path_str[ right ] = old_char;
         ++right;
         left = right;
      }
      else
      {
         if( find_entry( short_path, ( U16 *)&path_str[ left ], FALSE, &info, 
&entry ))
         {
            strcat(( char *)short_path, "\\" );
            strcat(( char *)short_path, ( char *)info.name );
            goto __end;
         }
         //else
            goto __bad_end;
      }
   }
__end:
   return TRUE;
__bad_end:
	return	FALSE;
}
//*
void mmc_sd_all_mode_dir_make( void )
{
	FS_ERR fs_err;
	U8 j;
	char tisiFileName[128];

	char dirpath[32];
	U16	m[] = {'\\','\0'};
	//U16		dictionary[] = {0x8BCD,0x5178, 0x0000};
	
	U16	story[] = {0x6545,0x4E8B,'\\',0x0000};//故事
	U16	erge[] = {0x513F,0x6B4C,'\\',0x0000};//儿歌
	//U16	music[] = {0x97F3,0x4E50,'\\',0x0000};//音乐
	U16	mp3_book[] = {0x6709,0x58F0,0x4E66,'\\',0x0000};//有声书

	U16	music[] = {0x97F3,0x4E50,0x6B23,0x8D4F,'\\',0x0000};//音乐欣赏
	U16	ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,'\\',0x0000};//有声课本
	U16	record[] = {0x5F55,0x97F3,'\\',0x0000};//录音


	U16   sd1_root[] = {'0',':','\\','\0'};
	U16   sd2_root[] = {'1',':','\\','\0'};


	
/*
		memset(tisiFileName,0,128);
		for(j=0;j<8;j++){
			tisiFileName[j]=subdir[j];
			}
		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)subdir);


		memset(tisiFileName,0,128);
		for(j=0;j<8;j++){
			tisiFileName[j]=AP4[j];
			}
		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)AP4);
*/
//		strcpy(tisiFileName,"点读文件");
//		fs_err = FSDirectory(tisiFileName);
//		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)AP4);

//		strcpy(tisiFileName,"系统文件");
//		fs_err = FSDirectory(tisiFileName);
//		fs_err =FSDirForUnicode((char *)"0:",tisiFileName,(unsigned char *)subdir);
		memset(tisiFileName,0,128);

		if(b_sd1_ok){
/*			
			//strcpy(tisiFileName,"0:\\DFO");
			//fs_err = FSDirectory(tisiFileName);
			
			//strcpy(tisiFileName,"0:\\dic");
			//fs_err = FSDirectory(tisiFileName);

			//strcpy(tisiFileName,"0:\\mp3");
			//fs_err = FSDirectory(tisiFileName);

			strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
			strcat_u((U16 *)tisiFileName,(U16 *)&ap4_book);
			//strcat_u((U16 *)tisiFileName,(U16 *)&m);
			if(!find_path_u((U16 *)&tisiFileName,(U8 *)dirpath))
				fs_err =FSDirForUnicode((char *)"0:",(char *)"AP4~1",(unsigned char *)ap4_book );

			//strcpy(SD1_AP4_DIRNAME,"0:\\AP4~1");
#if 0//def  TX_DEBUG
			SendString(( char *)"\r\nsd1_ap4_dirname=");
			SendString(SD1_AP4_DIRNAME);
#endif

			
			
			strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
			strcat_u((U16 *)tisiFileName,(U16 *)&music);
			//strcat_u((U16 *)tisiFileName,(U16 *)&m);
			if(!find_path_u((U16 *)&tisiFileName,(U8 *)dirpath))
				fs_err =FSDirForUnicode((char *)"0:",(char *)"MP3~1",(unsigned char *)music );

#ifdef  TX_DEBUG
			SendString(( char *)"\r\nsd1_mp3_dirname=");
			SendString("MP3~1");
#endif

			strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
			strcat_u((U16 *)tisiFileName,(U16 *)&record);
			//strcat_u((U16 *)tisiFileName,(U16 *)&m);
			if(!find_path_u((U16 *)&tisiFileName,(U8 *)dirpath))
				fs_err =FSDirForUnicode((char *)"0:",(char *)"REC~1",(unsigned char *)record);

#ifdef  TX_DEBUG
			SendString(( char *)"\r\nsd1_record_dirname=");
			SendString("REC~1");
#endif
//*/
			///*
			strcpy(tisiFileName,SD1_AP4_DIR);
			fs_err = FSDirectory(tisiFileName);
			
			strcpy(tisiFileName,SD1_MP3_DIR);
			fs_err = FSDirectory(tisiFileName);

			//strcpy(tisiFileName,"0:\\STORY");
			//fs_err = FSDirectory(tisiFileName);
		
			
			strcpy(tisiFileName,SD1_PASTER_DENTRY);
			fs_err = FSDirectory(tisiFileName);
			//*/	
			}
		if(b_sd2_ok){
			//strcpy(tisiFileName,"1:\\DFO");
			//fs_err = FSDirectory(tisiFileName);

			//strcpy(tisiFileName,"1:\\mp3");
			//fs_err = FSDirectory(tisiFileName);

	//		strcpy(tisiFileName,"0:\\rec");
	//		fs_err = FSDirectory(tisiFileName);		

			strcpy(tisiFileName,"1:\\AP4");
			fs_err = FSDirectory(tisiFileName);
			
			strcpy(tisiFileName,"1:\\MP3");
			fs_err = FSDirectory(tisiFileName);

			//strcpy(tisiFileName,"1:\\STORY");
			//fs_err = FSDirectory(tisiFileName);

			strcpy(tisiFileName,"1:\\record");
			fs_err = FSDirectory(tisiFileName);
			}

#if 0
		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)subdir);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)AP4);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)MP3_FILE);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);

		
		strcpy(tisiFileName,(char*)&root);
		strcat(tisiFileName,(char *)DIC_FILE);
		strcat(tisiFileName,"\\");
		fs_err = FSDirectory(tisiFileName);


		if(b_sd1_ok){
		strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
		strcat_u((U16 *)tisiFileName,(U16 *)&story);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)dirpath))
				fs_err =FSDirForUnicode((char *)"0:",(char *)"故事",(unsigned char *)story );
	}
	
	if(b_sd2_ok){	
	    strcpy_u((U16 *)tisiFileName,(U16 *)&sd2_root);
		strcat_u((U16 *)tisiFileName,(U16 *)&story);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)dirpath))
				fs_err =FSDirForUnicode((char *)"0:",(char *)"故事",(unsigned char *)story );

	}
#endif
}

//void LED_Callback(void)
//{
//}
