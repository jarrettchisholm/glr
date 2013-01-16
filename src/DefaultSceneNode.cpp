/*
 * DefaultSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>

#include <boost/log/trivial.hpp>

#include "DefaultSceneNode.h"

#include "shaders/ShaderProgram.h"


namespace oglre {



DefaultSceneNode::DefaultSceneNode() {
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;
	isVisible_ = true;
	
	model_ = nullptr;
}

DefaultSceneNode::DefaultSceneNode(std::string name) {
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;
	isVisible_ = true;
	
	model_ = nullptr;
}

DefaultSceneNode::DefaultSceneNode(std::string name, glm::vec3& position, glm::vec3& lookAt, glm::vec3& scale) {
	setPosition(position);
	setLookAt(lookAt);
	setScale(scale);

	active_ = true;
	isVisible_ = true;
	
	model_ = nullptr;
	shaderProgram_ = nullptr;
}

DefaultSceneNode::~DefaultSceneNode() {
}

void DefaultSceneNode::attach(models::IModel* model) {
	model_ = model;
}

void DefaultSceneNode::attach(shaders::IShaderProgram* shaderProgram) {
	shaderProgram_ = shaderProgram;
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

glm::vec3 DefaultSceneNode::getScale() {
	return scale_;
}

void DefaultSceneNode::setScale(glm::vec3 scale) {
	scale_ = scale;
}

void DefaultSceneNode::setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) {
	scale_ = glm::vec3(x, y, z);
}
	
void DefaultSceneNode::translate(glm::vec3 trans) {
	
}

void DefaultSceneNode::translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) {
	
}
	
void DefaultSceneNode::rotate(glm::vec3 axis, glm::detail::float32 radians) {
	
}

void DefaultSceneNode::rotate(glm::quat quaternion) {
	
}

bool DefaultSceneNode::isActive() {
	return active_;
}

void DefaultSceneNode::setVisible(bool isVisible) {
	isVisible_ = isVisible;
}

void DefaultSceneNode::render() {
	if (model_ != nullptr) {
		//if (shaderProgram_ != nullptr)
		//	shaderProgram_->bind();
		//else
		//	shaders::ShaderProgram::unbindAll();
		model_->render();
	}
}

}
