#ifndef __BCASTTASK_H
#define __BCASTTASK_H

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "systemConfig.h"
#include "bottomBoard.h"
#include "CRC16.h"
#include "halLed.h"

#pragma pack(push)
#pragma pack(1)
typedef struct type_RFPacket{
  uint16_t id;          //2bytes
  int32_t  locationX;    //4
  int32_t  locationY;    //4
  int16_t  dir;          //2
  int16_t  speedL;     //2
  int16_t  speedR;     //2
  uint16_t crc16Res;   //2
}type_RFPacket;
#pragma pack(pop)


extern void vBCastTask( void *pvParameters );

#endif

