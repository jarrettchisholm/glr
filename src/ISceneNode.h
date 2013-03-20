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

#include "models/IModel.h"


namespace glr {

namespace glmd = glm::detail;

class ISceneNode {
public:
	ISceneNode() {
	}
	;

	virtual ~ISceneNode() {
	}
	;
	
	virtual ISceneNode* createChild( const std::string name, glm::vec3& position, glm::vec3& lookAt ) = 0;
	virtual void addChild( ISceneNode* node ) = 0;
	virtual ISceneNode* getChild( const std::string& name ) = 0;
	virtual void removeChild( ISceneNode* node ) = 0;
	virtual void removeAllChildren() = 0;
	virtual glmd::uint32 getNumChildren() = 0;
	
	virtual std::string getName() = 0;

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
	
	virtual void attach(models::IModel* model) = 0;
	virtual void detach(models::IModel* model) = 0;
	virtual void render() = 0;

};

}

#endif /* ISCENENODE_H_ */
