#ifndef ANIMATIONDATA_H_
#define ANIMATIONDATA_H_

#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "glw/AnimatedBoneNode.hpp"

namespace glr
{
namespace models
{

struct AnimationData
{
	std::string name;
	glm::detail::float64 duration;
	glm::detail::float64 ticksPerSecond;
	
	std::map< std::string, glw::AnimatedBoneNode > animatedBoneNodes;
};

}
}

#endif /* ANIMATIONDATA_H_ */
