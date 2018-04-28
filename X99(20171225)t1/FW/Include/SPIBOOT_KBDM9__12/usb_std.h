#ifndef __MUSBC_STD_DEF_H__
#define __MUSBC_STD_DEF_H__

#include <usb_arch.h>
#include <usb_fnlst.h>

#ifndef USB_VAR
#define _AREA __X
#else
#define _AREA
#endif

// USB Core GLobal Variable
typedef struct {
	uint16_t reserved 		: 7 ;
	uint16_t FlushFIFO 		: 1 ;	// 8th bit
	uint16_t ServSetupEnd 	: 1 ;
	uint16_t ServRxPktRdy 	: 1 ;
	uint16_t SendStall		: 1 ;
	uint16_t SetupEnd		: 1 ;
	uint16_t DataEnd		: 1 ;
	uint16_t SentStall 		: 1 ;
	uint16_t TxPktRdy		: 1 ;
	uint16_t RxPktRdy		: 1 ;
} CSR0  ;


extern CSR0 _AREA csr0 ;

extern uint16_t  _AREA ep0_stage ;
extern uint32_t  _AREA ep0_len ;
extern uint16_t  _AREA ep0_buf[];
extern uint16_t* _AREA ep0_ptr;
extern uint16_t* _AREA ep1_ptr;
extern uint16_t  _AREA ep1_buf[];
extern uint32_t  _AREA ep1_len ;
extern uint16_t  _AREA fifo1_size_value;
extern uint16_t  _AREA usb_dma_intr;
extern uint16_t  _AREA usb_stage ;

extern void (* _AREA fp_Bulk_RX)(void) ;
extern void (* _AREA fp_Bulk_TX)(void) ;
extern void (* _AREA pfn_usb_suspend)(void);
extern void (* _AREA pfn_usb_resume)(void);
extern void (* _AREA pfn_usb_attach)(void);
extern int  (* _AREA fp_ep0_vendor_request)( __X uint16_t* psetup);
extern int  (* _AREA fp_ep0_class_request)( __X uint16_t* psetup);
extern void (* _AREA fp_USB_Vendor_Clear_Feature)(void);
extern void (* _AREA fp_USB_Vendor_Reset)(void);
extern void (* _AREA fp_TX_END_Vendor)(void);
extern void (* _AREA fp_RX_END_Vendor)(void);
//extern void (* _AREA fp_TX_ISO_END_Vendor)(void);
//extern void (* _AREA fp_RX_ISO_END_Vendor)(void);
extern void (* _AREA fp_ep0_RX)(unsigned int);
extern void (* _AREA fp_os_notify)(void);
extern int  (* _AREA fp_other_descriptor)( __X void* desc_buf, int req_len );

//in IRAM, because constant data
extern const uint8_t        USB_Test_Packet[];

extern const uint8_t*  _AREA p_USB_Device_Desc;
extern const uint8_t*  _AREA p_USB_Configuration_Desc;
extern const uint8_t*  _AREA p_USB_Configuration_FS_Desc;
extern const uint8_t*  _AREA p_Microsoft_OS_String;
extern const uint8_t** _AREA p_USB_String_Desc;
extern const uint8_t*  _AREA p_USB_Device_Qualifier_Desc;


extern uint8_t  _AREA USB_faddr ;
extern uint16_t _AREA ep1_mode ;
extern uint8_t  _AREA ep1_dma_state;
extern uint8_t  _AREA trans_remain ;

#define USB_DEVICE_QUALIFIER_DESC_LEN   10
#define USB_DEVICE_DESC_LEN             18
// usb standard request codes
enum {
 USB_GET_STATUS = 0,
 USB_CLEAR_FEATURE,
 USB_SET_FEATURE = 3,
 USB_SET_ADDRESS = 5,
 USB_GET_DESCRIPTOR,
 USB_SET_DESCRIPTOR,
 USB_GET_CONFIGURATION,
 USB_SET_CONFIGURATION,
 USB_GET_INTERFACE,
 USB_SET_INTERFACE,
 USB_SYNCH_FRAME
} ;

#define USB_GET_REPORT_DESCRIPTOR	    0x22
#define USB_GET_HID_CLASS_DESCRIPTOR	0x21

// usb descriptor types
enum {
	USB_DEVICE_TYPE = 1 ,
	USB_CONFIGURATION_TYPE,
	USB_STRING_TYPE,
	USB_INTERFACE_TYPE,
	USB_ENDPOINT_TYPE,
	USB_DEVICE_QUALIFIER,
	USB_OTHER_SPEED_CONFIGURATION,
	USB_INTERFACE_POWER
};

#define USB_REQUEST_MASK	0x60
#define USB_REQUEST_STD		0x0
#define USB_REQUEST_CLASS	0x20
#define USB_REQUEST_VENDOR	0x40

#ifdef BIG_ENDIAN

#define get_request_type( _request_buf ) \
			( ((_request_buf)[0] >>8 )& 0xff )


#define get_request_code( _request_buf ) \
			( (_request_buf)[0] & 0xff )

#define get_request_value( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[1]) )

#define get_request_index( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[2]) )

#define get_request_length( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[3]) )

#define get_request_value_swap( _request_buf ) \
			( (_request_buf)[1] )

#define get_request_index_swap( _request_buf ) \
			( (_request_buf)[2] )

#define get_request_length_swap( _request_buf ) \
			( (_request_buf)[3]) )

#else	// Little Endian
#define get_request_type( _request_buf ) \
			( (_request_buf)[0] & 0xff )


#define get_request_code( _request_buf ) \
			( ((_request_buf)[0] >>8 )& 0xff )


#define get_request_value( _request_buf ) \
			( (_request_buf)[1] )

#define get_request_index( _request_buf ) \
			( (_request_buf)[2] )

#define get_request_length( _request_buf ) \
			( ((_request_buf)[3] )


#define get_request_value_swap( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[1]) )

#define get_request_index_swap( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[2]) )

#define get_request_length_swap( _request_buf ) \
			( MUSBC_SWAP16((_request_buf)[3]) )
#endif	// Endian Difference




#endif
