#include "KBDM9_DDK.h"
#include <SFR.h>
#include <string.h>
//#include "cis.h"
#include "Calibration.h"
#include "config.h"


#pragma CODE = Test_Code
#pragma DATA = Test_Data

__X unsigned int RectifyStatus = 0;
__X unsigned int CalibrationData[ 256 ];

/*****************************************************************************/
void Nib_Rectify( void )
{
#if 0
   unsigned long SectorAddress, DataLength;

	RectifyStatus = 0;
   if( Calib_IR())
   {
      GetDataIdInfo( D_NIBDATA, &SectorAddress, &DataLength );
      Get_Calibration_Data( CalibrationData );
      CalibrationData[( DataLength >> 1 )- 1 ] = 0x55AA;
#if 0//def SDBOOT_KBDM9
      sdtl_resv_write( SectorAddress, CalibrationData );
#endif
#if 1//def SPIBOOT_KBDM9
      spif_resv_write( SectorAddress, CalibrationData );
#endif
      RectifyStatus = 0x55;
   }
   else
   {
      RectifyStatus = 0xFF;
   }
#else
   Calib_IR();
#endif
}


