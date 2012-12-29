/*
 * CameraSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "glm/gtc/type_ptr.hpp"

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
	setLookAt(glm::vec3(1, 1, 1));
	setPosition(glm::vec3(0, 0, 0));
	active_ = true;

	initialize();
}

CameraSceneNode::CameraSceneNode(glm::vec3 position, glm::vec3 lookAt, bool active) {
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

	moveSpeed_ = 1.20f;
	rotSpeed_ = 19.0f;

	rotation_ = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);
	rotation_ = glm::normalize(rotation_);
	//rotation_.normalize();
}

void CameraSceneNode::render() {
	if (isActive()) {
		//glm::detail::float32 matrix[16];
		glm::quat temp = glm::conjugate(rotation_);
		glm::mat4x4 matrix = glm::mat4_cast( temp );
		
		//rotation_.getConjugate().fillMatrix(matrix);

		glMultMatrixf( glm::value_ptr(matrix) );
		
		glTranslatef(-pos_.x, -pos_.y, -pos_.z);
	}
}

glm::vec3 CameraSceneNode::getLookAt() {
	return lookAt_;
}

void CameraSceneNode::setLookAt(glm::vec3 newLookAt) {
	lookAt_ = newLookAt;
}

/**
 *
 */
void CameraSceneNode::clearMovementBuffer() {
	for (glm::detail::uint32 i = 0; i < NUM_MOVE_DIRECTIONS; i++)
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
void CameraSceneNode::rotateX(glm::detail::float32 degrees) {
	xRot_ += degrees;
}

// left/right
/**
 *
 */
void CameraSceneNode::rotateY(glm::detail::float32 degrees) {
	yRot_ += degrees;
}

/**
 *
 */
void CameraSceneNode::tick(glm::detail::float32 time) {
	// movement direction
	if (movement_[MOVE_DIR_FORWARD] == 1)
		pos_ += rotation_ * glm::vec3(0, 0, -moveSpeed_ * time);

	if (movement_[MOVE_DIR_BACKWARD] == 1)
		pos_ += rotation_ * glm::vec3(0, 0, moveSpeed_ * time);

	if (movement_[MOVE_DIR_LEFT] == 1)
		pos_ += rotation_ * glm::vec3(-moveSpeed_ * time, 0, 0);

	if (movement_[MOVE_DIR_RIGHT] == 1)
		pos_ += rotation_ * glm::vec3(moveSpeed_ * time, 0, 0);

	// rotation
	glm::quat pitch = glm::angleAxis((xRot_ * time * rotSpeed_) * math::DEGTORAD, 1.0f, 0.0f, 0.0f);
	//pitch.buildFromAxisAngle(1, 0, 0, (xRot_ * time * rotSpeed_) * math::DEGTORAD);

	glm::quat heading = glm::angleAxis((yRot_ * time * rotSpeed_) * math::DEGTORAD, 0.0f, 1.0f, 0.0f);
	//heading.buildFromAxisAngle(0, 1, 0, (yRot_ * time * rotSpeed_) * math::DEGTORAD);

	rotation_ = heading * pitch;
}

}

}
