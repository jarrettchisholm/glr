#ifndef TERRAIN_H_
#define TERRAIN_H_

#include <atomic>

#include "ITerrain.hpp"
#include "BasicSceneNode.hpp"
#include "TerrainMesh.hpp"

#include "IVoxelChunkMeshGenerator.hpp"
#include "VoxelChunkNoiseGenerator.hpp"

namespace glr
{
namespace terrain
{

class Terrain : public virtual ITerrain, public BasicSceneNode
{
public:
	Terrain(Id id, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ,
		glmd::int32 length, glmd::int32 width, glmd::int32 height, IFieldFunction* fieldFunction, IVoxelChunkMeshGenerator* voxelChunkMeshGenerator);
	Terrain(Id id, std::string name, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ);
	Terrain(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice, 
		glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ);
	Terrain(Id id, const Terrain& other);
	virtual ~Terrain();
	
	virtual void render();
	virtual bool isDirty() const;
	virtual void setIsDirty(bool isDirty);
	
	virtual void update();
	
	virtual void setLod(LevelOfDetail lod);
	virtual LevelOfDetail getLod() const;
	
	virtual void generate();
	
	void freeVideoMemory();
	
	bool isActive() const;
	void setIsActive( bool isActive );
	
	virtual bool isEmptyOrSolid() const;
	
	virtual glm::detail::int32 getGridX() const;
	virtual glm::detail::int32 getGridY() const;
	virtual glm::detail::int32 getGridZ() const;

	// TODO: Something other than this?
	void setMesh(std::unique_ptr<glr::terrain::TerrainMesh> mesh);
	void setModel(std::unique_ptr<models::IModel> model);
	
	TerrainMesh* getData();
	virtual models::IModel* getModel() const;
	
	virtual void makeReadyForRender();
	void initializeShaderLocations();

private:
	glmd::int32 gridX_;
	glmd::int32 gridY_;
	glmd::int32 gridZ_;
	glmd::int32 length_;
	glmd::int32 width_;
	glmd::int32 height_;
	
	IFieldFunction* fieldFunction_;
	IVoxelChunkMeshGenerator* voxelChunkMeshGenerator_;
	
	std::atomic<bool> isActive_;
	std::atomic<bool> isEmptyOrSolid_;
	LevelOfDetail levelOfDetail_;

	std::atomic<bool> isDirty_;

	// The terrain object manages the memory for it's mesh
	std::unique_ptr<glr::terrain::TerrainMesh> meshData_;
	// The terrain object manages the memory for it's model
	std::unique_ptr<models::IModel> modelPtr_;
	
	glm::detail::uint32 vaoId_;
	glm::detail::uint32 vboId_;

	void initialize();
	void initialize(glmd::int32 x, glmd::int32 y);
	
	std::vector<glm::vec4> texBlendingValues_;
};

}
}

#endif /* TERRAIN_H_ */
