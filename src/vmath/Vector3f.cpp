/*
 * Vector3f.cpp
 *
 *  Created on: 2011-04-05
 *      Author: jarrett
 */

#include "Vector3f.h"

#include <math.h>

namespace vmath {

/**
 * Basic constructor.
 */
Vector3f::Vector3f(float32 x, float32 y, float32 z) {
	this->x = x;
	this->y = y;
	this->z = z;

}

/**
 * Copy constructor.
 */
Vector3f::Vector3f(const Vector3f& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}

/**
 *
 */
Vector3f::~Vector3f() {
}

/**
 * Overloaded *= operator.
 */
const Vector3f& Vector3f::operator*=(const float32& f) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

/**
 * Overloaded /= operator.
 */
const Vector3f& Vector3f::operator/=(const float32& f) {
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

/**
 * Overloaded += operator.
 */
const Vector3f& Vector3f::operator+=(const Vector3f& vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;
	return *this;
}

/**
 * Overloaded + operator.
 */
const Vector3f Vector3f::operator+(const Vector3f& vector) const {
	return Vector3f(x + vector.x, y + vector.y, z + vector.z);
}

/**
 * Overloaded - operator.
 */
const Vector3f Vector3f::operator-(const Vector3f& vector) const {
	return Vector3f(x - vector.x, y - vector.y, z - vector.z);
}

/**
 * Overloaded * operator (vector inner product).
 */
const float32 Vector3f::operator*(const Vector3f& vector) const {
	return (x * vector.x + y * vector.y + z * vector.z);
}

void Vector3f::normalize() {
	float32 length = getLength();
	x /= length;
	y /= length;
	z /= length;
}

const float32 Vector3f::getLength() const {
	return sqrt(x * x + y * y + z * z);
}

}
