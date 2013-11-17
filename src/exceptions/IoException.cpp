/*
 * IoException.cpp
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#include "IoException.h"

namespace glr {
namespace exception {

IoException::IoException(const std::string message) : Exception(message)
{
}

IoException::~IoException() throw()
{
}

}
}
