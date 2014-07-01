#ifdef USE_CEF

#include <iostream>
#include <string.h>
#include <cstring>

#include "gui/cef/GuiComponent.hpp"

#include "Configure.hpp"

#include "common/utilities/ImageLoader.hpp"
#include "common/utilities/BoostAnyUtilities.hpp"
#include "common/utilities/StringUtilities.hpp"

#include "common/logger/Logger.hpp"

#include "exceptions/ExceptionInclude.hpp"

#include "../../../cef_client/include/FunctionList.hpp"
#include "../../../cef_client/include/ExceptionList.hpp"

namespace glr
{
namespace gui
{
namespace cef
{

GuiComponent::GuiComponent(glw::IOpenGlDevice* openGlDevice, glmd::uint32 width, glmd::uint32 height) : openGlDevice_(openGlDevice), width_(width), height_(height)
{
	isVisible_ = false;
	bindDataSent_ = false;
	numMessagesSent_ = 0;
}

GuiComponent::~GuiComponent()
{
	unload();
}

bool GuiComponent::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message )
{
	std::wstring s = message->GetName().ToWString();
	//std::wcout << L"GuiComponent OnProcessMessageReceived " << s << std::endl;
	
	// Do we need this?
	// browser == m_Browser
	
	if ( s == cef_client::EXECUTE_FUNCTION )
	{	
		std::wstring objName = message->GetArgumentList()->GetString(0);
		std::wstring functionName = message->GetArgumentList()->GetString(1);
		glmd::int32 numArguments = message->GetArgumentList()->GetInt(2);
		
		//std::wcout << L"GuiComponent ExecuteFunction " << objName << "." << functionName << " " << numArguments << std::endl;
		
		auto params = std::vector< boost::any >();
		
		// Wrap parameters
		for (glmd::int32 i = 0; i < numArguments; i++)
		{
			const CefValueType type = message->GetArgumentList()->GetType( i+2 );
			switch (type)
			{
				case VTYPE_INT:
				{
					glmd::int32 arg = message->GetArgumentList()->GetInt(i+2);
					params.push_back(boost::any(arg));
					//std::cout << "VTYPE_INT - " << arg << std::endl;
				}
					break;
				
				case VTYPE_STRING:
				{
					std::string arg = message->GetArgumentList()->GetString(i+2);
					params.push_back(boost::any(arg));
					//std::cout << "VTYPE_STRING - " << arg << std::endl;
				}
					break;

				case VTYPE_BOOL:
				{
					bool arg = message->GetArgumentList()->GetBool(i+2);
					params.push_back(boost::any(arg));
					//std::cout << "VTYPE_BOOL - " << arg << std::endl;
				}
					break;
				
				case VTYPE_DOUBLE:
				{
					glmd::float64 arg = message->GetArgumentList()->GetDouble(i+2);
					params.push_back(boost::any(arg));
					//std::cout << "VTYPE_DOUBLE - " << arg << std::endl;
				}
					break;
				
				case VTYPE_BINARY:
				{
					std::string msg = "Error - VTYPE_BINARY not implemented as CEF3 argument type.";
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
					
				case VTYPE_DICTIONARY:
				{
					std::string msg = "Error - VTYPE_DICTIONARY not implemented as CEF3 argument type.";
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
				
				case VTYPE_LIST:
				{
					std::string msg = "Error - VTYPE_LIST not implemented as CEF3 argument type.";
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
				
				case VTYPE_INVALID:
				{
					std::string msg = "Error - VTYPE_INVALID not implemented as CEF3 argument type.";
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
					
				case VTYPE_NULL:
				{
					std::string msg = "Error - VTYPE_NULL not implemented as CEF3 argument type.";
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
				
				// Should the default case throw an exception?
				default:
				{
					std::string msg = "Error - Unknown CEF3 argument type:" + type;					
					LOG_ERROR( msg );
					throw exception::Exception( msg );
				}
					break;
			}
		}
		
		auto it = guiObjects_.find(objName);
		if ( it != guiObjects_.end() )
		{
			boost::any r = it->second->processCallback(functionName, params);
			std::string messageId = "Message_" + std::to_string(numMessagesSent_);
			messageIdMapMutex_.lock();
			messageIdMap_[messageId] = numMessagesSent_;
			messageIdMapMutex_.unlock();
			numMessagesSent_++;
			
			CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( cef_client::FUNCTION_RESULT );
			m->GetArgumentList()->SetString( 0, messageId );
			m->GetArgumentList()->SetString( 1, functionName );
			m->GetArgumentList()->SetInt( 2, 1 );
			
			if( r.empty() )
			{
				// There is no return value (i.e. void), so set num return results 0
				m->GetArgumentList()->SetInt( 2, 0 );
				//std::cout << "ExecuteFunction RESULT: void" << std::endl;
			}
			else if ( utilities::isString(r) )
			{
				m->GetArgumentList()->SetString( 3, boost::any_cast<std::string>(r) );
				//std::cout << "ExecuteFunction RESULT:" << boost::any_cast<std::string>(r) << std::endl;
			}
			else if ( utilities::isWstring(r) )
			{
				m->GetArgumentList()->SetString( 3, boost::any_cast<std::wstring>(r) );
				//std::wcout << L"ExecuteFunction RESULT:" << boost::any_cast<std::wstring>(r) << std::endl;
			}
			else if ( utilities::isInt(r) )
			{
				m->GetArgumentList()->SetInt( 3, boost::any_cast<int>(r) );
				//std::cout << "ExecuteFunction RESULT:" << boost::any_cast<int>(r) << std::endl;
			}
			else if ( utilities::isUint(r) )
			{
				m->GetArgumentList()->SetInt( 3, boost::any_cast<unsigned int>(r) );
				//std::cout << "ExecuteFunction RESULT:" << boost::any_cast<unsigned int>(r) << std::endl;
			}
			else if ( utilities::isFloat(r) )
			{
				m->GetArgumentList()->SetDouble( 3, boost::any_cast<float>(r) );
				//std::cout << "ExecuteFunction RESULT:" << boost::any_cast<float>(r) << std::endl;
			}
			else if ( utilities::isDouble(r) )
			{
				m->GetArgumentList()->SetDouble( 3, boost::any_cast<double>(r) );
				//std::cout << "ExecuteFunction RESULT:" << boost::any_cast<double>(r) << std::endl;
			}
			else
			{
				std::string msg = "Error - ExecuteFunction unable to determine result type.";
				LOG_ERROR( msg );
				throw exception::Exception( msg );
			}
			
			browser->SendProcessMessage(PID_RENDERER, m);
		}
	}
	else if ( s == cef_client::READY_FOR_BINDINGS && !bindDataSent_ )
	{ 
		glmd::uint32 numSent = sendBoundFunctionsToRenderProcess();
		std::string messageId = "Message_" + std::to_string(numMessagesSent_);
		messageIdMapMutex_.lock();
		messageIdMap_[messageId] = numMessagesSent_;
		messageIdMapMutex_.unlock();
		numMessagesSent_++;
		
		CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( cef_client::ALL_BINDINGS_SENT );
		m->GetArgumentList()->SetString( 0, messageId );
		m->GetArgumentList()->SetInt( 1, numSent );
		browser->SendProcessMessage(PID_RENDERER, m);
		//std::cout << "ReadyForBindings finished with " << numSent << " function(s) sent to the render process." << std::endl;
		bindDataSent_ = true;
	}
	else if ( s == cef_client::ALL_BINDINGS_RECEIVED && bindDataSent_ )
	{
		// Now that bindings are set, we can load the url
		browser_->GetMainFrame()->LoadURL(url_);
	}
	else if ( s == cef_client::ALL_BINDINGS_RECEIVED && !bindDataSent_ )
	{ 
		std::string msg = "AllBindingsReceived message processed, but no binding data was sent.";
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}
	else if ( s == cef_client::SUCCESS )
	{ 
		// TODO: deal with success
		std::string messageId = message->GetArgumentList()->GetString( 0 );
		
		messageIdMapMutex_.lock();
		auto it = messageIdMap_.find(messageId);
		if (it == messageIdMap_.end())
		{
			std::string msg = "Success message id '" + messageId + "' does not match any ids of sent messages.";
			LOG_ERROR( msg );
			throw exception::Exception( msg );
		}
		
		messageIdMap_.erase(it);
		messageIdMapMutex_.unlock();
		
		std::stringstream msg;
		msg << "GuiComponent Success: " << messageId;
		LOG_DEBUG( msg.str() );
	}
	else if ( s == cef_client::EXCEPTION )
	{ 
		//std::string messageId = message->GetArgumentList()->GetString( 0 );
		cef_client::Exception e = (cef_client::Exception) message->GetArgumentList()->GetInt( 1 );
		std::string errorMessage = message->GetArgumentList()->GetString( 2 );
		
		// Get string name for exception type
		std::string errorName = cef_client::getStringName(e);
		
		// c++ exception doesn't seem to support wchar...
		//std::wstring message = message->GetArgumentList()->GetWString( 2 );		
		
		std::stringstream msgStream;
		msgStream << "Exception message received: " << errorMessage << " - Exception type: " << errorName << " (" << e << ")";
		
		std::string msg = msgStream.str();
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}
	
	return true;
}

void GuiComponent::load()
{
	LOG_DEBUG( "Creating GuiComponent texture." );
	
	// Create texture to hold rendered view
	glGenTextures(1, &webTexture_);
	glBindTexture(GL_TEXTURE_2D, webTexture_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glw::GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream msgStream;
		msgStream << "Error loading GuiComponent in opengl: " << err.name;
		std::string msg = msgStream.str();
		
		LOG_ERROR( msg );
		throw exception::GlException( msg );
	}
	else
	{
		LOG_DEBUG( "Successfully created GuiComponent texture." );
	}

	LOG_DEBUG( "Creating GuiComponent CEF components." );

	CefWindowInfo windowInfo;
	CefBrowserSettings browserSettings;

	// in linux set a gtk widget, in windows a hwnd. If not available set nullptr - may cause some render errors, in context-menu and plugins.
	windowInfo.SetAsOffScreen(nullptr);
	windowInfo.SetTransparentPainting(true);
	
	renderHandler_ = new RenderHandler(webTexture_, width_, height_);
	
	LOG_DEBUG( "Creating CEF Browser object." );
	// Create initially with "/" appended to url to make sure CEF DOESN'T fully load the page.
	// We just want the render process created at this point so that we can do our bindings.
	browser_ = CefBrowserHost::CreateBrowserSync(windowInfo, this, url_ + "/", browserSettings, nullptr);
	
	if (browser_.get() == nullptr)
	{
		// The render process should clean up on its own.
		std::string msg = "Error loading GuiComponent - could not create CEF browser.";
		
		LOG_ERROR( msg );
		throw exception::Exception( msg );
	}

	//browser_->GetMainFrame()->LoadURL(std::string("http://www.jarrettchisholm.com").c_str());

	//sendBoundFunctionsToRenderProcess();

	LOG_DEBUG( "Successfully created GuiComponent CEF components." );
}

void GuiComponent::unload()
{
	// TODO: properly dispose of browser
	if ( browser_ != nullptr )
	{
		browser_->GetHost()->CloseBrowser( true );
		//browser_ = nullptr;
	}
		
	setVisible(false);
}

/** Maps an input coordinate to a texture coordinate for injection into
 *  Berkelium.
 *  \param glut_size the size of the GLUT window
 *  \param glut_coord the coordinate value received from GLUT
 *  \param tex_size the size of the texture/Berkelium window
 *  \returns the coordinate transformed to the correct value for the texture /
 *		   Berkelium window
 */
/*
unsigned int GuiComponent::mapGLUTCoordToTexCoord(unsigned int glut_coord, unsigned int glut_size, unsigned int tex_size)
{
	return (glut_coord * tex_size) / glut_size;
}
*/

void GuiComponent::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
}

bool GuiComponent::DoClose(CefRefPtr<CefBrowser> browser)
{
	return false;
}

void GuiComponent::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	browser_ = nullptr;
}

CefRefPtr<CefLifeSpanHandler> GuiComponent::GetLifeSpanHandler()
{
	return this;
}

void GuiComponent::mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos)
{
	////std::cout << "MOUSE MOVED EVENT: " << xPos << " " << yPos << std::endl;
	
	//unsigned int tex_coord_x = mapGLUTCoordToTexCoord(xPos, width_, width_);
	//unsigned int tex_coord_y = mapGLUTCoordToTexCoord(yPos, height_, height_);
	////std::cout << xPos << " " << yPos << " : " << tex_coord_x << " " << tex_coord_y << std::endl;
	//window_->mouseMoved(tex_coord_x, tex_coord_y);
	
	CefMouseEvent mouseEvent;
	mouseEvent.x = xPos;
	mouseEvent.y = yPos;
	//window->ApplyPopupOffset(mouseEvent.x, mouseEvent.y);
	mouseEvent.modifiers = getCefStateModifiers(0);
	
	bool mouseLeave = false; //(event->type == GDK_LEAVE_NOTIFY);
	
	browser_->GetHost()->SendMouseMoveEvent(mouseEvent, mouseLeave);
}

void GuiComponent::mouseButton(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos, bool down, glm::detail::int32 clickCount)
{
	//std::cout << "MOUSE BUTTON EVENT: " << buttonId << " " << xPos << " " << yPos << " " << down << std::endl;
	CefBrowserHost::MouseButtonType buttonType = MBT_LEFT;
	switch (buttonId)
	{
		case 0:
			break;
		case 1:
			buttonType = MBT_MIDDLE;
			break;
		case 2:
			buttonType = MBT_RIGHT;
			break;
		default:
			// Other mouse buttons are not handled here.
			std::stringstream msg;
			msg << "Unable to determine mouse button: " << buttonId;
			LOG_WARN( msg.str() );
			return;
	}
	
	CefMouseEvent mouseEvent;
	mouseEvent.x = xPos;
	mouseEvent.y = yPos;
	//window->ApplyPopupOffset(mouseEvent.x, mouseEvent.y);
	mouseEvent.modifiers = getCefStateModifiers(0);
	
	bool mouseUp = !down; //(event->type == GDK_BUTTON_RELEASE);
	//if (!mouseUp)
	//gtk_widget_grab_focus(widget);
	
	//int clickCount = 1;
	/*
	switch (event->type) {
	case GDK_2BUTTON_PRESS:
	  clickCount = 2;
	  break;
	case GDK_3BUTTON_PRESS:
	  clickCount = 3;
	  break;
	default:
	  break;
	}
	*/
	//browser_->GetHost()->SendFocusEvent(true);
	browser_->GetHost()->SendMouseClickEvent(mouseEvent, buttonType, mouseUp, clickCount);
}

void GuiComponent::mouseClick(glm::detail::uint32 buttonId, glm::detail::int32 xPos, glm::detail::int32 yPos)
{
	mouseButton(buttonId, xPos, yPos, true, 1);
	mouseButton(buttonId, xPos, yPos, false, 1);
}

void GuiComponent::mouseWheel(glm::detail::int32 xPos, glm::detail::int32 yPos, glm::detail::int32 xScroll, glm::detail::int32 yScroll)
{
	CefMouseEvent event;
	
	event.x = xPos;
	event.y = yPos;
	//event.modifiers = getCefStateModifiers(0);
	
	browser_->GetHost()->SendMouseWheelEvent( event, xScroll, yScroll );
}

void GuiComponent::textEvent(const wchar_t* evt, size_t evtLength)
{
	//std::cout << "TEXT EVENT: " << evt << std::endl;
	//std::cout << "HERE 1 " << (char*)evt << std::endl;
	//window_->focus();
	
	//wchar_t outchars[2];
	//outchars[0] = evt[0];
	//outchars[1] = 0;
	//std::cout << "HERE 2 " << outchars[0] << std::endl;
	//window_->textEvent(outchars, 1);
}

glm::detail::int32 GuiComponent::getCefStateModifiers(glm::detail::int32 state) {
  glm::detail::int32 modifiers = 0;
  /*
  if (state & GDK_SHIFT_MASK)
	modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (state & GDK_LOCK_MASK)
	modifiers |= EVENTFLAG_CAPS_LOCK_ON;
  if (state & GDK_CONTROL_MASK)
	modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (state & GDK_MOD1_MASK)
	modifiers |= EVENTFLAG_ALT_DOWN;
  if (state & GDK_BUTTON1_MASK)
	modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (state & GDK_BUTTON2_MASK)
	modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (state & GDK_BUTTON3_MASK)
	modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  */
  return modifiers;
}

int GuiComponent::sendBoundFunctionsToRenderProcess()
{
	glmd::uint32 numSent = 0;
	
	// Set all of our GuiObject contexts
	for ( auto& it : guiObjects_ )
	{
		std::string messageId = "Message_" + std::to_string(numMessagesSent_);
		messageIdMapMutex_.lock();
		messageIdMap_[messageId] = numMessagesSent_;
		messageIdMapMutex_.unlock();
		numMessagesSent_++;
		
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create( cef_client::ADD_OBJECT );
		message->GetArgumentList()->SetString( 0, messageId );
		message->GetArgumentList()->SetString( 1, it.second->getName() );
		browser_->SendProcessMessage(PID_RENDERER, message);
		numSent++;
		
		std::vector< std::wstring > names = it.second->getFunctionNames();
		
		message = CefProcessMessage::Create( cef_client::ADD_METHOD_TO_OBJECT );
		for ( auto& name : names )
		{
			messageId = "Message_" + std::to_string(numMessagesSent_);
			messageIdMapMutex_.lock();
			messageIdMap_[messageId] = numMessagesSent_;
			messageIdMapMutex_.unlock();
			
			numMessagesSent_++;
			
			message->GetArgumentList()->SetString( 0, messageId );
			message->GetArgumentList()->SetString( 1, it.second->getName() );
			message->GetArgumentList()->SetString( 2, name );
			browser_->SendProcessMessage(PID_RENDERER, message);
			numSent++;
		}
		
		// Binding wrapper
		browser_->GetMainFrame()->ExecuteJavaScript(
										L"if (!" + it.second->getName() + L")\
											var " + it.second->getName() + L" = (window." + it.second->getName() + L");",
										"about:blank", 
										0
									);
	}
	
	return numSent;
}

glm::detail::int32 GuiComponent::convertUnicodeToCef3(glm::detail::int32 keyCode)
{
	switch (keyCode)
	{
#ifdef OS_WINDOWS
		// Arrow keys: Left, Up, Right, Down
		case 276:
			return 37;
		case 273:
			return 38;
		case 275:
			return 39;
		case 274:
			return 40;
#else
		// Enter key
		case 13:
			return 65293;
		
		// Escape
		case 27:
			return 65307;
			
		// Tab
		case 9:
			return 65289;
		
		// Backspace
		case 8:
			return 65288;
		
		// Arrow keys: Left, Up, Right, Down
		case 276:
			return 65361;
		case 273:
			return 65362;
		case 275:
			return 65363;
		case 274:
			return 65364;
		
		// PrtScn, ScrLk, Pause, Num Lock
		case 316:
			return 65377;
		case 302:
			return 65300;
		case 19:
			return 65299;
		case 144:
			return 65407;
		
		// Insert, Delete
		case 277:
			return 65379;
		case 127:
			return 65535;
		
		// Home, End
		case 278:
			return 65360;
		case 279:
			return 65367;
		
		// Page Up, Page Down
		case 280:
			return 65365;
		case 281:
			return 65366;
		
		// Left Shift, Right Shift
		case 304:
			return 65505;
		case 303:
			return 65506;
		// Left Ctrl, Right Ctrl
		case 306:
			return 65507;
		case 305:
			return  65508;
		// Left Alt, Right Alt
		case 308:
			return 65513;
		case 313:
			return 65514;
		
		// F1-F12
		case 282:
			return 65470;
		case 283:
			return 65471;
		case 284:
			return 65472;
		case 285:
			return 65473;
		case 286:
			return 65474;
		case 287:
			return 65475;
		case 288:
			return 65476;
		case 289:
			return 65477;
		case 290:
			return 65478;
		case 291:
			return 65479;
		case 292:
			return 65480;
		case 293:
			return 65481;
#endif

		default:
			return keyCode;
	}
	
	std::string msg = std::string("This error should never happen.");
	LOG_ERROR(msg);
	throw exception::Exception(msg);
}

void GuiComponent::keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 virtualKeyCode, glm::detail::int32 scanCode)
{
	std::cout << "KEY EVENT: (" << pressed << ") " << (char)virtualKeyCode << " | " << virtualKeyCode << std::endl;

	// Translate key codes for CEF3
	virtualKeyCode = convertUnicodeToCef3(virtualKeyCode);
	
	CefKeyEvent keyEvent;
	keyEvent.windows_key_code = virtualKeyCode;
	keyEvent.native_key_code = virtualKeyCode;
	keyEvent.modifiers = getCefStateModifiers(mods);
	
	if (pressed)
	{
		keyEvent.type = KEYEVENT_RAWKEYDOWN;
		browser_->GetHost()->SendKeyEvent(keyEvent);
	}
	else
	{
		keyEvent.type = KEYEVENT_KEYUP;
		browser_->GetHost()->SendKeyEvent(keyEvent);
		
		keyEvent.type = KEYEVENT_CHAR;
		browser_->GetHost()->SendKeyEvent(keyEvent);
	}
}

/**
 * 
 */
int GuiComponent::setContents(std::string contents)
{
	// TODO: implement
	url_ = std::move(contents);

	return 0;
}

int GuiComponent::setContentsUrl(std::string url)
{
	url_ = std::move(url);

	return 0;
}

void GuiComponent::update()
{
	//browser_->GetMainFrame()->ExecuteJavaScript("update();", "about:blank", 0);
}

// TODO: Make this OpenGL 3.2 compliant
void GuiComponent::render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, webTexture_);

