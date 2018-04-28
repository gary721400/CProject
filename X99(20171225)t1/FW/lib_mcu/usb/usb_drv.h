//! @file usb_drv.h,v
//!
//! Copyright (c) 2006 Atmel.
//!
//! Use of this program is subject to Atmel's End User License Agreement.
//! Please read file license.txt for copyright notice.
//!
//! @brief This file contains the USB low level driver definition
//!
//! @version 2.2 snd3-dvk-1_9_5 $Id: usb_drv.h,v 2.2 2007/08/22 14:11:28 sguyon Exp $
//!
//! @todo
//! @bug


#ifndef _USB_DRV_H_
#define _USB_DRV_H_

//_____ I N C L U D E S ____________________________________________________

#include "modules\power\power.h"             // Power module definition

//! @defgroup USB_low_level_drivers USB low level drivers module
//! USB low level drivers Module
//! @{

//_____ M A C R O S ________________________________________________________

#define MAX_EP_NB                7

#define EP_CONTROL               0
#define EP_1                     1
#define EP_2                     2
#define EP_3                     3
#define EP_4                     4
#define EP_5                     5
#define EP_6                     6
#define EP_7                     7
                              
#define PIPE_CONTROL             0
#define PIPE_0                   0
#define PIPE_1                   1
#define PIPE_2                   2
#define PIPE_3                   3
#define PIPE_4                   4
#define PIPE_5                   5
#define PIPE_6                   6
#define PIPE_7                   7


// Parameters for endpoint configuration
// These define are the values used to enable and configure an endpoint.
#define TYPE_CONTROL             0
#define TYPE_ISOCHRONOUS         1
#define TYPE_BULK                2
#define TYPE_INTERRUPT           3

#define DIRECTION_OUT            0
#define DIRECTION_IN             1

#define SIZE_8                   0
#define SIZE_16                  1
#define SIZE_32                  2
#define SIZE_64                  3
#define SIZE_128                 4
#define SIZE_256                 5
#define SIZE_512                 6
#define SIZE_1024                7

#define ONE_BANK                 0
#define TWO_BANKS                1

#define NYET_ENABLED             0
#define NYET_DISABLED            1

#define TOKEN_SETUP              0
#define TOKEN_IN                 1
#define TOKEN_OUT                2

#define Is_ep_addr_in( ep_add )                          (ep_add & MSK_EP_DIR)


//! @defgroup Endpoints_configuration Configuration macros for endpoints
//! List of the standard macro used to configure pipes and endpoints
//! @{
#define Usb_build_ep_config0(type, dir, nyet)            (((type)<<6) | ((nyet)<<1) | (dir))
#define Usb_build_ep_config1(size, bank     )            (((size)<<4) | ((bank)<<2)        )
#define usb_configure_endpoint(num, type, dir, size, bank, nyet)             \
                                                         ( Usb_select_endpoint( num ),              \
                                                         usb_config_ep( Usb_build_ep_config0( type, dir, nyet ),\
                                                         Usb_build_ep_config1( size, bank )))

#define Host_build_pipe_config0(type, token, ep_num)     (((type)<<6) | ((token)<<4) | (ep_num))
#define Host_build_pipe_config1(size, bank     )         (((size)<<4) | ((bank)<<2)        )
#define host_configure_pipe(num, type, token,ep_num, size, bank, freq)             \
                                                         ( Host_select_pipe( num ),              \
                                                         Host_set_interrupt_frequency( freq ), \
                                                         host_config_pipe( Host_build_pipe_config0( type, token, ep_num ),\
                                                         Host_build_pipe_config1( size, bank )))
//! @}
         

//! @defgroup USB Pads drivers
//! Turns ON/OFF USB pads which scan VBUS signal
//! @{
#define Usb_enable_vbus_pad()                            //(USBCON |=  MSK_OTGPADE)
#define Usb_disable_vbus_pad()                           //(USBCON &= ~MSK_OTGPADE)
#define Is_usb_enable_vbus_pad()                         (USBCON &   MSK_OTGPADE)
//! @}

//! @defgroup gen_usb USB common management drivers
//! These macros manage the USB controller
//! @{
   //! Enable external UID pin
#define Usb_enable_uid_pin()            
//AVR #define Usb_enable_uid_pin()                       {U8 delay;(UHWCON |= (1<<UIDE));delay=PORTA;}
   //! Disable external UID pin
#define Usb_disable_uid_pin()           
   //! Force UID pin to force device mode in macro
#define Usb_force_device_mode()                          (P3_7    =  1)
   //! Force UID pin to force host mode in macro
#define Usb_force_host_mode()                            (P3_7    =  0)

   //! Use device hight speed mode (no possible then full)
//AVR #define Usb_high_speed_mode()
#define Usb_high_speed_mode()                            (UDTST   =  (UDTST & ~MSK_SPDCONF) | 2)
   //! For device mode, force full speed mode
#define Usb_full_speed_mode()                            (aDEVCSR[0] &= BIT1 )
#define Usb_full_speed_mode_utmi()                       (UDTST   =  (UDTST & ~MSK_SPDCONF) | 3)
   //! For device mode, force low speed mode
#define Usb_low_speed_mode()                                         // Specific AVR
   //! For device mode, enable speed detection between full and high speed
#define Usb_normal_speeed_mode()                         (UDTST   =  (UDTST & ~MSK_SPDCONF))
   //! tests if the USB controller runs in high speed
//AVR #define Is_usb_high_speed()                        (FALSE)
#define Is_usb_high_speed()                              (USBSTA  &  MSK_SPEED)
   //! tests if the USB controller runs in full speed
#define Is_host_full_speed()                             (TRUE)

   //! Enable both USB interface and Vbus pad
#define Usb_enable()                                     //(USBCON |=  MSK_USBE)
   //! Disable both USB interface and Vbus pad
#define Usb_disable()                                    //(USBCON &= ~MSK_USBE)
   //! tests if the USB controller is enabled
#define Is_usb_enabled()                                 (USBCON &   MSK_USBE)

   //! Enable VBUS pad
#define Usb_enable_vbus_pad()                            //(USBCON |=  MSK_OTGPADE)
   //! Disable VBUS pad
#define Usb_disable_vbus_pad()                           //(USBCON &= ~MSK_OTGPADE)
   //! tests if the VBUS pad is enabled
