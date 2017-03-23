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
#include "hal24LC02B.h"
#include "halBeep.h"
#include "vInfoList.h"
#include <string.h>

volatile uint16_t rbID;
volatile int32_t magX, magY;
//get magX and magY
typeMagSensor data[100], nowdata;
int32_t minX, maxX, minY, maxY;

static uint8_t magXbytes[4];
static uint8_t magYBytes[4];
static uint8_t IDBytes[2];
int32_t magXTmp[2];
int32_t magYTmp[2];
//void vMagTask( void *pvParameters ) {
void magSenPara(){
#ifdef STORE_ID
  rbID = ROBOT_ID;
  memcpy(IDBytes,(uint8_t *)&rbID,sizeof(rbID));
  for(i = 0; i < 2; i++){
    hal24LC02BByteWrite(MAG_ADDR_ID+i,IDBytes[i]); 
  }  
#else  
  magX = magY = 0;
  minX = minY = 100000;
  maxX = maxY = -100000;
  vTaskDelay(200);
  RobotRotate(15, 5);
  int datan;
  int i = 0;
  for (i = 0; i < 2; i++){
    for (datan = 0; datan < 48; ++ datan) {
      nowdata = ReadMagSensor();
      if (minX > nowdata.magX) minX = nowdata.magX;
      if (minY > nowdata.magY) minY = nowdata.magY;
      if (maxX < nowdata.magX) maxX = nowdata.magX;
      if (maxY < nowdata.magY) maxY = nowdata.magY;
      data[datan] = nowdata;
      RobotRotate(15, 5);
      halt(5);
    }
    magXTmp[i] = (int32_t)((minX + maxX) / 2);
    magYTmp[i] = (int32_t)((minY + maxY) / 2);  
  }
  magX = (int32_t)((magXTmp[0] + magXTmp[1]) / 2);
  magY = (int32_t)((magYTmp[0] + magYTmp[1]) / 2);
  //store magX, magY in EP2ROM
  memcpy(magXbytes,(uint8_t *)&magX,sizeof(magX));
  memcpy(magYBytes,(uint8_t *)&magX,sizeof(magY));
  for(i = 0; i < 4; i++){
     hal24LC02BByteWrite(MAG_ADDR_X+i,magXbytes[i]); 
     hal24LC02BByteWrite(MAG_ADDR_Y+i,magYBytes[i]);     
  }
#endif
  beepSignal();
  asm("NOP");
}


extern xQueueHandle xQueueHandleRFTx;
void vBCastMagTask( void *pvParameters ){
  while (1) {
    uint8_t tx[32];
    
    portTickType xLastWakeTime; 
    xLastWakeTime = xTaskGetTickCount(); 
    
    type_RFPacket* p = (type_RFPacket*)tx;
       
    p->id = rbID;
    p->speedL = magX;
    p->speedR = magY;
    
    while (xQueueSendToBack(xQueueHandleRFTx, tx, portMAX_DELAY)!=pdPASS)  {
      vTaskDelay(5);
    }
    
    halSetLedStatus(LED_RED, LED_TOGGLE);       
    vTaskDelayUntil(&xLastWakeTime, 200);  
  }
}