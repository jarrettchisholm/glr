#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif

#include <chrono>

#include <boost/test/unit_test.hpp>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>

#include <boost/any.hpp>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "../../cef_client/include/FunctionList.hpp"
#include "../../cef_client/include/ExceptionList.hpp"

#include "../../src/common/utilities/StringUtilities.hpp"

bool isEmpty(const boost::any& operand)
{
    return operand.empty();
}

bool isInt(const boost::any& operand)
{
    return operand.type() == typeid(int);
}

bool isUint(const boost::any& operand)
{
    return operand.type() == typeid(glm::detail::uint32);
}

bool isFloat(const boost::any& operand)
{
    return operand.type() == typeid(float);
}

bool isDouble(const boost::any& operand)
{
    return operand.type() == typeid(double);
}

bool isBool(const boost::any& operand)
{
    return operand.type() == typeid(bool);
}

bool isString(const boost::any& operand)
{
    return boost::any_cast<std::string>(&operand);
}

bool isWstring(const boost::any& operand)
{
    return boost::any_cast<std::wstring>(&operand);
}

struct GuiObject
{
	std::wstring name;
	std::vector<std::wstring> functions;
	std::vector< std::pair<std::wstring, boost::any> > attributes;
	
};

std::map< std::string, glm::detail::uint32 > messageIdMap;
std::vector< std::shared_ptr<GuiObject> > guiObjects;

void setupGuiObjects()
{
	auto obj = std::shared_ptr<GuiObject>(new GuiObject());
	obj->name = L"test1";
	obj->functions.push_back(L"func1");
	obj->functions.push_back(L"func2");
	obj->functions.push_back(L"func3");
	obj->attributes.push_back( std::pair<std::wstring, boost::any>( L"attr1", 5 ));
	obj->attributes.push_back( std::pair<std::wstring, boost::any>( L"attr2", std::string("attribute 2 value") ));
	obj->attributes.push_back( std::pair<std::wstring, boost::any>( L"attr3", 15.2 ));
	obj->attributes.push_back( std::pair<std::wstring, boost::any>( L"attr4", true ));
	guiObjects.push_back(obj);
	
	obj = std::shared_ptr<GuiObject>(new GuiObject());
	obj->name = L"test2";
	obj->functions.push_back(L"func4");
	guiObjects.push_back(obj);
}

glm::detail::uint32 sendTestBindings(CefRefPtr<CefBrowser> browser)
{
	glm::detail::uint32 numSent = 0;
	
	for ( auto& it : guiObjects )
	{
		std::string messageId = utilities::toString( it->name );
		messageIdMap[messageId] = numSent;
		
		std::wcout << L"obj name: " << it->name << std::endl;
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("AddObject");
		message->GetArgumentList()->SetString( 0, messageId );
		message->GetArgumentList()->SetString( 1, it->name );
		browser->SendProcessMessage(PID_RENDERER, message);
		numSent++;
		
		std::vector< std::wstring > names = it->functions;
		
		message = CefProcessMessage::Create("AddMethodToObject");
		for ( auto& name : names )
		{
			messageId = utilities::toString( it->name + name );
			messageIdMap[messageId] = numSent;
			
			std::wcout << L"func name: " << name << std::endl;
			message->GetArgumentList()->SetString( 0, messageId );
			message->GetArgumentList()->SetString( 1, it->name );
			message->GetArgumentList()->SetString( 2, name );
			browser->SendProcessMessage(PID_RENDERER, message);
			numSent++;
		}
		
		std::vector< std::pair<std::wstring, boost::any> > attributes = it->attributes;
		
		message = CefProcessMessage::Create("AddAttributeToObject");
		for ( auto& attr : attributes )
		{
			messageId = utilities::toString( it->name + attr.first );
			messageIdMap[messageId] = numSent;
			
			std::wcout << L"attr name: " << attr.first << std::endl;
			message->GetArgumentList()->SetString( 0, messageId );
			message->GetArgumentList()->SetString( 1, it->name );
			message->GetArgumentList()->SetString( 2, attr.first );
			
			boost::any r = attr.second;
			if ( isString(r) )
			{
				message->GetArgumentList()->SetString( 3, boost::any_cast<std::string>(r) );
			}
			else if ( isWstring(r) )
			{
				message->GetArgumentList()->SetString( 3, boost::any_cast<std::wstring>(r) );
			}
			else if ( isInt(r) )
			{
				message->GetArgumentList()->SetInt( 3, boost::any_cast<int>(r) );
			}
			else if ( isUint(r) )
			{
				message->GetArgumentList()->SetInt( 3, boost::any_cast<glm::detail::uint32>(r) );
			}
			else if ( isFloat(r) )
			{
				message->GetArgumentList()->SetDouble( 3, boost::any_cast<float>(r) );
			}
			else if ( isDouble(r) )
			{
				message->GetArgumentList()->SetDouble( 3, boost::any_cast<double>(r) );
			}
			else if ( isBool(r) )
			{
				message->GetArgumentList()->SetBool( 3, boost::any_cast<bool>(r) );
			}
			
			browser->SendProcessMessage(PID_RENDERER, message);
			numSent++;
		}
	}
	
	return numSent;
}

