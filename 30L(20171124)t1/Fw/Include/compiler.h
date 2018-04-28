//! @file compiler.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file redefines dedicated KEIL, RAISONANCE, TASKING,
//! _IAR_AVR_(__IAR_SYSTEMS_ASM__ and __ICCAVR__)
//! keywords in order to ensure that any source file can be processed by
//! these compilers.
//!
//! @version 1.42 snd3-dvk-1_9_5 $Id: compiler.h,v 1.42 2006/07/20 15:40:45 ppatron Exp $
//!
//! @todo
//! @bug

#ifndef _COMPILER_H_
#define _COMPILER_H_

//_____ D E C L A R A T I O N S ____________________________________________



#ifndef ASM_INCLUDE // define ASM_INCLUDE in your assembly source before including any .h file

typedef float               Float16;

typedef unsigned char       U8 ;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef signed char         S8 ;
typedef signed short        S16;
typedef signed long         S32;

typedef unsigned char       Bool;

typedef U8                  Status;
typedef Bool                Status_bool;
#define PASS 0
#define FAIL 1
#define LOW_LEVEL  0
#define HIGH_LEVEL 1





typedef union
{
  U32 dw ; // obsolete for new firmware 
           // dw (double word) add both definition (l & dw) for compatibility 
  U32 l  ; // l changed in dw (double word) because l is used for signed long...
  U16 w[2];
  U8  b[4];
} Union32;



typedef union
{
  U16 w   ;
  U8  b[2];
} Union16;
#endif // ASM_INCLUDE



#ifndef _TYPE_DEF_
#define _TYPE_DEF_
typedef unsigned char  BOOLEAN;     //!< BOOLEAN(TURE/FALSE) 8 bit quantity 
typedef unsigned char  INT8U;       //!< Unsigned  8 bit quantity             
typedef signed   char  INT8S;       //!< Signed    8 bit quantity             
typedef unsigned int   INT16U;      //!< Unsigned 16 bit quantity             
typedef signed   int   INT16S;      //!< Signed   16 bit quantity             
typedef unsigned long  INT32U;      //!< Unsigned 32 bit quantity             
typedef signed   long  INT32S;      //!< Signed   32 bit quantity             
typedef float          FP32;        //!< Single precision floating point      
typedef double         FP64;        //!< Double precision floating point      

typedef unsigned char  UCHAR;
typedef  unsigned long  Dword;
typedef  unsigned int   USHORT;
typedef  unsigned long  ULONG;
//typedef  unsigned char  UCHAR;
typedef  unsigned int   WORD;

#endif


#define _MEM_TYPE_SLOW_ __X
#define _MEM_TYPE_FAST_ __X
#define xdata 			__X
#define	_MEM_TYPE_BOOL_	_MEM_TYPE_FAST_


#define  G_STRING_SIZE     (30 *2)
#define  G_STRING_SIZE2    (30 *2)

#define	UnicodeBuffer	U8 _MEM_TYPE_SLOW_     
#define	_UnicodeBuffer	U8 

//_____ M A C R O S ________________________________________________________

// Some usefull macros...
#ifndef ASM_INCLUDE
#  define Max(a, b)            ( (a)>(b) ? (a) : (b) )       // Take the max between a and b
#  define Min(a, b)            ( (a)<(b) ? (a) : (b) )       // Take the min between a and b

// Align on the upper value <val> on a <n> boundary
// i.e. Upper(0, 4)= 4
//      Upper(1, 4)= 4
//      Upper(2, 4)= 4
//      Upper(3, 4)= 4
//      Upper(4, 4)= 8
//      ../..
#  define Upper(val, n)        ( ((val)+(n)) & ~((n)-1) )

// Align up <val> on a <n> boundary
// i.e. Align_up(0, 4)= 0
//      Align_up(1, 4)= 4
//      Align_up(2, 4)= 4
//      Align_up(3, 4)= 4
//      Align_up(4, 4)= 4
//      ../..
#  define Align_up(val, n)     ( ((val)+(n)-1) & ~((n)-1) )

