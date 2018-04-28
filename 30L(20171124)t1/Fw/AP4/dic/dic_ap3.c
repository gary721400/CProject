//!
//! @file mmi_ap3.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the MMI ap3
//!
//! @version 1.26 snd3b-dvk-1_0_33 $Id: mmi_ap3.c,v 1.26 2007/3/15 9:20:06 Gary_liu $
//!
//! @todo
//! @bug
//!

#pragma CODE = DIC_AP3_CODE
#pragma DATA = DIC_AP3_DATA


#ifndef _dic_ap3_c_
#define _dic_ap3_c_

//_____  I N C L U D E S ___________________________________________________
#include "compiler.h"
#include <stdio.h>
#include <string.h>
#include "conf_kb.h"
#include "system.h"
#include "dic_kernel.h"
#include "mp3_api.h"
#include "keypad.h"
///#include "keypad_drv.h"
#include "dic_ap3.h"
#include "dic_search.h"
#include "MP3_API.h"   
#include "com.h"
#include "conf_oid.h"
#include "dic_file.h"
#include "oid.h"
#include "dic_remote.h"
#include "filesearch.h"




#if 1//( AP3 == ENABLE )
//_____ P R I V A T E   D E F I N I T I O N S _____________________________
   _MEM_TYPE_SLOW_   U8       Ap3_MenuPath;
//   _MEM_TYPE_SLOW_   U16      ap3_file_number;
//const U8	 player_dir[]= "Mp3";
//const  U8       ADB[] = "ADB";
/*
#ifdef UNCOIDE_FORMAT
const  U16      root[] = {'0',':','\\','\0'};
const  U8		subdir[] = {0x7C,0xFB,0x7E,0xDF,0x65,0x87,0x4E,0xF6,0x00,0x00};//
const  U16 BATTERY_LOW_FILE[] = {'b','a','t','t','e','r','y','l','o','w','.','m','p','3','\0'};
const  U16 POWER_OFF_FILE[] = {'p','o','w','e','r','o','f','f','.','m','p','3','\0'};
const  U16 DIC_MODE[]={'d','i','c','t','i','o','n','a','r','y','.','m','p','3','\0'};
const  U16 NO_DIC[]={'n','o','d','i','c','f','i','l','e','.','m','p','3','\0'};
const  U16 DEL[] = {'d','e','l','.','m','p','3','\0'};
const  U16 ERROR[] = {'e','r','r','o','r','.','m','p','3','\0'};
#endif 
*/
//const  U8 root[] = "0:\\";
//const  U8 subdir[] = "subdir";

/*
#ifdef ASCII_FORMAT

const  U8 BATTERY_LOW_FILE[] = "batterylow.mp3";
const  U8 POWER_OFF_FILE[] = "poweroff.mp3";
const  U8 DIC_MODE[]="dictionary.mp3";
const  U8 NO_DIC[]="nodicfile.mp3";
#endif
*/


//const  U8       DICT[] = "dic";




//#define Remote_Dic       "0:\\adb\\Default.adb"
//#define AP4_GENDU_FILE   "0:\\ap4\\record.wav"

char tisiFileName[128];
//char	__X	g_full_path[256];

// UnicodeBuffer g_string   [G_STRING_SIZE ];
// UnicodeBuffer g_string2  [G_STRING_SIZE2];
   _MEM_TYPE_SLOW_   U8       ap4_play_state;
                     Bool           g_b_open_file_error;

 
   _MEM_TYPE_SLOW_   Bool      poweroff_falg;
   _MEM_TYPE_SLOW_   Bool      flag_load_mp3_dac;

//extern __X  U16				 adpcm_playwav_num,adpcm_record_num;
//extern unsigned int systerm_flag;
//extern __X  U16         rp_flag;

_MEM_TYPE_SLOW_   U16      sd1_adb_file_number;
_MEM_TYPE_SLOW_   U16      sd2_adb_file_number;
_MEM_TYPE_SLOW_   U16      curAdbFile;
extern _MEM_TYPE_SLOW_  U32             actAddressBak;
void Delay_ms(int count);
//extern void SpeakDetect(void);
extern void DAC_Stereo_to_Mono(unsigned Option);
void link_tishi_filename(U8 flag);


