/**
 * This is a simple example showing how to:
 * 
 *   - Create the basic GLR program
 *   - Create a camera
 * 
 * It is the minimum required code for a working scene.
 */

#include "Main.hpp"

Main::Main()
{
	// Create the GLR program
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram() );

	// Create our window
	window_ = glrProgram_->createWindow("Test Name", "Simple Glr Project", 800, 600, 32, false, false);
	
	// Get the scene manager
	auto smgr = glrProgram_->getSceneManager();
	
	// Create camera
	auto camera = smgr->createCamera();
	camera->setPosition(0.0f, 0.0f, 1.0f);
	camera->lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
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
		window_->handleEvents();

		glrProgram_->render();

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			done = true;
		}
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
