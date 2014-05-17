#ifndef ITERRAINMANAGER_H_
#define ITERRAINMANAGER_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "terrain/ITerrain.hpp"
#include "terrain/ITerrainManagerEventListener.hpp"

namespace glr
{
namespace terrain
{

class ITerrainManager
{
public:
	virtual ~ITerrainManager()
	{
	}
	;

	virtual ITerrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const = 0;
	
	/**
	 * Call this to get the terrain manager to update its internal structures and terrain.  Depending on the position
	 * of the follow target, the terrain manager may flag items for updating.
	 * 
	 * This method IS thread safe - you can call it outside of the OpenGL thread.
	 * 
	 * Note: You don't need to call this method every frame - calling once a second should be sufficient.
	 */
	virtual void tick() = 0;
	
	/**
	 * Will do up to maxUpdates number of updates to/from graphics card.
	 * 
	 * Note: This method is NOT thread safe.  Only call it from the OpenGL thread.
	 */
	virtual void update(glm::detail::uint32 maxUpdates = 10) = 0;
	
	/**
	 * Will render the terrain to the scene.
	 * 
	 * Note: This method is NOT thread safe.  Only call it from the OpenGL thread.
	 */
	virtual void render() = 0;

	/**
	 * Set the ISceneNode target that the terrain should be centered around.
	 */
	virtual void setFollowTarget(ISceneNode* target) = 0;
	virtual ISceneNode* getFollowTarget() const = 0;
	
	/**
	 * Generates (or regenerates) the terrain using the data loaded from disk or passed in as parameters.
	 * 
	 * This method IS thread safe - you can call it outside of the OpenGL thread.
	 */
	virtual void generate() = 0;
	virtual void generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) = 0;
	virtual void generate(ITerrain* terrain) = 0;
	
	virtual void addTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	virtual void removeTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	
	virtual void serialize(const std::string& filename) = 0;
	virtual void deserialize(const std::string& filename) = 0;
	
	virtual glm::detail::int32 getWidth() const = 0;
	virtual glm::detail::int32 getHeight() const = 0;
	virtual glm::detail::int32 getLength() const = 0;
	
	virtual glm::detail::int32 getBlockSize() const = 0;
};

}
}

#endif /* ITERRAINMANAGER_H_ */
