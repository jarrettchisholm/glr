/*
 * Animation.h
 *
 *  Created on: 2012-05-08
 *      Author: jarrett
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>
#include <map>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "AnimatedBoneNode.h"

#include "IOpenGlDevice.h"

namespace glr {
namespace glw {
	
class Animation {
public:
	Animation(IOpenGlDevice* openGlDevice, const std::string name, glm::detail::float64 duration, glm::detail::float64 ticksPerSecond, std::map< std::string, AnimatedBoneNode > animatedBoneNodes);
	virtual ~Animation();

private:
	std::string name_;
	glm::detail::float64 duration_;
	glm::detail::float64 ticksPerSecond_;

	std::map< std::string, AnimatedBoneNode > animatedBoneNodes_;
	
	IOpenGlDevice* openGlDevice_;

};

}
}
#endif /* ANIMATION_H_ */
