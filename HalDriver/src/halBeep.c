#include "halBeep.h"

////////////////////////////////////////////////////////////////////////////////
//BEEP->PE14ȫӳ��->TIM1_CH4 
////////////////////////////////////////////////////////////////////////////////
void halBeepInit(void){
  //1. RCC
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,  ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
  //2. GPIO  
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  //3. TIM1
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_DeInit(TIM1);
  TIM_InternalClockConfig(TIM1);
  TIM_TimeBaseStructure.TIM_Period=((uint32_t)72000000/2000);                         //ARR��ֵ����10K, �Զ���װ�ؼĴ�������ֵ
  TIM_TimeBaseStructure.TIM_Prescaler=0;                      //TIM1ʱ��Ƶ�ʵ�Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;             //ʱ�ӷָ�ֵ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  TIM_OCInitTypeDef TIMOCInitStructure;
  TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;             //PWMģʽ1���
  
  TIMOCInitStructure.TIM_Pulse = ((uint32_t)72000000/8000) ;   //ռ�ձ�=(CCRx/ARR)*100%
  TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //TIM����Ƚϼ��Ե���Ч
  TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�����״̬ѡ������Ƚ�״̬
  TIMOCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;
  
  TIM_OC4Init(TIM1, &TIMOCInitStructure);                      //TIM1��CH4���
  
  TIM_CtrlPWMOutputs(TIM1,ENABLE);                             //����TIM1��PWM���Ϊʹ��
  TIM_SetCounter(TIM1, 0);
  TIM_Cmd(TIM1, DISABLE);
}

void halBeepOn(uint32_t freq){
  TIM_CtrlPWMOutputs(TIM1,DISABLE);                            //����TIM1��PWM���Ϊʹ��
  TIM_Cmd(TIM1, DISABLE);
  TIM_SetCounter(TIM1, 0);
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_DeInit(TIM1);
  TIM_InternalClockConfig(TIM1);
  TIM_TimeBaseStructure.TIM_Period=((uint32_t)72000000/freq); //ARR��ֵ����10K, �Զ���װ�ؼĴ�������ֵ
  TIM_TimeBaseStructure.TIM_Prescaler=0;                      //TIM1ʱ��Ƶ�ʵ�Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;             //ʱ�ӷָ�ֵ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  TIM_OCInitTypeDef TIMOCInitStructure;
  TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                 //PWMģʽ1���
  
  TIMOCInitStructure.TIM_Pulse = ((uint32_t)(72000000/(freq*2))) ; //ռ�ձ�=(CCRx/ARR)*100%
  TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;         //TIM����Ƚϼ��Ը���Ч
  TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;     //ʹ�����״̬ѡ������Ƚ�״̬
  TIMOCInitStructure.TIM_OutputNState = TIM_OutputState_Disable;     //ʹ�����״̬ѡ������Ƚ�״̬
  TIM_OC4Init(TIM1, &TIMOCInitStructure);                          //TIM1��CH4���

  TIM_SetCounter(TIM1, 0);  
  TIM_CtrlPWMOutputs(TIM1,ENABLE);                                 //����TIM1��PWM���Ϊʹ��
  TIM_Cmd(TIM1, ENABLE);
}
void halBeepOff(void){
  TIM_SetCounter(TIM1, 0);
  TIM_CtrlPWMOutputs(TIM1,DISABLE);
  TIM_Cmd(TIM1, DISABLE);
  TIM_SetCounter(TIM1, 0);
}


