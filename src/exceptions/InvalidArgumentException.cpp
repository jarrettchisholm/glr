#include "exceptions/InvalidArgumentException.hpp"

namespace glr
{
namespace exception
{

InvalidArgumentException::InvalidArgumentException(const std::string& message) : LogicException(message)
{
}

InvalidArgumentException::~InvalidArgumentException() throw()
{
}

}
}
