//! @file auto_disp.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the automatic display module
//!        It is dedicated to the automatic display of standard time-depending graphical objects.
//!        The interface of this module has been designed generic to handle different objects with the same functions.
//!        Each object is dynamically implemented and can be configurated from mmi applications.
//!        Objects allocate the software timers to delay their state machine.
//!
//! @version 1.12 snd3-dvk-1_9_5 $Id: auto_disp.c,v 1.12 2007/08/22 14:12:11 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _auto_disp_c_
#define _auto_disp_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"

#include "lib_system\mailbox\mail_evt.h"
#include "lib_system\timer_soft\timer_soft.h"
#include "modules\mmi_manager\mmgr_kernel.h"

#include "conf\conf_lcd.h"
#include "drivers\mmi\lcd.h"

#include "auto_disp.h"
#include "modules\control_access\ctrl_access.h"
#include "mmi\rw_nor\rw_nor.h"

#include <sfr.h>
#include "KBDM9_DDK.H"
#include "KBDM952.H"

#pragma DATA = AUTO_DISP_DATA
#pragma CODE = AUTO_DISP_CODE

//_____ P R I V A T E   D E C L A R A T I O N S ______________________________

// (For the following structure, the byte being the most used is defined first,
// the less used is defined last.
// The adressing of this structure through a pointer will be then more efficient.)
typedef struct 
{
   U8  status;
   U8  id_appli;
   U8  id_timer;
   U8  obj_type;
   U8  state_machine;
   Ad_p_prm _MEM_TYPE_SLOW_ * p_param;
}Ad_obj_st;

_MEM_TYPE_SLOW_ Ad_obj_st s_process_obj[NB_AD_OBJ];

U8  _MEM_TYPE_SLOW_   ad_g_extra_param;

//!_____ D E F I N I T I O N S ______________________________________________

#pragma CODE = TASK_INIT_CODE

//! @brief ad_init
//!
//! This function initialises the automatic display process.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//! @return none
void ad_init(void)
{
#if( LCD_PRESENT==ENABLED )
#if 0
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;

   // Initialise the status of each timer
   p_obj = &s_process_obj[NB_AD_OBJ-1];
   
   do
   { 
      p_obj->status = 0;
      p_obj->id_timer = UNDEFINED_TIMER;
   }
   while( p_obj-- != &s_process_obj[0] );
#endif
#endif
}


#pragma CODE = AUTO_DISP_TEMP_CODE00

//! @brief ad_alloc
//!
//! Alloc a graphical object in the automatic display process
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  obj_type : type of the graphical object to implement
//!         p_param_struct : pointer on the input parameter structure declared and filled in MMI
//!
//! @return id of the graphical object allocated  if success
//!         value UNDEF_AD_OBJ                 if failure
//!
U8 ad_allocate( const U8 obj_type, Ad_p_prm _MEM_TYPE_SLOW_ *p_param_struct )
{
#if( LCD_PRESENT==ENABLED )
#if 0
   U8 i;
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;

   p_obj = &s_process_obj[NB_AD_OBJ-1];
   i = NB_AD_OBJ-1;
   do
   {
      if( 0 == (p_obj->status & AD_MASK_USED) )
      {
         p_obj->status  |= AD_MASK_USED; // This timer slot is then used
         p_obj->id_appli = mmgr_get_process_id_curr_appli();
         p_obj->p_param  = p_param_struct;
         p_obj->obj_type = obj_type;
         if( obj_type == AD_ID_ANIMATION )
         {
            p_obj->id_timer = ts_alloc();
         }
         return(i+1); // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection
      }
      p_obj--;
   }
   while( i-- != 0 );
#endif
   //no timer slot free or duration egal to zero gives UNDEF_AD_OBJ as returned value 
#else
   if( obj_type );
   if( p_param_struct );
#endif
   return UNDEF_AD_OBJ; 
}

#if 0
//! @brief ad_free
//!
//! Set free a graphical object
//! DO NOT CALL THIS FUNCTION DIRECTLY : use the macro Ad_free() instead of it.
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the graphical object to get free
//!
//! @return none
//!
void _ad_free( U8 id )
{      
#if( LCD_PRESENT==ENABLED )
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;
   id -= 1; // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection   

   if( id >= NB_AD_OBJ )   { return; }
   p_obj = &s_process_obj[id];
   p_obj->status  = 0;
   if( p_obj->obj_type == AD_ID_ANIMATION )
      { Ts_free( p_obj->id_timer ); }
#else
   if( id );
#endif
}
#endif

#pragma CODE = AUTO_DISP_CODE

#if (ACCESS_DISPLAY == ENABLED)
//! @brief ad_start
//!
//! Activates a graphical object from its id
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the graphical object to start
//!
//! @return none
//!
void ad_start( U8 id )
{ 
#if( LCD_PRESENT==ENABLED )
#if 0
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;
   Ad_p_prm p;
   id -= 1; // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection      

   if( id >= NB_AD_OBJ )   { return; }
   p_obj = &s_process_obj[id];

   if( AD_MASK_RUN != (p_obj->status & AD_MASK_RUN) )
   {
      p_obj->status  |= (AD_MASK_RUN|AD_MASK_REFRESH);
   
      if( p_obj->obj_type == AD_ID_ANIMATION )
      {
         p.anim = (Ad_prm_animation*) p_obj->p_param;
         ts_set_time( p_obj->id_timer, (U16)TIMER_MS(10) * p.anim->time_10ms );
         p_obj->state_machine = 0;
      }
   }
   else
   {
      p_obj->status |= AD_MASK_REFRESH;
      ad_process();
   }
#endif
#else
   mail_send_event(EVT_ANIMATION, (id)<<8|ANIMATION_END);
#endif
}
#endif   //#if (ACCESS_DISPLAY == ENABLED)

