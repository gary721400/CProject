#include <usb_def.h>

#ifndef __MUSBC_REG_H__
#define __MUSBC_REG_H__


							// in usb_storage_Descriptor.inc
#define USB_USE_EP 1							

#define HIGH_SPEED 1							


// USB Registers
#define USB_REGISTER_BASE 	0xff0000

#define USB_REG_FADDR		(USB_REGISTER_BASE + 0x0)		// 8 bits	(+7)
#define USB_REG_POWER		(USB_REGISTER_BASE + 0x1)		// 8 bits
#define USB_REG_INTRTX		(USB_REGISTER_BASE + 0x2)		// 16 bits
#define USB_REG_INTRRX		(USB_REGISTER_BASE + 0x4)		// 16 bits	(-15)
#define USB_REG_INTRTXE		(USB_REGISTER_BASE + 0x6)		// 16 bits
#define USB_REG_INTRRXE		(USB_REGISTER_BASE + 0x8)		// 16 bits	(-15)
#define USB_REG_INTRUSB		(USB_REGISTER_BASE + 0xA)		// 8 bits
#define USB_REG_INTRUSBE	(USB_REGISTER_BASE + 0xB)		// 8 bits
#define USB_REG_FRAME		(USB_REGISTER_BASE + 0xC)		// 16 bits (+11)
#define USB_REG_INDEX		(USB_REGISTER_BASE + 0xE)		// 4 bits (+4)
#define USB_REG_TESTMODE	(USB_REGISTER_BASE + 0xF)		// 8 bits
#if 1
#define USB_REG_TXMAXP		(USB_REGISTER_BASE + 0x10)	// 16 bits
#define USB_REG_CSR0		(USB_REGISTER_BASE + 0x12)	// 16 bits (+9)
#define USB_REG_TXCSR		(USB_REGISTER_BASE + 0x12) 	// 16 bits
#define USB_REG_RXMAXP		(USB_REGISTER_BASE + 0x14)	// 16 bits
#define USB_REG_RXCSR		(USB_REGISTER_BASE + 0x16)	// 16 bits
#define USB_REG_COUNT0		(USB_REGISTER_BASE + 0x18)	// 8 bits (+7)
#define USB_REG_RXCOUNT	 	(USB_REGISTER_BASE + 0x18)	// 16 bits (+13)
#define USB_REG_CONFIGDATA	(USB_REGISTER_BASE + 0x1F)	// 8 bits
#define USB_REG_FIFOSIZE	(USB_REGISTER_BASE + 0x1F)	// 8 bits*/
#else
//#define USB_EP0_TXMAXP		(USB_REGISTER_BASE + 0x100)	// 16 bits
//#define USB_EP0_CSR0		(USB_REGISTER_BASE + 0x102)	// 16 bits (+9)
//#define USB_EP0_TXCSR		(USB_REGISTER_BASE + 0x102) 	// 16 bits
//#define USB_EP0_RXMAXP		(USB_REGISTER_BASE + 0x104)	// 16 bits
//#define USB_EP0_RXCSR		(USB_REGISTER_BASE + 0x106)	// 16 bits
//#define USB_EP0_COUNT0		(USB_REGISTER_BASE + 0x108)	// 8 bits (+7)
//#define USB_EP0_RXCOUNT	 	(USB_REGISTER_BASE + 0x108)	// 16 bits (+13)
//#define USB_EP0_CONFIGDATA	(USB_REGISTER_BASE + 0x10F)	// 8 bits
//#define USB_EP0_FIFOSIZE	(USB_REGISTER_BASE + 0x10F)	// 8 bits
//---Bulk
//#define USB_BLK_TXMAXP		(USB_REGISTER_BASE + 0x100 + (USB_USE_EP<<4))	// 16 bits
//#define USB_BLK_CSR0		(USB_REGISTER_BASE + 0x102 + (USB_USE_EP<<4))	// 16 bits (+9)
//#define USB_BLK_TXCSR		(USB_REGISTER_BASE + 0x102 + (USB_USE_EP<<4)) 	// 16 bits
//#define USB_BLK_RXMAXP		(USB_REGISTER_BASE + 0x104 + (USB_USE_EP<<4))	// 16 bits
//#define USB_BLK_RXCSR		(USB_REGISTER_BASE + 0x106 + (USB_USE_EP<<4))	// 16 bits
//#define USB_BLK_COUNT0		(USB_REGISTER_BASE + 0x108 + (USB_USE_EP<<4))	// 8 bits (+7)
//#define USB_BLK_RXCOUNT	 	(USB_REGISTER_BASE + 0x108 + (USB_USE_EP<<4))	// 16 bits (+13)
//#define USB_BLK_CONFIGDATA	(USB_REGISTER_BASE + 0x10F + (USB_USE_EP<<4))	// 8 bits
//#define USB_BLK_FIFOSIZE	(USB_REGISTER_BASE + 0x10F + (USB_USE_EP<<4))	// 8 bits
#endif
#define USB_REG_FIFO0		(USB_REGISTER_BASE + 0x20)	// 32 bits, but we use 16 bits only
#define USB_REG_FIFO(x)		(USB_REGISTER_BASE + 0x20 + x*4 )

