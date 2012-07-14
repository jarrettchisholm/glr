/*
 * Test.cpp
 *
 *  Created on: 2011-05-05
 *      Author: jarrett
 */

#include "Test.h"

#include "engine/IceEngineCommon.h"
#include <iostream>

#include <boost/log/trivial.hpp>

Test::Test() {
	Test::test = false;
}

Test::~Test() {
}

float32 Test::rotationX = 0.0f;
float32 Test::rotationY = 0.0f;
sint32 Test::mousePosX = 0;
sint32 Test::mousePosY = 0;
bool Test::test = false;

int main() {
	BOOST_LOG_TRIVIAL(debug) << "Initializing engine test...";
	icee::engine::IOS* os = icee::engine::IceGraphicsEngine::createIOSObject();
	os->initialize();
	BOOST_LOG_TRIVIAL(debug) << "create window.";
	icee::engine::IIceWindow* window = os->createWindow(3840, 1080, 24, true, false);

	BOOST_LOG_TRIVIAL(debug) << "create scene manager.";
	icee::engine::ISceneManager* smgr = window->getSceneManager();
	BOOST_LOG_TRIVIAL(debug) << "create camera.";
	icee::engine::ICameraSceneNode* camera = smgr->addCamera(vmath::Vector3f(0, 0, 0),
			vmath::Vector3f(0, 0, 1));
	smgr->addDefaultSceneNode("test");
	
	BOOST_LOG_TRIVIAL(debug) << "initialize keyboard and mouse.";
	void* winPtr = window->getWindowPointer();
	lwis::engine::IInputManager* inputMgr = lwis::engine::InputFactory::getInputMgr(winPtr);
	lwis::engine::IKeyboardDevice* keyboard = inputMgr->createKeyboard();
	lwis::engine::IMouseDevice* mouse = inputMgr->createMouse();
	mouse->setCursorVisible(false);

	// add keyboard and mouse listeners
	BOOST_LOG_TRIVIAL(debug) << "add keyboard and mouse listeners...";
	Test* test = new Test();
	keyboard->addKeyboardListener((lwis::engine::IKeyboardListener*) test);
	mouse->addMouseListener((lwis::engine::IMouseListener*) test);

	bool done = false;
	int retVal = 0;
	
	BOOST_LOG_TRIVIAL(debug) << "Running engine...";

	while (!done) {
		keyboard->capture();
		if (keyboard->isKeyDown(lwis::engine::KEY_w))
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_FORWARD, true);
		else
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_FORWARD, false);

		if (keyboard->isKeyDown(lwis::engine::KEY_s))
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_BACKWARD, true);
		else
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_BACKWARD, false);

		if (keyboard->isKeyDown(lwis::engine::KEY_a))
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_LEFT, true);
		else
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_LEFT, false);

		if (keyboard->isKeyDown(lwis::engine::KEY_d))
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_RIGHT, true);
		else
			camera->move(icee::engine::ICameraSceneNode::MOVE_DIR_RIGHT, false);

		if (Test::rotationX != 0.0f) {
			camera->rotateY(Test::rotationX);
			Test::rotationX = 0.0f;
		}

		if (Test::rotationY != 0.0f) {
			camera->rotateX(Test::rotationY);
			Test::rotationY = 0.0f;
		}

		mouse->capture();

		camera->tick(0.33f);

		retVal = window->handleEvents();

		// check if the user alt-tabs
		if (window->blah() && !Test::test) {
			// destroy old mouse/keyboard
			inputMgr->destroyKeyboard();
			inputMgr->destroyMouse();

			// re-create them
			keyboard = inputMgr->createKeyboard();
			mouse = inputMgr->createMouse();

			// add 'Test' object as a listener to each
			keyboard->addKeyboardListener((lwis::engine::IKeyboardListener*) test);
			mouse->addMouseListener((lwis::engine::IMouseListener*) test);
			std::cout << "TEST 123 WORK!" << std::endl;
			//BOOST_LOG_TRIVIAL(debug) << "TEST 123 WORK!";

			smgr = window->getSceneManager();
			camera = smgr->addCamera(vmath::Vector3f(0, 0, 0),
					vmath::Vector3f(1, 1, 1));
			smgr->addDefaultSceneNode("test");

			Test::test = true;
		}

		if (retVal == -1) {
			done = true;
		} else
			window->render();

		if (keyboard->isKeyDown(lwis::engine::KEY_ESCAPE))
			done = true;
	}

	inputMgr->destroyKeyboard();
	inputMgr->destroyMouse();
	mouse = 0;
	keyboard = 0;

	window->destroy();

	delete inputMgr;

	delete os;

	return 0;
}

void Test::receiveKeyboardEvent(lwis::engine::KEYBOARD_EVENT evt, lwis::engine::KEY key,
		uint32 mod, char8 character) {
	//std::cout << "KEYBOARD EVENT: " << evt << " with character: " << character << " and Modifier: "
	//		<< mod << std::endl;

	//if (evt == lwis::engine::KEY_PRESSED && key == lwis::engine::KEY_TAB && mod == lwis::engine::KEY_MOD_ALT)
	//Test::test = true;
}

void Test::receiveMouseEvent(lwis::engine::MouseEvent evt) {
	//std::cout << "MOUSE EVENT: " << evt.evt << " with button: " << evt.button << " and Modifier: "
	//		<< evt.mod << std::endl;

	if (evt.evt == lwis::engine::MOUSE_MOVED_EVENT) {
		Test::rotationX = (float32) (Test::mousePosX - evt.x);
		Test::rotationY = (float32) (Test::mousePosY - evt.y);
		Test::mousePosX = evt.x;
		Test::mousePosY = evt.y;
	} else if (evt.evt == lwis::engine::MOUSE_REL_MOVE_EVENT) {
		Test::mousePosX = Test::mousePosX + evt.x;
		Test::mousePosY = Test::mousePosY + evt.y;
		Test::rotationX = (float32) (-evt.x/3.0f);
		Test::rotationY = (float32) (-evt.y/3.0f);
	}
}

