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
	/**
	 * Basic constructor.  The creator of the Mesh will have to set the data, and then call
	 * 'allocateVideoMemory()' and 'pushToVideoMemory()' before this mesh will render properly.
	 */
	Mesh(IOpenGlDevice* openGlDevice, const std::string& name);
	
	/**
	 * Standard constructor.  Once called, the Mesh will be ready for rendering (with no further action
	 * required by the creator).
	 */
	Mesh(IOpenGlDevice* openGlDevice,
		const std::string& name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< VertexBoneData > vertexBoneData,
		BoneData boneData
	);
	
	/**
	 * Standard constructor.  Once called, the Mesh will be ready for rendering (with no further action
	 * required by the creator).  This type of Mesh does not have any bone data set by default.
	 */
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
	
	const std::string& getName() const;
	void setName(const std::string& name);
	
	void setVertices(std::vector< glm::vec3 > vertices);
	void setNormals(std::vector< glm::vec3 > normals);
	void setTextureCoordinates(std::vector< glm::vec2 > textureCoordinates);
	void setColors(std::vector< glm::vec4 > colors);
	void setVertexBoneData(std::vector< VertexBoneData > vertexBoneData);
	
	std::vector< glm::vec3 >& getVertices();
	std::vector< glm::vec3 >& getNormals();
	std::vector< glm::vec2 >& getTextureCoordinates();
	std::vector< glm::vec4 >& getColors();
	std::vector< VertexBoneData >& getVertexBoneData();
	
protected:
	std::string name_;
	IOpenGlDevice* openGlDevice_;
	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboIds_[5];

	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	std::vector< VertexBoneData > vertexBoneData_;

	BoneData boneData_;

	std::string textureFileName_;
};

}
}
#endif /* MESH_H_ */
