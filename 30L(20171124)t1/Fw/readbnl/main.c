#pragma DATA = OPEN_MAIN_DATA
#pragma CODE = OPEN_MAIN_CODE
#include <sfr.h>
#include <stdlib.h>
#include <math.h>
#include <KBDM9_DDK.H>
#include "com.h"
#include "power.h"
//#include "USB_Detect.h"
#include "adc.h"
#include "conf_kb.h"
#include "system.h"
//nclude "globalvariable.h"
//#include "BinaryImage.h"
#include <string.h>
#include "fc_internal.h"
#include <ctype.h>
#include <fs.h>
#include <string.h>
#include "filesearch.h"
/*
#define _USBCON_b0 ((_bitref)0x410)
//void Delay_ms(int count);

#define OP_DET_DIR (_PCDIR_b0+2)
#define OP_DET_PH (_PCPH_b0+2)
#define OP_DET_OD (_PCOD_b0+2)
*/
ImageDeclareType(KB_RESIZE_CODE);

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
/*****************************************************************************/
//extern char	__X	g_full_path[256];
void Delay_ms(int count);
char REC_FILENAME[128];

#define BUF_LEN 	2000
U16 BOOK_NUM_0[BUF_LEN];
U16 BOOK_NUM_1[BUF_LEN];
unsigned int g_string[32];
U16	 current_id;
  __X FCFILE *REC_File;
  __X unsigned int Cache[256*CacheNum];
char SD1_AP4_DIRNAME[32];
//char SD2_AP4_DIRNAME[32];


#define  FS_ATTR_LFN_ENTRY    ((Byte)0x0F)  
 /*
const  U8       SD1_AP4_DIR[] = "0:\\AP4";
const  U8       SD1_MP3_FILE[] = "0:\\MP3";
const  U8       SD1_DIC_FILE[] = "0:\\DIC";
const  U8       SD2_AP4_DIR[] = "1:\\AP4";
const  U8       SD2_MP3_FILE[] = "1:\\MP3";
const  U8       SD2_DIC_FILE[] = "1:\\DIC";
const U8       AP4_Ext[] = "AP4";
*/
//------------------------------------------------------------------------------
FS_ERR kb_fat_resize( PxFCFILE file, unsigned long new_size );
void mmc_sd_all_mode_dir_make( void );
void readAp4DirName(void);
void readPasterDirName(void);


//--------------------------------------------------------------------------------
/*
void GPIO_init(void)
{
	PBEN = 0;
	PCEN = 0;
//	PDEN = 0;

	_bitSET(Lidian_EN_DIR);//锂电充电检查pin
	
	_bitCLR(PWR_CTRL_DIR); // Set PWR_CTRL with Output.

	_bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.

    _bitCLR(BLUE_LED_DIR);
	_bitCLR(RED_LED_DIR);


	_bitCLR(BLUE2_LED_DIR);
	_bitCLR(RED2_LED_DIR);	
	
	_bitCLR(CTL_OID_DIR);
	_bitCLR(CTL_OID_OD);
	
	_bitSET(HP_DET_DIR);
	_bitSET(HP_DET_PH);

	_bitSET(USB_DET_DIR);
	_bitCLR(USB_DET_PH);
	_bitCLR(OP_DET_DIR);
	_bitSET(OP_DET_PH);

}
*/



#ifdef NIEQING_DIANCHI

void Charge_Init(void)
{
	if(USB_DET()){
		Charge_EN_OUT();  //set output
		Charge_Enable();//使能充电
		Charge_STA_IN();
		nieqing_time_count=0;
		}
	else{
		Charge_EN_OUT();  //set output
		Charge_Disable();//使能充电
		charge_state_output();
		charge_state_islow();
		}
}

void Charge_Check(void)
{
	if(USB_DET()){	
		while(nieqing_time_count<TIMER_COUNT){
				if(!Charge_State()){
					nieqing_time_count=0;
					return;
				}
				nieqing_time_count++;
				Delay_ms(10);
			}
		if(nieqing_time_count>=TIMER_COUNT){
			Charge_Disable();
			charge_state_output();
			charge_state_islow();
			}	
		}

}
#endif




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


