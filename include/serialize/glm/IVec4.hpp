#ifndef GLMIVEC4SERIALIZE_H_
#define GLMIVEC4SERIALIZE_H_

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::ivec4)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::ivec4& v, unsigned int version)
{
	ar & v.w & v.x & v.y & v.z;
}

template<class Archive> void load(Archive& ar, glm::ivec4& v, unsigned int version)
{
	ar & v.w & v.x & v.y & v.z;
}

}
}

#endif /* GLMIVEC4SERIALIZE_H_ */
