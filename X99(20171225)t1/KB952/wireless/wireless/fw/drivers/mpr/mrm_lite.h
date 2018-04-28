#ifndef __MRM_LITE_H
#define __MRM_LITE_H

#include "mrmBase.h"
#include "sensor.h"
#include "mrm_interface.h"

/* 7601PLL */
#define MPR7601_PLL_80MHz   0
#define MPR7601_PLL_60MHz   1
#define MPR7601_PLL_40MHz   2
#define MPR7601_PLL_20MHz   3
#define MPR7601_PLL         MPR7601_PLL_80MHz

mrmBool  MrmReadReg     ( mrmByte reg, mrmByte *pData );
mrmBool  MrmWriteReg    ( mrmByte reg, mrmByte data );
mrmBool  MrmI2cWrite    ( mrmByte addr, mrmByte data );
mrmBool  MrmI2cRead     ( mrmByte addr );
void     MrmInit        ( void );
mrmWord  MrmGetChipID   ( void );
void     MrmStart       ( void );
mrmBool  MrmGetCode     ( mrmByte *pBuff, mrmBool bRestart );
void     _MDiv          ( mrmByte *mprcode, mrmDword *res_code, mrmByte *book_pre, mrmDword *book_post );

#endif

