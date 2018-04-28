/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: KEY_SCAN.H
 * Author		: Nick
 * Create Date	: January 12, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/
#ifndef _KEY_SCAN_H_
#define _KEY_SCAN_H_

#include <KBDM9_DDK.H>

typedef enum {
    KEY_NO_Press = -1,
    KEY_Press   = 0x0100, // Key Press After Debounce. 
    KEY_Short   = 0x0200, // Key Release After Key_Press Status.
    KEY_Long    = 0x0300, // Key Press After Key_Press Status.
    KEY_Gap     = 0x0400, // Key Press After Key_Long Status.
    KEY_Release = 0x0500  // Key Release After Key_Long Status or Key_Gap Status.
}tKEYSCAN_TYPE;

//------------------------------------------------------------------------------
// Parameter 1: Key Scan Physical Level Initial Function Pointer.
// Parameter 2: Key Scan Physical Level Get Key Function Pointer.
//------------------------------------------------------------------------------
extern void KeyScan_Init(int (*func_ptr)(void),
                         unsigned int Scan_Time);
                         
//------------------------------------------------------------------------------
// Return Value:
//  High-Byte => KEY SCAN TYPE( Please refence tKEYSCAN_TYPE )
//   Low-Byte => KEY SCAN CODE( 0: No Key Press, other: Key Code )
//------------------------------------------------------------------------------
extern int KeyScan_GetCode(void);

extern void KeyScan_CallBack(void);
#endif // _KEY_SCAN_H_
//------------------------------------------------------------------------------
// End of file
//------------------------------------------------------------------------------