void save_wav_num(void)
{
#if 1
    __X FS_ERR err;
  unsigned long temp_len;

  __X FCFILE *Temp_File;
   char temp_file[]="0:\\rec\\pos.dat";
   
	Temp_File = FSOpen((char *)temp_file,FS_MODE_READ_WRITE,&err);   
	FSSeek(Temp_File,0);
	FSWrite(Temp_File, (void *)&current_wav_num,2,&err);
	FSClose(Temp_File);
	FSSetAttribute((char *)temp_file,FS_ATTR_HIDDEN);
#endif	
}


///*
void write_nor_data( void )
{
   __X U16 TempBuf[ 256 ], i;
   __X U32 DataStartSector;
   __X U32 DataLength;

   memset(TempBuf,0,256);
   TempBuf[ 0 ] = MSB(current_wav_num);
   TempBuf[ 1 ] = LSB(current_wav_num);
//   TempBuf[ 2 ] = MSW( nor_mp3_bak );
//   TempBuf[ 3 ] = LSW( nor_mp3_bak );
   
   GetDataIdInfo( D_DATA, &DataStartSector, &DataLength );
#if 0//def NFBOOT_KBDM9
   nftl_resv_read( DataStartSector, TempBuf );
#endif
#ifdef SDBOOT_KBDM9
   //sdtl_resv_read( DataStartSector, TempBuf );
	sdtl_resv_write( DataStartSector, TempBuf );
#endif
#ifdef SPIBOOT_KBDM9
   //spif_resv_read( DataStartSector, TempBuf );
   spif_resv_write( DataStartSector, TempBuf );
#endif

}
//*/





//! Power supply switch off
//!
void power_system_off( void )
{

	unsigned int value;
	unsigned int i;
	unsigned int temp_count=0;
#ifdef TX_DEBUG
   SendString("ready to power off\r\n");
#endif
//	save_wav_num();
#if 1
    if(b_find_usb_flag)
   	{
   		b_find_usb_flag = FALSE;
   	   SendString("usb_det power off\r\n");
   	 	BlueLed_OFF();
//	   OID_PWR_OFF();
//   		PWR_OFF();
#ifndef TX_DEBUG	
       RedLed_OFF();
#endif
/*
		if(ChangeToUSBMode()){
              CommonLoadCode(C_USB);
			}
		else{
			system_reset();
			}*/
/*

            // Is not connect with USB Host.
       while(1){
	   			Delay_ms(500);
//				SendString("power_1\r\n");
	   			if(PWD_Key_Press())
	   			CommonLoadCode(C_OID);
       			}
*/
   		
   	}
#else
    if(USB_DET()&&(!b_usb))
   	{
   	   SendString("usb_det power off\r\n");
   	 	BlueLed_OFF();
//	   OID_PWR_OFF();
//   		PWR_OFF();
#ifndef TX_DEBUG	
       RedLed_OFF();
#endif

            // Is not connect with USB Host.
       while(1){
	   			Delay_ms(500);
//				SendString("power_1\r\n");
	   			if(PWD_Key_Press())
	   			CommonLoadCode(C_OID);
       			}

   		
   	}
#endif	
#if 0//( OID == ENABLE )
   turn_off_oid();
   pen_disable();
   oid_power_off();
   oid_dc_dc_off();
#endif

   OID_PWR_OFF();
   PWR_OFF();
//   GPIO_init();
   BlueLed_OFF();
#ifndef TX_DEBUG	
     RedLed_OFF();
#endif
   SendString("power off\r\n");
 //  while(PWD_Key_Press());
 while(1){
			Delay_ms(100);
			adc3_value = Get_ADCValue(3);
			
			temp_count ++;
#ifdef  TX_DEBUG
			SendString(( char *)"\r\nadc3=");
			send_hex_u16(adc3_value);
#endif
			if(!PWD_Key_Press()){
				if(temp_count >= 3)break;
				}
			else{
				temp_count = 0;
				}
		}
   Delay_ms(500);
    BlueLed_ON();
#ifndef TX_DEBUG	
     RedLed_ON();
#endif
   CommonLoadCode(C_OPEN);
/*
   while( 1 )
   	{
   	 PWR_OFF();
	 
   	}
*/   
}