#define USB_REG_DEVCTL		(USB_REGISTER_BASE + 0x60)	// 8 bits	(+8)

#define USB_REG_DMAINTR		(USB_REGISTER_BASE + 0x200)
#define USB_REG_DMACNTL		(USB_REGISTER_BASE + 0x204)
#define USB_REG_DMAADDR		(USB_REGISTER_BASE + 0x208)
#define USB_REG_DMACOUNT	(USB_REGISTER_BASE + 0x20C)

// Host Mode only Register
#define USB_REG_NAKLIMIT0	(USB_REGISTER_BASE + 0x1A)	// 8 bits (+5)
#define USB_REG_TXINTERVAL	(USB_REGISTER_BASE + 0x1A)	// 8 bits
#define USB_REG_TXTYPE		(USB_REGISTER_BASE + 0x1B)	// 8 bits (+6)
#define USB_REG_RXINTERVAL	(USB_REGISTER_BASE + 0x1C)	// 8 bits
#define USB_REG_RXTYPE		(USB_REGISTER_BASE + 0x1D)	// 8 bits (+6)

#define M_USB_REG_FADDR		*((volatile uint8_t *)(USB_REG_FADDR)) 		// 8 bits	(+7)
#define M_USB_REG_POWER		*((volatile uint8_t *)(USB_REG_POWER)) 		// 8 bits

#define M_USB_REG_INTRTX	*((volatile uint16_t *)(USB_REG_INTRTX))		// 16 bits
#define M_USB_REG_INTRRX	*((volatile uint16_t *)(USB_REG_INTRRX))		// 16 bits	(-15)
#define M_USB_REG_INTRTXE	*((volatile uint16_t *)(USB_REG_INTRTXE))		// 16 bits
#define M_USB_REG_INTRRXE	*((volatile uint16_t *)(USB_REG_INTRRXE))		// 16 bits	(-15)
#define M_USB_REG_INTRUSB	*((volatile uint8_t *)(USB_REG_INTRUSB)	)	// 8 bits
#define M_USB_REG_INTRUSBE	*((volatile uint8_t *)(USB_REG_INTRUSBE))		// 8 bits
#define M_USB_REG_FRAME		*((volatile uint16_t *)(USB_REG_FRAME))		// 16 bits (+11)
#define M_USB_REG_INDEX		*((volatile uint8_t *)(USB_REG_INDEX))		// 4 bits (+4)
#define M_USB_REG_TESTMODE	*((volatile uint8_t *)(USB_REG_TESTMODE)) 		// 8 bits
#define M_USB_REG_TXMAXP	*((volatile uint16_t *)(USB_REG_TXMAXP)) // 16 bits
#define M_USB_REG_CSR0		*((volatile uint16_t *)(USB_REG_CSR0)) // 16 bits (+9)
#define M_USB_REG_TXCSR		*((volatile uint16_t *)(USB_REG_TXCSR))  // 16 bits
#define M_USB_REG_RXMAXP	*((volatile uint16_t *)(USB_REG_RXMAXP))	// 16 bits
#define M_USB_REG_RXCSR		*((volatile uint16_t *)(USB_REG_RXCSR))	// 16 bits
#define M_USB_REG_COUNT0	*((volatile uint8_t *)(USB_REG_COUNT0))	// 8 bits (+7)
#define M_USB_REG_RXCOUNT	*((volatile uint16_t *)(USB_REG_RXCOUNT))	// 16 bits (+13)
#define M_USB_REG_CONFIGDATA	*((volatile uint8_t *)(USB_REG_CONFIGDATA))	// 8 bits
#define M_USB_REG_FIFOSIZE		*((volatile uint8_t *)(USB_REG_FIFOSIZE)	// 8 bits

