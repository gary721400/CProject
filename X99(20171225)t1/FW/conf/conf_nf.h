//! @file conf_nf.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! This file contains the possible external configuration of the Nand Flash interface
//!
//! @version 1.25 snd3-dvk-1_9_5 $Id: conf_nf.h,v 1.25 2007/07/24 11:34:45 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _CONF_NF_H_
#define _CONF_NF_H_

//_____ M A C R O S ________________________________________________________

//_____ D E F I N I T I O N ________________________________________________

// The list of the supported Nand Flash is available in "support_NF_Driver.pdf" file.


// !!!!!! WARNING !!!!!!!!
// !! The auto-detect mode (#define NF_AUTO_...) may be less speed that static compilation (#define NF_TYPE_...)


//! ************ Auto-detect mode
//!
//! Values: TRUE : Firmware will autodetect
//!         FALSE: Firmware will not autodetect
//! Note: the 2KB and 512B detect can't be enable in same time
//!
#define NF_AUTO_DETECT_2KB      TRUE//FALSE
#define NF_AUTO_DETECT_512B     FALSE



//! ************ Static mode
//!  To enable a reference, you shall define a NF_TYPE_X (X = reference)
//#define NF_TYPE_MT29F2G08AACWP
//#define NF_TYPE_K9F2G08U0M
//#define NF_TYPE_K9K4G08U0M


//! ************ For all mode
//! Define the number of NandFlash connected (= number of Chip Select)
#define NF_N_DEVICES            1


// ******** Exemples **********
//
// You have connected one MT29F2G08AACWP, you shall set
// #define NF_TYPE_MT29F2G08AACWP
// #define NF_N_DEVICES    1
//
// You have connected two MT29F2G08AACWP, you shall set
// #define NF_TYPE_MT29F2G08AACWP
// #define NF_N_DEVICES    2
//
// You have connected one MT29F16G08FAAWC, you shall set
// #define NF_TYPE_MT29F16G08FAAWC
// #define NF_N_DEVICES    2  // 2 because this reference use 2 Chip Select
//
// You have connected two MT29F16G08FAAWC, you shall set
// #define NF_TYPE_MT29F16G08FAAWC
// #define NF_N_DEVICES    4  // 4 because this reference use 2 Chip Select AND you have 2 NandFlash
//
// *** END OF Exemples ***


//!!!! Don't change following parameters
#define ERASING_ALL            DISABLE // erase the whole flash then hangs
#define NF_CACHE_LUT_LOG_SZ         64 // number of logical blocks cached from the LUT
#define NF_CACHE_FBB_LOG_SZ         32 // number of logical blocks cached from the Free-blocks Block


#endif // _CONF_NF_H_
