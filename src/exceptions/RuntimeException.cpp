#include "exceptions/RuntimeException.hpp"

namespace glr
{
namespace exception
{

RuntimeException::RuntimeException(const std::string& message) : std::runtime_error(message), message_(message)
{
}

RuntimeException::~RuntimeException() throw()
{
}

const char* RuntimeException::what() throw()
{
	return message_.c_str();
}

}
}
