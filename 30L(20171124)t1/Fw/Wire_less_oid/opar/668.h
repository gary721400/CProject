#ifndef _CIS668_H
#define _CIS668_H


//CIS 668
typedef enum
{
	cis_frame_flag	=	0,	//! new flame flag
	cis_line_flag	=	1,	//! new line flag
	cis_line_count	=	2,	//! read line count
	cis_buffer_point	=	3,	//! read line buffer point
	cis_frame_count	=	4,	//! read cis frame count
	cis_error_flag	=	0x100,	//! read cis error flag
					//! return 0 	--> no error
					//! return -1	--> write I2C error
					//! return -2	--> read I2C error
					//! return -4	--> set CIS view area error
					//! return -8	--> initial CIS Vsync time out
}cis_msg;

extern unsigned int read_cis(unsigned int cis_msg);

extern unsigned int cis_y_count;		//line count, _cis_flag.1=1 時, 才可參考此register
extern unsigned int cis_x_byte;		//line data count
extern unsigned int cis_flag	;		//bit 0  new frame，bit 1  new line
extern unsigned int ExtrationFlag;
//extern unsigned int cis_buffer_point;		//new line data buffer point

extern unsigned int i2c_read_byte(unsigned int reg_addr);
extern unsigned int i2c_write_byte(unsigned int reg_addr, unsigned int reg_data);
extern void i2c_int(void);
extern void cs_int(void);
extern void vsync_int(void);
extern unsigned int initial_cis_driver(void);
extern unsigned int start_cis_driver(void);
extern void clear_cis_line_buffer(void);
extern unsigned int initial_cis_reg(void);
extern unsigned int write_cis_reg_byte(unsigned int reg_addr, unsigned int reg_data);
extern unsigned int read_cis_reg_byte(unsigned int reg_addr);			//return data
extern unsigned int set_cis_view_area(unsigned int cis_x1, unsigned int cis_y1, unsigned int cis_x2, unsigned int cis_y2);
extern unsigned int cvt_cis_view_area(void);
extern void Delay30ms(void);

//CIS 668

#endif