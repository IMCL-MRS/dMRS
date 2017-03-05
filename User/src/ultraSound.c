#include "ultraSound.h"
#include "halLed.h"

static uint16_t timeTravel[4] = {0,0,0,0};         //接收到时间差判断距离
uint8_t  activeBeacon = 2;

////////////////////////////////////////////////////////////////////////////////
//C=331.45+0.61T
//Unit  mm
////////////////////////////////////////////////////////////////////////////////
uint32_t GetDistance(uint8_t bc) {
  return (uint32_t)(timeTravel[bc-1]*0.340);         //340 x 1000 mm,  1s->1000 000us
}

////////////////////////////////////////////////////////////////////////////////
//ULTRA SOUND INT ISR
////////////////////////////////////////////////////////////////////////////////
void EXIT9ISR(void){
  uint16_t beaconTimeRecStop=0, timeDiff=0;
  TIM_Cmd(TIM3, DISABLE);
  beaconTimeRecStop = TIM_GetCounter(TIM3);
  TIM_SetCounter(TIM3, 0);
  timeDiff = beaconTimeRecStop;
  
//  if (activeBeacon==0) {
//    return;
//  } 
  if (activeBeacon>4) {
    return;
  }

  timeTravel[activeBeacon-1] = timeDiff;
  if (timeDiff==0){
    asm ("NOP");
  }
  activeBeacon = 0;
  
  halSetLedStatus(LED_YELLOW, LED_TOGGLE);
}

void TIM3_IRQ_UPDATE_ISR(void) {  
  TIM_Cmd(TIM3, DISABLE);
  TIM_SetCounter(TIM3, 0);
  activeBeacon = 0;
}

