#ifndef RUNTIMEEXCEPTION_H_
#define RUNTIMEEXCEPTION_H_

#include "Exception.hpp"

namespace glr
{
namespace exception
{

class RuntimeException : public Exception
{
public:
	RuntimeException(const std::string message = "Glr RuntimeException");
	virtual ~RuntimeException() throw();
};

}
}

#endif /* RUNTIMEEXCEPTION_H_ */
