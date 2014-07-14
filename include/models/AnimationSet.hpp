#ifndef ANIMATIONSET_H_
#define ANIMATIONSET_H_

#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "glw/BoneNode.hpp"
#include "models/AnimationData.hpp"

namespace glr
{
namespace models
{

struct AnimationSet
{
	std::string name;
	
	glw::BoneNode rootBoneNode;
	std::map< std::string, AnimationData > animations;
	glm::mat4 globalInverseTransformation;
};

}
}

#endif /* ANIMATIONSET_H_ */
