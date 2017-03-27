#include "apps.h"
#include "halBeep.h"
#include "halADC12.h"
#include "halSPI.h"
#include "halMPU9250.h"
#include "hal24LC02B.h"
#include "halMPU9250.h"
#include "halI2S.h"

#include "stdlib.h"
#include "math.h"
#include "string.h"

#include "bottomBoard.h"
#include "ultraSound.h"
#include "mrslib.h"
#include "vTestCase.h"

extern uint16_t rbID;
extern volatile int32_t MAG_SENSOR_X;
extern volatile int32_t MAG_SENSOR_Y;

void RotateTest(){
  RobotRotate(90,SPEED_SLOW);
  vTaskDelay(1000);
  RobotRotate(-90,SPEED_SLOW);
  vTaskDelay(1000);
}

void SpeedTest(){
  SetRobotSpeed(SPEED_FAST, SPEED_FAST);
  vTaskDelay(2000);
  SetRobotSpeed(-SPEED_FAST, -SPEED_FAST);
  vTaskDelay(2000);
  SetRobotSpeed(SPEED_MID, -SPEED_MID);
  vTaskDelay(1000);
}

void PositionTest(){  
  static typeCoordinate pos;
  static uint32_t dis[2];
  dis[0] = GetDistance(1);  //distance between robot and beacon1
  dis[1] = GetDistance(2);
  pos = GetCoordinate();  //get coordinate with mm unit.
  if(pos.x < 1500){
     SetRobotSpeed(SPEED_FAST,SPEED_FAST);
  }else{
     SetRobotSpeed(0,0);
  }
  vTaskDelay(200);
}

void compassTest(){
  static int16_t nAngle = 0;
  static int16_t nXAngle = 0;
  nAngle = ReadMagSensorAngle2North();
  nXAngle = CalibrateNorth2X(); 
  vTaskDelay(200);
  nAngle =  nXAngle = 0;
 
}

void rotateTest(){
  rotateToNorthAngle(0,10);
  halt(20);
  rotateToNorthAngle(90,10);
  halt(20);
  rotateToNorthAngle(180,10);
  halt(20);
  rotateToNorthAngle(270,10);
  halt(20);
  rotateToNorthAngle(360,10);
  halt(20); 
}

void go2PointTest(){
    static typeCoordinate tarPos = {1830,700}; //mm
    go2Point(tarPos.x,tarPos.y,SPEED_MID);
    halt(20000);
}

void romReadTest(){
  u8 i = 0;
  static int16_t rbID = 0;
  static int32_t outDataX = 0;
  static int32_t outDataY = 0;
  for(i=0; i<2; i++){
    while(hal24LC02BRandomRead(MAG_ADDR_ID+i*2, (uint8_t *)(&rbID)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_ID+i*2+1, (uint8_t *)(&rbID)+i*2+1) == false);     
  }
  for (i=0;i<4;i++) { 
    while(hal24LC02BRandomRead(MAG_ADDR_X+i*2, (uint8_t *)(&outDataX)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_X+i*2+1, (uint8_t *)(&outDataX)+i*2+1) == false);
    
    while(hal24LC02BRandomRead(MAG_ADDR_Y+i*2, (uint8_t *)(&outDataY)+i*2) == false);
    while(hal24LC02BRandomRead(MAG_ADDR_Y+i*2+1, (uint8_t *)(&outDataY)+i*2+1) == false);
  }
  vTaskDelay(200);
}


//TASKS
////////////////////////////////////////////////////////////////////////////////
//MICRO PHONE SENSOR
////////////////////////////////////////////////////////////////////////////////
void MICROPHONETask( void *pvParameters ){
  while(1) {
    static uint16_t* p;
    
    halI2SStartSample();
    vTaskDelay(200);
    halI2SStopSample();
    p=halI2SReadMicroPhone();
    
    uint16_t i;
    static int32_t mRes[I2S_DMA_BUF_SIZE/4];
    uint32_t TData;
    
    i=0;
    while (i<I2S_DMA_BUF_SIZE-4) {
      TData = (((*(p+i) << 16) + *(p+i+1)) << 1);
      mRes[i/4] = *(int32_t*)(&TData)/128;
      i+=4;
    }
    asm ("NOP");
    //mRes store the microphone record result
    
  }
}

