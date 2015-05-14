#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/gtx/string_cast.hpp>

#include "terrain/TerrainManager.hpp"
#include "terrain/Constants.hpp"
#include "terrain/VoxelChunkNoiseGenerator.hpp"
#include "terrain/TerrainMeshSerializer.hpp"
#include "terrain/dual_contouring/VoxelChunkMeshGenerator.hpp"
#include "terrain/marching_cubes/VoxelChunkMeshGenerator.hpp"
#include "terrain/ITerrain.hpp"
#include "terrain/ITerrainManagerEventListener.hpp"
#include "terrain/IFieldFunction.hpp"
#include "terrain/IVoxelChunkMeshGenerator.hpp"
#include "terrain/Terrain.hpp"

#include "glw/IOpenGlDevice.hpp"

#include "models/Model.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/InvalidArgumentException.hpp"

#include "common/logger/Logger.hpp"
#include "common/utilities/GlmUtilities.hpp"

namespace glr
{
namespace terrain
{

TerrainManager::TerrainManager(glw::IOpenGlDevice* openGlDevice, IFieldFunction* fieldFunction, TerrainSettings terrainSettings)
	: openGlDevice_(openGlDevice), fieldFunction_(fieldFunction), terrainSettings_(terrainSettings)
{
	initialize();
}

TerrainManager::~TerrainManager()
{
}

void TerrainManager::initialize()
{
	followTarget_ = nullptr;
	
	switch (terrainSettings_.smoothingAlgorithm)
	{
		case ALGORITHM_MARCHING_CUBES:
			LOG_DEBUG("Using Marching Cubes smoothing algorithm.");
			voxelChunkMeshGenerator_ = std::unique_ptr<IVoxelChunkMeshGenerator>( new marching_cubes::VoxelChunkMeshGenerator() );
			break;
		
		case ALGORITHM_DUAL_CONTOURING:
			if (fieldFunction_ == nullptr)
			{
				const std::string message = std::string("Dual Contouring algorithm requires a field function.");
				LOG_ERROR(message);
				throw exception::InvalidArgumentException(message);
			}
			
			LOG_DEBUG("Using Dual Contouring smoothing algorithm.");
			voxelChunkMeshGenerator_ = std::unique_ptr<IVoxelChunkMeshGenerator>( new dual_contouring::VoxelChunkMeshGenerator(fieldFunction_) );
			break;
		
		default:
			const std::string message = std::string("Invalid smoothing algorithm set for terrain manager.");
			LOG_ERROR(message);
			throw exception::InvalidArgumentException(message);
	}
	
	terrain_ = std::vector< std::unique_ptr<Terrain> >();
	terrainToBeProcessed_ = std::vector< std::unique_ptr<Terrain> >();
	eventListeners_ = std::vector< ITerrainManagerEventListener* >();
	
	idManager_ = IdManager();

	LOG_DEBUG( "Terrain Manager initialized." );
}

ITerrain* TerrainManager::getTerrain(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z) const
{
	// TODO: Implement
	return nullptr;
}

glm::ivec3 TerrainManager::getTargetGridLocation()
{
	glm::ivec3 retVal = glm::ivec3(0, 0, 0);

	if (followTarget_ != nullptr)
	{
		glm::vec3 pos = followTarget_->getPosition() / (glmd::float32)constants::CHUNK_SIZE; // - 0.5f * totalWorldGridSize  (?)
		
		pos.x -= terrainSettings_.length/2;
		pos.y -= terrainSettings_.width/2;
		pos.z -= terrainSettings_.height/2;
		
		retVal = glm::ivec3((int)pos.x, (int)pos.y, (int)pos.z);
	}

	return retVal;
}

// TESTING
std::mutex gridMutex;
void TerrainManager::tick()
{
	auto followTargetGridLocation = getTargetGridLocation();
	//std::cout << glm::to_string(followTargetGridLocation) << "    " << glm::to_string(currentGridLocation_) << std::endl;
	if (followTargetGridLocation != currentGridLocation_)
	{
		{
			std::lock_guard<std::mutex> lock(gridMutex);
			previousGridLocation_ = currentGridLocation_;
			currentGridLocation_ = followTargetGridLocation;
		}
		std::cout << "updateTerrainLod START " << std::endl;
		this->updateTerrainLod();
		std::cout << "updateTerrainLod END " << std::endl;
	}
}

void TerrainManager::updateTerrainLod()
{
	for (int i=0; i < terrainSettings_.length; i++)
	{
		for (int j=0; j < terrainSettings_.height; j++)
		{
			for (int k=0; k < terrainSettings_.width; k++)
			{
				auto terrain = getTerrain(i, j, k);
				
				if (terrain != nullptr)
				{
					LevelOfDetail lod = getNewTerrainLod(*terrain);
					terrain->updateLod(lod);
					
					auto function = [=] {
						terrain->prepareOrUpdateGraphics();
						//this->moveTerrainFromProcessedToReady( terrain );
					};
					
					postOpenGlWork( function );
				}
			}
		}
		//std::cout << "MARK: " << i << std::endl;
	}
}

LevelOfDetail TerrainManager::getNewTerrainLod(Terrain& t)
{
	if (isWithinRadius(t, *followTarget_, terrainSettings_.lodHighestRadius))
	{
		return LOD_HIGHEST;
	}
	else if (isWithinRadius(t, *followTarget_, terrainSettings_.lodHighRadius))
	{
		return LOD_HIGH;
	}
	else if (isWithinRadius(t, *followTarget_, terrainSettings_.lodMediumRadius))
	{
		return LOD_MEDIUM;
	}
	else if (isWithinRadius(t, *followTarget_, terrainSettings_.lodLowRadius))
	{
		return LOD_LOW;
	}
	else
	{
		return LOD_LOWEST;
	}
}

bool TerrainManager::isWithinRadius(Terrain& t, ISceneNode& n, glmd::float32 radius)
{
	glm::vec3 pos = t.getPosition();
	glm::vec3 center = n.getPosition();
	
	return utilities::isVec3InSphere(pos, center, radius);
}

void TerrainManager::postOpenGlWork(std::function<void()> work)
{
	openGlWorkMutex_.lock();
	openGlWork_.emplace_back(std::move(work));
	openGlWorkMutex_.unlock();
}

void TerrainManager::createTerrain(glmd::float32 x, glmd::float32 y, glmd::float32 z, bool initialize)
{
	glmd::int32 i = (glmd::int32)(x / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 j = (glmd::int32)(y / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 k = (glmd::int32)(z / (glmd::float32)constants::CHUNK_SIZE);
	
	createTerrain(i, j, k, initialize);
}

void TerrainManager::createTerrain(const glm::ivec3& coordinates, bool initialize)
{
	createTerrain(coordinates.x, coordinates.y, coordinates.z, initialize);
}

void TerrainManager::createTerrain(glmd::int32 x, glmd::int32 y, glmd::int32 z, bool initialize)
{
	// Make sure this terrain doesn't already exist in the render list
	auto terrain = getTerrain(x, y, z);
	
	if (terrain != nullptr)
	{
		LOG_DEBUG("Terrain already exists in render list: " << x << ", " << y << ", " << z);
		return;
	}
	
	// Check to see if it exists in the 'to be processed' list
	terrain = getTerrainToBeProcessed(x, y, z);
	
	// If it doesn't, then we create it
	if (terrain == nullptr)
	{
		{
			std::lock_guard<std::mutex> lock(terrainToBeProcessedMutex_);
			terrainToBeProcessed_.push_back( 
				std::unique_ptr<Terrain>( new Terrain(idManager_.createId(), openGlDevice_, x, y, z, terrainSettings_.length, terrainSettings_.width, terrainSettings_.height, fieldFunction_, voxelChunkMeshGenerator_.get()) )
			);
			
			terrain = terrainToBeProcessed_.back().get();
		}
		
		auto shader = openGlDevice_->getShaderProgramManager()->getShaderProgram( std::string("voxel") );
		assert( shader != nullptr );
		
		terrain->attach(shader);
		
		std::stringstream ss;
		ss << "terrain_" << terrain->getGridX() << "_" << terrain->getGridY() << "_" << terrain->getGridZ();
		terrain->setName( ss.str() );
		
		// terrain manager stuff
		if (initialize)
		{
			terrain->generate();
			
			if (!terrain->isEmptyOrSolid())
			{
				auto function = [=] {
					terrain->prepareOrUpdateGraphics();
					this->moveTerrainFromProcessedToReady( terrain );
				};
				
				postOpenGlWork( function );
				
				sendAddedTerrainEventToEventListeners(terrain);
			}
			else
			{
				this->removeTerrainToBeProcessedAndReturn( terrain );
			}
		}
		else
		{
			sendAddedTerrainEventToEventListeners(terrain);
		}
	}
	else
	{
		LOG_DEBUG("Terrain already exists in 'to be processed' list: " << x << ", " << y << ", " << z);
	}
}

void TerrainManager::addTerrain(Terrain* terrain)
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	terrain_.push_back( std::unique_ptr<Terrain>( terrain ) );
}

void TerrainManager::addTerrain(std::unique_ptr<Terrain> terrain)
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	terrain_.push_back( std::move(terrain) );
}

void TerrainManager::removeTerrain(glmd::float32 x, glmd::float32 y, glmd::float32 z)
{
	glmd::int32 i = (glmd::int32)(x / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 j = (glmd::int32)(y / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 k = (glmd::int32)(z / (glmd::float32)constants::CHUNK_SIZE);
	
	removeTerrain(i, j, k);
}

void TerrainManager::removeTerrain(const glm::ivec3& coordinates)
{
	removeTerrain(coordinates.x, coordinates.y, coordinates.z);
}

// Immediately removes the terrain from the render list and ensures its video memory is freed before it is destroyed
void TerrainManager::removeTerrain(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	auto terrain = removeTerrainAndReturn(x, y, z);
	
	// Removes the terrain from the render list and ensures its video memory is freed before it is destroyed
	if (terrain.get() != nullptr)
	{
		auto p = terrain.release();
		
		auto f = [=]() {
			auto t = std::unique_ptr<Terrain>( p );
			t->freeVideoMemory();
		};
		
		postOpenGlWork( f );
	}
}

std::unique_ptr<Terrain> TerrainManager::removeTerrainAndReturn(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	auto retVal = std::unique_ptr<Terrain>();
	
	auto findFunction = [x, y, z](const std::unique_ptr<Terrain>& node) { return (node->getGridX() == x && node->getGridY() == y && node->getGridZ() == z); };
	
	auto it = std::find_if(terrain_.begin(), terrain_.end(), findFunction);
	
	if (it != terrain_.end())
	{
		retVal = std::move(*it);
		terrain_.erase(it);
	}
	
	return std::move(retVal);
}

std::unique_ptr<Terrain> TerrainManager::removeTerrainAndReturn(Terrain* terrain)
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	auto retVal = std::unique_ptr<Terrain>();
	
	auto findFunction = [terrain](const std::unique_ptr<Terrain>& node) { return node.get() == terrain; };
	
	auto it = std::find_if(terrain_.begin(), terrain_.end(), findFunction);
	
	if (it != terrain_.end())
	{
		retVal = std::move(*it);
		terrain_.erase(it);
	}
	
	return std::move(retVal);
}

std::unique_ptr<Terrain> TerrainManager::removeTerrainToBeProcessedAndReturn(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	std::lock_guard<std::mutex> lock(terrainToBeProcessedMutex_);
	
	auto retVal = std::unique_ptr<Terrain>();
	
	auto findFunction = [x, y, z](const std::unique_ptr<Terrain>& node) { return (node->getGridX() == x && node->getGridY() == y && node->getGridZ() == z); };
	
	auto it = std::find_if(terrainToBeProcessed_.begin(), terrainToBeProcessed_.end(), findFunction);
	
	if (it != terrainToBeProcessed_.end())
	{
		retVal = std::move(*it);
		terrainToBeProcessed_.erase(it);
	}
	
	return std::move(retVal);
}

std::unique_ptr<Terrain> TerrainManager::removeTerrainToBeProcessedAndReturn(Terrain* terrain)
{
	std::lock_guard<std::mutex> lock(terrainToBeProcessedMutex_);
	
	auto retVal = std::unique_ptr<Terrain>();
	
	auto findFunction = [terrain](const std::unique_ptr<Terrain>& node) { return node.get() == terrain; };
	
	auto it = std::find_if(terrainToBeProcessed_.begin(), terrainToBeProcessed_.end(), findFunction);
	
	if (it != terrainToBeProcessed_.end())
	{
		retVal = std::move(*it);
		terrainToBeProcessed_.erase(it);
	}
	
	return std::move(retVal);
}

void TerrainManager::removeAllTerrain()
{
	// TODO: Implement
	//assert(0);
	idManager_ = IdManager();
}

void TerrainManager::moveTerrainFromProcessedToReady(ITerrain* terrain)
{
	auto t = removeTerrainToBeProcessedAndReturn( dynamic_cast<Terrain*>( terrain ) );
	
	if (t.get() != nullptr)
	{
		addTerrain( std::move(t) );
	}
}

Terrain* TerrainManager::getTerrain(glmd::float32 x, glmd::float32 y, glmd::float32 z)
{
	glmd::int32 i = (glmd::int32)(x / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 j = (glmd::int32)(y / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 k = (glmd::int32)(z / (glmd::float32)constants::CHUNK_SIZE);
	
	i += terrainSettings_.length/2;
	j += terrainSettings_.width/2;
	k += terrainSettings_.height/2;
	
	return getTerrain(i, j, k);
}

Terrain* TerrainManager::getTerrain(const glm::ivec3& coordinates)
{	
	return getTerrain(coordinates.x, coordinates.y, coordinates.z);
}

Terrain* TerrainManager::getTerrain(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	Terrain* retVal = nullptr;
	
	auto findFunction = [x, y, z](const std::unique_ptr<Terrain>& node) { return (node->getGridX() == x && node->getGridY() == y && node->getGridZ() == z); };
	
	auto it = std::find_if(terrain_.begin(), terrain_.end(), findFunction);
	
	if (it != terrain_.end())
	{
		retVal = it->get();
	}

	return retVal;
}

Terrain* TerrainManager::getTerrainToBeProcessed(glmd::float32 x, glmd::float32 y, glmd::float32 z)
{
	glmd::int32 i = (glmd::int32)(x / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 j = (glmd::int32)(y / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 k = (glmd::int32)(z / (glmd::float32)constants::CHUNK_SIZE);
	
	i += terrainSettings_.length/2;
	j += terrainSettings_.width/2;
	k += terrainSettings_.height/2;
	
	return getTerrainToBeProcessed(i, j, k);
}

Terrain* TerrainManager::getTerrainToBeProcessed(const glm::ivec3& coordinates)
{
	return getTerrainToBeProcessed(coordinates.x, coordinates.y, coordinates.z);
}

Terrain* TerrainManager::getTerrainToBeProcessed(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	std::lock_guard<std::mutex> lock(terrainToBeProcessedMutex_);
	
	Terrain* retVal = nullptr;
	
	auto findFunction = [x, y, z](const std::unique_ptr<Terrain>& node) { return (node->getGridX() == x && node->getGridY() == y && node->getGridZ() == z); };
	
	auto it = std::find_if(terrainToBeProcessed_.begin(), terrainToBeProcessed_.end(), findFunction);
	
	if (it != terrainToBeProcessed_.end())
	{
		retVal = it->get();
	}

	return retVal;
}

void TerrainManager::update(glmd::uint32 maxUpdates)
{
	glmd::uint32 size = 0;
	openGlWorkMutex_.lock();
	size = openGlWork_.size();
	openGlWorkMutex_.unlock();

	for (glmd::uint32 i=0; i < maxUpdates && i < size; i++)
	{
		auto work = std::function<void()>();

		openGlWorkMutex_.lock();
		work = std::move(openGlWork_.front());
		openGlWork_.pop_front();
		openGlWorkMutex_.unlock();

		work();
	}
}

void TerrainManager::render()
{
	std::lock_guard<std::mutex> lock(terrainMutex_);
	
	//int numV = 0;
	for ( auto& terrain : terrain_ )
	{
		terrain->render();
		//numV += terrain->getData()->getVertices().size();
	}
	//std::cout << "DONE: " << numV << std::endl;
}

void TerrainManager::setFollowTarget(ISceneNode* target)
{
	followTarget_ = target;

	currentGridLocation_ = getTargetGridLocation();
	previousGridLocation_ = currentGridLocation_;
}

ISceneNode* TerrainManager::getFollowTarget() const
{
	return followTarget_;
}

void TerrainManager::generate(bool initialize)
{
	removeAllTerrain();
	
	for (int i=0; i < terrainSettings_.length; i++)
	{
		for (int j=0; j < terrainSettings_.height; j++)
		{
			for (int k=0; k < terrainSettings_.width; k++)
			{
				createTerrain( i, j, k, initialize );
			}
		}
		std::cout << "MARK: " << i << std::endl;
	}
}

void TerrainManager::generate(glm::detail::int32 x, glm::detail::int32 y, glm::detail::int32 z, bool initialize)
{
}

void TerrainManager::generate(ITerrain* terrain, bool initialize)
{
}

void TerrainManager::sendRemovedTerrainEventToEventListeners(ITerrain* terrain)
{
	for ( auto l : eventListeners_ )
	{
		l->terrainRemoved(terrain);
	}
}

void TerrainManager::sendAddedTerrainEventToEventListeners(ITerrain* terrain)
{
	for ( auto l : eventListeners_ )
	{
		l->terrainAdded(terrain);
	}
}

void TerrainManager::addTerrainManagerEventListener(ITerrainManagerEventListener* listener)
{
	eventListeners_.push_back(listener);
}

void TerrainManager::removeTerrainManagerEventListener(ITerrainManagerEventListener* listener)
{
	eventListeners_.erase( std::remove(eventListeners_.begin(), eventListeners_.end(), listener), eventListeners_.end() );
}

void TerrainManager::serialize(const std::string& filename)
{
	std::ofstream stream(filename, std::ios::out | std::ios::binary);
	
	{
		std::lock_guard<std::mutex> lock(terrainMutex_);
		for (auto& terrain : terrain_)
		{
			std::cout << "Saving..." << std::endl;
			auto mesh = terrain->getData();
			glr::terrain::serialize(stream, *mesh);
		}
	}
}

void TerrainManager::deserialize(const std::string& filename)
{
}

glm::detail::int32 TerrainManager::getLength() const
{
	return terrainSettings_.length;
}

glm::detail::int32 TerrainManager::getWidth() const
{
	return terrainSettings_.width;
}

glm::detail::int32 TerrainManager::getHeight() const
{
	return terrainSettings_.height;
}

glm::detail::int32 TerrainManager::getBlockSize() const
{
	return constants::CHUNK_SIZE;
}

}
}
