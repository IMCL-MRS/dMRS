/**
 * @file intMath.c
 * @brief fast integer math (no floating point processor on dMRS)
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/
#include <intMath.h>

/*
*	@brief Compute the integer square root of a number.
*
*	Based on Microchip app note TB040.
*	Can't take the root of numbers higher than MAX_INT32.
*	@param val is the number to be computed
*	@returns the computed integer square root
*/
uint32_t sqrtInt(uint32_t val) {
  uint32_t g;
  uint32_t bit = 0x8000;
  uint32_t result = 0;
  uint32_t test;
  
  
  // can't take the root of numbers higher than MAX_INT32
  if (val >= 0x80000000) {
    result = 46341;
  }
  else {
    while (bit != 0) {
      g = result | bit;
      test = g * g;
      if (test == val) {
        // we're done!
        result = g;
        break;
      }else if (test < val) {
        // our guess is too small, keep the temp bit
        result |= bit;
      }
      // shift the bit down one
      bit >>= 1;
    }
  }
  return result;
}


/*
 * @brief Counts how many bits the value has.
 *
 * Ignores leading zeros.
 * @param val is the value to be counted
 * @returns the number of bits of the input value
 */
uint8_t bitsCount(uint32_t val) {
	uint32_t bitMask;
	uint8_t returnVal = 0;

	for (bitMask = 1; bitMask != 0; bitMask = bitMask << 1) {
		if (val & bitMask) {
			returnVal++;
		}
	}
	return(returnVal);
}

/*
 * @brief Finds the min of the two arguments.
 *
 * Finds the min of two arguments.
 * @param x, y is the value to be compared
 * @returns the min value
 */
int32_t min(int32_t x, int32_t y) {
	if (x > y) {
		return y;
	}
	return x;
}

/*
 * @brief Finds the min of the two arguments.
 *
 * Finds the min of two arguments.
 * @param x, y is the value to be compared
 * @returns the min value
 */
int32_t max(int32_t x, int32_t y) {
	if (x > y) {
		return x;
	}
	return y;
}

/*
 * @brief Bounds the value with one specified bound as both lower and upper bound.
 *
 * Bounds the input value so that it stays within the range of -bound <= value <= bound.
 * If it exceeds the bound, set it to the bound.
 * @param val is the value to be bounded
 * @param bound is the lower and upper bound
 * @returns the bounded value
 */
int32_t boundAbs(int32_t val, int32_t bound) {
	if (val > bound) {
		val = bound;
	}
	else if (val < -bound){
		val = -bound;
	}
	return(val);
}

float FloatAbs(float data) {
  if (data<0.0) {
    return (0.0-data);
  }
  return data;
}