// Align down <val> on a <n> boundary
// i.e. Align_down(0, 4)= 0
//      Align_down(1, 4)= 0
//      Align_down(2, 4)= 0
//      Align_down(3, 4)= 0
//      Align_down(4, 4)= 4
//      ../..
#  define Align_down(val, n)   (  (val)        & ~((n)-1) )
#endif // ASM_INCLUDE

// little-big endian management
#define INTEL_ALIGNMENT     LITTLE_ENDIAN
#define MOTOROLA_ALIGNMENT  BIG_ENDIAN

// U16/U32 endian handlers
#ifdef LITTLE_ENDIAN     // => 16bit: (LSB,MSB), 32bit: (LSW,MSW) or (LSB0,LSB1,LSB2,LSB3) or (MSB3,MSB2,MSB1,MSB0)
#  define MSB(u16)        (((U8* )&u16)[1])
#  define LSB(u16)        (((U8* )&u16)[0])
#  define MSW(u32)        (((U16*)&u32)[1])
#  define LSW(u32)        (((U16*)&u32)[0])
#  define MSB0(u32)       (((U8* )&u32)[3]) // MSB of u32
#  define MSB1(u32)       (((U8* )&u32)[2])
#  define MSB2(u32)       (((U8* )&u32)[1])
#  define MSB3(u32)       (((U8* )&u32)[0]) // LSB of u32
#  define LSB0(u32)       MSB3(u32)         // LSB of u32
#  define LSB1(u32)       MSB2(u32)                      
#  define LSB2(u32)       MSB1(u32)                      
#  define LSB3(u32)       MSB0(u32)         // MSB of u32
#else // BIG_ENDIAN         => 16bit: (MSB,LSB), 32bit: (MSW,LSW) or (LSB3,LSB2,LSB1,LSB0) or (MSB0,MSB1,MSB2,MSB3)
#  define MSB(u16)        (((U8* )&u16)[0])
#  define LSB(u16)        (((U8* )&u16)[1])
#  define MSW(u32)        (((U16*)&u32)[0])
#  define LSW(u32)        (((U16*)&u32)[1])
#  define MSB0(u32)       (((U8* )&u32)[0]) // MSB of u32
#  define MSB1(u32)       (((U8* )&u32)[1])
#  define MSB2(u32)       (((U8* )&u32)[2])
#  define MSB3(u32)       (((U8* )&u32)[3]) // LSB of u32
#  define LSB0(u32)       MSB3(u32)         // LSB of u32
#  define LSB1(u32)       MSB2(u32)
#  define LSB2(u32)       MSB1(u32)
#  define LSB3(u32)       MSB0(u32)         // MSB of u32
#endif

// Endian converters
#define Le16(b)                        \
   (  ((U16)(     (b) &   0xFF) << 8)  \
   |  (     ((U16)(b) & 0xFF00) >> 8)  \
   )
#define Le32(b)                             \
   (  ((U32)(     (b) &       0xFF) << 24)  \
   |  ((U32)((U16)(b) &     0xFF00) <<  8)  \
   |  (     ((U32)(b) &   0xFF0000) >>  8)  \
   |  (     ((U32)(b) & 0xFF000000) >> 24)  \
   )

// host to network conversion: used for Intel HEX format, TCP/IP, ...
// Convert a 16-bit value from host-byte order to network-byte order
// Standard Unix, POSIX 1003.1g (draft)
#ifdef LITTLE_ENDIAN
#  define htons(a)    LE16(a)
#  define ntohs(a)    htons(a)
#  define htonl(a)    LE32(a)
#  define ntohl(a)    htonl(a)
#else
#  define htons(a)    (a)
#  define ntohs(a)    (a)
#  define htonl(a)    (a)
#  define ntohl(a)    (a)
#endif

// Memory Type Location
#ifndef _CONST_TYPE_
#  define _CONST_TYPE_ code
#endif
#ifndef _MEM_TYPE_
#  define _MEM_TYPE_
#endif

// Constants
#define ENABLE   1
#define ENABLED  1
#define DISABLED 0
#define DISABLE  0
#define FALSE   (0==1)
#define TRUE    (1==1)
#define KO      0
#define OK      1

#define true	(1==1)
#define false   (0==1)

#define boolean	unsigned char