//DEFINE  ALL BITS OF AP4_MP4_FLAG 
#define BANK_FORM_MP3       	0X8000
#define BANK_FORM_GAME          0X4000
#define BANK_HAVE_INDEX     	0X2000
#define CURRENT_CONTROL_GAME 	0X1000

#define CLUSTER_BUF_SIZE 4096
#define SKIPS_CLUSTER 1024
__X char dic_cluster_buf[CLUSTER_BUF_SIZE];
extern _MEM_TYPE_SLOW_  U16 current_adb_sd;
/*
extern unsigned int gflag;
extern unsigned int ap4_mp3_flag;

extern unsigned int bak_ap4_num;
extern unsigned int bak_book_num;
extern unsigned int bak_page_num;
extern unsigned int bak_control_mode_num;
extern unsigned int prev_task;
*/
extern Bool OID_PLAY;
extern Bool                  g_b_player_on;

extern Bool   oid_control_flag;


//extern void swap_code( U16 flag);
//extern void play_mp3_from_addr(U32 addr,U8 flag,U8 led_flag);
//extern void  delay1ms    ( int count );
//***********************************************
// ID of the timer soft used by this MMI appli
//_MEM_TYPE_SLOW_ U8  s_ts_ap3record_display_id;

//#define AP3RECORDER_TIME   TIMER_MS( 500 ) 


//extern U32		swap_code_flag;

//extern void vol_inc(void);
//extern void judge_op(void);
extern void vol_inc(void);
//extern tAudioErr AudioMP3SetVol(int volume);
//extern void save_wav_num(void);
Bool  g_b_search_record;
Bool  b_kbd_process_disable;
//_____ P R I V A T E   D E C L A R A T I O N S ____________________________

extern void AudioMP3FileOpenFirst(char *file);
extern MP3API_STATUS AudioMP3FileCloseAfter( void );
extern void ledFlash(void);
/*********************** Menus **********************/

//_MEM_TYPE_SLOW_ U8  current_api;

//add by xuanyu
extern _MEM_TYPE_SLOW_  U8    Point_Read;
extern _MEM_TYPE_SLOW_  U8    Point_Write;

extern   xdata 		U8 				current_api;
//extern unsigned int vol_idx;
//extern const int VolTbl[7];
//extern unsigned int volume_inc_flag;
//extern unsigned int g_string2[30];
//extern  void  player_stop_stream    ( void );
//extern  void  player_codec_stop     ( void );
extern Bool g_b_search_on;
extern FCFILE __X *	fat_g_cur_file;
extern void interrupt_play(void);
void readyGoNewPorject(unsigned int subProject);
///=========================================================================

//ImageDeclareType(POWER_SYSTEM_OFF_CODE);
ImageDeclareType(DIC_LINK_TISHI_CODE);
//ImageDeclareType(DAC_COMMON_CODE);
//ImageDeclareType(MP3_COMMON_CODE);
ImageDeclareType(DAC_COMMON_CODE);
ImageDeclareType(MP3_COMMON_CODE);

///==========================================================================


void dowith_USB(void)
{/*
	if( player_ap3_is_busy())
	     player_ap3_stop();
         // Delay_ms(10); // VBUS(5V) Debounce
           file_close();
            b_find_usb_flag = TRUE;
          CommonLoadCode(C_POWEROFF);
		  */
}


