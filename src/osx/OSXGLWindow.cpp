/*
 * OSXGLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// OSX ONLY COMPILATION
#ifdef __APPLE__

#include "OSXGLWindow.h"

// for strcmp function
#include <string.h>

namespace icee {

namespace macosx {

OSXGLWindow::OSXGLWindow() {
	fullscreen_ = False;
}

OSXGLWindow::~OSXGLWindow() {
}

void* OSXGLWindow::getWindowPointer() {
	return &window_;
}

std::size_t OSXGLWindow::getWindowHandle() {
	return window_;
}

sint32 OSXGLWindow::create(uint32 width, uint32 height, uint32 depth,
		bool fullscreen, bool vsync) {
	width_ = width;
	height_ = height;
	depth_ = depth;

	XVisualInfo *vi;
	Colormap cmap;
	sint32 i, dpyWidth, dpyHeight;
	sint32 glxMajor, glxMinor, vmMajor, vmMinor;
	//XF86VidModeModeInfo **modes;
	sint32 modeNum, bestMode;
	Atom wmDelete;

	// set best mode to -1 (if still -1 after search means we can't find a suitable display mode)
	bestMode = -1;

	// get a connection
	display_ = XOpenDisplay(0);
	screen_ = DefaultScreen(display_);

	/*
	 XF86VidModeQueryVersion(display_, &vmMajor, &vmMinor);
	 printf("XF86 VideoMode extension version %d.%d\n", vmMajor, vmMinor);
	 XF86VidModeGetAllModeLines(display_, screen_, &modeNum, &modes);

	 // save desktop-resolution before switching modes
	 originalMode_ = *modes[0];

	 // look for mode with requested resolution
	 for (i = 0; i < modeNum; i++) {
	 if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height))
	 bestMode = i;
	 }

	 // error check
	 if (bestMode == -1) {
	 // todo: improve error handling...
	 return -1;
	 }
	 */

	// choose our window attributes
	/* attributes for a single buffered visual in RGBA format with at least
	 * 4 bits per color and a depth_ bit depth buffer */
	sint32 attrListSgl[] = { GLX_RGBA, GLX_RED_SIZE, 4, GLX_GREEN_SIZE, 4,
			GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, depth_, None };

	/* attributes for a double buffered visual in RGBA format with at least
	 * 4 bits per color and a depth_ bit depth buffer */
	sint32 attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 4,
			GLX_GREEN_SIZE, 4, GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE, depth_, None };

	// get an appropriate visual
	vi = glXChooseVisual(display_, screen_, attrListDbl);
	if (vi == 0) {
		vi = glXChooseVisual(display_, screen_, attrListSgl);
		doubleBuffered_ = False;
		printf(
				"singlebuffered rendering will be used, no doublebuffering available\n");
	} else {
		doubleBuffered_ = True;
		printf("doublebuffered rendering available\n");
	}

	glXQueryVersion(display_, &glxMajor, &glxMinor);
	printf("GLX-Version %d.%d\n", glxMajor, glxMinor);

	// create a GLX context
	context_ = glXCreateContext(display_, vi, 0, GL_TRUE);

	// create a color map
	cmap = XCreateColormap(display_, RootWindow(display_, vi->screen),
			vi->visual, AllocNone);
	winAttr_.colormap = cmap;
	winAttr_.border_pixel = 0;

	if (fullscreen_) {
		// switch to fullscreen
		/*
		 XF86VidModeSwitchToMode(display_, screen_, modes[bestMode]);
		 XF86VidModeSetViewPort(display_, screen_, 0, 0);
		 dpyWidth = modes[bestMode]->hdisplay;
		 dpyHeight = modes[bestMode]->vdisplay;
		 printf("resolution %dx%d\n", dpyWidth, dpyHeight);
		 XFree(modes);

		 // set window attributes
		 winAttr_.override_redirect = True;
		 winAttr_.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
		 window_ = XCreateWindow(display_, RootWindow(display_, vi->screen), 0, 0, dpyWidth,
		 dpyHeight, 0, vi->depth, InputOutput, vi->visual,
		 CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect, &winAttr_);
		 XWarpPointer(display_, None, window_, 0, 0, 0, 0, 0, 0);
		 XMapRaised(display_, window_);
		 */
		/*
		 XGrabKeyboard(display_, window_, True, GrabModeAsync, GrabModeAsync, CurrentTime);
		 XGrabPointer(display_, window_, True, ButtonPressMask, GrabModeAsync, GrabModeAsync,
		 window_, None, CurrentTime);
		 */
	} else {
		// create a window in window mode
		winAttr_.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
		window_ = XCreateWindow(display_, RootWindow(display_, vi->screen), 0,
				0, width_, height_, 0, vi->depth, InputOutput, vi->visual,
				CWBorderPixel | CWColormap | CWEventMask, &winAttr_);

		// only set window title and handle wm_delete_events if in windowed mode
		wmDelete = XInternAtom(display_, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(display_, window_, &wmDelete, 1);
		XSetStandardProperties(display_, window_, "Untitled", "Untitled", None,
				NULL, 0, NULL);
		XMapRaised(display_, window_);
	}

	XMapWindow(display_, window_);
	XStoreName(display_, window_, "VERY SIMPLE APPLICATION");

	// connect the glx-context to the window
	glXMakeCurrent(display_, window_, context_);
	glEnable(GL_DEPTH_TEST);
	if (glXIsDirect(display_, context_))
		printf("DRI enabled\n");
	else
		printf("no DRI available\n");

	return 0;
}

