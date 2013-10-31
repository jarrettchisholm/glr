#include "ClientApp.h"

namespace glr {
namespace cef_client {
	
ClientApp::ClientApp()
{
	std::cout << "cef3_client here woo" << std::endl;
	totalBindingsSent_ = 0;
	totalBindingsReceived_ = 0;
	allBindingsSentMessageReceived_ = false;
	readyForBindingsMessageSent_ = false;
}

ClientApp::~ClientApp()
{
}

void ClientApp::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{		
	std::cout << "cef3_client OnContextCreated has been called!" << std::endl;
	
	if ( allBindingsSentMessageReceived_ )
	{			
		// Register our objects, functions and attributes
		objectBindingMapMutex_.lock();
		std::cout << "Binding " << objectBindingMap_.size() << " object(s)." << std::endl;
		for ( auto& it : objectBindingMap_ )
		{
			// Add object
			const std::wstring& objName = it.first;
			std::wcout << L"Binding object " << objName << std::endl;
			CefRefPtr<CefV8Value> obj = CefRefPtr<CefV8Value>();
			if (objName.length() != 0)
				obj = CefV8Value::CreateObject(nullptr);
			
			CefRefPtr<CefV8Handler> handler = &it.second;
			
			// Add attributes
			const std::vector< AttributeBinding >& attributes = it.second.getAttributes();
			std::cout << "Binding attributes " << attributes.size() << std::endl;
			for ( auto& a : attributes )
			{
				std::wcout << L"Binding attribute " << a.getName() << std::endl;
				CefRefPtr<CefV8Value> attrValue;
				
				if ( a.getValueV8()->IsString() )
					attrValue = CefV8Value::CreateString( a.getValueV8()->GetStringValue() );
				else if ( a.getValueV8()->IsInt() )
					attrValue = CefV8Value::CreateInt( a.getValueV8()->GetIntValue() );
				else if ( a.getValueV8()->IsUInt() )
					attrValue  = CefV8Value::CreateUInt( a.getValueV8()->GetUIntValue() );
				else if ( a.getValueV8()->IsBool() )
					attrValue = CefV8Value::CreateBool( a.getValueV8()->GetBoolValue() );
				else if ( a.getValueV8()->IsDouble() )
					attrValue = CefV8Value::CreateDouble( a.getValueV8()->GetDoubleValue() );
				
				if (obj.get() != nullptr)
				{
					obj->SetValue(a.getName(), attrValue, V8_PROPERTY_ATTRIBUTE_NONE);
					context->GetGlobal()->SetValue(objName, obj, V8_PROPERTY_ATTRIBUTE_NONE);
				}
				else
				{
					context->GetGlobal()->SetValue(a.getName(), attrValue, V8_PROPERTY_ATTRIBUTE_NONE);
				}
			}
			
			// Add functions
			const std::vector< FunctionBinding >& functions = it.second.getFunctions();				
			std::cout << "Binding functions " << functions.size() << std::endl;
			for ( auto& f : functions )
			{
				std::wcout << L"Binding function " << f.getName() << std::endl;
				
				CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(f.getName(), handler);
				
				if (obj.get() != nullptr)
				{
					obj->SetValue(f.getName(), func, V8_PROPERTY_ATTRIBUTE_NONE);
					context->GetGlobal()->SetValue(objName, obj, V8_PROPERTY_ATTRIBUTE_NONE);
				}
				else
				{
					context->GetGlobal()->SetValue(f.getName(), func, V8_PROPERTY_ATTRIBUTE_NONE);
				}
			}
		}
		objectBindingMapMutex_.unlock();
		
		// Register the standard callback function
		CefRefPtr<CefV8Handler> handler = this;
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
		readyForBindingsMessageSent_ = true;
	}
}

void ClientApp::OnContextInitialized()
{
	//std::cout << "cef3_client OnContextInitialized has been called!" << std::endl;
}

void ClientApp::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	std::cout << "cef3_client OnContextReleased has been called!" << std::endl;
}

void ClientApp::OnWebKitInitialized()
{
	std::cout << "cef3_client OnWebKitInitialized has been called!" << std::endl;
}

CefRefPtr<CefRenderProcessHandler> ClientApp::GetRenderProcessHandler()
{
	return this;
}

bool ClientApp::OnProcessMessageReceived( CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
	std::wstring messageName = message->GetName().ToWString();
	std::wcout << L"cef3_client OnProcessMessageReceived " << messageName << std::endl;
	
	
	if( messageName == ADD_OBJECT/* && browser == m_Browser*/ )
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		std::wstring objName = message->GetArgumentList()->GetString(1);
		std::wcout << L"cef3_client AddObject: " << objName << std::endl;
		
		objectBindingMapMutex_.lock();
		ObjectBinding& obj = objectBindingMap_[ objName ];
		obj.setName( objName );
		
		totalBindingsReceived_++;
		
		if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
		{
			sendMessageAllBindingsReceived( browser, totalBindingsReceived_ );
		}
		objectBindingMapMutex_.unlock();
		
		// TODO: Are there any failure criteria here?
		sendMessageSuccess( browser, messageId, L"Successfully added object." );
	}
	else if( messageName == ADD_METHOD_TO_OBJECT/* && browser == m_Browser*/ )
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		std::wstring objName = message->GetArgumentList()->GetString(1);
		std::wstring funcName = message->GetArgumentList()->GetString(2);
		std::wcout << L"cef3_client AddMethodToObject: " << objName << L" | " << funcName << std::endl;
		
