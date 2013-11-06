/*
 * ICamera.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ICAMERA_H_
#define ICAMERA_H_

#include "ISceneNode.h"

namespace glr {
class ICamera : public virtual ISceneNode {
public:
	virtual ~ICamera()
	{
	}
	;

	virtual bool isActive() = 0;

	virtual void move(const glm::vec3& moveVector) = 0;
	
	virtual void lookAt(const glm::vec3& lookAt) = 0;

	virtual void tick(glm::detail::float32 time) = 0;

	virtual const glm::mat4& getViewMatrix() = 0;
};
}
#endif /* ICAMERA_H_ */
