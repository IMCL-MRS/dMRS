#include "hal24LC02B.h"

////////////////////////////////////////////////////////////////////////////////
//24LC02B 需要最大5ms的写入周期
////////////////////////////////////////////////////////////////////////////////
void hal24LC02BInit(void) {
  return;
}

bool hal24LC02BByteWrite(uint8_t addr, uint8_t data){
  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_WRITE); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CWriteByte(addr);
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CWriteByte(data);
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CStop();
  //halMCUWaitMS(5);
  return true;
}

bool hal24LC02BPageWrite(uint8_t addr, uint8_t* data, uint8_t len){
  uint8_t i;
  
  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_WRITE); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CWriteByte(addr);
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  for (i=0;i<len;i++) {
    halI2CWriteByte(*(data+i));
    if (halI2CReadAck()==false) {
      halI2CStop();
      return false;
    }
  }
  halI2CStop();
  
  halMCUWaitMS(5);
  
  return true;
}

uint8_t hal24LC02BCurAddrRead(void) {
  uint8_t data;
  
  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_READ); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  data = halI2CReadByte();
  halI2CWriteNAck();
  halI2CStop();

  return data;
}

bool hal24LC02BRandomRead(uint8_t addr, uint8_t* data) {

  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_WRITE); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CWriteByte(addr);
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  
  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_READ); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  *data = halI2CReadByte();
  halI2CWriteNAck();
  halI2CStop();
  
  return true;
}

bool hal24LC02BSeqRead(uint8_t addr, uint8_t* data, uint8_t len) {
  uint8_t i;
  
  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_WRITE); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  halI2CWriteByte(addr);
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }

  halI2CStart();
  halI2CWriteByte(CONTROL_CODE + CHIP_SELECT_ADDR + R_W_READ); 
  if (halI2CReadAck()==false) {
    halI2CStop();
    return false;
  }
  for (i=0;i<len;i++) {
    *(data+i) = halI2CReadByte();
    if (i==(len-1)){
      halI2CWriteNAck();
    }
    else {
      halI2CWriteAck();
    }
  }
  halI2CStop();
  return true;
}



bool hal24LC02BWordWrite(uint8_t addr, int16_t inData){
  uint8_t i = 0;
  for (i=0;i<2;i++) { 
    while (hal24LC02BByteWrite(addr+i*2, *((uint8_t *)(inData) + i*2))==false);
    while (hal24LC02BByteWrite(addr+i*2 + 1, *((uint8_t *)(inData) + i*2+1))==false);
  }
  return true;
}

bool hal24LC02BRandomWordRead(uint8_t addr,int16_t *outData){
  uint8_t i = 0;
  for (i=0;i<2;i++) { 
    while (hal24LC02BRandomRead(addr+i*2, (uint8_t *)(outData)+i*2)==false);
    while (hal24LC02BRandomRead(addr+i*2 + 1, (uint8_t *)(outData) + i*2 + 1)==false);
  }
  return true;
}

bool hal24LC02BUWordWrite(uint8_t addr, uint16_t inData){
  uint8_t i = 0;
  for (i=0;i<2;i++) { 
    while (hal24LC02BByteWrite(addr+i*2, *((uint8_t *)(inData) + i*2))==false);
    while (hal24LC02BByteWrite(addr+i*2 + 1, *((uint8_t *)(inData) + i*2+1))==false);
  }
  return true;
}

bool hal24LC02BRandomUWordRead(uint8_t addr,uint16_t *outData){
  uint8_t i = 0;
  for (i=0;i<2;i++) { 
    while (hal24LC02BRandomRead(addr+i*2, (uint8_t *)(outData)+i*2)==false);
    while (hal24LC02BRandomRead(addr+i*2 + 1, (uint8_t *)(outData) + i*2 + 1)==false);
  }
  return true;
}

bool hal24LC02BDWordWrite(uint8_t addr, int32_t inData){
  uint8_t i = 0;
  for (i=0;i<4;i++) { 
    while (hal24LC02BByteWrite(addr+i*2, *((uint8_t *)(inData) + i*2))==false);
    while (hal24LC02BByteWrite(addr+i*2 + 1, *((uint8_t *)(inData) + i*2+1))==false);
  }
  return true;
}

bool hal24LC02BRandomDWordRead(uint8_t addr,int32_t *outData){
  uint8_t i = 0;
  for (i=0;i<4;i++) { 
    while (hal24LC02BRandomRead(addr+i*2, (uint8_t *)(outData)+i*2)==false);
    while (hal24LC02BRandomRead(addr+i*2 + 1, (uint8_t *)(outData) + i*2 + 1)==false);
  }
  return true;
}