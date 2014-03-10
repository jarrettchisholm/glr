#ifndef IMESHMANAGER_H_
#define IMESHMANAGER_H_

#include <string>

#include "Mesh.hpp"

namespace glr
{
namespace glw
{

class Mesh;

class IMeshManager
{
public:
	virtual ~IMeshManager()
	{
	}
	;

	virtual Mesh* getMesh(const std::string& name) const = 0;
	virtual Mesh* addMesh(const std::string& name) = 0;
	virtual Mesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData
	) = 0;
	
	virtual Mesh* addMesh(
		const std::string& name, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	) = 0;
};

}
}

#endif /* IMESHMANAGER_H_ */
