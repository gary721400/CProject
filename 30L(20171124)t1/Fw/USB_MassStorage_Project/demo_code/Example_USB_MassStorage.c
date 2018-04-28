/******************************************************************************
** Copyright (c) 2008
** Kingb Billion Electronics Co., Ltd
** http://www.kingb.com.tw
******************************************************************************/
#pragma DATA = EXAMPLE_XRAM
#pragma CODE = EXAMPLE_CODE

#include "KBDM9_DDK.h"
#include <string.h>
#include "Example_USB_MAssStorage.h"
//#include "CodeImg.h"
#include "SYSTEM.H"
#include "ADC.H"
#include "BinaryImage.h"
#include "config.h"
#include "conf_kb.h"
#include "com.h"
#include "fs.h"
#include "fc_internal.h"
#include "kbfat.h"


//=============================================================================
// VERSION
//=============================================================================
//#if (PACKAGE_VER != 0x0904)
//#error "Package Version should be 0904"
//#endif

//=============================================================================
// OPTION
//=============================================================================
//extern unsigned int gflag;
//#define OID_Off()		_bitCLR(CTL_OID_OD)
//#define PowerTurnOff()	_bitCLR(PWR_CTRL_OD)


//extern unsigned int ap4_mp3_flag;
unsigned int g_exit_usb;
unsigned long time_count;
unsigned long usb_attach_time;
unsigned int flag_chongdian;
unsigned int vid_data;
unsigned int pid_data;
__X unsigned int SerialNumber[6]; 
unsigned int vol_key;
Bool lidian_flag;
void Delay_ms(int count);
extern FS_ERR	FSSetCurEntryAttr(__X FS_ENTRY_INFO* fentry,unsigned char attr);
extern FS_ERR	FSGetCurEntryAttr(__X FS_ENTRY_INFO* fentry,unsigned char * attr);
//=============================================================================
// PROGRAM
void	local_to_unicode(U8 * local_str,U16 * unicode_str,U16 buffer_size);
int		str_comp_u(U16  * sl,U16  * sr,U8 use_case);
//=============================================================================
#ifdef NIEQING_DIANCHI

void Charge_Init(void)
{
	charge_state_output();
	charge_state_islow();

	Charge_EN_OUT();  //set output
	
	Charge_STA_IN();  //input
	//Charge_SET_PH();
	//Charge_Disable();
	//Charge_NO_PH();//state input no_pullup 
	Charge_Enable();//
//	time_count=0;
	BlueLed_ON();
	RedLed_ON();
	heigh_num = 0;
	low_num = 0;
	qieqing_stop_timer();
	qieqing_start_timer(2000);
#ifdef TX_DEBUG		
	 SendString("\r\nqieqing init!");
#endif		
}