#define Is_usb_vbus_pad_enabled()                        (USBCON &   MSK_OTGPADE)


#define Usb_select_device()                              //(USBCON &= ~MSK_HOST)
#define Usb_select_host()                                //(USBCON |=  MSK_HOST)
#define Is_usb_host_enabled()                            //(USBCON &   MSK_HOST)
#define Is_usb_device_enabled()                          //(0==(USBCON &  MSK_HOST))


   //! Stop internal USB clock in interface (freeze the interface register)
#define Usb_freeze_clock()                               //(USBCON |=  MSK_FRZCLK)
#define Usb_unfreeze_clock()                             //(USBCON &= ~MSK_FRZCLK)
#define Is_usb_clock_freezed()                           //(USBCON &  MSK_FRZCLK)

//AVR #define  Usb_enable_interrupt()        Enable_interrupt()
#define  Usb_enable_interrupt()                          Int_ex0_enable()
#define  Usb_disable_interrupt()                         Int_ex0_disable()

#define Usb_enable_id_interrupt()                        (USBCON |=  MSK_IDTE)   //!< enables the ID change interrupt
#define Usb_disable_id_interrupt()                       (USBCON &= ~MSK_IDTE)   //!< disables the ID change interrupt
#define Is_usb_id_interrupt_enabled()                    (USBCON &  MSK_IDTE)
#define Is_usb_id_device()                               (USBSTA &  MSK_ID)
#define Is_usb_id_host()                                 (0 == (USBSTA &  MSK_ID))
#define Usb_ack_id_transition()                          (USBINT  = ~MSK_IDTI)
#define Is_usb_id_transition()                           (USBINT &  MSK_IDTI)
                                       
/*
#define Usb_enable_vbus_interrupt()                      (USBCON |=  MSK_VBUSTE) //!< enables the VBus change interrupt
#define Usb_disable_vbus_interrupt()                     (USBCON &= ~MSK_VBUSTE) //!< disables the VBus change interrupt
#define Is_usb_vbus_interrupt_enabled()                  (USBCON &  MSK_VBUSTE)
#define Is_usb_vbus_high()                               (USBSTA &  MSK_VBUS)    // HERE PATCH SND3 BUG PULL UP residue
#define Is_usb_vbus_low()                                (0 == (USBSTA &  MSK_VBUS))
#define Usb_ack_vbus_transition()                        (USBINT  = ~MSK_VBUSTI)
#define Is_usb_vbus_transition()                         (USBINT &  MSK_VBUSTI)
*/

// Patch for SND3B
extern Bool g_b_usb_drv_vbus_state;
#define Usb_enable_vbus_interrupt()                      
#define Usb_disable_vbus_interrupt()                     
#define Is_usb_vbus_interrupt_enabled()                  (0)
#define Is_usb_vbus_high()                               power_is_vbus_present()
#define Is_usb_vbus_low()                                (!power_is_vbus_present())
#define Usb_ack_vbus_transition()                        (g_b_usb_drv_vbus_state = Is_usb_vbus_high())
#define Is_usb_vbus_transition()                         (g_b_usb_drv_vbus_state != Is_usb_vbus_high())

   //! returns the USB Pad interrupts (interrupt enabled)
#define Usb_get_otg_interrupt()                          (OTGINT  & OTGIEN)
   //! acks the USB Pad interrupts (interrupt enabled)
#define Usb_ack_all_otg_interrupt()                      (OTGINT  = ~OTGIEN)
#define Is_otg_cache_bconnection_error(x)                (((x)    &  MSK_BCERRI))
#define Usb_ack_cache_bconnection_error(x)               ((x)    &= ~MSK_BCERRI)

#define Usb_enter_dpram_mode()                           (UDPADDH =  MSK_DPACC)
#define Usb_exit_dpram_mode()                            (UDPADDH = ~MSK_DPACC)
#define Usb_set_dpram_address(addr)                      (UDPADDH =  MSK_DPACC + ((Uint16)addr >> 8), UDPADDL = (Uchar)addr)
#define Usb_write_dpram_byte(val)                        (UEDATX  =  val)
#define Usb_read_dpram_byte()			                     (UEDATX)

   //! requests for VBus activation
#define Usb_enable_vbus()                                (OTGCON |=  MSK_VBUSREQ)
   //! requests for VBus desactivation
#define Usb_disable_vbus()                               (OTGCON |=  MSK_VBUSRQC)
   //! Manually request VBUS ON
#define Usb_enable_vbus_manual()                         (P3_6=0)
   //! Manually request VBUS OFF
#define Usb_disable_vbus_manual()                        (P3_6=1)
   //! is vbus manually activated ?
#define Is_usb_vbus_manual_on()                          FALSE    // Specific AVR

   //! initiates a Host Negociation Protocol
#define Usb_device_initiate_hnp()                        (OTGCON |=  MSK_HNPREQ)
   //! stops a Host Negociation Protocol
#define Usb_device_stop_hnp()                            (OTGCON &= ~MSK_HNPREQ)
   //! accepts a Host Negociation Protocol
#define Usb_host_accept_hnp()                            (OTGCON |=  MSK_HNPREQ)
   //! rejects a Host Negociation Protocol
#define Usb_host_reject_hnp()                            (OTGCON &= ~MSK_HNPREQ)
   //! initiates a Session Request Protocol
#define Usb_device_initiate_srp()                        (OTGCON |=  MSK_SRPREQ)
   //! selects VBus as SRP method
#define Usb_select_vbus_srp_method()                     (OTGCON |=  MSK_SRPSEL)
   //! selects data line as SRP method
#define Usb_select_data_srp_method()                     (OTGCON &= ~MSK_SRPSEL)
   //! enables hardware control on VBus
#define Usb_enable_vbus_hw_control()                     (OTGCON &= ~MSK_VBUSHWC)
   //! disables hardware control on VBus
#define Usb_disable_vbus_hw_control()                    (OTGCON |=  MSK_VBUSHWC)
   //! tests if VBus has been requested
#define Is_usb_vbus_enabled()                            (OTGCON &  MSK_VBUSREQ)
   //! tests if a HNP occurs
#define Is_usb_hnp()                                     (OTGCON &  MSK_HNPREQ)
   //! tests if a SRP from device occurs
