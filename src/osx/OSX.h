/*
 * OSX.h
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

// OSX ONLY COMPILATION
#ifdef __APPLE__

#ifndef OSX_HH_
#define OSX_HH_

/* printf */
#include <stdio.h>

#include "../engine/OS.h"

#include "OSXGLWindow.h"

namespace icee {

namespace macosx {

using namespace compatibility;

class OSX: public icee::engine::OS {
public:
	OSX();
	virtual ~OSX();

	virtual sint32 initialize();

	virtual icee::engine::IceWindow* createWindow(uint32 width = 800,
			uint32 height = 600, uint32 depth = 24, bool fullscreen = false,
			bool vsync = false);
	void renderGL();

	virtual void run();
	//void test();

private:
	float32 rotQuad_;

	OSXGLWindow* lWin_;

	void DrawAQuad();
};

}

}

#endif /* OSX_HH_ */

#endif // OSX ONLY COMPILATION
