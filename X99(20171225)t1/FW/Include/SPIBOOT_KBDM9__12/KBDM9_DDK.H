//#if __DOGVER__<12220
//	#error "Dogstar Version should be higher than 1.2.2-20 to compile this package!!"
//#endif

#if __DOGVER__<13140
	#error "Dogstar Version should be higher than 1.3.1-40 to compile this package!!"
#endif

#define PACKAGE_VER 0x1002

#ifndef _KMDM9_DDK_
#define	_KMDM9_DDK_


//***************************************************************************************************************
//KBDM9_SystemGroup
#ifndef KBDM9_Resource
#define	KBDM9_Resource

//!\defgroup KBDM9_ResourceGroup KBDM9 System Resource Category Lists
//!@{\ingroup KBDM9_SystemGroup

/*!
KBDM9 目前所提供的SoftWare IP JumpTable的Trap No.\n
 
*/
typedef enum{
	SET_FLASH_BUSY_TRAP 	= 0x40,		/**<為避免同一個儲存媒體，同時被不同function呼叫時，導致重覆進入造成狀態及RAM的衝突，所設定的Busy flag */
	CLEAR_FLASH_BUSY_TRAP	= 0x41,		/**<為避免同一個儲存媒體，同時被不同function呼叫時，導致重覆進入造成狀態及RAM的衝突，所清除的Busy flag */	
	NFC_DELAY_CALLBACK_TRAP	= 0x42,		/**<NAND-FLSAH 忙錄中，會將主控權交給AP，籍由此Trap No所指到的位置，跳到AP的function，以避免效能的浪費*/
	SD_DELAY_CALLBACK_TRAP	= 0x43,		/**<SD 忙錄中，會將主控權交給AP，籍由此Trap No所指到的位置，跳到AP的function，以避免效能的浪費*/
	STORAGE_SET_BUSY 		= 0x44,		/**<為避免不同的儲存媒體同時存在時之Pin腳衝突，所設定的Busy flag */
	STORAGE_CLR_BUSY		= 0x45,		/**<為避免不同的儲存媒體同時存在時之Pin腳衝突，所清除的Busy flag */
	Set_File_System_Busy = 0x47,
	Clr_File_System_Busy = 0x48,
	Is_CWrite_Protect = 0x49,
	FS_Callback = 0x4A,
	GET_DATE_NTIME = 0x4B, 
	I_Change_Drive = 0x4C 
}tKBDM9_IP_JumpTableTrapNo;
 
/*!
KBDM9目前開放使用的硬體中斷(Interrupt)的Trap No\n
@sa SetVector()
*/
typedef enum{
 STACK_OVERFLOW_TRAP    = 0x04,    /**< Stack Overflow Interrupt  */
 STACK_UNDERFLOW_TRAP   = 0x08,    /**< Stack Underflow Interrupt  */
 EXCEPTION_TRAP         = 0x0C,    /**< Exception Interrupt  */
 TIMER0_INT_TRAP        = 0x10,    /**< Timer 0 Interrupt  */
 TIMER1_INT_TRAP        = 0x12,    /**< Timer 1 Interrupt  */
 TIMER2_INT_TRAP        = 0x14,    /**< Timer 2 Interrupt  */
 TIMER3_INT_TRAP        = 0x16,    /**< Timer 3 Interrupt  */
 TIMER4_INT_TRAP        = 0x18,    /**< Timer 4 Interrupt  */
 I2C_INT_TRAP           = 0x1E,    /**< I2C Interrupt  */
 ADC_INT_TRAP           = 0x20,    /**< Measurement ADC Interrupt  */
 PORT_B_INT_TRAP        = 0x22,    /**< Port B input pin Interrupt  */
 PORT_C_INT_TRAP        = 0x24,    /**< Port C input pin Interrupt  */
 RTC_INT_TRAP           = 0x26,    /**< Real Time Clock Interrupt  */
 CSI_INT_TRAP           = 0x28,    /**< CMOS Sensor Interface Interrupt  */
 SPI2_INT_TRAP          = 0x2A,    /**< SPI2 Interrupt  */
 SPI1_INT_TRAP          = 0x2C,    /**< SPI1 Interrupt  */
 P80_INT_TRAP           = 0x35,    /**< P8088 Interface Interrupt  */
 UART_INT_TRAP          = 0x36,    /**< UART Error Condition Interrupt  */
 UART_TX_INT_TRAP       = 0x37,    /**< UART Transmit Complete Interrupt  */
 UART_RX_INT_TRAP       = 0x38,    /**< UART Receive Complete Interrupt  */
 I2SL_INT_TRAP          = 0x3A,    /**< I2S Left Channel Interrupt  */
 I2SR_INT_TRAP          = 0x3B     /**< I2S Right Channel Interrupt  */
}tKBDM9_Interrupt_TrapNo;
/*!
KBDM9目前提供的DTU
@sa DAC_Init(); AUADC_Init()
*/
typedef enum{
  DTU0			=	0,		/**<使用 DTU0時，請使用此參數，根據KBDM9之硬體規格，此priority 為0(最低)*/
	DTU1			=	1,		/**<使用 DTU1時，請使用此參數，根據KBDM9之硬體規格，此priority 為1*/
	DTU2			=	2,		/**<使用 DTU2時，請使用此參數，根據KBDM9之硬體規格，此priority 為2*/
	DTU3			=	3,		/**<使用 DTU3時，請使用此參數，根據KBDM9之硬體規格，此priority 為3*/
}tKBDM9_DTUNumber;

//!@}

//**************************************END KBDM9_SystemGroup******************************************************************************** 
#endif   // KBDM9_Resource
 
/*
**	Direct access to DMCU memory areas.
*/

#ifndef _ABSACC_H_
#define _ABSACC_H_
/*
** Direct access Byte/Word
*/
#define m8HI4(arg)			(arg>>4)
#define m8LO4(arg)			(arg&0x0F)

#define m16HI8(arg)			(*( unsigned char *)&(arg))
#define m16LO8(arg)			(*((unsigned char *)&(arg) + 1))

#define m32HI16(arg)		(*( unsigned int  *)&(arg))
#define m32LO16(arg)		(*((unsigned int  *)&(arg) + 1))

#define m32HI8(arg)			(*( unsigned char *)&(arg))
#define m32MH8(arg)			(*((unsigned char *)&(arg) + 1))
#define m32ML8(arg)			(*((unsigned char *)&(arg) + 2))
#define m32LO8(arg)			(*((unsigned char *)&(arg) + 3))

/*
** Direct access GPR/YRAM/XRAM
*/
#define XBYTE ((__X volatile unsigned char *) (unsigned int)0x0000)
#define XWORD ((__X volatile unsigned int  *) (unsigned int)0x0000)
#define XLONG ((__X volatile unsigned long *) (unsigned int)0x0000)

#define YBYTE ((__Y volatile unsigned char *) (unsigned int)0x8000)
#define YWORD ((__Y volatile unsigned int  *) (unsigned int)0x8000)
#define YLONG ((__Y volatile unsigned long *) (unsigned int)0x8000)

#define GPREG ((__X volatile unsigned int  *) (unsigned int)0xF2E0)

#define __near __X

extern void* nearcpy(__near void* d, __near void* s, unsigned int l);

extern void* nearset(__near void* p, unsigned int d,  unsigned int l);

extern int nearcmp(__near void* m, __near void* n,  unsigned int l);

extern unsigned int min(unsigned int i, unsigned int j);

extern unsigned int max(unsigned int i, unsigned int j);


#endif	//_ABSACC_H_


#ifndef _clock_drv_h
#define _clock_drv_h

//! \defgroup SysClockGroup System Clock setting 
//! @{	\ingroup KBDM9_SystemGroup


/*! 	
	KBDM9可設定的系統頻率\n
*/
typedef enum{
	FEQ_60M		  =	 3,	/**< System clock = 60 MHz    */
	FEQ_48M		  =	 4,	/**< System clock = 48 MHz    */
	FEQ_40M		  =	 5,	/**< System clock = 40 MHz    */
	FEQ_34P28M	=	 6,	/**< System clock = 34.28 MHz	*/
	FEQ_30M		  =	 7,	/**< System clock = 30 MHz    */
	FEQ_26P66M	=	 8,	/**< System clock = 26.66 MHz */
	FEQ_24M		  =  9,	/**< System clock = 24 MHz    */
	FEQ_20M		  =	11,	/**< System clock = 20 MHz    */
	FEQ_16M  		=	14,	/**< System clock = 16 MHz    */
	FEQ_15M     = 15, /**< System clock = 15 MHz    */
	FEQ_12M     = 19, /**< System clock = 12 MHz    */
	FEQ_10M     = 23, /**< System clock = 10 MHz    */ 
	FEQ_9P6M    = 24, /**< System clock = 9.6 MHz   */ 
	FEQ_8M      = 29, /**< System clock = 8 MHz     */
	FEQ_7P5M    = 31  /**< System clock = 7.5 MHz   */
}tFEQ;



/*!	
	 執行此function設定KBDM9的System Clock
	@param	Freq :設定的頻率
	@return 
		0 : OK @n
		-1 : 不支援此設定 \n
	@note \b Code \b Address \b Name => \b SYSDRV_CODE 		
*/
extern unsigned int	SystemClock( tFEQ Freq);

/*!
詢問目前的系統頻率設定值
@return 目前的系統頻率設定值
@note \b Code \b Address \b Name => \b SYSDRV_CODE
*/
extern tFEQ QuerySysClock(void);

//! @} 

