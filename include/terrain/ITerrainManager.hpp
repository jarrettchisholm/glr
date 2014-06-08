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

	/**
	 * Will return the terrain that contains the point at (x, y, z).  If terrain doesn't exist at the given coordinates, nullptr is returned.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @param z The z coordinate.
	 * 
	 * @return The terrain at (x, y, z), or nullptr if no terrain exists.
	 */
	virtual ITerrain* getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const = 0;
	
	/**
	 * Call this to get the terrain manager to update its internal structures and terrain.  Depending on the position
	 * of the follow target, the terrain manager may flag items for updating.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 * 
	 * Note: You don't need to call this method every frame - calling once a second should be sufficient.
	 */
	virtual void tick() = 0;
	
	/**
	 * Will do up to maxUpdates number of updates to/from graphics card.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void update(glm::detail::uint32 maxUpdates = 10) = 0;
	
	/**
	 * Will render the terrain to the scene.
	 * 
	 * **Not Thread Safe**: This method is *not* safe to call in a multi-threaded environment, and should only be called from the 
	 * OpenGL thread.
	 */
	virtual void render() = 0;

	/**
	 * Set the ISceneNode target that the terrain should be centered around.
	 * 
	 * @param target The ISceneNode target that the terrain should be centered around.
	 */
	virtual void setFollowTarget(ISceneNode* target) = 0;
	
	/**
	 * Get the ISceneNode target that the terrain is centered around.
	 * 
	 * @return The ISceneNode target that the terrain is centered around.
	 */
	virtual ISceneNode* getFollowTarget() const = 0;
	
	/**
	 * Generates (or regenerates) the terrain using the data loaded from disk or passed in as parameters.
	 * 
	 * **Thread Safe**: This method is safe to call in a multi-threaded environment.
	 */
	virtual void generate(bool initialize = true) = 0;
	virtual void generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z, bool initialize = true) = 0;
	virtual void generate(ITerrain* terrain, bool initialize = true) = 0;
	
	virtual void addTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	virtual void removeTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	
	virtual void moveTerrainFromProcessedToReady(ITerrain* terrain) = 0;
	
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
