#define GLM_FORCE_RADIANS
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Camera.h"

#include "common/logger/Logger.h"

#include "common/math/Math.h"

#include <GL/glew.h>

#include <iostream>

namespace glr
{

Camera::Camera(glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(openGlDevice)
{
	setPosition(0.0f, 0.0f, 0.0f);
	setScale(1.0f, 1.0f, 1.0f);

	initialize();
}

Camera::~Camera()
{
}

void Camera::initialize()
{
	clearMovementBuffer();

	xRot_ = 0.0f;
	yRot_ = 0.0f;

	moveSpeed_ = 0.05f;
	rotSpeed_ = 18.0f;

	orientationQuaternion_ = glm::quat(); //glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	orientationQuaternion_ = glm::normalize(orientationQuaternion_);

	//orientationQuaternion_ = glm::normalize( orientationQuaternion_ * glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)) );
	//rotation_ = glm::vec3(0.0f, 0.0f, 0.0f);

	viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	LOG_DEBUG( "Camera initialized." );
	//orientationQuaternion_.normalize();
}

void Camera::render()
{
	if ( isActive() )
	{
		glm::quat temp = glm::conjugate(orientationQuaternion_);
		//viewMatrix_ = glm::lookAt(pos_, pos_ + direction_, glm::vec3(0.0f, 1.0f, 0.0f));
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
void Camera::tick(glm::detail::float32 time)
{
	//pos_ += orientationQuaternion_ * movementBuffer_ * time;
	pos_ += orientationQuaternion_ * movementBuffer_;

	// rotation
	//glm::quat pitch = glm::angleAxis(glm::radians(rotation_.x), glm::vec3(1.0f, 0.0f, 0.0f));
	//glm::quat heading = glm::angleAxis(glm::radians(rotation_.y), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::quat other = glm::angleAxis(glm::radians(rotation_.z), glm::vec3(0.0f, 0.0f, 1.0f));
	
	//std::cout << glm::to_string(rotation_) << " | " << glm::to_string(glm::eulerAngles(pitch)) << " | " << glm::to_string(glm::eulerAngles(heading)) << " | " << glm::to_string(glm::eulerAngles(other)) << std::endl;

	//orientationQuaternion_ = glm::normalize(heading * pitch * other);
	
	clearMovementBuffer();
}

}
