/*
 * OSXGLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// OSX ONLY COMPILATION
#ifdef __APPLE__

#ifndef OSXGLWINDOW_H_
#define OSXGLWINDOW_H_

#include "../engine/IceGLWindow.h"

/* printf */
#include <stdio.h>

/* Xlib.h is the default header that is included and has the core functionallity */
#include <X11/Xlib.h>
/* Xatom.h includes functionallity for creating new protocol messages */
#include <X11/Xatom.h>
/* keysym.h contains keysymbols which we use to resolv what keys that are being pressed */
#include <X11/keysym.h>

/* gl.h we need OpenGL */
#include <GL/gl.h>
/* this file is needed for X11 applications if we want to use hardware rendering */
#include <GL/glx.h>

namespace icee {

namespace macosx {

using namespace compatibility;

class OSXGLWindow: public icee::engine::IceGLWindow {
public:
	OSXGLWindow();
	virtual ~OSXGLWindow();

	virtual void* getWindowPointer();
	virtual std::size_t getWindowHandle();

	virtual sint32 initialize();
	virtual sint32 create(uint32 width, uint32 height, uint32 depth,
			bool fullscreen = false, bool vsync = false);
	void destroy();

	sint32 handleEvents();

	void beginRender();
	void endRender();
	virtual void render();

	void setFullscreen(bool enabled);
	bool isFullscreen();

private:
	/* most important variable
	 * it contains information about the X server which we communicate with
	 */
	Display* display_;
	sint32 screen_;

	/* our window instance */
	Window window_;
	GLXContext context_;
	XSetWindowAttributes winAttr_;Bool fullscreen_;Bool doubleBuffered_;

	/* original mode which we save so we can restore it later */
	//XF86VidModeModeInfo originalMode_;
};

}

}

#endif /* OSXGLWINDOW_H_ */

#endif // OSX ONLY COMPILATION