U16 GetBookId(void)
{
	U16 offset,bid = 0;
	U32 temp_id;
	//char tmpbuf[32];
	char rbuf[32];
	
	__X FS_ERR err;
    __X FCFILE *REC_File;
  // strcpy(tmpbuf,"0:\\AP4\\");
  // strcat(tmpbuf,( char *)g_string);
#ifdef TX_DEBUG
   	SendString(( char *)"\n\rtemp_path=");
	SendString(( char *)g_full_path);
#endif	
   REC_File = FSOpen((char *)g_full_path,FS_MODE_READ,&err);   
   if(err != FS_NO_ERROR)return 0;
			 
	//打开bnl文件，读取数据到rbuf
	//GetFileData_rBuf(0, rbuf, 0xc0);//192bytes
	//read_data_from_nf(0, (U8 _MEM_TYPE_SLOW_ *)rbuf, 0xc0);
	FSSeek(REC_File, 0xB8);
	FSRead(REC_File, rbuf, 0x08, &err);
	FSClose(REC_File);
	send_hex_u8(rbuf[0x0]);send_hex_u8(rbuf[0x1]);send_hex_u8(rbuf[0x2]);send_hex_u8(rbuf[0x3]);
	send_hex_u8(rbuf[0x4]);send_hex_u8(rbuf[0x5]);
	//该书存在"奇LE"标示
	//if(memcmp(rbuf+0xb8, "\xC6\xE6\x4C\x45", 4)==0)//184开始的连续4个bytes
	//if(memcmp(rbuf, "\x45\x4C\xE6\xC6", 4)==0)//184开始的连续4个bytes
	//if((rbuf[0x0]==0xC6)&&(rbuf[0x1]==0xE6)&&(rbuf[0x2]==0x4C)&&(rbuf[0x3]==0x45))
	MSB0(temp_id) = rbuf[0x0];
	MSB1(temp_id) = rbuf[0x1];
	MSB2(temp_id) = rbuf[0x2];
	MSB3(temp_id) = rbuf[0x3];
	if(temp_id == 0xc6e64c45)
	{
		//bid = rbuf[4]*256 + rbuf[5];//188,189 bookid
		MSB(current_id) = rbuf[0x5];
		LSB(current_id) = rbuf[0x4];
		//MSB(bid) = rbuf[0x5];
		//LSB(bid) = rbuf[0x4];
#ifdef TX_DEBUG
		SendString(( char *)"\n\rbid=");
		send_hex_u16(current_id);
#endif		
	}
	
	return current_id;
}

U8 test_area(U16 area_num)
{
	if((area_num<=OVER_NORMAL)){
		return AREA_NORMAL;
		}
	if((area_num >= BEGIN_RECITATIONS)&&(area_num<=OVER_RECITATIONS)){
		return AREA_RECITATION;
		}
	if((area_num >= BEGIN_TRANSLATIONS)&&(area_num<=OVER_TRANSLATIONS)){
		return AREA_TRANSLATION;
		}
	if((area_num >= BEGIN_EXPLANTION)&&(area_num<=OVER_EXPLANTION)){
		return AREA_EXPLANTION;
		}
	if((area_num >= BEGIN_OTHER)&&(area_num<=OVER_OTHER)){
		return AREA_OTHER;
		}
	return 0xFF;
}
///*
void create_readbnl_file(void)
{   

  __X FS_ERR err;
  U32 temp_len,need_len=16UL*1024;
  
	
	 	temp_len = FSGetFileSize((char *)REC_FILENAME, &err);
   	   if(temp_len >= need_len){
		}else{
    		 REC_File = FSOpen((char *)REC_FILENAME,FS_MODE_CREATE,&err);   
//			 FSResize(REC_File,2UL*1024*1024);
			 SDKLoadCode(KB_RESIZE_CODE);
			 err = kb_fat_resize(REC_File, 40UL*1024);
			 FSClose(REC_File);
			 
			}	
		//FSSetAttribute((char *)REC_FILENAME,FS_ATTR_HIDDEN);

}

