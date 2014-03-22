#include <utility>

#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include <stdlib.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "common/logger/Logger.hpp"

#include "BasicSceneNode.hpp"

#include "glw/shaders/GlslShaderProgram.hpp"
#include "exceptions/Exception.hpp"

namespace glr
{

BasicSceneNode::BasicSceneNode(Id id, glw::IOpenGlDevice* openGlDevice) : id_(id), openGlDevice_(openGlDevice)
{
	name_ = std::string();
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	active_ = true;

	renderable_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(Id id, std::string name, glw::IOpenGlDevice* openGlDevice) : id_(id), name_(std::move(name)), openGlDevice_(openGlDevice)
{
	setPosition(0, 0, 0);
	setScale(1, 1, 1);

	active_ = true;

	renderable_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(Id id, std::string name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice)
	 : id_(id), name_(std::move(name)), openGlDevice_(openGlDevice)
{
	setPosition(position);
	rotate(orientation);
	setScale(scale);

	active_ = true;

	renderable_ = nullptr;
	shaderProgram_ = nullptr;
}

BasicSceneNode::BasicSceneNode(Id id, const BasicSceneNode& other) : id_(id)
{
	copy(other);
}

BasicSceneNode::BasicSceneNode(const BasicSceneNode& other)
{
	std::string msg = std::string("Unable to copy BasicSceneNode...In order to copy a BasicSceneNode, you must provide a new id for the copy.");
	LOG_ERROR( msg );
	throw exception::Exception(msg);
}

BasicSceneNode::~BasicSceneNode()
{
}

void BasicSceneNode::copy(const BasicSceneNode& other)
{
	renderable_ = other.renderable_;
	shaderProgram_ = other.shaderProgram_;

	sceneManager_ = other.sceneManager_;

	name_ = other.name_;
	pos_ = other.pos_;
	orientationQuaternion_ = other.orientationQuaternion_;
	scale_ = other.scale_;
	openGlDevice_ = other.openGlDevice_;

	active_ = other.active_;
}

void BasicSceneNode::attach(models::IRenderable* renderable)
{
	renderable_ = renderable;
}

void BasicSceneNode::detach(models::IRenderable* renderable)
{
	// TODO: implement
}

void BasicSceneNode::attach(shaders::IShaderProgram* shaderProgram)
{
	shaderProgram_ = shaderProgram;
}

const Id& BasicSceneNode::getId() const
{
	return id_;
}

void BasicSceneNode::setName(std::string name)
{
	name_ = std::move(name);
}

const std::string& BasicSceneNode::getName() const
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

glm::vec3& BasicSceneNode::getScale()
{
	return scale_;
}

void BasicSceneNode::setScale(const glm::vec3& scale)
{
	scale_ = scale;
}

void BasicSceneNode::setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z)
{
	scale_ = glm::vec3(x, y, z);
}

void BasicSceneNode::translate(const glm::vec3& trans, TransformSpace relativeTo)
{
	pos_ += trans;
}

void BasicSceneNode::translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo)
{
	pos_ += glm::vec3(x, y, z);
}

const glm::quat& BasicSceneNode::getOrientation() const
{
	return orientationQuaternion_;
}

void BasicSceneNode::rotate(const glm::quat& quaternion, TransformSpace relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			orientationQuaternion_ = orientationQuaternion_ * glm::normalize( quaternion );
			break;
		
		case TransformSpace::TS_WORLD:
			orientationQuaternion_ =  glm::normalize( quaternion ) * orientationQuaternion_;
			break;
			
		default:
			// TODO: error
			break;
	}		
}

void BasicSceneNode::rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo)
{
	switch( relativeTo )
	{
		case TransformSpace::TS_LOCAL:
			orientationQuaternion_ = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * orientationQuaternion_;
			break;
		
		case TransformSpace::TS_WORLD:
			orientationQuaternion_ =  orientationQuaternion_ * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			// TODO: error
			break;
	}
}

void BasicSceneNode::lookAt(const glm::vec3& lookAt)
{
	assert(lookAt != pos_);
	
	glm::mat4 lookAtMatrix = glm::lookAt(pos_, pos_ + lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	orientationQuaternion_ =  glm::normalize( orientationQuaternion_ * glm::quat_cast( lookAtMatrix ) );
}

models::IRenderable* BasicSceneNode::getRenderable() const
{
	return renderable_;
}

shaders::IShaderProgram* BasicSceneNode::getShaderProgram() const
{
	return shaderProgram_;
}

void BasicSceneNode::render()
{
	if ( renderable_ != nullptr )
	{
		if (shaderProgram_ != nullptr)
		{
			GLint bindPoint = shaderProgram_->getBindPointByBindingName( shaders::IShader::BIND_TYPE_MATERIAL );
			shaderProgram_->bind();

			int modelMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "modelMatrix");
			int pvmMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "pvmMatrix");
			int normalMatrixLocation = glGetUniformLocation(shaderProgram_->getGLShaderProgramId(), "normalMatrix");

			const glm::mat4 modelMatrix = openGlDevice_->getModelMatrix();
			const glm::mat4 projectionMatrix = openGlDevice_->getProjectionMatrix();
			const glm::mat4 viewMatrix = openGlDevice_->getViewMatrix();
			
			glm::mat4 newModel = glm::translate(modelMatrix, pos_);
			newModel = newModel * glm::mat4_cast( orientationQuaternion_ );
			newModel = glm::scale(newModel, scale_);

			// Send uniform variable values to the shader		
			glm::mat4 pvmMatrix(projectionMatrix * viewMatrix * newModel);
			glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);

			glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(viewMatrix * newModel)));
			glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);

			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &newModel[0][0]);
			
			renderable_->render(shaderProgram_);
		}
	}
}

}