#define Is_usb_device_srp()                              (OTGCON &  MSK_SRPREQ)
   //! tests if device is initiating SRP
#define Is_usb_device_initiating_srp()                   (OTGCON &  MSK_SRPREQ)

//! selects one of the four OTG timers and sets up the specified delay
#define Set_otg_custom_timer(sel)                        (OTGTCON =  sel)

   //! enables suspend time out interrupt
#define Usb_enable_suspend_time_out_interrupt()          (OTGIEN |=  MSK_STOE)
   //! disables suspend time out interrupt
#define Usb_disable_suspend_time_out_interrupt()         (OTGIEN &= ~MSK_STOE)
#define Is_suspend_time_out_interrupt_enabled()          (OTGIEN &  MSK_STOE)
   //! acks suspend time out interrupt
#define Usb_ack_suspend_time_out_interrupt()             (OTGINT &= ~MSK_STOI)
   //! tests if a suspend time out occurs
#define Is_usb_suspend_time_out_interrupt()              (OTGINT &  MSK_STOI)

   //! enables HNP error interrupt
#define Usb_enable_hnp_error_interrupt()                 (OTGIEN |=  MSK_HNPERRE)
   //! disables HNP error interrupt
#define Usb_disable_hnp_error_interrupt()                (OTGIEN &= ~MSK_HNPERRE)
#define Is_hnp_error_interrupt_enabled()                 (OTGIEN &  MSK_HNPERRE)
   //! acks HNP error interrupt
#define Usb_ack_hnp_error_interrupt()                    (OTGINT &= ~MSK_HNPERRI)
   //! tests if a HNP error occurs
#define Is_usb_hnp_error_interrupt()                     (OTGINT&   MSK_HNPERRI)

   //! enables role exchange interrupt
#define Usb_enable_role_exchange_interrupt()             (OTGIEN |=  MSK_ROLEEXE)
   //! disables role exchange interrupt
#define Usb_disable_role_exchange_interrupt()            (OTGIEN &= ~MSK_ROLEEXE)
#define Is_role_exchange_interrupt_enabled()             (OTGIEN &  MSK_ROLEEXE)
   //! acks role exchange interrupt
#define Usb_ack_role_exchange_interrupt()                (OTGINT &= ~MSK_ROLEEXI)
   //! tests if a role exchange occurs
#define Is_usb_role_exchange_interrupt()                 (OTGINT&   MSK_ROLEEXI)

   //! enables B device connection error interrupt
#define Usb_enable_bconnection_error_interrupt()         (OTGIEN |=  MSK_BCERRE)
   //! disables B device connection error interrupt
#define Usb_disable_bconnection_error_interrupt()        (OTGIEN &= ~MSK_BCERRE)
#define Is_bconnection_error_interrupt_enabled()         (OTGIEN &  MSK_BCERRE)
   //! acks B device connection error interrupt
#define Usb_ack_bconnection_error_interrupt()            (OTGINT &= ~MSK_BCERRI)
   //! tests if a B device connection error occurs
#define Is_usb_bconnection_error_interrupt()             (OTGINT &   MSK_BCERRI)

   //! enables VBus error interrupt
#define Usb_enable_vbus_error_interrupt()                (OTGIEN |=  MSK_VBERRE)
   //! disables VBus error interrupt
#define Usb_disable_vbus_error_interrupt()               (OTGIEN &= ~MSK_VBERRE)
#define Is_vbus_error_interrupt_enabled()                (OTGIEN&   MSK_VBERRE)
   //! acks VBus error interrupt
#define Usb_ack_vbus_error_interrupt()                   (OTGINT &= ~MSK_VBERRI)
   //! tests if a VBus error occurs
#define Is_usb_vbus_error_interrupt()                    (OTGINT&   MSK_VBERRI)

   //! enables SRP interrupt
#define Usb_enable_srp_interrupt()                       (OTGIEN |=  MSK_SRPE)
   //! disables SRP interrupt
#define Usb_disable_srp_interrupt()                      (OTGIEN &= ~MSK_SRPE)
#define Is_srp_interrupt_enabled()                       (OTGIEN &  MSK_SRPE)
   //! acks SRP interrupt
#define Usb_ack_srp_interrupt()                          (OTGINT &= ~MSK_SRPI)
   //! tests if a SRP occurs
#define Is_usb_srp_interrupt()                           (OTGINT &  MSK_SRPI)
//! @}


//! @defgroup USB_device_driver USB device controller drivers
//! These macros manage the USB Device controller.
//! @{
   //! initiates a remote wake-up
#define Usb_initiate_remote_wake_up()                    (UDCON   |=  MSK_RMWKUP)
   //! detaches from USB bus
#define Usb_detach()                                     //(UDCON   |=  MSK_DETACH)
   //! attaches to USB bus
#define Usb_attach()                                     (UDCON   &= ~MSK_DETACH)
   //! test if remote wake-up still running
#define Is_usb_pending_remote_wake_up()                  (UDCON   & MSK_RMWKUP)
   //! test if the device is detached
#define Is_usb_detached()                                ((aDEVCSR[0]& BIT2)== 0)//(aINT_SRC[3]& BIT2)//

   //! returns the USB device interrupts (interrupt enabled)
#define Usb_get_device_interrupt()                       (UDINT   &   UDIEN)
   //! acks the USB device interrupts (interrupt enabled)
#define Usb_ack_all_device_interrupt()                   (UDINT   =  ~UDIEN)

   //! enables remote wake-up interrupt
#define Usb_enable_remote_wake_up_interrupt()            (UDIEN   |=  MSK_UPRSME)
   //! disables remote wake-up interrupt
#define Usb_disable_remote_wake_up_interrupt()           (UDIEN   &= ~MSK_UPRSME)
#define Is_remote_wake_up_interrupt_enabled()            (UDIEN   &   MSK_UPRSME)
   //! acks remote wake-up
#define Usb_ack_remote_wake_up_start()                   (UDINT   =  ~MSK_UPRSMI)
   //! tests if remote wake-up still running
#define Is_usb_remote_wake_up_start()                    (UDINT   &   MSK_UPRSMI)

   //! enables resume interrupt
#define Usb_enable_resume_interrupt()                    (UDIEN   |=  MSK_EORSME)
   //! disables resume interrupt
