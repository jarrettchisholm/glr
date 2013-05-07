/*
 * GlrProgram.cpp
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#include <algorithm>

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShader.h"
#include "exceptions/GlException.h"

#include "GlrProgram.h"

#include "Window.h"

namespace glr {
GlrProgram::GlrProgram()
{
	initialize();
}

GlrProgram::~GlrProgram()
{
}

void GlrProgram::initialize()
{
	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );

	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);

	//shaders::ShaderProgramManager::getInstance();
	
	bufferIds_ = std::vector<GLuint>();
	bindPoints_ = std::vector<GLuint>();
	boundBuffers_ = std::unordered_map<GLuint, GLuint>();
	
	maxNumBindPoints_ = 0;
	currentBindPoint_ = 0;
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
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception(msg.str());
	}

	window_ = std::unique_ptr<IWindow>(new Window(width, height, title));

	// VERY weird bug - I can call 'resize' all I want in the Window class - the initial perspective
	// doesn't seem to get set unless I call it OUTSIDE of the Window class...wtf?
	window_->resize(width, height);

	// Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if ( glewInit() != GLEW_OK )
	{
		std::string msg("Failed to initialize GLEW.");
		BOOST_LOG_TRIVIAL(warning) << msg;
		throw exception::GlException(msg);
	}

	// initialize shader program manager and scene manager AFTER we create the window
	
	std::vector<IShaderProgramBindListener*> defaultBindListeners = std::vector<IShaderProgramBindListener*>();
	defaultBindListeners.push_back( this );
	shaderProgramManager_ = std::unique_ptr< shaders::ShaderProgramManager >(new shaders::ShaderProgramManager(true, defaultBindListeners));
	//shaderProgramManager_->addDefaultBindListener( this );
	
	sMgr_ = std::unique_ptr<BasicSceneManager>(new BasicSceneManager(shaderProgramManager_.get(), this));

	
	// find and set the number of bind points available
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxNumBindPoints_);
		
	for (GLint i=1; i < maxNumBindPoints_+1; i++)
	{
		bindPoints_.push_back(i);
	}

	return window_.get();
}

void GlrProgram::beginRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
}

void GlrProgram::endRender()
{
	// display any changes we've made
	window_->render();
}

void GlrProgram::render()
{
	beginRender();
	//BOOST_LOG_TRIVIAL(debug) << "Begin render.";
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_basic");

	shader->bind();

	//setupUniformBufferObjectBindings(shader);
	//bindUniformBufferObjects(shader);
	sMgr_->setDefaultShaderProgram(shader);
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
				
				GLuint bindPoint = this->bindBuffer( ubo );
				
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

	lightUbos_[name][0] = this->createBufferObject(GL_UNIFORM_BUFFER, lightData.size() * sizeof(LightData), &lightData[0]);
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

	gui_ = std::unique_ptr<gui::GUI>(new gui::GUI(window_->getWidth(), window_->getHeight()));

	return gui_.get();
}

const glm::mat4& GlrProgram::getViewMatrix()
{
	ICamera* camera = sMgr_->getActiveCamera();
	if (camera != nullptr)
		return camera->getViewMatrix();
	
	return glm::mat4();
}

const glm::mat4& GlrProgram::getProjectionMatrix()
{
	return window_->getProjectionMatrix();
}

const glm::mat4& GlrProgram::getModelMatrix()
{
	return sMgr_->getModelMatrix();
}

GLuint GlrProgram::createBufferObject(GLenum target, glmd::uint32 totalSize, const void* dataPointer)
{
	GLuint bufferId = 0;
	glGenBuffers(1, &bufferId);
	glBindBuffer(target, bufferId);

	glBufferData(target, totalSize, dataPointer, GL_DYNAMIC_DRAW);
	glBindBuffer(target, 0);
	
	bufferIds_.push_back(bufferId);
	
	return bufferId;
}

void GlrProgram::releaseBufferObject(GLuint bufferId)
{
	auto it = std::find(bufferIds_.begin(), bufferIds_.end(), bufferId);
	
	if (it == bufferIds_.end())
	{
		// warning - buffer object not present
		return;
	}	
	
	unbindBuffer( bufferId );
	
	bufferIds_.erase(it);
	glDeleteBuffers(1, &bufferId);
}

/**
 * 
 */
GLuint GlrProgram::bindBuffer(GLuint bufferId)
{
	
	// TODO: Do I need a better algorithm here?
	GLuint bindPoint = bindPoints_[currentBindPoint_];
	currentBindPoint_++;
	
	if ( currentBindPoint_ > bindPoints_.size() )
		currentBindPoint_ = 1;
		
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, bufferId);
		
	return bindPoint;
	

	// This algorithm was my first attempt at making it more 'efficient' by keeping a cache, and by moving
	// recently used bind points to the bottom of a 'queue' (so they wouldn't be chosen again for a longer
	// period of time).  However, it didn't work :S
	/*
	GLuint bindPoint = 0; 
	 
	// Check if we have bound this buffer already
	auto boundBufferIter = boundBuffers_.find( bufferId );

	if (boundBufferIter != boundBuffers_.end())
	{
		// Pull bind point out of the list and push it on the back
		auto it = std::find( bindPoints_.begin(), bindPoints_.end(), boundBufferIter->second );
		bindPoint = *it;
		bindPoints_.erase( it );
		bindPoints_.push_back( bindPoint );
	} else
	{
		// If we haven't bound it already, use the first available bind point
		bindPoint = bindPoints_[0];

		// Remove any buffers from the bound buffers list that were bound to bindPoint (as it is now used by a different buffer)
		auto it = std::find( bindPoints_.begin(), bindPoints_.end(), bindPoint);
		if ( it != bindPoints_.end() ) 
		{
		    bindPoints_.erase( it );
		}

		// Pop bind point off the top of the list and push it on the back
		bindPoints_.erase( bindPoints_.begin() );
		bindPoints_.push_back( bindPoint );

		// Bind the buffer
		boundBuffers_[bufferId] = bindPoint;
		
	}

	//glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, bufferId);
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	
	return bindPoint;
	*/
}

void GlrProgram::unbindBuffer(GLuint bufferId)
{
	auto it = boundBuffers_.find( bufferId );
	if (it != boundBuffers_.end())
	{
		boundBuffers_.erase( it );
	}
}

GlError GlrProgram::getGlError()
{
	GlError glErrorObj = GlError();
	
	GLenum glError = glGetError();
	if ( glError )
	{
		switch ( glError )
		{
		case GL_INVALID_ENUM:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_ENUM";
			break;

		case GL_INVALID_VALUE:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_VALUE";
			break;

		case GL_INVALID_OPERATION:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_OPERATION";
			break;

		case GL_STACK_OVERFLOW:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_STACK_OVERFLOW";
			break;

		case GL_STACK_UNDERFLOW:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_STACK_UNDERFLOW";
			break;

		case GL_OUT_OF_MEMORY:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_OUT_OF_MEMORY";
			break;

		case GL_INVALID_FRAMEBUFFER_OPERATION:
			glErrorObj.type = glError;
			glErrorObj.name = "GL_INVALID_FRAMEBUFFER_OPERATION​";
			break;
		}
	}
	
	return glErrorObj;
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