//! \defgroup SystemModeGroup Standby & Sleep mode setting
//! @{	\ingroup KBDM9_SystemGroup

/*! 	
	從Standby 或 Sleep Mode 被 wake up之後所要執行之function. \n
*/
typedef void (*PFWAKEUP)(void);

/*!
 讓KBDM9進入Sleep Mode. \n
 執行此function之後, 整個KBDM9 chip只有RTC clock仍在運作, 其他的clock包含震盪電路都會被關閉. \n
 有三種方式可以使KBDM9回到正常模式(Normal Mode): \n
 1. GPIO pin 的 interrupt. \n
 2. 插接 USB. \n
 3. RTC 的 interrupt. \n
 
 @note KBDM9 進入Sleep Mode之後, YRAM的資料將全部消失, 除了XRAM位址0x0000 至 0x0FFF(4K byts)的資料以及 \n
       IRAM位址 0x010000 至 0x010FFF(4K byts)的資料會被保存下來, 其餘的XRAM, YRAM及 IRAM裡的資料將全部消失. \n        
*/
extern void EntrySleepMode(PFWAKEUP pfwakeup);

/*!
 讓KBDM9進入Standby Mode. \n
 執行此function之後, RTC clock及 RAM 的clock 仍會運作, 其他的clock包含震盪電路則會被關閉. \n
 有三種方式可以使KBDM9回到正常模式(Normal Mode): \n
 1. GPIO pin 的 interrupt. \n
 2. 插接 USB. \n
 3. RTC 的 interrupt. \n
 
 @note KBDM9 進入Standby mode之後, 因RAM 的clock 仍會運作, 所也的RAM的資料都會被保存下來. \n        
*/
extern void EntryStandbyMode(PFWAKEUP pfwakeup);


//! @} 
#endif	//_clock_drv_h


//==============================================================================
//! \defgroup KBDM9_SystemGroup  System tool functions

//! @{						

/*!
若需使用任何IP，必需呼叫此function，只需要執行一次即可。
*/
extern void IP_INIT(void);	


/*!
設定interrupt service routine的中斷向量\n
@param  TrapNo -->中斷發生裝置的TrapNo編號，請參考 \ref tKBDM9_Interrupt_TrapNo 
@param  pIsrFunc -->中斷發生時所有執行的function，此function若以C編寫，必需是__interrupt宣告，若為ASM必需以RETI返回。
@return None
*/							
extern void SetVector(unsigned int TrapNo, void (*pIsrFunc)(void));


/*!
將XRAM內某一個指定區域的內容，copy到IRAM某一個指定區域內\n
@param source_ptr =>XRAM內複製來源的區域的起始位置。
@param destination =>IRAM內複製目標區域的起始位置。
@param data_length =>所要複製的大小，以Byte為單位
@note data_length長度限制，必需是偶數，且最小為6
*/
extern void CopyXRAMtoIRAM(	__X int* source_ptr, int* destination, unsigned int data_length);

//! @}			

//**************************************END KBDM9_SystemGroup********************************************************************************


//**************************************************************************************************************************************
//KBDM9_MassStorageGroup



//#include "storbase.h"

#ifndef _NFTL_H_
#define _NFTL_H_

//! \defgroup KBDM9_MassStorageGroup Mass Storage Function 
//! @{

/*!
      \section intro_sec Introduction  
      KBDM9 提供 NAND-FLAH, SD card及SPI NOR-Flash 三種儲存媒體. 
*/
//! @}

//!@{ \ingroup KBDM9_MassStorageGroup

/*! KBDM9 Nand Flash Transfer Layer library\n
    - NFTL Version 1.00\n\n
*/

//! \defgroup NFTLGroup NAND-FLASH Trans-Layer

//! @}

/******************************
* Chip Type
******************************/
#define SAMSUNG_K9F1208     0x0076
#define SAMSUNG_K9F1G08     0x00F1
//! \defgroup NFTLIDGroup ID Code Lists
//! @{	 \ingroup NFTLGroup
#define NF1208              0x0076 /*!< samsung 1208 series compatible */
#define NF1G08              0x00F1 /*!< samsung 1g08 series compatible */
#define NF2G08              0x00DA /*!< samsung 2g08 series compatible */
#define NF4G08              0x00DC /*!< samsung 4g08 series compatible */
#define NF8G08              0x00D3 /*!< samsung 8g08 series compatible */

/******************************
* Flash Information
******************************/

/*! \struct _nand_flash_info
    NAND Flash Information
*/
typedef struct _nand_flash_info {
	unsigned int	chipType;    /*!< chip id */
	unsigned int	pageSize;    /*!< page size in bytes */
	unsigned int	spareSize;   /*!< spare size in bytes */
	unsigned int	blockCount;  /*!< total block count */
	unsigned long	blockSize;  /*!< block size in bytes */
} NAND_FLASH_INFO;

/*!< \struct _nf_block_info
    information for one block in NAND Flash
*/
typedef struct _nf_block_info {
    unsigned int    numUsedBlocks; /*!< block number is in used */
    unsigned int    numFreeBlocks; /*!< block number is free */
    unsigned int    numBadBlocks;  /*!< block number is bad */
    unsigned int    percent; /*!< percent completed */
    unsigned int    cancel;  /*!< cancel or not */
} NF_BLOCK_INFO;

//! @}


//! \defgroup NFTL_ErrGroup Error Code
//!@{	\ingroup NFTLGroup
#define NFTL_NO_DATA        (1)         /*!< this sector is empty */
#define NFTL_NO_ERR         (0)         /*!< no error */
#define NFTL_PARAM_ERR      (-1)        /*!< parameter is an invalid */
#define NFTL_MEDIA_ERR      (-2)        /*!< the flash chip is unrecognized */
#define NFTL_SYSTEM_ERR     (-3)        /*!< the NFTL system does not exist */
#define NFTL_NO_SPACE       (-4)        /*!< the NFTL is full */
#define NFTL_DATA_ERR       (-5)        /*!< the data maybe incorrect */
#define NFTL_CACHE_ERR      (-6)        /*!< NFTL need cache, please set it first.  */
#define NFTL_MEMORY_ERR     (-10)       /*!< the flash is bad. */                                      
//!@}	

/* Function prototype */

//! \defgroup NFTLApiGroup NFTL API functions
//! @{	\ingroup NFTLGroup
/*!	Unmount NAND Flash\n
 @return     0 is correct, others value see \ref NFTL_ErrGroup
 @note \n
 All APIs will be useless after this function.
*/
int nftl_unmount(void);

/*!	Read one sector data from Nand Flash\n
	It reads one sector of data from a specific sector number on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref NFTL_ErrGroup
*/
int nftl_read(unsigned long sector, __X void* buf);

/*!	Write one sector data to Nand Flash\n
	It writes one sector of data from a specific sector number to the NAND flash.
 @param  sector --> which the sector number will be written.
 @param  buf    --> the source data buffer.
 @return     0 is correct, others value see \ref NFTL_ErrGroup
*/
int nftl_write(unsigned long sector, __X void *buf);

/*!	It returns the size of total usable space.\n
 @return     size in sectors.
*/
unsigned long nftl_get_size(void);


/*!	It returns the size of reserved area on the NAND flash.
 @return     The size of reserved area in sectors.
 @note \n The size of reserved area must be the multiples of a block size.
*/
unsigned long nftl_resv_size(void);

/*!	It reads one sector of data from the reserved area on the NAND flash.
 @param  sector --> which the sector number will be read.
 @param  buf    --> the data buffer to put.
 @return     0 is correct, others value see \ref NFTL_ErrGroup
 @note
 -	The behavior of nftl_resv_read() is similar to nftl_read().
 -	The starting sector is number 0.
 -	The size of reserved area doesn't decrease even bad blocks increase.
*/
int nftl_resv_read(unsigned long sector, __X void* buf);

/*!	It writes one sector of data to the reserved area on the NAND flash.
 @param  sector --> which the sector number to be written.
 @param  buf    --> the data buffer to get..
 @return     0 is correct, others value see \ref NFTL_ErrGroup
*/
int nftl_resv_write(unsigned long sector, __X void* buf);

/*!	It force flush data in memory cache to NAND flash.
 @return     0
 @note \n User must call this function before ending of application
*/
int nftl_flush(void);

/*!	Return the version of this library.
 @return   verison number
*/
int nftl_version(void);


/*!	Erase all data in nand flash, and not check mark area in nand flash
    @return 0 is success, else is failure.
*/
int nftl_format(void);

/*!	Erase all data in nand flash, and check mark area in nand flash
    @return 0 is success, else is failure.
*/
int nftl_low_level_format( void );

//!@}	
#endif /* _NFTL_H_ */


/*!@{ \ingroup KBDM9_MassStorageGroup
 
	KBDM9 SD/MMC Driver Function Libraries
    - SD/MMC Driver Version 1.0\n\n
        
    This is SD and MMC Card Physical Layer driver library.\n\n
    It supports the following SD and MMC card versions. \n\n
    
    SD Card: \n
    - Physical Layer Version 1.0 ~ 1.1 Standard Capacity SD Card \n\n
    - Physical Layer Version 2.0 Standard and High Capacity(HC) SD Card \n\n
    
    MMC: \n
    - MMC System Spec. Version 3.0 ~ 3.31, 1-bit bus width
    - MMC System Spec. Version 4.0, 1-bit or 4-bit bus width  
   \defgroup SDDriverApiGroup SD/MMC Card Driver API Functions

 @}*/

#ifndef _SD_DRIVER_H
#define _SD_DRIVER_H

