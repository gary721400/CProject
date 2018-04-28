#include "KBDM9_DDK.h"
#include <fs.h>
#include <string.h>

//#include "668.h"
#include "cis.h"

#pragma CODE=Test_Code
#pragma DATA=Test_Data
/*

CIS_ERR CIS_Init(void)
{
	int init_err=-1;
	int err=-1;

	SetVector(0x1e,i2c_int);
	SetVector(0x28,cs_int);
//	SetVector(0x22,vsync_int);
	SetVector(0x24,vsync_int);  		//portC
	//clear_cis_line_buffer();
	init_err = initial_cis_driver();
	if (init_err == (-1))
		return -1;
#ifdef CMOS_CIF
	write_cis_reg_byte(0x12,0x00);
	write_cis_reg_byte(0x11,CMOS_NORMAL_CLK);
	write_cis_reg_byte(0x17,0x18);
	write_cis_reg_byte(0x18,0x36);
	write_cis_reg_byte(0x19,0x04);
	write_cis_reg_byte(0x1a,0x22);
	write_cis_reg_byte(3,0x4a);
	write_cis_reg_byte(0x32,0x2d);
	write_cis_reg_byte(0x00, 0x0A);
	write_cis_reg_byte(0x10, 0x1F);
	//Delay30ms();
	//Delay30ms();

#else
	write_cis_reg_byte(0x12,0x20);
	write_cis_reg_byte(0x11,CMOS_NORMAL_CLK);
	write_cis_reg_byte(0x17,0x14);
	write_cis_reg_byte(0x18,0x23);
	write_cis_reg_byte(0x19,0x02);


	write_cis_reg_byte(0x1a,0x11);
	write_cis_reg_byte(3,0x4a);
	write_cis_reg_byte(0x32,0x24);

	//Delay30ms();
	//Delay30ms();

	//write_cis_reg_byte(0x0F, 0x00);
	//write_cis_reg_byte(0x3C, 0xFF);
	//write_cis_reg_byte(0x04, 0x04);
#endif

	Delay30ms();
	Delay30ms();

	__asm{
		bclr	pcpif.3		;cis_vsync_pif
		bset	pcicr.0		;cis_vsync_icr
		bset	pcpie.3		;cis_vsync_pie
		nop
	}
	return 0;
}

CIS_ERR CIS_Write_Reg(unsigned int RegAddr, unsigned int RegData)
{
	int write_err;
	write_err = write_cis_reg_byte(RegAddr, RegData);
	if (write_err == (-1))
		return -2;
	return 0;
}

CIS_ERR CIS_Read_Reg(unsigned int RegAddr)
{
	int read_err;
	read_err = read_cis_reg_byte(RegAddr);
	if (read_err == (-1))
		return -3;
	return read_err;
}

CIS_FLAG CIS_Read_Flag(void)
{
	if(cis_y_count%2!=0)
	   return -1;

	if ((cis_flag ==3))  //&&(cis_y_count==0)
	{
		cis_flag = cis_flag & 0xFFFEU;
		return 0;
	}
	else if(cis_flag==2)
	{
		 cis_flag = cis_flag & 0xFFFDU;
		return 1;
	}
	else
	{
			return -1;
	}
}

unsigned int CIS_Line_Count(void)
{
	return cis_y_count/2;
}


void CIS_Read_CisData(__X unsigned int *ptr)
{
    int i=0;

	__X unsigned int* SensorData=(__X unsigned int*)cis_buffer_point;

	for(i=0;i<cis_x_byte/2;i++)
		*ptr++=*(SensorData+2*i);

	//memcpy(ptr, (__X unsigned int*)cis_buffer_point, cis_x_byte/2);
}
*/
void CIS_Read_CisCalibrationData(__X unsigned int *ptr,unsigned StartPoint,unsigned int length)
{
	memcpy(ptr, (__X unsigned int*)(cis_buffer_point+StartPoint),length);
}
