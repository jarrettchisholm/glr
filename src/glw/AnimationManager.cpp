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

Animation* AnimationManager::getAnimation(const std::string filename)
{
	if ( animations_.find(filename) != animations_.end() )
	{
		BOOST_LOG_TRIVIAL(debug) << "Animation found.";
		return animations_[filename].get();
	}

	BOOST_LOG_TRIVIAL(debug) << "Animation not found.";
	
	return nullptr;
}

Animation* AnimationManager::addAnimation(const std::string filename)
{
	// TODO: implement
	
	return nullptr;
}

}
}