//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
#if 1
//------------------------------------------------------------------------------
// Key Scan Process Function
//------------------------------------------------------------------------------
void Ap4_Key_Process(U16 Key_Code)
{
#ifdef TX_DEBUG
   	 SendString("\r\ntemp_key=");
   	 send_hex_u16(Key_Code);
#endif    
    switch(Key_Code)
    {

    	case (KBD_KEY_RELEASED<<8|KEY_POWER): // ap4 pause
#if 1			
			 if((!g_b_player_on)&&(b_play_pause))
        	{
        	    g_b_player_on = TRUE;
				b_play_pause = FALSE;
				vol_idx_bak = VOL_NUM;
				op_stop_timer();
				state_search_to_idle();
        		AudioMP3Continue();
				clkup_time(FALSE);
				break;
			}
			
          if((g_b_player_on)&&(!b_play_pause))
        	{
        		g_b_player_on = FALSE;
				b_play_pause = TRUE;
				state_search_to_goonplay();
				AudioMP3Pause();
				//OP_CLOSE();
				break;
			}
#else
			
			interrupt_play();
			/*
			 if( player_ap3_is_busy())
			 player_ap3_stop();
			 file_close();
			 bak_to_news();
			 prev_task=C_OID;
			 CommonLoadCode(C_MP3);
			 */
#endif		 
            break;		
		case (KBD_KEY_RELEASED<<8|KEY_MEN): //
		/*
			 if( player_ap3_is_busy())
			 player_ap3_stop();
			 file_close();
			 bak_to_news();
			 prev_task=C_OID;
			 wav_action = WAV_PLAY_FLAG;
			 CommonLoadCode(C_REC_PASTER);	 
			 */
			 interrupt_play();
			  file_close();
			  b_back_from_mp3 = TRUE;
			  b_back_have_key = TRUE;
			  bak_current_index = STATE_KEY_RELEASED|KEY_MEN;
			 CommonLoadCode(C_OID);
			 break;
		case (KBD_KEY_PRESSED<<8|KEY_MODE): // 
			 interrupt_play();
			 file_close();
			// bak_to_news();
			 CommonLoadCode(C_MP3);
            break;	
		case (KBD_KEY_RELEASED<<8|KEY_REC): // 
			 interrupt_play();
			 file_close();
			// bak_to_news();
			 wav_action = WAV_RECORD_FLAG;
			 CommonLoadCode(C_REC_PASTER);
            break;	
		case (KBD_KEY_RELEASED<<8|KEY_VOL_INC):
			//if(g_b_player_on&&b_vol_play){
				//interrupt_play();
			//	}
			if(vol_idx < (VOL_NUM-1)){
				vol_idx++;
				}
			
			if(!g_b_player_on){
				if(vol_idx == (VOL_NUM-1)){
					//interrupt_play();
					tishi_mp3(FLAG_MAXVOL);
					}
				else{
					//(void)act_do(All_Control.Addr_Antion_key, 0);
					//state_search_to_play();
					tishi_mp3(FLAG_VOLUME);
					}
				}
			break;
		case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC):
			//if(g_b_player_on&&b_vol_play){
				//interrupt_play();
			//	}
				if(vol_idx > 0){
					vol_idx--;
				}
			if(!g_b_player_on){
				if(vol_idx == 0){
					//interrupt_play();
					tishi_mp3(FLAG_MINVOL);
					}
				else{
					//(void)act_do(All_Control.Addr_Antion_key, 0);
					//state_search_to_play();
					tishi_mp3(FLAG_VOLUME);
					}
				}
		break;

//		 case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC): // ap4 Volume change
/*
        case (KBD_KEY_RELEASED<<8|KEY_VOL_DEC): // ap4 Volume change
				if( vol_idx > 0 )
					{
						vol_idx--;
					}
				if(!g_b_player_on)
				{
					(void)act_do(All_Control.Addr_Antion_key, 0);
					state_search_to_play();
				}
            break;
			*/

		case (KBD_KEY_LONGPRESSED<<8|KEY_POWER):
			 interrupt_play();
			 scan_oid_disable();
			 Kbd_set_evt_disabled();
			 tishi_mp3(FLAG_POWER_OFF_FILE);
		 	 CommonLoadCode(C_POWEROFF);
			 break;
			 
		case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_DEC):
			//interrupt_play();
			//SDKLoadCode(KEYLONG_VOL_CODE);
			//keylong_vol(KEY_VOL_DEC);
			break;
		case (KBD_KEY_LONGPRESSED<<8|KEY_VOL_INC):
			// interrupt_play();
			// SDKLoadCode(KEYLONG_VOL_CODE);
			// keylong_vol(KEY_VOL_INC);
			 break;	
		case (KBD_KEY_LONGPRESSED<<8|KEY_REC): // 
			 interrupt_play();
			 file_close();
			 //bak_to_news();
			 wav_action = WAV_PLAY_FLAG;
			 CommonLoadCode(C_REC_PASTER);
            break;	
			
        default:
            break;
    }
}
#endif