#ifndef ASM_INCLUDE
#  define CLR     0
#  define SET     1
#endif





// Bit and bytes manipulations
#ifndef ASM_INCLUDE
#  define Low(U16)                ((U8)U16)
#  define High(U16)               ((U8)(U16>>8))
#  define Tst_bit_x(addrx,mask)   (*addrx & mask)
#  define Set_bit_x(addrx,mask)   (*addrx = (*addrx |  mask))
#  define Clr_bit_x(addrx,mask)   (*addrx = (*addrx & ~mask))
#  define Out_x(addrx,value)      (*addrx = value)
#  define In_x(addrx)             (*addrx)
#endif // ASM_INCLUDE

//! @brief Call the routine at address addr: generate an Assembly LCALL addr opcode.
//!        Example: Long_call(0); // Software reset (if no IT used before)
//!
//! NOTE:
//! May be used as a long jump opcode in some special cases
//!
//! @param addr   address of the routine to call.
#define Long_call(addr)         ((*(void (_CONST_TYPE_*)(void))(addr))())


// The next definitions are obsoletes
// ----------------------------------
#ifndef ASM_INCLUDE
typedef unsigned char       Uint8;   // Obsolete. Please use U8
typedef unsigned char       Uchar;   // Obsolete. Please use U8
typedef unsigned char       Byte;    // Obsolete. Please use U8
typedef unsigned int        Uint16;  // Obsolete. Please use U16
typedef unsigned long int   Uint32;  // Obsolete. Please use U32
typedef char                Int8;    // Obsolete. Please use S8
typedef int                 Int16;   // Obsolete. Please use S16
typedef long int            Int32;   // Obsolete. Please use S32
typedef unsigned int        Word;    // Obsolete. Please use U16
typedef unsigned long int   DWord;   // Obsolete. Please use U32 S32


#  define LOW(U16)                ((U8)U16)                     // Obsolete. Please use Low(x)
#  define HIGH(U16)               ((U8)(U16>>8))                // Obsolete. Please use High(x)
#  define TST_BIT_X(addrx,mask)   (*addrx & mask)               // Obsolete. Please use Tst_bit_x
#  define SET_BIT_X(addrx,mask)   (*addrx = (*addrx |  mask))   // Obsolete. Please use Set_bit_x
#  define CLR_BIT_X(addrx,mask)   (*addrx = (*addrx & ~mask))   // Obsolete. Please use Clr_bit_x
#  define OUT_X(addrx,value)      (*addrx = value)              // Obsolete. Please use Out_x
#  define IN_X(addrx)             (*addrx)                      // Obsolete. Please use In_x

// Memory Type Location
#  ifndef _ConstType_
#     define _ConstType_ code
#  endif

#  ifndef _MemType_
#     define _MemType_
#  endif

#  ifndef _GenericType_
#     define _GenericType_
#  endif

#endif // ASM_INCLUDE


//! The default value to initialise pointers
#if (!defined NULL)
#  define  NULL           ((void *)0)
#endif


#ifdef KEIL               // KEIL compiler

#  define Reentrant(x)    x reentrant
#  define Sfr(x,y)        sfr x = y
#  define Sfr16(x,y)      sfr16 x = y
#  define Sbit(x,y,z)     sbit x = y ^ z
#  define Interrupt(x,y)  x interrupt y
#  define At(x)           _at_ x

#  ifdef __C51__          // C51 Compiler
#     define far  xdata   // far is for C251 only
#  endif
#endif                    // End of KEIL



#ifdef RAISONANCE         // RAISONANCE compiler

#  define Reentrant(x)    x reentrant
#  define Sfr(x,y)        sfr x = y
#  define Sbit(x,y,z)     sbit x = y ^ z
#  define Interrupt(x,y)  x interrupt y
#  define At(x)           _at_ x

#  ifdef __C51__          // C51 Compiler
#     define far  xdata   // far is for 251 only
#  endif
#endif                    // End of RAISONANCE



#ifdef TASKING            // TASKING compiler

