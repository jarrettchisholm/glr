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

#include "../common/compatibility/Types.h"

namespace oglre {

namespace engine {
	
using namespace compatibility;

class IGUI {
public:
	virtual ~IGUI() {
	}
	;
	
	virtual sint32 initialize() = 0;
	virtual void destroy() = 0;
	
	virtual void mouseMoved(sint32 xPos, sint32 yPos) = 0;
	virtual void mouseButton(uint32 buttonID, bool down, sint32 clickCount=1) = 0;
	virtual void mouseWheel(sint32 xScroll, sint32 yScroll) = 0;
	
	virtual void textEvent(const wchar_t *evt, size_t evtLength) = 0;
	virtual void keyEvent(bool pressed, sint32 mods, sint32 vk_code, sint32 scancode) = 0;
	
	virtual void update() = 0;
	virtual void render() = 0;
	virtual IGUIComponent* loadFromFile(std::string filename) = 0;
	virtual IGUIComponent* loadFromData(std::string data) = 0;
	virtual sint32 release(IGUIComponent*) = 0;
};

}

}

#endif /* IGUI_H_ */
