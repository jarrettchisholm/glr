/*
 * DefaultSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENENODE_H_
#define DEFAULTSCENENODE_H_

#include <string>

#include "ISceneNode.h"

#include "shaders/IShaderProgram.h"

namespace oglre {

namespace engine {

class DefaultSceneNode: public virtual ISceneNode {
public:
	DefaultSceneNode();
	DefaultSceneNode(std::string name);
	DefaultSceneNode(std::string name, glm::vec3& position, glm::vec3& lookAt, glm::vec3& scale);
	virtual ~DefaultSceneNode();

	virtual glm::vec3& getPosition();
	virtual void setPosition(glm::vec3& newPos);
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	
	virtual glm::vec3& getLookAt();
	virtual void setLookAt(glm::vec3& newPos);
	virtual void setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	
	virtual glm::vec3 getScale();
	virtual void setScale(glm::vec3 scale);
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	
	virtual void translate(glm::vec3 trans);
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	
	virtual void rotate(glm::vec3 axis, glm::detail::float32 radians);
	virtual void rotate(glm::quat quaternion);
	
	virtual bool isActive();	
	virtual void attach(models::IModel* model);
	virtual void attach(shaders::IShaderProgram* shaderProgram);
	virtual void setVisible(bool isVisible);
	virtual void render();
	
protected:
	models::IModel* model_;
	shaders::IShaderProgram* shaderProgram_;
	
	std::vector<ISceneNode*> children_;
	glm::vec3 pos_;
	glm::vec3 lookAt_;
	glm::vec3 scale_;

	bool active_;
	bool isVisible_;
};

}

}

#endif /* DEFAULTSCENENODE_H_ */
