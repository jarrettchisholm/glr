/*
 * GlrProgram.cpp
 *
 *  Created on: 2013-04-01
 *      Author: jarrett
 */

#include <GL/glew.h>

#include <boost/log/trivial.hpp>

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShader.h"
#include "exceptions/GlException.h"

#include "GlrProgram.h"

#include "Window.h"

namespace glr {

GlrProgram::GlrProgram() {
	initialize();
}

GlrProgram::~GlrProgram() {
}

void GlrProgram::initialize() {		
	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );
	
	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);
	
	//shaders::ShaderProgramManager::getInstance();
}

/**
 *
 */
IWindow* GlrProgram::createWindow(std::string name, std::string title, 
			glm::detail::uint32 width, glm::detail::uint32 height, glm::detail::uint32 depth,
			bool fullscreen, bool vsync) {

	if (window_.get() != nullptr) {
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
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::string msg("Failed to initialize GLEW.");
		BOOST_LOG_TRIVIAL(warning) << msg;
		throw exception::GlException(msg);
	}
	
	// initialize shader program manager and scene manager AFTER we create the window
	shaderProgramManager_ = std::unique_ptr< shaders::ShaderProgramManager >(new shaders::ShaderProgramManager());
	
	sMgr_ = std::unique_ptr<DefaultSceneManager>(new DefaultSceneManager(shaderProgramManager_.get()));
	
	return window_.get();
}

void GlrProgram::beginRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
}

void GlrProgram::endRender() {
	// display any changes we've made
	window_->render();
}

void GlrProgram::render() {
	beginRender();
	//BOOST_LOG_TRIVIAL(debug) << "Begin render.";
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_basic");

	shader->bind();

	setupUniformBufferObjectBindings(shader);
	bindUniformBufferObjects(shader);

	sMgr_->drawAll();
	
	shaders::GlslShaderProgram::unbindAll();
	
	if (gui_ != nullptr)
		gui_->render();
	
	endRender();
}

void GlrProgram::bindUniformBufferObjects(shaders::IShaderProgram* shader) {
	// Bind lights
	const std::vector<LightData> lightData = sMgr_->getLightData();
	
	if (lightData.size() > 0) {
		for (auto it = lightUbos_.begin(); it != lightUbos_.end(); ++it) {
			std::vector<GLuint> ubos = it->second;
			for( auto ubo : ubos ) {
				glBindBuffer(GL_UNIFORM_BUFFER, ubo);
				glBufferSubData(GL_UNIFORM_BUFFER, 0, lightData.size() * sizeof(LightData), &lightData[0]);
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
	
	ICamera* camera = sMgr_->getActiveCameraSceneNode();
	if (camera != nullptr) {
		const glm::mat4 viewMatrix = camera->getViewMatrix();
		// Send uniform variable values to the shader
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		
		glm::mat4 pvmMatrix(projectionMatrix * viewMatrix * modelMatrix);
		glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
		
		glm::mat3 normalMatrix = glm::inverse(glm::transpose( glm::mat3(viewMatrix * modelMatrix) ));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	}
	
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	
}

void GlrProgram::setupUniformBufferObjectBindings(shaders::IShaderProgram* shader) {
	shaders::IShader::BindingsMap bindings = shader->getBindings();
	
	for (auto it = bindings.begin(); it != bindings.end(); ++it) {
		std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
		
		// TODO: Make keywords like 'Lights' into some sort of contstant, enum, whatever...
		switch (it->first) {
			case shaders::IShader::BIND_TYPE_LIGHT:
				if (lightUbos_.find(it->second) == lightUbos_.end())
					setupLightUbo(it->second);
				break;
			
			// TODO: implement other bindings (materials, etc)
			
			default:
				break;
		}
	}
}

void GlrProgram::setupLightUbo(std::string name) {
	// TODO: implement
	
	//std::map<std::string, std::shared_ptr<ILight>> lights = sMgr_->getLights();
	//for (auto it = lights.begin(); it != lights.end(); ++it) {
	//	std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
	//}
	
	//lightsUbos_[it->second] = 0;
	
	/*
	if (!bound_light_ubo) {
		// the binding point must be smaller than GL_MAX_UNIFORM_BUFFER_BINDINGS
		GLuint bindingPoint = 1;
		
		GLuint uniformBlockIndex = glGetUniformBlockIndex(shader->getGLShaderProgramId(), "LightSources");
		glUniformBlockBinding(shader->getGLShaderProgramId(), uniformBlockIndex, bindingPoint);
		 
		glGenBuffers(1, &light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
		 
		glBufferData(GL_UNIFORM_BUFFER, sizeof(ls), &ls, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		bound_light_ubo = true;
	}
	*/
}

ISceneManager* GlrProgram::getSceneManager() {
	return sMgr_.get();
}

gui::IGUI* GlrProgram::getHtmlGui() {
	if (gui_.get() != nullptr) {
		return gui_.get();
	}
	
	gui_ = std::unique_ptr<gui::GUI>(new gui::GUI());
	
	return gui_.get();
}

IWindow* GlrProgram::getWindow() {
	return window_.get();
}

}
