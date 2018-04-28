//! @file lpregs.h,v
//!
//! Copyright (c) 2004 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the cyrf6936 driver definitions.
//!
//! @version 1.8 snd3b-dvk-1_0_33 $Id: lpregs.h,v 1.8 2006/12/08 16:21:58 sguyon Exp $
//!
//! @todo
//! @bug

#ifndef _LPREGS_H_
#define _LPREGS_H_

//_____ I N C L U D E S ____________________________________________________

#include "config.h"
#include "conf\conf_oid.h"

//_____ M A C R O S ________________________________________________________


//_____ D E F I N I T I O N ________________________________________________


//_____ D E C L A R A T I O N ______________________________________________

// -------------------------------
// Channel register
// -------------------------------
#define CHANNEL_ADR               0x00
#define CHANNEL_RST               0x48
#define MSK_CHANNEL               0x7F

//Channel
#define CHANNEL_MAX               0x62
#define CHANNEL_MIN               0x00
#define CHANNEL_2P498_GHZ         0x62
#define CHANNEL_2P4_GHZ           0x00

// -------------------------------
// TX Length register
// -------------------------------
#define TX_LENGTH_ADR             0x01
#define TX_LENGTH_RST             0x00
#define MSK_TX_LENGTH             0xFF

// -------------------------------
// TX Control register
// -------------------------------
#define TX_CTRL_ADR               0x02
#define TX_CTRL_RST               0x03

// See TX_IRQ for remaining bit position definitions

// TX_CTRL bit masks
#define MSK_TXE_IRQ_EN            0x01
#define MSK_TXC_IRQ_EN            0x02
#define MSK_TXBERR_IRQ_EN         0x04
#define MSK_TXB0_IRQ_EN           0x08
#define MSK_TXB8_IRQ_EN           0x10
#define MSK_TXB15_IRQ_EN          0x20
#define MSK_TX_CLR                0x40
#define MSK_TX_GO                 0x80

// -------------------------------
// TX Configuration register
// -------------------------------
#define TX_CFG_ADR                0x03
#define TX_CFG_RST                0x05

// separate bit field masks
#define MSK_PA_VAL                0x07
#define MSK_TX_DATMODE            0x18
#define MSK_TX_DATCODE_LEN        0x20

// DATCODE_LEN register masks
#define DATCODE_LEN_32            0x00
#define DATCODE_LEN_64            0x20

// DATMODE register masks
#define DATMODE_1MBPS             0x00
#define DATMODE_8DR               0x08
#define DATMODE_DDR               0x10
#define DATMODE_SDR               0x18

// PA_SET register masks
#define PA_N30_DBM                0x00
#define PA_N25_DBM                0x01
#define PA_N20_DBM                0x02
#define PA_N15_DBM                0x03
#define PA_N10_DBM                0x04
#define PA_N5_DBM                 0x05
#define PA_0_DBM                  0x06
#define PA_4_DBM                  0x07

// -------------------------------
// TX IRQ Status register
// -------------------------------
#define TX_IRQ_STATUS_ADR         0x04

// TX_IRQ bit masks
#define MSK_TXE_IRQ               0x01
#define MSK_TXC_IRQ               0x02
#define MSK_TXBERR_IRQ            0x04
#define MSK_TXB0_IRQ              0x08
#define MSK_TXB8_IRQ              0x10
#define MSK_TXB15_IRQ             0x20
#define MSK_LV_IRQ                0x40
#define MSK_XS_IRQ                0x80

// -------------------------------
// RX Control register
// -------------------------------
#define RX_CTRL_ADR               0x05
#define RX_CTRL_RST               0x07

// See RX_IRQ register for bit positions definitions also used for this register

// RX_CTRL bit masks
#define MSK_RXE_IRQEN             0x01
#define MSK_RXC_IRQEN             0x02
#define MSK_RXBERR_IRQEN          0x04
#define MSK_RXB1_IRQEN            0x08
#define MSK_RXB8_IRQEN            0x10
#define MSK_RXB16_IRQEN           0x20
#define MSK_RX_GO                 0x80

