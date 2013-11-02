/*
 * BasicSceneManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "BasicSceneManager.h"

#include "Camera.h"
#include "BasicSceneNode.h"
#include "Light.h"
#include "models/ModelManager.h"
#include "exceptions/Exception.h"

namespace glr {
BasicSceneManager::BasicSceneManager(shaders::IShaderProgramManager* shaderProgramManager, glw::IOpenGlDevice* openGlDevice) 
	: shaderProgramManager_(shaderProgramManager), openGlDevice_(openGlDevice)
{
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	
	modelManager_ = std::unique_ptr<models::IModelManager>(new models::ModelManager(openGlDevice_));
	
	rootSceneNode_ = std::shared_ptr<ISceneNode>(new BasicSceneNode(openGlDevice_));

	lightData_ = std::vector<LightData>();
	
	defaultShaderProgram_ = nullptr;
}

BasicSceneManager::~BasicSceneManager()
{
}

ISceneNode* BasicSceneManager::createSceneNode(const std::string name)
{
	// Error check
	if ( sceneNodes_.find(name) != sceneNodes_.end())
	{
		std::stringstream msg;
		msg << "SceneNode with name '" << name << "' already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ISceneNode> node = std::shared_ptr<ISceneNode>(new BasicSceneNode(name, openGlDevice_));
	sceneNodes_[name] = node;

	node->attach(defaultShaderProgram_);

	return node.get();
}

ICamera* BasicSceneManager::createCamera(const std::string name, glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed)
{
	// Error check
	if ( cameras_.find(name) != cameras_.end())
	{
		std::stringstream msg;
		msg << "Camera with name '" << name << "' already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ICamera> node = std::shared_ptr<ICamera>(new Camera(name, openGlDevice_));
	cameras_[name] = node;

	node->attach(defaultShaderProgram_);

	return node.get();
}

ILight* BasicSceneManager::createLight(const std::string name)
{
	// Error check
	if ( lights_.find(name) != lights_.end())
	{
		std::stringstream msg;
		msg << "Light with name '" << name << "' already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ILight> node = std::shared_ptr<ILight>(new Light(name, openGlDevice_));
	lights_[name] = node;

	return node.get();
}

void BasicSceneManager::drawAll()
{
	//defaultShaderProgram_->bind();
	
	for ( auto it = cameras_.begin(); it != cameras_.end(); ++it )
		it->second->render();
		
	for ( auto it = lights_.begin(); it != lights_.end(); ++it )
		it->second->render();

	for ( auto it = sceneNodes_.begin(); it != sceneNodes_.end(); ++it )
		it->second->render();
}

void BasicSceneManager::addCamera(std::shared_ptr<ICamera> camera)
{
	// Error check
	if ( cameras_.find(camera->getName()) != cameras_.end())
	{
		std::stringstream msg;
		msg << "Camera with name '" << camera->getName() << "' already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ICamera> node = camera;
	cameras_[camera->getName()] = node;

	node->attach(defaultShaderProgram_);
}

void BasicSceneManager::addSceneNode(std::shared_ptr<ISceneNode> sceneNode)
{
	// Error check
	if ( sceneNodes_.find(sceneNode->getName()) != sceneNodes_.end())
	{
		std::stringstream msg;
		msg << "SceneNode with name '" << sceneNode->getName() << "' already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ISceneNode> node = sceneNode;
	sceneNodes_[sceneNode->getName()] = node;

	// TODO: should we attach a default if no shader exists for scene node?
	//node->attach(defaultShaderProgram_);
}

void BasicSceneManager::setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram)
{
	defaultShaderProgram_ = shaderProgram;
}

shaders::IShaderProgram* BasicSceneManager::getDefaultShaderProgram()
{
	return defaultShaderProgram_;
}

ICamera* BasicSceneManager::getActiveCamera()
{

	if ( cameras_.size() > 0 )
		return cameras_.begin()->second.get();
		
	return nullptr;
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

ISceneNode* BasicSceneManager::getSceneNode(const std::string& name)
{
	if ( sceneNodes_.find(name) == sceneNodes_.end())
	{
		return nullptr;
	}

	return sceneNodes_[name].get();
}

ICamera* BasicSceneManager::getCamera(const std::string& name)
{
	if ( cameras_.find(name) == cameras_.end())
	{
		return nullptr;
	}

	return cameras_[name].get();
}

ILight* BasicSceneManager::getLight(const std::string& name)
{
	if ( lights_.find(name) == lights_.end())
	{
		return nullptr;
	}

	return lights_[name].get();
}

const std::vector<LightData>& BasicSceneManager::getLightData()
{
	lightData_ = std::vector<LightData>();
	
	for (auto it=lights_.begin(); it != lights_.end(); it++)
	{
		lightData_.push_back(it->second->getLightData());
	}
	
	return lightData_;
}

void BasicSceneManager::destroySceneNode(const std::string& name)
{
	sceneNodes_.erase( name );
}

void BasicSceneManager::destroySceneNode(ISceneNode* node)
{
	for (auto it=sceneNodes_.begin(); it != sceneNodes_.end(); it++)
	{
		if (it->second.get() == node)
		{
			sceneNodes_.erase(it);
			return;
		}
	}
}

void BasicSceneManager::destroyAllSceneNodes()
{
	sceneNodes_.clear();
}

void BasicSceneManager::destroyCamera(const std::string& name)
{
	cameras_.erase( name );
}

void BasicSceneManager::destroyCamera(ICamera* node)
{
	for (auto it=cameras_.begin(); it != cameras_.end(); it++)
	{
		if (it->second.get() == node)
		{
			cameras_.erase(it);
			return;
		}
	}
}

void BasicSceneManager::destroyAllCameras()
{
	cameras_.clear();
}

void BasicSceneManager::destroyLight(const std::string& name)
{
	lights_.erase( name );
}

void BasicSceneManager::destroyLight(ILight* node)
{
	for (auto it=lights_.begin(); it != lights_.end(); it++)
	{
		if (it->second.get() == node)
		{
			lights_.erase(it);
			return;
		}
	}
}

void BasicSceneManager::destroyAllLights()
{
	lights_.clear();
}

glmd::uint32 BasicSceneManager::getNumSceneNodes()
{
	return sceneNodes_.size();
}

glmd::uint32 BasicSceneManager::getNumCameras()
{
	return cameras_.size();
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
