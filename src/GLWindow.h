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
#include "Light.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

#include "shaders/ShaderProgramManager.h"

namespace glr {

struct LightSource {
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 position;
	glm::vec4 direction;
};

class GLWindow: public IWindow {
public:
	GLWindow(int width, int height, std::string title);
	virtual ~GLWindow();

	virtual WindowHandle getWindowHandle();
	virtual IWindow::InternalWindow getInternalWindowPointer();

	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height);
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
	virtual glm::vec2 getPosition();
	virtual glm::detail::uint32 getDepth();

	virtual ISceneManager* getSceneManager();
	
	virtual gui::IGUI* getHtmlGui();
	
protected:
	std::unique_ptr<DefaultSceneManager> sMgr_;
	
private:
	std::unique_ptr<sf::Window> window_;
	std::unique_ptr<gui::GUI> gui_;
	
	glm::mat4 projectionMatrix_;
	
	std::map<std::string, std::vector<GLuint>> lightUbos_;
	
	std::unique_ptr< shaders::ShaderProgramManager > shaderProgramManager_;
	

	void initialize();
	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name);
	void bindUniformBufferObjects();
};

}

#endif /* GLWINDOW_H_ */
