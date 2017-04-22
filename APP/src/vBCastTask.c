
#include "vBCastTask.h"
#include "mrslib.h"

extern volatile uint16_t rbID;
extern volatile int32_t MAG_SENSOR_X;
extern volatile int32_t MAG_SENSOR_Y;
extern xQueueHandle xQueueHandleRFTx;

int16_t _gAngle2X = 0;

void vBCastTask( void *pvParameters ){
  while (1) {
    uint8_t tx[32];
    typeCoordinate pos;
    
    portTickType xLastWakeTime; 
    xLastWakeTime = xTaskGetTickCount(); 
    
    type_RFPacket* p = (type_RFPacket*)tx;
    _gAngle2X = CalibrateNorth2X();
    pos = GetCoordinate();
    p->id = rbID;
    p->locationX = pos.x;
    p->locationY = pos.y;
    //p->locationX = MAG_SENSOR_X;
    //p->locationY = MAG_SENSOR_Y;
    p->dir    =  _gAngle2X;//ReadMagSensorAngle2North();
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