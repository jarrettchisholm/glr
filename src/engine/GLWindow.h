/*
 * GLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#ifdef _WIN32
#include <windows.h>
#endif

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include "IWindow.h"
#include "DefaultSceneNode.h"
#include "DefaultSceneManager.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

namespace icee {

namespace engine {

using namespace compatibility;

class GLWindow: public IWindow {
private:
	GUI* gui_;

protected:
	DefaultSceneManager* sMgr_;

public:
	GLWindow();
	virtual ~GLWindow();

	virtual void* getWindowPointer();

	virtual void resize(uint32 width, uint32 height);
	virtual sint32 initialize();
	virtual void destroy();
	sint32 handleEvents();
	virtual void render();
	virtual bool blah() {
		return false;
	}

	virtual uint32 getWidth();
	virtual uint32 getHeight();
	virtual uint32 getDepth();

	virtual ISceneManager* getSceneManager();
	
	virtual IGUI* getHtmlGui();
};

}

}

#endif /* GLWINDOW_H_ */
