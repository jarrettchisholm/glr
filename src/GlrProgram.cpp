#include <algorithm>
#include <utility>

#include <GL/glew.h>

#include "Window.hpp"

#include "glw/OpenGlDevice.hpp"
#include "glw/shaders/ShaderProgramManager.hpp"
#include "glw/shaders/IShader.hpp"
#include "exceptions/GlException.hpp"

#include "GlrProgram.hpp"

#include "common/logger/Logger.hpp"

#include "gui/cef/Gui.hpp"
#include "models/ModelManager.hpp"
#include "models/BillboardManager.hpp"

namespace glr
{

GlrProgram::GlrProgram(ProgramSettings settings)
{
	initialize( settings );
}

GlrProgram::~GlrProgram()
{
}

/**
 * Will setup our GlrProgram.
 * 
 * @param settings Used to initialize the GlrProgram settings
 */
void GlrProgram::initialize(ProgramSettings settings)
{
	initializeProperties( settings );
	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );

	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);

	//shaders::ShaderProgramManager::getInstance();

	openGlDevice_ = std::unique_ptr<glw::OpenGlDevice>();
	modelManager_ = std::unique_ptr<models::IModelManager>();
	billboardManager_ = std::unique_ptr<models::IBillboardManager>();
}

/**
 * Will override any settings in the GlrProgram settings with values set in the parameter settings.
 * 
 * @param settings The settings used to override the GlrProgram settings
 */
void GlrProgram::initializeProperties(ProgramSettings settings)
{
	if ( !settings.defaultTextureDir.empty() )
	{
		settings_.defaultTextureDir = settings.defaultTextureDir;
	}
}

/**
 *
 */
IWindow* GlrProgram::createWindow(std::string name, std::string title,
								  glm::detail::uint32 width, glm::detail::uint32 height, glm::detail::uint32 depth,
								  bool fullscreen, bool vsync)
{
	if ( window_.get() != nullptr )
	{
		std::stringstream msg;
		msg << "Cannot create window - Window already exists.";
		LOG_ERROR( msg.str() );
		throw exception::Exception(msg.str());
	}
	LOG_DEBUG( "Creating window." );
	window_ = std::unique_ptr<IWindow>(new Window(width, height, title));
	
	LOG_DEBUG( "Initializing GLEW." );
	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if ( glewInit() != GLEW_OK )
	{
		std::string msg("Failed to initialize GLEW.");
		LOG_WARN( msg );
		throw exception::GlException(msg);
	}
	
	LOG_DEBUG( "OpenGL setup." );
	// Setup settings for open gl device
	glr::glw::OpenGlDeviceSettings settings = glr::glw::OpenGlDeviceSettings();
	if ( !settings_.defaultTextureDir.empty() )
		settings.defaultTextureDir = settings_.defaultTextureDir;
	openGlDevice_ = std::unique_ptr< glw::OpenGlDevice >( new glw::OpenGlDevice(settings) );
	
	modelManager_ = std::unique_ptr<models::IModelManager>(new models::ModelManager(openGlDevice_.get()));
	billboardManager_ = std::unique_ptr<models::IBillboardManager>(new models::BillboardManager(openGlDevice_.get()));
	
	glw::GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream msgStream;
		msgStream << "Error loading opengl: " << err.name;
		std::string msg = msgStream.str();
		
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}

	LOG_DEBUG( "Initialize shader programs." );
	// initialize shader program manager and scene manager AFTER we create the window
	shaderProgramManager_ = openGlDevice_->getShaderProgramManager();
	shaderProgramManager_->addDefaultBindListener( this );
	
	LOG_DEBUG( "Create scene manager." );
	std::unique_ptr<BasicSceneManager> sceneManager = std::unique_ptr<BasicSceneManager>(new BasicSceneManager(shaderProgramManager_, openGlDevice_.get(), modelManager_.get(), billboardManager_.get()));
	setSceneManager( std::move(sceneManager) );
	
	openGlDevice_->setProjectionMatrix( window_->getProjectionMatrix() );
	
	return window_.get();
}

void GlrProgram::setSceneManager(std::unique_ptr<ISceneManager> sceneManager)
{
	// Question: Do I want to be able to just overwrite the previous scene manager?  Or do I maybe want to throw an exception
	// if a scene manager is already set?
	sMgr_ = std::move( sceneManager );

	// Set the default shader for the scene manager
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_basic");
	assert( shader != nullptr );
	
	sMgr_->setDefaultShaderProgram(shader);
	
	LOG_DEBUG( "Set OpenGL matrices." );
	// Set our opengl matrices
	openGlDevice_->setModelMatrix( sMgr_->getModelMatrix() );
	ICamera* camera = sMgr_->getCamera();
	if (camera != nullptr)
	{
		openGlDevice_->setViewMatrix( camera->getViewMatrix() );
	}
}

void GlrProgram::beginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GlrProgram::endRender()
{
	glDisable(GL_BLEND);
	
	// Display any changes we've made
	window_->render();
}

