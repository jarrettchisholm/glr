/*
 * HtmlGuiComponent.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef HTMLGUICOMPONENT_H_
#define HTMLGUICOMPONENT_H_

#include <string>
#include <map>

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>
#include <berkelium/Rect.hpp>
#include <berkelium/ScriptUtil.hpp>
#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>

#include "IGUIComponent.h"

#include "GUIObject.h"

namespace oglre {

namespace engine {

class HtmlGuiComponent : public IGUIComponent,  public Berkelium::WindowDelegate {
public:
	HtmlGuiComponent();
	virtual ~HtmlGuiComponent();
	
	virtual int load();
	virtual void unload();
	
	virtual void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	virtual void mouseButton(glm::detail::uint32 buttonID, bool down, glm::detail::int32 clickCount=1);
	virtual void mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll);
	
	virtual void textEvent(const wchar_t *evt, size_t evtLength);
	virtual void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode);
	
	int setContents(std::string contents);
	int loadContentsFromFile(std::string filename);
	
	virtual void update();
	virtual void render();
	
	virtual void executeScript(std::wstring script);
	
	virtual bool isVisible();
	virtual void setVisible(bool isVisible);
	
	virtual IGUIObject* createGUIObject(std::wstring name);
	virtual IGUIObject* getGUIObject(std::wstring name);
	
private:
	bool isVisible_;

	std::string url_;

	// Width and height of our window.
    glm::detail::uint32 width, height;
	// Storage for a texture
    glm::detail::uint32 web_texture;
    // Buffer used to store data for scrolling
    char* scroll_buffer;
    
    Berkelium::Window* window_;
    
    std::map< std::wstring, std::unique_ptr<GUIObject> > guiObjects_;
    
    std::wstring getFunctionName(std::wstring name);
	std::wstring getObjectName(std::wstring name);

	bool mapOnPaintToTexture(
		Berkelium::Window *wini,
	    const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
	    size_t num_copy_rects, const Berkelium::Rect *copy_rects,
	    int dx, int dy,
	    const Berkelium::Rect& scroll_rect,
	    unsigned int dest_texture,
	    unsigned int dest_texture_width,
	    unsigned int dest_texture_height,
	    bool ignore_partial,
	    char* scroll_buffer);
	    
	virtual void onPaint(Berkelium::Window* wini,
        const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
        size_t num_copy_rects, const Berkelium::Rect* copy_rects,
        int dx, int dy, const Berkelium::Rect& scroll_rect);
	
	virtual void onCrashed(Berkelium::Window *win);
    virtual void onUnresponsive(Berkelium::Window *win);
	virtual void onScriptAlert(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool &success, Berkelium::WideString &value);
	virtual void onJavascriptCallback(Berkelium::Window *win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, Berkelium::Script::Variant *args, size_t numArgs);
        
    void testLoadTexture();
    void testDrawTest1();
    void testDrawTestBerkelium();
    glm::detail::int32 testint;
    bool needs_full_refresh;
    bool webTextureReady_;

};

}

}

#endif /* HTMLGUICOMPONENT_H_ */