////////////////////////////////////////////////////////////////////////////////
//COMPASS SENSOR
////////////////////////////////////////////////////////////////////////////////
#define BUF_SIZE  1
void COMPASS_Task( void *pvParameters ){
  while(1) {    
    static int16_t  cX[1000];
    static uint16_t cXP=0;
    uint16_t i;
    
    static int16_t max=-32767, min=32767;
    
    SetRobotSpeed(10, -10);
    while (1) {
      while (halMPU9250RdCompassX(cX+cXP)==0){
        vTaskDelay(2);
      }
      cXP++;
      
      if (cXP==1000) {
        for (i=0;i<1000;i++) {
          if (cX[i]>max) {
            max=cX[i];
          }
          if (cX[i]<min) {
            min=cX[i];
          }
        }
        break;
      }
    }
    

    static int16_t testCompass;
    while (1) {
      
      while (halMPU9250RdCompassX(&testCompass)==0){
        vTaskDelay(2);
      }
    
      if(abs(max-testCompass) > 20) {
        SetRobotSpeed(-10, 10);
      }
      else {
        SetRobotSpeed(0, 0);
      }
      vTaskDelay(10);
    }
    
  }
}

////////////////////////////////////////////////////////////////////////////////
//GYRO SENSOR
////////////////////////////////////////////////////////////////////////////////
void GYRO_Task( void *pvParameters ) {  
  while(1) {
    static float gyroZ;
    gyroZ = halMPU9250RdGyroZ();
    if (gyroZ > 5) {
      halBeepOn(2093);
      vTaskDelay(20);
      halBeepOff();
      vTaskDelay(980);
    }
    else if (gyroZ <-5){
      halBeepOn(3951);
      vTaskDelay(20);
      halBeepOff();
      vTaskDelay(980);  
    }
    else {
      vTaskDelay(50);
    }
  }

}

////////////////////////////////////////////////////////////////////////////////
//ACCEL SENSOR
////////////////////////////////////////////////////////////////////////////////
void ACCEL_Task( void *pvParameters ) {  
  while(1) {
    
    static float accelZ;
    
    accelZ = halMPU9250RdAccelZ();

    if (accelZ > 0.5) {
      halBeepOn(2093);
      vTaskDelay(20);
      halBeepOff();
      vTaskDelay(980);
    }
    else if (accelZ <-0.5){
      halBeepOn(3951);
      vTaskDelay(20);
      halBeepOff();
      vTaskDelay(980);  
    }
    else {
      vTaskDelay(50);
    }            
  }
}

////////////////////////////////////////////////////////////////////////////////
//2 K EEPROM -> 256 BYTEs
////////////////////////////////////////////////////////////////////////////////
void EEPROM_Task( void *pvParameters ) {
  
  while (1) {
    uint8_t i;
    static int16_t inData[100], outData[100];      
    for (i=0;i<100;i++){
      inData[i] = -i;
      outData[i] = 0;
    }
    for (i=0;i<100;i++) { 
      while (hal24LC02BByteWrite(100+i*2, *((uint8_t *)(inData) + i*2))==false);
      while (hal24LC02BByteWrite(100+i*2 + 1, *((uint8_t *)(inData) + i*2+1))==false);
    }
    
    for (i=0;i<100;i++) { 
      while (hal24LC02BRandomRead(100+i*2, (uint8_t *)(outData)+i*2)==false);
      while (hal24LC02BRandomRead(100+i*2 + 1, (uint8_t *)(outData) + i*2 + 1)==false);
    }
    
    for (i=0;i<100;i++) {
      if (inData[i] != outData[i]) {
        break;
      }
    }
    if (i==100) {
      halSetLedStatus(LED_YELLOW, LED_TOGGLE);
    }
    vTaskDelay(250);
  }
}

////////////////////////////////////////////////////////////////////////////////
//Light sensor
//100HZ noise, simply filter raw data....
////////////////////////////////////////////////////////////////////////////////
void LIGHTSENSORTask( void *pvParameters )  {
  while(1)  {
    static uint16_t lightSensorData[10];
    static uint8_t p=0;
    uint8_t i;
    uint32_t sum;
    
    lightSensorData[p++] = halADC1GetData(0);
    if (p == 10){
      p =0 ;
      sum = 0;
      for (i=0;i<10;i++) {
        sum+=lightSensorData[i];
      }
      if ((sum/10)<800) {
        halBeepOn(2093);  
        vTaskDelay(50);
        halBeepOff();
        vTaskDelay(950);
      }
    }
    vTaskDelay(1);
  }
}

////////////////////////////////////////////////////////////////////////////////
//BUTTON AND LED
////////////////////////////////////////////////////////////////////////////////
extern xSemaphoreHandle xBSB1,xBSB2;

