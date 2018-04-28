#ifndef _CIS_H
#define _CIS_H

#define CMOS_CIF	// 240x240

#ifdef CMOS_CIF
#define CMOS_NORMAL_CLK	0x80
#define CMOS_DOWN_CLK		0x82
#else
#define CMOS_NORMAL_CLK	0
#define CMOS_DOWN_CLK		1
#endif

typedef enum {

	CIS_No_Err =  0,			/*!< No Error */
	CIS_Init_Err =  -1,			/*!< Init Error */
	CIS_Write_Reg_Err =  -2,		/*!< Write Reg Error */
	CIS_Read_Reg_Err =  -3,		/*!< Read Reg Error */
	CIS_Set_Area_Err = -4,		/*!< Set Area Error */

} CIS_ERR;

typedef enum {

	CIS_New_Line =  1,			/*!< New Line */
	CIS_New_Frame =0,		/*!< New Frame */
	CIS_No_Flag =  -1,			/*!< No Flag */

} CIS_FLAG;


extern CIS_ERR CIS_Init(void);

extern CIS_ERR CIS_Write_Reg(unsigned int RegAddr, unsigned int RegData);

extern CIS_ERR CIS_Read_Reg(unsigned int RegAddr);

extern CIS_FLAG CIS_Read_Flag(void);

extern unsigned int CIS_Line_Count(void);

extern void CIS_Read_CisData(__X unsigned int *ptr);

extern void CIS_Read_CisCalibrationData(__X unsigned int *ptr,unsigned StartPoint,unsigned int length);

extern void close_cis_driver(void);

#endif