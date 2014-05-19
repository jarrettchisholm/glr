#ifndef MESH_H_
#define MESH_H_

#include <vector>

#include <GL/glew.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "IMesh.hpp"

#include "shaders/IShaderProgram.hpp"
#include "IOpenGlDevice.hpp"

namespace glr
{
namespace glw
{
	
class IOpenGlDevice;

/**
 * A class that contains mesh data, and can load this data into OpenGL, as well as render that data
 * once it has been transferred.
 */
class Mesh : public IMesh
{
public:
	/**
	 * Required by serialization.
	 * 
	 * Made public for testing!
	 */
	Mesh();

	/**
	 * Basic constructor.  The creator of the Mesh will have to set the data, and then call
	 * 'allocateVideoMemory()' and 'pushToVideoMemory()' before this mesh will render properly.
	 */
	Mesh(IOpenGlDevice* openGlDevice, std::string name);
	
	/**
	 * Standard constructor.  Once called, the Mesh will be ready for rendering (with no further action
	 * required by the creator).
	 */
	Mesh(IOpenGlDevice* openGlDevice,
		std::string name,
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
		std::string name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors
	);
	virtual ~Mesh();

	virtual void render();
	
	virtual BoneData& getBoneData();
	
	virtual void allocateVideoMemory();
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeVideoMemory();
	virtual void loadLocalData();
	virtual void freeLocalData();
	
	virtual const std::string& getName() const;
	void setName(std::string name);
	
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
	
	virtual void serialize(const std::string& filename);
	virtual void serialize(serialize::TextOutArchive& outArchive);

	virtual void deserialize(const std::string& filename);
	virtual void deserialize(serialize::TextInArchive& inArchive);
	
protected:
	IOpenGlDevice* openGlDevice_;
	std::string name_;
	std::vector< glm::vec3 > vertices_;
	std::vector< glm::vec3 > normals_;
	std::vector< glm::vec2 > textureCoordinates_;
	std::vector< glm::vec4 > colors_;
	std::vector< VertexBoneData > vertexBoneData_;

	BoneData boneData_;

	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboIds_[5];

	std::string textureFileName_;

private:
	friend class boost::serialization::access;
	
	template<class Archive> void inline serialize(Archive& ar, const unsigned int version);
};

}
}

#include "Mesh.inl"

#endif /* MESH_H_ */
