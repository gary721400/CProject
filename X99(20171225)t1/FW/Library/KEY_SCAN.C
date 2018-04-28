/*******************************************************************
 * Copyright (c) 2008
 * King Billion Electronics Co., Ltd.
 * File Name	: KEY_SCAN.C
 * Author		: Nick
 * Create Date	: January 12, 2009
 * History		: Please Read the History.txt File.
 * Comment      :  
 *******************************************************************/

//------------------------------------------------------------------------------
#include <sfr.h>
#include "KEY_SCAN.H"

//------------------------------------------------------------------------------
// Option
//------------------------------------------------------------------------------
#define DEBOUNCE_TIME (50)    // millisecond 
#define LONG_KEY_TIME (2000)  // millisecond 
#define GAP_KEY_TIME (200)    // millisecond 
#define KEY_QUEUE_SIZE  10

//------------------------------------------------------------------------------
//  SPACE_XRAM
//------------------------------------------------------------------------------
static __X int Backup_Key;
static __X int KeyScan_Count;
static __X int KeyGap_Count;
static __X tKEYSCAN_TYPE Key_Status;
static __X unsigned int KeyScan_Time;
static __X int Queue_Left;
static __X int Queue_Right;
static __X int KeyScan_Queue[KEY_QUEUE_SIZE];

//------------------------------------------------------------------------------
#pragma DATA= KEY_SCAN_DATA
//------------------------------------------------------------------------------
static int (*pKeyScan_Phy_Func)(void);

//------------------------------------------------------------------------------
#pragma CODE= KEY_SCAN_INIT
//------------------------------------------------------------------------------
void KeyScan_Init(int (*func_ptr)(void),
                  unsigned int Scan_Time)
{
    int i;
    
    KeyScan_Count = 0;
    KeyGap_Count = 0;
    Backup_Key = 0;
    KeyScan_Time = Scan_Time;
    Key_Status = KEY_NO_Press;
    Queue_Left = 0;
    Queue_Right = 0;
    for(i=0;i<KEY_QUEUE_SIZE;i++)
        KeyScan_Queue[i] = -1;
    pKeyScan_Phy_Func = func_ptr;
}

//------------------------------------------------------------------------------
#pragma CODE= KEY_SCAN_CODE
//------------------------------------------------------------------------------
int KeyScan_GetCode(void)
{
    int ret;
    
    if(Queue_Left != Queue_Right)
    {
        ret = KeyScan_Queue[Queue_Left];
        KeyScan_Queue[Queue_Left] = -1;
        Queue_Left++;
        if(Queue_Left>=KEY_QUEUE_SIZE)
            Queue_Left = 0;
        return ret; 
    }
    return(-1);
}

//------------------------------------------------------------------------------
static void Push_KeyCode_to_Queue(int KeyCode)
{
    KeyScan_Queue[Queue_Right++] = (KeyCode|Key_Status);
    if(Queue_Right>=KEY_QUEUE_SIZE)
        Queue_Right = 0;
    if(Queue_Right == Queue_Left)
    {
        if(Queue_Right==0)
            Queue_Right = KEY_QUEUE_SIZE-1;
        else
            Queue_Right--;
    }
}

//------------------------------------------------------------------------------
void KeyScan_CallBack(void)
{
    int ret;
    int push_key_flag;
    
    push_key_flag = 0;
    ret = pKeyScan_Phy_Func();
    if(ret>0)
    {
        if(KeyScan_Count<=(DEBOUNCE_TIME+LONG_KEY_TIME)/KeyScan_Time)
            KeyScan_Count++;
        if(KeyScan_Count > (DEBOUNCE_TIME+LONG_KEY_TIME)/KeyScan_Time)
        {
            KeyGap_Count++;
            if(KeyGap_Count>GAP_KEY_TIME/KeyScan_Time)
            {
                KeyGap_Count = 0;
                Key_Status = KEY_Gap;
                push_key_flag = 1;                
            }
        }
        else if(KeyScan_Count == DEBOUNCE_TIME/KeyScan_Time)
        {
            Backup_Key = ret;
            Key_Status = KEY_Press;
            push_key_flag = 1;
        }
        else if(KeyScan_Count == (DEBOUNCE_TIME+LONG_KEY_TIME)/KeyScan_Time)
        {
            Key_Status = KEY_Long;
            push_key_flag = 1;
        }
        if(push_key_flag && Backup_Key == ret)
        {
            push_key_flag = 0;
            Push_KeyCode_to_Queue(ret);
        }
    }
    else
    {
        if(Key_Status != KEY_NO_Press)
        {
            if(Key_Status == KEY_Press)
                Key_Status = KEY_Short;
            else
                Key_Status = KEY_Release;
            Push_KeyCode_to_Queue(Backup_Key);
        }
        Key_Status = KEY_NO_Press;
        KeyScan_Count=0;
    }
}

//------------------------------------------------------------------------------
//  End Of File
//------------------------------------------------------------------------------