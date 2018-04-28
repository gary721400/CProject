//! @file conf_sio.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the SIO interface
//! All the default configuration for the SIO controller will be put
//! here.
//! This file will be given to any external customer
//!
//! @version 1.28 snd3b-dvk-1_0_33 $Id: conf_sio.h,v 1.28 2006/12/08 16:21:57 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _CONF_SIO_H_
#define _CONF_SIO_H_


//_____ G E N E R I C   C O N F I G U R A T I O N __________________________

//! --- earphone detcetion ---
//! Values: - ENABLED  :enable earphone detcetion
//!         - DISABLED : disable earphone detcetion
#define EARPHONE_DET                ( ENABLE )
//#define	EARPHONE_DET               ( DISABLE )


//
//#define A10F_NOPASTER_NOLCD         (ENABLE)
//#define A10F_NOPASTER_NOLCD         ( DISABLE )

//! --- serial communication debug ---
//! Values: - ENABLED  :enable sio debug
//!         - DISABLED : disable sio debug
//#define ENABLE_DEBUG_SIO            ( ENABLE )
#define ENABLE_DEBUG_SIO            ( DISABLE )


//! --- picture menu ---
//! Values: - ENABLED  :enable picture menu
//!         - DISABLED : disable picture menu
#define MAIN_MENU_PICTURE           ( DISABLED )
//#define MAIN_MENU_PICTURE           ( ENABLED )

//! --- support language numbers ---
#define NB_LANGUAGE               4

//! --- play list ---
//! Values: - ENABLED  :enable play list
//!         - DISABLED : disable play list
#define PLAY_LIST                   ( DISABLE )
//#define PLAY_LIST                   ( ENABLE )

//! --- paster ---
//! Values: - ENABLED  :enable paster
//!         - DISABLED : disable paster
//#define PASTER                      ( DISABLE )
#define PASTER                      ( ENABLE )

//! --- paster mode ---
//! Values: - ENABLED  :enable paster mode
//!         - DISABLED : disable paster mode
//#define PASTER_MODE                 ( DISABLE )
#define PASTER_MODE                 ( ENABLE )

//! --- ap3 ---
//! Values: - ENABLED  :enable ap3
//!         - DISABLED : disable ap3
//#define AP3                         ( DISABLE )
#define AP3                         ( ENABLE )

//! --- dictionary ---
//! Values: - ENABLED  :enable dictionary
//!         - DISABLED : disable dictionary
//#define DIC                         ( DISABLE )
#define DIC                         ( ENABLE )

#define TEXT_VIEWER                 ( DISABLED )

//! --- remote mp3 ---
//! Values: - ENABLED  :enable remote mp3
//!         - DISABLED : disable remote mp3
//#define REMOTE_MP3                  ( DISABLE )
#define REMOTE_MP3                  ( ENABLE )

//! --- remote dictionary ---
//! Values: - ENABLED  :enable remote dictionary
//!         - DISABLED : disable remote dictionary
//#define REMOTE_DIC                  ( DISABLE )
#define REMOTE_DIC                  ( ENABLE )

//! --- remote paster ---
//! Values: - ENABLED  :enable remote paster
//!         - DISABLED : disable remote paster
//#define REMOTE_PASTER               ( DISABLE )
#define REMOTE_PASTER               ( ENABLE )

//! --- remote recorder ---
//! Values: - ENABLED  :enable remote recorder
//!         - DISABLED : disable remote recorder
//#define REMOTE_REC                  ( DISABLE )
#define REMOTE_REC                  ( ENABLE )

//! --- mode cue ---
//! Values: - ENABLED  :enable mode cue
//!         - DISABLED : disable mode cue
#define MODE_CUE                    ( DISABLE )
//#define MODE_CUE                    ( ENABLE )

//! --- ebook ---
//! Values: - ENABLED  :enable ebook
//!         - DISABLED : disable ebook
#define EBOOK                       ( DISABLE )
//#define EBOOK                       ( ENABLE )

//! --- beijing renai ---
//! Values: - ENABLED  :enable beijing renai
//!         - DISABLED : disable beijing renai
#define BEIJING_RENAI               ( DISABLE )
//#define BEIJING_RENAI               ( ENABLE )

//不能同时使能或禁止aigo_pen和afaya_pen
//! --- afaya aigo pen ---
//! Values: - ENABLED  :enable aigo pen
//!         - DISABLED : disable aigo pen
#define aigo_pen                    ( DISABLE )
//#define aigo_pen                    ( ENABLE )
//! --- afaya aigo pen ---
//! Values: - ENABLED  :enable afaya pen
//!         - DISABLED : disable afaya pen
//#define afaya_pen                   ( DISABLE )
#define afaya_pen                   ( ENABLE )

//! --- select language menu ---
//! Values: - ENABLED  :enable select language menu
//!         - DISABLED : disable select language menu
//#define SELECT_LANGUAGE             ( DISABLE )
#define SELECT_LANGUAGE             ( ENABLE )

//! --- display language ---
#define ENGLISH                     0
#define Simplified                  2
#define Traditional                 3

#if( SELECT_LANGUAGE == DISABLE )
#define LANGUAGE                    ENGLISH
//#define LANGUAGE                    Simplified
//#define LANGUAGE                    Traditional

#define LANGUAGE_DEFAULT            LANGUAGE
#endif   //#if( SELECT_LANGUAGE == DISABLE )

#if( SELECT_LANGUAGE == ENABLE )
#define LANGUAGE_DEFAULT            ENGLISH
//#define LANGUAGE_DEFAULT            Simplified
//#define LANGUAGE_DEFAULT            Traditional
#endif   //#if( SELECT_LANGUAGE == ENABLE )

#define PLAY_TIME_LONG              ( ENABLE )
//#define PLAY_TIME_LONG              ( DISABLE )

#endif  //! _CONF_SIO_H_

