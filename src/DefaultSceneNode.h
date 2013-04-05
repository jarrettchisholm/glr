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

#include "ISceneManager.h"

namespace glr {
class DefaultSceneNode : public virtual ISceneNode {
public:
	DefaultSceneNode();
	DefaultSceneNode(const std::string name);
	DefaultSceneNode(const std::string name, glm::vec3& position, glm::vec3& lookAt, glm::vec3& scale);
	virtual ~DefaultSceneNode();

	virtual ISceneNode* createChild(const std::string name, glm::vec3& position, glm::vec3& lookAt);
	virtual void addChild(ISceneNode* node);
	virtual ISceneNode* getChild(const std::string& name);
	virtual void removeChild(ISceneNode* node);
	virtual void removeAllChildren();
	virtual glmd::uint32 getNumChildren();

	virtual std::string getName();

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

	virtual void attach(models::IModel* model);
	virtual void attach(shaders::IShaderProgram* shaderProgram);
	virtual void detach(models::IModel* model);
	virtual void render();

protected:
	models::IModel* model_;
	shaders::IShaderProgram* shaderProgram_;

	ISceneManager* sceneManager_;

	std::string name_;
	std::map<std::string, ISceneNode*> children_;
	glm::vec3 pos_;
	glm::vec3 direction_;
	glm::vec3 scale_;

	bool active_;
};
}
#endif /* DEFAULTSCENENODE_H_ */
