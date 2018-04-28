//!
//! @file recorder_task.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file is the entry point of the recorder task.
//!
//! @version 2.0 snd3-dvk-1_9_5 $Id: recorder_task.h,v 2.0 2007/03/30 12:11:45 sguyon Exp $
//!
//! @todo
//! @bug
#ifndef _RECORDER_TASK_H_
#define _RECORDER_TASK_H_

void recorder_task_init    (void);
void recorder_task_start   (void);
void recorder_task_stop    (void);
void recorder_task         (void);

void  record_process       ( void );


#endif  // _RECORDER_TASK_H_ 
