/**
 * @file posSystem.h
 * @brief robot localization system
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#ifndef __POS_SYSTEM_H
#define __POS_SYSTEM_H
#include "ultraSound.h"

#define DISTANSE_B1_2_B2         (1100)  //110mm
#define DISTANSE_B_2_GROUND      (1750)  //175mm

static uint32_t distanse2B1=0, distanse2B2=0;

typedef struct typeCoordinate{
  uint32_t x;
  uint32_t y;
}typeCoordinate;

extern uint32_t GetDistanse2B1(void);
extern uint32_t GetDistanse2B2(void);
extern typeCoordinate GetCoordinate(void);

#endif