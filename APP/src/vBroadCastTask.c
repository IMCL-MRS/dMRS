#include "vBroadCastTask.h"
#include "mrslib.h"
#include "vInfoList.h"
#include "halMPU9250.h"
#include <math.h>

extern xQueueHandle xQueueHandleRFTx;

static Magnetic __mag, __pmag;
static float __ang2n;
static float __ang2x;
static int8_t __mag_busy;
static Coordinate __cor;
static uint8_t tx[32];

void vBroadCastTask( void *pvParameters ){
  for(;;) {
    // Magnetic
    __pmag = __mag;
    __mag_busy = 1;
    while(halMPU9250RdCompassX(& (__mag.x) ) == 0){
      vTaskDelay(5);
    }  
    while(halMPU9250RdCompassY(& (__mag.y) ) == 0){ 
      vTaskDelay(5);
    }
    float compX = (float)(__mag.x-MAG_SENSOR_X);
    float compY = (float)(__mag.y-MAG_SENSOR_Y);
    float length = sqrt(compX*compX + compY*compY);
    __ang2n = (acos( compY / length ) * 180 / PI);
    if (compX > 0) {
      __ang2n = 360 - __ang2n;
    }
    __ang2x = __ang2n + 200;
    if (__ang2x >= 360) {
      __ang2x -= 360;
    }
    __mag_busy = 0;
    
    /*
    // localization
    uint32_t dis2b1 = GetDistance(1);
    uint32_t dis2b2 = GetDistance(2);
    float dis2b1g = sqrt( dis2b1*dis2b1 - DISTANCE_B_2_GROUND*DISTANCE_B_2_GROUND );
    float dis2b2g = sqrt( dis2b2*dis2b2 - DISTANCE_B_2_GROUND*DISTANCE_B_2_GROUND );


    float P = (dis2b1g + dis2b2g + DISTANCE_B1_2_B2) / 2;
    float S = sqrt( P * (P-dis2b1g) * (P-dis2b2g) * (P-DISTANCE_B1_2_B2) );
    __cor.x =  (2*S/DISTANCE_B1_2_B2);
    float b1y = (sqrt( dis2b1g*dis2b1g - __cor.x*__cor.x ) );
    float b2y = (sqrt( dis2b2g*dis2b2g - __cor.x*__cor.x ) );
    __cor.y = b1y;
    if (fabs(b2y - b1y - DISTANCE_B1_2_B2) < 1) {
      __cor.y = -b1y;
    }
    
    // broadcast
    Packet* p = (Packet*)tx;
    p->ang2x = __ang2x;
    p->cor = __cor;
    p->id = ROBOT_ID;
    p->mag = __mag;
    p->status = 0;
    p->crc16Res = CRC16(tx, sizeof(Packet)-2);
    while (xQueueSendToBack(xQueueHandleRFTx, tx, portMAX_DELAY)!=pdPASS)  {
      vTaskDelay(5);
    }
    */
    
    // light
    halSetLedStatus(LED_RED, LED_TOGGLE);
    vTaskDelay(100);
  }
}

Magnetic getMagnetic() {
  if (__mag_busy) {
    return __pmag;
  }
  return __mag;
}

float getAngle2North() {
  return __ang2n;
}

//0 - 360
float getAngle2X(){
  return __ang2x;
}

Coordinate getCoordinate() {
  return __cor;
}