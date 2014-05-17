#include <utility>
#include <sstream>
#include <iostream>

#include "terrain/Terrain.hpp"
#include "terrain/Constants.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace terrain
{

Terrain::Terrain(Id id, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, std::move(name), openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice, 
	glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, std::move(name), position, orientation, scale, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
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

void Terrain::freeVideoMemory()
{
	meshData_ = std::unique_ptr<TerrainMesh>( nullptr );
		
	modelPtr_ = std::unique_ptr<models::IModel>( nullptr );
	this->attach( (models::IModel*)nullptr );
}

void Terrain::initialize()
{
	isActive_ = true;
	isDirty_ = false;
	levelOfDetail_ = LOD_UNKNOWN;

	LOG_DEBUG( "Terrain initialized." );
}

glm::detail::int32 Terrain::getGridX() const
{
	return gridX_;
}

glm::detail::int32 Terrain::getGridY() const
{
	return gridY_;
}

glm::detail::int32 Terrain::getGridZ() const
{
	return gridZ_;
}

bool Terrain::isDirty() const
{
	return isDirty_;
}

void Terrain::setIsDirty(bool isDirty)
{
	isDirty_ = isDirty;
}
	
void Terrain::update()
{
}

void Terrain::setLod(LevelOfDetail lod)
{
	if (levelOfDetail_ == lod)
		return;
	
	levelOfDetail_ = lod;
	
	setIsDirty(true);
}

LevelOfDetail Terrain::getLod() const
{
	return levelOfDetail_;
}

bool Terrain::isActive() const
{
	return isActive_;
}

void Terrain::setIsActive( bool isActive )
{
	isActive_ = isActive;
}

bool Terrain::isEmptyOrSolid() const
{
	return isEmptyOrSolid_;
}

void Terrain::render()
{
	BasicSceneNode::render();
}

void Terrain::setMesh(std::unique_ptr<TerrainMesh> mesh)
{
	meshData_ = std::move(mesh);
}

void Terrain::setModel(std::unique_ptr<models::IModel> model)
{
	modelPtr_ = std::move(model);
}

TerrainMesh* Terrain::getData()
{
	return meshData_.get();
}

}
}
