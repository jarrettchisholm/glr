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

#include "IModel.h"


namespace oglre {

namespace engine {

class ISceneNode {
public:
	ISceneNode() {
		parent_ = 0;
		pos_ = glm::vec3(0, 0, 0);

		active_ = true;
		isVisible_ = true;
	}
	;

	virtual ~ISceneNode() {
	}
	;

	const glm::vec3& getPosition() const {
		return pos_;
	}

	void setPosition(const glm::vec3& newPos) {
		pos_ = newPos;
	}

	const ISceneNode* getParent() const {
		return parent_;
	}

	const bool isActive() const {
		return active_;
	}
	
	virtual void attach(IModel* model) = 0;
	
	void setVisible(bool isVisible) {
		isVisible_ = isVisible;
	}

	std::vector<glm::detail::float32> vertices;

	virtual void render() = 0;

protected:
	ISceneNode* parent_; // does not own the parent_ pointer
	std::vector<ISceneNode*> children_; // owns ALL the children pointers
	glm::vec3 pos_;

	bool active_;
	bool isVisible_;

};

}

}

#endif /* ISCENENODE_H_ */
