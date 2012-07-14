/*
 * IceGraphicsEngine.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef ICEGRAPHICSENGINE_H_
#define ICEGRAPHICSENGINE_H_

#include "IIceWindow.h"
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

class IceGraphicsEngine {
public:
	static IIceWindow* createWindow(uint32 width = 800, uint32 height = 600,
			bool fullscreen = false, bool vsync = false);

	static IOS* createIOSObject();

private:
	/**
	 * Private.
	 */
	IceGraphicsEngine() {
	}
	;

	/**
	 * Private.
	 */
	IceGraphicsEngine(const IceGraphicsEngine& iceGE) {
	}
	;

	/**
	 * Private.
	 */
	virtual ~IceGraphicsEngine() {
	}
	;
};

}

}

#endif /* ICEGRAPHICSENGINE_H_ */
