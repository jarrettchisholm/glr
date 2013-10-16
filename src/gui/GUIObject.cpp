/*
 * GUIObject.cpp
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#include <sstream>

#include <boost/log/trivial.hpp>

#include "GUIObject.h"

namespace glr {
namespace gui {
GUIObject::GUIObject(std::wstring name, IAddFunctionListener* addFunctionListener) :
	name_(name), addFunctionListener_(addFunctionListener)
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
	
	// Testing...
	addFunctionListener_->addedFunction(funcName);
	
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

std::wstring GUIObject::getFunctionDefinitions()
{
	std::wstringstream definitions;
	
	// Number of functions
	definitions << functionTypeMap_.size();
	
	for ( auto &it : functionTypeMap_ )
	{
		std::wstring name = it.first;
		int type = it.second;
		
		definitions << ",";
		
		if (type == FunctionTypes::TYPE_VOID || type == FunctionTypes::TYPE_WITH_PARAMETERS_VOID)
		{
			definitions << " " << FunctionTypes::TYPE_VOID << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_INT || type == FunctionTypes::TYPE_WITH_PARAMETERS_INT)
		{
			definitions << " " << FunctionTypes::TYPE_INT << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_FLOAT || type == FunctionTypes::TYPE_WITH_PARAMETERS_FLOAT)
		{
			definitions << " " << FunctionTypes::TYPE_FLOAT << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_STRING || type == FunctionTypes::TYPE_WITH_PARAMETERS_STRING)
		{
			definitions << " " << FunctionTypes::TYPE_STRING << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_CHAR || type == FunctionTypes::TYPE_WITH_PARAMETERS_CHAR)
		{
			definitions << " " << FunctionTypes::TYPE_CHAR << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_BOOL || type == FunctionTypes::TYPE_WITH_PARAMETERS_BOOL)
		{
			definitions << " " << FunctionTypes::TYPE_BOOL << " " << name << " 0";
		}
		else if (type == FunctionTypes::TYPE_INT || type == FunctionTypes::TYPE_WITH_PARAMETERS_INT)
		{
			definitions << " " << FunctionTypes::TYPE_INT << " " << name << " 0";
		}
	}
	
	// TODO: implement
	//std::string json = std::string("getVersion");
	
	return definitions.str();
}

/*
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
*/

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
