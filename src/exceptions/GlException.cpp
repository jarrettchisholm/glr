/*
 * GlException.cpp
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#include "GlException.h"

namespace glr {
namespace exception {

GlException::GlException(const std::string message) : Exception(message)
{
}

GlException::~GlException() throw()
{
}

}
}