// -------------------------------
// RX Configuration register
// -------------------------------
#define RX_CFG_ADR                0x06
#define RX_CFG_RST                0x92

#define MSK_VLD_EN                0x01
#define MSK_RXOW_EN               0x02
#define MSK_FASTTURN_EN           0x08
#define MSK_HI                    0x10
#define MSK_LO                    0x00
#define MSK_ATT_EN                0x20
#define MSK_LNA_EN                0x40
#define MSK_AUTO_AGC_EN           0x80

// -------------------------------
// RX IRQ register
// -------------------------------
#define RX_IRQ_STATUS_ADR         0x07
// There is no default value for this register.

// RX_IRQ bit masks
#define MSK_RXE_IRQ               0x01
#define MSK_RXC_IRQ               0x02
#define MSK_RXBERR_IRQ            0x04
#define MSK_RXB1_IRQ              0x08
#define MSK_RXB8_IRQ              0x10
#define MSK_RXB16_IRQ             0x20
#define MSK_SOFDET_IRQ            0x40
#define MSK_RXOW_IRQ              0x80

// -------------------------------
// RX Status register
// -------------------------------
#define RX_STATUS_ADR             0x08
// There is no default value for this register.

// single flag bits & multi-bit-field masks
#define MSK_RX_DATMODE            0x03
#define MSK_RX_DATCODE_LEN        0x04
#define MSK_RX_BAD_CRC            0x08
#define MSK_RX_CRC0               0x10
#define MSK_RX_EOPERR             0x20
#define MSK_RX_PKTERR             0x40
#define MSK_RX_ACK                0x80

// -------------------------------
// RX Count register
// -------------------------------
#define RX_COUNT_ADR              0x09
#define RX_COUNT_RST              0x00
#define MSK_RX_COUNT              0xFF

// -------------------------------
// RX Length Field register
// -------------------------------
#define RX_LENGTH_ADR             0x0A
#define RX_LENGTH_RST             0x00
#define MSK_RX_LENGTH             0xFF

// -------------------------------
// Power Control register
// -------------------------------
#define PWR_CTRL_ADR              0x0B
#define PWR_CTRL_RST              0xA0

// single flag bits & multi-bit-field masks
#define MSK_PMU_OUTV              0x03
#define MSK_LV_IRQ_TH             0x0C
#define MSK_PFET_OFF              0x10
#define MSK_PMU_SEN               0x20
#define MSK_LV_IRQ_EN             0x40
#define MSK_PMU_EN                0x80

// LV_IRQ_TH values
#define LV_IRQ_TH_PMU_OUTV        0x00
#define LV_IRQ_TH_2P2_V           0x04
#define LV_IRQ_TH_2P0_V           0x08
#define LV_IRQ_TH_1P8_V           0x0C

// PMU_OUTV values
#define PMU_OUTV_2P7              0x00
#define PMU_OUTV_2P6              0x01
#define PMU_OUTV_2P5              0x02
#define PMU_OUTV_2P4              0x03

// -------------------------------
// Crystal Control register
// -------------------------------
#define XTAL_CTRL_ADR             0x0C
#define XTAL_CTRL_RST             0x04

// single flag bits & multi-bit-field masks
#define MSK_XOUT_FREQ             0x07
#define MSK_XS_IRQ_EN             0x20
#define MSK_XOUT_FNC              0xC0

// XOUT_FNC values
#define XOUT_FNC_XOUT_FREQ        0x00
#define XOUT_FNC_PA_N             0x40
#define XOUT_FNC_RAD_STREAM       0x80
#define XOUT_FNC_GPIO             0xC0

// XOUT_FREQ values
#define XOUT_FREQ_12MHZ           0x00
#define XOUT_FREQ_6MHZ            0x01
#define XOUT_FREQ_3MHZ            0x02
#define XOUT_FREQ_1P5MHZ          0x03
#define XOUT_FREQ_P75MHZ          0x04