void clear_bnl(void)
{
  __X FS_ERR err;
  __X U16 temp_data;
  U16 i,j;
  char temp_buf[512];	
	memset(temp_buf,0xFF,sizeof(temp_buf));
	REC_File = FSOpen((char *)REC_FILENAME,FS_MODE_READ_WRITE,&err); 
	for(i=0;i<80;i++){
		(void)FSSeek(REC_File,i*512);
		(void)FSWrite(REC_File, (void *)&temp_buf,512, &err);
		}
	FSClose(REC_File);
}
///*
void write_readbnl_file(U32 offset,U16 data)
{   

  __X FS_ERR err;
  __X U16 temp_data;

/*  
#ifdef UNCOIDE_FORMAT 
  U16	m[] = {'\\','\0'};
  strcpy_u((U16 *)REC_FILE,(U16 *)&root);
  strcat_u((U16 *)REC_FILE,(U16 *)&subdir);
  strcat_u((U16 *)REC_FILE,(U16 *)&m);
  strcat_u((U16 *)REC_FILE,(U16 *)&GENDU_FILE);
#endif  
#ifdef ASCII_FORMAT
	strcpy(REC_FILE,(char*)"0:\\ap4\\record.wav");  
#endif
*/	
#if 0//def TX_DEBUG
   	SendString(( char *)"\n\rwrite_off:data=");
	send_hex_u32(offset);
	SendString(":");
	send_hex_u16(data);
#endif		
	//MSB(temp_data) = LSB(data);
	//LSB(temp_data) = MSB(data);
	temp_data = data;
	REC_File = FSOpen((char *)REC_FILENAME,FS_MODE_READ_WRITE,&err); 
	(void)FSSeek(REC_File,offset);
	(void)FSWrite(REC_File, (void *)&temp_data,2, &err);
	FSClose(REC_File);
}


void readbnl_file(U32 offset,U16 len)
{   

  __X FS_ERR err;
  __X U16 temp_data;
#ifdef TX_DEBUG
   	SendString(( char *)"\n\rread_off:len=");
	send_hex_u32(offset);
	SendString(":");
	send_hex_u16(len);
#endif
	memset(&BOOK_NUM_0,0xFF,sizeof(U16)*BUF_LEN);
	memset(&BOOK_NUM_1,0xFF,sizeof(U16)*BUF_LEN);
	REC_File = FSOpen((char *)REC_FILENAME,FS_MODE_READ_WRITE,&err); 
	(void)FSSeek(REC_File,offset);
	(void)FSRead(REC_File, (void *)&BOOK_NUM_0,(len<<1), &err);
	(void)FSSeek(REC_File,(offset+0x2000));
	(void)FSRead(REC_File, (void *)&BOOK_NUM_1,(len<<1), &err);
	FSClose(REC_File);
}
///*
void read_bnl_file(U32 offset,U16 len)
{   

  __X FS_ERR err;
  __X U16 temp_data;
	
	REC_File = FSOpen((char *)REC_FILENAME,FS_MODE_READ_WRITE,&err); 
	(void)FSSeek(REC_File,offset);
	(void)FSWrite(REC_File, (void *)&temp_data,2, &err);
	FSClose(REC_File);
}

 
//get ap3 file name's header
U16 get_name_lead( void )
{
#if 0
#ifdef UNCOIDE_FORMAT
   U16 *p = ( U16 *)( g_string );
#else
   U8 *p = ( U8 *)( g_string );
#endif
#else
	U8 *p = ( U8 *)( g_string );
#endif
   _MEM_TYPE_SLOW_ U16 iNameLead;

   //bRemoteChoiceAp3 = FALSE;
   iNameLead = 0;
   if( !((*p >= '0' )&&( *p <= '9')))
   {
      //bRemoteChoiceAp3 = TRUE;
      return 0xFFFF;
   }
   while((*p >= '0' )&&( *p <= '9'))
   {
      iNameLead = iNameLead * 10 + *p - '0';
      p++;
   }
   //SendString((char *)"\r\nnamelead=");
  // send_hex_u16(iNameLead);
   return iNameLead;
}

Bool scank_disk( Bool root_flag )
{
	
    ap3_file_number = 0;
    sd2_ap3_number = 0;
 
   if(b_sd1_ok){
   ap3_file_number = file_get_ext_count((char *)SD1_AP4_DIR, (char *)AP4_EXT);
    	//ap3_file_number = file_get_ext_count((char *)SD1_AP4_DIRNAME, (char *)AP4_EXT);
   	}
   if(b_sd2_ok){
   	sd2_ap3_number = file_get_ext_count((char *)SD2_AP4_DIR, (char *)AP4_EXT);
   	}
   if((ap3_file_number == 0)&&(sd2_ap3_number == 0)){
   	g_b_noap4 = TRUE;
   	}
#ifdef TX_DEBUG
   	SendString("\r\nsd1_number:sd2_num=");
   	send_hex_u16(ap3_file_number);
	SendString(":");
   	send_hex_u16(sd2_ap3_number);
#endif
   return TRUE;
}

