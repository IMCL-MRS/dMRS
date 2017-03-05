#include "halnRF24L01P.h"
#include "halSPI.h"
#include "string.h"

uint8_t TX_ADDRESS[5]  = {0x34,0x43,0x10,0x10,0x01};

////////////////////////////////////////////////////////////////////////////////
//PA11  -> RADIO_IRQ
//PC5   -> RADIO_CE
//PB1   -> SPI_RF
//PB13  -> SPI1_SCLK
//PB14  -> SPI1_MISO
//PB15  -> SPI1_MOSI
////////////////////////////////////////////////////////////////////////////////
void halnRF24L01PInit(void){
  //1. RCC
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,  ENABLE);
  //2. GPIO
  GPIO_InitTypeDef GPIO_InitStructure;                                             
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;                                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOD, &GPIO_InitStructure);                                    
  GPIO_ResetBits(GPIOD, GPIO_Pin_3); 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOD, &GPIO_InitStructure);      
  
  //3. EXIT
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line    = GPIO_Pin_5;                            //�ⲿ�ж���
  EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;                   //�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                  //�жϴ�����ʽ
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                                //���ж�
  EXTI_Init(&EXTI_InitStructure);
  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource5);
  
  //NVIC
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;           
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;         
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
  
  //////////////////////////////////////////////////////////////////////////////
  halnRF24L01PRxPacket();
}

uint8_t halnRF24L01PRdRegByte(uint8_t reg) {
  uint8_t value=0, cmd=0;
  cmd = (SPI_CMD_R_REGRESTER + (reg&0x1F));
  SPI_RF_ENABLE();
  halSPI1WrByte(cmd);
  value = halSPI1RdByte();
  SPI_RF_DISABLE();
  return value;
}

uint8_t halnRF24L01PWrRegByte(uint8_t reg, uint8_t data) {  
  uint8_t cmd;
  uint8_t status;
  cmd = (SPI_CMD_W_REGRESTER + (reg&0x1F));
  SPI_RF_ENABLE();
  status = halSPI1WrRdByte(cmd); 
  halSPI1WrByte(data);
  SPI_RF_DISABLE();
  return status;
}

uint8_t halnRF24L01PRdRegPacket(uint8_t reg, uint8_t* data, uint8_t len){
  uint8_t status;
  SPI_RF_ENABLE();
  status = halSPI1WrRdByte(reg);
  halSPI1RdPacket(data, len);
  SPI_RF_DISABLE();
  return status;
}

uint8_t halnRF24L01PWrRegPacket(uint8_t reg, uint8_t* data, uint8_t len){
  uint8_t status;
  SPI_RF_ENABLE();
  status = halSPI1WrRdByte(reg);
  halSPI1WrPacket(data, len);
  SPI_RF_DISABLE();
  return status;
}

void halnRF24L01PSetTxPower(uint8_t tp) {
}
void halnRF24L01PSetDataRate(uint8_t dr) {
}
void halnRF24L01PSetChannel(uint8_t hz) {
}
void halnRF24L01PSetTxAddr(uint8_t* addr, uint8_t len) {
}
void halnRF24L01PSetRxAddr(uint8_t* addr, uint8_t len) {
}

uint8_t halnRF24L01PBroadcastPacket(uint8_t* data, uint8_t len) {
  if (halnRF24L01PRdRegByte(REG_RPD)==RPD) {
    //return 0;
  }
  halnRF24L01PTxPacket(data, len);
  return 1;
}

void halnRF24L01PTxPacket(uint8_t* data, uint8_t len) {
  uint8_t dataLen;
  uint8_t dataInput[32];
  memset(dataInput, 0, 32);
  if (len >32) {
    dataLen=32;
  }
  else {
    dataLen=len;
  }
  memcpy(dataInput, data, dataLen);
  
  CEPinSetLow();
  halnRF24L01PWrRegByte(SPI_CMD_FLUSH_TX, 0xFF);                                //���TX FIFO
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_STATUS, MAX_RT+TX_DS);        //�����־λ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_FEATURE,(1<<1));              //ʹ��EN_DYN_ACK
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_EN_AA, 0x00);                 //��ֹACK
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_SETUP_RETR, 0x00);            //��ֹ�ش�
  halnRF24L01PWrRegPacket(SPI_CMD_W_REGRESTER+REG_TX_ADDR, TX_ADDRESS, 5);      //TX ��ַ -> Ŀ���ַ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_RF_CH,   100);                //ͨ��ѡ��(2.4G+100M)HZ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_RF_SETUP,(1<<3)+(3<<1));      //2Mbps+0dBm  
  halnRF24L01PWrRegPacket(SPI_CMD_W_TX_PAYLOAD_NOACK, dataInput, 32);           //д��TX_FIFO
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_CONFIG, 0x0E);                //Tx MODE + CRC16
  
  CEPinSetHigh();
}

void halnRF24L01PRxPacket(void) {
  CEPinSetLow();
  halnRF24L01PWrRegByte(SPI_CMD_FLUSH_RX, 0xFF);                                //���TX FIFO
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_STATUS, RX_DR);               //�����־λ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_EN_AA, 0x00);                 //��ֹACK
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_SETUP_RETR, 0x00);            //��ֹ�ش�
  halnRF24L01PWrRegPacket(SPI_CMD_W_REGRESTER + REG_RX_ADDR_P0, TX_ADDRESS, 5); //RX PIPE0 ��ַ -> Ŀ���ַ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_EN_RXADDR, 0x01);             //ʹ��PIPE0
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_RX_PW_P0, 32);                //��̬���ݳ���
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_RF_CH,   100);                //ͨ��ѡ��(2.4G+100M)HZ
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_RF_SETUP,(1<<3)+(3<<1));      //2Mbps+0dBm
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_CONFIG, 0x0F);                //Rx MODE + CRC16
  CEPinSetHigh(); 
}

__weak void EXTI5ISR_RF(void) {
  if (halSPI1BusBusy()==1) {
    return;
  }
  halnRF24L01PWrRegByte(SPI_CMD_W_REGRESTER + REG_STATUS, MAX_RT+TX_DS+RX_DR);  //�����־λ
}

