/*
 * GuiComponent.cpp
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#ifdef USE_CEF

#include <iostream>
#include <string.h>
#include <cstring>

#include <boost/log/trivial.hpp>

#include "GuiComponent.h"

#include "GuiObject.h"

#include "../../common/utilities/ImageLoader.h"
#include "../../common/utilities/BoostAnyUtilities.h"

#include "../../exceptions/ExceptionInclude.h"

#include "../../../cef_client/src/FunctionList.h"
#include "../../../cef_client/src/ExceptionList.h"

namespace glr {
namespace gui {
namespace cef {

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
	std::wcout << L"GuiComponent OnProcessMessageReceived " << s << std::endl;
	
	// Do we need this?
	// browser == m_Browser
	
	if( s == cef_client::EXECUTE_FUNCTION )
	{	
		std::wstring objName = message->GetArgumentList()->GetString(0);
		std::wstring functionName = message->GetArgumentList()->GetString(1);
		glmd::int32 numArguments = message->GetArgumentList()->GetInt(2);
		
		std::wcout << L"GuiComponent ExecuteFunction " << objName << "." << functionName << " " << numArguments << std::endl;
		
		std::vector< boost::any > params = std::vector< boost::any >();
		
		// Wrap parameters
		for (glmd::int32 i = 0; i < numArguments; i++)
		{
			CefValueType type = message->GetArgumentList()->GetType( i+2 );
			switch (type)
			{
				case VTYPE_INT:
				{
					glmd::int32 arg = message->GetArgumentList()->GetInt(i+2);
					params.push_back(boost::any(arg));
					std::cout << "VTYPE_INT - " << arg << std::endl;
				}
					break;
				
				case VTYPE_STRING:
				{
					std::string arg = message->GetArgumentList()->GetString(i+2);
					params.push_back(boost::any(arg));
					std::cout << "VTYPE_STRING - " << arg << std::endl;
				}
					break;

				case VTYPE_BOOL:
				{
					bool arg = message->GetArgumentList()->GetBool(i+2);
					params.push_back(boost::any(arg));
					std::cout << "VTYPE_BOOL - " << arg << std::endl;
				}
					break;
				
				case VTYPE_DOUBLE:
				{
					glmd::float64 arg = message->GetArgumentList()->GetDouble(i+2);
					params.push_back(boost::any(arg));
					std::cout << "VTYPE_DOUBLE - " << arg << std::endl;
				}
					break;
				
				case VTYPE_BINARY:
				{
					std::string msg = "Error - VTYPE_BINARY not implemented as CEF3 argument type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
					
				case VTYPE_DICTIONARY:
				{
					std::string msg = "Error - VTYPE_DICTIONARY not implemented as CEF3 argument type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
				
				case VTYPE_LIST:
				{
					std::string msg = "Error - VTYPE_LIST not implemented as CEF3 argument type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
				
				case VTYPE_INVALID:
				{
					std::string msg = "Error - VTYPE_INVALID not implemented as CEF3 argument type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
					
				case VTYPE_NULL:
				{
					std::string msg = "Error - VTYPE_NULL not implemented as CEF3 argument type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
				
				// Should the default case throw an exception?
				default:
				{
					std::string msg = "Error - Unknown CEF3 argument type:" + type;					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
				}
					break;
			}
		}
		
		if ( guiObjects_.find(objName) != guiObjects_.end() )
		{
			boost::any r = guiObjects_[objName]->processCallback(functionName, params);
			std::string messageId = "Message_" + std::to_string(numMessagesSent_);
			messageIdMap_[messageId] = numMessagesSent_;
			numMessagesSent_++;
			
			CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( cef_client::FUNCTION_RESULT );
			m->GetArgumentList()->SetString( 0, messageId );
			m->GetArgumentList()->SetString( 1, functionName );
			m->GetArgumentList()->SetInt( 2, 1 );
			
			if( r.empty() )
			{
				// There is no return value (i.e. void), so set num return results 0
				m->GetArgumentList()->SetInt( 2, 0 );
				std::cout << "ExecuteFunction RESULT: void" << std::endl;
			}
			else if ( utilities::isString(r) )
			{
				m->GetArgumentList()->SetString( 3, boost::any_cast<std::string>(r) );
				std::cout << "ExecuteFunction RESULT:" << boost::any_cast<std::string>(r) << std::endl;
			}
			else if ( utilities::isWstring(r) )
			{
				m->GetArgumentList()->SetString( 3, boost::any_cast<std::wstring>(r) );
				std::wcout << L"ExecuteFunction RESULT:" << boost::any_cast<std::wstring>(r) << std::endl;
			}
			else if ( utilities::isInt(r) )
			{
				m->GetArgumentList()->SetInt( 3, boost::any_cast<int>(r) );
				std::cout << "ExecuteFunction RESULT:" << boost::any_cast<int>(r) << std::endl;
			}
			else if ( utilities::isUint(r) )
			{
				m->GetArgumentList()->SetInt( 3, boost::any_cast<uint>(r) );
				std::cout << "ExecuteFunction RESULT:" << boost::any_cast<uint>(r) << std::endl;
			}
			else if ( utilities::isFloat(r) )
			{
				m->GetArgumentList()->SetDouble( 3, boost::any_cast<float>(r) );
				std::cout << "ExecuteFunction RESULT:" << boost::any_cast<float>(r) << std::endl;
			}
			else if ( utilities::isDouble(r) )
			{
				m->GetArgumentList()->SetDouble( 3, boost::any_cast<double>(r) );
				std::cout << "ExecuteFunction RESULT:" << boost::any_cast<double>(r) << std::endl;
			}
			else
			{
				std::cout << "ExecuteFunction Unable to determine result type." << std::endl;
				std::string msg = "Error - ExecuteFunction unable to determine result type.";					
				std::cout << msg << std::endl;
				throw exception::Exception( msg );
			}
			
			browser->SendProcessMessage(PID_RENDERER, m);
		}
	}
	else if( s == cef_client::READY_FOR_BINDINGS && !bindDataSent_ )
	{ 
		glmd::uint32 numSent = sendBoundFunctionsToRenderProcess();
		std::string messageId = "Message_" + std::to_string(numMessagesSent_);
		messageIdMap_[messageId] = numMessagesSent_;
		numMessagesSent_++;
		
		CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( cef_client::ALL_BINDINGS_SENT );
		m->GetArgumentList()->SetString( 0, messageId );
		m->GetArgumentList()->SetInt( 1, numSent );
		browser->SendProcessMessage(PID_RENDERER, m);
		std::cout << "ReadyForBindings finished with " << numSent << " function(s) sent to the render process." << std::endl;
		bindDataSent_ = true;
	}
	else if( s == cef_client::ALL_BINDINGS_RECEIVED && bindDataSent_ )
	{
		// Now that bindings are set, we can load the url
		browser_->GetMainFrame()->LoadURL(url_);
	}
	else if( s == cef_client::ALL_BINDINGS_RECEIVED && !bindDataSent_ )
	{ 
		std::string msg = "AllBindingsReceived message processed, but no binding data was sent.";
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception( msg );
	}
	else if( s == cef_client::SUCCESS )
	{ 
		// TODO: deal with success
		std::string messageId = message->GetArgumentList()->GetString( 0 );
		auto it = messageIdMap_.find(messageId);
		if (it == messageIdMap_.end())
		{
			std::string msg = "Success message id '" + messageId + "' does not match any ids of sent messages.";
			BOOST_LOG_TRIVIAL(error) << msg;
			throw exception::Exception( msg );
		}
		
		messageIdMap_.erase(it);
		//BOOST_LOG_TRIVIAL(warning) << "Success message not yet implemented.";
		std::cout << "GuiComponent Success: " << messageId << std::endl;
	}
	else if( s == cef_client::EXCEPTION )
	{ 
		//std::string messageId = message->GetArgumentList()->GetString( 0 );
		cef_client::Exception e = (cef_client::Exception) message->GetArgumentList()->GetInt( 1 );
		std::string errorMessage = message->GetArgumentList()->GetString( 2 );
		// c++ exception doesn't seem to support wchar...
		//std::wstring message = message->GetArgumentList()->GetWString( 2 );		
		
		std::stringstream msgStream;
		msgStream << errorMessage << " - Exception type: " << e;
		
		std::string msg = msgStream.str();
		
		BOOST_LOG_TRIVIAL(error) << "Exception message received: " << msg;
		std::cout << "Exception message received." << msg << std::endl;
		throw exception::Exception( msg );
	}
	
	return true;
}

void GuiComponent::load()
{
	BOOST_LOG_TRIVIAL(debug) << "Creating GuiComponent texture.";
	
	// Create texture to hold rendered view
	glGenTextures(1, &webTexture);
	glBindTexture(GL_TEXTURE_2D, webTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	glw::GlError err = openGlDevice_->getGlError();
	if (err.type != GL_NONE)
	{
		std::stringstream msgStream;
		msgStream << "Error loading GuiComponent in opengl: " << err.name;
		std::string msg = msgStream.str();
		
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::GlException( msg );
	}
	else
	{
		BOOST_LOG_TRIVIAL(debug) << "Successfully created GuiComponent texture.";
	}

	BOOST_LOG_TRIVIAL(debug) << "Creating GuiComponent CEF components.";
	
	// Create Cef processes
	CefMainArgs args;
	CefSettings settings;

	CefString(&settings.browser_subprocess_path).FromASCII("./cef3_client");
	BOOST_LOG_TRIVIAL(debug) << "Initializing CEF.";
	bool result = CefInitialize(args, settings, nullptr);
	
	// CefInitialize creates a sub-proccess and executes the same executeable, as calling CefInitialize, if not set different in settings.browser_subprocess_path
	// if you create an extra program just for the childproccess you only have to call CefExecuteProcess(...) in it.
	if (!result)
	{
		std::string msg = "Error loading GuiComponent - could not initialize CEF.";
		
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception( msg );		
	}

	CefWindowInfo window_info;
	CefBrowserSettings browserSettings;

	// in linux set a gtk widget, in windows a hwnd. If not available set nullptr - may cause some render errors, in context-menu and plugins.
	window_info.SetAsOffScreen(nullptr);
	window_info.SetTransparentPainting(true);
	
	this->renderHandler_ = new RenderHandler(webTexture);
	
	BOOST_LOG_TRIVIAL(debug) << "Creating CEF Browser object.";
	// Create initially with "/" appended to url to make sure CEF DOESN'T fully load the page.
	// We just want the render process created at this point so that we can do our bindings.
	browser_ = CefBrowserHost::CreateBrowserSync(window_info, this, url_+"/", browserSettings);
	
	if (browser_.get() == nullptr)
	{
		// The render process should clean up on its own.
		std::string msg = "Error loading GuiComponent - could not create CEF browser.";
		
		BOOST_LOG_TRIVIAL(error) << msg;
		throw exception::Exception( msg );
	}
	
	//browser_->GetMainFrame()->LoadURL(std::string("http://www.jarrettchisholm.com").c_str());
	
	// inject user-input by calling
	// browser_->GetHost()->SendKeyEvent(...);
	// browser_->GetHost()->SendMouseMoveEvent(...);
	// browser_->GetHost()->SendMouseClickEvent(...);
	// browser_->GetHost()->SendMouseWheelEvent(...);
    
    
    //sendBoundFunctionsToRenderProcess();
    
    BOOST_LOG_TRIVIAL(debug) << "Successfully created GuiComponent CEF components.";
}

void GuiComponent::unload()
{
	// TODO: properly dispose of browser
	browser_->GetHost()->CloseBrowser( false );
	browser_ = nullptr;
        
	this->setVisible(false);
}

/** Maps an input coordinate to a texture coordinate for injection into
 *  Berkelium.
 *  \param glut_size the size of the GLUT window
 *  \param glut_coord the coordinate value received from GLUT
 *  \param tex_size the size of the texture/Berkelium window
 *  \returns the coordinate transformed to the correct value for the texture /
 *           Berkelium window
 */
