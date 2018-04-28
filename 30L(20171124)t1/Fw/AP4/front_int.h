// 
//		<<<<< 1. front-end >>>>>
//

void  frontend_StallFifoIn             ( void ); 
void  frontend_StallFifoOut            ( void );   //040625
void  frontend_WaitStallClear          ( void );
//void  frontend_MarkShortPacket         ( void );

//040625
void  frontend_RcvData                 ( USHORT XferLen );
void  frontend_SendData                ( USHORT XferLen );
//Bool  frontend_TransferDone            ( void );
void  frontend_DecSectorCount          ( void );

Bool  sent_data                        ( void );
Bool  FrontEnd_RcvDataReq_512B         ( Byte buffer_number);
Bool  FrontEnd_SendDataReq_512B        ( Byte buffer_number);

#define frontend_RcvData512( x )       FrontEnd_RcvDataReq_512B( x )
#define frontend_SendData512( x )      FrontEnd_SendDataReq_512B( x )

void  FrontEnd_RcvDataReq_2KB          ( Byte buffer_number );
Bool  FrontEnd_SendDataReq_2KB         ( Byte buffer_number );
Bool  FrontEnd_SendDataReq_512B_fast   ( Byte buffer_number );
void  FrontEnd_SendDataReq_2KB_fast    ( Byte byBuffNo );


Bool buffer_ready();

#define buffer_ready_macro()              \
{                                         \
   while(( USB_DMA_CTRL & 0x88 ) != 0 );  \
   USB_DMA_2KB = 0x00;                    \
}

void  frontend_GetCBW   ( void );
void  frontend_SendCSW  ( void );

//GET_CIS_COMMAND
#define DISK_PAGE       0
#define VENDOR_PAGE     1
#define PASSWD_PAGE     2

#define backend_Init()  BackEnd_Init();
//#define load_disk_info() read_cis();  //06/08/2003, jacky

void  HostDmaEnable     ( UCHAR buf_no, UCHAR count, Bool fIn ); 
//#define HostDmaWaitReady()	while( HostDmaCount );

Bool  buffer_ready_2k   ( void );

