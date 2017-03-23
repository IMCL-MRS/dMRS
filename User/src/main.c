#include "halLed.h"
#include "halBeep.h"
#include "halADC12.h"
#include "halSPI.h"
#include "halMPU9250.h"
#include "halButton.h"
#include "halI2C.h"
#include "hal24LC02B.h"
#include "halTIM3.h"
#include "halUltraSound.h"
#include "halInfrared.h"
#include "halnRF24L01P.h"
#include "halI2S.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "bottomBoard.h"
#include "apps.h"

xQueueHandle xQueueHandleRFTx, xQueueHandleRFRx; //Queue of Send and Receive.
xSemaphoreHandle xBSB1,xBSB2;

int main(void) {  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //16 preemption priority, 0 sub-priority
  SystemInit();
  halMCUWaitMS(100);
  halLedInit();
  halSPI1Init();
  halBeepInit();
  
  halADC12Init();
  halButtonInit();
  halI2CInit();
  hal24LC02BInit();
  halI2SInit();
  
  halMPU9250Init();  
  SetRobotSpeed(0,0);
  
  //º”‘ÿ≤‚æ‡«˝∂Ø
  halTM3Init();
  halUltraSoundInit();
  halnRF24L01PInit();            
  
  //used by button
  vSemaphoreCreateBinary(xBSB1);
  vSemaphoreCreateBinary(xBSB2);
  
  //read mag parameters from EPPROM
  magParaInit();
  
  //used by radio
  xQueueHandleRFTx = xQueueCreate(10,  32);   //uxQueueLength, uxItemSize
  xQueueHandleRFRx = xQueueCreate(10,  32);  
  
  //KEEP THESE TASKS FOR LOCALIZATION£¨RF COMMUNICATION AND BASIC DEMO 
  xTaskCreate( vRadioTxTask, ( signed portCHAR * ) "RADIO TX", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );    
  xTaskCreate( vRadioRxTask, ( signed portCHAR * ) "RADIO RX", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );  
  xTaskCreate( vBCastTask,   ( signed portCHAR * ) "BCAST",    configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );    
  
  xTaskCreate( vDemoTask,    ( signed portCHAR * ) "Demo",      configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+3, NULL );      
  
  vTaskStartScheduler();
  return 0; 
}