/*
unsigned int GuiComponent::mapGLUTCoordToTexCoord(unsigned int glut_coord, unsigned int glut_size, unsigned int tex_size)
{
    return (glut_coord * tex_size) / glut_size;
}
*/

void GuiComponent::mouseMoved(glm::detail::int32 xPos, glm::detail::int32 yPos)
{
	//std::cout << "MOUSE MOVED EVENT: " << xPos << " " << yPos << std::endl;
	
	//unsigned int tex_coord_x = mapGLUTCoordToTexCoord(xPos, width_, width_);
    //unsigned int tex_coord_y = mapGLUTCoordToTexCoord(yPos, height_, height_);
    //std::cout << xPos << " " << yPos << " : " << tex_coord_x << " " << tex_coord_y << std::endl;
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
	std::cout << "MOUSE BUTTON EVENT: " << buttonId << " " << xPos << " " << yPos << " " << down << std::endl;
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
			BOOST_LOG_TRIVIAL(warning) << "Unable to determine mouse button: " << buttonId;
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
	this->mouseButton(buttonId, xPos, yPos, true, 1);
	this->mouseButton(buttonId, xPos, yPos, false, 1);
}

void GuiComponent::mouseWheel(glm::detail::int32 xScroll, glm::detail::int32 yScroll)
{
	
}

