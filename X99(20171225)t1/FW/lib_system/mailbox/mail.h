//!
//! @file mail.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the definition of mailboxs: one for event and one
//!        for commands
//!
//! Each system task can send to the MMI world some EVENTS with a parameter.
//! The MMI world can send to itself some events but can only send some
//! COMMANDS to the system tasks.
//! Those events and commands will be stored within 2 mailboxs defined here.
//!
//! @version 1.14 snd3-dvk-1_9_5 $Id: mail.h,v 1.14 2007/06/25 09:34:14 sguyon Exp $
//!
//! @todo
//! @bug
//!
#ifndef _MAIL_H_
#define _MAIL_H_

#undef _GLOBEXT_
#if (defined _mail_c_)
#  define _GLOBEXT_
#else
#  define _GLOBEXT_ extern
#endif

//_____ I N C L U D E S ____________________________________________________
#include "config.h"


//_____ M A C R O S ________________________________________________________

//!  Definition of Task ID. This ID is used to properly send the command to a
//!  specific task.
//!  The name of the define can be changed by another define. That customization
//!  should be done in the file mailcmd.h
#define TASK_DUMMY   0x00           // This define is mandatory
#define TASK_0       0x01
#define TASK_1       0x02
#define TASK_2       0x04
#define TASK_3       0x08
#define TASK_4       0x10
#define TASK_5       0x20
#define TASK_6       0x40
#define TASK_7       0x80

// This define is mandatory
#define ALL_TASK     (TASK_0|TASK_1|TASK_2|TASK_3|TASK_4|TASK_5|TASK_6|TASK_7)
//! End Task ID

//! The max value of event that we can store.
#define MAILBOX_EVT_SIZE_MAX  16        //! Must be a power of 2. The Max size of the mailbox for events.

//! The max value of commands that we can store.
#define MAILBOX_CMD_SIZE_MAX  8        //! Must be a power of 2. The Max size of the mailbox for cmds.

//! Possible return message when looking inside the mailbox
typedef enum
{
   MAILBOX_OK=PASS
,  MAILBOX_NO_MSG=FAIL      //! The mailbox is empty
,  MAILBOX_STACK_FULL       //! The mailbox is full
,  MAILBOX_ERROR            //! The stack operation was not a success
} Status_mailbox;

//! To know which mailbox the software is using
#define MAILBOX_USE_CMD       0x01     //! To use commands mailbox
#define MAILBOX_USE_EVT       0x02     //! To use eventss mailbox

//_____ D E F I N I T I O N S ______________________________________________
//! For us, a message is a command or a message.
//! A message needs to be formatted under the following structure
typedef struct
{
   U16   id;           //! The Id of the event
   U16   param;        //! The parameter of the message
}Msg;

typedef struct s_Maildesc
{
   U8    rd_ofst    ;
   U8    wr_ofst    ;
   //U8    size_mask  ; // This field is static
} Mail_desc ;

_GLOBEXT_   U8             mail_evt_used_room_get  ( void );
_GLOBEXT_   Status_mailbox mail_get_event          ( Msg _MEM_TYPE_MALLOC_MAIL_ * p_msg);
_GLOBEXT_   Status_mailbox mail_get_command        ( U8  task_id, Msg _MEM_TYPE_MALLOC_MAIL_ * p_msg);
_GLOBEXT_   Status_mailbox mail_send_event         ( U8  evt_id, U16 param);
_GLOBEXT_   Status_mailbox mail_send_command       ( U16 cmd_id, U16 param);



#endif  //! _MAIL_H_
