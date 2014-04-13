#ifndef ANIMATEDBONENODE_H_
#define ANIMATEDBONENODE_H_

#include <string>
#include <vector>
#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/glm/Vec3.hpp"
#include "serialize/glm/Quat.hpp"

namespace glr
{
namespace glw
{

struct AnimatedBoneNode
{
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
	) : name(std::move(name)), positionTimes(std::move(positionTimes)), rotationTimes(std::move(rotationTimes)), scalingTimes(std::move(scalingTimes)), positions(std::move(positions)), rotations(std::move(rotations)), scalings(std::move(scalings))
	{}

	std::string name;
	std::vector< glm::detail::float64 > positionTimes;
	std::vector< glm::detail::float64 > rotationTimes;
	std::vector< glm::detail::float64 > scalingTimes;
	std::vector< glm::vec3 > positions;
	std::vector< glm::quat > rotations;
	std::vector< glm::vec3 > scalings;
};

}
}

BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::AnimatedBoneNode)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::AnimatedBoneNode& abn, unsigned int version)
{
	ar & abn.name & abn.positionTimes & abn.rotationTimes & abn.scalingTimes;
	ar & abn.positions & abn.rotations & abn.scalings;
}

template<class Archive> void load(Archive& ar, glr::glw::AnimatedBoneNode& abn, unsigned int version)
{
	ar & abn.name & abn.positionTimes & abn.rotationTimes & abn.scalingTimes;
	ar & abn.positions & abn.rotations & abn.scalings;
}

}
}

#endif /* ANIMATEDBONENODE_H_ */
