/*
 * IceGraphicsEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "IceGraphicsEngine.h"

namespace icee {

namespace engine {

/**
 *
 */
IIceWindow* IceGraphicsEngine::createWindow(uint32 width, uint32 height,
		bool fullscreen, bool vsync) {

	return 0;
}

/**
 *
 */
IOS* IceGraphicsEngine::createIOSObject() {
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
