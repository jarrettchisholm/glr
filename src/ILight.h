/*
 * ILight.h
 *
 *  Created on: 2013-02-05
 *      Author: jarrett
 */

#ifndef ILIGHT_H_
#define ILIGHT_H_

#include <vector>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "ISceneNode.h"


namespace glr {

class ILight: public virtual ISceneNode {
public:
	ILight() {
	}
	;

	virtual ~ILight() {
	}
	;

};

}

#endif /* ILIGHT_H_ */
