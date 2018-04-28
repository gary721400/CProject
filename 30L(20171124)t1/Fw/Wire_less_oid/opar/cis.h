/*******************************************************************
 * Copyright (c) 2010
 * British Virgin Isiands Billion Gold Securities Ltd.
 * File Name    : IRControl.h
 * Author       :
 * Create Date  : Oct. 27, 2010
 * History      :
 * Remark       :
*******************************************************************/
#ifndef _Global_H
#define _Global_H

#define CSI_on           __asm{mov cscon,#0x95d0  };
#define CSI_off          __asm{mov cscon,#0x00};  
#define CSI_CLKOn        __asm{bset pben.14  rep #65535 nop};
#define CSI_CLKOff       __asm{rep #65535 nop bclr pben.14};
#define Sensor_on        write_cis_reg_byte(0x09,0x00);	
#define Sensor_off       write_cis_reg_byte(0x09,0x10);	 
#define T12_StandyBy     __asm{crit #4 bset modcon.14 nop nop crit #4 rep #65535 nop nop nop};  
#define Bright_Dark      ((unsigned long)76800)   
#define Bright_Gray      ((unsigned long)160000)  

#define Senor_Err     (unsigned long)0xF10000
#define IR_Err        (unsigned long)0xF20000
#define IR_Dark       (unsigned long)0xF30000
#define IR_Bright     (unsigned long)0xF40000
//#define Calibrate_Err  0xE50000
#define Calibrate_Pass (unsigned long)0xF60000

#define  Calibrate_Count  0x03
#defeine Center_Bright    392000   
#defeine Boundary_Bright  1215000  

#define IR_Max  0X68
#define IR_Min  0X01
#define IR_Middle 0x10
#define IR_Max_Cycle 0x7FF


#ifdef __KBCC__

#define _NEAR	__X
//#define nearset	memset
#define	_KB_ASM_
#else
#define _NEAR
#define _DEBUG_PC_SW
#define nearset	memset
#endif

//external use
extern void OPAR_Init(void);
extern unsigned int Decode(void);

//internal use
extern _NEAR unsigned int Bright;
extern char barcode(unsigned char *buf, unsigned int *value);
extern void DotExtraction(__X unsigned char *image_g);
#endif

