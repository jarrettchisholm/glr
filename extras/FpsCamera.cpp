#include "FpsCamera.hpp"

namespace glr
{
namespace extras
{
	
FpsCamera::FpsCamera(ICamera* camera, glmd::float32 speed) : camera_(camera), speed_(speed)
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
	camera_->move( glm::vec3(0.0f, 0.0f, -speed_) );
}

void FpsCamera::moveBack()
{
	camera_->move( glm::vec3(0.0f, 0.0f, speed_) );
}

void FpsCamera::moveLeft()
{
	camera_->move( glm::vec3(-speed_, 0.0f, 0.0f) );
}

void FpsCamera::moveRight()
{
	camera_->move( glm::vec3(speed_, 0.0f, 0.0f) );
}

ICamera* FpsCamera::getCamera()
{
	return camera_;
}

void FpsCamera::rotate(const glm::detail::float32& degrees, const glm::vec3& axis)
{
	if ( axis == glm::vec3(0.0f, 1.0f, 0.0f) )
		camera_->rotate(degrees, axis, ISceneNode::TransformSpace::TS_LOCAL);
	else
		camera_->rotate(degrees, axis, ISceneNode::TransformSpace::TS_WORLD);
}

void FpsCamera::tick(glm::detail::float32 time)
{
	camera_->tick(time);
}

}
}