//!@{ \ingroup SDDriverApiGroup

/* Function prototype */

/*!	Initialize the first SD or MMC card \n
		This function initializes the SD controller to attach the first SD or MMC.\n	
 @return 0 is success, others value refer to \ref SDTL_ErrGroup
*/
extern int SD1_Init(void);

/*!	Initialize the second SD or MMC card \n
		This function initializes the SD controller to attach the second SD or MMC.\n	
 @return 0 is success, others value refer to \ref SDTL_ErrGroup
*/
extern int SD2_Init(void);

/*!	Read data from the first SD or MMC card\n

 @param  block_addr --> the beginning sector number to be read.
 @param  ram_sdbufW --> the data buffer used to store the data.
 @param  block_count  --> the amount of blocks to be read.
 @return 0 is correct, others value refer to \ref SDTL_ErrGroup
*/
extern int SD1_read_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);

/*!	Read data from the second SD or MMC card\n

 @param  block_addr --> the beginning sector number to be read.
 @param  ram_sdbufR --> the data buffer used to store the data.
 @param  block_count  --> the amount of blocks to be read.
 @return 0 is correct, others value refer to \ref SDTL_ErrGroup
*/
extern int SD2_read_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);

/*!	Write data to the first SD or MMC card\n\n

 @param  block_addr --> the beginning block number to be written.
 @param  ram_sdbufW --> the data buffer which contains the the data.
 @param  block_count --> the amount of blocks to be written.
 @return 0 is correct, others value refer to \ref SDTL_ErrGroup
*/
extern int SD1_write_block(unsigned long block_addr, __X void *ram_sdbufW, unsigned int block_count);

/*!	Write data to the second SD or MMC card\n\n

 @param  block_addr --> the beginning block number to be written.
 @param  ram_sdbufR --> the data buffer which contains the the data.
 @param  block_count --> the amount of blocks to be written.
 @return 0 is correct, others value refer to \ref SDTL_ErrGroup
*/
extern int SD2_write_block(unsigned long block_addr, __X void *ram_sdbufR, unsigned int block_count);

/*!	Erase all data in the first SD card
 @return 0 is success, else is failure.
*/
extern int SD1_erase_all(void);

/*!	Erase all data in the second SD card
 @return 0 is success, else is failure.
*/
extern int SD2_erase_all(void);


/*! Set the first SD card to High-Speed mode \n   

This function will query the first SD card to check whether it can support high-speed mode. \n

If it can be operated in high-speed mode, the interfac clock is set to 48MHz. Otherwise, \n

the interface clock is set to 24MHz.

@return  0  --> the SD card supports High-Speed mode. \n\n -1 --> the SD card does't supports High-Speed mode.
*/
extern int SD1_fast_mode(void);

/*! Set the second SD card to High-Speed mode \n   

This function will query the second SD card to check whether it can support high-speed mode. \n

If it can be operated in high-speed mode, the interfac clock is set to 48MHz. Otherwise, \n

the interface clock is set to 24MHz.

@return  0  --> the SD card supports High-Speed mode. \n\n -1 --> the SD card does't supports High-Speed mode.
*/
extern int SD2_fast_mode(void);

//! @} \ingroup SDDriverApiGroup
extern void SD_Busy_Check(void);
extern unsigned long SD1_Total_Block;
extern unsigned long SD2_Total_Block;
extern unsigned int SD1_Insert;
extern unsigned int SD2_Insert;

#endif	

//=============================================================================

  
//!@{\ingroup KBDM9_MassStorageGroup
/*!
 This is SD and MMC Card Transfer Layer library. \n
 All the functions are available only for the first SD card, i.e. SD1.\n
 
 It supports the following SD and MMC card versions. \n
    
    SD Card: \n
    - Physical Layer Version 1.0 ~ 1.1 Standard Capacity SD Card \n\n
    - Physical Layer Version 2.0 Standard and High Capacity(HC) SD Card \n\n
    
    MMC: \n
    - MMC System Spec. Version 3.0 ~ 3.31, 1-bit bus width\n\n
    - MMC System Spec. Version 4.0, 1-bit or 4-bit bus width  
*/
//! \defgroup SDTLGroup SDTL API Functions 

//! @}	

#ifndef _SDtl_H_
#define _SDtl_H_

// Error Flag

//!    \defgroup SDTL_ErrGroup Error Flag List	
//! @{	\ingroup SDTLGroup
/*! Error messages of SDTL function. */
typedef enum {
  SDTL_No_Err           =  0,		/*!< No Error */
  Data_CRC_Err          =	-1,		/*!< Data CRC Error */
  RSP_CRC_Err	          =	-2,		/*!< Command Response CRC Error */
  CMD_RSP_Timeout       =	-3, 	/*!< Command Response Timeout */
  PRG_Timeout           =	-4, 	/*!< Program or Erase timeout */
  SD_Not_Ready_for_Data =	-5, 	/*!< SD or MMC card is not ready for data transaction */
  Write_Data_Err 	      =	-6, 	/*!< The data are not correctly written into the SD or MMC card */
  Erase_Addr_Err 	      =	-7, 	/*!< The starting address is not the multiple of the minimum erasable blocks */
  Erase_Block_No_Err 	  =	-8, 	/*!< The amounts of erased sector is not the multiple of the minimum erasable blocks */
  Write_To_Protect_ERR  =	-9, 	/*!< Write to write-protected area */
  Unusable_Card	        =	-10, 	/*!< The SD or MMC card is not usable.  */
  No_Card_Inserted	    =	-11,  /*!< No SD/MMC insert */
  SD_Over_Resv_Size     = -12,  /*!< The sector number is over the reserved area. */
  SD_Over_FAT_Size      = -13,  /*!< The sector number is over the usable FAT area. */
  Unknown_Err           = -14  	/*!< Unknown Error */
}SDTL_ERR;
//! @}	

/* Function prototype */
//!    \defgroup SDTL_FunGroup SDTL function	
//! @{	\ingroup SDTLGroup

/*!	Munt a SD card \n\n
This function initializes SD controller to attach the SD card and checks the SDTL system.\n

 @return     0 is success, others value refer to \ref SDTL_ERR

 @note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_mount(void);

/*!	Returns the size of reserved area on the SD card.
 @return size in sectors.
 
@note \b Code \b Address \b Name => \b SDTL_CODE 
*/						
extern unsigned long sdtl_resv_size(void);

/*!	Returns the size of total usable space.\n\n
 @return  size in sectors
@note \b Code \b Address \b Name => \b SDTL_CODE
*/ 
extern unsigned long sdtl_get_size(void);

/*!	Reads one sector of data from the reserved area on SD card.
 @param  sector --> the sector number will be read.
 @param  buf    --> the buffer to store the data.
 @return  0 is correct, others value see \ref SDTL_ERR

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_resv_read(unsigned long sector, __X void *buf);

/*!	Writes one sector of data to the reserved area on SD card.
 @param  sector --> the sector number to be written.
 @param  buf    --> the buffer that contains the data.
 @return  0 is correct, others value see \ref SDTL_ERR

@note \b Code \b Address \b Name => \b SDTL_CODE
*/							
extern SDTL_ERR sdtl_resv_write(unsigned long sector, __X void *buf);

/*!	Reads one sector of data from SD card.
 @param  sector --> the sector number will be read.
 @param  buf    --> the buffer to store the data.
 @return  0 is correct, others value see \ref SDTL_ERR

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_read(unsigned long sector, __X void *buf);

/*!	Writes one sector of data to SD card.
 @param  sector --> the sector number to be written.
 @param  buf    --> the buffer that contains the data.
 @return  0 is correct, others value see \ref SDTL_ERR

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_write(unsigned long sector, __X void *buf);

/*!	Initialize the status of cache for SD read/write operation. \n
This function will create a 2K-byte cache to speed the read/write operations. \n

@note This function shall be executed befor using the cache to speed up the read/write operations.
@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern void sdtl_cache_init(void);

/*!	Reads one sector of data from SD card with cache scheme.
 @param  sector --> the sector number will be read.
 @param  buf    --> the buffer to store the data.
 @return  0 is correct, others value see \ref SDTL_ErrGroup

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_cache_read(unsigned long sector, __X void *buf);



/*!	Writes one sector of data to SD card with cache scheme.
 @param  sector --> the sector number to be written.
 @param  buf    --> the buffer that contains the data.
 @return  0 is correct, others value see \ref SDTL_ERR

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_cache_write(unsigned long sector, __X void *buf);


/*!	Reads one sector of data from SD card eith two-cache scheme. \n
    This function is used to speed up the reading operation in USB Mass Storage application.
      
 @param  sector --> the sector number will be read.
 @param  buf    --> the buffer to store the data.
 @return  0 is correct, others value see \ref SDTL_ErrGroup

@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern SDTL_ERR sdtl_usb_read(unsigned long sector, __X void *buf);

/*!	Flush the data in memory cache to SD card. \n
 If the sdtl_cache_read() or sdtl_cache_write() is used in USB MassStorage or FAT interface. \n
 It must be executed before ending the USB MassStorage or FAT applications.
 @return 0
 
@note \b Code \b Address \b Name => \b SDTL_CODE 
*/

extern int sdtl_cache_flush(void); 

/*!	A null function which is used to match the interface of USB MassStorage and FAT.
 @return     0
@note \b Code \b Address \b Name => \b SDTL_CODE 
*/
extern int sdtl_flush(void);

/*!	A null function which is used to match the interface of USB MassStorage.
@note \b Code \b Address \b Name => \b SDTL_CODE
*/
extern void sdtl_prepare(unsigned long num);