class RenderHandler : public CefRenderHandler
{
public:
	RenderHandler()
	{
	}

	bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
	{
		rect = CefRect(0, 0, 800, 600);
		return true;
	};
	
    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void *buffer, int width, int height)
    {
	};
	
	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(RenderHandler)
};

class Client : public CefClient
{
public:
	Client()
	{
		renderHandler_ = new RenderHandler();
		numSent_ = 0;
		allBindingsReceived_ = false;
	}
	
	CefRefPtr<CefRenderHandler> GetRenderHandler()
	{
		return renderHandler_;
	}
	
	bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message )
	{
		
		std::wstring s = message->GetName().ToWString();
		std::wcout << L"CefClientTests OnProcessMessageReceived " << s << std::endl;
		/*	
		if( s == "ExecuteFunction" )
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
				
				CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create("FunctionResult");
				message->GetArgumentList()->SetString( 0, functionName );
				message->GetArgumentList()->SetInt( 1, 1 );
				
				if( r.empty() )
				{
					// There is no return value (i.e. void), so set num return results 0
					message->GetArgumentList()->SetInt( 1, 0 );
					std::cout << "ExecuteFunction RESULT: void" << std::endl;
				}
				else if ( utilities::isString(r) )
				{
					message->GetArgumentList()->SetString( 2, boost::any_cast<std::string>(r) );
					std::cout << "ExecuteFunction RESULT:" << boost::any_cast<std::string>(r) << std::endl;
				}
				else if ( utilities::isWstring(r) )
				{
					message->GetArgumentList()->SetString( 2, boost::any_cast<std::wstring>(r) );
					std::wcout << L"ExecuteFunction RESULT:" << boost::any_cast<std::wstring>(r) << std::endl;
				}
				else if ( utilities::isInt(r) )
				{
					message->GetArgumentList()->SetInt( 2, boost::any_cast<int>(r) );
					std::cout << "ExecuteFunction RESULT:" << boost::any_cast<int>(r) << std::endl;
				}
				else if ( utilities::isUint(r) )
				{
					message->GetArgumentList()->SetInt( 2, boost::any_cast<glm::detail::uint32>(r) );
					std::cout << "ExecuteFunction RESULT:" << boost::any_cast<glm::detail::uint32>(r) << std::endl;
				}
				else if ( utilities::isFloat(r) )
				{
					message->GetArgumentList()->SetDouble( 2, boost::any_cast<float>(r) );
					std::cout << "ExecuteFunction RESULT:" << boost::any_cast<float>(r) << std::endl;
				}
				else if ( utilities::isDouble(r) )
				{
					message->GetArgumentList()->SetDouble( 2, boost::any_cast<double>(r) );
					std::cout << "ExecuteFunction RESULT:" << boost::any_cast<double>(r) << std::endl;
				}
				else
				{
					std::cout << "ExecuteFunction Unable to determine result type." << std::endl;
					std::string msg = "Error - ExecuteFunction unable to determine result type.";					
					std::cout << msg << std::endl;
					throw exception::Exception( msg );
					// No result
					//message->GetArgumentList()->SetInt( 1, 0 );
				}
				
				browser->SendProcessMessage(PID_RENDERER, m);
			}
		}
		*/
		if( s == glr::cef_client::READY_FOR_BINDINGS )
		{ 
			numSent_ = sendTestBindings(browser);
			
			CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( glr::cef_client::ALL_BINDINGS_SENT );
			m->GetArgumentList()->SetString( 0, "TESTING" );
			m->GetArgumentList()->SetInt( 1, numSent_ );
			browser->SendProcessMessage(PID_RENDERER, m);
			
			std::cout << "ReadyForBindings finished with " << numSent_ << " messages(s) sent to the render process." << std::endl;
		}
		else if( s == glr::cef_client::ALL_BINDINGS_RECEIVED )
		{ 
			//browser_->GetMainFrame()->LoadURL(url_);
			allBindingsReceived_ = true;
		}
		else if( s == glr::cef_client::SUCCESS )
		{
			// TODO: deal with success
			std::string messageId = message->GetArgumentList()->GetString( 0 );

			auto it = messageIdMap.find(messageId);
			if (it == messageIdMap.end())
			{
				std::string msg = "Success message id '" + messageId + "' does not match any ids of sent messages.";
				BOOST_FAIL( msg );
			}
			
			messageIdMap.erase(it);
			std::cout << "GuiComponent Success: " << messageId << std::endl;
		}
		else if( s == glr::cef_client::EXCEPTION )
		{ 
			//browser_->GetMainFrame()->LoadURL(url_);
			glr::cef_client::Exception e = (glr::cef_client::Exception) message->GetArgumentList()->GetInt( 1 );
			std::string errorMessage = message->GetArgumentList()->GetString( 2 );
			// c++ exception doesn't seem to support wchar...
			//std::wstring message = message->GetArgumentList()->GetWString( 2 );
			
			std::stringstream msgStream;
			msgStream << errorMessage << " - Exception type: " << e;
			
			std::string msg = msgStream.str();
			
			BOOST_FAIL( msg );
		}
		
		return true;
	}
	
	bool isAllBindingsReceived()
	{
		return allBindingsReceived_ && messageIdMap.empty();
	}
	
	bool isAllBindingsSuccessful()
	{
		return messageIdMap.empty();
	}