#define M_USB_REG_FIFO0		*((volatile uint8_t *)(USB_REG_FIFO0))	// 32 bits, but we use 16 bits only
#define M_USB_REG_FIFO(x)	*((volatile uint8_t *)(USB_REG_FIFO(x)))
#define M_USB_REG_FIFO_16(x)	*((volatile uint16_t *)(USB_REG_FIFO(x)))

#define M_USB_REG_DEVCTL	*((volatile uint8_t *)(USB_REG_DEVCTL))	// 8 bits	(+8)

#define M_USB_REG_DMAINTR	*((volatile uint16_t *)(USB_REG_DMAINTR))
#define M_USB_REG_DMACNTL	*((volatile uint16_t *)(USB_REG_DMACNTL))
#define M_USB_REG_DMAADDR	*((volatile uint16_t *)(USB_REG_DMAADDR))
#define M_USB_REG_DMACOUNT	*((volatile uint16_t *)(USB_REG_DMACOUNT))

//EP0
//#define M_USB_EP0_TXMAXP	    *((volatile uint16_t*) (USB_EP0_TXMAXP	     ))  // 16 bits       
//#define M_USB_EP0_CSR0		    *((volatile uint16_t*) (USB_EP0_CSR0		 ))  // 16 bits (+9)  
//#define M_USB_EP0_TXCSR		    *((volatile uint16_t*) (USB_EP0_TXCSR		 ))  // 16 bits   
//#define M_USB_EP0_RXMAXP	    *((volatile uint16_t*) (USB_EP0_RXMAXP	     ))  // 16 bits       
//#define M_USB_EP0_RXCSR		    *((volatile uint16_t*) (USB_EP0_RXCSR		 ))  // 16 bits       
//#define M_USB_EP0_COUNT0	    *((volatile uint8_t*) (USB_EP0_COUNT0	     ))  // 8 bits (+7)   
//#define M_USB_EP0_RXCOUNT	    *((volatile uint16_t*) (USB_EP0_RXCOUNT	 ))      // 16 bits (+13) 
//#define M_USB_EP0_CONFIGDATA    *((volatile uint8_t*) (USB_EP0_CONFIGDATA  ))   // 8 bits        
//#define M_USB_EP0_FIFOSIZE	    *((volatile uint8_t*) (USB_EP0_FIFOSIZE	 ))  // 8 bits        
////Bulk                          *((volatile uint16_t*) (                    ))                    
//#define M_USB_BLK_TXMAXP	    *((volatile uint16_t*) (USB_BLK_TXMAXP	     ))  // 16 bits       
//#define M_USB_BLK_CSR0		    *((volatile uint16_t*) (USB_BLK_CSR0		 ))  // 16 bits (+9)  
//#define M_USB_BLK_TXCSR		    *((volatile uint16_t*) (USB_BLK_TXCSR		 ))  // 16 bits   
//#define M_USB_BLK_RXMAXP	    *((volatile uint16_t*) (USB_BLK_RXMAXP	     ))  // 16 bits       
//#define M_USB_BLK_RXCSR		    *((volatile uint16_t*) (USB_BLK_RXCSR		 ))  // 16 bits       
//#define M_USB_BLK_COUNT0	    *((volatile uint8_t*) (USB_BLK_COUNT0	     ))  // 8 bits (+7)   
//#define M_USB_BLK_RXCOUNT	    *((volatile uint16_t*) (USB_BLK_RXCOUNT	 ))      // 16 bits (+13) 
//#define M_USB_BLK_CONFIGDATA    *((volatile uint8_t*) (USB_BLK_CONFIGDATA  ))   // 8 bits        
//#define M_USB_BLK_FIFOSIZE	    *((volatile uint8_t*) (USB_BLK_FIFOSIZE	 ))  // 8 bits        


