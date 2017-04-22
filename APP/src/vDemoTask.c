
#include "apps.h"
#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "bottomBoard.h"
#include "ultraSound.h"
#include "mrslib.h"
#include "vTestCase.h"
#include "FreeRTOS.h"
#include "halBeep.h"
#include <task.h>

const typeCoordinate posA1 = {1410,50};
const typeCoordinate posA2 = {1410,1400};
const typeCoordinate posA3 = {2800,1400};
const typeCoordinate posA4 = {2800,50};

void vDemoTask( void *pvParameters ){  
  //magParaInit();   /*read mag parameters from EPPROM */
  SetRobotSpeed(0,0);
  vTaskDelay(200);  
  portTickType lastTimeReport = xTaskGetTickCount();
  while((xTaskGetTickCount() - lastTimeReport) < 500){
    if (bt3Pressed()==1) {
      magSenPara();  //Ð£×¼²¢Ð´EPPROM
      break;
    }
  } 
   // rotateTest();
  RotateToTest();
  //go2PointTest();
  //beepSignal();
  while(1){
    vTaskDelay(20);
  }
}