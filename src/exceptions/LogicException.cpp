#include "exceptions/LogicException.hpp"

namespace glr
{
namespace exception
{

LogicException::LogicException(const std::string& message) : std::logic_error(message), message_(message)
{
}

LogicException::~LogicException() throw()
{
}

const char* LogicException::what() throw()
{
	return message_.c_str();
}

}
}
