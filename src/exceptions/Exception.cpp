#include <utility>

#include "exceptions/Exception.hpp"

namespace glr
{
namespace exception
{

Exception::Exception(std::string message) : message_(std::move(message))
{
}


Exception::~Exception() throw()
{
}

const char* Exception::what() throw()
{
	return message_.c_str();
}

}
}
