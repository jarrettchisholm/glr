/*
 * Window.h
 *
 *  Created on: 2011-05-06
 *      Author: jarrett
 */

#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <memory>

#include <SFML/Window.hpp>

#include "IWindow.h"

namespace glr {

namespace glmd = glm::detail;

class Window : public IWindow {
public:
	Window(int width, int height, std::string title);
	virtual ~Window();

	virtual WindowHandle getWindowHandle();
	virtual IWindow::InternalWindow getInternalWindowPointer();

	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height);
	virtual void destroy();
	virtual void render();
	glm::detail::int32 handleEvents();

	virtual glm::detail::uint32 getWidth();
	virtual glm::detail::uint32 getHeight();
	virtual glm::vec2 getPosition();
	virtual glm::detail::uint32 getDepth();
	virtual const glm::mat4& getProjectionMatrix();

private:
	glmd::int32 x_, y_;
	glmd::uint32 width_, height_;
	glmd::uint32 depth_;

	std::unique_ptr<sf::Window> window_;

	glm::mat4 projectionMatrix_;

	void initialize();
};

}
#endif /* GLWINDOW_H_ */
