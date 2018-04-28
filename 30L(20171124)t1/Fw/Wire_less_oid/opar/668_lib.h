//
//	668_lib.h
//
//=============================

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

unsigned int	read_cis(unsigned int cis_msg);

unsigned int	i2c_read_byte(unsigned int reg_addr);
unsigned int	i2c_write_byte(unsigned int reg_addr, unsigned int reg_data);
//	void	i2c_int(void);
void	cs_int(void);
unsigned int	initial_cis_driver(void);
unsigned int	start_cis_driver(void);
void	close_cis_driver(void);
void	clear_cis_line_buffer(void);
unsigned int	initial_cis_reg(void);
unsigned int	write_cis_reg_byte(unsigned int reg_addr, unsigned int reg_data);
unsigned int	read_cis_reg_byte(unsigned int reg_addr);			//return data
unsigned int	set_cis_view_area(unsigned int cis_x1, unsigned int cis_y1, unsigned int cis_x2, unsigned int cis_y2);
unsigned int	cvt_cis_view_area(void);
//void	pb_int(void);
