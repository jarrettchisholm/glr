/*
 * IceGLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "IceGLWindow.h"

#include "GL/glu.h"

#include <boost/log/trivial.hpp>

namespace icee {

namespace engine {

IceGLWindow::IceGLWindow() {

}

IceGLWindow::~IceGLWindow() {
	destroy();
}

/**
 * Note: Should be over-ridden in subclass!
 */
void* IceGLWindow::getWindowPointer() {
	return 0;
}

void IceGLWindow::resize(uint32 width, uint32 height) {
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

sint32 IceGLWindow::initialize() {
	sMgr_ = 0;
	sMgr_ = new DefaultSceneManager();

	return 0;
}

void IceGLWindow::destroy() {
	if (sMgr_ != 0)
		delete sMgr_;
}

sint32 IceGLWindow::handleEvents() {
	return 0;
}

void IceGLWindow::render() {
	BOOST_LOG_TRIVIAL(debug) << "render in IceGLWindow.";
	if (gui_)
		gui_->render();
}

uint32 IceGLWindow::getWidth() {
	return width_;
}

uint32 IceGLWindow::getHeight() {
	return height_;
}

uint32 IceGLWindow::getDepth() {
	return depth_;
}

ISceneManager* IceGLWindow::getSceneManager() {
	return sMgr_;
}

IGUI* IceGLWindow::createHtmlGui() {
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