// Initial USB Registers
#define INIT_USB_INT_MASK 	 0x06//0x27 // discon, reset, resume, suspend
//#define INIT_USB_INT_MASK 	 0xff // discon, reset, resume, suspend
#define INIT_USB_EPX_TX_MASK 0x007F
#define INIT_USB_EPX_RX_MASK 0x007E

#if HIGH_SPEED
	#define INIT_USB_POWER		0x61		// High Speed
	#define INIT_USB_POWER_DOWN		0x21		// USB POWER DOWN
#else
	#define INIT_USB_POWER		0x41		// Full speed
	#define INIT_USB_POWER_DOWN		0x01		// USB POWER DOWN
#endif

#define INIT_USB_CSR0		0x0100
//#define INIT_USB_RXCSRn		0x90
#define INIT_USB_RXCSRn		(REGUSB_RXCSR_AutoClear | REGUSB_RXCSR_DMAReqEnab | REGUSB_RXCSR_DMAReqMode )
#define INIT_USB_TXCSRn		( 0x0048 | REGUSB_TXCSR_Autoset )

//Shutdown USB
#define SHUTDOWN_USB_POWER  0

// DMA TX
#define USB_DMA_CNTL_TX_MODE0(x) 	(0xb|(x<<4))
#define USB_DMA_CNTL_TX(x) 	        (0xf|(x<<4))
// DMA RX
#define USB_DMA_CNTL_RX_MODE0(x)  	(0x9|(x<<4))
#define USB_DMA_CNTL_RX(x)  	    (0xd|(x<<4))

// Interrupt Mask and Status Register for Endpoints
#define REGUSB_INT_EP(x) 	(1<<x)

// USB Interrupt Mask and Status bit set
#define REGUSB_INT_VBUSERR 	(1<<7)		// Valid in A device
#define REGUSB_INT_SESSREQ 	(1<<6)		// Valid in A device
#define REGUSB_INT_DISCON 	(1<<5)
#define REGUSB_INT_CONN 	(1<<4)		// Only valid in host mode
#define REGUSB_INT_SOF	 	(1<<3)
#define REGUSB_INT_RESET 	(1<<2)
#define REGUSB_INT_RESUME 	(1<<1)
#define REGUSB_INT_SUSPEND 	(1<<0)		// Only valid in peripheral mode

// Index select for Endpoint
//#define REGUSB_INDEX_EP0 	0
//#define REGUSB_INDEX_EP2 	2

#define REGUSB_CSR0_RxPktRdy		0x1
#define REGUSB_CSR0_TxPktRdy		0x2
#define REGUSB_CSR0_SentStall		0x4
#define REGUSB_CSR0_DataEnd			0x8
#define REGUSB_CSR0_SetupEnd		0x10
#define REGUSB_CSR0_SendStall		0x20
#define REGUSB_CSR0_ServRxPktRdy	0x40
#define REGUSB_CSR0_ServSetupEnd	0x80
#define REGUSB_CSR0_FlushFIFO		0x100


