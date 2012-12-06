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

namespace icee {

namespace engine {

DefaultSceneNode::DefaultSceneNode() {
	/*
	vertices.push_back(-2.0f); // x
	vertices.push_back(-2.0f); // y
	vertices.push_back(-18.0f); // z

	vertices.push_back(2.0f);
	vertices.push_back(-2.0f);
	vertices.push_back(-18.0f);

	vertices.push_back(0.0f);
	vertices.push_back(2.0f);
	vertices.push_back(-18.0f);
	*/
}

DefaultSceneNode::~DefaultSceneNode() {
}

void DefaultSceneNode::attach(IModel* model) {
	
}

void DefaultSceneNode::render() {
	
	/*
	// enable the vertex array
	glEnableClientState(GL_VERTEX_ARRAY);

	// tell OpenGL where the vertices are
	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

	// draw the triangle, starting from vertex index zero
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// finally, disable the vertex array
	glDisableClientState(GL_VERTEX_ARRAY);
	*/
}

}

}
