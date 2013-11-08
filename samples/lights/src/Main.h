/*
 * Main.h
 *
 *  Created on: 2013-04-04
 *      Author: jarrett
 */


#include "glm/glm.hpp"
#include <SFML/Window.hpp>

#include "GlrInclude.h"
#include "extras/FpsCamera.cpp"

#ifndef MAIN_H
#define MAIN_H

class Main {
public:
	Main();
	virtual ~Main();

	void run();

private:
	std::unique_ptr< glr::GlrProgram > glrProgram_;
	glr::IWindow* window_;
	sf::Window* sfmlWindow_;
	glr::ISceneManager* smgr_;

	std::shared_ptr< glr::extras::FpsCamera > camera_;
	std::unique_ptr<glr::models::IModel> cubeModel_;

	void tick();
	void destroy();
};
#endif /* MAIN_H */