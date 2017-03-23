#include "vInfoList.h"
#include "halMPU9250.h"
#include "hal24LC02B.h"
#include "halBeep.h"
#include "mrslib.h"
#include <FreeRTOS.h>
#include <task.h>
#include <readMPUSensor.h>
#include <intMath.h>
#include <math.h>
#include <string.h>

extern volatile uint16_t rbID;
volatile int32_t MAG_SENSOR_X;
volatile int32_t MAG_SENSOR_Y;

static typeMagSensor magSensor;
typeMagSensor ReadMagSensorStatus(u8 st[]) {
  st[0] = halMPU9250RdCompassX(&magSensor.magX); //cmopass data update every 10ms
  st[1] = halMPU9250RdCompassY(&magSensor.magY);
  st[2] = halMPU9250RdCompassZ(&magSensor.magZ);
  return magSensor;
}

typeMagSensor ReadMagSensor(void) {
  u8 st[3] = {0};
  magSensor = ReadMagSensorStatus(st);
  while(!(st[0] && st[1] && st[2])){
    vTaskDelay(5);
    magSensor = ReadMagSensorStatus(st);
  }
  return magSensor;
}

int16_t magParaInit(){
  u8 i = 0;
  uint8_t outDataID[2] = {0};
  uint8_t outDataX[4] = {0};
  uint8_t outDataY[4] = {0};
  for(i=0; i<2; i++){
    hal24LC02BRandomRead(MAG_ADDR_ID+i, outDataID+i);
  }
  
  for (i=0;i<4;i++) { 
    hal24LC02BRandomRead(MAG_ADDR_X+i, outDataX+i);
    hal24LC02BRandomRead(MAG_ADDR_Y+i, outDataY+i);
  }
  memcpy((uint8_t *)&rbID, &outDataID, sizeof(outDataID));
  memcpy((uint8_t *)&MAG_SENSOR_X,&outDataX,sizeof(outDataX));
  memcpy((uint8_t *)&MAG_SENSOR_Y,&outDataY,sizeof(outDataY));
  return 0;
}

//angle -180 to 180
int16_t ReadMagSensorAngle2North(void) {
  typeMagSensor magSensor;
  static float edgeLong=0,compX=0, compY=0;
  static float angleReturn;
  
  magSensor =  ReadMagSensor();
  compX = (float)(magSensor.magX-MAG_SENSOR_X);
  compY = (float)(magSensor.magY-MAG_SENSOR_Y);
  
  edgeLong= sqrt(compX*compX + compY*compY);     
  angleReturn = ((180/__PI)*acos(compY/edgeLong));
  
  if (compX < 0){    
    angleReturn=0-angleReturn;
  }   

  return (int16_t)(angleReturn);
}

//0 - 360
int16_t CalibrateNorth2X(void){
  int16_t nAngle = ReadMagSensorAngle2North();
  if((nAngle >= -180) && (nAngle < -160)){
    nAngle = -nAngle - 160;
  }else{
    nAngle = -nAngle + 200;
  }
  return nAngle;
}
