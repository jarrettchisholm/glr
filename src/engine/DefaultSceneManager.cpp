/*
 * DefaultSceneManager.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "DefaultSceneManager.h"

#include "CameraSceneNode.h"
#include "DefaultSceneNode.h"
#include "models/ModelManager.h"

namespace oglre {

namespace engine {

DefaultSceneManager::DefaultSceneManager() {
}

DefaultSceneManager::~DefaultSceneManager() {
}

/**
 * FOR TESTING PURPOSES ONLY.
 */
ISceneNode* DefaultSceneManager::createDefaultSceneNode(const std::string name, glm::vec3 position, glm::vec3 lookAt) {
	DefaultSceneNode* testNode = new DefaultSceneNode();
	sceneNodes_.push_back(testNode);

	return testNode;
	//return 0;
}

ISceneNode* DefaultSceneManager::createSceneNode(const std::string name, glm::vec3 position, glm::vec3 lookAt) {
	DefaultSceneNode* testNode = new DefaultSceneNode();
	sceneNodes_.push_back(testNode);

	return testNode;
}

ICameraSceneNode* DefaultSceneManager::createCamera(const std::string name, glm::vec3 position, glm::vec3 lookAt, glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed) {
	CameraSceneNode* node = new CameraSceneNode(position, lookAt);

	std::vector<ISceneNode*>::iterator it;

	it = sceneNodes_.begin();
	it = sceneNodes_.insert(it, node);

	//sceneNodes_.push_back(node);

	return node;
}

ICameraSceneNode* DefaultSceneManager::createCameraFPS(const std::string name, glm::vec3 position, glm::vec3 lookAt, glm::detail::uint32 speed, glm::detail::uint32 rotationSpeed) {
	CameraSceneNode* node = new CameraSceneNode();
	sceneNodes_.push_back(node);

	return node;
}

void DefaultSceneManager::drawAll() {
	for (glm::detail::uint32 i = 0; i < sceneNodes_.size(); i++) {
		sceneNodes_[i]->render();
	}
}

models::IModelManager* DefaultSceneManager::getModelManager() {
	return models::ModelManager::getInstance();
}

}

}
