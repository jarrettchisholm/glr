/*
 * Math.h
 *
 *  Created on: 2011-04-13
 *      Author: jarrett
 */

#ifndef MATH_H_
#define MATH_H_

namespace math {

static const float PI = 3.14159265358979323846f;
static const float RECIPROCAL_PI = 1.0f / PI;
static const float HALF_PI = PI / 2.0f;
static const float DEGTORAD = PI / 180.0f;
static const float RADTODEG = 180.0f / PI;

class Math {
public:
	Math();
	virtual ~Math();

	static float linearInterp(float a, float b, float dist);
	static float cosineInterp(float a, float b, float dist);
	static float cubicInterp(float a0, float a1, float b1, float b2, float dist);

	/**
	 *	Compares a and b and returns the greater of the two.
	 */
	template<class T> static T max(T a, T b) {
		return (a > b ? a : b);
	}

	/**
	 *	Compares a and b and returns the lesser of the two.
	 */
	template<class T> static T min(T a, T b) {
		return (a < b ? a : b);
	}

	static int random();

	static const int RANDOM_MAX = 32767;
};

}

#endif /* MATH_H_ */
