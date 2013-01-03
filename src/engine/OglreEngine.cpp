/*
 * OglreEngine.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include "OglreEngine.h"

namespace icee {

namespace engine {

/**
 * Method createWindow
 * 
 * Creates a new window and returns it.  You cannot have more than one window at a time - if you try to
 * create more than one window, it will return a null pointer.
 */
IOglreWindow* OglreEngine::createWindow(uint32 width, uint32 height, bool fullscreen, bool vsync) {
	if (windowCreated_) {
		BOOST_LOG_TRIVIAL(error) << "Could not create window - a window was already created!";
		return 0;
	}
	
	OglreWindow* window = new OglreWindow(width, height, fullscreen, vsync);
	
	if (window->initialize() != 0) {
		BOOST_LOG_TRIVIAL(error) << "Could not initialize window.";
		delete window;
		return 0;
	}
	
	windowCreated_ = true;
	
	oglerWindow_ = std::unique_ptr<OglreWindow>( window );
	return oglerWindow_.get();
}

/**
 *
 */
IOS* OglreEngine::createIOSObject() {
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
