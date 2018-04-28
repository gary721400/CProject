#ifndef __MRM_INTERFACE_H
#define __MRM_INTERFACE_H

#define DEMO_BOARD  0//1

#define SPI_CS_LOW()      _bitCLR( _PDOD_b0 + 7 )//PBOD &= (~(( unsigned int )0x0001 << 0 ))
#define SPI_CS_HIGH()     _bitSET( _PDOD_b0 + 7 )//PBOD |= (( unsigned int )0x0001 << 0 )
#if DEMO_BOARD
#define MRM_RESET_HIGH()  PBOD |= (( unsigned int )0x0001 << 5 )
#define MRM_RESET_LOW()   PBOD &= (~(( unsigned int )0x0001 << 5 ))
#define MRM_PWRDN_EN()    PBOD |= (( unsigned int )0x0001 << 6 )
#define MRM_PWRDN_DIS()   PBOD &= (~(( unsigned int )0x0001 << 6 ))
#else
#define MRM_RESET_HIGH()  //PDOD |= (( unsigned int )0x0001 << 7 )
#define MRM_RESET_LOW()   //PDOD &= (~(( unsigned int )0x0001 << 7 ))
#define MRM_PWRDN_EN()    //PDOD |= (( unsigned int )0x0001 << 5 )
#define MRM_PWRDN_DIS()   //PDOD &= (~(( unsigned int )0x0001 << 5 ))
#define MAIN_POWER_ON()   //PCOD |= (( unsigned int )0x0001 << 3 )
#define MAIN_POWER_OFF()  //PCOD &= (~(( unsigned int )0x0001 << 3 ))

#define POWER_KEY()       (!( PCID &(( unsigned int )0x0001 << 5 )))
#define VOLUME_UP_KEY()   (!( PBID &(( unsigned int )0x0001 << 8 )))
#define VOLUME_DOWN_KEY() (!( PBID &(( unsigned int )0x0001 << 9 )))
#endif

void     DelayUS           ( mrmWord us );
void     DelayMS           ( mrmWord ms );
void     InitMrmInterface  ( void );
void     InitSPI           ( void );
mrmBool  SPI_Transmit      ( mrmByte *pData, mrmByte size );
mrmByte  *GetMprPath       ( void );

#endif
