//!
//! @file com_appli.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file defines the common code to make it possible the call of the mmi applications
//!
//! @version 2.2 snd3-dvk-1_9_5 $Id: com_appli.c,v 2.2 2007/07/25 13:16:11 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _com_appli_c
#define _com_appli_c

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "mmi\shared\com_appli.h"
#include "conf\conf_mmi.h"

#include "conf\conf_sio.h"
#include <KBDM9_DDK.H>

#pragma DATA = COM_APPLI_DATA
#pragma CODE = COM_APPLI_CODE

//U16   swap_code_flag = 0;

/*
ImageDeclareType( USB_CORE_CODE );
ImageDeclareType( USB_STORAGE_CODE );
ImageDeclareType( MMI_STARTUP_CODE );
ImageDeclareType( MMI_IDLE_CODE );
ImageDeclareType( MMI_PLAYER_CODE );
ImageDeclareType( SRV_PLAYER_CODE );
*/

void call_mmi_appli(U8 idx, U8 evt_id, U16 evt_param)
{
   // WARNING NOTE : if the code banking feature is used, all mmi applications
   // below should have the following definition: void mmi_appli(U8 event, U16 param)
   switch( idx )
   {
      case 0                     :  status_mmi_appli     (evt_id, evt_param); break;
      //case MMI_APPLI_IDLE        :  idle_mmi_appli       (evt_id, evt_param); break;
      //case MMI_APPLI_MSTORAGE    :  swap_code( USB_STORAGE_CODE_FLAG ); storage_mmi_appli    (evt_id, evt_param); break;
      //case MMI_APPLI_PLAYER      :  swap_code( MMI_PLAYER_CODE_FLAG );  player_mmi_appli     (evt_id, evt_param); break;
      case MMI_APPLI_SETTING     :  setting_mmi_appli    (evt_id, evt_param); break;
      //case MMI_APPLI_RECORDER    :  recorder_mmi_appli   (evt_id, evt_param); break;
      //case MMI_APPLI_DUMMY       :  dummy_mmi_appli      (evt_id, evt_param); break;
      //case MMI_APPLI_STARTUP     :  startup_mmi_appli    (evt_id, evt_param); break;
      //case MMI_APPLI_POWEROFF    :  poweroff_mmi_appli   (evt_id, evt_param); break;
      
#if 0//( PASTER == ENABLE )
      case MMI_APPLI_PASTER      :  paster_mmi_appli     (evt_id, evt_param); break;
#endif

#if 0//( AP3 == ENABLE )
      case MMI_APPLI_AP3         :  ap3_mmi_appli        (evt_id, evt_param); break;
#endif

#if 0//( DIC == ENABLE )
      case MMI_APPLI_DIC         :  dic_mmi_appli        (evt_id, evt_param); break;
#endif
      //default                    :                                            break;
   }
}

/*
void swap_code( U16 code_flag )
{
   if( swap_code_flag & code_flag )
      return;
   switch( code_flag )
   {
      case USB_STORAGE_CODE_FLAG:
      swap_code_flag = 0;
      swap_code_flag |= USB_STORAGE_CODE_FLAG;
      SDKLoadCode( USB_CORE_CODE );
      SDKLoadCode( USB_STORAGE_CODE );
      break;

      case MMI_STARTUP_CODE_FLAG:
      swap_code_flag = 0;
      swap_code_flag |= MMI_STARTUP_CODE_FLAG;
      SDKLoadCode( MMI_STARTUP_CODE );
      break;

      case MMI_IDLE_CODE_FLAG:
      //swap_code_flag &= ~( USB_STORAGE_CODE_FLAG | MMI_STARTUP_CODE_FLAG );
      swap_code_flag = 0;
      swap_code_flag |= MMI_IDLE_CODE_FLAG;
      SDKLoadCode( MMI_IDLE_CODE );
      break;

      case MMI_PLAYER_CODE_FLAG:
      swap_code_flag = 0;
      swap_code_flag |= MMI_PLAYER_CODE_FLAG;
      SDKLoadCode( MMI_PLAYER_CODE );
      SDKLoadCode( SRV_PLAYER_CODE );
      break;
   }
}
*/

#endif	// _com_appli_c_
