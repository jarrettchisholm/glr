#ifndef TERRAINMESH_H_
#define TERRAINMESH_H_

#include "glw/Mesh.hpp"

namespace glr
{
namespace terrain
{

/*
 * This extends the glw Mesh class to include texture blending data for voxel terrain.
 */
class TerrainMesh : public glr::glw::Mesh
{
public:
	/**
	 * Basic constructor.  The creator of the Mesh will have to set the data, and then call
	 * 'allocateVideoMemory()' and 'pushToVideoMemory()' before this mesh will render properly.
	 */
	TerrainMesh(glw::IOpenGlDevice* openGlDevice, std::string name);
	
	/**
	 * Standard constructor.  Once called, the Mesh will be ready for rendering (with no further action
	 * required by the creator).
	 */
	TerrainMesh(glw::IOpenGlDevice* openGlDevice,
		std::string name,
		std::vector< glm::vec3 > vertices,
		std::vector< glm::vec3 > normals,
		std::vector< glm::vec2 > textureCoordinates,
		std::vector< glm::vec4 > colors,
		std::vector< glm::vec4 > texBlendingData,
		GLint shaderVariableLocation,
		bool initialize = true
	);
	virtual ~TerrainMesh();
	
	virtual void pushToVideoMemory();
	virtual void pullFromVideoMemory();
	virtual void freeLocalData();
	virtual void freeVideoMemory();
	virtual void allocateVideoMemory();
	
	void setTextureBlendingData(std::vector< glm::vec4 > texBlendingData);
	void setShaderVariableLocation(GLint shaderVariableLocation);
	
	std::vector< glm::vec4 >& getTextureBlendingData();
	GLint getShaderVariableLocation() const;

protected:
	std::vector< glm::vec4 > texBlendingData_;
	GLint shaderVariableLocation_;

	glm::detail::uint32 texBlendVboId_;
};

}
}

#endif /* TERRAINMESH_H_ */
