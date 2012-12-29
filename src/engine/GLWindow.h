/*
 * GLWindow.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

#include <SFML/Window.hpp>

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include "IWindow.h"
#include "DefaultSceneNode.h"
#include "DefaultSceneManager.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

namespace oglre {

namespace engine {

class GLWindow: public IWindow {
private:
	sf::Window* window_;
	std::unique_ptr<GUI> gui_;

protected:
	DefaultSceneManager* sMgr_;

public:
	GLWindow(int width, int height, std::string title);
	virtual ~GLWindow();

	virtual WindowHandle getWindowHandle();
	virtual IWindow::InternalWindow getInternalWindowPointer();

	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height);
	virtual glm::detail::int32 initialize();
	virtual void destroy();
	glm::detail::int32 handleEvents();
	
	void beginRender();
	void endRender();
	virtual void render();
	virtual bool blah() {
		return false;
	}

	virtual glm::detail::uint32 getWidth();
	virtual glm::detail::uint32 getHeight();
	virtual glm::detail::uint32 getDepth();

	virtual ISceneManager* getSceneManager();
	
	virtual IGUI* getHtmlGui();
	
	void DrawAQuad();
};

}

}

#endif /* GLWINDOW_H_ */
