#ifndef FORMATEXCEPTION_H_
#define FORMATEXCEPTION_H_

#include "Exception.hpp"

namespace glr
{
namespace exception
{

class FormatException : public Exception
{
public:
	FormatException(const std::string message = "Glr FormatException");
	virtual ~FormatException() throw();
};

}
}

#endif /* FORMATEXCEPTION_H_ */
