#pragma CODE = USB_HID_CODE
#pragma DATA = USB_HID_XRAM
unsigned int HID_rx_len;

unsigned char HID_get_idle( unsigned int id )
{
    return 0;
}

void HID_set_idle( unsigned int id, unsigned int value )
{
}

void HID_set_protocol( unsigned int value )
{
}

unsigned int HID_get_protocol( void )
{
    return 0;
}

typedef void (*report_rx_func)( unsigned int length );
extern __X unsigned char ep0_buf[];
void HID_report_rx( unsigned int length )
{
    int i, temp;
    for( i=0; i<length; ++i )
    {
        temp = ep0_buf[i];
    }
}

report_rx_func HID_set_report( unsigned int rx_len )
{
    HID_rx_len = rx_len;
    return HID_report_rx;
}

void HID_get_report( __X unsigned char* buf, unsigned int length )
{
    
}