/**
 * @file posSystem.c
 * @brief multi-robot localization system
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include "posSystem.h"
#include "intMath.h"
#include <math.h>

uint32_t GetDistanse2B1(void) {  
  return GetDistance(1);  
}

uint32_t GetDistanse2B2(void) {
  return GetDistance(2);
}

typeCoordinate GetCoordinate(void) {
  static float disB1, disB2;
  static float yTmp;
  static float xTmp;
  static typeCoordinate coordinateC;
  
  disB1 = GetDistanse2B1();
  disB2 = GetDistanse2B2();
  
  coordinateC.y = (disB1*disB1 - disB2*disB2 + DISTANSE_B1_2_B2*DISTANSE_B1_2_B2)/(2*DISTANSE_B1_2_B2);
  coordinateC.x = sqrt(disB1*disB1 - DISTANSE_B1_2_B2*DISTANSE_B1_2_B2 - yTmp*yTmp);
  return coordinateC;
}


