/*
 * GlException.h
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#ifndef GLEXCEPTION_H_
#define GLEXCEPTION_H_

#include "Exception.h"

namespace glr {
namespace exception {
class GlException : public Exception {
public:
GlException(const std::string message = "Glr GlException");
virtual ~GlException() throw();
};
} /* namespace exception */
} /* namespace glr */

#endif /* GLEXCEPTION_H_ */
