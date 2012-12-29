/*
 * DefaultSceneNode.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include "DefaultSceneNode.h"

#ifdef _WIN32
#include <windows.h>
#endif

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include <stdlib.h>

namespace oglre {

namespace engine {

DefaultSceneNode::DefaultSceneNode() {	
	model_ = 0;
}

DefaultSceneNode::~DefaultSceneNode() {
}

void DefaultSceneNode::attach(IModel* model) {
	model_ = model;
}

void DefaultSceneNode::render() {	
	if (model_ != 0)
		model_->render();
}

}

}
