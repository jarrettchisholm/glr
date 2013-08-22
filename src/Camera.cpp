/*
 * Camera.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include <boost/log/trivial.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Camera.h"

#include "common/math/Math.h"

#include <GL/glew.h>

#include <iostream>

namespace glr {
Camera::Camera(glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Camera::Camera(const std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(name, openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	initialize();
}

Camera::~Camera()
{
}

void Camera::initialize()
{
	clearMovementBuffer();

	xRot_ = 0;
	yRot_ = 0;

	moveSpeed_ = 0.05f;
	rotSpeed_ = 18.0f;

	rotationQuaternion_ = glm::quat(1.0f, 1.0f, 1.0f, 1.0f);
	rotationQuaternion_ = glm::normalize(rotationQuaternion_);
	rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);

	viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	BOOST_LOG_TRIVIAL(debug) << "Camera initialized.";
	//rotationQuaternion_.normalize();
}

void Camera::render()
{
	if ( isActive() )
	{
		glm::quat temp = glm::conjugate(rotationQuaternion_);
		viewMatrix_ = glm::mat4_cast(temp);
		viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(-pos_.x, -pos_.y, -pos_.z));
	}
}

const glm::mat4& Camera::getViewMatrix()
{
	return viewMatrix_;
}

bool Camera::isActive()
{
	return active_;
}

/**
 * Does nothing in the Camera.
 */
void Camera::attach(models::IModel* model)
{
}

/**
 *
 */
void Camera::clearMovementBuffer()
{
	movementBuffer_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

/**
 *
 */
void Camera::move(const glm::vec3& moveVector)
{
	movementBuffer_ += moveVector;
}

/**
 *
 */
void Camera::rotate(const glm::vec3& radians)
{
	rotation_ += radians;
}

/**
 *
 */
void Camera::tick(glm::detail::float32 time)
{
	//pos_ += rotationQuaternion_ * movementBuffer_ * time;
	pos_ += rotationQuaternion_ * movementBuffer_;

	// rotation
	glm::quat pitch = glm::angleAxis(rotation_.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat heading = glm::angleAxis(rotation_.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat other = glm::angleAxis(rotation_.z, glm::vec3(0.0f, 0.0f, 1.0f));

	rotationQuaternion_ = glm::normalize(heading * pitch * other);
	
	clearMovementBuffer();
}
}
