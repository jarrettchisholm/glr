#ifndef BONE_H_
#define BONE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/glm/Mat4.hpp"

namespace glr
{
namespace glw
{

struct Bone
{
	std::string name;
	glm::mat4 boneOffset;
};

}
}


BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::Bone)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::Bone& b, unsigned int version)
{
	ar & b.name & b.boneOffset;
}

template<class Archive> void load(Archive& ar, glr::glw::Bone& b, unsigned int version)
{
	ar & b.name & b.boneOffset;
}

}
}

#endif /* BONE_H_ */