//_____ D E F I N I T I O N S ______________________________________________
//!
//! ap3_mmi_appli
//!
//! Manage the application template 
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param U8  event
//!        U16 param
//!
//! @return none
void ap3_mmi_appli(void)
{
#if 1
     poweroff_falg = FALSE;
   //  create_gendu_file();
//	SDKLoadCode(AP3_OPEN_FILE_CODE);
      if( !scank_disk())
      {
		tishi_mp3(FLAG_NODIC);
       //adb_file_number = 0;
	    CommonLoadCode(C_OID);
      }
     //g_b_search_on = TRUE;

      State_Search = Open_File_Search;
      g_b_open_file_error = FALSE;
		

#endif   
}
/*
int	str_comp_a(U8 * sl,U8 * sr,U8 use_case)
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
*/

Bool scank_disk( void )
{

	char temp_dir[256];
	int  temp_data;
	curAdbFile = 0;

	if(b_sd1_ok){
		strcpy(temp_dir,SD1_DIC_DIR);
		sd1_adb_file_number = file_get_ext_count((char *)&temp_dir, (char *)DIC_EXE);
		}
	if(b_sd2_ok){
		strcpy(temp_dir,SD2_DIC_DIR);
		sd2_adb_file_number = file_get_ext_count((char *)&temp_dir, (char *)DIC_EXE);
		}

#ifdef TX_DEBUG
   	SendString("\r\nsd1_number:sd2_num=");
   	send_hex_u16(sd1_adb_file_number);
	SendString(":");
   	send_hex_u16(sd2_adb_file_number);
#endif   

	if(sd1_adb_file_number!=0){
		current_adb_sd = FLAG_SD1_CON;
		return TRUE;
		}
	 if(sd2_adb_file_number!=0){
		current_adb_sd = FLAG_SD2_CON;
		return TRUE;
		}
		return FALSE;

}

//the funcion is open the first file of changed file;
U8 open_file( U16 fileNum )
{
   U16 _MEM_TYPE_SLOW_ iter;
   U16 _MEM_TYPE_SLOW_ num;
	__X FS_ERR err;

	char temp_dir[256];
	int  temp_data;
	unsigned long temp_len;
	

	if(current_adb_sd == FLAG_SD1_CON){
		strcpy(temp_dir,SD1_DIC_DIR);
		}
	else{
		strcpy(temp_dir,SD2_DIC_DIR);
		}	
		//strcpy(temp_dir,DIC_DIR);
		if(!file_get_path((char *)&temp_dir,(char *)DIC_EXE,fileNum))
			{
			 return 0xFF;
			}
		strcpy(temp_dir,g_full_path);		

//	*fat_g_cur_file = FSOpen(temp_dir,FS_MODE_READ,&err);	
#ifdef TX_DEBUG
   	SendString("\r\ncurrent_adb=");
	SendString(temp_dir);
#endif
//	fat_g_cur_file = (FCFILE __X *)_FSOpenForFastSeek((char *)temp_dir,FS_MODE_READ,(__X void*)&dic_cluster_buf,CLUSTER_BUF_SIZE,&err);
	fat_g_cur_file = (__X FCFILE*)_FSOpenForFastSeek((char *)temp_dir,FS_MODE_READ,(__X void *)&dic_cluster_buf,CLUSTER_BUF_SIZE,(__X FS_ERR *)&err);
	if(err==FS_NO_ERROR){
		return 0xAA;
		}
	else{
		return 0xFF;
		}
}
//===============================================

