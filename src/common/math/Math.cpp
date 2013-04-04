/*
 * Math.cpp
 *
 *  Created on: 2011-04-13
 *      Author: jarrett
 */

#include "Math.h"

#include <math.h>
#include <stdlib.h>

namespace math {
Math::Math()
{
}

Math::~Math()
{
}

/**
 * Implementation of Linear Interpolation between 2 values.
 *
 * @param a The starting value
 * @param b The ending value
 * @param dist A value between 0.0 and 1.0; will return a value between a and b
 *
 * @return A value between a and b, based on the dist value given
 */
float Math::linearInterp(float a, float b, float dist)
{
	return a * (1 - dist) + b * dist;
}

/**
 * Implementation of Cosine Interpolation between 2 values.  Basically, gives a
 * smoother result compared to the Linear Interpolation function, but requires a
 * a little more computation.
 *
 * @param a The starting value
 * @param b The ending value
 * @param dist A value between 0.0 and 1.0; will return a value between a and b
 *
 * @return A value between a and b, based on the dist value given
 */
float Math::cosineInterp(float a, float b, float dist)
{
	float ft = dist * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;

	return a * (1.0f - f) + b * f;
}

/**
 * Implementation of Cubic Interpolation between 2 values.  Basically, gives a
 * smoother result compared to the Cosine Interpolation function, but requires a
 * a bunch more computation.
 *
 * @param a0 The value before the starting value
 * @param a1 The starting value
 * @param b1 The ending value
 * @param b2 The value after the starting value
 * @param dist A value between 0.0 and 1.0; will return a value between a1 and b1
 *
 * @return A value between a and b, based on the dist value given
 */
float Math::cubicInterp(float a0, float a1, float b1, float b2, float dist)
{
	float P = (b2 - b1) - (a0 - a1);
	float Q = (a0 - a1) - P;
	float R = b1 - a0;
	float S = a1;

	// Px^3 + Qx^2 + Rx + S;
	return P * (dist * dist * dist) + Q * (dist * dist) + R * (dist) + S;
}

/**
 *
 */
int Math::random()
{
	static unsigned int retVal = 5323;

	retVal = (8253729 * retVal + 2396402);

	// returns value between 0 and 32767
	return retVal % RANDOM_MAX;

	/*
	   int m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	   int m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	   return (m_z << 16) + m_w;
	 */
}
}
