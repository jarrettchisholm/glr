/*
 * IWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <string>

#include "../common/compatibility/Types.h"

#include "ISceneManager.h"

#include "gui/IGUI.h"

namespace oglre {

namespace engine {

using namespace compatibility;

class IWindow {
public:
	virtual ~IWindow() {
	}
	;

	virtual void* getWindowPointer() = 0;

	virtual sint32 initialize() = 0;
	virtual void destroy() = 0;
	virtual void resize(uint32 width, uint32 height) = 0;
	virtual sint32 handleEvents() = 0;
	virtual void render() = 0;
	virtual bool blah() = 0;

	virtual ISceneManager* getSceneManager() = 0;
	
	virtual IGUI* getHtmlGui() = 0;

protected:
	sint32 x_, y_;
	uint32 width_, height_;
	uint32 depth_;
};

}

}

#endif /* IWINDOW_H_ */
