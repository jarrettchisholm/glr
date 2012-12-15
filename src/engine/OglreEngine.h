/*
 * OglerEngine.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef OGLREENGINE_H_
#define OGLREENGINE_H_

#include <memory>

#include "OglreWindow.h"
#include "IOS.h"

// we require a C++ compiler.
#if !defined(__cplusplus)
#error C++ compiler required.
#endif

// check if we are using windows
#ifdef _WIN32
#include "../windows/Common.h"
#define SUPPORTED_PLATFORM 1
#endif

// check if we are using linux
#ifdef linux
#include "../linux/Common.h"
#define SUPPORTED_PLATFORM 1
#endif

// check if we are using osx
#ifdef __APPLE__ //__MAC_OS_X_VERSION_MAX_ALLOWED
#include "../osx/Common.h"
#define SUPPORTED_PLATFORM 1
#endif

// error check
#ifndef SUPPORTED_PLATFORM
#error Your platform is not supported.
#endif

namespace icee {

namespace engine {

using namespace compatibility;

class OglreEngine {
public:
	static IOglreWindow* createWindow(uint32 width = 800, uint32 height = 600, bool fullscreen = false, bool vsync = false);

	static IOS* createIOSObject();

private:
	static bool windowCreated_;
	static std::unique_ptr<OglreWindow> oglerWindow_;

	/**
	 * Private.
	 */
	OglreEngine() {
		windowCreated_ = false;
	}
	;

	/**
	 * Private.
	 */
	OglreEngine(const OglreEngine& iceGE) {
	}
	;

	/**
	 * Private.
	 */
	virtual ~OglreEngine() {
	}
	;
};

}

}

#endif /* OGLREENGINE_H_ */
