#ifndef  _AGC_H_
#define _AGC_H_

extern void AGC_Init(unsigned int Frame_Size);
extern void AGC_Sensitive(unsigned int Frame_Count);
extern void AGC(__X int * In_Buffer);

#endif   // _AGC_H_