
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


void vDemoTask( void *pvParameters ){  
  //magParaInit();   /*read mag parameters from EPPROM */
  SetRobotSpeed(0,0);
  vTaskDelay(200);  
  portTickType lastTimeReport = xTaskGetTickCount();
  while((xTaskGetTickCount() - lastTimeReport) < 500){
    if (bt3Pressed()==1) {
      magSenPara();  //У׼��дEPPROM
      break;
    }
  } 
  go2PointTest();
  beepSignal();
  halt(5000);     //stop
}