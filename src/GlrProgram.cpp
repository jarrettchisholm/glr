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

#include "GLWindow.h"

namespace glr {

GlrProgram::GlrProgram() {
}

GlrProgram::~GlrProgram() {
}

void GLWindow::initialize() {	
	shaderProgramManager_ = std::unique_ptr< shaders::ShaderProgramManager >(new shaders::ShaderProgramManager());
	
	sMgr_ = std::unique_ptr<DefaultSceneManager>(new DefaultSceneManager(shaderProgramManager_.get()));
	
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
IWindow* GraphicsEngine::createWindow(std::string name, std::string title, 
			glm::detail::uint32 width, glm::detail::uint32 height, glm::detail::uint32 depth,
			bool fullscreen, bool vsync) {

	if (window_.get() != nullptr) {
		// TODO: error
	}

	window_ = std::unique_ptr<IWindow>(new GLWindow(width, height, title));
	
	// VERY weird bug - I can call 'resize' all I want in the GLWindow class - the initial perspective
	// doesn't seem to get set unless I call it OUTSIDE of the GLWindow class...wtf?
	window_->resize(width, height);
	
	return window_.get();
}

void GLWindow::beginRender() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
}

void GLWindow::endRender() {
	// display any changes we've made
	window_->display();
}

void GLWindow::render() {
	beginRender();
	//BOOST_LOG_TRIVIAL(debug) << "Begin render.";
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_basic");

	shader->bind();

	setupUniformBufferObjects(shader);
	bindUniformBufferObjects();

	sMgr_->drawAll();
	
	shaders::GlslShaderProgram::unbindAll();
	
	if (gui_ != nullptr)
		gui_->render();
	
	endRender();
}

void GLWindow::bindUniformBufferObjects() {
	// Bind lights
	const std::vector<LightData> lightData = sMgr_->getLightData();
	
	if (lightData.size() > 0) {
		for (auto it = lightUbos_.begin(); it != lightUbos_.end(); ++it) {
			glBindBuffer(GL_UNIFORM_BUFFER, it->second);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, lightData.size() * sizeof(LightData), &lightData[0]);
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
	
	ICamera* camera = sMgr_->getActiveCameraSceneNode();
	if (camera != nullptr) {
		const glm::mat4 viewMatrix = camera->getViewMatrix();
		// Send uniform variable values to the shader
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
		
		glm::mat4 pvmMatrix(projectionMatrix_ * viewMatrix * modelMatrix);
		glUniformMatrix4fv(pvmMatrixLocation, 1, GL_FALSE, &pvmMatrix[0][0]);
		
		glm::mat3 normalMatrix = glm::inverse(glm::transpose( glm::mat3(viewMatrix * modelMatrix) ));
		glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, &normalMatrix[0][0]);
	}
	
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix_[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	
}

void GLWindow::setupUniformBufferObjectBindings(shaders::IShaderProgram* shader) {
	shaders::IShader::BindingsMap bindings = shader->getBindings();
	
	for (auto it = bindings.begin(); it != bindings.end(); ++it) {
		std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
		
		// TODO: Make keywords like 'Lights' into some sort of contstant, enum, whatever...
		switch (it->first) {
			case "Lights":
				if (lightUbos_.find(it->second) == lightUbos_.end())
					setupLightUbo(it->second);
				break;
			
			// TODO: implement other bindings (materials, etc)
			
			default:
				break;
		}
	}
}

void GLWindow::setupLightUbo(std::string name) {
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

ISceneManager* GLWindow::getSceneManager() {
	return sMgr_.get();
}

gui::IGUI* GLWindow::getHtmlGui() {
	gui_ = std::unique_ptr<gui::GUI>(new gui::GUI());

	int result = gui_->initialize();
	
	if (result < 0) {
		//delete gui_;
		// TODO: Should I delete the raw pointer?
		BOOST_LOG_TRIVIAL(warning) << "GUI did not initialize successfully.";
		return 0;		
	}
	
	return gui_.get();
}

}