		objectBindingMapMutex_.lock();
		ObjectBinding& obj = objectBindingMap_[ objName ];
		
		if ( !obj.hasFunction( funcName ) )
		{
			FunctionBinding func = FunctionBinding(funcName);
			obj.addFunction( func );
		}
		else
		{
			sendMessageException(browser, messageId, Exception::BIND_EXCEPTION, L"Method '" + objName + L"." + funcName + L"' already exists.");
			return false;
		}
		
		totalBindingsReceived_++;
		
		if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
		{
			sendMessageAllBindingsReceived( browser, totalBindingsReceived_ );
		}
		objectBindingMapMutex_.unlock();
		
		sendMessageSuccess( browser, messageId, L"Successfully added method to object." );
	}
	else if( messageName == ADD_ATTRIBUTE_TO_OBJECT/* && browser == m_Browser*/ )
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		std::wstring objName = message->GetArgumentList()->GetString(1);
		std::wstring attrName = message->GetArgumentList()->GetString(2);
		CefRefPtr<CefV8Value> attrValue = getValue(message, 3);
		
		if (attrValue.get() == nullptr)
		{
			sendMessageException(browser, messageId, Exception::BIND_EXCEPTION, L"Value for attribute '" + objName + L"." + attrName + L"' was unparsable.");
			return false;
		}
		
		std::wcout << L"cef3_client AddAttributeToObject: " << objName << L" | " << attrName << L" | ";
		if ( attrValue->IsString() )
			std::cout << "(string) " << attrValue->GetStringValue().ToString() << std::endl;
		else if ( attrValue->IsInt() )
			std::cout << "(int) " <<  attrValue->GetIntValue() << std::endl;
		else if ( attrValue->IsUInt() )
			std::cout << "(uint) " <<  attrValue->GetUIntValue() << std::endl;
		else if ( attrValue->IsBool() )
			std::cout << "(bool) " <<  attrValue->GetBoolValue() << std::endl;
		else if ( attrValue->IsDouble() )
			std::cout << "(double) " <<  attrValue->GetDoubleValue() << std::endl;
		else
		{
			sendMessageException(browser, messageId, Exception::BIND_EXCEPTION, L"Attribute '" + objName + L"." + attrName + L"' has an unknown value type.");
			return false;
		}
		
		objectBindingMapMutex_.lock();
		ObjectBinding& obj = objectBindingMap_[ objName ];
		
		if ( !obj.hasAttribute( attrName ) )
		{
			AttributeBinding attr = AttributeBinding(attrName, attrValue);
			obj.addAttribute( attr );
		}
		else
		{
			sendMessageException(browser, messageId, Exception::BIND_EXCEPTION, L"Attribute '" + objName + L"." + attrName + L"' already exists.");
			return false;
		}
		
		totalBindingsReceived_++;
		
		if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
		{
			sendMessageAllBindingsReceived( browser, totalBindingsReceived_ );
		}
		objectBindingMapMutex_.unlock();
		
		sendMessageSuccess( browser, messageId, L"Successfully added attribute to object." );
	}
	else if( messageName == ADD_FUNCTION/* && browser == m_Browser*/ )
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		std::wstring funcName = message->GetArgumentList()->GetString(1);
		std::wcout << L"cef3_client AddFunction: " << funcName << std::endl;
		
		objectBindingMapMutex_.lock();
		ObjectBinding& obj = objectBindingMap_[ L"" ];
		
		if ( !obj.hasFunction( funcName ) )
		{
			FunctionBinding func = FunctionBinding(funcName);
			obj.addFunction( func );
		}
		else
		{
			sendMessageException(browser, messageId, Exception::BIND_EXCEPTION, L"Function '" + funcName + L"' already defined.");
			return false;
		}
		
		totalBindingsReceived_++;
		
		if (allBindingsSentMessageReceived_ && totalBindingsSent_ == totalBindingsReceived_)
		{
			sendMessageAllBindingsReceived( browser, totalBindingsReceived_ );
		}
		objectBindingMapMutex_.unlock();
		
		sendMessageSuccess( browser, messageId, L"Successfully added function." );
	}
	else if( messageName == ADD_ATTRIBUTE/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client AddAttribute message not yet implemented." << std::endl;
	}
	else if( messageName == REMOVE_FUNCTION/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client RemoveFunction message not yet implemented." << std::endl;
	}
	else if( messageName == REMOVE_ATTRIBUTE/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client RemoveAttribute message not yet implemented." << std::endl;
	}
	else if( messageName == REMOVE_OBJECT/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client RemoveObject message not yet implemented." << std::endl;
	}
	else if( messageName == REMOVE_ATTRIBUTE_FROM_OBJECT/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client RemoveAttributeFromObject message not yet implemented." << std::endl;
	}
	else if( messageName == REMOVE_METHOD_FROM_OBJECT/* && browser == m_Browser*/ )
	{
		std::cout << "cef_client RemoveMethodFromObject message not yet implemented." << std::endl;
	}
	else if ( messageName == ALL_BINDINGS_SENT)
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		totalBindingsSent_ = message->GetArgumentList()->GetInt(1);
		allBindingsSentMessageReceived_ = true;
		
		objectBindingMapMutex_.lock();
		if (totalBindingsSent_ == totalBindingsReceived_)
		{
			sendMessageAllBindingsReceived( browser, totalBindingsReceived_ );
		}
		objectBindingMapMutex_.unlock();
		std::cout << "cef_client AllBindingsSent message processed" << std::endl;
	}
	else if( messageName == FUNCTION_RESULT )
	{
		std::string messageId = message->GetArgumentList()->GetString(0);
		std::wstring funcName = message->GetArgumentList()->GetString(1);
		int numArguments = message->GetArgumentList()->GetInt(2);
		
		CefRefPtr<CefListValue> list = CefListValue::Create();
		list->SetSize( numArguments );
		
		CefRefPtr<CefV8Value> temp;
		
		for (int i = 0; i < numArguments; i++)
		{
			temp = getValue(message, i+3);
			
			if (temp.get() == nullptr)
			{
				// TODO: is EXECUTE_EXCEPTION the right kind of exception to throw here?
				sendMessageException(browser, messageId, Exception::EXECUTE_EXCEPTION, L"Result value of function '" + funcName + L"' was unparsable.");
				return false;
			}
			
			if (temp != nullptr)
			{
				copyToList(temp, i, list);
				temp = nullptr;
			}
			
		}
		
		// Execute the registered JavaScript callback if any.
		if (!callbackMap_.empty())
		{
			CallbackMap::const_iterator it = callbackMap_.find( std::make_pair(funcName, browser->GetIdentifier()));
			
			if (it != callbackMap_.end())
			{
				// Keep a local reference to the objects. The callback may remove itself from the callback map.
				CefRefPtr<CefV8Context> context = it->second.first;
				CefRefPtr<CefV8Value> callback = it->second.second;
				
				// Enter the context.
				context->Enter();
				
				CefV8ValueList arguments;
				
				// First argument is the message name.
				arguments.push_back(CefV8Value::CreateString(funcName));
				
				// Second argument is the list of message arguments.
				//CefRefPtr<CefListValue> list = message->GetArgumentList();
				//std::cout << "list->GetSize() " << list->GetSize() << std::endl;
				CefRefPtr<CefV8Value> args = CefV8Value::CreateArray(list->GetSize());
				SetList(list, args);  // Helper function to convert CefListValue to CefV8Value.
				
				arguments.push_back( args );
				
				// Execute the callback.
				CefRefPtr<CefV8Value> retval = callback->ExecuteFunction(NULL, arguments);
				if (retval.get())
				{
					if (retval->IsBool())
					{
						bool handled = retval->GetBoolValue();
						if (!handled)
							sendMessageException( browser, messageId, Exception::EXECUTE_EXCEPTION, L"Function '" + funcName + L"' message callback did not return successfully." );
						else
							sendMessageSuccess( browser, messageId, L"Function '" + funcName + L"' message callback returned successfully." );
					}
				}
			
				// Exit the context.
				context->Exit();
			}
		}
	}
	
	return true;
}

