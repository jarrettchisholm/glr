#ifndef DUPLICATEFUNCTIONEXCEPTION_H_
#define DUPLICATEFUNCTIONEXCEPTION_H_

#include <stdexcept>

namespace glr
{
namespace cef_client
{

class DuplicateFunctionException : public std::runtime_error
{
public:
	DuplicateFunctionException(std::wstring funcName);
	virtual ~DuplicateFunctionException()  throw();
};

}
}

#endif /* DUPLICATEFUNCTIONEXCEPTION_H_ */