	// display
	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 1.f); glVertex3f(-1.f, -1.f, 0.f);
	glTexCoord2f(0.f, 0.f); glVertex3f(-1.f,  1.f, 0.f);
	glTexCoord2f(1.f, 0.f); glVertex3f(1.f,  1.f, 0.f);
	glTexCoord2f(1.f, 1.f); glVertex3f(1.f, -1.f, 0.f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GuiComponent::render(shaders::IShaderProgram& shader)
{
	// TODO: Implement
	//texture_->bind();
	//shader.bindVariableByBindingName( shaders::IShader::BIND_TYPE_TEXTURE, texture_->getBindPoint() );
}

/*
void GuiComponent::onCrashed(Berkelium::Window*win)
{
	//std::cout << "*** onCrashed " << std::endl;
}

void GuiComponent::onUnresponsive(Berkelium::Window*win)
{
	//std::cout << "*** onUnresponsive " << std::endl;
}

void GuiComponent::onScriptAlert(Berkelium::Window*win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool&success, Berkelium::WideString&value)
{
	//std::wcout << L"*** onScriptAlert " << message << std::endl;
}
*/

std::wstring GuiComponent::getObjectName(const std::wstring& name) const
{
	std::string::size_type loc = name.find(L".", 0);

	if ( loc != std::string::npos )
	{
		return name.substr(0, (int)loc);
	}

	return L"";
}

std::wstring GuiComponent::getFunctionName(const std::wstring& name) const
{
	std::string::size_type loc = name.find(L".", 0);

	if ( loc != std::string::npos )
	{
		return name.substr((int)loc + 1);
	}

	return L"";
}

CefRefPtr<CefRenderHandler> GuiComponent::GetRenderHandler()
{
	return renderHandler_;
}

void GuiComponent::executeScript(const std::wstring& script)
{
	browser_->GetMainFrame()->ExecuteJavaScript(script, "about:blank", 0);
}

bool GuiComponent::isVisible() const
{
	return isVisible_;
}

void GuiComponent::setVisible(bool isVisible)
{
	isVisible_ = isVisible;
}

IGuiObject* GuiComponent::createGuiObject(const std::wstring& name)
{
	auto it = guiObjects_.find(name);
	if ( it != guiObjects_.end() )
	{
		std::string msg = std::string("Gui Object with name '") + utilities::toString(name) + std::string("' already exists.");
		LOG_ERROR(msg);
		throw exception::Exception(msg);
	}
	
	auto object = std::unique_ptr<GuiObject>(new GuiObject(name));
	auto objectPointer = object.get();

	guiObjects_[name] = std::move(object);

	return objectPointer;
}

IGuiObject* GuiComponent::getGuiObject(const std::wstring& name) const
{
	auto it = guiObjects_.find(name);
	if ( it == guiObjects_.end() )
	{
		return nullptr;
	}
	
	return it->second.get();
}

void GuiComponent::windowSizeUpdate(glm::detail::uint32 width, glm::detail::uint32 height)
{
	// Is this the right way to cast?  Is there a better way to do this?
	if ( renderHandler_.get() != nullptr )
		((RenderHandler*)renderHandler_.get())->windowSizeUpdate( width, height );
	
	browser_->GetHost()->WasResized();
}

}
}
}

#endif /* USE_CEF */
