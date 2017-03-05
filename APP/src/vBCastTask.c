
#include "vBCastTask.h"


extern xQueueHandle xQueueHandleRFTx;

void vBCastTask( void *pvParameters ){
  while (1) {
    uint8_t tx[32];
    
    portTickType xLastWakeTime; 
    xLastWakeTime = xTaskGetTickCount(); 
    
    type_RFPacket* p = (type_RFPacket*)tx;
    
    p->id = 1;
    p->speedL = GetRobotSpeedLeft();
    p->speedR = GetRobotSpeedRight();
    p->dir    = 0;
    p->locationX = 0;
    p->locationY = 0;
    p->crc16Res = CRC16(tx, sizeof(type_RFPacket)-2);
    
    while (xQueueSendToBack(xQueueHandleRFTx, tx, portMAX_DELAY)!=pdPASS)  {
      vTaskDelay(5);
    }
    
    halSetLedStatus(LED_RED, LED_TOGGLE);
    
    
    vTaskDelayUntil(&xLastWakeTime, 200);  
  }
}