#if 0
//! @brief ad_pause
//!
//! Suspends the current activation of a graphical object
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the graphical object to suspend
//!
//! @return none
//!
void ad_pause( U8 id)
{    
#if( LCD_PRESENT==ENABLED )
   id -= 1; // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection      

   if( id >= NB_AD_OBJ )   { return; }
   s_process_obj[id].status &= ~AD_MASK_RUN;
#else
   if( id );
#endif
}
#endif

//! @brief ad_stop
//!
//! Disables one of the graphical objects from its id
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the graphical object to stop
//!
//! @return none
//!
void ad_stop( U8 id )
{   
#if( LCD_PRESENT==ENABLED )
#if 0
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;
   id -= 1; // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection      

   if( id >= NB_AD_OBJ )   { return; }
   p_obj = &s_process_obj[id];

   p_obj->status &= ~AD_MASK_RUN;
   if( p_obj->obj_type == AD_ID_ANIMATION )
      { p_obj->state_machine = 0; }
#endif
#else
   if( id );
#endif
}

#if 0
//! @brief ad_refresh
//!
//! Refresh the graphical appparence of an object from its id
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  U8 id : id of the graphical object to refresh
//!
//! @return none
//!
void ad_refresh( U8 id)
{      
#if( LCD_PRESENT==ENABLED )
   id -= 1; // Object index : from 1 to NB_AD_OBJ; 0 is kept for no object selection      
   s_process_obj[id].status |= AD_MASK_REFRESH;
   ad_process();
#else
   if( id );
#endif
}
#endif

//! @brief ad_scan_evt
//!
//! Manage the behaviour of graphical objects
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  evt : input event to scan
//!
//! @return TRUE if event caught by the automatic display module
//!         else FALSE 
//!
Bool ad_scan_evt( const Msg evt )
{
#if( LCD_PRESENT==ENABLED )
#if 0
   Ad_obj_st _MEM_TYPE_SLOW_* p_obj;
   Ad_p_prm p;

   if( LSB(evt.id) == EVT_TIMER )
   {
      // Initialise the status of each timer
      p_obj = &s_process_obj[NB_AD_OBJ-1];
   
      do
      { 
         if( p_obj->id_timer == LSB(evt.param) )
         {
            p_obj->status |= (AD_MASK_UPDATE|AD_MASK_REFRESH);
            if( p_obj->obj_type == AD_ID_ANIMATION )
            {
               p.anim = (Ad_prm_animation*) p_obj->p_param;
               ts_set_time( p_obj->id_timer, (U16)TIMER_MS(10) * p.anim->time_10ms );
            }
            return TRUE;
         }
      }
      while( p_obj-- != &s_process_obj[0] );
   }
#endif
#else
   if( evt.id );
#endif
   return FALSE;
}

//! @brief ad_process
//!
//! Manage the behaviour of graphical objects
//!
//! @warning Code:XX bytes (function code length)
//!
//! @param  none
//!
//! @return none
//!
void ad_process( void )
{
#if( LCD_PRESENT==ENABLED )
#if 0
   Ad_obj_st   _MEM_TYPE_SLOW_*  p_obj;
   Ad_p_prm    _MEM_TYPE_SLOW_   p;
   U8          _MEM_TYPE_SLOW_   i = NB_AD_OBJ-1;
   
   do
   {
      p_obj = &s_process_obj[i];

      // This graphical object is active 
      if( (AD_MASK_RUN|AD_MASK_REFRESH) == (p_obj->status & (AD_MASK_RUN|AD_MASK_REFRESH)) )
      { 
         // Process animation objects
         if( p_obj->obj_type == AD_ID_ANIMATION )
         {
            p.anim = (Ad_prm_animation*) p_obj->p_param;
            if( (mmgr_is_appli_at_the_top(p_obj->id_appli))||(0!=(p.anim->mode & AD_DISP_OVER)) )
            {
               // Update consists to draw the following sub-image of the animation
               if( 0 != (p_obj->status & (AD_MASK_UPDATE)) )
               {
                  if( 0 != (p_obj->state_machine & AD_MASK_END_ANIM) )
                  {
                     switch( (p.anim->mode & AD_REPEAT_MODE_MASK) )
                     {
                        case AD_FOREVER:
                           p_obj->state_machine = 0;
                           break;
                        case AD_SINGLE:
                        default:
                           mail_send_event(EVT_ANIMATION, (i+1)<<8|ANIMATION_END);
                           p_obj->status &= ~(AD_MASK_RUN|AD_MASK_REFRESH);
                           break;
                     }
                  }
                  else
                  {
                     p_obj->state_machine++;
                  }
               }
			
               // Drawing done if refresh set
               if( 0 != (p_obj->status & (AD_MASK_REFRESH)) )
               {
                  ad_g_extra_param = p_obj->state_machine & (~AD_MASK_END_ANIM);

                  g_b_animation = TRUE;
                  lcd_draw_picture(p.anim->x, p.anim->y, p.anim->IDP);
                  g_b_animation = FALSE;

                  if( 0 != (p_obj->status & (AD_MASK_UPDATE)) )
                  {
                     p_obj->state_machine = ad_g_extra_param;
                  }
               }
            }
         }
         p_obj->status &= ~(AD_MASK_UPDATE|AD_MASK_REFRESH);   // Clear Refresh bit too
      }
   }
   while( i-- != 0 );
   ad_g_extra_param = 0; // To display a animation picture without animation
#endif
#endif

}



#endif   // _auto_disp_c_
