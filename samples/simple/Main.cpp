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
	camera_ = smgr_->createCamera("camera");
	camera_->setPosition(0, 0, 0);
	camera_->setLookAt(0, 0, 1);

	igui_ = glrProgram_->getHtmlGui();

	sfmlWindow_ = (sf::Window*)window_->getInternalWindowPointer();

	sfmlWindow_->setKeyRepeatEnabled(false);



	// Add model
	glr::models::IModel* creature = smgr_->getModelManager()->loadModel("/home/jarrett/projects/chisholmsoft/glr/samples/data/PlainsWolf.x");
	glr::ISceneNode* node = smgr_->createSceneNode("test_node");

	node->setScale(0.1f, 0.1f, 0.1f);

	node->attach(creature);
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
			camera_->move(glr::ICamera::MOVE_DIR_FORWARD, true);
		else
			camera_->move(glr::ICamera::MOVE_DIR_FORWARD, false);

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			camera_->move(glr::ICamera::MOVE_DIR_BACKWARD, true);
		else
			camera_->move(glr::ICamera::MOVE_DIR_BACKWARD, false);

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			camera_->move(glr::ICamera::MOVE_DIR_LEFT, true);
		else
			camera_->move(glr::ICamera::MOVE_DIR_LEFT, false);

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			camera_->move(glr::ICamera::MOVE_DIR_RIGHT, true);
		else
			camera_->move(glr::ICamera::MOVE_DIR_RIGHT, false);

		if ( rotationX_ != 0.0f )
		{
			camera_->rotateY(rotationX_);
			rotationX_ = 0.0f;
		}

		if ( rotationY_ != 0.0f )
		{
			camera_->rotateX(rotationY_);
			rotationY_ = 0.0f;
		}

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
	//igui_->release(htmlGuiComponent);

	//htmlGuiComponent = nullptr;
	igui_ = nullptr;

	window_->destroy();
}

int main()
{
	Main main;

	main.run();

	return 0;
}

