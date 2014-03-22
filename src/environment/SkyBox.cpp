#include "environment/SkyBox.hpp"

#include "common/logger/Logger.hpp"

#include "models/Model.hpp"

namespace glr
{
namespace env
{

SkyBox::SkyBox(Id id, glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager) : BasicSceneNode(id, openGlDevice)
{
	initialize(modelManager);
}

SkyBox::SkyBox(Id id, std::string name, glw::IOpenGlDevice* openGlDevice, models::IModelManager* modelManager)
	: BasicSceneNode(id, std::move(name), openGlDevice)
{
	initialize(modelManager);
}

SkyBox::~SkyBox()
{
}

void SkyBox::render()
{
	for ( auto& node : sides_ )
		node->render();
}

void SkyBox::move( const glm::vec3& movement )
{
	for ( auto& node : sides_ )
		node->move( movement );
}

void SkyBox::initialize(models::IModelManager* modelManager)
{
	assert(modelManager != nullptr);
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	auto meshManager = openGlDevice_->getMeshManager();
	auto textureManager = openGlDevice_->getTextureManager();
	auto materialManager = openGlDevice_->getMaterialManager();
	
	sides_ = std::vector< std::unique_ptr<SkyBoxPlane> >();
	shaders::IShaderProgram* shader = openGlDevice_->getShaderProgramManager()->getShaderProgram( std::string("sky_box") );
	assert( shader != nullptr );
	
	glmd::float32 radius = 300.0f;
	glmd::float32 cz = 0.0f;
	glmd::float32 cx = 1.0f;
	
	auto material = materialManager->addMaterial(
		std::string("basic"),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		1.0f
	);

	assert(material != nullptr);

	// Front
	auto vertices = std::vector< glm::vec3 >();
	auto textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(-radius, 0.0f, -radius) );
	vertices.push_back( glm::vec3(-radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, -radius) );
	vertices.push_back( glm::vec3(-radius, 0.0f, -radius) );
	
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	
	auto mesh = meshManager->addMesh( std::string("skybox_front"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	auto texture = textureManager->addTexture2D( std::string("sky_box/skybox_front.bmp"), std::string("sky_box/skybox_front.bmp") );
	
	assert(mesh != nullptr);
	assert(texture != nullptr);
	
	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_front_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_front"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );
		node->translate( glm::vec3(0.0f, 0.0f, radius) );
		node->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
		node->rotate( 180.0f, glm::vec3(0.0f, 0.0f, 1.0f) );
		node->rotate( 180.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
	}

	
	// Back
	vertices = std::vector< glm::vec3 >();
	textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(-radius, 0.0f, -radius) );
	vertices.push_back( glm::vec3(-radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, radius) );
	vertices.push_back( glm::vec3(radius, 0.0f, -radius) );
	vertices.push_back( glm::vec3(-radius, 0.0f, -radius) );
	
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	
	mesh = meshManager->addMesh( std::string("skybox_back"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	texture = textureManager->addTexture2D( std::string("sky_box/skybox_back.bmp"), std::string("sky_box/skybox_back.bmp") );

	assert(mesh != nullptr);
	assert(texture != nullptr);

	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_back_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_back"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );		
		node->translate( glm::vec3(0.0f, 0.0f, -radius) );
		node->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
		node->rotate( 180.0f, glm::vec3(0.0f, 0.0f, 1.0f) );
	}

	
	// Left
	vertices = std::vector< glm::vec3 >();
	textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(0.0f, -radius, radius) );
	vertices.push_back( glm::vec3(0.0f, radius, radius) );
	vertices.push_back( glm::vec3(0.0f, radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, -radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, -radius, radius) );
	
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	
	mesh = meshManager->addMesh( std::string("skybox_left"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	texture = textureManager->addTexture2D( std::string("sky_box/skybox_left.bmp"), std::string("sky_box/skybox_left.bmp") );

	assert(mesh != nullptr);
	assert(texture != nullptr);

	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_left_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_left"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );
		node->translate( glm::vec3(-radius, 0.0f, 0.0f) );
		node->rotate( -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
		node->rotate( 180.0f, glm::vec3(0.0f, 1.0f, 0.0f) );
	}
	
	
	// Right
	vertices = std::vector< glm::vec3 >();
	textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(0.0f, -radius, radius) );
	vertices.push_back( glm::vec3(0.0f, radius, radius) );
	vertices.push_back( glm::vec3(0.0f, radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, -radius, -radius) );
	vertices.push_back( glm::vec3(0.0f, -radius, radius) );
	
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	
	mesh = meshManager->addMesh( std::string("skybox_right"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	texture = textureManager->addTexture2D( std::string("sky_box/skybox_right.bmp"), std::string("sky_box/skybox_right.bmp") );

	assert(mesh != nullptr);
	assert(texture != nullptr);

	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_right_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_right"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );
		node->translate( glm::vec3(radius, 0.0f, 0.0f) );
		node->rotate( -90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	}
	
	
	// Top
	vertices = std::vector< glm::vec3 >();
	textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(radius, -radius, 0.0f) );
	vertices.push_back( glm::vec3(radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, -radius, 0.0f) );
	vertices.push_back( glm::vec3(radius, -radius, 0.0f) );
	
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	
	mesh = meshManager->addMesh( std::string("skybox_top"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	texture = textureManager->addTexture2D( std::string("sky_box/skybox_top.bmp"), std::string("sky_box/skybox_top.bmp") );

	assert(mesh != nullptr);
	assert(texture != nullptr);

	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_top_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_top"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );
		node->translate( glm::vec3(0.0f, radius, 0.0f) );
		node->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
	}
	
	
	// Bottom
	vertices = std::vector< glm::vec3 >();
	textureCoordinates = std::vector< glm::vec2 >();
	
	vertices.push_back( glm::vec3(radius, -radius, 0.0f) );
	vertices.push_back( glm::vec3(radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, radius, 0.0f) );
	vertices.push_back( glm::vec3(-radius, -radius, 0.0f) );
	vertices.push_back( glm::vec3(radius, -radius, 0.0f) );
	
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cz) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cx, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cx) );
	textureCoordinates.push_back( glm::vec2(cz, cz) );
	
	mesh = meshManager->addMesh( std::string("skybox_bottom"), vertices, std::vector< glm::vec3 >(), textureCoordinates, std::vector< glm::vec4 >() );
	texture = textureManager->addTexture2D( std::string("sky_box/skybox_bottom.bmp"), std::string("sky_box/skybox_bottom.bmp") );

	assert(mesh != nullptr);
	assert(texture != nullptr);

	{
		// Create the model
		models_.push_back( std::unique_ptr<models::IModel>(new models::Model(glr::Id::INVALID, std::string("skybox_bottom_model"), mesh, texture, material, openGlDevice_)) );
		
		// Add side to the skybox
		sides_.push_back( std::unique_ptr<SkyBoxPlane>(new SkyBoxPlane(glr::Id::INVALID, std::string("skybox_bottom"), openGlDevice_)) );
		
		SkyBoxPlane* node = sides_.back().get();
		
		node->attach(shader);
		node->attach( models_.back().get() );
		node->translate( glm::vec3(0.0f, -radius, 0.0f) );
		node->rotate( 90.0f, glm::vec3(1.0f, 0.0f, 0.0f) );
		node->rotate( 180.0f, glm::vec3(0.0f, 0.0f, 1.0f) );
	}

	LOG_DEBUG( "SkyBox initialized." );
}

}
}
