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

class ICameraSceneNode: public virtual ISceneNode {
public:
	ICameraSceneNode() {
	}
	;
	
	virtual ~ICameraSceneNode() {
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

	virtual void move(MOVE_DIRECTION dir, bool enabled) = 0;
	// up/down
	virtual void rotateX(glm::detail::float32 degrees) = 0;
	// left/right
	virtual void rotateY(glm::detail::float32 degrees) = 0;

	virtual void tick(glm::detail::float32 time) = 0;
};

}

}

#endif /* ICAMERASCENENODE_H_ */
