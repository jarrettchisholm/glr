#include <utility>

#include "exceptions/FormatException.hpp"

namespace glr
{
namespace exception
{

FormatException::FormatException(std::string message) : Exception(std::move(message))
{
}

FormatException::~FormatException() throw()
{
}

}
}
