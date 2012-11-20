/*
 * GUI.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <iostream>
#include <string.h>

#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>
#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>

#include <boost/log/trivial.hpp>

#include "GUI.h"

#include "IGUIComponent.h"
#include "HtmlGuiComponent.h"

#include "../../common/utilities/ImageLoader.h"

#define DEBUG_PAINT true

namespace icee {

namespace engine {

GUI::GUI() {
	
}

GUI::~GUI() {
	
}

int GUI::initialize() {
	if (!Berkelium::init(Berkelium::FileString::empty())) {
		BOOST_LOG_TRIVIAL(debug) << "Failed to initialize berkelium!";
		return -1;
	}
	
	// Create texture to hold rendered view
	glGenTextures(1, &web_texture);
	glBindTexture(GL_TEXTURE_2D, web_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	Berkelium::Context* context = Berkelium::Context::create();
    Berkelium::Window* window = Berkelium::Window::create(context);
    delete context;
    
    width = 600;
    height = 600;
    
    scroll_buffer = new char[width*(height+1)*4];
    
    //MyDelegate* delegate = new MyDelegate();
    window->setDelegate(this);
    window->resize(width, height);
    window->setTransparent(true);
    std::string url = "http://yahoo.ca";
    window->navigateTo(Berkelium::URLString::point_to(url.data(), url.length()));
    
    
    // testing only!
    testLoadTexture();
    
    needs_full_refresh = true;
    
	return 0;
}

void GUI::destroy() {
	 Berkelium::destroy();
}
	
void GUI::render() {
	//glClear( GL_COLOR_BUFFER_BIT );

    //glColor3f(1.f, 1.f, 1.f);
	
	// Black out the page
	//unsigned char black = 0;
	//glBindTexture(GL_TEXTURE_2D, web_texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, &black);
	
	// bind
	
    
    // bind
	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBindTexture(GL_TEXTURE_2D, web_texture);
	
	
	
	// release
	//glBindTexture(GL_TEXTURE_2D, 0);
	
	//testDrawTest1();
	testDrawTestBerkelium();
}

void GUI::testLoadTexture() {
	glBindTexture(GL_TEXTURE_2D, textureid);
    glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    
    utilities::ImageLoader il;
    utilities::Image* image = il.loadImageData("/home/jarrett/projects/icebreak/dark_horizon/data/oblivion.jpg");
    //utilities::Image* image = il.loadImageData("/tmp/chromium_render_1353369478_10.ppm");
    BOOST_LOG_TRIVIAL(debug) << "GUI::initialize: image: " << image->width << "x" << image->height;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
	GLenum huboError = glGetError();
	if(huboError){
 
		BOOST_LOG_TRIVIAL(debug) << "GUI::initialize: error loading texture in opengl";
	}
    
    delete image;
}

void GUI::testDrawTest1() {
	glEnable( GL_TEXTURE_2D );
	
	glBindTexture(GL_TEXTURE_2D, textureid);
	glBegin(GL_QUADS);
	     //glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
	     //glTexCoord2f(1.0, 0.0); glVertex3f(10.0, 0.0, 0.0);
	     //glTexCoord2f(1.0, 1.0); glVertex3f(10.0, 10.0, 0.0);
	     //glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 10.0, 0.0);
	    glTexCoord2f(0.f, 0.f); glVertex3f(-10.f, -10.f, 0.f);
	    glTexCoord2f(0.f, 1.f); glVertex3f(-10.f,  10.f, 0.f);
	    glTexCoord2f(1.f, 1.f); glVertex3f( 10.f,  10.f, 0.f);
	    glTexCoord2f(1.f, 0.f); glVertex3f( 10.f, -10.f, 0.f);
	glEnd();
}

void GUI::testDrawTestBerkelium() {
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	//Glfloat global_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_TEXTURE_2D );
	glBindTexture(GL_TEXTURE_2D, web_texture);
	
	// display	
	glBegin(GL_QUADS);
	    glTexCoord2f(0.f, 1.f); glVertex3f(-1.f, -1.f, 0.f);
	    glTexCoord2f(0.f, 0.f); glVertex3f(-1.f,  1.f, 0.f);
	    glTexCoord2f(1.f, 0.f); glVertex3f( 1.f,  1.f, 0.f);
	    glTexCoord2f(1.f, 1.f); glVertex3f( 1.f, -1.f, 0.f);
    glEnd();
    
    // wait a bit before calling Berkelium::update() again
    if (testint > 10) {
		BOOST_LOG_TRIVIAL(debug) << "calling update";
		Berkelium::update();
		testint = -1;
	}
	
	testint++;
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void GUI::onPaint(Berkelium::Window* wini,
	const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
	size_t num_copy_rects, const Berkelium::Rect* copy_rects,
	int dx, int dy, const Berkelium::Rect& scroll_rect) {
		
		/*
		std::string mURL = "http://google.ca";
		std::cout << "*** onPaint "<<mURL<<std::endl;
        static int call_count = 0;
        FILE *outfile;
        {
            std::ostringstream os;
			os <<
#ifdef _WIN32
				getenv("TEMP") << "\\"
#else
				"/tmp/"
#endif
				<< "chromium_render_" << time(NULL) << "_" << (call_count++) << ".ppm";
            std::string str (os.str());
            outfile = fopen(str.c_str(), "wb");
			if(outfile == NULL) {
				std::cout << "*** cant open file "<<str<<std::endl;
				return;
			}
        }
        const int width = bitmap_rect.width();
        const int height = bitmap_rect.height();

        fprintf(outfile, "P6 %d %d 255\n", width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unsigned char r,g,b,a;
		b = *(bitmap_in++);
		g = *(bitmap_in++);
		r = *(bitmap_in++);
		a = *(bitmap_in++);
                fputc(r, outfile);  // Red
                //fputc(255-a, outfile);  // Alpha
                fputc(g, outfile);  // Green
                fputc(b, outfile);  // Blue
                //(pixel >> 24) & 0xff;  // Alpha
            }
        }
        fclose(outfile);
		*/
		
		
		bool updated = mapOnPaintToTexture(
			wini, bitmap_in, bitmap_rect, num_copy_rects, copy_rects,
			dx, dy, scroll_rect,
			web_texture, width, height, needs_full_refresh, scroll_buffer
		);
		
		
		if (updated) {
            needs_full_refresh = false;
            //glutPostRedisplay();
        }
        
}

