+                       1  
+                       5  
+                       6  .area _$USB_MassStorage_SD2_c$(ROM,REL) 
+B 00 01
000000                  7  Ltext0: 
+                      41  .include "kbccmacro.asm" 
000000                 42   .define _sdtl_mount	 _sd1_mount        
000000                 43   .define _sdtl_resv_size	 _sd1_res_size     
000000                 44   .define _sdtl_get_size	 _sd1_fat_size     
000000                 45   .define _sdtl_resv_read	 _sd1_res_read     
000000                 46   .define _sdtl_resv_write	 _sd1_res_write    
000000                 47   .define _sdtl_read	 _sd1_read         
000000                 48   .define _sdtl_write	 _sd1_write        
000000                 49   .define _sdtl_cache_init	 _sd1_cache2k_init 
000000                 50   .define _sdtl_cache_read	 _sd1_cache_read   
000000                 51   .define _sdtl_cache_write _sd1_cache_write  
000000                 52   .define _sdtl_cache_flush _sd1_cache_flush  
000000                 53   .define _sdtl_allocate    _sd1_allocate     
000000                 54   .define _spif_res_size	 _spif_resv_size   
000000                 55   .define _spif_res_read	 _spif_resv_read   
000000                 56   .define _spif_res_write	 _spif_resv_write  
+                     246  
+                     247  .area _$USB_MassStorage_SD2_c$(ROM,REL) 
+B 00 01
000000                249  Letext: 
+                     250  
+                     251  
+
+Symbol Table
+
+    KERNEL1_5 =000000 
+    KERNEL1_E =000000 
+    LIB_PRE_PRPCESS =000000 
+  1 Letext  000000 R
+  1 Ltext0  000000 R
+    R0 =0007D0 
+    R1 =0007D1 
+    R2 =0007D2 
+    R3 =0007D3 
+    R4 =0007D4 
+    R5 =0007D5 
+    R6 =0007D6 
+    R7 =0007D7 
+    Rb0 =0003E8 
+    Rb1 =0003E9 
+    Rb10 =0003F2 
+    Rb11 =0003F3 
+    Rb12 =0003F4 
+    Rb13 =0003F5 
+    Rb14 =0003F6 
+    Rb15 =0003F7 
+    Rb2 =0003EA 
+    Rb3 =0003EB 
+    Rb4 =0003EC 
+    Rb5 =0003ED 
+    Rb6 =0003EE 
+    Rb7 =0003EF 
+    Rb8 =0003F0 
+    Rb9 =0003F1 
+    SPIBOOT_KBDM9 v1.06  000001 
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _$USB_MassStorage_SD2_c$ size    0   flags 0
