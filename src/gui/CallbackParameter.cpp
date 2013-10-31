/*
 * CallbackParameter.cpp
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#include "CallbackParameter.h"

namespace glr {
namespace gui {

CallbackParameter::CallbackParameter()
{
}

CallbackParameter::CallbackParameter(int i)
{
	intValue_ = i;
}

CallbackParameter::CallbackParameter(float f)
{
	floatValue_ = f;
}

CallbackParameter::CallbackParameter(double d)
{
	doubleValue_ = d;
}

CallbackParameter::CallbackParameter(std::string s)
{
	stringValue_ = s;
}

CallbackParameter::CallbackParameter(std::wstring s)
{
	wstringValue_ = s;
}

CallbackParameter::CallbackParameter(char c)
{
	charValue_ = c;
}

CallbackParameter::CallbackParameter(bool b)
{
	boolValue_ = b;
}

CallbackParameter::~CallbackParameter()
{
}

CallbackParameter::ParamType CallbackParameter::getType()
{
	return type_;
}

void CallbackParameter::setType(ParamType type)
{
	type_ = type;
}

}
}
