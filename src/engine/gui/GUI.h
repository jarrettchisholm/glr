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

#include "IGUI.h"

namespace icee {

namespace engine {

class GUI : public IGUI,  public Berkelium::WindowDelegate {
public:
	GUI();
	virtual ~GUI();
	
	virtual int initialize();
	virtual void destroy();
	
	virtual void render();
	virtual IGUIComponent* load(std::string filename);
	
private:
	// Width and height of our window.
    unsigned int width, height;
	// Storage for a texture
    unsigned int web_texture;
    // Buffer used to store data for scrolling
    char* scroll_buffer;

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
};

}

}

#endif /* GUI_H_ */