#define Usb_disable_resume_interrupt()                   (UDIEN   &= ~MSK_EORSME)
#define Is_resume_interrupt_enabled()                    (UDIEN   &   MSK_EORSME)
   //! acks resume
#define Usb_ack_resume()                                 (UDINT   =  ~MSK_EORSMI)
   //! tests if resume occurs
#define Is_usb_resume()                                  (UDINT   &   MSK_EORSMI)

   //! enables wake-up interrupt
#define Usb_enable_wake_up_interrupt()                   (UDIEN   |=  MSK_WAKEUPE)
   //! disables wake-up interrupt
#define Usb_disable_wake_up_interrupt()                  (UDIEN   &= ~MSK_WAKEUPE)
#define Is_swake_up_interrupt_enabled()                  (UDIEN   &   MSK_WAKEUPE)
   //! acks wake-up
#define Usb_ack_wake_up()                                (UDINT   =  ~MSK_WAKEUPI)
   //! tests if wake-up occurs
#define Is_usb_wake_up()                                 (UDINT   &   MSK_WAKEUPI)

   //! enables USB reset interrupt
#define Usb_enable_reset_interrupt()                     (UDIEN   |=  MSK_EORSTE)
   //! disables USB reset interrupt
#define Usb_disable_reset_interrupt()                    (aINT_MSK[1] &= ~BIT0)
#define Is_reset_interrupt_enabled()                     (aINT_MSK[1] & BIT0)
   //! acks USB reset
#define Usb_ack_reset()                                  (UDINT   =  ~MSK_EORSTI)
   //! tests if USB reset occurs
#define Is_usb_reset()                                   (UDINT   &   MSK_EORSTI)

   //! enables Start Of Frame Interrupt
#define Usb_enable_sof_interrupt()                       (UDIEN   |=  MSK_SOFE)
   //! disables Start Of Frame Interrupt
#define Usb_disable_sof_interrupt()                      (UDIEN   &= ~MSK_SOFE)
#define Is_sof_interrupt_enabled()                       (UDIEN   &   MSK_SOFE)
   //! acks Start Of Frame
#define Usb_ack_sof()                                    (UDINT   =  ~MSK_SOFI)
   //! tests if Start Of Frame occurs
#define Is_usb_sof()                                     (UDINT   &   MSK_SOFI)

   //! enables suspend state interrupt
#define Usb_enable_suspend_interrupt()                   (UDIEN   |=  MSK_SUSPE)
   //! disables suspend state interrupt
#define Usb_disable_suspend_interrupt()                  (aINT_MSK[0] &= ~BIT3)
#define Is_suspend_interrupt_enabled()                   (aINT_MSK[0] & BIT3)
   //! acks Suspend
#define Usb_ack_suspend()                                (UDINT   =  ~MSK_SUSPI)
   //! tests if Suspend state detected
#define Is_usb_suspend()                                 (UDINT   &   MSK_SUSPI)

   //! enables USB device address
#define Usb_enable_address()                             (UDADDR  |=  MSK_ADDEN)
   //! disables USB device address
#define Usb_disable_address()                            (UDADDR  &= ~MSK_ADDEN)
   //! sets the USB device address
#define Usb_configure_address(addr)                      (UDADDR  = (UDADDR & MSK_ADDEN) | ((U8)addr & MSK_UADD))

   //! returns the last frame number
#define Usb_frame_number()                               ((U16)((((U16)UDFNUMH) << 8) | ((U16)UDFNUML)))
   //! tests if a crc error occurs in frame number
#define Is_usb_frame_number_crc_error()                  (UDMFN   &   MSK_FNCERR)
//! @}




//! @defgroup usb_gen_ep USB endpoint drivers
//! These macros manage the common features of the endpoints.
//! @{
   //! selects the endpoint number to interface with the CPU
#define Usb_select_endpoint(ep)                          (UECONX  &= ~MSK_EPNUMS, UENUM = (U8)(ep))
   //! selects the endpoint number to interface with the DFC
#define Usb_select_endpoint_dfc(ep)                      (UECONX  |=  MSK_EPNUMS, UENUM = (U8)(ep))

   //! enables USB to DFC interface
#define Usb_enable_dfc()                                 (UECONX  |=  MSK_DFCRDY)
   //! disables USB to DFC interface
#define Usb_disable_dfc()                                (UECONX  &= ~MSK_DFCRDY)

   //! get the currently selected endpoint number
#define Usb_get_selected_endpoint()                      (UENUM)

   //! resets the selected endpoint
#define Usb_reset_endpoint(ep)                           (UERST = 1<<(U8)ep, UERST=0)

   //! enables the current endpoint
#define Usb_enable_endpoint()                            (UECONX  |=  MSK_EPEN)
   //! enables the STALL handshake for the next transaction
#define Usb_enable_stall_handshake()                     (UECONX  |=  MSK_STALLRQ)
   //! resets the data toggle sequence
#define Usb_reset_data_toggle()                          (UECONX  |=  MSK_RSTDT)
   //! disables the current endpoint
#define Usb_disable_endpoint()                           (UECONX  &= ~MSK_EPEN)
   //! disables the STALL handshake
#define Usb_disable_stall_handshake()                    (UECONX  |=  MSK_STALLRQC)
   //! selects endpoint interface on CPU
#define Usb_select_epnum_for_cpu()                       (UECONX  &= ~MSK_EPNUMS)
   //! selects endpoint interface on DFC
#define Usb_select_epnum_for_dfc()                       (UECONX  |=  MSK_EPNUMS)
   //! tests if the current endpoint is enabled
#define Is_usb_endpoint_enabled()                        (UECONX  &   MSK_EPEN)
   //! tests if STALL handshake request is running
#define Is_usb_endpoint_stall_requested()                (UECONX  &   MSK_STALLRQ)

   //! configures the current endpoint
#define Usb_configure_endpoint_type(type)                (UECFG0X =   (UECFG0X & ~(MSK_EPTYPE)) | ((U8)type << 6))
   //! configures the current endpoint direction
#define Usb_configure_endpoint_direction(dir)            (UECFG0X =   (UECFG0X & ~(MSK_EPDIR))  | ((U8)dir))
   //! enables autoswitch bank
#define Usb_enable_autoswitch()                          (UECFG0X |=  MSK_AUTOSW)       
   //! disables autoswitch bank
