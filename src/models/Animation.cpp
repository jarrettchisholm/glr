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
	
	std::cout << "HERE: " << other.startFrame_ << " " << other.endFrame_ << std::endl;
	runningTime_ = other.runningTime_;
	startFrame_ = other.startFrame_;
	endFrame_ = other.endFrame_;
}

Animation::~Animation()
{
}

void Animation::initialize(glw::Animation* animation)
{
	assert( animation != nullptr );
	
	this->animation_ = animation;
	runningTime_ = 0.0f;
	startFrame_ = 0;
	endFrame_ = 0;
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

glm::detail::uint32 Animation::getStartFrame()
{
	return startFrame_;
}

glm::detail::uint32 Animation::getEndFrame()
{
	return endFrame_;
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

void Animation::setFrameClampping(glm::detail::uint32 startFrame, glm::detail::uint32 endFrame)
{
	startFrame_ = startFrame;
	endFrame_ = endFrame;
	animation_->setFrameClampping( startFrame_, endFrame_ );
}

}
}


