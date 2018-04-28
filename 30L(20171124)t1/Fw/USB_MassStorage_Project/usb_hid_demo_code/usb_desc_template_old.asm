_MSC_IF_INDEX      == 0
_HID_IF_INDEX      == 0
.even
.area USB_STORAGE_IRAM(USB_STORAGE_IRAM)
_USB_Device_Desc::
	.DB  #012H        ;
	.DB  #001H        ;
	.DB  #000H, #002H ;
;    .DB  #010H, #001H ;
	.DB  #000H        ;
	.DB  #000H        ;
	.DB  #000H        ;
	.DB  #040H        ;
	.DB  #0DFH, #016H ;
	.DB  #0FFH, #000H ;
	.DB  #000H, #001H ;
	.DB  #001H        ;
	.DB  #002H        ;
	.DB  #000H        ;
	.DB  #001H        ;
_end_device_descriptor:
String00::                    
        .db     #004h         
        .db     #003h         
        .db     #009h, #004h  
String01::                    
    $begin:                   
        .db     #($end-$begin)
        .db     #003h         
        .db     #056H,#00H     
        .db     #065H,#00H     
        .db     #06EH,#00H     
        .db     #064H,#00H     
        .db     #06FH,#00H     
        .db     #072H,#00H     
    $end:                     
String02::                    
    $begin:                   
        .db     #($end-$begin)
        .db     #003h         
        .db     #050H,#00H     
        .db     #072H,#00H     
        .db     #06FH,#00H     
        .db     #064H,#00H     
        .db     #075H,#00H     
        .db     #063H,#00H     
        .db     #074H,#00H     
    $end:                     
String03::                    
    $begin:                   
        .db     #($end-$begin)
        .db     #003h         
        .db     #031H,#00H     
        .db     #032H,#00H     
        .db     #033H,#00H     
        .db     #034H,#00H     
        .db     #035H,#00H     
        .db     #036H,#00H     
    $end:                     
   _end_string_descriptor:    
_Microsoft_OS_String::        
    .db  #00H                  
    .db  #00H                  
  _end_os_string:             
.even
_USB_Configuration_Desc::   
    .DB  #009H              
    .DB  #002H              
    .DB  #<(_end_config_descriptor - _USB_Configuration_Desc)
    .DB  #000H
    .DB  #001H        
    .DB  #001H                  
    .DB  #000H                  
    .DB  #080H                  
    .DB  #032H                  
   $HID_descriptor03:           
    .DB  #9                     
    .DB  #004H                  
    .DB  #000H        
    .DB  #000H                  
    .DB  #001H                  
    .DB  #003H                  
    .DB  #000H                  
    .DB  #003H                  
    .DB  #000H                  
   $HID_INTERFACE_Descriptor:  
    .db  #009h                   
    .db  #021h                   
    .db  #010h                   
    .db  #001h                   
    .db  #021h                   
    .db  #001h                   
    .db  #022h                   
    .db  #<(_end_report_descriptor - _USB_Report_Descrption)
    .db  #000H
  $hid_endpoint_descriptor00:   
    .db  #007h                   
    .db  #005h                   
    .db  #082h                   
    .db  #003h                   
    .db  #020h                   
    .db  #000h                   
    .db  #000h                   
   _end_config_descriptor:      
.even
_USB_Configuration_Desc_FS::   
    .DB  #009H              
    .DB  #002H              
    .DB  #<(_end_config_descriptor_FS - _USB_Configuration_Desc_FS)
    .DB  #000H
    .DB  #001H        
    .DB  #001H                  
    .DB  #000H                  
    .DB  #080H                  
    .DB  #032H                  
   $HID_descriptor03:           
    .DB  #9                     
    .DB  #004H                  
    .DB  #000H        
    .DB  #000H                  
    .DB  #001H                  
    .DB  #003H                  
    .DB  #000H                  
    .DB  #003H                  
    .DB  #000H                  
   $HID_INTERFACE_Descriptor:  
    .db  #009h                   
    .db  #021h                   
    .db  #010h                   
    .db  #001h                   
    .db  #021h                   
    .db  #001h                   
    .db  #022h                   
    .db  #<(_end_report_descriptor - _USB_Report_Descrption)
    .db  #000H
  $hid_endpoint_descriptor00:   
    .db  #007h                   
    .db  #005h                   
    .db  #082h                   
    .db  #003h                   
    .db  #020h                   
    .db  #000h                   
    .db  #000h                   
   _end_config_descriptor_FS:      
.even
_USB_Report_Descrption:: 
	.db	#005h, #001h                     
    .db #009h, #004h        
    .db #0A1h, #001h        
    .db #0A1h, #002h        
    .db #075h, #008h        
    .db #095h, #004h        
	.db #015h, #000h                     
    .db #026h, #0FFh, #000h 
    .db #035h, #000h        
    .db #046h, #0FFh, #000h 
    .db #009h, #032h        
    .db #009h, #035h        
    .db #009h, #030h        
    .db #009h, #031h        
    .db #081h, #002h        
    .db #075h, #001h        
    .db #095h, #008h        
    .db #025h, #001h        
    .db #045h, #001h        
    .db #005h, #009h        
    .db #019h, #001h        
    .db #029h, #008h        
    .db #081h, #002h        
    .db #0C0h               
    .db #0A1h, #002h        
    .db #075h, #008h        
    .db #095h, #040h        
    .db #046h, #0FFh, #000h 
    .db #026h, #0FFh, #000h 
    .db #009h, #002h        
    .db #091h, #002h        
    .db #0C0h               
    .db #0C0h               
   _end_report_descriptor::            
_USB_Device_Qualifier_Desc::
	.DB  #00AH			            
	.DB  #006H                      
	.DB  #000H, #002H               
	.DB  #000H                      
	.DB  #000H                      
	.DB  #000H                      
	.DB  #040H                      
	.DB  #001H                      
    .DB  #000H                      
   _end_device_qualifier_desc:      