void GlrProgram::render()
{
	beginRender();

	// Set our opengl matrices
	openGlDevice_->setModelMatrix( sMgr_->getModelMatrix() );
	ICamera* camera = sMgr_->getCamera();
	if (camera != nullptr)
	{
		openGlDevice_->setViewMatrix( camera->getViewMatrix() );
	}
	openGlDevice_->setProjectionMatrix( window_->getProjectionMatrix() );

	//setupUniformBufferObjectBindings(shader);
	//bindUniformBufferObjects(shader);
	sMgr_->drawAll();
	
	openGlDevice_->unbindAllShaderPrograms();

	if ( gui_ != nullptr )
	{
		gui_->render();
	}

	endRender();
}

void GlrProgram::reloadShaders()
{
	shaderProgramManager_->reloadShaders();
}

void GlrProgram::bindUniformBufferObjects(shaders::IShaderProgram* shader)
{
	// Bind lights
	const std::vector<LightData> lightData = sMgr_->getLightData();

	if ( lightData.size() > 0 )
	{
		if (numLights_ != lightData.size())
		{
			if (lightUbos_.begin() != lightUbos_.end())
			{
				releaseLightUbo(lightUbos_.begin()->first);
			}
			setupUniformBufferObjectBindings(shader);
		}
		
		numLights_ = lightData.size();
		
		for ( auto it = lightUbos_.begin(); it != lightUbos_.end(); ++it )
		{
			std::vector<GLuint> ubos = it->second;
			for ( auto ubo : ubos )
			{
				GLint bindPoint = shader->getBindPointByBindingName( shaders::IShader::BIND_TYPE_LIGHT );
				if ( bindPoint >= 0 )
				{
					glBindBuffer(GL_UNIFORM_BUFFER, ubo);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, lightData.size() * sizeof(LightData), &lightData[0]);
				
					openGlDevice_->bindBuffer( ubo, bindPoint );
				}
				
				//GLuint bindPoint = openGlDevice_->bindBuffer( ubo );
				//shader->bindVariableByBindingName(shaders::IShader::BIND_TYPE_LIGHT, bindPoint);
			}
		}
	}

	// TODO: bind other stuff (i.e. materials, etc)

	// TODO: bind number of lights, etc (We'll want to let the GLSL shader know how many lights to
	// iterate through - we don't want it to use garbage data

	// Get uniform variable locations
	int projectionMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "modelMatrix");
	int pvmMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "pvmMatrix");
	int normalMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "normalMatrix");

	glm::mat4 modelMatrix = sMgr_->getModelMatrix();
	glm::mat4 projectionMatrix = window_->getProjectionMatrix();

	ICamera* camera = sMgr_->getCamera();
	if ( camera != nullptr )
	{
		const glm::mat4 viewMatrix = camera->getViewMatrix();
		// Send uniform variable values to the shader
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

		glm::mat4 pvmMatrix(projectionMatrix * viewMatrix * modelMatrix);
		glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);

		glm::mat3 normalMatrix = glm::inverse(glm::transpose(glm::mat3(viewMatrix * modelMatrix)));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	}

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
}

void GlrProgram::setupUniformBufferObjectBindings(shaders::IShaderProgram* shader)
{
	shaders::IShader::BindingsMap bindings = shader->getBindings();
	
	for ( auto& b : bindings )
	{
		//std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";

		switch ( b.type )
		{
		case shaders::IShader::BIND_TYPE_LIGHT:
			if ( lightUbos_.find(b.variableName) == lightUbos_.end())
				setupLightUbo(b.variableName, shader);
			break;

		// TODO: implement other bindings (materials, etc)

		default:
			break;
		}
	}
}

void GlrProgram::setupLightUbo(std::string name, shaders::IShaderProgram* shader)
{
	// TODO: Will we want to add multiple Ubos in a single shader for light sources
	
	const std::vector<LightData> lightData = sMgr_->getLightData();

	lightUbos_[name] = std::vector<GLuint>();
	lightUbos_[name].push_back(0);

	lightUbos_[name][0] = openGlDevice_->createBufferObject(GL_UNIFORM_BUFFER, lightData.size() * sizeof(LightData), &lightData[0]);
}

void GlrProgram::releaseLightUbo(std::string name)
{
	if (lightUbos_.find(name) == lightUbos_.end())
		return;
	
	glDeleteBuffers(1, &lightUbos_[name][0]);
	
	lightUbos_.erase(name);
}

ISceneManager* GlrProgram::getSceneManager()
{
	return sMgr_.get();
}

gui::IGui* GlrProgram::getHtmlGui()
{
	if ( gui_.get() != nullptr )
	{
		return gui_.get();
	}
#ifdef USE_CEF
	gui_ = std::unique_ptr<gui::cef::Gui>(new gui::cef::Gui(openGlDevice_.get(), shaderProgramManager_, window_->getWidth(), window_->getHeight()));
	window_->addWindowResizeListener( gui_.get() );

	return gui_.get();
#else
	return nullptr;
#endif
}

glw::IOpenGlDevice* GlrProgram::getOpenGlDevice()
{
	return openGlDevice_.get();
}

models::IBillboardManager* GlrProgram::getBillboardManager() const
{
	return billboardManager_.get();
}

models::IModelManager* GlrProgram::getModelManager() const
{
	return modelManager_.get();
}

void GlrProgram::shaderBindCallback(shaders::IShaderProgram* shader)
{
	setupUniformBufferObjectBindings(shader);
	bindUniformBufferObjects(shader);
}

IWindow* GlrProgram::getWindow()
{
	return window_.get();
}

}
