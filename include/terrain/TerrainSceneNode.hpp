#ifndef TERRAINSCENENODE_H_
#define TERRAINSCENENODE_H_

#include "ITerrainSceneNode.hpp"
#include "BasicSceneNode.hpp"
#include "TerrainMesh.hpp"

#include "VoxelChunkMeshGenerator.hpp"
#include "VoxelChunkNoiseGenerator.hpp"

namespace glr
{
namespace terrain
{

class TerrainSceneNode : public ITerrainSceneNode, public BasicSceneNode
{
public:
	TerrainSceneNode(Id id, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ);
	TerrainSceneNode(Id id, const std::string& name, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ);
	virtual ~TerrainSceneNode();
	
	virtual void render();
	
	void freeVideoMemory();
	
	bool isActive() const;
	void setIsActive( bool isActive );
	
	bool isEmptyOrSolid() const;
	
	virtual glm::detail::int32 getGridX() const;
	virtual glm::detail::int32 getGridY() const;
	virtual glm::detail::int32 getGridZ() const;

	// TODO: Something other than this?
	void setMesh(std::unique_ptr<glr::terrain::TerrainMesh> mesh);
	void setModel(std::unique_ptr<models::IModel> model);
	
	TerrainMesh* getData();

private:
	glmd::int32 gridX_, gridY_, gridZ_;
	
	bool isActive_;
	bool isEmptyOrSolid_;

	std::unique_ptr<glr::terrain::TerrainMesh> meshData_;
	std::unique_ptr<models::IModel> modelPtr_;
	
	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboId_;

	void initialize();
	void initialize(glmd::int32 x, glmd::int32 y);
	
	std::vector<glm::vec4> texBlendingValues_;
};

}
}

#endif /* TERRAINSCENENODE_H_ */