bool ClientApp::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{		
	std::wstring s = name.ToWString();
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	
	if (exception.length() > 0)
	{
		// Handle exceptions
		std::wstring msg = L"cef3_client exception was detected for '" + s + L"' - Exception: " + exception.ToWString();
		std::wcout << msg << std::endl;
		sendMessageException(context->GetBrowser(), "", Exception::EXECUTE_EXCEPTION, msg);
	}	
	else if (s == L"setMessageCallback" && arguments.size() == 2 && arguments[0]->IsString() && arguments[1]->IsFunction())
	{
		std::wcout << L"cef3_client Execute --- setMessageCallback" << std::endl;
		std::wstring messageName = arguments[0]->GetStringValue();
		
		int browserId = context->GetBrowser()->GetIdentifier();
		callbackMap_.insert( std::make_pair(std::make_pair(messageName, browserId), std::make_pair(context, arguments[1])) );
		return true;
	}
	else
	{
		// TODO: error
		std::wstring msg = L"Cannot execute function '" + s + L"'.";
		std::wcout << L"cef_client - " << msg << std::endl;
		sendMessageException(context->GetBrowser(), "", Exception::EXECUTE_EXCEPTION, msg);
	}
	
	// Function does not exist.
	return false;
}

void ClientApp::sendMessageAllBindingsReceived(CefRefPtr<CefBrowser> browser, uint numBindings)
{
	CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( ALL_BINDINGS_RECEIVED );
	m->GetArgumentList()->SetInt( 0, numBindings );
	browser->SendProcessMessage(PID_BROWSER, m);
}

