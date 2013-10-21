#include <iostream>
#include <algorithm>
#include <mutex>

#include <cef_app.h>

class ClientApp : public CefApp, public CefRenderProcessHandler, public CefV8Handler
{
public:
    ClientApp()
    {
		std::cout << "cef3_client here woo" << std::endl;
		totalFunctionCount_ = 0;
		allBindingsSentMessageReceived_ = false;
		readyForBindingsMessageSent_ = false;
	};
	
	/**
	 * Implement CEF3's OnContextCreated method in order to add callable native functions to javascript.
	 */
	void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{		
		std::cout << "cef3_client OnContextCreated has been called!" << std::endl;
		
		if ( allBindingsSentMessageReceived_ )
		{
			CefRefPtr<CefV8Handler> handler = this;
			
			// Register our functions
			functionListMutex_.lock();
			for ( auto &f : functionList_ )
			{
				CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(f, handler);
				context->GetGlobal()->SetValue(f, func, V8_PROPERTY_ATTRIBUTE_NONE);
			}
			functionListMutex_.unlock();
			
			// Register the standard callback function
			std::string callbackFunction = "setMessageCallback";
			CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(callbackFunction, handler);
			context->GetGlobal()->SetValue(callbackFunction, func, V8_PROPERTY_ATTRIBUTE_NONE);
		}
		
		if ( !readyForBindingsMessageSent_ )
		{
			// Indicate that the render process is ready to receive function binding data
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ReadyForBindings");
			//message->GetArgumentList()->SetInt(0, browser->GetIdentifier() );
			//message->GetArgumentList()->SetInt(1, frame->GetIdentifier() );
			browser->SendProcessMessage(PID_BROWSER, message);
		}
	};
	
	void OnContextInitialized()
	{
		//std::cout << "cef3_client OnContextInitialized has been called!" << std::endl;
	};
	
	void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
	{
		std::cout << "cef3_client OnContextReleased has been called!" << std::endl;
	};
	
	void OnWebKitInitialized()
	{
		std::cout << "cef3_client OnWebKitInitialized has been called!" << std::endl;
		
		// testing extensions
		/*
		CefRefPtr<CefV8Handler> handler = this;
		std::string extensionCode = std::string(
		R"<STRING>(
			var game;
			if (!game)
				game = {};
			(function() {
				game.getVersion = function() {
					native function getVersion();
					return getVersion();
				};
			})();
		)<STRING>"
		);
		CefRegisterExtension("v8/game", extensionCode, handler);
		*/
	};
		
	void OnBrowserCreated(CefRefPtr<CefBrowser> browser)
	{
		//std::cout << "cef3_client OnBrowserCreated has been called!" << std::endl;
	};
	
	void sendMessageAllBindingsReceived(CefRefPtr<CefBrowser> browser)
	{
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("AllBindingsReceived");
		browser->SendProcessMessage(PID_BROWSER, message);
	}
	
	/**
	 * 
	 * 
	 * List of available functions:
	 * 
	 * AddFunction
	 * RemoveFunction
	 * AddAttribute
	 * RemoveAttribute
	 * AddObject
	 * RemoveObject
	 * AddAttributeToObject
	 * RemoveAttributeFromObject
	 * AddMethodToObject
	 * RemoveMethodFromObject
	 * FunctionResult
	 * AllBindingsSent
	 * 
	 */
	bool OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
	{
		std::string messageName = message->GetName();
		std::cout << "cef3_client OnProcessMessageReceived " << messageName.c_str() << std::endl;
		
		if( messageName == "AddFunction"/* && browser == m_Browser*/ )
		{
			std::wstring s = message->GetArgumentList()->GetString(0);
			std::wcout << "cef3_client AddFunction: " << s << std::endl;
			functionListMutex_.lock();
			functionList_.push_back( s );
			if (allBindingsSentMessageReceived_ && totalFunctionCount_ == functionList_.size())
			{
				sendMessageAllBindingsReceived( browser );
			}
			functionListMutex_.unlock();
		}
		else if ( messageName == "AllBindingsSent")
		{
			totalFunctionCount_ = message->GetArgumentList()->GetInt(0);
			allBindingsSentMessageReceived_ = true;
			
			functionListMutex_.lock();
			if (totalFunctionCount_ == functionList_.size())
			{
				sendMessageAllBindingsReceived( browser );
			}
			functionListMutex_.unlock();
			std::cout << "cef_client AllBindingsSent message processed" << std::endl;
		}
		/*
		else if( messageName == "FunctionResult" )
		{
			std::string funcName = message->GetArgumentList()->GetString(0);
			int numArguments = message->GetArgumentList()->GetInt(1);
			
			for (int i = 0; i < numArguments; i++)
			{
				CefValueType type = message->GetArgumentList()->GetType( i );
				switch (type) {
					case VTYPE_LIST:
						// TODO: should I implement this?
						std::cout << "Error - FunctionResult - VTYPE_LIST not implemented as CEF3 argument type." << std::endl;
						break;
						
					case VTYPE_BOOL: {
						bool arg = message->GetArgumentList()->GetBool(i);
						std::cout << "VTYPE_BOOL - FunctionResult -  " << arg << std::endl;
						}
						break;
					
					case VTYPE_DOUBLE: {
						double arg = message->GetArgumentList()->GetDouble(i);
						std::cout << "VTYPE_DOUBLE - FunctionResult -  " << arg << std::endl;
						}
						break;
					
					case VTYPE_INT: {
						int arg = message->GetArgumentList()->GetInt(i);
						std::cout << "VTYPE_INT - FunctionResult -  " << arg << std::endl;
						}
						break;
					
					case VTYPE_STRING: {
						std::string arg = message->GetArgumentList()->GetString(i);
						std::cout << "VTYPE_STRING - FunctionResult -  " << arg << std::endl;
						}
						break;
					
					default:
						std::cout << "Error - FunctionResult -  Unknown CEF3 argument type: " << type << std::endl;
						break;
				}
			}
		}
		*/
		
		return true;
	};
	
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
	{
		//std::cout << "cef3_client GetRenderProcessHandler has been called!" << std::endl;
		return this;
	};
	
