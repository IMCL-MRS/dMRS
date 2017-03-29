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
  float disB1, disB2, xTmp,yTmp;
  static typeCoordinate coordinateC;
  
  disB1 = GetDistanse2B1();
  disB2 = GetDistanse2B2();
  
  yTmp = (disB1*disB1 - disB2*disB2 + DISTANSE_B1_2_B2*DISTANSE_B1_2_B2)/(2*DISTANSE_B1_2_B2);
  xTmp = sqrt(disB1*disB1 - DISTANSE_B1_2_B2*DISTANSE_B1_2_B2 - yTmp*yTmp);
  coordinateC.x = xTmp;
  coordinateC.y = yTmp;
  return coordinateC;
}


