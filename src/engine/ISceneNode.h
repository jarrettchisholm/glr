/*
 * ISceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISCENENODE_H_
#define ISCENENODE_H_

#include <vector>

#include <glm/glm.hpp>
#include "glm/gtc/quaternion.hpp"

#include "IModel.h"


namespace oglre {

namespace engine {

class ISceneNode {
public:
	ISceneNode() {
	}
	;

	virtual ~ISceneNode() {
	}
	;

	virtual glm::vec3& getPosition() = 0;
	virtual void setPosition(glm::vec3& newPos) = 0;
	virtual void setPosition(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;
	
	virtual glm::vec3& getLookAt() = 0;
	virtual void setLookAt(glm::vec3& newPos) = 0;
	virtual void setLookAt(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;
	
	virtual glm::vec3 getScale() = 0;
	virtual void setScale(glm::vec3 scale) = 0;
	virtual void setScale(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;
	
	virtual void translate(glm::vec3 trans) = 0;
	virtual void translate(glm::detail::float32 x, glm::detail::float32 y, glm::detail::float32 z) = 0;
	
	virtual void rotate(glm::vec3 axis, glm::detail::float32 radians) = 0;
	virtual void rotate(glm::quat quaternion) = 0;
	
	virtual bool isActive() = 0;	
	virtual void attach(IModel* model) = 0;
	virtual void setVisible(bool isVisible) = 0;
	virtual void render() = 0;

};

}

}

#endif /* ISCENENODE_H_ */