void ClientApp::sendMessageException(CefRefPtr<CefBrowser> browser, std::string messageId, Exception exception, std::wstring message)
{
	CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( EXCEPTION );
	m->GetArgumentList()->SetString( 0, messageId );
	m->GetArgumentList()->SetInt( 1, exception );
	m->GetArgumentList()->SetString( 2, message );
	browser->SendProcessMessage(PID_BROWSER, m);
}

void ClientApp::sendMessageSuccess(CefRefPtr<CefBrowser> browser, std::string messageId, std::wstring message)
{
	CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( SUCCESS );
	m->GetArgumentList()->SetString( 0, messageId );
	m->GetArgumentList()->SetString( 1, message );
	browser->SendProcessMessage(PID_BROWSER, m);
}

void ClientApp::copyToList(CefRefPtr<CefV8Value> value, int i, CefRefPtr<CefListValue> list)
{
	SetListValue(list, i, value);
}

// Transfer a V8 array to a List.
void ClientApp::SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {
	//assert(source->IsArray());

	int argLength = source->GetArrayLength();
	if (argLength == 0)
		return;
	
	// Start with null types in all spaces.
	target->SetSize(argLength);
	
	for (int i = 0; i < argLength; ++i)
		SetListValue(target, i, source->GetValue(i));
}

