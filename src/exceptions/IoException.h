/*
 * IoException.h
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#ifndef IOEXCEPTION_H_
#define IOEXCEPTION_H_

#include "Exception.h"

namespace oglre {

namespace exception {

class IoException: public Exception {
public:
	IoException(const std::string message="Oglre IoException");
	virtual ~IoException() throw();
};

} /* namespace exception */

} /* namespace oglre */

#endif /* IOEXCEPTION_H_ */
