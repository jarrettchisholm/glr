/*
 * LinuxGLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// LINUX ONLY COMPILATION
#ifdef linux

#include "LinuxGLWindow.h"

// for strcmp function
#include <string.h>

#include <iostream>

#include <boost/log/trivial.hpp>

namespace icee {

namespace linuxos {

// define these for when we are dealing with Atom properties (I don't know why they aren't already defined for me..)
enum {
	_NET_WM_STATE_REMOVE = 0, // remove/unset property
	_NET_WM_STATE_ADD = 1, // add/set property
	_NET_WM_STATE_TOGGLE = 2
// toggle property
};

LinuxGLWindow::LinuxGLWindow() {
	fullscreen_ = false;
	inWindowedState_ = false;
}

LinuxGLWindow::~LinuxGLWindow() {
}

void* LinuxGLWindow::getWindowPointer() {
	return &window_;
}

std::size_t LinuxGLWindow::getWindowHandle() {
	return window_;
}

sint32 LinuxGLWindow::create(uint32 width, uint32 height, uint32 depth, bool fullscreen, bool vsync) {
	width_ = width;
	height_ = height;
	depth_ = depth;

	fullscreen_ = fullscreen;

	XVisualInfo *vi;
	Colormap cmap;
	sint32 i, dpyWidth, dpyHeight;
	sint32 glxMajor, glxMinor, vmMajor, vmMinor;
	XF86VidModeModeInfo **modes;
	sint32 modeNum, bestMode;
	Atom wmDelete;

	// set best mode to -1 (if still -1 after search means we can't find a suitable display mode)
	bestMode = -1;

	// get a connection
	display_ = XOpenDisplay(0);
	screen_ = DefaultScreen(display_);
	XF86VidModeQueryVersion(display_, &vmMajor, &vmMinor);
	BOOST_LOG_TRIVIAL(debug) << "XF86 VideoMode extension version " << vmMajor << "." << vmMinor;
	XF86VidModeGetAllModeLines(display_, screen_, &modeNum, &modes);

	// save desktop-resolution before switching modes (make a copy of the XF86VidModeModeInfo)
	originalMode_ = XF86VidModeModeInfo(*modes[0]);
	BOOST_LOG_TRIVIAL(debug) << "searching for mode with resolution " << width << " x " << height;
	// look for mode with requested resolution
	for (i = 0; i < modeNum; i++) {
		BOOST_LOG_TRIVIAL(debug) << "Mode found: " << modes[i]->hdisplay << " x " << modes[i]->vdisplay;
		if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height)) {
			bestMode = i;
		}
	}

	// error check
	if (bestMode == -1) {
		// todo: improve error handling...
		BOOST_LOG_TRIVIAL(error) << "Mode with requested resolution not found!";
		return -1;
	} else {
		BOOST_LOG_TRIVIAL(debug) << "Mode chosen: " << modes[bestMode]->hdisplay << " x " << modes[bestMode]->vdisplay;
	}

	BOOST_LOG_TRIVIAL(debug) << "Requested depth is " << depth_;

	// choose our window attributes
	/* attributes for a single buffered visual in RGBA format with at least
	 * 4 bits per color and a depth_ bit depth buffer */
	sint32 attrListSgl[] = { GLX_RGBA, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4,
			GLX_DEPTH_SIZE, depth_, None };

	/* attributes for a double buffered visual in RGBA format with at least
	 * 4 bits per color and a depth_ bit depth buffer */
	sint32 attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4,
			GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, depth_, None };

	// get an appropriate visual
	vi = glXChooseVisual(display_, screen_, attrListDbl);
	if (vi == NULL) {
		vi = glXChooseVisual(display_, screen_, attrListSgl);
		doubleBuffered_ = False;
		BOOST_LOG_TRIVIAL(debug) << "singlebuffered rendering will be used, no doublebuffering available.";
	} else {
		doubleBuffered_ = True;
		BOOST_LOG_TRIVIAL(debug) << "doublebuffered rendering available.";
	}
	glXQueryVersion(display_, &glxMajor, &glxMinor);
	BOOST_LOG_TRIVIAL(debug) << "GLX-Version " << glxMajor << "." << glxMinor;

	if (vi == NULL) {
		BOOST_LOG_TRIVIAL(error) << "Visual display is not set (value is NULL)!";
	}

	BOOST_LOG_TRIVIAL(debug) << "create a GLX context.";
	// create a GLX context
	context_ = glXCreateContext(display_, vi, 0, GL_TRUE);

	BOOST_LOG_TRIVIAL(debug) << "create a color map.";
	// create a color map
	cmap = XCreateColormap(display_, RootWindow(display_, vi->screen), vi->visual, AllocNone);
	winAttr_.colormap = cmap;
	winAttr_.border_pixel = 0;

	if (fullscreen_ && !inWindowedState_) {
		// switch to fullscreen
		XF86VidModeSwitchToMode(display_, screen_, modes[bestMode]);
		XF86VidModeSetViewPort(display_, screen_, 0, 0);
		dpyWidth = modes[bestMode]->hdisplay;
		dpyHeight = modes[bestMode]->vdisplay;
		BOOST_LOG_TRIVIAL(debug) << "resolution " << dpyWidth << "x" << dpyHeight;
		XFree(modes);

		// set window attributes
		winAttr_.override_redirect = True;
		winAttr_.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;
		window_ = XCreateWindow(display_, RootWindow(display_, vi->screen), 0, 0, dpyWidth,
				dpyHeight, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect, &winAttr_);
		XWarpPointer(display_, None, window_, 0, 0, 0, 0, 0, 0);
		XMapRaised(display_, window_);
	} else {
		// create a window in window mode
		winAttr_.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask | FocusChangeMask;
		window_ = XCreateWindow(display_, RootWindow(display_, vi->screen), 0, 0, width_, height_,
				0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask,
				&winAttr_);

		// only set window title and handle wm_delete_events if in windowed mode
		wmDelete = XInternAtom(display_, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(display_, window_, &wmDelete, 1);
		XSetStandardProperties(display_, window_, "Untitled", "Untitled", None, NULL, 0, NULL);
		XMapRaised(display_, window_);
		
		//XMoveWindow(display_, window_, 0, 0);
	}

	XStoreName(display_, window_, "VERY SIMPLE APPLICATION");
	XMapWindow(display_, window_);

	// connect the glx-context to the window
	glXMakeCurrent(display_, window_, context_);
	glEnable(GL_DEPTH_TEST);
	if (glXIsDirect(display_, context_)) {
		BOOST_LOG_TRIVIAL(debug) << "DRI enabled.";
	} else {
		BOOST_LOG_TRIVIAL(debug) << "no DRI available.";
	}

	return 0;
}

