
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
  SetRobotSpeed(0,0);
  vTaskDelay(200);  
  portTickType lastTimeReport = xTaskGetTickCount();
  while((xTaskGetTickCount() - lastTimeReport) < 500){
    if (bt3Pressed()==1) {
      magSenPara();  //校准并写EPP2ROM,写的是对的，下面读出来就是错的。
      break;
    }
  }    
  romReadTest();  //mag parameters should re-calibrate  
  halt(5000);     //stop
}