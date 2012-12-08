/*
 * HtmlGuiComponent.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */
 
#include <iostream>
#include <string.h>
#include <cstring>

#include <boost/log/trivial.hpp>

#include "HtmlGuiComponent.h"

#include "../../common/utilities/ImageLoader.h"

#define DEBUG_PAINT true

namespace icee {

namespace engine {

HtmlGuiComponent::HtmlGuiComponent() {
	isVisible_ = false;
}


HtmlGuiComponent::~HtmlGuiComponent() {
	
}

int HtmlGuiComponent::load() {
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
    window_ = Berkelium::Window::create(context);
    delete context;
    
    width = 1024;
    height = 768;
    
    scroll_buffer = new char[width*(height+1)*4];
    
    //MyDelegate* delegate = new MyDelegate();
    window_->setDelegate(this);
    window_->resize(width, height);
    window_->setTransparent(true);
    //std::string url = "file:///home/jarrett/projects/chisholmsoft/dark_horizon/data/test.html";
    window_->navigateTo(Berkelium::URLString::point_to(url_.data(), url_.length()));
    
    // TESTING CALLBACKS START
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutCallback"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"glutCB"), false));
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutCallbackSync"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"glutCB"), true));
	window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest"),
        Berkelium::Script::Variant::emptyObject());
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray"),
        Berkelium::Script::Variant::emptyArray());
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray[0]"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"arrayFunc"), false));
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.someArray[1]"),
        Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(L"arrayFunc"), true));
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.nullValue"),
        Berkelium::Script::Variant());
    window_->addBindOnStartLoading(
        Berkelium::WideString::point_to(L"glutObjectTest.stringValue"),
        Berkelium::Script::Variant("Hello, World!"));
	// TESTING CALLBACKS END
    
    testint = 31;
    webTextureReady_ = false;
    needs_full_refresh = true;
    
	return 0;
}

void HtmlGuiComponent::unload() {
	this->setVisible(false);
	window_ = 0;
	Berkelium::destroy();
}

void HtmlGuiComponent::mouseMoved(sint32 xPos, sint32 yPos) {
	
}

void HtmlGuiComponent::mouseButton(uint32 buttonID, bool down, sint32 clickCount) {
	
}

void HtmlGuiComponent::mouseWheel(sint32 xScroll, sint32 yScroll) {
	
}
	
void HtmlGuiComponent::textEvent(const wchar_t *evt, size_t evtLength) {
	std::cout << "TEXT EVENT: " << evt << std::endl;
	if ( wcsncmp(evt, L"`", evtLength) == 0 || wcsncmp(evt, L"~", evtLength) == 0 ) {
		/*
		 *	if( $('#console').hasClass('hidden') ) {
		 *		$('#console').removeClass('hidden');
		 *	} else {
		 *		$('#console').addClass('hidden');
		 *	} 
		 */ 
		window_->executeJavascript( Berkelium::WideString::point_to(
			L"if( $('#console').hasClass('hidden') ) {"
			"	$('#console').removeClass('hidden');"
			"	$('#console').addClass('visible');"
			"	$('#console').click();"
			"} else {"
			"	$('#console').addClass('hidden');"
			"	$('#console').removeClass('visible');"
			"}"
			) );
	} else {
		std::cout << "HERE 1 " << (char*)evt << std::endl;
		window_->focus();
		wchar_t outchars[2];
        outchars[0] = evt[0];
        outchars[1] = 0;
        std::cout << "HERE 2 " << outchars[0] << std::endl;
		window_->textEvent(outchars, 1);
	}
}

void HtmlGuiComponent::keyEvent(bool pressed, sint32 mods, sint32 vk_code, sint32 scancode) {
	std::cout << "KEY EVENT: (" << pressed << ") " << (char)vk_code << std::endl;
	
	if ( vk_code == '`' || vk_code == '~' ) {
		window_->executeJavascript( Berkelium::WideString::point_to(
			L"if( $('#console').hasClass('hidden') ) {"
			"	$('#console').removeClass('hidden');"
			"	$('#console').addClass('visible');"
			"	$('#console').click();"
			"} else {"
			"	$('#console').addClass('hidden');"
			"	$('#console').removeClass('visible');"
			"}"
			) );
	} else {	
		//window_->focus();
		wchar_t outchars[2];
        outchars[0] = vk_code;
        outchars[1] = 0;
        std::cout << "HERE 2 " << outchars[0] << std::endl;
		window_->textEvent(outchars, 1);
		//window_->keyEvent(pressed, mods, vk_code, scancode);
	}
	
}

