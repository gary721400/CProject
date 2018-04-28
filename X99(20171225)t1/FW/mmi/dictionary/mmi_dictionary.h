#ifndef	_MMI_DICTIONARY_H_
#define	_MMI_DICTIONARY_H_

#include "conf\conf_sio.h"
#include "conf\conf_lcd.h"
//#include "mmi\ap3\mmi_ap3.h"

#if( DIC == ENABLE )

#define mmi_dic_kbd_disable()       ( b_kbd_process_disable = TRUE )
#define mmi_dic_kbd_enable()        ( b_kbd_process_disable = FALSE )
#define mmi_dic_kbd_is_disable()    ( b_kbd_process_disable == TRUE )

//#define dic_play_state              ap4_play_state

#if( LCD_PRESENT == ENABLE )
void  mmi_dic_display_volume     ( void );
#else
#define mmi_dic_display_volume()
#endif   //#if( LCD_PRESENT == ENABLE )

#endif   //#if( DIC == ENABLE )
#endif   //_MMI_DICTIONARY_H_
