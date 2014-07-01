#ifdef USE_CEF

#ifndef GUICOMPONENT_H_
#define GUICOMPONENT_H_

#include <string>
#include <map>
#include <mutex>

#include <GL/glew.h>

#include <cef_app.h>
#include <cef_client.h>

#include "glw/shaders/IShaderProgram.hpp"
#include "glw/IOpenGlDevice.hpp"

#include "../IGuiComponent.hpp"

#include "GuiObject.hpp"
#include "RenderHandler.hpp"

namespace glmd = glm::detail;

namespace glr
{
namespace gui
{
namespace cef
{

class GuiComponent : public IGuiComponent, public CefClient, public CefLifeSpanHandler
{
public:
	GuiComponent(glw::IOpenGlDevice* openGlDevice, glmd::uint32 width, glmd::uint32 height);
	virtual ~GuiComponent();

	virtual void load();
	virtual void unload();

	void mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos);
	void mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount = 1);
	void mouseClick(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos);
	void mouseWheel(glm::detail::int32 xPos, glm::detail::int32 yPos, glm::detail::int32 xScroll, glm::detail::int32 yScroll);

	void textEvent(const wchar_t*evt, size_t evtLength);
	void keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 virtualKeyCode, glm::detail::int32 scanCode);
	
	/**
	 * Converts the unicode character represented by keyCode to a Cef3 compatible keycode and returns it.
	 * 
	 * @param keyCode Integer value of a unicode character
	 * 
	 * @return A Cef3 compatible keycode.
	 */
	glm::detail::int32 convertUnicodeToCef3(glm::detail::int32 keyCode);

	int setContents(std::string contents);
	int setContentsUrl(std::string url);

	void update();
	// Temporary method until we remove legacy OpenGL rendering code
	void render();
	void render(shaders::IShaderProgram& shader);

	virtual void executeScript(const std::wstring& script);

	virtual bool isVisible() const;
	virtual void setVisible(bool isVisible);

	virtual IGuiObject* createGuiObject(const std::wstring& name);
	virtual IGuiObject* getGuiObject(const std::wstring& name) const;
	
	void windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height);
	
	// Implement functions for CefClient
	/**
	 * Processes a message received from the render process.
	 * 
	 * List of available functions:
	 * 
	 * ExecuteFunction
	 * 		funcName [<argument> [, ...]]
	 * ReadyForBindings
	 *
	 * AllBindingsReceived
	 */
	virtual bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message);
	virtual CefRefPtr<CefRenderHandler> GetRenderHandler();
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE;
	
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

private:
	bool isVisible_;

	std::string url_;
	
	glw::IOpenGlDevice* openGlDevice_;

	// Width and height of our window.
	glmd::uint32 width_;
	glmd::uint32 height_;
	// Storage for a texture
	glmd::uint32 webTexture_;

	// CEF3 variables
	CefRefPtr<CefBrowser> browser_;
    CefRefPtr<CefRenderHandler> renderHandler_;
    
    bool bindDataSent_;
    // The number of unique message ids will be limited by the size of uint32 (but I don't think this will be a problem)
    glmd::uint32 numMessagesSent_;
    std::map< std::string, glmd::uint32 > messageIdMap_;
    std::mutex messageIdMapMutex_;

	std::map< std::wstring, std::unique_ptr<GuiObject> > guiObjects_;

	std::wstring getFunctionName(const std::wstring& name) const;
	std::wstring getObjectName(const std::wstring& name) const;

	glm::detail::int32 getCefStateModifiers(glm::detail::int32 state);
	
	/**
	 * 
	 * @return The number of functions sent via IPC to the render process.
	 */
	int sendBoundFunctionsToRenderProcess();
	
	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(GuiComponent)
    //IMPLEMENT_LOCKING(GuiComponent)
};

}
}
}

#endif /* GUICOMPONENT_H_ */

#endif /* USE_CEF */
