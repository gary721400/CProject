/*******************************************************************************
*
*   Storbase.h - DMCU Storage Storage System Base Header
*
*   Including: Flags Checking Routings, Error Codes
*
*******************************************************************************/

#ifndef __STORBASE_H
#define __STORBASE_H

//#define KBMPT03
//#define _FAT32_ASM




//SEPERATE those bits for Signal Safe
void SetFlashBusy(void);
//          { INT8U nftl_oserr;\
			//OSSemPend(nftl_semaphore, 0, &nftl_oserr); }
void ClearFlashBusy(void);//        OSSemPost(nftl_semaphore)
int  isFlashBusy(void);  	//		0


#define Diable_Interrupts_Coming()  \
        __asm {                     \
            PUSH SOCR               \
            BCLR SOCR.15            \
            NOP                     \
            NOP\
            NOP\
            NOP  }

#define Enable_Interrupts_Coming()  __asm("POP SOCR")

/******************************
* NFTL Defition
******************************/
#define SECTOR_SIZE (512)

/******************************
* NFTL Error Code
******************************/
#define NFTL_NO_DATA        (1)
#define NFTL_NO_ERR         (0)
#define NFTL_PARAM_ERR      (-1)
#define NFTL_MEDIA_ERR      (-2)
#define NFTL_SYSTEM_ERR     (-3)
#define NFTL_NO_SPACE       (-4)
#define NFTL_DATA_ERR       (-5)
#define NFTL_CACHE_ERR      (-6)
#define NFTL_MEMORY_ERR     (-10)

#define NAND_MAKER_CODE1    (0xEC)
#define NAND_MAKER_CODE2    (0x20) //ST
#define NAND_MAKER_CODE3    (0xAD) //hynix

/******************************
* ECC Control Number
******************************/
#define ECC_MAX_RETRY       (3)

/******************************
* ECC Error Code
******************************/
#define ECC_FIXED_ONE_BIT   (2)
#define ECC_NO_ERROR        (0)
#define ECC_UNRECORRECTABLE (-7)



//for __STORBASE_H
#endif
