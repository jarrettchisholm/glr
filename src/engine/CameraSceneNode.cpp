/*
 * CameraSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "CameraSceneNode.h"

#include "../common/math/Math.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>

#include <iostream>

namespace oglre {

namespace engine {

CameraSceneNode::CameraSceneNode() {
	setLookAt(vmath::Vector3f(1, 1, 1));
	setPosition(vmath::Vector3f(0, 0, 0));
	active_ = true;

	initialize();
}

CameraSceneNode::CameraSceneNode(vmath::Vector3f position, vmath::Vector3f lookAt, bool active) {
	setPosition(position);
	setLookAt(lookAt);
	active_ = active;

	initialize();
}

CameraSceneNode::~CameraSceneNode() {
}

void CameraSceneNode::initialize() {
	clearMovementBuffer();

	xRot_ = 0;
	yRot_ = 0;

	moveSpeed_ = 0.20f;
	rotSpeed_ = 1.0f;

	rotation_ = Quaternion();//(1, 1, 1, 1);
	rotation_.normalize();
}

void CameraSceneNode::render() {
	if (isActive()) {
		float32 matrix[16];
		rotation_.getConjugate().fillMatrix(matrix);

		glMultMatrixf(&matrix[0]);
		
		glTranslatef(-pos_.x, -pos_.y, -pos_.z);
	}
}

const vmath::Vector3f& CameraSceneNode::getLookAt() {
	return lookAt_;
}

void CameraSceneNode::setLookAt(const vmath::Vector3f& newLookAt) {
	lookAt_ = newLookAt;
}

/**
 *
 */
void CameraSceneNode::clearMovementBuffer() {
	for (uint32 i = 0; i < NUM_MOVE_DIRECTIONS; i++)
		movement_[i] = 0;
}

/**
 *
 */
void CameraSceneNode::move(MOVE_DIRECTION dir, bool enabled) {
	movement_[dir] = (enabled ? 1 : 0);
}

// up/down
/**
 *
 */
void CameraSceneNode::rotateX(float32 degrees) {
	xRot_ += degrees;
}

// left/right
/**
 *
 */
void CameraSceneNode::rotateY(float32 degrees) {
	yRot_ += degrees;
}

/**
 *
 */
void CameraSceneNode::tick(float32 time) {
	// movement direction
	if (movement_[MOVE_DIR_FORWARD] == 1)
		pos_ += rotation_ * vmath::Vector3f(0, 0, -moveSpeed_ * time);

	if (movement_[MOVE_DIR_BACKWARD] == 1)
		pos_ += rotation_ * vmath::Vector3f(0, 0, moveSpeed_ * time);

	if (movement_[MOVE_DIR_LEFT] == 1)
		pos_ += rotation_ * vmath::Vector3f(-moveSpeed_ * time, 0, 0);

	if (movement_[MOVE_DIR_RIGHT] == 1)
		pos_ += rotation_ * vmath::Vector3f(moveSpeed_ * time, 0, 0);

	// rotation
	Quaternion pitch;
	pitch.buildFromAxisAngle(1, 0, 0, (xRot_ * time * rotSpeed_) * math::DEGTORAD);

	Quaternion heading;
	heading.buildFromAxisAngle(0, 1, 0, (yRot_ * time * rotSpeed_) * math::DEGTORAD);

	rotation_ = heading * pitch;
}

}

}