//BUTTON 1
void EXTI3ISR_BUTTON(void){
  portBASE_TYPE xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE; 
  xHigherPriorityTaskWoken = xSemaphoreGiveFromISR(xBSB1, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken == pdTRUE ) {
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken );
  }
}
//BUTTON 2
void EXTI4ISR(void){
  portBASE_TYPE xHigherPriorityTaskWoken;
  xHigherPriorityTaskWoken = pdFALSE; 
  xHigherPriorityTaskWoken = xSemaphoreGiveFromISR(xBSB2, &xHigherPriorityTaskWoken);
  if( xHigherPriorityTaskWoken == pdTRUE ) {
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken );
  }  
}

void BUTTON_LED_DEMOTask( void *pvParameters )  {
  
  portBASE_TYPE xStatus;
  while(1)  {
   xStatus = pdFALSE ;
   xStatus = xSemaphoreTake(xBSB1, 500);
   if (xStatus == pdPASS) {
     halSetLedStatus(LED_RED, LED_TOGGLE);
   }
   
   xStatus = pdFALSE ;
   xStatus = xSemaphoreTake(xBSB2, 10);
   if (xStatus == pdPASS) {
     halSetLedStatus(LED_GREEN, LED_TOGGLE);
   }
   // BUTTON 3... POLLING...
   if (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_5) == Bit_RESET) {
     halSetLedStatus(LED_YELLOW, LED_TOGGLE);
     while(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_5) == Bit_RESET);
   }
  }
}

////////////////////////////////////////////////////////////////////////////////
//BEEP DEMO
////////////////////////////////////////////////////////////////////////////////
//已知最低音的6（拉）的频率是220，设公比为q，那么：7的频率就是220*q*q 。
//乘了两个q，是因为6、7之间隔了个黑键。以此类推，两个6之间，共有12个琴键，所以：
//低音区的6（拉）的频率就是：220*q*q*q*q*q*q*q*q*q*q*q*q = 440。
//马上就可以看出，12个q相乘，等于： 440 / 220 = 2。
//那么就可以求出公比：q = 2的12次方根 = 1.059463094。
//用这个公比，和已知的220进行计算，可以得出全部琴键所对应的频率
////////////////////////////////////////////////////////////////////////////////
//2/4   四分音符为1拍  DELAY_1_4   定义DELAY_1_4为1/4拍的时间长度
#define DELAY_1_4   (100)
const int16_t musicTable[] = {
  -6, -6, -5, -6, -6, 1,  1,  2,  1, -6,  0xFF,
   1,  1, -5,  1,  2, 3,  5,  5,  3,  2,  3, 0xFF, 
   6,  6,  5,  3,  3, 1, -6, -6, -6,  3,  2, 3,    2, 0xFF,
   3,  3,  5,  3,  2, 3,  2,  1, -6, -5, -6, 0xFF, 
   3,  3,  5,  3,  3, 5,  5,  6,  8,  6,  5, 6, 8, 6, 0xFF,
};

//每个时间单位为1/4拍
const uint16_t delayTable[] = {
  4,2,2,4,2,2,4,2,2,4,4,
  4,2,2,2,2,2,2,2,2,4,4,4,
  2,2,2,2,2,2,2,2,2,2,4,1,1,2,4,
  2,2,2,2,2,2,2,2,4,4,4,4,
  2,2,2,2,2,2,2,2,2,2,4,1,1,2,4,
};
////////////////////////////////////////////////////////////////////////////
// do、re、mi、fa、so、la、ti  Table
////////////////////////////////////////////////////////////////////////////
//static uint16_t SoundLTable[7] = {1047,1175,1329,1397,1568,1760,1976};    //LOW 1234567
//static uint16_t SoundTable[7] = {2093,2349,2637,2794,3136,3520,3951};     //1234567
//static uint16_t SoundHTable[3] = {4186,4699,5274};                        //HIGH 123
const uint16_t SoundTable[18] = {
  1976,1760,1568,1397,1329,1175,1047, 0,   //LOW 7654321 -> 0-> no meaning 
  2093,2349,2637,2794,3136,3520,3951,      //1234567
  4186,4699,5274                           //HIGH 123
};                         

void BEEP_DEMOTask( void *pvParameters )  {
  while(1)  {
    uint16_t i, len;
    len = sizeof (musicTable);
    
    for (i=0;i<len;i++) {
      if (musicTable[i] == 0xFF) {
        halBeepOff();
      }
      else {
        halBeepOn(SoundTable[musicTable[i]+7]);      //halBeepOn(FREQUENCY) 
      }
      vTaskDelay(delayTable[i] * DELAY_1_4);
    }
    
    vTaskDelay(100000);    
  }
}
