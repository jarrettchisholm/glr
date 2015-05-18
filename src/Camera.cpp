#include <iostream>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "Camera.hpp"

#include "common/logger/Logger.hpp"

#include "common/math/Math.hpp"

#include "exceptions/Exception.hpp"

namespace glr
{

Camera::Camera(Id id, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, openGlDevice)
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

	orientationQuaternion_ = glm::quat();
	orientationQuaternion_ = glm::normalize(orientationQuaternion_);

	viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	LOG_DEBUG( "Camera initialized." );
}

void Camera::render()
{
	if ( isActive() )
	{
		glm::quat temp = glm::conjugate(orientationQuaternion_);

		viewMatrix_ = glm::mat4_cast(temp);
		viewMatrix_ = glm::translate(viewMatrix_, glm::vec3(-pos_.x, -pos_.y, -pos_.z));
	}
}

const glm::mat4& Camera::getViewMatrix() const
{
	return viewMatrix_;
}

bool Camera::isActive() const
{
	return active_;
}

/**
 * Does nothing in the Camera.
 */
void Camera::attach(models::IModel* model)
{
	std::string msg = std::string("__FILE__(__LINE__): Cannot attach a model to a Camera.");
	LOG_ERROR( msg );
	throw new exception::Exception( msg );
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
	pos_ += orientationQuaternion_ * movementBuffer_;

	clearMovementBuffer();
}

}
