#include "KBDM9_DDK.H"
#include "SDTL_internal.h"

#pragma CODE = SDTL_CODE
#pragma DATA = SDTL_XRAM

SDTL_ERR sdtl_mount(void)
{
int result;
  	
 	SetFlashBusy();

// 2010/03/17 modified, ������SD card ����busy��״̬��, ������mount��ʹ data����	
 __asm{
 	
   OR PDEN , #0x000F          ; D0 ~ D3
   OR PDPH , #0X000F
   OR PDDS, #0x000F  	
 	
 	BSET PCPH.0
 	BSET PCEN.0               ; SD1_CMD
  BSET PCDS.0
 	 
 	BSET PCEN.4 
	BCLR PCPH.4	
	BSET PCDS.4								;  SD_CLK	
		 	 
   BSET SDCCR.15
   BSET SDCKCR.8
   NOP
 $44:
 	 NOP
   JB SDCCR.7, $44	          ; check busy status
   NOP
   BCLR SDCKCR.8
   BCLR SDCCR.15
 }
 	
 	result = SD1_Init();  	
  	
 	if(result !=0){
 		_is_sd_mounted = 0;    
   	SD1_Insert = 0;
     ClearFlashBusy();     	
   	return(result);
 	}
   	
	SD1_read_block(0, SD_Catch, 1);				//MBR sector	
  
  if(	SD_Catch[0]=='K' &&			//���MBR�Ƿ��о���sdtl_allocate���ı��
	  	SD_Catch[1]=='B' &&
		  SD_Catch[2]=='S' && (SD_Catch[510]==0x55) && (SD_Catch[511]==0xAA)){
		SDTL_FAT_StartSector = SDTL_ConvertDWORD( &SD_Catch[0x01C6]);
		//�������Ĵ�С=FAT��ʼ��sector�۵�boot��128(64kbytes)��MBR(sector0)��sector1 
		SDTL_resv_TotalSector = SDTL_FAT_StartSector - 130UL; 
		SDTL_FAT_TotalSector = SDTL_ConvertDWORD( &SD_Catch[0x01CA]);		  	
	}	     /*��� sector 0 �ǲ���DOS boot sector(��һ��byte����E9Ҳ����EB), ��Ӧ����partition table */	 														
	else if((SD_Catch[0]!= 0xE9) && (SD_Catch[0]!= 0xEB)&&(SD_Catch[510]==0x55)&&(SD_Catch[511]==0xAA))		
	{
		SDTL_FAT_StartSector = SDTL_ConvertDWORD( &SD_Catch[0x01C6]);
		//�������Ĵ�С=FAT��ʼ��sector�۵�MBR(sector0) 
		SDTL_resv_TotalSector = SDTL_FAT_StartSector - 1UL; 
		SDTL_FAT_TotalSector = SDTL_ConvertDWORD( &SD_Catch[0x01CA]);
	}else{
  		SDTL_FAT_StartSector = 0;
  		SDTL_resv_TotalSector = 0;
  		SDTL_FAT_TotalSector = SD1_Total_Block;
  }       
  		
  _is_sd_mounted = 1;
 
  SD1_Insert = 1;
  
  //__asm{ MOV SDCKCR, #0x0001} // 2009/10/30 set SD clock to 24MHz
  __asm{ MOV SDCKCR, #0x0003} // 2009/10/30 set SD clock to 12MHz
  //result = SD1_fast_mode();
  ClearFlashBusy();
  return(SDTL_No_Err);   	
}
