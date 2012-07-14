/*
 * Vector3f.h
 *
 *  Created on: 2011-04-05
 *      Author: jarrett
 */

#ifndef VECTOR3F_H_
#define VECTOR3F_H_

#include "../common/compatibility/Types.h"

namespace vmath {

using namespace compatibility;

class Vector3f {
public:
	Vector3f(float32 x = 0.f, float32 y = 0.f, float32 z = 0.f);
	Vector3f(const Vector3f& v);
	/*
	 * removed virtual declaration so that no vtable is required (makes class
	 * 12 bytes instead of 16 bytes)
	 */
	~Vector3f();

	// Scale the vector
	const Vector3f& operator*=(const float32& f);
	const Vector3f& operator/=(const float32& f);
	const Vector3f& operator+=(const Vector3f& vector);
	const Vector3f operator+(const Vector3f& vector) const;
	const Vector3f operator-(const Vector3f& vector) const;
	const float32 operator*(const Vector3f& vector) const;

	void normalize();
	const float32 getLength() const;

	// these variables are exposed in the interests of speed
	float32 x, y, z;
};

}

#endif /* VECTOR3F_H_ */