U16 find_sd1(U16 find_num)
{
	U16 i;
	for(i=0;i<ap3_file_number;i++){
		if(file_get_path((char *)SD1_AP4_DIR, (char *)AP4_EXT, i)){
			strcpy(( char *)( g_string ),(char *)fat_g_info.name);
			current_id = get_name_lead();
			if(current_id == find_num){
				return TRUE;
				}
			}
		}
	return FALSE;
}

U16 find_sd2(U16 find_num)
{
	U16 i;
	for(i=0;i<sd2_ap3_number;i++){
		if(file_get_path((char *)SD2_AP4_DIR, (char *)AP4_EXT, i)){
			strcpy(( char *)( g_string ),(char *)fat_g_info.name);
			current_id = get_name_lead();
			if(current_id == find_num){
				return TRUE;
				}
			}
		}
	return FALSE;
}


U16	record_area(U16 begin_num,U16 over_num,U32 offset)
{
	U16 j,x,m=0;
	x = 0;
	for(j=0;j<over_num;j++){
		if(BOOK_NUM_0[j] != 0xFFFF){
			write_readbnl_file((offset+(x)), FLAG_SD1_CON);
			write_readbnl_file((offset+(x+2)), BOOK_NUM_0[j]);
			x += 4;
			m++;
			}
		else if(BOOK_NUM_1[j] != 0xFFFF){
			write_readbnl_file((offset+(x)), FLAG_SD2_CON);
			write_readbnl_file((offset+(x+2)), BOOK_NUM_1[j]);
			m++;
			x += 4;
			}
		}
	return m;
/*
	U16 j,m=0;
	for(j=begin_num;j<=over_num;j++){
		if(find_sd1(j)){
			write_readbnl_file((offset+(j<<1)), 0x0000);
			write_readbnl_file((offset+(j<<2)), j);
			m++;
			}
		else if(find_sd2(j)){
			write_readbnl_file((offset+(j<<1)), 0x0001);
			write_readbnl_file((offset+(j<<2)), j);
			m++;
			}
		}
	return m;
	*/
	
}




/*
Bool find_wav(U16 num)
{
	U16 i,current_dir_mask,mask,temp_data_1;
	char dir_path[128];
	char digi[4];
	U16 temp_index;
	U32 temp_file_size=0;
	temp_index = num;
	//strcpy(dir_path,PASTER_DIR);
	 if(b_sd2_ok){
	 	strcpy(dir_path,(char *)SD2_PASTER_DIR);
	 	}
	 else{
	 	strcpy(dir_path,(char *)SD1_PASTER_DIR);
	 	}
	digi[0] = '0';
	digi[1] = '0'+temp_index/10;
	digi[2] = '0'+temp_index%10;
	digi[3] = 0;
	strcat(dir_path,"REC");
	strcat(dir_path,digi);
	strcat(dir_path,".WAV");
#ifdef  TX_DEBUG 	
	SendString("\r\ndir_path:size=");
	SendString(dir_path);
#endif		

 	wavfile_size = FSGetFileSize((char *)dir_path,&fserr);
#ifdef  TX_DEBUG 	
	SendString(":");
	send_hex_u32(wavfile_size);
#endif	
  if(wavfile_size==0)
	{

		return FALSE;
	}
  
   if(wavfile_size>(REC_REMOVE_SEC*2)){
   	  flag_bigfile=TRUE;
   }else{
      flag_bigfile=FALSE;
   }   
   return TRUE;
}
*/



