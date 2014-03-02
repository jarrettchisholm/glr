#include <iostream>

#include "ObjectBinding.hpp"

#include "DuplicateFunctionException.hpp"
#include "DuplicateAttributeException.hpp"
#include "FunctionList.hpp"

#include "common/logger/Logger.hpp"

namespace glr
{
namespace cef_client
{

ObjectBinding::ObjectBinding()
{
}
	
ObjectBinding::ObjectBinding(std::wstring name) : name_(name)
{
}

ObjectBinding::~ObjectBinding()
{
}

bool ObjectBinding::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{		
	std::wstring s = name.ToWString();
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
	
	if (exception.length() > 0)
	{
		// Handle exceptions
		std::wstring msg = L"cef3_client exception was detected for '" + s + L"' - Exception: " + exception.ToWString();
		LOG_DEBUG( msg );
		sendMessageException(context->GetBrowser(), "", Exception::EXECUTE_EXCEPTION, msg);
	}
	if ( hasFunction(s) )
	{
		LOG_DEBUG( "cef3_client Execute " << name_ << "." << s );
		
		// Send function call to browser process
		CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("ExecuteFunction");
		
		message->GetArgumentList()->SetString( 0, name_ );
		message->GetArgumentList()->SetString( 1, s );
		message->GetArgumentList()->SetInt( 2, arguments.size() );
		
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
		
		context->GetBrowser()->SendProcessMessage(PID_BROWSER, message);
		
		return true;
	}
	else
	{
		// TODO: error
		LOG_DEBUG( "CEF Object " << name_ << " has no method '" << s << "'" );
	}
	
	// Function does not exist.
	return false;
}

bool ObjectBinding::hasFunction(std::wstring funcName)
{
	for ( auto& f : functions_ )
	{
		if (f.getName() == funcName)
			return true;
	}
	
	return false;
}

bool ObjectBinding::hasAttribute(std::wstring attrName)
{
	for ( auto& a : attributes_ )
	{
		if (a.getName() == attrName)
			return true;
	}
	
	return false;
}

void ObjectBinding::addFunction(FunctionBinding function)
{
	if ( hasFunction(function.getName()) )
		throw DuplicateFunctionException( function.getName() );
	
	functions_.push_back( function );
}

void ObjectBinding::addAttribute(AttributeBinding attribute)
{
	if ( hasAttribute(attribute.getName()) )
		throw DuplicateAttributeException( attribute.getName() );
	
	attributes_.push_back( attribute );
}

const std::vector< FunctionBinding > ObjectBinding::getFunctions()
{
	return functions_;
}

const std::vector< AttributeBinding > ObjectBinding::getAttributes()
{
	return attributes_;
}

void ObjectBinding::sendMessageException(CefRefPtr<CefBrowser> browser, std::string messageId, Exception exception, std::wstring message)
{
	CefRefPtr<CefProcessMessage> m = CefProcessMessage::Create( EXCEPTION );
	m->GetArgumentList()->SetString( 0, messageId );
	m->GetArgumentList()->SetInt( 1, exception );
	m->GetArgumentList()->SetString( 2, message );
	browser->SendProcessMessage(PID_BROWSER, m);
}

}
}
