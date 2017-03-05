
#include <readMPUSensor.h>
#include "halMPU9250.h"

static typeMagSensor magSensor;
typeMagSensor ReadMagSensor(void) {
  halMPU9250RdCompassX(&magSensor.magX);
  halMPU9250RdCompassY(&magSensor.magY);
  halMPU9250RdCompassZ(&magSensor.magZ);
  return magSensor;
}