void main(void)
{

    FS_ERR fserr;
	U16 temp_data,bnl_num = 0;
	U16 i;
	Bool first_flag = FALSE;
	U8 temp_area;
	unsigned char	attr;
	IP_INIT();
	//SystemClock(SYS_CLOCK);
	SystemClock(0x02);
	OP_CLOSE();
	//ADC_init();
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nKB readbnl!");
#endif

	//write_nor_data();
	//FSInit(1);
	//fserr = FS_mount(&SD_Drive);
	//fserr = FS_mount(&NF_Drive);	
	//g_b_readbnl = TRUE;
	ready_mount_sd();
	if((!b_sd1_ok)&&(!b_sd2_ok)){
#ifdef TX_DEBUG
		SendString(( char *)"\r\nno sd!");
#endif		
		  Exception();
	 }
#ifdef  TX_DEBUG
	SendString(( char *)"\r\nmount sd ok!");
#endif

//	SD1_fast_mode();

	 //  BlueLed_ON();
	  // RedLed_OFF();

	  // BlueLed2_OFF();
	  // RedLed2_OFF();

	 //  power_system_off();

   


	 	//bBnl_FastSeek = FALSE;

		mmc_sd_all_mode_dir_make();
		//readAp4DirName();
		(void)scank_disk(FALSE);
		
		//readPasterDirName();

		//BlueLed_ON();
	   if(!bDiskFullFlag){
	   	bDiskFullFlag = TRUE;
#ifdef DISK_FULL_CHECK	
#if 1
/*
		if(b_sd2_ok){
		free_space = FSFreeSpace(1,&fserr);	
		free_space = free_space*512UL;
		}
		else{
			free_space = FSFreeSpace(0,&fserr);	
			free_space = free_space*512UL;
			}*/
#else
		if(b_sd2_ok){
		free_space = FSFreeSpace(1,&fserr);	
		free_space = free_space*512UL;
		}
		else{
			free_space = FSFreeSpace(0,&fserr);	
			free_space = free_space*512UL;
			}
#endif		
#else	
		//free_space = 500*1024UL*1024UL;
#endif //#ifdef DISK_FULL_CHECK	 
	   	}	

#if 0//def TX_DEBUG
		SendString("\r\nfree_space=");
		send_hex_u32(free_space);
#endif

		
		Current_Record.Ap3_num = 0;
		Current_Record.Book_num = 0;
		Current_Record.Page_num = 0;
		Current_Record.Control_num	= 0;

		if(ap3_file_number != 0){
			current_sd = FLAG_SD1_CON;
			}
		else if(sd2_ap3_number != 0){
			current_sd = FLAG_SD2_CON;
			}
		else{
			g_b_noap4 = TRUE;
			}




#if 0

		//CommonLoadCode(C_OID);
		if(b_sd1_ok){
		    strcpy(REC_FILENAME,(char*)SD1_BOOK_SYS);
			}
		else{
			strcpy(REC_FILENAME,(char*)SD2_BOOK_SYS);
			}
		create_readbnl_file();
		clear_bnl();

		for(i=0;i<NUM_AREA;i++){
			Current_lastbook[i].Book_num = 0xFFFF;
			Current_lastbook[i].Sd_num = 0xFFFF;
			Current_lastbook[i].Book_id = 0xFFFF;
			Current_booknews[i].Cur_num = 0;
			Current_booknews[i].Max_num = 0;
		}
		
		//bnl_num = file_get_ext_count((char *)SD1_AP4_DIR, (char *)AP4_Ext);
		for(i=0;i<ap3_file_number;i++){
			if(file_get_path((char *)SD1_AP4_DIR, (char *)AP4_Ext, i)){
				 strcpy(( char *)( g_string ),(char *)fat_g_info.name);
				//temp_data = GetBookId();
				current_id = get_name_lead();
				if(!first_flag){
					temp_area = test_area(current_id);
					if(temp_area!=0xFF){
							first_flag = TRUE;
							current_area = temp_area;
							current_sd = FLAG_SD1_CON;
							Current_Record.Ap3_num = i;
							Current_lastbook[current_area].Book_num = i;
							Current_lastbook[current_area].Book_id = current_id;
							Current_lastbook[current_area].Sd_num = FLAG_SD1_CON;
						}
					}
				if(current_id != 0xFFFF){
					write_readbnl_file(current_id*2, i);
					}
				}
			}
		//current_id = bnl_num;
		//write_readbnl_file(0, bnl_num);

			//bnl_num = file_get_ext_count((char *)SD2_AP4_DIR, (char *)AP4_Ext);			
			for(i=0;i<sd2_ap3_number;i++){
				if(file_get_path((char *)SD2_AP4_DIR, (char *)AP4_Ext, i)){
					strcpy(( char *)( g_string ),(char *)fat_g_info.name);
					current_id = get_name_lead();
					if(!first_flag){
						temp_area = test_area(current_id);
						if(temp_area!=0xFF){
								first_flag = TRUE;
								current_sd = FLAG_SD2_CON;
								current_area = temp_area;
								Current_Record.Ap3_num = i;
								Current_lastbook[current_area].Book_num = i;
								Current_lastbook[current_area].Book_id = current_id;
								Current_lastbook[current_area].Sd_num = FLAG_SD2_CON;
							}
					}
					if(current_id != 0xFFFF){
						write_readbnl_file((0x2000+current_id*2), i);
						}
					}
				}
			//current_id = bnl_num;
			//write_readbnl_file(0x2000, bnl_num);


		
		//FSSetAttribute((char *)REC_FILENAME,FS_ATTR_HIDDEN);
		//attr =  FSGetAttribute((char *)REC_FILENAME, &fserr);
		//attr&=(~FS_ATTR_HIDDEN);
		//FSSetAttribute((char *)REC_FILENAME,attr);
///*		
#ifdef TX_DEBUG
		SendString("\r\nrecord area begin!");
#endif
		readbnl_file((BEGIN_NORMAL<<1),RANGE_NORMAL);
		bnl_num = record_area(BEGIN_NORMAL,RANGE_NORMAL, OFFSET_NORMAL);
		Current_booknews[AREA_NORMAL].Max_num = bnl_num;

		readbnl_file((BEGIN_RECITATIONS<<1),RANGE_RECITATIONS);
		bnl_num = record_area(BEGIN_RECITATIONS,RANGE_RECITATIONS, OFFSET_RECITATIONS);
		Current_booknews[AREA_RECITATION].Max_num = bnl_num;

		
		readbnl_file((BEGIN_TRANSLATIONS<<1),RANGE_TRANSLATIONS);
		bnl_num = record_area(BEGIN_TRANSLATIONS,RANGE_TRANSLATIONS, OFFSET_TRANSLATIONS);
		Current_booknews[AREA_TRANSLATION].Max_num = bnl_num;

		readbnl_file((BEGIN_EXPLANTION<<1),RANGE_EXPLANTION);
		bnl_num = record_area(BEGIN_EXPLANTION,RANGE_EXPLANTION, OFFSET_EXPLANATION);
		Current_booknews[AREA_EXPLANTION].Max_num = bnl_num;

		readbnl_file((BEGIN_OTHER<<1),RANGE_OTHER);
		bnl_num = record_area(BEGIN_OTHER,RANGE_OTHER, OFFSET_OTHER);
		Current_booknews[AREA_OTHER].Max_num = bnl_num;
#ifdef TX_DEBUG
		SendString("\r\nrecord area over!\r\n");
		for(i=0;i<NUM_AREA;i++){
			send_hex_u16(Current_booknews[i].Max_num);
			SendString(":");
			}
#endif
//*/
		if(!first_flag)g_b_noap4 = TRUE;
		bBnl_FastSeek = TRUE;
#endif		
		CommonLoadCode(FIRST_PROJECT);
}



