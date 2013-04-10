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

#include "CameraSceneNode.h"
#include "BasicSceneNode.h"
#include "Light.h"
#include "models/ModelManager.h"
#include "exceptions/Exception.h"

namespace glr {
BasicSceneManager::BasicSceneManager(shaders::ShaderProgramManager* shaderProgramManager) : shaderProgramManager_(shaderProgramManager)
{
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	modelManager_ = std::unique_ptr<models::IModelManager>(new models::ModelManager());
	rootSceneNode_ = std::shared_ptr<ISceneNode>(new BasicSceneNode());

	// TESTING ONLY
	LightData ld;
	ld.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	lightData_.push_back(ld);
	
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
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ISceneNode> node = std::shared_ptr<ISceneNode>(new BasicSceneNode(name));
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
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ICamera> node = std::shared_ptr<ICamera>(new CameraSceneNode(name));
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
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}

	std::shared_ptr<ILight> node = std::shared_ptr<ILight>(new Light(name));
	lights_[name] = node;

	return node.get();
}

void BasicSceneManager::drawAll(IMatrixData* matrixData)
{
	for ( auto it = cameras_.begin(); it != cameras_.end(); ++it )
		it->second->render(matrixData);

	for ( auto it = sceneNodes_.begin(); it != sceneNodes_.end(); ++it )
		it->second->render(matrixData);
}

void BasicSceneManager::setDefaultShaderProgram(shaders::IShaderProgram* shaderProgram)
{
	defaultShaderProgram_ = shaderProgram;
}

ICamera* BasicSceneManager::getActiveCameraSceneNode()
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
	// TODO: implement
	return lightData_;
}

void BasicSceneManager::destroySceneNode(const std::string& name)
{
}

void BasicSceneManager::destroySceneNode(ISceneNode* node)
{
}

void BasicSceneManager::destroyAllSceneNodes()
{
}

void BasicSceneManager::destroyCamera(const std::string& name)
{
}

void BasicSceneManager::destroyCamera(ICamera* node)
{
}

void BasicSceneManager::destroyAllCameras()
{
}

void BasicSceneManager::destroyLight(const std::string& name)
{
}

void BasicSceneManager::destroyLight(ILight* node)
{
}

void BasicSceneManager::destroyAllLights()
{
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
