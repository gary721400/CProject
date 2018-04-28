
#pragma CODE = USB_HID_CODE
#pragma DATA = USB_HID_XRAM

#include <string.h>
#include "KBDM9_DDK.h"
#include "SDimg.h"
#include "sio\com.h"
#include "modules\calibration\Calibration.h"


unsigned int   HID_rx_len;

#define BYTE  unsigned char
#define DWORD unsigned long
#define WORD  unsigned int

typedef struct
{
   union
   {
      BYTE data[ 64 ];
      struct _Cmd
      {
         DWORD sig;
         WORD  op;
         WORD  param;  
         WORD  SN[ 16 ];
      }cmd;
   }uuu;
}_tdHidCmd;

typedef enum
{
   //-- audio switch operation
   //OP_HID_AUDIO_SWITCH = 1,                  //Param ==> 0 is Remove USB Audio, 1 is else.
   //-- configure operation
//	OP_FMTX_FTX_FREQ  		= 0x0100,         //Param ==> Value:11bits, Unit:100KHz, ex: 881 = 88.1MHz
//	OP_FMTX_MONO_MODE 		= 0x0101,         //Param ==> 1 is monaural operation, 0 is stereo opertaion
//	OP_FMTX_PD_PHASE  		= 0x0102,         //Param ==> see _enPDPHASE
//	OP_FMTX_RF_POWER  		= 0x0103,         //Param ==> see _enRFPOWER
//	OP_NIB_RECTIFY_DAT 		= 0x0104,         //Param ==> value:0~20 = gain:0db~-20db, 21 = Mute
//	OP_NIB_RECTIFY_CMD		= 0x0105,         //param ==> Serial Number
   //-- configure operation
//	OP_FMTX_GET_FTX_FREQ  	= 0x0200,         //Param ==> Value:11bits, Unit:100KHz, ex: 881 = 88.1MHz
//	OP_FMTX_GET_MONO_MODE 	= 0x0201,         //Param ==> 1 is monaural operation, 0 is stereo opertaion
//	OP_FMTX_GET_PD_PHASE  	= 0x0202,         //Param ==> see _enPDPHASE
//	OP_FMTX_GET_RF_POWER  	= 0x0203,         //Param ==> see _enRFPOWER
   OP_NIB_GET_RECTIFY_DAT	= 0x0204,         //Param ==> value:0~20 = gain:0db~-20db, 21 = Mute
   OP_NIB_GET_RECTIFY_ACK 	= 0x0205,//0x0205,//         //Param	==> Get Product Serial Number
}_enHID_OP;

__X   _tdHidCmd   gHidCmd;

extern __X unsigned int RectifyStatus;
extern __X unsigned int CalibrationData[ 256 ];
extern void Nib_Rectify( void );


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


void HID_report_rx( unsigned int length )
{
   unsigned long Cnt;
   __X _tdHidCmd* pcmd = (__X _tdHidCmd* )ep0_buf;

   if( pcmd->uuu.cmd.sig != 0xFECACAFE )
      goto RX_EXIT; 

   switch( pcmd->uuu.cmd.op )
   {
      //case OP_HID_AUDIO_SWITCH:
      //break;
 
      case OP_NIB_GET_RECTIFY_DAT:
      //__asm{ NOP }
      gHidCmd.uuu.cmd.op = OP_NIB_GET_RECTIFY_DAT;
      break;

      case OP_NIB_GET_RECTIFY_ACK:
      //__asm{ NOP }
      //Nib_Rectify();
      gHidCmd.uuu.cmd.op = OP_NIB_GET_RECTIFY_ACK;
      //HID_get_report();
      //Nib_Rectify();
      calib_state = CALIB_START;
      RectifyStatus = 0;
      break;

      default:
      gHidCmd = *pcmd;
      break;
   }
RX_EXIT:    
   return;
}


report_rx_func HID_set_report( unsigned int rx_len )
{
   HID_rx_len = rx_len;
   return HID_report_rx;
}


void HID_get_report( __X unsigned char* buf, unsigned int length )
{
   unsigned int* pt;
   unsigned long Cnt;
   unsigned long SectorAddress, DataLength;

	switch( gHidCmd.uuu.cmd.op )
	{
      case OP_NIB_GET_RECTIFY_DAT:
      GetDataIdInfo( D_CAB, &SectorAddress, &DataLength );
#if 0//def SDBOOT_KBDM9
      sdtl_resv_read( SectorAddress, CalibrationData );	  
#endif       
#if 1//def SPIBOOT_KBDM9
      spif_resv_read( SectorAddress, CalibrationData );
#endif 

      ep0_buf[ 0 ] = ep0_buf[ 2 ];
      ep0_buf[ 1 ] = 0x11;

      pt = ( unsigned int *)&CalibrationData;
      memcpy( ep0_buf + 2, pt, 62 );

      gHidCmd.uuu.cmd.op = 0;
      gHidCmd.uuu.cmd.param += 64;
      break;

      case OP_NIB_GET_RECTIFY_ACK:
      //Nib_Rectify();
      ep0_buf[ 0 ] = ep0_buf[ 2 ];
      ep0_buf[ 1 ] = RectifyStatus;    //PASS
      //ep0_buf[ 1 ]	= 0x00;           //FAIL
      //gHidCmd.uuu.cmd.op = 0;
      //gHidCmd.uuu.cmd.param += 64 ;	
      break;
	        
      default:
      (( unsigned long __X *)ep0_buf )[ 0 ] = ( unsigned long )0xFECACAFE;
      break;
   }
}

