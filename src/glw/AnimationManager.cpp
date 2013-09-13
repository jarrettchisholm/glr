/*
 * AnimationManager.cpp
 *
 *  Created on: 2013-05-13
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "AnimationManager.h"


namespace glr {
namespace glw {
	
AnimationManager::AnimationManager(IOpenGlDevice* openGlDevice) : openGlDevice_(openGlDevice)
{
	// Create and add GLR_IDENTITY_BONES animation
	
}

AnimationManager::~AnimationManager()
{
}

Animation* AnimationManager::getAnimation(const std::string name)
{
	if ( animations_.find(name) != animations_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Animation found.";
		return animations_[name].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Animation not found.";
	
	return nullptr;
}

Animation* AnimationManager::addAnimation(const std::string name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes)
{
	BOOST_LOG_TRIVIAL(debug) << "Loading animation...";

	if ( animations_.find(name) != animations_.end() && animations_[name].get() != nullptr )
	{
		BOOST_LOG_TRIVIAL(debug) << "Animation already exists.";
		return animations_[name].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Creating Animation.";
	animations_[name] = std::unique_ptr<Animation>(new Animation(openGlDevice_, name, duration, ticksPerSecond, animatedBoneNodes));

	return animations_[name].get();
}

}
}
