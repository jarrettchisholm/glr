#include <utility>

#include "terrain/Terrain.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace terrain
{

Terrain::Terrain(Id id, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, openGlDevice)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glw::IOpenGlDevice* openGlDevice) : BasicSceneNode(id, std::move(name), openGlDevice)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice)
	: BasicSceneNode(id, std::move(name), position, orientation, scale, openGlDevice)
{
	initialize();
}

Terrain::Terrain(Id id, const Terrain& other) : BasicSceneNode(id, other)
{
	// TODO: Implement
}

Terrain::~Terrain()
{
}

void Terrain::render()
{
}

bool Terrain::isDirty() const
{
	return isDirty_;
}

void Terrain::initialize()
{
	isDirty_ = false;

	LOG_DEBUG( "Terrain initialized." );
}

}
}
