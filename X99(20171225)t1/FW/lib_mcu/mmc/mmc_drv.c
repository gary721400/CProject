//! @file mmc_drv.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the MMC driver routines
//!
//! @version 1.16 snd3-dvk-1_9_5 $Id: mmc_drv.c,v 1.16 2007/07/24 09:48:30 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _mmc_drv_c_
#define _mmc_drv_c_

//#define MMC_USE_INTERRUPT

//_____  I N C L U D E S ___________________________________________________

#include "dp8051.h"
#include "lib_mcu\compiler.h"

#include "mmc_drv.h"
// Include the timer macros
#include "lib_mcu\timer\timer_drv.h"
#include "conf\conf_access.h"
#include "stdio.h"
#include "sio\com.h"
#include "sdhost.h"      // Global constant for SD_CIF 

#if( MEM_MMC!=DISABLED )


//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________
#define CRC7_MSK           0x01

//_____ D E C L A R A T I O N ______________________________________________
_MEM_TYPE_SLOW_ U8 cmdcon;


//extern Bool gfSysUSBMP3Mode;
//_____ F U N C T I O N S __________________________________________________

//! Enable MMC driver
//!
void mmc_drv_on( void )
{
   SDSPITIMER = 0x04;   // enable SD Card reader mode
   // for SD_Host initial, 100KHz~400KHz
   // Either VBUS in or not
   /*if( gfSysUSBMP3Mode )
   {
      // in USB mode
      SDH_CLOCK = 0x4C; 
   }
   else*/
   {
      // in MP3 mode
      SDH_CLOCK = 0x1F;    // CLK down to 380(375)KHz for MMC card, 
   }  // but In USB V_Bus mode will be 375*2.5=937KHz

  	// select SD DMA Buffer
   SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800

   //varAl.USB_DMA_CS = 0;
  	SDIFCON0 = CLK_EN;   //clock enable
  	BLKCNT11_08 = 0;
  	BLKCNT07_00 = 0;
  	BLKLEN11_08 = 0x02;
  	BLKLEN07_00 = 0x00;
  	DATACON = 0x00;

  	// pls note that new FPGA has added one byte timer count
  	DTIMER15_08 = 0xFF;
  	DTIMER07_00 = 0xFF;

   STATUS07_00 = 0xFF;
   STATUS15_08 = 0x03;
}


//! Disable MMC driver
//!
void mmc_drv_off( void )
{
   // for SD_Host initial, 100KHz~400KHz
   // Either VBUS in or not
   /*if( gfSysUSBMP3Mode )
   {
      // in USB mode
      SDH_CLOCK = 0x4C; 
   }
   else*/
   {
      // in MP3 mode
      SDH_CLOCK = 0x1F;    // CLK down to 380(375)KHz for MMC card, 
   }  // but In USB V_Bus mode will be 375*2.5=937KHz
   
  	// select SD DMA Buffer
   SDH_DMA_CS = 0x06;   // select buffer 0 at 0xA800
 
   //varAl.USB_DMA_CS = 0;
  	
  	BLKCNT11_08 = 0;
  	BLKCNT07_00 = 0;
  	BLKLEN11_08 = 0x02;
  	BLKLEN07_00 = 0x00;
  	DATACON = 0x00;

  	// pls note that new FPGA has added one byte timer count
  	DTIMER15_08 = 0xFF;
  	DTIMER07_00 = 0xFF;

  	SDIFCON0 &= ~CLK_EN;

   STATUS07_00 = 0xFF;
   STATUS15_08 = 0x03;

   SDSPITIMER = 0x00;
}


void Check_SDHost_STM( void )
{
   //while( SDHost_STM & 0xF0 ); // wait data STM
   //while( SDHost_STM & 0x01 );
   while( SDHost_STM & 0x03 );
}


//! Send a command on the bus
//!
//! The fifo lock flag is not tested it is under firmware responsability to
//! take care of inter-command delays
//!
//! @param
//!   index:    command index
//!   argument: argument (32 bits) of the command to send
//!   response: expected response to the command to send
//!
Bool mmc_drv_send_cmd( Byte index, Uint32 argument, Byte response )
{
   STATUS07_00 = 0xFF;
   STATUS15_08 = 0x03;
   
   ARG31_24 = MSB0( argument );
   ARG23_16 = MSB1( argument );
   ARG15_08 = MSB2( argument );
   ARG07_00 = MSB3( argument );

   switch (response)
   {
      case MMC_RESP_R1:
      case MMC_RESP_R4:
      case MMC_RESP_R5:
      case SD_RESP_R6:
      case SD_RESP_R7:
      CMDCON &= ~LONGRSP;                    // set 48 bits response
      cmdcon |= CRC7_MSK;                    // set response with CRC7
      CMDCON |= WAITRSP;                     // send command with response
      break;
      
      case MMC_RESP_R2:
      CMDCON |= LONGRSP;                     // set 136 bits response
      cmdcon &= ~CRC7_MSK;                   // without CRC7 because other internal CRC7 management
      CMDCON |= WAITRSP;                     // send command with response
      break;

      case MMC_RESP_R3:
      CMDCON &= ~LONGRSP;                    // set 48 bits response without CRC7
      cmdcon &= ~CRC7_MSK;
      CMDCON |= WAITRSP;                     // send command with response
      break;
      
      case SSS_RESP_C12:                     // Ron@2008/10/23 3S CMD12
      CMDCON = 0x0A;                         // send command with response
      cmdcon |= CRC7_MSK;                    // set response with CRC7
      break;

      case MMC_NO_RESP:
      default:
      CMDCON = 0;                            // send command without response
      Check_SDHost_STM();
      CMDINDEX = index;
      mmc_drv_wait_cmd_trans();
      STATUS07_00 = 0xFF;
      return TRUE;
   }
   Check_SDHost_STM();
   CMDINDEX = index;
   
   // Execpt MMC_NO_RESP, you shall wait response
   mmc_drv_wait_cmd_resp();
   return mmc_drv_check_response();
}


