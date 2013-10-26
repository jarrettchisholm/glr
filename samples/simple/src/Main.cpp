/*
 * Main.cpp
 *
 *  Created on: 2013-04-04
 *      Author: jarrett
 */

#include "Main.h"

Main::Main()
{
	glrProgram_ = glr::GraphicsEngine::createProgram();
	window_ = glrProgram_->createWindow("Test Name", "Simple Glr Project", 800, 600, 32, false, false);
	smgr_ = glrProgram_->getSceneManager();
	
	// Create FPS camera
	camera_ = std::shared_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera("camera", glrProgram_->getOpenGlDevice(), 0.020f) );
	camera_->setPosition(0, 0, 0.5);
	smgr_->addCamera(camera_);

	sfmlWindow_ = (sf::Window*)window_->getInternalWindowPointer();
	sfmlWindow_->setKeyRepeatEnabled(false);
}

Main::~Main()
{
	destroy();
}

void Main::run()
{
	bool done = false;

	while ( !done )
	{
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			camera_->moveForward();

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			camera_->moveBack();

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			camera_->moveLeft();
		
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			camera_->moveRight();

		camera_->tick(0.33f);

		window_->handleEvents();

		glrProgram_->render();

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			done = true;
	}
}

void Main::tick()
{
}

void Main::destroy()
{
	window_->destroy();
}

int main()
{
	Main main;

	main.run();

	return 0;
}

