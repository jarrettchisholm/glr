#ifndef ISCENENODE_H_
#define ISCENENODE_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "models/IRenderable.hpp"

#include "Id.hpp"

namespace glr
{

class ISceneNode
{
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

	virtual const Id& getId() const = 0;
	virtual void setName(std::string name) = 0;
	virtual const std::string& getName() const = 0;

	virtual glm::vec3& getPosition() = 0;
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;

	virtual glm::vec3& getScale() = 0;
	virtual void setScale(const glm::vec3& scale) = 0;
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;

	virtual void translate(const glm::vec3& trans, TransformSpace relativeTo = TS_LOCAL) = 0;
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo = TS_LOCAL) = 0;

	virtual const glm::quat& getOrientation() const = 0;
	/**
	 * Rotates the scene node around the axis 'axis' the specified number of degrees.  This rotation is relative to the transform space specified
	 * by 'relativeTo'.
	 */
	virtual void rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TS_LOCAL) = 0;
	virtual void rotate(const glm::quat& quaternion, TransformSpace relativeTo = TS_LOCAL) = 0;
	
	virtual void lookAt(const glm::vec3& lookAt) = 0;

	virtual void attach(models::IRenderable* renderable) = 0;
	virtual void attach(shaders::IShaderProgram* shaderProgram) = 0;
	virtual void detach(models::IRenderable* renderable) = 0;
	virtual models::IRenderable* getRenderable() const = 0;
	virtual shaders::IShaderProgram* getShaderProgram() const = 0;
	
	virtual void render() = 0;
	
};

}

#endif /* ISCENENODE_H_ */
