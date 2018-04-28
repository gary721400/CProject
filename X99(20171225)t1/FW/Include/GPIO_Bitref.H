/*******************************************************************
 * Copyright (c) 2009
 * King Billion Electronics Co., Ltd.
 * File Name	: GPIO_Bitref.H
 * Author		: Nick
 * Create Date	: February 26, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

//------------------------------------------------------------------------------
#ifndef _GPIO_BITREF_H_
#define _GPIO_BITREF_H_

#include <sfr.h>

//------------------------------------------------------------------------------
/*
#define _PAEN_b0    ((_bitref)(0x390U))
#define _PADIR_b0   ((_bitref)(0x3A0U))
#define _PAOD_b0    ((_bitref)(0x3B0U))
#define _PADS_b0    ((_bitref)(0x3C0U))
#define _PAID_b0    ((_bitref)(0x3D0U))
#define _PAPH_b0    ((_bitref)(0x3E0U))
#define _PAPIE_b0   ((_bitref)(0x3F0U))
#define _PAPIP_b0   ((_bitref)(0x400U))
#define _PAPIF_b0   ((_bitref)(0x410U))
#define _PAICR_b0   ((_bitref)(0xD80U))
*/

#define _PBEN_b0    ((_bitref)(0x420U))
#define _PBDIR_b0   ((_bitref)(0x430U))
#define _PBOD_b0    ((_bitref)(0x440U))
//#define _PBDS_b0    ((_bitref)(0x450U))
#define _PBID_b0    ((_bitref)(0x460U))
#define _PBODN_b0   ((_bitref)(0x470U))
#define _PBPH_b0    ((_bitref)(0x480U))
#define _PBPIE_b0   ((_bitref)(0x490U))
#define _PBPIP_b0   ((_bitref)(0x4A0U))
#define _PBPIF_b0   ((_bitref)(0x4B0U))
#define _PBICR_b0   ((_bitref)(0xD90U))

#define _PCEN_b0    ((_bitref)(0x4C0U))
#define _PCDIR_b0   ((_bitref)(0x4D0U))
#define _PCOD_b0    ((_bitref)(0x4E0U))
#define _PCDS_b0    ((_bitref)(0x4F0U))
#define _PCID_b0    ((_bitref)(0x500U))
#define _PCPH_b0    ((_bitref)(0x510U))
#define _PCPIE_b0   ((_bitref)(0x520U))
//#define _PCPIP_b0   ((_bitref)(0x530U))
#define _PCPIF_b0   ((_bitref)(0x540U))
#define _PCICR_b0   ((_bitref)(0xDA0U))

#define _PDEN_b0    ((_bitref)(0x550U))
#define _PDDIR_b0   ((_bitref)(0x560U))
#define _PDOD_b0    ((_bitref)(0x570U))
#define _PDDS_b0    ((_bitref)(0x580U))
#define _PDID_b0    ((_bitref)(0x590U))
#define _PDPH_b0    ((_bitref)(0x5A0U))
//#define _PDPIE_b0   ((_bitref)(0x5B0U))
//#define _PDPIP_b0   ((_bitref)(0x5C0U))
//#define _PDPIF_b0   ((_bitref)(0x5D0U))
//#define _PDICR_b0   ((_bitref)(0xDB0U))

/*
#define _PEEN_b0    ((_bitref)(0x5E0U))
#define _PEDIR_b0   ((_bitref)(0x5F0U))
#define _PEOD_b0    ((_bitref)(0x600U))
#define _PEDS_b0    ((_bitref)(0x610U))
#define _PEID_b0    ((_bitref)(0x620U))
#define _PEPH_b0    ((_bitref)(0x630U))
#define _PEPIE_b0   ((_bitref)(0x640U))
#define _PEPIP_b0   ((_bitref)(0x650U))
#define _PEPIF_b0   ((_bitref)(0x660U))
#define _PEICR_b0   ((_bitref)(0xDC0U))
*/

/*
#define _PFEN_b0    ((_bitref)(0x670U))
#define _PFDIR_b0   ((_bitref)(0x680U))
#define _PFOD_b0    ((_bitref)(0x690U))
#define _PFDS_b0    ((_bitref)(0x6A0U))
#define _PFID_b0    ((_bitref)(0x6B0U))
#define _PFPH_b0    ((_bitref)(0x6C0U))
#define _PFPIE_b0   ((_bitref)(0x6D0U))
#define _PFPIP_b0   ((_bitref)(0x6E0U))
#define _PFPIF_b0   ((_bitref)(0x6F0U))
#define _PFICR_b0   ((_bitref)(0xDD0U))
*/

#endif // _GPIO_BITREF_H_
//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------