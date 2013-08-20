/*
 * Mesh.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"

//#include "../../vmath/Vector3f.h"


namespace glr {
namespace glw {
	
class IOpenGlDevice;
	
class Mesh {
public:
	Mesh(IOpenGlDevice* openGlDevice,
		const std::string path, 
		std::vector< glm::vec3 > vertices, 
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec2 > bones);
	virtual ~Mesh();

	void render();

private:
	IOpenGlDevice* openGlDevice_;
	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboIds_[4];

	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	std::vector< glm::vec2 > bones_;

	std::string textureFileName_;
};
}
}
#endif /* MESH_H_ */
