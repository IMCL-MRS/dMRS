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

#endif

