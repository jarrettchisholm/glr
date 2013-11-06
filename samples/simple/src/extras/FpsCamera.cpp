/*
 * FpsCamera.cpp
 *
 *  Created on: 2013-06-19
 *      Author: jarrett
 */

#include "FpsCamera.h"

namespace glr {
namespace extras {
	
FpsCamera::FpsCamera(const std::string name, glw::IOpenGlDevice* openGlDevice, glmd::float32 speed) : Camera(name, openGlDevice), speed_(speed)
{
	initialize();
}

FpsCamera::~FpsCamera()
{
}

void FpsCamera::initialize()
{

}

bool FpsCamera::isActive()
{
	return true;
}

void FpsCamera::moveForward()
{
	this->move( glm::vec3(0.0f, 0.0f, -speed_) );
}

void FpsCamera::moveBack()
{
	this->move( glm::vec3(0.0f, 0.0f, speed_) );
}

void FpsCamera::moveLeft()
{
	this->move( glm::vec3(-speed_, 0.0f, 0.0f) );
}

void FpsCamera::moveRight()
{
	this->move( glm::vec3(speed_, 0.0f, 0.0f) );
}

void FpsCamera::rotate(const glm::detail::float32& degrees, const glm::vec3& axis)
{
	if ( axis == glm::vec3(0.0f, 1.0f, 0.0f) )
		Camera::rotate(degrees, axis, TransformSpace::TS_LOCAL);
	else
		Camera::rotate(degrees, axis, TransformSpace::TS_WORLD);
}

/**
 *
 */
void FpsCamera::tick(glm::detail::float32 time)
{
	Camera::tick(time);
}

}
}
