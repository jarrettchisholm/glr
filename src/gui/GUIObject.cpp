/*
 * GUIObject.cpp
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#include <boost/log/trivial.hpp>

#include "GUIObject.h"

namespace glr {
namespace gui {
GUIObject::GUIObject(std::wstring name, CefRefPtr<CefV8Value> contextObject) :
	name_(name), contextObject_(contextObject)
{
	/*
	window_->addBindOnStartLoading(
		Berkelium::WideString::point_to(name_),
		Berkelium::Script::Variant::emptyObject());
		*/
}

GUIObject::~GUIObject()
{
}

void GUIObject::addFunction(std::wstring funcName)
{
	std::wstring pointTo = name_ + L"." + funcName;
	
	//contextObject_ = CefV8Context::GetCurrentContext()->GetGlobal();
	
	if (contextObject_ != nullptr)
	{
		// Create an instance of my CefV8Handler object.
		//CefRefPtr<CefV8Handler> handler = new MyV8Handler();
		
		CefRefPtr<CefV8Handler> handler = this;
		
		// Create the 'funcName' function.
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(pointTo, handler);
		
		// Add the 'funcName' function to the "window" object.
		contextObject_->SetValue(pointTo, func, V8_PROPERTY_ATTRIBUTE_NONE);
		
		BOOST_LOG_TRIVIAL(warning) << "Added function '" << pointTo << "' to GUIObject.";
	}
	else
	{
		BOOST_LOG_TRIVIAL(warning) << "Unable to add function '" << pointTo << "' - no contextObject currently exists.";
	}
	
	/*
	window_->addBindOnStartLoading(
		Berkelium::WideString::point_to(pointTo),
		Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(pointTo), true));
		*/
}

void GUIObject::addFunction(std::wstring name, std::function<void()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_VOID;
	functionMapVoid_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<int()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_INT;
	functionMapInt_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<float()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_FLOAT;
	functionMapFloat_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<std::wstring()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_STRING;
	functionMapString_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<char()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_CHAR;
	functionMapChar_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<bool()> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_BOOL;
	functionMapBool_[name] = function;
	addFunction(name);
}


void GUIObject::addFunction(std::wstring name, std::function<void(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_VOID;
	functionMapWithParamatersVoid_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<int(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_INT;
	functionMapWithParamatersInt_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<float(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT;
	functionMapWithParamatersFloat_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<std::wstring(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_STRING;
	functionMapWithParamatersString_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<char(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_CHAR;
	functionMapWithParamatersChar_[name] = function;
	addFunction(name);
}

void GUIObject::addFunction(std::wstring name, std::function<bool(std::vector<CallbackParameter>)> function)
{
	functionTypeMap_[name] = FunctionTypes::TYPE_WITH_PARAMETERS_BOOL;
	functionMapWithParamatersBool_[name] = function;
	addFunction(name);
}

void GUIObject::setContextObject(CefRefPtr<CefV8Value> contextObject)
{
	contextObject_ = contextObject;
	
	// Re-bind all of our functions
	rebindFunctions();
}

bool GUIObject::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
	int type = functionTypeMap_[name];
	
	//if (name == "myfunc")
	{
		// Return my string value.
		retval = CefV8Value::CreateString("My Value!");
		return true;
	}
	
	// Function does not exist.
	return false;	
}

void GUIObject::rebindFunctions()
{
	for ( auto &it : functionTypeMap_ )
	{
		addFunction( it.first );
	}
}

/*
Berkelium::Script::Variant GUIObject::processCallback(std::wstring name, std::vector< CallbackParameter > params)
{
	int type = functionTypeMap_[name];

	Berkelium::Script::Variant variant;

	switch ( type )
	{
	case FunctionTypes::TYPE_VOID: {
		functionMapVoid_[name]();
	}
	break;

	case FunctionTypes::TYPE_INT: {
		int r = functionMapInt_[name]();
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_FLOAT: {
		float r = functionMapFloat_[name]();
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_STRING: {
		std::wstring r = functionMapString_[name]();
		variant = Berkelium::Script::Variant(r.c_str());
	}
	break;

	case FunctionTypes::TYPE_CHAR: {
		char r = functionMapChar_[name]();
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_BOOL: {
		bool r = functionMapBool_[name]();
		variant = Berkelium::Script::Variant(r);
	}
	break;


	// Lambda functions with parameters
	case FunctionTypes::TYPE_WITH_PARAMETERS_VOID:   {
		functionMapWithParamatersVoid_[name](params);
	}
	break;

	case FunctionTypes::TYPE_WITH_PARAMETERS_INT: {
		int r = functionMapWithParamatersInt_[name](params);
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT: {
		float r = functionMapWithParamatersFloat_[name](params);
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_WITH_PARAMETERS_STRING: {
		std::wstring r = functionMapWithParamatersString_[name](params);
		variant = Berkelium::Script::Variant(r.c_str());
	}
	break;

	case FunctionTypes::TYPE_WITH_PARAMETERS_CHAR: {
		char r = functionMapWithParamatersChar_[name](params);
		variant = Berkelium::Script::Variant(r);
	}
	break;

	case FunctionTypes::TYPE_WITH_PARAMETERS_BOOL: {
		bool r = functionMapWithParamatersBool_[name](params);
		variant = Berkelium::Script::Variant(r);
	}
	break;
	}

	return variant;
}
*/

}
}
