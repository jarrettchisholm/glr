#ifndef FORMATEXCEPTION_H_
#define FORMATEXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace glr
{
namespace exception
{

class FormatException : public Exception
{
public:
	FormatException(std::string message = "Glr FormatException");
	virtual ~FormatException() throw();
};

}
}

#endif /* FORMATEXCEPTION_H_ */
