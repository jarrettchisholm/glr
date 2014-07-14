#ifndef MATERIALDATA_H_
#define MATERIALDATA_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "glw/VertexBoneData.hpp"

namespace glr
{
namespace models
{

struct MaterialData
{
	std::string name;
	GLenum fill_mode;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	glm::detail::float32 shininess;
	glm::detail::float32 strength;
};

}
}

#endif /* MATERIALDATA_H_ */