#define Usb_disable_autoswitch()                         (UECFG0X &= ~MSK_AUTOSW)       
   //! enables the isochronous autoswitch
#define Usb_enable_iso_autoswitch()                      (UECFG0X |=  MSK_ISOSW)        
   //! disables enables the isochronous autoswitch
#define Usb_disable_iso_autoswitch()                     (UECFG0X &= ~MSK_ISOSW)        
   //! enables Not Yet generation
#define Usb_enable_nyet()                                (UECFG0X &= ~MSK_NYETDIS)      
   //! disables Not Yet generation
#define Usb_disable_nyet()                               (UECFG0X |=  MSK_NYETDIS)      

   //! configures the current endpoint size
#define Usb_configure_endpoint_size(size)                (UECFG1X =   (UECFG1X & ~MSK_EPSIZE) | ((U8)size << 4))
   //! configures the current endpoint number of banks
#define Usb_configure_endpoint_bank(bank)                (UECFG1X =   (UECFG1X & ~MSK_EPBK)   | ((U8)bank << 2))
   //! allocates the current configuration in DPRAM memory
#define Usb_allocate_memory()                            (UECFG1X |=  MSK_ALLOC)
   //! un-allocates the current configuration in DPRAM memory
#define Usb_unallocate_memory()                          (UECFG1X &= ~MSK_ALLOC)

   //! acks endpoint overflow interrupt
#define Usb_ack_overflow_interrupt()                     (UESTA0X &= ~MSK_OVERFI)
   //! acks endpoint underflow memory
#define Usb_ack_underflow_interrupt()                    (UESTA0X &= ~MSK_UNDERFI)
   //! acks Zero Length Packet received
#define Usb_ack_zlp()                                    (UESTA0X &= ~MSK_ZLPSEEN)
   //! returns data toggle
#define Usb_data_toggle()                                ((UESTA0X & MSK_DTSEQ) >> 2)
   //! returns the number of busy banks
#define Usb_nb_busy_bank()                               (UESTA0X &   MSK_NBUSYBK)
   //! tests if at least one bank is busy
#define Is_usb_one_bank_busy()                           ((UESTA0X &  MSK_NBUSYBK) != 0)
   //! tests if current endpoint is configured
#define Is_endpoint_configured()                         (UESTA0X &   MSK_CFGOK)
   //! tests if an overflows occurs
#define Is_usb_overflow()                                (UESTA0X &   MSK_OVERFI)
   //! tests if an underflow occurs
#define Is_usb_underflow()                               (UESTA0X &   MSK_UNDERFI)
   //! tests if a ZLP has been detected
#define Is_usb_zlp()                                     (UESTA0X &   MSK_ZLPSEEN)

   //! returns the control direction
#define Usb_control_direction()                          ((UESTA1X &  MSK_CTRLDIR) >> 2)
   //! returns the number of the current bank
#define Usb_current_bank()                               (UESTA1X &   MSK_CURRBK)


   //! clears FIFOCON bit
#define Usb_ack_fifocon()                                (FIFOCON = 0)
   //! acks NAK IN received
#define Usb_ack_nak_in()                                 (NAKINI  = 0)
   //! acks NAK OUT received
#define Usb_ack_nak_out()                                (NAKOUTI = 0)
   //! acks receive SETUP
#define Usb_ack_receive_setup()                          (RXSTPI  = 0)
   //! acks reveive OUT
#define Usb_ack_receive_out()                            (RXOUTI  = 0, Usb_ack_fifocon())
   //! acks STALL sent
#define Usb_ack_stalled()                                (STALLI  = 0)
   //! acks IN ready
#define Usb_ack_in_ready()                               (TXINI   = 0, Usb_ack_fifocon())
   //! Kills last bank
#define Usb_kill_last_in_bank()                          (RXOUTI  = 1)
   //! tests if endpoint read allowed
#define Is_usb_read_enabled()                            (RWAL)
   //! tests if endpoint write allowed
#define Is_usb_write_enabled()                           (RWAL)
   //! tests if read allowed on control endpoint
#define Is_usb_read_control_enabled()                    (TXINI)
   //! tests if a NAK has been sent
#define Is_usb_nak_out_sent()                            (NAKOUTI)
   //! tests if SETUP received
#define Is_usb_receive_setup()                           (aINT_SRC[3]& BIT5)
   //! tests if OUT received
#define Is_usb_receive_out()                             (RXOUTI)
   //! tests if IN ready
#define Is_usb_in_ready()                                (TXINI)
   //! sends IN
#define Usb_send_in()                                    (FIFOCON = 0)
   //! sends IN on control endpoint
#define Usb_send_control_in()                            (TXINI   = 0)
   //! frees OUT bank
#define Usb_free_out_bank()                              (FIFOCON = 0)
   //! acks OUT on control endpoint
#define Usb_ack_control_out()                            (RXOUTI  = 0)


   //! enables flow error interrupt
#define Usb_enable_flow_error_interrupt()                (UEIENX  |=  MSK_FLERRE)
   //! enables NAK IN interrupt
#define Usb_enable_nak_in_interrupt()                    (UEIENX  |=  MSK_NAKINE)
   //! enables NAK OUT interrupt
#define Usb_enable_nak_out_interrupt()                   (UEIENX  |=  MSK_NAKOUTE)
   //! enables receive SETUP interrupt
#define Usb_enable_receive_setup_interrupt()             (UEIENX  |=  MSK_RXSTPE)
   //! enables receive OUT interrupt
#define Usb_enable_receive_out_interrupt()               (UEIENX  |=  MSK_RXOUTE)
   //! enables STALL sent interrupt
#define Usb_enable_stalled_interrupt()                   (UEIENX  |=  MSK_STALLEDE)
   //! enables IN ready interrupt
#define Usb_enable_in_ready_interrupt()                  (UEIENX  |=  MSK_TXIN)
   //! disables flow error interrupt
#define Usb_disable_flow_error_interrupt()               (UEIENX  &= ~MSK_FLERRE)
   //! disables NAK IN interrupt
#define Usb_disable_nak_in_interrupt()                   (UEIENX  &= ~MSK_NAKINE)
   //! disables NAK OUT interrupt
