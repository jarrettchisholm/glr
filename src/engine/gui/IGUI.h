/*
 * IGUI.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef IGUI_H_
#define IGUI_H_

#include <string>

#include "IGUIComponent.h"

namespace oglre {

namespace engine {

class IGUI {
public:
	virtual ~IGUI() {
	}
	;
	
	virtual glm::detail::int32 initialize() = 0;
	virtual void destroy() = 0;
	
	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos) = 0;
	virtual void mouseButton(glm::detail::uint32 buttonID, bool down, glm::detail::int32 clickCount=1) = 0;
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll) = 0;
	
	virtual void textEvent(const wchar_t *evt, size_t evtLength) = 0;
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode) = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	virtual IGUIComponent* loadFromFile(std::string filename) = 0;
	virtual IGUIComponent* loadFromData(std::string data) = 0;
	virtual glm::detail::int32 release(IGUIComponent*) = 0;
};

}

}

#endif /* IGUI_H_ */
