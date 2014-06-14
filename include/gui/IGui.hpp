#ifndef IGUI_H_
#define IGUI_H_

#include <string>

#include "IGuiComponent.hpp"

#include "IWindowResizeListener.hpp"

namespace glr
{
namespace gui
{

class IGui: public IWindowResizeListener
{
public:
	virtual ~IGui()
	{
	}
	;

	virtual void destroy() = 0;

	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos) = 0;
	virtual void mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount = 1) = 0;
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll) = 0;

	virtual void textEvent(const wchar_t* evt, size_t evtLength) = 0;
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 virtualKeyCode, glm::detail::int32 scanCode) = 0;

	virtual void update() = 0;
	virtual void render() = 0;
	virtual IGuiComponent* loadFromFile(std::string filename) = 0;
	virtual IGuiComponent* loadFromData(std::string data) = 0;
	virtual void release(IGuiComponent*) = 0;
	
	virtual void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height) = 0;
};

}
}

#endif /* IGUI_H_ */