private:
	CefRefPtr<CefRenderHandler> renderHandler_;
	glm::detail::uint32 numSent_;
	
	bool allBindingsReceived_;

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(Client)
};

bool createCefClientTest()
{	
	CefMainArgs args;
	CefSettings settings;

	CefString(&settings.browser_subprocess_path).FromASCII("./cef3_client");
	return CefInitialize(args, settings, nullptr);
}

struct data
{
	// We don't have to worry about deleting client, as CEF does that for us
	Client* client;
	CefRefPtr<CefBrowser> browser;
};

data createCefBrowserTest()
{
	data d = data();
	CefWindowInfo windowInfo;
	CefBrowserSettings browserSettings;

	// in linux set a gtk widget, in windows a hwnd. If not available set nullptr - may cause some render errors, in context-menu and plugins.
	windowInfo.SetAsOffScreen(nullptr);
	windowInfo.SetTransparentPainting(true);
	
	d.client = new Client();
	
	d.browser = CefBrowserHost::CreateBrowserSync(windowInfo, d.client, std::string("http://www.google.com"), browserSettings, nullptr);
	
	return d;
}

BOOST_AUTO_TEST_SUITE(cefClient)

BOOST_AUTO_TEST_CASE(createCefClient)
{
	setupGuiObjects();
	
	if ( !createCefClientTest() )
		BOOST_FAIL( "Could not initialize CEF client." );
	
	data d = createCefBrowserTest();
	if (d.browser.get() == nullptr)
		BOOST_FAIL( "Could not create CEF browser." );
	
	
	bool doWork = true;
	auto start = std::chrono::system_clock::now();
	while ( doWork )
	{
		CefDoMessageLoopWork();
		
		// Allow cef_client time to process the messages
		auto end = std::chrono::system_clock::now();
		doWork = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() < 5;
	}
	
	if ( !d.client->isAllBindingsReceived() )
		BOOST_FAIL( "Not all bindings were received by the cef_client." );
	if ( !d.client->isAllBindingsSuccessful() )
		BOOST_FAIL( "Not all bindings were confirmed successful by the cef_client...it's possible they may have taken too long?" );
	
	// Properly dispose of the browser
	d.browser->GetHost()->CloseBrowser( false );
	
	CefShutdown();
}

// TODO: maybe make an html page to actually call some of the bound functions...test the back and forth.

BOOST_AUTO_TEST_SUITE_END()
