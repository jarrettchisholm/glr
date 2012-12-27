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
	
using namespace compatibility;

class GUI : public IGUI {
public:
	GUI();
	virtual ~GUI();
	
	virtual sint32 initialize();
	virtual void destroy();
	
	virtual void mouseMoved(sint32 xPos, sint32 yPos);
	virtual void mouseButton(uint32 buttonID, bool down, sint32 clickCount=1);
	virtual void mouseWheel(sint32 xScroll, sint32 yScroll);
	
	virtual void textEvent(const wchar_t *evt, size_t evtLength);
	virtual void keyEvent(bool pressed, sint32 mods, sint32 vk_code, sint32 scancode);
	
	virtual void update();
	virtual void render();
	virtual IGUIComponent* loadFromFile(std::string filename);
	virtual IGUIComponent* loadFromData(std::string data);
	virtual sint32 release(IGUIComponent*);
	
private:
    std::vector<HtmlGuiComponent*> views_;
    
};

}

}

#endif /* GUI_H_ */
