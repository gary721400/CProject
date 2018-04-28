/*! \mainpage KBDM9 SPI Flash API Function Library

    \section intro_sec Introduction
    This library provides the functions to access the MX25L8005 1M-byte serial flash. \n
    The SPI interface of KBDM9 is used as the control interface.  
*/

//!    \defgroup ApiGroup SPI Flash API Functions
//! @b Include: SPI_Flash.h 

//! @{	\ingroup ApiGroup


typedef enum {
  SPIF_No_Err           =  0,		/*!< No Error */
  Do_Not_Support        =	-1,		/*!< The SPI Flash is not supported */
  Parameter_Error       =	-2,		/*!< Command Response CRC Error */
  SPIF_No_Space         = -3,    /*!< SPI Falsh do not have enough space */
  Over_Resv_Size        = -4,
  Over_FAT_Size         = -5
} SPIF_ERR;




/*!	Initial the serial flash \n
		This function initializes registers to attach the serial flash.\n
		
 @return -> please refer to the definition of SPIF_ERR.
 @note
- spif_mount() must be called prior to calling any other function.
*/
extern SPIF_ERR spif_mount(void);

/*!	Read data byte by byte\n\n
	It reads 'num' of bytes from specific address in the serial flash. \n
 @param  byte_addr --> the beginning byte address to be read.
 @param  buf    --> the data buffer used to store the data.
 @param  num    --> the amount of byte to be read.
 @return 0 --> correct \n
        -1 --> the address is over the maximum range.
 @note
- The byte address ranges from 0x000000 to 0x0FFFFF.     
*/
extern int SPIF_Read_Byte(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);

/*!	Read data from the specific sector \n\n
	It reads one sector of data from specific sector in the serial flash. \n
	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> please refer to the definition of SPIF_ERR.
*/
extern SPIF_ERR spif_read(unsigned long sector, __X void *buf);



/*!	Write data to specific sector \n\n
	It writes one sector sector of data to specific sector in the serial flash. \n
	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> please refer to the definition of SPIF_ERR.
 
*/
extern SPIF_ERR spif_write(unsigned long sector, __X void *buf);

/*!	Read data from the reserved area.   \n\n
	It reads one sector of data from specific sector in the reserved area. \n
	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> please refer to the definition of SPIF_ERR.
*/
extern SPIF_ERR spif_resv_read(unsigned long sector, __X void *buf);


/*!	Write data to reserved area. \n\n
	It writes one sector sector of data to specific sector in the reserved area. \n
	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> please refer to the definition of SPIF_ERR.
 
*/

extern SPIF_ERR spif_resv_write(unsigned long sector, __X void *buf);

/*!	Write data page by page \n\n
	It writes 'num' page of data to specific page in the serial flash. \n
	Each page of the serial flash contains 256 bytes.\n
 @param  page --> the beginning page number to be written.
 @param  buf  --> the data buffer which contains the data.
 @param  num  --> the amount of pages to be written.
 @return 0 --> correct \n
         -1 --> the page number is over the maximum page.
*/
extern int SPIF_Page_Program(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);

/*!	It returns the sector size of total usable space.\n\n
 @return     size in sector, each sector contains 512 bytes.
*/
extern unsigned long spif_get_size(void);

/*!	Set the chip to deep power-down mode to minimize the power consumption. \n
 @return  no return value.
*/
extern void SPIF_Power_Down(void);

/*!	Release the chip from deep power-down mode. \n
 @return  no return value.
*/
extern void SPIF_Wake_Up(void);

/*!	Erase one of 4K-byte sector in the serial flash. \n
 @param  byte_addr --> the byte address to be erased, the address must be in the 4k-byte boundary.

 @return 0 --> correct \n
         others --> the address is not on 4k-byte boundary.
*/ 
extern int SPIF_Sector_Erase(unsigned long byte_addr);


/*!	Erase the whole chip. All the data byte become 0xFF after eraseing. \n
 @return  no return value.
*/
extern void SPIF_Chip_Erase(void);

extern int SPIF_Init(void);
extern int spif_flush(void);
extern void spif_prepare(unsigned long num);

//! @}	\ingroup ApiGroup

