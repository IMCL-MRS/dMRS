#include "halADC12.h"

uint16_t adcBuf[ADC_NUMBER_PER_CH][ADC_CH_NUM];

void halADC12Init(void) {
 //1. ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,  ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 
  //2. GPIO ��ʼ��
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //3. ADC ��ʼ��
  ADC_DeInit(ADC1);
  
  ADC_InitTypeDef ADC_InitStructure;     
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                   //ADC1��ADC2�����ڶ���ģʽ
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;                         //ʹ��ɨ��
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   //ADCת������������ģʽ
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  //���������ת��
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;               //ת�������Ҷ���
  ADC_InitStructure.ADC_NbrOfChannel = ADC_CH_NUM;                     //����ת��ͨ����Ϊ8
  ADC_Init(ADC1, &ADC_InitStructure);                                  //��ʼ��ADC

  //3. 2. 4. 1
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 3, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 4, ADC_SampleTime_239Cycles5 );

  ADC_DMACmd(ADC1, ENABLE);                                            //ʹ��ADC1ģ��DMA
  ADC_Cmd(ADC1, ENABLE);                                               //ʹ��ADC1
  
  ADC_ResetCalibration(ADC1);                                          //����ADC1У׼�Ĵ���
  while(ADC_GetResetCalibrationStatus(ADC1));                          //�ȴ�ADC1У׼�������
  ADC_StartCalibration(ADC1);                                          //��ʼADC1У׼
  while(ADC_GetCalibrationStatus(ADC1));                               //�ȴ�ADC1У׼���
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);                              //ʹ��ADC1�����ʼת��x 
  
  //4. DMA����
  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel1);                                                  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&ADC1->DR);                //DMA����ADC����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)adcBuf;                         //DMA�ڴ����ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
  DMA_InitStructure.DMA_BufferSize = ADC_BUF_SIZE;                            //DMAͨ����DMA����Ĵ�С
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //�����ַ�Ĵ�������
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�ڴ��ַ�Ĵ�������
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //���ݿ��Ϊ16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //������ѭ������ģʽ
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         //DMAͨ�� xӵ�и����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��  
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
//����DMA ��ʼ����
////////////////////////////////////////////////////////////////////////////////
void halADC1Start(void) {
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
//����˲�������
////////////////////////////////////////////////////////////////////////////////
uint16_t halADC1GetData(uint8_t ch) {
  uint16_t j;
  uint32_t sum;
  sum = 0;
  DMA_Cmd(DMA1_Channel1, DISABLE);
  for (j=0;j<ADC_NUMBER_PER_CH;j++) {
    sum += adcBuf[j][ch];
  }
  DMA_Cmd(DMA1_Channel1, ENABLE);
  return sum/ADC_NUMBER_PER_CH;
}


