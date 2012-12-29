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

//#include "IInputListener.h"

namespace oglre {

namespace engine {

class CameraSceneNode: public ICameraSceneNode {
public:
	CameraSceneNode();
	CameraSceneNode(glm::vec3 position, glm::vec3 lookAt, bool active = true);
	virtual ~CameraSceneNode();

	// inherited from ICameraSceneNode
	virtual void render(); // don't really need this

	// inherited from ICameraSceneNode
	virtual glm::vec3 getLookAt();
	virtual void setLookAt(glm::vec3 newLookAt);

	virtual void move(MOVE_DIRECTION dir, bool enabled);
	// up/down
	virtual void rotateX(glm::detail::float32 degrees);
	// left/right
	virtual void rotateY(glm::detail::float32 degrees);

	virtual void tick(glm::detail::float32 time);

protected:
	glm::vec3 lookAt_;
	glm::quat rotation_;
	glm::detail::int32 prevX_, prevY_;

	char movement_[NUM_MOVE_DIRECTIONS];
	glm::detail::float32 moveSpeed_, rotSpeed_;
	void clearMovementBuffer();
	glm::detail::float32 xRot_, yRot_;

	void initialize();
};

}

}

#endif /* CAMERASCENENODE_H_ */