#  include <keil.h>       // Include Tasking / Keil compatibility header file
#  define far             _far
#  define Reentrant(x)    _reentrant x
#  define Sfr(x,y)        _sfrbyte x _at(y)
#  define Sbit(x,y,z)     _sfrbit x _atbit(y,z)
#  define Interrupt(x,y)  _interrupt(y) x

#endif                    // End of TASKING



#ifdef _IAR_AVR_

#  define __IOMACRO_H
#  define TID_GUARD(proc) ((__TID__ & 0x7FF0) != ((90 << 8) | ((proc) << 4)))

#  if !(__IAR_SYSTEMS_ICC__ > 2) && !defined(__IAR_SYSTEMS_ASM__)
#     error This file should only be compiled with iccavr or aavr.
#  endif // !(__IAR_SYSTEMS_ICC__ > 2) && !defined __IAR_SYSTEMS_ASM__

// The assembler uses a special set of macros...
#  ifdef __IAR_SYSTEMS_ASM__

// Byte sized SFRs
#     define SFR_B_BITS(_NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H)\
         sfrb   _NAME = _ADDR

#     define SFR_B_BITS_EXT(_NAME,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H)\
         sfrb   _NAME = _ADDR

#     define SFR_B2_BITS(_NAME1,_NAME2,_ADDR,_A,_B,_C,_D,_E,_F,_G,_H)\
         ASMSFRB2 _NAME1, _NAME2, _ADDR

ASMSFRB2 MACRO
   sfrb  \1 = \3
   sfrb  \2 = \3
   ENDM


// Word sized SFRs, needs to be expanded into an assembler macro first.
#     define SFR_W_BITS(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H, _I,_J,_K,_L,_M,_N,_O,_P)\
         ASMSFRW  _NAME, _ADDR

#     define SFR_W_BITS_EXT(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H, _I,_J,_K,_L,_M,_N,_O,_P)\
         ASMSFRW  _NAME, _ADDR

ASMSFRW  MACRO
   sfrw  \1  = \2
   sfrb  \1L = (\2+0)
   sfrb  \1H = (\2+1)
   ENDM

#  endif // __IAR_SYSTEMS_ASM__



#  ifdef __ICCAVR__

#     define __BYTEBITS(_NAME,_A,_B,_C,_D,_E,_F,_G,_H) \
         U8   _NAME ## _ ## _A:1,                      \
              _NAME ## _ ## _B:1,                      \
              _NAME ## _ ## _C:1,                      \
              _NAME ## _ ## _D:1,                      \
              _NAME ## _ ## _E:1,                      \
              _NAME ## _ ## _F:1,                      \
              _NAME ## _ ## _G:1,                      \
              _NAME ## _ ## _H:1;

#     define SFR_B_BITS(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H)           \
         __io union                                                      \
         {                                                               \
            U8 _NAME;                       /* The sfrb as 1 byte */     \
            struct                          /* The sfrb as 8 bits */     \
            {                                                            \
               __BYTEBITS(_NAME, _A,_B,_C,_D,_E,_F,_G,_H)                \
            };                                                           \
         } @ _ADDR;

#     define SFR_B2_BITS(_NAME1, _NAME2, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H) \
         __io union                                                      \
         {                                                               \
            U8 _NAME1;                      /* The sfrb as 1 byte */     \
            U8 _NAME2;                      /* The sfrb as 1 byte */     \
            struct                          /* The sfrb as 8 bits */     \
            {                                                            \
               __BYTEBITS(_NAME1, _A,_B,_C,_D,_E,_F,_G,_H)               \
            };                                                           \
            struct                          /* The sfrb as 8 bits */     \
            {                                                            \
               __BYTEBITS(_NAME2, _A,_B,_C,_D,_E,_F,_G,_H)               \
            };                                                           \
         } @ _ADDR;

#     define SFR_B_BITS_EXT(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H)      \
         __tiny __no_init volatile union                                \
         {                                                              \
            U8 _NAME;                       /* The sfrb as 1 byte */    \
            struct                          /* The sfrb as 8 bits */    \
            {                                                           \
               __BYTEBITS(_NAME, _A,_B,_C,_D,_E,_F,_G,_H)               \
            };                                                          \
         } @ _ADDR;