	/*
	 * When your javascript calls a method that has been defined as a native api call, it will call this method.
	 * 
	 * This method will encode the function call, and send it to the browser process over IPC.
	 * 
	 * The format that is sent via IPC looks like this:
	 * <function_name> <number_of_arguments> [<argument>, ..]
	 * 
	 * The browser process will process the function call, execute the requested function (if it exists), and send the result via IPC to this
	 * process.
	 */
	bool Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
	{		
		std::string s = name.ToString();
		
		// Implementation for “setMessageCallback”
		if (s == "setMessageCallback" && arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsFunction())
		{
			std::cout << "cef3_client Execute setMessageCallback" << std::endl;
			std::string messageName = arguments[0]->GetStringValue();
			CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
			int browser_id = context->GetBrowser()->GetIdentifier();
			callbackMap_.insert( std::make_pair(std::make_pair(messageName, browser_id), std::make_pair(context, arguments[1])) );
		}
		else
		{
			// testing
			if (s == "getVersion")
			{
				retval = CefV8Value::CreateString("0.0.party");
				return true;
			}
			
			std::cout << "cef3_client Execute " << s << std::endl;
			
			// Send function call to browser process
			CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ExecuteFunction");
			
			message->GetArgumentList()->SetString( 0, s );
			message->GetArgumentList()->SetInt( 1, arguments.size() );
			
			// Encode function parameters
			for ( int i = 0; i < arguments.size(); i++ )
			{
				if ( arguments[i]->IsString() )
					message->GetArgumentList()->SetString( i+2, arguments[i]->GetStringValue() );
				else if ( arguments[i]->IsInt() )
					message->GetArgumentList()->SetInt( i+2, arguments[i]->GetIntValue() );
				else if ( arguments[i]->IsUInt() )
					message->GetArgumentList()->SetInt( i+2, arguments[i]->GetUIntValue() );
				else if ( arguments[i]->IsBool() )
					message->GetArgumentList()->SetBool( i+2, arguments[i]->GetBoolValue() );
				else if ( arguments[i]->IsDouble() )
					message->GetArgumentList()->SetDouble( i+2, arguments[i]->GetDoubleValue() );
			}
			
			CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
			context->GetBrowser()->SendProcessMessage(PID_BROWSER, message);
		}
		
		// Function does not exist.
		return false;
	}
	
private:
	// Map of message callbacks.
	typedef std::map<std::pair<std::string, int>, std::pair<CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > CallbackMap;
	CallbackMap callbackMap_;
	
	// Function list
	std::vector < std::wstring > functionList_;
	uint totalFunctionCount_;
	bool allBindingsSentMessageReceived_;
	bool readyForBindingsMessageSent_;
	std::mutex functionListMutex_;

	// NOTE: Must be at bottom
public:
    IMPLEMENT_REFCOUNTING(ClientApp)
};

int main(int argc, char *argv[])
{
#ifdef _WIN32
	CefMainArgs args(nullptr);
#else
	CefMainArgs args(argc, argv);
#endif
    CefRefPtr<ClientApp> app = new ClientApp();
	return CefExecuteProcess(args, app.get());
}




