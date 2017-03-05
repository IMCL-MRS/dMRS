#ifndef __RADIOTASK_H
#define __RADIOTASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "halLed.h"
#include "halSPI.h"
#include "halnRF24L01P.h"

#include "systemConfig.h"
#include "bottomBoard.h"
#include "CRC16.h"

extern void vRadioRxTask( void *pvParameters );
extern void vRadioTxTask( void *pvParameters );


#endif

