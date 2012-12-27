/*
 * ISceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef ISCENENODE_H_
#define ISCENENODE_H_

#include <vector>

#include "../vmath/Vector3f.h"

#include "IModel.h"


namespace oglre {

namespace engine {

using namespace compatibility;

class ISceneNode {
public:
	ISceneNode() {
		parent_ = 0;
		pos_ = vmath::Vector3f(0, 0, 0);

		active_ = true;
		isVisible_ = true;
	}
	;

	virtual ~ISceneNode() {
	}
	;

	const vmath::Vector3f& getPosition() const {
		return pos_;
	}

	void setPosition(const vmath::Vector3f& newPos) {
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

	std::vector<float32> vertices;

	virtual void render() = 0;

protected:
	ISceneNode* parent_; // does not own the parent_ pointer
	std::vector<ISceneNode*> children_; // owns ALL the children pointers
	vmath::Vector3f pos_;

	bool active_;
	bool isVisible_;

};

}

}

#endif /* ISCENENODE_H_ */
