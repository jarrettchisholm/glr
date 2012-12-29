/*
 * GUI.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef GUI_H_
#define GUI_H_

#include <string>
#include <vector>

#include "IGUI.h"

#include "IGUIComponent.h"
#include "HtmlGuiComponent.h"

namespace oglre {

namespace engine {

class GUI : public IGUI {
public:
	GUI();
	virtual ~GUI();
	
	virtual glm::detail::int32 initialize();
	virtual void destroy();
	
	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseButton(glm::detail::uint32 buttonID, bool down, glm::detail::int32 clickCount=1);
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll);
	
	virtual void textEvent(const wchar_t *evt, size_t evtLength);
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode);
	
	virtual void update();
	virtual void render();
	virtual IGUIComponent* loadFromFile(std::string filename);
	virtual IGUIComponent* loadFromData(std::string data);
	virtual glm::detail::int32 release(IGUIComponent*);
	
private:
    std::vector<HtmlGuiComponent*> views_;
    
};

}

}

#endif /* GUI_H_ */
