#ifndef MATERIALDATA_H_
#define MATERIALDATA_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "serialize/Serialization.hpp"
#include "serialize/std/Vector.hpp"
#include "serialize/glm/Vec4.hpp"

namespace glr
{
namespace glw
{

struct MaterialData
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;	
	glm::vec4 emission;
	//glm::detail::float32 shininess;
	//glm::detail::float32 strength;
};

}
}

BOOST_SERIALIZATION_SPLIT_FREE(glr::glw::MaterialData)

namespace boost
{
namespace serialization
{

template<class Archive> void save(Archive& ar, const glr::glw::MaterialData& md, unsigned int version)
{
	ar & md.ambient & md.diffuse & md.specular & md.emission;
}

template<class Archive> void load(Archive& ar, glr::glw::MaterialData& md, unsigned int version)
{
	ar & md.ambient & md.diffuse & md.specular & md.emission;
}

}
}

#endif /* MATERIALDATA_H_ */
