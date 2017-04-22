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

typeCoordinate GetCoordinate1(void) {
  static float disB1, disB2, xTmp,yTmp;
  static typeCoordinate coordinateC;
  
  disB1 = GetDistanse2B1();
  disB2 = GetDistanse2B2();
  
  yTmp = (disB1*disB1 - disB2*disB2 + DISTANCE_B1_2_B2*DISTANCE_B1_2_B2)/(2*DISTANCE_B1_2_B2);
  xTmp = sqrt(disB1*disB1 - DISTANCE_B_2_GROUND*DISTANCE_B_2_GROUND - yTmp*yTmp);
  coordinateC.y = (int32_t)yTmp;
  coordinateC.x = (int32_t)xTmp;
  return coordinateC;
}


typeCoordinate GetCoordinate(void) {
    static typeCoordinate __cor;
    float dis2b1 = GetDistance(1);
    float dis2b2 = GetDistance(2);
    float dis2b1g = sqrt( dis2b1*dis2b1 - DISTANCE_B_2_GROUND*DISTANCE_B_2_GROUND );
    float dis2b2g = sqrt( dis2b2*dis2b2 - DISTANCE_B_2_GROUND*DISTANCE_B_2_GROUND );

    float P = (dis2b1g + dis2b2g + DISTANCE_B1_2_B2) / 2;
    float S = sqrt( P * (P-dis2b1g) * (P-dis2b2g) * (P-DISTANCE_B1_2_B2) );
    __cor.x =  (int32_t)(2*S/DISTANCE_B1_2_B2);

    float b1y = (sqrt( dis2b1g*dis2b1g - __cor.x*__cor.x ) );
    float b2y = (sqrt( dis2b2g*dis2b2g - __cor.x*__cor.x ) );
    __cor.y = (int32_t)b1y;
    if (fabs(b2y - b1y - DISTANCE_B1_2_B2) < 1) {
      __cor.y = (int32_t)-b1y;
    }
    return __cor;
}