/** Handles an onPaint call by mapping the results into an OpenGL texture. The
 *  first parameters are the same as Berkelium::WindowDelegate::onPaint.  The
 *  additional parameters and return value are:
 *  \param dest_texture - the OpenGL texture handle for the texture to render
 *                        the results into.
 *  \param dest_texture_width - width of destination texture
 *  \param dest_texture_height - height of destination texture
 *  \param ignore_partial - if true, ignore any partial updates.  This is useful
 *         if you have loaded a new page, but updates for the old page have not
 *         completed yet.
 *  \param scroll_buffer - a temporary workspace used for scroll data.  Must be
 *         at least dest_texture_width * dest_texture_height * 4 bytes large.
 *  \returns true if the texture was updated, false otherwiase
 */
bool GUI::mapOnPaintToTexture(
    Berkelium::Window *wini,
    const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
    size_t num_copy_rects, const Berkelium::Rect *copy_rects,
    int dx, int dy,
    const Berkelium::Rect& scroll_rect,
    unsigned int dest_texture,
    unsigned int dest_texture_width,
    unsigned int dest_texture_height,
    bool ignore_partial,
    char* scroll_buffer) {
	
	BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: " << dest_texture_width << "x" << dest_texture_width << " dest_texture: " << dest_texture;

    glBindTexture(GL_TEXTURE_2D, dest_texture);

    const int kBytesPerPixel = 4;
    
    
    // TESTING BEGIN
    //int length = strlen((char*)bitmap_in);
    int theMax = bitmap_rect.right() * bitmap_rect.bottom();
    int length = theMax*kBytesPerPixel;
    unsigned char* bitmap_in_copy = new unsigned char[length]();
    //strncpy((char*)bitmap_in_copy, (char*)bitmap_in, length);
    
    BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: theMax: " << theMax << " (" << length << ")";
    for(int j= 0; j < theMax; j++) {
		bitmap_in_copy[j*4+0] = bitmap_in[j*4+0];
		bitmap_in_copy[j*4+1] = bitmap_in[j*4+1];
		bitmap_in_copy[j*4+2] = bitmap_in[j*4+2];
		bitmap_in_copy[j*4+3] = bitmap_in[j*4+3];//0;
		//std::cout<<j<<": "<<bitmap_in[j*4+0]<<"**"<<bitmap_in[j*4+1]<<"**"<<bitmap_in[j*4+2]<<"**"<<bitmap_in[j*4+3]<<std::endl;
	}
	// TESTING END

    // If we've reloaded the page and need a full update, ignore updates
    // until a full one comes in.  This handles out of date updates due to
    // delays in event processing.
    if (ignore_partial) {
        if (bitmap_rect.left() != 0 ||
            bitmap_rect.top() != 0 ||
            bitmap_rect.right() != dest_texture_width ||
            bitmap_rect.bottom() != dest_texture_height) {
            return false;
        }

		BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 0a";
        glTexImage2D(GL_TEXTURE_2D, 0, kBytesPerPixel, dest_texture_width, dest_texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap_in_copy);
        ignore_partial = false;
        BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 0b";
        return true;
    }
    
    BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 1";


    // Now, we first handle scrolling. We need to do this first since it
    // requires shifting existing data, some of which will be overwritten by
    // the regular dirty rect update.
    if (dx != 0 || dy != 0) {
        // scroll_rect contains the Rect we need to move
        // First we figure out where the the data is moved to by translating it
        Berkelium::Rect scrolled_rect = scroll_rect.translate(-dx, -dy);
        // Next we figure out where they intersect, giving the scrolled
        // region
        Berkelium::Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);
        // Only do scrolling if they have non-zero intersection
        if (scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0) {
            // And the scroll is performed by moving shared_rect by (dx,dy)
            Berkelium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);

            int wid = scrolled_shared_rect.width();
            int hig = scrolled_shared_rect.height();
            
            if (DEBUG_PAINT) {
              std::cout << "Scroll rect: w=" << wid << ", h=" << hig << ", ("
                        << scrolled_shared_rect.left() << "," << scrolled_shared_rect.top()
                        << ") by (" << dx << "," << dy << ")" << std::endl;
            }
            
            int inc = 1;
            char *outputBuffer = scroll_buffer;
            // source data is offset by 1 line to prevent memcpy aliasing
            // In this case, it can happen if dy==0 and dx!=0.
            char *inputBuffer = scroll_buffer+(dest_texture_width*1*kBytesPerPixel);
            int jj = 0;
            if (dy > 0) {
                // Here, we need to shift the buffer around so that we start in the
                // extra row at the end, and then copy in reverse so that we
                // don't clobber source data before copying it.
                outputBuffer = scroll_buffer+(
                    (scrolled_shared_rect.top()+hig+1)*dest_texture_width
                    - hig*wid)*kBytesPerPixel;
                inputBuffer = scroll_buffer;
                inc = -1;
                jj = hig-1;
            }

            // Copy the data out of the texture
            glGetTexImage(
                GL_TEXTURE_2D, 0,
                GL_BGRA, GL_UNSIGNED_BYTE,
                inputBuffer
            );

            // Annoyingly, OpenGL doesn't provide convenient primitives, so
            // we manually copy out the region to the beginning of the
            // buffer
            for(; jj < hig && jj >= 0; jj+=inc) {
                memcpy(
                    outputBuffer + (jj*wid) * kBytesPerPixel,
//scroll_buffer + (jj*wid * kBytesPerPixel),
                    inputBuffer + (
                        (scrolled_shared_rect.top()+jj)*dest_texture_width
                        + scrolled_shared_rect.left()) * kBytesPerPixel,
                    wid*kBytesPerPixel
                );
            }

            // And finally, we push it back into the texture in the right
            // location
            glTexSubImage2D(GL_TEXTURE_2D, 0,
                shared_rect.left(), shared_rect.top(),
                shared_rect.width(), shared_rect.height(),
                GL_BGRA, GL_UNSIGNED_BYTE, outputBuffer
            );
        }
    }

	BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 2";
    if (DEBUG_PAINT) {
      std::cout << (void*)wini << " Bitmap rect: w="
                << bitmap_rect.width()<<", h="<<bitmap_rect.height()
                <<", ("<<bitmap_rect.top()<<","<<bitmap_rect.left()
                <<") tex size "<<dest_texture_width<<"x"<<dest_texture_height
                <<std::endl;
    }
    
    BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 3";
    for (size_t i = 0; i < num_copy_rects; i++) {
        int wid = copy_rects[i].width();
        int hig = copy_rects[i].height();
        int top = copy_rects[i].top() - bitmap_rect.top();
        int left = copy_rects[i].left() - bitmap_rect.left();
        BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 4";
        
        if (DEBUG_PAINT) {
            std::cout << (void*)wini << " Copy rect: w=" << wid << ", h=" << hig << ", ("
                      << top << "," << left << ")" << std::endl;
        }
        BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 5";
        for(int jj = 0; jj < hig; jj++) {
            memcpy(
                scroll_buffer + jj*wid*kBytesPerPixel,
                bitmap_in_copy + (left + (jj+top)*bitmap_rect.width())*kBytesPerPixel,
                wid*kBytesPerPixel
                );
        }
		BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 6";
        // Finally, we perform the main update, just copying the rect that is
        // marked as dirty but not from scrolled data.
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        copy_rects[i].left(), copy_rects[i].top(),
                        wid, hig,
                        GL_BGRA, GL_UNSIGNED_BYTE, scroll_buffer
            );
        BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 7";
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    
    BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 8";

    return true;
}



/**
 * TESTING - Need to actually implement this properly (i.e. own the pointer, etc)
 */ 
IGUIComponent* GUI::load(std::string filename) {
	HtmlGuiComponent* comp = new HtmlGuiComponent();
	comp->setContents("<html><body>himom</body></html>");
	
	if (comp->initialize() < 0) {
		comp->destroy();
		delete comp;
		return 0;
	}
	
	return comp;
	
}

}

}