void openNewAdbFile(void)
{
	Bool flag = false;
	U16 temp = 0;
	U16 encry_flag;
	
	//if(0xFF!=open_file(curAdbFile)){
	//	 return;
	//  	}
	 
	temp = curAdbFile;
	while(temp < sd1_adb_file_number){
			if(0xFF!=open_file(temp)){
			 curAdbFile = temp;
#if(DIC_ENCRY ==  ENABLE)	
				( void )file_seek( 0, FS_SEEK_SET );
   				( void )file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&encry_flag, sizeof( U16 ));
   				if( encry_flag != 0xFFEE ){
					file_close();
					continue;
   					}
#endif
			 return;
		  	}
		}
	///*
	temp = 0;
	while(temp < curAdbFile){
		if(0xFF!=open_file(temp)){
		 curAdbFile = temp;
#if(DIC_ENCRY ==  ENABLE)	
			( void )file_seek( 0, FS_SEEK_SET );
				( void )file_read_buf(( U8 _MEM_TYPE_SLOW_ *)&encry_flag, sizeof( U16 ));
				if( encry_flag != 0xFFEE ){
				file_close();
				continue;
					}
#endif
		 return;
	  	}
	}
	//*/
	 CommonLoadCode(C_OID);
}

/*
void close_record_file(void)
{
	FSClose(TEMP_File);
}
*/


//#pragma CODE = DIC_LINK_TISHI_CODE
//===============================================


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


#if 0
//change ap3 file

//get ap3 file name's header
U16 get_name_lead( void )
{
   U8 *p = ( U8 *)( g_string2 );
   _MEM_TYPE_SLOW_ U16 iNameLead;

   //bRemoteChoiceAp3 = FALSE;
   iNameLead = 0;
   if( !((*p >= '0' )&&( *p <= '9')))
   {
      //bRemoteChoiceAp3 = TRUE;
      return ( RemoteNumber + 1 );
   }
   while((*p >= '0' )&&( *p <= '9'))
   {
      iNameLead = iNameLead * 10 + *p - '0';
      p++;
   }

   return iNameLead;
}
#endif



Bool	find_entry(U8 * spec,U16 * name,Bool is_dir,FILE_INFO __X * finfo,FS_ENTRY_INFO __X * entry)
{
//	if(FSGetEntry(spec,0,finfo,entry)!=FS_NO_ERROR)
//		return	FALSE;
	U16	buffer[25];
	do
	{
		if((finfo->attribute&FS_ATTR_LFN_ENTRY)!=FS_ATTR_LFN_ENTRY)
		{
			local_to_unicode(finfo->name,buffer,sizeof(buffer));
			if(str_comp_u((U16 *)finfo->lfn,name,FALSE)==0||str_comp_u(buffer,name,FALSE)==0)
			{
				if(is_dir)
				{
					if(finfo->attribute&FS_ATTR_DIRECTORY)
						return	TRUE;
				}
				else
				{
					if(!(finfo->attribute&FS_ATTR_DIRECTORY))
						return	TRUE;
				}
			}
		}
	}
	while(FSGetNextEntry(finfo,entry)==FS_NO_ERROR);
	return FALSE;
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
   return FALSE;
}


//==============================================
void tishi_mp3(U8 flag)
{

	    FS_ERR fs_err;
	    tAudioErr   Audio_err;

		unsigned long len=0;
		char path[64];
        U16	m[] = {'\\','\0'};
		//swap_code(AP3_OPEN_FILE_CODE_FLAG);
 		//swap_code(FILE_EXTERN_CODE_FLAG);
 		//judge_op();
		SDKLoadCode(DIC_LINK_TISHI_CODE);
		link_tishi_filename(flag);
#ifdef UNCOIDE_FORMAT
		if(!find_path_u((U16 *)&tisiFileName,(U8 *)path))return;
#endif
#ifdef ASCII_FORMAT
		if(!find_path_a((U8 *)tisiFileName,(U8 *)path))return;
#endif	
	
		len = FSGetFileSize((char *)path, &fs_err);
		if(0==len)return;
		strcpy(tisiFileName,path);
#ifdef TX_DEBUG
		SendString("\r\ntishimp3=");
		SendString(tisiFileName);
#endif			
//		swap_code(DAC_CODE_FLAG);
		//sysup_opstart();
		clkup_time(TRUE);
		OID_PLAY = FALSE;
		//g_b_play_afx = FALSE;
	    //SDKLoadCode(DAC_COMMON_CODE);
	    //SDKLoadCode(MP3_COMMON_CODE);
	    SDKLoadCode(DAC_COMMON_CODE);
		SDKLoadCode(MP3_COMMON_CODE);
		AudioMP3FileOpenFirst( (char *)tisiFileName );
        Audio_err = AudioMP3Init1k0(0);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 return;
        	}
 //       Audio_err = AudioMP3Open((signed char *)tisiFileName);
			//DAC_HeadPhone_Volume(0);
			//judge_op();
			//vol_inc();
			//DAC_Stereo_to_Mono(1);
 		Audio_err = AudioMP3Open((char *)path);
        if(Audio_err != AUDIO_ERROR_OK)
        	{
        	 	Audio_err = AudioMP3Uninit();
				return;
        	}
