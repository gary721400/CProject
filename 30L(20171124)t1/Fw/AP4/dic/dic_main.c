
#pragma CODE = DIC_MAIN_CODE
#pragma DATA = DIc_MAIN_DATA

#include "stdio.h"
//#include "timer_drv.h"
//#include "com.h"
//#include "player_task.h"
//#include "keypad.h"

#include "oid.h"
#include "power.h"             // Power module definition

#include "adc.h"
#include "BinaryImage.h"
#include <sfr.h>
#include "system.h"
#include <math.h>
#include "KBDM9_DDK.H"
#include "dic_kernel.h"
#include "dic_search.h"
extern void Delay_ms(int count);

//extern void  player_task(void);

extern void __interrupt TimerISR_RP(void);
extern void __interrupt TimerISR_ADPCM(void);
void __interrupt TimerISR_RP(void)
{
}
void __interrupt TimerISR_ADPCM(void)
{
}
//------------------------------------------------------------------------------
/*
void GPIO_init(void)
{
	_bitCLR(PWR_CTRL_DIR); // Set PWR_CTRL with Output.

	_bitSET(KBD_PWR_DIR); // Set PWR_CTRL with Input.
	_bitSET(KBD_PWR_PH);  // Set PWR_CTRL with Pull-high.

    _bitCLR(BLUE_LED_DIR);
	_bitCLR(RED_LED_DIR);
	
	_bitCLR(CTL_OID_DIR);
	_bitCLR(CTL_OID_OD);
	
	_bitSET(HP_DET_DIR);
	_bitSET(HP_DET_PH);

	_bitSET(USB_DET_DIR);
	_bitCLR(USB_DET_PH);

#if 0
	_bitCLR(OP_DET_DIR);
	_bitSET(OP_DET_PH);
#endif	
}
*/

void main( void )
{

	IP_INIT();
	SystemClock(SYS_CLOCK);	
	SDKLoadCodInit();	
//	gflag = 0;
//	srvplayer_g_arg.volume = 2;
//	GPIO_init();
//	OP_CLOSE();


    mmgr_task_init();
	while(1)
		{
			mmgr_task();
			dic_player_task();
		}
}




