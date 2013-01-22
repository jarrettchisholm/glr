/*
 * FormatException.h
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#ifndef FORMATEXCEPTION_H_
#define FORMATEXCEPTION_H_

#include "Exception.h"

namespace oglre {

namespace exception {

class FormatException: public Exception {
public:
	FormatException(const std::string message="Oglre FormatException");
	virtual ~FormatException() throw();
};

} /* namespace exception */

} /* namespace oglre */

#endif /* FORMATEXCEPTION_H_ */
