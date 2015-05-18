#ifndef BASICSCENENODE_H_
#define BASICSCENENODE_H_

#include <string>

#include "ISceneNode.hpp"
#include "glw/IOpenGlDevice.hpp"

#include "glw/shaders/IShaderProgram.hpp"

#include "ISceneManager.hpp"

// Forward declaration (so we can set it as a friend class below)
class BasicSceneManager;

namespace glr
{

class BasicSceneNode : public virtual ISceneNode
{
public:
	BasicSceneNode(Id id, glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(Id id, std::string name, glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(Id id, std::string name, const glm::vec3& position, const glm::quat& orientation, const glm::vec3& scale, glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(Id id, const BasicSceneNode& other);
	virtual ~BasicSceneNode();

	void copy(const BasicSceneNode& other);

	virtual const Id& getId() const;
	virtual void setName(std::string name);
	virtual const std::string& getName() const;

	virtual glm::vec3& getPosition();
	virtual void setPosition(const glm::vec3& newPos);
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

	virtual glm::vec3& getScale();
	virtual void setScale(const glm::vec3& scale);
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

	virtual void translate(const glm::vec3& trans, TransformSpace relativeTo = TS_LOCAL);
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo = TS_LOCAL);

	virtual const glm::quat& getOrientation() const;
	virtual void rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TS_LOCAL);
	virtual void rotate(const glm::quat& quaternion, TransformSpace relativeTo = TS_LOCAL);
	
	virtual void lookAt(const glm::vec3& lookAt);

	virtual void attach(models::IRenderable* renderable);
	virtual void attach(shaders::IShaderProgram* shaderProgram);
	virtual void detach(models::IRenderable* renderable);
	virtual models::IRenderable* getRenderable() const;
	virtual shaders::IShaderProgram* getShaderProgram() const;
	
	virtual void render();

protected:
	models::IRenderable* renderable_;
	shaders::IShaderProgram* shaderProgram_;

	ISceneManager* sceneManager_;

	Id id_;
	std::string name_;
	glm::vec3 pos_;
	glm::quat orientationQuaternion_;
	glm::vec3 scale_;
	glw::IOpenGlDevice* openGlDevice_;

	bool active_;

private:
	// We don't copy straight up, since we need a new id for the copy
	BasicSceneNode(const BasicSceneNode& other);
};

}

#endif /* BASICSCENENODE_H_ */
