#ifndef _USB_FNLST_H
#define _USB_FNLST_H

#include <usb_def.h>

void std_Bulk_RX(void) ;
void std_Bulk_TX(void) ;

int musbc_setbit8( uint32_t* port_addr,  uint32_t val ) ;
int musbc_setbit16( uint32_t* port_addr,  uint32_t val ) ;
int musbc_setbit32( uint32_t* port_addr,  uint32_t val ) ;
int musbc_clrbit8( uint32_t* port_addr,  uint32_t val ) ;
int musbc_clrbit16( uint32_t* port_addr,  uint32_t val ) ;
int musbc_clrbit32( uint32_t* port_addr,  uint32_t val ) ;

void DisConn_ISR( void ) ;
void usb_ep0_isr( void );
void usb_ep0_rx(void) ;
void ep0_standard_request(void);
void usb_ep0_tx(void) ;
void usb_ep1_init_bulk_mode(void);
void usb_resume_isr( void );
void usb_reset_isr( void );
void usb_suspend_isr( void );
void usb_ep1_tx_isr(void);
void usb_ep1_rx_isr(void);
void usb_ep1_tx_flush(void);
void usb_ep1_rx_flush(void);
void usb_ep1_tx_stall(void);
void usb_ep1_rx_stall(void);
void usb_ep1_rx_init(void);
void usb_ep1_tx_init(void);
void usb_ep1_init_ISO_mode(void);
void usb_init_interface_func(unsigned int interface, unsigned int setting);
void usb_dma_tx_go(void);
void usb_dma_rx_go(void);
void usbdc_init(void);
//void usb_isr(void);
void usb_shutdown(void);
int  usb_get_descriptor( uint16_t* setup_cmd );
//void mymemcpy( void* dst, const void* src, unsigned long len );
#define mymemcpy memcpy

// Prototype for Vendor Request Code
//extern int  ep0_class_request( uint16_t* psetup);
extern int  ep0_vendor_request( __X uint16_t* psetup);
//extern void TX_END_Vendor(void);
//extern void RX_END_Vendor(void);
//extern void TX_ISO_END_Vendor(void);
//extern void RX_ISO_END_Vendor(void);
//extern void DMA_TX_Backend( void );
//extern void DMA_RX_Backend( void );
//extern void USB_Vendor_Clear_Feature(void);
//extern void USB_Vendor_Reset(void);
void usb_empty_func(void);
void usb_empty_func1(unsigned int);
int  usb_empty_func2(void);
int  usb_empty_func3(__X void*, int);
// USB API
void USBAPI_init( void );
void USBAPI_reg_bulk_tx( void (*fn_Bulk_TX)(void)  );
void USBAPI_reg_bulk_rx( void (*fn_Bulk_RX)(void)  );
void USBAPI_EP1_Mode_Select( int mode ) ;

void usbdc_loop(void);
#endif