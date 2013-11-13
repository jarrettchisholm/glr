/*
 * IWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "Configure.h"


#include <windows.h>


#include "gui/IGui.h"

namespace glr {

class IWindow {
public:
	virtual ~IWindow()
	{
	}
	;

#if defined(OS_LINUX)
	typedef unsigned long WindowHandle;
#elif defined(OS_WINDOWS)
	typedef HWND* WindowHandle;
#elif defined(OS_MAC)
	typedef void* WindowHandle;
#endif

	typedef void* InternalWindow;

	virtual WindowHandle getWindowHandle() = 0;
	virtual InternalWindow getInternalWindowPointer() = 0;

	virtual void destroy() = 0;
	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height) = 0;
	virtual glm::detail::int32 handleEvents() = 0;
	virtual void render() = 0;

	virtual glm::detail::uint32 getWidth() = 0;
	virtual glm::detail::uint32 getHeight() = 0;
	virtual glm::vec2 getPosition() = 0;
	virtual glm::detail::uint32 getDepth() = 0;
	virtual const glm::mat4& getProjectionMatrix() = 0;
};

}
#endif /* IWINDOW_H_ */
