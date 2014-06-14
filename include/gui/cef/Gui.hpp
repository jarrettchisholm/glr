#ifdef USE_CEF

#ifndef GUI_H_
#define GUI_H_

#include <string>
#include <vector>

#include "../IGui.hpp"

#include "GuiComponent.hpp"

#include "glw/shaders/IShaderProgramManager.hpp"
#include "glw/IOpenGlDevice.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

class Gui : public IGui
{
public:
	Gui(glw::IOpenGlDevice* openGlDevice, shaders::IShaderProgramManager* shaderProgramManager, glmd::uint32 width, glmd::uint32 height);
	virtual ~Gui();

	virtual void destroy();

	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount = 1);
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll);

	virtual void textEvent(const wchar_t* evt, size_t evtLength);
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 virtualKeyCode, glm::detail::int32 scanCode);

	virtual void update();
	virtual void render();
	virtual IGuiComponent* loadFromFile(std::string filename);
	virtual IGuiComponent* loadFromData(std::string data);
	virtual void release(IGuiComponent*);
	
	virtual void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height);

private:
	std::vector< std::unique_ptr<GuiComponent> > views_;

	glw::IOpenGlDevice* openGlDevice_;
	shaders::IShaderProgramManager* shaderProgramManager_;

	glmd::uint32 width_;
	glmd::uint32 height_;
};

}
}
}

#endif /* GUI_H_ */

#endif /* USE_CEF */
