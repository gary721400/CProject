//! @file e2radio.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 management routines
//!
//! @version 1.18 snd3b-dvk-1_0_33 $Id: e2radio.c,v 1.18 2006/12/08 16:21:58 sguyon Exp $
//!
//! @configuration
//! @todo
//! @bug
#ifndef _infrared_c_
#define _infrared_c_

//_____ I N C L U D E S ____________________________________________________
#include <ISD12_DDK.H>
#include "config.h"
#include "compiler.h"
#include "system.h"
#include "power.h"
#include "timer_drv.h"
#include "infrared.h"
#include "com.h"

#pragma DATA = INFRARED_DATA
#pragma CODE = INFRARED_CODE

//#define  ID_NEC 	0X20DF
#define  ID_NEC 	0XDF20


#define FLAG_OFF	1
#define FLAG_ON		2
#define FLAG_FLASH	3

U16	infrared_count;
U8	Infrared_Timer;
U8 infrared_flag;
//===========================================================================
extern void delay1ms( int delay_time );
extern void delay3us(void);
extern void delay10us(void);
extern void delay100us(void);
//===========================================================================
//_____ M A C R O S ________________________________________________________

//========================================================================


//=============================================================================


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________


void init_infrared(void)
{
	_bitCLR(RED_CLK_DIR); // Set infrared clk with Output.

	_bitSET(RED_CLK_PH);  // Set FR_RST with Pull-high.

//	_bitCLR(RED_DATA_DIR); // Set infrared data with Output.

	Clr_red_clk();
}

//====================================================================================

void __interrupt Timer_infrared(void)
{
	Infrared_Timer++;
	infrared_count++;
	if(infrared_flag==FLAG_FLASH){
		if(Infrared_Timer>4){
			Infrared_Timer=0;
			}
		if(Infrared_Timer<3){
			Set_red_clk();
			}
		else{
			Clr_red_clk();
			}
		}
	if(infrared_flag==FLAG_OFF){
		Clr_red_clk();
//		Set_red_clk();
		}
	if(infrared_flag==FLAG_ON){
		Set_red_clk();
//		Clr_red_clk();
		}

}

//===================================================================================



void infrared_control(Bool temp_flag,U16 temp_count)
{
	U16 endcount;
	endcount=temp_count; 
	infrared_flag=temp_flag;
	infrared_count=0;
	do{}while(infrared_count<endcount); 
}


void test_38K(void)
{
    System_Timer_Stop(3);
	Infrared_Timer = 0;
	System_Timer_Init(2,1);
	infrared_flag=FLAG_FLASH;
}
//============================================================================
void Infrared_Send_data(U8 address, U8 command)
{
	
	U8 i,temp_data,flag_temp_data;
	U16	data,flag_data;
	infrared_flag=FLAG_OFF;
	infrared_count=0;
	System_Timer_Stop(3);
	Infrared_Timer = 0;
	System_Timer_Init(2,1);

	//begin 9ms	
	//infrared_control(FLAG_FLASH,337);
//	infrared_control(FLAG_FLASH,1698);
	infrared_control(FLAG_FLASH,1700);



	//over 4.5ms
//	infrared_control(FLAG_OFF,168);
//	infrared_control(FLAG_OFF,849);
	infrared_control(FLAG_OFF,850);

	//over 2.25ms
//	infrared_control(FLAG_OFF,425);


	


#if 1
	//send nec id
	data=ID_NEC; 
	flag_data= 0x0001;
	for(i=0;i<16;i++) 
	{ 
		//first send 560us
		infrared_control(FLAG_FLASH,105);
		if(data&flag_data)  //bit 0 is low or high
		{ 
			infrared_control(FLAG_OFF,315);  //1 --->1.68ms
		} 
		else 
		{ 
			infrared_control(FLAG_OFF,105);   //0--->0.56ms
		} 

		flag_data=flag_data<<1; 
	} 

#else
#endif
	//send address
	temp_data=command; 
	flag_temp_data = 0x01;
	for(i=0;i<8;i++) 
	{ 
		//first send 560us
		infrared_control(FLAG_FLASH,105);
		if(temp_data&flag_temp_data)  //bit 0 is low or high
		{ 
			infrared_control(FLAG_OFF,315);  //1 --->1.68ms
		} 
		else 
		{ 
			infrared_control(FLAG_OFF,105);   //0--->0.56ms
		} 

		flag_temp_data=flag_temp_data<<1;
	} 
// /*   
   	//send inverted address 
	temp_data= ~command; 
	flag_temp_data = 0x01;
	for(i=0;i<8;i++) 
	{ 
		 
		//first send 560us
		infrared_control(FLAG_FLASH,105);
		 
		if(temp_data&flag_temp_data)  //bit 0 is low or high
		{ 
			infrared_control(FLAG_ON,315);  //1 --->1.68ms
		} 
		else 
		{ 
			infrared_control(FLAG_ON,105);   //0--->0.56ms
		} 
		flag_temp_data=flag_temp_data<<1;
	} 
//#endif
/*
	//send command
	temp_data=command; 
	flag_temp_data=0x01;
	for(i=0;i<8;i++) 
	{ 
		//first send 560us
		infrared_control(TRUE,22);
		if(flag_temp_data&temp_data)  //bit 0 is low or high
		{ 
			infrared_control(FALSE,65);  //1
		} 
		else 
		{ 
			infrared_control(FALSE,22);   //0
		} 

		flag_temp_data=flag_temp_data<<1;
	} 

   	//send inverted command
	temp_data= ~command; 
	flag_temp_data=0x01;
	for(i=0;i<8;i++) 
	{ 
		 
		//first send 560us
		infrared_control(TRUE,22);
		 
		if(flag_temp_data&temp_data)  //test bit  is low or high
		{ 
			infrared_control(FALSE,65);  //1
		} 
		else 
		{ 
			infrared_control(FALSE,22);   //0
		} 
		flag_temp_data=flag_temp_data<<1;
	} 
*/
	

	//begin 9ms	
	infrared_control(FLAG_FLASH,1700);


	//over 2.25ms
	infrared_control(FLAG_OFF,425);

	System_Timer_Stop(2);
	Clr_red_clk();
	delay1ms(50);
	System_Timer_Init(3, 2);
	delay1ms(50);
	
}




#endif  // _infrared_c_
