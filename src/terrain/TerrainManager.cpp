#include <utility>

#define GLM_FORCE_RADIANS
#include <glm/gtx/string_cast.hpp>

#include "terrain/TerrainManager.hpp"
#include "terrain/Constants.hpp"
#include "terrain/VoxelChunkNoiseGenerator.hpp"
#include "terrain/TerrainMeshSerializer.hpp"
#include "terrain/dual_contouring/VoxelChunkMeshGenerator.hpp"
#include "terrain/marching_cubes/VoxelChunkMeshGenerator.hpp"

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
				auto t = getTerrain(i, j, k);
				
				if (t != nullptr)
				{
					LevelOfDetail lod = getNewTerrainLod(*t);
					t->setLod(lod);
				}
			}
		}
		//std::cout << "MARK: " << i << std::endl;
	}
}

LevelOfDetail TerrainManager::getNewTerrainLod(Terrain& t)
{
	if (isWithinRadius(t, *followTarget_, terrainSettings_.lodHighestRadius))
		return LOD_HIGHEST;
	if (isWithinRadius(t, *followTarget_, terrainSettings_.lodHighRadius))
		return LOD_HIGH;
	if (isWithinRadius(t, *followTarget_, terrainSettings_.lodMediumRadius))
		return LOD_MEDIUM;
	if (isWithinRadius(t, *followTarget_, terrainSettings_.lodLowRadius))
		return LOD_LOW;
	else
		return LOD_LOWEST;
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

void TerrainManager::createTerrain(glmd::float32 x, glmd::float32 y, glmd::float32 z)
{
	glmd::int32 i = (glmd::int32)(x / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 j = (glmd::int32)(y / (glmd::float32)constants::CHUNK_SIZE);
	glmd::int32 k = (glmd::int32)(z / (glmd::float32)constants::CHUNK_SIZE);
	
	createTerrain(i, j, k);
}

void TerrainManager::createTerrain(const glm::ivec3& coordinates)
{
	createTerrain(coordinates.x, coordinates.y, coordinates.z);
}

void TerrainManager::createTerrain(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	auto terrain = getTerrain(x, y, z);
	
	if (terrain == nullptr)
	{
		//std::cout << "add: " << x << ", " << y << ", " << z << std::endl;
		terrain = new Terrain(idManager_.createId(), openGlDevice_, x, y, z);
		//terrain_.push_back( std::unique_ptr<Terrain>(  ) );
		//terrain = terrain_.back().get();
	}
	else
	{
		terrainMutex_.lock();
		if (!terrain->isActive())
			terrain->setIsActive( true );
		terrainMutex_.unlock();
		return;
	}

	// MARK: Previous lambda function
	{
		//std::cout << "createTerrain START 1 " << std::this_thread::get_id() << std::endl;
		
		VoxelChunk voxelChunk = VoxelChunk(terrain->getGridX(), terrain->getGridY(), terrain->getGridZ());
	
		generateNoise(voxelChunk, terrainSettings_.length, terrainSettings_.width, terrainSettings_.height, *fieldFunction_);
		
		bool isEmptyOrSolid = determineIfEmptyOrSolid(voxelChunk);

		//std::cout << "createTerrain START 2 " << std::this_thread::get_id() << std::endl;
		if (!isEmptyOrSolid)
		{
			auto vertices = std::vector< glm::vec3 >();
			auto normals = std::vector< glm::vec3 >();
			auto textureBlendingValues = std::vector< glm::vec4 >();
			
			voxelChunkMeshGenerator_->generateMesh(voxelChunk, terrainSettings_.length, terrainSettings_.width, terrainSettings_.height, vertices, normals, textureBlendingValues);

			auto shader = openGlDevice_->getShaderProgramManager()->getShaderProgram( std::string("voxel") );
			assert( shader != nullptr );
			
			std::stringstream ss;
			ss << "terrain_" << terrain->getGridX() << "_" << terrain->getGridY() << "_" << terrain->getGridZ();
			ss << "_model";
			
			auto mesh = std::unique_ptr<TerrainMesh>( new TerrainMesh(openGlDevice_, ss.str()) );
			mesh->setVertices( vertices );
			mesh->setNormals( normals );
			mesh->setTextureBlendingData( textureBlendingValues );
			
			const std::string materialName = std::string("terrain_material_1");
			auto material = openGlDevice_->getMaterialManager()->getMaterial(materialName);
			if (material == nullptr)
			{
				material = openGlDevice_->getMaterialManager()->addMaterial(materialName);
				assert(material != nullptr);
				
				auto materialData = models::MaterialData();
				materialData.name = materialName;
				materialData.ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
				materialData.diffuse = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
				// None of these are actually used (yet)
				materialData.specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				materialData.emission = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
				materialData.shininess = 1.0f;
				materialData.strength = 1.0f;
				
				material->setAmbient(materialData.ambient);
				material->setDiffuse(materialData.diffuse);
				material->setSpecular(materialData.specular);
				material->setEmission(materialData.emission);
				material->setShininess(materialData.shininess);
				material->setStrength(materialData.strength);
			}
			
			terrain->attach(shader);
			
			auto meshPtr = mesh.get();
			terrain->setMesh( std::move(mesh) );
			
			//glmd::float32 posX = (glmd::float32)(terrain->getGridX()*constants::CHUNK_SIZE) * constants::RESOLUTION;
			//glmd::float32 posY = (glmd::float32)(terrain->getGridY()*constants::CHUNK_SIZE) * constants::RESOLUTION;
			//glmd::float32 posZ = (glmd::float32)(terrain->getGridZ()*constants::CHUNK_SIZE) * constants::RESOLUTION;
			
			// Note: We don't need to set position, as the model vertices already have the correct positions
			// TODO: Do we want to remove the positions from the vertices, and instead store the 'relative origin' position in the scene node (i.e. this Terrain object)?
			//std::cout << posX << ", " << posY << ", " << posZ << std::endl;
			//this->setPosition(posX, posY, posZ);
			
			// Center the node
			terrain->translate( glm::vec3(-(glmd::float32)(constants::CHUNK_SIZE/2), 0.0f, -(glmd::float32)(constants::CHUNK_SIZE/2)) );
			
			terrain->setIsActive( true );
			auto function = [=]() {
				LOG_DEBUG( "Pushing terrain to graphics card." );

				auto texture = openGlDevice_->getTextureManager()->getTexture2DArray(std::string("terrain_textures_2d_array"));
				if (texture == nullptr)
				{
					auto textureFilenames = std::vector<std::string>();
					textureFilenames.push_back( std::string("terrain/cgrass2.jpg") );
					textureFilenames.push_back( std::string("terrain/co_stone.jpg") );
					
					auto textureSettings = glr::glw::TextureSettings();
					textureSettings.textureWrapS = GL_REPEAT;
					textureSettings.textureWrapT = GL_REPEAT;
					
					texture = openGlDevice_->getTextureManager()->addTexture2DArray(std::string("terrain_textures_2d_array"), textureSettings);
					
					assert(texture != nullptr);
					
					const std::string& basepath = openGlDevice_->getOpenGlDeviceSettings().defaultTextureDir;
				
					utilities::ImageLoader il = utilities::ImageLoader();
					
					auto images = std::vector< std::unique_ptr<utilities::Image> >();
					for (auto& s : textureFilenames)
					{
						auto image = il.loadImageData(basepath + s);
						if (image.get() == nullptr)
						{
							// Cleanup
							images.clear();
							std::string msg = std::string( "Unable to load texture for texture 2d array: " + s );
							LOG_ERROR( msg );
							throw exception::Exception( msg );
						}
						
						images.push_back( std::move(image) );
					}
					
					auto imagesAsPointers = std::vector<utilities::Image*>();
					for (auto& image : images)
					{
						imagesAsPointers.push_back( image.get() );
					}
					
					texture->setData( imagesAsPointers );
					
					texture->allocateVideoMemory();
					texture->pushToVideoMemory();
				}
				
				// We wrap this pointer in a unique ptr eventually
				auto model = new models::Model(glr::Id(), std::string(""), meshPtr, texture, material, std::vector<glw::IAnimation*>(), glw::BoneNode(), glm::mat4(), openGlDevice_);
				
				// Create the model
				auto modelPtr = std::unique_ptr<models::IModel>( model );
				terrain->attach( modelPtr.get() );
				terrain->setModel( std::move(modelPtr) );
				
				// TODO: Load material only once
				if (material->getBufferId() == 0)
				{
					material->allocateVideoMemory();
					material->pushToVideoMemory();
				}
				
				GLint loc = shader->getVertexAttributeLocationByName( std::string("in_texBlend") );
				assert(loc >= 0);
				
				meshPtr->setShaderVariableLocation( loc );
				
				meshPtr->allocateVideoMemory();
				meshPtr->pushToVideoMemory();
				
				std::stringstream ss;
				ss << "terrain_" << terrain->getGridX() << "_" << terrain->getGridY() << "_" << terrain->getGridZ();
				terrain->setName( ss.str() );
				
				this->addTerrain( terrain );
			};

			postOpenGlWork( function );
		}
		else
		{
			delete terrain;
		}
		//std::cout << "createTerrain END " << std::this_thread::get_id() << std::endl;
	}
}

void TerrainManager::addTerrain(Terrain* terrain)
{
	terrainMutex_.lock();
	auto up = std::unique_ptr<Terrain>( terrain );
	terrain_.push_back( std::move(up) );
	terrainMutex_.unlock();
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

void TerrainManager::removeTerrain(glmd::int32 x, glmd::int32 y, glmd::int32 z)
{
	auto terrain = getTerrain(x, y, z);
	
	if (terrain != nullptr)
	{
		terrainMutex_.lock();
		
		if (terrain->isActive())
		{			
			terrain->setIsActive( false );
			
			// MARK: Previous lambda function
			{
				//std::cout << "remove: " << x << ", " << y << ", " << z << std::endl;
				//std::cout << "removeTerrain START " << std::this_thread::get_id() << std::endl;
				
				auto f = [=]() {
					terrain->freeVideoMemory();
					this->removeTerrain( terrain );
				};
				
				
				postOpenGlWork( f );
				//std::cout << "removeTerrain END " << std::this_thread::get_id() << std::endl;
				//this->openGlLoader_->postWork( std::bind(&Terrain::freeVideoMemory, c) );
			}
		}
		
		terrainMutex_.unlock();
	}
}

void TerrainManager::removeTerrain(Terrain* terrain)
{
	terrainMutex_.lock();
	
	auto findFunction = [terrain](const std::unique_ptr<Terrain>& node) { return node.get() == terrain; };
	
	auto it = std::find_if(terrain_.begin(), terrain_.end(), findFunction);
	
	if (it != terrain_.end())
	{
		terrain_.erase(it);
	}

	terrainMutex_.unlock();
}

void TerrainManager::removeAllTerrain()
{
	// TODO: Implement
	assert(0);
	idManager_ = IdManager();
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
	//std::cout << "getTerrain START " << std::this_thread::get_id() << std::endl;
	Terrain* retVal = nullptr;
	
	terrainMutex_.lock();
	
	auto findFunction = [x, y, z](const std::unique_ptr<Terrain>& node) { return (node->isActive() && node->getGridX() == x && node->getGridY() == y && node->getGridZ() == z); };
	
	auto it = std::find_if(terrain_.begin(), terrain_.end(), findFunction);
	
	if (it != terrain_.end())
	{
		retVal = it->get();
	}

	terrainMutex_.unlock();
	//std::cout << "getTerrain END " << std::this_thread::get_id() << std::endl;
	return retVal;
}

//bool saved = false;
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
	
	/*
	if (!saved && size == 0)
	{
		// Save all terrains
		serialize( std::string("testing.bin") );
		saved = true;
	}
	*/
}

void TerrainManager::render()
{
	terrainMutex_.lock();
	for ( auto& terrain : terrain_ )
	{
		if (terrain.get() != nullptr && terrain->isActive())
		{
			terrain->render();
		}
	}
	terrainMutex_.unlock();
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

void TerrainManager::generate()
{
	for (int i=0; i < terrainSettings_.length; i++)
	{
		for (int j=0; j < terrainSettings_.height; j++)
		{
			for (int k=0; k < terrainSettings_.width; k++)
			{
				createTerrain( i, j, k );
			}
		}
		std::cout << "MARK: " << i << std::endl;
	}

	// Save all terrains
	//serialize( std::string("testing.bin") );
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
	constants::CHUNK_SIZE;
}

}
}
