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

namespace glr {
namespace gui {
GUI::GUI(glmd::uint32 width, glmd::uint32 height) : width_(width), height_(height)
{
	initialize();
}

GUI::~GUI()
{
}

int GUI::initialize()
{
	if ( !Berkelium::init(Berkelium::FileString::empty()) )
	{
		BOOST_LOG_TRIVIAL(debug) << "Failed to initialize berkelium!";
		return -1;
	}
	
	return 0;
}

void GUI::destroy()
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		views_.at(i).get()->unload();
	}
	
	views_.clear();
	
	Berkelium::destroy();
}



void GUI::update()
{
	//window_->executeJavascript( Berkelium::WideString::point_to(L"update();") );
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->executeScript(L"update();");
	}
}

void GUI::render()
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->render();
	}
}

void GUI::mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos)
{
}

void GUI::mouseButton(glm::detail::uint32 buttonID, bool down, glm::detail::int32 clickCount)
{
}

void GUI::mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll)
{
}

void GUI::textEvent(const wchar_t*evt, size_t evtLength)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->textEvent(evt, evtLength);
		}
	}
}

void GUI::keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->keyEvent(pressed, mods, vk_code, scancode);
		}
	}
}


/**
 * TESTING - Need to actually implement this properly (i.e. own the pointer, etc)
 */
IGUIComponent* GUI::loadFromFile(std::string filename)
{
	std::unique_ptr<HtmlGuiComponent> comp = std::unique_ptr<HtmlGuiComponent>( new HtmlGuiComponent(width_, height_) );

	comp->loadContentsFromFile(filename);

	if ( comp->load() < 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Unable to load gui component.";
		comp->unload();
		return nullptr;
	}

	views_.push_back( std::move(comp) );

	return views_.back().get();
}

IGUIComponent* GUI::loadFromData(std::string data)
{
	std::unique_ptr<HtmlGuiComponent> comp = std::unique_ptr<HtmlGuiComponent>( new HtmlGuiComponent(width_, height_) );

	comp->setContents(data);

	if ( comp->load() < 0 )
	{
		BOOST_LOG_TRIVIAL(debug) << "Unable to load gui component.";
		comp->unload();
		return 0;
	}

	views_.push_back( std::move(comp) );

	return views_.back().get();
}

int GUI::release(IGUIComponent* comp)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get() == comp )
		{
			views_.erase(views_.begin() + i);
			comp->unload();
			//delete comp;
		}
	}

	return 0;
}
}
}
