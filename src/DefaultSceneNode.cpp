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

#include "shaders/GlslShaderProgram.h"


namespace oglre {



DefaultSceneNode::DefaultSceneNode() {
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;
	
	model_ = nullptr;
}

DefaultSceneNode::DefaultSceneNode(const std::string name) {
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;
	
	model_ = nullptr;
}

DefaultSceneNode::DefaultSceneNode(const std::string name, glm::vec3& position, glm::vec3& lookAt, glm::vec3& scale) {
	setPosition(position);
	setLookAt(lookAt);
	setScale(scale);

	active_ = true;
	
	model_ = nullptr;
	shaderProgram_ = nullptr;
}

DefaultSceneNode::~DefaultSceneNode() {
}

void DefaultSceneNode::attach(models::IModel* model) {
	model_ = model;
}

void DefaultSceneNode::detach(models::IModel* model) {
	// TODO: implement
}

void DefaultSceneNode::attach(shaders::IShaderProgram* shaderProgram) {
	shaderProgram_ = shaderProgram;
}

ISceneNode* DefaultSceneNode::createChild( const std::string name, glm::vec3& position, glm::vec3& lookAt ) {
	ISceneNode* node = sceneManager_->createSceneNode( name );
	node->setPosition(position);
	node->setLookAt(lookAt);
	
	if (node != nullptr)
		children_[node->getName()] = node;
		
	return node;
}

void DefaultSceneNode::addChild( ISceneNode* node ) {
	children_[node->getName()] = node;
}

ISceneNode* DefaultSceneNode::getChild( const std::string& name ) {
	if ( children_.find(name) != children_.end() )
		return children_[name];
		
	return nullptr;
}

void DefaultSceneNode::removeChild( ISceneNode* node ) {
	if ( children_.find(node->getName()) != children_.end() )
		children_.erase(node->getName());
}

void DefaultSceneNode::removeAllChildren() {
	children_.clear();
}

glmd::uint32 DefaultSceneNode::getNumChildren() {
	return children_.size();
}

std::string DefaultSceneNode::getName() {
	return name_;
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
	return direction_;
}

void DefaultSceneNode::setLookAt(glm::vec3& newLookAt) {
	direction_ = newLookAt;
}

void DefaultSceneNode::setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) {
	direction_ = glm::vec3(x, y, z);
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
