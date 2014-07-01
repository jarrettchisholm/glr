#ifdef USE_CEF

#include <string>

#include "gui/cef/RenderHandler.hpp"

#include "common/logger/Logger.hpp"

#include "exceptions/ExceptionInclude.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

namespace glmd = glm::detail;

RenderHandler::RenderHandler(glmd::uint32 webTexture, glmd::uint32 width, glmd::uint32 height) : webTexture_(webTexture), width_(width), height_(height)
{
	if (webTexture_ == 0)
	{
		std::string msg = "Error - Cef 3 texture id is invalid.";
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
}

// CefRenderHandler interface
bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
	rect = CefRect(0, 0, width_, height_);
	return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
	//std::cout << "painting!: " << width << " " << height << std::endl;
	//memcpy(texBuf->getCurrentLock().data, buffer, width*height*4);
	
	// TODO: make this use a shader
	// TODO: make this paint only sections that have changed (do I really want/need to do this?)
	glBindTexture(GL_TEXTURE_2D, webTexture_);
	
	// Finally, we perform the main update, just copying the rect that is
	// marked as dirty but not from scrolled data.
	/*
	glTexSubImage2D(GL_TEXTURE_2D, 0,
					0, 0,
					width, height,
					GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, 
					buffer
					);
					*/
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderHandler::windowSizeUpdate(glmd::uint32 width, glmd::uint32 height)
{
	width_ = width;
	height_ = height;
}

}
}
}

#endif /* USE_CEF */
