/*
 * GLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "GLWindow.h"

#include "GL/glu.h"

#include <boost/log/trivial.hpp>

namespace icee {

namespace engine {

GLWindow::GLWindow() {

}

GLWindow::~GLWindow() {
	destroy();
}

/**
 * Note: Should be over-ridden in subclass!
 */
void* GLWindow::getWindowPointer() {
	return 0;
}

void GLWindow::resize(uint32 width, uint32 height) {
	/* prevent divide-by-zero */
	if (height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);

	width_ = width;
	height_ = height;
}

sint32 GLWindow::initialize() {
	sMgr_ = 0;
	sMgr_ = new DefaultSceneManager();

	return 0;
}

void GLWindow::destroy() {
	if (sMgr_ != 0)
		delete sMgr_;
}

sint32 GLWindow::handleEvents() {
	return 0;
}

void GLWindow::render() {
	if (gui_)
		gui_->render();
}

uint32 GLWindow::getWidth() {
	return width_;
}

uint32 GLWindow::getHeight() {
	return height_;
}

uint32 GLWindow::getDepth() {
	return depth_;
}

ISceneManager* GLWindow::getSceneManager() {
	return sMgr_;
}

IGUI* GLWindow::getHtmlGui() {
	gui_ = new GUI();

	int result = gui_->initialize();
	
	if (result < 0) {
		delete gui_;
		return 0;		
	}
	
	return gui_;
}

}

}
