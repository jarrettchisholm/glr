/*
 * GLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#include <iostream>
 
#include <GL/glew.h>

#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>

#include <boost/log/trivial.hpp>

#include "GLWindow.h"

#include "shaders/ShaderProgramManager.h"
#include "shaders/IShader.h"
#include "exceptions/GlException.h"

namespace glr {

GLWindow::GLWindow(int width, int height, std::string title) {
	sf::ContextSettings settings;
	settings.depthBits = 32;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 2;
	
	window_ = std::unique_ptr<sf::Window>(new sf::Window(
		sf::VideoMode(width, height, 32), 
		title,
		sf::Style::Default, 
		settings
	));
	
	// Initialize GLEW
	glewExperimental=true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		std::string msg("Failed to initialize GLEW.");
		BOOST_LOG_TRIVIAL(warning) << msg;
		throw exception::GlException(msg);
	}
	
	initialize();
	//window_->setVerticalSyncEnabled(true);
	//window_->setActive();
	
	//window_->setFramerateLimit(120);
}

GLWindow::~GLWindow() {
	destroy();
}

IWindow::WindowHandle GLWindow::getWindowHandle() {
	sf::WindowHandle handle = window_->getSystemHandle();
	
	return (IWindow::WindowHandle) handle;
}

IWindow::InternalWindow GLWindow::getInternalWindowPointer() {
	return window_.get();
}

void GLWindow::resize(glm::detail::uint32 width, glm::detail::uint32 height) {
	/* prevent divide-by-zero */
	if (height == 0)
		height = 1;
	
	glViewport(0, 0, width, height);

	width_ = width;
	height_ = height;
	
	projectionMatrix_ = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 100.f);
}

void GLWindow::initialize() {
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// we use resize once to set up our initial perspective
	resize(width_, height_);
	
	glFlush();
	
	shaderProgramManager_ = std::unique_ptr< shaders::ShaderProgramManager >(new shaders::ShaderProgramManager());
	
	sMgr_ = std::unique_ptr<DefaultSceneManager>(new DefaultSceneManager(shaderProgramManager_.get()));
	
	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );
	
	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);
	
	//shaders::ShaderProgramManager::getInstance();
}

void GLWindow::destroy() {
}

glm::detail::int32 GLWindow::handleEvents() {
	sf::Event event;
	
	while (window_->pollEvent(event)) {		
	    // check the type of the event...
	    switch (event.type) {
	        // window closed
	        case sf::Event::Closed:
	            //window.close();
	            break;
	
	        // key pressed
	        case sf::Event::KeyPressed:
	            break;
	
	        // we don't process other types of events
	        default:
	            break;
	    }
	}
	
	return 0;
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

glm::detail::uint32 GLWindow::getWidth() {
	return width_;
}

glm::detail::uint32 GLWindow::getHeight() {
	return height_;
}

glm::vec2 GLWindow::getPosition() {	
	const sf::Vector2i windowPosition = window_->getPosition();
	
	return glm::vec2( windowPosition.x, windowPosition.y );
}

glm::detail::uint32 GLWindow::getDepth() {
	return depth_;
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
