;//! @file memory.a51,v
;//!
;//! Copyright (c) 2003-2004
;//!
;//! Use of this program is subject to Atmel's End User License Agreement.
;//! Please read file license.txt for copyright notice.
;//!
;//! @brief This file contains the API to copy a space memory with multi DPTR
;//!
;//! @version 1.3 snd3-dvk-1_9_5 $Id: memory.a51,v 1.3 2006/03/23 15:22:45 sguyon Exp $
;//!
;//! @todo
;//! @bug


$TITLE      (*** snd3-dvk-1_9_5 ***)

;//_____ I N C L U D E S ______________________________________________________

#define  ASM_INCLUDE
#include "config.h"

#if 0
;//_____ M A C R O S __________________________________________________________


;//_____ D E F I N I T I O N S ________________________________________________


;//_____ D E C L A R A T I O N S ______________________________________________

PUBLIC  _asm_code2xdata
PUBLIC  _asm_xdata2xdata
PUBLIC  _asm_invertxdata
PUBLIC  _asm_clearxdata
PUBLIC  _asm_data2xdata
PUBLIC  _asm_invert_data_xdata


MEMORY      SEGMENT  CODE
            RSEG     MEMORY
            
;//! @brief fonction copy code space to xdata space
;//!
;//! @param code source       DPTR  (MSB R6, LSB R7)
;//! @param xdata destination DPTR  (MSB R4, LSB R5)
;//! @param nomber of data to copy  (R3)
;//!
_asm_code2xdata:
          MOV     A,R3
          JNZ     start_code2xdata
          RET
start_code2xdata:
          MOV     R2,PPCON    ; save page
          MOV     PPCON,#01H  ; set  page 0 for AUXR1
          MOV     DPL,R7
          MOV     DPH,R6
          INC     AUXR1       ; select DPTR of dest
          MOV     DPL,R5
          MOV     DPH,R4
LOOP_CODE2XDATA:
          INC     AUXR1       ; select DPTR of source
          CLR     A           ; Init loop
          MOVC    A,@A+DPTR
          INC     DPTR
          INC     AUXR1       ; select DPTR of dest
          MOVX    @DPTR,A
          INC     DPTR
          DJNZ    R3,LOOP_CODE2XDATA
          MOV     PPCON,R2    ; restore page
          RET


;//! @brief fonction copy xdata space to xdata space
;//!
;//! @param xdata source      DPTR  (MSB R6, LSB R7)
;//! @param xdata destination DPTR  (MSB R4, LSB R5)
;//! @param nomber of data to copy  (R3)
;//!
_asm_xdata2xdata:
          MOV     A,R3
          JNZ     start_xdata2xdata
          RET
start_xdata2xdata:
          MOV     R2,PPCON    ; save page
          MOV     PPCON,#01H  ; set  page 0 for AUXR1
          MOV     DPL,R7
          MOV     DPH,R6
          INC     AUXR1       ; select DPTR of dest
          MOV     DPL,R5
          MOV     DPH,R4
LOOP_XDATA2XDATA:
          INC     AUXR1       ; select DPTR of source
          MOVX    A,@DPTR
          INC     DPTR
          INC     AUXR1       ; select DPTR of dest
          MOVX    @DPTR,A
          INC     DPTR
          DJNZ    R3,LOOP_XDATA2XDATA
          MOV     PPCON,R2    ; restore page
          RET


;//! @brief fonction copy xdata space to xdata space
;//!
;//! @param xdata source 1    DPTR  (MSB R6, LSB R7)
;//! @param xdata source 2    DPTR  (MSB R4, LSB R5)
;//! @param nomber of data to invert  (R3)
;//!
_asm_invertxdata:
          MOV     A,R3
          JNZ     start_invertxdata
          RET
start_invertxdata:
          MOV     R2,PPCON    ; save page
          MOV     PPCON,#01H  ; set  page 0 for AUXR1
                              ; record DPTR of source 2
          MOV     DPL,R5
          MOV     DPH,R4
          INC     AUXR1       ; select DPTR of source 1
          MOV     DPL,R7
          MOV     DPH,R6
LOOP_XDATA_INVERT:
          ; get value source 1
          MOVX    A,@DPTR
          MOV     R4,A
          ; get value source 2 and record value of source 1
          INC     AUXR1
          MOVX    A,@DPTR
          MOV     R5,A        ; get value of source 2
          MOV     A,R4        ; record value of source 1
          MOVX    @DPTR,A
          INC     DPTR        ; go to next position
          ; record value source 2 in source 1
          INC     AUXR1
          MOV     A,R5
          MOVX    @DPTR,A
          INC     DPTR
          DJNZ    R3,LOOP_XDATA_INVERT
          MOV     PPCON,R2    ; restore page
          RET


;//! @brief fonction copy xdata space to xdata space
;//!
;//! @param xdata source      DPTR  (MSB R6, LSB R7)
;//! @param nomber of data to clear (MSB R4, LSB R5)
;//!
_asm_clearxdata:
          MOV     DPL,R7
          MOV     DPH,R6
          CLR     A
LOOP_CLEARXDATA:
          CJNE    R5,#000H,DEC_R5
          CJNE    R4,#000H,DEC_R4R5
          RET
DEC_R4R5:
          DEC     R4
DEC_R5:
          DEC     R5
          MOVX    @DPTR,A
          INC     DPTR
          SJMP    LOOP_CLEARXDATA


;//! @brief fonction copy xdata space to xdata space
;//!
;//! @param data  source            (R7)
;//! @param xdata destination DPTR  (MSB R4, LSB R5)
;//! @param nomber of data to copy  (R3)
;//!
_asm_data2xdata:
          MOV     DPL,R5
          MOV     DPH,R4
          MOV     R0,07    ; get register R7 (AR7)
LOOP_DATA2XDATA:
          MOV     A,R3
          JZ      END_LOOP_DATA2XDATA
          MOV     A,@R0
          MOVX    @DPTR,A

          ; Update loop
          DEC     R3
          INC     R0
          INC     DPTR
          SJMP    LOOP_DATA2XDATA
END_LOOP_DATA2XDATA:
          RET     


;//! @brief fonction copy xdata space to xdata space
;//!
;//! @param  data source 1    (R7)
;//! @param xdata source 2    DPTR  (MSB R4, LSB R5)
;//! @param nomber of data to invert  (R3)
;//!
_asm_invert_data_xdata:
          MOV     DPL,R5
          MOV     DPH,R4
          MOV     R0,07    ; get register R7 (AR7)

LOOP_INVERT_DATA_XDATA:
          MOV     A,R3
          JZ      END_LOOP_INVERT_DATA_XDATA
          MOV     A,@R0
          MOV     R1,A
          MOVX    A,@DPTR
          MOV     @R0,A
          MOV     A,R1
          MOVX    @DPTR,A

          ; Update loop
          DEC     R3
          INC     R0
          INC     DPTR
          SJMP    LOOP_INVERT_DATA_XDATA
END_LOOP_INVERT_DATA_XDATA:
          RET   

END

#endif   //#if 0
