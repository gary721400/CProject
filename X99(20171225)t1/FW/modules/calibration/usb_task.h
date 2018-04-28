//!
//! @file usb_task.h,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file manages the USB task either device/host or both.
//!
//! @version 2.0 snd3-refd1-1_9_5 $Id: usb_task.h,v 2.0 2007/05/25 14:18:57 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _USB_TASK_H_
#define _USB_TASK_H_

//_____ D E C L A R A T I O N S ____________________________________________
/*#define _JM_PHY      1
#define MP3_JMPHY    1
#define SDIO_ENABLE  0
#define TX_DBG       0
*/

/**
 * @brief This function initializes the USB proces.
 *
 *  This function enables the USB controller and init the USB interrupts.
 *  The aim is to allow the USB connection detection in order to send
 *  the appropriate USB event to the operating mode manager.
 *  Depending on the mode supported (HOST/DEVICE/DUAL_ROLE) the function
 *  calls the corespong usb mode initialization function
 *
 *  @param none
 *
 *  @return none
 */
void usb_task_init     (void);

/**
 * @brief This function initializes the USB proces.
 *
 *  Depending on the mode supported (HOST/DEVICE/DUAL_ROLE) the function
 *  calls the coresponding usb mode initialization function
 *
 *  @param none
 *
 *  @return none
 */
void usb_task_stop     (void);

/**
 *  @brief Entry point of the USB mamnagement
 *
 *  Depending on the mode supported (HOST/DEVICE/DUAL_ROLE) the function
 *  calls the corespong usb management function
 *
 *  @param none
 *
 *  @return none
*/
void  usb_task                      ( void);


/**
 *  @brief Start the device mode (in case of USB_DEVICE_AUTO_START == DISABLED)
 *
 *  @param none
 *
 *  @return none
*/
void  usb_task_start_device_mode    ( void );


/**
 *  @brief force stop the device mode
 *
 *  @param none
 *
 *  @return none
*/
void  usb_task_stop_device_mode     ( void );

//! Returns the state of the device mode
//!
//! @return TRUE if we are in the device mode
//!         FALSE if we are not in the device mode
//!
Bool  Is_usb_device_started         ( void );

#endif // _USB_TASK_H_

