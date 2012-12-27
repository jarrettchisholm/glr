/*
 * GraphicsEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "GraphicsEngine.h"

namespace icee {

namespace engine {

/**
 *
 */
IWindow* GraphicsEngine::createWindow(uint32 width, uint32 height,
		bool fullscreen, bool vsync) {

	return 0;
}

/**
 *
 */
IOS* GraphicsEngine::createIOSObject() {
	IOS* retObj = 0;
#ifdef _WIN32
	retObj = new icee::windowsos::OSWindows();
#endif

#ifdef linux
	retObj = new icee::linuxos::OSLinux();
#endif

#ifdef __APPLE__
	retObj = new icee::macosx::OSX();
#endif

	return retObj;
}

}

}
