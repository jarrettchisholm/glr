#ifndef MESHDATA_H_
#define MESHDATA_H_

#include <vector>
#include <string>

#include "glw/VertexBoneData.hpp"

namespace glr
{
namespace models
{

struct MeshData
{
	std::string name;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > textureCoordinates;
	std::vector< glm::vec4 > colors;
	std::vector< glw::VertexBoneData > bones;
};

}
}

#endif /* MESHDATA_H_ */