void OSXGLWindow::destroy() {
	if (context_) {
		if (!glXMakeCurrent(display_, None, NULL)) {
			printf("Could not release drawing context.\n");
		}

		// destroy the context
		glXDestroyContext(display_, context_);
		context_ = NULL;
	}
	// switch back to original desktop resolution if we were in fullscreen
	if (fullscreen_) {
		//XF86VidModeSwitchToMode(display_, screen_, &originalMode_);
		//XF86VidModeSetViewPort(display_, screen_, 0, 0);
	}
	XCloseDisplay(display_);
}

sint32 OSXGLWindow::initialize() {
	// call 'superclasses' initialize function
	icee::engine::GLWindow::initialize();
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

sint32 OSXGLWindow::handleEvents() {
	XEvent event;
	sint32 retVal = 0;

	// handle the events in the queue
	while (XPending(display_) > 0) {
		XNextEvent(display_, &event);
		switch (event.type) {
		case Expose:
			if (event.xexpose.count != 0)
				break;
			//renderGL();
			break;

		case ConfigureNotify:
			// call resize only if our window-size changed
			if ((event.xconfigure.width != width_) || (event.xconfigure.height
					!= height_)) {
				resize(event.xconfigure.width, event.xconfigure.height);
			}
			break;

			// exit in case of a mouse button press
		case ButtonPress:
			retVal = -1;
			break;

		case KeyPress:
			if (XLookupKeysym(&event.xkey, 0) == XK_Escape) {
				retVal = -1;
			}
			if (XLookupKeysym(&event.xkey, 0) == XK_F1) {
				destroy();
				//fullscreen_ = !fullscreen_;
				setFullscreen(!isFullscreen());
				create(width_, height_, depth_);
			}
			break;

		case ClientMessage:
			if (strcmp(XGetAtomName(display_, event.xclient.message_type),
					"WM_PROTOCOLS") == 0) {
				retVal = -1;
			}
			break;

		default:
			break;
		}
	}

	return retVal;
}

void OSXGLWindow::setFullscreen(bool enabled) {
	if (fullscreen_ && !enabled) {
		fullscreen_ = False;
	} else if (!fullscreen_ && enabled) {
		fullscreen_ = True;
	}
}

bool OSXGLWindow::isFullscreen() {
	if (fullscreen_)
		return true;
	else
		return false;
}

void OSXGLWindow::beginRender() {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void OSXGLWindow::endRender() {
	// swap the buffers if we have doublebuffered
	if (doubleBuffered_) {
		glXSwapBuffers(display_, window_);
	}
}

void OSXGLWindow::render() {
	beginRender();

	test->render();

	endRender();
}

}

}
#endif // OSX ONLY COMPILATION
