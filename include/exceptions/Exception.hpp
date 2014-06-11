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
	Exception(const std::string& message = std::string("Glr Exception"));
	virtual ~Exception() throw();

	virtual const char* what() const throw();

protected:
	std::string message_;
};

}
}

#endif /* EXCEPTION_H_ */
