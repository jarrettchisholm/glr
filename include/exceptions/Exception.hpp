#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <exception>

namespace glr
{
namespace exception
{

class Exception : public std::exception
{
public:
	Exception(std::string message = "Glr Exception");
	virtual ~Exception() throw();

	virtual const char* what() throw();

protected:
	std::string message_;
};

}
}

#endif /* EXCEPTION_H_ */
