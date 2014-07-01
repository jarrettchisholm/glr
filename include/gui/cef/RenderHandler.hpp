#ifdef USE_CEF

#ifndef RENDERHANDLER_H_
#define RENDERHANDLER_H_

#include <GL/glew.h>

#include <cef_app.h>
#include <cef_render_handler.h>

#include "glw/IOpenGlDevice.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

class RenderHandler : public CefRenderHandler
{
public:
	RenderHandler(glm::detail::uint32 webTexture, glm::detail::uint32 width, glm::detail::uint32 height);

	/**
	 * If the size of the window changes, this method should be called with the new dimensions.
	 * 
	 * @param width The new width of the window.
	 * @param height The new height of the window.
	 */
	void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height);
	
	// CefRenderHandler interface
	virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);
    virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height);
	
private:
	glm::detail::uint32 webTexture_;
	glm::detail::uint32 width_;
	glm::detail::uint32 height_;
    
    // CefBase interface
	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(RenderHandler)
};

}
}
}

#endif /* RENDERHANDLER_H_ */

#endif /* USE_CEF */