void Charge_Check(void)
{

if(qieqing_is_timer_timeout()){
	qieqing_start_timer(QIEQING_TIME);
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
			flag_chongdian = TRUE;
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


#define  FS_ATTR_FILE         ((Byte)0x00)   // Normal File Attribute
#define  FS_ATTR_READ_ONLY    ((Byte)0x01)   // Read Only File Attribute
//#define  FS_ATTR_HIDDEN       ((Byte)0x02)   // Hidden File Attribute
//#define  FS_ATTR_SYSTEM       ((Byte)0x04)   // System File Attribute
#define  FS_ATTR_VOLUME_ID    ((Byte)0x08)   // Volume id Attribute
//#define  FS_ATTR_DIRECTORY    ((Byte)0x10)   // Directory Attribute


#define  FS_ATTR_LFN_ENTRY    ((Byte)0x0F)   // LFN entry attribute
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
#if 0
void show_subdir(void)
{
  __X FS_ERR err;
  unsigned temp_len=0;
  unsigned char temp_attr=0;
  temp_len = (FSGetFileSize((char *)"0:\\show.sys", &err));
   	   if(0!=temp_len){ 
	   	temp_attr = FSGetAttribute("0:\\subdir", &err);
		FSSetAttribute("0:\\subdir",(temp_attr&(~FS_ATTR_HIDDEN))); 
		}else{
    		 FSSetAttribute("0:\\subdir",(FS_ATTR_HIDDEN)); 
			}	
	
}
#else

void show_subdir(void)
{
  __X FS_ERR err;
  U16	subdir[] = {'s','u','b','d','i','r','\0'};
  U8	dir[] = "0:\\";
  U16	subdirpath[] = {'0',':','\\','s','u','b','d','i','r','\\','\0'};
  FILE_INFO __X	fi;
  FS_ENTRY_INFO	__X	entry;
  unsigned char	attr;
  //unsigned long temp_len=0;
  unsigned char temp_attr=0;
  Bool	result;
  KBFatDir temp_dir;
  
  
  temp_dir.data_buf.buffer = 0;
   result = kb_fat_path(&temp_dir,subdirpath,(U16 *)fi.lfn,(U8 *)fi.name);
   if(result){
   	 (void)kb_fat_dir_get_attr(&temp_dir, &attr);
	 attr&=(~FS_ATTR_VOLUME_ID);
	 attr|=(FS_ATTR_DIRECTORY);
  	 (void)kb_fat_dir_set_attr(&temp_dir,attr);
  	}
}

void check_show_subdir(void)
{
  __X FS_ERR err;
  U16	subdir[] = {'s','u','b','d','i','r','\0'};
  U8	dir[] = "0:\\";
  U16	subdirpath[] = {'0',':','\\','s','u','b','d','i','r','\\','\0'};
  FILE_INFO __X	fi;
  FS_ENTRY_INFO	__X	entry;
  unsigned char	attr;
  unsigned long temp_len=0;
  unsigned char temp_attr=0;
  Bool	result;
  KBFatDir temp_dir;
  
  
  temp_dir.data_buf.buffer = 0;
  temp_len = FSGetFileSize((char *)"0:\\show.sys", &err);
   	   if(0!=temp_len)
	   	{ 
		   result = kb_fat_path(&temp_dir,subdirpath,(U16 *)fi.lfn,(U8 *)fi.name);
		   if(result){
		   	 (void)kb_fat_dir_get_attr(&temp_dir, &attr);
			 attr&=(~FS_ATTR_VOLUME_ID);
			 attr|=(FS_ATTR_DIRECTORY);
		  	 (void)kb_fat_dir_set_attr(&temp_dir,attr);
		  	}
		   

		}
	   else
	   	{
	   	   result = kb_fat_path(&temp_dir,subdirpath,(U16 *)fi.lfn,(U8 *)fi.name);
		   if(result){
		   	 (void)kb_fat_dir_get_attr(&temp_dir, &attr);
			 attr|=(FS_ATTR_VOLUME_ID);
			 attr|=(FS_ATTR_DIRECTORY);
		  	 (void)kb_fat_dir_set_attr(&temp_dir,attr);
		  	}
		}	
	
}
#endif
/*
void Update_VID_PID(unsigned int VID,unsigned int PID);
__asm{
_Update_VID_PID::
        LINK #0
        PUSH R0
        PUSH R1
        PUSH R2
        PUSH DS0
        PUSH SB

        MOV R0,#^_USB_Device_Desc
        SL  R0,#1
        MOVX DS0,R0
        SR  R0,#3
        MOV R0,#_USB_Device_Desc
        ADD R0,#8
        BMOV DS0.0,R0.15
        MOV R1,R0
        SROSB  R0,#2
        MOV R2,R0
        SL R2,#2
        CMP R1,R2
        AJMP IF_EQ,WR4B
WR8B:
        MOV R1,DS0:(_USB_Device_Desc+6)
        MOV R2,[FP+#6]
        CRIT #3
        MOVX IRAMA,R0
        MOVX IRAMDH,R1
        MOVX IRAMDL,R2
        ADD R0,#1
        MOV R1,[FP+#8]
        MOV R2,DS0:(_USB_Device_Desc+12)
        AJMP WRIRAM
WR4B:
        MOV R1,[FP+#6]
        MOV R2,[FP+#8]
WRIRAM:
        CRIT #3
        MOVX IRAMA,R0
        MOVX IRAMDH,R1
        MOVX IRAMDL,R2

        POP SB
        POP DS0        
        POP R2
        POP R1
        POP R0
        UNLINK
        RETS
}
*/
void Update_VID_PID(unsigned int VID_parm,unsigned int PID_parm);
void Update_SerialNumber(__X unsigned int SerialNo[],unsigned char len);
__asm{
.define VID_PID_OFFSET (_USB_Device_Desc+8)
.define VID_param [FP+#6]
.define PID_param [FP+#8]
_Update_VID_PID::
        LINK #0
        PUSH R0
        PUSH R1
        PUSH R2
        PUSH DS0
        PUSH SB

        MOV R0,#^VID_PID_OFFSET
        SR  R0,#2
        MOV R0,#VID_PID_OFFSET
        MOV R1,R0
        SROSB  R0,#2
        JNB R1.1,$WR4B
$WR8B:
        MOV DS0,#@(VID_PID_OFFSET)
        MOV R1,DS0:(VID_PID_OFFSET-2)
        MOV R2,VID_param
        CRIT #3
        MOVX IRAMA,R0
        MOVX IRAMDH,R1
        MOVX IRAMDL,R2
        ADD R0,#1
        MOV R1,PID_param
        MOV R2,DS0:(VID_PID_OFFSET+4)
        AJMP $WRIRAM
$WR4B:
        MOV R1,VID_param
        MOV R2,PID_param
$WRIRAM:
        CRIT #3
        MOVX IRAMA,R0
        MOVX IRAMDH,R1
        MOVX IRAMDL,R2

        POP SB
        POP DS0        
        POP R2
        POP R1
        POP R0
        UNLINK
        RETS

.define SerialNumberOffset (String03)
.define SerialNoPtr [FP+#6]
.define LenParam [FP+#9]
_Update_SerialNumber::
    LINK #0
    PUSH R0
    PUSH R1
    PUSH R2
    PUSH R3
    PUSH R4
    PUSH DS0
    PUSH SB

    MOV R4,SerialNoPtr
    MOV R0,#SerialNumberOffset
    JNB R0.1,$WR_last_4B
$WR_last_2B:
    MOV R1,#^SerialNumberOffset
    SR R1,#2
    SROSB R0,#2
    MOV DS0,#@(SerialNumberOffset)
    MOV R1,DS0:(SerialNumberOffset-2)
    MOVB Rb4,LenParam
    MOV R3,#0
    MOVB Rb7,Rb4
    SR R3,#1
    ADDB Rb4,#2
    MOVB Rb5,#03H
    CRIT #3
    MOVX IRAMA,R0
    MOVX IRAMDH,R1
    MOVX IRAMDL,R2
    CMP R3,#0
    AJMP IF_EQ,$EXIT
    ADD R0,#1
    AJMP $WR_OTHER

$WR_last_4B:
    MOV R1,#^SerialNumberOffset
    SR R1,#2
    SROSB R0,#2
    MOVB Rb2,LenParam
    MOV R3,#0
    MOVB Rb7,Rb2
    SR R3,#1
    ADDB Rb2,#2
    MOVB Rb3,#03H
    MOV R2,#0
    CMP R3,#0
    AJMP IF_EQ,$NO_SN
    IGNDS #1
    MOV R2,[R4++]
$NO_SN:
    CRIT #3
    MOVX IRAMA,R0
    MOVX IRAMDH,R1
    MOVX IRAMDL,R2
    CMP R3,#0
    AJMP IF_EQ,$EXIT
    SUB R3,#1
    ADD R0,#1
    
$WR_OTHER:
    CMP R3,#2
    AJMP IF_UL,$ODD
    IGNDS #2
    MOV R1,[R4++]
    MOV R2,[R4++]
    SUB R3,#2
    AJMP $WRIRAM
$ODD:
    IGNDS #1
    MOV R1,[R4++]
    MOV R2,#0
    SUB R3,#1
$WRIRAM:
    CRIT #3
    MOVX IRAMA,R0
    MOVX IRAMDH,R1
    MOVX IRAMDL,R2
    ADD R0,#1
    CMP R3,#0
    AJMP IF_NE,$WR_OTHER
$EXIT:
    POP SB
    POP DS0
    POP R4
    POP R3        
    POP R2    
    POP R1    
    POP R0    
    UNLINK
    RETS
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
	U16	left ,right;
	U16	iter;
	U16	old_char;
	FILE_INFO	__X	info;
	FS_ENTRY_INFO __X entry;
	left = 3,right = 3;

	strcpy((char *)short_path,"0:");
	memset(&info,0,sizeof(info));
	entry.drive = 0;
	FSGetEntry((char *)short_path,0,&info,&entry);
	if(path_str[left] == '\0')
		return	TRUE;
	while(path_str[left]!='\0')
	{
		while(path_str[right]!='\\'&&path_str[right]!='/'&&path_str[right]!='\0')
			++right;
		if(path_str[right] == '\\'||path_str[right] == '/')
		{
			old_char = path_str[right];
			path_str[right] = 0;
			if(find_entry(short_path,(U16 *)&path_str[left],TRUE,&info,&entry))
			{
		//	if(nav_filelist_findname((FS_STRING)(&path_str[left]),FALSE)&&nav_file_isdir())
				strcat((char *)short_path,(char *)"\\");
				strcat((char *)short_path,(char *)info.name);
				if(FSGetEntry((char *)short_path,0,&info,&entry)!=FS_NO_ERROR)
					goto	__bad_end;
			}
			else
				goto	__bad_end;
			path_str[right] = old_char;
			++right;
			left = right;
		}
		else
		{
			if(find_entry(short_path,(U16 *)&path_str[left],FALSE,&info,&entry))
			{
				strcat((char *)short_path,"\\");
				strcat((char *)short_path,(char *)info.name);
				goto	__end;
			}
			else
				goto	__bad_end;
		}
	}
__end:
	return	TRUE;
__bad_end:
	return	FALSE;
}

void set_vidpid(void)
{
    __X FS_ERR err;
  unsigned long temp_len;

  __X FCFILE *REC_File;
  char temp_file[]="0:\\subdir\\usbserial.sys";
  char path[64];
  
#if 0
  if(!find_path_a((U8 *)temp_file,(U8 *)path)){
  	 vid_data=0x3412;
 	 pid_data=0x7856;
  	 return;
  	}
  temp_len = FSGetFileSize((char *)path, &err);
   	   if(0!=temp_len){ 
	   	REC_File = FSOpen((char *)path,FS_MODE_READ,&err);   
		FSSeek(REC_File,0);
		FSRead(REC_File, (void *)&vid_data,4,&err);
		FSSeek(REC_File,4);
		FSRead(REC_File, (void *)&pid_data,4,&err);
		FSClose(REC_File);
//		 Update_VID_PID(vid_data,pid_data);  //VID=0x7890, PID=0x55AA
		}else{
 //   		 FSSetAttribute("0:\\subdir",(FS_ATTR_HIDDEN)); 
 //			 Update_VID_PID(0x3412,0x7856);  //VID=0x7890, PID=0x55AA
			vid_data=0x3412;
 	 		pid_data=0x7856;
			}	
#else
	char temp_data[6];
	memset(SerialNumber,0,6);
	memset(temp_data,0,6);
	vid_data=0x3412;
 	pid_data=0x7856;
 if(!find_path_a((U8 *)temp_file,(U8 *)path)){
  	 SerialNumber[0]=0x3000;
	 SerialNumber[1]=0x3100;
	 SerialNumber[2]=0x3200;
	 SerialNumber[3]=0x3300;
	 SerialNumber[4]=0x3400;
	 SerialNumber[5]=0x3500;
  	 return;
  	}
  temp_len = FSGetFileSize((char *)path, &err);
   	   if(temp_len>=6){ 
	   	REC_File = FSOpen((char *)path,FS_MODE_READ,&err);   
		FSSeek(REC_File,0);
		FSRead(REC_File, (void *)&temp_data,6,&err);
		FSClose(REC_File);
		MSB(SerialNumber[0])=temp_data[0];
		MSB(SerialNumber[1])=temp_data[1];
		MSB(SerialNumber[2])=temp_data[2];
		MSB(SerialNumber[3])=temp_data[3];
		MSB(SerialNumber[4])=temp_data[4];
		MSB(SerialNumber[5])=temp_data[5];
//		 Update_VID_PID(vid_data,pid_data);  //VID=0x7890, PID=0x55AA
		}else{
 //   		 FSSetAttribute("0:\\subdir",(FS_ATTR_HIDDEN)); 
 //			 Update_VID_PID(0x3412,0x7856);  //VID=0x7890, PID=0x55AA
		  	 SerialNumber[0]=0x3000;
			 SerialNumber[1]=0x3100;
			 SerialNumber[2]=0x3200;
			 SerialNumber[3]=0x3300;
			 SerialNumber[4]=0x3400;
			 SerialNumber[5]=0x3500;
			}	
#endif
}





Bool  key_down(void)
{

	unsigned int count;
	unsigned int adc_value =0;
	count = 0;	
#ifdef NIEQING_DIANCHI
//	Charge_Check();
#endif	

		//if(rec_led_is_timer_timeout()){
		//	rec_led_start_timer(REC_LED_ON_NORMAL_TIME);
		if(sys_led_is_timer_timeout()){
			sys_led_start_timer(SYS_LED_ON_TIME);
			adc3_value = Get_ADCValue(3);
		//		SendString("\radc_value=");
		//		send_hex_u16(adc_value);
/*		
			if((adc_value >= 600) && ( adc_value <= 730))
			{
				flag_chongdian++;
				
			}
			else{
				flag_chongdian=0;
				}
			
			if(flag_chongdian>=4){
				MassStorage_Exit();
		   		CommonLoadCode(C_USB_HID);
				}
*/
#ifdef LI_DIANCHI
		 if(lidian_flag){
		 	BlueLed_OFF();
			RedLed_ON();
		 	}
		 else{
			 if(Lidian_ok()){
				BlueLed_OFF();
				RedLed_ON();
				lidian_flag = TRUE;
				}
			else{
				BlueLed_OFF();
				RedLed_FLASH();
				}
		 	}

#endif
#if( T01OID_CAB == ENABLE )
		//if((adc_value >= 600)&&(adc_value <= (630))){
			if(Mode_Key_Press()){
			if(vol_key++>=5){
				MassStorage_Exit();
				//gflag |= 0x0001; 
				b_usb = TRUE;
				CommonLoadCode(C_CAB);
				}
			}
		else{
			vol_key = 0;
			}
#endif	

		if(PWD_Key_Press())
			{
				if(time_count++>5){
					return TRUE;
					}
			}
			else{
				time_count = 0;
				}

   }
	
  return FALSE;
}

void main(void)
{
    //Variable Declare
    int                 lun;
	
	FS_ERR fserr;
	char temp_lab[12];
	char sys_lab[12];
	char digi[6];
	__asm{BCLR USBCON.14};	
	//System Initial
    IP_INIT();
//	Int_enable();
//	GPIO_init();
/*
	__asm{
		BCLR PBEN.12
		BSET PBDIR.12
		};	
	*/
	ADC_init();
    SystemClock(SYS_CLOCK);
	OP_CLOSE();
    PowerTurnOff();
//    BlueLed_ON();
	BlueLed_OFF();
	RedLed_ON();
	ap4_mp3_flag = 0;
	g_exit_usb=0;
	flag_chongdian = 0;
	lidian_flag = FALSE;
	System_Timer3_Init();
#ifdef  TX_DEBUG
    
    UART_Init();
	SendString(( char *)"\r\nKB usb begin!");
#endif
#if 0//def NIEQING_DIANCHI
	System_Timer_Init(4,10); 
	Charge_Init();
#endif	
	FSInit(1);
#if USE_SD
	fserr = FS_mount(&SD_Drive);
	
	if( fserr != FS_DRIVE_ERROR ){
		
		//show_subdir();

	    digi[0] = HARD_VERSION;
		digi[1] = MAJOR_FW_VERSION;
		digi[2] = MINOR_FW_VERSION;
		digi[3] = 0;
		memset(sys_lab,0,12);
		strcpy(sys_lab,SYS_LAB);
		strcat(sys_lab,digi);


    	FSVolume(0,temp_lab,0);
		if(strcmp(temp_lab,sys_lab))
			{
	     	FSVolume(0,sys_lab,1);
			}
/*
    FSVolume(0,temp_lab,0);
	if(strcmp(temp_lab,SYS_LAB))
		{
	     FSVolume(0,SYS_LAB,1);
		}
*/
		}
	
#endif


#if 0
  	GetDataIdInfo(D_FLAG,&DataStartSector,&DataLength);
    sdtl_resv_read(DataStartSector,&Rev_Data);
	memset(temp,0,9);
	memset(upgrade,0,32);
	for(i=0;i<6;i++){
		temp[i]=Rev_Data[i];
		}
	strcpy(upgrade,"0:\\");
	strcat(upgrade,upgrade);
	strcat(upgrade,".bin");
	if(FSGetFileSize(upgrade,&fserr)!=0){
		CommonLoadCode(C_UPG);
	}
//#else
	if(FSGetFileSize("0:\\tiraburn.bin",&fserr)>0){//
		CommonLoadCode(C_UPGRADE);
	}
#endif	
	


    set_vidpid();
#if 0//def  TX_DEBUG
	SendString(( char *)"\r\n838");
#endif
	//show_subdir();
	//check_show_subdir();
#if 0//def  TX_DEBUG
	SendString(( char *)"\r\n843");
#endif   
   
		usb_attach_time = 0x180000;
//  	USBStoragePoweroff();
    //USB MassStorage Initial
    MassStorage_Init();
   
#ifdef NIEQING_DIANCHI
	Charge_Init();
#endif

USB_Attach:
        
   	USBSetDebounceCount(usb_attach_time);
//	while( !USBAttachCheck() ){
		// check attached or not
		// do your things, for example: mp3 play
//	}
		
	// USB Storage Initial
	lun=0;
#if 0
	 Update_VID_PID(0x9078,0xAA55);
  	 SerialNumber[0]=0x3000;
	 SerialNumber[1]=0x3100;
	 SerialNumber[2]=0x3200;
	 SerialNumber[3]=0x3300;
	 SerialNumber[4]=0x3400;
	 SerialNumber[5]=0x3500;
	Update_SerialNumber(SerialNumber,sizeof(SerialNumber));
#else
    Update_VID_PID(vid_data,pid_data);
 	Update_SerialNumber(SerialNumber,sizeof(SerialNumber));
#endif

    MassStorage_Registers();
	__asm{BSET USBCON.14};
    while(1)
    {		
	if( MassStorage_Loop()==USB_DISCONN )	
       	{
    		break;
       	}	

#ifdef NIEQING_DIANCHI
	Charge_Check();
#endif	
///*
	if( key_down()){
		break;
		}
//	*/

   	}
    // un-init USB
	MassStorage_Exit();
	//gflag |= 0x0001; 

	if(!USB_DET()){
		 __asm("reset");
		}
	b_usb = TRUE;
	CommonLoadCode(C_OID);	
//	CommonLoadCode(C_MP3);
//	CommonLoadCode(C_WR_TX);

   	goto USB_Attach;
    while(1);
}

//=================================================================================
// EOF
//=================================================================================