//! @} 

extern SDTL_ERR sdtl_allocate(unsigned long  resv_ksize, char *stamp);
#endif /* _SDtl_H_ */


#ifndef KBDM9_SPIF
#define	KBDM9_SPIF
/*!@{\ingroup KBDM9_MassStorageGroup

    This library provides the functions to access the serial flashs of MXIC and Winbond. \n\n
    The SPI interface of KBDM9 is used as the control interface.  
	\defgroup SPI_FLASH_APIGroup SPI Flash API Functions

@}*/

// Error Flag
//!    \defgroup SPIF_ErrGroup Error Flag List	
//! @{	\ingroup SPI_FLASH_APIGroup
/*! Error messages of SPIF function. */
typedef enum {
  SPIF_No_Err           =  0,		 /*!< No Error */
  Do_Not_Support        =	-1,		 /*!< The SPI Flash is not supported */
  Parameter_Error       =	-2,		 /*!< Invalid parameter */
  SPIF_No_Space         = -3,    /*!< SPI Falsh do not have enough space */
  Over_Resv_Size        = -4,    /*!< The sector number is over the reserved area */
  Over_FAT_Size         = -5     /*!< The sector number is over the usable FAT area */
} SPIF_ERR;
//! @}

/* Function prototype */
//!    \defgroup SPIF_FunGroup SPIF Function	
//!@{ \ingroup SPI_FLASH_APIGroup

/*!	Initial the serial flash \n
		This function initializes SPI interace to attach the serial flash and setup the Transfer Layer functions. 
		
 @return -> 0 is successful, others value refer to \ref SPIF_ERR.
 @note
- spif_mount() must be called prior to calling any other function.

@note \b Code \b Address \b Name => \b SPIF_CODE

*/
extern SPIF_ERR spif_mount(void);

/*!	Configure the flash to read-only mode. \n
		This function performs the software write-protection on the whole flah. \n
		Any write or erase operation will not modify the contents of flash. 
		
 @return no return value.
*/
extern void SPIF_Read_Only(void);

/*!	Configure the flash to read/write mode. \n
		This function will allow the flash to perform both read and write operations.  
		
 @return no return value.
*/
extern void SPIF_Read_Write(void); 

/*!	Read data byte by byte\n
	It reads numbers of bytes from specific address in the serial flash. \n
 @param  byte_addr --> the beginning byte address to be read.
 @param  buf    --> the data buffer used to store the data.
 @param  num    --> the amount of byte to be read.
 @return -> 0 is correct, others value refer to \ref SPIF_ERR.
 
 @note \b Code \b Address \b Name => \b SPIF_CODE     
*/
extern int SPIF_Read_Byte(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);

/*!	Read one sector of data from the serial flash. \n
  	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> 0 is correct, others value refer to \ref SPIF_ERR.
 
 @note \b Code \b Address \b Name => \b SPIF_CODE    
*/
extern SPIF_ERR spif_read(unsigned long sector, __X void *buf);



/*!	Write one sector sector of data to the serial flash \n
  	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> 0 is correct, others value refer to \ref SPIF_ERR.

 @note \b Code \b Address \b Name => \b SPIF_CODE    
*/
extern SPIF_ERR spif_write(unsigned long sector, __X void *buf);

/*!	Read one sector of data from the reserved area.   \n
	Each sector contains 512 bytes. \n
 @param  sector --> the sector to be read.
 @param  buf    --> the data buffer used to store the data.

 @return -> 0 is correct, others value refer to \ref SPIF_ERR.

 @note \b Code \b Address \b Name => \b SPIF_CODE   
*/
extern SPIF_ERR spif_resv_read(unsigned long sector, __X void *buf);


/*!	Write one sector sector of data to reserved area. \n
	Each sector contains 512 bytes.\n
 @param  sector --> the sector to be written.
 @param  buf  --> the data buffer which contains the data.
 @return -> 0 is correct, others value refer to \ref SPIF_ERR.

 @note \b Code \b Address \b Name => \b SPIF_CODE   
 
*/

extern SPIF_ERR spif_resv_write(unsigned long sector, __X void *buf);

/*!	It returns the sector size of total usable space.\n\n
 @return     size in sector, each sector contains 512 bytes.
*/
extern unsigned long spif_get_size(void);

/*!	It returns the sector size of total reserved space.\n\n
 @return     size in sector, each sector contains 512 bytes.
*/
extern unsigned long spif_resv_size(void);

/*!	Set the serial flash to deep power-down mode to minimize the power consumption. \n
 @return  no return value.
*/
extern void SPIF_Power_Down(void);

/*!	Release the serial flash from deep power-down mode. \n
 @return  no return value.
*/
extern void SPIF_Wake_Up(void);


/*!	Erase the whole chip. All the data byte become 0xFF after eraseing. \n
 @return  no return value.
*/
extern void SPIF_Chip_Erase(void);
//! @}	


/*	Write data page by page \n\n
	It writes 'num' page of data to specific page in the serial flash. \n
	Each page of the serial flash contains 256 bytes.\n
 @param  page --> the beginning page number to be written.
 @param  buf  --> the data buffer which contains the data.
 @param  num  --> the amount of pages to be written.
 @return 0 --> correct \n
         -1 --> the page number is over the maximum page.
*/
extern int SPIF_Page_Program(unsigned long byte_addr, __X unsigned char *buf, unsigned int num);


/*	Erase one of 4K-byte sector in the serial flash. \n
 @param  byte_addr --> the byte address to be erased, the address must be in the 4k-byte boundary.

 @return 0 --> correct \n
         others --> the address is not on 4k-byte boundary.
*/ 
extern int SPIF_Sector_Erase(unsigned long byte_addr);

extern int SPIF_Init(void);
extern int spif_flush(void);
extern void spif_prepare(unsigned long num);

#endif  /*KBDM9_SPIF */




//****************************************************************************************************************
//!@{

/*!
	KBDM9週邊Communication相關的Driver介紹 \n
	主要有：USB
	
*/
//!    \defgroup KBDM9_PeripheralGroup KBDM9 Peripheral API Functions   


//!@}



//!@{\ingroup KBDM9_PeripheralGroup

/*! 
 \section intro Introduction
 USB Mass Storage API functions for KBDM9\n
 -  version 1.0\n\n
  \section us_sect Using Library
    User need to prepare extra memory for library usage.\n
    If user want USB to show 2 device, user may declare as:\n\n
    -  _STORAGE usb_stor[2];
    
    \defgroup USBfunctionGroup	USB Library API    
*/

//!@}

#ifndef __USB_STOR_API_H__
#define __USB_STOR_API_H__


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


void USBHIDInit(void);

void USBCoreInit(void);

void USBAudioInit(void);

int  usb_empty_func2(void);


//!@{\ingroup USBfunctionGroup
/*!
	USB debounce delay value\n
*/
void USBSetDebounceCount(unsigned long value);

/*!	Detect the attachment of USB\n
 @return
    1, attached\n
    0, not attached\n
*/
int USBAttachCheck(void);

/*!
Power off the usb mass storage
*/
void USBStoragePoweroff(void);

/*!	Detect the remove of USB\n
 @return
    1, remove\n
    0, connect\n
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

/*!	Setup the device on USB storage\n
 @param  lun            --> which device order.
 @param  stor_read      --> read function pointer.
 @param  stor_write     --> write function pointer.
 @param  stor_check_wp  --> check write-propection function pointer.
 @param  stor_flush     --> flush device statue function pointer.
 @param  stor_size_sectors  --> device size in sectors.
 @param  stor_status    --> device status, see \ref status_group .
 @param  device_type    --> device type, see \ref dev_type; e.g. (FIXED|CDROM)
 @param  device_vendor  --> vendor string, 8 bytes; e.g. "KB345678"
 @param  device_identify--> production identify string, 16 bytes. e.g. "KB34567890123456"
 @param  device_revision--> revision string, 4 bytes; e.g. "1.01"
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


/*!	USB loop function, need be called in USB while loop\n
    @return
        USB_WAIT,       wait for connected.
        USB_ATTACHED,   usb attached or busy.
        USB_DISCONECTED, usb disconnected, user can removed.
*/
int USBCoreLoop(void);


/*!	Setup the function pointer when device is under specified case\n
    @param type --> which type would be registered\ref usb_func_group
    @param func --> function pointer
*/
void USBFunctionRegister( int type, void (*func)(void) );
//! @}


//! \defgroup dev_type Device type
//! @{	\ingroup USBfunctionGroup
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
//! @{	\ingroup USBfunctionGroup
enum
{
    USB_UPLOAD   = 0x10,  /*!< device is upload data to PC side */
    USB_DOWNLOAD = 0x20   /*!< device is download data from PC side */
};
//! @}

// for usb storage status

//! \defgroup status_group Device status
//! @{ \ingroup USBfunctionGroup
#define USB_STORAGE_NOT_READY 	0  /*!< device not ready */
#define USB_STORAGE_READY 		1  /*!< device ready */
//! @}

//! \defgroup usb_func_group USB function register type
//! @{ \ingroup USBfunctionGroup
#define USB_ATTACH_ISR   0      /*!< function when attached */
#define USB_RESUME_ISR   1      /*!< function when usb resume command */
#define USB_SUSPEND_ISR  2      /*!< function when suspend command */
//! @}


#endif

//****************************************************************************************************************

/*!@{
    \defgroup KBDM9_AudioInOutGroup KBDM9 Audio In/Out API Functions   

 	KBDM9聲音輸入與輸出相關的Driver介紹 \n
 	主要有：Audio DAC及Audio ADC Drvier \n


@}*/


