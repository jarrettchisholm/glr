#include <sstream>
#include <iostream>

#include "terrain/TerrainSceneNode.hpp"
#include "terrain/TerrainMesh.hpp"
#include "terrain/Constants.hpp"

#include "models/Model.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace terrain
{

TerrainSceneNode::TerrainSceneNode(Id id, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ) 
	: BasicSceneNode(id, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
{
	initialize();
}

TerrainSceneNode::TerrainSceneNode(Id id, const std::string& name, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, name, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ)
{
	initialize();
}

TerrainSceneNode::~TerrainSceneNode()
{
}

void TerrainSceneNode::freeVideoMemory()
{
	meshData_ = std::unique_ptr<glr::terrain::TerrainMesh>( nullptr );
		
	modelPtr_ = std::unique_ptr<models::IModel>( nullptr );
	this->attach( (models::IModel*)nullptr );
}

void TerrainSceneNode::initialize()
{	
	isActive_ = true;
}

glm::detail::int32 TerrainSceneNode::getGridX() const
{
	return gridX_;
}

glm::detail::int32 TerrainSceneNode::getGridY() const
{
	return gridY_;
}

glm::detail::int32 TerrainSceneNode::getGridZ() const
{
	return gridZ_;
}

bool TerrainSceneNode::isActive() const
{
	return isActive_;
}

void TerrainSceneNode::setIsActive( bool isActive )
{
	isActive_ = isActive;
}

bool TerrainSceneNode::isEmptyOrSolid() const
{
	return isEmptyOrSolid_;
}

void TerrainSceneNode::render()
{
	BasicSceneNode::render();
}

void TerrainSceneNode::setMesh(std::unique_ptr<glr::terrain::TerrainMesh> mesh)
{
	meshData_ = std::move(mesh);
}

void TerrainSceneNode::setModel(std::unique_ptr<models::IModel> model)
{
	modelPtr_ = std::move(model);
}

}
}
