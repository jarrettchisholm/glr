/*
 * DefaultSceneManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DefaultSceneManager.h"

#include "CameraSceneNode.h"
#include "DefaultSceneNode.h"
#include "Light.h"
#include "models/ModelManager.h"
#include "exceptions/Exception.h"

namespace glr {

DefaultSceneManager::DefaultSceneManager(shaders::ShaderProgramManager* shaderProgramManager) : shaderProgramManager_(shaderProgramManager) {
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	modelManager_ = std::unique_ptr<models::IModelManager>( new models::ModelManager() );
	rootSceneNode_ = std::shared_ptr<ISceneNode>( new DefaultSceneNode() );
	
	// TESTING ONLY
	LightData ld;
	ld.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	lightData_.push_back(ld);
}

DefaultSceneManager::~DefaultSceneManager() {
}

ISceneNode* DefaultSceneManager::createSceneNode(const std::string name) {
	// Error check
	if (sceneNodes_.find(name) != sceneNodes_.end()) {
		std::stringstream msg;
		msg << "SceneNode with name '" << name << "' already exists.";
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}
	
	std::shared_ptr<ISceneNode> node = std::shared_ptr<ISceneNode>( new DefaultSceneNode(name) );
	sceneNodes_[name] = node;

	return node.get();
}

ICamera* DefaultSceneManager::createCamera(const std::string name, glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed) {
	// Error check
	if (cameras_.find(name) != cameras_.end()) {
		std::stringstream msg;
		msg << "Camera with name '" << name << "' already exists.";
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}
	
	std::shared_ptr<ICamera> node = std::shared_ptr<ICamera>( new CameraSceneNode(name) );	
	cameras_[name] = node;

	return node.get();
}

ILight* DefaultSceneManager::createLight( const std::string name ) {
	// Error check
	if (lights_.find(name) != lights_.end()) {
		std::stringstream msg;
		msg << "Light with name '" << name << "' already exists.";
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}
	
	std::shared_ptr<ILight> node = std::shared_ptr<ILight>( new Light(name) );	
	lights_[name] = node;

	return node.get();
}

void DefaultSceneManager::drawAll() {
	for (auto it = cameras_.begin(); it != cameras_.end(); ++it)
		it->second->render();
	
	for (auto it = sceneNodes_.begin(); it != sceneNodes_.end(); ++it)
		it->second->render();
}

ICamera* DefaultSceneManager::getActiveCameraSceneNode() {
	std::cout << "here2" << std::endl;
	if (cameras_.size() > 0)
		return cameras_.begin()->second.get();
		
	std::cout << "here3" << std::endl;
	return nullptr;
}

const glm::mat4& DefaultSceneManager::getModelMatrix() {
	return modelMatrix_;
}

models::IModelManager* DefaultSceneManager::getModelManager() {
	return modelManager_.get();
}

shaders::IShaderProgramManager* DefaultSceneManager::getShaderProgramManager() {
	return shaderProgramManager_;
}
	
ISceneNode* DefaultSceneManager::getSceneNode( const std::string& name ) {
	if (sceneNodes_.find(name) == sceneNodes_.end()) {
		return nullptr;
	}
	
	return sceneNodes_[name].get();
}

ICamera* DefaultSceneManager::getCamera( const std::string& name ) {
	if (cameras_.find(name) == cameras_.end()) {
		return nullptr;
	}
	
	return cameras_[name].get();
}

ILight* DefaultSceneManager::getLight( const std::string& name ) {
	if (lights_.find(name) == lights_.end()) {
		return nullptr;
	}
	
	return lights_[name].get();
}

const std::vector<LightData>& DefaultSceneManager::getLightData() {
	// TODO: implement
	return lightData_;
}

void DefaultSceneManager::destroySceneNode( const std::string& name ) {
	
}

void DefaultSceneManager::destroySceneNode( ISceneNode* node ) {
	
}

void DefaultSceneManager::destroyAllSceneNodes() {
	
}

void DefaultSceneManager::destroyCamera( const std::string& name ) {
	
}

void DefaultSceneManager::destroyCamera( ICamera* node ) {
	
}

void DefaultSceneManager::destroyAllCameras() {
	
}

void DefaultSceneManager::destroyLight( const std::string& name ) {
	
}

void DefaultSceneManager::destroyLight( ILight* node ) {
	
}

void DefaultSceneManager::destroyAllLights() {
	
}

glmd::uint32 DefaultSceneManager::getNumSceneNodes() {
	return sceneNodes_.size();
}

glmd::uint32 DefaultSceneManager::getNumCameras() {
	return cameras_.size();
}

glmd::uint32 DefaultSceneManager::getNumLights() {
	return lights_.size();
}


ISceneNode* DefaultSceneManager::getRootSceneNode() {
	return rootSceneNode_.get();
}


}
