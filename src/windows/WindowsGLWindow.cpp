/*
 * LinuxGLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

// WINDOWS ONLY COMPILATION
#ifdef _WIN32

#include "WindowsGLWindow.h"

// for strcmp function
#include <string.h>

#include <iostream>

#include <Wingdi.h>

namespace icee
{

namespace windowsos
{

WindowsGLWindow::WindowsGLWindow()
{
	fullscreen_ = false;
	doubleBuffered_ = false;
}

WindowsGLWindow::~WindowsGLWindow()
{
}

void* WindowsGLWindow::getWindowPointer()
{
	return &hWindow_;
}

sint32 WindowsGLWindow::create(uint32 width, uint32 height, uint32 depth, bool fullscreen,
		bool vsync)
{
	width_ = width;
	height_ = height;
	depth_ = depth;

	char* title = "WINDOWS window";
	int x = 0;
	int y = 0;
	BYTE type = PFD_TYPE_RGBA;
	DWORD flags = 0;

	int pf;
	HDC hDC;
	WNDCLASS wc;
	PIXELFORMATDESCRIPTOR pfd;
	static HINSTANCE hInstance = 0;

	/* only register the window class once - use hInstance as a flag. */
	if (!hInstance) {
		hInstance = (HINSTANCE) GetModuleHandle(0);

		wc.style = CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC) &WindowsGLWindow::WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = "WindowsGLWindow";

		if (!RegisterClass(&wc)) {
			MessageBox(NULL, "RegisterClass() failed:  "
				"Cannot register window class.", "Error", MB_OK);
			return 0;
		}
	}

	hWindow_ = CreateWindow("WindowsGLWindow", title, WS_OVERLAPPEDWINDOW |
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
			x, y, width, height, NULL, NULL, hInstance, (LPVOID*)this);

	if (hWindow_ == NULL) {
		MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.", "Error", MB_OK);
		return 0;
	}

	hDC = GetDC(hWindow_);

	/* there is no guarantee that the contents of the stack that become
	 the pfd are zeroed, therefore _make sure_ to clear these bits. */
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | flags;
	doubleBuffered_ = true;
	pfd.iPixelType = type;
	pfd.cColorBits = 32;
	pfd.cDepthBits = depth;

	pf = ChoosePixelFormat(hDC, &pfd);
	if (pf == 0) {
		MessageBox(NULL, "ChoosePixelFormat() failed:  "
			"Cannot find a suitable pixel format.", "Error", MB_OK);
		return 0;
	}

	if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat() failed:  "
			"Cannot set format specified.", "Error", MB_OK);
		return 0;
	}

	DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	ReleaseDC(hWindow_, hDC);

	if (hWindow_ == 0)
		return -1;

	STARTUPINFO startupInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	GetStartupInfo(&startupInfo);

	hDeviceContext_ = GetDC(hWindow_);
	hRenderContext_ = wglCreateContext(hDeviceContext_);
	wglMakeCurrent(hDeviceContext_, hRenderContext_);

	//ShowWindow(hWindow_, startupInfo.wShowWindow/*nCmdShow*/);
	ShowWindow(hWindow_, SW_SHOW);
	UpdateWindow(hWindow_);

	return 0;
}

void WindowsGLWindow::destroy()
{
	if (hWindow_) {
		if (!wglMakeCurrent(0, 0)) {
			printf("Could not release drawing context.\n");
		}

		if (ReleaseDC(hWindow_, hDeviceContext_) != 1) // return value of 1 means success
			printf("Could not release Device Context.\n");

		// destroy the context
		if (!wglDeleteContext(hRenderContext_))
			printf("Could not release Render Context.\n");

		if (DestroyWindow(hWindow_) == 0) // 0 means failure
			printf("Could not destroy the window.\n");

		hDeviceContext_ = 0;
		hRenderContext_ = 0;
		hWindow_ = 0;
	}

	// switch back to original desktop resolution if we were in fullscreen
	if (fullscreen_) {
		//XF86VidModeSwitchToMode(display_, screen_, &originalMode_);
		//XF86VidModeSetViewPort(display_, screen_, 0, 0);
	}
	//XCloseDisplay(display_);
}

sint32 WindowsGLWindow::initialize()
{
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

const uint32 WindowsGLWindow::WM_NEED_RESIZE = WM_USER + 100;
LONG WINAPI WindowsGLWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static PAINTSTRUCT ps;// do I need this?

	if (uMsg == WM_CREATE) {
		LPCREATESTRUCT lpCreate = (LPCREATESTRUCT) lParam;
		WindowsGLWindow* pThis = (WindowsGLWindow*) lpCreate->lpCreateParams;
		SetWindowLong(hWnd, GWLP_USERDATA, (LONG_PTR) pThis);
		return 0;
	}

	WindowsGLWindow* pThis = 0;

	switch (uMsg) {
		case WM_PAINT:
			//display();
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0;

		case WM_SIZE:
			pThis = (WindowsGLWindow*) GetWindowLong(hWnd, GWLP_USERDATA);
			if (pThis) {
				std::cout << "OK: WindowsGLWindow::WM_NEED_RESIZE" << std::endl;

				// call resize if our window-size changed
				pThis->resize(LOWORD(lParam), HIWORD(lParam));

				PostMessage(hWnd, WM_PAINT, 0, 0);
			}

			return 0;

		case WM_CHAR:
			switch (wParam) {
				case 27: /* ESC key */
					PostQuitMessage(0);
					break;
			}
			return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

sint32 WindowsGLWindow::handleEvents()
{
	sint32 retVal = 0;

	// PM_QS_SENDMESSAGE instead of PM_REMOVE?
	// PM_QS_INPUT -> only process keyboard/mouse messages
	// PM_REMOVE -> Messages are removed from the queue after processing by PeekMessage.

	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE) == TRUE) // maybe use GetInputState(?) as well?
	{
		if (msg.message == WM_QUIT)
			retVal = -1;

		switch (msg.message) {
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			case WM_KEYUP:
			case WM_KEYDOWN:
				/*
				 switch (msg.wParam) {
				 case 'W':
				 // w key pressed
				 break;
				 case VK_RIGHT:
				 // Right arrow pressed
				 break;
				 default:
				 break;
				 }
				 */
				break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	while (PeekMessage(&msg, 0, WM_IME_STARTCOMPOSITION, WM_CTLCOLORSTATIC, PM_REMOVE) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	while (PeekMessage(&msg, 0, WM_PARENTNOTIFY, WM_PENWINLAST, PM_REMOVE) == TRUE) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	/*
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
	 if ((event.xconfigure.width != width_) || (event.xconfigure.height != height_)) {
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
	 if (strcmp(XGetAtomName(display_, event.xclient.message_type), "WM_PROTOCOLS") == 0) {
	 retVal = -1;
	 }
	 break;

	 default:
	 break;
	 }
	 }
	 */

	return retVal;
}

void WindowsGLWindow::setFullscreen(bool enabled)
{
	fullscreen_ = enabled;
}

bool WindowsGLWindow::isFullscreen()
{
	return fullscreen_;
}

void WindowsGLWindow::beginRender()
{
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}

void WindowsGLWindow::endRender()
{
	// swap the buffers if we have doublebuffered
	if (doubleBuffered_) {
		SwapBuffers(hDeviceContext_);
	}
}

void WindowsGLWindow::render()
{
	beginRender();

	test->render();

	endRender();
}

}

}
#endif // WINDOWS ONLY COMPILATION
