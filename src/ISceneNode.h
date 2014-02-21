/*
 * ISceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISCENENODE_H_
#define ISCENENODE_H_

#include <vector>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "models/IModel.h"


namespace glr
{

class ISceneNode {
public:
	virtual ~ISceneNode()
	{
	}
	;
	
	enum TransformSpace
	{
		TS_LOCAL = 0,
		TS_WORLD
	};
	
	static const glm::detail::uint32 INVALID_ID = -1;

	virtual glm::detail::uint32 getId() = 0;
	// TODO: Find some way to make this inaccessible to 3rd party code
	virtual void setId(glm::detail::uint32 id) = 0;
	virtual const std::string& getName() = 0;

	virtual glm::vec3& getPosition() = 0;
	virtual void setPosition(glm::vec3& newPos) = 0;
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;

	virtual glm::vec3& getScale() = 0;
	virtual void setScale(const glm::vec3& scale) = 0;
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;

	virtual void translate(const glm::vec3& trans, TransformSpace relativeTo = TS_LOCAL) = 0;
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo = TS_LOCAL) = 0;

	virtual const glm::quat& getOrientation() = 0;
	/**
	 * Rotates the scene node around the axis 'axis' the specified number of degrees.  This rotation is relative to the transform space specified
	 * by 'relativeTo'.
	 */
	virtual void rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TS_LOCAL) = 0;
	virtual void rotate(const glm::quat& quaternion, TransformSpace relativeTo = TS_LOCAL) = 0;
	
	virtual void lookAt(const glm::vec3& lookAt) = 0;

	virtual void attach(models::IModel* model) = 0;
	virtual void attach(shaders::IShaderProgram* shaderProgram) = 0;
	virtual void detach(models::IModel* model) = 0;
	virtual models::IModel* getModel() = 0;
	virtual shaders::IShaderProgram* getShaderProgram() = 0;
	
	virtual void render() = 0;
	
};

}

#endif /* ISCENENODE_H_ */
