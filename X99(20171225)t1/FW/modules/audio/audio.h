//! @file audio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the audio controller driver definitions.
//!
//! @version 1.32 snd3-dvk-1_9_5 $Id: audio.h,v 1.32 2007/07/24 09:18:59 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _AUDIO_H_
#define _AUDIO_H_

//_____ I N C L U D E S ____________________________________________________

//#include "lib_mcu\aup\aup_drv.h"             // Audio processor definition


//_____ M A C R O S ________________________________________________________

//! @defgroup Audio stream control & status registers masks.
//! @{
#define MSK_SID               0x0F  // audio stream status 0
#define MSK_BGSR              0x80
#define MSK_SCHM              0x30  // audio stream status 1
#define MSK_SFS               0x0F
#define MSK_SBR               0x1F  // audio stream status 2
//! @}

//! @defgroup Audio Inputs.
//! @{
#define AUDIO_IN_MIC_SOURCE            ((bit)0)
#define AUDIO_IN_LINE_SOURCE           ((bit)1)
//! @}

//! @defgroup Audio Output Sources.
//! @{
#define AUDIO_OUT_AUP_SOURCE           ((bit)0)
#define AUDIO_OUT_LINE_SOURCE          ((bit)1)
//! @}

//! @defgroup AUDIO_OUT_TYPE configuration values.
//! @{
#define INTERNAL_DAC                   0
#define EXTERNAL_DAC                   1
//! @}

//! @defgroup AUDIO_OUT_DRIVE configuration values.
//! @{
#define LINE_OUT_DRIVE                 0
#define INTERNAL_HEADSET_DRIVE         1
#define EXTERNAL_HEADSET_DRIVE         2
//! @}

//! @defgroup EXT_DAC_IF_TYPE configuration values.
//! @{
#define DAC_IF_I2S                     0
#define DAC_IF_PCM                     1
//! @}

//! @defgroup VOLUME_VALUE configuration values.
//! @{
#define AUDIO_VOLUME_MAX               ((S8)6)     // +6 dB
#define AUDIO_VOLUME_MIN               ((S8)-54)   // -54 dB
#define AUDIO_VOLUME_MUTE              ((S8)-55)   // Dummy value belower than -54dB
//! @}


//! @defgroup MIC_BIAS_CTRL configuration values.
//! @{
#define NO_MIC_BIAS                    0
#define POWER_SELECTED_BIAS            1
#define V2_0_BIAS                      2
#define V1_5_BIAS                      3
//! @}


//! @defgroup audio_set_vol_equalizer() BAND values.
//! @{
#define BASS_BAND                      0
#define MEDIUM_BAND                    1
#define TREBLE_BAND                    2
//! @}


//! @defgroup Codec States
//! @{
typedef enum
{
  CODEC_STATE_POWER_OFF = 0
, CODEC_STATE_REC 
, CODEC_STATE_WAIT_SYNCHRO
, CODEC_STATE_SYNCHRO
, CODEC_STATE_WAIT_PLAY
, CODEC_STATE_WAIT_PLAY_SLOW
, CODEC_STATE_PLAY    
, CODEC_STATE_WAIT_STOP
, CODEC_STATE_STOP    
, CODEC_STATE_WAIT_JUMP
, CODEC_STATE_WAIT_PAUSE
, CODEC_STATE_PAUSE    
, CODEC_STATE_WAIT_FFW1
, CODEC_STATE_FFW1
, CODEC_STATE_WAIT_FFW2
, CODEC_STATE_FFW2
, CODEC_STATE_WAIT_FFW3
, CODEC_STATE_FFW3
, CODEC_STATE_WAIT_EOF
, CODEC_STATE_VIEWER_START
, CODEC_STATE_VIEWER_DECODE
, CODEC_STATE_ADC_RUN 
}Codec_state;
//! @}


//! @defgroup Audio Codec Commands
//! @{
#define CODEC_CMD_STOP                 0x00
#define CODEC_CMD_PLAY                 0x01
#define CODEC_CMD_PAUSE                0x02
#define CODEC_CMD_FAST_FWD             0x03
#define CODEC_CMD_FAST_FWD2            0x04
#define CODEC_CMD_FAST_FWD3            0x05
#define CODEC_CMD_JUMP                 0x06
#define CODEC_CMD_PLAY_SLOW            0x07
#define CODEC_CMD_ACK_IT               0x10
#define CODEC_CMD_GET_CODEC            0x13
#define CODEC_CMD_GET_VERSION          0x14
#define CODEC_CMD_GET_NEXT_DATA        0x15
#define CODEC_CMD_SIGNAL_EOF           0x1F
//! @}

//! @defgroup JPEG Codec Commands
//! @{
#define CODEC_CMD_GET_HEADER_INFO      0x01
//! @}


