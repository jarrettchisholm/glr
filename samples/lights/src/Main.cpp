/**
 * This is a simple example showing how to:
 * 
 *   - Create a light
 *   - Load an asset
 *   - Move the camera around the scene
 */

#include "Main.hpp"

Main::Main()
{
	// Setup our model directory
	std::string resourcesDir = std::string("../../resources/models/StickMan_0/");
	glr::ProgramSettings settings = glr::ProgramSettings();
	settings.defaultTextureDir = std::string( resourcesDir );
	
	// Create the GLR program
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram(settings) );
	
	// Create our window
	window_ = glrProgram_->createWindow("Test Name", "Simple Light and Model Test", 800, 600, 32, false, false);
	
	// Get the scene manager
	auto smgr = glrProgram_->getSceneManager();
	
	// Create FPS camera
	auto camera = smgr->createCamera();
	camera->setPosition(0, 0, 1);
	camera->lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
	
	// Wrap our camera in a 'Fps' camera
	camera_ = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(camera, 0.020f) );

	// Don't repeat keys if they are held down
	auto sfmlWindow = (sf::Window*)window_->getInternalWindowPointer();
	sfmlWindow->setKeyRepeatEnabled(false);

	// Load our model
	glrProgram_->getModelManager()->loadModel( std::string("stick_man"), resourcesDir + std::string("StickMan_0.x") );
	
	// Create an instance of our model
	auto model = glrProgram_->getModelManager()->createInstance( std::string("stick_man") );
	
	// Create the node to hold our model
	auto node = smgr->createSceneNode("stick_man_node");
	node->setPosition( 0.0f, 0.0f, 0.0f );
	node->setScale(0.1f, 0.1f, 0.1f);
	node->attach( model );
	
	// No texture for this model, so use the 'mesh_only' shader
	auto shader = smgr->getShaderProgramManager()->getShaderProgram( std::string("mesh_only") );

	// Attach the model and shader to the node
	node->attach( model );
	node->attach( shader );

	// Add light
	auto light = smgr->createLight("light_1");
	
	glr::LightData ld;
	ld.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.5f, 0.5f, 0.5f, 0.f);
	light->setLightData(ld);
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
		// Handle keyboard events (for movement of the camera)
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera_->moveForward();
		}

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			camera_->moveBack();
		}

		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			camera_->moveLeft();
		}
		
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			camera_->moveRight();
		}

		
		
		camera_->tick(0.33f);

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

