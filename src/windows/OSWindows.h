/*
 * OSWindows.h
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

// WINDOWS ONLY COMPILATION
#ifdef _WIN32

#ifndef OSWINDOWS_H_
#define OSWINDOWS_H_

/* printf */
#include <stdio.h>

#include "../engine/OS.h"

#include "WindowsGLWindow.h"

namespace icee {

namespace windowsos {

using namespace compatibility;

class OSWindows: public icee::engine::OS {
public:
	OSWindows();
	virtual ~OSWindows();

	virtual sint32 initialize();

	virtual icee::engine::IIceWindow* createWindow(uint32 width = 800, uint32 height = 600,
			uint32 depth = 24, bool fullscreen = false, bool vsync = false);
	void renderGL();

	virtual void run();

private:
	float32 rotQuad_;

	WindowsGLWindow* lWin_;

	void DrawAQuad();
};

}

}

#endif /* OSWINDOWS_H_ */

#endif // WINDOWS ONLY COMPILATION