#define REGUSB_TXCSR_TxPktRdy		0x1
#define REGUSB_TXCSR_FIFONotEmpty	0x2
#define REGUSB_TXCSR_UnderRun		0x4
#define REGUSB_TXCSR_FlushFIFO		0x8
#define REGUSB_TXCSR_SendStall		0x10
#define REGUSB_TXCSR_SentStall		0x20
#define REGUSB_TXCSR_ClrDataTog		0x40
#define REGUSB_TXCSR_IncompTx		0x80
#define REGUSB_TXCSR_DMAReqMode		0x400
#define REGUSB_TXCSR_FrcDataTog		0x800
#define REGUSB_TXCSR_DMAReqEnab		0x1000
//#define REGUSB_TXCSR_Mode_TX		0x2000
//#define REGUSB_TXCSR_Mode_RX		0x000
#define REGUSB_TXCSR_ISO			0x4000
#define REGUSB_TXCSR_Autoset		0x8000

typedef struct {
	uint16_t Autoset 		: 1 ;
	uint16_t ISO 			: 1 ;
	uint16_t Mode	 		: 1 ;
	uint16_t DMAReqEnab		: 1 ;
	uint16_t FrcDataTog		: 1 ;
	uint16_t DMAReqMode		: 1 ;
	uint16_t reserved 		: 2 ;	// 8th bit
	uint16_t IncompTx 		: 1 ;
	uint16_t ClrDataTog		: 1 ;
	uint16_t SentStall		: 1 ;
	uint16_t SendStall 		: 1 ;
	uint16_t FlushFIFO 		: 1 ;
	uint16_t UnderRun 		: 1 ;
	uint16_t FIFONotEmpty	: 1 ;
	uint16_t TxPktRdy 		: 1 ;
} TXCSR ;


#define REGUSB_RXCSR_RxPktRdy		0x1
#define REGUSB_RXCSR_FIFOFull		0x2
#define REGUSB_RXCSR_OverRun		0x4
#define REGUSB_RXCSR_DataError		0x8
#define REGUSB_RXCSR_FlushFIFO		0x10
#define REGUSB_RXCSR_SendStall		0x20
#define REGUSB_RXCSR_SentStall		0x40
#define REGUSB_RXCSR_ClrDataTog	0x80
#define REGUSB_RXCSR_IncompRx		0x100
#define REGUSB_RXCSR_DMAReqMode	0x800
#define REGUSB_RXCSR_DisNyet		0x1000
#define REGUSB_RXCSR_DMAReqEnab	0x2000
#define REGUSB_RXCSR_ISO			0x4000
#define REGUSB_RXCSR_AutoClear		0x8000


typedef struct {
	uint16_t Autoset 		: 1 ;
	uint16_t ISO 			: 1 ;
	uint16_t DMAReqEnab		: 1 ;
	uint16_t DisNyet		: 1 ;
	uint16_t DMAReqMode		: 1 ;
	uint16_t reserved 		: 2 ;
	uint16_t IncompRx 		: 1 ;	// 8th bit
	uint16_t ClrDataTog		: 1 ;
	uint16_t SentStall		: 1 ;
	uint16_t SendStall 		: 1 ;
	uint16_t FlushFIFO 		: 1 ;
	uint16_t DataError 		: 1 ;
	uint16_t OverRun 		: 1 ;
	uint16_t FIFOFull		: 1 ;
	uint16_t RxPktRdy 		: 1 ;
} RXCSR ;

#define REGUSB_DMAINTR_CHANNEL1		0x01

#define REGUSB_DMACNTL_ENABLE		0x01
#define REGUSB_DMACNTL_RX			0x00
#define REGUSB_DMACNTL_TX			0x02
#define REGUSB_DMACNTL_DMAMODE		0x04
#define REGUSB_DMACNTL_INT			0x08
#define REGUSB_DMACNTL_EP_MASK		0xf0
#define REGUSB_DMACNTL_EP(x)		(0x0 | (x<<4))

#define REGUSB_DMACNTL_BUSERR		0x100
#define REGUSB_DMACNTL_BURST0		0x000
#define REGUSB_DMACNTL_BURST1		0x200
#define REGUSB_DMACNTL_BURST2		0x400
#define REGUSB_DMACNTL_BURST3		0x600

#define REGSUB_CONFIG_MPRxE    0x80
#define REGSUB_CONFIG_MPTxE    0x40

#endif 	/* multiple inclusion protection */