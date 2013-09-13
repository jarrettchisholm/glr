/*
 * Animation.cpp
 *
 *  Created on: 2013-09-13
 *      Author: jarrett
 */

#include "Animation.h"

namespace glr {
namespace models {

Animation::Animation(glw::Animation* animation, glw::IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	initialize(animation);
}

/**
 * Copy constructor.
 */
Animation::Animation(const Animation& other)
{
	animation_ = other.animation_;
	openGlDevice_ = other.openGlDevice_;
}

Animation::~Animation()
{
}

void Animation::initialize(glw::Animation* animation)
{
	assert( animation != nullptr );
	
	this->animation_ = animation;
}

void Animation::destroy()
{
}

glw::Animation* Animation::getAnimation()
{
	return animation_;
}

glm::detail::float32  Animation::getAnimationTime()
{
	return runningTime_;
}

const std::string Animation::getName()
{
	return animation_->getName();
}

void Animation::setAnimationTime(glm::detail::float32 runningTime)
{
	runningTime_ = runningTime;
	animation_->setAnimationTime( runningTime );
}

}
}