void LinuxGLWindow::destroy() {
	IceGLWindow::destroy();

	if (context_) {
		if (!glXMakeCurrent(display_, None, NULL)) {
			BOOST_LOG_TRIVIAL(warning) << "Could not release drawing context!";
		}

		// destroy the context
		glXDestroyContext(display_, context_);
		context_ = NULL;
	}

	// switch back to original desktop resolution if we were in fullscreen
	if (fullscreen_ && !inWindowedState_) {
		XF86VidModeSwitchToMode(display_, screen_, &originalMode_);
		XF86VidModeSetViewPort(display_, screen_, 0, 0);
	}

	XCloseDisplay(display_);
}

sint32 LinuxGLWindow::initialize() {
	// call 'superclasses' initialize function
	sint32 retVal = icee::engine::IceGLWindow::initialize();
	if (retVal != 0)
		return retVal;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// we use resizeGL once to set up our initial perspective
	resize(width_, height_);

	// Reset the rotation angle of our object
	//rotQuad_ = 0;
	glFlush();

	return 0;
}

/**
 * Note: should be called before any other event handling functions.
 * Note 2: If you are grabbing the keyboard/mouse, keyboard/mouse events may not be handled
 * as expected in this function (i.e. the ESC key may no longer cause the function to return -1).
 */