#define Usb_disable_nak_out_interrupt()                  (UEIENX  &= ~MSK_NAKOUTE)
   //! disables receive SETUP interrupt
#define Usb_disable_receive_setup_interrupt()            (UEIENX  &= ~MSK_RXSTPE)
   //! disables receive OUT interrupt
#define Usb_disable_receive_out_interrupt()              (UEIENX  &= ~MSK_RXOUTE)
   //! disables STALL sent interrupt
#define Usb_disable_stalled_interrupt()                  (UEIENX  &= ~MSK_STALLEDE)
   //! disables IN ready interrupt
#define Usb_disable_in_ready_interrupt()                 (UEIENX  &= ~MSK_TXIN)

   //! returns FIFO byte for current endpoint
#define Usb_read_byte()                                  (UEDATX)
   //! writes byte in FIFO for current endpoint
#define Usb_write_byte(byte)                             (UEDATX  =   (U8)byte)

   //! returns number of bytes in FIFO current endpoint (16 bits)
#define Usb_byte_counter()                               ((((U16)UEBCHX) << 8) | (UEBCLX))
   //! returns number of bytes in FIFO current endpoint (8 bits)
#define Usb_byte_counter_8()                             ((U8)UEBCLX)

   //! tests the general endpoint interrupt flags
#define Usb_interrupt_flags()                            (UEINT)
   //! tests the general endpoint interrupt flags
#define Is_usb_endpoint_event()                          (Usb_interrupt_flags() != 0x00)
//! @}



//! @defgroup host_management USB host controller drivers
//! These macros manage the USB Host controller.
//! @{
   //! allocates the current configuration in DPRAM memory
#define Host_allocate_memory()                           (UPCFG1X |=  MSK_ALLOC)
   //! un-allocates the current configuration in DPRAM memory
#define Host_unallocate_memory()                         (UPCFG1X &= ~MSK_ALLOC)
   //! Check if pipe memory is allocated
#define Is_host_pipe_memory_allocated()                  (UPCFG1X &   MSK_ALLOC)

   //! enables USB Host function
#define Host_enable()                                    (USBCON  |=  MSK_HOST)
#ifndef    SOFEN
   #define    SOFEN           0       //For AVRGCC, SOFEN bit missing in default sfr file
#endif
   //! enables SOF generation
#define Host_enable_sof()                                (UHCON   |=  MSK_SOFEN)
   //! disables SOF generation
#define Host_disable_sof()                               (UHCON   &= ~MSK_SOFEN)
   //! sends a USB Reset to the device
#define Host_send_reset()                                (UHCON   |=  MSK_RESET)
   //! tests if USB Reset running
#define Host_is_reset()                                  (UHCON   &   MSK_RESET)
   //! sends a USB Resume to the device
#define Host_send_resume()                               (UHCON   |=  MSK_RESUME)
   //! tests if USB Resume running
#define Host_is_resume()                                 (UHCON   &   MSK_RESUME)
   //! Read SOF number
#define Host_sof_number()                                ((U16)(UHFNUMH<<8 | UHFNUML))

   //! enables host start of frame interrupt
#define Host_enable_sof_interrupt()                      (UHIEN   |=  MSK_HSOFE)
   //! enables host start of frame interrupt
#define Host_disable_sof_interrupt()                     (UHIEN   &= ~MSK_HSOFE)
#define Is_host_sof_interrupt_enabled()                  (UHIEN   &   MSK_HSOFE)
   //! tests if SOF detected
#define Is_host_sof()                                    (UHINT   &   MSK_HSOFI)
#define Host_ack_sof()                                   (UHINT   &= ~MSK_HSOFI)

   //! enables host wake up interrupt detection
#define Host_enable_hwup_interrupt()                     (UHIEN   |=  MSK_HWUPE)
   //! disables host wake up interrupt detection
#define Host_disable_hwup_interrupt()                    (UHIEN   &= ~MSK_HWUPE)
#define Is_host_hwup_interrupt_enabled()                 (UHIEN   &   MSK_HWUPE)
   //! tests if host wake up detected
#define Host_is_hwup()                                   (UHINT   &   MSK_HWUPI)
   //! Ack host wake up detection
#define Host_ack_hwup()                                  (UHINT   &= ~MSK_HWUPI)

   //! enables host down stream rsm sent interrupt detection
#define Host_enable_down_stream_resume_interrupt()       (UHIEN   |=  MSK_RSMEDE)
   //! disables host down stream rsm sent interrupt detection
#define Host_disable_down_stream_resume_interrupt()      (UHIEN   &= ~MSK_RSMEDE)
#define Is_host_down_stream_resume_interrupt_enabled()   (UHIEN   &   MSK_RSMEDE)
   //! Ack host down stream resume sent
#define Is_host_down_stream_resume()                     (UHINT   &   MSK_RSMEDI)
#define Host_ack_down_stream_resume()                    (UHINT   &= ~MSK_RSMEDI)

   //! enables host remote wake up interrupt detection
#define Host_enable_remote_wakeup_interrupt()            (UHIEN   |=  MSK_RXRSME)
   //! disables host remote wake up interrupt detection
#define Host_disable_remote_wakeup_interrupt()           (UHIEN   &= ~MSK_RXRSME)
#define Is_host_remote_wakeup_interrupt_enabled()        (UHIEN   &   MSK_RXRSME)
   //! tests if host wake up detected
#define Host_is_remote_wakeup()                          (UHINT   &   MSK_RXRSMI)
   //! Ack host wake up detection
#define Is_host_remote_wakeup()                          (UHINT   &   MSK_RXRSMI)
#define Host_ack_remote_wakeup()                         (UHINT   &= ~MSK_RXRSMI)

   //! enables host device connection interrupt
#define Host_enable_device_connection_interrupt()        (UHIEN   |=  MSK_DCONNE)
   //! disables USB device connection interrupt
#define Host_disable_device_connection_interrupt()       (UHIEN   &= ~MSK_DCONNE)
#define Is_host_device_connection_interrupt_enabled()    (UHIEN   &   MSK_DCONNE)
   //! tests if a USB device has been detected
#define Is_device_connection()                           (UHINT   &   MSK_DCONNI)
   //! acks device connection
#define Host_ack_device_connection()                     (UHINT   =  ~MSK_DCONNI)

   //! enables host device disconnection interrupt
