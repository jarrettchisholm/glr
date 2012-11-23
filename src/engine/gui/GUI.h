/*
 * GUI.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifndef GUI_H_
#define GUI_H_

#include <string>

/* gl.h we need OpenGL */
#include <GL/gl.h>

#include <berkelium/Window.hpp>
#include <berkelium/Rect.hpp>
#include <berkelium/WindowDelegate.hpp>
#include "berkelium/ScriptUtil.hpp"

#include "IGUI.h"

namespace icee {

namespace engine {

class GUI : public IGUI,  public Berkelium::WindowDelegate {
public:
	GUI();
	virtual ~GUI();
	
	virtual int initialize();
	virtual void destroy();
	
	virtual void update();
	virtual void render();
	virtual IGUIComponent* loadFromFile(std::string filename);
	virtual IGUIComponent* loadFromData(std::string data);
	virtual int release(IGUIComponent*);
	
private:
	// Width and height of our window.
    unsigned int width, height;
	// Storage for a texture
    unsigned int web_texture;
    // Buffer used to store data for scrolling
    char* scroll_buffer;
    
    GLuint textureid;
    
    Berkelium::Window* window_;

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
    int testint;
    bool needs_full_refresh;
    bool webTextureReady_;
};

}

}

#endif /* GUI_H_ */