// -------------------------------
// I/O Configuration register
// -------------------------------
#define IO_CFG_ADR                0x0D
#define IO_CFG_RST                0x00
#define MSK_IO_CFG                0xFF

// single flag bits & multi-bit-field masks
#define MSK_IRQ_GPIO              0x01
#define MSK_SPI_3_PIN             0x02
#define MSK_PACTL_GPIO            0x04
#define MSK_PACTL_OD              0x08
#define MSK_XOUT_OD               0x10
#define MSK_MISO_OD               0x20
#define MSK_IRQ_POL               0x40
#define MSK_IRQ_OD                0x80

// -------------------------------
// GPIO Control register
// -------------------------------
#define GPIO_CTRL_ADR             0x0E
#define GPIO_CTRL_RST             0x00
#define MSK_GPIO_CTRL             0xF0

// single flag bits & multi-bit-field masks
#define MSK_IRQ_IP                0x01
#define MSK_PACTL_IP              0x02
#define MSK_MISO_IP               0x04
#define MSK_XOUT_IP               0x08
#define MSK_IRQ_OP                0x10
#define MSK_PACTL_OP              0x20
#define MSK_MISO_OP               0x40
#define MSK_XOUT_OP               0x80

// -------------------------------
// Transaction Configuration register
// -------------------------------
#define XACT_CFG_ADR              0x0F
#define XACT_CFG_RST              0x80

// single flag bits & multi-bit-field masks
#define MSK_ACK_TO                0x03
#define MSK_END_STATE             0x1C
#define MSK_FRC_END_STATE         0x20
#define MSK_ACK_EN                0x80

// END_STATE field values
#define END_STATE_SLEEP           0x00
#define END_STATE_IDLE            0x04
#define END_STATE_TXSYNTH         0x08
#define END_STATE_RXSYNTH         0x0C
#define END_STATE_RX              0x10

// ACK_TO field values
#define ACK_TO_4X                 0x00  //4x
#define ACK_TO_8X                 0x01  //8x
#define ACK_TO_12X                0x02  //12x
#define ACK_TO_15X                0x03  //15x

// -------------------------------
// Framing Configuration register
// -------------------------------
#define FRAMING_CFG_ADR           0x10
#define FRAMING_CFG_RST           0xA5

// single flag bits & multi-bit-field masks
#define MSK_SOP_THRESH            0x1F
#define MSK_LEN_EN                0x20
#define MSK_SOP_LEN               0x40
#define MSK_SOP_EN                0x80

//SOP correlator threshold
#define SOP32_TH                  0x04
#define SOP64_TH                  0x0E

// -------------------------------
// Data Threshold 32 register
// -------------------------------
#define DATA32_THOLD_ADR          0x11
#define DAT32_THRESH_RST          0x04
#define MSK_DAT32_THRESH          0x0F

// -------------------------------
// Data Threshold 64 register
// -------------------------------
#define DATA64_THOLD_ADR          0x12
#define DAT64_THRESH_RST          0x0A
#define MSK_DAT64_THRESH          0x1F

// -------------------------------
// RSSI register
// -------------------------------
#define RSSI_ADR                  0x13
#define RSSI_RST                  0x20

// single flag bits & multi-bit-field masks
#define MSK_RSSI_LVL              0x1F
#define MSK_LNA_STATE             0x20
#define MSK_SOP_RSSI              0x80

// -------------------------------
// EOP Control register
// -------------------------------
#define EOP_CTRL_ADR              0x14
#define EOP_CTRL_RST              0xA4

// single flag bits & multi-bit-field masks
#define MSK_EOP                   0x0F
#define MSK_HINT_EOP              0x70
#define MSK_HINT_EN               0x80

