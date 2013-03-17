/*
 * ICamera.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ICAMERA_H_
#define ICAMERA_H_

#include "ISceneNode.h"

namespace oglre {

class ICamera: public virtual ISceneNode {
public:
	ICamera() {
	}
	;
	
	virtual ~ICamera() {
	}
	;

	static const glm::detail::uint32 NUM_MOVE_DIRECTIONS = 4;
	enum MOVE_DIRECTION {
		MOVE_DIR_FORWARD = 0, MOVE_DIR_BACKWARD = 1, MOVE_DIR_LEFT = 2, MOVE_DIR_RIGHT = 3
	};

	static const glm::detail::uint32 NUM_LOOK_DIRECTIONS = 4;
	enum LOOK_DIRECTION {
		LOOK_DIR_UP = 0, LOOK_DIR_DOWN = 1, LOOK_DIR_LEFT = 2, LOOK_DIR_RIGHT = 3
	};

	virtual bool isActive() = 0;

	virtual void move(MOVE_DIRECTION dir, bool enabled) = 0;
	// up/down
	virtual void rotateX(glm::detail::float32 degrees) = 0;
	// left/right
	virtual void rotateY(glm::detail::float32 degrees) = 0;

	virtual void tick(glm::detail::float32 time) = 0;
	
	virtual const glm::mat4& getViewMatrix() = 0;
};

}

#endif /* ICAMERA_H_ */
