#include <utility>

#include "exceptions/InvalidArgumentException.hpp"

namespace glr
{
namespace exception
{

InvalidArgumentException::InvalidArgumentException(std::string message) : LogicException(std::move(message))
{
}

InvalidArgumentException::~InvalidArgumentException() throw()
{
}

}
}
