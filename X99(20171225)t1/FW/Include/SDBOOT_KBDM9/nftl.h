/*! \mainpage T02 NFTL API function libraries

    \section intro_sec Introduction
    This is Nand Flash Transfer Layer library.\n\n
    - NFTL Version 1.10\n\n
    Total Used XRAM :  3846 bytes\n\n
    Total Code Size : 22606 bytes\n\n
    - nftl_read() only                :  1374+362=1736 bytes\n
    - nftl_resv_read() only           :  1374+454=1828 bytes\n
    - nftl_mount(nftl_read_only)      :  9180 bytes\n
    - nftl_mount(nftl_read_write)     : 11506 bytes\n
    - write                           : 12124 bytes\n

    \defgroup ApiGroup NFTL API functions
    \defgroup RWGroup nftl_mount parameters
    \defgroup IDGroup ID Code Lists
*/
#include "storbase.h"

#ifndef _NFTL_H_
#define _NFTL_H_


/******************************
* Chip Type
******************************/
#define SAMSUNG_K9F1208     0x0076
#define SAMSUNG_K9F1G08     0x00F1
//! @{	\ingroup IDGroup
#define NF1208              0x0076 /*!< samsung 1208 series compatible */
#define NF1G08              0x00F1 /*!< samsung 1g08 series compatible */
#define NF2G08              0x00DA /*!< samsung 2g08 series compatible */
#define NF4G08              0x00DC /*!< samsung 4g08 series compatible */
#define NF8G08              0x00D3 /*!< samsung 8g08 series compatible */
//! @}	\ingroup IDGroup
/******************************
* Flash Information
******************************/


/* Function prototype */


//! @{	\ingroup ApiGroup
/*!	Unmount NAND Flash\n
 @return     0 is correct, others value see \ref ErrGroup
 @note
- All APIs will be useless after this function.
*/
int nftl_unmount(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	Read one sector data from Nand Flash\n
	It reads 'num' sectors of data from a specific sector number on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_read(unsigned long sector, __X void *buf);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	Write one sector data to Nand Flash\n
	It writes 'num' sectors of data from a specific sector number to the NAND flash.
 @param  sector --> which the sector number will be written.
 @param  buf    --> the source data buffer.
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_write(unsigned long sector, __X void *buf);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	It returns the size of total usable space.\n
 @return     size in sectors.
*/
unsigned long nftl_get_size(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	It returns the size of reserved area on the NAND flash.
 @return     The size of reserved area in sectors.
 @note \n The size of reserved area must be the multiples of a block size.
*/
unsigned long nftl_resv_size(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	It reads 'num' sectors of data from the reserved area on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref ErrGroup
 @note
 -	The behavior of nftl_resv_read() is similar to nftl_read().
 -	The starting sector is number 0.
 -	The size of reserved area doesn't decrease even bad blocks increase.
*/
int nftl_resv_read(unsigned long sector, __X void *buf);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	It writes 'num' sectors of data to the reserved area on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to get..
 @return     0 is correct, others value see \ref ErrGroup
*/
int nftl_resv_write(unsigned long sector, __X void *buf);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	It force flush data in memory cache to NAND flash.
 @return     0
 @note \n User must call this function before ending of application
*/
int nftl_flush(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	Return the version of this library.
 @return   verison number
*/
int nftl_version(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	Erase all data in nand flash, and not check mark area in nand flash
    @return 0 is success, else is failure.
*/
int nftl_format(void);
//! @}	\ingroup ApiGroup
//---------------------------------------------
//! @{	\ingroup ApiGroup
/*!	Erase all data in nand flash, and check mark area in nand flash
    @return 0 is success, else is failure.
*/
int nftl_low_level_format( void );
//! @}	\ingroup ApiGroup
//---------------------------------------------


/** \example example.c
 * This is an example of how to use the Test class.
 * More details about this example.
 */

#endif /* _NFTL_H_ */
