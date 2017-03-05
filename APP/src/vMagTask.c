/**
 * @file vMagTask.c
 * @brief Mag sensor parameters calibration.
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include "vMagTask.h"
#include "mrslib.h"
#include "readMPUSensor.h"
#include "vBCastTask.h"

volatile int32_t magX, magY;

//get magX and magY
typeMagSensor data[100], nowdata;
int32_t minX, maxX, minY, maxY;

int datan;
void vMagTask( void *pvParameters ) {
  magX = magY = 0;
  minX = minY = 100000;
  maxX = maxY = -100000;
  vTaskDelay(500);
  RobotRotate(30, 5);
  for (datan = 0; datan < 48; ++ datan) {
    nowdata = ReadMagSensor();
    if (minX > nowdata.magX) minX = nowdata.magX;
    if (minY > nowdata.magY) minY = nowdata.magY;
    if (maxX < nowdata.magX) maxX = nowdata.magX;
    if (maxY < nowdata.magY) maxY = nowdata.magY;
    data[datan] = nowdata;
    RobotRotate(30, 5);
    halt(5);
  }
  magX = (minX + maxX) / 2;
  magY = (minY + maxY) / 2;
  vTaskDelay(100000);
  asm("NOP");
}


extern xQueueHandle xQueueHandleRFTx;

void vBCastMagTask( void *pvParameters ){
  while (1) {
    uint8_t tx[32];
    
    portTickType xLastWakeTime; 
    xLastWakeTime = xTaskGetTickCount(); 
    
    rbNode* p = (rbNode*)tx;
    
//    coordinateK = GetCoordinate();
//    bcastInfo.nodeID = rbID; 
//    bcastInfo.rpos.locationX = magX;
//    bcastInfo.rpos.locationY = magY;
//    bcastInfo.angle2n     = ReadMagSensorAngle2North();
    p->crc16Res = CRC16(tx, sizeof(rbNode)-2);
    
    while (xQueueSendToBack(xQueueHandleRFTx, tx, portMAX_DELAY)!=pdPASS)  {
      vTaskDelay(5);
    }
    
    halSetLedStatus(LED_RED, LED_TOGGLE);       
    vTaskDelayUntil(&xLastWakeTime, 200);  
  }
}