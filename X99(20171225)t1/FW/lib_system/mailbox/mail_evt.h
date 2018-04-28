//!
//! @file mail_evt.h,v
//!
//! Copyright (c) 2006
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of all events that could be find
//! within the embedded SW.
//! Those events are the property of the system and are private.
//! Additional events can be added or deleted in the mmi/shared/com_evt.h file
//! 
//! If an event needs to be removed, the user needs to take care that 
//! the event is no more used in the embedded SW
//!
//! @version 2.7 snd3-dvk-1_9_5 $Id: mail_evt.h,v 2.7 2007/07/24 15:28:52 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MAIL_EVT_H_
#define _MAIL_EVT_H_

//  Definition of Systems Events - DO NOT REMOVE - NEVER
//! @defgroup Systems events 
//! @{
#define EVT_MMGR_KRN                  0x00
#define EVT_KEY_PRESSED               (EVT_MMGR_KRN+1)
#define EVT_SWITCH                    (EVT_MMGR_KRN+2)
#define EVT_START_APPLI               (EVT_MMGR_KRN+3)
#define EVT_BACK_TO_TOP               (EVT_MMGR_KRN+4)
#define EVT_APPLI_KILLED              (EVT_MMGR_KRN+5)
#define EVT_TIMER                     (EVT_MMGR_KRN+6)
#define EVT_MAILBOX_CMD_FULL          (EVT_MMGR_KRN+7)  // The Command mailbox is full. Commands are lost
#define EVT_MAILBOX_EVT_FULL          (EVT_MMGR_KRN+8)  // The Event mailbox is full. Events are lost
#define EVT_ANIMATION                 (EVT_MMGR_KRN+9)
#define EVT_TIMER_ALLOC_FAILURE       (EVT_MMGR_KRN+10)
#define EVT_DATE					  (EVT_MMGR_KRN+11)

#define EVT_CHANGE_OID_TYPE			  (EVT_MMGR_KRN+12)
#define EVT_CHANGE_AP4_VOL			  (EVT_MMGR_KRN+13)

//! @}


//! @defgroup Power events.
//! @{
#define EVT_POWER                     0x10              // Power event
#define EVT_POWER_ON                  (EVT_POWER+1)
#define EVT_BATTERY_CHARGING          (EVT_POWER+2)
#define EVT_BATTERY_LEVEL             (EVT_POWER+3)
#define EVT_BATTERY_LEVEL_ALERT       (EVT_POWER+4)
#define EVT_POWER_VBUS_STATE          (EVT_POWER+5)     // "0": no power from USB; "1": power from USB
#define EVT_BACKLIGHT_OFF             (EVT_POWER+6)
#define EVT_POWER_ENTER_IN_POWEROFF   (EVT_POWER+7)
//! @}


//! @defgroup USB events.
//! @{
#define  EVT_USB                       0x20              // USB Event
// MAIN
#define  EVT_USB_CTRL_POWERED          (EVT_USB+1)       // USB plugged
#define  EVT_USB_CTRL_UNPOWERED        (EVT_USB+2)       // USB un-plugged
// USB DEVICE
#define  EVT_USB_DEVICE_START          (EVT_USB+3)      // USB enter in device mode
#define  EVT_USB_DEVICE_STOP           (EVT_USB+4)      // USB exit of device mode
#define  EVT_USB_DEVICE_ENUM_HIGH      (EVT_USB+5)      // USB enumerated in hight speed mode
#define  EVT_USB_DEVICE_ENUM_FULL      (EVT_USB+6)      // USB enumerated in full speed mode
#define  EVT_USB_DEVICE_SUSPEND        (EVT_USB+7)      // USB suspend
#define  EVT_USB_DEVICE_WAKE_UP        (EVT_USB+8)      // USB wake up
#define  EVT_USB_DEVICE_RESUME         (EVT_USB+9)      // USB resume
#define  EVT_USB_DEVICE_RESET          (EVT_USB+10)      // USB reset
#define  EVT_USB_DEVICE_SEND           (EVT_USB+11)      // USB command send
#define  EVT_USB_DEVICE_RECEIV         (EVT_USB+12)      // USB command receiv
// USB DEVICE mass storage
#define  EVT_USB_DEVICE_MS_STARTED     (EVT_USB+13)      // USB mass storage is started
// USB DEVICE cdc
#define  EVT_USB_DEVICE_CDC_RECEIV     (EVT_USB+14)      // USB CDC has receiv a datas
// USB HOST
#define  EVT_USB_HOST_START            (EVT_USB+15)      // USB enter host mode
#define  EVT_USB_HOST_EXIT             (EVT_USB+16)      // USB exit host mode
#define  EVT_USB_HOST_LIMITATION_HW    (EVT_USB+17)      // USB HW limitation for new device
#define  EVT_USB_HOST_LIMITATION_SW    (EVT_USB+18)      // USB SW limitation for new device
// USB HOST mass storage
#define  EVT_USB_HOST_MS_CHANGE        (EVT_USB+19)      // USB mass storage memory change
// USB HOST hub
#define  EVT_USB_HOST_HUB_CHANGE       (EVT_USB+20)      // USB HUB change
// USB HOST hid
#define  EVT_USB_HOST_HID_CHANGE       (EVT_USB+21)      // USB HID change
#define  EVT_USB_HOST_HID_MOUSE_BTN1   (EVT_USB+22)      // USB HID mouse bouton 1 clic
#define  EVT_USB_HOST_HID_MOUSE_BTN2   (EVT_USB+23)      // USB HID mouse bouton 2 clic
#define  EVT_USB_HOST_HID_MOUSE_BTN3   (EVT_USB+24)      // USB HID mouse bouton 3 clic
#define  EVT_USB_HOST_HID_MOUSE_MOVE   (EVT_USB+25)      // USB HID mouse move (param X Y move)
#define  EVT_USB_HOST_HID_KB_KEY       (EVT_USB+26)      // USB HID keyboard key press
// USB HOST cdc
#define  EVT_USB_HOST_CDC_CHANGE       (EVT_USB+27)      // USB CDC change
#define  EVT_USB_DEVICE_CHARGE         (EVT_USB+30)
//! @}




