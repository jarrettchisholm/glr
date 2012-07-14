/*
 * WindowsGLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// WINDOWS ONLY COMPILATION
#ifdef _WIN32

#ifndef WINDOWSGLWINDOW_H_
#define WINDOWSGLWINDOW_H_

#include "../engine/IceGLWindow.h"

/* printf */
#include <stdio.h>

#include <windows.h>
/* gl.h we need OpenGL */
#include <GL/gl.h>


namespace icee {

namespace windowsos {

using namespace compatibility;

class WindowsGLWindow: public icee::engine::IceGLWindow {
public:
	WindowsGLWindow();
	virtual ~WindowsGLWindow();

	virtual void* getWindowPointer();

	virtual sint32 initialize();
	virtual sint32 create(uint32 width, uint32 height, uint32 depth, bool fullscreen = false,
			bool vsync = false);
	void destroy();

	sint32 handleEvents();

	void beginRender();
	void endRender();
	virtual void render();

	void setFullscreen(bool enabled);
	bool isFullscreen();

	static LONG WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static const uint32 WM_NEED_RESIZE;

private:
	bool fullscreen_;
	bool doubleBuffered_;

	/* our window handle */
	HWND  hWindow_;
	// Device Context
	HDC hDeviceContext_;
	// OpenGL Context
	HGLRC hRenderContext_;
};

}

}

#endif /* WINDOWSGLWINDOW_H_ */

#endif // WINDOWS ONLY COMPILATION
