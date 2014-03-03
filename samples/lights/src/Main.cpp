#include "Main.h"

Main::Main()
{
	// Setup our model directory
	std::string modelsDir = "../../data/";
	glr::ProgramSettings settings = glr::ProgramSettings();
	settings.defaultTextureDir = std::string( modelsDir );
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram(settings) );
	
	window_ = glrProgram_->createWindow("Test Name", "Simple Lights Test", 800, 600, 32, false, false);
	smgr_ = glrProgram_->getSceneManager();
	
	// Create FPS camera
	camera_ = std::shared_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(glrProgram_->getOpenGlDevice(), 0.020f) );
	camera_->setPosition(0, 0, 0.5);
	smgr_->addCamera(camera_);

	sfmlWindow_ = (sf::Window*)window_->getInternalWindowPointer();
	sfmlWindow_->setKeyRepeatEnabled(false);

	// Add model
	smgr_->getModelManager()->loadModel( std::string("LuckyBunny"), std::string(modelsDir + "LuckyBunny.3ds") );
	cubeModel_ = smgr_->getModelManager()->createInstance( std::string("LuckyBunny") );
	glr::ISceneNode* node = smgr_->createSceneNode("test_node");
	node->setPosition( 0.0f, 0.0f, 0.0f );
	node->setScale(0.1f, 0.1f, 0.1f);
	node->attach( cubeModel_.get() );
   
   
	// Add light
	glr::ILight* light = smgr_->createLight("my_light1");
	
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

