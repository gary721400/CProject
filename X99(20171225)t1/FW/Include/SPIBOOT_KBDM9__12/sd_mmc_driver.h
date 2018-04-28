/*! \mainpage KBMP3 SD/MMC Driver Function Libraries
    - SD/MMC Driver Version 0.1\n\n
    
    \section intro_sec Introduction
    This is SD and MMC Card Physical Layer driver library.\n\n
    It supports the following SD and MMC card versions. \n\n
    
    SD Card: \n
    - Physical Layer Version 1.0 ~ 1.1 Standard Capacity SD Card \n\n
    - Physical Layer Version 2.0 Standard and High Capacity(HC) SD Card \n\n
    
    MMC: \n
    - MMC System Spec. Version 3.0 ~ 3.31, 1-bit bus width
    - MMC System Spec. Version 4.0, 1-bit or 4-bit bus width  
*/

#ifndef _SD_DRIVER_H
#define _SD_DRIVER_H

//!    \defgroup ApiGroup SD/MMC Card Driver API Functions

//#define	 WIDE_4		    1
//#define	 WIDE_1		    0

typedef  unsigned char	BYTE;
typedef  unsigned int   WORD;
typedef  unsigned long  DWORD;

/*! \struct Operating_Condition
    SD/MMC OCR Register
*/
//typedef  struct Operating_Condition			// 8 bytes
//{
//  WORD  Power_Up_Status;
//  WORD	Card_Capacity;
//  DWORD Oper_Voltage;  
//}SD_OCR;   /* OCR  Register */
//
//
///*! \struct Card_Identification
//    SD/MMC CID Register
//*/
//typedef  struct Card_Identification			// 20 bytes
//{
//  WORD  Manufacturer_ID;							// 0
//  WORD  Application_ID;								// 2
//  WORD  Product_Name[3];							// 4
//  WORD  Product_Revision;							// 10
//  DWORD Serial_Number;								// 12
//  WORD  Manufacture_Date_Code;				// 16
//  WORD  Check_Sum;										// 18
//
//}SD_CID;  /* CID Register */
//
///*! \struct Card_Specific_Data
//    SD/MMC CSD Register
//*/
//typedef  struct Card_Specific_Data		// 60 bytes
//{
//  WORD  CSD_Structure;								// 0
//  WORD  TAAC;													// 2
//  WORD  NSAC;													// 4
//  WORD  TRAN_SPEED;                   // 6
//  WORD  CCC;                          // 8 
//  WORD  READ_BL_LEN;                  // 10
//  WORD  READ_BL_PARTIAL;  						// 12
//  WORD  WRITE_BLK_MISALIGN;						// 14
//  WORD  READ_BLK_MISALIGN;  					// 16
//  WORD	DSR_IMP	;											// 18
//  DWORD C_SIZE;												// 20
//  WORD  VDD_R_CURR_MIN;								// 24
//  WORD  VDD_R_CURR_MAX;								// 26
//  WORD  VDD_W_CURR_MIN;								// 28
//  WORD  VDD_W_CURR_MAX;  							// 30
//  WORD  C_SIZE_MULT;									// 32
//  WORD  ERASE_BLK_EN;									// 34
//  WORD  SD_SECTOR_SIZE;									// 36
//  WORD  WP_GRP_SIZE;									// 38
//  WORD  WP_GRP_ENABLE;								// 40
//  WORD  R2W_FACTOR;										// 42
//  WORD	WRITE_BL_LEN;									// 44
//  WORD  WRITE_BL_PARTIAL;							// 46
//  WORD  FILE_FORMAT_GRP;							// 48
//  WORD  COPY;													// 50
//  WORD  PERM_WRITE_PROTECT;						// 52
//  WORD  TMP_WRITE_PROTECT;						// 54
//  WORD  FILE_FORMAT;									// 56
//  WORD	CRC;													// 58
// 
//}SD_CSD;       /* CSD Register */
//
///*! \struct Card_Status
//    SD/MMC Card Status
//*/
//typedef  struct Card_Status			// 4 bytes
//{
//  WORD  STATUS_FLAG_FIELD_1;    // 0
//  WORD  STATUS_FLAG_FIELD_2;		// 2
//    
//}SD_Card_Status;             /* Status Register */   
//
///*! \struct SD_CARD
//    SD/MMC Card Information
//*/
//typedef  struct SD_CARD
//{
//  SD_CID sd_identify;           /*!< Contents of CID register */  // offset = 0 bytes     
//  SD_OCR sd_condition;          /*!< Contents of OCR register */ // offset = 20 bytes           
//  SD_CSD  sd_data;              /*!< Contents of CSD register */ // offset = 28 bytes
//  SD_Card_Status  sd_status;    /*!< Status of SD/MMC card */ // offset = 88 bytes        
//  unsigned int Card_Type;              /*!< 0 --> SD card;   1 --> MMC card */ // offset = 92 bytes, 0 -> SD; 1--> MMC
//  unsigned long DEVICE_CAPACITY;        /*!< Capacity of the card. The unit is KByte */ // offset = 94 bytes, unit is KByte
//  unsigned long Total_Block;						/*!< Total usable blocks. One block has 512 bytes.*/
//  unsigned int Min_Erase_Block;					/*!< The minimum number of blocks that can be erased in a single erase command */
//}SD_INFO;

/* Function prototype */

//! @{	\ingroup ApiGroup

/*!	Initialize a SD or MMC card \n\n
		This function initializes registers to attach the SD or MMC.\n\n
 @param  ptr --> the address of a SD_INFO structure.		
 @return     0 is success, others value refer to the file "sd_mmc.inc"
*/
extern int SD1_Init(void);

extern int SD2_Init(void);
/*!	Write data to SD or MMC card\n\n
	It writes 'block_count' blocks of data to specific block numbers in the SD or MMC card. \n\n
 @param  block_addr --> the beginning block number to be written.
 @param  ram_sdbufW --> the data buffer which contains the the data.
 @param  block_count --> the amount of blocks to be written.
 @return     0 is correct, others value refer to the file "sd_mmc.inc"
*/

extern int SD1_read_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);


extern int SD2_read_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);

extern int SD1_write_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);


extern int SD2_write_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);




extern unsigned long SD1_Total_Block;
extern unsigned long SD2_Total_Block;
extern unsigned int SD1_Insert;
extern unsigned int SD2_Insert;
#endif	