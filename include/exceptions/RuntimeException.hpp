#ifndef RUNTIMEEXCEPTION_H_
#define RUNTIMEEXCEPTION_H_

#include <stdexcept>

namespace glr
{
namespace exception
{

class RuntimeException : public std::runtime_error
{
public:
	RuntimeException(const std::string& message = std::string("Glr RuntimeException"));
	virtual ~RuntimeException() throw();
	
	virtual const char* what() throw();

protected:
	std::string message_;
};

}
}

#endif /* RUNTIMEEXCEPTION_H_ */
