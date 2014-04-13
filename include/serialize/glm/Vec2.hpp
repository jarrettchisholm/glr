#ifndef GLMVEC2SERIALIZE_H_
#define GLMVEC2SERIALIZE_H_

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::vec2)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::vec2& v, unsigned int version)
{
	ar & v.x & v.y;
}

template<class Archive> void load(Archive& ar, glm::vec2& v, unsigned int version)
{
	ar & v.x & v.y;
}

}
}

#endif /* GLMVEC2SERIALIZE_H_ */
