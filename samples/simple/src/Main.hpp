#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <SFML/Window.hpp>

#include "GlrInclude.hpp"
#include "extras/FpsCamera.hpp"

#ifndef MAIN_H
#define MAIN_H

class Main
{
public:
	Main();
	virtual ~Main();

	void run();

private:
	std::unique_ptr< glr::GlrProgram > glrProgram_;
	glr::IWindow* window_;

	void tick();
	void destroy();
};

#endif /* MAIN_H */
