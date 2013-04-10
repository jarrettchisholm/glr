/*
 * BasicSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdlib.h>

#include <boost/log/trivial.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "BasicSceneNode.h"

#include "shaders/GlslShaderProgram.h"


namespace glr {
BasicSceneNode::BasicSceneNode()
{
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(const std::string name)
{
	setPosition(0, 0, 0);
	setLookAt(1, 1, 1);
	setScale(1, 1, 1);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(const std::string name, glm::vec3& position, glm::vec3& lookAt, glm::vec3& scale)
{
	setPosition(position);
	setLookAt(lookAt);
	setScale(scale);

	active_ = true;

	model_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::~BasicSceneNode()
{
}

void BasicSceneNode::attach(models::IModel* model)
{
	model_ = model;
}

void BasicSceneNode::detach(models::IModel* model)
{
	// TODO: implement
}

void BasicSceneNode::attach(shaders::IShaderProgram* shaderProgram)
{
	shaderProgram_ = shaderProgram;
}

/*
ISceneNode* BasicSceneNode::createChild(const std::string name, glm::vec3& position, glm::vec3& lookAt)
{
	ISceneNode* node = sceneManager_->createSceneNode(name);

	node->setPosition(position);
	node->setLookAt(lookAt);

	if ( node != nullptr )
		children_[node->getName()] = node;

	return node;
}

void BasicSceneNode::addChild(ISceneNode* node)
{
	children_[node->getName()] = node;
}

ISceneNode* BasicSceneNode::getChild(const std::string& name)
{
	if ( children_.find(name) != children_.end())
		return children_[name];

	return nullptr;
}

void BasicSceneNode::removeChild(ISceneNode* node)
{
	if ( children_.find(node->getName()) != children_.end())
		children_.erase(node->getName());
}

void BasicSceneNode::removeAllChildren()
{
	children_.clear();
}

glmd::uint32 BasicSceneNode::getNumChildren()
{
	return children_.size();
}
*/

std::string BasicSceneNode::getName()
{
	return name_;
}

glm::vec3& BasicSceneNode::getPosition()
{
	return pos_;
}

void BasicSceneNode::setPosition(glm::vec3& newPos)
{
	pos_ = newPos;
}

void BasicSceneNode::setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	pos_ = glm::vec3(x, y, z);
}

glm::vec3& BasicSceneNode::getLookAt()
{
	return direction_;
}

void BasicSceneNode::setLookAt(glm::vec3& newLookAt)
{
	direction_ = newLookAt;
}

void BasicSceneNode::setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	direction_ = glm::vec3(x, y, z);
}

glm::vec3 BasicSceneNode::getScale()
{
	return scale_;
}

void BasicSceneNode::setScale(glm::vec3 scale)
{
	scale_ = scale;
}

void BasicSceneNode::setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	scale_ = glm::vec3(x, y, z);
}

void BasicSceneNode::translate(glm::vec3 trans)
{
}

void BasicSceneNode::translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
}

void BasicSceneNode::rotate(glm::vec3 axis, glm::detail::float32 radians)
{
}

void BasicSceneNode::rotate(glm::quat quaternion)
{
}

void BasicSceneNode::render(IMatrixData* matrixData)
{
	if ( model_ != nullptr )
	{
		
		static float temp = 0.0f;
		if (shaderProgram_ != nullptr) {
			int modelMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "modelMatrix");
			int pvmMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "pvmMatrix");
			int normalMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "normalMatrix");
		
			const glm::mat4 modelMatrix = matrixData->getModelMatrix();
			const glm::mat4 projectionMatrix = matrixData->getProjectionMatrix();
			const glm::mat4 viewMatrix = matrixData->getViewMatrix();
			
			glm::vec3 translate = glm::vec3(temp, temp, temp);
			glm::mat4 newModel = glm::translate(modelMatrix, translate);
			
			
			// Send uniform variable values to the shader		
			glm::mat4 pvmMatrix(projectionMatrix * viewMatrix * newModel);
			glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
		
			glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(viewMatrix * newModel)));
			glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
		
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &newModel[0][0]);
		}
		temp += 0.1f;
		
		//if (shaderProgram_ != nullptr)
		//	shaderProgram_->bind();
		//else
		//	shaders::ShaderProgram::unbindAll();
		model_->render(matrixData, shaderProgram_);
	}
}
}
