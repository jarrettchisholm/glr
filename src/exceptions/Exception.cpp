/*
 * Exception.cpp
 *
 *  Created on: 2013-01-16
 *      Author: jarrett
 */

#include "Exception.h"

namespace glr {
namespace exception {

Exception::Exception(const std::string message) : message_(message)
{
}


Exception::~Exception() throw()
{
}

const char* Exception::what() throw()
{
	return message_.c_str();
}

}
}
