// GUI code

oglre::IGUI* igui = window->getHtmlGui();
oglre::IGUIComponent* htmlGuiComponent = igui->load("../data/test.html");
htmlGuiComponent->setVisible(true);

oglre::IGUIComponent* htmlGuiComponent2 = igui->load("../data/test2.html");
//htmlGuiComponent->add( htmlGuiComponent2 );
htmlGuiComponent2->setVisible(true);

// do stuff

htmlGuiComponent2->setVisible(false); // do we need this?

// do stuff

igui->release( htmlGuiComponent2 );

// do more stuff

igui->release( htmlGuiComponent );
//window->release( igui );






// Interacting with Graphics Engine
std::unique_ptr<oglre::IWindow> window = oglre::GraphicsEngine::createWindow();

oglre::ISceneManager* scnMgr = window->getSceneManager();
oglre::ISceneNode* rootNode = scnMgr->getRootSceneNode();

oglre::IModelManager* modelMgr = scnMgr->getModelManager();
oglre::IShaderProgramManager* shaderProgramMgr = scnMgr->getShaderProgramManager();

shaderProgramMgr->loadShaderPrograms("shader_programs.glsl.json");


oglre::IModel* model1 = nullptr;
oglre::IModel* model2 = nullptr;
oglre::ILight* light1 = nullptr;
oglre::IShaderProgram* shaderProgram = nullptr;


shaderProgram = shaderProgramMgr->getShaderProgram("oglre_simple");


modelMgr->loadModels("my_models.list.json");
modelMgr->loadModel("model_blah", "model_blah.obj");
model1 = modelMgr->getModel("my_model1");
model2 = modelMgr->getModel("my_model2");

light1 = scnMgr->createLight("my_light_1", oglre::ILight::DIRECTIONAL_LIGHT, glm::vec3(5, 5, 5), glm::vec3(0, 0, 0));

model1->attach(shaderProgram);

oglre::ISceneNode* node = rootNode->createSceneNode("my_model", model1, glm::vec3(0, 0, 0));

oglre::ISceneNode* node2 = rootNode->createSceneNode("my_model2", model2, glm::vec3(5, 5, 0));
//node->attach(node2);

//rootNode->attach(node);









// Exceptions example
try {
	std::unique_ptr<oglre::IWindow> window = oglre::GraphicsEngine::createWindow();
}
catch (oglre::Exception& e) {
	BOOST_LOG_TRIVIAL(error) << "Error starting Oglre engine: " << e.what();
	exit(EXIT_FAILURE);
}

oglre::ISceneManager* scnMgr = window->getSceneManager();

oglre::IModelManager* modelMgr = scnMgr->getModelManager();
oglre::IShaderProgramManager* shaderProgramMgr = scnMgr->getShaderProgramManager();

try {
	shaderProgramMgr->loadShaderPrograms("shader_programs.glsl")
}
catch (oglre::IoException& e) {
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}
catch (oglre::GlException& e) {
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}
catch (oglre::FormatException& e) {
	BOOST_LOG_TRIVIAL(error) << "Error loading shader programs: " << e.what();
	exit(EXIT_FAILURE);
}

// etc
