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
	virtual void tick() = 0;
	virtual void update(glm::detail::uint32 maxUpdates = 10) = 0;
	virtual void render() = 0;

	virtual void setFollowTarget(ISceneNode* target) = 0;
	virtual ISceneNode* getFollowTarget() const = 0;
	virtual void generate() = 0;
	virtual void generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) = 0;
	virtual void generate(ITerrain* terrain) = 0;
	
	virtual void addTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	virtual void removeTerrainManagerEventListener(ITerrainManagerEventListener* listener) = 0;
	
	virtual void serialize(const std::string& filename) = 0;
	virtual void deserialize(const std::string& filename) = 0;
	
	virtual glm::detail::float32 getWidth() const = 0;
	virtual glm::detail::float32 getHeight() const = 0;
	virtual glm::detail::float32 getLength() const = 0;
	
	virtual glm::detail::int32 getBlockSize() const = 0;
};

}
}

#endif /* ITERRAINMANAGER_H_ */
