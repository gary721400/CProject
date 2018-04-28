#ifndef _TEXT_REMOTE_H_
#define _TEXT_REMOTE_H_

#define  DICREMOTE_Version_01       1
#define  DICREMOTE_Version_02       2

#define DICREMOTE_Version           DICREMOTE_Version_01
//#define DICREMOTE_Version           DICREMOTE_Version_02

#define  DICREMOTE_Code_None        0
#define  DICREMOTE_Code_Enter       1
#define  DICREMOTE_Code_VolUp       5
#define  DICREMOTE_Code_VolDown     6
#define  DICREMOTE_Code_PowerOff    7
#define  DICREMOTE_Code_NumStart    8
#define  DICREMOTE_Code_NumEnd      17
#define  DICREMOTE_Code_Add         60
#define  DICREMOTE_Code_Sub         61
#define  DICREMOTE_Code_AlphaStart  66
#define  DICREMOTE_Code_AlphaEnd    91
#define  DICREMOTE_Code_CancelInput 92
#define  DICREMOTE_Code_Exit        93
#define  DICREMOTE_Code_Backspace	182

#define  DICREMOTE_Code_Dot			183
#define  DICREMOTE_Code_Hyphen		3378
#define  DICREMOTE_Code_Space		3379

enum
{
   DICREMOTE_State_Idle,   //0
   DICREMOTE_State_Init,	//1
   DICREMOTE_State_WaitInput,	//2
	
   DICREMOTE_State_VoiceStart,	//3
   DICREMOTE_State_Voice,		//4
   DICREMOTE_State_VoiceEnd,	//5

   DICREMOTE_State_End,			//6

   DICREMOTE_State_Error		//7
};

enum
{
   DICREMOTE_Cmd_None,
   DICREMOTE_Cmd_PlayCodeVoice
};

typedef struct _DicRemoteHeader	//Version1
{
   U16   items_num;
   U32   voice_table_addr;
}DicRemoteHeader;

typedef struct _DicRemoteHeaderExtend
{
   U16   items_num;
   U32   items_addr;
}DicRemoteHeaderExtend;

typedef	struct _DicRemoteSpecialItem
{
   U16   oid_index;
   U16   remote_code;
}DicRemoteSpecialItem;

typedef struct _DicRemoteItem
{
   U16   oid_index;
   U16   remote_code;
   U32   voice_addr;
   U32   voice_size;
   U8    voice_codec;
}DicRemoteItem;

typedef struct _DicRemoteResult
{
   U16   remote_code_type;
   U16   remote_code;
}DicRemoteResult;

void              dic_remote_cmd_reset       ( void );
void              dic_remote_cmd_send        ( U8 id, U16 param );
Bool              dic_remote_get_info        ( void );
void              dic_remote_reset_info      ( void );
DicRemoteResult   *dic_remote_get_result     ( void );
Bool              dic_remote_search_result   ( void );
Bool              dic_remote_task_init       ( U32 remote_table_addr );
void              dic_remote_task_start      ( void );
void              dic_remote_task_stop       ( void );
/*Bool*/void      dic_remote_task            ( void );
#endif   //_TEXT_REMOTE_H_