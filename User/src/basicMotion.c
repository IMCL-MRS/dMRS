/**
 * @file basicMotion.c
 * @brief basic motion functions.
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include "basicMotion.h"
#include "FreeRTOS.h"
#include "task.h"
#include <intMath.h>

void RobotRotate(int16_t angle,int16_t inSpeed) {
  static int16_t giveAngle;
  static uint32_t timeInterval;
  
  int16_t speed = boundAbs(inSpeed,SPEED_MID);
  if (angle<0) {
    giveAngle = (int16_t)(-angle*MILLIRAD_PI/180.0);   //rad
    SetRobotSpeed(speed, 0-speed);
  }
  else {
    giveAngle = (int16_t)(angle*MILLIRAD_PI/180);   //rad
    SetRobotSpeed(0-speed, speed);
  }     
  timeInterval = (uint32_t)(giveAngle*(WHEEL_L_R_DISTANCE*10)/(2*10*speed)); //(L = n*PI*r/180)
  vTaskDelay(timeInterval);
  SetRobotSpeed(0, 0);
}

void RobotGoStrait(int16_t ls, int16_t rs) {

}

void RobotGoCircle(int16_t s, uint16_t r) {
  
}

void RobotGoTo(uint16_t x, uint16_t y) {

}


void RobotTowardDst(uint16_t x, uint16_t y) {

}

void RobotFollowLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  
}

void RobotFollowCircle(uint16_t x, uint16_t y, uint16_t r, int16_t s) {

}

void halt(uint16_t time){
  vTaskDelay(time*100);  //0.5s
}



