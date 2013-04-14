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

#ifdef _WIN32
#include <windows.h>
#endif

#include "IMatrixData.h"
#include "IOpenGlDevice.h"
#include "IWindow.h"
#include "BasicSceneNode.h"
#include "BasicSceneManager.h"
#include "Light.h"

//#include "gui/IGUI.h"
#include "gui/GUI.h"

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShaderProgramBindListener.h"


namespace glr {
class GlrProgram : public shaders::IShaderProgramBindListener, public IMatrixData, public IOpenGlDevice {
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
	
	virtual const glm::mat4& getViewMatrix();
	virtual const glm::mat4& getProjectionMatrix();
	virtual const glm::mat4& getModelMatrix();
	
	virtual GLuint createBufferObject(glmd::uint32 totalSize, void* dataPointer);
	virtual void releaseBufferObject(GLuint bufferId);
	virtual GLuint bindBuffer(GLuint bufferId);
	virtual GLuint unbindBuffer(GLuint bufferId);
	
	virtual void shaderBindCallback(shaders::IShaderProgram* shader);

	//void destroyWindow();
	//void destroySceneManager();

private:
	glm::detail::uint32 numLights_;
	std::map<std::string, std::vector<GLuint> > lightUbos_;
	std::map<std::string, std::vector<GLuint> > materialUbos_;
	
	std::vector<GLuint> bufferIds_;
	std::vector<GLuint> bindPoints_;
	std::unordered_map<GLuint, GLuint> boundBuffers_;
	GLint maxNumBindPoints_;

	std::unique_ptr< shaders::ShaderProgramManager > shaderProgramManager_;
	std::unique_ptr< BasicSceneManager > sMgr_;
	std::unique_ptr< IWindow > window_;
	std::unique_ptr< gui::GUI > gui_;

	void setupUniformBufferObjectBindings(shaders::IShaderProgram* shader);
	void setupLightUbo(std::string name, shaders::IShaderProgram* shader);
	void releaseLightUbo(std::string name);
	void bindUniformBufferObjects(shaders::IShaderProgram* shader);

	void initialize();
};
}
#endif /* GLRPROGRAM_H_ */
