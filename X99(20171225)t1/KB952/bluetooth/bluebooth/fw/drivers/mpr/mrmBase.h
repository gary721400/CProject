#ifndef __9C7B6ACF_2660_4fb5_BB40_FD6C47BC5BD9__
#define __9C7B6ACF_2660_4fb5_BB40_FD6C47BC5BD9__

#ifdef __cplusplus
extern "C"
{
#endif

#define mrmLittleEndian

#ifdef mrmLittleEndian
    #define mrmEndianHigh 1
    #define mrmEndianLow  0
#else
    #define mrmEndianHigh 0
    #define mrmEndianLow  1
#endif

#define mrmTrue  0x01
#define mrmFalse 0x00

typedef char mrmSByte;              /* 8位有符号整数 */
typedef unsigned char mrmByte;      /* 8位无符号整数 */
typedef mrmByte mrmBool;            /* bool类型 */

typedef short mrmSWord;             /* 16位有符号整数 */
typedef unsigned short mrmWord;     /* 16位无符号整数 */

typedef long mrmSDword;             /* 32位有符号整数 */
typedef unsigned long mrmDword;     /* 32位无符号整数 */

typedef mrmWord* mrmWString;        /* 宽字符串 */
typedef const mrmWord* mrmWCString; /* 宽字符串常量 */
typedef char*   mrmAString;         /* ascii 字符串 */
typedef const char*   mrmACString;  /* ascii 字符串常量 */

/*
 *
 * UNICODE时:
 *  mrmTChar    宽字符
 *  mrmTString  宽字符串
 *  mrmTCString 宽字符串常量
 * 否则:
 *  mrmTChar    ascii字符
 *  mrmTString  ascii字符串
 *  mrmTCString ascii字符串常量
 *
 */
#ifdef UNICODE
typedef mrmWord mrmTChar;
typedef mrmWString mrmTString;
typedef mrmWCString mrmTCString;

#define mrmText(s) L ## s
#else
typedef char mrmTChar;
typedef mrmAString mrmTString;
typedef mrmACString mrmTCString;

#define mrmText(s) s
#endif

/* SDK包执行结果 */
typedef char mrmResult;

#define mrmResultOk                   0   /* 成功 */
#define mrmResultNoMemory            1   /* 内存不足 */
#define mrmResultNoResource          2   /* 资源不足 */
#define mrmResultInvalidParams       3   /* 无效的参数 */
#define mrmResultOutOfIndex          4   /* 索引超出范围 */
#define mrmResultOutOfRange          5   /* 不在范围内 */
#define mrmResultIOError             6   /* 输入输出错误 */
#define mrmResultInvalidOperate     7   /* 无效的操作 */
#define mrmResultTimeout             8   /* 超时 */
#define mrmResultNotImplement       9   /* 没实现 */
#define mrmResultNotAuth             10  /* 没验证 */
#define mrmResultNoData              11  /* 无数据 */
#define mrmResultNotExisted          12  /* 不存在 */
#define mrmResultInvalidFormat       13  /* 无效的格式 */
#define mrmResultNotSurportVer       14  /* 不支持的版本 */
#define mrmResultNotCompatibleVer    15  /* 不兼容的版本 */

#define mrmResultIsOk(ret) ((ret) <= 0)     /* 执行成功 */
#define mrmResultIsNotOk(ret) ((ret) > 0)   /* 执行失败 */

#define mrmOffset(type, member) ((mrmDword)((mrmByte*)&(((type*)16)->member) - 16))
#define mrmCountOf(array) ((mrmDword)(sizeof(array)/sizeof( (array)[0]) ))

/* 模组工作模式 */
typedef enum
{
    mrmModeAuto = 0,  /* 自动工作模式 */
    mrmModeManual = 1 /* 手动工作模式 */
    
} mrmMode;

/* MPR解码芯片工作状态 */
typedef enum
{
    mrmStatusIdle = 0,    /* 空闲状态 */
    mrmStatusNormal,      /* 常规状态 */
    mrmStatusUnnormal     /* 非常规状态 */
    
} mrmStatus;

/* MPR 信息码 */
enum { MprInfoCodeBytes = 7 };

typedef mrmByte MprInfoCode[MprInfoCodeBytes + 1];

typedef void (*mrmPSimpleFunc)(void);   /* 简单函数指针类型 */
typedef void (*mrmPCallBack)(void* pObj, void* pArg);   /* 回调函数指针类型 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
