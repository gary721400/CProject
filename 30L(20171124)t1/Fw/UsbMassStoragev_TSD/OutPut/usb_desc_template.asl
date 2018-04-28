000000                  1 _MSC_IF_INDEX      == 0
+                       2 .even
+                       3 .area USB_STORAGE_IRAM(USB_STORAGE_IRAM)
+B 00 01
000000                  4 _USB_Device_Desc::
000000  12              5 	.DB  #012H        ;
000001  01              6 	.DB  #001H        ;
000002  00 02           7 	.DB  #000H, #002H ;
000004  00              8 	.DB  #000H        ;
000005  00              9 	.DB  #000H        ;
000006  00             10 	.DB  #000H        ;
000007  40             11 	.DB  #040H        ;
000008  DF 16          12 	.DB  #0DFH, #016H ;
00000A  34 12          13 	.DB  #034H, #012H ;
00000C  00 01          14 	.DB  #000H, #001H ;
00000E  01             15 	.DB  #001H        ;
00000F  02             16 	.DB  #002H        ;
000010  03             17 	.DB  #003H        ;
000011  01             18 	.DB  #001H        ;
000012                 19 _end_device_descriptor:
000012                 20 String00::                    
000012  04             21         .db     #004h         
000013  03             22         .db     #003h         
000014  09 04          23         .db     #009h, #004h  
000016                 24 String01::                    
+                      25     $begin:                   
000016  0E             26         .db     #($end-$begin)
000017  03             27         .db     #003h         
000018  56 00          28         .db     #056H,#00H     
00001A  65 00          29         .db     #065H,#00H     
00001C  6E 00          30         .db     #06EH,#00H     
00001E  64 00          31         .db     #064H,#00H     
000020  6F 00          32         .db     #06FH,#00H     
000022  72 00          33         .db     #072H,#00H     
+                      34     $end:                     
000024                 35 String02::                    
+                      36     $begin:                   
000024  10             37         .db     #($end-$begin)
000025  03             38         .db     #003h         
000026  50 00          39         .db     #050H,#00H     
000028  72 00          40         .db     #072H,#00H     
00002A  6F 00          41         .db     #06FH,#00H     
00002C  64 00          42         .db     #064H,#00H     
00002E  75 00          43         .db     #075H,#00H     
000030  63 00          44         .db     #063H,#00H     
000032  74 00          45         .db     #074H,#00H     
+                      46     $end:                     
000034                 47 String03::                    
+                      48     $begin:                   
000034  0E             49         .db     #($end-$begin)
000035  03             50         .db     #003h         
000036  31 00          51         .db     #031H,#00H     
000038  32 00          52         .db     #032H,#00H     
00003A  33 00          53         .db     #033H,#00H     
00003C  34 00          54         .db     #034H,#00H     
00003E  35 00          55         .db     #035H,#00H     
000040  36 00          56         .db     #036H,#00H     
+                      57     $end:                     
000042                 58    _end_string_descriptor:    
000042                 59 _Microsoft_OS_String::        
000042  00             60     .db  #00H                  
000043  00             61     .db  #00H                  
000044                 62   _end_os_string:             
+                      63 .even
000044                 64 _USB_Configuration_Desc::   
000044  09             65     .DB  #009H              
000045  02             66     .DB  #002H              
000046  20             67     .DB  #<(_end_config_descriptor - _USB_Configuration_Desc)
000047  00             68     .DB  #000H
000048  01             69     .DB  #001H        
000049  01             70     .DB  #001H                  
00004A  00             71     .DB  #000H                  
00004B  80             72     .DB  #080H                  
00004C  32             73     .DB  #032H                  
+                      74    $interface_descriptor00:      
00004D  09             75     .DB  #9                     
00004E  04             76     .DB  #004H                  
00004F  00             77     .DB  #000H                  
000050  00             78     .DB  #000H                  
000051  02             79     .DB  #002H                  
000052  08             80     .DB  #008H                  
000053  06             81     .DB  #006H                  
000054  50             82     .DB  #050H                  
000055  00             83     .DB  #000H                  
+                      84    $endpoint_descriptor00:       
000056  07             85     .db  #7                      
000057  05             86     .db  #005h                   
000058  81             87     .db  #081h                   
000059  02             88     .db  #002h                   
00005A  00 02          89     .db  #000h, #002h            
00005C  0F             90     .db  #00fh                   
+                      91    $endpoint_descriptor01:       
00005D  07             92     .db  #7                      
00005E  05             93     .db  #005h                   
00005F  01             94     .db  #001h                   
000060  02             95     .db  #002h                   
000061  00 02          96     .db  #000h, #002h            
000063  0F             97     .db  #00fh                   
000064                 98    _end_config_descriptor:      
+                      99 .even
000064                100 _USB_Configuration_Desc_FS::   
000064  09            101     .DB  #009H              
000065  02            102     .DB  #002H              
000066  20            103     .DB  #<(_end_config_descriptor_FS - _USB_Configuration_Desc_FS)
000067  00            104     .DB  #000H
000068  01            105     .DB  #001H        
000069  01            106     .DB  #001H                  
00006A  00            107     .DB  #000H                  
00006B  80            108     .DB  #080H                  
00006C  32            109     .DB  #032H                  
+                     110    $interface_descriptor00:      
00006D  09            111     .DB  #9                     
00006E  04            112     .DB  #004H                  
00006F  00            113     .DB  #000H                  
000070  00            114     .DB  #000H                  
000071  02            115     .DB  #002H                  
000072  08            116     .DB  #008H                  
000073  06            117     .DB  #006H                  
000074  50            118     .DB  #050H                  
000075  00            119     .DB  #000H                  
+                     120    $endpoint_descriptor00:       
000076  07            121     .db  #7                      
000077  05            122     .db  #005h                   
000078  81            123     .db  #081h                   
000079  02            124     .db  #002h                   
00007A  40 00         125     .db  #040h, #000h            
00007C  0F            126     .db  #00fh                   
+                     127    $endpoint_descriptor01:       
00007D  07            128     .db  #7                      
00007E  05            129     .db  #005h                   
00007F  01            130     .db  #001h                   
000080  02            131     .db  #002h                   
000081  40 00         132     .db  #040h, #000h            
000083  0F            133     .db  #00fh                   
000084                134    _end_config_descriptor_FS:      
000084                135 _USB_Device_Qualifier_Desc::
000084  0A            136 	.DB  #00AH			            
000085  06            137 	.DB  #006H                      
000086  00 02         138 	.DB  #000H, #002H               
000088  00            139 	.DB  #000H                      
000089  00            140 	.DB  #000H                      
00008A  00            141 	.DB  #000H                      
00008B  40            142 	.DB  #040H                      
00008C  01            143 	.DB  #001H                      
00008D  00            144     .DB  #000H                      
00008E                145    _end_device_qualifier_desc:      
+
+Symbol Table
+
+    SPIBOOT_KBDM9 v1.06  000001 
+  1 String00  000012 GR
+  1 String01  000016 GR
+  1 String02  000024 GR
+  1 String03  000034 GR
+  0 _MSC_IF_INDEX =000000 GR
+  1 _Microsoft_OS_String  000042 GR
+  1 _USB_Configuration_Desc  000044 GR
+  1 _USB_Configuration_Desc_FS  000064 GR
+  1 _USB_Device_Desc  000000 GR
+  1 _USB_Device_Qualifier_Desc  000084 GR
+  1 _end_config_descriptor  000064 R
+  1 _end_config_descriptor_FS  000084 R
+  1 _end_device_descriptor  000012 R
+  1 _end_device_qualifier_desc  00008E R
+  1 _end_os_string  000044 R
+  1 _end_string_descriptor  000042 R
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 USB_STORAGE_IRAM size   8E   flags 0
