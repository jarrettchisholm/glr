/*
 * CallbackParameter.h
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#ifndef CALLBACKPARAMETER_H_
#define CALLBACKPARAMETER_H_

#include <string>

#include "../common/utilities/Macros.h"

namespace glr {
namespace gui {
	
/**
 * Class to wrap around parameters that are required for a callback method.
 */
class CallbackParameter {
public:
	CallbackParameter();
	CallbackParameter(int i);
	CallbackParameter(float f);
	CallbackParameter(double d);
	CallbackParameter(std::string s);
	CallbackParameter(std::wstring s);
	CallbackParameter(char c);
	CallbackParameter(bool b);
	virtual ~CallbackParameter();

	enum ParamType {
		TYPE_VOID = 0,
		TYPE_INT,
		TYPE_FLOAT,
		TYPE_DOUBLE,
		TYPE_STRING,
		TYPE_WSTRING,
		TYPE_CHAR,
		TYPE_BOOL
	};

	ParamType getType();
	ParamType setType(ParamType type);

	GETSET(int, intValue_, Int)
	GETSET(float, floatValue_, Float)
	GETSET(double, doubleValue_, Double)
	GETSET(std::string, stringValue_, String)
	GETSET(std::wstring, wstringValue_, Wstring)
	GETSET(char, charValue_, Char)
	GETSET(bool, boolValue_, Bool)

private:
	ParamType type_;

	int intValue_;
	float floatValue_;
	double doubleValue_;
	std::string stringValue_;
	std::wstring wstringValue_;
	char charValue_;
	bool boolValue_;
};

}
}
#endif /* CALLBACKPARAMETER_H_ */
