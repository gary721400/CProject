_MSC_IF_INDEX      == 0
.even
.area USB_STORAGE_IRAM(USB_STORAGE_IRAM)
_USB_Device_Desc::
	.DB  #012H        ;
	.DB  #001H        ;
	.DB  #000H, #002H ;
	.DB  #000H        ;
	.DB  #000H        ;
	.DB  #000H        ;
	.DB  #040H        ;
	.DB  #0DFH, #016H ;
	.DB  #034H, #012H ;
	.DB  #000H, #001H ;
	.DB  #001H        ;
	.DB  #002H        ;
	.DB  #003H        ;
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
   $interface_descriptor00:      
    .DB  #9                     
    .DB  #004H                  
    .DB  #000H                  
    .DB  #000H                  
    .DB  #002H                  
    .DB  #008H                  
    .DB  #006H                  
    .DB  #050H                  
    .DB  #000H                  
   $endpoint_descriptor00:       
    .db  #7                      
    .db  #005h                   
    .db  #081h                   
    .db  #002h                   
    .db  #000h, #002h            
    .db  #00fh                   
   $endpoint_descriptor01:       
    .db  #7                      
    .db  #005h                   
    .db  #001h                   
    .db  #002h                   
    .db  #000h, #002h            
    .db  #00fh                   
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
   $interface_descriptor00:      
    .DB  #9                     
    .DB  #004H                  
    .DB  #000H                  
    .DB  #000H                  
    .DB  #002H                  
    .DB  #008H                  
    .DB  #006H                  
    .DB  #050H                  
    .DB  #000H                  
   $endpoint_descriptor00:       
    .db  #7                      
    .db  #005h                   
    .db  #081h                   
    .db  #002h                   
    .db  #040h, #000h            
    .db  #00fh                   
   $endpoint_descriptor01:       
    .db  #7                      
    .db  #005h                   
    .db  #001h                   
    .db  #002h                   
    .db  #040h, #000h            
    .db  #00fh                   
   _end_config_descriptor_FS:      
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
