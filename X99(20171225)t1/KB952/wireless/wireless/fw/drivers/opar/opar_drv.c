
#ifndef _opar_drv_c_
#define _opar_drv_c_

#include "lib_mcu\compiler.h" 
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "sio\com.h"
#include "opar_drv.h"
#include "mmi\kbd\keypad.h"
#include "timer_drv.h"
#include "string.h"

#include "mbc_decode.h"
#include "IRControl.h"
#include "668.h"
#include "cis.h"
#include "SDImg.h"
#include "oid.h"


#pragma DATA = OPAR_DRV_DATA
#pragma CODE = OPAR_DRV_CODE

ImageDeclareType( CIS_INIT_CODE );
ImageDeclareType( CIS_INIT_TEMP_CODE00 );

#define Bright_Dark      ((unsigned long)76800)   //one pixel is 8*6=48  76800

extern __X   U32   Start_Sector, Length;
extern __X   U16   Buf[ 256 ];

__X   unsigned int   IR_Level[ 7 ][ 2 ];

__X   unsigned int   IR_Count;

__X   unsigned int   currentIndex;
__X   unsigned long  IndexBak;

__X   unsigned long  IndexDataAddress = 0;


extern void DotExtraction( __X unsigned char *image_g );


#pragma CODE = TASK_INIT_CODE

void opar_init( void )
{
   SDKLoadCode( CIS_INIT_CODE );
   CIS_Init();

   SDKLoadCode( CIS_INIT_TEMP_CODE00 );
   GC0309_Write_Sensor_Initial_Setting();

   GetDataIdInfo( D_IR, &Start_Sector, &Length );
#ifdef SDBOOT_KBDM9
   sdtl_resv_read( Start_Sector, ( __X void *)Buf );
#endif
#ifdef SPIBOOT_KBDM9
   spif_resv_read( Start_Sector, ( __X void *)Buf );
#endif

   memcpy( IR_Level, Buf, sizeof( IR_Level[ 0 ][ 0 ])* 14 );

   GetDataIdInfo( D_SONIX, &Start_Sector, &Length );
#ifdef SDBOOT_KBDM9
   sdtl_resv_read( Start_Sector, ( __X void *)Buf );
#endif
#ifdef SPIBOOT_KBDM9
   spif_resv_read( Start_Sector, ( __X void *)Buf );
#endif
   IndexDataAddress = 0;

   IR_Count = 6;
 	
	LeftIR_Level( IR_Level[ IR_Count ][ 0 ]);
	RightIR_Level( IR_Level[ IR_Count ][ 1 ]);

   cis_line_go = 0;
#if( SONIX_CODE_TYPE != SONIX_CODE_NORMAL )
   index_check = 0;
#endif   //#if( SONIX_CODE_TYPE != SONIX_CODE_NORMAL )

   IndexBak = 0xFFFFFF;
   currentIndex = 0xFFFF;
}


#pragma CODE = OPAR_DRV_CODE

void opar_task( void )
{
   __X unsigned char temp2, t2;
   __X unsigned long temp1, k;
   __X unsigned char t1[ 9 ];
   __X unsigned int index_temp;
   
   if( cis_line_go == 0 )
      cs_frame_go();
   else if( cis_line_go == 2 )
   {
      ReadBright();
      if( C_Bright < Bright_Dark )
      {
         IndexBak = 0xFFFFFF;
         cis_line_go = 3;

         IR_Count = 0;

#if( USE_STAT_CODE == TRUE )
         stat_code_init();
#endif
      }
      else
      {
         DotExtraction( cis_line_tmp3 );
         
         if( g_dots_count > 13 )
         {
			   k = decode_num( t1, &t2 );
            
   			if( t2 == Decode_OK )
   			{
               index_temp = ( unsigned int )k;
               if( g_axis.type == TYPE_SONIX )
               {
                  temp1 = ( unsigned long )( k >> 8);
                  temp2 = ( unsigned char )k;

                  temp1 += Start_Sector;
                  if( IndexDataAddress != temp1 )
                  {
                     IndexDataAddress = temp1;
#ifdef SDBOOT_KBDM9
                     sdtl_resv_read( temp1, Buf );
#endif
#ifdef SPIBOOT_KBDM9
                     spif_resv_read( temp1, Buf );
#endif
                  }
                  //currentIndex = ReserveCalibrationData[ temp2 ];
                  index_temp = Buf[ temp2 ];
#if( SONIX_CODE_TYPE == SONIX_CODE_FUZHOU )
                  if( 1 == index_check )
                  {
                     if(( index_temp <= 5000 )||( index_temp >= 7000 ))
                     {
                        goto next_image;
                     }
                  }
                  else
                  {
                     if(( index_temp > 5000 )&&( index_temp < 7000 ))
                     {
                        goto next_image;
                     }
                  }
#endif   //#if( SONIX_CODE_TYPE == SONIX_CODE_FUZHOU )
               }

               currentIndex = index_temp;
               if( IndexBak != currentIndex )
               {
                  g_b_oid_data_flag = TRUE;
                  Current_Index = currentIndex;
#if (POWER_OFF_AUTO == ENABLE)
                  power_rearm_autopoweroff_delay();
#endif
               }
               IndexBak = currentIndex;
               
   				cis_line_go = 4;
   			}
            else
            {
#if( SONIX_CODE_TYPE != SONIX_CODE_NORMAL )
next_image:
               index_check =( index_check + 1 )& 0x01;
#endif   //#if( SONIX_CODE_TYPE != SONIX_CODE_NORMAL )

   			   cis_line_go = 3;

change_ir:
               IR_Count += 1;
               if( IR_Count > 6 )
               {
                  IR_Count = 0;
               }
            }
         }
         else
         {
            cis_line_go = 3;
            goto change_ir;
         }
      }
      
      cis_line_go = 0;
      RightIR_Level( IR_Level[ IR_Count ][ 0 ]);
      LeftIR_Level( IR_Level[ IR_Count ][ 1 ]);
   }
}


#endif   //_opar_drv_c_
