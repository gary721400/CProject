#ifndef _CONF_KB_H_
#define _CONF_KB_H_

//_____ G E N E R I C   C O N F I G U R A T I O N __________________________
#include "conf_oid.h"

#define MASTER_0				'3'
#define MASTER_1				'0'
#define MASTER_2				'l'
#define MASTER_3				' '
#define MASTER_4				' '
#define MASTER_5				' '
#define MASTER_6				' '
#define MASTER_7				' '

#define SYS_LAB_1                  "30L__SD1"
#define SYS_LAB_2                  "30L__SD2"

//#define OID_VERSION                 OID1
#define OID_VERSION                 OID2

#if( OID_VERSION == OID2 )
#define	T01OID						(ENABLE)
//#define	T01OID						(DISABLE)

#define	T01OID_CAB						(ENABLE)
//#define	T01OID_CAB					(DISABLE)

//#define DPR3							(ENABLE)
#define DPR3							(DISABLE)

#define WAIYAN_OID2							(ENABLE)
//#define WAIYAN_OID2							(DISABLE)

#endif  //#if( OID_VERSION == OID2 )



#define WAV_SPEAKER_VOLUME_INCREASE			12  //14
#define MP3_SPEAKER_VOLUME_INCREASE			8  //10

//#define USE_AUADO

#define DISK_FULL_CHECK
//
#define RP_RECORD
//
//#define ALL_NEWS_KEY

//#define NF_DISK						
#define SD_DISK						

//#define UNCOIDE_FORMAT     
#define ASCII_FORMAT	  

//#define NIEQING_DIANCHI
//#define LI_DIANCHI


#define SAVE_OID_CAL_ADDR		(1020*1024UL)//1020K//(1036288UL) //1012K
//#define SAVE_OID_CAL_ADDR		(1040384UL)
//#define SAVE_OID_CAL_ADDR		(2024UL)
//#define NIEQING_DIANCHI_DIANYA


#define LI_DIANCHI_DIANYA			(ENABLE)
//#define NIEQING_DIANCHI_DIANYA	(DISABLE)


#define QUICKLY_FLASH				(ENABLE)
//#define QUICKLY_FLASH				(DISABLE)




#define TISHI_TIME			 	 (5)
#define AUTO_CLOSE_TIME			 (10)


#define HARD_VERSION              '1'

#if ( OID_VERSION == OID2 )
#define OID_TYPE                  '2'
#else 
#define OID_TYPE                  '1'
#endif

#define MAJOR_FW_VERSION          '1'
#define MINOR_FW_VERSION          '0'

#define USB_DEVICE_MS_VENDOR			 "30L  110"


#define USE_SD1		1	//  1:use SD1	0:no use SD2
#define USE_SD2		0	//  1:use SD1	0:no use SD2


#define CHECKT_SD_DET							(ENABLE)
//#define CHECKT_SD_DET							(DISABLE)

#define LI_DIANCHI_CHONGDIAN					(ENABLE)
//#define LI_DIANCHI_CHONGDIAN					(DISABLE)

#define INDEX_LENGTH_FOUTH						(ENABLE)
//#define  INDEX_LENGTH_FOUTH						(DISABLE)

#define FIRST_PROJECT	C_OID
#define USB_PROJECT		C_USB_D

//=============================================================================
// OPTION

/*
//#define USE_SD              1           //  1:use SD            0:no use SD
#define USE_SD              0           //  1:use SD            0:no use SD
#define USE_NF              1           //  1:use NAND-FLASH    0:no use NADN-FLASH
//#define USE_NF              0           //  1:use NAND-FLASH    0:no use NADN-FLASH
*/

#endif  // _CONF_KB_H_
