/*
 * FormatException.h
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#ifndef FORMATEXCEPTION_H_
#define FORMATEXCEPTION_H_

#include "Exception.h"

namespace glr {
namespace exception {
class FormatException : public Exception {
public:
	FormatException(const std::string message = "Glr FormatException");
	virtual ~FormatException() throw();
};
} /* namespace exception */
} /* namespace glr */
#endif /* FORMATEXCEPTION_H_ */