//! @defgroup Explorer events and parameters.
//! @{
#define EVT_EXP_PASTE_FINISH           0x40
#define EVT_EXP_PASTE_FAIL             0x41
#define EVT_EXP_MEMORY_MMCSD_CHANGE    0x42
//! @}


//! @defgroup player events and parameters
//! @{
#define EVT_PLAYER                     (0x50)            //!< In the argument the specific event PLAYER
#define EVT_PLAYER_LYRIC_CHANGE        (0x51)            //!< New lyric string is available

#define EVT_START_PLAYER_APPLI         (0x52)
#define EVT_PLAYER_NO_FILE             (0x53)

#define EVT_INT_BROWSER_PLAYER_APPLI   (0x54)
#define EVT_BROWSER_PLAYER_APPLI       (0x55)
#define EVT_ENTER_PLAYER_APPLI         (0x56)

#define EVT_LIST_PLAYER_FILES          (0x57)
#define EVT_NO_LIST_PLAYER_FILES       (0x58)
//! @}


//! @defgroup update events and parameters
//! @{
#define EVT_UPDATE_OK                  (0x60)            //!< Update was finished successfully
#define EVT_UPDATE_START               (EVT_UPDATE_OK+1) //!< Update start
#define EVT_UPDATE_PROGRESSING         (EVT_UPDATE_OK+2) //!< Update is progressing. Information about the update is in parameters
#define EVT_UPDATE_FINISHED_FAIL       (EVT_UPDATE_OK+3) //!< Update is finished with problem (see parameter)
#define EVT_UPDATE_ERR_COMMAND         (EVT_UPDATE_OK+4) //!< Bad command
#define EVT_UPDATE_END_OF_EVT          (EVT_UPDATE_OK+5) //!< Flag to signal the last+1 event
//! @}


//! @defgroup recorder events and parameters
//! @{
#define EVT_RECORDER                   (0x70)            //!< In the argument the specific event RECORDER
#define EVT_START_RECORDER_APPLI       ( EVT_RECORDER + 1 )
#define EVT_STOP_RECORDER_APPLI        ( EVT_RECORDER + 2 )
#define EVT_PLAY_RECORDER_APPLI        ( EVT_RECORDER + 3 )
#define EVT_READY_RECORDER_APPLI       ( EVT_RECORDER + 4 )


//! @}


//! @defgroup viewer events and parameters
//! @{
#define EVT_VIEWER_EOF                 (0x80)            //!< End of picture decoding
#define EVT_VIEWER_ERR_ACCESS_FILE     (0x81)            //!< Error access file, the viewer is stopped
#define EVT_VIEWER_FAIL                (0x82)            //!< error during decoding
//! @}


//! @defgroup Customer events and parameters.
//! @{
#define EVT_START_EVENT_LIST_CUSTOMER  0xA0

#if 1
#define  EVT_DIC                       ( 0xE0 )
#define  EVT_START_DIC_APPLI           ( EVT_DIC + 1 )
#define  EVT_DIC_NO_FILE               ( EVT_DIC + 2 )
#define  EVT_DIC_VOL_PLAY              ( EVT_DIC + 3 )
#define  EVT_DIC_FLUSH_TEXT            ( EVT_DIC + 4 )
#define  EVT_DIC_VOL_CHANGE            ( EVT_DIC + 5 )

