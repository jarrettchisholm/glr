// GUI code

glr::IGUI* igui = window->getHtmlGui();
glr::IGUIComponent* htmlGuiComponent = igui->load("../data/test.html");
htmlGuiComponent->setVisible(true);

glr::IGUIComponent* htmlGuiComponent2 = igui->load("../data/test2.html");
//htmlGuiComponent->add( htmlGuiComponent2 );
htmlGuiComponent2->setVisible(true);

// do stuff

htmlGuiComponent2->setVisible(false); // do we need this?

// do stuff

igui->release(htmlGuiComponent2);

// do more stuff

igui->release(htmlGuiComponent);
//window->release( igui );






// Interacting with Graphics Engine
std::unique_ptr<glr::IWindow> window = glr::GraphicsEngine::createWindow();

glr::ISceneManager* scnMgr = window->getSceneManager();
glr::ISceneNode* rootNode = scnMgr->getRootSceneNode();

glr::IModelManager* modelMgr = scnMgr->getModelManager();
glr::IShaderProgramManager* shaderProgramMgr = scnMgr->getShaderProgramManager();

shaderProgramMgr->loadShaderPrograms("my/shaders/directory");


glr::IModel* model1 = nullptr;
glr::IModel* model2 = nullptr;
glr::ILight* light1 = nullptr;
glr::IShaderProgram* shaderProgram = nullptr;


shaderProgram = shaderProgramMgr->getShaderProgram("glr_simple");


modelMgr->loadModels("my_models.list.json");
modelMgr->loadModel("model_blah", "model_blah.obj");
model1 = modelMgr->getModel("my_model1");
model2 = modelMgr->getModel("my_model2");

light1 = scnMgr->createLight("my_light_1", glr::ILight::DIRECTIONAL_LIGHT, glm::vec3(5, 5, 5), glm::vec3(0, 0, 0));

model1->attach(shaderProgram);

glr::ISceneNode* node = rootNode->createSceneNode("my_model", model1, glm::vec3(0, 0, 0));

glr::ISceneNode* node2 = rootNode->createSceneNode("my_model2", model2, glm::vec3(5, 5, 0));
//node->attach(node2);

//rootNode->attach(node);









// Exceptions example
try
{
	std::unique_ptr<glr::IWindow> window = glr::GraphicsEngine::createWindow();
}
catch ( glr::Exception& e )
{
	BOOST_LOG_TRIVIAL(error) << "Error starting Glr engine: " << e.what();
	exit(EXIT_FAILURE);
}

glr::ISceneManager* scnMgr = window->getSceneManager();

glr::IModelManager* modelMgr = scnMgr->getModelManager();
glr::IShaderProgramManager* shaderProgramMgr = scnMgr->getShaderProgramManager();

try
{
	shaderProgramMgr->loadShaderPrograms("shader_programs.glsl")
}
catch ( glr::IoException& e )
{
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}
catch ( glr::GlException& e )
{
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}
catch ( glr::FormatException& e )
{
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}

// etc
