/*
 * DuplicateAttributeException.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <stdexcept>

#ifndef DUPLICATEATTRIBUTEEXCEPTION_H_
#define DUPLICATEATTRIBUTEEXCEPTION_H_

namespace glr {
namespace cef_client {

class DuplicateAttributeException : public std::runtime_error
{
public:
	DuplicateAttributeException(std::wstring attrName);
	virtual ~DuplicateAttributeException() throw();

	

private:
	
};

}
}

#endif /* DUPLICATEATTRIBUTEEXCEPTION_H_ */
