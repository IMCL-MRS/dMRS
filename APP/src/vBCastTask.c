
#include "vBCastTask.h"
#include "mrslib.h"

extern volatile uint16_t rbID;
extern volatile int32_t MAG_SENSOR_X;
extern volatile int32_t MAG_SENSOR_Y;

extern xQueueHandle xQueueHandleRFTx;

void vBCastTask( void *pvParameters ){
  while (1) {
    uint8_t tx[32];
    typeCoordinate pos;
    
    portTickType xLastWakeTime; 
    xLastWakeTime = xTaskGetTickCount(); 
    
    type_RFPacket* p = (type_RFPacket*)tx;
    pos = GetCoordinate();
    p->id = 1;
    //p->locationX = pos.x;
    //p->locationY = pos.y;
    p->locationX = MAG_SENSOR_X;
    p->locationY = MAG_SENSOR_Y;
    p->dir    = 90;//CalibrateNorth2X();
    p->speedL = GetRobotSpeedLeft();
    p->speedR = GetRobotSpeedRight();
    p->crc16Res = CRC16(tx, sizeof(type_RFPacket)-2);
    
    while (xQueueSendToBack(xQueueHandleRFTx, tx, portMAX_DELAY)!=pdPASS)  {
      vTaskDelay(5);
    }
    
    halSetLedStatus(LED_RED, LED_TOGGLE);       
    vTaskDelayUntil(&xLastWakeTime, 200);  
  }
}