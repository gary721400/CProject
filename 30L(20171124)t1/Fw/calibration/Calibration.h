#ifndef _Calibration_H
#define _Calibration_H

//IR Control Function

extern __X unsigned int CalibrationData[ 256 ];

extern unsigned int Calib_IR(void);
extern void Get_Calibration_Data(__X unsigned int*);    //Return 1 pass, Return 0 fail
/*
typedef enum
{
   CALIB_IDLE = 0,
   CALIB_START = 1,
   CALIB_WAIT = 2,
   CALIB_OK = 3,
   CALIB_KO = 4,
   CALIB_RD_START = 5,
   CALIB_RD = 6,
   CALIB_RD1 = 7,
   CALIB_RESTART = 8,
   CALIB_RESTART_ACK = 9,
}CalibStatus;
*/
typedef enum
{
   CALIB_IDLE = 0,
   CALIB_START = 1,
   CALIB_WAIT = 2,
   CALIB_OK = 3,
   CALIB_KO = 4,
   CALIB_RD_START = 5,
   CALIB_RD = 6,
   CALIB_RD1 = 7,
   CALIB_RD2 = 10,
   CALIB_RESTART = 8,
   CALIB_RESTART_ACK = 9,
}CalibStatus;
extern unsigned int calib_state;

#endif