#if 0
        AudioMP3SetEQ(EQ_NOMAL);
//		DAC_Enable_HeadPhone();
//		DAC_HeadPhone_Volume(VolTbl[vol_idx]);
//		OP_CLOSE();
//		SpeakDetect();
		DAC_Enable_HeadPhone();
		judge_op();
		DAC_HeadPhone_Volume(0);
//		judge_op();
		vol_inc();
//#else
		OP_CLOSE();
		DAC_Headphone_Reset();
    	
    	DAC_HeadPhone_Volume(VolTbl[vol_idx]); // 此處的Vol為UI之前設定的音量值
    	DAC_Enable_HeadPhone();
    	DAC_Go();
		judge_op();
#endif
		vol_idx_bak = VOL_NUM;
		SpeakDetect(TRUE);
		//DAC_FadeIn();
		op_stop_timer();
		clkup_time(FALSE);
        Audio_err = AudioMP3Play();
    	while (CheckPlaying(FALSE))
    	{   
///*    	
	    	ledFlash();
//*/			  
    	}
	//	DAC_FadeOut();
 //   	OP_CLOSE();
 		Audio_err = AudioMP3Stop();
    	Audio_err = AudioMP3Close();
        Audio_err = AudioMP3Uninit();
		AudioMP3FileCloseAfter();
		op_start_timer();
		clkup_time(TRUE);
}


void readyGoNewPorject(unsigned int subProject)
{
	//prev_task = C_DIY;
	CommonLoadCode(subProject);
	
}

#pragma CODE = DIC_LINK_TISHI_CODE
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

void link_tishi_filename(U8 flag)
{
		char path[64];
        U16	m[] = {'\\','\0'};
 #ifdef UNCOIDE_FORMAT
		strcpy_u((U16 *)tisiFileName,(U16 *)&root);
		strcat_u((U16 *)tisiFileName,(U16 *)&subdir);
		strcat_u((U16 *)tisiFileName,(U16 *)&m);
		switch (flag)
			{
				case 0:
					strcat_u((U16 *)tisiFileName,(U16 *)&DIC_MODE);
					break;
				case 2:
					strcat_u((U16 *)tisiFileName,(U16 *)&BATTERY_LOW_FILE);
					break;
				case 3:
					strcat_u((U16 *)tisiFileName,(U16 *)&POWER_OFF_FILE);
					break;	
				case 4:
					strcat_u((U16 *)tisiFileName,(U16 *)&NO_DIC);
					break;	
				case 6:
					strcat_u((U16 *)tisiFileName,(U16 *)&ERROR);	
					break;	
				case 7:
					strcat_u((U16 *)tisiFileName,(U16 *)&DEL);		
				default:
					return;
					break;
			}
#endif
#ifdef ASCII_FORMAT
		strcpy(tisiFileName,(char*)ROOT);
		strcat(tisiFileName,(char *)SUBDIR);
		strcat(tisiFileName,"\\");
		switch (flag)
			{
				case FLAG_GO_DIC:
					strcat(tisiFileName,(char *)GO_DIC);
					break;
				case FLAG_BATTERY_LOW_FILE:
					strcat(tisiFileName,(char *)BATTERY_LOW_FILE);
					break;
				case FLAG_POWER_OFF_FILE:
					strcat(tisiFileName,(char *)POWER_OFF_FILE);
					break;	
				case FLAG_NODIC:
					strcat(tisiFileName,(char *)NO_DIC);
					break;	
				case FLAG_ERROR:
					strcat(tisiFileName,(char *)ERROR_FILE);
					break;		
				default:
					return;
					break;
			}
#endif	
}

#endif  //#if( AP3 == ENABLE )

#endif // _mmi_ap3_c_

