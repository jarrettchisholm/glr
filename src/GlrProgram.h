/*
 * GlrProgram.h
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#ifndef GLRPROGRAM_H_
#define GLRPROGRAM_H_

#include <memory>
#include <unordered_map>

#include "Configure.h"

#if defined(OS_WINDOWS)
#include <windows.h>
#endif

#include "glw/OpenGlDevice.h"
#include "IWindow.h"
#include "BasicSceneNode.h"
#include "BasicSceneManager.h"
#include "Light.h"

#include "gui/cef/Gui.h"

#include "glw/shaders/IShaderProgramManager.h"
#include "glw/shaders/IShaderProgramBindListener.h"


namespace glr {

/**
 * Used to pass in GlrProgram settings (so we don't have to have a method with a whole ton of parameters).
 */
struct ProgramSettings {
	ProgramSettings() : defaultTextureDir("")
	{
	}
	
	std::string defaultTextureDir;
};

/**
 * 
 */
class GlrProgram : public shaders::IShaderProgramBindListener {
public:
	GlrProgram(ProgramSettings properties = ProgramSettings());
	virtual ~GlrProgram();

	void beginRender();
	void endRender();
	virtual void render();

	IWindow* createWindow(std::string name = "Window", std::string title = "Untitled Window", glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
						  bool fullscreen = false, bool vsync = false);

	void setSceneManager(std::shared_ptr<ISceneManager> sceneManager);

	IWindow* getWindow();
	ISceneManager* getSceneManager();
	gui::IGui* getHtmlGui();
	glw::IOpenGlDevice* getOpenGlDevice();
	
	void reloadShaders();
	
	/* Implementation of IShaderProgramBindListener method */
	virtual void shaderBindCallback(shaders::IShaderProgram* shader);

private:
	glm::detail::uint32 numLights_;
	std::map<std::string, std::vector<GLuint> > lightUbos_;
	std::map<std::string, std::vector<GLuint> > materialUbos_;

	std::unique_ptr< glw::OpenGlDevice > openGlDevice_;
	std::shared_ptr< ISceneManager > sMgr_;
	std::unique_ptr< IWindow > window_;
	
	// Would it be better to use IGui instead of the cef Gui class here?
	std::unique_ptr< gui::cef::Gui > gui_;
	
	shaders::IShaderProgramManager* shaderProgramManager_;
	
	ProgramSettings settings_;

	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name, shaders::IShaderProgram* shader);
	void releaseLightUbo(std::string name);
	void bindUniformBufferObjects(shaders::IShaderProgram* shader);

	void initialize(ProgramSettings settings);
	void initializeProperties(ProgramSettings settings);
};
}
#endif /* GLRPROGRAM_H_ */
