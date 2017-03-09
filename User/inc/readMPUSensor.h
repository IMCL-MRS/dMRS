#ifndef __READMPUSENSOR_H
#define __READMPUSENSOR_H
#include "stdint.h"

typedef struct typeMagSensor{
  int16_t magX;
  int16_t magY;
  int16_t magZ;
  uint32_t   timeStamp;
}typeMagSensor;

extern typeMagSensor ReadMagSensor(void);
extern int16_t ReadMagSensorAngle2North(void);
extern int16_t CalibrateNorth2X(void);
#endif

