//! @file mmc_drv.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the MMC driver routines
//!
//! @version 1.17 snd3-dvk-1_9_5 $Id: mmc_drv.h,v 1.17 2007/07/24 09:48:30 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _MMC_DRV_H_
#define _MMC_DRV_H_

#include "lib_mcu\compiler.h"
#include "absacc.h"

// Clock of MMC macro
#define MMC_CLK_400K             400                  // 400 KHz
#define MMC_CLK_12M              12000                //  12 MHz
#define MMC_CLK_20M              20000                //  20 MHz
#define MMC_CLK_25M              25000                //  25 MHz
#define MMC_CLK_26M              26000                //  26 MHz
#define MMC_CLK_52M              52000                //  52 MHz

// Functions parameters
#define MMC_NO_RESP               0
#define MMC_RESP_R1               1
#define MMC_RESP_R2               2
#define MMC_RESP_R3               3
#define MMC_RESP_R4               4
#define MMC_RESP_R5               5
#define SD_RESP_R6                6    // specific response for SD
#define SD_RESP_R7                7    // specific response for SD

#define SSS_RESP_C12              8    // Ron@2008/10/23
// Controler bus width
#define MMC_BUS_SIZE_1_BIT  0x00
#define MMC_BUS_SIZE_4_BIT  0x04
#define MMC_BUS_SIZE_8_BIT  0x10

// _____ D E F I N I T I O N ________________________________________________


// _____ D E C L A R A T I O N ______________________________________________
/*
#define Mmc_reset()               (  MMCON2 |= (MSK_DCR | MSK_CCR))
#define Mmc_reset_data_error()    (  MMCON2 |=  MSK_DCR)
#define Mmc_is_under_reset()      ( (MMCON2 &   MSK_DCR) == MSK_DCR)
#define Mmc_reset_cmd_fifos()     (  MMCON0 |= (MSK_CRPTR | MSK_CTPTR))
#define Mmc_reset_data_fifos()    (  MMCON0 |= (MSK_DPTRR))
#define Mmc_enable()              (  MMCON2 |= (MSK_MMCEN | MSK_DATD))
#define Mmc_disable()             (  MMCON2 &=~(MSK_MMCEN | MSK_DATD))
*/
#define Mmc_force_clk()           (  SDIFCON0 |=  CLK_EN )
#define Mmc_unforce_clk()         (  SDIFCON0 &= ~CLK_EN )
#define Mmc_set_bus_size(s)       (  SDIFCON0 = (( SDIFCON0 & ~WIDEBUS )| s ))
//#define Mmc_card_busy()           ( (MMSTA  &   MSK_CBUSY) == MSK_CBUSY) // some card are not compliant whit this test because of wrong Nst value
#define Mmc_read_busy_signal()    ( P3_2 )
#define Mmc_card_present()        ( P1_7 != 0 )
#define Mmc_card_not_present()    ( P1_7 == 0 )
//#define Mmc_card_wp()             ( P1_4 != 0 )
//#define Mmc_card_not_wp()         ( P1_4 == 0 )
#define Mmc_crc16s_ok()           ( !( STATUS07_00 & CRC16ERROR ))
#define Mmc_crc16s_ko()           ( STATUS07_00 & CRC16ERROR )
/*#define Mmc_format_ok()           (  MMSTA  &   MSK_DATFS  )

// DATA FIFO MACRO STATUS
#define Mmc_fifo_16bytes_available()      ((MMSTA & MSK_WFRS) == MSK_WFRS)
#define Mmc_fifo_8bytes_available()       ((MMSTA & MSK_HFRS) == MSK_HFRS)
#define Mmc_fifo_16bytes_not_available()  ((MMSTA & MSK_WFRS) == 0)
#define Mmc_fifo_8bytes_not_available()   ((MMSTA & MSK_HFRS) == 0)
#define Mmc_fifo_not_ready()              Mmc_fifo_16bytes_not_available()
*/

// Data
#define Mmc_set_write()           ( DATACON = (( DATACON & ~MSK_XFER_MODE )| XFER_WRITE_MODE ))
#define Mmc_set_read()            ( DATACON = (( DATACON & ~MSK_XFER_MODE )| XFER_READ_MODE ))
#define Mmc_set_block_size(s)     ( BLKLEN11_08 = (( U16 )s & 0x0F00 )>> 8 );\
                                  ( BLKLEN07_00 = ( U16 )s & 0x00FF )
