#ifndef GLMQUATSERIALIZE_H_
#define GLMQUATSERIALIZE_H_

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

BOOST_SERIALIZATION_SPLIT_FREE(glm::quat)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::quat& q, unsigned int version)
{
	ar & q.x & q.y & q.z & q.w;
}

template<class Archive> void load(Archive& ar, glm::quat& q, unsigned int version)
{
	ar & q.x & q.y & q.z & q.w;
}

}
}

#endif /* GLMQUATSERIALIZE_H_ */
