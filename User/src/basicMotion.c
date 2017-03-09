/**
 * @file basicMotion.c
 * @brief basic motion functions.
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include "basicMotion.h"
#include "FreeRTOS.h"
#include "task.h"
#include <posSystem.h>
#include <readMPUSensor.h>
#include <intMath.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

void RobotRotate(int16_t angle,int16_t inSpeed) {
  static int16_t giveAngle;
  static uint32_t timeInterval;
  
  int16_t speed = boundAbs(inSpeed,SPEED_MID);
  if (angle < 0) {
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

void rotateToNorthAngle(float tar, float speed) {
  float ang = tar - CalibrateNorth2X();
  if (ang < -180) ang += 360 ;
  if (ang > 180) ang -= 360;
  RobotRotate(ang, speed);
}

//0 - 360 degree
int16_t GetLineDirectionX(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  float angle,eLong,comX, comY;
  
  comX = (float)(x1-x0);
  comY = (float)(y1-y0);
  
  //eLong = sqrtInt(comX*comX + comY*comY);
  //angle = (180*1000/MILLIRAD_PI) * (int16_t)acos(abs(comX)/eLong);  //0 to 90 degree
  eLong= sqrt(comX*comX + comY*comY);     
  angle = ((180/__PI)*acos(comY/eLong));
  
  if ((comX>=0) && (comY>=0)) {     //1
	angle = angle;
  }
  if ((comX<=0) && (comY>=0)) {     //2 
	angle = 180 - angle;
  }
  if ((comX<=0) && (comY<=0)) {     //3
	angle = 180 + angle;
  }
  if ((comX>=0) && (comY<=0)) {     //4
	angle = 360 - angle;
  }
  return (int16_t)angle;
}

void rotateTo(int16_t x,int16_t y, int16_t speed, u8 flag){
  typeCoordinate start = GetCoordinate();
  int16_t lineDir = GetLineDirectionX(start.x, start.y, x, y);
  int16_t robotDir = CalibrateNorth2X();
  int16_t turnangle = lineDir - robotDir;
  if (turnangle < -180) turnangle += 360 ;
  if (turnangle > 180) turnangle -= 360;
  if(flag == ROTATE_LARGER_15){
    if(abs(turnangle) <= 15)    
      return;
  }
  
  if(flag == ROTATE_LARGER_25){
    if(abs(turnangle) <= 25)   
      return;
  }
  
  RobotRotate(turnangle, speed);
}

void rotateFastTo(int16_t x, int16_t y, int16_t speed,u8 flag) {
  rotateTo(x,y,SPEED_MID,ROTATE_ACCURATE);
  rotateTo(x,y,SPEED_SLOW,ROTATE_ACCURATE);
}

int16_t getDistance(int16_t Ax, int16_t Ay, int16_t Bx, int16_t By) {
  return sqrtInt( (Ax-Bx)*(Ax-Bx) + (Ay-By)*(Ay-By));
}

bool GoalInFront(int16_t x, int16_t y){
  typeCoordinate nowp = GetCoordinate();
  int16_t dist = getDistance(nowp.x, nowp.y, x,y);
  if( dist < MIN_RANGE)
	return true;
  else
	return false;
}

int whichSide(int16_t x, int16_t y){
  typeCoordinate start = GetCoordinate();
  int16_t lineDir = GetLineDirectionX(start.x, start.y, x, y);
  int16_t robotDir = CalibrateNorth2X();
  int16_t turnangle = lineDir - robotDir;
  if (turnangle < -180) turnangle += 360;
  if (turnangle > 180) turnangle -= 360;
  
  if(abs(turnangle) <= 25){
    if(turnangle > 0)
      return 1; //right side
    if(turnangle < 0)
      return -1;  //left side
  }
  
  if(abs(turnangle) > 25){
    RobotRotate(turnangle, SPEED_SLOW);
  }
  return 0;  // on the line
}

void go2Point(int16_t x, int16_t y,int16_t speed){
  rotateFastTo(x,y,SPEED_MID,ROTATE_ACCURATE);
  int cell = MAX_MOTORDETA;
  int16_t speedL = speed;
  int16_t speedR = speed;
  
  if(GoalInFront(x,y) == true)
    return;
  
  typeCoordinate nowp = GetCoordinate();
  while (1) {		
    int16_t dist = getDistance(nowp.x, nowp.y, x, y);	
    if (dist < MIN_RANGE) break;			
    
    while((dist <= 0.06)){
      rotateTo(x,y,SPEED_SLOW,ROTATE_ACCURATE); 
      SetRobotSpeed(SPEED_SLOW,SPEED_SLOW);
      vTaskDelay(300);
      if((GoalInFront(x,y) == true))
        return;
    }
    whichSide(x, y);
    SetRobotSpeed(speed,speed);
    vTaskDelay(400);
    int side = whichSide(x, y);
    if (side == RIGHT_HAND) { // right side
      speedL = speed;
      speedR = speed + cell;
    } else if (side == LEFT_HAND) { // left side
      speedL = speed + cell;
      speedR = speed;
    } else {
      speedL = speedR = speed;
    }
    SetRobotSpeed(speedL,speedR);
    vTaskDelay(400);
    nowp = GetCoordinate();
  }
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



