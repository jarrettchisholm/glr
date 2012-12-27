/*
 * CameraSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef CAMERASCENENODE_H_
#define CAMERASCENENODE_H_

#include "ICameraSceneNode.h"
#include "Quaternion.h"
//#include "IInputListener.h"

namespace oglre {

namespace engine {

class CameraSceneNode: public ICameraSceneNode {
public:
	CameraSceneNode();
	CameraSceneNode(vmath::Vector3f position, vmath::Vector3f lookAt, bool active = true);
	virtual ~CameraSceneNode();

	// inherited from ICameraSceneNode
	virtual void render(); // don't really need this

	// inherited from ICameraSceneNode
	virtual const vmath::Vector3f& getLookAt();
	virtual void setLookAt(const vmath::Vector3f& newLookAt);

	virtual void move(MOVE_DIRECTION dir, bool enabled);
	// up/down
	virtual void rotateX(float32 degrees);
	// left/right
	virtual void rotateY(float32 degrees);

	virtual void tick(float32 time);

protected:
	vmath::Vector3f lookAt_;
	Quaternion rotation_;
	sint32 prevX_, prevY_;

	char8 movement_[NUM_MOVE_DIRECTIONS];
	float32 moveSpeed_, rotSpeed_;
	void clearMovementBuffer();
	float32 xRot_, yRot_;

	void initialize();
};

}

}

#endif /* CAMERASCENENODE_H_ */
