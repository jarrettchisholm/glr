/*
 * IWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "ISceneManager.h"

#include "gui/IGUI.h"

namespace oglre {

class IWindow {
public:
	virtual ~IWindow() {
	}
	;
	
#if defined(_WIN32)
    // Windows
    typedef HWND* WindowHandle;
#elif defined(linux) || defined(freebsd)
    // Unix - X11
    typedef unsigned long WindowHandle;
#elif defined(__APPLE__)
    // Mac OS X - Cocoa
    typedef void* WindowHandle;
#endif

	typedef void* InternalWindow;

	virtual WindowHandle getWindowHandle() = 0;
	virtual InternalWindow getInternalWindowPointer() = 0;

	virtual glm::detail::int32 initialize() = 0;
	virtual void destroy() = 0;
	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height) = 0;
	virtual glm::detail::int32 handleEvents() = 0;
	virtual void render() = 0;
	virtual bool blah() = 0;

	virtual ISceneManager* getSceneManager() = 0;
	
	virtual gui::IGUI* getHtmlGui() = 0;

protected:
	glm::detail::int32 x_, y_;
	glm::detail::uint32 width_, height_;
	glm::detail::uint32 depth_;
};

}

#endif /* IWINDOW_H_ */
