#include <utility>
#include <sstream>
#include <iostream>

#include "terrain/Terrain.hpp"
#include "terrain/Constants.hpp"

#include "models/IModel.hpp"
#include "models/Model.hpp"

#include "terrain/TerrainMesh.hpp"
#include "terrain/IVoxelChunkMeshGenerator.hpp"
#include "terrain/VoxelChunkNoiseGenerator.hpp"

#include "glw/IMaterialManager.hpp"
#include "glw/ITextureManager.hpp"

#include "glw/IMaterial.hpp"
#include "glw/ITexture.hpp"

#include "models/MaterialData.hpp"

#include "exceptions/InvalidArgumentException.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace terrain
{

Terrain::Terrain(Id id, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ, 
	glmd::int32 length, glmd::int32 width, glmd::int32 height, IFieldFunction* fieldFunction, IVoxelChunkMeshGenerator* voxelChunkMeshGenerator)
		: BasicSceneNode(id, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ), 
			length_(length), width_(width), height_(height), fieldFunction_(fieldFunction), voxelChunkMeshGenerator_(voxelChunkMeshGenerator)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glw::IOpenGlDevice* openGlDevice, glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, std::move(name), openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ), 
		length_(0), width_(0), height_(0), fieldFunction_(nullptr), voxelChunkMeshGenerator_(nullptr)
{
	initialize();
}

Terrain::Terrain(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice, 
	glmd::int32 gridX, glmd::int32 gridY, glmd::int32 gridZ)
	: BasicSceneNode(id, std::move(name), position, orientation, scale, openGlDevice), gridX_(gridX), gridY_(gridY), gridZ_(gridZ), 
		length_(0), width_(0), height_(0), fieldFunction_(nullptr), voxelChunkMeshGenerator_(nullptr)
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

void Terrain::prepareOrUpdateGraphics()
{
	if (isDirty_)
	{
		initializeShaderLocations();
		
		if ( !modelPtr_->isVideoMemoryAllocated() )
		{
			modelPtr_->allocateVideoMemory();
		}
		
		modelPtr_->pushToVideoMemory();
		
		this->setIsDirty( false );
	}
}

void Terrain::initializeShaderLocations()
{
	GLint loc = shaderProgram_->getVertexAttributeLocationByName( std::string("in_texBlend") );
	assert(loc >= 0);
	
	meshData_->setShaderVariableLocation( loc );
}

void Terrain::freeVideoMemory()
{
	if (modelPtr_.get() != nullptr)
	{
		modelPtr_->freeVideoMemory();
	}
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

void Terrain::updateLod(LevelOfDetail lod)
{
	if (levelOfDetail_ == lod)
	{
		return;
	}
	
	levelOfDetail_ = lod;
	
	// TODO: Actually prepare lod mesh, etc.
	
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

models::IModel* Terrain::getModel() const
{
	return modelPtr_.get();
}

void Terrain::generate(TerrainSettings settings)
{
	if (fieldFunction_ == nullptr)
	{
		std::string msg = "Unable to generate terrain - no field function set.";
		LOG_ERROR(msg);
		throw exception::InvalidArgumentException(msg);
	}
	else if (voxelChunkMeshGenerator_ == nullptr)
	{
		std::string msg = "Unable to generate terrain - no voxel chunk mesh generator set.";
		LOG_ERROR(msg);
		throw exception::InvalidArgumentException(msg);
	}
	
	VoxelChunk voxelChunk = VoxelChunk(getGridX(), getGridY(), getGridZ());

	glr::terrain::generateNoise(voxelChunk, length_, width_, height_, *fieldFunction_);
	
	this->isEmptyOrSolid_ = glr::terrain::determineIfEmptyOrSolid(voxelChunk);
	
	if (this->isEmptyOrSolid_)
	{
		LOG_DEBUG("Terrain at (" << getGridX() << ", " << getGridY() << ", " << getGridZ() << ") is empty or solid - skipping mesh generation.");
		return;
	}
	
	auto vertices = std::vector< glm::vec3 >();
	auto normals = std::vector< glm::vec3 >();
	auto textureBlendingValues = std::vector< glm::vec4 >();
	
	voxelChunkMeshGenerator_->generateMesh(voxelChunk, length_, width_, height_, vertices, normals, textureBlendingValues);
	
	std::stringstream ss;
	ss << "terrain_" << this->getGridX() << "_" << this->getGridY() << "_" << this->getGridZ();
	ss << "_model";
	
	this->setMesh( std::unique_ptr<TerrainMesh>( new TerrainMesh(openGlDevice_, ss.str()) ) );
	this->translate( glm::vec3(-(glmd::float32)(settings.chunkSize/2), 0.0f, -(glmd::float32)(settings.chunkSize/2)) );
	
	meshData_->setVertices( vertices );
	meshData_->setNormals( normals );
	meshData_->setTextureBlendingData( textureBlendingValues );
	
	const std::string materialName = std::string("terrain_material_1");
	auto material = openGlDevice_->getMaterialManager()->getMaterial(materialName);
	if (material == nullptr)
	{
		// Should be safe to call this outside opengl thread
		material = openGlDevice_->getMaterialManager()->addMaterial(materialName);
		assert(material != nullptr);
		
		material->setAmbient( glm::vec4(0.2f, 0.2f, 0.2f, 1.0f) );
		material->setDiffuse( glm::vec4(0.2f, 0.2f, 0.2f, 1.0f) );
		// None of these are actually used (yet)
		material->setSpecular( glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		material->setEmission( glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		material->setShininess( 1.0f );
		material->setStrength( 1.0f );
	}
	
	auto texture = openGlDevice_->getTextureManager()->getTexture2DArray(std::string("terrain_textures_2d_array"));
	if (texture == nullptr)
	{
		auto textureFilenames = std::vector<std::string>();
		textureFilenames.push_back( std::string("terrain/cgrass2.jpg") );
		textureFilenames.push_back( std::string("terrain/co_stone.jpg") );
		
		auto textureSettings = glr::glw::TextureSettings();
		textureSettings.textureWrapS = GL_REPEAT;
		textureSettings.textureWrapT = GL_REPEAT;
		
		// Should be safe to call this outside opengl thread as we set 'initialize' to false
		texture = openGlDevice_->getTextureManager()->addTexture2DArray(std::string("terrain_textures_2d_array"), textureFilenames, textureSettings, false);
		assert(texture != nullptr);
	}
	
	// Create the model
	auto model = std::unique_ptr<models::IModel>( new models::Model(glr::Id(), std::string(""), meshData_.get(), texture, material, std::vector<glw::IAnimation*>(), glw::BoneNode(), glm::mat4(), openGlDevice_) );
	this->attach( model.get() );
	this->setModel( std::move(model) );
	
	this->setIsDirty( true );
}

}
}
