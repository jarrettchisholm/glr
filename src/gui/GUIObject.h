/*
 * GUIObject.h
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#ifndef GUIOBJECT_H_
#define GUIOBJECT_H_

#include <map>

#include <berkelium/Window.hpp>
#include <berkelium/WindowDelegate.hpp>
#include <berkelium/Rect.hpp>
#include <berkelium/ScriptUtil.hpp>
#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>

#include "IGUIObject.h"

namespace glr {
namespace gui {
class GUIObject : public IGUIObject {
public:
GUIObject(std::wstring name, Berkelium::Window* window);
virtual ~GUIObject();

virtual void addFunction(std::wstring name, std::function<void()> function);
virtual void addFunction(std::wstring name, std::function<int()> function);
virtual void addFunction(std::wstring name, std::function<float()> function);
virtual void addFunction(std::wstring name, std::function<std::wstring()> function);
virtual void addFunction(std::wstring name, std::function<char()> function);
virtual void addFunction(std::wstring name, std::function<bool()> function);


virtual void addFunction(std::wstring name, std::function<void(std::vector<CallbackParameter>)> function);
virtual void addFunction(std::wstring name, std::function<int(std::vector<CallbackParameter>)> function);
virtual void addFunction(std::wstring name, std::function<float(std::vector<CallbackParameter>)> function);
virtual void addFunction(std::wstring name, std::function<std::wstring(std::vector<CallbackParameter>)> function);
virtual void addFunction(std::wstring name, std::function<char(std::vector<CallbackParameter>)> function);
virtual void addFunction(std::wstring name, std::function<bool(std::vector<CallbackParameter>)> function);

Berkelium::Script::Variant processCallback(std::wstring name, std::vector< CallbackParameter > params);

private:
enum FunctionTypes {
	TYPE_VOID = 0,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_CHAR,
	TYPE_BOOL,

	TYPE_WITH_PARAMETERS_VOID,
	TYPE_WITH_PARAMETERS_INT,
	TYPE_WITH_PARAMETERS_FLOAT,
	TYPE_WITH_PARAMETERS_STRING,
	TYPE_WITH_PARAMETERS_CHAR,
	TYPE_WITH_PARAMETERS_BOOL
};

Berkelium::Window* window_;
std::wstring name_;

std::map< std::wstring, int > functionTypeMap_;

std::map< std::wstring, std::function<void()> >                 functionMapVoid_;
std::map< std::wstring, std::function<int()> >                  functionMapInt_;
std::map< std::wstring, std::function<float()> >                functionMapFloat_;
std::map< std::wstring, std::function<std::wstring()> > functionMapString_;
std::map< std::wstring, std::function<char()> >                 functionMapChar_;
std::map< std::wstring, std::function<bool()> >                 functionMapBool_;

std::map< std::wstring, std::function<void( std::vector<CallbackParameter> )> >                 functionMapWithParamatersVoid_;
std::map< std::wstring, std::function<int( std::vector<CallbackParameter> )> >                  functionMapWithParamatersInt_;
std::map< std::wstring, std::function<float( std::vector<CallbackParameter> )> >                functionMapWithParamatersFloat_;
std::map< std::wstring, std::function<std::wstring(std::vector<CallbackParameter> )> > functionMapWithParamatersString_;
std::map< std::wstring, std::function<char( std::vector<CallbackParameter> )> >                 functionMapWithParamatersChar_;
std::map< std::wstring, std::function<bool( std::vector<CallbackParameter> )> >                 functionMapWithParamatersBool_;


void addFunction(std::wstring funcName);
};
}
}

#endif /* GUIOBJECT_H_ */
