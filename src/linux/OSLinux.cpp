/*
 * OSLinux.cpp
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

// LINUX ONLY COMPILATION
#ifdef linux

#include "OSLinux.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

/* gl.h we need OpenGL :-) */
#include <GL/gl.h>

// for strcmp function
#include <string.h>

namespace icee {

namespace linuxos {

OSLinux::OSLinux() {
	rotQuad_ = 0.0f;
}

OSLinux::~OSLinux() {
}

sint32 OSLinux::initialize() {

	return 0;
}

void OSLinux::DrawAQuad() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	/*GLU.gluLookAt(
	 player.getPosX(),
	 player.getPosY(),
	 player.getPosZ(),
	 ( player.getLookAtX()+player.getPosX() ),
	 ( player.getLookAtY()+player.getPosY() ),
	 ( player.getLookAtZ()+player.getPosZ() ),
	 0.0f, 1.0f, 0.0f
	 );*/

	/** Orient the world according to the players position and the direction he/she is looking */
	// rotate the world according to the direction of the player on the (x, z) plane
	//GL11.glRotatef(-player.getDirectionInDegrees(), 0, 1, 0);
	// rotate world using the vector PERPENDICULAR to the lookat vector of the player
	//GL11.glRotatef(player.getLookDirectionInDegrees(), -player.getLookAtZ(), 0, player.getLookAtX());
	// shift the world according to the location of the player
	//GL11.glTranslatef(-player.getPosX(), -(player.getPosY()+player.getHeight()), -player.getPosZ());
	glTranslatef(-0., -0., -10.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.);
	glVertex3f(-.75, -.75, 0.);
	glColor3f(0., 1., 0.);
	glVertex3f(.75, -.75, 0.);
	glColor3f(0., 0., 1.);
	glVertex3f(.75, .75, 0.);
	glColor3f(1., 1., 0.);
	glVertex3f(-.75, .75, 0.);
	glEnd();
}

void OSLinux::renderGL() {
	lWin_->beginRender();

	glTranslatef(0.0f, 0.0f, -2.0f);
	glRotatef((GLfloat) rotQuad_, 1.0f, 0.5f, 0.25f);
	glBegin(GL_QUADS);
	/* top of cube */
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	/* bottom of cube */
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	/* front of cube */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	/* back of cube */
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	/* right side of cube */
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	/* left side of cube */
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	rotQuad_ += 0.1f;

	lWin_->endRender();
}

/*
 * create a window
 */
icee::engine::IWindow* OSLinux::createWindow(uint32 width, uint32 height, uint32 depth,
		bool fullscreen, bool vsync) {
	LinuxGLWindow* lWin = new LinuxGLWindow();
	if (lWin->create(width, height, depth, fullscreen, vsync) != 0) {
		delete lWin;
		return 0;
	}
	lWin->initialize();

	lWin_ = lWin;

	return lWin;
}

void OSLinux::run() {
	bool done = false;
	sint32 retVal = 0;

	/* wait for events and eat up cpu. ;-) */
	while (!done) {
		retVal = lWin_->handleEvents();
		if (retVal == -1)
			done = true;
		renderGL();
	}
}

}
}

#endif // LINUX ONLY COMPILATION
