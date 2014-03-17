#include <sstream>
#include <utility>
#include <algorithm>
#include <functional>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BasicSceneManager.hpp"

#include "Camera.hpp"
#include "BasicSceneNode.hpp"
#include "Light.hpp"
#include "models/ModelManager.hpp"
#include "models/BillboardManager.hpp"
#include "exceptions/RuntimeException.hpp"

namespace glr
{

BasicSceneManager::BasicSceneManager(shaders::IShaderProgramManager* shaderProgramManager, glw::IOpenGlDevice* openGlDevice) 
	: shaderProgramManager_(shaderProgramManager), openGlDevice_(openGlDevice)
{
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	
	modelManager_ = std::unique_ptr<models::IModelManager>(new models::ModelManager(openGlDevice_));
	billboardManager_ = std::unique_ptr<models::IBillboardManager>(new models::BillboardManager(openGlDevice_));

	lightData_ = std::vector<LightData>();
	
	defaultShaderProgram_ = nullptr;

	idManager_ = IdManager();
}

BasicSceneManager::~BasicSceneManager()
{
}

ISceneNode* BasicSceneManager::createSceneNode(const std::string& name)
{
	sceneNodes_.push_back( std::unique_ptr<ISceneNode>(new BasicSceneNode(idManager_.createId(), name, openGlDevice_)) );
	
	auto node = sceneNodes_.back().get();
	node->attach(defaultShaderProgram_);

	return node;
}

ICamera* BasicSceneManager::createCamera(glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed)
{
	if (camera_.get() != nullptr)
	{
		std::stringstream msg;
		msg << "A Camera already exists.";
		LOG_ERROR( msg.str() );
		throw exception::RuntimeException(msg.str());
	}
	
	camera_ = std::unique_ptr<ICamera>(new Camera(idManager_.createId(), openGlDevice_));

	camera_->attach(defaultShaderProgram_);

	return camera_.get();
}

ILight* BasicSceneManager::createLight(const std::string& name)
{
	lights_.push_back( std::unique_ptr<ILight>(new Light(idManager_.createId(), name, openGlDevice_)) );
	
	auto node = lights_.back().get();

	return node;
}

void BasicSceneManager::drawAll()
{
	//defaultShaderProgram_->bind();
	
	if (camera_.get() != nullptr)
		camera_->render();
		
	for ( auto& light : lights_ )
		light->render();

	for ( auto& node : sceneNodes_ )
		node->render();
}

void BasicSceneManager::setCamera(std::unique_ptr<ICamera> camera)
{
	assert(camera.get() != nullptr);
	
	if (camera_.get() != nullptr)
	{
		std::stringstream msg;
		msg << "A Camera already exists.";
		LOG_ERROR( msg.str() );
		throw exception::RuntimeException(msg.str());
	}

	camera_ = std::move(camera);

	camera_->attach(defaultShaderProgram_);
}

void BasicSceneManager::setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram)
{
	defaultShaderProgram_ = shaderProgram;
}

shaders::IShaderProgram* BasicSceneManager::getDefaultShaderProgram() const
{
	return defaultShaderProgram_;
}

const glm::mat4& BasicSceneManager::getModelMatrix() const
{
	return modelMatrix_;
}

models::IBillboardManager* BasicSceneManager::getBillboardManager() const
{
	return billboardManager_.get();
}

models::IModelManager* BasicSceneManager::getModelManager() const
{
	return modelManager_.get();
}

shaders::IShaderProgramManager* BasicSceneManager::getShaderProgramManager() const
{
	return shaderProgramManager_;
}

ISceneNode* BasicSceneManager::getSceneNode(Id id) const
{	
	auto findFunction = [id](const std::unique_ptr<ISceneNode>& node) { return node->getId() == id; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if ( it == sceneNodes_.end())
		return nullptr;

	return it->get();
}

ISceneNode* BasicSceneManager::getSceneNode(const std::string& name) const
{	
	auto findFunction = [&name](const std::unique_ptr<ISceneNode>& node) { return node->getName() == name; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if ( it == sceneNodes_.end())
		return nullptr;

	return it->get();
}

ICamera* BasicSceneManager::getCamera() const
{
	return camera_.get();
}

ILight* BasicSceneManager::getLight(Id id) const
{
	auto findFunction = [id](const std::unique_ptr<ILight>& node) { return node->getId() == id; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if ( it == lights_.end())
		return nullptr;

	return it->get();
}

ILight* BasicSceneManager::getLight(const std::string& name) const
{
	auto findFunction = [&name](const std::unique_ptr<ILight>& node) { return node->getName() == name; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if ( it == lights_.end())
		return nullptr;

	return it->get();
}

const std::vector<LightData>& BasicSceneManager::getLightData()
{
	lightData_ = std::vector<LightData>();
	
	for (auto& light : lights_)
		lightData_.push_back(light->getLightData());
	
	return lightData_;
}

void BasicSceneManager::destroySceneNode(Id id)
{
	auto findFunction = [id](const std::unique_ptr<ISceneNode>& node) { return node->getId() == id; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroySceneNode(const std::string& name)
{
	auto findFunction = [&name](const std::unique_ptr<ISceneNode>& node) { return node->getName() == name; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroySceneNode(ISceneNode* node)
{
	auto findFunction = [node](const std::unique_ptr<ISceneNode>& n) { return n.get() == node; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroyAllSceneNodes()
{
	sceneNodes_.clear();
}

void BasicSceneManager::destroyCamera()
{	
	camera_.reset();
}

void BasicSceneManager::destroyLight(Id id)
{
	auto findFunction = [id](const std::unique_ptr<ILight>& node) { return node->getId() == id; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyLight(const std::string& name)
{
	auto findFunction = [&name](const std::unique_ptr<ILight>& node) { return node->getName() == name; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyLight(ILight* node)
{
	auto findFunction = [node](const std::unique_ptr<ILight>& n) { return n.get() == node; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyAllLights()
{
	lights_.clear();
}

glmd::uint32 BasicSceneManager::getNumSceneNodes() const
{
	return sceneNodes_.size();
}

glmd::uint32 BasicSceneManager::getNumLights() const
{
	return lights_.size();
}

}