void	strcpy_u(U16 * dst,U16 * src)
{
	while(*src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
}
void	strcat_u(U16 * dst,U16 * src)
{
	while(*dst)	++dst;
	while(*src)
	{
		*dst++ = *src++;
	}
	*dst = 0;
}

void	unicode_to_local(U16 * unicode_str,U8 * local_str,U16 buffer_size)
{
	while(*unicode_str)
	{
		if(*unicode_str<256)
		{
			*local_str++=*unicode_str++;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
		}
		else
		{
			*local_str++=*unicode_str;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			*local_str++=((*unicode_str)&0xFF)>>8;
			--buffer_size;
			if(buffer_size == 1)
			{
				*local_str = 0;
				return;
			}
			++unicode_str;
		}
	}
	*local_str = 0;
}
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

Bool	find_entry(U8 * spec,U16 * name,Bool is_dir,FILE_INFO __X * finfo,FS_ENTRY_INFO __X * entry)
{
   //if( FSGetEntry( spec, 0, finfo, entry ) != FS_NO_ERROR )
   //   return FALSE;
   U16	buffer[ 25 ];
  //ange 2013-05-22 zoe
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

      //ange 2013-05-22 zoe
      fserr = FSGetNextEntry( finfo, entry );
      if(( fserr != FS_NO_ERROR )&&( fserr != FS_FILE_NOT_FOUND ))
         break;
   }
   //while( FSGetNextEntry( finfo, entry )== FS_NO_ERROR );
   while( 1 );
   return FALSE;
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

#define	find_path_a(a,b)	find_path_u(atou(a),b)   

Bool	find_path_u(U16 * path_str,U8 * short_path)
{
   U16 left, right;
   //U16 iter;
   U16 old_char;
   FILE_INFO __X info;
   FS_ENTRY_INFO __X entry;
   left = right = 3;

   strcpy(( char *)short_path, "0:" );
   memset( &info, 0, sizeof( info ));
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
         if( find_entry( short_path, ( U16 *)&path_str[ left ], FALSE, &info, &entry ))
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
   return FALSE;
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
	
	U16	story[] = {0x6545,0x4E8B,0x0000};//故事
	U16	erge[] = {0x513F,0x6B4C,0x0000};//儿歌
	//U16	music[] = {0x97F3,0x4E50,0x0000};//音乐
	U16	mp3_book[] = {0x6709,0x58F0,0x4E66,0x0000};//有声书

	U16	music[] = {0x97F3,0x4E50,0x6B23,0x8D4F,0x0000};//音乐欣赏
	U16	ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,0x0000};//有声课本
	U16	record[] = {0x5F55,0x97F3,0x0000};//录音


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


void readAp4DirName(void)
{
	FS_ERR fs_err;
		U8 j;
		char tisiFileName[128];
	
		//char dirpath[32];
		U16   sd1_root[] = {'0',':','\\','\0'};
		U16 m[] = {'\\','\0'};
		//U16		dictionary[] = {0x8BCD,0x5178, 0x0000};
		
		//U16 story[] = {0x6545,0x4E8B,0x0000};//故事
		//U16 erge[] = {0x513F,0x6B4C,0x0000};//儿歌
		//U16	music[] = {0x97F3,0x4E50,0x0000};//音乐
		//U16 mp3_book[] = {0x6709,0x58F0,0x4E66,0x0000};//有声书
	
		//U16 music[] = {0x97F3,0x4E50,0x6B23,0x8D4F,0x0000};//音乐欣赏
		U16 ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,0x0000};//有声课本
		//U16 record[] = {0x5F55,0x97F3,0x0000};//录音

	strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
	strcat_u((U16 *)tisiFileName,(U16 *)&ap4_book);
	strcat_u((U16 *)tisiFileName,(U16 *)&m);
    (void)find_path_u((U16 *)&tisiFileName,(U8 *)SD1_AP4_DIRNAME);
#ifdef  TX_DEBUG
		SendString(( char *)"\r\nsd1_ap4_dirname=");
		SendString(SD1_AP4_DIRNAME);
#endif

}

