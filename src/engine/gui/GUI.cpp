/*
 * GUI.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>
#include <string.h>

#include <boost/log/trivial.hpp>

#include "GUI.h"

namespace oglre {

namespace engine {

GUI::GUI() {
	
}

GUI::~GUI() {
	
}

int GUI::initialize() {
	return 0;
}

void GUI::destroy() {
	for (int i = 0; i < views_.size(); i++) {
		views_.at(i)->unload();
		delete views_.at(i);
	}
}



void GUI::update() {
	//window_->executeJavascript( Berkelium::WideString::point_to(L"update();") );
	for (int i = 0; i < views_.size(); i++) {
		if (views_.at(i)->isVisible())
			views_.at(i)->executeScript( L"update();" );
	}
}
	
void GUI::render() {
	for (int i = 0; i < views_.size(); i++) {
		if (views_.at(i)->isVisible())
			views_.at(i)->render();
	}
}

void GUI::mouseMoved(sint32 xPos, sint32 yPos) {
	
}

void GUI::mouseButton(uint32 buttonID, bool down, sint32 clickCount) {
	
}

void GUI::mouseWheel(sint32 xScroll, sint32 yScroll) {
	
}
	
void GUI::textEvent(const wchar_t *evt, size_t evtLength) {
	for (int i = 0; i < views_.size(); i++) {
		if (views_.at(i)->isVisible()) {
			views_.at(i)->textEvent(evt, evtLength);
		}
	}
}

void GUI::keyEvent(bool pressed, sint32 mods, sint32 vk_code, sint32 scancode) {
	for (int i = 0; i < views_.size(); i++) {
		if (views_.at(i)->isVisible()) {
			views_.at(i)->keyEvent(pressed, mods, vk_code, scancode);
		}
	}	
}


/**
 * TESTING - Need to actually implement this properly (i.e. own the pointer, etc)
 */ 
IGUIComponent* GUI::loadFromFile(std::string filename) {
	HtmlGuiComponent* comp = new HtmlGuiComponent();
	comp->loadContentsFromFile(filename);
	
	if (comp->load() < 0) {
		comp->unload();
		delete comp;
		return 0;
	}
	
	views_.push_back(comp);
	
	return comp;
}

IGUIComponent* GUI::loadFromData(std::string data) {
	HtmlGuiComponent* comp = new HtmlGuiComponent();
	comp->setContents( data );
	
	if (comp->load() < 0) {
		comp->unload();
		delete comp;
		return 0;
	}
	
	views_.push_back(comp);
	
	return comp;
}

int GUI::release(IGUIComponent* comp) {
	for (int i = 0; i < views_.size(); i++) {
		if (views_.at(i) == comp) {
			views_.erase( views_.begin() + i);
			comp->unload();
			delete comp;
		}
	}
	
	return 0;
}

}

}