sint32 LinuxGLWindow::handleEvents() {
	XEvent event;
	sint32 retVal = 0;

	XEvent xev;
	Atom WM_STATE = XInternAtom(display_, "_NET_WM_STATE", False);
	Atom WM_STATE_FULLSCREEN = XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False);
	Atom WM_STATE_MAXIMIZED_VERT = XInternAtom(display_, "_NET_WM_STATE_MAXIMIZED_VERT", False);
	Atom WM_STATE_MAXIMIZED_HORZ = XInternAtom(display_, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
	Atom WM_STATE_HIDDEN = XInternAtom(display_, "_NET_WM_STATE_HIDDEN", False);

	// handle the events in the queue
	while (XPending(display_) > 0) {
		XNextEvent(display_, &event);
		switch (event.type) {
			case FocusIn:

				/*
				 if (minimized_) {
				 destroy();
				 //fullscreen_ = !fullscreen_;
				 //setFullscreen(!isFullscreen());
				 create(width_, height_, depth_, fullscreen_, false);
				 }
				 */

				break;

			case FocusOut:
				//XIconifyWindow(display_, window_, screen_);

				/*
				 memset(&xev, 0, sizeof(xev));
				 xev.type = ClientMessage;
				 xev.xclient.window = window_;
				 xev.xclient.message_type = WM_STATE;
				 xev.xclient.format = 32;
				 xev.xclient.data.l[0] = _NET_WM_STATE_REMOVE;
				 xev.xclient.data.l[1] = WM_STATE_HIDDEN;

				 XSendEvent(display_, DefaultRootWindow(display_), False, SubstructureNotifyMask,
				 &xev);
				 */

				if (fullscreen_ && !inWindowedState_) {
					destroy();
					//fullscreen_ = !fullscreen_;
					//setFullscreen(!isFullscreen());
					inWindowedState_ = true;
					create(width_, height_, depth_, fullscreen_, true);
					initialize();
					glFlush();
					XFlush(display_);
				}

				break;

			case Expose:
				if (event.xexpose.count != 0)
					break;
				resize(width_, height_);
				//renderGL();
				break;

			case ConfigureNotify:
				// call resize only if our window-size changed
				if ((event.xconfigure.width != width_) || (event.xconfigure.height != height_)) {
					resize(event.xconfigure.width, event.xconfigure.height);
				}
				break;

				// exit in case of a mouse button press
			case ButtonPress:
				//retVal = -1;
				break;

			case KeyPress:
				if (XLookupKeysym(&event.xkey, 0) == XK_Escape) {
					retVal = -1;
				}
				if (XLookupKeysym(&event.xkey, 0) == XK_F1) {
					destroy();
					//fullscreen_ = !fullscreen_;
					setFullscreen(!isFullscreen());
					create(width_, height_, depth_, fullscreen_);
				}
				break;

			case ClientMessage:
				if (strcmp(XGetAtomName(display_, event.xclient.message_type), "WM_PROTOCOLS") == 0) {
					retVal = -1;
				}
				break;

			default:
				break;
		}
	}

	return retVal;
}

void LinuxGLWindow::setFullscreen(bool enabled) {
	if (fullscreen_ && !enabled) {
		fullscreen_ = False;
	} else if (!fullscreen_ && enabled) {
		fullscreen_ = True;
	}
}

bool LinuxGLWindow::isFullscreen() {
	if (fullscreen_)
		return true;
	else
		return false;
}

void LinuxGLWindow::beginRender() {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void LinuxGLWindow::endRender() {
	// swap the buffers if we have doublebuffered
	if (doubleBuffered_) {
		glXSwapBuffers(display_, window_);
	}
}

void LinuxGLWindow::render() {
	beginRender();

	sMgr_->drawAll();

	endRender();
}

}

}
#endif // LINUX ONLY COMPILATION
