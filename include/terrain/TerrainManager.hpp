#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include <memory>
#include <string>
#include <functional>
#include <deque>
#include <mutex>

#include "TerrainSettings.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "IdManager.hpp"

#include "terrain/ITerrainManager.hpp"
#include "terrain/IFieldFunction.hpp"
#include "terrain/IVoxelChunkMeshGenerator.hpp"
#include "terrain/TerrainSceneNode.hpp"
#include "terrain/Terrain.hpp"

namespace glr
{
namespace terrain
{
	
class TerrainManager : public ITerrainManager
{
public:
	TerrainManager(glw::IOpenGlDevice* openGlDevice, IFieldFunction* fieldFunction = nullptr, TerrainSettings terrainSettings = TerrainSettings());
	virtual ~TerrainManager();

	virtual ITerrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const;
	virtual void tick();
	virtual void update(glmd::uint32 maxUpdates = 10);
	virtual void render();

	virtual void setFollowTarget(ISceneNode* target);
	virtual ISceneNode* getFollowTarget() const;
	virtual void generate();
	virtual void generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	virtual void generate(ITerrain* terrain);
	
	virtual void addTerrainManagerEventListener(ITerrainManagerEventListener* listener);
	virtual void removeTerrainManagerEventListener(ITerrainManagerEventListener* listener);
	
	virtual void serialize(const std::string& filename);
	virtual void deserialize(const std::string& filename);
	
	virtual glm::detail::float32 getWidth() const;
	virtual glm::detail::float32 getHeight() const;
	virtual glm::detail::float32 getLength() const;
	
	virtual glm::detail::int32 getBlockSize() const;

private:	
	glw::IOpenGlDevice* openGlDevice_;
	ISceneNode* followTarget_;

	glm::ivec3 currentGridLocation_;
	glm::ivec3 previousGridLocation_;

	IFieldFunction* fieldFunction_;
	TerrainSettings terrainSettings_;
	std::unique_ptr<IVoxelChunkMeshGenerator> voxelChunkMeshGenerator_;
	
	std::vector< std::unique_ptr<TerrainSceneNode> > terrainChunks_;
	std::mutex terrainChunksMutex_;
	
	mutable std::mutex openGlWorkMutex_;
	std::deque< std::function<void()> > openGlWork_;
	
	IdManager idManager_;
	
	void initialize();
	void postOpenGlWork(std::function<void()> work);
	glm::ivec3 getTargetGridLocation();
	void updateChunks();

	void addChunk(glmd::float32 x, glmd::float32 y, glmd::float32 z);
	void addChunk(glmd::int32 x, glmd::int32 y, glmd::int32 z);
	void addChunk(const glm::ivec3& coordinates);
	void addChunk(TerrainSceneNode* chunk);
	void removeChunk(glmd::float32 x, glmd::float32 y, glmd::float32 z);
	void removeChunk(glmd::int32 x, glmd::int32 y, glmd::int32 z);
	void removeChunk(const glm::ivec3& coordinates);
	void removeChunk(TerrainSceneNode* chunk);
	void removeAllChunks();
	TerrainSceneNode* getChunk(glmd::float32 x, glmd::float32 y, glmd::float32 z);
	TerrainSceneNode* getChunk(glmd::int32 x, glmd::int32 y, glmd::int32 z);
	TerrainSceneNode* getChunk(const glm::ivec3& coordinates);
};

}
}

#endif /* TERRAINMANAGER_H_ */
