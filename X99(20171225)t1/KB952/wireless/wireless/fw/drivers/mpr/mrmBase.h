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

typedef char mrmSByte;              /* 8λ�з������� */
typedef unsigned char mrmByte;      /* 8λ�޷������� */
typedef mrmByte mrmBool;            /* bool���� */

typedef short mrmSWord;             /* 16λ�з������� */
typedef unsigned short mrmWord;     /* 16λ�޷������� */

typedef long mrmSDword;             /* 32λ�з������� */
typedef unsigned long mrmDword;     /* 32λ�޷������� */

typedef mrmWord* mrmWString;        /* ���ַ��� */
typedef const mrmWord* mrmWCString; /* ���ַ������� */
typedef char*   mrmAString;         /* ascii �ַ��� */
typedef const char*   mrmACString;  /* ascii �ַ������� */

/*
 *
 * UNICODEʱ:
 *  mrmTChar    ���ַ�
 *  mrmTString  ���ַ���
 *  mrmTCString ���ַ�������
 * ����:
 *  mrmTChar    ascii�ַ�
 *  mrmTString  ascii�ַ���
 *  mrmTCString ascii�ַ�������
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

/* SDK��ִ�н�� */
typedef char mrmResult;

#define mrmResultOk                   0   /* �ɹ� */
#define mrmResultNoMemory            1   /* �ڴ治�� */
#define mrmResultNoResource          2   /* ��Դ���� */
#define mrmResultInvalidParams       3   /* ��Ч�Ĳ��� */
#define mrmResultOutOfIndex          4   /* ����������Χ */
#define mrmResultOutOfRange          5   /* ���ڷ�Χ�� */
#define mrmResultIOError             6   /* ����������� */
#define mrmResultInvalidOperate     7   /* ��Ч�Ĳ��� */
#define mrmResultTimeout             8   /* ��ʱ */
#define mrmResultNotImplement       9   /* ûʵ�� */
#define mrmResultNotAuth             10  /* û��֤ */
#define mrmResultNoData              11  /* ������ */
#define mrmResultNotExisted          12  /* ������ */
#define mrmResultInvalidFormat       13  /* ��Ч�ĸ�ʽ */
#define mrmResultNotSurportVer       14  /* ��֧�ֵİ汾 */
#define mrmResultNotCompatibleVer    15  /* �����ݵİ汾 */

#define mrmResultIsOk(ret) ((ret) <= 0)     /* ִ�гɹ� */
#define mrmResultIsNotOk(ret) ((ret) > 0)   /* ִ��ʧ�� */

#define mrmOffset(type, member) ((mrmDword)((mrmByte*)&(((type*)16)->member) - 16))
#define mrmCountOf(array) ((mrmDword)(sizeof(array)/sizeof( (array)[0]) ))

/* ģ�鹤��ģʽ */
typedef enum
{
    mrmModeAuto = 0,  /* �Զ�����ģʽ */
    mrmModeManual = 1 /* �ֶ�����ģʽ */
    
} mrmMode;

/* MPR����оƬ����״̬ */
typedef enum
{
    mrmStatusIdle = 0,    /* ����״̬ */
    mrmStatusNormal,      /* ����״̬ */
    mrmStatusUnnormal     /* �ǳ���״̬ */
    
} mrmStatus;

/* MPR ��Ϣ�� */
enum { MprInfoCodeBytes = 7 };

typedef mrmByte MprInfoCode[MprInfoCodeBytes + 1];

typedef void (*mrmPSimpleFunc)(void);   /* �򵥺���ָ������ */
typedef void (*mrmPCallBack)(void* pObj, void* pArg);   /* �ص�����ָ������ */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
