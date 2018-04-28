//! @file conf_audio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the keyboard
//! This file will be given to any external customer
//!
//! @version 2.3 snd3-dvk-1_9_5 $Id: conf_audio.h,v 2.3 2007/07/24 11:31:29 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_AUDIO_H_
#define _CONF_AUDIO_H_

//#include "conf\conf_kbd.h"
#include "modules\audio\audio.h"

// List of supported external DAC.
// Please add your DAC definition 
#define AUDIO_EXTERNAL_DAC_ATMEL_AT73C213   (0)  // Do not change this value
#define AUDIO_EXTERNAL_DAC_TI_PCM1770       (1)  // Do not change this value
//#define AUDIO_EXTERNAL_DAC_               (2)  // Put here new DAC


//! --- Enable the record mode ---
//! Values: - ENABLE
//!         - DISABLE
#define AUDIO_RECORD             ENABLE


//! --- Fast forward speed configuration ---
//! Values: - AUDIO_FF_SPEED_1  is used for the 4 first seconds of fast forward ( [0; 4] sec)
//!         - AUDIO_FF_SPEED_2  is used for the 4 following seconds             ( [4; 8] sec)
//!         - AUDIO_FF_SPEED_3  is used for the 4 following seconds             ( [8; 12] sec)
//!
//! Use: - CODEC_CMD_FAST_FWD   is the first speed of fast forward
//!      - CODEC_CMD_FAST_FWD2  is the second speed of fast forward
//!      - CODEC_CMD_FAST_FWD3  is the third speed of fast forward
#define AUDIO_FF_SPEED_1            CODEC_CMD_FAST_FWD3
#define AUDIO_FF_SPEED_2            CODEC_CMD_FAST_FWD3
#define AUDIO_FF_SPEED_3            CODEC_CMD_FAST_FWD3

//! --- Configure the time from which song replay is done with a keypressed previous ---
//! Values: in seconds (If you exceed 59 seconds, then feature disable)
#define AUDIO_TIME_FOR_SONG_REPLAY   4


//! --- Audio Output Type Configuration ---
//! Values: - EXTERNAL_DAC       use external audio DAC
//!         - INTERNAL_DAC       use internal on-chip audio DAC
//# define AUDIO_OUT_TYPE          EXTERNAL_DAC
# define AUDIO_OUT_TYPE          INTERNAL_DAC

//! --- Name of file included in case you use external DAC ---
//! Values: AUDIO_EXTERNAL_DAC_TI_PCM1770        use external DAC Texas Instrument PCM1770
//!         AUDIO_EXTERNAL_DAC_ATMEL_AT73C213    use external DAC Atmel AT73C213
#if (AUDIO_OUT_TYPE==EXTERNAL_DAC)
#  if (defined BOARD_REFD1)
#     define AUDIO_EXTERNAL_DAC_PART_NUMBER           AUDIO_EXTERNAL_DAC_ATMEL_AT73C213
#     define AUDIO_EXTERNAL_DAC_INCLUDE_NAME_FILE     "drivers\audio\dacc213.h"

#  elif (defined BOARD_USER)
#     define AUDIO_EXTERNAL_DAC_PART_NUMBER           AUDIO_EXTERNAL_DAC_ATMEL_AT73C213
#     define AUDIO_EXTERNAL_DAC_INCLUDE_NAME_FILE     "drivers\audio\dacc213.h"
 
#  else
#     error   Please specify AUDIO_EXTERNAL_DAC_PART_NUMBER
#  endif
#endif


//! --- Number of step to control all volumes level (must be a U8)
#define VOLUME_LEVEL_NB_STEP  50

//! --- Audio Output Drive Configuration ---
//! Useless if the audio out is not the internal DAC
//! Values: - LINE_OUT_DRIVE           high impedance drive
//!         - INTERNAL_HEADSET_DRIVE   low impedance drive (high voltage only)
//!         - EXTERNAL_HEADSET_DRIVE   high impedance drive
#define AUDIO_OUT_DRIVE          INTERNAL_HEADSET_DRIVE

//! --- External Headphone Amplifier ON I/O signal ---
//! Values: - I/O pin asserted low
#define AUDIO_EXT_HEAD_AMP_ON    P1_6           // AT85DVK-07 board

//! --- External Audio DAC Oversamplig Ratio Configuration ---
//! Useless if the audio out is not the external DAC
//! Values: - OVERSAMP_128X
//!         - OVERSAMP_256X
//!         - OVERSAMP_384X
#define EXT_DAC_OVERSAMP         OVERSAMP_256X

//! --- External Audio DAC Interface Type Configuration ---
//! Useless if the audio out is not the external DAC
//! Values: - DAC_IF_I2S
//!         - DAC_IF_PCM
#define EXT_DAC_IF_TYPE          DAC_IF_I2S

//! --- External Audio DAC Bit Number Configuration ---
//! Useless if the audio out is not the external DAC
//! Values: - from 16 bits up to 32 bits
#define EXT_DAC_NB_BITS          16


//! --- Microphone Bias Output Configuration ---
//! Values: - NO_MIC_BIAS        microphone bias output not used
//!         - POWER_SELECTED_BIAS power related: very low voltage  => 1.5V bias
//!                                              low voltage       => 2.0V bias
//!         - V2_0_BIAS          force 2.0V bias
//!         - V1_5_BIAS          force 1.5V bias
#define MIC_BIAS_CTRL            POWER_SELECTED_BIAS

//! --- Microphone Input Amplifier Gain Configuration ---
//! This parameter is used when MIC input is selected
//! Values: - AUP_INPUT_GAIN_0DB
//!         - AUP_INPUT_GAIN_6DB
//!         - AUP_INPUT_GAIN_12DB
//!         - AUP_INPUT_GAIN_18DB
//!         - AUP_INPUT_GAIN_24DB
#define AUDIO_IN_MIC_GAIN              AUP_INPUT_GAIN_24DB

//! --- Microphone Input Digital Amplifier Gain Configuration ---
//! This parameter is used when MIC input is selected
//! by audio_select_in_source() function
//! Values: - AUP_INPUT_GAIN_DIG_1, _2, _4, _8, _12, _16, _20, _24
#define AUDIO_IN_MIC_DIGITAL_GAIN      AUP_INPUT_GAIN_DIG_2


//! --- Line Input Amplifier Gain Configuration ---
//! This parameter is used when LINE input is selected
//! by audio_select_in_source() function
//! Values: - AUP_INPUT_GAIN_0DB
//!         - AUP_INPUT_GAIN_6DB
//!         - AUP_INPUT_GAIN_12DB
//!         - AUP_INPUT_GAIN_18DB
//!         - AUP_INPUT_GAIN_24DB
#define AUDIO_IN_LINE_GAIN             AUP_INPUT_GAIN_0DB

//! --- Line Input Digital Amplifier Gain Configuration ---
//! This parameter is used when LINE input is selected
//! by audio_select_in_source() function
//! Values: - AUP_INPUT_GAIN_DIG_1, _2, _4, _8, _12, _16, _20, _24
#define AUDIO_IN_LINE_DIGITAL_GAIN     AUP_INPUT_GAIN_DIG_1


//! --- Line In Source Preamplifier Gain Configuration ---
//! This parameter is used when LINE input is selected as output source
//! Values: - AUP_LIN_BACK_GAIN_6DB
//!         - AUP_LIN_BACK_GAIN_12DB
#define AUDIO_LINE_BACK_GAIN     AUP_LINE_BACK_GAIN_6DB


#endif  // _CONF_AUDIO_H_