#ifndef KBDM9_DAC
#define	KBDM9_DAC
/*!@{\ingroup KBDM9_AudioInOutGroup

	\section intro Introduction
 	Audio DAC driver Interface\n
 	-       version 1.0\n\n
 	1、輸入資料格式為 16-bit PCM。 \n
 	2、支援雙聲道輸出, 輸出方式可選擇耳機或喇吧。  
	2、支援 8, 9.6, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48, 及 96 KHz Sampling rate。	
   \defgroup DACGroup DAC Driver Introduction

@}*/

//!\defgroup DACErrGroup Error Flag Lists
//!@{\ingroup DACGroup
/*!
DAC driver function的回傳值\n
使用DAC driver發生錯誤時，查詢用。
*/
typedef enum{
	DAC_NO_ERR					    =	0,			  /**< DAC function沒有錯誤 */
	DAC_SAMPLE_RATE_ERR	    =	-1,	      /**< Sampling rate設定錯誤*/
	DAC_NO_SET_FRAME_SIZE	  =	-2,				/**< DAC啟動但未設定Frame size*/
	DAC_DTU_SETUP_ERR		    =	-3,			  /**< DTU number 設定錯誤*/
	DAC_FRAME_SIZE_ERR		  =	-4,				/**<Frame size設定超過0x1fff的大小*/
	DAC_NO_INIT				      =	-5,		    /**<尚未初始化，需先執行DAC_Init*/
	DAC_PLL_SETUP_ERR		    =	-6,			  /**<PLL沒打開，無法以正常頻率播放*/
	DAC_SOCR_BIT15_DISABLE	=	-7,  			/**<SOCR.15未打開=>Interrupt的總開關未打開*/
}tDAC_Error;

//! @}

//!\defgroup DACSRAGroup Sampling Rate Lists
//!@{\ingroup DACGroup
/*!
DAC 可以設定的 Sampling Rate. 除此之外的數值均為無效的設定.

@sa DAC_Init
*/
typedef enum{
	DAC_SAMPLE_RATE_96K			=	0,				/**< DAC sampling rate設定為96KHz*/	
	DAC_SAMPLE_RATE_48K			=	1,				/**< DAC sampling rate設定為48KHz*/
	DAC_SAMPLE_RATE_44P1K		=	2,				/**< DAC sampling rate設定為44.1KHz*/
	DAC_SAMPLE_RATE_32K			=	3,				/**< DAC sampling rate設定為32KHz*/
	DAC_SAMPLE_RATE_24K			=	4,				/**< DAC sampling rate設定為24KHz*/
	DAC_SAMPLE_RATE_22P05K	=	5,				/**< DAC sampling rate設定為22.05KHz*/
	DAC_SAMPLE_RATE_16K			=	6,				/**< DAC sampling rate設定為16KHz*/
	DAC_SAMPLE_RATE_12K			=	7,				/**< DAC sampling rate設定為12KHz*/
	DAC_SAMPLE_RATE_11P025K	=	8,				/**< DAC sampling rate設定為11.025KHz*/
	DAC_SAMPLE_RATE_9P6K  	=	9,				/**< DAC sampling rate設定為9.6KHz*/	
	DAC_SAMPLE_RATE_8K			=	10				/**< DAC sampling rate設定為8KHz*/
}tDAC_SampleRate;

//! @}

//!\defgroup DACFunctionGroup DAC driver function
//! @{ \ingroup DACGroup

extern __X int  * DAC_LChannelBuffer1;		/**<左聲道Buffer1的起始位址，Size必需大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型態轉換即可 */
extern __X int  * DAC_RChannelBuffer1;		/**<右聲道Buffer1的起始位址，Size必需大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型態轉換即可 ，若所播放的音源為Mono，可與LChannelBuffer1相同*/
extern __X int  * DAC_LChannelBuffer2;		/**<左聲道Buffer2的起始位址，Size必需大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型態轉換即可 */
extern __X int  * DAC_RChannelBuffer2;		/**<右聲道Buffer2的起始位址，Size必需大於等於frame size，可以指到XRAM或YRAM，若指到YRAM，以C的型態轉換即可 ，若所播放的音源為Mono，可與LChannelBuffer2相同*/
extern unsigned int DAC_FrameSize;				/**<PCM資料Frame size，以Word為單位*/
extern tDAC_SampleRate DAC_SampleRate;		/**<Sampleing rate設定, 請參考 \ref tDAC_SampleRate */

/*!
DAC 的初始化

@param LChannelDTUNumber 左聲道所使用的DTU, 請參考 \ref tKBDM9_DTUNumber
@param RChannelDTUNumber 右聲道所使用的DTU, 請參考 \ref tKBDM9_DTUNumber
@param FillBufferFunc1 DAC buffer若空了, 則會進入此function pointer所指到的function內, 此function一定要宣告.
@param FillBufferFunc2 DAC buffer若空了且FillBufferFunc1所指到的function已經執行完畢，則會再進入此function pointer所指到的function內, 此function可以不用宣告, 若不使用, 請設定成0. 
@return 請參考 \ref tDAC_Error
*/
extern tDAC_Error DAC_Init(
								tKBDM9_DTUNumber LChannelDTUNumber,	
								tKBDM9_DTUNumber RChannelDTUNumber,	
								void(*FillBufferFunc1)(void),
								void(*FillBufferFunc2)(void));

/*!
使用耳機輸出聲音

*/
extern void DAC_Enable_HeadPhone(void);

/*!
關閉耳機輸出

*/
extern void DAC_Disable_HeadPhone(void);

/*!
使用喇叭輸出聲音

*/
extern void DAC_Enable_Speaker(void);

/*!
關閉喇叭輸出

*/
extern void DAC_Disable_Speaker(void);
								
/*!
耳機輸出的音量調整 \n
總共有32段音量可調整

@param Volume  必須為0 ~ 31的數值, 0表示靜音, 31為最大聲 

*/								
extern void DAC_HeadPhone_Volume(unsigned int Volume);								

/*!
喇叭輸出的音量調整 \n
總共有16段音量可調整

@param Volume  必須為0 ~ 15的數值, 0表示靜音, 15為最大聲 

*/								
extern void DAC_Speaker_Volume(unsigned int Volume);


/*!
啟動DAC
@return 請參考 \ref tDAC_Error
*/
extern tDAC_Error DAC_Go(void);
								
/*!
判斷buffer1是否空了
@return 0，Buffer1還沒空
@return 1，Buffer1已經空了，必需填入要播放的PCM資料
*/
extern int DAC_Buffer1Empty(void);  	

/*!
判斷buffer2是否空了
@return 0，Buffer2還沒空
@return 1，Buffer2已經空了，必需填入要播放的PCM資料
*/
extern int DAC_Buffer2Empty(void);	

/*!
填完Buffer1的資料後，必需執行此function
*/
extern void DAC_FillBuffer1(void); 	

/*!
填完Buffer2的資料後，必需執行此function
*/
extern void DAC_FillBuffer2(void); 	

/*!
暫停DAC的播放
*/
extern void DAC_Pause(void);

/*!
解除DAC播放暫停的狀態，繼續播放
*/
extern void DAC_Continue(void);

/*!
DAC播放靜音
*/
extern void DAC_Mute(void);

/*!
解除DAC靜音狀態
*/
extern void DAC_Unmute(void);

/*!
播放結束時，執行此function可將輸出準位拉為0，*/
extern void DAC_FadeOut(void);	

/*!
請在填完buffer之後，DAC_Go之前執行此function會將一開始的值拉到要播放的起始資料值
*/
extern void DAC_FadeIn(void);	

//! @}

#endif  /*KBDM9_DAC */

/*!@{\ingroup KBDM9_AudioInOutGroup

	\section intro Introduction
 	
 	Audio ADC Driver Interface\n
 	-       version 0.1\n\n
	1、可選擇 Microphone 或 Line-In 輸入, 僅提供單聲道模式 \n
	2、支援 8, 9.6, 11.025, 12, 16, 22.05, 24, 32, 44.1, 48, 及 96 KHz 16bit 取樣。

   \defgroup KBDM9_AudioInGroup KBDM9 Audio In Introduction

@}*/



#ifndef KBDM9_Audio_ADC_H
#define KBDM9_Audio_ADC_H




//!\defgroup AuadcErrGroup Error Flag Lists
//!@{\ingroup KBDM9_AudioInGroup
/*!
Audio ADC function 錯誤訊息
*/
typedef enum {
	AUADC_NO_ERR	         		 =	0,		/**<沒有錯誤*/
	AUADC_SampleRate_SETUP_ERR =	-1,		/**<Sampling rate設定錯誤*/
	AUADC_DTU_SETUP_ERR		     =	-2,		/**<DTU number 設定錯誤*/
	AUADC_OUT_BUF_SETUP_ERR		 =	-3,		/**<沒有設定output buffer*/
	AUADC_BUF_SIZE_SETUP_ERR	 =	-4,		/**<Output buffer設定超過0x1fff的大小*/
	AUADC_CALL_BACK_SETUP_ERR	 =	-5,		/**<沒有設定Call back function*/
	AUADC_PARAMETER_SETUP_ERR	 =	-6,		/**<參數設定錯誤*/
	AUADC_NO_INIT_ERR	     =	-7	    	/**<Audio ADC未做初始化*/
} tAUADC_Err;
//! @}

/**** Initial code *****/

