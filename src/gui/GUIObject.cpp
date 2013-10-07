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
GUIObject::GUIObject(std::wstring name) :
	name_(name)
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
