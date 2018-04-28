//!
//! @file com_appli.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of all the MMI application that could
//! be find within the embedded SW.
//! This file can be easily updated by anybody.
//!
//! If an MMI application needs to be removed, the user needs to take care that
//! the application is no more used in the embedded SW
//!
//! @version 2.1 snd3-dvk-1_9_5 $Id: com_appli.h,v 2.1 2007/06/25 11:06:51 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _COM_APPLI_H_
#define _COM_APPLI_H_


//_____ D E C L A R A T I O N S ____________________________________________
//!
//!  Put the prototype of the mmi applications here to make it possible the compilation
//!
void    status_mmi_appli      (U8, U16);
void    idle_mmi_appli        (U8, U16);
void    player_mmi_appli      (U8, U16);
void    radio_mmi_appli	      (U8, U16);
void    storage_mmi_appli     (U8, U16);
void    volume_mmi_appli      (U8, U16); 
void    dlgbox_mmi_appli      (U8, U16);
void    anim_mmi_appli        (U8, U16);
void    equalizer_mmi_appli   (U8, U16);
void    explorer_mmi_appli    (U8, U16);
void    info_mmi_appli        (U8, U16);
void    setting_mmi_appli     (U8, U16);
void    recorder_mmi_appli    (U8, U16);
void    viewer_mmi_appli      (U8, U16);
void    snake_mmi_appli       (U8, U16);
void    lyrics_mmi_appli      (U8, U16);
void    backlight_mmi_appli   (U8, U16);
void    dummy_mmi_appli       (U8, U16);
void    update_mmi_appli      (U8, U16);
void    ebook_mmi_appli       (U8, U16);
void    startup_mmi_appli     (U8, U16);
void    cdc_mmi_appli         (U8, U16);

void    paster_mmi_appli      (U8, U16);
void    ap3_mmi_appli         (U8, U16);
void    poweroff_mmi_appli    (U8, U16);
void    dic_mmi_appli         (U8, U16);
void    wireless_mmi_appli    (U8, U16);
//_____ D E F I N I T I O N S ______________________________________________
//!
//!  Define the ID label of each mmi applications here in order to launch them further
//!
// Value '0' is reserved for the definition of MMI_APPLI_STATUS in "mmgr_kernel.h"
#define MMI_APPLI_IDLE              1
#define MMI_APPLI_MSTORAGE          2
#define MMI_APPLI_PLAYER            3
#define MMI_APPLI_EXPLORER          4
#define MMI_APPLI_VOLUME            5
#define MMI_APPLI_SETTING           6
#define MMI_APPLI_RADIO             7
#define MMI_APPLI_INFO              8
#define MMI_APPLI_EQUALIZER         9
#define MMI_APPLI_DUMMY             12
#define MMI_APPLI_RECORDER          13
#define MMI_APPLI_VIEWER            14
#define MMI_APPLI_GAME              15
#define MMI_APPLI_LYRICS            16
#define MMI_APPLI_EBOOK             17
#define MMI_APPLI_BACKLIGHT         18
#define MMI_APPLI_UPDATE            19
#define MMI_APPLI_STARTUP           20
#define MMI_APPLI_CDC               21

#define MMI_APPLI_PASTER            24
#define MMI_APPLI_AP3               25
#define MMI_APPLI_POWEROFF          26
#define MMI_APPLI_DIC               27
#define MMI_APPLI_WIRELESS			28
#define MMI_APPLI_REREAD			29
#define MMI_APPLI_STUDY				30


#define USB_STORAGE_CODE_FLAG       (( U16 )0x0001 )
#define MMI_STARTUP_CODE_FLAG       (( U16 )0x0002 )
#define MMI_IDLE_CODE_FLAG          (( U16 )0x0004 )
#define MMI_PLAYER_CODE_FLAG        (( U16 )0x0008 )

//_____ D E C L A R A T I O N S ____________________________________________

void call_mmi_appli(U8 idx, U8 evt_id, U16 evt_param);

void swap_code( U16 code_flag );

#endif  //! _COM_APPLI_H_