/**
 * Need to implement!
 */ 
int HtmlGuiComponent::setContents(std::string contents) {
	url_ = contents;
	
	return 0;
}

int HtmlGuiComponent::loadContentsFromFile(std::string filename) {
	url_ = filename;
	
	return 0;
}

void HtmlGuiComponent::update() {
	window_->executeJavascript( Berkelium::WideString::point_to(L"update();") );
}


void HtmlGuiComponent::render() {
	testDrawTestBerkelium();
}

void HtmlGuiComponent::testDrawTestBerkelium() {
	if (webTextureReady_) {
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		glColor3f(1.0, 1.0, 1.0);
		
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
	    
	    glBindTexture(GL_TEXTURE_2D, 0);
	    glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glEnable(GL_LIGHTING);
    }
    
    // wait a bit before calling Berkelium::update() again
    //if (testint > 3) {
		Berkelium::update();
		testint = -1;
	//}
	
	testint++;
}

void HtmlGuiComponent::onCrashed(Berkelium::Window *win) {
	std::cout << "*** onCrashed " << std::endl;
}

void HtmlGuiComponent::onUnresponsive(Berkelium::Window *win) {
	std::cout << "*** onUnresponsive " << std::endl;
}

void HtmlGuiComponent::onScriptAlert(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool &success, Berkelium::WideString &value) {
	std::wcout << L"*** onScriptAlert " << message << std::endl;
}

void HtmlGuiComponent::onJavascriptCallback(Berkelium::Window *win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, Berkelium::Script::Variant *args, size_t numArgs) {
	std::cout << "*** onJavascriptCallback at URL " << url << ", "
			  << (replyMsg?"synchronous":"async") << std::endl;
	std::wcout << L"    Function name: " << funcName << std::endl;
	for (size_t i = 0; i < numArgs; i++) {
		Berkelium::WideString jsonStr = toJSON(args[i]);
		std::wcout << L"    Argument " << i << ": ";
		if (args[i].type() == Berkelium::Script::Variant::JSSTRING) {
			std::wcout << L"(string) " << args[i].toString() << std::endl;
		} else {
			std::wcout << jsonStr << std::endl;
		}
		Berkelium::Script::toJSON_free(jsonStr);
	}
	if (replyMsg) {
		win->synchronousScriptReturn(replyMsg, numArgs ? args[0] : Berkelium::Script::Variant());
	}
}

void HtmlGuiComponent::onPaint(Berkelium::Window* wini,
	const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
	size_t num_copy_rects, const Berkelium::Rect* copy_rects,
	int dx, int dy, const Berkelium::Rect& scroll_rect) {
		bool updated = mapOnPaintToTexture(
			wini, bitmap_in, bitmap_rect, num_copy_rects, copy_rects,
			dx, dy, scroll_rect,
			web_texture, width, height, needs_full_refresh, scroll_buffer
		);
		
		
		if (updated) {
            needs_full_refresh = false;
            webTextureReady_ = true;
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
bool HtmlGuiComponent::mapOnPaintToTexture(
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

        glTexImage2D(GL_TEXTURE_2D, 0, kBytesPerPixel, dest_texture_width, dest_texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap_in);
        ignore_partial = false;
        return true;
    }


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
                bitmap_in + (left + (jj+top)*bitmap_rect.width())*kBytesPerPixel,
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

void HtmlGuiComponent::executeScript(std::wstring script) {
	window_->executeJavascript( Berkelium::WideString::point_to(script) );
}

bool HtmlGuiComponent::isVisible() {
	return isVisible_;
}

void HtmlGuiComponent::setVisible(bool isVisible) {
	isVisible_ = isVisible;
}

}

}
