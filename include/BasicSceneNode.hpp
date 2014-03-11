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
	BasicSceneNode(glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(glm::detail::uint32 id, glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(glm::detail::uint32 id, const std::string& name, glw::IOpenGlDevice* openGlDevice);
	BasicSceneNode(glm::detail::uint32 id, const std::string& name, glm::vec3& position, const glm::quat& orientation, glm::vec3& scale, glw::IOpenGlDevice* openGlDevice);
	virtual ~BasicSceneNode();

	virtual glm::detail::uint32 getId() const;
	virtual void setId(glm::detail::uint32 id);
	virtual void setName(const std::string& name);
	virtual const std::string& getName() const;

	virtual glm::vec3& getPosition();
	virtual void setPosition(glm::vec3& newPos);
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

	virtual glm::vec3& getScale();
	virtual void setScale(const glm::vec3& scale);
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);

	virtual void translate(const glm::vec3& trans, TransformSpace relativeTo = TS_LOCAL);
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z, TransformSpace relativeTo = TS_LOCAL);

	virtual const glm::quat& getOrientation();
	virtual void rotate(const glm::detail::float32 degrees, const glm::vec3& axis, TransformSpace relativeTo = TS_LOCAL);
	virtual void rotate(const glm::quat& quaternion, TransformSpace relativeTo = TS_LOCAL);
	
	virtual void lookAt(const glm::vec3& lookAt);

	virtual void attach(models::IModel* model);
	virtual void attach(shaders::IShaderProgram* shaderProgram);
	virtual void detach(models::IModel* model);
	virtual models::IModel* getModel();
	virtual shaders::IShaderProgram* getShaderProgram();
	
	virtual void render();

protected:
	models::IModel* model_;
	shaders::IShaderProgram* shaderProgram_;

	ISceneManager* sceneManager_;

	glm::detail::uint32 id_;
	std::string name_;
	glm::vec3 pos_;
	glm::quat orientationQuaternion_;
	glm::vec3 scale_;
	glw::IOpenGlDevice* openGlDevice_;

	bool active_;	
};

}

#endif /* BASICSCENENODE_H_ */
