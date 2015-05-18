#include "Main.hpp"

Main::Main()
{
	glr::ProgramSettings settings = glr::ProgramSettings();
	settings.defaultTextureDir = std::string("../../resources/models/");
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram(settings) );

	window_ = glrProgram_->createWindow("Test Name", "Simple Glr Project", 800, 600, 32, false, false);
	smgr_ = glrProgram_->getSceneManager();
	igui_ = glrProgram_->getHtmlGui();
	
	// Create FPS camera
	auto camera = smgr_->createCamera();
	camera->setPosition(0.0f, 0.0f, 1.0f);
	//camera->lookAt( glm::vec3(0.0f, 0.0f, 0.0f) );
	camera_ = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(camera, 0.020f) );

	sfmlWindow_ = static_cast<sf::Window*>( window_->getInternalWindowPointer() );
	sfmlWindow_->setKeyRepeatEnabled(false);
	
	auto shader = smgr_->getShaderProgramManager()->getShaderProgram( std::string("mesh_only") );
	assert( shader != nullptr );
	
	// Create model
	glrProgram_->getModelManager()->loadModel(std::string("stick_man"), std::string("../../resources/models/StickMan_0.x"));
	auto node = smgr_->createSceneNode( std::string("stick_man_node") );
	
	node->setScale(0.1f, 0.1f, 0.1f);
	node->setPosition( 0.0f, 0.0f, 0.0f );
	model_ = glrProgram_->getModelManager()->createInstance( std::string("stick_man") );
	assert( model_ != nullptr );
	
	node->attach( model_ );
	node->attach( shader );
	
	auto animations = model_->getAnimations();
	
	for (auto a : animations)
	{
		std::cout << "HERE: " << a->getName() << std::endl;
	}
	
	auto a = animations[1];
	assert(a != nullptr);
	//model_->playAnimation(a);
	
	// Create a light
	auto light = smgr_->createLight( std::string("light1") );
	
	auto ld = glr::LightData();
	ld.ambient = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.specular = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
	ld.position = glm::vec4(2.5f, 2.5f, 2.5f, 0.f);
	ld.direction = glm::vec4(0.0f, 0.0f, 0.0f, 0.f);
	light->setLightData(ld);
}

Main::~Main()
{
	destroy();
}

void Main::run()
{
	bool done = false;
	float animationTime = 0.0;
	
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

		// Handle animations
		animationTime += 0.01;
		if ( model_->getPlayingAnimation() != nullptr)
		{
			model_->setAnimationTime( animationTime );
		}
		
		igui_->processMessages();
		
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