//!\defgroup AuadcSetGroup Audio ADC Options
//!@{\ingroup KBDM9_AudioInGroup
/*!
信號擷取採樣頻率ID*/
typedef enum {
	AUADC_SAMPLE_RATE_96K			=	0,				/**< Audio ADC sampling rate設定為96KHz*/	
	AUADC_SAMPLE_RATE_48K			=	1,				/**< Audio ADC sampling rate設定為48KHz*/
	AUADC_SAMPLE_RATE_44P1K		=	2,				/**< Audio ADC sampling rate設定為44.1KHz*/
	AUADC_SAMPLE_RATE_32K			=	3,				/**< Audio ADC sampling rate設定為32KHz*/
	AUADC_SAMPLE_RATE_24K			=	4,				/**< Audio ADC sampling rate設定為24KHz*/
	AUADC_SAMPLE_RATE_22P05K	=	5,				/**< Audio ADC sampling rate設定為22.05KHz*/
	AUADC_SAMPLE_RATE_16K			=	6,				/**< Audio ADC sampling rate設定為16KHz*/
	AUADC_SAMPLE_RATE_12K			=	7,				/**< Audio ADC sampling rate設定為12KHz*/
	AUADC_SAMPLE_RATE_11P025K	=	8,				/**< Audio ADC sampling rate設定為11.025KHz*/
	AUADC_SAMPLE_RATE_9P6K  	=	9,				/**< Audio ADC sampling rate設定為9.6KHz*/	
	AUADC_SAMPLE_RATE_8K			=	10				/**< Audio ADC sampling rate設定為8KHz*/
} tAUADC_SmapleRate;                         
                                             
/*! Audio ADC Input Gain */                   
typedef enum{                                
  AUADC_Gain_0dB     = 0,           /**< Audio ADC Input Gain 設定為 0dB    */	          
  AUADC_Gain_1P5dB   = 1,           /**< Audio ADC Input Gain 設定為 1.5dB  */    
  AUADC_Gain_3dB     = 2, 	        /**< Audio ADC Input Gain 設定為 3db    */  
  AUADC_Gain_4P5dB   = 3,           /**< Audio ADC Input Gain 設定為 4.5dB  */
  AUADC_Gain_6dB     = 4,           /**< Audio ADC Input Gain 設定為 6dB    */
  AUADC_Gain_7P5dB   = 5,           /**< Audio ADC Input Gain 設定為 7.5dB  */
  AUADC_Gain_9dB     = 6,           /**< Audio ADC Input Gain 設定為 9dB    */
  AUADC_Gain_10P5dB  = 7,           /**< Audio ADC Input Gain 設定為 10.5dB */
  AUADC_Gain_12dB    = 8, 	        /**< Audio ADC Input Gain 設定為 12dB   */
  AUADC_Gain_13P5dB  = 9,           /**< Audio ADC Input Gain 設定為 13.5dB */
  AUADC_Gain_15dB    = 10,          /**< Audio ADC Input Gain 設定為 15dB   */
  AUADC_Gain_16P5dB  = 11,          /**< Audio ADC Input Gain 設定為 16.5dB */
  AUADC_Gain_18dB    = 12, 	        /**< Audio ADC Input Gain 設定為 18dB   */
  AUADC_Gain_19P5dB  = 13,          /**< Audio ADC Input Gain 設定為 19.5dB */
  AUADC_Gain_21dB    = 14,          /**< Audio ADC Input Gain 設定為 21dB   */
  AUADC_Gain_22P5dB  = 15    	      /**< Audio ADC Input Gain 設定為 22.5dB */      
}tAUADC_Input_Gain;

/*! Audio ADC Microphone Gain */
typedef enum{
  MIC_Gain_0dB       = 0,          /**< Audio ADC Microphone Gain 設定為 0dB    */	 
  MIC_Gain_20dB      = 1           /**< Audio ADC Microphone Gain 設定為 20dB  */ 
}tAUADC_MIC_Gain;

/*! Audio ADC Input Path */

typedef enum{
  MIC_In       = 0,          /**< 選擇 Microphone  */	 
  Line_In      = 1           /**< 選擇 Line In  */ 
}tAUADC_In_Path;

/*!
Audio ADC 完成填滿buffer的call-back function
@param buf	=> 存放Audio ADC轉換的資料
@param buf_size	=>Buffer的大小，以Word為單位。 
*/
typedef void (*tAUADC_BufNotify)(__X int* buf, unsigned int buf_size);

//! @}

//!\defgroup AudioInGroup Audio ADC Functions
//!@{ \ingroup KBDM9_AudioInGroup
/*!
Audio ADC Device初始化 function.
@param Input_Type  => 選擇Microphone 或 Line In 輸入, 請參考 \ref tAUADC_In_Path
@param DTUNumber	 =>	設定DTU number 請參考 \ref tKBDM9_DTUNumber
@param SampleRate  => Audio ADC 的 sampling rate, 請參考 \ref tAUADC_SmapleRate 
@param notify		   =>	完成填滿buffer的call-back function

@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
@note 因為Audio ADC約需1秒鐘的穩定時間, 所以執行AUADC_Init()需1秒的時間. 
*/
tAUADC_Err AUADC_Init(tAUADC_In_Path Input_Type, tKBDM9_DTUNumber DTUNumber,	tAUADC_SmapleRate SampleRate, tAUADC_BufNotify notify);	

/**** Setup code *****/


/*!
輸出buffer的設定, 須有兩個buffer輪流接收ADC轉換出來的資料
@param	OutBuf1		 =>	第一個output buffer的pointer
@param	OutBuf2		 =>	第二個output buffer的pointer
@param	OutBufSize =>	output buffer 的大小, 單位為 word

@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
*/
tAUADC_Err AUADC_Set_Buffer(__X int *OutBuf1, __X int	*OutBuf2, unsigned int OutBufSize);						



/*! 
設定 Audio ADC Input Gain 
@param gain 	=>	請參考 \ref tAUADC_Input_Gain
@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
@note 因Audio ADC內部的放大器在變動gain時, 需要約0.02秒才能穩定, 在未達穩定時聲音可能會不正常, 所以請勿在錄音過程中改變gain的值. 
*/
tAUADC_Err AUADC_InputGain(tAUADC_Input_Gain gain);


/*!
設定 Audio ADC Microphone Gain 
@param gain		請參考 \ref tAUADC_MIC_Gain                            
@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
@note 因Audio ADC內部的放大器在變動gain時, 需要約0.02秒才能穩定, 在未達穩定時聲音可能會不正常, 所以請勿在錄音過程中改變gain的值
*/
tAUADC_Err AUADC_MicGain(tAUADC_MIC_Gain gain);


/**** Running code *****/
/*!
Audio ADC 開始工作 function
@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
*/
tAUADC_Err AUADC_Start(void);


/*!
Audio ADC 停止工作 function
@return  0為無錯誤, 其他回傳值請參考 \ref tAUADC_Err
*/
tAUADC_Err AUADC_Stop(void);
//!@}

#endif      /* KBDM9_Audio_ADC_H */


#ifndef KBDM9_Mixer_H
#define KBDM9_Mixer_H


/*!@{\ingroup KBDM9_AudioInOutGroup

	\section intro Introduction
 	
 	Hardware Mixer Driver Interface\n
 	-       version 0.1\n\n
	1、可使用 Line-In 到硬體混音器(Mixer)的通道, 使Line-In輸入端進來的聲音可以不經由Audio ADC轉換, 直接經過混音器後, 由耳機或喇吧輸出聲音. \n
	2、可同時啟動 Audio ADC錄製Line-In輸入的聲音。
	3、可選擇是否要與Audio DAC的輸出做混音。  

   \defgroup KBDM9_MixerGroup KBDM9 Audio In Introduction

@}*/


//!\defgroup MixerSetGroup Error Flag Lists
//!@{\ingroup KBDM9_MixerGroup
/*! 硬體混音器的選項
*/
typedef enum {
	No_Mixed_To_DAC	=	0,				/**<不與DAC的輸出做混音 */	
	Mixed_To_DAC = 1            /**<與DAC的輸出做混音 */
} tMixer_Option;

/*! 選擇混音之後的聲音輸出方式
*/
typedef enum {	
	Head_Phone_Out = 0,    /**<耳機輸出 */
	Speaker_Out=	1				 /**<喇吧輸出 */	
} tMixer_Out;
//!@} 

//!\defgroup MixerGroup Hardware Mixer Functions
//!@{ \ingroup KBDM9_MixerGroup 
/*!
啟動 Line-In 到硬體混音器(Mixer)的通道, 使Line-In輸入端進來的聲音可以不經由Audio ADC轉換, \n
直接經過混音器後, 由耳機或喇吧輸出聲音.
@param Mix_Option 選擇是否要與DAC的輸出做混音, 設定值請參考 \ref tMixer_Option
@param Out_type 選擇輸出的方式, 設定值請參考 \ref tMixer_Out

@note 
-如果要將Line-In輸入的聲音錄音及儲存, 請在執行此功能之前, 參考 \ref KBDM9_AudioInGroup的說明設定Audio ADC.
@note
-如果要與DAC輸出做混音, 請在執行此功能之前, 參考 \ref DACGroup的說明設定DAC. 
*/
extern void Line_In_To_Mixer_Enable(tMixer_Option Mix_Option, tMixer_Out Out_type);

/*!
關閉 Line-In 到硬體混音器(Mixer)的通道
*/
extern void Line_In_To_Mixer_Disable(void);

