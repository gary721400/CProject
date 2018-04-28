#ifndef _SPI_H_
#define _SPI_H_

//----------------------------------------------------------------------------
#define BIT( n )           ( 1 << n)
#define SPI_SPE( n )       ( n << 15 )
#define SPI_SPFIE( n )     ( n << 10 )
#define SPI_SPIS( n )      ( n << 9 )
#define SPI_SPIE( n )      ( n << 8 )
#define SPI_MFEN( n )      ( n << 7 )
#define SPI_MSTR( n )      ( n << 6 )
#define SPI_MODE( n )      ( n << 4 )
#define SPI_SPR( n )       ( n )

typedef enum
{
   SPI_Mode0 = 0,
   SPI_Mode1 = 1,
   SPI_Mode2 = 2,
   SPI_Mode3 = 3
}tSpiMode;

typedef enum
{
   SPI_30MHz = 0,
   SPI_20MHz = 1,
   SPI_15MHz = 2,
   SPI_12MHz = 3,
   SPI_10MHz = 4,
   SPI_7p5MHz = 5,
   SPI_6MHz = 6,
   SPI_4MHz = 7,
   SPI_3MHz = 8,
   SPI_2MHz = 9,
   SPI_1MHz = 10,
   SPI_750KHz = 11,
   SPI_600KHz = 12,
   SPI_400KHz = 13,
   SPI_200KHz = 14,
   SPI_100KHz = 15
}tSpiClock;

//----------------------------------------------------------------------------
void  rf_spi_master_init  ( void );
//char spi1_master_byte_txrx(char);
#endif // End of SPI.H
