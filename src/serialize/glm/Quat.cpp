#define GLM_FORCE_RADIANS
#include "glm/gtc/quaternion.hpp"

#include "serialize/glm/Quat.hpp"

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
