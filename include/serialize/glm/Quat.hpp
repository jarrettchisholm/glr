#ifndef GLMQUATSERIALIZE_H_
#define GLMQUATSERIALIZE_H_

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(glm::quat)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glm::quat& q, unsigned int version);

template<class Archive> void load(Archive& ar, glm::quat& q, unsigned int version);

}
}

#endif /* GLMQUATSERIALIZE_H_ */
