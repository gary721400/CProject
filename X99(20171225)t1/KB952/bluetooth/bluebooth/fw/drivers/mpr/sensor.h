#ifndef __SENSOR_H
#define __SENSOR_H

/* sensor����У������ֵ */
#define BRIGHTNESS_MIN  140
#define BRIGHTNESS_MAX  170
#define MAX_EXPOSE      0x500
#define MIN_EXPOSE      0x40

/* sensorУ������ */
typedef struct
{
    mrmWord x;
    mrmWord y;
    mrmWord expose;
}SensorPara_t;

/* sensor����ֵ */
#define BRIGHTNESS_0    0
#define BRIGHTNESS_1    1
#define BRIGHTNESS_2    2
#define BRIGHTNESS_3    3
#define BRIGHTNESS_4    4
#define BRIGHTNESS_5    5
#define BRIGHTNESS_6    6
#define BRIGHTNESS      BRIGHTNESS_4

/* sensor����Ƶ�� */
#define SENSOR_24MHZ    0
#define SENSOR_12MHZ    1
#define SENSOR_6MHZ     2
#define SENSOR_FREQ     SENSOR_24MHZ

mrmBool  AdjustExpose   ( void );
mrmBool  WinInit        ( void );
mrmBool  GetSensorPara  ( mrmWord *pStartX, mrmWord *pStartY, mrmWord *pExpose );
mrmBool  SetSensorPara  ( mrmWord startX, mrmWord startY, mrmWord expose );

#endif
