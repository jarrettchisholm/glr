/*
 * Mesh.h
 * 
 * A class that contains mesh data, and can load this data into OpenGL, as well as render that data
 * once it has been transferred.
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

#include "IMesh.h"

#include "shaders/IShaderProgram.h"
#include "IOpenGlDevice.h"


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
		std::vector< VertexBoneData > bones,
		BoneData boneData);
	virtual ~Mesh();

	void render();
	
	BoneData& getBoneData();

private:
	IOpenGlDevice* openGlDevice_;
	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboIds_[5];

	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	std::vector< VertexBoneData > bones_;

	BoneData boneData_;

	std::string textureFileName_;
};

}
}
#endif /* MESH_H_ */