/*#define Mmc_rd_block_size()       (((U16)(MMCON1 & MSK_BLEN) << 4) + (U16)MMBLP)
#define Mmc_set_stream()          (MMCON0 &= ~MSK_DFMT)
*/
#define Mmc_set_multi_block()     ( DATACON |= ( MULTI | CHK_BC ))
#define Mmc_set_single_block()    ( DATACON &= ~( MULTI | CHK_BC ));
#define Mmc_enable_data()         ( CMDCON |= DATA_WITH );
/*#define Mmc_rd_byte()             (MMDAT)
#define Mmc_rd_word()             ((U16)(MMDAT | (MMDAT << 8)))
#define Mmc_wr_byte(b)            (MMDAT = (b))
*/
#define Mmc_cmd_in_progress()     (( STATUS07_00 & CMDSEND ) != CMDSEND )
#define Mmc_resp_received()       (( STATUS07_00 & RSPRXVP ) == RSPRXVP )
//#define Mmc_resp_not_received()   ((MMCON1 & MSK_RXCEN) == MSK_RXCEN)
//#define Mmc_data_in_progress()    ((MMCON1 & MSK_DATEN) == MSK_DATEN)

#define Mmc_drv_card_change()           ( b_card_state != Mmc_card_present())

//#define Mmc_rd_cmd()                      (MMCMD)
//#define Mmc_wr_cmd(b)                     (MMCMD = b)




//***********************************************
//2006-10-27Ôö¼Ó
//SDIFCON0
#define CLK_EN                0x01	//clock enable
#define HISPEED               0x02	//high speed
#define WIDEBUS               0x04	//wide bus mode

//CMDCON
#define LONGRSP               0x01  //long response is expected
#define WAITRSP               0x02	//response is expected
#define DATA_WITH             0x04	//data transfer with this command
#define STOP                  0x08	//current command is a stop command

//STATUS07_00
#define CMDSEND               0x01	//command transmitted
#define RSPRXVP               0x02	//response received
#define RSPTIMEOUT            0x04	//timeout
#define CRC7ERROR             0x08	//response CRC error
#define CRC16ERROR            0x10	//receive crc fail
#define CRCSTA_ERROR          0x20	//crc status fail
#define DTIMEOUT              0x40	//timeout
#define XFEREND               0x80	//data transfer end

//STATUS15_08
#define BUSYEND               0x01	//busy end
#define SDIO_INT              0x02	//SDIO device interrupt event

//SDH_CLOCK
#define CLK_SEL               0x80	//system clock

//DATCON
#define CHK_BC                0x01	//
#define MULTI                 0x02	//
//#define XFER_MODE0            0x04	//data transfer mode
//#define XFER_MODE1            0x80	//
#define MSK_XFER_MODE         0xC0

#define XFER_READ_MODE        0x00	//block read mode
#define XFER_WRITE_MODE       0x04  //block write mode
#define XFER_CHECK_MODE       0x08  //check busy end

//SDH_DMA_ACK
#define DMA_ACK_F             0x01	//DMA acknowledge flag, low active
#define DMA_ACK               0x02	//DMA acknowledge
#define DMA_REQ               0x04	//DMA request, low active
#define DMA_AUTO              0x80  //auto clear DMA request in multiple block mode
//
//***********************************************


//_____ D E C L A R A T I O N ______________________________________________

void        mmc_drv_on              ( void );
void        mmc_drv_off             ( void );
Bool        mmc_drv_send_cmd        ( U8, U32, U8 );
void        mmc_drv_send_scmd       ( U8, U8 );
Bool        mmc_drv_wait_cmd_resp   ( void );
Bool        mmc_drv_wait_cmd_trans  ( void );
bit         mmc_drv_check_response  ( void );
U32         mmc_drv_read_response   ( void );
void        mmc_wait_busy_signal    ( void );
Bool        mmc_drv_check_clear_int ( U8 flag );
void        mmc_drv_blockctn_reset  ( void );
U16         mmc_drv_blockctn_get    ( void );
void        Check_SDHost_STM        ( void );



#endif // _MMC_DRV_H_