#define Host_enable_device_disconnection_interrupt()     (UHIEN   |=  MSK_DDISCE)
   //! disables USB device connection interrupt
#define Host_disable_device_disconnection_interrupt()    (UHIEN   &= ~MSK_DDISCE)
#define Is_host_device_disconnection_interrupt_enabled() (UHIEN   &   MSK_DDISCE)
   //! tests if a USB device has been removed
#define Is_device_disconnection()                        (UHINT   &   MSK_DDISCI)
   //! acks device disconnection
#define Host_ack_device_disconnection()                  (UHINT   =  ~MSK_DDISCI)

   //! enables host USB reset interrupt
#define Host_enable_reset_interrupt()                    (UHIEN   |=  MSK_RSTE)
   //! disables host USB reset interrupt
#define Host_disable_reset_interrupt()                   (UHIEN   &= ~MSK_RSTE)
#define Is_host_reset_interrupt_enabled()                (UHIEN   &   MSK_RSTE)
   //! acks host USB reset sent
#define Host_ack_reset()                                 (UHINT   =  ~MSK_RSTI)
   //! tests if USB reset has been sent
#define Is_host_reset()                                  Host_is_reset()


   //! configures the address to use for the device
#define Host_configure_address(addr)                     (UHADDR  = addr & MSK_HADDR)

//! @}



//! @defgroup general_pipe USB pipe drivers
//! These macros manage the common features of the pipes.
//! @{
   //! selects pipe for CPU interface
#define Host_select_pipe(p)                              (UPCONX  &= ~MSK_PNUMS, UPNUM = (U8)p)
   //! selects pipe for DFC interface
#define Host_select_pipe_dfc(p)                          (UPCONX  |=  MSK_PNUMS, UPNUM = (U8)p)

   //! get the currently selected pipe number
#define Host_get_selected_pipe()                         (UPNUM)

   //! enables pipe
#define Host_enable_pipe()                               (UPCONX  |=  MSK_PEN)
   //! disables pipe
#define Host_disable_pipe()                              (UPCONX  &= ~MSK_PEN)
   //! enables DFC interface for current pipe
#define Host_enable_dfc()                                (UPCONX  |=  MSK_DFCRDY)       
   //! disables DFC interface for current pipe
#define Host_disable_dfc()                               (UPCONX  &= ~MSK_DFCRDY)       
#define Is_host_enable_dfc()                             (UPCONX  &   MSK_DFCRDY)       
   //! sets SETUP token
#define Host_set_token_setup()                           (UPCFG0X =  UPCFG0X & ~MSK_TOKEN_SETUP)
   //! sets IN token
#define Host_set_token_in()                              (UPCFG0X = (UPCFG0X & ~MSK_TOKEN_SETUP) | MSK_TOKEN_IN)
   //! sets OUT token
#define Host_set_token_out()                             (UPCFG0X = (UPCFG0X & ~MSK_TOKEN_SETUP) | MSK_TOKEN_OUT)

   //! returns the number of the endpoint associated to the current pipe
#define Host_get_endpoint_number()                       (UPCFG0X &   MSK_PEPNUM)

   //! returns pipe interrupt register
#define Host_get_pipe_interrupt()                        (UPINT)

   //! sets the interrupt frequency
#define Host_set_interrupt_frequency(frq)                (UPCFG2X = (U8)frq)
                                                        
   //! tests if current pipe is configured              
#define Is_pipe_configured()                             (UPSTAX  &   MSK_CFGOK)
   //! tests if at least one bank is busy               
#define Is_host_one_bank_busy()                          ((UPSTAX &   MSK_NBUSYBK) != 0)
   //! returns the number of busy banks                 
#define Host_number_of_busy_bank()                       (UPSTAX  &   MSK_NBUSYBK)
                                                        
   //! resets the pipe                                  
#define Host_reset_pipe(p)                               (UPRST = 1<<p , UPRST = 0)
                                                        
   //! writes a byte into the pipe FIFO                 
#define Host_write_byte(dat)                             (UPDATX  = dat)
   //! reads a byte from the pipe FIFO                  
#define Host_read_byte()                                 (UPDATX)
                                                        
   //! freezes the pipe                                 
#define Host_freeze_pipe()                               (UPCONX  |=  MSK_PFREEZE)
   //! un-freezees the pipe                             
#define Host_unfreeze_pipe()                             (UPCONX  &= ~MSK_PFREEZE)
   //! tests if the current pipe is frozen              
#define Is_host_pipe_freeze()                            (UPCONX  &   MSK_PFREEZE)
   //! enables autoswitch bank
#define Host_set_autoswitch_bank()                       (UPCONX  |=  MSK_HAUTOSW)
   //! disables autoswitch bank
#define Host_clear_autoswitch_bank()                     (UPCONX  &= ~MSK_HAUTOSW)
   //! tests if autoswitch bank is enabled
#define Is_host_autoswitch_bank()                        (UPCONX  &   MSK_HAUTOSW)
   //! resets data toggle                               
#define Host_reset_pipe_data_toggle()                    (UPCONX  |=  MSK_RSTDT)
                                                        
   //! tests if SETUP has been sent                     
#define Is_host_setup_sent()                             (TXSTPI)
   //! tests if control IN has been received            
#define Is_host_control_in_received()                    (RXINI)
   //! tests if control OUT has been sent               
#define Is_host_control_out_sent()                       (TXOUTI)
   //! tests if a STALL has been received               
#define Is_host_stall()                                  (RXSTALLI)
   //! tests if an error occurs on current pipe
#define Is_host_pipe_error()                             (PERRI)
   //! sends a setup                           
#define Host_send_setup()                                (UPINTX  &= ~MSK_FIFOCON)
   //! sends a control IN                      
#define Host_send_control_in()                           (UPINTX  &= ~MSK_FIFOCON)
   //! sends a control OUT                     
#define Host_send_control_out()                          (UPINTX  &= ~MSK_FIFOCON)
   //! acks control OUT                        
#define Host_ack_control_out()                           (UPINTX  &= ~MSK_TXOUTI)
   //! acks control IN                         
#define Host_ack_control_in()                            (UPINTX  &= ~MSK_RXINI)
   //! acks setup                              
#define Host_ack_setup()                                 (UPINTX  &= ~MSK_TXSTPI)
   //! acks STALL reception                    
