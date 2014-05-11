#ifndef IOEXCEPTION_H_
#define IOEXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace glr
{
namespace exception
{

class IoException : public Exception
{
public:
	IoException(std::string message = "Glr IoException");
	virtual ~IoException() throw();
};

}
}

#endif /* IOEXCEPTION_H_ */
