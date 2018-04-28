/*! \mainpage USB Mass Storage API

 \section intro Introduction
 USB Mass Storage for KBMPT03\n
 -       version 1.0\n\n
  \section us_sect Using Library
    User need to prepare extra memory for library usage.\n
    If user want USB to show 2 device, user may declare as:\n\n
    -  _STORAGE usb_stor[2];
*/
#ifndef __USB_STOR_API_H__
#define __USB_STOR_API_H__

//#include "usb_def.h"
//#include "usb_stor_def.h"
void* nearcpy(__X void* d, __X void* s, unsigned int l);
// USB Storage Structure, include function pointer and some variables
typedef struct __STORAGE {
	int         (*stor_status)(void);
	unsigned long    (*stor_size)(void) ;
    int         (*stor_read)(unsigned long sector, __X void *buf);	// Storage Read
    int         (*stor_write)(unsigned long sector, __X void *buf);	// Storage Write
    int         (*stor_check_wp)(void);					        // Check Write Protect
    int         (*stor_flush)(void);							// Flush Cache of Write
    unsigned int     device_type;
    char        *device_vendor;
    char        *device_identify;
    char        *device_revision;
} _STORAGE;

extern __X _STORAGE usb_stor[];

int  usb_empty_func2(void);
/** \example usb_main.c
 * This is an example of how to use the Test class.
 * More details about this example.
 */

//! \defgroup APIGroup API Functions

//! @{	\ingroup APIGroup
/*!	USB debounce delay value\n
*/
void USBSetDebounceCount(unsigned long value);
/*!	USB detect attach or not function\n
 @return
    1, attached\n
    0, not\n
*/
int USBAttachCheck(void);
/*!
*/
void USBStoragePoweroff(void);
/*!	USB detect attach or not function\n
 @return
    1, not attach\n
    0, attached\n
*/
int USBRemoveCheck(void);
/*!	USB un-initialize function, close usb interrupt\n
*/
void USBStorageUninit(void);
/*!	USB initialize function, need be called before any function\n
 @param  maxlun             --> max device will be seen in USB storage.
 @param  intrrupt_priority  --> USB interrupt priority, 7 is highest.
*/
void USBStorageInit( unsigned int maxlun, int intrrupt_priority );
//! @}
//! @{	\ingroup APIGroup
/*!	Setup the device on USB storage\n
 @param  lun            --> which device order.
 @param  stor_read      --> read function pointer.
 @param  stor_write     --> write function pointer.
 @param  stor_check_wp  --> check write-propection function pointer.
 @param  stor_flush     --> flush device statue function pointer.
 @param  stor_size_sectors  --> device size in sectors.
 @param  device_type    --> device type, see \ref dev_type; e.g. (FIXED|CDROM)
 @param  decive_vendor  --> vendor string, 8 bytes; e.g. "KB345678"
 @param  decive_identify--> production identify string, 16 bytes. e.g. "KB34567890123456"
 @param  decive_revision--> revision string, 4 bytes; e.g. "1.01"
 @return
    1, parameter error\n
    0, success\n
*/
int USBStorageRegister( int lun,
			int             (*stor_read)(unsigned long sector, __X void *buf),
			int             (*stor_write)(unsigned long sector, __X void *buf),
			int             (*stor_check_wp)(void),
			int             (*stor_flush)(void),
			unsigned long        (*stor_size_sectors)(void),
			int             (*stor_status)(void),
			unsigned int         device_type,
			char            *device_vendor,
			char            *device_identify,
			char            *device_revision );
//! @}
//! @{	\ingroup APIGroup
/*!	USB loop function, need be called in USB while loop\n
    @return
        USB_WAIT,       wait for connected.
        USB_ATTACHED,   usb attached or busy.
        USB_DISCONECTED, usb disconnected, user can removed.
*/
int USBStorageLoop(void);

//! @}
//! @{	\ingroup APIGroup
/*!	Setup the function pointer when device is under specified case\n
    @param type --> which type would be registered\ref usb_func_group
    @param func --> function pointer
*/
void USBFunctionRegister( int type, void (*func)(void) );
//! @}

//! \defgroup dev_type Device type
//! \defgroup status_group Device status
//! \defgroup usb_func_group USB function register type

//! @{	\ingroup dev_type
enum
{
    DIRECT_ACCESS       =0x000,  /*!< direct-access device */
    CDROM               =0x500   /*!< CD-ROM device, not supported yet*/
};
enum
{
    REMOVABLE           =0x80, /*!< medium is removable */
    FIXED               =00    /*!< not removable */
};
//! @}

//! \defgroup stage_type USB device stage
//! @{	\ingroup stage_type
enum
{
    USB_UPLOAD   = 0x10,  /*!< device is upload data to PC side */
    USB_DOWNLOAD = 0x20   /*!< device is download data from PC side */
};
//! @}
// for usb storage status

//! @{ \ingroup status_group
#define USB_STORAGE_NOT_READY 	0  /*!< device not ready */
#define USB_STORAGE_READY 		1  /*!< device ready */
//! @}

//! @{ \ingroup usb_func_group
#define USB_ATTACH_ISR   0      /*!< function when attached */
#define USB_RESUME_ISR   1      /*!< function when usb resume command */
#define USB_SUSPEND_ISR  2      /*!< function when suspend command */
//! @}


#endif
