#ifndef ANIMATEDBONENODE_H_
#define ANIMATEDBONENODE_H_

#include <string>
#include <vector>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

namespace glr
{
namespace glw
{

class AnimatedBoneNode
{
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
	) : name_(std::move(name)), positionTimes_(std::move(positionTimes)), rotationTimes_(std::move(rotationTimes)), scalingTimes_(std::move(scalingTimes)), positions_(std::move(positions)), rotations_(std::move(rotations)), scalings_(std::move(scalings))
	{}
	virtual ~AnimatedBoneNode()
	{}

// TODO: make these private
public:
	std::string name_;
	std::vector< glm::detail::float64 > positionTimes_;
	std::vector< glm::detail::float64 > rotationTimes_;
	std::vector< glm::detail::float64 > scalingTimes_;
	std::vector< glm::vec3 > positions_;
	std::vector< glm::quat > rotations_;
	std::vector< glm::vec3 > scalings_;
};

}
}

#endif /* ANIMATEDBONENODE_H_ */
