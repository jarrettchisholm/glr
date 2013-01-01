/*
 * DefaultSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "DefaultSceneNode.h"

#ifdef _WIN32
#include <windows.h>
#endif

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include <stdlib.h>

namespace oglre {

namespace engine {

DefaultSceneNode::DefaultSceneNode() {
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);

	active_ = true;
	isVisible_ = true;
	
	model_ = nullptr;
}

DefaultSceneNode::~DefaultSceneNode() {
}

void DefaultSceneNode::attach(IModel* model) {
	model_ = model;
}

glm::vec3& DefaultSceneNode::getPosition() {
	return pos_;
}

void DefaultSceneNode::setPosition(glm::vec3& newPos) {
	pos_ = newPos;
}

void DefaultSceneNode::setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) {
	pos_ = glm::vec3(x, y, z);
}

glm::vec3& DefaultSceneNode::getLookAt() {
	return lookAt_;
}

void DefaultSceneNode::setLookAt(glm::vec3& newLookAt) {
	lookAt_ = newLookAt;
}

void DefaultSceneNode::setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) {
	lookAt_ = glm::vec3(x, y, z);
}

bool DefaultSceneNode::isActive() {
	return active_;
}

void DefaultSceneNode::setVisible(bool isVisible) {
	isVisible_ = isVisible;
}

void DefaultSceneNode::render() {	
	if (model_ != nullptr)
		model_->render();
}

}

}