// -------------------------------
// CRC Seed registers
// -------------------------------
#define CRC_SEED_LSB_ADR          0x15
#define CRC_SEED_MSB_ADR          0x16
#define CRC_SEED_LSB_RST          0x00
#define CRC_SEED_MSB_RST          0x00

// CRC related values
// USB CRC-16
#define CRC_POLY_MSB              0x80
#define CRC_POLY_LSB              0x05
#define CRC_RESI_MSB              0x80
#define CRC_RESI_LSB              0x0D

// -------------------------------
// TX CRC Calculated registers
// -------------------------------
#define TX_CRC_LSB_ADR            0x17
#define TX_CRC_MSB_ADR            0x18

// -------------------------------
// RX CRC Field registers
// -------------------------------
#define RX_CRC_LSB_ADR            0x19
#define RX_CRC_MSB_ADR            0x1A
#define RX_CRC_LSB_RST            0xFF
#define RX_CRC_MSB_RST            0xFF

// -------------------------------
// Synth Offset registers
// -------------------------------
#define TX_OFFSET_LSB_ADR         0x1B
#define TX_OFFSET_MSB_ADR         0x1C
#define TX_OFFSET_LSB_RST         0x00
#define TX_OFFSET_MSB_RST         0x00

// single flag bits & multi-bit-field masks
#define MSK_STRIM_LSB             0xFF
#define MSK_STRIM_MSB             0x0F

// -------------------------------
// Mode Override register
// -------------------------------
#define MODE_OVERRIDE_ADR         0x1D
#define MODE_OVERRIDE_RST         0x00

#define FRC_AWAKE                 0x03
#define FRC_AWAKE_OFF_1           0x01
#define FRC_AWAKE_OFF_2           0x00

// single flag bits & multi-bit-field masks
#define MSK_RST                   0x01
#define MSK_FRC_PA                0x02
#define MSK_FRC_AWAKE             0x18
#define MODE_OVRD_FRC_AWAKE         0x18
#define MODE_OVRD_FRC_AWAKE_OFF_1   0x08
#define MODE_OVRD_FRC_AWAKE_OFF_2   0x00
#define MSK_FRC_SEN               0x20
#define MSK_SEN_TXRXB             0x40
#define MSK_DIS_AUTO_SEN          0x80

// -------------------------------
// RX Override register
// -------------------------------
#define RX_OVERRIDE_ADR           0x1E
#define RX_OVERRIDE_RST           0x00

// single flag bits & multi-bit-field masks
#define MSK_ACE                   0x02
#define MSK_DIS_RXCRC             0x04
#define MSK_DIS_CRC0              0x08
#define MSK_FRC_RXDR              0x10
#define MSK_MAN_RXACK             0x20
#define MSK_EXTEND_RX_TX          0x40
#define MSK_ACK_RX                0x80

// -------------------------------
// TX Override register
// -------------------------------
#define TX_OVERRIDE_ADR           0x1F
#define TX_OVERRIDE_RST           0x00

// single flag bits & multi-bit-field masks
#define MSK_TXINV                 0x01
#define MSK_CO                    0x02
#define MSK_DIS_TXCRC             0x04
#define MSK_OVRRD_ACK             0x08
#define MSK_MAN_TX_ACK            0x10
#define MSK_DIS_TX_RETRANS        0x20
#define MSK_FRC_PREAMBLE          0x40
#define MSK_ACK_TX_SEN            0x80

//------------------------------------------------------------------------------
//      File Function Detail
//------------------------------------------------------------------------------

// -------------------------------
// TX Buffer - 16 bytes
// -------------------------------
#define TX_BUFFER_ADR             0x20

// -------------------------------
// RX Buffer - 16 bytes
// -------------------------------
#define RX_BUFFER_ADR             0x21

// -------------------------------
// Framing Code - 8 bytes
// -------------------------------
#define SOP_CODE_ADR              0x22

