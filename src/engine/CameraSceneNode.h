/*
 * CameraSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef CAMERASCENENODE_H_
#define CAMERASCENENODE_H_

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "ICameraSceneNode.h"
#include "DefaultSceneNode.h"

//#include "IInputListener.h"

namespace oglre {

namespace engine {

class CameraSceneNode: public virtual ICameraSceneNode, public DefaultSceneNode {
public:
	CameraSceneNode();
	CameraSceneNode(glm::vec3 position, glm::vec3 lookAt, bool active = true);
	virtual ~CameraSceneNode();

	// inherited from ICameraSceneNode
	

	
	
	virtual bool isActive();
	virtual void render();
	
	virtual void attach(models::IModel* model);
	virtual void setVisible(bool isVisible);

	virtual void move(MOVE_DIRECTION dir, bool enabled);
	// up/down
	virtual void rotateX(glm::detail::float32 degrees);
	// left/right
	virtual void rotateY(glm::detail::float32 degrees);

	virtual void tick(glm::detail::float32 time);
	
	glm::mat4& getViewMatrix();

private:
	glm::quat rotation_;
	glm::detail::int32 prevX_, prevY_;
	
	glm::mat4 viewMatrix_;

	char movement_[NUM_MOVE_DIRECTIONS];
	glm::detail::float32 moveSpeed_, rotSpeed_;
	void clearMovementBuffer();
	glm::detail::float32 xRot_, yRot_;

	void initialize();
};

}

}

#endif /* CAMERASCENENODE_H_ */
