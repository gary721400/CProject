//!
//! @file mail.c,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of the mailbox
//!
//! Each application can send to the external world some events with parameters
//! Those events will be stored within a mailbox defined here.
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: mail.c,v 2.0 2007/03/30 12:12:48 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _mail_c_
#define _mail_c_

//_____  I N C L U D E S ___________________________________________________
#include "config.h"
#include "mail.h"
#include "mail_evt.h"
#include "mail_cmd.h"

#pragma DATA = MAIL_DATA
#pragma CODE = MAIL_CODE

//_____ P R I V A T E   D E C L A R A T I O N S ______________________________
//! Globale array used to store all of the events send by anybody
//! MAILBOX_EVT_SIZE_MAX is the max value of event that we can store.
/*static*/ Msg       _MEM_TYPE_MALLOC_MAIL_ s_mailbox_evt[MAILBOX_EVT_SIZE_MAX];
/*static*/ Mail_desc _MEM_TYPE_MALLOC_MAIL_ s_mailboxdesc_evt={0, 0};

//! Globale array used to store all of the commands send by anybody
//! MAILBOX_CMD_SIZE_MAX is the max value of event that we can store.
/*static*/ Msg       _MEM_TYPE_MALLOC_MAIL_ s_mailbox_cmd[MAILBOX_CMD_SIZE_MAX];
/*static*/ Mail_desc _MEM_TYPE_MALLOC_MAIL_ s_mailboxdesc_cmd={0, 0};


//_____ P R I V A T E   D E C L A R A T I O N S ______________________________
static  U8             mail_cmd_free_room_get( void );
static  U8             mail_evt_free_room_get( void );
static  U8             mail_cmd_used_room_get( void );


//! @brief Return the free room of the Command mailbox, in number of message.
//!
//! @param none
//!
//! @return the size in number of message.
static U8 mail_cmd_free_room_get( void )
{
   return
      MAILBOX_CMD_SIZE_MAX -1        // FIFO size minus one element (it can not be hundred percent full)
   -  mail_cmd_used_room_get();      // ... minus the used room.
}

//! @brief Return the free room of the Evenement mailbox, in number of message.
//!
//! @param none
//!
//! @return the size in number of message.
static U8 mail_evt_free_room_get( void )
{
   return
      MAILBOX_EVT_SIZE_MAX -1        // FIFO size minus one element (it can not be hundred percent full)
   -  mail_evt_used_room_get();      // ... minus the used room.
}

//! @brief Return the used room of the Command mailbox, in number of message.
//!
//! @param none
//!
//! @return the size in number of message.
static U8 mail_cmd_used_room_get( void )
{
   return (s_mailboxdesc_cmd.wr_ofst - s_mailboxdesc_cmd.rd_ofst) & (MAILBOX_CMD_SIZE_MAX -1) ;
}

//! @brief Return the used room of the Evenement mailbox, in number of message.
//!
//! @param none
//!
//! @return the size in number of message.
U8 mail_evt_used_room_get( void )
{
   return (s_mailboxdesc_evt.wr_ofst - s_mailboxdesc_evt.rd_ofst) & (MAILBOX_EVT_SIZE_MAX -1) ;
}

//! @brief Push a message in top of the Command mailbox to a specific task.
//!
//! @param id       Id of the message.
//! @param param    Parameter of the message
//!
//! @return PASS: ok, the message is pushed; MAILBOX_STACK_FULL if the mailbox is full.
Status_mailbox mail_send_command(
   U16   id
,  U16   param
) {
   U8 wr;
   if ( mail_cmd_free_room_get() )
   {
      // Add our command in the stack
      wr = s_mailboxdesc_cmd.wr_ofst;
      s_mailbox_cmd[wr].id      =  id;
      s_mailbox_cmd[wr].param   =  param;
      wr += 1 ;
      s_mailboxdesc_cmd.wr_ofst = wr & (MAILBOX_CMD_SIZE_MAX -1) ;//8&7=0
      return PASS ;
   } else {
      mail_send_event(EVT_MAILBOX_CMD_FULL, 0);
      return MAILBOX_STACK_FULL ;
   }
}

//! @brief Push a message in top of the Evenement mailbox to the MMI.
//!
//! @param id       Id of the message.
//! @param param    Parameter of the message
//!
//! @return PASS: ok, the message is pushed; MAILBOX_STACK_FULL if the mailbox is full.
Status_mailbox mail_send_event(
   U8 id
,  U16 param
) {
   // Push an event towards the MMI
   // For that case, we need to integrate inside the eventId, the
   // id of the MMI task because that our politic to send only
   // events towards MMI
   //
   Status_mailbox status;
   U8  wr= s_mailboxdesc_evt.wr_ofst;

   if ( mail_evt_free_room_get() )
   {
      status = PASS ;

   } else {
      wr --;                               // Point on top event and corrupt it
      wr &= (MAILBOX_EVT_SIZE_MAX -1);
      if( id!=EVT_MAILBOX_CMD_FULL )   { id=EVT_MAILBOX_EVT_FULL; }
      status = MAILBOX_STACK_FULL ;
   }

   // Add our event/cmd in the stack
   s_mailbox_evt[wr].id      =  (U16)id;
   s_mailbox_evt[wr].param   =  param;
   wr += 1 ;
   s_mailboxdesc_evt.wr_ofst = wr & (MAILBOX_EVT_SIZE_MAX -1)  ;
   return status ;
}

//! @brief Pop a message from the top of the Command mailbox.
//!
//! @param task_id  Id of the task that shall match the message.
//! @param p_msg    Id and Parameter of the message are returned here
//!
//! @return PASS: ok, the message is pushed; MAILBOX_NO_MSG if the mailbox is empty,
//!               or does not contain a message for that task.
Status_mailbox mail_get_command(
   U8   task_id
,  Msg _MEM_TYPE_MALLOC_MAIL_ * p_msg
) {
   U8 rd;
   if( mail_cmd_used_room_get() )
   {
      if( MSB(s_mailbox_cmd[s_mailboxdesc_cmd.rd_ofst].id) == task_id )
      {
         rd     = s_mailboxdesc_cmd.rd_ofst;
         *p_msg = s_mailbox_cmd[rd];
         rd += 1 ;
         s_mailboxdesc_cmd.rd_ofst = rd & (MAILBOX_CMD_SIZE_MAX -1) ;
         return PASS ;
      }
   }
   p_msg->id       =  0;
   p_msg->param    =  0;
   return MAILBOX_NO_MSG;
}

//! @brief Pop a message from the top of the Evenement mailbox.
//!
//! @param task_id  Id of the task that shall match the message.
//! @param p_msg    Id and Parameter of the message are returned here
//!
//! @return PASS: ok, the message is pushed; MAILBOX_NO_MSG if the mailbox is empty,
//!               or does not contain a message for that task.
Status_mailbox mail_get_event(
   Msg _MEM_TYPE_MALLOC_MAIL_ * p_msg
) {
   U8 rd;
   if( mail_evt_used_room_get() )
   {
      rd     = s_mailboxdesc_evt.rd_ofst;
      *p_msg = s_mailbox_evt[rd];
      rd += 1 ;														
      s_mailboxdesc_evt.rd_ofst = rd & (MAILBOX_EVT_SIZE_MAX -1) ;//8&7=0
      return PASS ;
   }
   p_msg->id       =  0;
   p_msg->param    =  0;
   return MAILBOX_NO_MSG ;
}

#endif // _mail_c_
