/*
 * Quaternion.h
 *
 *  Created on: 2011-05-12
 *      Author: jarrett
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "../vmath/Vector3f.h"
#include "../common/math/Math.h"
#include <math.h>

namespace oglre {

namespace engine {

using namespace compatibility;

class Quaternion {

private:
	float32 real_;
	vmath::Vector3f imaginary_;

public:
	/**
	 * Constructor.
	 */
	Quaternion() {
		real_ = 1;
		imaginary_ = vmath::Vector3f(0, 0, 0);
	}

	/**
	 * Constructor.
	 */
	Quaternion(float32 x, float32 y, float32 z, float32 real) {
		real_ = real;
		imaginary_ = vmath::Vector3f(x, y, z);
	}

	/**
	 * Constructor.
	 */
	Quaternion(const vmath::Vector3f& imaginary, float32 real) {
		real_ = real;
		imaginary_ = imaginary;
	}

	/**
	 * Build this Quaternion from an axis angle.  This is useful if you want to use the Quaternion
	 * to rotate through an angle about the axis (unit) vector.  The equation for this is:
	 * q = cos(theta/2) + vector*sin(theta/2)
	 *
	 * @param x
	 * @param y
	 * @param z
	 * @param angle The angle, in radians
	 */
	void buildFromAxisAngle(float32 x, float32 y, float32 z, float32 angle) {
		// convert from degrees to radians
		//float32 angle = ((degrees / 180.0f) * math::PI);

		float32 sinAngle;
		angle *= 0.5f;
		//vmath::Vector3f vector(vec);
		//vector.normalize();

		sinAngle = sin(angle);

		imaginary_.x = (x * sinAngle);
		imaginary_.y = (y * sinAngle);
		imaginary_.z = (z * sinAngle);
		real_ = cos(angle);
	}

	/*
	 // Convert from Euler Angles
	 Quaternion(float32 pitch, float32 yaw, float32 roll) {
	 // Basically we create 3 Quaternions, one for pitch, one for yaw, one for roll
	 // and multiply those together.
	 // the calculation below does the same, just shorter

	 float p = pitch * PIOVER180 / 2.0;
	 float y = yaw * PIOVER180 / 2.0;
	 float r = roll * PIOVER180 / 2.0;

	 float sinp = sin(p);
	 float siny = sin(y);
	 float sinr = sin(r);
	 float cosp = cos(p);
	 float cosy = cos(y);
	 float cosr = cos(r);

	 this->x = sinr * cosp * cosy - cosr * sinp * siny;
	 this->y = cosr * sinp * cosy + sinr * cosp * siny;
	 this->z = cosr * cosp * siny - sinr * sinp * cosy;
	 this->w = cosr * cosp * cosy + sinr * sinp * siny;

	 normalise();
	 }
	 */

	/**
	 * Copy constructor.
	 */
	Quaternion(const Quaternion &quat) {
		real_ = quat.real_;
		imaginary_ = quat.imaginary_;

	}

	/**
	 * Destructor.
	 */
	virtual ~Quaternion() {

	}

	const Quaternion& operator =(const Quaternion &quat) {
		real_ = quat.real_;
		imaginary_ = quat.imaginary_;

		return *this;
	}

	const Quaternion operator +(const Quaternion &quat) const {
		return Quaternion(imaginary_ + quat.imaginary_, real_ + quat.real_);
	}

	const Quaternion operator -(const Quaternion &quat) const {
		return Quaternion(imaginary_ - quat.imaginary_, real_ - quat.real_);
	}

	const Quaternion operator *(const Quaternion &quat) const {
		return Quaternion(
				imaginary_.y * quat.imaginary_.z - imaginary_.z * quat.imaginary_.y + real_
						* quat.imaginary_.x + imaginary_.x * quat.real_,
				imaginary_.z * quat.imaginary_.x - imaginary_.x * quat.imaginary_.z + real_
						* quat.imaginary_.y + imaginary_.y * quat.real_,
				imaginary_.x * quat.imaginary_.y - imaginary_.y * quat.imaginary_.x + real_
						* quat.imaginary_.z + imaginary_.z * quat.real_,
				real_ * quat.real_ - imaginary_ * quat.imaginary_);
	}

	// Multiplying a quaternion q with a vector v applies the q-rotation to v
	const vmath::Vector3f operator*(const vmath::Vector3f &vec) {
		vmath::Vector3f vector(vec);
		vector.normalize();

		Quaternion vectorQuat, resultQuat;
		vectorQuat.imaginary_.x = vector.x;
		vectorQuat.imaginary_.y = vector.y;
		vectorQuat.imaginary_.z = vector.z;
		vectorQuat.real_ = 0.0f;

		resultQuat = vectorQuat * getConjugate();
		resultQuat = *this * resultQuat;

		return (vmath::Vector3f(resultQuat.imaginary_.x, resultQuat.imaginary_.y,
				resultQuat.imaginary_.z));
	}

	const Quaternion operator /(const Quaternion &quat) const {
		Quaternion retQuat(quat);
		retQuat.invert();

		return *this * retQuat;
	}

	const Quaternion& operator /=(float32 scale) {
		real_ /= scale;
		imaginary_ /= scale;

		return *this;
	}

	void invert() {
		conjugate();
		*this /= lengthSquared();
	}

	void conjugate() {
		imaginary_ *= (-1.f);
	}

	Quaternion getConjugate() {
		return Quaternion(-imaginary_.x, -imaginary_.y, -imaginary_.z, real_);
	}

	float32 length() const {
		return (float32) (sqrt(real_ * real_ + imaginary_ * imaginary_));
	}

	/**
	 * Get the squared length of this quaternion.
	 */
	float32 lengthSquared() {
		return (float32) (real_ * real_ + imaginary_ * imaginary_);
	}

	void normalize() {
		static float32 TOLERANCE = 0.00001f;
		float32 mag2 = lengthSquared();
		if (mag2 != 0.f && (fabs(mag2 - 1.0f) > TOLERANCE)) {
			float32 mag = sqrt(mag2);
			*this /= mag;
		}

		//*this /= length();
	}

	/**
	 *
	 * Fills in Column-Major order.
	 */
	void fillMatrix(float32 matrix[]) {
		// First col
		matrix[0] = 1.0f - 2.0f * (imaginary_.y * imaginary_.y + imaginary_.z * imaginary_.z);
		matrix[1] = 2.0f * (imaginary_.x * imaginary_.y + imaginary_.z * real_);
		matrix[2] = 2.0f * (imaginary_.x * imaginary_.z - imaginary_.y * real_);
		matrix[3] = 0.0f;

		// Second col
		matrix[4] = 2.0f * (imaginary_.x * imaginary_.y - imaginary_.z * real_);
		matrix[5] = 1.0f - 2.0f * (imaginary_.x * imaginary_.x + imaginary_.z * imaginary_.z);
		matrix[6] = 2.0f * (imaginary_.z * imaginary_.y + imaginary_.x * real_);
		matrix[7] = 0.0f;

		// Third col
		matrix[8] = 2.0f * (imaginary_.x * imaginary_.z + imaginary_.y * real_);
		matrix[9] = 2.0f * (imaginary_.y * imaginary_.z - imaginary_.x * real_);
		matrix[10] = 1.0f - 2.0f * (imaginary_.x * imaginary_.x + imaginary_.y * imaginary_.y);
		matrix[11] = 0.0f;

		// Fourth col
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1.0f;
	}

	/*
	 // Convert to Matrix
	 Matrix4 getMatrix() const {
	 float x2 = x * x;
	 float y2 = y * y;
	 float z2 = z * z;
	 float xy = x * y;
	 float xz = x * z;
	 float yz = y * z;
	 float wx = w * x;
	 float wy = w * y;
	 float wz = w * z;

	 // This calculation would be a lot more complicated for non-unit length quaternions
	 // Note: The constructor of Matrix4 expects the Matrix in column-major format like expected by
	 //   OpenGL
	 return	Matrix4( 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
	 2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
	 2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
	 0.0f, 0.0f, 0.0f, 1.0f)
	 }
	 */

	/*
	 // Convert to Axis/Angles
	 void getAxisAngle(Vector3* axis, float* angle) {
	 float scale = sqrt(x * x + y * y + z * z);
	 axis->x = x / scale;
	 axis->y = y / scale;
	 axis->z = z / scale;
	 *angle = acos(w) * 2.0f;
	 }
	 */

};

}

}

#endif /* QUATERNION_H_ */
