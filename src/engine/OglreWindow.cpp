/*
 * OglreWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include <GL/glu.h>

#include <boost/log/trivial.hpp>

#include "OglreWindow.h"

namespace icee {

namespace engine {

OglreWindow::OglreWindow(uint32 width, uint32 height, bool fullscreen, bool vsync) : 
	width_(width), height_(height), fullscreen_(fullscreen), vsync_(vsync) {
}

OglreWindow::~OglreWindow() {
	destroy();
}

/**
 * 
 */ 
void* OglreWindow::getWindowPointer() {
	sf::WindowHandle handle = window.getSystemHandle();
	
	return &handle;
}

void OglreWindow::resize(uint32 width, uint32 height) {
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

sint32 OglreWindow::initialize() {
	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;
	
	// determine whether we need to be fullscreen or not
	sint32 style = sf::Style::Titlebar | sf::Style::Close;
	if (fullscreen_)
		style = sf::Style::Fullscreen;
	
	// attempt to create our window
	sfmlWindow_ = window(sf::VideoMode(width_, height_), "OpenGL", style, settings);
    window.setVerticalSyncEnabled(vsync_);
	
	// create our scene manager
	sMgr_ = 0;
	sMgr_ = new DefaultSceneManager();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_LIGHTING);

	// we use resizeGL once to set up our initial perspective
	resize(width_, height_);

	// Reset the rotation angle of our object
	//rotQuad_ = 0;
	glFlush();

	return 0;

	return 0;
}

void OglreWindow::destroy() {
	if (sMgr_ != 0)
		delete sMgr_;
}

sint32 OglreWindow::handleEvents() {
	// handle events
	sf::Event event;
	while (sfmlWindow_.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			// end the program
			//running = false;
		}
		else if (event.type == sf::Event::Resized) {
			// adjust the viewport when the window is resized
			resize(event.size.width, event.size.height);
		}
	}
	
	return 0;
}

void OglreWindow::beginRender() {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void OglreWindow::endRender() {
	// end the current frame (internally swaps the front and back buffers)
	window.display();
}

void OglreWindow::render() {
	beginRender();
	
	sMgr_->drawAll();
	
	if (gui_)
		gui_->render();
	
	endRender();
}

uint32 OglreWindow::getWidth() {
	return width_;
}

uint32 OglreWindow::getHeight() {
	return height_;
}

uint32 OglreWindow::getDepth() {
	return depth_;
}

ISceneManager* OglreWindow::getSceneManager() {
	return sMgr_;
}

IGUI* OglreWindow::getHtmlGui() {
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
