
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
  while(1){  
    SetRobotSpeed(0,0);
    vTaskDelay(200);  
    portTickType lastTimeReport = xTaskGetTickCount();
    while((xTaskGetTickCount() - lastTimeReport) < 500){
      if (bt3Pressed()==1) {
        magSenPara();  //Ð£×¼²¢Ð´EPP2ROM
        break;
      }
    }    
    romReadTest();   //mag parameters should re-calibrate  
    beepSignal();
    halt(5000);     //stop
  }
}