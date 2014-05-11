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
	IoException(const std::string& message = std::string("Glr IoException"));
	virtual ~IoException() throw();
};

}
}

#endif /* IOEXCEPTION_H_ */
