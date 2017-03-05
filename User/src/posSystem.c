/**
 * @file posSystem.c
 * @brief multi-robot localization system
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include "posSystem.h"
#include "intMath.h"

uint32_t GetDistanse2B1(void) {  
  return GetDistance(1);  
}

uint32_t GetDistanse2B2(void) {
  return GetDistance(2);
}

typeCoordinate GetCoordinate(void) {
  static uint32_t disB1, disB2;
  static typeCoordinate coordinateC;
  
  disB1 = GetDistanse2B1();
  disB2 = GetDistanse2B2();
  
  coordinateC.y = (disB1*disB1 - disB2*disB2 + DISTANSE_B1_2_B2*DISTANSE_B1_2_B2)/(2*DISTANSE_B1_2_B2);
  coordinateC.x = sqrtInt(disB1*disB1 - DISTANSE_B1_2_B2*DISTANSE_B1_2_B2 - coordinateC.y*coordinateC.y);
  return coordinateC;
}
