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
	// TODO: implement
	
	return nullptr;
}

}
}