#if ( 0 )
//! Send a short command on the bus
//!
//! The fifo lock flag is not tested it is under firmware responsability to
//! take care of inter-command delays
//!
//! @param
//!   index:    command index
//!   response: expected response to the command to send
void mmc_drv_send_scmd (Byte index, Byte response)
{
   MMCMD = index;
   MMCMD = (Byte)MMC_NO_ARG;
   MMCMD = (Byte)MMC_NO_ARG;
   MMCMD = (Byte)MMC_NO_ARG;
   MMCMD = (Byte)MMC_NO_ARG;

   switch (response)
   {
      case MMC_RESP_R1:
      case MMC_RESP_R4:
      case MMC_RESP_R5:
      case SD_RESP_R6:
      case SD_RESP_R7:
      {
         MMCON0 |= MSK_RFMT;                 // set 48 bits response
         MMCON0 &= ~MSK_CRCDIS;              // set response with CRC7
         MMCON1 |= (MSK_TXCEN|MSK_RXCEN);    // send command with response
         break;
      }
      case MMC_RESP_R2:
      {
         MMCON0 &= ~(MSK_RFMT | MSK_CRCDIS); // set 136 bits response with CRC7
         MMCON1 |= (MSK_TXCEN|MSK_RXCEN);    // send command with response
         break;
      }
      case MMC_RESP_R3:
      {
         MMCON0 |= (MSK_RFMT | MSK_CRCDIS);  // set 48 bits response without CRC7
         MMCON1 |= (MSK_TXCEN|MSK_RXCEN);    // send command with response
         break;
      }
      case MMC_NO_RESP:
      default:
      {
         MMCON1 |= MSK_TXCEN;                // send command without response
         break;
      }
   }
}
#endif


//! This fonction waits the end of command and eventualy response reception
//!
//! the time out must wait 48 bits are transmited or received
//! with frequency of 400KHz to 25MHz
//! time out length depends on mmc frequency 120us at 400KHz
//!                                          2,4us at 20MHz
//!
//! @return OK, or KO for timeout
//!
static Bool mmc_drv_wait( Bool rep_check )
{
   //_MEM_TYPE_SLOW_ U32 u32_timeout;

   //u32_timeout = Timer_set_timer( TIMER_MS( 10 ));
   while( Mmc_cmd_in_progress());
   while( 1 )
   {
      //if( !Mmc_cmd_in_progress())
      {
         if( !rep_check )
            break;
         if( Mmc_resp_received() )
            break;
      }
      //if( Timer_is_timeout( u32_timeout ))
         //return KO;  // No reponse
      if( STATUS07_00 & RSPTIMEOUT )
      {
         return KO;  // No reponse
      }
      /*if( cmdcon & CRC7_MSK )
      {
         if( STATUS07_00 & CRC7ERROR )
         {
            return KO;  // No reponse
         }
      }*/
   }
   return OK;
}


//! This fonction waits the end of command response reception
//!
//! @return OK, or KO for timeout
//!
Bool mmc_drv_wait_cmd_resp(void)
{
   return mmc_drv_wait( TRUE );
}


//! This fonction waits the end of command transmission
//!
//! @return OK, or KO for timeout
//!
Bool mmc_drv_wait_cmd_trans(void)
{
   return mmc_drv_wait( FALSE );
}


//!   Check command response
//!
//! @return
//!   MMC_ERR_RESP : no response or bad format received
//!   MMC_RESP_OK  : response received
//!
Bool  mmc_drv_check_response (void)
{
   if (!Mmc_resp_received())
   {
      STATUS07_00 = 0xFF;
      STATUS15_08 = 0x03;
      return FALSE;  // no response received
   }
   // here, response received
   
   if(( cmdcon & CRC7_MSK ) == 0 )
   {
      STATUS07_00 = 0xFF;
      STATUS15_08 = 0x03;
      return TRUE;   // CRC7 not computed
   }
   
   if(( STATUS07_00 & CRC7ERROR ) != 0 )
   {
      STATUS07_00 = 0xFF;
      STATUS15_08 = 0x03;
      return FALSE;  // format or CRC7 error
   }

   STATUS07_00 = 0xFF;
   STATUS15_08 = 0x03;
   return TRUE;      // CRC7 computed and OK
}


//! Read command argument response
//!
//! @return
//!   4-byte argument of the response
//!
Uint32 mmc_drv_read_response (void)
{
    U32  argument;

   ((Byte*)&argument)[0] = RESP119_112;
   ((Byte*)&argument)[1] = RREP111_104;
   ((Byte*)&argument)[2] = RREP103_096;
   ((Byte*)&argument)[3] = RREP095_088;

   return argument;
}

//! Waits the end of busy signal on DAT0
//! There is no time out cause the signal goes high when the card is removed
//!
void  mmc_wait_busy_signal ( void )
{
   Mmc_force_clk();
   while(Mmc_read_busy_signal() == 0);
   //Mmc_unforce_clk();
}

#endif	// #if ( MEM_MMC != DISABLED )

#endif 	// _mmc_drv_c_
