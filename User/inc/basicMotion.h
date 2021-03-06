/**
 * @file basicMotion.h
 * @brief basic motion functions.
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#ifndef __BASICMOTION_H
#define __BASICMOTION_H
   
#include "bottomBoard.h"
#include "vInfoList.h"
#include "halBeep.h"
#include <systemConfig.h>
#include <stdbool.h>


/*Motor Speed*/
#define SPEED_FAST 30  // 80mm/s
#define SPEED_MID  15  // 50mm/s
#define SPEED_SLOW 10  // 30mm/s

#define MAX_MOTORDETA      (10)

#define ROTATE_ACCURATE          (0)
#define ROTATE_LARGER_15         (1)
#define ROTATE_LARGER_25         (2)

#define MIN_RANGE                (20)
#define MAX_RANGE                (50)   

#define RIGHT_HAND               (1)
#define LEFT_HAND               (-1)
   
typedef struct robotPos{
  int32_t locationX;        //4bytes
  int32_t locationY;        //4bytes
}robotPos;

//package information 28 Bytes
typedef struct rbNode
{
  u8 nodeID;           //1 byte	 4
  robotPos rpos;       //8 bytes 12 
  uint16_t angle2n;     //4 bytes 16
  u8 isActive;         //1 byte  
  u8 isLeaderOK;       //1 byte
  u8 isReady;          //1 bytes
  u8 isFoundPath;      //1 byte  20
  u8 type;	       //1 byte
  u8 isStop;           //1 byte  24
  u8 nbrList[GROBOTS];  //ROBOTS*1 byte 32
  uint16_t crc16Res;
}rbNode;

extern void RobotRotate(float angle,int16_t speed); 
extern void rotateToNorthAngle(int16_t tar, int16_t speed);
extern int16_t GetLineDirectionX(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
extern void rotateTo(int32_t x,int32_t y, int16_t speed, u8 flag);
extern void rotateFastTo(int32_t x, int32_t y, int16_t speed,u8 flag);
extern int16_t getDistance(int32_t Ax, int32_t Ay, int32_t Bx, int32_t By);
extern bool GoalInFront(int32_t x, int32_t y);
extern int whichSide(int32_t x, int32_t y);
extern void go2Point(int32_t x, int32_t y,int16_t speed);

extern void RobotGoStrait(int16_t ls, int16_t rs);
extern void RobotGoCircle(int16_t s, uint16_t r);
extern void RobotGoTo(uint16_t x, uint16_t y);
extern void RobotTowardDst(uint16_t x, uint16_t y);
extern void RobotFollowLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void RobotFollowCircle(uint16_t x, uint16_t y, uint16_t r, int16_t s);
extern void halt(uint16_t time);
extern u8 bt3Pressed();
extern void beepSignal();
#endif

