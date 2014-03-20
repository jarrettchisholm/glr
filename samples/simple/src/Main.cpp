#include "Main.hpp"

Main::Main()
{
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );
	window_ = glrProgram_->createWindow("Test Name", "Simple Glr Project", 800, 600, 32, false, false);
	smgr_ = glrProgram_->getSceneManager();
	
	// Create FPS camera
	glr::ICamera* camera = smgr_->createCamera();
	camera->setPosition(0, 0, 0.5);
	camera_ = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(camera, 0.020f) );

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
