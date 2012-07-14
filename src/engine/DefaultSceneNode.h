/*
 * DefaultSceneNode.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef DEFAULTSCENENODE_H_
#define DEFAULTSCENENODE_H_

#include "ISceneNode.h"

namespace icee {

namespace engine {

class DefaultSceneNode: public ISceneNode {
public:
	DefaultSceneNode();
	virtual ~DefaultSceneNode();

	void render();
};

}

}

#endif /* DEFAULTSCENENODE_H_ */
