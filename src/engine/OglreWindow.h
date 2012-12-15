/*
 * OglreWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef OGLREWINDOW_H_
#define OGLREWINDOW_H_

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <SFML/Window.hpp>

#include "IOglreWindow.h"
#include "DefaultSceneNode.h"
#include "DefaultSceneManager.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

#include "../common/compatibility/Types.h"

namespace icee {

namespace engine {

using namespace compatibility;

class OglreWindow: public IOglreWindow {
private:
	GUI* gui_;
	
	sf::Window sfmlWindow_;

protected:
	DefaultSceneManager* sMgr_;

public:
	OglreWindow(uint32 width = 800, uint32 height = 600, bool fullscreen = false, bool vsync = false);
	virtual ~OglreWindow();

	virtual void* getWindowPointer();

	virtual void resize(uint32 width, uint32 height);
	virtual sint32 initialize();
	virtual void destroy();
	sint32 handleEvents();
	
	void beginRender();
	void endRender();
	virtual void render();

	virtual uint32 getWidth();
	virtual uint32 getHeight();
	virtual uint32 getDepth();

	virtual ISceneManager* getSceneManager();
	
	virtual IGUI* getHtmlGui();
};

}

}

#endif /* OGLREWINDOW_H_ */
