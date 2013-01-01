/*
 * DefaultSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENENODE_H_
#define DEFAULTSCENENODE_H_

#include "ISceneNode.h"

namespace oglre {

namespace engine {

class DefaultSceneNode: public ISceneNode {
public:
	DefaultSceneNode();
	virtual ~DefaultSceneNode();

	virtual glm::vec3& getPosition();
	virtual void setPosition(glm::vec3& newPos);
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	virtual glm::vec3& getLookAt();
	virtual void setLookAt(glm::vec3& newPos);
	virtual void setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z);
	
	virtual bool isActive();	
	virtual void attach(IModel* model);
	virtual void setVisible(bool isVisible);
	virtual void render();
	
private:
	IModel* model_;
	
	std::vector<ISceneNode*> children_;
	glm::vec3 pos_;
	glm::vec3 lookAt_;

	bool active_;
	bool isVisible_;
};

}

}

#endif /* DEFAULTSCENENODE_H_ */
