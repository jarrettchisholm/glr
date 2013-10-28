/*
 * DuplicateFunctionException.h
 *
 *  Created on: 2011-05-08
 *      Author: jarrett
 */

#include <stdexcept>

#ifndef DUPLICATEFUNCTIONEXCEPTION_H_
#define DUPLICATEFUNCTIONEXCEPTION_H_

namespace glr {
namespace cef_client {

class DuplicateFunctionException : public std::runtime_error
{
public:
	DuplicateFunctionException(std::wstring funcName);
	virtual ~DuplicateFunctionException()  throw();

	

private:
	
};

}
}

#endif /* DUPLICATEFUNCTIONEXCEPTION_H_ */