// Transfer a V8 value to a List index.
void ClientApp::SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value) {
	if (value->IsArray()) {
		CefRefPtr<CefListValue> new_list = CefListValue::Create();
		SetList(value, new_list);
		list->SetList(index, new_list);
	}
	else if (value->IsString())
	{
		list->SetString(index, value->GetStringValue());
	}
	else if (value->IsBool())
	{
		list->SetBool(index, value->GetBoolValue());
	}
	else if (value->IsInt())
	{
		list->SetInt(index, value->GetIntValue());
	}
	else if (value->IsDouble())
	{
		list->SetDouble(index, value->GetDoubleValue());
	}
}

// Transfer a List value to a V8 array index.
void ClientApp::SetListValue(CefRefPtr<CefV8Value> list, int index, CefRefPtr<CefListValue> value) {
	CefRefPtr<CefV8Value> newValue;
	
	CefValueType type = value->GetType(index);
	switch (type)
	{
		case VTYPE_LIST: 
		{
			CefRefPtr<CefListValue> list = value->GetList(index);
			newValue = CefV8Value::CreateArray(static_cast<int>(list->GetSize()));
			SetList(list, newValue);
		} 
			break;
		case VTYPE_BOOL:
			newValue = CefV8Value::CreateBool(value->GetBool(index));
			break;
		case VTYPE_DOUBLE:
			newValue = CefV8Value::CreateDouble(value->GetDouble(index));
			break;
		case VTYPE_INT:
			newValue = CefV8Value::CreateInt(value->GetInt(index));
			break;
		case VTYPE_STRING:
			newValue = CefV8Value::CreateString(value->GetString(index));
			break;
		default:
			break;
	}

	if (newValue.get())
	{
		list->SetValue(index, newValue);
	}
	else
	{
		list->SetValue(index, CefV8Value::CreateNull());
	}
}

// Transfer a List to a V8 array.
void ClientApp::SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target) {
	//ASSERT(target->IsArray());
	
	int argLength = static_cast<int>(source->GetSize());
	if (argLength == 0)
		return;
	
	for (int i = 0; i < argLength; ++i)
		SetListValue(target, i, source);
}

CefRefPtr<CefV8Value> ClientApp::getValue(CefRefPtr<CefProcessMessage> message, uint index)
{
	CefValueType type = message->GetArgumentList()->GetType( index );
	switch (type) {
		case VTYPE_STRING:
		{
			std::string arg = message->GetArgumentList()->GetString(index);
			std::cout << "VTYPE_STRING - getValue -  " << arg << std::endl;
			return CefV8Value::CreateString( arg );
		}
			break;
		
		case VTYPE_INT:
		{
			int arg = message->GetArgumentList()->GetInt(index);
			std::cout << "VTYPE_INT - getValue -  " << arg << std::endl;
			return CefV8Value::CreateInt( arg );
		}
			break;
		
		case VTYPE_DOUBLE:
		{
			double arg = message->GetArgumentList()->GetDouble(index);
			std::cout << "VTYPE_DOUBLE - getValue -  " << arg << std::endl;
			return CefV8Value::CreateDouble( arg );
		}
			break;
		
		case VTYPE_BOOL:
		{
			bool arg = message->GetArgumentList()->GetBool(index);
			std::cout << "VTYPE_BOOL - getValue -  " << arg << std::endl;
			return CefV8Value::CreateBool( arg );
		}
			break;
	
		case VTYPE_BINARY:
			// TODO: error
			std::cout << "Error - getValue - VTYPE_BINARY not implemented as CEF3 function result type." << std::endl;
			break;
			
		case VTYPE_DICTIONARY:
			// TODO: error
			std::cout << "Error - getValue - VTYPE_DICTIONARY not implemented as CEF3 function result type." << std::endl;
			break;
		
		case VTYPE_LIST:
			// TODO: error
			std::cout << "Error - getValue - VTYPE_LIST not implemented as CEF3 function result type." << std::endl;
			break;
		
		case VTYPE_INVALID:
			// TODO: error
			std::cout << "Error - getValue - VTYPE_INVALID CEF3 function result type." << std::endl;
			break;
			
		case VTYPE_NULL:
			// TODO: error
			std::cout << "Error - getValue - VTYPE_NULL not implemented as CEF3 function result type." << std::endl;
			break;
		
		default:
			std::cout << "Error - getValue - Unknown CEF3 function result type: " << type << std::endl;
			break;
	}
	
	return nullptr;
}

}
}
