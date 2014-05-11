#ifndef INVALIDARGUMENTEXCEPTION_H_
#define INVALIDARGUMENTEXCEPTION_H_

#include "exceptions/LogicException.hpp"

namespace glr
{
namespace exception
{

class InvalidArgumentException : public LogicException
{
public:
	InvalidArgumentException(std::string message = std::string("Glr InvalidArgumentException"));
	virtual ~InvalidArgumentException() throw();
};

}
}

#endif /* INVALIDARGUMENTEXCEPTION_H_ */
