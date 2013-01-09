/*
 * GLWindow.cpp
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */
 
#include <GL/glew.h>

#include "GL/glu.h"

#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>  
#include <glm/gtc/matrix_transform.hpp>

#include <boost/log/trivial.hpp>

#include "GLWindow.h"

#include "shaders/ShaderProgramManager.h"

namespace oglre {

namespace engine {

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
		BOOST_LOG_TRIVIAL(warning) << "Failed to initialize GLEW.";
	}
	
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
	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	*/

	width_ = width;
	height_ = height;
	
	projectionMatrix_ = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 100.f);
	//viewMatrix_ = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.f));
	modelMatrix_ = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
}

glm::detail::int32 GLWindow::initialize() {	
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glEnable(GL_LIGHTING);
	
	// we use resize once to set up our initial perspective
	resize(width_, height_);
	
	glFlush();
	
	sMgr_ = std::unique_ptr<DefaultSceneManager>(new DefaultSceneManager());
	
	
	// load all of the shaders
	std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );
	
	shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);
	
	return 0;
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
	//glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
}

void GLWindow::endRender() {
	// display any changes we've made
	window_->display();
}

void GLWindow::render() {
	beginRender();
	
	shaders::IShaderProgram* shader = shaders::ShaderProgramManager::getInstance()->getShaderProgram("test");
	shader->bind();
	
	int projectionMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "projectionMatrix"); // Get the location of our projection matrix in the shader  
	int viewMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "viewMatrix"); // Get the location of our view matrix in the shader  
	int modelMatrixLocation = glGetUniformLocation(shader->getGLShaderProgramId(), "modelMatrix"); // Get the location of our model matrix in the shader
	
	glm::mat4 viewMatrix = sMgr_->getActiveCameraSceneNode()->getViewMatrix();
	
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix_[0][0]); // Send our projection matrix to the shader  
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader  
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix_[0][0]); // Send our model matrix to the shader
	DrawAQuad();
	sMgr_->drawAll();
	
	shaders::ShaderProgram::unbindAll();
	
	if (gui_)
		gui_->render();
	
	endRender();
}

void GLWindow::DrawAQuad() {
	glTranslatef(-0., -0., -30.);

	glBegin(GL_QUADS);
		glColor3f(1., 0., 0.);
		glVertex3f(-.75, -.75, 0.);
		glColor3f(0., 1., 0.);
		glVertex3f(.75, -.75, 0.);
		glColor3f(0., 0., 1.);
		glVertex3f(.75, .75, 0.);
		glColor3f(1., 1., 0.);
		glVertex3f(-.75, .75, 0.);
	glEnd();
}

glm::detail::uint32 GLWindow::getWidth() {
	return width_;
}

glm::detail::uint32 GLWindow::getHeight() {
	return height_;
}

glm::detail::uint32 GLWindow::getDepth() {
	return depth_;
}

ISceneManager* GLWindow::getSceneManager() {
	return sMgr_.get();
}

IGUI* GLWindow::getHtmlGui() {
	gui_ = std::unique_ptr<GUI>(new GUI());

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

}
