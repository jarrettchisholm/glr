#include <sstream>
#include <algorithm>
#include <functional>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BasicSceneManager.h"

#include "Camera.h"
#include "BasicSceneNode.h"
#include "Light.h"
#include "models/ModelManager.h"
#include "exceptions/RuntimeException.h"

namespace glr
{

BasicSceneManager::BasicSceneManager(shaders::IShaderProgramManager* shaderProgramManager, glw::IOpenGlDevice* openGlDevice) 
	: shaderProgramManager_(shaderProgramManager), openGlDevice_(openGlDevice)
{
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	
	modelManager_ = std::unique_ptr<models::IModelManager>(new models::ModelManager(openGlDevice_));
	
	rootSceneNode_ = std::shared_ptr<ISceneNode>(new BasicSceneNode(openGlDevice_));

	lightData_ = std::vector<LightData>();
	
	defaultShaderProgram_ = nullptr;
	
	nextSceneNodeId_ = 0;
	nextLightSceneNodeId_ = 0;
}

BasicSceneManager::~BasicSceneManager()
{
}

ISceneNode* BasicSceneManager::createSceneNode(const std::string& name)
{
	auto node = std::shared_ptr<ISceneNode>(new BasicSceneNode(nextSceneNodeId_, name, openGlDevice_));
	sceneNodes_.push_back(node);

	node->attach(defaultShaderProgram_);

	nextSceneNodeId_++;

	return node.get();
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
	
	camera_ = std::shared_ptr<ICamera>(new Camera(openGlDevice_));

	camera_->attach(defaultShaderProgram_);

	return camera_.get();
}

ILight* BasicSceneManager::createLight(const std::string& name)
{
	auto node = std::shared_ptr<ILight>(new Light(nextLightSceneNodeId_, name, openGlDevice_));
	lights_.push_back(node);
	
	nextLightSceneNodeId_++;

	return node.get();
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

void BasicSceneManager::addCamera(std::shared_ptr<ICamera> camera)
{
	assert(camera.get() != nullptr);
	
	if (camera_.get() != nullptr)
	{
		std::stringstream msg;
		msg << "A Camera already exists.";
		LOG_ERROR( msg.str() );
		throw exception::RuntimeException(msg.str());
	}

	camera->setId(0);
	
	camera_ = camera;
	
	camera->attach(defaultShaderProgram_);
}

void BasicSceneManager::addSceneNode(std::shared_ptr<ISceneNode> sceneNode)
{
	assert(sceneNode.get() != nullptr);
	
	// Error check
	if (sceneNode->getId() != ISceneNode::INVALID_ID)
	{
		std::stringstream msg;
		msg << "SceneNode already has an id: '" << sceneNode->getId() << "'.";
		LOG_ERROR( msg.str() );
		throw exception::RuntimeException(msg.str());
	}

	sceneNode->setId(nextSceneNodeId_);
	
	nextSceneNodeId_++;

	sceneNodes_.push_back(sceneNode);

	// TODO: should we attach a default if no shader exists for scene node?
	//sceneNode->attach(defaultShaderProgram_);
}

void BasicSceneManager::setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram)
{
	defaultShaderProgram_ = shaderProgram;
}

shaders::IShaderProgram* BasicSceneManager::getDefaultShaderProgram()
{
	return defaultShaderProgram_;
}

const glm::mat4& BasicSceneManager::getModelMatrix()
{
	return modelMatrix_;
}

models::IModelManager* BasicSceneManager::getModelManager()
{
	return modelManager_.get();
}

shaders::IShaderProgramManager* BasicSceneManager::getShaderProgramManager()
{
	return shaderProgramManager_;
}

ISceneNode* BasicSceneManager::getSceneNode(glm::detail::uint32 id)
{	
	auto findFunction = [id](const std::shared_ptr<ISceneNode>& node) { return node->getId() == id; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if ( it == sceneNodes_.end())
		return nullptr;

	return it->get();
}

ISceneNode* BasicSceneManager::getSceneNode(const std::string& name)
{	
	auto findFunction = [&name](const std::shared_ptr<ISceneNode>& node) { return node->getName() == name; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if ( it == sceneNodes_.end())
		return nullptr;

	return it->get();
}

ICamera* BasicSceneManager::getCamera()
{
	return camera_.get();
}

ILight* BasicSceneManager::getLight(glm::detail::uint32 id)
{
	auto findFunction = [id](const std::shared_ptr<ILight>& node) { return node->getId() == id; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if ( it == lights_.end())
		return nullptr;

	return it->get();
}

ILight* BasicSceneManager::getLight(const std::string& name)
{
	auto findFunction = [&name](const std::shared_ptr<ILight>& node) { return node->getName() == name; };
	
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

void BasicSceneManager::destroySceneNode(glm::detail::uint32 id)
{
	auto findFunction = [id](const std::shared_ptr<ISceneNode>& node) { return node->getId() == id; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroySceneNode(const std::string& name)
{
	auto findFunction = [&name](const std::shared_ptr<ISceneNode>& node) { return node->getName() == name; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroySceneNode(ISceneNode* node)
{
	auto findFunction = [node](const std::shared_ptr<ISceneNode>& n) { return n.get() == node; };
	
	auto it = std::find_if(sceneNodes_.begin(), sceneNodes_.end(), findFunction);
	
	if (it != sceneNodes_.end())
		sceneNodes_.erase( it );
}

void BasicSceneManager::destroyAllSceneNodes()
{
	sceneNodes_.clear();
	nextSceneNodeId_ = 0;
}

void BasicSceneManager::destroyCamera()
{
	if (!camera_.unique())
		LOG_WARN( "Camera object is still referenced somewhere else - cannot fully destroy camera." );
	
	camera_.reset();
}

void BasicSceneManager::destroyLight(glm::detail::uint32 id)
{
	auto findFunction = [id](const std::shared_ptr<ILight>& node) { return node->getId() == id; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyLight(const std::string& name)
{
	auto findFunction = [&name](const std::shared_ptr<ILight>& node) { return node->getName() == name; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyLight(ILight* node)
{
	auto findFunction = [node](const std::shared_ptr<ILight>& n) { return n.get() == node; };
	
	auto it = std::find_if(lights_.begin(), lights_.end(), findFunction);
	
	if (it != lights_.end())
		lights_.erase( it );
}

void BasicSceneManager::destroyAllLights()
{
	lights_.clear();
	nextLightSceneNodeId_ = 0;
}

glmd::uint32 BasicSceneManager::getNumSceneNodes()
{
	return sceneNodes_.size();
}

glmd::uint32 BasicSceneManager::getNumLights()
{
	return lights_.size();
}

ISceneNode* BasicSceneManager::getRootSceneNode()
{
	return rootSceneNode_.get();
}

}
