
/*
**	Direct access to DMCU memory areas.
*/

#ifndef _ABSACC_H_
#define _ABSACC_H_
/*
** Direct access Byte/Word
*/
#define m8HI4(arg)			(arg>>4)
#define m8LO4(arg)			(arg&0x0F)

#define m16HI8(arg)			(*( unsigned char *)&(arg))
#define m16LO8(arg)			(*((unsigned char *)&(arg) + 1))

#define m32HI16(arg)		(*( unsigned int  *)&(arg))
#define m32LO16(arg)		(*((unsigned int  *)&(arg) + 1))

#define m32HI8(arg)			(*( unsigned char *)&(arg))
#define m32MH8(arg)			(*((unsigned char *)&(arg) + 1))
#define m32ML8(arg)			(*((unsigned char *)&(arg) + 2))
#define m32LO8(arg)			(*((unsigned char *)&(arg) + 3))

/*
** Direct access GPR/YRAM/XRAM
*/
#define XBYTE ((__X volatile unsigned char *) (unsigned int)0x0000)
#define XWORD ((__X volatile unsigned int  *) (unsigned int)0x0000)
#define XLONG ((__X volatile unsigned long *) (unsigned int)0x0000)

#define YBYTE ((__Y volatile unsigned char *) (unsigned int)0x8000)
#define YWORD ((__Y volatile unsigned int  *) (unsigned int)0x8000)
#define YLONG ((__Y volatile unsigned long *) (unsigned int)0x8000)

#define GPREG ((__X volatile unsigned int  *) (unsigned int)0xF2E0)

#endif	//_ABSACC_H_