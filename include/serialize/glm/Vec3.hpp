#ifndef GLMVEC3SERIALIZE_H_
#define GLMVEC3SERIALIZE_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::vec3)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::vec3& v, unsigned int version)
{
	ar & v.x & v.y & v.z;
}

template<class Archive> void load(Archive& ar, glm::vec3& v, unsigned int version)
{
	ar & v.x & v.y & v.z;
}

}
}

#endif /* GLMVEC3SERIALIZE_H_ */
