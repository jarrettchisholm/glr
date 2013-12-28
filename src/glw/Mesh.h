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

#include "../common/utilities/Macros.h"


namespace glr {
namespace glw {
	
class IOpenGlDevice;
	
class Mesh : public IMesh {
public:
	Mesh(IOpenGlDevice* openGlDevice, const std::string& name);
	Mesh(IOpenGlDevice* openGlDevice,
		const std::string& name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > bones,
		BoneData boneData
	);
	Mesh(IOpenGlDevice* openGlDevice,
		const std::string& name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	);
	virtual ~Mesh();

	virtual void render();
	
	virtual BoneData& getBoneData();
	
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeLocalData();
	virtual void freeVideoMemory();
	virtual void allocateVideoMemory();
	
	GETSET(std::string, name_, Name)
protected:
	std::string name_;
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
