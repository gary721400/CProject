+                       1 .area _ADC_MAIN_DATA_(ADC_MAIN_DATA)
+                       2 .even
+B 00 01
000000                  3 ADC_BusyFlag:
000000                  4 	.ds 2
000002                  5 ADC_last_channel:
000002                  6 	.ds 2
+                       7 	
+                       8 .area _ADC_MAIN_CODE_(ADC_MAIN_CODE)
+B 00 02
000000                  9 _ADC_init::
000000  58 F0          10 	PUSH R0
000002  48 00          11 	MOV	R0,#0
000004  AA F0 00 00    12 	MOVX ADC_BusyFlag,R0
000008  68 F0 FF FF    13 	MOV	R0,#0xFFFF
00000C  AA F0 00 02    14 	MOVX ADC_last_channel,R0
000010  38 0F          15 	POP R0
000012  C6 20          16 	RETS
+                      17 	
000014                 18 _Get_ADCValue::
000014  2E 01          19 	LINK #1
000016  58 F1          20 	PUSH R1
+                      21 		
000018  BA F0 00 00    22     MOVX R0,ADC_BusyFlag
00001C  56 00          23     CMP R0,#0
00001E  AC 02 00 5C    24     AJMP IF_NE,$IsBusy
000022  68 F0 FF FF    25     MOV	R0,#(-1)
000026  AA F0 00 00    26     MOVX ADC_BusyFlag,R0
+                      27 
00002A  78 0B 00 06    28 	MOV R0,[FP+#(6)]		;Cnt = Chx
00002E  54 07          29 	AND R0,#7
000030  4C 03          30 	SL R0,#3
000032  75 F0 80 00    31 	OR R0,#0x8000
000036  BA B8 F2 E0    32 	MOVX ADCCON,R0
+                      33 
00003A  48 00          34 	MOV R0,#0
+                      35 $LOOP:
00003C  E0 B8          36 	BSET ADCCON.0
+                      37 $WAIT_KICKOFF:
00003E  9A B8 00 FE    38 	JB	ADCCON.0, $WAIT_KICKOFF			
000042  4F 0A          39 	CMPI1 R0,#10
000044  AC 05 00 3C    40 	AJMP IF_UL,$LOOP
+                      41 
000048  48 00          42 	MOV R0,#0
+                      43 $LOOP2:
00004A  4F 0A          44 	CMPI1 R0,#10
00004C  AC 05 00 4A    45 	AJMP IF_UL,$LOOP2
+                      46 
000050  BA F0 F2 72    47 	MOVX R0,ADCD
000054  4D 06          48 	SR R0,#6
000056  48 10          49     MOV	R1,#0
000058  AA F1 00 00    50     MOVX ADC_BusyFlag,R1
+                      51 $IsBusy:
00005C  38 1F          52 	POP R1
00005E  2F 00          53 	UNLINK	
000060  C6 20          54 	RETS	
+
+Symbol Table
+
+  1 ADC_BusyFlag  000000 R
+  1 ADC_last_channel  000002 R
+    SPIBOOT_KBDM9 v1.06  000001 
+  2 _ADC_init  000000 GR
+  2 _Get_ADCValue  000014 GR
+
+
+Area Table
+
+   0 . size    0   flags 0
+   1 _ADC_MAIN_DATA_ size    4   flags 0
+   2 _ADC_MAIN_CODE_ size   62   flags 0