/*!
Line-In輸入到Mixer的音量 \n
總共有32段音量可調整

@param volume  必須為0 ~ 31的數值, 0為靜音, 31為最大聲 

*/
extern void Line_In_To_Mixer_Volume(unsigned int volume);

/*!
DAC輸出到Mixer的音量 \n
總共有16段音量可調整

@param volume  必須為0 ~ 15的數值, 0為靜音, 15為最大聲 

*/
extern void DAC_To_Mixer_Volume(unsigned int volume);

//!@}

#endif  /*KBDM9_Mixer_H */

 
//********************************************************************************************************

/*!@{
    \defgroup KBDM9_SoftWareIPGroup KBDM9 Software IP API Functions   

 	KBDM9與週邊無關，純軟體運算之IP介紹，各類的壓縮，解壓縮應用 \n
 	主要有：包含MP3 Decode等\n

@}*/


/*!@{\ingroup KBDM9_SoftWareIPGroup

 	MP3 Decoder Interface\n
 	-       version 1.0\n\n
	-
	KB MP3 IP provides the ability for DMCU to play MP3 music. The MP3 decoder supports the MPEG 1/2/2.5
	Layer 3 , MPEG 1 Layer2 and VBR format. The input stream and the audio output all can be controlled by users
	program. So it can be used on many application fields such as mp3 player, wireless player, ELA, ED, TV, CD
	MP3…, etc. There are many other software IPs could be integrated with the MP3 IP to form applications quickly.

   \defgroup Mp3DecoderGroup MP3 Decoder Introduction

@}*/

#ifndef _MP3_H_
#define _MP3_H_


//!@{\ingroup Mp3DecoderGroup
/*!
The global variables could be referenced in application assembler program by access the name with ‘_’ prefixed.
Using C program to access it need not with ‘_’ prefix. For example, to access mp3VolumeCtrl variable, in assembly
language user can use “_mp3VolumeCtrl” as a direct memory address. In C program, it should use “mp3VolumeCtrl” as an
external variable.Application should not modify these variables directly. These variables are used for passing 
the status of mp3 decoding to applications.
\defgroup Mp3VarGroup MP3 Global Variables
*/
//!@}

//!\defgroup Mp3VarListGroup MP3 Global Variables List
//!@{\ingroup Mp3VarGroup

/*!
The sampling rate be supported by mp3 decoder as follows.
*/
typedef enum{
	SMP_8K   		=	1,		/**< Sampling rate = 8Khz */
	SMP_16K  		=	2,		/**< Sampling rate = 16Khz */
	SMP_32K  		=	3,		/**< Sampling rate = 32Khz */
	SMP_11P025K  	=	4,		/**< Sampling rate = 11.025Khz */
	SMP_22P05K  	=	5,		/**< Sampling rate = 22.05Khz */
	SMP_44P1K  		=	6,		/**< Sampling rate = 44.1Khz */
	SMP_12K  		=	7,		/**< Sampling rate = 12Khz */	
	SMP_24K  		=	8,		/**< Sampling rate = 24Khz */
	SMP_48K  		=	9		/**< Sampling rate = 48Khz */
}tMp3SampleRateID;

/*!	SmpRateID could be checked to see what sampling rate is the mp3 decoder output. */
extern tMp3SampleRateID mp3SmpID;

/*!Decoded frame counter*/
extern unsigned long __X mp3FrameCount;

/*!Decoded bit stream word counter*/
extern unsigned long __X mp3WordCount;

/*!
 Information 1 \n
\b	bit0: Protection \n
\b	bit1..2: Layer \n
\b	bit3: ID \n
bit 4..7: Reserved.
*/
extern unsigned char __X mp3HeaderInfo1;

/*!
Information 2 						\n
\b bit0..1: emphasis 				\n
\b bit2: original 					\n
\b bit3: copyright					\n
\b bit4..5: mode extension			\n
\b bit6..7: mode					\n
\b bit8: private bit				\n
\b bit9: padding bit				\n
\b bit10..11: sampling frequency	\n
\b bit12..15: bit rate				\n
*/
extern unsigned int __X mp3HeaderInfo2;

/*!
 Number of ancillary bits of current frame
*/
extern unsigned int __X mp3NumAncillaryBit;

/*!
Ancillary bits of current frame
*/
extern unsigned char * __X mp3AncillaryData;

/*!
\b bit0..3: Right Channel Volume (16 levels) \n
\b bit4..7: Left Channel Volume (16 levels)
*/
extern unsigned char __X mp3VolumeCtrl;

/*!
Bit stream buffer (BS_BUF) tail pointer. User
should fill the bit stream to the address
pointed by this pointer and then update this
pointer
*/
extern unsigned int __X mp3Buf;

/*!
Report the bit rate of the decoded bit stream \n
Unsigned integer represent the bit rate (KHz) of the decoding bit stream
*/
extern unsigned int __X mp3Bitrate;

/*!
Decode bit stream error count \n
Use this count to know whether bit stream has error
*/
extern unsigned int __X mp3ErrorCnt;

/*!
Flag on decoding \n
bit 0~9: Reserved, do not modify it \n
\b bit10: 1:MPEG2/2.5, 0:MPEG1 \n
\b bit11: stereo2 \n
\b bit12: Reserved, do not modify it \n
\b bit13: Decoder is proceeding \n
\b bit14: 1:Stereo, 0:Mono \n
\b bit15: fsGot (header coded and got sampling \n
rate)
*/
extern __DPRAM int  mp3Flag;

/*!
 rst=0x0 when mp3_rst() \n
\b bit15: free format detection, 要求偵測 nSlots of free format \n
\b bit14: (1)free format mp3 file found and nSlots calculate OK (1)otherwise \n
 bit13: reserved \n
\b bit12: SYNCWORD_FOUND, (1)SYNCWORD已找到, 但該frame之bit stream尚未讀完 (0)需重新找SYNC_WORD \n
\b bit11: preflagCur, preflag of channel which is under processing \n
\b bit10: scDirty, 第一次進入DEQUANTIZATION_4時未必見得會碰到boundary, 因此scVar[]可能不會更新, 靠此訊號通知DEQUANTIZATION_4更新scVar[] \n
\b bit9: ms_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit8:  i_stereo, {1=on, 0=off} if bit14==1, undefined if bit14==0 \n
\b bit7: WORKING_CH, (0)正處理ch0, (1)正處理ch1, valid only in BACK_END()/POST_PROCESSING() \n
\b bit6: bsInitOK, bit stream struct (0)尚未初始化 (1)初始化完畢 \n
\b bit5: NEXT_GR, (0)下一個處理gr0, (1)下一個處理gr1 \n
\b bit4: bAUX1, temporarily bit storage \n
 bit3..0: reserved
*/
extern __DPRAM int  mp3Flag2;


/*!
mp3EQgain[sb0..31]: UNS0.16, 0x8000=0.5 \n
synthesis 時再將音量 double
*/															
extern __Y unsigned int mp3EQgain[];

/*!
Report the stereo or mono of the decoded bit stream \n
1=>Stereo \n
0=>Mono
*/
extern __X int mp3StereoMode;


/*!
mp3 decode error status \n
\b bit0: LAYER_ERR    , not layer III or II file \n
\b bit1: CRC_ERR      , crc check fail \n
\b bit2: BAD_BYTES_ERR, bad_bytes_to_discard \n
\b bit3: HUFF_DEC_ERR, HUFFMAN_DECODER_ERROR \n
\b bit4: BS_NOT_ENOUGH, _mp3_bs_buf[]中的資料不足一個frame \n
bit15~5: reserved
*/
extern unsigned int mp3ErrorStatus; 		 

//!@}

//!\defgroup Mp3DecFunGroup MP3 Decoder function List
//!@{\ingroup Mp3DecoderGroup
/*!
Initial MP3 decoder IP and set the five buffers pointers. This function should be the first function to initialize MP3 IP.\n
In the normal concept, the first four buffers are mainly used for stereo dual buffer operation.
The first stereo buffer is so called PLAY_BUF_L1 and PLAY_BUF_R1. The second stereo
buffer is PLAY_BUF_L2 and PLAY_BUF_R2. These two stereo may switch each other during
decode and play processing. DMCU MP3 IP uses one more buffer named HEAP buffer to do
acceleration. The HEAP buffer may change with one of the four buffers after a frame decoded.
So application should call mp3_GetPlayBufPtr() to get the changed play buffer pointers to play the correct output sound.\n

The bit stream buffer should be defined as a circular buffer. There is a directive in assembly
language “.CIRCBUF”, application should use this directive to define the bit stream buffer. \n
For example,\n
.area MP3_RAM2(XRAM) \n
_bitstream_buf:: .CIRCBUF 722\n
This example defines a 722 words circular buffer for the bit stream buffer. Please note that the
circular buffer declaration should with a dedicated “.area” definition. Don’t declare it with
other variables in the same area.\n
@param pu32MemPool1 =>Buffer Size is 576 words. The buffer is for decoding buffer left channel .
@param pu32MemPool2 =>Buffer Size is 576 words. The buffer is for decoding buffer right channel.
@param pu32MemPool3 =>Buffer Size is 576 words. The buffer is for decoding buffer left channel .
@param pu32MemPool4 =>Buffer Size is 576 words. The buffer is for decoding buffer right channel.
@param HEAP => Buffer Size is 576 words. This buffer is used for store the temporary decoded data or final PCM data.
@param bs_buf => The input bit stream buffer, Buffer size could be defined by user for different application requirement.
@param bs_len=> The bit stream buffer size counted in words.

*/
extern void mp3_init(	int *pu32MemPool1 , int *pu32MemPool2 , int *pu32MemPool3 , int *pu32MemPool4, int *HEAP, int *bs_buf, int bs_len);
					
