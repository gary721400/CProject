
#ifndef _i2c_drv_c_
#define _i2c_drv_c_

#include "lib_mcu\compiler.h" 
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "sio\com.h"
#include "i2c_drv.h"
#include "mmi\kbd\keypad.h"
#include "timer_drv.h"
#include "string.h"


#pragma DATA = I2C_DRV_DATA
#pragma CODE = I2C_DRV_CODE

void delay_500ns(U16 time_num)
{
 while(time_num){
 	__asm{
		REP #30
		NOP
		} 
 	time_num--;
 	}
}

void i2c_init( void )
{
  SCL_GPIO();
  SDL_GPIO();
  SCL_OUTPUT();
  SCL_HEIGH();
  
  SDL_OUTPUT();
  SDL_HEIGH();
  delay_500ns(1);
  SDL_LOW();
}

void i2c_exit( void )
{
	SCL_HEIGH();
	delay_500ns(1);
	SDL_HEIGH();
	delay_500ns(1);
#ifdef TX_DEBUG
	SCL_INTERRUPT();
#endif
}

void i2c_send_data(U8 send_data)
{
  U8 value,out_mask = 0x80;
  int count = 8;
  int i = 0;
  Bool  ack_flag;
    /* Send data bytes one bit at a time. */   
    while(count > 0)
    {            
     value = ((send_data & out_mask) ? 1 : 0);
      if (value == 1){
       SDL_HEIGH();
		}
      else{
       SDL_LOW();
	   }
      SCL_HEIGH();
      delay_500ns(1);    

      SCL_LOW();
      out_mask >>= 1; 
      count--;   
     } 
	SDL_INPUT();
	SCL_HEIGH();
	delay_500ns(1);
	ack_flag = GET_SDL();
	SCL_LOW();
	SDL_LOW();
	SDL_OUTPUT();
}

U8 i2c_receive_data(void)
{
  U8 value,out_mask = 0x01;
  int count = 8;
  Bool  ack_flag;

  value = 0;
  SDL_INPUT();
  while(count > 0) 
  {
	  SCL_HEIGH();
      delay_500ns(1);    
    if (GET_SDL()){
       value |= 0x01;
		}
      SCL_LOW();
      value <<= 1; 
      count--;  
	  delay_500ns(1);
   }
	SCL_HEIGH();
	delay_500ns(1);
	ack_flag = GET_SDL();
	SCL_LOW();
	SDL_LOW();
	SDL_OUTPUT();
    return value;
}

void i2c_read_time(void)
{
	U8 temp_data;
	
	i2c_init();
	i2c_send_data(ADDR_READ_BEGIN);
	i2c_exit();
}
void i2c_set_time(void)
{
	i2c_init();
    i2c_send_data(ADDR_WRITE_BEGIN);
	i2c_exit();
}
#if 0
		PUSH	R1
		PUSH	R2
		PUSH	R3
		BSET	PBEN.8
		BSET	PBEN.9
		MOV	R1,	#3
	$loop:	MOV	I2CSAR,	#0xE0
		MOV	I2CCR,	#0x8020|I2C_SPEED
		MOV	I2CTDR,	#0x00
		MOV	R3,	#0x900
		BMOV	R3.1,	I2CCR.2
		BMOV	R3.0,	I2CCR.1
		SR	R3,	rb7
		BSET	I2CCR.0
		MOV	R2,	R3
	$waitc:	CMPD1	R2,	#0
		AJMP	IF_EQ,	$tout
		JNB	I2CCR.9,$waitc
		BCLR	I2CCR.9

//		MOV	R0,	[FP+#(6)]
		MOVX R0, _wt_rindex

		AND	R0,	#0xFF
		MOVX	I2CTDR,	R0

		MOV	R2,	R3
	$waitd:	CMPD1	R2,	#0
		AJMP	IF_EQ,	$tout
		JNB	I2CCR.9,$waitd
		BCLR	I2CCR.9
		MOV	I2CSAR,	#0xE1
		MOV	I2CCR,	#0x8028|I2C_SPEED
		MOV	R2,	R3
	$waitr:	CMPD1	R2,	#0
		AJMP	IF_EQ,	$tout
		JNB	I2CCR.8,$waitr
		BCLR	I2CCR.8
		MOVX	R0,	I2CRDR
		SL	R0,	#8
		MOV	R2,	R3
	$waitt:	CMPD1	R2,	#0
		AJMP	IF_EQ,	$tout
		JNB	I2CCR.8,$waitt
		BCLR	I2CCR.8
		SCALL	_wt_stop_delay
		IGNDS	#1
//		MOVX	R0,	I2CRDR
		OR	R0,	I2CRDR
	$exit:	POP	R3
		POP	R2
		POP	R1
		RETS
	$tout:	CMPD1	R1,	#1
		AJMP	IF_NE,	$loop
		SCALL	_wt_stop_delay
		MOV	R0,	#-1
		AJMP	$exit
#endif
#endif   //_i2c_drv_c_
