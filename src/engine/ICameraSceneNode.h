/*
 * ICameraSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ICAMERASCENENODE_H_
#define ICAMERASCENENODE_H_

#include "ISceneNode.h"

namespace oglre {

namespace engine {

using namespace compatibility;

class ICameraSceneNode: public ISceneNode {
public:
	virtual ~ICameraSceneNode() {
	}
	;

	static const uint32 NUM_MOVE_DIRECTIONS = 4;
	enum MOVE_DIRECTION {
		MOVE_DIR_FORWARD = 0, MOVE_DIR_BACKWARD = 1, MOVE_DIR_LEFT = 2, MOVE_DIR_RIGHT = 3
	};

	static const uint32 NUM_LOOK_DIRECTIONS = 4;
	enum LOOK_DIRECTION {
		LOOK_DIR_UP = 0, LOOK_DIR_DOWN = 1, LOOK_DIR_LEFT = 2, LOOK_DIR_RIGHT = 3
	};
	
	/**
	 * Does not implement the attach method.
	 */ 
	virtual void attach(IModel* model) {
	}
	;

	// new methods
	virtual const vmath::Vector3f& getLookAt() = 0;
	virtual void setLookAt(const vmath::Vector3f& newLookAt) = 0;

	virtual void move(MOVE_DIRECTION dir, bool enabled) = 0;
	// up/down
	virtual void rotateX(float32 degrees) = 0;
	// left/right
	virtual void rotateY(float32 degrees) = 0;

	virtual void tick(float32 time) = 0;

	// inherited from ISceneNode
	virtual void render() = 0;
};

}

}

#endif /* ICAMERASCENENODE_H_ */
