#ifndef __DEMOTASK_H
#define __DEMOTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "halLed.h"

extern void vDemoTask( void *pvParameters );
extern void MICROPHONETask( void *pvParameters );
extern void COMPASS_Task( void *pvParameters );
extern void GYRO_Task( void *pvParameters );
extern void ACCEL_Task( void *pvParameters );
extern void EEPROM_Task( void *pvParameters );
extern void LIGHTSENSORTask( void *pvParameters );
extern void BUTTON_LED_DEMOTask( void *pvParameters );
extern void BEEP_DEMOTask( void *pvParameters );

#endif