/*
 * CallbackParameter.h
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#ifndef CALLBACKPARAMETER_H_
#define CALLBACKPARAMETER_H_

#include <string>

namespace oglre {

namespace gui {

class CallbackParameter {
public:
	CallbackParameter();
	CallbackParameter(int i);
	CallbackParameter(float f);
	CallbackParameter(double d);
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
		TYPE_CHAR,
		TYPE_BOOL
	};
	
	ParamType getType();
	
	int getInt();
	float getFloat();
	double getDouble();
	std::wstring getString();
	char getChar();
	bool getBool();
	
private:
	ParamType type_;

	int intValue_;
	float floatValue_;
	double doubleValue_;
	std::wstring stringValue_;
	char charValue_;
	bool boolValue_;

};

}

}

#endif /* CALLBACKPARAMETER_H_ */