//! @defgroup Events (codec states or error) (return by Audio_get_event fonction)
//! @{
#define AUDIO_NO_EVENT                 0xFF
#define CODEC_EVT_SYNCHRO              0x00
#define CODEC_EVT_START                0x01
#define CODEC_EVT_STOP                 0x02
#define CODEC_EVT_PLAY                 0x03
#define CODEC_EVT_PAUSE                0x04
#define CODEC_EVT_FAST_FWD             0x05
#define CODEC_EVT_JUMPFINISH           0x06
//#define CODEC_EVT_RECORD               0x07
#define CODEC_EVT_UNSYNCHRONIZED       0x11
#define CODEC_EVT_UNKNOWN_STREAM       0x12
#define CODEC_EVT_BAD_CODEC            0x13
#define CODEC_EVT_UNDERRUN             0x14
#define CODEC_EVT_OVERRUN              0x15
#define CODEC_EVT_END_OF_STREAM        0x16
#define CODEC_EVT_VIEWER_HEADER_INFO   0x30
#define CODEC_EVT_VIEWER_BUFF_DATA_RDY 0x31
#define CODEC_EVT_VIEWER_END           0x32
#define CODEC_EVT_VIEWER_ERROR         0x33
#define CODEC_EVT_VIEWER_INIT_DONE     0x34
#define CODEC_VERSION                  0x40
//! @}


//! @defgroup Aup_get_stream_id() macro return value.
//! @{
#define STREAM_UNKNOWN                 0x00
#define STREAM_MP3                     0x01
#define STREAM_WMA                     0x02
#define STREAM_WAV                     0x03
#define STREAM_G726                    0x04
//! @}

//! @defgroup Aup_get_stream_bit_rate() macro return value.
//! @{
#define STREAM_BR_8_KBPS               0x00
#define STREAM_BR_11_KBPS              0x01
#define STREAM_BR_16_KBPS              0x02
#define STREAM_BR_22_KBPS              0x03
#define STREAM_BR_24_KBPS              0x04
#define STREAM_BR_32_KBPS              0x05
#define STREAM_BR_40_KBPS              0x06
#define STREAM_BR_44_KBPS              0x07
#define STREAM_BR_48_KBPS              0x08
#define STREAM_BR_56_KBPS              0x09
#define STREAM_BR_64_KBPS              0x0A
#define STREAM_BR_80_KBPS              0x0B
#define STREAM_BR_96_KBPS              0x0C
#define STREAM_BR_112_KBPS             0x0D
#define STREAM_BR_128_KBPS             0x0E
#define STREAM_BR_144_KBPS             0x0F
#define STREAM_BR_160_KBPS             0x10
#define STREAM_BR_192_KBPS             0x11
#define STREAM_BR_224_KBPS             0x12
#define STREAM_BR_256_KBPS             0x13
#define STREAM_BR_320_KBPS             0x14
//! @}

//! @defgroup Aup_get_stream_sampling_freq() macro return value.
//! @{
#define STREAM_FS_8_KHZ                0x00
#define STREAM_FS_11_KHZ               0x01
#define STREAM_FS_12_KHZ               0x02
#define STREAM_FS_16_KHZ               0x03
#define STREAM_FS_22_KHZ               0x04
#define STREAM_FS_24_KHZ               0x05
#define STREAM_FS_32_KHZ               0x06
#define STREAM_FS_44_KHZ               0x07
#define STREAM_FS_48_KHZ               0x08
//! @}

//! @defgroup Audio_get_channel_type() macro return value.
//! @{
#define STREAM_CH_STEREO               0x00
#define STREAM_CH_JOINT_STEREO         0x10
#define STREAM_CH_DUAL_MONO            0x20
#define STREAM_CH_SINGLE_MONO          0x30
//! @}


extern const int VolTbl[];

//_____ D E C L A R A T I O N S ____________________________________________
 
// routines audio I/O configuration
void  audio_dac_on            ( void );
void  audio_dac_off           ( void );
void  audio_linein_on         ( void );
void  audio_linein_off        ( void );
void  audio_record_on         ( Bool source );
void  audio_record_off        ( void );
Bool  audio_adc_on            ( void );
void  audio_adc_off           ( void );

// routines to enabled/disabled audio/picture codecs
Bool  audio_codec_load        ( U8 u8_id_codec );
U8    audio_codec_getid       ( void );
void  audio_codec_play_on     ( void );
void  audio_codec_record_on   ( void );
void  audio_codec_record_start( void );
void  audio_codec_off         ( void );
void  audio_codec_viewer_on   ( void );
void  audio_codec_viewer_off  ( void );

// routines to communicate with audio codecs
U8    audio_get_state         ( void );
U8    audio_get_event         ( void );
void  audio_clear_evt_it      ( void );
void  audio_cmd_stop          ( void );
void  audio_cmd_play          ( void );
void  audio_cmd_play_slow     ( void );
void  audio_cmd_jump          ( void );
void  audio_cmd_eof           ( void );
void  audio_cmd_pause         ( void );
void  audio_cmd_fast_fwd      ( void );
void  audio_cmd_fast_fwd2     ( void );
void  audio_cmd_fast_fwd3     ( void );
                                     
U8	   audio_get_codec         ( void );
U8    audio_get_stream_id     ( void );
U8    audio_get_bit_rate      ( void );
U8    audio_get_sampling_Freq ( void );
U8    audio_get_channel_type  ( void );
void  audio_set_bit_rate      ( U8 br );
void  audio_set_sampling_freq ( U8 fs );
U32   audio_get_play_time     ( void );
void  audio_set_play_time     ( U32 time );
//#define audio_get_adc_value() audio_get_play_time()


// routines to control volumes
void  audio_set_vol           ( U8 volume_right /*, U8 volume_left*/ );
void  audio_set_vol_equalizer ( U8 band, U8 volume );
void  audio_surround          ( Bool enabled );
void  audio_bassboost         ( Bool enabled );


#endif  // _AUDIO_H_
