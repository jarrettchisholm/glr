/*
 * Gui.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef USE_CEF

#ifndef GUI_H_
#define GUI_H_

#include <string>
#include <vector>

#include "../IGui.h"

#include "GuiComponent.h"

#include "../../glw/shaders/IShaderProgramManager.h"
#include "../../glw/IOpenGlDevice.h"

#include "../../IWindowResizeListener.h"

namespace glr {
namespace gui {
namespace cef {

class Gui : public IGui, public IWindowResizeListener {
public:
	Gui(glw::IOpenGlDevice* openGlDevice, shaders::IShaderProgramManager* shaderProgramManager, glmd::uint32 width, glmd::uint32 height);
	virtual ~Gui();

	virtual void destroy();

	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount = 1);
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll);

	virtual void textEvent(const wchar_t*evt, size_t evtLength);
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode);

	virtual void update();
	virtual void render();
	virtual IGuiComponent* loadFromFile(std::string filename);
	virtual IGuiComponent* loadFromData(std::string data);
	virtual void release(IGuiComponent*);
	
	virtual void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height);

private:
	std::vector< std::unique_ptr<GuiComponent> > views_;
	
	shaders::IShaderProgramManager* shaderProgramManager_;
	
	glw::IOpenGlDevice* openGlDevice_;
	
	// Width and height of our window.
	glmd::uint32 width_, height_;
};

}
}
}

#endif /* GUI_H_ */

#endif
