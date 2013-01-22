/*
 * IoException.cpp
 *
 *  Created on: 2013-01-17
 *      Author: jarrett
 */

#include "IoException.h"

namespace oglre {

namespace exception {

IoException::IoException(const std::string message) : Exception(message) {

}

IoException::~IoException() throw() {
}

} /* namespace exception */

} /* namespace oglre */
