#ifndef TERRAINMANAGER_H_
#define TERRAINMANAGER_H_

#include <memory>
#include <string>

#include "terrain/ITerrainManager.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "terrain/Terrain.hpp"

namespace glr
{
namespace terrain
{
	
class TerrainManager : public ITerrainManager
{
public:
	TerrainManager(glw::IOpenGlDevice* openGlDevice);
	virtual ~TerrainManager();

	virtual ITerrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const;
	virtual void tick();
	virtual void update();
	virtual void drawAll();

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

	std::vector< std::unique_ptr<Terrain> > terrainChunks_;
	
	void initialize();
};

}
}

#endif /* TERRAINMANAGER_H_ */
