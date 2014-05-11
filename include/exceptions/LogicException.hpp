#ifndef LOGICEXCEPTION_H_
#define LOGICEXCEPTION_H_

#include <stdexcept>

namespace glr
{
namespace exception
{

class LogicException : public std::logic_error
{
public:
	LogicException(const std::string& message = std::string("Glr LogicException"));
	virtual ~LogicException() throw();
	
	virtual const char* what() throw();

protected:
	std::string message_;
};

}
}

#endif /* LOGICEXCEPTION_H_ */
