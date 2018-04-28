
#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"
#include "SDImg.h"
#include "variable.h"
#include "modules\power\power.h" 
#include "include\adc.h"
#include "sio\com.h"

#pragma DATA = MAIN_DATA
#pragma CODE = MAIN_CODE

void anjian_test(void);

#if 0//def TX_DEBUG
void InitUart( void )
{
   UARBR = 0x0002u;  //115200 bps
   UARCR = 0x8001;   // disable parity, one stop bit, 8 bits per character
   PBEN |= 0x0010;
}
#endif

void delay(unsigned int temp)
{
	unsigned int i,j;
	for(i = 0;i < temp;i++){
		for(j = 0;j < 100;j++){}
		}
}

void test_adc(void)
{
	unsigned int j;
	unsigned int adc0_value,adc2_value,adc3_value,adc7_value;
	PWR_ON();
	 while(1){
	     adc0_value = Get_ADCValue((unsigned int)0 );
		 delay(1000);;
		 adc2_value = Get_ADCValue((unsigned int)2 );
		 delay(1000);;
		 adc3_value = Get_ADCValue((unsigned int)3 );
	 	 delay(1000);;
		 adc7_value = Get_ADCValue((unsigned int)7 );
		 delay(1000);;
	     SendString((unsigned char *)"\r\nadc0:adc2:adc3:adc7:bi=");
	     send_hex_u16(adc0_value);
		 SendString((unsigned char *)":");
	     send_hex_u16(adc2_value);
		 SendString((unsigned char *)":");
		 send_hex_u16(adc3_value);
		 SendString((unsigned char *)":");
	     send_hex_u16(adc7_value);
		  SendString((unsigned char *)":");
		  j = (unsigned int)((adc0_value*100UL)/adc7_value);
	     send_hex_u16(j);
	 	}
	
}


void main( void )
{
   unsigned int bat_val,temp_data,key_power_press_time = 5;
   IP_INIT();

	
   SystemClock( SYSTEM_CLOCK );
   SDKLoadCodInit();

	g_system_flag.value = 0;
   ap4_bit_news.value = 0;
   g_sys_flag.value = 0;
   
   //SendString((U8 *)"KB mode\r\n");
#if 0
	   ADC_init();
	   
	   _bitCLR( PWRON_DIR );
	   PWR_ON();
	   //anjian_test();
	   test_adc();
//#else
#endif
	ADC_init();
    InitUart();
	SendString((unsigned char *)"\r\nKB boot!");
	temp_data = 0;
	do{
 	delay( 15 );
    temp_data += Get_ADCValue( 3 );
	delay( 15 );
    bat_val = Get_ADCValue( 2 );
	
	}while(--key_power_press_time);

		temp_data = temp_data/5;

		if(bat_val>100){
		if(temp_data < 900){
			SendString((unsigned char *)"\r\nburn fw!");
			CommonLoadCode(C_BURN);
			}
		}

		/*
	if(bat_val > 100){
		temp_data = temp_data/5;
		  if((temp_data >= 0x211)&&(temp_data <= 0x221)){
				SendString((unsigned char *)"\r\nburn fw!");
				CommonLoadCode(C_BURN);
				}
		}*/
   
   CommonLoadCode( C_STARTUP );   
}




void anjian_test(void)
{
#ifdef TX_DEBUG
	unsigned int temp,value;
	unsigned int adc_7;
	unsigned int temp_channel = 0;  
	SendString((unsigned char *)"\r\nthis is the adc test");
	while(1)
		{
		delay(1000);
		adc_7 = Get_ADCValue(7);
		delay(1000);
		temp_channel = 0;
		temp = Get_ADCValue(temp_channel);
		value = (unsigned int)((temp*100UL)/adc_7);
		SendString((unsigned char *)"\r\nchan:Get_ADCValue:adc_7:bili = ");
		send_hex_u16(temp_channel);
		SendString((unsigned char *)":");
		send_hex_u16(temp);
		SendString((unsigned char *)":");
		send_hex_u16(adc_7);
		SendString((unsigned char *)":");
		send_hex_u16(value);
		/*
		if(temp_channel == 0){
			temp_channel = 2;
			continue;
			}
		if(temp_channel == 2){
			temp_channel = 3;
			continue;
			}
		if(temp_channel == 3){
			temp_channel = 0;
			continue;
			}
		//*/
		}
#endif	
	
}
