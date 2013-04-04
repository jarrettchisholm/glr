/*
 * Window.cpp
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

#include "Window.h"

namespace glr {
Window::Window(int width, int height, std::string title)
{
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

	initialize();
	//window_->setVerticalSyncEnabled(true);
	//window_->setActive();

	//window_->setFramerateLimit(120);
}

Window::~Window()
{
	destroy();
}

IWindow::WindowHandle Window::getWindowHandle()
{
	sf::WindowHandle handle = window_->getSystemHandle();

	return (IWindow::WindowHandle)handle;
}

IWindow::InternalWindow Window::getInternalWindowPointer()
{
	return window_.get();
}

void Window::render()
{
	window_->display();
}

void Window::resize(glm::detail::uint32 width, glm::detail::uint32 height)
{
	/* prevent divide-by-zero */
	if ( height == 0 )
		height = 1;

	glViewport(0, 0, width, height);

	width_ = width;
	height_ = height;

	projectionMatrix_ = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 100.f);
}

void Window::initialize()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// we use resize once to set up our initial perspective
	resize(width_, height_);

	glFlush();

	// load all of the shaders
	//std::vector< std::pair <std::string, shaders::IShader::Type> > shaders;
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.vert", shaders::IShader::TYPE_VERTEX) );
	//shaders.push_back( std::pair <std::string, shaders::IShader::Type>("shader.frag", shaders::IShader::TYPE_FRAGMENT) );

	//shaders::ShaderProgramManager::getInstance()->getShaderProgram("test", shaders);

	//shaders::ShaderProgramManager::getInstance();
}

void Window::destroy()
{
}

glm::detail::int32 Window::handleEvents()
{
	sf::Event event;

	while ( window_->pollEvent(event))
	{
		// check the type of the event...
		switch ( event.type )
		{
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

glm::detail::uint32 Window::getWidth()
{
	return width_;
}

glm::detail::uint32 Window::getHeight()
{
	return height_;
}

glm::vec2 Window::getPosition()
{
	const sf::Vector2i windowPosition = window_->getPosition();

	return glm::vec2(windowPosition.x, windowPosition.y);
}

glm::detail::uint32 Window::getDepth()
{
	return depth_;
}

const glm::mat4& Window::getProjectionMatrix()
{
	return projectionMatrix_;
}
}
