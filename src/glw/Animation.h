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

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

namespace glr {
namespace glw {
	
/*
struct BoneNode {
	std::string name;
	glm::mat4 transformation;
	std::vector< BoneNode > children;
};
*/

class AnimatedBoneNode {
public:
	AnimatedBoneNode()
	{}
	AnimatedBoneNode(
		std::string name,
		std::vector< glm::detail::float64 > positionTimes,
		std::vector< glm::detail::float64 > rotationTimes,
		std::vector< glm::detail::float64 > scalingTimes,
		std::vector< glm::vec3 > positions,
		std::vector< glm::quat > rotations,
		std::vector< glm::vec3 > scalings
	) : name_(name), positionTimes_(positionTimes), rotationTimes_(rotationTimes), scalingTimes_(scalingTimes), positions_(positions), rotations_(rotations), scalings_(scalings)
	{}
	virtual ~AnimatedBoneNode()
	{}

private:
	std::string name_;
	std::vector< glm::detail::float64 > positionTimes_;
	std::vector< glm::detail::float64 > rotationTimes_;
	std::vector< glm::detail::float64 > scalingTimes_;
	std::vector< glm::vec3 > positions_;
	std::vector< glm::quat > rotations_;
	std::vector< glm::vec3 > scalings_;
};

	
class Animation {
public:
	Animation(const std::string path, 
			std::vector< glm::vec3 > vertices);
	virtual ~Animation();

private:
	std::string name_;
	glm::detail::float64 duration_;
	glm::detail::float64 ticksPerSecond_;

	std::map< std::string, AnimatedBoneNode > animatedBoneNodes_;

};

}
}
#endif /* ANIMATION_H_ */
