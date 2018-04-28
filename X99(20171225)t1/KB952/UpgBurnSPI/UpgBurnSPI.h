//--------------------------------------------------------------------
#define BUF_B4KB   8
#define BUF_SIZW   256
#define BUF_SIZB  (BUF_SIZW*2)
//------
#define CMD_ID1   0x20141120UL
#define CMD_ID2   0x36576646UL
#define CMD_BURN  0x0402
#define CMD_RESET 0x4020
//--------------------------------------------------------------------
typedef struct
{
	unsigned long Signature1;
	unsigned long Signature2;
	unsigned long Command;
}BHCBW;
//--------------------------------------------------------------------
