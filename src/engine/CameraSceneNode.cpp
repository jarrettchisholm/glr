/*
 * CameraSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <boost/log/trivial.hpp>

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
	setLookAt( 1, 1, 1 );
	setPosition( 0, 0, 0 );
	setScale(1, 1, 1);
	
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

	moveSpeed_ = 0.05f;
	rotSpeed_ = 18.0f;

	rotation_ = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);
	rotation_ = glm::normalize(rotation_);
	
	viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	BOOST_LOG_TRIVIAL(debug) << "Camera initialized.";
	//rotation_.normalize();
}

void CameraSceneNode::render() {
	if (isActive()) {
		glm::quat temp = glm::conjugate(rotation_);
		viewMatrix_ = glm::mat4_cast( temp );
		viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(-pos_.x, -pos_.y, -pos_.z));
	}
}

glm::mat4& CameraSceneNode::getViewMatrix() {
	return viewMatrix_;
}

bool CameraSceneNode::isActive() {
	return active_;
}

/**
* Does nothing in the CameraSceneNode.
*/ 
void CameraSceneNode::attach(models::IModel* model) {
	
}

/**
* Does nothing in the CameraSceneNode.
*/ 
void CameraSceneNode::setVisible(bool isVisible) {
	
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
