/*
 * Animation.cpp
 *
 *  Created on: 2012-05-08
 *      Author: jarrett
 */

#include "Animation.h"

namespace glr {
namespace glw {
	
Animation::Animation(
		IOpenGlDevice* openGlDevice,
		const std::string name, 
		glm::detail::float64 duration, 
		glm::detail::float64 ticksPerSecond, 
		std::map< std::string, AnimatedBoneNode > animatedBoneNodes
	) : openGlDevice_(openGlDevice), name_(name), duration_(duration), ticksPerSecond_(ticksPerSecond), animatedBoneNodes_(animatedBoneNodes)
{
	
}

Animation::~Animation()
{
	
}

}
}

