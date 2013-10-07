/*
 * GlrProgram.cpp
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#include <algorithm>

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "glw/shaders/ShaderProgramManager.h"
#include "glw/shaders/IShader.h"
#include "exceptions/GlException.h"

#include "GlrProgram.h"

#include "Window.h"

namespace glr {
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
		BOOST_LOG_TRIVIAL(error) << msg.str();
		throw exception::Exception(msg.str());
	}
	BOOST_LOG_TRIVIAL(debug) << "Creating window.";
	window_ = std::unique_ptr<IWindow>(new Window(width, height, title));
	
	BOOST_LOG_TRIVIAL(debug) << "Initializing GLEW.";
	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if ( glewInit() != GLEW_OK )
	{
		std::string msg("Failed to initialize GLEW.");
		BOOST_LOG_TRIVIAL(warning) << msg;
		throw exception::GlException(msg);
	}
	
	BOOST_LOG_TRIVIAL(debug) << "OpenGL setup.";
	// Setup settings for open gl device
	glr::glw::OpenGlDeviceSettings settings = glr::glw::OpenGlDeviceSettings();
	if ( !settings_.defaultTextureDir.empty() )
		settings.defaultTextureDir = settings_.defaultTextureDir;
	openGlDevice_ = std::unique_ptr< glw::OpenGlDevice >( new glw::OpenGlDevice(settings) );

	BOOST_LOG_TRIVIAL(debug) << "Initialize shader programs.";
	// initialize shader program manager and scene manager AFTER we create the window
	shaderProgramManager_ = openGlDevice_->getShaderProgramManager();
	shaderProgramManager_->addDefaultBindListener( this );
	
	BOOST_LOG_TRIVIAL(debug) << "Create scene manager.";
	sMgr_ = std::unique_ptr<BasicSceneManager>(new BasicSceneManager(shaderProgramManager_, openGlDevice_.get()));

	// Set the default shader for the scene manager
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_basic");
	assert( shader != nullptr );
	
	sMgr_->setDefaultShaderProgram(shader);
	
	BOOST_LOG_TRIVIAL(debug) << "Set OpenGL matrices.";
	// Set our opengl matrices
	openGlDevice_->setModelMatrix( sMgr_->getModelMatrix() );
	ICamera* camera = sMgr_->getActiveCamera();
	if (camera != nullptr)
	{
		openGlDevice_->setViewMatrix( camera->getViewMatrix() );
	}
	openGlDevice_->setProjectionMatrix( window_->getProjectionMatrix() );
	
	return window_.get();
}

void GlrProgram::beginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GlrProgram::endRender()
{
	// Display any changes we've made
	window_->render();
}

void GlrProgram::render()
{
	beginRender();

	// Set our opengl matrices
	openGlDevice_->setModelMatrix( sMgr_->getModelMatrix() );
	ICamera* camera = sMgr_->getActiveCamera();
	if (camera != nullptr)
	{
		openGlDevice_->setViewMatrix( camera->getViewMatrix() );
	}
	openGlDevice_->setProjectionMatrix( window_->getProjectionMatrix() );

	//setupUniformBufferObjectBindings(shader);
	//bindUniformBufferObjects(shader);
	sMgr_->drawAll();

	shaders::GlslShaderProgram::unbindAll();

	if ( gui_ != nullptr )
		gui_->render();

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
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, lightData.size() * sizeof(LightData), &lightData[0]);
				
				GLuint bindPoint = openGlDevice_->bindBuffer( ubo );
				
				shader->bindVariableByBindingName(shaders::IShader::BIND_TYPE_LIGHT, bindPoint);
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

	ICamera* camera = sMgr_->getActiveCamera();
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
	
	for ( auto it = bindings.begin(); it != bindings.end(); ++it )
	{
		//std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";

		switch ( it->first )
		{
		case shaders::IShader::BIND_TYPE_LIGHT:
			if ( lightUbos_.find(it->second) == lightUbos_.end())
				setupLightUbo(it->second, shader);
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

gui::IGUI* GlrProgram::getHtmlGui()
{
	if ( gui_.get() != nullptr )
	{
		return gui_.get();
	}

	gui_ = std::unique_ptr<gui::GUI>(new gui::GUI(openGlDevice_.get(), shaderProgramManager_, window_->getWidth(), window_->getHeight()));

	return gui_.get();
}

glw::IOpenGlDevice* GlrProgram::getOpenGlDevice()
{
	return openGlDevice_.get();
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
