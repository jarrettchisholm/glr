#ifndef GLWINDOW_H_
#define GLWINDOW_H_

#include <memory>

#include <SFML/Window.hpp>

#include "IWindow.hpp"

namespace glr
{

namespace glmd = glm::detail;

class Window : public IWindow
{
public:
	Window(glm::detail::int32 width, glm::detail::int32 height, std::string title);
	virtual ~Window();

	virtual WindowHandle getWindowHandle() const;
	virtual IWindow::InternalWindow getInternalWindowPointer() const;

	virtual void resize(glm::detail::uint32 width, glm::detail::uint32 height);
	virtual void destroy();
	virtual void render();
	glm::detail::int32 handleEvents();

	virtual glm::detail::uint32 getWidth() const;
	virtual glm::detail::uint32 getHeight() const;
	virtual glm::vec2 getPosition() const;
	virtual glm::detail::uint32 getDepth() const;
	virtual const glm::mat4& getProjectionMatrix() const;
	
	virtual void addWindowResizeListener(IWindowResizeListener* listener);
	virtual void removeWindowResizeListener(IWindowResizeListener* listener);

private:
	glmd::int32 x_;
	glmd::int32 y_;
	glmd::uint32 width_;
	glmd::uint32 height_;
	glmd::uint32 depth_;

	std::unique_ptr<sf::Window> window_;

	glm::mat4 projectionMatrix_;
	
	std::vector< IWindowResizeListener* > windowResizeListeners_;

	void initialize();
};

}

#endif /* GLWINDOW_H_ */
