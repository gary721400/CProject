//--------------------------------------------------------------------
#ifndef _EXAMPLE_USB_STORAGE_H_
#define _EXAMPLE_USB_STORAGE_H_
//--------------------------------------------------------------------
	#define USE_SD	0			// 1:use  SD, 0:not use  SD
	#define USB_UGP	1			// 1:use UGP, 0:not use UGP
#if USE_SD
	#define	USE_SD_CACHE 1			// 1:use cache, 0:no use
#endif
#define  USE_TIMER       3			//  Timer : 0~3, other is disable
#define  USB_COMMIT_TIME 8			//  N/2 Sec
//--------------------------------------------------------------------
typedef enum
{
	USB_CONNECT = 0,			/**< USB�u�ް��AUSB_HelperState����l�]�w */
	USB_DISCONN = 1,			/**< ��PC�ݱj������USB�˸m�A�D�ް�USB�u�A�q�`�O���F�i�H�~��HUSB�R�q*/
}tUSBStatus;
//--------------------------------------------------------------------
#if  USE_SD
void MassStorage_SD_Init( void );
void MassStorage_SD_Loop( void );
void MassStorage_SD_Register( int lun );
#endif
#if  USB_UGP
void MassStorage_ugp_Init( void );
void MassStorage_ugp_Loop( void );
void MassStorage_ugp_Register( int lun );
#endif

void       MassStorage_Init(void);		/*1. Initial */
void       MassStorage_Registers(void);	/*2. Register */
tUSBStatus MassStorage_Loop(void);		/*3. In Loop */
void       MassStorage_Exit(void);		/*4. Exit */
void       MassStorage_Write(void);
#endif						// _EXAMPLE_USB_STORAGE_H_
//--------------------------------------------------------------------
