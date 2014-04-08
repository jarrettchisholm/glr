#ifndef GLRPROGRAM_H_
#define GLRPROGRAM_H_

#include <memory>
#include <unordered_map>

#include "Configure.hpp"

#ifdef OS_WINDOWS
#include <windows.h>
#endif

#include "glw/IOpenGlDevice.hpp"
#include "IWindow.hpp"
#include "BasicSceneNode.hpp"
#include "BasicSceneManager.hpp"
#include "Light.hpp"

#include "gui/IGui.hpp"


#include "models/IModelManager.hpp"
#include "models/IBillboardManager.hpp"

#include "glw/shaders/IShaderProgramManager.hpp"
#include "glw/shaders/IShaderProgramBindListener.hpp"

namespace glr
{

/**
 * @mainpage Graphics Language Renderer Documentation
 *
 * @b Note: This documentation is for an alpha product, and is not necessarily representative of the final product.
 * 
 */

/**
 * Used to pass in GlrProgram settings (so we don't have to have a method with a whole ton of parameters).
 */
struct ProgramSettings
{
	ProgramSettings() : defaultTextureDir("")
	{
	}
	
	std::string defaultTextureDir;
};

/**
 * 
 */
class GlrProgram : public shaders::IShaderProgramBindListener
{
public:
	GlrProgram(ProgramSettings properties = ProgramSettings());
	virtual ~GlrProgram();

	void beginRender();
	void endRender();
	virtual void render();

	IWindow* createWindow(std::string name = std::string("Window"), std::string title = std::string("Untitled Window"), glm::detail::uint32 width = 800, glm::detail::uint32 height = 600, glm::detail::uint32 depth = 24,
						  bool fullscreen = false, bool vsync = false);

	void setSceneManager(std::unique_ptr<ISceneManager> sceneManager);

	IWindow* getWindow();
	ISceneManager* getSceneManager();
	gui::IGui* getHtmlGui();
	glw::IOpenGlDevice* getOpenGlDevice();
	models::IModelManager* getModelManager() const;
	models::IBillboardManager* getBillboardManager() const;
	
	void reloadShaders();
	
	/* Implementation of IShaderProgramBindListener method */
	virtual void shaderBindCallback(shaders::IShaderProgram* shader);

private:
	glm::detail::uint32 numLights_;
	std::map<std::string, std::vector<GLuint> > lightUbos_;
	std::map<std::string, std::vector<GLuint> > materialUbos_;

	// Should this be IOpenGlDevice instead of OpenGlDevice?
	std::unique_ptr< glw::IOpenGlDevice > openGlDevice_;
	std::unique_ptr< models::IModelManager > modelManager_;
	std::unique_ptr< models::IBillboardManager > billboardManager_;
	std::unique_ptr< ISceneManager > sMgr_;
	std::unique_ptr< IWindow > window_;
	std::unique_ptr< gui::IGui > gui_;
	
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
