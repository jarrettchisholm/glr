/*
 * OSLinux.h
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

// LINUX ONLY COMPILATION
#ifdef linux

#ifndef OSLINUX_H_
#define OSLINUX_H_

/* printf */
#include <stdio.h>

#include "../engine/OS.h"

#include "LinuxGLWindow.h"

namespace icee {

namespace linuxos {

using namespace compatibility;

class OSLinux: public icee::engine::OS {
public:
	OSLinux();
	virtual ~OSLinux();

	virtual sint32 initialize();

	virtual icee::engine::IWindow* createWindow(uint32 width = 800, uint32 height = 600,
			uint32 depth = 24, bool fullscreen = false, bool vsync = false);
	void renderGL();

	virtual void run();
	//void test();

private:
	float32 rotQuad_;

	LinuxGLWindow* lWin_;

	void DrawAQuad();
};

}

}

#endif /* OSLINUX_H_ */

#endif // LINUX ONLY COMPILATION
