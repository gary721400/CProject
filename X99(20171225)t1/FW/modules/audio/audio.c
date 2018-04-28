//! @file audio.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the player management routines
//!
//! @version 1.75 snd3-dvk-1_9_5 $Id: audio.c,v 1.75 2007/07/26 14:43:53 sguyon Exp $
//!
//! @configuration
//! @todo SG !!rework audio_int
//! @bug
#ifndef _audio_c_
#define _audio_c_

//_____  I N C L U D E S ___________________________________________________

#include "config.h"
#include "conf\conf_audio.h"                 // Audio user's configuration

#include "modules\audio\audio.h"             // Player module definition
//#include "modules\control_access\ctrl_access.h" // Codec load

#include "lib_mcu\timer\timer_drv.h"
#include "lib_system\timer_soft\timer_soft.h"
//#include "SSSMP3_Regs.h"
#include "conf\conf_sio.h"
#include "modules\power\power.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "MP3_API.H"
#include "mmi\shared\com_var.h"
#include "modules\player\player.h"


#pragma DATA = AUDIO_DATA
#pragma CODE = AUDIO_CODE

ImageDeclareType( DAC_COMMON_CODE );

//#define FIX_BUG_RL

//_____ M A C R O S ________________________________________________________

// Check Module Configuration
#ifndef AUDIO_RECORD
#  error  AUDIO_RECORD must be defined in conf_audio.h
#  define AUDIO_RECORD       DISABLE              // default to avoid errors
#endif

#ifndef AUDIO_OUT_TYPE
#  error  AUDIO_OUT_TYPE must be defined in conf_audio.h
#  define AUDIO_OUT_TYPE     INTERNAL_DAC         // default to avoid errors
#else
#  if (AUDIO_OUT_TYPE != INTERNAL_DAC)
#     if (AUDIO_OUT_TYPE != EXTERNAL_DAC)
#        error AUDIO_OUT_TYPE badly defined in conf_audio.h
#     else
#        include AUDIO_EXTERNAL_DAC_INCLUDE_NAME_FILE
#     endif
#  endif
#endif

#if ((AUDIO_OUT_TYPE == INTERNAL_DAC) && (!defined AUDIO_OUT_DRIVE))
#  error  AUDIO_OUT_DRIVE must be defined in conf_audio.h
#  define AUDIO_OUT_DRIVE    LINE_OUT_DRIVE       // default to avoid errors
#else
#  if ((AUDIO_OUT_DRIVE != LINE_OUT_DRIVE)          \
   &&  (AUDIO_OUT_DRIVE != INTERNAL_HEADSET_DRIVE)  \
   &&  (AUDIO_OUT_DRIVE != EXTERNAL_HEADSET_DRIVE))
#     error AUDIO_OUT_DRIVE badly defined in conf_audio.h
#  endif
#endif

#if ((AUDIO_OUT_TYPE == EXTERNAL_DAC) && (!defined EXT_DAC_OVERSAMP))
#  error  EXT_DAC_OVERSAMP must be defined in conf_audio.h
#  define EXT_DAC_OVERSAMP   OVERSAMP_128X        // default to avoid errors
#else
#endif

#if ((AUDIO_OUT_TYPE == EXTERNAL_DAC) && (!defined EXT_DAC_IF_TYPE))
#  error  EXT_DAC_IF_TYPE must be defined in conf_audio.h
#  define EXT_DAC_IF_TYPE  DAC_IF_I2S             // default to avoid errors
#else
#  if ((EXT_DAC_IF_TYPE != DAC_IF_I2S)  \
   &&  (EXT_DAC_IF_TYPE != DAC_IF_PCM))
#     error EXT_DAC_IF_TYPE badly defined in conf_audio.h
#  endif
#endif

#if ((AUDIO_OUT_TYPE == EXTERNAL_DAC) && (!defined EXT_DAC_NB_BITS))
#  error  EXT_DAC_NB_BITS must be defined in conf_audio.h
#  define EXT_DAC_NB_BITS  16                     // default to avoid errors
#else
#  if ((EXT_DAC_NB_BITS > 32))
#     error EXT_DAC_NB_BITS badly defined in conf_audio.h
#  endif
#endif


#if ((AUDIO_OUT_DRIVE == EXTERNAL_HEADSET_DRIVE) && (!defined AUDIO_EXT_HEAD_AMP_ON))
#  error  AUDIO_EXT_HEAD_AMP_ON must be defined in conf_audio.h
#  define AUDIO_EXT_HEAD_AMP_ON  P3_5             // default to avoid errors
#endif