/*!
Reset the mp3 decoder, flush the bit stream buffer and temporary decoded data. When an
application needs to play another song, it should call this function to let mp3 decoder seek the
sync word to decide new sampling rate.
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_rst(void);

/*!
To know the empty word numbers in the BS_BUF, application should call this function to get
the return value (word number). The start address pointer of empty BS_BUF is stored in
_mp3Buf (global variable).\n
The BS_BUF is a circular buffer. The size is 722 words in default (could be set by user). To fill
the bit stream data into it, application should set the circular buffer pointer and size (counted in
byte) into X index registers.The operation that fill bit stream to BS_BUF will automatically scroll to the start address of
BS_BUF when the X0 exceed the end of BS_BUF. \n
Please note that in the new MP3 IP, it is not necessary to fill all the empty word numbers into BS_BUF in one time. The
application can separate two or more times to fill data into BS_BUF. After each filling, the ‘_mp3Buf’ pointer should 
be updated to let mp3dec function know the last data pointer.
@return: Return the length of empty BS_BUF word numbers that application can fill bit stream
data into it. The returned value is stored in R0. The unit is counted by word number.

*/
extern unsigned int mp3QueryBuf(void);

/*!
Decode one granual (1/2 frame ) data to play buffer. This function is the mainly routine to do
mp3 decoding. It will parse the bit stream header and recognize the format of the bit stream
include data rate, mono/stereo mode and the sampling rate, etc. \n
After decoding one granual of data, application should get the play buffer pointer by calling
mp3_GetPlayBufPtr().\n

Mp3dec() function can be triggered to execute from interrupt service routine to ensure the
decode timing. But it can’t be called directly in interrupt service routine. There is a trigger to
execute method demo in example code. Please refer to it. Note that an important issue is that
mp3dec() can’t be re-entrant\n
@return  0 =>decode fail \n
		1 => decode OK
*/
extern int mp3dec(void);

/*!
Get the current play buffer that the decoded PCM data placed there. Because MP3 IP will auto
change the play buffer pointers, application should call this function to get the updated play
buffer address.
@param LeftBuf => the buffer that contains the PCM data of left channel
@param RightBuf => the buffer that contains the PCM data of right channel
*/
extern void mp3_GetPlayBufPtr(void *LeftBuf, void *RightBuf);

/*!
Register a user call back function to handle the case when some error occurred in mp3
decoding. The error case has two types. The error case is mainly come from bit stream error. To
process this situation, the user call back function can play the last frame data or play a silence
frame or not doing any play. The call back function can also display some error message on
LCD to tell user what happening. The global variable “mp3ErrorStatus” will set the
corresponding bit to let user distinguish the error type.
@sa mp3ErrorStatus
*/
extern void mp3_SetErrorFunc(void (*CallBackFun)(void));		// call back function: For handling error case

/*!
Set the equalizer attributes to the MP3 decoder. The first parameter is the equalizer attributes
array. It should be an unsigned integer array, the element number of the array can be set by the
parameter EqNo. The number should between 1~32. There are 32 frequency bands in the mp3
decoder. The default values of the equalizer attributes are 0x8000, it represent x1.000 to the
frequency bands. The maximum value settable is 0xFFFF, it means the frequency band will be
emphasize for about x2.000. The minimum value is 0x0000, it means zero volume of the
frequency band.\n
Each frequency band width is calculated by dividing the total bandwidth of the mp3 music with
32. For example, a 44.1KHz mp3 music’s bandwidth may be 22.05KHz, so each band
frequency bandwidth is about (22.05 / 32)KHz.

@param EQList=> equalizer attributes array
@param EqNo =>The number should between 1~32. There are 32 frequency bands in the mp3 decoder.

*/
extern void mp3_SetEQ(unsigned int *EQList, int EqNo);	

/*!
Enable the mp3 equalizer calculation.
@note \b Code \b Address \b Name => \b MP3DEC_TEMP2_CODE
*/
extern void mp3_EnableEQ(void);

/*!
Disable the mp3 equalizer calculation.
@note \b Code \b Address \b Name => \b MP3DEC_TEMP2_CODE
*/
extern void mp3_DisableEQ(void);


/*!
Befor executing mp3dec(), this function must be executed.
*/
extern void mp3Clear(void);

/*!
This function will setup the pointer of the buffer to put the spectrums data given from mp3dec()
function. User can get the information to display spectrums from the buffer.
@param spectrum_ptr => the pointer of a buffer that is used to store the spectrums. Its size must be 32 words.
@note \b Code \b Address \b Name => \b MP3DEC_INIT_CODE
*/
extern void mp3_spectrum_data(unsigned int * spectrum_ptr);

#endif
//!@}

#include "IMA_ADPCM.h"

//=============================================================================

/*!@{
    \defgroup KBDM9_SDK_Group KBDM9 Software Development Kits   

	提供API整合產品用所需的load code function。\n
	區分為以下幾個部份。\n
	- 1、Common load code : 由Boot code將所需的code放到系統保留區，供AP作切換project用。\n
	- 2、SDK Load code : 每個project內載code用的function，例如initiation的code，執行完後，其空間可空出來下載其他的code，此function必需搭配addhereto的作法，有關 addhereto的作法請參考dogstar的help說明\n
	- 3、FAT : 可掛各類的儲存裝置，例如SD卡、NAND-FLASH\n

@}*/

//***************CommonLoadCode*********************************************

//!\defgroup KBDM9_CommonLoadCodeGroup KBDM9 Common Load Code Functions 
//!@{\ingroup KBDM9_SDK_Group

/*!
切換project時，將指定project的code載入IRAM，載入的位置與長度是根據Dogstar Builder所建的表。\n
此fucntion不會將XRAM, YRAM與IRAM清空。

@param CodeID  => 所要載入的code的辨識碼, 允許的數值為 0至62 以及 64至126.
*/
void CommonLoadCode(unsigned int CodeID);


/*!
讀取NAND-Flash, SD card或SPI NOR-Flash保留區內的Data的儲存位址之相關資訊，\n

@param DataID : 此Data的編號
@param DataStartSector : data存放在保留區內的起始sector資訊，並放置於此pointer所指到的位置\n
@param DataLength :  data的長度資訊，並放置於此pointer所指到的位置，以bytes為單位。
*/
extern void GetDataIdInfo(	unsigned int DataID, __X unsigned long * DataStartSector, __X unsigned long * DataLength);
//!@}

void SD_CommonLoadCode(unsigned int CodeID);

void NFTL_CommonLoadCode(unsigned int CodeID);

void SPI_CommonLoadCode(unsigned int CodeID);

//**************SDK load Code**************************************************
//! \defgroup SDKLoadCodeGroup SDK load code function 
//! @{\ingroup KBDM9_SDK_Group	

/*!
SDKLoadCode所要載入的 image 之宣告
*/
#define ImageDeclareType(ImageName) extern __X unsigned long ImageName##_begin; \
extern __X int * ImageName##_anchor; 		\
extern __X unsigned long ImageName##_size;	\
extern __X unsigned long ImageName##_CInitFun

/*!
SDKLoadCode，將VIRTUAL_SPACE內的image，載入到IRAM要執行的位置\n
例如，某example的lcf內容部份截取如下：\n
	IRAM 0x10000 0xD000 \n
	{\n
		SPACE_IRAM 0x0 0x8000 : interrupt 0x0 0x100 ,image;\n
		INIT_CODE 0x8000 0x5000 : image ,adhereto VIRTUAL_SPACE;\n
		PLAY_MP3_CODE 0x8000 0x5000 : image ,adhereto VIRTUAL_SPACE;\n
	}\n
	EXTMEM3 0x500000 0x100000 : ws 3\n
	{\n
		VIRTUAL_SPACE 0x0 0x100000 : image;\n
	}\n
	其中INIT_CODE與PLAY_MP3_CODE就是ImageName，彼此是overlap，由SDKLoadCode(INIT_CODE);\n
	先將initiation的code載到IRAM的0x8000，執行完後，再執行SDKLoadCode(PLAY_MP3_CODE);\n
	將PLAY_MP3_CODE載到同一個位置，執行mp3 play，這就是SDKLoadCode()的作法來使用IRAM可以overlap code的特性。\n
	
@param ImageName : addhereto的image name ，所要載入的image，載入的位置由linker control file來編輯
@note  1、SPACE_IRAM的起始位置不能改變，必需固定在0。
@note  2、程式的入口，也就是main()也必需擺在SPACE_IRAM。
*/									
#define SDKLoadCode(ImageName) 		LoadCode(&##ImageName##_begin , &##ImageName##_CInitFun)

/*!
SDKLoadCode的執行的狀態
*/
typedef enum{
	SDKLOADCODE_NO_ERR		=	   	0,  /*!< no error */
	SDKLOADCODE_PARAM_ERR   =	   	-1, /*!< invalid parameter */
	SDKLOADCODE_DATA_ERR    =   	-5 	/*!< the data maybe incorrect */
}tSDKLoadCodeStatus;


/*!
SDKLoadCode的初始化
@return : 請參考 \ref tSDKLoadCodeStatus
*/
extern int SDKLoadCodInit(void);		

/*!
SDKLoadCode所使用的function
*/
tSDKLoadCodeStatus LoadCode(	__X unsigned long * ImageName_begin, __X unsigned long * pInitFun);

//!@}

#include "fs.h"
 
#endif  /* _KMDM9_DDK_ */
