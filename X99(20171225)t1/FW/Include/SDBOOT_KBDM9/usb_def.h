#ifndef __MUSBC_DEF_H__
#define __MUSBC_DEF_H__

#define USING_QUEUE 
#define USB_CVTEST 
#define IROM_CODE 


#define QUEUE_SIZE 4
#define USB_STATUS_SUCCESS 	0
#define USB_MEDIA_ERROR		(-1)
#define USB_PARAMETER_ERROR	(-2)
#define USB_CMD_NOT_SUPPORT	(-3)
#define USB_CMD_BUSY    	(-4)



#define AUDIO_SET_CUR			0x01
#define AUDIO_SET_MIN			0x02
#define AUDIO_SET_MAX			0x03
#define AUDIO_SET_RES			0x04


typedef unsigned long uint32_t ;
typedef unsigned int uint16_t  ;
typedef unsigned char uint8_t ;
typedef long int32_t ;
typedef int int16_t ;
typedef char int8_t ;

#define FIFO0_SIZE 64
//#define FIFO1_SIZE 64  // 512x2
#define FIFO1_SIZE fifo1_size_value
#define ISO_FIFO1_SIZE 512 //512  // 512x2
#define MEM_FIFO1_SIZE 512*2*2
#define FIFO2_SIZE 64

enum {
 EP0_STAGE_SETUP = 0,
 EP0_STAGE_DATA_W,
 EP0_STAGE_DATA_R,
 EP0_STAGE_END
} ;

enum {
	EP1_MODE_ISO	= 1,
	EP1_MODE_BULK 	= 2,
} ;

enum {
	DMA_STOP = 0,
	DMA_TX_END,
	DMA_RX_END,
	DMA_TX_GO,
	DMA_RX_GO,
	DMA_TX_GOING,
	DMA_RX_GOING
};

enum {
    USB_WAIT,
    USB_ATTACHED,
    USB_DISCONECTED,
    USB_ADDRESS = 0x40,
    USB_CONFIG,
};

#define get_ep0_remain_size() \
			ep0_len

#define current_ep0_buf_pointer() \
			ep0_ptr

#define update_ep0_remain_size( _remain_len ) \
			ep0_len = (_remain_len )

#define update_ep0_buf_pointer( _ep0_ptr ) \
			ep0_ptr = ( _ep0_ptr )

#define bulk_current_buf_pointer() \
			ep1_ptr

#define bulk_update_buf_pointer( _ep1_ptr ) \
			ep1_ptr = ( _ep1_ptr )

#define remain_size_to_TX() \
			ep1_len

#define remain_size_Store_TX( _remain_len ) \
			ep1_len = (_remain_len )

#define remain_size_to_RX() \
			ep1_len

#define remain_size_Store_RX( _remain_len ) \
			ep1_len = (_remain_len )
#endif 	/* multiple inclusion protection */