void GuiComponent::textEvent(const wchar_t* evt, size_t evtLength)
{
	std::cout << "TEXT EVENT: " << evt << std::endl;
	std::cout << "HERE 1 " << (char*)evt << std::endl;
	//window_->focus();
	wchar_t outchars[2];
	outchars[0] = evt[0];
	outchars[1] = 0;
	std::cout << "HERE 2 " << outchars[0] << std::endl;
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
		messageIdMap_[messageId] = numMessagesSent_;
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
			messageIdMap_[messageId] = numMessagesSent_;
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

void GuiComponent::keyEvent(bool pressed, glm::detail::int32 mods, glm::detail::int32 vk_code, glm::detail::int32 scancode)
{
	std::cout << "KEY EVENT: (" << pressed << ") " << (char)vk_code << std::endl;

	CefKeyEvent keyEvent;
	keyEvent.native_key_code = vk_code;
	keyEvent.modifiers = getCefStateModifiers(mods);

	/*
	if (event->type == GDK_KEY_PRESS) {
		keyEvent.type = KEYEVENT_RAWKEYDOWN;
		host->SendKeyEvent(keyEvent);
	} else */
	{
		// Need to send both KEYUP and CHAR events.
	/*
		keyEvent.type = KEYEVENT_KEYUP;
		host->SendKeyEvent(keyEvent);
	*/
		std::cout << "HERE 2 " << vk_code << std::endl;
		keyEvent.type = KEYEVENT_CHAR;
		browser_->GetHost()->SendKeyEvent(keyEvent);
	}
	
	
	// TODO: make this not hardcoded
	if ( vk_code == '`' || vk_code == '~' )
	{
		browser_->GetMainFrame()->ExecuteJavaScript(
										L"if( $('#console').hasClass('hidden') ) {\
											$('#console').removeClass('hidden');\
											$('#console').addClass('visible');\
											$('#console').click();\
										} else {\
											$('#console').addClass('hidden');\
											$('#console').removeClass('visible');\
										}",
										"about:blank", 
										0
									);
	}
	//else
	
	//{
		//window_->focus();
		//wchar_t outchars[2];
		//outchars[0] = vk_code;
		//outchars[1] = 0;
		//std::cout << "HERE 2 " << outchars[0] << std::endl;
		//window_->textEvent(outchars, 1);
		//window_->keyEvent(pressed, mods, vk_code, scancode);
	//}
}

/**
 * 
 */
int GuiComponent::setContents(std::string contents)
{
	// TODO: implement
	url_ = contents;

	return 0;
}

int GuiComponent::setContentsUrl(std::string url)
{
	url_ = url;

	return 0;
}

void GuiComponent::update()
{
	//browser_->GetMainFrame()->ExecuteJavaScript("update();", "about:blank", 0);
}


void GuiComponent::render(shaders::IShaderProgram* shader)
{
	//texture_->bind();
	//shader->bindVariableByBindingName( shaders::IShader::BIND_TYPE_TEXTURE, texture_->getBindPoint() );
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, webTexture);

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

/*
void GuiComponent::onCrashed(Berkelium::Window*win)
{
	std::cout << "*** onCrashed " << std::endl;
}

void GuiComponent::onUnresponsive(Berkelium::Window*win)
{
	std::cout << "*** onUnresponsive " << std::endl;
}

void GuiComponent::onScriptAlert(Berkelium::Window*win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool&success, Berkelium::WideString&value)
{
	std::wcout << L"*** onScriptAlert " << message << std::endl;
}
*/

std::wstring GuiComponent::getObjectName(std::wstring name)
{
	//return L"testObj";
	std::string::size_type loc = name.find(L".", 0);

	if ( loc != std::string::npos )
	{
		return name.substr(0, (int)loc);
	}

	return L"";
}

std::wstring GuiComponent::getFunctionName(std::wstring name)
{
	//return L"testFunc";
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

/*
void GuiComponent::onJavascriptCallback(Berkelium::Window*win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, boost::any*args, size_t numArgs)
{
	   std::cout << "*** onJavascriptCallback at URL " << url << ", "
	                  << (replyMsg?"synchronous":"async") << std::endl;
	   std::wcout << L"    Function name: " << funcName << std::endl;

	   for (size_t i = 0; i < numArgs; i++) {
	        Berkelium::WideString jsonStr = toJSON(args[i]);
	        std::wcout << L"    Argument " << i << ": ";
	        if (args[i].type() == boost::any::JSSTRING) {
	                std::wcout << L"(string) " << args[i].toString() << std::endl;
	        } else {
	                std::wcout << jsonStr << std::endl;
	        }
	        Berkelium::Script::toJSON_free(jsonStr);
	   }

	// parse object and function names
	const std::wstring berkeliumFuncName = std::wstring(funcName.data(), funcName.length());

	const std::wstring objName = getObjectName(berkeliumFuncName);
	const std::wstring functionName = getFunctionName(berkeliumFuncName);

	//std::wcout << objName << " " << functionName << std::endl;


	std::vector< CallbackParameter > params;

	// parse parameters
	for ( size_t i = 0; i < numArgs; i++ )
	{
		switch ( args[i].type())
		{
		case boost::any::JSSTRING: {
			std::wstring p = std::wstring(args[i].toString().data(), args[i].toString().length());
			params.push_back(CallbackParameter(p));
		}
		break;

		case boost::any::JSDOUBLE: {
			double p = args[i].toDouble();
			params.push_back(CallbackParameter(p));
		}
		break;

		case boost::any::JSBOOLEAN: {
			bool p = args[i].toBoolean();
			params.push_back(CallbackParameter(p));
		}
		break;

		case boost::any::JSNULL:

			break;

		default:
			// int i = args[i].toInteger();
			break;
		}
	}


	if ( guiObjects_[objName] != nullptr )
	{
		boost::any r = guiObjects_[objName]->processCallback(functionName, params);

		if ( replyMsg )
			win->synchronousScriptReturn(replyMsg, r);
	}
}
*/

/*
bool GuiComponent::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
	rect = CefRect(0, 0, 800, 600);
	return true;
}

void GuiComponent::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
{

	memcpy(texBuf->getCurrentLock().data, buffer, width*height*4);

}
*/

/*
void GuiComponent::onPaint(
		Berkelium::Window* wini,
		const unsigned char*bitmap_in, 
		const Berkelium::Rect&bitmap_rect,
		size_t num_copy_rects, 
		const Berkelium::Rect* copy_rects,
		int dx, 
		int dy, 
		const Berkelium::Rect& scroll_rect
	)
{
	bool updated = mapOnPaintToTexture(
		wini, 
		bitmap_in, 
		bitmap_rect, 
		num_copy_rects, 
		copy_rects,
		dx,
		dy, 
		scroll_rect,
		webTexture, 
		width_, 
		height_, 
		needs_full_refresh, 
		scroll_buffer
	);


	if ( updated )
	{
		needs_full_refresh = false;
		webTextureReady_ = true;
	}
}
*/

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
/*
bool GuiComponent::mapOnPaintToTexture(
		Berkelium::Window*wini,
		const unsigned char* bitmap_in, 
		const Berkelium::Rect& bitmap_rect,
		size_t num_copy_rects, 
		const Berkelium::Rect*copy_rects,
		int dx, 
		int dy,
		const Berkelium::Rect& scroll_rect,
		unsigned int dest_texture,
		unsigned int dest_texture_width,
		unsigned int dest_texture_height,
		bool ignore_partial,
		char* scroll_buffer
	)
{
	//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: " << dest_texture_width << "x" << dest_texture_width << " dest_texture: " << dest_texture;

	glBindTexture(GL_TEXTURE_2D, dest_texture);

	const int kBytesPerPixel = 4;

	// If we've reloaded the page and need a full update, ignore updates
	// until a full one comes in.  This handles out of date updates due to
	// delays in event processing.
	if ( ignore_partial )
	{
		if ( bitmap_rect.left() != 0 ||
			 bitmap_rect.top() != 0 ||
			 bitmap_rect.right() != dest_texture_width ||
			 bitmap_rect.bottom() != dest_texture_height )
		{
			return false;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, kBytesPerPixel, dest_texture_width, dest_texture_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bitmap_in);
		ignore_partial = false;
		return true;
	}


	// Now, we first handle scrolling. We need to do this first since it
	// requires shifting existing data, some of which will be overwritten by
	// the regular dirty rect update.
	if ( dx != 0 || dy != 0 )
	{
		// scroll_rect contains the Rect we need to move
		// First we figure out where the the data is moved to by translating it
		Berkelium::Rect scrolled_rect = scroll_rect.translate(-dx, -dy);
		// Next we figure out where they intersect, giving the scrolled
		// region
		Berkelium::Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);
		// Only do scrolling if they have non-zero intersection
		if ( scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0 )
		{
			// And the scroll is performed by moving shared_rect by (dx,dy)
			Berkelium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);

			int wid = scrolled_shared_rect.width();
			int hig = scrolled_shared_rect.height();

			if ( DEBUG_PAINT )
			{
				std::cout << "Scroll rect: w=" << wid << ", h=" << hig << ", ("
						  << scrolled_shared_rect.left() << "," << scrolled_shared_rect.top()
						  << ") by (" << dx << "," << dy << ")" << std::endl;
			}

			int inc = 1;
			char*outputBuffer = scroll_buffer;
			// source data is offset by 1 line to prevent memcpy aliasing
			// In this case, it can happen if dy==0 and dx!=0.
			char*inputBuffer = scroll_buffer + (dest_texture_width * 1 * kBytesPerPixel);
			int jj = 0;
			if ( dy > 0 )
			{
				// Here, we need to shift the buffer around so that we start in the
				// extra row at the end, and then copy in reverse so that we
				// don't clobber source data before copying it.
				outputBuffer = scroll_buffer + (
					(scrolled_shared_rect.top() + hig + 1) * dest_texture_width
					- hig * wid) * kBytesPerPixel;
				inputBuffer = scroll_buffer;
				inc = -1;
				jj = hig - 1;
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
			for (; jj < hig && jj >= 0; jj += inc )
			{
				memcpy(
					outputBuffer + (jj * wid) * kBytesPerPixel,
				    //scroll_buffer + (jj*wid * kBytesPerPixel),
					inputBuffer + (
						(scrolled_shared_rect.top() + jj) * dest_texture_width
						+ scrolled_shared_rect.left()) * kBytesPerPixel,
					wid * kBytesPerPixel
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

	//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 2";
	if ( DEBUG_PAINT )
	{
		std::cout << (void*)wini << " Bitmap rect: w="
				  << bitmap_rect.width() << ", h=" << bitmap_rect.height()
				  << ", (" << bitmap_rect.top() << "," << bitmap_rect.left()
				  << ") tex size " << dest_texture_width << "x" << dest_texture_height
				  << std::endl;
	}

	//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 3";
	for ( size_t i = 0; i < num_copy_rects; i++ )
	{
		int wid = copy_rects[i].width();
		int hig = copy_rects[i].height();
		int top = copy_rects[i].top() - bitmap_rect.top();
		int left = copy_rects[i].left() - bitmap_rect.left();
		//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 4";

		if ( DEBUG_PAINT )
		{
			std::cout << (void*)wini << " Copy rect: w=" << wid << ", h=" << hig << ", ("
					  << top << "," << left << ")" << std::endl;
		}
		//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 5";
		for ( int jj = 0; jj < hig; jj++ )
		{
			memcpy(
				scroll_buffer + jj * wid * kBytesPerPixel,
				bitmap_in + (left + (jj + top) * bitmap_rect.width()) * kBytesPerPixel,
				wid * kBytesPerPixel
				);
		}
		//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 6";
		// Finally, we perform the main update, just copying the rect that is
		// marked as dirty but not from scrolled data.
		glTexSubImage2D(GL_TEXTURE_2D, 0,
						copy_rects[i].left(), copy_rects[i].top(),
						wid, hig,
						GL_BGRA, GL_UNSIGNED_BYTE, scroll_buffer
						);
		//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 7";
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	//BOOST_LOG_TRIVIAL(debug) << "mapOnPaintToTexture: here 8";

	return true;
}
*/

void GuiComponent::executeScript(std::wstring script)
{
	browser_->GetMainFrame()->ExecuteJavaScript(script, "about:blank", 0);
}

bool GuiComponent::isVisible()
{
	return isVisible_;
}

void GuiComponent::setVisible(bool isVisible)
{
	isVisible_ = isVisible;
}

IGuiObject* GuiComponent::createGuiObject(std::wstring name)
{
	if ( guiObjects_[name] != nullptr )
	{
		return nullptr;
	}
	
	guiObjects_[name] = std::unique_ptr<GuiObject>(new GuiObject(name));

	return guiObjects_[name].get();
}

IGuiObject* GuiComponent::getGuiObject(std::wstring name)
{
	if ( guiObjects_[name] == nullptr )
	{
		return nullptr;
	}

	return guiObjects_[name].get();
}

}
}
}

#endif
