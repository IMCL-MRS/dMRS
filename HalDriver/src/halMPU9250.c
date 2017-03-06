#include "halMPU9250.h"
#include "halSPI.h"
#include "halMCUWait.h"
#include "halLed.h"

////////////////////////////////////////////////////////////////////////////////
//��ȡ�Ĵ���
////////////////////////////////////////////////////////////////////////////////
void halMPU9250RdReg(uint8_t addr, uint8_t* data, uint8_t len){
  uint8_t i;
  SPI_MPU_ENABLE();
  halSPI1WrByte(0x80 + addr);
  for (i=0;i<len;i++) {
    data[i] = halSPI1RdByte();
  }
  SPI_MPU_DISABLE();
}

////////////////////////////////////////////////////////////////////////////////
//д��Ĵ���
////////////////////////////////////////////////////////////////////////////////
void halMPU9250WrReg(uint8_t addr, uint8_t data){
  SPI_MPU_ENABLE();
  halSPI1WrByte(addr);
  halSPI1WrByte(data);
  SPI_MPU_DISABLE();
}

void halMPU9250I2CMasterWait(void) {
  uint8_t status=0x00;
  while((status & 0x40) != 0x40 ) {
    halMPU9250RdReg(I2C_MST_STATUS, &status, 1);
    halMCUWaitMS(2);
  }  
}

////////////////////////////////////////////////////////////////////////////////
//��ʼ��
////////////////////////////////////////////////////////////////////////////////
void halMPU9250Init(void){
  halMPU9250WrReg(USER_CTRL,  0x30);         //ʹ�� MPU9250 SPI ����ģʽ�� I2C ���� I/F ģʽ 
  halMPU9250WrReg(CONFIG,     0x06);
  halMPU9250WrReg(PWR_MGMT_1, 0);
  halMPU9250WrReg(PWR_MGMT_2, 0);
  
  halMPU9250WrReg(I2C_SLV4_ADDR,0x0C);
  halMPU9250WrReg(I2C_SLV4_REG,0x0A);
  halMPU9250WrReg(I2C_SLV4_DO, 0x00);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250WrReg(I2C_SLV4_ADDR,0x0C);
  halMPU9250WrReg(I2C_SLV4_REG,0x0A);
  halMPU9250WrReg(I2C_SLV4_DO, 0x16);        //��������100HZ
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  asm("NOP");
  
  uint8_t wmi=0;
  halMPU9250RdReg(REG_WHO_AM_I ,&wmi, 1);
  if( wmi== 0x71){
    halSetLedStatus(LED_RED, LED_ON);
    halMCUWaitMS(10);
    halSetLedStatus(LED_RED, LED_OFF);
  }
  
  while (wmi != 0x71) {
    halMCUWaitMS(10);
  }
  
  halMCUWaitMS(10);
}

uint8_t halMPU9250RdCompassX(int16_t* cx){
  uint8_t regValue[2]={0,0};
  uint8_t st;
  //��ȡ״̬�Ĵ���ST1
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x02);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  
  if ((st&0x01) == 0x00) {
    return 0;
  }
  
  asm ("NOP");
  //��ȡCOMPASS XLOW
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x03);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[0]), 1);
  //��ȡCOMPASS XHIGH
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x04);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[1]), 1);
  //��ȡ״̬�Ĵ���ST1 ��ȡ����
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x09);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  asm ("NOP");
  
  *cx = *(int16_t*)(regValue);
  
  return 1;
}

uint8_t halMPU9250RdCompassY(int16_t* cx){
  uint8_t regValue[2]={0,0};
  uint8_t st;
  //��ȡ״̬�Ĵ���ST1
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x02);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  
  if ((st&0x01) == 0x00) {
    return 0;
  }
  
  asm ("NOP");
  //��ȡCOMPASS XLOW
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x05);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[0]), 1);
  //��ȡCOMPASS XHIGH
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x06);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[1]), 1);
  //��ȡ״̬�Ĵ���ST2 ��ȡ����
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x09);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  asm ("NOP");
  
  *cx = *(int16_t*)(regValue);
  
  return 1;
}

uint8_t halMPU9250RdCompassZ(int16_t* cx){
  uint8_t regValue[2]={0,0};
  uint8_t st;
  //��ȡ״̬�Ĵ���ST1
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x02);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  
  if ((st&0x01) == 0x00) {
    return 0;
  }
  
  asm ("NOP");
  //��ȡCOMPASS XLOW
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x07);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[0]), 1);
  //��ȡCOMPASS XHIGH
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x08);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &(regValue[1]), 1);
  //��ȡ״̬�Ĵ���ST2 ��ȡ����
  halMPU9250WrReg(I2C_SLV4_ADDR,0x8C);
  halMPU9250WrReg(I2C_SLV4_REG,0x09);
  halMPU9250WrReg(I2C_SLV4_CTRL,0x81);
  halMPU9250I2CMasterWait();
  halMPU9250RdReg(I2C_SLV4_DI, &st, 1);
  asm ("NOP");
  
  *cx = *(int16_t*)(regValue);
  
  return 1;
}

float halMPU9250RdAccelX(void){
  uint8_t regValue[2];
  halMPU9250RdReg(ACCEL_XOUT_L, &regValue[0], 1);
  halMPU9250RdReg(ACCEL_XOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/16384.0);
}

float halMPU9250RdAccelY(void){
  uint8_t regValue[2];
  halMPU9250RdReg(ACCEL_YOUT_L, &regValue[0], 1);
  halMPU9250RdReg(ACCEL_YOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/16384.0);  
}

float halMPU9250RdAccelZ(void){
  uint8_t regValue[2];
  halMPU9250RdReg(ACCEL_ZOUT_L, &regValue[0], 1);
  halMPU9250RdReg(ACCEL_ZOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/16384.0);  
}

float halMPU9250RdGyroX(void){
  uint8_t regValue[2];
  halMPU9250RdReg(GYRO_XOUT_L, &regValue[0], 1);
  halMPU9250RdReg(GYRO_XOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/131.0);  
}

float halMPU9250RdGyroY(void){
  uint8_t regValue[2];
  halMPU9250RdReg(GYRO_YOUT_L, &regValue[0], 1);
  halMPU9250RdReg(GYRO_YOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/131.0);  
}

float halMPU9250RdGyroZ(void){
  uint8_t regValue[2];
  halMPU9250RdReg(GYRO_ZOUT_L, &regValue[0], 1);
  halMPU9250RdReg(GYRO_ZOUT_H, &regValue[1], 1);
  return ((float)(*(int16_t*)(regValue))/131.0);  
}


