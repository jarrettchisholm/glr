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

#include "IWindow.h"
#include "DefaultSceneNode.h"
#include "DefaultSceneManager.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

namespace oglre {



class GLWindow: public IWindow {
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
	
	virtual gui::IGUI* getHtmlGui();
	
	void DrawAQuad();
	void LoadAQuad();
	
protected:
	std::unique_ptr<DefaultSceneManager> sMgr_;
	
private:
	std::unique_ptr<sf::Window> window_;
	std::unique_ptr<gui::GUI> gui_;
	
	glm::mat4 projectionMatrix_;
	glm::mat4 modelMatrix_;
	
	unsigned int vaoID[1]; // Our Vertex Array Object  
  
	unsigned int vboID[1]; // Our Vertex Buffer Object
};

}

#endif /* GLWINDOW_H_ */
