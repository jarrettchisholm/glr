/*
 * LinuxGLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// LINUX ONLY COMPILATION
#ifdef linux

#ifndef LINUXGLWINDOW_H_
#define LINUXGLWINDOW_H_

#include "../engine/IceGLWindow.h"
#include "../engine/TextureManager.h"

/* printf */
#include <stdio.h>

/* Xlib.h is the default header that is included and has the core functionallity */
#include <X11/Xlib.h>
/* Xatom.h includes functionallity for creating new protocol messages */
#include <X11/Xatom.h>
/* keysym.h contains keysymbols which we use to resolv what keys that are being pressed */
#include <X11/keysym.h>

/* the XF86 Video Mode extension allows us to change the displaymode of the server
 * this allows us to set the display to fullscreen and also read videomodes and
 * other information.
 */
#include <X11/extensions/xf86vmode.h>

/* gl.h we need OpenGL */
#include <GL/gl.h>
/* this file is needed for X11 applications if we want to use hardware rendering */
#include <GL/glx.h>

namespace icee {

namespace linuxos {

using namespace compatibility;

class LinuxGLWindow: public icee::engine::IceGLWindow {
public:
	LinuxGLWindow();
	virtual ~LinuxGLWindow();

	virtual void* getWindowPointer();
	virtual std::size_t getWindowHandle();

	virtual sint32 initialize();
	virtual sint32 create(uint32 width, uint32 height, uint32 depth, bool fullscreen = false,
			bool vsync = false);
	void destroy();

	sint32 handleEvents();

	void beginRender();
	void endRender();
	virtual void render();

	void setFullscreen(bool enabled);
	bool isFullscreen();

	virtual bool blah() {
		return inWindowedState_;
	}

private:
	/* most important variable
	 * it contains information about the X server which we communicate with
	 */
	Display* display_;
	sint32 screen_;

	/* our window instance */
	Window window_;
	GLXContext context_;
	XSetWindowAttributes winAttr_;
	Bool fullscreen_;
	Bool doubleBuffered_;

	/* original mode which we save so we can restore it later */
	XF86VidModeModeInfo originalMode_;

	bool inWindowedState_;
	
	icee::engine::TextureManager textureManager_;
};

}

}

#endif /* LINUXGLWINDOW_H_ */

#endif // LINUX ONLY COMPILATION
