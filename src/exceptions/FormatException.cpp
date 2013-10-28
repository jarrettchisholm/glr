/*
 * FormatException.cpp
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#include "FormatException.h"

namespace glr {
namespace exception {

FormatException::FormatException(const std::string message) : Exception(message)
{
}

FormatException::~FormatException() throw()
{
}

} /* namespace exception */
} /* namespace glr */
