

/* 
 //======================================================================= 
 // Copyright: 2006-2008 Solid State System Co., Ltd. 
 // Project  : 3S1120 USB_MP3 (8041)
 // File     : main_mp3.c 
 // Author   : Mason Yang  
 // Date     :
 // Version  : v01
 // Abstract : MP3 main C function
 //======================================================================= 

(1) Afaya demo code.
    This demo code used EVB board UART port to communicate w/h User interface by PC terminal(Teraterm)
    set-up serial port in 14400bps,8 data,None parity,1 bit stop and None Flowcontrol..
    (P) Play/Pause music
    (F) Forward Music          (B) Backward Music
    (U) Volume Up              (D) Voulme Down

    MP3_SysInit()
        BackEnd_Init_Phase2.. FALSE then UART show warning message & stop program.
    Earphone Class-D defult : 0x24.        
                                                                                    -> Ron@2008/3/20
----------------------------------------------------------------------------------------------------

*/

#pragma CODE = AP4_MAIN_CODE
#pragma DATA = AP4_MAIN_DATA

#include "stdio.h"
#include "system.h"
//#include "navigation.h"
//#include "explorer.h"
#include "mmgr_kernel.h"
#include "com.h"
#include "player_task.h"
#include "fat.h"
#include "file.h"           // for file management 
#include "keypad.h"
#include "search_task.h"

//#include "oid.h"
//#include "power_task.h"
//#include "scheduler.h"
//#include "com_var.h"


//#include "usb_task_sch.h"
//#include "power.h"             // Power module definition
#include "mmi_ap3.h"
#include "adc.h"

//#include "srv_player.h"
#include "BinaryImage.h"

#include <sfr.h>
//#include <stdlib.h>
#include <math.h>
//#include "KBDM9_DDK.H"
//Bool     gfSysUSBMP3Mode = FALSE;      // global flg of system in USB or MP3 mode

//#define  SysMP3xdataAddr      0xA000

       

//#define VARAL_VAR_ADDR        SysMP3xdataAddr  
//#define SD_DEVICE_VAR_ADDR    ( VARAL_VAR_ADDR + varAlSize )


//xdata    AL_Variables   varAl    _at_( VARAL_VAR_ADDR ); 

//xdata    USB_Variables  USB      _at_( FRONT_VAR_BASE );
//xdata    SDDEVICE_VAR   SDVar    _at_( SD_DEVICE_VAR_ADDR );
//__X    unsigned int   ui_j;
//__X    unsigned char  uc_ISP_sel;

//__X    unsigned short PMC_ID;
//extern unsigned int gflag;


//#pragma CODE = AP4_MAIN_CODE2

ImageDeclareType(MMGR_KERNEL_INIT_CODE);
//ImageDeclareType(READ_SGDB_CODE);
/*
extern void Delay_ms(int count);

//------------------------------------------------------------------------------
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
	SDKLoadCode(MMGR_KERNEL_INIT_CODE);
	mmgr_task_init();
//	SDKLoadCode(READ_SGDB_CODE);
	while(1){
		mmgr_task();
		player_task();
		}
}




