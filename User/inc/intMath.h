/**
 * @file intMath.h
 * @brief fast integer math (no floating point processor on dMRS)
 * @since March 5, 2017
 * @author Jinlin CHEN
 **/

#ifndef __INT_MATH_H
#define __INT_MATH_H
#include <stdint.h>

/******** Defines ********/
#define __PI 3.14159
/* Angles */
#define MILLIRAD_HALF_PI			1571
#define MILLIRAD_PI				(MILLIRAD_HALF_PI * 2)
#define MILLIRAD_2PI				(MILLIRAD_HALF_PI * 4)

/******** Functions ********/
//General math
/**
 *	@brief Compute the integer square root of a number.
 *
 *	Based on Microchip app note TB040.
 *	Can't take the root of numbers higher than MAX_INT32.
 *	@param val is the number to be computed
 *	@returns the computed integer square root
 */
uint32_t sqrtInt(uint32_t val);

/**
 * @brief Average two values.
 *
 * @param val1 is the first value
 * @param val2 is the second value
 * @returns the average of val1 and val2
 */
int32_t average(int32_t val1, int32_t val2);

/**
 * @brief Counts how many bits the value has.
 *
 * Ignores leading zeros.
 * @param val is the value to be counted
 * @returns the number of bits of the input value
 */
uint8_t bitsCount(uint32_t val);


/**
 * @brief Finds the min of the two arguments.
 *
 * Finds the min of two arguments.
 * @param x is the value to be compared
 * @param y is the value to be compared * @returns the min value
 */
int32_t min(int32_t x, int32_t y);

/**
 * @brief Finds the min of the two arguments.
 *
 * Finds the min of two arguments.
 * @param x is the value to be compared
 * @param y is the value to be compared
 * @returns the min value
 */
int32_t max(int32_t x, int32_t y);

/**
 * @brief Bounds the value with one specified bound as both lower and upper bound.
 *
 * Bounds the input value so that it stays within the range of -bound <= value <= bound.
 * If it exceeds the bound, set it to the bound.
 * @param val is the value to be bounded
 * @param bound is the lower and upper bound
 * @returns the bounded value
 */
int32_t boundAbs(int32_t val, int32_t bound);

#endif