#else
#define  EVT_DIC                       ( EVT_START_EVENT_LIST_CUSTOMER )
#define  EVT_START_DIC_APPLI           ( EVT_DIC + 1 )
#define  EVT_DIC_NO_FILE               ( EVT_DIC + 2 )
#define  EVT_DIC_VOL_PLAY              ( EVT_DIC + 3 )
#define  EVT_DIC_FLUSH_TEXT            ( EVT_DIC +  )
#endif

//! @}


//! @defgroup idle events and parameters.
//! @{
#define EVT_IDLE_DISPLAY_OID           			0x90
#define EVT_IDLE_DOWITH_OID           			(EVT_IDLE_DISPLAY_OID + 1)
#define EVT_GOTO_OID_MODE             			(EVT_IDLE_DISPLAY_OID + 2)

#define EVT_IDLE_INIT_MODE             			(EVT_IDLE_DISPLAY_OID + 3)
#define EVT_IDLE_INIT_OVER_MODE             	(EVT_IDLE_DISPLAY_OID + 4)
#define EVT_IDLE_CHOICE_BOOK_MODE             	(EVT_IDLE_DISPLAY_OID + 5)

//! @}


//! @defgroup paster events and parameters
//! @{
#define  EVT_PASTER                    ( 0xB0 )
#define  EVT_PASTER_STATE_CHANGE       ( EVT_PASTER )
#define  EVT_PASTER_ERROR              ( EVT_PASTER + 1 )
#define  EVT_PASTER_COPY_PROC          ( EVT_PASTER + 2 )
#define  EVT_START_PASTER_APPLI        ( EVT_PASTER + 3 )
#define  EVT_PASTER_NO_APF_FILE        ( EVT_PASTER + 4 )
//! @}

//! @defgroug search events and parameters.
//! @{
#define  EVT_SEARCH_OK                 ( 0xC0 )
#define  EVT_SEARCH_AP3                ( EVT_SEARCH_OK + 1 )
#define  EVT_SEARCH_BOOK               ( EVT_SEARCH_OK + 2 )
#define  EVT_SEARCH_PAGE               ( EVT_SEARCH_OK + 3 )
#define  EVT_SEARCH_CONTROL            ( EVT_SEARCH_OK + 4 )
#define  EVT_SEARCH_END                ( EVT_SEARCH_OK + 5 )
#define  EVT_SEARCH_AP3RECORD          ( EVT_SEARCH_OK + 6 )
#define  EVT_SEARCH_AP3POWEROFF        ( EVT_SEARCH_OK + 7 )
#define  EVT_SEARCH_AP3EXIT            ( EVT_SEARCH_OK + 8 )
#define  EVT_SEARCH_CHANGE_FILE        ( EVT_SEARCH_OK + 9 )

#define  EVT_REMOTE                    ( EVT_SEARCH_OK + 10 )
#define  EVT_START_AP3_APPLI           ( EVT_SEARCH_OK + 11 )
#define  EVT_SEARCH_REMOTE_CHANGE_AP3  ( EVT_SEARCH_OK + 12 )
#define  EVT_SEARCH_SEQUENCE           ( EVT_SEARCH_OK + 13 )
#define  EVT_SEARCH_AP3RECORD_PLAY     ( EVT_SEARCH_OK + 14 )
#define  EVT_SEARCH_NO_FIND_FILE       ( EVT_SEARCH_OK + 15 )
#define  EVT_SEARCH_RP_RECORD          ( EVT_SEARCH_OK + 16 )
#define  EVT_SEARCH_RP_RECORD_START    ( EVT_SEARCH_OK + 17 )
#define  EVT_SEARCH_RP_PLAY            ( EVT_SEARCH_OK + 18 )
#define  EVT_SEARCH_RP_PLAY_START      ( EVT_SEARCH_OK + 19 )
#define  EVT_SEARCH_RP_STOP            ( EVT_SEARCH_OK + 20 )

#define  EVT_SEARCH_REREADMODE         ( EVT_SEARCH_OK + 21 )
#define  EVT_SEARCH_PLAYREREAD         ( EVT_SEARCH_OK + 22 )

#define  EVT_SEARCH_TO_BROWSER         ( EVT_SEARCH_OK + 23 )
#define  EVT_SEARCH_FLUSH_ALLTIME      ( EVT_SEARCH_OK + 24 )
#define  EVT_SEARCH_FLUSH_TEXT         ( EVT_SEARCH_OK + 25 )

#define  EVT_SEARCH_FLUSH_CONTROL      ( EVT_SEARCH_OK + 26 )

#define  EVT_SEARCH_HINT_PLAY          ( EVT_SEARCH_OK + 27 )
//! @}


#endif  //! _MAIL_EVT_H_ 
