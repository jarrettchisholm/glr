/*
 * GlrProgram.h
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#ifndef GLRPROGRAM_H_
#define GLRPROGRAM_H_

#include <memory>

#ifdef _WIN32
#include <windows.h>
#endif

#include "IWindow.h"
#include "BasicSceneNode.h"
#include "BasicSceneManager.h"
#include "Light.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShaderProgramBindListener.h"


namespace glr {
class GlrProgram : public shaders::IShaderProgramBindListener {
public:
	GlrProgram();
	virtual ~GlrProgram();

	void beginRender();
	void endRender();
	virtual void render();

	IWindow* createWindow(std::string name = "Window", std::string title = "Untitled Window", glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
						  bool fullscreen = false, bool vsync = false);

	IWindow* getWindow();
	ISceneManager* getSceneManager();
	gui::IGUI* getHtmlGui();
	
	void shaderBindCallback(shaders::IShaderProgram* shader);

	//void destroyWindow();
	//void destroySceneManager();

private:
	std::map<std::string, std::vector<GLuint> > lightUbos_;
	std::map<std::string, std::vector<GLuint> > materialUbos_;

	std::unique_ptr< shaders::ShaderProgramManager > shaderProgramManager_;
	std::unique_ptr< BasicSceneManager > sMgr_;
	std::unique_ptr< IWindow > window_;
	std::unique_ptr< gui::GUI > gui_;

	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name, shaders::IShaderProgram* shader);
	void bindUniformBufferObjects(shaders::IShaderProgram* shader);

	void initialize();
};
}
#endif /* GLRPROGRAM_H_ */