#     define SFR_W_BITS(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H, _I,_J,_K,_L,_M,_N,_O,_P)                           \
         __io union                                                                                               \
         {                                                                                                        \
            U16 _NAME;                                                          /* The sfrw as 1 short       */   \
            struct                                                              /* The sfrw as 16 bits       */   \
            {                                                                                                     \
               __BYTEBITS(_NAME, _A,_B,_C,_D,_E,_F,_G,_H)                       /* Bit names defined by user */   \
               __BYTEBITS(_NAME, _I,_J,_K,_L,_M,_N,_O,_P)                       /* Bit names defined by user */   \
            };                                                                                                    \
            struct                                                              /* The sfrw as 2 bytes       */   \
            {                                                                                                     \
               U8 _NAME ## L;                                                                                     \
               U8 _NAME ## H;                                                                                     \
            };                                                                                                    \
            struct                                                              /* The sfrw as 2 x 8 bits      */ \
            {                                                                                                     \
               __BYTEBITS(_NAME ## L, Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)  /* Bit names hard coded to 0-7 */ \
               __BYTEBITS(_NAME ## H, Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)  /* Bit names hard coded to 0-7 */ \
            };                                                                                                    \
         } @ _ADDR;

#     define SFR_W_BITS_EXT(_NAME, _ADDR, _A,_B,_C,_D,_E,_F,_G,_H, _I,_J,_K,_L,_M,_N,_O,_P)                       \
         __tiny __no_init volatile union {                                                                        \
            U16 _NAME;                                                                                            \
            struct                                                              /* The sfrw_ext as 16 bits   */   \
            {                                                                                                     \
               __BYTEBITS(_NAME, _A,_B,_C,_D,_E,_F,_G,_H)                       /* Bit names defined by user */   \
               __BYTEBITS(_NAME, _I,_J,_K,_L,_M,_N,_O,_P)                       /* Bit names defined by user */   \
            };                                                                                                    \
            struct   /* The sfrw _ext as 2 bytes */                                                               \
            {                                                                                                     \
               U8 _NAME ## L;                                                                                     \
               U8 _NAME ## H;                                                                                     \
            };                                                                                                    \
            struct                                                              /* The sfrw_ext as 2 x 8 bits  */ \
            {                                                                                                     \
               __BYTEBITS(_NAME ## L, Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)  /* Bit names hard coded to 0-7 */ \
               __BYTEBITS(_NAME ## H, Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)  /* Bit names hard coded to 0-7 */ \
            };                                                                                                    \
         } @ _ADDR;

#  endif // __ICCAVR__



#  define SFR_B(_NAME, _ADDR) SFR_B_BITS(_NAME, _ADDR,                              \
                                    Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)

#  define SFR_B2(_NAME1, _NAME2, _ADDR) SFR_B2_BITS(_NAME1, _NAME2, _ADDR,          \
                                    Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)

#  define SFR_B_EXT(_NAME, _ADDR) SFR_B_BITS_EXT(_NAME, _ADDR,                      \
                                    Bit0,Bit1,Bit2,Bit3,Bit4,Bit5,Bit6,Bit7)

#  define SFR_W(_NAME, _ADDR)  SFR_W_BITS(_NAME, _ADDR,                             \
                                    Bit0,Bit1,Bit2 ,Bit3 ,Bit4 ,Bit5 ,Bit6 ,Bit7 ,  \
                                    Bit8,Bit9,Bit10,Bit11,Bit12,Bit13,Bit14,Bit15)

#  define SFR_W_EXT(_NAME, _ADDR)  SFR_W_BITS_EXT(_NAME, _ADDR,                     \
                                    Bit0,Bit1,Bit2 ,Bit3 ,Bit4 ,Bit5 ,Bit6 ,Bit7 ,  \
                                    Bit8,Bit9,Bit10,Bit11,Bit12,Bit13,Bit14,Bit15)

#endif // _IAR_AVR_

#define BIT0                        0x01
#define BIT1                        0x02
#define BIT2                        0x04
#define BIT3                        0x08
#define BIT4                        0x10
#define BIT5                        0x20
#define BIT6                        0x40
#define BIT7                        0x80

#define	BS(v,n)	(((unsigned char *)&(v))[n])

#endif // _COMPILER_H_
