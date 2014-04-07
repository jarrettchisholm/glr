#ifndef GLMMAT4SERIALIZE_H_
#define GLMMAT4SERIALIZE_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::mat4)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::mat4& m, unsigned int version);

template<class Archive> void load(Archive& ar, glm::mat4& m, unsigned int version);

}
}

#endif /* GLMMAT4SERIALIZE_H_ */
