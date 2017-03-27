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

volatile uint16_t rbID;
volatile int32_t MAG_SENSOR_X;
volatile int32_t MAG_SENSOR_Y;

static typeMagSensor magSensor;
typeMagSensor ReadMagSensor() {  
  while(halMPU9250RdCompassX(&magSensor.magX) == 0){ //cmopass data update every 10ms
    vTaskDelay(5);
  }  
  while(halMPU9250RdCompassX(&magSensor.magY) == 0){ 
    vTaskDelay(5);
  }  
  while(halMPU9250RdCompassX(&magSensor.magZ) == 0){ 
    vTaskDelay(5);
  }

  return magSensor;
}

int16_t magParaInit(){
  u8 i = 0;
  for(i=0; i<2; i++){
    while(hal24LC02BRandomRead(MAG_ADDR_ID+i*2, (uint8_t *)(&rbID)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_ID+i*2+1, (uint8_t *)(&rbID)+i*2+1) == false);
  }
  
  for (i=0;i<4;i++) { 
    while(hal24LC02BRandomRead(MAG_ADDR_X+i*2, (uint8_t *)(&MAG_SENSOR_X)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_X+i*2+1, (uint8_t *)(&MAG_SENSOR_X)+i*2+1) == false);
    
    while(hal24LC02BRandomRead(MAG_ADDR_Y+i*2, (uint8_t *)(&MAG_SENSOR_Y)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_Y+i*2+1, (uint8_t *)(&MAG_SENSOR_Y)+i*2+1) == false);
  }
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