// CODESTORE_REG_SOF_RST        64'h17_ff_9e_21_36_90_c7_82
#define CODESTORE_BYTE7_SOF_RST         0x17
#define CODESTORE_BYTE6_SOF_RST         0xFF
#define CODESTORE_BYTE5_SOF_RST         0x9E
#define CODESTORE_BYTE4_SOF_RST         0x21
#define CODESTORE_BYTE3_SOF_RST         0x36
#define CODESTORE_BYTE2_SOF_RST         0x90
#define CODESTORE_BYTE1_SOF_RST         0xC7
#define CODESTORE_BYTE0_SOF_RST         0x82

// -------------------------------
// Data Code - 16 bytes
// -------------------------------
#define DATA_CODE_ADR                   0x23

// CODESTORE_REG_DCODE0_RST            64'h01_2B_F1_DB_01_32_BE_6F
#define CODESTORE_BYTE7_DCODE0_RST      0x01
#define CODESTORE_BYTE6_DCODE0_RST      0x2B
#define CODESTORE_BYTE5_DCODE0_RST      0xF1
#define CODESTORE_BYTE4_DCODE0_RST      0xDB
#define CODESTORE_BYTE3_DCODE0_RST      0x01
#define CODESTORE_BYTE2_DCODE0_RST      0x32
#define CODESTORE_BYTE1_DCODE0_RST      0xBE
#define CODESTORE_BYTE0_DCODE0_RST      0x6F

// CODESTORE_REG_DCODE1_RST            64'h02_F9_93_97_02_FA_5C_E3
#define CODESTORE_BYTE7_DCODE1_RST      0x02
#define CODESTORE_BYTE6_DCODE1_RST      0xF9
#define CODESTORE_BYTE5_DCODE1_RST      0x93
#define CODESTORE_BYTE4_DCODE1_RST      0x97
#define CODESTORE_BYTE3_DCODE1_RST      0x02
#define CODESTORE_BYTE2_DCODE1_RST      0xFA
#define CODESTORE_BYTE1_DCODE1_RST      0x5C
#define CODESTORE_BYTE0_DCODE1_RST      0xE3

// -------------------------------
// Preamble - 3 bytes
// -------------------------------
#define PREAMBLE_ADR                    0x24

#define PREAMBLE_CODE_MSB_RST           0x33
#define PREAMBLE_CODE_LSB_RST           0x33
#define PREAMBLE_LEN_RST                0x02

// -------------------------------
// Laser Fuses - 8 bytes (2 hidden)
// -------------------------------
#define MFG_ID_ADR                      0x25

// -------------------------------
// XTAL Startup Delay
// -------------------------------
#define XTAL_CFG_ADR                    0x26
#define XTAL_CFG_RST                    0x00

// -------------------------------
// Clock Override
// -------------------------------
#define CLK_OVERRIDE_ADR                0x27
#define CLK_OVERRIDE_RST                0x00

// -------------------------------
// Clock Enable
// -------------------------------
#define CLK_EN_ADR                0x28
#define CLK_EN_RST                0x00

#define MSK_RXF                   0x02

// -------------------------------
// Receiver Abort
// -------------------------------
#define RX_ABORT_ADR              0x29
#define RX_ABORT_RST              0x00

#define MSK_ABORT_EN              0x20

// -------------------------------
// Auto Calibration Time
// -------------------------------
#define AUTO_CAL_TIME_ADR         0x32
#define AUTO_CAL_TIME_RST         0x0C

#define AUTO_CAL_TIME_MAX         0x3C

// -------------------------------
// Auto Calibration Offset
// -------------------------------
#define AUTO_CAL_OFFSET_ADR       0x35
#define AUTO_CAL_OFFSET_RST       0x00

#define AUTO_CAL_OFFSET_MINUS_4   0x14

// -------------------------------
// RX Configuration register
// -------------------------------
#define ANALOG_CTRL_ADR           0x39
#define ANALOG_CTRL_RST           0x00

#define ALL_SLOW_EN               0x01


#endif  // _LPREGS_H_

