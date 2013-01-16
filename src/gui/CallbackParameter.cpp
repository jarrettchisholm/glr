/*
 * CallbackParameter.cpp
 *
 *  Created on: 2012-12-30
 *      Author: jarrett
 */

#include "CallbackParameter.h"

namespace oglre {

namespace gui {

CallbackParameter::CallbackParameter() {	
}

CallbackParameter::CallbackParameter(int i) {
	intValue_ = i;
}

CallbackParameter::CallbackParameter(float f) {
	floatValue_ = f;
}

CallbackParameter::CallbackParameter(double d) {
	doubleValue_ = d;
}

CallbackParameter::CallbackParameter(std::wstring s) {
	stringValue_ = s;
}

CallbackParameter::CallbackParameter(char c) {
	charValue_ = c;
}

CallbackParameter::CallbackParameter(bool b) {
	boolValue_ = b;
}

CallbackParameter::~CallbackParameter() {
	
}
	
CallbackParameter::ParamType CallbackParameter::getType() {
	return type_;
}

int CallbackParameter::getInt() {
	return intValue_;
}

float CallbackParameter::getFloat() {
	return floatValue_;
}

double CallbackParameter::getDouble() {
	return doubleValue_;
}

std::wstring CallbackParameter::getString() {
	return stringValue_;
}

char CallbackParameter::getChar() {
	return charValue_;
}

bool CallbackParameter::getBool() {
	return boolValue_;
}

}

}
