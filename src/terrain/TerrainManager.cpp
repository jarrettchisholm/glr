#include <utility>

#include "terrain/TerrainManager.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace terrain
{

TerrainManager::TerrainManager(glw::IOpenGlDevice* openGlDevice)
{
	initialize();
}

TerrainManager::~TerrainManager()
{
}

void TerrainManager::initialize()
{
	LOG_DEBUG( "Terrain Manager initialized." );
}

ITerrain* TerrainManager::getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const
{
}

void TerrainManager::tick()
{
}

void TerrainManager::update()
{
}

void TerrainManager::drawAll()
{
}

void TerrainManager::setFollowTarget(ISceneNode* target)
{
}

ISceneNode* TerrainManager::getFollowTarget() const
{
}

void TerrainManager::generate()
{
}

void TerrainManager::generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z)
{
}

void TerrainManager::generate(ITerrain* terrain)
{
}

void TerrainManager::addTerrainManagerEventListener(ITerrainManagerEventListener* listener)
{
}

void TerrainManager::removeTerrainManagerEventListener(ITerrainManagerEventListener* listener)
{
}

void TerrainManager::serialize(const std::string& filename)
{
}

void TerrainManager::deserialize(const std::string& filename)
{
}

glm::detail::float32 TerrainManager::getWidth() const
{
}

glm::detail::float32 TerrainManager::getHeight() const
{
}

glm::detail::float32 TerrainManager::getLength() const
{
}

glm::detail::int32 TerrainManager::getBlockSize() const
{
}

}
}
