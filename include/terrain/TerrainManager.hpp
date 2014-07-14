#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <deque>
#include <mutex>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "TerrainSettings.hpp"

#include "IdManager.hpp"
#include "terrain/ITerrainManager.hpp"

namespace glr
{

namespace glw
{
class IOpenGlDevice;
}

namespace terrain
{

class IFieldFunction;
class IVoxelChunkMeshGenerator;
class Terrain;

class TerrainManager : public ITerrainManager
{
public:
	TerrainManager(glw::IOpenGlDevice* openGlDevice, IFieldFunction* fieldFunction = nullptr, TerrainSettings terrainSettings = TerrainSettings());
	virtual ~TerrainManager();

	virtual ITerrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const;
	virtual void tick();
	virtual void update(glm::detail::uint32 maxUpdates = 10);
	virtual void render();

	virtual void setFollowTarget(ISceneNode* target);
	virtual ISceneNode* getFollowTarget() const;
	virtual void generate(bool initialize = true);
	virtual void generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z, bool initialize = true);
	virtual void generate(ITerrain* terrain, bool initialize = true);
	
	virtual void addTerrainManagerEventListener(ITerrainManagerEventListener* listener);
	virtual void removeTerrainManagerEventListener(ITerrainManagerEventListener* listener);
	
	virtual void serialize(const std::string& filename);
	virtual void deserialize(const std::string& filename);
	
	virtual glm::detail::int32 getWidth() const;
	virtual glm::detail::int32 getHeight() const;
	virtual glm::detail::int32 getLength() const;
	
	virtual glm::detail::int32 getBlockSize() const;

	virtual void moveTerrainFromProcessedToReady(ITerrain* terrain);

private:	
	glw::IOpenGlDevice* openGlDevice_;
	ISceneNode* followTarget_;

	glm::ivec3 currentGridLocation_;
	glm::ivec3 previousGridLocation_;

	IFieldFunction* fieldFunction_;
	TerrainSettings terrainSettings_;
	std::unique_ptr<IVoxelChunkMeshGenerator> voxelChunkMeshGenerator_;
	
	std::vector< std::unique_ptr<Terrain> > terrain_;
	std::vector< std::unique_ptr<Terrain> > terrainToBeProcessed_;
	std::mutex terrainMutex_;
	std::mutex terrainToBeProcessedMutex_;
	
	std::vector<ITerrainManagerEventListener*> eventListeners_;
	
	mutable std::mutex openGlWorkMutex_;
	std::deque< std::function<void()> > openGlWork_;
	
	IdManager idManager_;
	
	void initialize();
	void postOpenGlWork(std::function<void()> work);
	glm::ivec3 getTargetGridLocation();
	void updateTerrainLod();

	LevelOfDetail getNewTerrainLod(Terrain& t);
	bool isWithinRadius(Terrain& t, ISceneNode& n, glm::detail::float32 radius);

	void createTerrain(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, bool initialize = true);
	void createTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z, bool initialize = true);
	void createTerrain(const glm::ivec3& coordinates, bool initialize = true);
	
	void addTerrain(Terrain* terrain);
	void addTerrain(std::unique_ptr<Terrain> terrain);
	
	void removeTerrain(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	void removeTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	void removeTerrain(const glm::ivec3& coordinates);
	void removeTerrain(Terrain* terrain);
	
	std::unique_ptr<Terrain> removeTerrainAndReturn(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	std::unique_ptr<Terrain> removeTerrainAndReturn(Terrain* terrain);
	
	std::unique_ptr<Terrain> removeTerrainToBeProcessedAndReturn(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	std::unique_ptr<Terrain> removeTerrainToBeProcessedAndReturn(Terrain* terrain);
	
	void removeAllTerrain();
	
	Terrain* getTerrain(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	Terrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	Terrain* getTerrain(const glm::ivec3& coordinates);
	
	Terrain* getTerrainToBeProcessed(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	Terrain* getTerrainToBeProcessed(const glm::ivec3& coordinates);
	Terrain* getTerrainToBeProcessed(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z);
	
	void sendAddedTerrainEventToEventListeners(ITerrain* terrain);
	void sendRemovedTerrainEventToEventListeners(ITerrain* terrain);
};

}
}

#endif /* TERRAINMANAGER_H_ */
