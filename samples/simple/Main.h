/*
 * Main.h
 *
 *  Created on: 2013-04-04
 *      Author: jarrett
 */


#include "glm/glm.hpp"
#include <SFML/Window.hpp>

#include "GLRInclude.h"

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
	glr::gui::IGUI* igui_;
	glr::ISceneManager* smgr_;
	glr::ICamera* camera_;

	glm::detail::float32 rotationX_;
	glm::detail::float32 rotationY_;
	glm::detail::int32 mousePosX_;
	glm::detail::int32 mousePosY_;

	void tick();
	void destroy();
};
#endif /* MAIN_H */
