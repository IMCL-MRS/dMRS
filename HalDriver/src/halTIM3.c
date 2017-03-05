#include "halTIM3.h"
#include "halLed.h"

void halTM3Init(void){
  //1. RCC
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  //2. TM3
  TIM_TimeBaseInitTypeDef TIM_BaseInitStructure; 
  TIM_DeInit( TIM3);                               //��λTIM3��ʱ�� 

  TIM_BaseInitStructure.TIM_Period = 15000;        //TIM_Period��TIM1_ARR��=1000�����������ϼ�����1000����������¼�������ֵ���� 
  TIM_BaseInitStructure.TIM_Prescaler = 71;        //����Ԥ��Ƶ����Ƶϵ��71����APB2=72M, TIM1_CLK=72/72=1MHz 
  TIM_BaseInitStructure.TIM_ClockDivision = 0; 
  TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    //���ϼ���ģʽ
  TIM_BaseInitStructure.TIM_RepetitionCounter = 0; //TIM_RepetitionCounter(TIM1_RCR)=0��ÿ��������������������¼� 

  TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure); 
  TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
  TIM_ClearFlag(TIM1, TIM_FLAG_Update); 
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

__weak void TIM3_IRQ_UPDATE_ISR(void) {  
  asm ("NOP");
}

