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

namespace oglre {

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
	
	// Testing of lights
	light_ubo = 0;
	bound_light_ubo = false;
	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );
	
	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);
	
	//shaders::ShaderProgramManager::getInstance();
	
	
	LoadAQuad(); //TESTING
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
	//BOOST_LOG_TRIVIAL(debug) << "Begin render.";
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("oglre_basic");

	shader->bind();

	bindUniformBufferObjects(shader);
	
	//DrawAQuad();

	sMgr_->drawAll();
	
	shaders::GlslShaderProgram::unbindAll();
	
	if (gui_ != nullptr)
		gui_->render();
	
	endRender();
}

void GLWindow::bindUniformBufferObjects(shaders::IShaderProgram* shader) {
	shaders::IShader::BindingsMap bindings = shader->getBindings();
	
	for (auto it = bindings.begin(); it != bindings.end(); ++it) {
		std::cout << "'" << it->second << "' annotated with name '" << it->first << "'\n";
		
	}
	
	// Testing lights
	LightSource ls;
	
	ls.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ls.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ls.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ls.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ls.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	
	// char array of the variable names in my shader program
	//const GLchar* names[] = {"ambient", "diffuse", "specular", "position", "direction"};
	//GLint uniActive; // how many active variables are in my shader program
	//GLint uniBlockBind, uniBlockSize;
	
	//GLuint uniformBlockIndex = glGetUniformBlockIndex(shader->getGLShaderProgramId(), "LightSources");
	
	/*
	if (light_ubo < 0) {
		glGenBuffers(1, &light_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4) * 5, nullptr, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	
	glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
    glBufferData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &ls.ambient[0]);
    glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), &ls.diffuse[0]);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &ls.specular[0]);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &ls.position[0]);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), sizeof(glm::vec4), &ls.direction[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
	*/
	
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
	} else {
		glBindBuffer(GL_UNIFORM_BUFFER, light_ubo);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ls), &ls);
	}
	
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
	
	/*
	glGenBuffers (1, &light_ubo);
	glBindBuffer (GL_UNIFORM_BUFFER, light_ubo);
	
	const GLchar *uniformNames[1] =
	{
	    "LightSources.lightSources"
	};
	GLuint uniformIndices;
	
	glGetUniformIndices(shader->getGLShaderProgramId(), 1, uniformNames, &uniformIndices);
	
	GLint uniformOffsets[1];
	glGetActiveUniformsiv (shader->getGLShaderProgramId(), 1, &uniformIndices, GL_UNIFORM_OFFSET, uniformOffsets);
	
	GLuint uniformBlockIndex = glGetUniformBlockIndex (shader->getGLShaderProgramId(), "LightSources");
	
	GLsizei uniformBlockSize(0);
	glGetActiveUniformBlockiv (shader->getGLShaderProgramId(), uniformBlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &uniformBlockSize);
	
	const GLchar *names[] = {
	    "LightSources.lightSources[0].ambient",
	    "LightSources.lightSources[0].diffuse",
	    "LightSources.lightSources[0].specular",
	    "LightSources.lightSources[0].position",
	    "LightSources.lightSources[0].direction"
	};
	GLuint indices[5];
	
	glGetUniformIndices(shader->getGLShaderProgramId(), 5, names, indices);
	
	std::vector _lightUniformOffsets(5);
	glGetActiveUniformsiv (shader->getGLShaderProgramId(), _lightUniformOffsets.size(), indices, GL_UNIFORM_OFFSET, &_lightUniformOffsets[0]);
	GLint *offsets = &_lightUniformOffsets[0];
	
	const unsigned int uboSize (uniformBlockSize);
	std::vector buffer (uboSize);
	
	int offset;
	
	for (unsigned int n = 0; n < _lights.size (); ++n) {
	    // Light ambient color (vec4)
	    offset = offsets[0 + n * 5];
	    for (int i = 0; i < 4; ++i) {
	        *(reinterpret_cast<float*> (&buffer[0] + offset)) =
	            _lights[n].ambient[i];
	        offset += sizeof (GLfloat);
	    }
	    // Light diffuse color (vec4)
	    offset = offsets[1 + n * 5];
	    for (int i = 0; i < 4; ++i) {
	        *(reinterpret_cast<float*> (&buffer[0] + offset)) =
	            _lights[n].diffuse[i];
	        offset += sizeof (GLfloat);
	    }
	    // Light specular color (vec4)
	    offset = offsets[2 + n * 5];
	    for (int i = 0; i < 4; ++i) {
	        *(reinterpret_cast<float*> (&buffer[0] + offset)) =
	            _lights[n].specular[i];
	        offset += sizeof (GLfloat);
	    }	    
	    // Light position (vec4)
	    offset = offsets[3 + n * 5];
	    for (int i = 0; i < 4; ++i) {
	        *(reinterpret_cast<float*> (&buffer[0] + offset)) =
	            _lights[n].position[i];
	        offset += sizeof (GLfloat);
	    }
	    // Light spot direction (vec4)
	    offset = offsets[4 + n * 5];
	    for (int i = 0; i < 4; ++i) {
	        *(reinterpret_cast<float*> (&buffer[0] + offset)) =
	            _lights[n].spotDirection[i];
	        offset += sizeof (GLfloat);
	    }
	}
	glBufferData (GL_UNIFORM_BUFFER, uboSize, &buffer[0], GL_DYNAMIC_DRAW);
	glBindBufferBase (GL_UNIFORM_BUFFER, 0, light_ubo);
	glUniformBlockBinding (shader->getGLShaderProgramId(), uniformBlockIndex, 0)
	*/
}

void GLWindow::LoadAQuad() {
	float* vertices = new float[18];  // Vertices for our square  
  
	vertices[0] = -0.5; vertices[1] = -0.5; vertices[2] = 0.0; // Bottom left corner  
	vertices[3] = -0.5; vertices[4] = 0.5; vertices[5] = 0.0; // Top left corner  
	vertices[6] = 0.5; vertices[7] = 0.5; vertices[8] = 0.0; // Top Right corner  
	  
	vertices[9] = 0.5; vertices[10] = -0.5; vertices[11] = 0.0; // Bottom right corner  
	vertices[12] = -0.5; vertices[13] = -0.5; vertices[14] = 0.0; // Bottom left corner  
	vertices[15] = 0.5; vertices[16] = 0.5; vertices[17] = 0.0; // Top Right corner  
	
	glGenVertexArrays(1, &vaoID[0]); // Create our Vertex Array Object  
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it  
	  
	glGenBuffers(1, vboID); // Generate our Vertex Buffer Object  
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Bind our Vertex Buffer Object  
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Set the size and data of our VBO and set it to STATIC_DRAW  
	  
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer  
	  
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object  
	glBindVertexArray(0); // Disable our Vertex Buffer Object
	  
	delete [] vertices; // Delete our vertices from memory
}

void GLWindow::DrawAQuad() {
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object
	
	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square  
	
	glBindVertexArray(0); // Unbind our Vertex Array Object
	
	/*
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