void readPasterDirName(void)
{
	FS_ERR fs_err;
		U8 j;
		char tisiFileName[128];
	
		char dirpath[32];
		U16   sd1_root[] = {'0',':','\\','\0'};
		U16 m[] = {'\\','\0'};
		//U16		dictionary[] = {0x8BCD,0x5178, 0x0000};
		
		//U16 story[] = {0x6545,0x4E8B,0x0000};//故事
		//U16 erge[] = {0x513F,0x6B4C,0x0000};//儿歌
		//U16	music[] = {0x97F3,0x4E50,0x0000};//音乐
		//U16 mp3_book[] = {0x6709,0x58F0,0x4E66,0x0000};//有声书
	
		//U16 music[] = {0x97F3,0x4E50,0x6B23,0x8D4F,0x0000};//音乐欣赏
		//U16 ap4_book[] = {0x6709,0x58F0,0x8BFE,0x672C,0x0000};//有声课本
		U16 record[] = {0x5F55,0x97F3,0x0000};//录音
	memset(g_paster_dir,0,60);
	strcpy_u((U16 *)tisiFileName,(U16 *)&sd1_root);
	strcat_u((U16 *)tisiFileName,(U16 *)&record);
	strcat_u((U16 *)tisiFileName,(U16 *)&m);
    (void)find_path_u((U16 *)&tisiFileName,(U8 *)g_paster_dir);
#ifdef  TX_DEBUG
		SendString(( char *)"\r\nsd1_paster_dirname=");
		SendString((char *)g_paster_dir);
#endif

}


//void LED_Callback(void)
//{
//}
