#ifndef _ADC_KEY_H_
#define _ADC_KEY_H_

#include <sfr.h>
#include <KBDM9_DDK.H>

/*#define ADC_CH0 1
#define ADC_CH1 0
#define ADC_CH2 1
#define ADC_CH3 1
#define ADC_CH4 0
#define ADC_CH5 0*/

void ADC_init(void);
unsigned int Get_ADCValue(unsigned int Chx);

#endif // _ADC_KEY_H_