#define Host_ack_stall()                                 (UPINTX  &= ~MSK_RXSTALLI)
                                               
   //! sends a OUT                             
#define Host_send_out()                                  (UPINTX  &= ~MSK_FIFOCON)
   //! tests if OUT has been sent              
#define Is_host_out_sent()                               (TXOUTI)
   //! acks OUT sent                           
#define Host_ack_out_sent()                              (UPINTX  &= ~MSK_TXOUTI)
   //! tests if IN received                    
//#define Is_host_in_received()                            (RXINI)
#define Is_host_in_received()                            (UPINTX  &   MSK_FIFOCON)
   //! acks IN reception                       
//#define Host_ack_in_received()                           (UPINTX  &= ~MSK_RXINI)
#define Host_ack_in_received()                           (UPINTX  &= ~MSK_FIFOCON)
   //! sends a IN                              
//#define Host_send_in()                                   (UPINTX  &= ~MSK_FIFOCON)
#define Host_send_in()                                   (UPINTX  &= ~MSK_RXINI)
   //! tests if nak handshake has been received
#define Is_host_nak_received()                           (NAKEDI)
   //! acks NAk received sent                  
#define Host_ack_nak_received()                          (UPINTX  &= ~MSK_NAKEDI)
                                               
                                               
                                               
   //! tests if endpoint read allowed          
#define Is_host_read_enabled()                           (UPINTX  &   MSK_RWAL)
   //! tests if endpoint read allowed                   
#define Is_host_write_enabled()                          (UPINTX  &   MSK_RWAL)
                                                        
   //! sets IN in standard mode                         
#define Host_standard_in_mode()                          (UPCONX  &= ~MSK_INMODE)
   //! sets IN in continuous mode              
#define Host_continuous_in_mode()                        (UPCONX  |=  MSK_INMODE)

   //! sets number of IN requests to perform before freeze
#define Host_in_request_number(in_num)                   (UPINRQX = (U8)in_num)
   //! returns number of remaining IN requests
#define Host_get_in_request_number()                     (UPINRQX)
                                                        
   //! returns number of bytes (8 bits)                 
#define Host_data_length_U8()                            (UPBCLX)
   //! returns number of bytes (16 bits)                
#define Host_data_length_U16()                           ((((U16)UPBCHX)<<8) | UPBCLX)
   //! for device compatibility                         
#define Host_byte_counter()                              Host_data_length_U16()
   //! for device compatibility                         
#define Host_byte_counter_8()                            Host_data_length_U8()
                                                        
   //! returns the size of the current pipe             
#define Host_get_pipe_length()                           ((U16)0x08 << ((UPCFG1X & MSK_PSIZE)>>4))
                                                        
   //! returns the type of the current pipe             
#define Host_get_pipe_type()                             (UPCFG0X>>6)
                                                        
   //! tests if error occurs on pipe                    
#define Host_error_status()                              (UPERRX & MSK_ERROR)
   //! acks all pipe error                              
#define Host_ack_all_errors()                            (UPERRX = 0x00)
                                                        
   //! Enable pipe end transmission interrupt           
#define Host_enable_transmit_interrupt()                 (UPIENX  |=  MSK_TXOUTE)
   //! Disable pipe end transmission interrupt
#define Host_disable_transmit_interrupt()                (UPIENX  &= ~MSK_TXOUTE)
                                                        
   //! Enable pipe reception interrupt                  
#define Host_enable_receive_interrupt()                  (UPIENX  |=  MSK_RXINE)
   //! Disable pipe recption interrupt                  
#define Host_disable_receive_interrupt()                 (UPIENX  &= ~MSK_RXINE)

   //! Enable pipe stall interrupt
#define Host_enable_stall_interrupt()                    (UPIENX  |=  MSK_RXSTALLE)
   //! Disable pipe stall interrupt                     
#define Host_disable_stall_interrupt()                   (UPIENX  &= ~MSK_RXSTALLE)
                                                        
   //! Enable pipe error interrupt                      
#define Host_enable_error_interrupt()                    (UPIENX  |=  MSK_PERRE)
   //! Disable pipe error interrupt                     
#define Host_disable_error_interrupt()                   (UPIENX  &= ~MSK_PERRE)
                                                        
   //! Enable pipe NAK interrupt                        
#define Host_enable_nak_interrupt()                      (UPIENX  |=  MSK_NAKEDE)
   //! Disable pipe NAK interrupt                       
#define Host_disable_nak_interrupt()                     (UPIENX  &= ~MSK_NAKEDE)
                                                        
#define Get_pipe_token(x)                                ((x & MSK_EP_DIR) ? TOKEN_IN : TOKEN_OUT)

//! @}

//! wSWAP
//! This macro swaps the U8 order in words.
//!
//! @param x        (U16) the 16 bit word to swap
//!
//! @return         (U16) the 16 bit word x with the 2 bytes swaped

#define wSWAP(x)        \
   (   (((x)>>8)&0x00FF) \
   |   (((x)<<8)&0xFF00) \
   )


//! Usb_write_word_enum_struc
//! This macro help to fill the U16 fill in USB enumeration struct.
//! Depending on the CPU architecture, the macro swap or not the nibbles
//!
//! @param x        (U16) the 16 bit word to be written
//!
//! @return         (U16) the 16 bit word written
#ifdef LITTLE_ENDIAN
	#define Usb_write_word_enum_struc(x)	(x)
#else //BIG_ENDIAN
	#define Usb_write_word_enum_struc(x)	(wSWAP(x))
#endif


//! @}

//_____ D E C L A R A T I O N ______________________________________________

U8       usb_config_ep                 ( U8, U8 );
U8       usb_select_enpoint_interrupt  ( void );
U16      usb_get_nb_byte_epw           ( void );
U8       usb_send_packet               ( U8 , U8*, U8 );
U8       usb_read_packet               ( U8 , U8*, U8 );
void     usb_halt_endpoint             ( U8 );
void     usb_reset_endpoint            ( U8 );
U8       usb_init_device               ( void );
                                         
U8       host_config_pipe              ( U8, U8 );
U8       host_determine_pipe_size      ( U16 );
void     host_disable_all_pipe         ( void );
U8       usb_get_nb_pipe_interrupt     ( void );

#endif  // _USB_DRV_H_

