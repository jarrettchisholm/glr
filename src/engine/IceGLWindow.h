/*
 * IceGLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef ICEGLWINDOW_H_
#define ICEGLWINDOW_H_

#ifdef _WIN32
#include <windows.h>
#endif

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include "IIceWindow.h"
#include "DefaultSceneNode.h"
#include "DefaultSceneManager.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

namespace icee {

namespace engine {

using namespace compatibility;

class IceGLWindow: public IIceWindow {
private:
	GUI* gui_;

protected:
	DefaultSceneManager* sMgr_;

public:
	IceGLWindow();
	virtual ~IceGLWindow();

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
	
	virtual IGUI* createHtmlGui();
};

}

}

#endif /* ICEGLWINDOW_H_ */
