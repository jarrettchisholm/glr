/*
 * Gui.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>
#include <string.h>

#include <boost/log/trivial.hpp>

#include "Gui.h"

#include "../../glw/shaders/GlslShaderProgram.h"

namespace glr {
namespace gui {
namespace cef {

Gui::Gui(glw::IOpenGlDevice* openGlDevice, shaders::IShaderProgramManager* shaderProgramManager, glmd::uint32 width, glmd::uint32 height) : openGlDevice_(openGlDevice), shaderProgramManager_(shaderProgramManager), width_(width), height_(height)
{
}

Gui::~Gui()
{
}

void Gui::destroy()
{
	views_.clear();
	
	//Berkelium::destroy();
}



void Gui::update()
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->executeScript(L"update();");
	}
}

void Gui::render()
{
	shaders::IShaderProgram* shader = shaderProgramManager_->getShaderProgram("glr_gui");

	//shader->bind();
	
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
			views_.at(i).get()->render(nullptr);
	}
	
	shaders::GlslShaderProgram::unbindAll();
}

void Gui::mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseMoved(xPos, yPos);
		}
	}
}

void Gui::mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseButton(buttonId, xPos, yPos, down, clickCount);
		}
	}
}

void Gui::mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->mouseWheel(xScroll, yScroll);
		}
	}
}

void Gui::textEvent(const wchar_t* evt, size_t evtLength)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get()->isVisible())
		{
			views_.at(i).get()->textEvent(evt, evtLength);
		}
	}
}

void Gui::keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode)
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
 * TESTING
 */
IGuiComponent* Gui::loadFromFile(std::string filename)
{
	std::unique_ptr<GuiComponent> comp = std::unique_ptr<GuiComponent>( new GuiComponent(openGlDevice_, width_, height_) );

	comp->setContentsUrl(filename);

	/*
	if ( comp->load() < 0 )
	{
		// TODO: throw error instead of returning a 'return status' from load() method
		BOOST_LOG_TRIVIAL(debug) << "Unable to load gui component.";
		comp->unload();
		return nullptr;
	}
	*/
	views_.push_back( std::move(comp) );

	return views_.back().get();
}

IGuiComponent* Gui::loadFromData(std::string data)
{
	std::unique_ptr<GuiComponent> comp = std::unique_ptr<GuiComponent>( new GuiComponent(openGlDevice_, width_, height_) );

	comp->setContents(data);

	if ( comp->load() < 0 )
	{
		// TODO: throw error instead of returning a 'return status' from load() method
		BOOST_LOG_TRIVIAL(debug) << "Unable to load gui component.";
		comp->unload();
		return 0;
	}

	views_.push_back( std::move(comp) );

	return views_.back().get();
}

int Gui::release(IGuiComponent* comp)
{
	for ( int i = 0; i < views_.size(); i++ )
	{
		if ( views_.at(i).get() == comp )
		{
			views_.erase(views_.begin() + i);
		}
	}

	return 0;
}

}
}
}