//_____ D E F I N I T I O N ________________________________________________
extern Bool g_b_player_on;

//_____ D E C L A R A T I O N ______________________________________________

#if (AUDIO_OUT_TYPE == INTERNAL_DAC)
#  define Dac_power_on()        //dacint_power_on()
#  define Dac_power_off()       //dacint_power_off()
#  define Dac_sel_dac()         //dacint_select_input( TRUE  )
#  define Dac_sel_line_in()     //dacint_select_input( FALSE )
#  define Dac_desel_all()       //dacint_deselect_input()
#  define Dac_set_vol( R , L )  dacint_set_vol( R , L )
   void  dacint_power_on      ( void );
   void  dacint_power_off     ( void );
   void  dacint_select_input  ( Bool source );
   void  dacint_deselect_input( void );
   void  dacint_set_vol       ( U8 volume_right /*, U8 volume_left*/ );
#endif
  

//----------------------------------------------------------------------------
//---------------- routines to control volumes -------------------
//----------------------------------------------------------------

#pragma CODE = DAC_COMMON_CODE

//! Sets the channel volume output
//! If DAC is external, volume is controlled by external DAC.
//! If DAC is internal, volume is analog & digital controlled.
//!
//! @param: volume:  0 to VOLUME_LEVEL_NB_STEP
//!
void audio_set_vol( U8 volume_right /*, U8 volume_left*/ )
{
   // Check the range max of volumes
   /*if(volume_right > VOLUME_LEVEL_NB_STEP) {
      volume_right = VOLUME_LEVEL_NB_STEP;
   }
   if(volume_left > VOLUME_LEVEL_NB_STEP) {
      volume_left = VOLUME_LEVEL_NB_STEP;
   }*/
   //Dac_set_vol( volume_right , volume_left );
   dacint_set_vol( volume_right );
}


//------------------ Routines to control internal DAC ---------------
//-------------------------------------------------------------------

#if (AUDIO_OUT_TYPE == INTERNAL_DAC)

//const int VolTbl[ 11 ] = { 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31 };
const int VolTbl[ 8 ] = { 1, 7, 13, 16, 19, 25, 28, 31 };

//! Control volume of internal DAC (volume is analog & digital)
//!
//! @param: volume:  0 to VOLUME_LEVEL_NB_STEP
//!
void dacint_set_vol( U8 volume_right /*, U8 volume_left*/ )
{
   //extern Bool g_b_volume_on;
   // Convertion step
   //volume_right = ( 0x1C *(U16)(VOLUME_LEVEL_NB_STEP-volume_right))/VOLUME_LEVEL_NB_STEP;

   //if( !g_b_player_on )
   //   SDKLoadCode( DAC_COMMON_CODE );
   //DAC_FadeOut();
   // Set the volume right
#if 0
   if( volume_right == 0 )
   {
      /*if( audio_is_wav == 0 )
      {
         AudioMP3SetVol( 0 );
      }
      else*/
         DAC_HeadPhone_Volume( 0 );
#if( EARPHONE_DET == ENABLE )
      if( !g_b_earphone_present && g_b_volume_on )
         op_disable();
#endif   //#if( EARPHONE_DET == ENABLE )
   }
   else
#endif
   {
#if 0//( EARPHONE_DET == ENABLE )
      if( !g_b_earphone_present && g_b_volume_on )
      {
         op_enable();
         u32_op_delay_off_time = 0;
      }
#endif   //#if( EARPHONE_DET == ENABLE )
      /*if( audio_is_wav == 0 )
      {
         AudioMP3SetVol( VolTbl[ volume_right ]);
      }
      else*/
         DAC_HeadPhone_Volume( VolTbl[ volume_right ]);
      /*for( volume_left = 1; volume_left < VolTbl[ volume_right ]; volume_left++ )
      {
         U16 TempV;
         for( TempV = 0; TempV <= 10000; TempV++ )
            __asm( "nop" );
         DAC_HeadPhone_Volume( volume_left ); 
      }*/
   }

   // Convertion step
   //volume_left = ( 0x1C*(U16)(VOLUME_LEVEL_NB_STEP-volume_left))/VOLUME_LEVEL_NB_STEP;
//   volume_left = volume_right;

   // Set the volume left
   /*if( volume_left == 0x1C )
      CLASS_D_VOL_L_CTL = 0x3E;
   else
      CLASS_D_VOL_L_CTL = volume_left;*/
   //DAC_FadeIn();
}
#endif

#endif	// _audio_c_
